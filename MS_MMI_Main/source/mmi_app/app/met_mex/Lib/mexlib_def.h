/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexLib_Def.h
* 文件标识：
* 内容摘要： //定义平台各种类型、枚举
*
************************************************************************/

#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_DEF_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_DEF_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
#define __MEXLIB_DEF_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
#define __MEXLIB_DEF_CLIENT_H__
#endif

#if !defined(__MEXLIB_H__)
	#error Please Include MexLib.h instead of MexLib_Def.h!!!
#endif

#ifdef MEX_PLATFORM_SPREAD
    typedef signed long int   int32;
    typedef unsigned long int uint32;
#else
    typedef signed int        int32;
    typedef unsigned int      uint32;
#endif

typedef void*			funcptr;
typedef signed short	int16;
typedef unsigned short	uint16;
typedef signed char		int8;
typedef unsigned char	uint8;
typedef unsigned char	byte;
#if !defined(MEX_RVCT_4_0_CPP)

#if defined(_MSC_VER)
#if (_MSC_VER < 1400) || !defined(__cplusplus)
typedef unsigned short	wchar_t;
#endif
#else
typedef unsigned short	wchar_t;
#endif

#endif /* !MEX_RVCT_4_0_CPP */
typedef unsigned int	size_t;

// mex color type
typedef	uint16          mex_color;

#ifndef __cplusplus
typedef unsigned char	bool;
#endif

#define true			(1)
#define false			(0)

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifndef TRUE
#define TRUE			(1)
#define FALSE			(0)
#endif
#endif
#ifndef NULL
#define NULL		    (0)
#endif

#ifdef __cplusplus
#define EXTERN	extern "C"
#else
#define EXTERN	extern
#endif

#ifdef __cplusplus
#define MEX_DECLARE_NEW_DELETE	\
	void *operator new(uint32 sz)	{ return malloc(sz); } \
	void operator delete(void *p)	{ free(p); }
#else
#define MEX_DECLARE_NEW_DELETE
#endif

#if defined(MEX_PLATFORM_MTK) || defined(MEX_PLATFORM_SPREAD)
#define MEX_EXPORT EXTERN
#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_WIN32) || defined(MEX_PLATFORM_SIMULATOR)
#define MEX_EXPORT EXTERN __declspec( dllexport )
#endif

//max file path length
#define MEX_FILE_NAME_MAX_LENGTH			100			//like "D:\MexApp\HelloWorld\HelloWorld.mex"
#define MEX_SHORT_FILE_NAME_MAX_LENGTH 		30			//like "HelloWorld.mex"


typedef enum
{
	E_MSR_NoError,
	E_MSR_NoMemory,
	E_MSR_NoLib,
	E_MSR_UserError = 50,
	E_MSR_Other = 200,
	E_MSR_MAX
} E_Mex_Startup_Result;

typedef enum
{
	E_MIMR_Ok,
	E_MIMR_NoMemory,
	E_MIMR_NoPermission,
	E_MIMR_UserError = 50,
	E_MIMR_Other = 200,
	E_MIMR_MAX
} E_Mex_InitMode_Result;

//key event 
typedef enum
{
	E_MKE_Up,
	E_MKE_Down,
	E_MKE_Repeat,
	E_MKE_MAX
} E_Mex_KeyEvent;

typedef enum
{
	E_MME_Down,
	E_MME_Move,
	E_MME_Up,
	E_MME_MAX
} E_Mex_MouseEvent;


typedef enum
{
	E_MRM_None,
	E_MRM_Tiny,
	E_MRM_Small,
	E_MRM_Full,
	E_MRM_MAX
} E_Mex_RunMode;

/*MTK hardware version*/
typedef enum
{
	E_MMH_6205B,
	E_MMH_6208,
	E_MMH_6217,
	E_MMH_6218,
	E_MMH_6218B,
	E_MMH_6219,	
	E_MMH_6223,
	E_MMH_6223P,
	E_MMH_6225,
	E_MMH_6226,
	E_MMH_6226D,
	E_MMH_6226M,
	E_MMH_6227,
	E_MMH_6227D,
	E_MMH_6228,
	E_MMH_6229,
	E_MMH_6230,
	E_MMH_6235,
	E_MMH_6235B,
	E_MMH_6238,
	E_MMH_6239,
	E_MMH_6253,
	E_MMH_6253T,
	E_MMH_6268,
	E_MMH_6268A,
	E_MMH_6268T,
	E_MMH_6516,
	E_MMH_UNKNOW_VER
}E_Mex_MTK_HWVER;

/*MTK software version*/
typedef enum
{
	E_MMS_O7B,
	E_MMS_O8A,
	E_MMS_O9A,
	E_MMS_UNKNOW_VER
}E_Mex_MTK_SWVER;

/*SPREAD hardware version*/
typedef enum
{
	E_MSH_6600L,
	E_MSH_UNKNOW_VER
}E_Mex_PHONE_HWVER;

/*SPREAD software version*/
typedef enum
{
	E_MSS_O7B,
	E_MSS_UNKNOW_VER
}E_Mex_SPREAD_SWVER;


/* platform enum */
typedef enum
{
	E_MPP_MTK,
	E_MPP_SPREAD,
	E_MPP_MSTAR,
	E_MPP_UNKNOW_PLFM,
	E_MPP_SPREAD_LITTLE,

    E_MPP_MAX_PLATFORM = 0xF0
}E_Mex_Phone_Platform;

typedef enum
{
	E_MSS_None = 0,
	E_MSS_Idle = -1
}E_Mex_Sys_ScreenId;

typedef enum
{
	E_MPC_DualSim = 0,								//是否支持双卡 返回0表示不支持 1表示支持
	E_MPC_MaxIntMem = 1,							//最大支持内部内存（byte）
	E_MPC_MaxExtMem = 2,							//最大支持外部内存（byte）
	E_MPC_MaxReservedMem = 3,						//最大支持给全屏程序使用的保留内存（byte）
	E_MPC_SystemRootDir = 4,						//系统根目录 返回 wchar_t *   unicode编码
	E_MPC_AppRootDir = 5,							//应用程序根目录 返回 wchar_t *  unicode编码
	E_MPC_Imei = 6,									//手机设备 imei号  如果没有成功获取 返回0   如果成功获取 返回 int8 [16] ascii码字符串
	E_MPC_Master_Service_Type = 7,					//sim1 服务状态 为0 则表示无服务   为1 为服务状态良好
	E_MPC_Slave_Service_Type = 8,					//sim2 服务状态 为0 则表示无服务   为1 则服务状态良好
	E_MPC_Master_Imsi = 9,							//sim1 imsi号 获取失败返回0  获取成功 返回 int8 [17] ascii码字符串
	E_MPC_Slave_Imsi = 10,							//sim2 imsi号 获取失败返回0  获取成功 返回 int8 [17] ascii码字符串
	E_MPC_Master_SmsCent = 11,						//sim1 短信中心号 获取失败返回0  获取成功 返回 int8 [27] ascii码字符串						
	E_MPC_Slave_SmsCent = 12,						//sim2 短信中心号 获取失败返回0  获取成功 返回 int8 [27] ascii码字符串	
	E_MPC_Master_CellInfo = 13,						//sim1 基站信息  返回 结构Mex_Cell_Info的指针   要根据时间戳来判断信息的有效性					
	E_MPC_Slave_CellInfo = 14,						//sim2 基站信息  返回 结构Mex_Cell_Info的指针	要根据时间戳来判断信息的有效性
	E_MPC_Master_PLMN = 15,							//暂不公开
	E_MPC_Slave_PLMN = 16,							//暂不公开
	E_MPC_Master_Sim_Remove = 17,					//sim1 是否被拔掉 返回 1 表示SIM1 被拔掉
	E_MPC_Slave_Sim_Remove = 18,						//sim2 是否被拔掉 返回 1 表示SIM2 被拔掉
	E_MPC_Master_Sim_Valid = 19,						//sim1 是否合法 返回 1 表示SIM1 合法						
	E_MPC_Slave_Sim_Valid = 20,						//sim2 是否合法 返回 1 表示SIM2 合法
	E_MPC_Master_Sim_Service = 21,					//sim1 服务商ID   返回枚举 E_MEX_Net_Service_ID
	E_MPC_Slave_Sim_Service = 22,					//sim2 服务商ID   返回枚举 E_MEX_Net_Service_ID
	E_MPC_PHONE_CODE_VERSION = 23,					//PHONE源代码版本号 返回枚举 E_Mex_MTK_SWVER or E_Mex_SPREAD_SWVER
	E_MPC_Profile = 24,								//获取当前情景模式	返回枚举 E_MEX_Phs_Profile
	E_MPC_RingVolumeLevel = 25,						//获取当前铃声音量	返回uint8  返回 0 - 6 七级						
	E_MPC_BacklightOn = 26,							//获取当前背光状态  0表示背光关闭  1 表示背光开启
	E_MPC_Language = 27,								//获取当前手机语言	返回枚举 E_MEX_Phs_Language
	E_MPC_TimeFmt = 28,								//获取当前时间格式  0 表示12小时制 1表示24小时制
	E_MPC_DateFmt = 29,								//获取当前日期格式	返回枚举E_MEX_Phs_DateFormat
	E_MPC_FlightMode = 30,							//获取当前是否为飞航模式  0为否 1为是
	E_MPC_CurCityName = 31,							//获取当前设置时区城市名称 返回 wchar_t * unicode编码 且和当前语言设置有关系
	E_MPC_TimeZone = 32,								//获取当前设置的时区与GMT时间的差值 ,因为时间差是float型的,所以返回值放大了100倍,用户需自己做相应转换。
	E_MPC_AudPlyRun = 33,							//mp3是否正在播放
	E_MPC_FMRun = 34,								//fm是否正在播放
	E_MPC_CurWPID = 35,								//当前壁纸ID 为0 是自定义壁纸
	E_MPC_CurWPPath = 36,							//自定义壁纸路径	返回int8 *   unicode编码的路径  注意 当E_MPC_CurWPID返回不为0时 该值可能会错误	
	E_MPC_Usb_MassStorage = 37,						//usb是否处于存储状态
	E_MPC_Tcard_State = 38,							//存储卡状态 0 为无存储卡 1为有存储卡
	E_MPC_Tcard_Drive = 39,							//获取存储卡盘符字母 一般为 D,E..... 无存储卡时该返回值无意义
	E_MPC_Tcard_FreeSpace = 40,						//获取存储卡剩余空间  单位 byte
	E_MPC_System_FreeSpace = 41,						//获取系统盘剩余空间  单位 byte
	E_MPC_ClamClose = 42,							//获取是否合盖  返回1 表示当前已合盖 （如果是直板机 永远为0）
	E_MPC_BT_Receiving = 43,							//蓝牙是否处于开启并接收数据状态 0为否 1为是
	E_MPC_MSG_Interrupt = 44,						//当前是否中断 （主要是判断当前是否正在闹铃或来电）
	E_MPC_Screen_Width = 45,							//屏幕的宽
	E_MPC_Screen_Height = 46,						//屏幕的高
	E_MPC_Touch_Supprot = 47,						//是否支持触屏
	E_MPC_KeyPadLock = 48,							//当前键盘锁是否开启
	E_MPC_EarphonePlugged = 49,						//是否插入耳机
	E_MPC_LcdSleep = 50,								//lcd屏是否正在休眠
	E_MPC_Master_NWK_ACCOUNT = 51,					//sim1 联网时的账号
	E_MPC_Slave_NWK_ACCOUNT = 52,					//sim2 联网时的账号
	E_MPC_Screen_AllowBltDbBuf = 53,					//是否允许双缓冲绘画
	E_MPC_SUPPORT_CAMERA_NUM = 54,					//支持摄像头的数目 0 ，1 ，2
	E_MPC_CMAERA_ZOOM_LIMIT = 55,					//摄像头最大缩放数 
	E_MPC_BACK_CAMERA_HORIZONTAL = 56,				//后摄像头是否默认为水平方向的
	E_MPC_CAMERA_PREVIEW_MAINLCD_ROTATE = 57,		//预览屏幕旋转角度
	E_MPC_LCM_MAINLCD_ROTATE = 58,					//主屏幕旋转角度
	E_MPC_PHONEBOOK_READY = 59,                      //PhoneBook是否准备好.返回1表示PhoneBook已经准备好
	E_MPC_PHONEBOOK_ENTRY_INDEX_PTR = 60,            //PhoneBook条目索引数组的指针
	E_MPC_PHONEBOOK_ENTRY_COUNT = 61,                //PhoneBook条目总数
	E_MPC_PHONEBOOK_MEM_PHONE_TOTAL = 62,            //PhoneBook手机容量,返回在手机端可存储电话条目总数
	E_MPC_PHONEBOOK_MEM_PHONE_USED = 63,             //PhoneBook手机容量,返回已使用的条目数量
	E_MPC_PHONEBOOK_MEM_SIM1_TOTAL = 64,             //PhoneBook sim1容量
	E_MPC_PHONEBOOK_MEM_SIM1_USED = 65,              //PhoneBook sim1已使用容量
    E_MPC_PHONEBOOK_MEM_SIM2_TOTAL = 66,             //PhoneBook sim2容量
    E_MPC_PHONEBOOK_MEM_SIM2_USED = 67,              //PhoneBook sim2已使用容量

	E_MPC_MEX_PLATFOEM_STATUS_ICON_ID = 68,			//  系统状态栏 平台图标 id
	E_MPC_MEX_MASTER_QQ_STATUS_ICON_ID = 69,			//  系统状态栏 QQ1图标 id
	E_MPC_MEX_SLAVE_QQ_STATUS_ICON_ID = 70,			//  系统状态栏 QQ2图标 id

	E_MPC_MEX_PLATFOEM_ICON_IMAGE_ID = 71,			// 平台图标图片ID
	E_MPC_MEX_QQ_ONLINE_ICON_IMAGE_ID = 72,			// QQ在线图标图片ID
	E_MPC_MEX_QQ_OFFLINE_ICON_IMAGE_ID = 73,			// QQ离线图标图片ID
	E_MPC_MEX_QQ_MSG_ICON_IMAGE_ID = 74,				// QQ来消息图标图片ID
	E_MPC_MEX_QQ_HIDE_ICON_IMAGE_ID = 75,			// QQ来消息图标图片ID
	E_MPC_MEX_QQ_AWAY_ICON_IMAGE_ID = 76,			// QQ离开消息图标图片ID

	E_MPC_MEX_DEFUALT_FOLDER_PHOTO = 77,				// 默认照片/图片存放路径
	E_MPC_MEX_DEFUALT_FOLDER_EBOOKS = 78,			// 默认电子书存放路径 
	E_MPC_MEX_DEFUALT_FOLDER_VIDEO = 79,				// 默认视频数据存放路径	
	E_MPC_MEX_DEFUALT_FOLDER_TEMP = 80,				// 默认系统临时文件夹
	E_MPC_MEX_DEFUALT_FOLDER_MUSIC = 81,				// 默认MP3存放歌曲路径
	E_MPC_MEX_DEFUALT_FOLDER_AUDIO = 82,				// 默认铃声存放路径


	E_MPC_DualBattery = 83,							//是否支持双电池
	E_MPC_Master_Battery_Status = 84,				//主电池的状态   参考E_MEX_Battery_Status
	E_MPC_Master_Battery_Level = 85,					//主电池的电量
	E_MPC_Slave_Battery_Status = 86,					//副电池状态		参考E_MEX_Battery_Status
	E_MPC_Slave_Battery_Level = 87,					//副电池电量   
	E_MPC_Max_Battery_Level = 88,					//电池的最大电量


	E_MPC_Master_Sim_Signal = 89,					//主卡sim信号 0 到 E_MPC_Max_Sim_Signal 0 为无信号  
	E_MPC_Slave_Sim_Signal = 90,
	E_MPC_Max_Sim_Signal = 91,
	E_MPC_Invaild_Sim_Signal = 92,

	E_MPC_MEX_Widget_AudioPlayer = 93,				//是否支持 桌面播放器
	E_MPC_AudPly_Play_Handle = 94,					// 播放/暂停按钮事件
	E_MPC_AudPly_Stop_Handle = 95,					//	停止按钮事件
	E_MPC_AudPly_Next_Handle = 96,					// 下一曲
	E_MPC_AudPly_Prev_Handle = 97,					// 上一曲
	E_MPC_AudPly_Inc_Vol_Handle = 98,				// 增大音量
	E_MPC_AudPly_Dec_Vol_Handle = 99,				// 减少音量
	E_MPC_AudPly_Fast_Forward_Handle = 100,			// 快进
	E_MPC_AudPly_Rewind_Handle = 101,					// 快退
	E_MPC_AudPly_Cencal_Seek_Handle = 102,			// 取消快进快退

	E_MPC_AudPly_GetInitState = 103,					//获取更新状态	
	E_MPC_AudPly_Is_Stop = 104,						//是否已停止
	E_MPC_AudPly_GetMusicName = 105,					//获取当前歌曲的名字
	E_MPC_AudPly_GetPlayedMs = 106,					//获取当前歌曲已播放毫秒数
	E_MPC_AudPly_GetTotalMs = 107,					//获取当前歌曲总时间
	E_MPC_AudPly_MusicIndex = 108,					//获取当前歌曲在播放列表中的索引号
	E_MPC_AudPly_TotalMusicNum = 109,					//获取当前播放列表中总歌曲数
	E_MPC_AudPly_Volume = 110,						//获取当前音量
	E_MPC_AudPly_Max_Volume = 111,					//获取最大音量

	E_MPC_AudPly_Init = 112,							//初始化播放器

	E_MPC_AudPly_Spectrum_Display = 113,				//是否支持频谱显示
	E_MPC_AudPly_Start_Calc_Spectrum = 114,			//开始计算频谱信息
	E_MPC_AudPly_Stop_Calc_Spectrum = 115,			//停止计算频谱信息
	E_MPC_AudPly_Get_Spectrum = 116,					//获取频谱信息  参数 U8 top_p[16] U8 val_p[16]

	E_MPC_DEFAULT_NW_MODE = 117,
	E_MPC_DEFAULT_NW_SIM = 118,
	
	E_MPC_MexDir_In_Phone = 119,
	E_MPC_PHONE_HW_VERSION = 120,						//PHONE硬件版本号 返回枚举 E_Mex_MTK_HWVER or E_Mex_SPREAD_HWVER
	E_MPC_MexPlatFromVersion = 121,					//获取平台版本号
	E_MPC_MexVenderID = 122,							//渠道号
	E_MPC_MexProjectID = 123,							//项目编号
	E_MPC_MexPhoneUA = 124,							//机型号
	E_MPC_MexMachineID = 125,							//机器编号
	E_MPC_MinFreeSpace = 126,							//平台最少需要的存储卡空间  单位byte
	E_MPC_VideoIsPlaying = 127,						//是否在播放视频
	E_MPC_VideoIsRecing = 128,

	E_MPC_EXTERNALLIB_GUI = 129,						//是否支持GUI外置库

    E_MPC_Master_DummyInCall = 130,                   //sim1是否支持模拟来电 1-支持, 0-不支持
    E_MPC_Slave_DummyInCall = 131,                    //sim2是否支持模拟来电 1-支持, 0-不支持

	E_MPC_MaxIntMem_App = 132,						//内部内存中App内存占用的大小
	E_MPC_MaxIntMem_Med = 133,						//内部内存中Med内存占用的大小

	E_MPC_GetActiveScreenId = 134,					//获取当前screen id
	E_MPC_SysMinUpdateDelay = 135,					//获取系统tick最少等待时间
	E_MPC_ScrWarmUpDelay = 136,						//屏幕暗掉->恢复的等待时间

	E_MPC_CanFileOptionInBackLightOff = 137,			//当背光按掉能否操作文件

	E_MPC_SPECIAL_VERSION = 138,						//取得特殊版本ID, 如qq版本,msn版本等等

	E_MPC_MEX_VERSION = 139,							//获取平台版本号

	E_MPC_MEX_PATCH_VERSION = 140,					//获取平台补丁号

	E_MPC_PauseBgMusicOnLaunch = 141,					//是否在平台启动时暂停背景音乐的播放

	E_MPC_MEX_SDK_VER = 142,							//获取平台SDK版本号(int)
	
#if MEX_SDK_VER >= 1002
    E_MPC_PhnSetWallPaperTypeMask = 143,              //获取手机所支持的壁纸类型
#endif /* MEX_SDK_VER */

#if MEX_SDK_VER >= 1003
		E_MPC_MaxSocNum = 144,						//获取最大支持同时打开的socket数
#endif /* MEX_SDK_VER */

#if MEX_SDK_VER >= 1007
	E_MPC_DeleteScreenHotShutdown = 145,				//mex屏幕被删除时，是否hotshutdown
#endif /* MEX_SDK_VER */

#if MEX_SDK_VER >= 1008
	E_MPC_SUPPORT_ENCODEJPGTOMEM = 146,			//是否支持直接压缩jpg到内存里
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1009
	E_MPC_AnyAlarmActivated = 147,				//是否有闹铃处于激活状态
	E_MPC_CanConfigScreen = 148,					//是否可以配置屏幕大小
#endif

#if MEX_SDK_VER >= 1010
	E_MPC_Master_Have_Missed_Call = 149,			//主卡是否有未接且未读电话
	E_MPC_Slave_Have_Missed_Call = 150,			//副卡是否有未接且未读电话
#endif

#if MEX_SDK_VER >= 1011
	E_MPC_Has_File_Read_Cache = 151,				//文件读写是否需要进行缓存
#endif

#if MEX_SDK_VER >= 1012
	E_MPC_DCM_Address = 152,                      // DCM (dynamic code memory) Address [TCM (tightly coupled memory)] (uint8 *)
	E_MPC_DCM_Length = 153,                       // DCM (dynamic code memory) Length (uint32)
	E_MPC_Sram_Address = 154,                     // Sram Address (uint8 *)
	E_MPC_Sram_Length = 155,                      // Sram Length (uint32)
	E_MPC_Sram_Left_Size = 156,                   // Sram 当前剩余大小(uint32)
	E_MPC_Sram_Max_Alloc_Size = 157,              // Sram 当前可申请的最大值(uint32)
	E_MPC_Mdi_Support_Mid_Play_File = 158,		// Mex Mdi 是否支持循环播放mid文件
	E_MPC_Mdi_Support_Wav_Play_File = 159,		// Mex Mdi 是否支持循环播放Wav文件
	E_MPC_Mdi_Support_Mp3_Play_File = 160,		// Mex Mdi 是否支持循环播放Mp3文件
	E_MPC_SwitchStackToRun_Funcptr = 161,         // uint32 INT_SwitchStackToRun(void *stack_start, uint32 stack_size, funcptr func, uint32 argc, ...)
    E_MPC_Mix_Control_Interface = 162,            //混音插件控制接口
#endif


#if MEX_SDK_VER >= 1013
    E_MPC_SleepTime_For_SoundPlay = 163,          //sound play 时系统sleeptime
#endif

#if MEX_SDK_VER >= 1017
	E_MPC_MSN_ICON_ID = 164,
	E_MPC_MSN_IMAGE_ONLINE_ID = 165,
	E_MPC_MSN_IMAGE_OFFLINE_ID = 166,	
	E_MPC_MSN_IMAGE_AWAY_ID = 167,	
	E_MPC_MSN_IMAGE_BUSY_ID = 168,	
	E_MPC_MSN_IMAGE_MSG_ID = 169,	
	E_MPC_MSN_IMAGE_IDLE_ID = 170,	
#endif

//#if MEX_SDK_VER >= 1018                   //支持三卡功能
	E_MPC_Third_NWK_ACCOUNT = 171,					//sim1 联网时的账号
	E_MPC_Third_Service_Type = 172,					//sim3 服务状态 为0 则表示无服务   为1 为服务状态良好
	E_MPC_Third_Imsi = 173,							//sim3 imsi号 获取失败返回0  获取成功 返回 int8 [17] ascii码字符串
	E_MPC_Third_SmsCent = 174,						//sim3 短信中心号 获取失败返回0  获取成功 返回 int8 [27] ascii码字符串						
	E_MPC_Third_CellInfo = 175,						//sim3 基站信息  返回 结构Mex_Cell_Info的指针   要根据时间戳来判断信息的有效性					
	E_MPC_Third_PLMN = 176,							//暂不公开
	E_MPC_Third_Sim_Remove = 177,				        //sim3 是否被拔掉 返回 1 表示SIM1 被拔掉
	E_MPC_Third_Sim_Valid = 178,						//sim3 是否合法 返回 1 表示SIM1 合法						
	E_MPC_Third_Sim_Service = 179,					//sim3 服务商ID   返回枚举 E_MEX_Net_Service_ID
    E_MPC_PHONEBOOK_MEM_SIM3_TOTAL = 180,             //PhoneBook sim3容量
    E_MPC_PHONEBOOK_MEM_SIM3_USED = 181,              //PhoneBook sim3已使用容量
	E_MPC_Third_Sim_Signal = 182,					    //sim3信号 0 到 E_MPC_Max_Sim_Signal 0 为无信号  
//#endif

	E_MPC_Reserved_Start = 0x400,			//预留位 从1024开始
	E_MPC_Reserved_UpdateDelay,
	E_MPC_Reserved_Pre_StartUp,	
	E_MPC_Reserved_Tiny_Loaing_Page,		//Tiny加载页面显示文字背景信息，返回Mex_Config_Loading_Page *
	E_MPC_Reserved_Man_Loaing_Page,			//Man加载页面显示文字背景信息，返回Mex_Config_Loading_Page *
	E_MPC_Phone_Platform,					//取得手机属于何种平台，返回E_Mex_Phone_Platform
	E_MPC_Reserved_Enable_Select_DataAccount, //是否可以让用户自主更改网络接入点
	E_MPC_Reserved_Entry_Select_DataAccount,  //进入网络接入点界面
	E_MPC_Reserved_BreathLed_PowerOn = 0x408,   // 返回函数指针mmi_breath_led_power_on_qq, 函数原型void mmi_breath_led_power_on_qq(void)      
	E_MPC_Reserved_BreathLed_PowerOff = 0x409,  // 返回函数指针mmi_power_off_QQ_breath_led, 函数原型void mmi_power_off_QQ_breath_led(void)
	E_MPC_Download_font_size= 0x40A,
	E_MPC_GB2312_SUPPORT = 0x40B, //支持GB2312,1 not support, 0 support.
	E_MPC_Gprs_sim_card = 0x40C,
    E_MPC_Reserved_Dewav_GetPID = 0x500,        //获取鼎为PID函数指针，函数原型kal_char * release_version_information(void)
    E_MPC_Reserved_HuaQin_GetPID = 0x501,       //获取华勤的PID
    E_MPC_Reserved_Youle_Support = 0x502,       //是否是Youle特殊版本
	E_MPC_Reserved_End	=	0x800,			//预留位到2048结束
#if MEX_SDK_VER >= 1016
	E_MPC_FuncPtrMallocForProgData	= 0x1001,	//获取自定义程序加载的内存函数指针，内部使用
	E_MPC_FuncPtrFreeForProgData	= 0x1002,	//获取自定义程序加载的内存函数指针，内部使用
#endif

#if MEX_SDK_VER >= 1022
	E_MPC_FETION_ICON_ID,
	E_MPC_FETION_IMAGE_ONLINE_ID,
	E_MPC_FETION_IMAGE_OFFLINE_ID, 
	E_MPC_FETION_IMAGE_AWAY_ID,	
	E_MPC_FETION_IMAGE_BUSY_ID,	
	E_MPC_FETION_IMAGE_MSG_ID, 
	E_MPC_FETION_IMAGE_HIDE_ID,	
#endif

#if MEX_SDK_VER >= 1023                   //获取手机支持的sim卡数
    E_MPC_Numbers_Sim,                            
#endif
//#ifdef __MET_MEX_SIM_PLUS__
	E_MPC_Fouth_Service_Type =0x1101,					//sim4 服务状态 为0 则表示无服务   为1 则服务状态良好
	E_MPC_Fouth_Imsi =0x1103,							//sim4 imsi号 获取失败返回0  获取成功 返回 int8 [17] ascii码字符串
	E_MPC_Fouth_SmsCent =0x1105,						//sim4 短信中心号 获取失败返回0  获取成功 返回 int8 [27] ascii码字符串	
	E_MPC_Fouth_CellInfo =0x1107,						//sim4基站信息  返回 结构Mex_Cell_Info的指针	要根据时间戳来判断信息的有效性
	E_MPC_Fouth_PLMN =0x1109,							//暂不公开
	E_MPC_Fouth_Sim_Remove =0x1111,						//sim4 是否被拔掉 返回 1 表示SIM2 被拔掉
	E_MPC_Fouth_Sim_Valid =0x1113,						//sim4是否合法 返回 1 表示SIM2 合法
	E_MPC_Fouth_Sim_Service =0x1115,					//sim4 服务商ID   返回枚举 E_MEX_Net_Service_ID
	E_MPC_Fouth_NWK_ACCOUNT =0x1117,					//sim4 联网时的账号
	E_MPC_PHONEBOOK_MEM_SIM4_TOTAL =0x1120, 			//PhoneBook sim4容量
	E_MPC_PHONEBOOK_MEM_SIM4_USED =0x1121,				//PhoneBook sim4已使用容量
	E_MPC_Fouth_Sim_Signal =0x1123,
    E_MPC_Third_DummyInCall =0x1124,                   //sim1是否支持模拟来电 1-支持, 0-不支持
   	E_MPC_Fouth_DummyInCall =0x1125,                    //sim2是否支持模拟来电 1-支持, 0-不支持
//#endif/*__MET_MEX_SIM_PLUS__*/
	E_MPC_FuncPtrInvoke_Post_Event = 0x1719,
	
#if defined(MEX_SUPPORT_FOREIGN_VERSION)
	E_MPC_PHONE_CHINESE_IS_DEFINED = 0x1720,
#endif
	E_MPC_MAX  
	
} E_Mex_Phone_Config;

typedef enum
{
	E_MMP_Internal,
	E_MMP_External,
	E_MMP_Reserved,
	E_MMP_MAX
} E_Mex_MemoryPool;


//Sys permission type , add before E_MPT_Max
typedef enum
{
	E_MAPT_SmsSend,
	E_MAPT_SmsRead,	
	E_MAPT_SmsDelete,
	E_MAPT_PhonebookAdd,
	E_MAPT_PhonebookRead,
	E_MAPT_PhonebookUpdate,
	E_MAPT_PhonebookDelete,
	E_MAPT_HookSms,
	E_MAPT_HookCall,
	E_MAPT_CallLogRead,
	E_MAPT_CallLogDelete,
	E_MAPT_Charge,
	E_MAPT_SysAnalyse,
    E_MAPT_MakeCall,
    E_MAPT_SmsSave,
    
	E_MAPT_MAX
}E_Mex_App_Permission_Type;

typedef enum
{
	E_MAEIT_Version,
	E_MAEIT_RegistWidth,
	E_MAEIT_RegistHeight,
	E_MAEIT_PermissionL,
	E_MAEIT_PermissionH,
	E_MAEIT_ProductId,
	E_MAEIT_RequireSdkVer,
	E_MAEIT_TargetOption,
	E_MAEIT_MaxMemSize,
	E_MAEIT_Reserved6,
	E_MAEIT_MAX
}E_Mex_App_Extra_Info_Type;

typedef enum
{
	E_MAT_Kernel,
	E_MAT_LibUpdate,
	E_MAT_HandleIcon,
	E_MAT_HandleWidget,
	E_MAT_HandleFull,
	E_MAT_Internal,
	E_MAT_WidgetUseExtMem,
	E_MAT_WidgetNoExitOnSwitch,
	E_MAT_Reserved2,
	E_MAT_Reserved3,
	E_MAT_Reserved4,
	E_MAT_Reserved5,
	E_MAT_MAX
} E_Mex_App_Type;

typedef enum
{
	E_MMMF_Malloc,				//ret vm ptr, size
	E_MMMF_Free,				//ret void, vm ptr
	E_MMMF_Memcpy,				//ret vm ptr, vm ptr, mem ptr, size
	E_MMMF_ToMem,				//ret mem ptr, mem ptr, vm ptr, size
	E_MMMF_PMalloc,				//ret mem ptr, key str, size, balreadyexist
	E_MMMF_PFree,				//ret void, vm ptr

	E_MMMF_RegLaunchOnScreen,	//ret bSuccess, appid, screen id
	E_MMMF_RegLaunchOnStartup,	//ret bSuccess, appid
	E_MMMF_RegLaunchOnFullAppExit,	//ret bSuccess, appid

	E_MMMF_Invalidate,				//ret bAccepted, appid
	E_MMMF_LaunchOnExit,			//ret void, int* launch parameters
	E_MMMF_GetLastFullScreenBuf,	//ret void, pbuf, pwidth, pheight
	E_MMMF_RegistInitParam,			//ret bSuccess, pInitParam
	E_MMMF_SetCurrScreen,			//ret void, iCurrScreen

	E_MMMF_DisableQuitMsg,			//ret bool, appid, bDisable

	E_MMMF_RegisterIntent,		//future use
	E_MMMF_CallIntent,			//future use
	E_MMMF_GetAppPosition,		//ret bIsExist, appid, pPosX, pPosY
	E_MMMF_SetAppPosition,		//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_SetWidgetAlwaysTop,	//ret bIsExist, appid, pPosX, pPosY
	E_MMMF_SetWidgetVisiable,	//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_GetDeskAppImgPro,	//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_SetAppPriority,		//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_AllowAcceptKey,		//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_SetWidgetFullModeMemSize,	//ret bSuccess, appid, memsize	
	E_MMMF_GetFreeMemory,
	E_MMMF_SetManCurAppId,
	E_MMMF_AppIsInMemory,
	E_MMMF_Reserved15,
	E_MMMF_Reserved16,

	E_MMMF_MAX
} E_Mex_Mgr_Manager_Func;

typedef enum
{
	E_MSSS_LeaveStart,
	E_MSSS_LeaveEnd,
	E_MSSS_EnterStart,
	E_MSSS_EnterEnd,
	E_MSSS_MAX
} E_Mex_ScreenSwitch_Step;

typedef enum
{
	E_MMT_SPLIT_APP_SYS_MANAGER_BEGIN	= 0,	//below this line are messages used by app -> sys -> manager

	E_MMT_Mgr_Init,								//params (0-3): /
	E_MMT_Mgr_Update,							//params (0-3): /
	E_MMT_Mgr_Destroy,							//params (0-3): bExitAll
	E_MMT_Mgr_MouseEvent,						//params (0-3): mouse event, x, y
	E_MMT_Mgr_KeyEvent,							//params (0-3): keyEvent, keyCode
	E_MMT_Mgr_GetKeyState, 						//params (0-3): keyCode
	E_MMT_Mgr_SetKeyState, 						//params (0-3): keyEvent, keyCode
	E_MMT_Mgr_SetTimer,							//params (0-3): timer, params, bIsInternal
	E_MMT_Mgr_KillTimer,						//params (0-3): timerid, bIsInternal
	E_MMT_Mgr_SetTimerInterval,					//params (0-3): timerid, iNewTime, pOldTime,bIsInternal
	E_MMT_Mgr_GetRunMode,						//params (0-3): appid
	E_MMT_Mgr_SwitchRunMode,					//params (0-3): appid, newMode
	E_MMT_Mgr_GetRegistry,						//params (0-3): /
	E_MMT_Mgr_AppWantExit,						//params (0-3): appid
	E_MMT_Mgr_AppLaunched,						//params (0-3): appid, internal, insystemdir, apptype
	E_MMT_Mgr_GetAppAttribute,					//params (0-3): appid
	E_MMT_Mgr_SetAppAttribute,					//params (0-3): appid, attribute
	E_MMT_Mgr_PauseTimer,						//params (0-3): appid, timerid, bPause
	E_MMT_Mgr_ManagerFunc,						//params (0-3): function category, param0, param1, param2
	E_MMT_Mgr_PaintPause,						//params (0-3): /
	E_MMT_Mgr_PaintResume,						//params (0-3): /
	E_MMT_Mgr_SRamNeedLock,						//params (0-3): appid
	E_MMT_Mgr_SRamUnlocked,						//params (0-3): appid
	E_MMT_Mgr_UpdateThread,						//params (0-3): /
	E_MMT_Mgr_SetThreadTimer,					//params (0-3): timer, params
	E_MMT_Mgr_KillThreadTimer,					//params (0-3): timerid
	E_MMT_Mgr_SetThreadTimerInterval,			//params (0-3): timerid, iNewTime, pOldTime

	E_MMT_Mgr_CircleDeviceEvent,				//params (0-3): Circle input device, input type(clockwise or not)

	E_MMT_Mgr_DualSimModeChanged,				//params (0-3): Old mode, New Mode (see: enum E_Mex_Dual_Sim_Mode)


	E_MMT_Mgr_KeyPadLockChanged,				//params (0-3):  event(see: enum E_Mex_KeyPad_Lock_Event)

	E_MMT_Mgr_LcdRotate,						//params (0-3): E_Mex_LcdRotateType, newWidth , newHeight	

	E_MMT_Mgr_RecvSms,							//reveive new sms
	E_MMT_Mgr_SlideEvent,						//slide on/off

	E_MMT_Mgr_User						= 50,	//params (0-3): tbd, user messages

	E_MMT_SPLIT_APP_SYS_MANAGER_END		= 60,	//above this line are messages used by app -> sys -> manager


	E_MMT_SPLIT_MANAGER_APP_BEGIN		= 61,	//below this line are messages used by manager -> app

	E_MMT_InitMode,								//params (0-3): new run mode, app return <0 on error
	E_MMT_ExitMode,								//params (0-3): old run mode
	E_MMT_MouseEvent,							//params (0-3): mouse event, x, y
	E_MMT_KeyEvent,								//params (0-3): keyEvent, keyCode
	E_MMT_CharInput,							//params (0-3): character (can be unicode)
	E_MMT_Paint,								//params (0-3): buffer ptr, width, height, Mex_Point*
	E_MMT_Timer,								//params (0-3): timer id, timer parameter
	E_MMT_ScreenSwitch,							//params (0-3): eswitchstep
	E_MMT_ManagerDisplay,						//params (0-3): is now showing
	E_MMT_ManagerShutDown,						//params (0-3): /
	E_MMT_SRamNeedLock,							//params (0-3): appid
	E_MMT_SRamUnlocked,							//params (0-3): appid
	E_MMT_Destroy,								//params (0-3): /
	E_MMT_PaintPause,							//params (0-3): /
	E_MMT_PaintResume,							//params (0-3): /
	E_MMT_BackLightOff,							//params (0-3): /
	E_MMT_BackLightOn,							//params (0-3): /
	
	E_MMT_CircleDeviceEvent,					//params (0-3): Circle input device, input type(clockwise or not)

	E_MMT_DualSimModeChanged,					//params (0-3): Old mode, New Mode (see: enum E_Mex_Dual_Sim_Mode)

	E_MMT_KeyPadLockChanged,					//params (0-3):  event(see: enum E_Mex_KeyPad_Lock_Event)

	E_MMT_LcdRotate,							//params (0-3): E_Mex_LcdRotateType, newWidth , newHeight
	E_MMT_AppDumpIn,							//params (0-3): /						
	E_MMT_AppDumpOut,							//params (0-3): /
	E_MMT_AppWantExited,							//params (0-3): /
	E_MMT_AppHasExited,							//params (0-3): /
	E_MMT_GetDeskAppImgPro,						//params (0-3): /
	E_MMT_RecvSms,								//reveive new sms
	E_MMT_SlideEvent,							//params(0) 0:Close 1:Open
	E_MMT_User							= 200,	//params (0-3): tbd, user messages

	E_MMT_SPLIT_MANAGER_APP_END			= 250,	//above this line are messages used by manager -> app

	E_MMT_MAX
} E_Mex_MessageType;

typedef enum
{
	E_MLRT_Rotate_0,
	E_MLRT_Rotate_90,
	E_MLRT_Rotate_180,
	E_MLRT_Rotate_270,
	E_MLRT_Rotate_max
}E_Mex_LcdRotateType;

typedef enum 
{
	E_MKC_KEY_0, 						//按键 0
	E_MKC_KEY_1, 						//按键 1
	E_MKC_KEY_2, 						//按键 2
	E_MKC_KEY_3, 						//按键 3
	E_MKC_KEY_4, 						//按键 4
	E_MKC_KEY_5, 						//按键 5
	E_MKC_KEY_6,						//按键 6
	E_MKC_KEY_7, 						//按键 7
	E_MKC_KEY_8, 						//按键 8
	E_MKC_KEY_9, 						//按键 9
	E_MKC_KEY_SOFTLEFT, 				//按键 左软键
	E_MKC_KEY_SOFTRIGHT,				//按键 右软键
	E_MKC_KEY_ENTER,					//按键 确认/选择
	E_MKC_KEY_UP,						//按键 上
	E_MKC_KEY_DOWN, 					//按键 下
	E_MKC_KEY_LEFT, 					//按键 左
	E_MKC_KEY_RIGHT,					//按键 右
	E_MKC_KEY_SEND, 					//按键 接听键
	E_MKC_KEY_END,						//按键 挂机键
	E_MKC_KEY_CLEAR,					//按键 清除键
	E_MKC_KEY_STAR, 					//按键 *
	E_MKC_KEY_POUND,					//按键 #
	E_MKC_KEY_VOLUME_UP, 				//按键 侧键上
	E_MKC_KEY_VOLUME_DOWN, 				//按键 侧键下
	E_MKC_KEY_OTHER,					//其他按键
	E_MKC_KEY_EXTRA_1,
	
	E_MKC_KEY_A,                        //添加对全键盘的支持
	E_MKC_KEY_B,
    E_MKC_KEY_C,
    E_MKC_KEY_D,
	E_MKC_KEY_E,
	E_MKC_KEY_F,
	E_MKC_KEY_G,
	E_MKC_KEY_H,
	E_MKC_KEY_I,
	E_MKC_KEY_J,
	E_MKC_KEY_K,
	E_MKC_KEY_L,
	E_MKC_KEY_M,
	E_MKC_KEY_N,
	E_MKC_KEY_O,
	E_MKC_KEY_P,
	E_MKC_KEY_Q,
	E_MKC_KEY_R,
	E_MKC_KEY_S,
	E_MKC_KEY_T,
	E_MKC_KEY_U,
	E_MKC_KEY_V,
	E_MKC_KEY_W,
	E_MKC_KEY_X,
    E_MKC_KEY_Y,
    E_MKC_KEY_Z,
    E_MKC_KEY_SPACE,
    E_MKC_KEY_TAB,
    E_MKC_KEY_DEL,
    E_MKC_KEY_ALT,
    E_MKC_KEY_CTRL,
    E_MKC_KEY_WIN,
    E_MKC_KEY_SHIFT,
    E_MKC_KEY_QUESTION,
    E_MKC_KEY_PERIOD,
    E_MKC_KEY_COMMA,
    E_MKC_KEY_EXCLAMATION,
    E_MKC_KEY_APOSTROPHE,
    E_MKC_KEY_AT,
    E_MKC_KEY_BACKSPACE,
    E_MKC_KEY_QWERTY_ENTER,
    E_MKC_KEY_FN,
    E_MKC_KEY_SYMBOL,
    E_MKC_KEY_NUM_LOCK,
    E_MKC_KEY_QWERTY_MENU,
    E_MKC_KEY_OK,
    E_MKC_MAX_QWERTY_KEYS,
    
    E_MKC_KEY_MAX,
	E_MKC_KEY_INVALID = 0xFE			//非法值
} E_Mex_KeyCode;

typedef enum 
{
	E_MCI_ClockWise,
	E_MCI_AntiClockWise,
	E_MCI_MAX,
	E_MCI_INVALID = 0xFE			//非法值
} E_Mex_CircleInput;


typedef enum
{

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
    // MTK SYSTEM DEFINE
    //从mtk系统常量到mex常量的转换
	// 系统模块ID 通常为task id   可以参考文件 stack_config.h
	E_M2MDC_MOD_NVRAM,			//NVRAM
	E_M2MDC_MOD_SMS,			//SMS
	E_M2MDC_MOD_SMS_2,			//SMS_2
	E_M2MDC_MOD_SIM,			//SIM
	E_M2MDC_MOD_SIM_2,
	E_M2MDC_MOD_L4C,
	E_M2MDC_MOD_L4C_2,
	E_M2MDC_MOD_PHB,
	E_M2MDC_MOD_PHB_2,
	E_M2MDC_MOD_AS,
	E_M2MDC_MOD_AS_2,
	E_M2MDC_MOD_MED,
	E_M2MDC_MOD_MMI,			//MMI

	//task id end  
	//SAP
	E_M2MDC_MMI_L4C_SAP,
	//END
	//按键   可参考文件 GlobalConstants.h
	E_M2MDC_KEY_0,
	E_M2MDC_KEY_1,
	E_M2MDC_KEY_2,
	E_M2MDC_KEY_3,
	E_M2MDC_KEY_4,
	E_M2MDC_KEY_5,
	E_M2MDC_KEY_6,
	E_M2MDC_KEY_7,
	E_M2MDC_KEY_8,
	E_M2MDC_KEY_9,
	E_M2MDC_KEY_UP_ARROW,
	E_M2MDC_KEY_DOWN_ARROW,
	E_M2MDC_KEY_LEFT_ARROW,
	E_M2MDC_KEY_RIGHT_ARROW,
	E_M2MDC_KEY_ENTER,
//#if defined(__MET_MEX_SUPPORT__)&& defined(MEX_G7_UI_SUPPORT)
 //   E_M2MDC_KEY_End,  
//#endif
	E_M2MDC_KEY_STAR,
	E_M2MDC_KEY_POUND,
	E_M2MDC_KEY_LSK,
	E_M2MDC_KEY_RSK,
	//END
	//KEY EVENT
	E_M2MDC_KEY_EVENT_UP,
	E_M2MDC_KEY_EVENT_DOWN,
	//END
	//ProtocolEvents msg

	//end
	//MSG ID
	E_M2MDC_MSG_MMI_CALL_RING_IND,				//MSG_ID_MMI_CC_CALL_RING_IND
	E_M2MDC_MSG_MMI_SMS_DELIVER_IND,			//MSG_ID_MMI_SMS_DELIVER_MSG_IND
	E_M2MDC_MSG_NW_GET_IMEI_REQ,				//MSG_ID_MMI_NW_GET_IMEI_REQ
	E_M2MDC_MSG_NW_GET_IMEI_RSP,				//MSG_ID_MMI_NW_GET_IMEI_RSP
	E_M2MDC_PRT_GET_IMSI_REQ,					//PRT_GET_IMSI_REQ
	E_M2MDC_PRT_GET_IMSI_RSP,					//PRT_GET_IMSI_RSP
	E_M2MDC_MSG_EM_START_REQ,					//MSG_ID_MMI_EM_START_REQ
	E_M2MDC_MSG_EM_START_RSP,					//MSG_ID_MMI_EM_START_RSP
	E_M2MDC_MSG_EM_STATUS_IND,					//MSG_ID_MMI_EM_STATUS_IND
	E_M2MDC_MSG_EM_STOP_REQ,					//MSG_ID_MMI_EM_STOP_REQ
	E_M2MDC_GET_GPRS_DATA_ACCOUNT_REQ,			//PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ
	E_M2MDC_GET_GPRS_DATA_ACCOUNT_RSP,			//PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP
	E_M2MDC_SET_GPRS_DATA_ACCOUNT_REQ,			//PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ
	E_M2MDC_SET_GPRS_DATA_ACCOUNT_RSP,			//PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP
	E_M2MDC_MSG_GET_HOST_BY_NAME_IND,			//MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND
	E_M2MDC_MSG_SOC_NOTIFY_IND,					//MSG_ID_APP_SOC_NOTIFY_IND

	//end

	//sms
	E_M2MDC_SMSAL_MAX_MSG_LEN,
	E_M2MDC_SMSAL_MTI_DELIVER,
	E_M2MDC_SMSAL_MTI_SUBMIT,
	E_M2MDC_SMSAL_DEFAULT_DCS,
	E_M2MDC_SMSAL_UCS2_DCS,
	E_M2MDC_SMSAL_DEFAULT_PID,
	E_M2MDC_SMS_APP_NOBOX,
	E_M2MDC_SMS_INVALID_INDEX,
	E_M2MDC_MAX_DIGITS_SMS,
	E_M2MDC_ENCODING_LENGTH,
	E_M2MDC_SMS_SEND_SAVE_NIL,
	E_M2MDC_SEND_CASE_SEND_ONLY,
	E_M2MDC_FRM_SMS_OK,							//MMI_FRM_SMS_OK
	//end
	//ENCODE TYPE
	E_M2MDC_ENCODE_ASCII,
	E_M2MDC_ENCODE_UNICODE,
/*	E_M2MDC_ENCODE_H_ASCII,
	E_M2MDC_ENCODE_H_UNICODE,
*/
	//END
	//INPUT TYPE   参考 wgui_categories_inputsenum.h
	E_M2MDC_INPUT_NUMERIC, 				
	E_M2MDC_INPUT_ALPHANUMERIC_UPPERCASE,
	E_M2MDC_INPUT_ALPHANUMERIC_LOWERCASE,
	E_M2MDC_INPUT_ALPHANUMERIC_SENTENCECASE,
	E_M2MDC_INPUT_NUMERIC_PASSWORD, 		
	E_M2MDC_INPUT_ALPHANUMERIC_PASSWORD, 
	E_M2MDC_INPUT_SM_PINYIN,				
	E_M2MDC_INPUT_SM_STROKE,				
	E_M2MDC_INPUT_SM_MULTITAP_PINYIN,	
	//END
	//EM_STRUCT   参考em_struct.h
	E_M2MDC_RR_EM_CELL_SELECT_PARA_INFO,			//RR_EM_CELL_SELECT_PARA_INFO
	E_M2MDC_RR_EM_CHANNEL_DESCR_INFO,				//RR_EM_CHANNEL_DESCR_INFO     
	E_M2MDC_RR_EM_CTRL_CHANNEL_DESCR_INFO,			//RR_EM_CTRL_CHANNEL_DESCR_INFO
	E_M2MDC_RR_EM_RACH_CTRL_PARA_INFO,				//RR_EM_RACH_CTRL_PARA_INFO    
	E_M2MDC_RR_EM_LAI_INFO,							//RR_EM_LAI_INFO               
	E_M2MDC_RR_EM_RADIO_LINK_COUNTER_INFO,			//RR_EM_RADIO_LINK_COUNTER_INFO
	E_M2MDC_RR_EM_MEASUREMENT_REPORT_INFO,			//RR_EM_MEASUREMENT_REPORT_INFO
	E_M2MDC_RR_EM_CONTROL_MSG_INFO,					//RR_EM_CONTROL_MSG_INFO
	E_M2MDC_RR_EM_TBF_INFO,							//RR_EM_TBF_INFO
	E_M2MDC_RR_EM_GPRS_GENERAL_INFO,				//RR_EM_GPRS_GENERAL_INFO
	//EN
	//sscstring 参考sscstringhandle.h
	E_M2MDC_SSC_AUTO,
	E_M2MDC_SSC_SCHINESE,
	E_M2MDC_SSC_TCHINESE,
	E_M2MDC_SSC_ENGLISH,
	E_M2MDC_SSC_DEFAULT,
	//END

	//FS
	E_M2MDC_FS_READ_WRITE,       
	E_M2MDC_FS_READ_ONLY,        
	E_M2MDC_FS_OPEN_SHARED,      
	E_M2MDC_FS_OPEN_NO_DIR,      
	E_M2MDC_FS_OPEN_DIR,         
	E_M2MDC_FS_CREATE,           
	E_M2MDC_FS_CREATE_ALWAYS,    
	E_M2MDC_FS_COMMITTED,		
	E_M2MDC_FS_CACHE_DATA,	    
	E_M2MDC_FS_LAZY_DATA,		
	E_M2MDC_FS_NONBLOCK_MODE,	
	E_M2MDC_FS_PROTECTION_MODE,	
	E_M2MDC_FS_NOBUSY_CHECK_MODE,
	E_M2MDC_FS_DI_BASIC_INFO,
	E_M2MDC_FS_DI_FREE_SPACE,
	E_M2MDC_FS_DI_FAT_STATISTICS,
	//END
	//GDI&LCD
	E_M2MDC_GDI_COLOR_TRANSPARENT,
	E_M2MDC_CAMERA_PREVIEW_LCM_MAINLCD,				//MDI_CAMERA_PREVIEW_LCM_MAINLCD
	E_M2MDC_GDI_LAYER_ENABLE_LAYER_0,
	E_M2MDC_GDI_LAYER_ENABLE_LAYER_1,
	E_M2MDC_GDI_LAYER_ROTATE_0,
	//END
	//CAMERA RESULT
	E_M2MDC_MDI_RES_CAMERA_SUCCEED,
	//END
	//mdi audio  参考 Mdi_audio.h
	E_M2MDC_MDI_DEVICE_SPEAKER,						//MDI_DEVICE_SPEAKER
	E_M2MDC_MDI_DEVICE_MICROPHONE,					//MDI_DEVICE_MICROPHONE
	E_M2MDC_MDI_DEVICE_SPEAKER2,					//MDI_DEVICE_SPEAKER2
	E_M2MDC_MDI_DEVICE_LOUDSPEAKER,					//MDI_DEVICE_LOUDSPEAKER
	E_M2MDC_MDI_DEVICE_SPEAKER_BOTH,				//MDI_DEVICE_SPEAKER_BOTH
	E_M2MDC_MDI_DEVICE_BT_HEADSET,					//MDI_DEVICE_BT_HEADSET
	E_M2MDC_MDI_DEVICE_AUTO_SELECT,					//自动选择  

	E_M2MDC_MDI_AUDIO_SUCCESS,						//MDI_AUDIO_SUCCESS
	E_M2MDC_MDI_AUDIO_FAIL,							//MDI_AUDIO_FAIL
	E_M2MDC_MDI_AUDIO_END_OF_FILE,					//MDI_AUDIO_END_OF_FILE
	E_M2MDC_MDI_AUDIO_TERMINATED,					//MDI_AUDIO_TERMINATED
	E_M2MDC_MDI_AUDIO_BAD_FORMAT,					//MDI_AUDIO_BAD_FORMAT

	E_M2MDC_MDI_FORMAT_WAV,							//MDI_FORMAT_WAV
	E_M2MDC_MDI_FORMAT_SMF,							//MDI_FORMAT_SMF

    // 参考enum Media_Format
	E_M2MDC_MEDIA_FORMAT_PCM_8K,                    //MEDIA_FORMAT_PCM_8K
	E_M2MDC_MEDIA_FORMAT_PCM_16K,                   //MEDIA_FORMAT_PCM_16K

    // 参考enum Media_Status
    E_M2MDC_MEDIA_SUCCESS,                          //MEDIA_SUCCESS
    E_M2MDC_MEDIA_FAIL,                             //MEDIA_FAIL

    // 参考enum med_result_enum
    E_M2MDC_MED_RES_OK,                             //MED_RES_OK
    E_M2MDC_MED_RES_AUDIO_END,                      //MED_RES_AUDIO_END

	E_M2MDC_MDI_AUD_VOL_EX_NUM,						//MDI_AUD_VOL_NUM

	//参考文件devices.h
	E_M2MDC_DEVICE_AUDIO_PLAY_INFINITE,				//DEVICE_AUDIO_PLAY_INFINITE
	E_M2MDC_DEVICE_AUDIO_PLAY_ONCE,					//DEVICE_AUDIO_PLAY_ONCE

	E_M2MDC_VOL_TYPE_MEDIA,							//VOL_TYPE_MEDIA

	//end
	//SOC
	E_M2MDC_SOCK_STREAM,
	E_M2MDC_SOCK_DGRAM,
	E_M2MDC_SOCK_SMS,
	E_M2MDC_SOCK_RAW,

	E_M2MDC_SOC_OOBINLINE,
	E_M2MDC_SOC_LINGER,
	E_M2MDC_SOC_NBIO,
	E_M2MDC_SOC_ASYNC,

	E_M2MDC_SOC_NODELAY,
	E_M2MDC_SOC_KEEPALIVE,
	E_M2MDC_SOC_RCVBUF,
	E_M2MDC_SOC_SENDBUF,

	E_M2MDC_SOC_NREAD,
	E_M2MDC_SOC_PKT_SIZE,
	E_M2MDC_SOC_SILENT_LISTEN,
	E_M2MDC_SOC_QOS,

	E_M2MDC_SOC_TCP_MAXSEG,
	E_M2MDC_SOC_IP_TTL,
	E_M2MDC_SOC_LISTEN_BEARER,
	E_M2MDC_SOC_UDP_ANY_FPORT,

	E_M2MDC_SOC_WIFI_NOWAKEUP,
	E_M2MDC_SOC_UDP_NEED_ICMP,
	E_M2MDC_SOC_IP_HDRINCL,
	E_M2MDC_SOC_NON_AUTO_ACT_BEARER,
	E_M2MDC_SOC_AUTO_DEACT_BEARER,

	E_M2MDC_SOC_READ,
	E_M2MDC_SOC_WRITE,
	E_M2MDC_SOC_ACCEPT,
	E_M2MDC_SOC_CONNECT,
	E_M2MDC_SOC_CLOSE,

	E_M2MDC_SOC_SUCCESS,
	E_M2MDC_SOC_ERROR,
	E_M2MDC_SOC_WOULDBLOCK,
	E_M2MDC_SOC_LIMIT_RESOURCE,
	E_M2MDC_SOC_INVALID_SOCKET,
	E_M2MDC_SOC_INVALID_ACCOUNT,
	E_M2MDC_SOC_NAMETOOLONG,
	E_M2MDC_SOC_ALREADY,
	E_M2MDC_SOC_OPNOTSUPP,
	E_M2MDC_SOC_CONNABORTED,
	E_M2MDC_SOC_INVAL,
	E_M2MDC_SOC_PIPE,
	E_M2MDC_SOC_NOTCONN,
	E_M2MDC_SOC_MSGSIZE,
	E_M2MDC_SOC_BEARER_FAIL,
	E_M2MDC_SOC_CONNRESET,
	E_M2MDC_SOC_DHCP_ERROR,
	E_M2MDC_SOC_IP_CHANGED,
	E_M2MDC_SOC_ADDRINUSE,
	E_M2MDC_SOC_CANCEL_ACT_BEARER,

	E_M2MDC_MAX_SOCKET_NUM,
	E_M2MDC_PF_INET,
	E_M2MDC_IPPROTO_IP,
	//END
	//profile
	E_M2MDC_PROFILE_GENERAL,
	E_M2MDC_PROFILE_MEETING,
	E_M2MDC_PROFILE_OUTDOOR,
	E_M2MDC_PROFILE_INDOOR,
	E_M2MDC_PROFILE_HEADSET,
	E_M2MDC_PROFILE_SILENT,
	E_M2MDC_PROFILE_BT,
	//end
	//date format
	E_M2MDC_DF_DD_MMM_YYYY_H,
	E_M2MDC_DF_DD_MM_YYYY_S,
	E_M2MDC_DF_MM_DD_YYYY_S,
	E_M2MDC_DF_YYYY_MM_DD_S,
	E_M2MDC_DF_YYYY_MM_DD_H,
	E_M2MDC_DF_MMM_DD_YYYY,
	//end

	//TextEncodeing   参考 Conversions.h
	E_M2MDC_BIG_5_ENCODING,					//BIG_5_ENCODING_TYPE
	E_M2MDC_GB2312_ENCODING,				//GB2312_ENCODING_TYPE
	//end

	E_M2MDC_ALWAYS_ASK_SIM_INFO_1,
	E_M2MDC_ALWAYS_ASK_SIM_INFO_2,
	
	E_M2MDC_IPPROTO_ICMP,
	E_M2MDC_IPPROTO_HOPOPTS,
	E_M2MDC_IPPROTO_IGMP,
	E_M2MDC_IPPROTO_IPV4,
	E_M2MDC_IPPROTO_IPIP, 
	E_M2MDC_IPPROTO_TCP,
	E_M2MDC_IPPROTO_UDP,
	E_M2MDC_IPPROTO_RAW,  

    //mmi_frm_sms_app_msgbox_enum
    E_M2MDC_SMS_APP_UNREAD,         //MMI_FRM_SMS_APP_UNREAD
    E_M2MDC_SMS_APP_INBOX,          //MMI_FRM_SMS_APP_INBOX
    E_M2MDC_SMS_APP_OUTBOX,         //MMI_FRM_SMS_APP_OUTBOX
    E_M2MDC_SMS_APP_DRAFTS,         //MMI_FRM_SMS_APP_DRAFTS
    E_M2MDC_SMS_APP_AWAITS,         //MMI_FRM_SMS_APP_AWAITS
    E_M2MDC_SMS_APP_DATA,           //MMI_FRM_SMS_APP_DATA
    //end

	//遗漏的vol up/down and send
	E_M2MDC_KEY_VOL_UP,
	E_M2MDC_KEY_VOL_DOWN,
	E_M2MDC_KEY_SEND,

#if MEX_SDK_VER >= 1005
    E_M2MDC_MDI_FORMAT_MP3,                         //MDI_FORMAT_DAF
    E_M2MDC_MDI_FORMAT_AMR,                         //MDI_FORMAT_AMR

    E_M2MDC_MEDIA_FORMAT_AMR,                       //MEDIA_FORMAT_AMR
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1012
	//遗漏的key clear
	E_M2MDC_KEY_CLEAR,
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
    // SPREAD SYSTEM DEFINE
    //从Spread系统常量到mex常量的转换
	E_S2MDC_BLM_MEX_MEM_INTERNAL,					// BLOCK_MEM_POOL_MET_MEX_MEM_INTERNAL (block_mem.h)
	E_S2MDC_BLM_MEX_MEM_EXTERNAL,					// BLOCK_MEM_POOL_MET_MEX_MEM_EXTERNAL (block_mem.h)
	E_S2MDC_BLM_MEX_MEM_RESERVED,					// BLOCK_MEM_POOL_MET_MEX_MEM_RESERVED (block_mem.h)
	E_S2MDC_BLM_NOT_USE,							// NOT_USE (block_mem_def.h)

	//Audio begin
	E_S2MDC_ADR_NO_ERROR,							// AUDIO_RESULT_E(Audio_api.h)
    E_S2MDC_ADR_AUDIO_STOP,
	//Audio end

	//Socket begin
	E_S2MDC_PF_INET,								//pf_inet
	E_S2MDC_IPPROTO_IP,								//ip NOT FIND YET RETURN -1
	E_S2MDC_SOC_ERROR,								// NOT FIND YET,  RETURN -1
	E_S2MDC_SOC_SUCCESS,							// SUCCESS (App_tcp_if.h)
	E_S2MDC_SOC_CLOSE,								// NOT FIND YET,  RETURN 0
	E_S2MDC_SOC_WOULDBLOCK,
    
	E_S2MDC_SOC_READ,								
	E_S2MDC_SOC_WRITE,
	E_S2MDC_SOC_ACCEPT,
	E_S2MDC_SOC_CONNECT,
	

	
	E_S2MDC_SOC_STREAM,								// SOCK_STREAM(App_tcp_if.h)
	E_S2MDC_SOC_DGRAM,								// SOCK_DGRAM(App_tcp_if.h)
	E_S2MDC_SOC_RAW,								// SOCK_RAW(App_tcp_if.h)
	E_S2MDC_SOC_SMS, 								// SOCK_SMS(sprd have no)
	
	E_S2MDC_SOC_DEBUG,								// SO_DEBUG(App_tcp_if.h)
	E_S2MDC_SOC_ACCEPTCONN,							// SO_ACCEPTCONN(App_tcp_if.h)
	E_S2MDC_SOC_REUSEADDR,							// SO_REUSEADDR( App_tcp_if.h)
	E_S2MDC_SOC_KEEPALIVE,							// SO_KEEPALIVE(App_tcp_if.h)
	E_S2MDC_SOC_OOBINLINE,							// SO_OOBINLINE(App_tcp_if.h)
	E_S2MDC_SOC_LINGER,
	E_S2MDC_SOC_ASYNC,

	E_S2MDC_SOC_NBIO,								// SO_NBIO (App_tcp_if.h)
	E_S2MDC_SOC_BIO,								// SO_BIO(App_tcp_if.h)
	E_S2MDC_SOC_MYADDR,								// SO_MYADDR(App_tcp_if.h)
	E_S2MDC_SOC_RXDATA,								// SO_RXDATA(App_tcp_if.h)
	E_S2MDC_SOC_TXDATA,								// SO_TXDATA(App_tcp_if.h)

	E_S2MDC_SOC_AF_INET,							// AF_INET(App_tcp_if.h)
	    
	//Socket end
	E_S2MDC_ADR_AUDIO_MAX_VOL,                      // MAX SOUND VOLUME 

#if (MEX_SDK_VER >= 1018)   //支持三SIM卡
    E_S2MDC_MN_DUAL_SYS_1,
    E_S2MDC_MN_DUAL_SYS_2,
    E_S2MDC_MN_DUAL_SYS_3,
    E_S2MDC_CC_SIM1_CALL,
    E_S2MDC_CC_SIM2_CALL,
    E_S2MDC_CC_SIM3_CALL,   
#endif

#if (MEX_SDK_VER >= 1019)   //支持字体转换
    E_S2MDC_SONG_FONT_16,
    E_S2MDC_SONG_FONT_12,
#endif

#if (MEX_SDK_VER >= 1020)   //增加SOCKET状态码
    E_M2MDC_SOC_SS_NOFDREF,           /* no file table ref any more */
    E_M2MDC_SOC_SS_ISCONNECTED,       /* socket connected to a peer */
    E_M2MDC_SOC_SS_ISCONNECTING,      /* in process of connecting to peer */
    E_M2MDC_SOC_SS_ISDISCONNECTING,   /*  in process  of disconnecting */
    E_M2MDC_SOC_SS_CANTSENDMORE,      /* can't send more data to peer */
    E_M2MDC_SOC_SS_CANTRCVMORE,       /* can't receive more data from peer */
    E_M2MDC_SOC_SS_RCVATMARK,         /* at mark on input */
    E_M2MDC_SOC_SS_PRIV,              /* privileged for broadcast, raw... */
    E_M2MDC_SOC_SS_NBIO,              /* non-blocking ops */
    E_M2MDC_SOC_SS_ASYNC,             /* async i/o notify */
    E_M2MDC_SOC_SS_UPCALLED,          /* zerocopy data has been upcalled (for select) */
    E_M2MDC_SOC_SS_INUPCALL,          /* inside zerocopy upcall (reentry guard) */
    E_M2MDC_SOC_SS_UPCFIN,            /* inside zerocopy upcall (reentry guard) */
    E_M2MDC_SOC_SS_WASCONNECTING,     /* SS_ISCONNECTING w/possible pending error */
	E_M2MDC_MET_MEX_SEND,
#endif

#if	(MEX_SDK_VER >= 1021)
    E_S2MDC_ADR_AUDIO_ERROR_IND,
    E_S2MDC_ADR_AUDIO_PLAYEND_IND,
#endif

    E_S2MDC_MED_PCM_SRC_OK,                             //AUDIO_STREAM_GET_SRC_SUCCESS
    E_S2MDC_MED_PCM_SRC_ERROR,                      //MED_RES_AUDIO_END
    E_S2MDC_MED_WAV_TYPE_PCM,
#endif
//#ifdef __MET_MEX_SIM_PLUS__

	E_M2MDC_MOD_SMS_3 =0x400,
	E_M2MDC_MOD_SMS_4 =0x401,
	E_M2MDC_MOD_SIM_3 =0x402,
	E_M2MDC_MOD_SIM_4 =0x403,
	E_M2MDC_MOD_L4C_3 =0x404,
	E_M2MDC_MOD_L4C_4 =0x405,
	E_M2MDC_MOD_PHB_3 =0x406,
	E_M2MDC_MOD_PHB_4 =0x407,
	E_M2MDC_MOD_AS_3 =0x408,
	E_M2MDC_MOD_AS_4 =0x409,
	E_M2MDC_ALWAYS_ASK_SIM_INFO_3 =0x40A,
	E_M2MDC_ALWAYS_ASK_SIM_INFO_4 =0x40B,
//#endif/*__MET_MEX_SIM_PLUS__*/
#if defined(__MET_MEX_SUPPORT__)&& defined(MEX_G7_UI_SUPPORT)
	E_M2MDC_KEY_End,  
#endif
#if defined(MEX_SUPPORT_KEY_EXTRA_1)
    E_M2MDC_KEY_EXTRA_1,
#endif    
	E_SYS2MDC_MAX                                   // system define max	
} E_SYS2MEX_Def_Convert;


//从mtk系统结构到mex结构的转换
typedef enum
{
	E_M2MSC_SockAddr_Struct,

    //struct convert for hook
    E_M2MSC_Hk_Sms_Deliver_Msg,  // mmi_sms_deliver_msg_ind_struct -> Mex_Hk_Sms_Deliver_Msg
    E_M2MSC_Hk_Cc_Call_Ring,     // mmi_cc_call_ring_ind_struct -> Mex_Hk_Cc_Call_Ring
    //end
    
	E_M2MSC_MAX
}E_MTK2MEX_Struct_Convert;

//进入系统其他功能
typedef enum
{
	E_MESA_NONE = 0,					//无
	E_MESA_PHONEBOOK = 1,				//电话簿
	E_MESA_SMS = 2,						//短信
	E_MESA_NEWSMS = 3,					//新短信
	E_MESA_SMSINBOX = 4,				//收件箱
	E_MESA_WAP = 5,						//浏览器						
	E_MESA_CAMERA = 6,					//照相机
	E_MESA_VIDEOREC = 7,				//视频录像
	E_MESA_AUDIOPLAYER = 8,				//音频播放器
	E_MESA_VIDEOPLAYER = 9,				//视频播放器
	E_MESA_PHOTO = 10,					//相册
	E_MESA_FM = 11,						//FM
	E_MESA_TV = 12,						//TV
	E_MESA_CALCULATOR = 13,				//计算器
	E_MESA_ALRAM = 14,					//闹铃
	E_MESA_STOPWATCH = 15,				//秒表
	E_MESA_CALENDAR = 16,				//日历
	E_MESA_FILEMGR = 17,					//文件管理浏览
	E_MESA_DIGITHANDLER = 18,			//拨号程序
	E_MESA_MAINMENU = 19,				//主菜单

	E_MESA_WAP_TO_URL = 20,				//浏览到网址

	E_MESA_MEX_PLATFORM = 21,			//启动Mex平台
	E_MESA_MEX_SPECIAL_ENTRY = 22,		//启动Mex平台特殊入口
	E_MESA_IDLE_SCREEN = 23,				//回到待机界面
	E_MESA_GO_BACK_HISTORY = 24,			//回到上个界面
#if MEX_SDK_VER >= 1010
	E_MESA_CALLHISTORY = 25,
#endif

#if MEX_SDK_VER >= 1014
	E_MESA_BT = 26,                      //蓝牙
	E_MESA_BT_ENTRY_POWER_SWITCH = 27,   //蓝牙状态切换 mmi_bt_entry_power_switch
	E_MESA_BT_ENTRY_CLOSE_ALL_CONNS = 28,//断开所有蓝牙设备 mmi_bt_entry_release_all_connections
#endif

#if MEX_SDK_VER >= 1019
    E_MESA_DATETIME_SET = 29,
#endif

#if (MEX_MTK_CODE_VERSION >= 0x1032)
	E_MESA_FUNANDGAMES = 30,				//游戏和娱乐
	E_MESA_MULTIMEDIA = 31,				//多媒体
	E_MESA_ORGANIZER = 32,				//工具箱
	E_MESA_SETTINGS = 33,				//设置
	E_MESA_PROFILE = 34,					//情景模式
	E_MESA_EXTRA = 35,					//附加功能
	E_MESA_WLAN = 36,					//无线局域网
#endif
	E_MESA_MAX
}E_MEX_Entry_System_App;

//**
typedef enum
{
	E_MPS_None,
	E_MPS_Pause,
	E_MPS_HotReboot,
	E_MPS_ColdReboot,
	E_MPS_HotShutdown,
	E_MPS_ColdShutdown,
	E_MPS_MAX
} E_Mex_PlatformShutdown;

typedef enum
{
	E_MEE_OK,					//进入成功
	E_MEE_USBMS,				//usb处于存储模式
	E_MEE_NOCARD,				//无储存卡
	E_MEE_SPACESHORTAGE,		//存储空间不足
	E_MEE_INTMEMINITERR,		//Internal内存初始化失败
	E_MEE_EXTMEMINITERR,		//External内存初始化失败
	E_MEE_RESMEMINITERR,		//Reserved内存初始化失败
	E_MEE_PREPARETFLASHERR,		//T卡文件加载失败
	E_MEE_STARTUPTINYERR,		//启动ManagerTiny失败
	E_MEE_MAX
}E_Mex_Entry_Error;
typedef enum
{
	E_MDWM_None,
	E_MDWM_CheckExist,					//下载不存在的
	E_MDWM_CheckExistAndVersion,		//下载不存在的和版本低的
	E_MDWM_Resume,						//继续下载
	E_MDWM_AllCorver,					//重头下载
	E_MDWM_Max
}E_Mex_Downloader_Work_Mode;

//mexDownloader下载完的处理方式
typedef enum
{
	E_MDDM_DownUncompress,				//边下边解压
	E_MDDM_OnlyDownload,				//只下载
	E_MDDM_UncompressFile,				//只解压
	E_MDDM_Max
}E_Mex_Downloader_Deal_Method;

//mex平台启动状态，便于平台恢复和死机原因查找
typedef enum
{
	E_MSES_None,
	E_MSES_StartManagerTiny,
	E_MSES_StartMmcMonitor,
	E_MSES_StartManagerFull,
	E_MSES_StartPolicygetter,
	E_MSES_StartDownloader,
	E_MSES_StartMexApp,
	E_MSES_Max
}E_Mex_System_Entry_Step;

//手机多sim卡模式设置
typedef enum
{
	E_MDSM_DualSIM,
	E_MDSM_OnlySIM1,
	E_MDSM_OnlySIM2,
	E_MDSM_FlightMode,
	E_MDSM_Max
}E_Mex_Dual_Sim_Mode;

typedef enum
{
	E_MKLE_KeyPad_Lock,
	E_MKLE_KeyPad_UnLock,	
	E_MKLE_Max
}E_Mex_KeyPad_Lock_Event;

#if MEX_SDK_VER >= 1009
//sync  mmi_fmgr_filetype_enum
typedef enum
{
	E_MFF_TYPE_ALL,      /* this type represent all files (included not supported types, like *.doc */
    E_MFF_TYPE_UNKNOWN,
    E_MFF_TYPE_FOLDER,
    E_MFF_TYPE_FOLDER_DOT,
    
    // Image type
    E_MFF_TYPE_BMP,
    E_MFF_TYPE_JPG,
    E_MFF_TYPE_JPEG,    
    E_MFF_TYPE_GIF,
    E_MFF_TYPE_PNG,
    E_MFF_TYPE_WBMP,
    E_MFF_TYPE_WBM,
    E_MFF_TYPE_M3D,
    E_MFF_TYPE_EMS,
    E_MFF_TYPE_ANM,
    E_MFF_TYPE_SVG,

    // Audio type
    E_MFF_TYPE_IMY,
    E_MFF_TYPE_MID,
    E_MFF_TYPE_MIDI,
    E_MFF_TYPE_WAV,
    E_MFF_TYPE_AMR,
    E_MFF_TYPE_AAC,
    E_MFF_TYPE_MP3,
    E_MFF_TYPE_MP2,
    E_MFF_TYPE_VM,
    E_MFF_TYPE_AWB,
    E_MFF_TYPE_AIF,
    E_MFF_TYPE_AIFF,
    E_MFF_TYPE_AIFC,
    E_MFF_TYPE_AU,
    E_MFF_TYPE_SND,
    E_MFF_TYPE_M4A,
    E_MFF_TYPE_MMF,
    E_MFF_TYPE_WMA,

    // Video type
    E_MFF_TYPE_3PG,
    E_MFF_TYPE_MP4,
    E_MFF_TYPE_MPG,
    E_MFF_TYPE_AVI,
    E_MFF_TYPE_3G2,
    E_MFF_TYPE_SDP,
    E_MFF_TYPE_RAM,
    
   
    E_MFF_TYPE_WML,
    E_MFF_TYPE_HTML,
    E_MFF_TYPE_HTM,
    E_MFF_TYPE_XHTML,
    
    E_MFF_TYPE_JAD,
    E_MFF_TYPE_JAR,

    E_MFF_TYPE_LRC,
    E_MFF_TYPE_THEME,
    E_MFF_TYPE_EBOOK_TXT,
    E_MFF_TYPE_EBOOK_PDB,
    E_MFF_TYPE_FOTA_FULL,

    E_MFF_TYPE_URL,
    
    E_MFF_TYPE_COUNT
}E_Mex_Fmgr_Filtertype;

typedef enum
{
	E_MFST_SELECT_FILE							= 0x01,
	E_MFST_SELECT_FOLDER 						= 0x02,
	E_MFST_SELECT_FILE_OR_FOLDER 				= 0x04,
	E_MFST_SELECT_BROWSE						= 0x08,
	E_MFST_SELECT_REPEAT						= 0x10,
	E_MFST_SELECT_FILE_SET						= 0x20,
	E_MFST_SELECT_ARCHIVE						= 0x30,
	E_MFST_SELECT_ARCHIVE_WITH_SINGLE_OBJECT	= 0x40,
}E_Mex_Fmgr_SelType;


typedef enum
{
	E_MFR_SUCCESS,
	E_MFR_BUSY,
	E_MFR_MEMERROR,
	E_MFR_MAX
}
E_Mex_Fmgr_Reuslt;

typedef void(*Fmgr_CallBack)(wchar_t *path,bool bIsShort);


#endif


#define MEX_LOG_SYSTEM			0x00000001
#define MEX_LOG_MEMORY			0x00000002
#define MEX_LOG_MANAGER			0x00000004
#define MEX_LOG_STDLIB			0x00000008
#define MEX_LOG_OSLIB			0x00000010
#define MEX_LOG_CONFIG			0x00000020
#define MEX_LOG_SOC				0x00000040
#define MEX_LOG_PHS				0x00000080
#define MEX_LOG_MDI				0x00000100
#define MEX_LOG_GUI				0x00000200
#define MEX_LOG_REG				0x00000400
#define MEX_LOG_LIB				0x00000800
#define MEX_LOG_APP0			0x00010000
#define MEX_LOG_APP1			0x00020000
#define MEX_LOG_APP2			0x00040000
#define MEX_LOG_APP3			0x00080000


///////////////////////////////////////////////////////
// target-specific macro definitions
///////////////////////////////////////////////////////

#define MEX_H_LAUNCH_FUNC( cat )							MexLib_Entry_##cat
#define MEX_C_LIB_FUNC_NAME( cat )							g_pMexLib_##cat##_Funcs
#define MEX_H_GET_FUNC_DEFINE_BASE( cat, fn )				(MEX_C_LIB_FUNC_NAME(cat)[E_MF_##cat##_##fn])

#define MEX_H_ENUM_FUNC_START( cat )						typedef enum {
#define MEX_H_ENUM_FUNC( cat, fn )							E_MF_##cat##_##fn,
#define MEX_H_ENUM_FUNC_END( cat )							MEX_H_NB_FUNC(cat) } E_Mex_##cat##_Funcs;

#define MEX_H_NB_FUNC( cat )								E_MF_##cat##_MAX

#if defined(MEX_TARGET_SERVER)

	#define MEX_H_DECLARE_FUNC( ftype, fname, fparams )		extern ftype mex_##fname##fparams;
	#define MEX_C_OVERRIDE_FUNC( cat, fn )					((funcptr)MEX_H_GET_FUNC_DEFINE_BASE(cat, fn)) = (funcptr)mex_##fn

#endif

#if defined(MEX_TARGET_CLIENT)

	#define MEX_H_DECLARE_FUNC( ftype, fname, fparams )		typedef ftype (*fd_##fname)fparams;

	#define MEX_H_GET_FUNC_DEFINE( cat, fname )				((fd_##fname)MEX_H_GET_FUNC_DEFINE_BASE( cat, fname ))
	#define MEX_C_OVERRIDE_FUNC( cat, fn )					MEX_C_LIB_FUNC_NAME(cat)[E_MF_##cat##_##fn] = (funcptr)mex_##fn
#endif

typedef funcptr* (*MexApp_LoadLib_Func)(const wchar_t *szLibName, const wchar_t *szFullFileName);
typedef void (*MexApp_FreeLib_Func)(funcptr * libFuncPtr);

typedef E_Mex_Startup_Result (*MexApp_Start_Func)(int32 iMyAppId, MexApp_LoadLib_Func fnLoadLib, MexApp_FreeLib_Func fnFreeLib);
typedef void (*MexApp_Exit_Func)(int32 iMyAppId, MexApp_FreeLib_Func fnFreeLib);

typedef int32 (*MexApp_MsgHandler_Func)(int32 iSrcAppId, int32 iDstAppId, E_Mex_MessageType eMsgType, int32 iMsgData0, int32 iMsgData1, int32 iMsgData2, int32 iMsgData3);

typedef void (*MexFuncPtr)(void);

//输入法回调函数
//参数1: 输入之后的结果
//参数2: 是否确认输入
typedef void (*Mex_Input_Func)(uint8 * inputBuf, bool bIsFirm);


#endif
