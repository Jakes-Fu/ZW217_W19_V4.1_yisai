/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
* 
* �ļ����ƣ� // MexLib_Os.h
* �ļ���ʶ��
* ����ժҪ�� //mexƽ̨os ��
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

//�ļ�����fseek����ʼλ��
#ifndef SEEK_SET
	#define SEEK_SET			0		//�ļ���ͷ
	#define SEEK_CUR			1		//��ǰ��ȡλ��
	#define SEEK_END			2		//�ļ���β
#endif

//������������������
typedef enum 
{
	E_MITT_NUMERIC,						//��������
    E_MITT_ALPHANUMERIC_UPPERCASE,		//�����д��ĸ������
    E_MITT_ALPHANUMERIC_LOWERCASE,		//����Сд��ĸ������
    E_MITT_SM_PINYIN,					//���뺺��ƴ��
    E_MITT_SM_STROKE,					//���뺺��ʻ�
    E_MITT_SM_MULTITAP_PINYIN			//���뺺��ȫƴ
}E_Mex_Input_Type;

//���뷨�������֧��
#define MEX_TITLE_BUFFER_LENGTH		64

//�����������Ļ���
typedef struct _Mex_Input_Context
{
	int32			m_iInputType;								//��ǰ�����������
	wchar_t			m_pInputTitle[MEX_TITLE_BUFFER_LENGTH];		//��ǰ����������
	uint32			m_iInputMaxNum;								//��ǰ�������ַ�������
	uint8 			*m_pInputBuffer;							//��ǰ�����뻺��
	bool 			m_bIsPassword;								//��ǰ�Ƿ�Ϊ����ģʽ
	bool			m_bCanChange;								//��ǰ�Ƿ������л����뷨
	int32			m_iAppId;									//����������App ID
	Mex_Input_Func	m_pInputFunc;								//�����Ļص�����ָ��
	bool			m_bIsRunning;								//�Ƿ��Ѿ��˳�
	int32			m_iInputBufferPool;							//��ǰ����������Ŀ��ڴ��
} Mex_Input_Context;



//ʱ�����ڣ�����GetDateTime
typedef struct
{
	uint16 year;		//��
	uint8 month;		//��
	uint8 day;			//��
    uint8  dayIndex;	// ���� 0=Sunday
	uint8 hour;			//Сʱ
	uint8 minute;		//��
	uint8 second;		//��
} Mex_DateTime;

//����
typedef struct
{
	int32 m_iPosX;		//x
	int32 m_iPosY;		//y
}Mex_Point;

//ԭGUI�ⲿ�ֺ��� begin ////////////////////////////////////
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
	uint16 type;				                 // ����
	uint16 attr;                                 // ����
	int32 width, height;                         // ��ȡ��߶�
	int16 center_x, center_y;                    // ����
	uint16 draw_x1, draw_y1, draw_x2, draw_y2;   // ��������
	uint16 draw_width, draw_height;              // ����������
	mex_color *buffer;                           // ͼ��/��ɫ��
	byte *Abuffer;                               // alpha����
	byte *img_Ibuffer;                           // ͼ��->��ɫ����������
	int16 Ibuffer_width, Ibuffer_height;         // ͼ��->��ɫ�����������ȼ��߶�
	byte **palChk_Ibuffer;                       // ��ɫ��->��ɫ����������
	int16 palChk_width, palChk_height;           // ��ɫ��->��ɫ�����������ȼ��߶�
	byte palChk_count;                           // ��ɫ����Ŀ
	byte palClr_count;                           // ��ɫ����ɫ��
	byte alpha;                                  // alpha�ȼ���0~16��
	uint16 **accel;                              // ������
	mex_color colorkey;                          // ͸��ɫ
} Mex_Image;

// ϵͳ״̬����ʾ����
typedef enum
{
	E_MQST_INVALID = -1,	//�Ƿ�ֵ

	E_MQST_ONLINE,			//����
	E_MQST_OFFLINE,			//����
	E_MQST_MSG,				//����Ϣ
	E_MQST_HIDE,			//����
	E_MQST_AWAY,			//�뿪

	E_MQST_MAX
}E_MEX_QQ_STATUS_TYPE;



//ϵͳ����������ʾ����
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
//ԭGUI�ⲿ�ֺ��� end ////////////////////////////////////

#define MEX_APP_ATTRIB_FULL_NO_NEED_LAST_SCREEN		0x00000001		//��ʾȫ������ÿ�λ�ͼʱ����Ҫ�ϴε���Ļ����
#define MEX_APP_ATTRIB_FULL_MANUAL_UPDATE_SCREEN	0x00000002		//��ʾȫ����������Լ�����Invalidate()ʱ�Ż��յ�Paint��Ϣ
#define MEX_APP_ATTRIB_RESERVED2					0x00000004		//�������ԣ��Ժ���չ��
#define MEX_APP_ATTRIB_RESERVED3					0x00000008		//�������ԣ��Ժ���չ��
#define MEX_APP_ATTRIB_RESERVED4					0x00000010		//�������ԣ��Ժ���չ��
#define MEX_APP_ATTRIB_RESERVED5					0x00000020		//�������ԣ��Ժ���չ��
#define MEX_APP_ATTRIB_RESERVED6					0x00000040		//�������ԣ��Ժ���չ��
#define MEX_APP_ATTRIB_RESERVED7					0x00000080		//�������ԣ��Ժ���չ��

//�ļ�����
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
MEX_H_DECLARE_FUNC( bool,			NotifyLockSRam,				(void) )		//����ϵͳ��������ʱʹ�����е�sram�ռ�
MEX_H_DECLARE_FUNC( bool,			NotifyUnlockSRam,			(void) )		//����ϵͳ������sram�ռ��ú���

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

// ԭGUI�ⲿ�ֺ��� begin ////////////////////////////////////
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
// ԭGUI�ⲿ�ֺ��� end ////////////////////////////////////

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
// ԭGUI�ⲿ�ֺ��� end


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

	// ԭGUI�ⲿ�ֺ��� begin ////////////////////////////////////
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
	// ԭGUI�ⲿ�ֺ��� end ////////////////////////////////////

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

	// ԭGUI�ⲿ�ֺ��� begin ////////////////////////////////////
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
	// ԭGUI�ⲿ�ֺ��� end ////////////////////////////////////
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

//ȡ�ú����⺯���б�ָ��
//����1��lib��ID
//����2����lib�еĺ�����Ŀ�����룩
//���أ�����ָ���б��ָ��
funcptr** ExchangeLibPtrPtr(int32 iLibId, int32 iNbFuncsInLib);	//�ڲ�����

//���ص�ǰӦ�ó�������ʱ�Ĳ������ò�����LaunchAppEx����ָ��
const char* GetLaunchParam(void);

//ͬLaunchApp�������������szParam������ͨ��GetLaunchParam��ȡ

int32 LaunchAppEx(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir, const char *szParam);
int32 LaunchAppExByAdm(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir, const char *szParam, int32 iAdmId );

//��������ָ��Ӧ�ó���
//����1��Ӧ�������ļ���ȥ����׺��
//����2���ļ�Ŀ¼�����Ŀ¼��app������mexapp�µ�Ŀ¼����L"HelloWorld"��system��L""
//����3���ļ���,��L"HelloWorld.mex"
//����4���Ƿ���ص�sys�ڲ�
//����5���Ƿ���sys��Ŀ¼
//���أ�Ӧ��ID��ΪNULL�����ʧ��
int32 LaunchApp(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir);

bool RegisterLaunchAppAfterExit(bool bEnable, const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir);

//����ָ��Ӧ��
//����1��App��ID
//���أ���
void FreeApp(int32 iAppId);		
void FreeAppByAdm(int32 iAppId ,int32 iAdmId);

//Ӧ��ע����Ϣ�ص�����
//����1��App��ID
//����2����Ϣ�ص�����
//���أ���
void RegisterMsgHandler(int32 iAppId, MexApp_MsgHandler_Func pFunc);		

//Ӧ�û�libע���˳�����
//����1��App��ID
//����2���˳�����
//����3���Ƿ�ΪLib
//���أ���
void RegisterExitFunc(int32 iAppId, MexApp_Exit_Func pFunc, bool bAmILib);		

//����1��App��ID
//���أ�Ӧ����Ϣ�ص�����
MexApp_MsgHandler_Func GetMsgHandler(int32 iAppId);		

//Ӧ���˳�
void Exit(void);		

//�ѳ������μ�����sram
//����1��Ӧ��id
bool LoadAppIntoSRam(int32 iAppId);

//�ѳ������δ�sram�ͷţ���Manager���ã�
//����1��Ӧ��id
void FreeAppFromSRam(int32 iAppId);

//��lib�Ĵ���μ�����sram
//����1��lib�⺯��ָ��
bool LoadLibIntoSRam(funcptr pLibFuncPtr);

//��lib�Ĵ���δ�sram�ͷţ�Manager���ã�
//����1��lib�⺯��ָ��
void FreeLibFromSRam(funcptr pLibFuncPtr);

//ȡ�õ�ǰ���App������Lib�ǵ���lib������Ӧ�ã�����App���Լ�
//���أ���ǰ���AppId
int32 GetCurrentApp(void);		


//���õ�ǰ���App
//����1�����AppId
//���أ���
void SetCurrentApp(int32 iAppId);	


//ȡ��ƽ̨ϵͳ��Ŀ¼
//��������
//���أ�ƽ̨ϵͳ��Ŀ¼
const wchar_t* GetSystemRootDir(void);		

//ȡ��Ӧ��Ӧ�����ڵ�Ŀ¼
//������Ӧ����
//���أ���ʵĿ¼
//���磬szAppFileName == L"HelloWorld.mex"�����ؿ����� L"D:\\MexApp\\HelloWorld"
const wchar_t* GetAppDir(const wchar_t *szAppFileName);

//ȡ��ƽ̨Ӧ�ø�Ŀ¼
//��������
//���أ�ƽ̨Ӧ�ø�Ŀ¼
const wchar_t* GetAppRootDir(void);		


//ȡ�õ�ǰ���App��Ŀ¼
//����1����Ŀ¼Ҫ������buf���ⲿ����
//���أ���ǰ���App��Ŀ¼
const wchar_t* GetCurAppDir(wchar_t* curAppDir);		

//ȡ�õ�ǰ���App�ļ���
//����1�����ļ���Ҫ������buf���ⲿ����
//���أ���ǰ���App�ļ���
const wchar_t* GetCurAppFileName(wchar_t* curAppFileName);		


//���ƿ�ʼ������buf
//���أ��Ƿ�ɹ��������ⰵʱfalse��
bool BeginPaint(void);		

//���ƽ������������
//���أ���
void EndPaint(void);		

bool Invalidate(void);

//ȡ�õ�ǰʣ���ڴ棬��Ϊsys�ڴ棬Ӧ���ڴ�
//����1���Ƿ�Ϊsys�ڴ�
//���أ�ʣ���ڴ�size
int32 GetFreeMemory(E_Mex_MemoryPool ePool);

//ȡ��ָ��Ӧ�õ�����
//����1��Ӧ��ID
//���أ�Ӧ�õ�����

//Ӧ�������У���&��
//	MEXAPP_ATTRIB_SMALL_TINY_ALWAYS_ON_TOP			��������ǰ��
//	MEXAPP_ATTRIB_FULL_NEED_BACKUP_BUF					ȫ������ÿ֡���ػ�
//	MEXAPP_ATTRIB_FULL_PAINT_ON_BACKGROUND			���Ʋ��ᱻ�ж�
//	MEXAPP_ATTRIB_SMALL_TINY_CAN_BE_MOVE				����ģʽ�Ƿ���Ա��ƶ�
int32 GetAppAttribute(int32 iAppId);		

//����ָ��Ӧ�õ�����
//����1��Ӧ��ID
//����2��Ӧ�õ�����
//���أ�Ӧ�õ�����
int32 SetAppAttribute(int32 iAppId int32 iAttribute);		

//����ָ��Ӧ�õ�ָ���߳��Ƿ���ͣ
//����1��Ӧ��ID
//����2��timer ID
//����3���Ƿ���ͣ
//���أ��Ƿ�ɹ�
bool PauseAppTimer(int32 iAppId, int32 iTimerId, bool bPause);	


//ȡ��ϵͳgdi layer���ָ��
//������layer id��һ�� 0 ���� 1
//���أ�����ָ�룬NULL��ʾû�иò�
uint16* GetScreenLayerBuffer(int32 iLayer);

//����ϵͳ˫�����Ż�
//�������Ƿ����
//ע��Ӧ�ó�������ò���true���ã������˳�ǰ������false���ã�����Ӱ����������
void DisableDbBufferBlt(bool bDisable);


//ȡ��ϵͳ���е���Ļbuf��Ϣ
//����1��bufָ�루out��
//����2��buf��out��
//����3��buf�ߣ�out��
//���أ���
void GetFullScreenBuffer(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight);		

//ȡ��Ӧ��ע���buf��Ϣ
//����1��AppID
//����2��bufָ�루out��
//����3��buf��out��
//����4��buf�ߣ�out��
//���أ���
void GetAppScreenBuffer(int32 iAppId, uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight);	

//ע��Ӧ�õ���Ļbuf��Ϣ
//����1��bufָ��
//����2��buf��
//����3��buf��
//���أ���
void RegisterScreenBuffer(uint16 *pBuffer, uint16 uWidth, uint16 uHeight);		

//ɾ��ע��Ӧ�õ���Ļbuf��Ϣ
//���أ���
void UnRegisterScreenBuffer(void);		


//������ʱ�������ؼ�ʱ��ID
//����1��timerʱ���϶
//���أ���ʱ��ID
int32 SetTimer(int32 iTime);

//�رռ�ʱ��
//����1����ʱ��ID
//���أ��Ƿ�ɹ�
bool KillTimer(int32 iTimerId);		

//�޸ļ�ʱ���ļ�ʱ���
//����1��App ID
//����2��timerID
//����3��timerʱ���϶
//����4��timerԭʼʱ���϶ָ�루out��
//���أ��Ƿ�ɹ�
bool SetTimerInterval(int32 iAppId, int32 iTimerId, int32 iNewDuration, int32 *pOldDuration);	

//����ϵͳ��ʱ������
//���أ�������
int32 gettime( void );		

//ȡ��Ӧ������ģʽ
//���أ�Ӧ������ģʽ���μ�E_Mex_RunMode
E_Mex_RunMode GetRunMode(void);		

//�л�Ӧ������ģʽ
//��������ģʽ���μ�enum E_Mex_RunMode
//���أ��л��Ƿ�ɹ�
bool SwitchRunMode(E_Mex_RunMode newMode);


//ȡ��ע���ָ��
//���أ�ע���ָ��
void* GetRegistry(void);		


//��ָ��Ӧ�÷�����Ϣ
//����iSrcAppId ����ϢԴӦ��ID��SYSTEMΪ0��
//����iDstAppId ����ϢĿ��Ӧ��ID����Ϊ��Ӧ��ID����
//����eMsgType ����Ϣ���ͣ��μ�enum E_Mex_MessageType��
//����iMsgData0 ~ iMsgData3����Ϣ����0-3��������Ϣ�������ض����塣
//���� int32     ����������0-δ����1-�Ѵ���
int32 SendMessage(int32 iSrcAppId, int32 iDstAppId, E_Mex_MessageType eMessage, int32 iMsgData0, int32 iMsgData1, int32 iMsgData2, int32 iMsgData3);		

//��ȡ��һ��AppId
//����iCurAppId���Ӵ�AppId��������0���ص�һ��AppId
//���أ���CurAppId�������һ��App��Id
int32 GetNextAppId(int32 iCurAppId);	//�ڲ�����

//ͨ��appid��ȡApp��Ϣ
//����1��Ӧ��id
//����2��Ӧ������type  (out)
//����3��Ӧ�ð汾��Ϣ(out)
//����4��Ӧ������(out)
//����5��Ӧ��Ŀ¼�������mexapp����"HelloWorld"��system��Ϊ""(out)
//����6��Ӧ���ļ�������"HelloWorld.mex"(out)
//���أ���ȡ�Ƿ�ɹ���ֻ��iCurAppId�ǷǷ�ֵʱ����false
bool GetAppInfoFromId(int32 iCurAppId, int32 *piAppType, int16 *piAppVersion, wchar_t **ppAppName, wchar_t **ppFileDir, wchar_t **ppFileName);	//�ڲ�����

//ͨ���ļ�����ȡApp��Ϣ
//����1��Ӧ��Ŀ¼�������mexapp����"HelloWorld"��system��Ϊ""(in)
//����2��Ӧ���ļ�������"HelloWorld.mex"(in)
//����3���Ƿ���sysĿ¼��(in)
//����4��Ӧ������type  (out)
//����5��Ӧ�ð汾��Ϣ(out)
//���أ���ȡ�Ƿ�ɹ������ļ�������ʱ����false
bool GetAppInfoFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int32 *piAppType, int16 *piAppVersion);

//ͨ������·���ļ�����ȡApp��Ϣ
//����1��Ӧ���ļ�������"E:\MexApp\Test\Test.mex"(in)
//����2��Ӧ������type  (out)
//����3��Ӧ�ð汾��Ϣ(out)
//���أ���ȡ�Ƿ�ɹ������ļ�������ʱ����false
bool GetAppInfoFromFile_ext(const wchar_t *szFullFileName, int32 *piAppType, int16 *piAppVersion);

//ȡ��ϵͳ������Ϣ
//����cfg����Ϣ���͡�
//���أ���Ϣ����
int32 GetPhoneConfig(E_Mex_Phone_Config cfg);		


//ȡ��ϵͳʱ��
//����pDt��ʱ����Ϣ�ṹ��out�����μ���Mex_DateTime
//���أ���
void GetDateTime(Mex_DateTime *pDt);		


//file functions

//�����Ծ���·�����ļ��ĺ���
//����1���ļ�������·��
//����2��ģʽ��"	��⣫"��"���"��"����"
//���أ��ļ����
int32* fopenw_org(const wchar_t *filename, const wchar_t *mode);

//�����Ծ���·�����ļ��ĺ���
//����1���ļ���
//����2��ģʽ��"	��⣫"��"���"��"����"
//����3���ļ���·��������·��
//����4��mex�ļ�����NULL��ֱ�Ӵ�path��filename�ļ�
//����5���Ƿ�ֻ��mex�ļ��ж�ȡ
//���أ��ļ����
int32* fopenw_ext(const wchar_t *filename, const wchar_t *mode, const wchar_t *path, const wchar_t *mexFileName, bool bCheckInsideMexOnly);		

//���ļ������ֽ�·����֧��"rb+"  "rb"  "wb"
//���أ��ļ����
int32* fopenw(const wchar_t *filename, const wchar_t *mode);		

//���ļ���֧��"rb+"  "rb"  "wb"
//���أ��ļ����
int32* fopen(const char *filename, const char *mode);	

//�ļ���λ
int32 fseek(int32* handle, long offset, int32 whence);		

//�ļ���ǰ��ȡλ��
int32 ftell(int32* handle);		

//�ļ��ر�
int32 fclose(int32* handle);		


//ȡ���ļ���С
size_t GetFileSize(int32* handle);		

//�ļ�ȡ��
size_t fread(void *ptr, size_t size, size_t nitems, int32* handle);		

//д���ļ�
size_t fwrite(const void *ptr, size_t size, size_t nitems, int32* handle);		

//flush�ļ���
int32 fflush(int32* handle);		

//ɾ���ļ�(Ӧ������·��)
int32 remove(const wchar_t *file);		

//�ļ�������(Ӧ������·��)
int32 rename(const wchar_t *ofile, const wchar_t *nfile);		

//�����ļ���(Ӧ������·��)
int32 CreateDir(const wchar_t *dir);		

//ɾ���ļ���(Ӧ������·��)
int32 RemoveDir(const wchar_t *dir);	

//ɾ���ļ�(����·��)
int32 remove_ext(const wchar_t *file);		

//�ļ�������(����·��)
int32 rename_ext(const wchar_t *ofile, const wchar_t *nfile);		

//�����ļ���(����·��)
int32 CreateDir_ext(const wchar_t *dir);		

//ɾ���ļ���(����·��)
int32 RemoveDir_ext(const wchar_t *dir);	

//�ж�Ŀ¼�Ƿ����(����·��)
bool IsDirExist_ext(const wchar_t *dir);

//�����ļ����µ������ļ�
//����1���ļ���·��
//����2���ļ���ʽ
//����3��Ҫ���ҵ����ļ����Ʊ���buf��out�����ⲿ����
//����4��buf��С
//���أ����Ҿ��
int32 FindFirst(const wchar_t *path, const wchar_t *namePattern, const wchar_t *fileName, uint32 maxLength);	

//�����ļ����µ��ļ���
//����1���ļ���·��
//����2��Ҫ���ҵ����ļ������Ʊ���buf��out�����ⲿ����
//����3��buf��С
//���أ����Ҿ��
int32 FindDirFirst(const wchar_t *path, const wchar_t *fileName, uint32 maxLength);	

//�����ļ����µ���һ���ļ���
//����1�����Ҿ��
//����2��Ҫ���ҵ����ļ������Ʊ���buf��out�����ⲿ����
//����3��buf��С
//���أ��Ƿ��ҵ���<0 ���ҽ���
int32 FindNext(int32 findHandle, const wchar_t *fileName, uint32 maxLength);		

//�����ļ����µ���һ���ļ���
//����1�����Ҿ��
//���أ�0 �رճɹ�
int32 FindClose(int32 findHandle);	


//��ӡlog��Ϣ���������ļ���
//iFilter��Ϊ��
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

//ȡ��logiFilter��&��
int32 getlogfilter(void);

//����logiFilter��&��
void setlogfilter(int32 iFilter);

//��profilingʹ�õĺ���
void StartProfiling(bool bRestart, int32 iNbFrames);
void AddProfilingData(bool bIsStart, int16 *piLevel, const char *pText);

//ȡ�ð���״̬
E_Mex_KeyEvent GetKeyState(E_Mex_KeyCode keyCode);

//���ð���״̬
E_Mex_KeyEvent SetKeyState(E_Mex_KeyEvent keyEvent, E_Mex_KeyCode keyCode);	

//���������ڴ�
void* VMalloc(size_t iSize);

//�ͷ������ڴ�
void VFree(void* pVmPtr);

//����ʵ�ڴ渴�����ݵ������ڴ�
void* VMemcpy(void* pVmPtr, void *pMemPtr, size_t size);

//�������ڴ����ݸ��Ƶ���ʵ�ڴ�
void* VToMem(void *pMemPtr, void *pVmPtr, size_t size);

//����͵õ��̶��ڴ棬�ֻ��ػ�Ҳ���Ǵ���
void* VPMalloc(const char *szKeyName, size_t iSize, bool *pbAlreadyExist);

//�ͷ�����̶��ڴ�
void VPFree(void* pVmPtr);

//����ʹ�ñ������ⲿ�ڴ�
//����ԭ�������ã����ڳɶԵ��ã�
//�ó���Ӧ��ֻ��manager����
bool SetUseReservedMem(bool bUse);

//����ϵͳ�߳�, ���ڲ�������ʹ��
//����1 : ʱ����
//����2 : �ص�����
//����: �̱߳��
int32 StartSystemTimer (int32 iTime, MexFuncPtr func);

//�ر�ϵͳ�߳�, ���ڲ�������ʹ��
//����1 :  �̱߳��
//����:�Ƿ�ɹ�
bool KillSystemTimer (int32 iTimeId);

//�����̼߳��, ���ڲ�������ʹ��
//����1 :  �̱߳��
//����2 : ���¼����
//����3 : ԭʱ����(out)
//����:�Ƿ�ɹ�
bool SetSystemTimerInterval (int32 iTimerId, int32 iNewTime, int32 *pOldTime);

//ע������Ӧ�õ���Ļ�л�ʱ�Զ�����
//����1����ĻId��0-3
bool RegisterLaunchOnScreen(int32 iScreenId);

//ע������Ӧ�õ�ƽ̨����ʱ�Զ�����
bool RegisterLaunchOnStartup(void);

//��ȡ��һ֡ȫ����buffer��manager����������Ӧ��ʹ��
void GetLastFullScreenBuffer(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight);
bool RegisterLaunchOnFullAppExit(void);
//���������ĺ���
//---------------------------------------------
//����1: �����ı��⣬unicode ��
//����2: ���������ͣ���ö�ٶ���
//����3: ����������������볤��
//����4: �Ƿ�Ϊ���������
//����5: �Ƿ������л����뷨
//����6: ������ַ����洢��buf������ΪNULL
//����7: ���ص��ĺ���ָ��
//---------------------------------------------
bool InputBox_Create(  wchar_t* pInputTitle, E_Mex_Input_Type iInputType, uint32 iMaxNum,
								bool bIsPassword, bool bCanChange, 
								uint8 * pContent, Mex_Input_Func pInputFunc )

//��ѹ��
//����1: ��ѹ���ָ��
//����2: ��ѹ����ָ��
//����3: ��ѹԴ
//����4: ��ѹԴ����
//����: ��ѹ���
int32 zip_uncompress(int8 * pDest, unsigned long* lDestLen, const int8 * pSource, unsigned long lSourceLen);

//��ͣϵͳ��������ͼ
//������֡��
//ע����֡��Ч
void DisablePaintOpt(int32 iMaxNbFrames);

//����sram�ڴ�
void* smalloc(size_t iSize);

//�ͷ�sram�ڴ�
void sfree(void *psPtr);

//����ϵͳ��������ʱʹ�����е�sram�ռ�
bool NotifyLockSRam(void);

//����ϵͳ������sram�ռ��ú���
bool NotifyUnlockSRam(void);

//�ر�mexƽ̨
//�������ر����࣬ö��ֵ
//		E_MPS_HotReboot��������
//		E_MPS_ColdReboot��������
//		E_MPS_HotShutdown���ȹر�
//		E_MPS_ColdShutdown����ر�
//����ֵ����
//ע��ϵͳ������һ֡�������߹ر�
void ShutdownPlatform(E_Mex_PlatformShutdown eOp);

//ȡ��Ӧ��ע���Screen ͨ͸ɫ��Ϣ
//����1: Ӧ��id (in)
//����2: ͨ͸ɫɫֵ(out)
//����3: �Ƿ�ͨ͸(out)
void GetAppScreenKeyColor(int32 iAppId, uint16 *pKeyColor, bool *pbIsTrans );

//Ӧ��ע���Լ���Screen ͨ͸ɫ��Ϣ
//����1: ͨ͸ɫɫֵ
//����2: �Ƿ�ͨ͸
void RegisterScreenKeyColor(uint16 uKeyColor, bool bIsTrans );

// ����  �ܡ�  �������·��
// ��˵  ����  �ú����������·����·��������
// ��ע  �⡿  ·�����ּ����Ʋ��ֵ�����ʹ������Ҫ�ͷ�
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      pathPart        ·������
// ������ 2��      namePart        ���Ʋ���
// ������ 3��      path            ����·��
// ������ֵ��      bool��          �Ƿ�ɹ�
bool SeparateAbsPath(wchar_t **pathPart, wchar_t **namePart, const wchar_t *path);

//��������·�����Զ��ݹ�һ����������
//����������·��
//���أ��Ƿ�ɹ�
bool CreateDir_ext_recur(const wchar_t *szFullDir);


//����ѯ����ʾ�� �Ƿ�Ҫ�˳�ƽ̨ ���ѡ�� ���˳�ƽ̨���ص�idle����
//����1:�Ƿ� ��ȫ�˳�
void ExitPlatform( bool bExitAll );

//ȡ�ó������õ�Ĭ�ϴ�С��������������Ч
//����1: Ӧ��id   (IN)
//����2: ע���(OUT)
//����3: ע���(OUT)
//------------------
//����: �Ƿ�ɹ�
bool GetAppRegistSizeFromId(int32 iCurAppId, uint16 *puWidth, uint16 *puHeight );

//ȡ�ó������õ�Ĭ�ϴ�С��������������Ч
//����1: Ӧ��Ŀ¼   (IN)
//����2: Ӧ���ļ���  (IN)
//����3: ע���(OUT)
//����4: ע���(OUT)
//------------------
//����: �Ƿ�ɹ�
//ע�⣺��֧��1970����ǰ��ʱ��
bool GetAppRegistSizeFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, uint16 *puWidth, uint16 *puHeight );

//ȡ�õ�ǰϵͳʱ��
//------------------
//����: ��1970��1��1��0���𵽵�ǰʱ�������
uint32 GetCurrentTime(void);

//������ת��ΪMex_DateTime
//����1:��1970��1��1��0����ĳʱ�������  (IN)
//����2��ת���������ʱ��	��out��
//------------------
//����:
//ע�⣺��֧��1970����ǰ��ʱ��
void UtcSec2DateTime(uint32 utc_sec, Mex_DateTime *pdt);

//���ֻ�������ʱ�ڴ�
//ע�����ڴ�����ͬһ֡���ͷţ�����Խ��Խ��
void* xmalloc(size_t iSize);

//�ͷ���ʱ�ڴ�
void xfree(void* pPtr);

//��ֹmanager�� "#��" ��ʾ�˳���ǰӦ�ó��� �Ĺ���
//�������Ƿ��ֹ�ù���
//���أ�ָ���Ƿ���ܣ�������õ�Ӧ�ó���û��Ȩ�ޣ�������widget�������ʧ�ܣ�
bool DisableQuitControl(bool bDisable);

//��ȡӦ�ó����extra������mpj�ļ��еڶ��е�10��int16���֣�����֪���壺
//[0]: �汾��
//[1]: �������к�Ŀ��
//[2]: �������к�ĸ߶�
//[3]: Ӧ�õ�ΨһId��
//[4]: ����
//[5]: ����
//[6]: ����
//[7]: ����
//[8]: ����
//[9]: ����
//����ʹ��ͬGetAppInfo
bool GetAppExtraInfoFromId(int32 iCurAppId, int16 *ppInfo );
bool GetAppExtraInfoFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int16 *ppInfo );

//��ȡϵͳ�ڲ�����
int32 GetSystemParam(void);

//����һЩ֡��
void Sleep(int32 iTicks);

//ע������ϵͳ���򣨵�ƽ̨�˳�ʱ��
//�������E_MEX_Entry_System_Appö�٣�pParamΪ����
void RegisterLaunchSystemApp(E_MEX_Entry_System_App eApp, void *pParam, int32 iParamSize);

// ԭGUI�ⲿ�ֺ��� begin ////////////////////////////////////
/************************************************************************
* ���洦����
************************************************************************/

// ����  �ܡ�  ��ȡ����ü��߽�
// ��˵  ����  �ú�����ȡ��ǰ�������ü��߽磬��Ϊ���û��潫����ϵͳĬ�ϲü��߽�
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] x1              �ü��߽����Ͻ�x1
// ������ 2��      [OUT] y1              �ü��߽����Ͻ�y1
// ������ 3��      [OUT] x2              �ü��߽����½�x2
// ������ 4��      [OUT] y2              �ü��߽����½�y2
// ������ֵ��      [OUT] ��
void GetBufferClipper(int16 *x1, int16 *y1, int16 *x2, int16 *y2);

// ����  �ܡ�  ���û������ü��߽�
// ��˵  ����  �ú������õ�ǰ�������ü��߽磬ϵͳĬ�ϻ������ü��߽�Ϊ��0��0��-->��width - 1�� height - 1��������OS�����
//             ���������Ըòü��߽���ַ���/ͼ��/ͼ����вü���
//             �������û����øú����������û������ü��߽�ʱ��OS����ƺ��������²ü��߽���ַ���/ͼ��/ͼ����вü�
// ��ע  �⡿  ��������� x1 <= x2, y1 <= y2�����û����õĲü��߽� x1 > x2, y1 > y2������ƺ����������ڻ����л����κ��ַ�
//             ��/ͼ��/ͼ��
//           ���ü��߽粻����������ԭ�򣬼��ü��߽罫�������ϽǼ����½ǣ��ü��ַ���/ͼ��/ͼ��ʱ�������ϽǼ����½ǵ�Ϊ��׼
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 2��      [IN]  x1              �ü��߽����Ͻ�x1
// ������ 2��      [IN]  y1              �ü��߽����Ͻ�y1
// ������ 2��      [IN]  x2              �ü��߽����½�x2
// ������ 2��      [IN]  y2              �ü��߽����½�y2
// ������ֵ��      [OUT] ��
void SetBufferClipper(int16 x1, int16 y1, int16 x2, int16 y2);

// ����  �ܡ�  ���û������Ӳü��߽�
// ��˵  ����  �ú������õ�ǰ�������Ӳü��߽磬�ú�������ǰ�ü��߽�����ϼ������õڶ���ü��߽磬
//             ���ú�Ĳü��߽�Ϊ���ü��߽���ԭ�ü��߽�Ľ���
// ��ע  �⡿���ü��߽粻����������ԭ�򣬼��ü��߽罫�������ϽǼ����½ǣ��ü��ַ���/ͼ��/ͼ��ʱ�������ϽǼ����½ǵ�Ϊ��׼
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1              �ü��߽����Ͻ�x1
// ������ 2��      [IN]  y1              �ü��߽����Ͻ�y1
// ������ 3��      [IN]  x2              �ü��߽����½�x2
// ������ 4��      [IN]  y2              �ü��߽����½�y2
// ������ֵ��      [OUT] ��
void SetBufferSubClipper(int16 x1, int16 y1, int16 x2, int16 y2);

// ����  �ܡ�  ����OS���ƻ���
// ��˵  ����  �ú�������OS���ƻ��������û���ָ�������л����ַ���ͼ��ʱ����ҪԤ�ȵ��øú�����
//             ��������OS���ƻ��棬��Ĭ�Ͻ�ϵͳ��������������Ļ���棩��Ϊ���ƻ���
// ��ע  �⡿  �ú��������û����ã��ú������ɺ꡾OS_BEGIN����OS_END���Զ�����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  buffer          ָ������
// ������ֵ��      [OUT] Mex_GuiBuffer�� ����ǰ����
Mex_GuiBuffer SelectGuiBuffer(Mex_GuiBuffer *buffer);

/************************************************************************
* �ַ�/�ַ���������
*
* ���������˵��
*-----------------------------------------------------------------------
* ��������                      ˵��
* MEX_FONTATTR_NORMAL           ����
* MEX_FONTATTR_BOLD             ����
* MEX_FONTATTR_ITALIC           б��1
* MEX_FONTATTR_OBLIQUE          б��2
* MEX_FONTATTR_UNDERLINE        �»���
* MEX_FONTATTR_STRIKETHROUGH    ɾ���ߣ�δʹ�ã�
*-----------------------------------------------------------------------
* �����С                      ˵��
* MEX_FONTSIZE_SMALL            С��
* MEX_FONTSIZE_MEDIUM           �к�
* MEX_FONTSIZE_LARGE            ��ţ�δʹ�ã�
* MEX_FONTSIZE_SUBLCD           ��δʹ�ã�
* MEX_FONTSIZE_DIALER           ��δʹ�ã�
* MEX_FONTSIZE_VIRTUAL_KEYBOARD ��δʹ�ã�
*-----------------------------------------------------------------------
* ����λ��                      ˵������������DrawStringEx��ʹ�ã�
* MEX_FONTPOS_DEFAULT           Ĭ��
* MEX_FONTPOS_VTOP              ��ֱ�϶���
* MEX_FONTPOS_VMIDDLE           ��ֱ����
* MEX_FONTPOS_VBOTTOM           ��ֱ�¶���
* MEX_FONTPOS_HLEFT             ˮƽ�����
* MEX_FONTPOS_HCENTER           ˮƽ����
* MEX_FONTPOS_HRIGHT            ˮƽ�Ҷ���
************************************************************************/

// ����  �ܡ�  ��ȡ�ַ��ߴ�
// ��˵  ����  �ú�����ȡָ����ʽ�ַ��ĳߴ磬�������Լ������С�μ������������˵����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  ch              ָ���ַ�
// ������ 2��      [OUT] width           �ַ���ȣ�ָ�룩
// ������ 3��      [OUT] height          �ַ��߶ȣ�ָ�룩
// ������ 4��      [IN]  fontAttr        ��������
// ������ 5��      [IN]  fontSize        �����С
// ������ֵ��      [OUT] ��
void GetCharDimension(uint32 ch, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize);

// ����  �ܡ�  ��ȡ�ַ����ߴ�
// ��˵  ����  �ú�����ȡָ����ʽ�ַ����ĳߴ磬�������Լ������С�μ������������˵����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          ָ���ַ���
// ������ 2��      [OUT] width           �ַ���ȣ�ָ�룩
// ������ 3��      [OUT] height          �ַ��߶ȣ�ָ�룩
// ������ 4��      [IN]  fontAttr        ��������
// ������ 5��      [IN]  fontSize        �����С
// ������ֵ��      [OUT] ��
void GetStringDimension(const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize);

// ����  �ܡ�  ��ȡ�Զ������ַ����ߴ�
// ��˵  ����  �ú�����ȡָ����ʽ�ַ����ĳߴ磬�������Լ������С�μ������������˵����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          ָ���ַ���
// ������ 2��      [OUT] width           �ַ���ȣ�ָ�룩
// ������ 3��      [OUT] height          �ַ��߶ȣ�ָ�룩
// ������ 4��      [IN]  fontAttr        ��������
// ������ 5��      [IN]  fontSize        �����С
// ������ 6��      [IN]  charSpace       �ַ����
// ������ֵ��      [OUT] ��
void GetStringSpacingDimension(const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize, int16 charSpace);

// ����  �ܡ�  ����ַ�����ߴ�
// ��˵  ����  �ú������ָ��������ָ���������ָ����ʽ�ַ���ʱ�ַ�����ߴ磬Ĭ��Ϊ��Ļ���档�������Լ������С��
//             λ�òμ������������˵����
//             �ú����Զ������з���\r������\n������\r��������ƶ������п�ʼ������\n������������ƶ�һ��
//           ����ʹ��Ĭ��λ�ã���ӻ����������Ͻǿ�ʼ�����ַ������ϸ���ӻ��з�����
//             ����ʹ��Ĭ��λ�ã����з�Ч������ɲ�ͬ�ַ�������ķָ����\r�������������ǰ��ֿ����������䣬����
//             ��ͬһ�У�����\n����������������ƶ�һ�У���ʵ��ÿһ������ʾλ�ý���Ӳ�����λ������
//           ���������Զ����У����ַ������Ƴ������������ұ߽�ʱ�����Զ����е���һ�С�
//             ���������Զ����У����ַ�����ʹ�������������ұ߽磬Ҳ�����Զ����У����ǽ��ַ������Ƶ����ɼ�������
// ��ע  �⡿  �ú������յ��ַ�������ΪUCS2�����ʽ���ַ����������Ƶ�ʱӢ���ַ���������ֱ����Ӣ���ַ���ǰ���ϡ�L����ʾ��
//             �������Ƶ�ʱ�����ַ�������ʹ�ú�����str2nwcs����tstr2wcs������ת���ſ�������ʾ��������ֱ���������ַ���
//             ǰ�ӡ�L��ǰ׺��ʾ����L��ǰ׺�������ַ�������Ļ�ϻ���ʾ���롣
// ����  �ԡ�  �Ѳ���  T1 
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          �ַ���
// ������ 2��      [OUT] width           ���ݿ��
// ������ 3��      [OUT] height          ���ݸ߶�
// ������ 4��      [IN]  areaWidth       ����������
// ������ 5��      [IN]  fontAttr        ��������
// ������ 6��      [IN]  fontSize        �����С
// ������ 7��      [IN]  lineWrap        �Ƿ��Զ�����
// ������ֵ��      [OUT] uint16��        ����
uint16 GetStringExDimension(const wchar_t *string, int32 *width, int32 *height, int16 areaWidth, 
							uint8 fontAttr, uint8 fontSize, bool lineWrap);

// ����  �ܡ�  ����Զ������м���ַ�����ߴ�
// ��˵  ����  �ú������ָ��������ָ���������ָ����ʽ�ַ���ʱ�ַ�����ߴ磬Ĭ��Ϊ��Ļ���档�������Լ������С��
//             λ�òμ������������˵����
//             �ú����Զ������з���\r������\n������\r��������ƶ������п�ʼ������\n������������ƶ�һ��
//           ����ʹ��Ĭ��λ�ã���ӻ����������Ͻǿ�ʼ�����ַ������ϸ���ӻ��з�����
//             ����ʹ��Ĭ��λ�ã����з�Ч������ɲ�ͬ�ַ�������ķָ����\r�������������ǰ��ֿ����������䣬����
//             ��ͬһ�У�����\n����������������ƶ�һ�У���ʵ��ÿһ������ʾλ�ý���Ӳ�����λ������
//           ���������Զ����У����ַ������Ƴ������������ұ߽�ʱ�����Զ����е���һ�С�
//             ���������Զ����У����ַ�����ʹ�������������ұ߽磬Ҳ�����Զ����У����ǽ��ַ������Ƶ����ɼ�������
// ��ע  �⡿  �ú������յ��ַ�������ΪUCS2�����ʽ���ַ����������Ƶ�ʱӢ���ַ���������ֱ����Ӣ���ַ���ǰ���ϡ�L����ʾ��
//             �������Ƶ�ʱ�����ַ�������ʹ�ú�����str2nwcs����tstr2wcs������ת���ſ�������ʾ��������ֱ���������ַ���
//             ǰ�ӡ�L��ǰ׺��ʾ����L��ǰ׺�������ַ�������Ļ�ϻ���ʾ���롣
// ����  �ԡ�  �Ѳ���  T1 
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          �ַ���
// ������ 2��      [OUT] width           ���ݿ��
// ������ 3��      [OUT] height          ���ݸ߶�
// ������ 4��      [IN]  areaWidth       ����������
// ������ 5��      [IN]  fontAttr        ��������
// ������ 6��      [IN]  fontSize        �����С
// ������ 7��      [IN]  lineWrap        �Ƿ��Զ�����
// ������ 8��      [IN]  lineSpace       �м��
// ������ 9��      [IN]  columnSpace     �м��
// ������ֵ��      [OUT] uint16��        ����
uint16 GetStringExSpacingDimension(const wchar_t *string, int32 *width, int32 *height, int16 areaWidth,
								   uint8 fontAttr, uint8 fontSize, bool lineWrap, int16 lineSpace, int16 columnSpace);

// ����  �ܡ�  �����ַ���
// ��˵  ����  �ú�����ָ��������ָ��λ�û���ָ����ʽ�ַ�����Ĭ��Ϊ��Ļ���档�������Լ������С�μ������������˵����
// ��ע  �⡿  �ú������յ��ַ�������ΪUCS2�����ʽ���ַ����������Ƶ�ʱӢ���ַ���������ֱ����Ӣ���ַ���ǰ���ϡ�L����ʾ��
//             �������Ƶ�ʱ�����ַ�������ʹ�ú�����str2nwcs����tstr2wcs������ת���ſ�������ʾ��������ֱ���������ַ���
//             ǰ�ӡ�L��ǰ׺��ʾ����L��ǰ׺�������ַ�������Ļ�ϻ���ʾ���롣
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          �ַ���
// ������ 2��      [IN]  x               ָ��λ��X����
// ������ 3��      [IN]  y               ָ��λ��Y����
// ������ 4��      [IN]  clr             ������ɫ
// ������ 5��      [IN]  fontAttr        ��������
// ������ 6��      [IN]  fontSize        �����С
// ������ֵ��      [OUT] int32��         ���ƺ���x��λ��
int32 DrawString(const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize);

// ����  �ܡ�  ����ָ���ַ������ַ���
// ��˵  ����  �ú�����ָ��������ָ��λ�û���ָ����ʽ�ַ�����Ĭ��Ϊ��Ļ���档�������Լ������С�μ������������˵����
// ��ע  �⡿  �ú������յ��ַ�������ΪUCS2�����ʽ���ַ����������Ƶ�ʱӢ���ַ���������ֱ����Ӣ���ַ���ǰ���ϡ�L����ʾ��
//             �������Ƶ�ʱ�����ַ�������ʹ�ú�����str2nwcs����tstr2wcs������ת���ſ�������ʾ��������ֱ���������ַ���
//             ǰ�ӡ�L��ǰ׺��ʾ����L��ǰ׺�������ַ�������Ļ�ϻ���ʾ���롣
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          �ַ���
// ������ 2��      [IN]  x               ָ��λ��X����
// ������ 3��      [IN]  y               ָ��λ��Y����
// ������ 4��      [IN]  clr             ������ɫ
// ������ 5��      [IN]  fontAttr        ��������
// ������ 6��      [IN]  fontSize        �����С
// ������ 7��      [IN]  charSpace       �ַ����
// ������ֵ��      [OUT] int32��         ���ƺ���x��λ��
int32 DrawStringSpacing(const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize, int16 charSpace);

// ����  �ܡ�  �����ַ�����
// ��˵  ����  �ú�����ָ��������ָ���������ָ����ʽ�ַ�����Ĭ��Ϊ��Ļ���档�������Լ������С�μ������������˵����
//             �ú����Զ������з���\r������\n������\r��������ƶ������п�ʼ������\n������������ƶ�һ��
//           ����ʹ��Ĭ��λ�ã���ӻ����������Ͻǿ�ʼ�����ַ������ϸ���ӻ��з�����
//             ����ʹ��Ĭ��λ�ã����з�Ч������ɲ�ͬ�ַ�������ķָ����\r�������������ǰ��ֿ����������䣬����
//             ��ͬһ�У�����\n����������������ƶ�һ�У���ʵ��ÿһ������ʾλ�ý���Ӳ�����λ������
//           ���������Զ����У����ַ������Ƴ������������ұ߽�ʱ�����Զ����е���һ�С�
//             ���������Զ����У����ַ�����ʹ�������������ұ߽磬Ҳ�����Զ����У����ǽ��ַ������Ƶ����ɼ�������
// ��ע  �⡿  �ú������յ��ַ�������ΪUCS2�����ʽ���ַ����������Ƶ�ʱӢ���ַ���������ֱ����Ӣ���ַ���ǰ���ϡ�L����ʾ��
//             �������Ƶ�ʱ�����ַ�������ʹ�ú�����str2nwcs����tstr2wcs������ת���ſ�������ʾ��������ֱ���������ַ���
//             ǰ�ӡ�L��ǰ׺��ʾ����L��ǰ׺�������ַ�������Ļ�ϻ���ʾ���롣
// ����  �ԡ�  �Ѳ���  T1 
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          �ַ���
// ������ 2��      [IN]  x1              �ַ��������������Ͻ�x����
// ������ 3��      [IN]  y1              �ַ��������������Ͻ�y����
// ������ 4��      [IN]  x2              �ַ��������������½�x����
// ������ 5��      [IN]  y2              �ַ��������������½�y����
// ������ 6��      [IN]  clr             ������ɫ
// ������ 7��      [IN]  fontAttr        ��������
// ������ 8��      [IN]  fontSize        �����С
// ������ 9��      [IN]  fontPos         ������뷽ʽ
// ������10��      [IN]  lineWrap        �Ƿ��Զ�����
// ������ֵ��      [OUT] int32��         ���ƺ���x��λ��
int32 DrawStringEx(const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, 
				   mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap);

// ����  �ܡ�  �����ַ����飨��ָ���м�༰�м�ࣩ
// ��˵  ����  �ú�����ָ��������ָ���������ָ����ʽ�ַ�����Ĭ��Ϊ��Ļ���档�������Լ������С��λ�òμ������������˵����
//             �ú����Զ������з���\r������\n������\r��������ƶ������п�ʼ������\n������������ƶ�һ��
//           ����ʹ��Ĭ��λ�ã���ӻ����������Ͻǿ�ʼ�����ַ������ϸ���ӻ��з�����
//             ����ʹ��Ĭ��λ�ã����з�Ч������ɲ�ͬ�ַ�������ķָ����\r�������������ǰ��ֿ����������䣬����
//             ��ͬһ�У�����\n����������������ƶ�һ�У���ʵ��ÿһ������ʾλ�ý���Ӳ�����λ������
//           ���������Զ����У����ַ������Ƴ������������ұ߽�ʱ�����Զ����е���һ�С�
//             ���������Զ����У����ַ�����ʹ�������������ұ߽磬Ҳ�����Զ����У����ǽ��ַ������Ƶ����ɼ�������
// ��ע  �⡿  �ú������յ��ַ�������ΪUCS2�����ʽ���ַ����������Ƶ�ʱӢ���ַ���������ֱ����Ӣ���ַ���ǰ���ϡ�L����ʾ��
//             �������Ƶ�ʱ�����ַ�������ʹ�ú�����str2nwcs����tstr2wcs������ת���ſ�������ʾ��������ֱ���������ַ���
//             ǰ�ӡ�L��ǰ׺��ʾ����L��ǰ׺�������ַ�������Ļ�ϻ���ʾ���롣
// ����  �ԡ�  �Ѳ���  T1 
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  string          �ַ���
// ������ 2��      [IN]  x1              �ַ��������������Ͻ�x����
// ������ 3��      [IN]  y1              �ַ��������������Ͻ�y����
// ������ 4��      [IN]  x2              �ַ��������������½�x����
// ������ 5��      [IN]  y2              �ַ��������������½�y����
// ������ 6��      [IN]  clr             ������ɫ
// ������ 7��      [IN]  fontAttr        ��������
// ������ 8��      [IN]  fontSize        �����С
// ������ 9��      [IN]  fontPos         ������뷽ʽ
// ������10��      [IN]  lineWrap        �Ƿ��Զ�����
// ������11��      [IN]  lineSpace       �м��
// ������12��      [IN]  columnSpace     �м��
// ������ֵ��      [OUT] int32��         ���ƺ���x��λ��
int32 DrawStringExSpacing(const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, 
						  mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap, int16 lineSpace, int16 columnSpace);

// ����  �ܡ�  �����ַ�
// ��˵  ����  �ú�����ָ��������ָ��λ�û���ָ����ʽ�ַ���Ĭ��Ϊ��Ļ���档�������Լ������С�μ������������˵����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  pCharData       �ַ���������
// ������ 2��      [IN]  dataSize        �������ݴ�С
// ������ 3��      [IN]  dataWidth       �������ݿ��
// ������ 4��      [IN]  dataHeight      �������ݸ߶�
// ������ 5��      [IN]  x               ����λ��X����
// ������ 6��      [IN]  y               ����λ��Y����
// ������ 7��      [IN]  clr             �ַ���ɫ
// ������ 8��      [IN]  fontAttr        �ַ�����
// ������ֵ��      [OUT] ��
void DrawChar(uint8 *pCharData, uint32 dataSize, uint16 dataWidth, uint16 dataHeight, int32 x, int32 y, mex_color clr, uint8 fontAttr);

// ����  �ܡ�  ��ȡ�ַ���������
// ��˵  ����  �ú�����ȡָ����С�ַ��������ݡ������С�μ������������˵����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  ch              ָ���ַ�
// ������ 2��      [OUT] ppCharData      �������ݣ�ָ���ָ�룩
// ������ 3��      [OUT] pDataWidth      �������ݿ�ȣ�ָ�룩
// ������ 4��      [OUT] pDataHeight     �������ݸ߶ȣ�ָ�룩
// ������ 5��      [IN]  fontSize        �����С
// ������ֵ��      [OUT] int32��         �������ݴ�С
int32 GetCharData(int32 ch, uint8 **ppCharData, uint16 *pDataWidth, uint16 *pDataHeight, uint8 fontSize);

// ����  �ܡ�  ���Ƶ�
// ��˵  ����  �ú�����ָ�������л���ָ���㣬Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x               X����
// ������ 2��      [IN]  y               Y����
// ������ 3��      [IN]  clr             ��ɫ
// ������ֵ��      [OUT] ��
void DrawPointInternal(int32 x, int32 y, mex_color clr);

// ����  �ܡ�  ����ֱ��
// ��˵  ����  �ú�����ָ�������л���ָ��ֱ�ߣ�Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1              ���Ͻ�X����
// ������ 2��      [IN]  y1              ���Ͻ�Y����
// ������ 3��      [IN]  x2              ���½�X����
// ������ 4��      [IN]  y2              ���½�Y����
// ������ 5��      [IN]  clr             ��ɫ
// ������ֵ��      [OUT] ��
void DrawLineInternal(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr);

// ����  �ܡ�  �����߿��������
// ��˵  ����  �ú�����ָ�������л���ָ���߿��������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1              ���Ͻ�X����
// ������ 2��      [IN]  y1              ���Ͻ�Y����
// ������ 3��      [IN]  x2              ���½�X����
// ������ 4��      [IN]  y2              ���½�Y����
// ������ 5��      [IN]  clr             ��ɫ
// ������ֵ��      [OUT] ��
void DrawFrameRectInternal(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr);

// ����  �ܡ�  ����ʵ�ľ�������
// ��˵  ����  �ú�����ָ�������л���ָ��ʵ�ľ�������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1              ���Ͻ�X����
// ������ 2��      [IN]  y1              ���Ͻ�Y����
// ������ 3��      [IN]  x2              ���½�X����
// ������ 4��      [IN]  y2              ���½�Y����
// ������ 5��      [IN]  clr             ��ɫ
// ������ֵ��      [OUT] ��
void DrawFillRectInternal(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr);

// ����  �ܡ�  �����߿�Բ������
// ��˵  ����  �ú�����ָ�������л���ָ���߿�Բ������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x               Բ��X����
// ������ 2��      [IN]  y               Բ��Y����
// ������ 3��      [IN]  r               �뾶
// ������ 5��      [IN]  clr             ��ɫ
// ������ֵ��      [OUT] ��
void DrawFrameCircleInternal(int32 x, int32 y, int32 r, mex_color clr);

// ����  �ܡ�  ����ʵ��Բ������
// ��˵  ����  �ú�����ָ�������л���ָ��ʵ��Բ������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x               Բ��X����
// ������ 2��      [IN]  y               Բ��Y����
// ������ 3��      [IN]  r               �뾶
// ������ 5��      [IN]  clr             ��ɫ
// ������ֵ��      [OUT] ��
void DrawFillCircleInternal(int32 x, int32 y, int32 r, mex_color clr);
// ԭGUI�ⲿ�ֺ��� end ////////////////////////////////////

//��ȡӦ���ڲ��������ڲ�����
void GetAppParam(int32 iAppId, void *pResults);

//��ȡ�ڴ�ع��������ڲ�����
void GetAdmFunctions(funcptr *pResults);

//ȡ��Ӧ���Ƿ��������ĳ�����
//����1: Ӧ��id   
//����2: Ȩ������
//------------------
//����: �Ƿ�����
bool IsAppPermissiveToDo( int32 iCurAppId, E_Mex_App_Permission_Type ePermission );


// ����  �ܡ�  ��ѹ�ڴ�����
// ��˵  ����  �ú�����ѹ�ڴ�����
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      memData         �ڴ�����
// ������ 2��      orgData         ԭʼ����
// ������ֵ��      bool��          �Ƿ�ɹ�
bool UnCompressDataFromMemory(uint8 *memData, uint8 *orgData);

// ����  �ܡ�  ��ѹ�ļ�����
// ��˵  ����  �ú�����ѹ�ļ�����
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      file_handle     �ļ����
// ������ 2��      orgData         ԭʼ����
// ������ֵ��      bool��          �Ƿ�ɹ�
bool UnCompressDataFromFile(int32 *file_handle, uint8 *orgData);


//����Manager���ܣ���SendMessageʵ�֣��ڲ�������չʹ��
int32 CallManagerFunction(E_Mex_Mgr_Manager_Func eFunc, int32 iParam0, int32 iParam1, int32 iParam2);

//ƽ̨�˳���ģ�����밴��
//���ͬһ֡������RegisterLaunchSystemApp,��ģ�ⰴ�������������˵�Ӧ����
void RegisterSimulateKey(E_Mex_KeyEvent eKeyEvent, E_Mex_KeyCode eKeyCode);

//���߳���غ�����ʹ�÷���ͬSystemTimer
//func����һ���߳���ִ�У���С�Ļ���������ͷ�����
//�����о�����Աʹ��
int32 StartThreadTimer(int32 iTime, MexFuncPtr func);
bool KillThreadTimer(int32 iTimeId);
bool SetThreadTimerInterval(int32 iTimerId, int32 iNewTime, int32 *pOldTime);

void TakeMutex(int32 iIdx);
void GiveMutex(int32 iIdx);

//ȡ��Ӧ��λ����Ϣ
//����1 : Ӧ��id
//����2 : X����(out)
//����3 : Y����(out)
//����: �Ƿ�ɹ�
bool GetAppPosition( int32 iAppId, int16 * pCenterPosX, int16 * pCenterPosY );

//����Ӧ��λ����Ϣ������widget��Ч
//����1 : Ӧ��id
//����2 : X����
//����3 : Y����
//����: �Ƿ�ɹ�
bool SetAppPosition( int32 iAppId, int16 iCenterPosX, int16 iCenterPosY );

//QQϵͳ״̬��ע��
//���� iAppid  �����ߵ�Appid
//����ֵ  ���С��0 ˵��ע��ʧ��
// -1 ��ʾ �Ѿ�������QQע����״̬�� Ŀǰ��֧������QQͬʱ����
// -2 ��ʾ ϵͳ��֧��״̬��
//�����С��0 ����״̬�����  �Ժ��ÿ�ζ�״̬���Ĳ��� ����Ҫʹ�øþ��
int8 RegSystemStatusIcon(int32 iAppid);

//ע����״̬����ע��  
//���� handle Ҫע����״̬���ľ��
//iAppid  �����ߵ�appid  
void UnRegSystemStatusIcon(int8 handle,int32 iAppid);

//����״̬��״̬
//���� handle ��Ҫ���ĵ�״̬���ľ��
//type  ���ĵ�״̬
//iAppid  �����ߵ�appid 
void SetSystemStatusIcon(int8 handle,E_MEX_QQ_STATUS_TYPE type,int32 iAppid);

//��ȡ״̬��״̬
//���� handle ��Ҫ��ȡ��״̬���ľ��

//iAppid  �����ߵ�appid 

//����ֵ ��ǰ״̬��״̬

E_MEX_QQ_STATUS_TYPE GetSystemStatusIcon(int8 handle,int32 iAppid);

//��ʾ���߹ر�״̬��ͼ��
//���� handle ��Ҫ���ĵ�ϵͳͼ��ľ��
//bShow  trueΪ��ʾ falseΪ�ر�
//iAppid ������id
void SetSystemStatusIconStatus(int8 handle,bool bShow,int32 iAppid);

//sdk >= 1001����
//��ȡ������layer buffer�Լ�������Ϣ
bool GetScreenLayerBufferWithInfo(int32 iLayer, uint16 **ppScrBuf, uint16 *puWidth, uint16 *puHeight);

//sdk >= 1004����
//������ͷ�t�ڴ�
void* tmalloc(size_t iSize);
void tfree(void* psPtr);

//sdk >= 1009����
//��ʼ���ļ����
//����sel_type �ļ����ѡ������
//		base_path ��·��
//		pfCB �ļ�ѡ�����ص�
//�����Ƿ�ɹ�

int8 Fmgr_Create(uint32 sel_type, wchar_t* base_path,Fmgr_CallBack pfCB);

//sdk >= 1009����
//ȡ���ļ����
//Fmgr_Create ��Fmgr_Cancel��ɶԳ���

void Fmgr_Cancel(void);

//sdk >= 1009����
//�����ļ����͹���

void Fmgr_SetFilter(E_Mex_Fmgr_Filtertype filter);

//sdk >= 1009����
//��ʾ�ļ��������
void Fmgr_Show(void);


void Pen_Block(void);

void Pen_unBlock(void);

void KeyPad_Lock(void);	

void KeyPad_unLock(void); 

void ReStartKeyPadLock(void);


//����code cache
//����1 [IN]: start_addr ��ʼ��ַ
//����2 [IN]: total_size ����δ�С
//����3 [OUT]: cache_size ����Ĵ�С
//����: cache_id ������,0Ϊ��Ч��ţ�����ֵΪ��Ч��cache_id
int32 SetROCache(uint32 start_addr, uint32 total_size, uint32 *cache_size);

//ȡ��code cache
//����1 : cache_id ������
//����: �Ƿ�ɹ�
bool ClearROCache(int32 cache_id);

//��icon id��Ӧ��״̬��ͼ���޸ĵ�ָ����image
void SetSystemStatusIcon_ext(int32 icon_id, int32 image_id);


//����icon�Ƿ���ʾ
void SetSystemStatusIconStatus_ext(int32 icon_id, bool bShow);

// rgb565 ת��Ϊ420
int Rgb565ToYuv420(unsigned int *srcYUV, 
                              unsigned char *dstY, unsigned char *dstU, unsigned char *dstV, 
                              int width, int height);

//���õ�ǰϵͳʱ�� 
//ע������dayIndex �ֶ���Ч
bool SetDateTime( Mex_DateTime DateTime );

//ͨ�������ļ�ȡ��Ӧ�ó���ʹ���ڴ�����ֵ
//����1��szFileDir���ļ�Ŀ¼��
//����2��szFileName���ļ���
//����3��bInSystemDir���Ƿ���systemĿ¼
//����ֵ��Ӧ�ó���ʹ���ڴ�����ֵ
int32 GetAppMaxMemSize(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir);

//ȡ��ָ���ڴ��ʣ��Ŀ����ڴ�
//����1��eMemoryPoolType���Ŀ��ڴ��
//����ֵ��ָ���ڴ��ʣ��Ŀ����ڴ�
int32 GetLeftMemSize(E_Mex_MemoryPool eMemoryPoolType);

//�жϵ�ǰӦ�ó����Ƿ�ʹ�õ������ڴ�
//����1��Ӧ�ó���Id
//����ֵ��true����ǰӦ�ó���ʹ�õ������ڴ棻false����ʹ��
bool IsUsingReservedMem(int32 iAppId);

//ͨ�������ļ�ȡ��Ӧ�ó����ǲ���internal or kernal
//����1��szFileDir���ļ�Ŀ¼��
//����2��szFileName���ļ���
//����3��bInSystemDir���Ƿ���systemĿ¼
//����ֵ: true��Ӧ�ó�����
bool IsKernalApp(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir);

//��ǰӦ���Ƿ���internalӦ��
bool IsAppInternal(int32 iAppId);

bool SetWidgetTop(bool bEnable, bool bTop);

bool SetWidgetVisiable(int32 iAppId, bool bVisiable);

//void BeginWidgetPaint(void);

//void EndWidgetPaint(void);

void* GetGdiLayer2Buf(int32* pMemSize);

void ReturnGdiLayer2Buf(void);

//����Ӧ�ó�������ȼ�
//����1��iAppId��Ӧ�ó���id
//����2��iPriority�����ȼ�
//����ֵ: true�����óɹ���false������ʧ��
bool SetAppPriority( int32 iAppId, int32 iPriority );

//����widgetӦ���Ƿ����key�¼�
//����1��iAppId��Ӧ�ó���id
//����2��bAcccept��true������Ӧ����Ҫ����key�¼���false������Ӧ�ý�������key event
//����ֵ: true�����óɹ���false������ʧ��
bool AllowAcceptKey( int32 iAppId, bool bAcccept );
bool GetAppFrameBuffer( int32 iAppId, uint16 **ppBuffer, int32 * pWidth, int32 * pHeight );
bool SetWidgetFullModeMemSize( int32 iAppId, uint32 size );
void* GetDeskAppImgPro(void);
int32 	SetManCurAppId(int32 iAppId);
E_Mex_RunMode GetRunModeFromAppId(int32 iAppId);
int32 		  AppIsInMemory(int32 iAppId);

#endif

#endif
