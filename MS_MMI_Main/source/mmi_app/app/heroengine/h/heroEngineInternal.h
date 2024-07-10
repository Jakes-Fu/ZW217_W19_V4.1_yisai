
#ifndef  _MMI_HERO_ENGINE_INTERNAL_H_    
#define  _MMI_HERO_ENGINE_INTERNAL_H_  

 /**-------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiaudio.h"
#include "Mmi_theme.h"
#include "heroEngineApp.h"
#include "mmifmm_export.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/*trace 开关*/
#define HERO_APP_LOGOPEN
//#define HR_APP_LOG_FILE  //开启该宏LOG信息将保存在文件中



#ifdef HERO_APP_LOGOPEN
#define MMIHR_Trace(a)    hr_printf a
#define MMIHR_TraceM(a)   hr_printf a
#if defined(PLATFORM_UIX8910)
#define MMIHR_TraceApp(a) SCI_TRACE_MID a
#else
#define MMIHR_TraceApp(a) SCI_TRACE_LOW a
#endif
#define MMIHR_TraceT(a)   //hr_printf a
#define MMIHR_TraceN(a)   hr_printf a
#define MMIHR_TraceB(a)   //hr_printf a
#define MMIHR_TraceF(a)   hr_printf a
#else//关闭TRACE
#define MMIHR_Trace(a)
#define MMIHR_TraceM(a)
#define MMIHR_TraceApp(a)
#define MMIHR_TraceT(a)
#define MMIHR_TraceN(a)
#define MMIHR_TraceB(a)
#define MMIHR_TraceF(a)
#endif

#define M_NEW_VER
#define HR_FS_ASYN //if support asyn ffs ,open it
#define HR_SHARE_BLOCK_MEM
#undef _STANDARD_FS_//do not define
#define HR_USE_PLAT_LCD_BUFFER//use  plat lcd buffer directly
#ifdef WIN32
#undef HERO_ENGINE_SUPPORT_CLKADAPT
#else
//#define HERO_ENGINE_SUPPORT_CLKADAPT
#endif
#define HR_BINDTO_VIRTUAL_WIN//将timer等消息挂在virtual win上
//for differentiate the different interface of nile and rocky 
#ifdef HERO_PORT_BASE_NILE
/*for nile base plat*/
//FFS. adapt len to 1byte union
#define HERO_HERO_MMIFILE_DEVICE_SYS_NAME_LEN_AD		(HERO_MMIFILE_DEVICE_SYS_NAME_LEN)*2
#define HERO_HERO_MMIFILE_DEVICE_UDISK_LEN_AD			(HERO_MMIFILE_DEVICE_UDISK_LEN)*2
#define HERO_HERO_MMIFILE_DEVICE_SDCARD_LEN_AD		(HERO_MMIFILE_DEVICE_SDCARD_LEN)*2
#ifdef DOUBLE_TFCARD
#define HERO_HERO_MMIFILE_DEVICE_SDCARD_LEN_2_AD		(HERO_MMIFILE_DEVICE_SDCARD_LEN_2)*2
#endif

#define HERO_MMIFILE_FULL_PATH_MAX_LEN_AD		(MMIFILE_FULL_PATH_MAX_LEN)*2
#define HERO_MMIFILE_FILE_NAME_MAX_LEN_AD		(MMIFILE_FILE_NAME_MAX_LEN)*2
#define HERO_MMIFILE_DEVICE_NAME_MAX_LEN_AD		(MMIFILE_DEVICE_NAME_MAX_LEN)*2
#define HERO_MMIFILE_DIR_NAME_MAX_LEN_AD			(MMIFILE_DIR_NAME_MAX_LEN)*2
//GUI
#define HERO_GUILIST_STRING_MAX_NUM_AD		    GUILIST_STRING_MAX_NUM
#else
/*for rocky base plat*/
//FFS. adapt len to 1byte union
#define HERO_HERO_MMIFILE_DEVICE_SYS_NAME_LEN_AD		(HERO_MMIFILE_DEVICE_SYS_NAME_LEN)
#define HERO_HERO_MMIFILE_DEVICE_UDISK_LEN_AD			(HERO_MMIFILE_DEVICE_UDISK_LEN)
#define HERO_HERO_MMIFILE_DEVICE_SDCARD_LEN_AD		(HERO_MMIFILE_DEVICE_SDCARD_LEN)
#ifdef DOUBLE_TFCARD
#define HERO_HERO_MMIFILE_DEVICE_SDCARD_LEN_2_AD		(HERO_MMIFILE_DEVICE_SDCARD_LEN_2)
#endif

#define HERO_MMIFILE_FULL_PATH_MAX_LEN_AD		(MMIFILE_FULL_PATH_MAX_LEN)
#define HERO_MMIFILE_FILE_NAME_MAX_LEN_AD		(MMIFILE_FILE_NAME_MAX_LEN)
#define HERO_MMIFILE_DEVICE_NAME_MAX_LEN_AD		(MMIFILE_DEVICE_NAME_MAX_LEN)
#define HERO_MMIFILE_DIR_NAME_MAX_LEN_AD			(MMIFILE_DIR_NAME_MAX_LEN)
//GUI
#define HERO_GUILIST_STRING_MAX_NUM_AD		    LISTBOX_STRING_MAX_NUM

#endif

#if (HERO_SPR_VERSION >= 0x10A1140)
//UDISK represented by "D"
#define HERO_MMIFILE_DEVICE_UDISK                (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK)
#define HERO_MMIFILE_DEVICE_UDISK_LEN            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)

//SD card represented by "E"
#define HERO_MMIFILE_DEVICE_SDCARD               (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD)
#define HERO_MMIFILE_DEVICE_SDCARD_LEN           MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)

//SD card2 represented by "F" 
#define HERO_MMIFILE_DEVICE_SDCARD2               (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1)
#define HERO_MMIFILE_DEVICE_SDCARD2_LEN           MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)
//hidden partion "C" on nand flash
#define HERO_MMIFILE_DEVICE_SYS_NAME             (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM)
#define HERO_MMIFILE_DEVICE_SYS_NAME_LEN         MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM)

typedef enum
{
    FS_HS,
    FS_UDISK,
    FS_MMC,
    FS_INVALID 
} FILE_DEV_E_T;
#endif

/*UI depend*/
#define MMI_HERO_APP_DEFAULT_MAX_LEN             500 		//允许的最大输入值
#define MMI_HERO_APP_EDITPASSWORD_MAX_LEN       100 		//允许的最大密码输入值
#define MMI_HERO_APP_UCS2_NAME_MAX_LEN          (HERO_MMIFILE_FULL_PATH_MAX_LEN_AD + 2)//255				//UCS2存储的文件名的最大长度
#define MMI_HERO_APP_FULL_PATH_MAX_LEN          (HERO_MMIFILE_FULL_PATH_MAX_LEN_AD + 2)
#define MMI_HERO_APP_FILE_NAME_MAX_LEN          (HERO_MMIFILE_FILE_NAME_MAX_LEN_AD + 2)
#define MMI_HERO_APP_DIR_NAME_MAX_LEN           (HERO_MMIFILE_DIR_NAME_MAX_LEN_AD + 2)

#define HERO_APP_APPLIST_TITLE               "游戏中心"
#define MMI_HERO_APP_DIR_FLAG					'/'	 //目录标志
#define MMI_HERO_APP_DIR_FLAGSTR				"/"	 //目录标志
#define HERO_APP_DYN_WIN_ID_MAXNUM             25   //可以取得的最大win id数目
#define HERO_APP_MENU_ITEMSTR_MAXLEN           20   //MENU可显示的最大ITEM string长度-BYTE, depend lower ui
#define HERO_APP_MENU_ITEM_MAXNUM              40   //MENU可显示的最大ITEM 数目
#define HERO_APP_MENU_TREE_MAXDEEP             20   //MENU层次
#define HERO_APP_ENV_VALUE_INITED              0x5a //应用设置初始化标志

/*the app base win msgs*/
#define   HERO_RAPP_MSG_ASYN_FFS_RESULT	0xEE00//asyn ffs operation result msg
#define   HERO_APP_MSG_NETINIT_RESULT		0xEE01//bearer init msg
#define   HERO_APP_MSG_NETREDAIL_IND		0xEE02//bearer redail msg
#define   HERO_APP_MSG_SYS_EXIT			0xEE03//vm exit msg

#define   HERO_HERO_QQ2008_MSG_NETINIT_RESULT     0xEE04//skyqq

#define   HERO_APP_MSG_ASY_CLOSE_MENU     0xEE05

#define HR_LAYER_CLIP_STACK_COUNT        (16)
/*for socket*/
#define MMI_HERO_APP_GPRSPROTECT_TIME_OUT			60000		//gprs激活连接超时时间60s
#define HERO_APP_TCP_CNT_TIMEOUT     15 //连接超时时间15秒
#define HERO_APP_SOCKET_MAX_NUM      10 //2007add
#define HERO_APP_PDP_RETRY_TIMES     3 //pdp重连次数
#define HERO_APP_PDP_RETRY_TIMER_OUT 10000//5000 //pdp重连间隔

#ifndef INVALID_SOCKET
#define  INVALID_SOCKET -1    /* WINsock-ish synonym for SYS_SOCKETNULL */
#endif

#define     ENOBUFS        1
#define     ETIMEDOUT      2
#define     EISCONN        3
#define     EOPNOTSUPP     4
#define     ECONNABORTED   5
#define     EWOULDBLOCK    6
#define     ECONNREFUSED   7
#define     ECONNRESET     8
#define     ENOTCONN       9
#define     EALREADY       10
#define     EINVAL         11
#define     EMSGSIZE       12
#define     EPIPE          13//对端关闭连接
#define     EDESTADDRREQ   14
#define     ESHUTDOWN      15
#define     ENOPROTOOPT    16
#define     EHAVEOOB       17
#define     ENOMEM         18
#define     EADDRNOTAVAIL  19
#define     EADDRINUSE     20
#define     EAFNOSUPPORT   21
#define     EINPROGRESS    22
#define     ELOWER         23    /* lower layer (IP) error */
#define     ENOTSOCK	   24    /* Includes sockets which closed while blocked */
#define     EIEIO		   27 /* bad input/output on Old Macdonald's farm :-) */

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#include "heroEngineApi.h"

//declare win type 
typedef enum
{
	HERO_APP_WINTYPE_DIALOG = 0,
	HERO_APP_WINTYPE_TEXT,
	HERO_APP_WINTYPE_EDITOR,
	HERO_APP_WINTYPE_MENU,
	HERO_APP_WINTYPE_APP,

	HERO_APP_WINTYPE_MAX
}MMI_HERO_APP_DYN_WIN_TYPE_E;

//declare id  type 
typedef enum
{
	HERO_APP_ID_WIN,
	HERO_APP_ID_CTRL,
	
	HERO_APP_ID_INVALID
}MMI_HERO_APP_ID_TYPE_E;

//declare the operation type to IDs
typedef enum
{
	HERO_APP_GET_ID,
	HERO_APP_FREE_ID	
}MMI_HERO_APP_DYN_ID_OP_TYPE_E;

//declare the command type for the app wins
typedef enum
{
	MMI_HERO_APP_KEY_NORMAL,
	MMI_HERO_APP_MOUSE_DOWN,
	MMI_HERO_APP_MOUSE_UP,
	MMI_HERO_APP_MOUSE_MOVE,
	MMI_HERO_APP_MENU_SELECT,
	MMI_HERO_APP_MENU_RETURN,
	MMI_HERO_APP_DIALOG_SELECT,
	MMI_HERO_APP_DIALOG_RETURN,
	MMI_HERO_APP_KEYTYPE_MAX
}MMI_HERO_APP_KEYTYPE_E;

/*gprs state type*/
typedef enum
{
	HERO_APP_GPRS_DEACTIVED = 0,	
	HERO_APP_GPRS_ACTIVING,
	HERO_APP_GPRS_ACTIVED,
	HERO_APP_GPRS_DEACTIVEING
}MMI_HERO_APP_GPRS_STATE_E; 

/*image type*/
typedef enum
{
	HERO_IMAGE_TYPE_JPG_FILE,
	HERO_IMAGE_TYPE_GIF_FILE,
	HERO_IMAGE_TYPE_PNG_FILE,
	HERO_IMAGE_TYPE_BMP_FILE,
	HERO_IMAGE_TYPE_WBMP_FILE,
	HERO_IMAGE_TYPE_INVALID
}HERO_APP_IMG_TYPE;
 
//declare the operation type to menu icon
typedef enum
{
	HERO_APP_ICON_ON,
	HERO_APP_ICON_OFF	
}MMI_HERO_APP_MENUICON_OP_TYPE_E;

//2007add
typedef enum
{
	HERO_APP_NETSUBEVN_NONE,
	HERO_APP_NETSUBEVN_WAITDAIL,
}MMI_HERO_APP_NETSUBEVN_E;

//2007add
typedef enum
{
	HERO_APP_SOCKET_CLOSED,
	HERO_APP_SOCKET_OPENED,
	HERO_APP_SOCKET_CONNECTING,
	HERO_APP_SOCKET_CONNECTED,
	HERO_APP_SOCKET_ERROR
}MMI_HERO_APP_SOCKETSTATE_E;

//2007add
typedef struct
{
	int32 s;
	MMI_HERO_APP_SOCKETSTATE_E state;
	uint32 ip;
	uint16 port;
}MMI_HERO_APP_SOCKET_T;


typedef struct
{
	BOOLEAN mrNetActived;
	void *gprsCb;
	MMI_HERO_APP_GPRS_STATE_E gprsState;
	//2007add
	char apn[40];
	int32 subEvn;
	void *deactWaitFunc;
	uint32 data;

	int32 pdp_retry_times;
	//for gprs data flow
	uint32 gprs_send[MN_DUAL_SYS_MAX - MN_DUAL_SYS_1];
	uint32 gprs_recv[MN_DUAL_SYS_MAX - MN_DUAL_SYS_1];
	BOOLEAN need_count;
	int32 netid;
}MMI_HERO_APP_NETCONTEXT_T;

/*wuwenjie 2011-12-01 START*/
typedef struct
{
	char hostBuffer[128];
	int32 timerCount;
	HR_GET_HOST_CB cb;
	uint8 getHostTimer;
}MMI_HERO_APP_HOSTCONTEXT_T;
/*wuwenjie 2011-12-01 END*/

//declare id table struct 
typedef struct 
{
	uint8 *tablePtr;
	uint8 tableSize;
	uint32 baseId;	
}MMI_HERO_APP_DYN_ID_TABLE_T;

//declare dialog win private data struct 
typedef struct
{
//	MMI_STRING_T title;
	uint8 type;//HR_DIALOG_OK or HR_DIALOG_OK_CANCEL
}HERO_APP_DIALOG_DATA;

//declare text win private data struct 
typedef struct
{
	MMI_STRING_T title;
	MMI_STRING_T textInfo;
	uint8 type;//HR_DIALOG_OK or HR_DIALOG_OK_CANCEL
}HERO_APP_TEXT_DATA;

//declare editor win private data struct 
typedef struct
{
	MMI_STRING_T title;
	MMI_STRING_T textOut;
	uint16 maxLen;//max unicode input numbers
	uint8 type;//
}HERO_APP_EDIT_DATA;

//declare the element struct of dynamic menu win
typedef struct _HERO_APP_LIST_ITEM_T
{
	uint16 index;
	int16 key;
	
	struct _HERO_APP_LIST_ITEM_T *nextItem;
}HERO_APP_LIST_ITEM_T;

//declare dynamic menu win private data struct 
typedef struct
{
	uint8 type;
	uint8 maxItemNum;//item number
	int16 curAddItemIdx;
	MMI_STRING_T title;
	int16 curFocusItemIdx;
	int16 preFocusItemIdx;
	HERO_APP_LIST_ITEM_T *listElem;
}HERO_APP_MENU_DATA;

//no used now
typedef struct
{
	void *dummy;
}HERO_APP_APP_DATA;

//declare user data's struct add to indicated win
typedef struct
{
	MMI_HERO_APP_DYN_WIN_TYPE_E dynWinType;
	MMI_WIN_ID_T winId;
	MMI_CTRL_ID_T ctrlId;//from we used, one ctrl element enough

	union
	{
		HERO_APP_DIALOG_DATA dialog;
		HERO_APP_TEXT_DATA text;
		HERO_APP_EDIT_DATA edit;
		HERO_APP_MENU_DATA menu;
		HERO_APP_APP_DATA app;	
	}u;

}MMI_HERO_APP_ADD_PARAMETERS_T;

//declare dialog data's struct
typedef struct _DIALOG_PARAMETERS_T
{
	MMI_WIN_ID_T winId;
	HERO_APP_DIALOG_DATA dialog;
	struct _DIALOG_PARAMETERS_T *next;
}MMI_HERO_APP_DIALOG_PARAMETERS_T;

typedef struct
{
	uint32	opSize;
 	void		*cb;      //回调函数
	uint32	cb_param;               //回调参数
}MMI_HERO_APP_ASYN_FS_PARAM_T;

typedef struct
{
 	void		*cb;      //回调函数
	uint32	cb_param;               //回调参数
	int32	result;
}MMI_HERO_APP_ASYN_FS_RESULT_T;

typedef struct
{
 	void		*cb;      //回调函数
	int32	result;
}MMI_HERO_APP_NETINIT_RESULT_T;

typedef struct
{
	uint32 audioHdle;
	uint32 curSetPlayPos;
	uint32 curSetPlayTime;
	int32 totalLen;
	int32 totalTime;
	int32 uiSampleRate;
	int8 soundVolume;
	HR_AUDIO_STATUS_T curStatus;
	//HRAPP_AUDIO_OFFSET_T curTimePos;
	HRAPP_AUDIO_OFFSET_T curDataPos;
	
	MMISRVAUD_RING_FMT_E ring_type;
	HR_SRC_TYPE src_type;
	uint8 *name;
	uint32 dataLen;
		
}HERO_APP_AUDIO_MNG_T;

typedef struct
{
	HR_AUDIO_STATUS_T curStatus;
	HRAPP_AUDIO_OFFSET_T dataPos;		
}HERO_APP_PCM_MNG_T;

typedef struct
{
	uint8 msg[2];//信息提示
	uint8 voltage[2];//低压警告
	uint8 flip[2];//翻盖音
	uint8 call[2];//来电提示
	uint8 alarm[2];//闹钟提示
}HERO_APP_ENV_CONFIG_T;

typedef struct
{
	int32 count;
}HERO_APP_ENVINFO_T;

typedef struct
{
	T_HR_PHB_ENTRY info;
	int8 result;
	int8 haveBack;
    BOOLEAN needInform;
}HERO_APP_PBINFO_T;

typedef struct
{
	uint8 devSel;
	int32 simSel;	
	uint8 dummy;
}HERO_APP_DUALSYS_MNG_T;

typedef struct
{
	BOOLEAN setted;
	uint32 curCpuClk;	
	uint32 curCpuClkParam;
	uint32 orgCpuClk;
	uint32 orgCpuClkParam;
}HERO_APP_CPUCLK_MNG_T;

typedef struct 
{
	uint8* data;
	uint32 dataLen;
	int8 type;
	int8 loop;
	uint8 Ckcount;
}HERO_APP_MRPLAYSOUND_INFO_T;

typedef struct
{
    LCD_ANGLE_E lcd_angle;
    WINDOW_SUPPORT_ANGLE win_angle;
}hr_screen_angle_t;

//modify 20181120 begin
#define HERO_SMART_REMINDER_COUNT  (3)

typedef struct{
	unsigned char isOpen;
	unsigned char hour;
	unsigned char minute;
	unsigned char audioType;
	unsigned short reminder[32];
	unsigned char audioPath[64];
}hero_smart_reminder_nv_item;

typedef struct{
	unsigned char magic;
	hero_smart_reminder_nv_item item[HERO_SMART_REMINDER_COUNT];
}hero_smart_reminder_nv;
//modify 20181120 end

////////////////////////////////////////////////////////////
#ifdef HERO_APP_SHORTCUT_SUPPORT
#define HR_SHORCUT_MAGIC  (0X18)
#define HERO_SHORTCUT_MENU_COUNT  (5)
typedef struct tagShortcutMenuItemSt{
	unsigned char isShow;
	char reserved1;
	char reserved2;
	char reserved3;
	int appid;
	char entry[64];
	char menuName[32];
}heroShortcutMenuItemSt;

typedef struct tagShortcutMenuSt{
	unsigned short magic;
	char reserved2;
	char reserved3;
	heroShortcutMenuItemSt item[HERO_SHORTCUT_MENU_COUNT];
}heroShortcutMenuSt;
#endif

#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
#define NVRAM_OLALA_SLIM_CACHE_SIZE  128
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTINS                                         *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : the mr app entity entry
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
BOOLEAN MMIHEROAPP_HrAppRun(void* param, uint32 appId);

/*****************************************************************************/
// 	Description :根据动态ID 创建一需求窗体
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
int32 MMIHEROAPP_createADynWin(MMI_HERO_APP_ADD_PARAMETERS_T *addData, MMI_STRING_T *textInfo);

/*****************************************************************************/
// 	Description : stop the timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_timerStop(int32 t);

/*****************************************************************************/
// 	Description : start a timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_timerStart(uint16 t);

/*****************************************************************************/
// 	Description : start a vibrator
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_startShake(int32 t);

/*****************************************************************************/
// 	Description : start a timer for gprs activing timeout protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_gprsProtectTimerStart(uint16 t);

/*****************************************************************************/
// 	Description : stop the protect timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_gprsProtectTimerStop(void);

/*****************************************************************************/
// 	Description : disconnect the gprs launched by mr app
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
void MMIHEROAPP_CloseHrNet(void);

/*****************************************************************************/
// 	Description : report the rerult to mr app
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
void MMIHEROAPP_netInitResult(int32 result);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
int32 MMIHEROAPP_SMSfilter(MN_SMS_ALPHABET_TYPE_E  alphabet_type, MN_SMS_USER_VALID_DATA_T *data, MN_CALLED_NUMBER_T *number);

/*****************************************************************************/
// 	Description : set root win state to opened or closed
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_setAllReady(BOOLEAN ready);

/*****************************************************************************/
// 	Description : pause mr app
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
int32 MMIHEROAPP_pauseApp(void);

/*****************************************************************************/
// 	Description : resume mr app
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
int32 MMIHEROAPP_resumeApp(void);

/*****************************************************************************/
// 	Description : set arm clk to changed
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
BOOLEAN MMIHEROAPP_CLKSetted(BOOLEAN setted, uint32 curCpuClkParam, uint32 curCpuClk);

/*****************************************************************************/
// 	Description : process ps signal
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
MMI_RESULT_E MMIHEROAPP_processPsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);


/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
MMI_HERO_APP_DIALOG_PARAMETERS_T*  MMIHEROAPP_GetPromptWinAddDataPtr(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description :创建一dialog win
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
int32 MMIHEROAPP_createPromptWin(MMI_HERO_APP_DIALOG_PARAMETERS_T *addData, MMI_STRING_T *textInfo);

/*****************************************************************************/
// 	Description : init local resource
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_stopShake(void);

/*****************************************************************************/
// 	Description :设置默认文件设备与目录
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
void MMIHEROAPP_setDefFilePath(void);

/*****************************************************************************/
// 	Description :保存mrp
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
int32 MMIHEROAPP_savePack(uint8 *p,uint32 l);

/*****************************************************************************/
// 	Description : get according icon of a item
//	Global resource dependence : none
//  Author: qgp
//	Note: item icon 数目依赖平台定义了的数目，当前为20
/*****************************************************************************/
MMI_IMAGE_ID_T MMIHEROAPP_getItemIcon(MMI_HERO_APP_MENUICON_OP_TYPE_E opType, uint16 index);

/*****************************************************************************/
// 	Description : 初始化资源
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
void MMIHEROAPP_initAllExtraSrc(void);

/*****************************************************************************/
// 	Description : 检查并释放所有使用中的资源
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
void MMIHEROAPP_freeAllExtraSrc(void);

/*****************************************************************************/
// 	Description : 进入某些应用中可事先选择pause mp3播放。
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
void MMIHEROAPP_pauseMp3(void);

/*****************************************************************************/
// 	Description : 应用过程中恢复MP3的函数。
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
void MMIHEROAPP_resumeMp3(void);

/*****************************************************************************/
// 	Description :清空socket对列
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIHEROAPP_freeAllSnode(void);

/*****************************************************************************/
// 	Description : free mr mmi res
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_freeMmiRes(void);

/*****************************************************************************/
// 	Description : exit vm ,because use hr_stop() in this function, so if trigger by mr app, must use Asyn
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_exitMrApp(BOOLEAN Asyn);

/*****************************************************************************/
// 	Description : re-dail
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
void MMIHEROAPP_redail(void);

/*****************************************************************************/
// 	Description : 当前启动的应用
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
void MMIHEROAPP_setCurApp(HERO_APP_APPENTRY_E app);

/*****************************************************************************/
// 	Description : add for hr_playSound() protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_startPlaySoundProtect(void);

/*****************************************************************************/
// 	Description : add for hr_playSound() protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_StopPlaySoundProtect(void);

/*****************************************************************************/
// 	Description : 检查hr_playSound() 启动的playing是否被外部中断。
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
void MMIHEROAPP_playingCheck(void);

/*****************************************************************************/
// 	Description : start a timer for gprs activing timeout protect
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
uint8 MMIHEROAPP_gprsRetryTimerStart(uint16 t);

/*****************************************************************************/
// 	Description : stop the protect timer
//	Global resource dependence : none
//  Author: qgp
//	Note:  
/*****************************************************************************/
void MMIHEROAPP_gprsRetryTimerStop(void);

/*****************************************************************************/
// 	Description : close mr base win及其上启动的所有win.
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
BOOLEAN MMIHEROAPP_closeAllWins(void);


/*****************************************************************************/
// 	Description : 提高cpu clk
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIHEROAPP_SetArmClkHigh(void);

/*****************************************************************************/
// 	Description : 恢复clk到设置前状态
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIHEROAPP_RestoreARMClk(void);

/*****************************************************************************/
// 	Description : 为了向前版本兼容
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIHEROAPP_GB2UCS(uint16 *ucs2_ptr, const uint8 *gb_ptr, uint16 len);

/*****************************************************************************/
//  Description : handle file browser win message
//  Global resource dependence :
//  Author: zack.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIHEROAPP_HandleFileBrowserWinMsg(
													 MMI_WIN_ID_T          win_id,
													 MMI_MESSAGE_ID_E      msg_id,
													 DPARAM                param
													 );

PUBLIC void MMIHEROAPP_setLcdAsynMode(int32 isSet);

//2009-7-7 add begin
PUBLIC uint8 MMIHEROAPP_startGetApnAccountTimer(void);
PUBLIC void MMIHEROAPP_getApnAccountTimerStop(void);
PUBLIC uint8 MMIHEROAPP_startSetApnAccountTimer(void);
PUBLIC void MMIHEROAPP_setApnAccountTimerStop(void);
//2009-7-7 add end

//2010-06-24 add begin
PUBLIC uint8 MMIHEROAPP_StartGetSCNumberTimer(void);
PUBLIC void MMIHEROAPP_GetSCNumberTimerStop(void);
//2010-06-24 add end

/* begin:added by Tommy.yan 20120120 */
PUBLIC uint8 MMIHEROAPP_StartGetSMSCapacityTimer(void);
PUBLIC void MMIHEROAPP_GetSMSCapacityTimerStop(void);
PUBLIC uint8 MMIHEROAPP_StartGetSMSContentTimer(void);
PUBLIC void MMIHEROAPP_GetSMSContentTimerStop(void);
PUBLIC uint8 MMIHEROAPP_StartDeleteSMSTimer(void);
PUBLIC void MMIHEROAPP_DeleteSMSTimerStop(void);
PUBLIC uint8 MMIHEROAPP_StartSetSMSStateTimer(void);
PUBLIC void MMIHEROAPP_SetSMSStateTimerStop(void);
/* end:added by Tommy.yan 20120120 */

PUBLIC uint8 MMIHEROAPP_StartStatusIconTimer(void);
PUBLIC void MMIHEROAPP_StopStatusIconTimer(void);
PUBLIC void MMIHEROAPP_HandleStatusIconTimer(void);

/*2009-10-22 111,17787 add. for overseas*/
void MMIHEROAPP_initApn(void);
void MMIHEROAPP_saveApn(void);
/*2009-10-22 111,17787 add end*/

int32 hr_layer_create(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_layer_set_postion(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_layer_get_base_handle(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_layer_release(int32 param);
int32 hr_layer_set_active(int32 param);
int32 hr_layer_set_blt_layer(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_layer_get_active_layer(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);
int32 hr_layer_set_alpha(BOOLEAN enable, int32 param);
int32 hr_layer_set_transparent(BOOLEAN enable, int32 param);

int32 hr_fs_get_filename(char *fullpath, const char *filename);

int32 hr_fs_get_free_space(uint8 *input, int32 input_len, uint8** output, int32 *output_len, HR_PLAT_EX_CB *cb);
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern uint32  g_hr_curApp;
extern BOOLEAN g_hr_BLmallocingAtMrModule;
extern BOOLEAN s_hr_allReady;
extern BOOLEAN s_hr_VMReady;
extern int32 g_hr_phoneModState[HERO_APP_MD_MAXNUM];
#ifdef HR_BINDTO_VIRTUAL_WIN
extern uint32 g_hr_vmBackRun;
#endif
extern BOOLEAN s_hr_inMrSelfWinCreating;
extern BOOLEAN s_hr_PBOfCancelMsg;
extern BOOLEAN s_hr_isWindowOfPB;
extern BOOLEAN s_hr_isReadBySMS;
extern uint8 s_hr_fbhup2idle;
extern hr_screen_angle_t g_hr_screen_angle;



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


#ifdef   __cplusplus
   }
#endif

#endif/*_MMI_HERO_ENGINE_INTERNAL_H_*/

