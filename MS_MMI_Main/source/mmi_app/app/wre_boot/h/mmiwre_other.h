#ifndef     _MMIWRE_OTHER_H_
#define     _MMIWRE_OTHER_H_


#ifndef WRE_KERNEL_MODULE
#include "mmi_module.h"
#endif


#define WRE_MAX_PATH            255
#define WRE_SD_DISK             L"e"
/* adapter MOCOR window return value */
#define WRE_RESULT_FALSE        0
#define WRE_RESULT_TRUE         1

#define WRE_MSG_TP_PRESS                0x1000                        // press the touch panel
#define WRE_MSG_TP_PRESS_DOWN           (WRE_MSG_TP_PRESS + 0x01)     // press down the touch panel 
#define WRE_MSG_TP_PRESS_UP             (WRE_MSG_TP_PRESS + 0x02)     // press up the touch panel 
#define WRE_MSG_TP_PRESS_MOVE           (WRE_MSG_TP_PRESS + 0x03)     // 
#define WRE_MSG_TP_PRESS_LONG           (WRE_MSG_TP_PRESS + 0x04)     // 
#define WRE_MSG_TP_PRESS_DOUBLE         (WRE_MSG_TP_PRESS + 0x05)     // press the touch panel long
#define WRE_MSG_TP_PRESS_DRAG           (WRE_MSG_TP_PRESS + 0x06)
#define WRE_MSG_TP_GESTURE              (WRE_MSG_TP_PRESS + 0x07)
#define WRE_MSG_TP_SDPLUG_OUT           (WRE_MSG_TP_PRESS + 0x08)

#define WRE_MSG_TP_VOL_UP               (WRE_MSG_TP_PRESS + 0x09)//added by leichi 20120416
#define WRE_MSG_TP_VOL_DW               (WRE_MSG_TP_PRESS + 0x0A)//added by leichi 20120416
// Gesture command ID as follow,may see windowsmobile
#define WRE_GID_BEGIN           1
#define WRE_GID_END             2
#define WRE_GID_PAN             4
#define WRE_GID_SCROLL          5
#define WRE_GID_HOLD            9
#define WRE_GID_SELECT          10
#define WRE_GID_DOUBLESELECT    11
#define WRE_GID_ZOOM_IN         12
#define WRE_GID_ZOOM_OUT        13

#define WRE_KEY_PRESSED                 0x1100          /*!< key pressed */
#define WRE_KEY_RELEASED                0x1200          /*!< key released */      
#define WRE_KEY_LONG_PRESSED            0x1300          /*!< key long pressed */   
#define WRE_KEY_LONG_RELEASED           0x1400          /*!< key long released */   
#define WRE_KEY_REPEATED                0x1500          /*!< key repeated */   

#define WRE_MSG_WIN                     0x2000
#define WRE_MSG_TIMER                   WRE_MSG_WIN
//window
#define WRE_MSG_OPEN_WINDOW             (WRE_MSG_WIN + 0x1)
#define WRE_MSG_CLOSE_WINDOW            (WRE_MSG_WIN + 0x2)
#define WRE_MSG_LOSE_FOCUS              (WRE_MSG_WIN + 0x3)
#define WRE_MSG_GET_FOCUS               (WRE_MSG_WIN + 0x4)
#define WRE_MSG_FULL_PAINT              (WRE_MSG_WIN + 0x5)
#define WRE_MSG_LCD_SWITCH              (WRE_MSG_WIN + 0x6)
#define WRE_MSG_PRE_FULL_PAINT          (WRE_MSG_WIN + 0x7)
#define WRE_MSG_GSENSOR                 (WRE_MSG_WIN + 0x8)
#define WRE_MSG_BK_TURN_ON              (WRE_MSG_WIN + 0x9)
#define WRE_MSG_BK_TURN_OFF             (WRE_MSG_WIN + 0xa)
#define WRE_MSG_RTC_MIN                 (WRE_MSG_WIN + 0xb)
#define WRE_MSG_RTC_HOUR                (WRE_MSG_WIN + 0xc)
#define WRE_MSG_CHR_WARNING             (WRE_MSG_WIN + 0xd)    

#define WRE_MSG_VOL_UP                  (WRE_MSG_WIN + 0xe) //defined by leichi 20120416
#define WRE_MSG_VOL_DW                  (WRE_MSG_WIN + 0xf)//defined by leichi 20120416
//blue tooth
#define WRE_MSG_BT                          (WRE_MSG_WIN + 0x100)
#define WRE_BT_ME_ON_CNF                    (WRE_MSG_BT + 0x1)
#define WRE_BT_ME_OFF_CNF                   (WRE_MSG_BT + 0x2)
#define WRE_BT_VISIBILE_CNF                 (WRE_MSG_BT + 0x3)
#define WRE_BT_HIDDEN_CNF                   (WRE_MSG_BT + 0x4)
#define WRE_BT_INQ_DEV_ADDR                 (WRE_MSG_BT + 0x5)
#define WRE_BT_INQ_DEV_NAME                 (WRE_MSG_BT + 0x6)
#define WRE_BT_INQ_COMP_CNF                 (WRE_MSG_BT + 0x7)
#define WRE_BT_INQUIRY_CANCEL               (WRE_MSG_BT + 0x8)
#define WRE_BT_DEV_PIN_REQ                  (WRE_MSG_BT + 0x9)
#define WRE_BT_DEV_PAIR_COMPLETE            (WRE_MSG_BT + 0xa)
#define WRE_BT_DEV_SERVICE_RESULT           (WRE_MSG_BT + 0xb)
#define WRE_BT_A2DP_SRC_CONFIG_STREAM       (WRE_MSG_BT + 0xc)
#define WRE_BT_A2DP_SRC_OPEN_STREAM         (WRE_MSG_BT + 0xd)
#define WRE_BT_A2DP_SRC_START_STREAM        (WRE_MSG_BT + 0xe)
#define WRE_BT_A2DP_SRC_SUSPEND_STREAM      (WRE_MSG_BT + 0xf)
#define WRE_BT_A2DP_SRC_CLOSE_STREAM        (WRE_MSG_BT + 0x10)
#define WRE_BT_HFAG_SLC_CONNECTED           (WRE_MSG_BT + 0x11)
#define WRE_BT_HFAG_SLC_DISCONNECTED        (WRE_MSG_BT + 0x12)
#define WRE_BT_HFAG_DIAL_NUMBER             (WRE_MSG_BT + 0x13)
#define WRE_BT_HFAG_DIAL_MEMORY             (WRE_MSG_BT + 0x14)
#define WRE_BT_HFAG_REDAIL_CALL             (WRE_MSG_BT + 0x15)
#define WRE_BT_HFAG_HFU_HOLD                (WRE_MSG_BT + 0x16)
#define WRE_BT_HFAG_AUDIO_CONNECTED         (WRE_MSG_BT + 0x17)
#define WRE_BT_HFAG_AUDIO_DISCONNECTED      (WRE_MSG_BT + 0x18)
#define WRE_BT_HFU_DTMF_NUMBER              (WRE_MSG_BT + 0x19)
#define WRE_BT_HFU_CALL_ANSWER              (WRE_MSG_BT + 0x1a)
#define WRE_BT_HFU_CALL_HANGUP              (WRE_MSG_BT + 0x1b)
#define WRE_BT_HFU_STATUS_REQ               (WRE_MSG_BT + 0x1c)
#define WRE_BT_HS_MIC_VOL_CHANGE            (WRE_MSG_BT + 0x1d)
#define WRE_BT_HS_SPK_VOL_CHANGE            (WRE_MSG_BT + 0x1e)
#define WRE_BT_OPC_CONNECT_RESULT           (WRE_MSG_BT + 0x1f)
#define WRE_BT_OPC_SERVER_ABORT             (WRE_MSG_BT + 0x20)
#define WRE_BT_OPC_PUT_ABORT                (WRE_MSG_BT + 0x21)
#define WRE_BT_OPC_SEND_RES                 (WRE_MSG_BT + 0x22)
#define WRE_BT_FTS_GET_REQ                  (WRE_MSG_BT + 0x23)
#define WRE_BT_OPS_PUT_REQ                  (WRE_MSG_BT + 0x24)
#define WRE_BT_OPS_CLIENT_ABORT             (WRE_MSG_BT + 0x25)
#define WRE_BT_OPS_PUT_ABORT                (WRE_MSG_BT + 0x26)
#define WRE_BT_SEND_CANCELED                (WRE_MSG_BT + 0x27)
#define WRE_BT_RECEIVE_CANCELED             (WRE_MSG_BT + 0x28)
#define WRE_BT_FTS_SHARED_FOLDER_UPDATE     (WRE_MSG_BT + 0x29)
#define WRE_BT_FTS_PUT_REQ                  (WRE_MSG_BT + 0x2a)
#define WRE_BT_FTS_CLIENT_ABORT             (WRE_MSG_BT + 0x2b)
#define WRE_BT_FTS_PUT_ABORT                (WRE_MSG_BT + 0x2c)
#define WRE_BT_DELETE_DEVICE_RES            (WRE_MSG_BT + 0x2d)
#define WRE_BT_PACKET_SENT                  (WRE_MSG_BT + 0x2e)
#define WRE_BT_PACKET_RECEIVED              (WRE_MSG_BT + 0x2f)
#define WRE_BT_RECEIVE_FILE_RES             (WRE_MSG_BT + 0x30)
#define WRE_BT_FTS_DEL_REQ                  (WRE_MSG_BT + 0x31)
#define WRE_BT_FTS_DEL_FAIL                 (WRE_MSG_BT + 0x32)
#define WRE_BT_FTS_ADD_FOLDER_REQ           (WRE_MSG_BT + 0x33)
#define WRE_BT_DUN_LINK_REQ                 (WRE_MSG_BT + 0x34)
#define WRE_BT_DUN_LINK_SUC                 (WRE_MSG_BT + 0x35)
#define WRE_BT_DUN_LINK_CLOSE               (WRE_MSG_BT + 0x36)
#define WRE_BT_A2DP_OPERATION_BACKWARD      (WRE_MSG_BT + 0x37)
#define WRE_BT_A2DP_OPERATION_FORWARD       (WRE_MSG_BT + 0x38)
#define WRE_BT_A2DP_OPERATION_VOL_UP        (WRE_MSG_BT + 0x39)
#define WRE_BT_A2DP_OPERATION_VOL_DOWN      (WRE_MSG_BT + 0x3a)
#define WRE_BT_A2DP_OPERATION_PLAY          (WRE_MSG_BT + 0x3b)
#define WRE_BT_A2DP_OPERATION_STOP          (WRE_MSG_BT + 0x3c)
#define WRE_BT_A2DP_OPERATION_PAUSE         (WRE_MSG_BT + 0x3d)
#define WRE_BT_AVRCP_DISCONNECT             (WRE_MSG_BT + 0x3e)
#define WRE_BT_AVRCP_CONNECT                (WRE_MSG_BT + 0x3f)
#define WRE_BT_A2DP_DISCONNECTED            (WRE_MSG_BT + 0x40)
#define WRE_BT_OPEN_DEVICE_CNF              (WRE_MSG_BT + 0x41)
#define WRE_BT_RESUME_DEVICE_CNF            (WRE_MSG_BT + 0x42)
#define WRE_BT_SUSPEND_DEVICE_CNF           (WRE_MSG_BT + 0x43)
#define WRE_BT_A2DP_START_STREAM_IND        (WRE_MSG_BT + 0x44)
#define WRE_BT_A2DP_SUSPEND_STREAM_IND      (WRE_MSG_BT + 0x45)
#define WRE_BT_A2DP_RESUME_STREAM_IND       (WRE_MSG_BT + 0x46)
#define WRE_BT_A2DP_ABORT_IND               (WRE_MSG_BT + 0x47)
#define WRE_BT_A2DP_ABORT_CFM               (WRE_MSG_BT + 0x48)
#define WRE_BT_CLOSE_DEVICE_CNF             (WRE_MSG_BT + 0x49)
#define WRE_BT_PAUSE_STOP_DEVICE_CNF        (WRE_MSG_BT + 0x4a)
#define WRE_BT_PLAY_RESUNME_DEVICE_CNF      (WRE_MSG_BT + 0x4b)
#define WRE_BT_FTS_CONNECT_REQ              (WRE_MSG_BT + 0x4c)
#define WRE_BT_OPS_CONNECT_REQ              (WRE_MSG_BT + 0x4d)

#define WRE_MSG_USER_START              0x3000

/* key code */
#define WRE_KEY_NONE                0x00                /*!< press none */                    
#define WRE_KEY_UP                  0x01                /*!< press [up] */
#define WRE_KEY_DOWN                0x02                /*!< press [down] */
#define WRE_KEY_LEFT                0x03                /*!< press [left] */
#define WRE_KEY_RIGHT               0x04                /*!< press [right] */
#define WRE_KEY_GREEN               0x05                /*!< press [Green] */
#define WRE_KEY_CAMREA              0x06                /*!< press [CAMERA] */

#define WRE_KEY_1                   0x08                /*!< press [1] */
#define WRE_KEY_2                   0x09                /*!< press [2] */
#define WRE_KEY_3                   0x0a                /*!< press [3] */
#define WRE_KEY_4                   0x0b                /*!< press [4] */
#define WRE_KEY_5                   0x0c                /*!< press [5] */
#define WRE_KEY_6                   0x0d                /*!< press [6] */
#define WRE_KEY_7                   0x0e                /*!< press [7] */
#define WRE_KEY_8                   0x0f                /*!< press [8] */
#define WRE_KEY_9                   0x10                /*!< press [9] */
#define WRE_KEY_STAR                0x11                /*!< press [*] */
#define WRE_KEY_0                   0x12                /*!< press [0] */
#define WRE_KEY_HASH                0x13                /*!< press [#] */
#define WRE_KEY_SPUP                0x14
#define WRE_KEY_SPDW                0x15
#define WRE_KEY_OK                  0x16                /*!< press left softkey(OK) */
#define WRE_KEY_CANCEL              0x17                /*!< press right softkey (Cancel) */
#define WRE_KEY_RED                 0x18                /*!< press red key */
#define WRE_KEY_UPSIDEKEY           0x19                /*!< press up side key */
#define WRE_KEY_DOWNSIDEKEY         0x1a                /*!< press down side key */
#define WRE_KEY_WEB                 0x1b                /*!< press web key */
#define WRE_KEY_GPIO_SIG1           0x1c
#define WRE_KEY_GPIO_SIG2           0x1d
#define WRE_KEY_FLIP                0x1e                /*!< flip gpio  */
#define WRE_KEY_HEADSET_BUTTIN      0x1f                /*!< handset gpio buttion */
#define WRE_KEY_HEADSET_DETECT      0x20                /*!< handset gpio detect */
#define WRE_KEY_SDCARD_DETECT       0x21                /*!< SD card detct */
#define WRE_KEY_VIDEO_TEL           0x22
#define WRE_KEY_PLAYANDSTOP         0x23
#define WRE_KEY_FORWARD             0x24
#define WRE_KEY_BACKWARD            0x25
#define WRE_KEY_MIDDLE              0x26
#define WRE_KEY_AT                  0x27
// QWERT_ADD
#define WRE_KEY_Q                   0x28  
#define WRE_KEY_W                   0x29
#define WRE_KEY_E                   0x2a
#define WRE_KEY_R                   0x2b
#define WRE_KEY_T                   0x2c
#define WRE_KEY_Y                   0x2d
#define WRE_KEY_U                   0x2e
#define WRE_KEY_I                   0x2f
#define WRE_KEY_O                   0x30
#define WRE_KEY_P                   0x31
#define WRE_KEY_A                   0x32
#define WRE_KEY_S                   0x33
#define WRE_KEY_D                   0x34
#define WRE_KEY_F                   0x35
#define WRE_KEY_G                   0x36
#define WRE_KEY_H                   0x37
#define WRE_KEY_J                   0x38
#define WRE_KEY_K                   0x39
#define WRE_KEY_L                   0x3a
#define WRE_KEY_DEL                 0x3b
#define WRE_KEY_Z                   0x3c
#define WRE_KEY_X                   0x3d
#define WRE_KEY_C                   0x3e
#define WRE_KEY_V                   0x3f
#define WRE_KEY_B                   0x40
#define WRE_KEY_N                   0x41
#define WRE_KEY_M                   0x42
#define WRE_KEY_COMMA               0x43
#define WRE_KEY_PERIOD              0x44
#define WRE_KEY_ENTER               0x45
#define WRE_KEY_FN                  0x46
#define WRE_KEY_SHIFT               0x47
#define WRE_KEY_AT_QWERTY           0x48
#define WRE_KEY_SPACE               0x49
#define WRE_KEY_AND                 0x4a
#define WRE_KEY_QUESTION            0x4b
#define WRE_KEY_CTRL                0x4c
#define WRE_KEY_PLUS                0x4d  // +
#define WRE_KEY_LEFT_PARENTHESIS    0x4e  // (
#define WRE_KEY_RIGHT_PARENTHESIS   0x4f  // )
#define WRE_KEY_MINUS               0x50  // -
#define WRE_KEY_DOUBLE_QUOTES       0x51  // "
#define WRE_KEY_SEMICOLON           0x52  // ;
#define WRE_KEY_COLON               0x53  // : 
#define WRE_KEY_SLASH               0x54  // /
#define WRE_KEY_SHIFT_AND           0x55  // &
#define WRE_KEY_EXCLAMATION         0x56  // !
// QWERT_ADD
#define WRE_KEY_SLIDE               0x57                //滑盖消息
#define WRE_KEY_TV                  0x58
#define WRE_KEY_CALL2               0x59
#define WRE_KEY_CALL3               0x5a
#define WRE_KEY_MP3                 0x5b
#define WRE_KEY_NOTES               0x5c
#define WRE_KEY_SMS                 0x5d
#define WRE_KEY_CALENDER            0x5e
#define WRE_KEY_IE                  0x5f
#define WRE_KEY_HANG                0x60
#define WRE_KEY_VOL_UP              0x61
#define WRE_KEY_VOL_DOWN            0x62
#define WRE_KEY_MO                  0x63
#define WRE_KEY_HOOK                0x64
#define WRE_KEY_HANDFREE            0x65
#define WRE_KEY_CALL4               0x66
#define WRE_KEY_POWER               0x67
#define WRE_KEY_MENU                0x68

#define WRE_KEY_MAX_NUM             0xff

#define WRE_INIT_OK                 0
#define WRE_INIT_NO_ROOM            -1
#define WRE_INIT_LOADKERNEL_FALI    -2
#define WRE_INIT_RUNNING            -3
#define WRE_INIT_APP_PARAERR        -4
#define WRE_INIT_UDISK_NORUNNING    -5
#define WRE_INIT_VIRTUALDISK_FAIL    -6
#define WRE_INIT_INCALLING          -7
#define WRE_INIT_QUERY_UPDATE       -8
#define WRE_INIT_MOCORAPP_BACKRUNNING -9
#define WRE_INIT_NOSDCARD           -10

#define  WRE_NORMAL_EXIT_PATH            0
#define  WRE_FROM_HOMEKEY_EXIT_PATH      1


typedef uint32                  WRE_HANDLE_T;

typedef struct _tagWRE_DOWNLOAD_PARAM_T
{
    uint32    simNo;
    uint32    is_wifi_used;
    uint32    link_index;
    uint32    is_net_type;
    char      server_ip[32];
    char      cmd[64];
    
}WRE_DOWNLOAD_PARAM_T,*PWRE_DOWNLOAD_PARAM_T;


typedef void (* WRE_NETWORKSEL_FUNC)(PWRE_DOWNLOAD_PARAM_T pDLParam, BOOLEAN bSimSelSucc);
int WRE_NetworkSelectGUI(WRE_NETWORKSEL_FUNC pWreNetWorkSelFunc,BOOLEAN downloading_part2);
int WRE_DownLoad_Window_Progress(int percent);

typedef int (* WRE_START_UPDATA_FUNC)(PWRE_DOWNLOAD_PARAM_T pDLParam, char *filename);
typedef int (* WRE_YY_EXIT_APP_FUNC)(int exitcmd);

void WRE_Get_Window_YY_Message(WRE_START_UPDATA_FUNC wre_start_updata,WRE_YY_EXIT_APP_FUNC yy_exit_app,char *filename,int strcode);


typedef enum
{
    MMICAF_WIN_ID_START = (MMI_MODULE_WRE << 16),
        MMIMWIN_WIN_ID,
        MMIMWIN_WRE_TMP_ID,  //heng.xiao add 
        MMICAF_MAX_WIN_ID
}MMICAF_WINDOW_ID_E;

typedef enum
{
    WRE_TP_NONE = 0,
    WRE_TP_DOWN = 1,
    WRE_TP_UP   = 2,
    WRE_TP_MOVE = 3,
    WRE_TP_LONG = 4,
    WRE_TP_GESTURE = 5,
    WRE_TP_STATUS_MAX
}WRE_TP_STATUS_E;

typedef struct 
{
    uint8   mday;       // day of the month - [1,31] 
    uint8   mon;        // months  - [1,12] 
    uint16  year;       // years [2000,2049] 
    uint8   wday;       // days since Sunday - [0,6], (Sunday : 0, Monday : 1)
}WRE_DATE_T;

typedef struct 
{
    uint8   sec;        // seconds after the minute - [0,59] 
    uint8   min;        // minutes after the hour - [0,59] 
    uint8   hour;       // hours since midnight - [0,23] 
}WRE_TIME_T;

/* for version information */
#define     VERSIN_LEN      32

typedef struct strVERSION
{
    char version[VERSIN_LEN];
}VERSION_T;

typedef enum tagDIAL_TYPE
{
    DIAL_WAP = 0,
    DIAL_NET
}DIAL_TYPE;

typedef enum
{
    WRE_DEVICE_SYSTEM,    /*!< C盘，不显示给用户，只供应用保存配置文件 */
    WRE_DEVICE_UDISK,     /*!< U盘 */
    WRE_DEVICE_SDCARD_1,  /*!< 卡1 */ 
    WRE_DEVICE_SDCARD_2,  /*!< 卡2 */ 
    WRE_DEVICE_MAX
}WRE_FILE_DEVICE_E;

typedef struct tagPRELOAD_INFO 
{
    uint16   PreloadPath[32];
    uint8    PreloadType;
}PRELOAD_INFO, *LPPRELOAD_INFO;

typedef enum
{
	WRE_SYS_PATH_KERNEL,
	WRE_SYS_PATH_WREAPP,
	WRE_SYS_PATH_WREAPP_SD,

    WRE_SYS_PATH_AUDIO,
    WRE_SYS_PATH_AUDIO_SD,

    WRE_SYS_PATH_VIDEO,
    WRE_SYS_PATH_VIDEO_SD,

    WRE_SYS_PATH_EBOOK,
    WRE_SYS_PATH_EBOOK_SD,
    
    WRE_SYS_PATH_PHOTOS,
    WRE_SYS_PATH_PHOTOS_SD,
		
    WRE_SYS_PATH_OTHERS,
    WRE_SYS_PATH_OTHERS_SD,

    WRE_SYS_PATH_STORE,
    WRE_SYS_PATH_REGISTER,

    WRE_SYS_PATH_KERNEL_UPDATE,
    WRE_SYS_PATH_STORE_UPDATE,
    WRE_SYS_PATH_KERNEL_PREV,
    WRE_SYS_PATH_STORE_PREV,

	WRE_SYS_PATH_TOTAL,
}WRE_SYS_PATH_TYPE;

WRE_FILE_DEVICE_E WRE_GetInstallDevice(uint16 *pSysPath);
uint16 WRE_GetUpdateDevice(WRE_SYS_PATH_TYPE sysPathType);
BOOLEAN WRE_IsDiskFreeSpaceEnough(WRE_SYS_PATH_TYPE sysPathType, uint16 *lpDirPath);
BOOLEAN WRE_IsSystemPath(uint16 *lpDirPath);

#define MAX_WRE_FILE_PATH_LEN        128
#ifdef WIN32
#define WRE_SYS_FILE_NAME_KERNELL     L"wrekernel.dll"
#else
#define WRE_SYS_FILE_NAME_KERNELL     L"wrekernel.wrd"
#endif

#define WRE_SYS_FILE_NAME_KERNELL_WPK   L"wrepart1.wpk"
#define WRE_SYS_FILE_NAME_KERNELL_SZIP  L"wrepart1.wpk.szip"
#define WRE_SYS_FILE_NAME_STORE_WPK     L"wrepart2.wpk"
#define WRE_SYS_FILE_NAME_STORE_SZIP    L"wrepart2.wpk.szip"
#define WRE_SYS_FILE_NAME_KERNEL_DEVICEID L"wredevice.dat"

#define WRE_SYS_FILE_NAME_APPSTORE    L"appstore.exe"

#define         CM_MAX_PATH     (148 - 1)

typedef struct tagKSHCAMERACAPTURE
{
    int         cbSize;
    int         hwndOwner;
    uint16      szFile[CM_MAX_PATH];
    uint16*     pszInitialDir;
    uint16*     pszDefaultFileName;
    uint16*     pszTitle;
    int         StillQuality;
    int         VideoTypes;
    int         nResolutionWidth;
    int         nResolutionHeight;
    int         nVideoTimeLimit;
    int         Mode;
    uint32    Mem;
}KSHCAMERACAPTURE, *PKSHCAMERACAPTURE;


#define WRE_UNINSTALLAPP_INI_NAME      L"\\wreuninstall.ini"
#define WRE_GUID_NUM    40
typedef struct _tagWRE_APPGUID
{
    char guid[WRE_GUID_NUM];
}WRE_APPGUID, *PWRE_APPGUID;

typedef struct _tagWRE_UNINSTALLAPPINI
{
    uint32 iUnInstallAppNum;
    PWRE_APPGUID   pWreAppGuid; 
}WRE_UNINSTALLAPPINI, *PWRE_UNINSTALLAPPINI;

BOOLEAN Find_App_GUID(uint32 appid,  uint8 *guid, uint8 guidNum);
BOOLEAN WriteGUIDToDeleteIni(uint8 *guid, uint8 guidNum);

#define WRE_MALLOC(size)    WRE_Malloc(size, __FILE__, __LINE__)
#define WRE_FREE(ptr)       WRE_Free(ptr)

void WRE_BootTrace(const char *string);


void WRE_Assert(void *exp, void *file, int  line);
void WRE_PAssert(void *exp, void *file, uint32  line, char *assert_info_ptr);
//uint16 *GetLcdLayer(void);
void *WRE_Malloc(uint32 size, const char * file, uint32 line);
uint32 WRE_Free(void *memory_ptr);
BOOLEAN WRETHEME_UpdateRect(void);
/*void WRE_ChangeFreqByQuickKey(void);*/
WRE_HANDLE_T GetMwinMocorWinHandle(void);
WRE_HANDLE_T GetMwinWiFiWinHandle(void);
void WRE_SetSysDate(WRE_DATE_T  wre_sys_date);
void WRE_GetSysDate(WRE_DATE_T  *date_ptr);
void WRE_SetSysTime(WRE_TIME_T  wre_sys_time);
void WRE_GetSysTime(WRE_TIME_T  *time_ptr);
void CreateMwinMangerWin(void);
void DispatchMocorSignal(void);
void DispatchMocorMSG(void);
BOOLEAN GetMwinMsgAndMocorMsg(int *mmi_msg_type_ptr);
BOOLEAN WRE_IsMwinWin(int mmi_msg_type);
long WRE_GetMessageExx(void *lpMsg, void *hwnd, uint32 wMsgFilterMin, uint32 wMsgFilterMax);
BOOLEAN WRE_MMK_PostMsg(WRE_HANDLE_T handle, uint32 msg_id, void  *param_ptr, uint32 size_of_param);
BOOLEAN WRE_MMK_CloseWin(WRE_HANDLE_T win_id);
uint16 WRE_ConvertTpXcoord(void *ptr);
uint16 WRE_ConvertTpYcoord(void *ptr);
void WRE_APICOM_OtherTaskToMMI(WRE_HANDLE_T handle, uint32 msg_id, void *param_ptr, uint32 size_of_param);
void *WRE_MEMCPY( void *dest, const void *src, uint32 count );
void* ADAPT_CFL_MspaceMalloc(void * msp_handle, int bytes);
void ADAPT_CFL_MspaceFree(void* msp_handle, void* mem_ptr);
int ADAPT_CFL_DestroyMspace(void *msp_handle);
void * ADAPT_CFL_CreateMspace(void* base_ptr, int capacity);
void ADAPT_TurnOnBackLight(void);
void ADAPT_AllowTurnOffBackLight(BOOLEAN allow_turn_off);
BOOLEAN WBOOT_GetMwinMessageForMocor(void);
void WBOOT_DispatchMwinMSG(void);
BOOLEAN WBOOT_IsMwinQuitMsg(void);
void WBOOT_ClearThreadNormalQueue(uint32 threadId);
uint32 WBOOT_GetWreCurMwinThd(void);
void DownloadAndInstallWRE(BOOLEAN bStartup);
BOOLEAN WBOOT_SetMwinMsg(void *ptr);
uint32 WBOOT_MwinWin_HandleMsg(uint32 win_id, uint32 msg_id, void *param);
void SetWatchDogPtr(void *SwdgPtr);
uint32 WreMwinWin_HandleMsg(uint32 win_id, uint32 msg_id, void *param);
void WRE_GetWREVersion(VERSION_T * version);
void WRE_GetBOOTVersion(VERSION_T * version);
int WRE_GetCHIPVersion(void);
void WRE_GetIDHID(VERSION_T * version);
void WRE_GetPRJName(VERSION_T * version);
void WRE_GetScreenSize(int* w,int* h);
const uint16 *WRE_Get_SYS_Dir(WRE_SYS_PATH_TYPE sysPathType);
BOOLEAN WRE_Set_SYS_Dir(WRE_SYS_PATH_TYPE sysPathType, uint16 *lpSysPath);
BOOLEAN ADAPT_Vibrate(uint32 duaration);
BOOLEAN ADAPT_StopVibrate(void);
int unzip(const uint16 *srcfile, uint16 *dstfile);
int unpack(const uint16 *srcfile, uint16 *dstdir);

BOOLEAN Adapt_InstallApp(uint16 * appname,
                      uint16 * guid,
                      uint16 * apppath,
                      void *   appparam,
                      uint16 * iconpath);

BOOLEAN  Adapt_UninstallApp(uint16 * guid,uint16 * apppath);
WRE_TP_STATUS_E WRE_GetTpStatus(uint16 *px, uint16 *py);
BOOLEAN  WBOOT_IsWRERunning(void);
void K_SHCameraCapture(PKSHCAMERACAPTURE pshcc);
BOOLEAN WRE_GetPhoneInfo(char* info,int len,int which);
void WRE_NETWORK_IND_Handle(void *param);
void WRE_ShowWarningBox(void);
BOOLEAN WRE_GetPhoneInfo(char* info,int len,int which);
void WRE_Init(void);
void WRE_Reset();
uint32 WRE_GetKernelMinSpace(WRE_SYS_PATH_TYPE sysPathType);

BOOLEAN Adapt_InstallJavaApp(uint16 * jarpathname,void *pCallbackFunc,void *pParam);
BOOLEAN Adapt_StartupJavaApp(int32 suiteId);
BOOLEAN Adapt_UninstallJavaApp(int32 suiteId,BOOLEAN bOnlyDyDelete);
/*****************************************************************************/
//  Description : WRE_UpdateWreRuningFlag 
//  Global resource dependence : 
//  Author:yiwen.man
//  Note: it means wre running times.
/*****************************************************************************/
void   WRE_UpdateWreRuningFlag(void);

/*****************************************************************************/
//  Description : WRE_GetWreRuningFlag 
//  Global resource dependence : 
//  Author:yiwen.man
//  Note: get current wre running flags.
/*****************************************************************************/
uint32  WRE_GetWreRuningFlag(void);

/*****************************************************************************/
//  Description : Startup a app with data 
//  Global resource dependence : 
//  Author: langbiao.tan
//  Note: mocor startup a app with data
/*****************************************************************************/
BOOLEAN WRE_App_Startup_With_Data(uint32 appid, uint16 *guid, void *data);

/*****************************************************************************/
//  Description : Set mainlcd contrast 
//  Global resource dependence : 
//  Author: langbiao.tan
//  Note: 
/*****************************************************************************/
uint8 WRE_SetMainLCDContrast(uint8 lcd_contrast);

typedef struct
{
    short    x;
    short    y;
}WRE_POINT_T;

#define WRE_TP_POINTS_MAX           5
typedef struct
{
    WRE_POINT_T points[WRE_TP_POINTS_MAX];
    unsigned short      num;
} WRE_TP_POINTS_T;

typedef struct
{
    unsigned short      keycode;
    unsigned short      keystatus;
} WRE_KEYS_T;

int WRE_Get_Gesture(void *ptr);
void WRE_GetLogicTPMsg(WRE_TP_STATUS_E *tp_status_ptr, WRE_POINT_T *point_ptr);
BOOLEAN WRE_GetTpPoints(WRE_TP_POINTS_T* points_ptr);
int WRE_GetMultiKeyMsg(WRE_KEYS_T* multi_key_msg_ptr);
uint8 WRE_GetDownLoadLinkIndex(uint32 dual_sys);
void HideWreDirectroy(BOOLEAN bHide);
void Adapt_EntryVideoPlayer(uint32 nVideoType, uint16 * strFileName, uint32  nFileNameLen, uint8  *  strUrl);
void WRE_DYN_Startup(uint32  appid, uint16 *appname);
void MMIAPIWRE_ExitWre(void);
BOOLEAN Adapt_IsBacklightOn(void);
void MMIWRE_CloseWin(void);
void MMIWRE_CloseWindow(void);
void MMIWRE_MaxmizeAPP(void* app_handle);
uint32 WRE_GetRuningStatus(void);
BOOLEAN WRE_ShowApp(void* app_handle,int opercode);
int WRE_GetAppCount(void);
BOOLEAN MMIWRE_SetAppParam(uint16 * apppath, uint16 * appname, uint16 * iconname);
void MMIWRE_InitRuntimePath(void);
void MMIWRE_ResetRuntimePath();
BOOLEAN WRE_IsRuntimeEnvRestoreFromPreload();
BOOLEAN RestoreWREEnv();
BOOLEAN RestoreFiles(WRE_SYS_PATH_TYPE sysPathType);
BOOLEAN WRE_Get_Preload_Dir(LPPRELOAD_INFO lpPreloadInfo, WRE_SYS_PATH_TYPE sysPathType);
BOOLEAN WRE_CopyFile(uint16 *dst, uint16 *src);
BOOLEAN WRE_CopyTree(uint16 *dstDir, uint16 *srcDir);
uint16 *WRE_Get_SYS_Dir_Update(WRE_SYS_PATH_TYPE sysPathType);
PUBLIC void MMIWRE_PrintNVInfo();
PUBLIC void MMIWRE_SetUdiskFlag(BOOLEAN flag);
PUBLIC BOOLEAN MMIWRE_isUsingUdisk(void);

typedef enum
{
    WRE_DEBUG_UNZIP,       //0 不解压1 解压
    WRE_DEBUG_STARTPATH,

    WRE_DEBUG_TOTAL = 255
}WRE_DEBUG_TYPE;
void MMIWRE_InitDebugMode(void);
void MMIWRE_ExitDebugMode(void);
BOOLEAN MMIWRE_GetDebugMode(void);
void MMIWRE_SetDebugMode(BOOLEAN bDebugMode);
BOOLEAN Adapt_GetDebugInfo(void *debugParam);
void WREPDP_ResetPdpProc(void);
void WRE_TraceWstr(const uint16 *lpszString);

typedef struct _tagFIXED_APP_INFO_T
{
    uint32  fixed_id;
    uint16 * guid;
    uint16 * store_app_id;
}FIXED_APP_INFO_T,*PFIXED_APP_INFO_T;

FIXED_APP_INFO_T *WRE_GetFixAppList();

typedef enum
{
    WRE_FEATURE_NETCONNECT,   //0: 不支持, 1: 支持
    WRE_FEATURE_CHECKSUM,     //0: 不支持, 1: 支持
    WRE_FEATURE_KERNEL_PRELOAD,
    WRE_FEATURE_STORE_PRELOAD,
    WRE_FEATURE_APP_PRELAOD,
    WRE_FEATURE_FIXAPP,   //1:fix 0:normal
    WRE_FEATURE_APPTYPE,  //0:app 1: store
    WRE_FEATURE_ONEKEYDOWNLOAD,   //0:不支持， 1: 支持
        
    WRE_FEATURE_TOTAL
}WRE_FEATURE_TYPE;

uint8 WRE_GetFeatureSupport(WRE_FEATURE_TYPE future_type);
PUBLIC BOOLEAN MMIWRE_SetStatusBarValue(int index, void *item_data_ptr);
PUBLIC BOOLEAN MMIWRE_HAVE_BG(int index);
PUBLIC void MAIN_UpdateIdleWREState(int index,BOOLEAN wre_state,BOOLEAN isupdate);
PUBLIC BOOLEAN MMIWRE_IsMocorAppBackRunning(void);
PUBLIC uint32 MMIWRE_IsFromHomeKeyExit(void);
PUBLIC void MMIWRE_ClearHomeKeyFlag(void);

#ifdef SNS_PULLING_SUPPORT
PUBLIC void WRE_Wakeup_IMMISNS_Release(void);
PUBLIC void WRE_StartSnsPullingTimer(int widget_id);
PUBLIC void WRE_StopSnsPullingTimer(int widget_id);
PUBLIC void WRE_SetSnsPullingTimer(int widget_id, uint32 time);
PUBLIC void WRE_SetSnsPullingPresetTimer(int widget_id, uint16 preset_type);
PUBLIC uint16 WRE_GetSnsPullingPresetType(int widget_id);
#endif //SNS_PULLING_SUPPORT

PUBLIC BOOLEAN WRE_IsFocusRunWin(void);

#endif
