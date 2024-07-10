/********************************************************************
	FILE:	
		OEMOS.h
	DESCRIPTION:

	AUTHOR:		
		
	DATE:
		
*********************************************************************/

#ifndef OEMOS_H
#define OEMOS_H


#if defined(__cplusplus)
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//		system definition
#define  SUCCESS              0        // No error
#define  EFAILED              1        // General failure

#define APPLE_OS_EXTENSION_BEGIN 0x7fff

#define APPLE_APPMAN_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 1000)
#define APPLE_VP_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 2000)
#define APPLE_BOOK_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 3000)
#define APPLE_GAME_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 4000)
#define APPLE_MP_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 5000)
#define APPLE_OEM_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 6000)
#define APPLE_RP_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 7000)
#define APPLE_FLVPLAYER_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 8000) 
#define APPLE_SPREADTRUM_BEGIN (APPLE_OS_EXTENSION_BEGIN + 10000)
#define APPLE_UMVCHAT_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 13000) 
#define APPLE_MINIAPPMAN_EX_BEGIN (APPLE_OS_EXTENSION_BEGIN + 14000)
//--------------------------------------------------------------------------

#define APPLE_OS_API_IMEI 2
#define APPLE_OS_API_PLATFORM (0x7fff+1)
#define APPLE_OS_API_MODEL (0x7fff+2)
#define APPLE_OS_API_APN (0x7fff+3)
#define APPLE_OS_API_YUV_SUPPORT (0x7fff+4)
#define APPLE_OS_API_VDO_PATH (0x7fff+7)
#define APPLE_OS_API_VDO_SYNC (0x7fff+8)
#define APPLE_OS_API_DEVICECAPABILITY (0x7fff+9)
#define APPLE_OS_API_LCDWIDTH (0x7fff+10)
#define APPLE_OS_API_LCDHEIGHT (0x7fff+11)
#define APPLE_OS_API_LCDROTATE (0x7fff+12)
#define APPLE_OS_API_DEFAULT_APN (0x7fff+13)
#define APPLE_OS_API_MP_LIMITATION (0x7fff+17)
#define APPLE_OS_API_SOFTUSB (0x7fff+18)
//#define APPLE_OS_API_LCDDMA (0x7fff+19)
#define APPLE_OS_API_APPMAN_MODE (0x7fff+20)
#define APPLE_OS_API_APPMAN_GO (0x7fff+21)
#define APPLE_OS_API_APPMAN_MENUITEM (0x7fff+22)
#define APPLE_OS_API_VP_KEY (0x7fff+23)
#define APPLE_OS_API_MP23_START_ENTRY (0x7fff+24)
#define APPLE_OS_API_SCR_ROTATE (0x7fff+25)
#define APPLE_OS_API_ASYNC_FPS (0x7fff+26)
#define APPLE_OS_API_MIN_FPS (0x7fff+27)
#define APPLE_OS_API_VP_FULLSCREEN_R (0x7fff+28)
#define APPLE_OS_API_VP_HELP (0x7fff+29)
#define APPLE_OS_API_SYS_DIR (0x7fff+30)

//--------------------------------------------------------------------------
#define APPLE_OS_API_VP_HCODEC (APPLE_VP_EX_BEGIN + 0)//是否支持264解码
#define APPLE_OS_API_VP_SPMENU (APPLE_VP_EX_BEGIN + 1)//是否隐藏VP的SP菜单
#define APPLE_OS_API_VP_MANUAL_UPDATE (APPLE_VP_EX_BEGIN + 2)//是否手动更新list 
#define APPLE_OS_API_VP_SETCONFIG (APPLE_VP_EX_BEGIN + 4)//获取vp设置选项，如最大缓存，默认缓存等
#define APPLE_OS_API_VP_SUPPORTSVGIV     (APPLE_VP_EX_BEGIN + 6)//是否支持SVGIV
#define APPLE_OS_API_VP_15x			(APPLE_VP_EX_BEGIN + 14)
#define APPLE_OS_API_VP_NewEE		(APPLE_VP_EX_BEGIN + 16)


#define APPLE_OS_API_VP_NoBrans    (APPLE_VP_EX_BEGIN + 33)//是否去除品牌标识，1是，0否，默认0；


//--------------------------------------------------------------------------
#define APPLE_OS_API_MP_SIM (APPLE_MP_EX_BEGIN + 0)//SIM是否有效
#define APPLE_OS_API_MP_SHOWURL (APPLE_MP_EX_BEGIN + 1)//是否显示URL
#define APPLE_OS_API_MP_SHOWAPP (APPLE_MP_EX_BEGIN + 2)//是否显示游戏乐园 
#define APPLE_OS_API_MP_PLATFORM (APPLE_MP_EX_BEGIN + 3)//平台标志，如07A，08B等
#define APPLE_OS_API_MP_MAXVOLUME (APPLE_MP_EX_BEGIN + 4)//最大音量 
#define APPLE_OS_API_MP_APPLECBCALL (APPLE_MP_EX_BEGIN + 5)//系统回调APPLE
#define APPLE_OS_API_MP_SETTING (APPLE_MP_EX_BEGIN + 9)//设置bit0：海外菜单提示
#define APPLE_OS_API_MP_FACEDATA (APPLE_MP_EX_BEGIN + 10)
//--------------------------------------------------------------------------
#define APPLE_OS_API_GAME_CLOCK (APPLE_GAME_EX_BEGIN + 0)//游戏时钟频率

//--------------------------------------------------------------------------
#define APPLE_OS_API_CORE_SIM1 (APPLE_OEM_EX_BEGIN + 0)//SIM1卡状态(0xffff0000 & rv )的值是营运商，0移动，1联能，2无效;(0x0000ffff & rv)的值是1时卡是有效状态（非飞行模式）
#define APPLE_OS_API_CORE_SIM2 (APPLE_OEM_EX_BEGIN + 1)//SIM2卡状态
#define APPLE_OS_API_CORE_MEMSOUND (APPLE_OEM_EX_BEGIN + 2)//是否支持内存播放声音
#define APPLE_OS_API_CORE_KEYPAD (APPLE_OEM_EX_BEGIN + 3)//是否有键盘
#define APPLE_OS_API_CORE_TOUCHPAD (APPLE_OEM_EX_BEGIN + 4)//是否支持触模屏
#define APPLE_OS_API_CORE_MOTIONSENSOR (APPLE_OEM_EX_BEGIN + 5)//是否支持传感器
#define APPLE_OS_API_CORE_SIM_SELECT (APPLE_OEM_EX_BEGIN + 6)//联网或发短信选取SIM卡，0 默认卡，1 SIM1，2 SIM2。目前先不实现。
#define APPLE_FEE_CHANNEL (APPLE_OEM_EX_BEGIN + 7) //判断是否需要龙旗平台的接口: 0 普通渠道     1 龙旗渠道
#define APPLE_CURRENT_IMSI (APPLE_OEM_EX_BEGIN + 8) //取IMSI字符串
#define APPLE_CURRENT_OPERATOR (APPLE_OEM_EX_BEGIN + 9) //运营商判断:0 中国移动  1中国联通  2中国电信
#define APPLE_CURRENT_RESPONSE (APPLE_OEM_EX_BEGIN + 10)
#define APPLE_OS_LCD_FASTUPDATE (APPLE_OEM_EX_BEGIN + 11)//替换老的APPLE_OS_API_LCDDMA
#define APPLE_OS_RELEASE_HW_VER (APPLE_OEM_EX_BEGIN + 13)  //子id
#define APPLE_OS_RELEASE_BUILD_DATETIME (APPLE_OEM_EX_BEGIN + 14) // 编译时间
#define APPLE_OS_API_CORE_KEYPAD (APPLE_OEM_EX_BEGIN + 3) 
#define APPLE_OS_PLATFORMCAP (APPLE_OEM_EX_BEGIN + 31)    //是否支持输入法，MTK定义如下：

#define APPLE_SMS_CONVERTER (APPLE_OEM_EX_BEGIN + 32) // 短信转换函数
#define APPLE_OS_SUPPORTNEWAA_API (APPLE_OEM_EX_BEGIN + 35)//是否支持电话模式 

#define APPLE_OS_APPSMSBLACKLIST (APPLE_OEM_EX_BEGIN + 20)//应用的屏蔽SMS号码
#define APPLE_OS_API_MP_PCMSETTING		(APPLE_MP_EX_BEGIN + 23)//设置pcm声道，采样率
#define APPLE_OS_APPVOICERECORD_API (APPLE_OEM_EX_BEGIN + 24)//应用的录音接口 
#define APPLE_OS_DCSWITCH (APPLE_OEM_EX_BEGIN + 28)
#define APPLE_OS_JPEGCODEC (APPLE_OEM_EX_BEGIN + 40)
#define APPLE_OS_SMSCENTER (APPLE_OEM_EX_BEGIN + 50)//返回短信中心号码
#define APPLE_OS_SET_PS_MODE (APPLE_OEM_EX_BEGIN + 51)
#define APPLE_OS_SPT_JPEGBUFFSIZE (APPLE_OEM_EX_BEGIN + 64)
#define APPLE_OS_USELCDROTATE			(APPLE_OEM_EX_BEGIN + 65) //是否支持LCD ROTATE,0:不支持,1:支持,默认支持
#define APPLE_OS_UCS2SMS (APPLE_OEM_EX_BEGIN + 55)//ucs2 sms,1:unicode 格式,0或默认是SMSAL_DEFAULT_DCS
#define APPLE_OS_API_VP_GET_SWAPXY (APPLE_VP_EX_BEGIN + 11)//客户端是否需要交换xy坐标，返回0不需要，其他需要（默认）

#define APPLE_OS_API_VP_GetVPRomData (APPLE_VP_EX_BEGIN + 32)//获取rom数据

#define APPLE_OS_API_MP_AVData					(APPLE_MP_EX_BEGIN + 72)//avdata:buf[0]=avdataptr,buf[1]=avdatasize

#define APPLE_OS_API_MP_DPLAYER					(APPLE_MP_EX_BEGIN + 73)//获取系统播放器接口
#define APPLE_OS_API_MP_SupportFlv				(APPLE_MP_EX_BEGIN + 74)//是否支持flv, 返回：1，支持；0，不支持


// 能力与接口定义
  #define APPLE_OS_PHONEBOOKAPI (APPLE_OEM_EX_BEGIN + 62) //取电话本 


  #define APPLE_OS_API_MP_MediaLeftData			(APPLE_MP_EX_BEGIN + 68)//展讯media底层buf剩余数据

#define APPLE_OS_DISP					(APPLE_OEM_EX_BEGIN + 175)



#define APPLE_OS_API_FILEMGR_NEW					(APPLE_OEM_EX_BEGIN + 188)//一个是拓展的枚举文件的

//--------------------------------------------------------------------------
#define APPLE_APPMAN_DEFAULT_GAMELIST (APPLE_APPMAN_EX_BEGIN + 0)//APPMAN 默认推荐游戏
#define APPLE_APPMAN_SUPPORT_UNLOAD (APPLE_APPMAN_EX_BEGIN + 1)//APPMAN 支持卸载
#define APPLE_APPMAN_ALWAYS_BACKLIGHT (APPLE_APPMAN_EX_BEGIN + 3)//下载过程中是否长亮背光
#define APPLE_MINIAPM_CHECK_SPACE (APPLE_MINIAPPMAN_EX_BEGIN + 1)//是否要检测space
#define APPLE_MINIAPM_ALWAYS_BACKLIGHT (APPLE_MINIAPPMAN_EX_BEGIN + 2)//是否持续背光 
#define APPLE_MINIAPM_GET_ID_INFO  (APPLE_OEM_EX_BEGIN + 173)//用来做mini appman的下载来源识别

#define APPLE_MINIAPM_NO_LOAD_WAP_PROFILE (APPLE_MINIAPPMAN_EX_BEGIN + 10) //不去读取存储的接入点，直接枚举,1不读取
//--------------------------------------------------------------------------
#define APPLE_OS_APPUSEBLOCK_MEM (APPLE_SPREADTRUM_BEGIN + 0)

#define APPLE_OS_API_RP_SUPPORTAUDIO (APPLE_RP_EX_BEGIN + 5)//获取Rp所支持的audio类型
//bit0:amr,0:不支持，1:支持
//bit1:mp3,0:不支持，1:支持
//bit2:aac,0:不支持，1:支持

#define APPLE_OS_API_RP_RecordFolder  (APPLE_RP_EX_BEGIN + 7)//获取rp录音时保存相对路径（rp判定保存到手机还是T卡），默认保存到my music目录

#define APPLE_UMVCHAT_APN (APPLE_UMVCHAT_EX_BEGIN + 1)//默认接入点（cmwap或其他接入点名称），无配置则默认枚举cmnet
#define APPLE_UMVCHAT_MULTIP (APPLE_UMVCHAT_EX_BEGIN + 2)//群聊房间内最多支持下行视频数，默认25上3路下行，35上5路下行 
#define APPLE_UMVCHAT_HOTKEY (APPLE_UMVCHAT_EX_BEGIN + 3)//快捷键及说明，返回如下结构体信息，如果不支持，则使用默认值
#define APPLE_UMVCHAT_RecordKeyTip (APPLE_UMVCHAT_EX_BEGIN + 4)//录音键提示,如果不支持，不给提示
#define APPLE_UMVCHAT_USETH (APPLE_UMVCHAT_EX_BEGIN + 5)//是否支持线程模式,0：不支持，其他支持，默认支持
#define APPLE_UMVCHAT_SUPCALL (APPLE_UMVCHAT_EX_BEGIN + 6)//是否支持呼叫手机号码,0：不支持，其他支持，默认支持
#define APPLE_UMVCHAT_LargeV (APPLE_UMVCHAT_EX_BEGIN + 7)//高清模式是否使用大分辨率，1：使用，0：不使用，默认使用，仅25有效
#define APPLE_UMVCHAT_DEFAULT_VIDEOCNT (APPLE_UMVCHAT_EX_BEGIN + 8)    //多人视频聊天时，默认打开的下行视频路数 
#define APPLE_UMVCHAT_DEFAULT_SPACETIME (APPLE_UMVCHAT_EX_BEGIN + 9)    //按键的时间间隔,不支持或是取道的时间小于1000毫秒,默认1000毫秒
#define APPLE_UMVCHAT_ChangePowerOn (APPLE_UMVCHAT_EX_BEGIN + 10)    //是否只在poweron的情况下才能旋转，1：poweron状态，0：不需要，默认0
#define APPLE_OS_SUPPORT_MULTI (APPLE_UMVCHAT_EX_BEGIN + 11)    //是否支持多人聊天,0不支持，1支持，默认支持
#define APPLE_OS_SUPPORT_MULTI_ONLYOPENRO (APPLE_UMVCHAT_EX_BEGIN + 12)    //多人聊天时只有房主能够avopen, 1只有房主支持，0全部支持，默认全部支持 
#define APPLE_OS_SUPPORT_HighDef (APPLE_UMVCHAT_EX_BEGIN + 13)    //是否支持高清模式，0不支持，1支持，默认支持
#define APPLE_UMVCHAT_RotateClip (APPLE_UMVCHAT_EX_BEGIN + 14)    //是否需要软件旋转camera数据（展讯），1：需要，其他不需要，默认不需要
#define APPLE_UMVCHAT_SUPFULLSCREEN (APPLE_UMVCHAT_EX_BEGIN + 15)    //是否支持全屏，0不支持，1支持，默认支持
#define APPLE_UMVCHAT_StopVInputMode (APPLE_UMVCHAT_EX_BEGIN + 16)    //在输入法界面是否停止视频，1：停止，0：不停止，默认不停止
#define APPLE_UMVCHAT_MULTIPERIOD (APPLE_UMVCHAT_EX_BEGIN + 17)    //绘制周期倍数，最小1倍，最大5倍，默认为1
#define APPLE_UMVCHAT_ALLOWOPENCAMARA (APPLE_UMVCHAT_EX_BEGIN + 18)    //是否允许打开自己摄像头（视频），1允许，0不允许，默认允许
#define APPLE_UMVCHAT_PreviewSize (APPLE_UMVCHAT_EX_BEGIN + 19)    //返回camera的prevsize，int buf[2],buf[0]=width,buf[1]=height,如果返回>0,则表示preview模式，否则表示正常模式，默认正常模式
#define APPLE_UMVCHAT_UseTcm (APPLE_UMVCHAT_EX_BEGIN + 20)    //是否使用tcm，仅23有效,1:使用，0：不使用，默认使用 
#define APPLE_UMVCHAT_NumAcInput (APPLE_UMVCHAT_EX_BEGIN + 21)    //是否数字键可以激活输入法，1：可以，0：不可以，默认是1：可以
#define APPLE_UMVCHAT_ROOM_SUP_OPENOWNER (APPLE_UMVCHAT_EX_BEGIN + 22)    //进入固定房间是否打开房主视频，1：打开，0：不打开，默认是1：打开 
#define APPLE_UMVCHAT_AUTORECORD (APPLE_UMVCHAT_EX_BEGIN + 30) // 电话模式的，0不支持，1支持
#define APPLE_UMPlayer_FSC_LeftOrRight      (APPLE_UMPlayer_EX_BEGIN + 1) // 全屏时向左还是向右旋转：1,向左；0，向右，默认向右


#define APPLE_UMVCHAT_SysTextInputSuspend (APPLE_UMVCHAT_EX_BEGIN + 98)		//系统输入法界面是否有被打断，1是， 0否， 默认0
#define APPLE_OS_API_VP_MediaSupportResume		(APPLE_VP_EX_BEGIN + 42)//media是否支持resume，0：不支持，1：支持，默认1
//---------------------------------------------------------------------------------------------------------------
#define APPLE_UMPlayer_EX_BEGIN (APPLE_FLVPLAYER_EX_BEGIN+30)

#define APPLE_UMPlayer_GetHelpInfo      (APPLE_UMPlayer_EX_BEGIN + 3) // 获取平台指定的帮助内容，unicode编码,底层拷贝返回内容到buffer(换行用$分隔),并返回长度

typedef struct
{
	int					iOpCode;//操作码,由客户端传入
	int					iKeyCode;//键值，由平台返回
	unsigned short*		iOpPrompt;//操作提示，由平台返回
}CHotKeyInfo; 
enum EVChatOpCode
{
	EVChatOpCode_Mic1 = 0x0,	//抢麦1,默认按键是AVK_VOLUME_UP
	EVChatOpCode_Mic2,	//抢麦2，默认按键是AVK_VOLUME_DOWN
	EVChatOpCode_FireMsg,	//提取消息,默认按键是AVK_5,描述语是5
	EVChatOpCode_SwitchCam,	//切换摄像头,默认按键是AVK_STAR
	EVChatOpCode_ReduVol,	//降低音量，默认按键是AVK_9
	EVChatOpCode_AddVol,	//增加音量，默认按键是AVK_3
	EVChatOpCode_Count
}; 


typedef unsigned char		uint8;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
// //typedef unsigned long int 	uint64;
// typedef unsigned int		uint;
// 
typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;
typedef unsigned char		boolean;
typedef unsigned char		byte;

#define OEM(api) api
#if defined(FEATURE_BIG_ENDIAN)
#define ConvEndianS(x)  ( (((x)&0xff00)>>8) | (((x)&0x00ff)<<8) )
#define ConvEndianL(x)  ( (((x)&0xff000000)>>24) | (((x)&0x00ff0000)>>8) | (((x)&0xff00)<<8) | (((x)&0x00ff)<<24) )
#define ConvEndianWstr(usPtr, nSize) \
								{	int i=0;	\
									for(;i<((int)nSize/2);i++)	\
									{\
										((uint16*)usPtr)[i]=ConvEndianS(((uint16*)usPtr)[i]); \
									}\
								}
#else
#define ConvEndianS(x) (x)
#define ConvEndianL(x) (x)
#define ConvEndianWstr(usPtr, nSize)
#endif

#ifdef WIN32
#define __align(x) 
#endif

typedef enum
{
	EVpOp_None = 0x0,
		EVpOp_PlayResume,//停止状态则重新播放，暂停状态下resume
		EVpOp_Pause,//播放状态下暂停
		EVpOp_PauseResume,//停止状态则重新播放，播放状态则pause,暂停状态则启动播放
		EVpOp_FullScreen,//全屏切换
		EVpOp_Stop,//停止
		EVpOp_SkipForward,//快进
		EVpOp_SkipBackward,//快退
		EVpOp_VolumeUp,
		EVpOp_VolumeDown,
		EVpOp_PlayNext,
		EVpOp_PlayPrev,
}; 


#define APPLE_EVT_KEY                  0x100 
#define APPLE_EVT_KEY_PRESS            0x101 
#define APPLE_EVT_KEY_RELEASE          0x102 
#define APPLE_EVT_KEY_HELD             0x103 

#define APPLE_GCG_PATH_MAX_LEN 20


//////////////////////////////////////////////////////////////////////////
//	system call
typedef struct
{
	unsigned int appid;		//	app ID
	unsigned int flag;		//	app 标识
	unsigned int scrwidth;	//	app 屏幕宽
	unsigned int scrheight;	//	app 屏幕高

	char appdir[20];		//	app 安装目录
	char appsig[20];		//	app 签名文件
	char appbin[20];		//	app 执行文件
	unsigned int inrom;		//	是否从rom中恢复
	void* pAppData;	//	
}apple_preinstal_appinfo;

typedef struct
{
	uint16   wYear;
	uint16   wMonth;
	uint16   wDay;
	uint16   wHour;
	uint16   wMinute;
	uint16   wSecond;
	uint16   wWeekDay;
} AppleJulianType;

typedef struct
{
    SIGNAL_VARS
    unsigned int op_code; 
    unsigned int data1;
    unsigned int data2;
} apple_msg_struct;

typedef struct  
{
	int32 x;
	int32 y;
}apple_point;

typedef struct 
{
	int16    x,y;
	int16   dx, dy;
} apple_rect;

typedef struct _apple_color
{	
	byte r;
	byte g;
	byte b;
	byte alpha;
} apple_color;

typedef void (* PFNAEntry)(void);
typedef struct _OEM_NA_Info
{
	uint16		usScrBaseID;
	uint16		usScrMaxID;
	char*		pName;
	PFNAEntry	pfNAEntry;
}OEM_NA_Info;

extern unsigned int get_apple_ver(void);
#define APPLE_SYS_VER (get_apple_ver(void))

extern void* APPLE_Entry(void* pParam);
extern int32 APPLE_Init(uint32 iMsgID, uint32 iSysID);
extern void APPLE_Exit(void);
extern uint32 APPLE_Dispatch(void);
extern boolean APPLE_Event(uint16 evt, uint16 wParam, uint32 dwParam);
extern int APPLE_Notify(uint32 clsType, uint32 dwMask, void * pData);
extern int32 APPLE_RegAppInfo(void * pAppInfo);
extern void *APPLE_GetHelper(void);
extern boolean APPLE_Key(uint16 usKey, uint16 usEvt);
extern int32 APPLE_KeyHookOn(uint16 key);
extern void APPLE_KeyHookOff(uint16 key);
extern boolean APPLE_PenHandle(uint16 usEvt, uint16 x, uint16 y);
extern int32 APPLE_BrowseFile(char* filepath);
extern int32 APPLE_StartAppMan(char* pArg);
extern int32 APPLE_StartApp(void *pInfo,  char* pArg);
extern int32 APPLE_StartAppByID(uint32 iAppID, char* pArg);
extern int32 APPLE_StopApp(uint32 iAppID);
extern int32 APPLE_ToFG(void);
extern int32 APPLE_ToBG(void);
extern int32 APPLE_RegClsCB(uint32 iAppID,uint32 cls, uint32 iHandler, void * pcb);
extern uint32 APPLE_GetCurrAppID(void);
extern int g_IsAppleInited;

typedef void (* PFNAENTRY)(void);
extern void OEMNA_InitNa(void);
extern boolean OEMNA_IsSameNa(uint16 usID1, uint16 usID2) ;
extern int32 OEMNA_GetNaIdx( uint16 usID );
extern char* OEMNA_GetNaName( int32 iIdx);
extern PFNAENTRY OEMNA_GetNaEntry( int32 iIdx);
extern uint16 OEMNA_GetNaID( int32 iIdx );
extern int32 OEMNA_GetNaNum( void );

extern void OEMOS_post_win_msg(unsigned int iMsg,unsigned int data1, unsigned int data2);
extern void OEMOS_send_win_msg(unsigned int iMsg, unsigned int data1, unsigned int data2);
extern int16  OEMOS_InitLayer( void ) ;
extern void OEMOS_UnInitLayer( void);

extern void OEMOS_SendOsMsg( unsigned int iMsg, unsigned int data1, unsigned int data2);
extern void OEMOS_ToExitCore( void);
extern void   OEMOS_SignalDispatch( void ) ;
//extern void   OEMOS_CancelDispatch( void ) ;
extern void OEMOS_GetJulianDate(uint32 dwSecs, AppleJulianType *pDate);

extern uint32  OEMOS_InitTimer(void ) ;
extern void OEMOS_UnInitTimer( uint32 iTimer) ;
extern void OEMOS_SetTimer( uint32 iTimer, uint32 nMSecs) ;
extern void OEMOS_StopTimer( uint32 iTimer);
extern int32 OEMOS_CheckMsg(void) ;

//	interaction between apple and oem ui history
extern uint16 OEMOS_GetIdleNaID(void);
extern uint16 OEMOS_GetActiveNaID(void) ;
extern uint16 OEMOS_Switch2Apple(uint16 usFromID) ;
extern uint16 OEMOS_Switch2Oem(uint16 usToID) ;

extern uint16 OEMOS_KeyCodeMap(uint16 sKey);
extern uint16 OEMOS_KeyEvtMap(uint16 sEvt);

/////////////////////////////////////////////////////////////
// APPLE FILE
////////////////////////////////////////////////////////////
extern int OEMFS_GetDriveStatus();
extern int8 OEMFS_GetDrive(uint32 type);
	
///////////////////////////////////////////////////////////
//select sim
//////////////////////////////////////////////////////////

#ifndef APPLE_ON_TARGET
extern uint32 OEMOS_LoadLib(char* pPath);
extern uint8* OEMOS_GetFunc(uint32 hLib, char* pFuncName);
extern void OEMOS_FreeLib(uint32 hLib);
#endif //APPLE_ON_TARGET

extern void OEMNET_RegNetEvt(void);
extern void OEMNET_UnRegNetEvt(void);
extern void OEMGetIMEI(uint8* pBuf, uint32 nBufSize);
extern uint32 OEMOS_GetKeyState(uint32 *r1,uint32 *r2,uint32 *r3);
extern int OEMOS_BRW_ConnectUrl(char* url);
extern uint32 OEM_JulianToSeconds(AppleJulianType* pDate);
unsigned char* OEMOS_GetBinInitCode(void);
extern int OEMOS_ClearScreen(void);
extern int OEMOS_GetLangId(void);
extern char* OEMOS_GetString(unsigned int appid, unsigned int id);
extern void OEM_mmi_idle_key_event_backlight_check(void);
extern void OEMOS_DLoadApp(uint32 iAppid, uint32 iCurrVer);
extern void* OEMOS_GetAppEntryFromRom(unsigned int iAppID, int iBegin, unsigned int *iType,int* pSize);

extern int OEM_GetDeviceInfoEx(int nItem, void *pBuff, int *pnSize );
extern void OEMOS_dbgprintf(const char *format, ...);
extern void OEMOS_AppDbgprintf(const char *format, ...);
extern void OEMOS_HexLog(const unsigned char *data, int datalen);
extern int OEMOS_AccessNvRam(void *nvram_apple, int isWrite);
extern void OEMOS_GetSysIMEI(uint8* pBuf, uint32 nBufSize);
extern int32 OEMOS_GetActiveSim(void);
extern int32 OEMOS_SetActiveSim(int32 iSim);
extern  int OEMOS_Addapn(void);
extern void OEMOS_Delapn();

extern int apple_des3(unsigned char * pKey, unsigned int size_key, 
					  unsigned char *data_in, unsigned int size_in, 
					  unsigned char * data_out, unsigned int* size_out, int iIsEnc);
extern char* apple_get_testdata(int type);

extern void* apple_preapp_get_entry(unsigned int app_id);
extern int apple_preapp_is_in_rom(unsigned int app_id);
extern void apple_select_network_sim(int sim);
extern int apple_sys_get_sim_network(unsigned int *pSim1,unsigned int *pSim2);

extern int CheckRemovableStorage(void);

extern int32 g_apple_active_sim;
extern int OEMOS_GetLCDWidth(void);
extern int OEMOS_GetLCDHeight(void);
extern int OEMOS_GetLCDPitch(void);
extern void *OEMOS_GetLCDFrameBuffer(unsigned int * buf_size);
extern void sxm_get_imsi(char* imsi, unsigned int imsi_len, int sim_id);
extern void OEM_AvFillTbl(int * pTbl, int *pSize);
extern uint32 OEM_GetTimeMS(void) ;

extern int OEMOS_TextCtl_Show(void *pTextCtl);
extern void* OEMOS_TextCtl_Create(char* pTitle,char * pTextBuffer,int nTextMaxSize,int nFlags,int iLeaveNotify,void* callback,void *pUser);
extern void OEMOS_TextCtl_Destroy(void *pTextCtl);
extern int OEMOS_TextCtl_Close(void *pTextCtl);
extern int OEMOS_TextCtl_GetActive(void);
extern void OEMOS_TextCtl_SetActive(int b);
extern void OEMOS_TextCtl_SetString(void *pStr,int len);

extern void OEMDisplay_DrawText(uint16 nFont, const uint16* wstr_ptr,int nChars,
						  int x,int y, apple_rect * prcBackground,uint32 dwFlags);
extern void OEMDisplay_DrawRect(const apple_rect * pRect,uint32 clrFrame, uint32 clrFill, uint32 dwFlags);
extern void OEMDisplay_set_rotate(int r);
extern int OEMDisplay_get_rotate(void);
extern int OEMDisplay_LCD_Update(unsigned char *buf_ptr, int cx,int cy, int xSrc, int ySrc, int cxSrc, int cySrc, int xDst, int yDst);
extern void OEMDisplay_BackLight(int n);
extern void AppleEnableLcdUpdate(void);
extern void AppleDisableLcdUpdate(void);
extern int OEMDisplay_LCD_SetDirection(int iLcd, uint8 direct);
extern void OEMDisplay_DirectDrawImg( uint16 x,  uint16 y, uint16 width, uint16 height,  uint32 *buf_ptr, int rot  );
void OEM_YUVDirectPainterDrawWait(void);

extern void* OEMHEAP_GlobalMalloc(uint32 size);
extern void OEMHEAP_GlobalFree(void* ptr);
extern int OEMHEAP_GlobalSize(void);
extern void* OEMHEAP_MainMemMalloc(void);
extern void OEMHEAP_MainMemFree(void* p);
extern uint32 OEMHEAP_MainMemSize(void);
extern void* OEMHEAP_SecondMemMalloc(void);
extern void OEMHEAP_SecondMemFree(void* p);
extern uint32 OEMHEAP_SecondMemSize(void);
extern void * OEMHEAP_AppMemMalloc(uint32 size);
extern void  OEMHEAP_AppMemFree(void* p);
extern uint32 OEMHEAP_AppMemSize(void);
extern void *   OEMHeap_Malloc(uint32 dwBytes) ;
extern void *	OEM_Heap_Mark(void* pBuf,const char* pFile, int iLine);

#ifdef APPLE_MEM_LEAK

#ifdef WIN32
#define OEM_Heap_Malloc_DBG(size) OEM_Heap_Mark(OEMHeap_Malloc(size),__FILE__,__LINE__)
#else
#define OEM_Heap_Malloc_DBG(size) OEM_Heap_Mark(OEMHeap_Malloc(size),__MODULE__,__LINE__)
#endif	//WIN32

#else 
#define OEM_Heap_Malloc_DBG(size) OEMHeap_Malloc(size)
#endif // APPLE_MEM_LEAK


extern int OEMISP_Open(void);
extern void OEMISP_Close(void);
extern int OEMISP_ConvImg(int iInType, int iInW, int iInH, char* pInBuf,  int iOutType, int iOutW, int iOutH, char* pOutBuf);

extern void OEMCAM_Restore(void);
extern int OEMCAM_PowerOn(void);
extern int OEMCAM_PowerDown(void);
extern int OEMCAM_Init(unsigned short width, unsigned short height);
extern void OEMCAM_SetDataCb(int n, void* funcPtr, void* thisv,int thatv );
extern int    OEMCAM_DataReq(void);
extern void OEMCAM_Switch(void);
extern void  OEMCAM_SetRotate(int );
extern void  OEMCAM_SendCmdWorker(int ,int,int);
extern int AppleJpegDec(int *pType, int iW, int iH, char* pInBuf, int iInLen, char* pOutBuf, int * pOutLen, char* pWorkBuf, int iWorkLen);
extern int AppleJpegEnc(int iType, int iW, int iH, char* pInBuf, int iInLen, char* pOutBuf, int * pOutLen, char* pWorkBuf, int iWorkLen);

extern int8 OEMFS_GetDrive(uint32 type);


extern void AppleScrEntry(void);
extern void AppleScrExit(unsigned short iScrID);
extern void AppleScrInterrupt(void);
extern void AppleToBG(void);
extern void AppleToFG(void);
extern void AppleEntry(void);
extern void AppleMsgInd(void *data);
extern int AppleKeyHandler(unsigned short sKey,unsigned short sType);
extern void AppleTimerExpiredInt(void *pUser);
extern int APPLE_OS_GetPhoneID(unsigned char* pBuf, unsigned int nBufSize);
extern void ApplePenDown(apple_point pos);
extern void ApplePenUp(apple_point pos);
extern void ApplePen(apple_point pos);
extern void ApplePenHeld(apple_point pos);
extern void AppleExit(void);

extern int AppleIsSmsDisplay(char *pn, int mode);//1:删除2:屏蔽3:忽略
extern void AppleSetSmsDisplay(unsigned char *param);
extern uint32 AppleIsRun(void);
extern uint32 AppleVCIsRun(void);
extern int32 AppleIsBG(void);
extern void AppleSysCall(void* p);
extern void sxm_get_imsi(char* imsi,unsigned int imsi_len, int sim_id);

extern int apple_mdi_audio_start_record(int format,  void *data_callback, void *data_this);
extern int apple_mdi_audio_stop_record(void);
extern void OEMMedia_aud_Free_RingBuffer(void);

extern int  OEMCAM_IsSecond();
extern void OEM_wsprintf(uint16 *outstr, int nSize, const uint8 *fmt, ...);
extern uint32 OEMOS_GPRSDataAccountNode_GetSize(void);


extern void OEMNET_ResumeTimer(void);
extern void OEMNET_PauseTimer(void);
extern void OEMNET_SocTimerExpiredInt(void);

extern void OEMTAPI_Init(void);
extern void OEMTAPI_UnInit(void);

extern 	void DelSxmobiDMS(void);
extern  int SXHHttp_IsActive();

unsigned int  APPLE_GetCoreVerLib();
extern uint32  Apple_GetCurStartAPPID();
 void OEMDisplay_FreeRotateBuf();
 void  OEMOS_Delapn();
 void  OEMHeap_Free(void * pBuff);


//////////////////////////////////////////////////////////////////////////////////////////////
//OEM_BITMAP
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef OEM_BITMAP_H
#define OEM_BITMAP_H

#define MAKE_BREW_RGB(r,g,b) ((r >> 3)<<11 | (g>>2)<<5 | (b>>3))

#define gdi_handle uint32
#define U8 uint8
#define U16 uint16
//#define BOOL boolean
#define gdi_color unsigned int

#define GDI_COLOR_FORMAT_1          0   /* 1-bit , black/white color */
#define GDI_COLOR_FORMAT_8          1   /* 8-bit , index color */
#define GDI_COLOR_FORMAT_16            2        /* 16-bit, rgb 565 */
#define GDI_COLOR_FORMAT_24            3        /* 24-bit, B,G,R       8,8,8 */
#define GDI_COLOR_FORMAT_32            4        /* 32-bit, B,G,R,A  8,8,8,8 */

#define GDI_IMAGE_TYPE_INVALID                  0

#define GDI_RESULT uint32

#define GDI_LCD_LAYER_ROTATE_0            0x00
#define GDI_LCD_LAYER_ROTATE_90           0x01
#define GDI_LCD_LAYER_ROTATE_180          0x02
#define GDI_LCD_LAYER_ROTATE_270       0x03

#define GDI_IMAGE_TYPE_BMP 		0x00
#define GDI_IMAGE_TYPE_GIF 		0x01
#define GDI_IMAGE_TYPE_PNG 		0x02
#define GDI_IMAGE_TYPE_JPG 		0x03

extern void OEMBitmap_DrawLine(int x1, int y1, int x2, int y2, apple_color rgb,apple_rect clip_rect);
extern void OEMBitmap_FillRectangle(int x1, int y1, int x2, int y2, apple_color c,apple_rect clip_rect);
extern void OEMBitmap_LCDupdate(apple_rect *prect);
extern uint16 OEMBitmap_GetStringWidth(uint16 nFont, const uint16* str_ptr, uint16 str_len);
extern uint16 OEMBitmap_GetStringHeight(uint16 nFont, const uint16* str_ptr, uint16 str_len);
extern void OEMBitmap_DrawText(uint16 nFont, const uint16* wstr_ptr,int nChars,apple_rect box,apple_rect clip, apple_color text_color);
extern void OEMBitmap_DrawRectangle(int x1, int y1, int x2, int y2, apple_color c,apple_rect clip_rect);

#endif//OEM_BITMAP_H



///////////////////////////////////////////////////////////////////////////////////////////////
// OEMNET
//////////////////////////////////////////////////////////////////////////////////////////////
#ifndef OEMNET_H
#define OEMNET_H



typedef struct 
{	
	long             sockfd;
	int             bStream:1;
	int             nShutdown; 
	int				nLastErr;
	uint32          addr;        /* destination address */
	uint16          port;         /* destination port */
}OEMSocket;

typedef enum 
{
	APPLE_SOCKET_STREAM = 0x0,
	APPLE_SOCKET_DGRAM,
};
typedef struct _OEM_gprs_service_data_account_node
{
	uint32 result;
    uint8 DataAccountName[60];      /* Name of a Data Account */
    uint8 APN[60];    /* GPRS APN */
    uint8 prx_ip[4];		//网络序
    uint16 prx_port;		//网络序
	uint8 id;
} OEM_GPRSDataAccountNode;
typedef struct
{
	int request_id;
	int      nResult;
	uint32   addrs[4];
} OEM_DNSResult;

typedef void (*PFNSOCKETEVENTHANDLER)(unsigned char evt, void* po, void* pUser);
typedef void *                   OEMCONTEXT;

int OEMNet_OpenNetlib(void);
int OEMNet_CloseNetlib(void);
OEMCONTEXT OEMSocket_Open( int8 type,int *err,uint32 nwk_account_id);
int OEMSocket_Connect
(
 OEMCONTEXT            sockd,        /* Socket descriptor */
 uint32                addr,         /* destination address */
 uint16                port          /* destination port */
 );
int OEMSocket_Close(OEMCONTEXT sockd);
int32 OEMSocket_Read
(
 OEMCONTEXT            sockd,      /* socket descriptor */
 byte                 *buffer,     /* user buffer to which to copy data */
 uint32                nbytes,     /* number of bytes to be read from socket */
 int                 *err
 ) ;
int32 OEMSocket_Write
(
 OEMCONTEXT         sockd,      /* socket descriptor */
 const byte        *buffer,     /* user buffer from which to copy data */
 uint32             nbytes,     /* number of bytes to be written to socket */
 int             *err         /* error condition value */
 );
int OEMSocket_Shutdown
(
 OEMCONTEXT            sockd,                       /* Socket descriptor */
 int32                 how
 );

int OEMSocket_getipnumber_byename(const char *host,int request_id, void *pres,uint32 nwk_account_id);

char *OEMSocket_getlocaladdress_string(void);

int OEMSocket_getport(OEMCONTEXT sockd, int local);

char *OEMSocket_getipnumber(OEMCONTEXT sockd, int local);

// int OEMSocket_setsockopt(OEMCONTEXT sockd, int flag, int optval, char **errThrowClassName);

// int OEMSocket_getsockopt(OEMCONTEXT sockd, int flag, int *optval, char **errThrowClassName);

int32 OEMSocket_GetSockName(OEMCONTEXT sockd, uint32 * pAddr, uint16 * pwPort);

int32 OEMSocket_GetPeerName(OEMCONTEXT sockd, uint32 * pAddr, uint16 * pwPort);

PFNSOCKETEVENTHANDLER OEMNet_InstallSocketEventHandler(PFNSOCKETEVENTHANDLER pfnSocketEventHandler);

int32 OEMSocket_SendTo
(
 OEMCONTEXT            sockd,     /* socket descriptor */
 const byte           *buffer,    /* user buffer from which to copy the data */
 uint32                nbytes,    /* number of bytes to be written */
 uint16                flags,     /* data transport options */
 uint32                addr,      /* IP address: network byte order */
 uint16                port,      /* port number: network byte order */
 int                 *err
);

int32 OEMSocket_RecvFrom
(
 OEMCONTEXT        sockd,                         /* socket descriptor */
 byte             *buffer,  /* user buffer from which to copy the data */
 uint32            nbytes,            /* number of bytes to be written */
 uint16            flags,             /* unused */
 uint32           *addr,             /* IP address: network byte order */
 uint16           *port,            /* port number: network byte order */
 int            *err
 );

int OEMSocket_GetLastErr(OEMCONTEXT* sockd);

int32 OEMNET_GetMyIPAddr(void);

void OEMNET_GetAPN(char* outbuffer, int pos);
void OEMNET_RegNetEvt(void);
void OEMNET_UnRegNetEvt(void);

typedef struct
{
   uint32     socket_id;    /* socket ID */
   uint8    event_type;   /* soc_event_enum */
   int     result;
} APPLE_app_soc_notify_ind_struct;

#define APPLE_SOC_EVENT_NONE 0x00
#define APPLE_SOC_EVENT_READ 0x01 /* Notify for read */ 
#define APPLE_SOC_EVENT_WRITE 0x02  /* Notify for write */
#define APPLE_SOC_EVENT_ACCEPT 0x04  /* Notify for accept */
#define APPLE_SOC_EVENT_CONNECT 0x08  /* Notify for connect */
#define APPLE_SOC_EVENT_CLOSE 0x10   /* Notify for close */

int OEMSocket_MapSocNotify(APPLE_app_soc_notify_ind_struct* pAPPLE_inf, void *pOEM_inf);
int OEMNET_MapDNSResult(void* pUser, void* pres);
/* Bearers */
#define OEMNET_SOCKET_BEARER_IP_ANY                  0
#define OEMNET_SOCKET_BEARER_GSM_SMS                 3
#define OEMNET_SOCKET_BEARER_GSM_CSD                 10
#define OEMNET_SOCKET_BEARER_GSM_GPRS                11
#define OEMNET_SOCKET_BEARER_WIFI                    12
#define OEMNET_SOCKET_BEARER_BT                      150
#define OEMNET_SOCKET_BEARER_ANY                     255

int OEMNET_networkAccountGetBearer(uint32 networkAccountID);
int OEMNET_networkAccountGetName(uint32 networkAccountID, char *buf, int buflen);
int OEMNET_networkAccountGetId(int bearer);
int OEMNET_networkAccountGetFirst(void);
int OEMNET_networkAccountGetNext(int networkAccountID);
int OEMNET_networkAccountGetAPN(uint32 networkAccountID, char *buf, int buflen);

extern uint32 OEMOS_GPRSDataAccountNode_GetSize(void);
extern uint32 OEMOS_GPRSDataAccountNode_GetNode(uint32 index);
extern uint32 OEMNET_GetDataAccount(uint32 index, void *pNode);
extern void OEM_get_nitz_nw_name(int *nw);
extern void OEMNET_hungup(void);

extern void OEMNET_SocTimerExpiredInt(void);
extern void APPLE_Socket_Notify(void* inMsg );
extern void APPLE_GetHostByName_Notify(void *inMsg);
extern void OEMNET_GprsActiveFailNotify(void);
extern int32 OEMNET_GetGprsState(void);
extern int32 OEMNET_GprsCheckApn(void);
extern uint8 g_iAppleSocTimerId;

extern int32 gnnwk_account_id;
extern int32 OEMNET_GprsActive(void);
extern void OEMNET_GprsDeactive(void);
extern void OEMNET_ResumeTimer(void);
extern void OEMNET_PauseTimer(void);
extern char *APPLE_GetHostPtr(void* po);

extern BOOL OEM_UTF8ToWStr(const byte * pSrc,int nLen, uint16 * pDst, int nSize);
extern 	int APPLE_getcalltime_sec(void);
extern void OEMOS_pauseMp3(void);
extern void OEMOS_resumeMp3(void);


#endif


/////////////////////////////////////////////////////////////////////////////////////
// OEMFS
////////////////////////////////////////////////////////////////////////////////////
#ifndef OEM_FS_H
#define OEM_FS_H

#define OEMFS_FILENAME_MAX_LENGTH 1024 //256

#define  ENUMFILE_MAX_FILE_NAME  64  //必须与BREW 里的AEE_MAX_FILE_NAME 定义的长度一致


typedef struct
{      
   char        attrib;
   uint32      dwCreationDate;		//File creation date. Elapsed time in seconds since January 6, 1980 00:00:00 GMT.
   uint32      dwSize;
//   char *      pszFile;
   char szFile[OEMFS_FILENAME_MAX_LENGTH ];
} OEMFSInfo;

// file access restrictions
typedef uint32 OEMFSRmtAccessMode;
#define FRAM_READ   0x01
#define FRAM_WRITE  0x02
#define FRAM_LIST   0x04
#define FRAM_DELETE 0x08

#define OEMFS_MODE_READ			0x0001
#define OEMFS_MODE_READWRITE	0x0002
#define OEMFS_MODE_CREATE		0x0004
#define OEMFS_MODE_APPEND		0x0008

typedef boolean (*PFNCHKRMTACCESS)( char *pName, OEMFSRmtAccessMode mode );

/**************************************************************************/
/*               GENERIC FILESYSTEM EXPORTED OPERATIONS                   */
/**************************************************************************/
extern void* OEMFS_Open( const char *szFilename,  int8 nMode) ;
extern int OEMFS_Close( void *pFileHandle) ;
extern uint32 OEMFS_Read(void  *pFileHandle,void *buffer, uint32 nBytes) ;
extern uint32 OEMFS_Write(void *pFileHandle,const void *buffer, uint32 nBytes ) ;
extern int OEMFS_GetFileAttributes( const char *szFilename, void *pInfo) ;
//extern int OEMFS_GetOpenFileAttributes( void* pFileHandle,const char* szFilename, void *pInfo ) ;
extern int  OEMFS_Seek(void *pFileHandle,int8 sType, int32 nPos) ;
extern int32 OEMFS_Tell(void *pFileHandle) ;
extern int OEMFS_Truncate(void *pFileHandle, uint32 nPos) ;
extern int OEMFS_Remove(const char *szFilename) ;
extern int OEMFS_Rename(const char *szSource,const char *szDest) ;
extern int OEMFS_Mkdir(const char *szDir) ;
extern int OEMFS_Rmdir(const char *szDir) ;
extern int OEMFS_Test(const char *szFilename) ;
extern OEMFSInfo * OEMFS_EnumStart(const char *szDir,char isDir) ;
extern int OEMFS_EnumNext(OEMFSInfo * pcxt ) ;
extern int OEMFS_EnumStop( OEMFSInfo * pcxt) ;
extern uint32 OEMFS_SpaceAvail(  void) ;
extern int OEMFS_GetFreeSpaceEx( const char *szPath, uint32 *pdwTotal, uint32 *pdwFree);
extern uint32 OEMFS_SpaceUsed(  void) ;
extern int OEMFS_GetLastError (   void ) ;
extern int OEMFS_GetDirInfo( void *pfm, const char* pszDir, uint16 *pwCount, uint32 *pdwSize);
extern void OEMFS_RegRmtAccessChk( const char **pszDirList, uint32 sizeDirList,PFNCHKRMTACCESS   pfn);
extern int OEMFS_TranslatePath(void* pIShell, const char *szOrigin, char* szFileName);
extern void OEMFS_SetBuffer(void *pFileHandle, int uBufferSize, uint8 *pbBuf/*do not use*/);
//extern int OEMFS_OpenFileDialog(uint8 sel_type, APP_FMGR_FILTER filter, char* start_path, void* callback);
extern uint32 OEMFS_GetFreeSpace(uint32 *pdwTotal);
extern int8 OEMFS_GetDrive(uint32 type);
extern int OEMFS_IsDir(OEMFSInfo *pfile);
extern unsigned char* OEM_ReadFile(unsigned char* filename, unsigned int *pLen,unsigned int iStart, unsigned int iEnd);
extern uint32 apple_createfile(uint16 *szUCS2Filename);

extern void OEM_FillFileExtAPI(void *p);


void AppleUpdateApp_SMD(unsigned int iAppId);

static int SMDTransferHighlight2Index(unsigned int SMD_highlight_item);
int SXHHttp_IsActive();


extern int stricmp (const char *a, const char *b);

extern 	int OEM_wstrlen(const uint16 *p);
extern 	boolean OEM_WStrToUTF8(const uint16 * pSrc,int nLen, byte * pDst, int nSize);
extern void OEMOS_ReleaseModule(void);
extern void SXMOBI_MMIAUDIO_Resume();
extern void SXMOBI_MMIAUDIO_Pause();
extern void OEMOS_CleanCache();


#if defined(__cplusplus)
}
#endif
#endif // OEMFS 
#endif /* OEMOS_H */
