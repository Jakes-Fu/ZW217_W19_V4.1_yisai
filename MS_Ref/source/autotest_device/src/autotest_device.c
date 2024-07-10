/******************************************************************************
 ** File Name:      autotest_device.c                                           *
 ** Author:         Jackey.Ling                                              *
 ** DATE:           2012.05.04                                                *
 ** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2012.05.04    Jackey.Ling         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "tb_dal.h"
#include "cmddef.h"
#include "tasks_id.h"
#include "caf.h"

#include "autotest_device.h"
#include "autotest_audio.h"

#include "diag.h"
#include "at_common.h"
#include "ipc_drvapi.h"

#ifdef CHIP_VER_6530
#include "sc6530_reg_base.h"
#include "sc6530_module_ver.h"
#include "sc6530_int_cfg.h"
#include "sc6530_gpio_cfg.h"
#endif

#ifdef CHIP_VER_6531
#include "sc6531_int_cfg.h"
#endif

#ifdef CHIP_VER_6531EFM
#include "sc6531efm_int_cfg.h"
#endif

#if !defined(CAMERA_SUPPORT_NONE)|| !defined(ATV_SUPPORT_NONE)
#include "deep_sleep.h"
#include "sensor_drv.h"
#include "dal_dcamera.h"
#endif

#include "i2c_drvapi.h"

#include "sci_types.h"
#include "sci_api.h"
#include "kpd_drvapi.h"

#include "pinmap_drvapi.h"

#include "dal_lcd.h"
#include "lcm_drvapi.h"

#include "audio_api.h"
#include "audio_drvapi.h"
#include "gpio_drvapi.h"
#include "gpio_prod_api.h"

#include "gpio_phy.h"
#include "analog_drvapi.h"

#if !defined(FM_SUPPORT_NONE)
#include "fm_drv.h"
#endif

#if !defined(BLUETOOTH_SUPPORT_NONE)
#include "bt_abs.h"
#endif

#include "chg_drvapi.h"
#include "chip_drv_common_inter.h"
#include "prod_param.h"
#include "chip_drv_common_io.h"

#if !defined(WIFI_SUPPORT_NONE)
#include "wifi_api.h"
#endif

#include "adi_hal_internal.h"
#include "sfs.h"

#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
#include "gps_drv.h"
#include "gps_interface.h"
#endif
#include "scm_api.h"

#if defined(PLATFORM_UWS6121E)
#include "../../../../chip_drv/chip_module/pin/uws6121e/pinmap.h"
#endif

#include "ldo_drvapi.h"
#include "nv_item_id.h"
#include "Thermal.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
//#define AUTOTEST_DEBUG

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define     KEYPAD_NUM_MAX          64
#define     CHARGE_ADC_VALUE        200
#define     MAX_DEVICE_DISCOVERY    20

#define     SCI_FAILURE       0x01

#define     MAINLCD_ID		             0x0
#define     AUTODEV_I2C_WRITE_ADDR       0xa0

#define     CAM_EXT_BUF_LEN        (50*1024)
#define     ATV_EXT_BUF_LEN        (50*1024)
#define     VIDEO_BUF_LEN            (160*1024)
#define     CAM_SEND_DATA_LENGTH    240
#define     ATV_SEND_DATA_LENGTH   (120*160)

#define     AUTOTEST_NVRAM_LEN        8
#define     MICBUFF_START_ADDR         800
#define     MIC_BUFFLEN                400

#define     DC_RIGHT_TRUNK(x, y)    (((x)+(y)-1)/(y)*(y))
#define	    RTN_IF_ERR(n)				if(n) goto exit

#define     GLOBAL_DIAG_COMMAND
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

typedef enum  _DEVICE_AUTOTEST_ID_E
{
    DEVICE_AUTOTEST_KEYPAD = 1,
    DEVICE_AUTOTEST_LCD_PARALLEL = 2,
    DEVICE_AUTOTEST_LCD_SPI = 3,
    DEVICE_AUTOTEST_CAMERA_IIC = 4,
    DEVICE_AUTOTEST_CAMERA_PARALLEL = 5,
    DEVICE_AUTOTEST_CAMERA_SPI = 6,
    DEVICE_AUTOTEST_GPIO =7,  //and TP test
    DEVICE_AUTOTEST_TF = 8,
    DEVICE_AUTOTEST_SIM = 9,
    DEVICE_AUTOTEST_MIC = 10,
    DEVICE_AUTOTEST_SPEAK = 11, //speak&&receiver&&earphone
    DEVICE_AUTOTEST_MISC = 12,    //lcd backlight,vibrator,keypadbacklight
    DEVICE_AUTOTEST_FM = 13,
    DEVICE_AUTOTEST_ATV = 14,
    DEVICE_AUTOTEST_BT = 15,
    DEVICE_AUTOTEST_WIFI = 16,
    DEVICE_AUTOTEST_IIC_DEV= 17, //speak&&receiver&&earphone
    DEVICE_AUTOTEST_CHARGE= 18,
    DEVICE_AUTOTEST_NVRAM_R= 19,
    DEVICE_AUTOTEST_NVRAM_W= 20,
    DEVICE_AUTOTEST_VEDIO=21,
#if defined(GPS_SUPPORT) &&  defined(GPS_CHIP_VER_GREENEYE2)
    DEVICE_AUTOTEST_GPS=22,
#endif
    DEVICE_AUTOTEST_ANT=23,
    DEVICE_AUTOTEST_CHECK= 28,
    DEVICE_AUTOTEST_MAX_F
}DEVICE_AUTOTEST_ID_E;

typedef enum
{
    MN_DUAL_SYS_1 = 0,
    MN_DUAL_SYS_2 = 1,
    MN_DUAL_SYS_3 = 2,
    MN_DUAL_SYS_4 = 3,
    MN_DUAL_SYS_MAX
} MN_DUAL_SYS_E;

#if defined(PRODUCT_CONFIG_uis8910ff_mpphone) || defined(PRODUCT_CONFIG_uis8910ff_zxf)
typedef enum
{
    MMISET_RING_TYPE_CALL               = 0x00, //来单铃声
    MMISET_RING_TYPE_RECORDER_RECORD    = 0x01,
    MMISET_RING_TYPE_RECORDER_PLAY      = 0x02,
    MMISET_RING_TYPE_MSG                = 0x03, //信息铃声
    MMISET_RING_TYPE_KEY                = 0x04, //按键铃声
    MMISET_RING_TYPE_TKEY               = 0x05, //触屏音
    MMISET_RING_TYPE_POWER              = 0x06, //开关机
    MMISET_RING_TYPE_POWER_OFF          = 0x07, //#ifdef CMCC_UI_STYLE
    MMISET_RING_TYPE_FLIP               = 0x08, //翻盖
    MMISET_RING_TYPE_FLIP_DOWN          = 0x09, //盒盖
    MMISET_RING_TYPE_ALARM              = 0x0a, //闹钟
    MMISET_RING_TYPE_DC                 = 0x0b, //照相
    MMISET_RING_TYPE_ASP                = 0x0c, //#ifdef ASP_SUPPORT
    MMISET_RING_TYPE_DC_COUNT           = 0x0d,
    MMISET_RING_TYPE_DC_FINAL_COUNT     = 0x0e,
    MMISET_RING_TYPE_BAT                = 0x0f, //低电量
    MMISET_RING_TYPE_OTHER              = 0x10, //其他一些下载铃声(预览铃声)等
    MMISET_RING_TYPE_AUDIO_PLAYER       = 0x11, //音频播放器
    MMISET_RING_TYPE_KURO_PLAYER        = 0x12, //kuro播放器
    MMISET_RING_TYPE_FM_PLAYER          = 0x13, //收音机
    MMISET_RING_TYPE_CALL_IN_CALL       = 0x14, //通话中来电
    MMISET_RING_TYPE_MSG_IN_CALL        = 0x15, //通话中来信息
    MMISET_RING_TYPE_MINHINT_IN_CALL    = 0x16, //通话中提示音
    MMISET_RING_TYPE_MYDOCUMENT         = 0x17, //文件管理
    MMISET_RING_TYPE_JAVA               = 0x18, //java
    MMISET_RING_TYPE_SLIDE_ON           = 0x19, //开盖音
    MMISET_RING_TYPE_SLIDE_OFF          = 0x1a, //合盖音
    MMISET_RING_TYPE_DL                 = 0x1b, //download
#ifdef DPHONE_SUPPORT
    MMISET_RING_TYPE_HOOK_HANDFREE_TONE = 0x1c,
    MMISET_RING_TYPE_HANGUP_REMIND = 0x1d,
    MMISET_RING_TYPE_AZAN               =0x1e,
    MMISET_RING_TYPE_NONE               = 0x1f,
#else
    MMISET_RING_TYPE_AZAN               =0x1c,
    MMISET_RING_TYPE_NONE               = 0x1d,
#endif
#ifdef UNISOC_CAMERA_UI
    MMISET_RING_TYPE_DC_REC_START,
    MMISET_RING_TYPE_DC_REC_STOP,
#endif /* UNISOC_CAMERA_UI */
#ifdef MMI_KEY_LOCK_SUPPORT
    MMISET_RING_TYPE_KEY_LOCK,
#endif
#ifdef MMI_READMENU_ENABLE
    MMISET_RING_TYPE_READMENU,
#endif
#ifdef WRE_SUPPORT
    MMISET_RING_TYPE_WRE, //wre
#endif
    MMISET_RING_TYPE_SCH,
#ifdef MMI_INDIAN_SOS_SUPPORT
    MMISET_RING_TYPE_SOS,
#endif
#ifdef MMI_ETWS_SUPPORT
    MMISET_RING_TYPE_ETWS,
    MMISET_RING_TYPE_ETWS_PWS,
#endif
    MMISET_RING_TYPE_HANDHOLD,
    MMISET_RING_TYPE_MAX
} MMISET_ALL_RING_TYPE_E;
#endif


typedef enum
{
    KEYPAD_INIT = 0x01,
    KEYPAD_ASK,
    KEYPAD_DONE,
    KEYPAD_CODE,
    KEYPAD_OPERA_START = 0x6f,
    KEYPAD_IDLE = 0x7f
} AUTOTEST_KEYPAD_E;

typedef enum
{
    FM_INIT_PLAY = 0x01,
    FM_STOP
} AUTOTEST_FM_E;


typedef enum
{
    ATV_OPEN = 0x01,
    ATV_STATE_ASK,
    ATV_GET_IMAGE,
    ATV_STOP
} AUTOTEST_ATV_E;


typedef enum
{
    BT_OPEN = 0x01,
    BT_SEARCH,
    BT_GET_ADR,
    BT_CLOSE,
    BT_RSSI
} AUTOTEST_BT_E;


#if defined(GPS_SUPPORT) &&  defined(GPS_CHIP_VER_GREENEYE2)
typedef enum
{
    GPS_OPEN = 0x01,
    GPS_SEARCH,
    GPS_READ_SATELLITES,
    GPS_CLOSE,
    GPS_READ_RSSI
} AUTOTEST_GPS_E;

LOCAL uint32 snr_len = 0;
LOCAL uint8 snr_max = 0;

#endif


typedef enum
{
    SIM_OK = 0x00,
    SIM_FAIL
} AUTOTEST_SIM_E;


typedef enum
{
    LCD_WRITE_START = 0x01,
    LCD_WRITE_END = 0x02,

    LCD_READ_START = 0x03,
    LCD_READ_END = 0x04,
    //to adapt tool
    LCD_COLOUR_LOOP=0x20,
    LCD_DRAW_COLOUR =0x21,
    LCD_CLOSE_DEVICE=0x22,
    LCD_HALF_COLOR=0x23,
} AUTOTEST_LCD_E;
typedef enum
{
    DRAW_COLOUR_WHITE = 0x0,
    DRAW_COLOUR_RED,
    DRAW_COLOUR_GREEN,
    DRAW_COLOUR_BLUE,
    DRAW_COLOUR_BLACK,
    DISABLE_LCD_KEYLOCK,
    ENABLE_LCD_KEYLOCK,
} LCD_DRAW_COLOUR_E;

typedef enum
{
    GPIO_GET_VALUE = 0x0,
    GPIO_SET_VALUE = 0x1,
    GPIOEX_KB_GET_VALUE = 0x2,      //keyboard keyin/keyout pin as io func(out high or low signal,capture signal)  2:capture signal
    GPIOEX_KB_SET_VALUE = 0x3,      //keyboard keyin/keyout pin as io func(out high or low signal,capture signal)  3:out signal
    GPIO_SETBACK = 0x4
} AUTOTEST_GPIO_E;

typedef enum
{
    AUTOTEST_ANA_DEV_ID_LCM_BL = 0x00,
    AUTOTEST_ANA_DEV_ID_LCM_BRIGHTNESS,
    AUTOTEST_ANA_DEV_ID_KPD_BL,
    AUTOTEST_ANA_DEV_ID_VIBRATOR,
    AUTOTEST_ANA_DEV_ID_SD_PWR,
    AUTOTEST_ANA_DEV_ID_FLASH_LIGHT,
    AUTOTEST_ANA_DEV_ID_HEADSET,
    AUTOTEST_ANA_DEV_ID_BAT_TEMP = 0xA,
        AUTOTEST_ANA_DEV_ID_THERMAL = 0xB,
    AUTOTEST_ANA_DEV_ID_MAX
}AUTOTEST_ANA_DEV_ID_E;

typedef enum
{
    CHARGE_OPEN = 0x01,
    CHARGE_READ,
    CHARGE_CLOSE
} AUTOTEST_CHARGE_E;

typedef enum
{
        IIC_REG_READ = 0x01,
        IIC_REG_END
}AUTOTEST_IIC_E;

#if defined(PLATFORM_UWS6121E)
typedef enum
{
    WIFI_OPEN = 0x01,
    WIFI_SCAN,
    WIFI_READ,
    WIFI_CLOSE,
    WIFI_RSSI
} AUTOTEST_WIFI_E;
#else
typedef enum
{
    WIFI_INIT = 0x01,
    WIFI_OPEN,
    WIFI_SCAN,
    WIFI_CLOSE
} AUTOTEST_WIFI_E;
#endif

#if defined(PLATFORM_UWS6121E)
typedef struct AUTOTST_PM_PINFUNC_tag
{
    uint8	pin_num;
    uint8   pin_sel;
    uint8   gpio_port;
    uint8   reserved;
    uint32 addr;
} AUTOTST_PM_PINFUNC_T;
#else
typedef struct AUTOTST_PM_PINFUNC_tag
{
    uint8	pin_num;
    uint32 addr;
    uint32 value;
} AUTOTST_PM_PINFUNC_T;
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct _DIAG_MSG_SIM_T
{
    MSG_HEAD_T   msg_head;
    uint8        sim_select;
}DIAG_MSG_SIM_T;

typedef struct DIAG_MSG_LCD_T
{
    MSG_HEAD_T   msg_head;
    uint8 cmmand;
	uint8 reserve;
	uint16  data;
}DIAG_MSG_LCD_T;

typedef struct _DIAG_MSG_MISC_T
{
    MSG_HEAD_T   msg_head;
    uint8 dev_mode;
    uint8 set_data;
}DIAG_MSG_MISC_T;

typedef struct _DIAG_MSG_ANTTEST_DATA_T
{
    uint8 is_open;
    uint8 bw;
    uint16 band;
    uint16 apc;
    uint16 amp;
    uint32 channel;
    uint32 reserved;
}DIAG_MSG_ANTTEST_DATA_T;


typedef struct _DIAG_MSG_ANTTEST_T
{
    MSG_HEAD_T   msg_head;
    DIAG_MSG_ANTTEST_DATA_T data;
}DIAG_MSG_ANTTEST_T;

typedef struct _DIAG_MSG_AUDIOLOOP_T
{
    MSG_HEAD_T msg_head;
    uint8 dev_mode;
    uint8 indev;
    uint8 outdev;
} DIAG_MSG_AUDIOLOOP_T;

typedef struct _DIAG_MSG_AUDIO_T
{
    MSG_HEAD_T msg_head;
    uint8 dev_mode;
    uint8 cmmand;
    uint8 subcmmand;
} DIAG_MSG_AUDIO_T;

typedef struct _DIAG_MSG_GPIO_T
{
    MSG_HEAD_T   msg_head;
	uint8        command;
    uint8        gpio_id;
    uint8        gpio_value;
    uint8        reserved;
}DIAG_MSG_GPIO_T;


typedef struct _DIAG_MSG_FM_T
{
    MSG_HEAD_T   msg_head;
    uint8 cmmand;
    uint8 fm_type;
    uint16  fm_channel;
}DIAG_MSG_FM_T;


typedef struct _DIAG_MSG_IIC_T
{
    MSG_HEAD_T   msg_head;
    uint8 iic_addr;
    uint8 reg_addr;
    uint8 port;
    uint8 ack_en;
}DIAG_MSG_IIC_T;


typedef struct _DIAG_MSG_ATV_T
{
    MSG_HEAD_T   msg_head;
    uint8 cmmand;
    uint8 reserved;
    uint16 atv_channel;
}DIAG_MSG_ATV_T;

typedef struct _DIAG_MSG_VIDEO_T
{
    MSG_HEAD_T   msg_head;
    uint8 cmmand;
}DIAG_MSG_VIDEO_T;

typedef struct _DIAG_MSG_NVRAM_T
{
    MSG_HEAD_T   msg_head;
    uint16 factory_name;
    uint16 worker_name;
    uint16 work_name;
    uint16 reserved;
}DIAG_MSG_NVRAM_T;

#ifdef	PLATFORM_SC6530
#define     PIN_NUM_MAX          131
    #ifdef  CHIP_VER_6530
        const AUTOTST_PM_PINFUNC_T autodev_pm_func[]=
        {
         // |  gpio NO. 	|	 Register addr		|     goio func  number(1 or 3   |
         	{0,		PIN_GPIO_0_REG,			( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{1,		0xffffffff, 				0xffffffff},
		{2,		0xffffffff, 				0xffffffff},
        	{3,		0xffffffff, 				0xffffffff},
		{4,		PIN_U0CTS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{5,		PIN_U0RTS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{6,		PIN_IISDI_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{7,		PIN_IISDO_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{8,		PIN_IISCLK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{9,		PIN_IISLRCK_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{10,		PIN_SD_CMD_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{11,		PIN_SD_D0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{12,		PIN_SD_CLK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{13,		PIN_SD_D3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{14,		0xffffffff, 				0xffffffff},
		{15,		PIN_SIMDA2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{16,		PIN_SIMRST2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{17,		PIN_SIMCLK2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{18,		PIN_SPI1_0_CS0_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{19,		PIN_MTMS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{20,		PIN_MTDI_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{21,		PIN_MTCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{22,		PIN_MTDO_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{23,		PIN_MTRSTN_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{24,		PIN_CLK_AUX_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{25,		PIN_PWM_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{26,		PIN_KEYOUT5_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{27,		PIN_KEYOUT6_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{28,		PIN_KEYIN6_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{29,		PIN_KEYIN7_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{30,		PIN_KEYIN4_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{31,		PIN_KEYIN5_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{32,		0xffffffff, 				0xffffffff},
		{33,		0xffffffff, 				0xffffffff},
		{34,		PIN_CCIRD6_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{35,		PIN_CCIRD5_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{36,		PIN_CCIRD4_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{37,		PIN_CAM_PD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{38,		PIN_CAM_RSTN_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{39,		PIN_LCMRD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{40,		PIN_LCMWR_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{41,		PIN_LCMCD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{42,		PIN_LCMCS0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{43,		PIN_RF_PABAND_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{44,		PIN_RF_VBS1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{45,		PIN_RF_VBS2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{46,		PIN_RF_VBS3_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{47,		PIN_RF_PAMODE_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{48,		PIN_RF_PAEN_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{49,		PIN_LCMCS1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{50,		PIN_CCIRD7_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{51,		PIN_CCIRD3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{52,		PIN_CCIRD2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{53,		PIN_LCMD0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{54,		PIN_LCMD1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{55,		PIN_LCMD2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{56,		PIN_LCMD3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{57,		PIN_LCMD4_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{58,		PIN_LCMD5_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{59,		PIN_LCMD6_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{60,		PIN_LCMD7_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{61,		PIN_LCMD8_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{62,		PIN_LCMRSTN_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{63,		PIN_GPIO_63_REG,		        ( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{64,		PIN_CCIRD1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{65,		PIN_CCIRHS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{66,		PIN_CCIRMCK_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{67,		PIN_SD_D1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{68,		PIN_SD_D2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{69,		PIN_SD_CLK1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{70,		0xffffffff, 				0xffffffff},
		{71,		0xffffffff, 				0xffffffff},
		{72,		0xffffffff, 				0xffffffff},
		{73,		PIN_CCIRCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{74,		PIN_CCIRVS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{75,		PIN_IISMCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{76,		PIN_KEYIN3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{77,		0xffffffff, 				0xffffffff},
		{78,		0xffffffff, 				0xffffffff},
		{79,		PIN_FMARK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{80,		PIN_SIMRST0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{81,		PIN_SIMCLK0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{82,		PIN_SIMDA0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{83,		PIN_SIMDA1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{84,		PIN_SIMRST1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{85,		PIN_SIMCLK1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{86,		PIN_KEYOUT0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{87,		PIN_KEYOUT1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{88,		PIN_KEYOUT2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{89,		PIN_KEYOUT3_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{90,		PIN_KEYOUT4_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{91,		PIN_KEYIN0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{92,		PIN_KEYIN1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{93,		PIN_KEYIN2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{94,		PIN_U0TXD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{95,		PIN_U0RXD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{96,		0xffffffff, 				0xffffffff},
		{97,		0xffffffff, 				0xffffffff},
		{98,		0xffffffff, 				0xffffffff},
		{99,		0xffffffff, 				0xffffffff},
		{100,	0xffffffff, 				0xffffffff},
		{101,	0xffffffff, 				0xffffffff},
		{102,	0xffffffff, 				0xffffffff},
		{103,	0xffffffff, 				0xffffffff},
		{104,	0xffffffff, 				0xffffffff},
		{105,	0xffffffff, 				0xffffffff},
		{106,	0xffffffff, 				0xffffffff},
		{107,	0xffffffff, 				0xffffffff},
		{108,	0xffffffff, 				0xffffffff},
		{109,	0xffffffff, 				0xffffffff},
		{110,	0xffffffff, 				0xffffffff},
		{111,	0xffffffff, 				0xffffffff},
		{112,	0xffffffff, 				0xffffffff},
		{113,	0xffffffff, 				0xffffffff},
		{114,	0xffffffff, 				0xffffffff},
		{115,	0xffffffff, 				0xffffffff},
		{116,	0xffffffff, 				0xffffffff},
		{117,	PIN_KEYIN3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{118,	0xffffffff, 				0xffffffff},
		{119,	0xffffffff, 				0xffffffff},
		{120,	0xffffffff, 				0xffffffff},
		{121,	0xffffffff, 				0xffffffff},
		{122,	0xffffffff, 				0xffffffff},
		{123,	0xffffffff, 				0xffffffff},
		{124,	0xffffffff, 				0xffffffff},
		{125,	0xffffffff, 				0xffffffff},
		{126,	0xffffffff, 				0xffffffff},
		{127,	0xffffffff, 				0xffffffff},
            	{128,	ANA_PIN_TP_XL_REG, 		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},    // GPIO_128:
           	{129, 	ANA_PIN_TP_XR_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},    // GPIO_129:
            	{130, 	ANA_PIN_TP_YU_REG, 		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},    // GPIO_130:
            	{131,	ANA_PIN_TP_YD_REG, 		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},    // GPIO_131:
            	{0xff,	0xffffffff, 				0xffffffff}
        };

    #else
        const AUTOTST_PM_PINFUNC_T autodev_pm_func[]=
        {
         // |  gpio NO. 	|	 Register addr		|     goio func  number(1 or 3   |
         	{0,		PIN_GPIO_0_REG,			( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{1,		PIN_GPIO_1_REG,		( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{2,		PIN_GPIO_2_REG,			( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
        	{3,		PIN_GPIO_3_REG,			( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{4,		PIN_U0CTS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{5,		PIN_U0RTS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{6,		PIN_IISDI_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{7,		PIN_IISDO_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{8,		PIN_IISCLK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{9,		PIN_IISLRCK_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{10,		PIN_SD_CMD_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{11,		PIN_SD_D0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{12,		PIN_SD_CLK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{13,		PIN_SD_D3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{14,		PIN_SPI0_3_CS0_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{15,		PIN_SIMDA2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{16,		PIN_SIMRST2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{17,		PIN_SIMCLK2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{18,		PIN_SPI1_0_CS0_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{19,		PIN_MTMS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{20,		PIN_MTDI_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{21,		PIN_MTCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{22,		PIN_MTDO_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{23,		PIN_MTRSTN_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{24,		PIN_CLK_AUX_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{25,		PIN_PWM_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{26,		PIN_KEYOUT5_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{27,		PIN_KEYOUT6_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{28,		PIN_KEYIN6_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{29,		PIN_KEYIN7_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{30,		PIN_KEYIN4_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{31,		PIN_KEYIN5_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{32,		PIN_SCL0_0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{33,		PIN_SDA0_0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{34,		PIN_CCIRD6_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{35,		PIN_CCIRD5_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{36,		PIN_CCIRD4_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{37,		PIN_CAM_PD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{38,		PIN_CAM_RSTN_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{39,		PIN_LCMRD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{40,		PIN_LCMWR_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{41,		PIN_LCMCD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{42,		PIN_LCMCS0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{43,		PIN_RF_VBS0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{44,		PIN_RF_VBS1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{45,		PIN_RF_VBS2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{46,		PIN_RF_VBS3_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{47,		PIN_RF_RSV_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{48,		PIN_RF_PAEN_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{49,		PIN_LCMCS1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{50,		PIN_CCIRD7_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{51,		PIN_CCIRD3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{52,		PIN_CCIRD2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{53,		PIN_LCMD0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{54,		PIN_LCMD1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{55,		PIN_LCMD2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{56,		PIN_LCMD3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{57,		PIN_LCMD4_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{58,		PIN_LCMD5_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{59,		PIN_LCMD6_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{60,		PIN_LCMD7_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{61,		PIN_LCMD8_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{62,		PIN_LCMRSTN_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{63,		PIN_GPIO_63_REG,		        ( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{64,		PIN_CCIRD1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{65,		PIN_CCIRHS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{66,		PIN_CCIRMCK_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{67,		PIN_SD_D1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{68,		PIN_SD_D2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{69,		PIN_SPI0_3_CLK_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{70,		PIN_GPIO_70_REG,		        ( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},//NO BALL
		{71,		PIN_GPIO_71_REG,		        ( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},//NO BALL
		{72,		PIN_GPIO_72_REG,		        ( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},//NO BALL
		{73,		PIN_CCIRCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{74,		PIN_CCIRVS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{75,		PIN_IISMCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{76,		PIN_KEYIN3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{77,		PIN_SPI0_3_DO_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{78,		PIN_SPI0_3_DI_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{79,		PIN_FMARK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{80,		PIN_SIMRST0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{81,		PIN_SIMCLK0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{82,		PIN_SIMDA0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{83,		PIN_SIMDA1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{84,		PIN_SIMRST1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{85,		PIN_SIMCLK1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{86,		PIN_KEYOUT0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{87,		PIN_KEYOUT1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{88,		PIN_KEYOUT2_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{89,		PIN_KEYOUT3_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{90,		PIN_KEYOUT4_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{91,		PIN_KEYIN0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{92,		PIN_KEYIN1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{93,		PIN_KEYIN2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{94,		PIN_U0TXD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{95,		PIN_U0RXD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{96,		PIN_CCIRD0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{97,		PIN_SD_CLK1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{98,		0xffffffff, 				0xffffffff},
		{99,		0xffffffff, 				0xffffffff},
		{100,	0xffffffff, 				0xffffffff},
		{101,	0xffffffff, 				0xffffffff},
		{102,	0xffffffff, 				0xffffffff},
		{103,	0xffffffff, 				0xffffffff},
		{104,	0xffffffff, 				0xffffffff},
		{105,	0xffffffff, 				0xffffffff},
		{106,	0xffffffff, 				0xffffffff},
		{107,	0xffffffff, 				0xffffffff},
		{108,	0xffffffff, 				0xffffffff},
		{109,	0xffffffff, 				0xffffffff},
		{110,	0xffffffff, 				0xffffffff},
		{111,	0xffffffff, 				0xffffffff},
		{112,	0xffffffff, 				0xffffffff},
		{113,	0xffffffff, 				0xffffffff},
		{114,	0xffffffff, 				0xffffffff},
		{115,	0xffffffff, 				0xffffffff},
		{116,	0xffffffff, 				0xffffffff},
		{117,	0xffffffff, 				0xffffffff},
		{118,	0xffffffff, 				0xffffffff},
		{119,	0xffffffff, 				0xffffffff},
		{120,	0xffffffff, 				0xffffffff},
		{121,	0xffffffff, 				0xffffffff},
		{122,	0xffffffff, 				0xffffffff},
		{123,	0xffffffff, 				0xffffffff},
		{124,	0xffffffff, 				0xffffffff},
		{125,	0xffffffff, 				0xffffffff},
		{126,	0xffffffff, 				0xffffffff},
		{127,	0xffffffff, 				0xffffffff},
            	{128,	 ANA_PIN_TP_XL_REG,		( ANA_PIN_FUNC_3 | ANA_PIN_FPX_EN | ANA_PIN_DS_1 )},    // GPIO_128:
            	{129, 	 ANA_PIN_TP_YU_REG, 		( ANA_PIN_FUNC_3 | ANA_PIN_FPX_EN | ANA_PIN_DS_1 )},    // GPIO_129:
            	{130, 	 ANA_PIN_TP_XR_REG, 		( ANA_PIN_FUNC_3 | ANA_PIN_FPX_EN | ANA_PIN_DS_1 )},    // GPIO_130:
            	{131,	 ANA_PIN_TP_YD_REG, 		( ANA_PIN_FUNC_3 | ANA_PIN_FPX_EN | ANA_PIN_DS_1 )},    // GPIO_131:
            	{0xff,	 0xffffffff, 				0xffffffff}
        };

    #endif

#elif defined(PLATFORM_SC6531EFM)
		#define     PIN_NUM_MAX          76
		const AUTOTST_PM_PINFUNC_T autodev_pm_func[]=
        {
         // |  gpio NO. 	|	 Register addr		|     goio func  number(1 or 3   |
             	{0,		PIN_GPIO_0_REG,			( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{1,		PIN_GPIO_1_REG,		( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{2,		PIN_GPIO_2_REG,			( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
        	{3,		PIN_GPIO_3_REG,			( PIN_SPX_EN  | PIN_FUNC_DEF | PIN_FPX_EN | PIN_DS_1 )},
		{4,		PIN_MTMS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{5,		PIN_MTCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{6,		PIN_KEYOUT0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{7,		PIN_KEYOUT1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{8,		PIN_KEYOUT2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{9,		PIN_KEYOUT3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{10,		PIN_KEYOUT4_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{11,		PIN_KEYIN0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{12,		PIN_KEYIN1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{13,		PIN_KEYIN2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{14,		PIN_KEYIN3_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{15,		PIN_KEYIN4_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{16,		PIN_IISDI_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{17,		PIN_IISDO_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{18,		PIN_IISCLK_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{19,		PIN_IISLRCK_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{20,		PIN_PWM_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{21,		PIN_SPI0_CLK_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{22,		PIN_SPI0_CS0_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{23,		PIN_SPI0_DI_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{24,		PIN_SPI0_DO_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{25,		PIN_SPI1_DI_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{26,		PIN_SPI1_DO_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{27,		PIN_SPI1_CLK_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{28,		PIN_SPI1_CS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{29,		PIN_FMARK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{30,		PIN_LCMRSTN_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{31,		PIN_CLK_AUX_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{32,		PIN_U0TXD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{33,		PIN_U0RXD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{34,		PIN_U0CTS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{35,		PIN_U0RTS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{36,		PIN_SD_CMD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{37,		PIN_SD_D0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{38,		PIN_SD_CLK_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{39,		PIN_SD_D3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{40,		PIN_CCIRD0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{41,		PIN_CCIRD1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{42,		PIN_CCIRD2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{43,		PIN_CCIRD3_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{44,		PIN_CCIRD4_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{45,		PIN_CCIRD5_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{46,		PIN_CCIRD6_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{47,		PIN_CCIRD7_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{48,		PIN_CMPCLK_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{49,		PIN_CCIRMCK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{50,		PIN_CCIRHS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{51,		PIN_CCIRVS_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{52,		PIN_CAM_PD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{53,		PIN_CAM_RSTN_REG,		( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{54,		PIN_SCL0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{55,		PIN_SDA0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{56,		PIN_SD_D1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{57,		PIN_SD_D2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{58,		PIN_SIMRST0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{59,		PIN_SIMCLK0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{60,		PIN_SIMDA0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{61,		PIN_SIMDA1_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{62,		PIN_SIMRST1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{63,		PIN_SIMCLK1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{64,		PIN_SPI1_CD_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{65,		PIN_RFCTL0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{66,		PIN_RFCTL1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{67,		PIN_RF_PAEN_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{68,		PIN_EXTINT0_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{69,		PIN_EXTINT1_REG,		       ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{70,		PIN_ESMCSN0_REG,		 ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{71,		PIN_ESMD0_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{72,		PIN_ESMD1_REG,		        ( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{73,		PIN_ESMD2_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{74,		PIN_ESMD3_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{75,		PIN_ESMCLK_REG,			( PIN_SPX_EN  | PIN_FUNC_3 | PIN_FPX_EN | PIN_DS_1 )},
		{0xff,	 0xffffffff, 				0xffffffff}
        };
#elif defined(PLATFORM_UWS6121E)
#define     PIN_NUM_MAX          48        //max index of pin can be gpio
#define     NUM_AUTOTST_PM_PINFUNC_T   (sizeof(autodev_pm_func) / sizeof(const AUTOTST_PM_PINFUNC_T))
const AUTOTST_PM_PINFUNC_T autodev_pm_func[]=
{
 //pin_num,pin_sel,gpio_id,reserved,       addr
    {0,          0x0,         0,     0x0,      REG_PIN_GPIO0},
    {1,          0x0,         1,     0x0,      REG_PIN_GPIO1},
    {2,          0x0,         2,     0x0,      REG_PIN_GPIO2},
    {3,          0x0,         3,     0x0,      REG_PIN_GPIO3},
    {4,          0x0,         4,     0x0,      REG_PIN_GPIO4},
    {5,          0x0,         5,     0x0,      REG_PIN_GPIO5},
    {6,          0x0,         6,     0x0,      REG_PIN_GPIO6},
    {7,          0x0,         7,     0x0,      REG_PIN_GPIO7},
    {8,          0x1,         8,     0x0,      REG_PIN_KEYIN4},
    {9,          0x1,         9,     0x0,      REG_PIN_KEYIN5},
    {10,        0x1,        10,     0x0,      REG_PIN_KEYOUT4},
    {11,        0x1,        11,     0x0,      REG_PIN_KEYOUT5},
    {12,        0x1,        12,     0x0,      REG_PIN_U1RXD},
    {13,        0x1,        13,     0x0,      REG_PIN_U1TXD},
    {14,        0x1,        14,     0x0,      REG_PIN_U1CTS},
    {15,        0x4,        15,     0x0,      REG_PIN_U1RTS},
    {16,        0x0,        16,     0x0,      REG_PIN_GPIO16},
    {17,        0x0,        17,     0x0,      REG_PIN_GPIO17},
    {18,        0x0,        18,     0x0,      REG_PIN_GPIO18},
    {19,        0x0,        19,     0x0,      REG_PIN_GPIO19},
    {20,        0x0,        20,     0x0,      REG_PIN_GPIO20},
    {21,        0x0,        21,     0x0,      REG_PIN_GPIO21},
    {22,        0x0,        22,     0x0,      REG_PIN_GPIO22},
    {23,        0x0,        23,     0x0,      REG_PIN_GPIO23},
    {24,        0x1,        24,     0x0,      REG_PIN_SW_CLK},
    {25,        0x1,        25,     0x0,      REG_PIN_SW_DIO},
    {26,        0x1,        26,     0x0,      REG_PIN_DEBUG_HOST_RX},
    {27,        0x1,        27,     0x0,      REG_PIN_DEBUG_HOST_TX},
    {28,        0x1,        28,     0x0,      REG_PIN_DEBUG_HOST_CLK},
    {29,        0x1,        29,     0x0,      REG_PIN_KEYIN1},
    {30,        0x1,        30,     0x0,      REG_PIN_KEYIN2},
    {31,        0x1,        31,     0x0,      REG_PIN_KEYIN3},
    {32,        0x1,          32,     0x0,      REG_PIN_KEYOUT0},
    {33,        0x1,          33,     0x0,      REG_PIN_KEYOUT1},
    {34,        0x1,          34,     0x0,      REG_PIN_KEYOUT2},
    {35,        0x1,          35,     0x0,      REG_PIN_KEYOUT3},
    {36,        0x2,          36,     0x0,      REG_PIN_SPI_LCD_SDC},
    {37,        0x2,          37,     0x0,      REG_PIN_SPI_LCD_CLK},
    {38,        0x2,          38,     0x0,      REG_PIN_SPI_LCD_CS},
    {39,        0x2,          39,     0x0,      REG_PIN_SPI_LCD_SELECT},
    {40,        0x2,          40,     0x0,      REG_PIN_LCD_FMARK},
    {41,        0x2,         41,     0x0,      REG_PIN_LCD_RSTB},
    {42,        0x2,         42,     0x0,      REG_PIN_I2C_M2_SCL},
    {43,        0x2,         43,     0x0,      REG_PIN_I2C_M2_SDA},
    {44,        0x3,         44,     0x0,      REG_PIN_CAMERA_RST_L},
    {45,        0x3,         45,     0x0,      REG_PIN_CAMERA_PWDN},
    {46,        0x2,         46,     0x0,      REG_PIN_CAMERA_REF_CLK},
    {47,        0x3,         47,     0x0,      REG_PIN_SPI_CAMERA_SI_0},
};
#else
const AUTOTST_PM_PINFUNC_T autodev_pm_func[]=
{
    {0xff,0xffffffff, 0xffffffff},
    {0xff,0xffffffff, 0xffffffff}
};

#endif
#ifdef FM_SUPPORT
extern void  MMIAPIENVSET_SetTestMode(void);
extern void  MMIAPIENVSET_ClearTestMode(void);
#endif
#if !defined(MAINLCM_INTERFACE_NONE)
extern void TestMainLCD(uint32 msg_id);
extern void MMIDEFAULT_AllowTurnOffBackLight(BOOLEAN allow_turn_off);
extern void MMIDEFAULT_TurnOnLCDBackLight(void);
#endif

extern void LCD_Clear (LCD_ID_E lcd_id, uint32 color);
extern void MMIENG_BydTest_CreatWin(void);
/*******************************************************************************
 **     static variable
 *******************************************************************************/
LOCAL int32 AUTODEV_IIC_Handler = -1;
LOCAL I2C_DEV AUTODEV_IIC_cfg =
{
	0,            	    	//id, use camera iic port default
	100*1000,   			//freq
	0,						//bus
	AUTODEV_I2C_WRITE_ADDR, //slave_addr
	1,						//reg_addr_num
	1,						//check_ack
	1						// no_stop
};



LOCAL  volatile uint16  keypadcode = 0;
LOCAL uint8  keypad_in = 0;
LOCAL uint8  keypad_out = 0;

LOCAL DCAMERA_YUV_CHN_ADDR_T cap_image_addr = {0x00};
LOCAL DCAMERA_SIZE_T cap_size = {0x00};
LOCAL uint32 ext_buf_addr = NULL;
LOCAL uint32 ext_buf_addr_align = NULL;
LOCAL SENSOR_OUTPUT_MODE_E s_auto_test_sensor_output_mode = SENSOR_OUTPUT_MODE_MAX;


#if !defined(BLUETOOTH_SUPPORT_NONE)
LOCAL BT_DEVICE_INFO  s_bt_dev_info[MAX_DEVICE_DISCOVERY] = {0};
LOCAL uint8  s_bt_dev_count = 0;
LOCAL BOOLEAN Is_BTSearchStart = SCI_FALSE;
#endif

LOCAL BOOLEAN Is_WifiScanStart = SCI_FALSE;
LOCAL DIAG_AUTOTEST_CALLBACK s_diag_autotest_callback[DEVICE_AUTOTEST_MAX_F] = {NULL};

#ifdef FM_SUPPORT
static uint32 s_fm_volum = 0;
static HAUDIO FmHandle = INVALID_HANDLE;

//bug963073修正////////////////////////
#if defined(VOLUME_MAX_LEVEL_15)
#define   FM_VOLUME_LEVEL_MAX   15
#elif defined(VOLUME_MAX_LEVEL_9)
#define FM_VOLUME_LEVEL_MAX   9
#else
#define FM_VOLUME_LEVEL_MAX   9
#endif
/////////////////////////////////////////

#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern PUBLIC BOOLEAN MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_E dual_sys);
extern void OS_Pfree(void *ptr);
extern MSG_HEAD_T s_calibration_req;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
#if !defined(CAMERA_SUPPORT_NONE)
LOCAL uint32 HandleParallel_CameraAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 ) ;


LOCAL uint32 HandleSPI_CameraAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 ) ;
#endif
#ifdef VIDEO_RECORD_SUPPORT
LOCAL uint32 HandleVIDEO_Autotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 ) ;
#endif
#if !defined(ATV_SUPPORT_NONE)
LOCAL uint32 HandleATV_Autotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 ) ;
#endif

LOCAL uint32 HandleKeypadAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );

#if !defined(FM_SUPPORT_NONE)
LOCAL uint32 HandleFMAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );
#endif

#if !defined(BLUETOOTH_SUPPORT_NONE)
LOCAL uint32 HandleBTAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );
#endif


LOCAL uint32 HandleChargeAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
);

#if defined(GPS_SUPPORT) &&  defined(GPS_CHIP_VER_GREENEYE2)
LOCAL uint32 HandleGpsAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
);
#endif

LOCAL uint32 Handle_MicAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );

#if !defined(WIFI_SUPPORT_NONE)
LOCAL uint32 HandleWifiAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 );
#endif

LOCAL uint32 Handle_SpeakerAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
);

LOCAL uint32 HandleIicDeviceAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );

LOCAL uint32 HandleTFAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );

LOCAL uint32 HandleSIMAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );

LOCAL uint32 HandlePARALLEL_LCDAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );


#ifdef MAINLCM_INTERFACE_SPI	//LCM_INTERFACE_SPI mengzhao
LOCAL uint32 HandleSpi_LCDAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );
#endif

LOCAL uint32 HandleAntennaTest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 );

LOCAL uint32 HandleDevice_MiscAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );

LOCAL uint32 HandleGpioAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
 );

LOCAL uint32 Autotest_Result_Read(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
);

LOCAL uint32 Autotest_Result_Write(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
);


LOCAL uint32 Handle_Autotest_Check(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
);


LOCAL uint32 HandleDeviceAutotest(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len
);

#ifdef AUTOTEST_DEBUG
LOCAL void AMR_REOCED_File(uint8 *data_ptr, uint32 data_len)
{

    uint8  iis_file_name[40] = {0};
    uint16 ucs2_iis_file_name[40] = {0};
    uint32 tmp = 0;

    SFS_HANDLE file_handle_r = NULL;
    sprintf((char*)iis_file_name,"E:\\AMR_Recode-1.amr");

    GUI_GB2UCS(ucs2_iis_file_name, iis_file_name, (uint16)(sizeof(iis_file_name)));

    file_handle_r = SFS_CreateFile((const uint16 *)ucs2_iis_file_name, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE,NULL,NULL);

    if(file_handle_r > 0)
    {
        SFS_WriteFile(file_handle_r, data_ptr, data_len, (uint32*)&tmp, NULL);

    }

    SFS_CloseFile(file_handle_r);
    return;
}
#endif

#if !defined(BLUETOOTH_SUPPORT_NONE)

PUBLIC BOOLEAN BT_AutotestDeviceMode(void)
{
    return Is_BTSearchStart;
}

PUBLIC void BT_AutotestDeviceCallback(const BT_MSG_T *param)
{
	SCI_TRACE_LOW("BT_AutotestDeviceCallback: msg_id = %d, status = %d",
	    param->msg_id, param->status);
	switch(param->msg_id)
	{
    	case ID_STATUS_CM_INQUIRY_RES:
    	{
            s_bt_dev_count++;
            if (s_bt_dev_count > MAX_DEVICE_DISCOVERY)
            {
                s_bt_dev_count = MAX_DEVICE_DISCOVERY;//
            }

    	    s_bt_dev_info[s_bt_dev_count-1]   = *((BT_DEVICE_INFO*)param->body_ptr);

     		SCI_TRACE_LOW("BT_AutotestDeviceCallback: addr=%X-%X-%X-%X-%X-%X RSSI=0x%x",
    		    s_bt_dev_info[s_bt_dev_count-1].addr.addr[0], s_bt_dev_info[s_bt_dev_count-1].addr.addr[1],\
    		    s_bt_dev_info[s_bt_dev_count-1].addr.addr[2], s_bt_dev_info[s_bt_dev_count-1].addr.addr[3],\
    		    s_bt_dev_info[s_bt_dev_count-1].addr.addr[4], s_bt_dev_info[s_bt_dev_count-1].addr.addr[5],\
    		    s_bt_dev_info[s_bt_dev_count-1].rssi);

    		break;
        }
    	default:
    		break;
	}

	//Free msg space
	if (param->body_size >0)
	{
		#ifdef WIN32
		SCI_FREE(param->body_ptr);
		#else
		//OS_Pfree(param->body_ptr);
		#endif
	}
}

PUBLIC void MMIAutotest_BTDeviceMode(BT_MSG_T *param)
{
	if(BT_AutotestDeviceMode())
	{
		BT_AutotestDeviceCallback(param);
		return;
	}
}

#endif


LOCAL int8 Get_SimCard_Status(MSG_HEAD_T *msg_head, uint8 card_id)
{
    MCU_MODE_E  reset_mode = NORMAL_MODE;
    uint32 cmd_id;
    BOOLEAN     sim_status = SCI_FALSE;

    reset_mode = POWER_GetResetMode();
    if (reset_mode == CALIBRATION_MODE) {
        //save the request header
        SCI_MEMCPY((void*)&s_calibration_req, msg_head, sizeof(MSG_HEAD_T));
        cmd_id = (uint32)((uint16)msg_head->type | ((uint16)card_id << 8));
        NV_IPC_sendCmdToCp(cmd_id, NV_CMD_CALIB, NULL, NULL);
        return (-1);
    } else {
        sim_status = DMSIM_GetSimExistStatus(card_id);
        return ((sim_status == SCI_FALSE)? 0:1);
    }
}


PUBLIC void Register_DeviceAutoTest_CmdRoutine (void)
{
    SCI_TRACE_LOW ("Device Autotest: Register Cmd Routine");

    DIAG_InitAutotestCallback();

    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_KEYPAD, HandleKeypadAutotest);


    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_LCD_PARALLEL, HandlePARALLEL_LCDAutotest);

#if defined (MAINLCM_INTERFACE_SPI)
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_LCD_SPI, HandleSpi_LCDAutotest);
#endif

    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_GPIO, HandleGpioAutotest);
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_TF, HandleTFAutotest);
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_SIM, HandleSIMAutotest);

    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_MIC, Handle_MicAutotest);

    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_SPEAK, Handle_SpeakerAutotest);
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_MISC, HandleDevice_MiscAutotest);

#if !defined(CAMERA_SUPPORT_NONE)
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_CAMERA_PARALLEL, HandleParallel_CameraAutotest);
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_CAMERA_SPI, HandleSPI_CameraAutotest);
#endif

#if !defined(FM_SUPPORT_NONE)
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_FM, HandleFMAutotest);
#endif

#if !defined(ATV_SUPPORT_NONE)
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_ATV, HandleATV_Autotest);
#endif
#ifdef VIDEO_RECORD_SUPPORT
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_VEDIO, HandleVIDEO_Autotest);
#endif
#if !defined(BLUETOOTH_SUPPORT_NONE)
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_BT, HandleBTAutotest);
#endif
#if !defined(WIFI_SUPPORT_NONE)
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_WIFI, HandleWifiAutotest);
#endif
	DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_IIC_DEV, HandleIicDeviceAutotest);
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_CHARGE, HandleChargeAutotest);
	DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_NVRAM_R, Autotest_Result_Read);
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_NVRAM_W, Autotest_Result_Write);
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_CHECK, Handle_Autotest_Check);
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_GPS, HandleGpsAutotest);
#endif
    DIAG_RegisterAutotestCallback( DEVICE_AUTOTEST_ANT, HandleAntennaTest);
    DIAG_RegisterCmdRoutine (DIAG_DEVICE_AUTOTEST_F,  HandleDeviceAutotest);
}
#ifndef GLOBAL_DIAG_COMMAND
uint16 AutoTest_key_code_count[21] = {0};
static uint8 AutoTest_key_count=0;
LOCAL  void KeypadAutotest_RecordAll_CallBackFunc(
                              BLOCK_ID id,    // Client thread ID
                              uint32   argc,  // Signal code  event!
                              void     *argv  // Key Code
                              )
{
	uint32  i =0;
	uint32  j =0;
	uint32 key_total_num = 21;
	uint32      key_code     = (uint32)(argv);
	uint16      single_code = (uint16)(argc & 0xFFFF);
	uint8 key_count_flag = 0;
	uint16 key_temp=0;
	keypadcode = (uint16)(key_code&0xFFFF);
	if(single_code == KPD_DOWN)
	{
				for(j=0;j<AutoTest_key_count;j++)
				{
					if((AutoTest_key_code_count[j]&0xff) == (keypadcode&0xff))
					{
						key_count_flag = 1;
						break;
					}
				}
				if(key_count_flag != 1)
				{
					AutoTest_key_code_count[AutoTest_key_count++] = (keypadcode|0x100);
					key_count_flag = 0;
					//SCI_TRACE_LOW(" AutoTest_key_code_count[%d] = 0x%x",AutoTest_key_count-1,AutoTest_key_code_count[AutoTest_key_count-1]);
				}
				keypadcode = keypadcode|0x100;
	}

	if(single_code == KPD_UP)
	{
		for(j=0;j<AutoTest_key_count;j++)
		{
			if((AutoTest_key_code_count[j]&0xff) == (keypadcode&0xff))
			{

				AutoTest_key_code_count[j] = (keypadcode&0xff);

				//SCI_TRACE_LOW("  AutoTest_key_code_count[%d] = 0x%x",j,AutoTest_key_code_count[j]);
				break;
			}
		}
		keypadcode = keypadcode&0xff;
	}

	return;
}
#endif
LOCAL  void KeypadAutotestCallBackFunc(
                              BLOCK_ID id,    // Client thread ID
                              uint32   argc,  // Signal code  event!
                              void     *argv  // Key Code
                              )
{
    uint32  i =0;
    uint32  j =0;

    uint32      key_code     = (uint32)(argv);
    uint32      keymap_size = 0;
    uint16      single_code = (uint16)(argc & 0xFFFF);
    uint16      *kaymap_ptr;

    kaymap_ptr = KPD_GetKeyMap (&keymap_size);

    if(single_code == KPD_DOWN)
        {
            keypadcode = (uint16)(key_code&0xFFFF);
            for(i=0; i<keymap_size; i++)
            {
                    if((keypadcode == kaymap_ptr[i])&&(keypadcode != SCI_VK_INVALID_KEY))
                    {
                        keypad_in = i/8;
                        keypad_out = i%8;
                        break;
                    }
             }
        }
    return;
}

LOCAL  void KeypadAutotest_InOut(void)
{
    uint32  i =0;
    uint32  j =0;

    uint32      keymap_size = 0;
    uint16      *kaymap_ptr;

    kaymap_ptr = KPD_GetKeyMap (&keymap_size);

    for(i=0; i<keymap_size; i++)
    {
            if((keypadcode == kaymap_ptr[i])&&(keypadcode != SCI_VK_INVALID_KEY))
            {
                keypad_in = i/8;
                keypad_out = i%8;
                break;
            }
     }
    return;
}
//extern uint32 uiHeadSetButton;

LOCAL uint32 HandleKeypadAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )

{

		uint32      keymap_size = 0;
		uint16      *kaymap_ptr;
		uint8 i =0;
		uint8  cmmand = 0;
		wchar* opera_file_name = L"E:\\OperaMini\\mini4\\reksio.ini";
		MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
		cmmand = *(uint8 *)(msg_head + 1);

//           SCI_TRACE_LOW("{xx}HandleKeypadAutotest: cmmand=%d,uiHeadSetButton = 0x%x",cmmand,uiHeadSetButton);
        switch(cmmand)
            {
                case KEYPAD_INIT:
                    {
                        MMIENVSET_SetCurModeOptValue(5, 0, 4);
			#ifdef GLOBAL_DIAG_COMMAND
                        SCI_RegisterTaskMsg( KEYPAD_SERVICE,
                            (uint8)(KPD_DOWN & 0xff),
                            (uint8)(KPD_UP & 0xff),
                            DIAG,
                            KeypadAutotestCallBackFunc);
			#else
				SCI_MEMSET(AutoTest_key_code_count,0,sizeof(AutoTest_key_code_count));
				SCI_RegisterTaskMsg( KEYPAD_SERVICE,
                            (uint8)(KPD_DOWN & 0xff),
                            (uint8)(KPD_UP & 0xff),
                            DIAG,
                            KeypadAutotest_RecordAll_CallBackFunc);
			#endif

			            msg_head->subtype = ATCMD_SUCCESS;
        	            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                    }
                    break;


                case KEYPAD_ASK:
                    {
                        SCI_TRACE_LOW("{xx}HandleKeypadAutotest: keypadcode=%d",keypadcode);
                        if(keypadcode)
                            {
                            #ifdef GLOBAL_DIAG_COMMAND
					    *dest_len_ptr = sizeof(MSG_HEAD_T)+4;
                                    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                                    SCI_ASSERT(SCI_NULL != *dest_ptr);
                                    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));/*lint !e718 !e746 -e718 -e746*/

                                    *(*dest_ptr+sizeof(MSG_HEAD_T)) = keypad_out;
                                    *(*dest_ptr+sizeof(MSG_HEAD_T)+1) = keypad_in;
                                    if(SCI_VK_POWER == keypadcode) {
				            keypadcode = 0x74;
                                        }
                                    *(*dest_ptr+sizeof(MSG_HEAD_T)+2) = (keypadcode&0xFF00)>>16;
			               *(*dest_ptr+sizeof(MSG_HEAD_T)+3) = keypadcode&0x00FF;
            	                    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
            	                    ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
                                    keypadcode = 0;
				#else
                                    SCI_TRACE_LOW("{xx}HandleKeypadAutotest: 1");
 					   *dest_len_ptr = sizeof(MSG_HEAD_T)+2;
                                    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                                    SCI_ASSERT(SCI_NULL != *dest_ptr);

                                    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));/*lint !e718 !e746 -e718 -e746*/

                                    *(*dest_ptr+sizeof(MSG_HEAD_T)) = (keypadcode>>8);
                                    *(*dest_ptr+sizeof(MSG_HEAD_T)+1) = (keypadcode&0xff);
            	                    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
            	                    ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
                                    keypadcode = 0;

				 #endif
                            }
                        else
                            {
                                    msg_head->subtype = SCI_FAILURE;
                                    ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                            }
                    }
                    break;

                case KEYPAD_DONE:
                    {
			#ifdef GLOBAL_DIAG_COMMAND
                          SCI_UnregisterTaskMsg( KEYPAD_SERVICE,
                                (uint8)(KPD_DOWN & 0xff),
                                (uint8)(KPD_UP & 0xff),
                                DIAG,
                                KeypadAutotestCallBackFunc);

                            msg_head->subtype = ATCMD_SUCCESS;
                            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
			#else
                                    SCI_TRACE_LOW("{xx}HandleKeypadAutotest: 3");

				SCI_UnregisterTaskMsg( KEYPAD_SERVICE,
                                (uint8)(KPD_DOWN & 0xff),
                                (uint8)(KPD_UP & 0xff),
                                DIAG,
                                KeypadAutotest_RecordAll_CallBackFunc);

				if(keypadcode)
				{
					*dest_len_ptr = sizeof(MSG_HEAD_T)+2*AutoTest_key_count;
					*dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
					SCI_ASSERT(SCI_NULL != *dest_ptr);
    					SCI_MEMSET( *dest_ptr, 0, *dest_len_ptr);
					SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));/*lint !e718 !e746 -e718 -e746*/
					for(i=0;i<AutoTest_key_count;i++)
					{
						*(*dest_ptr+sizeof(MSG_HEAD_T)+2*i)= (( AutoTest_key_code_count[i]&0xff00)>>8);
						*(*dest_ptr+sizeof(MSG_HEAD_T)+2*i+1) =( AutoTest_key_code_count[i]&0xff);
					}
					((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
					((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
					keypadcode = 0;
				}
				else
				{
					msg_head->subtype = SCI_FAILURE;
					ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
				}
				AutoTest_key_count=0;


			#endif

                    }
                    break;

		   case KEYPAD_CODE:
		      {
                                    SCI_TRACE_LOW("{xx}HandleKeypadAutotest: 4");
				if(keypadcode)
				{

					*dest_len_ptr = sizeof(MSG_HEAD_T)+2;
	                            *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
	                            SCI_ASSERT(SCI_NULL != *dest_ptr);

	                            SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

					*(*dest_ptr+sizeof(MSG_HEAD_T)) = keypadcode/256;
					*(*dest_ptr+sizeof(MSG_HEAD_T)+1) = keypadcode%256;
	            	              ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
	            	              ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
					keypadcode = 0;
				}
                        	else
                        {
                                    SCI_TRACE_LOW("{xx}HandleKeypadAutotest: 5");
                                msg_head->subtype = SCI_FAILURE;
                                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                        }
		      }
	             break;
			case KEYPAD_OPERA_START:
				if(MMIFILE_IsFileExist(opera_file_name, MMIAPICOM_Wstrlen(opera_file_name)))
				{
					 msg_head->subtype = ATCMD_SUCCESS;
				}
				else
				{
					msg_head->subtype = SCI_FAILURE;
				}
				 ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
				break;
		    case KEYPAD_IDLE:
			     MMI_HanldeEndKeyForTest(); //return to mainmenu and then return to idle
			      msg_head->subtype = ATCMD_SUCCESS;
        	            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
			     break;
                 default:
                            // msg_head->subtype = ATCMD_BADCOMMAND;
                            //ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                             msg_head->subtype = 0x5A;
                            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                    break;
            }

        return 1;
}



#if !defined(FM_SUPPORT_NONE)

/*****************************************************************************/
//  Description:    This function is used to set voice codec enable/disable.
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EnableFmVoiceCodec ( // If succeed, return SCI_TRUE, else return error value
    BOOLEAN is_enable          // SCI_TRUE: Enable,   SCI_FALSE: Disable
)
{
#ifndef WIN32
    AUDIO_RESULT_E audio_result = 0;


    if (is_enable)
    {
        if(INVALID_HANDLE == FmHandle)
        {
            FmHandle = AUDIO_CreateNormalHandle(hNullCodec, PNULL, hFMDev, PNULL, PNULL);
            SCI_ASSERT(INVALID_HANDLE != FmHandle); /*assert verified*/
        }
        audio_result = AUDIO_Play (FmHandle, 0);
        SCI_TRACE_LOW("EnableFmVoiceCodec(TRUE) audio_result = %d, hAtcFmHandle=0x%x", audio_result, FmHandle);

    }
    else
    {
        audio_result = AUDIO_Stop (FmHandle);
        SCI_TRACE_LOW("EnableFmVoiceCodec(FALSE)ATC: AUDIO_Stop: audio_result = %d, hAtcFmHandle=0x%x", audio_result, FmHandle);
    }

    if(audio_result == AUDIO_NO_ERROR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

#else
        return TRUE;
#endif
}

LOCAL uint32 HandleFMAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    static uint32 s_fm_cur_test_freq = 0;
    uint8  result = ATCMD_SUCCESS;
    DIAG_MSG_FM_T  *MSG_HEAD = (DIAG_MSG_FM_T *)src_ptr;
    uint32 RSSI=0;
    uint32 freq=0;
    uint32 i = 0;
    MCU_MODE_E cali_mode = POWER_GetResetMode();

    switch(MSG_HEAD->cmmand)
    {
        case FM_INIT_PLAY:
        {
            //if(FM_ERROR != FM_Init())
            {
#ifdef FM_S_ANT_SUPPORT
                GPIO_SetFmLNA(0);
#endif

                //FM_Open();
                if (CALIBRATION_MODE != cali_mode)
                {
                    MMIAPIENVSET_SetTestMode();//silent mode
                    AUDIO_SetVolume(FM_VOLUME_LEVEL_MAX);
                    AUDIO_SetDevMode(AUDIO_DEVICE_MODE_EARPHONE);//earphone
                    EnableFmVoiceCodec(TRUE);
                }
                SCI_TRACE_LOW("MSG_HEAD->fm_channel = %d\n",MSG_HEAD->fm_channel);
                freq = ((MSG_HEAD->fm_channel) >>8) |  ((MSG_HEAD->fm_channel & 0xff) <<8);

                if (s_fm_cur_test_freq != (freq*10))
                {
                    if(FM_ERROR == FM_ManualSeek(freq*10,0,0,PNULL))
                    {
                        result = ATCMD_BADCOMMAND;
                    }
                    s_fm_cur_test_freq = freq*10;
                    SCI_Sleep(500);//delay
                }

                for (i=0; i<10; i++)
                {
                    RSSI += FM_OPERATION_get_inpwr_info_rt();
                }
                RSSI = RSSI/10;
		RSSI = (~RSSI+1)&0X7F;
		SCI_TraceLow("RSSI = %d\n",RSSI);

                RSSI= (uint32)((((RSSI)<<24)&0xff000000L)|(((RSSI)<<8)&0x00ff0000L)|(((RSSI)>>8)&0x0000ff00L)|(((RSSI)>>24)&0x000000ffL));//to adapt the tool
            }
            //else
            {
                //result = ATCMD_BADCOMMAND;
            }
        }
        break;

        case FM_STOP:
        {
            if (CALIBRATION_MODE != cali_mode)
            {
                EnableFmVoiceCodec(FALSE);
            }
            FM_Close();
            if (CALIBRATION_MODE != cali_mode)
            {
               MMIAPIENVSET_ClearTestMode();//disable slient mode
            }
            s_fm_cur_test_freq = 0;
            result = ATCMD_SUCCESS;
        }
        break;

        default:
            result = ATCMD_BADCOMMAND;
            break;
    }

    *dest_len_ptr = sizeof(MSG_HEAD_T)+4/*data*/;
    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
    SCI_ASSERT(SCI_NULL != *dest_ptr);

    SCI_MEMCPY(*dest_ptr, MSG_HEAD, sizeof(MSG_HEAD_T));/*lint !e516 -e516*/

    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
    ((MSG_HEAD_T  *)(*dest_ptr))->subtype = result;
    *(uint32*)(*dest_ptr+sizeof(MSG_HEAD_T)) = RSSI;

    return 1;
}
#endif

#if !defined(BLUETOOTH_SUPPORT_NONE)
LOCAL uint32 HandleBTAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
)
{
    BT_STATUS     return_value = BT_SUCCESS;
    uint8 i = 0;
    uint8  command = 0;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    command = *(uint8 *)(msg_head + 1);

    switch(command)
    {
        case BT_OPEN:
        {
            Is_BTSearchStart = SCI_FALSE;
            s_bt_dev_count = 0;
            BT_Start();
            SCI_Sleep(600);//UIS8910 BT RISC-V int slow
            msg_head->subtype = SCI_SUCCESS;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
         }
         break;

        case BT_SEARCH:
        {
           if( SCI_FALSE == Is_BTSearchStart)  // not start serach
            {
               return_value = BT_SearchDevice(BT_SERVICE_ALL);
                if(BT_PENDING != return_value)
                {
                    msg_head->subtype = SCI_FAILURE;
                    ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                    return 1;
                }
               else
                {
                     Is_BTSearchStart = SCI_TRUE;

                     SCI_Sleep(300);//bug1326928

                     msg_head->subtype = SCI_SUCCESS;
                     ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                     return 1;
                }
            }
           else
            {
               SCI_Sleep(100);
		 msg_head->subtype = SCI_SUCCESS;
               ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);

            }
        }
        break;

	case BT_GET_ADR:
                if(s_bt_dev_count)
                    {
                           *dest_len_ptr = sizeof(MSG_HEAD_T) + s_bt_dev_count*sizeof(BT_ADDRESS);
                           *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                            SCI_ASSERT(SCI_NULL != *dest_ptr);
                            //msg_head->len = *dest_len_ptr;
                            //msg_head->subtype = SCI_SUCCESS;
                            SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                            for(i = 0; i < s_bt_dev_count; i++)
                            {
                                SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T)+i*sizeof(BT_ADDRESS), s_bt_dev_info[i].addr.addr, sizeof(BT_ADDRESS));
                            }
				((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
				((MSG_HEAD_T  *)(*dest_ptr))->subtype  = SCI_SUCCESS;
                    }
                else
                    {
                        msg_head->subtype = SCI_SUCCESS;
                        ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                    }
		break;

        case BT_CLOSE:
        {
            if(Is_BTSearchStart == SCI_TRUE)
            {
                BT_CancelSearch();
                SCI_Sleep(100);
            }

	   msg_head->subtype = SCI_SUCCESS;
	    if(BT_GetState())			//BT had already opened
	    {
		     return_value =  BT_Stop();

		     SCI_Sleep(100);
		     if(BT_PENDING != return_value)
	            {
				msg_head->subtype = SCI_FAILURE;
		     }
		     else
		    {
				msg_head->subtype = SCI_SUCCESS;
		    }
	    }


            SCI_Sleep(100);
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
        }
        break;

        case BT_RSSI:
        {
            uint8 addr_and_rssi_data[10] = {0};

            if (s_bt_dev_count)
            {
                *dest_len_ptr = sizeof(MSG_HEAD_T) + s_bt_dev_count*(sizeof(BT_ADDRESS)+4);
                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                SCI_ASSERT(SCI_NULL != *dest_ptr);
                //msg_head->len = *dest_len_ptr;
                //msg_head->subtype = SCI_SUCCESS;
                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

                for(i = 0; i < s_bt_dev_count; i++)
                {
                    SCI_MEMCPY(&addr_and_rssi_data[0], s_bt_dev_info[i].addr.addr, sizeof(BT_ADDRESS));
                    SCI_MEMCPY(&addr_and_rssi_data[6], &(s_bt_dev_info[i].rssi), 4);
                    SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T)+i*(sizeof(BT_ADDRESS)+4), addr_and_rssi_data, (sizeof(BT_ADDRESS)+4));// 6_bytes address + 4_bytes rssi
                }

                ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = SCI_SUCCESS;
            }
            else
            {
                msg_head->subtype = SCI_SUCCESS;
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            }
        }
        break;

        default:
            msg_head->subtype = SCI_FAILURE;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            break;
    }

    return 1;
}
#endif

LOCAL uint32 HandleChargeAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
)
{
    uint8  command = 0;
    uint8 result = 0;
    CHGMNG_STATE_INFO_T* charge_state = PNULL;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;

    command = *(uint8 *)(msg_head + 1);

    switch(command)
    {
        case CHARGE_OPEN:
                {
                   CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_IN_MSG, 0);
                   SCI_SLEEP(10);
                   msg_head->subtype = ATCMD_SUCCESS;
                   ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                }
           break;

        case CHARGE_READ:
                {
                   msg_head->subtype = ATCMD_SUCCESS;
                   ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                }
           break;


        case CHARGE_CLOSE:
                {
                    CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PESUDO_PLUG_OUT_MSG, 0);
                    SCI_SLEEP(10);
                    msg_head->subtype = ATCMD_SUCCESS;
                    ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                }
          break;

          default:
              msg_head->subtype = ATCMD_BADCOMMAND;
              ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
              break;
    }
    return 1;
}

#if defined(GPS_SUPPORT) &&  defined(GPS_CHIP_VER_GREENEYE2)
LOCAL GPS_ERR_E GPS_StartBy_Autotest()
{
    GPS_ERR_E err_val = GPS_ERR_NONE;
#if 0
    GPS_Init_Request_Factory();
    GPS_Reset_Request_Factory(GPS_HOT_MODE);
#endif
    return err_val;
}

LOCAL uint32 HandleGpsAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
)
{
    uint8  command = 0;
    uint8 result = 0;
    CHGMNG_STATE_INFO_T* charge_state = PNULL;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    GPS_ERR_E err_val = GPS_ERR_NONE;
    uint8   satellites;
    uint8 status;
    static BOOLEAN open_flag = SCI_FALSE;
    static BOOLEAN download_flag = SCI_FALSE;
    uint32 cwcn = 0;
    GpsSvStatus svinfo;
    uint8 i = 0;

    command = *(uint8 *)(msg_head + 1);
    switch(command)
    {
#if 0//TBD
        case GPS_OPEN:
            {
		if(!download_flag)
                	err_val = GPS_Download();
		else
			GPS_Reset_Request_Factory(GPS_HOT_MODE);
                if(GPS_ERR_NONE == err_val)
                {
                    download_flag = SCI_TRUE;
                    msg_head->subtype = ATCMD_SUCCESS;
                }
                else
                {
                    msg_head->subtype = ATCMD_BADCOMMAND;
                }
                //SCI_TRACE_LOW("[GE2]%s,GPS_OPEN,sub_type=%d,len=%d",__FUNCTION__,msg_head->subtype,*dest_len_ptr);
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            }
            break;
        case GPS_SEARCH:
            {
                if(!open_flag)
                {
                    err_val = GPS_StartBy_Autotest();
                }
                if(GPS_ERR_NONE == err_val)
                {
                    open_flag = SCI_TRUE;
                    status = GPS_Get_FixStatus();
                    if(0x1 >= status)
                        msg_head->subtype = ATCMD_BADCOMMAND;
                    else
                        msg_head->subtype = ATCMD_SUCCESS;
                }
                else
                {
                    msg_head->subtype = ATCMD_BADCOMMAND;
                }
                //SCI_TRACE_LOW("[GE2]%s,GPS_SEARCH,sub_type=%d,len=%d",__FUNCTION__,msg_head->subtype,*dest_len_ptr);
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            }
            break;
        case GPS_READ_SATELLITES:
            {
                satellites = GPS_Get_FixSatellites();
                msg_head->subtype = ATCMD_SUCCESS;

                *dest_ptr 	  = SCI_ALLOC_APP(sizeof(MSG_HEAD_T) + 0x1);
                *(*dest_ptr + sizeof(MSG_HEAD_T)) = satellites;
                *dest_len_ptr = sizeof(MSG_HEAD_T) + 0x1;
                SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
                ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
                //SCI_TRACE_LOW("[GE2]%s,GPS_READ_SATELLITES,sub_type=%d,len=%d",__FUNCTION__,msg_head->subtype,*dest_len_ptr);
            }
            break;
        case GPS_CLOSE:
            {
                err_val = GPS_Close();
                if(GPS_ERR_NONE == err_val)
                    msg_head->subtype = ATCMD_SUCCESS;
                else
                    msg_head->subtype = ATCMD_BADCOMMAND;
                msg_head->subtype = ATCMD_SUCCESS;
                //open_flag = SCI_FALSE;
                //SCI_TRACE_LOW("[GE2]%s,GPS_CLOSE,sub_type=%d,len=%d",__FUNCTION__,msg_head->subtype,*dest_len_ptr);
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            }
            break;
        case GPS_READ_RSSI:
            {
                GPS_Get_Svinfo(&svinfo);
                snr_max = 0;
                msg_head->subtype = ATCMD_SUCCESS;

                *dest_ptr 	  = SCI_ALLOC_APP(sizeof(GPS_DIAG_OUTPUT_SVINFO_T));
                SCI_MEMSET(*dest_ptr,0x0,sizeof(GPS_DIAG_OUTPUT_SVINFO_T));
                ((GPS_DIAG_OUTPUT_SVINFO_T *)*dest_ptr)->svinfo.num_svs = svinfo.num_svs;
                for(i = 0; i < svinfo.num_svs;i++)
                {
                    ((GPS_DIAG_OUTPUT_SVINFO_T *)*dest_ptr)->svinfo.snr[i] = svinfo.sv_list[i].snr;
                    if (snr_max < svinfo.sv_list[i].snr)
                        snr_max = svinfo.sv_list[i].snr;
                }
                *dest_len_ptr = (sizeof(MSG_HEAD_T) + svinfo.num_svs + 0x1);
                SCI_MEMCPY(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
                ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
                snr_len = *dest_len_ptr;
                //SCI_TRACE_LOW("[GE2]%s,GPS_READ_RSSI,sub_type=%d,len=%d",__FUNCTION__,msg_head->subtype,*dest_len_ptr);
            }
            break;
#endif

        default:
            msg_head->subtype = ATCMD_BADCOMMAND;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            break;
    }

    return 1;
}
#endif

typedef enum BBAT_MIC_SET
{
    BBAT_MIC_SET_DEFAULT_NORMAL = 0,
    BBAT_MIC_SET_BBAT_MODE,
    BBAT_MIC_SET_BBAT_FORCE_MAINMIC,
    BBAT_MIC_SET_BBAT_FORCE_EARMIC
}BBAT_MIC_SET_E;

LOCAL uint32 Handle_MicAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    uint32 ret = SCI_FAILURE;
    uint16 *buff_ptr = PNULL;
    MSG_HEAD_T *msg_head = (MSG_HEAD_T *)src_ptr;
    DIAG_MSG_AUDIO_T *MSG_HEAD = (DIAG_MSG_AUDIO_T *)src_ptr;
    DIAG_MSG_AUDIOLOOP_T *MSG_HEAD_LOOP = (DIAG_MSG_AUDIOLOOP_T *)src_ptr;
    AUDIO_OUT_DEV_TYPE_E out_dev = AUDIO_OUT_DEFAULT;
    AUDIO_IN_DEV_TYPE_E in_dev = AUDIO_IN_DEFAULT;

    switch (MSG_HEAD->dev_mode) {
    case 0x00:  // main mic
        switch (MSG_HEAD->cmmand) {
        case 0x01:  // start
            /* dev_mode: only support main mic */
            ret= Autotest_Mic_Start(AUDIO_DEVICE_MODE_HANDFREE);
            msg_head->subtype = (SCI_FAILURE != ret)? ATCMD_SUCCESS :ATCMD_BADPARAMETER;
            break;

        case 0x02:  // state
            ret = Autotest_GetMicState();
            msg_head->subtype = (SCI_FAILURE != ret)? ATCMD_SUCCESS :ATCMD_BADPARAMETER;
            break;

        case 0x03:  //read data
            if (SCI_FAILURE != Autotest_GetMicState()) {
                buff_ptr = Autotest_GetDataAddr(MSG_HEAD->subcmmand); // block 0~3
                if (PNULL != buff_ptr) {
                    //AMR_REOCED_File((uint8*)buff_ptr,MICBUFFLEN);
                    *dest_len_ptr = sizeof(MSG_HEAD_T) + MIC_BUFFLEN;
                    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                    SCI_ASSERT(SCI_NULL != *dest_ptr);

                    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                    SCI_MEMCPY((*dest_ptr) + sizeof(MSG_HEAD_T), buff_ptr, MIC_BUFFLEN);
                    ((MSG_HEAD_T *)(*dest_ptr))->len = *dest_len_ptr;
                    ((MSG_HEAD_T *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
                    goto print_return;
                } else {
                    msg_head->subtype = ATCMD_BADADDRESS;
                }
            } else {
                msg_head->subtype = ATCMD_BADADDRESS; // has no enough data
            }
            break;

        case 0x04:  // stop
            ret = Autotest_Mic_Stop();
            msg_head->subtype = (SCI_FAILURE != ret)? ATCMD_SUCCESS :ATCMD_BADPARAMETER;

            break;

        default:
            SCI_TRACE_LOW("[AUTOTEST][%s] unsupport cmd", __FUNCTION__);
            msg_head->subtype = ATCMD_BADCOMMAND;
            break;
        }
        break;

    case 0x10:  // loop start
    case 0x11:  // loop stop
        {
            uint32 ops = SCI_FALSE, res_loop = SCI_FALSE;
            // only support main mic and speaker
            ops = (0x10 == MSG_HEAD->dev_mode)? SCI_TRUE : SCI_FALSE;
            res_loop = AUD_EnableVoiceLoopback(ops, 0, 0, 100);

            msg_head->subtype = (SCI_FAILURE != res_loop)? ATCMD_SUCCESS :ATCMD_BADPARAMETER;
            break;
        }
    case 0x01:  // hp mic
    case 0x02:  // aux mic
    default:
        SCI_TRACE_LOW("[AUTOTEST][%s] unsupport dev_mode", __FUNCTION__);
        msg_head->subtype = ATCMD_BADCOMMAND;
        break;
    }

    ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);

print_return:
    SCI_TRACE_LOW("[AUTOTEST][%s] entry dev:0x%x cmd:0x%x subcmd:0x%x res:%d",
                __FUNCTION__,
                MSG_HEAD->dev_mode,
                MSG_HEAD->cmmand,
                MSG_HEAD->subcmmand,
                msg_head->subtype);

    return 1;
}

LOCAL uint32 Handle_SpeakerAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
)
{
    uint32 ret = 0x0;
    uint32 pcm_len = 0;
    MSG_HEAD_T *msg_head = (MSG_HEAD_T *)src_ptr;
    DIAG_MSG_AUDIO_T *MSG_HEAD = (DIAG_MSG_AUDIO_T *)src_ptr;

    pcm_len = (uint32)(msg_head->len - sizeof(DIAG_MSG_AUDIO_T) + 1);

    SCI_TRACE_BUF("[AUTOTEST][]", src_ptr, src_len);

    switch (MSG_HEAD->cmmand) {
    case 0x01:  // start local
        Autotest_Speaker_Stop();
        /* dev_mode: 0:SPK 1:RCV 2:HP */
        ret = Autotest_Speaker_Start_Local(MSG_HEAD->dev_mode);
        msg_head->subtype = (SCI_FAILURE != ret)? ATCMD_SUCCESS :ATCMD_BADPARAMETER;
        break;

    case 0x02:
        Autotest_Speaker_Stop();
        ret= Autotest_Speaker_Start(\
                            (uint8*)(src_ptr + sizeof(DIAG_MSG_AUDIO_T) - 1),\
                            pcm_len,\
                            MSG_HEAD->dev_mode,\
                            MSG_HEAD->subcmmand);
        msg_head->subtype = (SCI_FAILURE != ret)? ATCMD_SUCCESS :ATCMD_BADPARAMETER;
        break;

    case 0x03:
        ret= Autotest_Speaker_Stop();
        msg_head->subtype = (SCI_FAILURE != ret)? ATCMD_SUCCESS :ATCMD_BADPARAMETER;
        break;

    default:
        SCI_TRACE_LOW("[AUTOTEST][%s] unsupport cmd", __FUNCTION__);
        msg_head->subtype = ATCMD_BADCOMMAND;
        break;
    }

    ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
    SCI_TRACE_LOW("[AUTOTEST][%s] entry dev:0x%x cmd:0x%x subcmd:0x%x res:%d",
                __FUNCTION__,
                MSG_HEAD->dev_mode,
                MSG_HEAD->cmmand,
                MSG_HEAD->subcmmand,
                msg_head->subtype);
    return 1;
}

/*****************************************************************************/
//  FUNCTION:     _Read_One_Reg
//  Description:  This function will read regs through i2c
//	Note:
/*****************************************************************************/
LOCAL uint8 Read_One_Reg(uint8 reg_addr)
{

	uint8  reg_value = 0;

	if((AUTODEV_IIC_Handler == -1) &&
		((AUTODEV_IIC_Handler = I2C_HAL_Open(&AUTODEV_IIC_cfg)) == -1))
	{

		return SCI_FAILURE;
	}

    I2C_HAL_Read(AUTODEV_IIC_Handler, &reg_addr, &reg_value, 1);

	if(AUTODEV_IIC_Handler != -1)
	{
 		I2C_HAL_Close(AUTODEV_IIC_Handler);
		AUTODEV_IIC_Handler = -1;
	}

    return reg_value;

}


LOCAL uint32 HandleIicDeviceAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    DIAG_MSG_IIC_T  *MSG_HEAD = (DIAG_MSG_IIC_T *)src_ptr;

    uint8 iic_addr = MSG_HEAD->iic_addr;
    uint8 reg_addr = MSG_HEAD->reg_addr;
    uint8 port = MSG_HEAD->port;
    BOOLEAN ack_en = MSG_HEAD->ack_en;

    uint8 reg_value = 0;

    AUTODEV_IIC_cfg.id = port;
    AUTODEV_IIC_cfg.check_ack = ack_en;
    AUTODEV_IIC_cfg.slave_addr= iic_addr;

    reg_value = Read_One_Reg(reg_addr);

    *dest_len_ptr = sizeof(MSG_HEAD_T) + 2;
    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);

    SCI_ASSERT(SCI_NULL != *dest_ptr);
    SCI_MEMCPY(*dest_ptr, MSG_HEAD, sizeof(MSG_HEAD_T));

    *(*dest_ptr+sizeof(MSG_HEAD_T)) = reg_value;  //?
    *(*dest_ptr+sizeof(MSG_HEAD_T)+1) = 0;  //?

    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
    ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;

    return 1;
}

LOCAL uint32 HandleTFAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )

{
		uint8  ret = 0;
		wchar at_sdcard[2]  = { 'E', 0 };
		MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
		SFS_ERROR_E device_error = 0;
#ifdef  SDCARD_SUPPORT
		if (( SCM_NOT_EXIST != SCM_GetSlotStatus(SCM_SLOT_0)) && (SFS_ERROR_NONE == SFS_GetDeviceStatus(at_sdcard)))
		{
			ret = SCI_SUCCESS;
		}
		else
		{
			if(SCM_IOCTL(SCM_SLOT_0,SCM_FUNC_REINIT,PNULL))
			{
				device_error = MMIAPIFMM_RegisterDevice ( at_sdcard,MMIAPICOM_Wstrlen(at_sdcard));

				if (SFS_NO_ERROR == device_error)
				{
					if (SFS_ERROR_NONE == SFS_GetDeviceStatus(at_sdcard))
					{
						ret = SCI_SUCCESS;
					}
				}
				else
				{
					ret = SCI_FAILURE;
				}
			}
			else
			{
				ret = SCI_FAILURE;
			}

		}

		*dest_len_ptr = sizeof(MSG_HEAD_T);
		*dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
		SCI_ASSERT(SCI_NULL != *dest_ptr);

		SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

		((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
		((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ret;
#endif
        return 1;
}


LOCAL uint32 HandleSIMAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    AUTOTEST_SIM_E  ret = 0;
    BOOLEAN     sim_status = SCI_FALSE;
    int8    rc = 0;
    DIAG_MSG_SIM_T  *msg_head = (DIAG_MSG_SIM_T *)src_ptr;
    switch((MN_DUAL_SYS_E)(msg_head->sim_select))
    {
      case MN_DUAL_SYS_1:
        rc = Get_SimCard_Status(&msg_head->msg_head, MN_DUAL_SYS_1);
        if (rc < 0) {
            return 1;
        } else {
            sim_status = (rc?  SCI_TRUE:SCI_FALSE);
        }

        if(!sim_status)
        {
            ret=SIM_FAIL;
        }
        else
        {
            ret=SIM_OK;
        }
        break;

     case MN_DUAL_SYS_2:
        rc = Get_SimCard_Status(&msg_head->msg_head, MN_DUAL_SYS_2);
        if (rc < 0) {
            return 1;
        } else {
            sim_status = (rc?  SCI_TRUE:SCI_FALSE);
        }

        if(!sim_status)
        {
            ret=SIM_FAIL;
        }
        else
        {
            ret=SIM_OK;
        }
        break;

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case MN_DUAL_SYS_3:
        rc = Get_SimCard_Status(&msg_head->msg_head, MN_DUAL_SYS_3);
        if (rc < 0) {
            return 1;
        } else {
            sim_status = (rc?  SCI_TRUE:SCI_FALSE);
        }

        if(!sim_status)
        {
            ret=SIM_FAIL;
        }
        else
        {
            ret=SIM_OK;
        }
        break;
#endif

#ifdef MMI_MULTI_SIM_SYS_QUAD
     case MN_DUAL_SYS_4:
        rc = Get_SimCard_Status(&msg_head->msg_head, MN_DUAL_SYS_4);
        if (rc < 0) {
            return 1;
        } else {
            sim_status = (rc?  SCI_TRUE:SCI_FALSE);
        }

        if(!sim_status)
        {
            ret=SIM_FAIL;
        }
        else
        {
            ret=SIM_OK;
        }
        break;
#endif
        default:
            ret = SIM_FAIL;
        	break;

    }

    *dest_len_ptr = sizeof(MSG_HEAD_T);
    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
    SCI_ASSERT(SCI_NULL != *dest_ptr);

    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;


    ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = (uint8)ret;
     //SCI_MEMSET(*dest_ptr+sizeof(MSG_HEAD_T),(uint8)ret,1);

    return 1;
}

/**************************************************/
/**                      HandlePARALLEL_LCDAutotest
/**
/**************************************************/
#define AUTOTEST_COLOR_BLACK		0x20E4
#define AUTOTEST_COLOR_WHITE		0xFFFF
#define AUTOTEST_COLOR_RED		0xF800
#define AUTOTEST_COLOR_GREEN		0x07E0
#define AUTOTEST_COLOR_BLUE		0x001F




void AutoTest_LCD_Half_Color(LCD_ID_E lcd_id, uint32 color1,uint32 color2)
{
	LCD_INFO_T lcd_info = {0};
	uint32 i;
	BLOCKCFG_T cfg = {0};
	uint16 *Lcd_Buffer_Ptr = PNULL;


	LCD_GetInfo (lcd_id,&lcd_info);

	LCD_DisableAllBlock (lcd_id);
	Lcd_Buffer_Ptr = (uint16 *)(lcd_info.lcdbuff_ptr);
	for (i = 0; i < (lcd_info.lcd_width * lcd_info.lcd_height)/2; i++)
	{
	    *(Lcd_Buffer_Ptr)++ = color1;
	}
	i=(lcd_info.lcd_width * lcd_info.lcd_height)/2;
	for (i; i < lcd_info.lcd_width * lcd_info.lcd_height; i++)
	{
	    *(Lcd_Buffer_Ptr)++ = color2;
	}
	cfg.colorkey         = 0;
	cfg.resolution       = 2;
	cfg.type             = 1;
	cfg.priority         = 0;
	cfg.start_x          = 0;
	cfg.start_y          = 0;
	cfg.end_x            = lcd_info.lcd_width -1;
	cfg.end_y            = lcd_info.lcd_height - 1;
	cfg.width            =  lcd_info.lcd_width;
	cfg.alpha            = 0xff;
	cfg.alpha_sel        = 1;
	cfg.colorkey_en      = FALSE;

	LCD_ConfigBlock (lcd_id, 0, cfg);

	LCD_SetBlockBuffer (lcd_id, 0, lcd_info.lcdbuff_ptr);

	LCD_EnableBlock(lcd_id, 0);

	LCD_Invalidate (lcd_id);

}

LOCAL uint32 HandlePARALLEL_LCDAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
	uint16    j=0;
	uint8     ret = 0;
	DIAG_MSG_LCD_T  *msg_head = (DIAG_MSG_LCD_T *)src_ptr;
	uint32 iDrawColor[]={AUTOTEST_COLOR_WHITE,AUTOTEST_COLOR_BLACK,AUTOTEST_COLOR_RED,AUTOTEST_COLOR_GREEN,AUTOTEST_COLOR_BLUE};
    switch((AUTOTEST_LCD_E)(msg_head->cmmand))
    {
        case LCD_WRITE_START:
            {
				LCD_EnterSleep(MAINLCD_ID, SCI_FALSE);
				for(j = 0;j < 32;j++)
				{
				  ret = LCM_SendData(msg_head->data,MAINLCD_ID);
	       		}
		}
           break;
        case LCD_CLOSE_DEVICE:
        case LCD_WRITE_END:
            {
			GPIO_SetLcdBackLight(SCI_FALSE);
			LCD_EnterSleep(MAINLCD_ID, SCI_TRUE);
			LCD_Close();
			ret = SCI_SUCCESS;
		}
            break;

        case LCD_READ_START:
        case LCD_READ_END:
        	 ret = SCI_SUCCESS;
            break;
	  case LCD_COLOUR_LOOP:
	  {

		for(j=0;j<5;j++)
		 {
			LCD_Clear(MAIN_LCD_ID, iDrawColor[j]);
   		 	SCI_SLEEP(1200);
	  	}

		ret=SCI_SUCCESS;
	  }

	  break;
	case  LCD_HALF_COLOR :
		AutoTest_LCD_Half_Color(MAIN_LCD_ID,iDrawColor[0],iDrawColor[1]);
		ret = SCI_SUCCESS;
	break;
        case LCD_DRAW_COLOUR://start lcd draw calour test
        {

            switch(msg_head->reserve)
            {
                        case DRAW_COLOUR_WHITE:
                        LCD_Clear(MAIN_LCD_ID, iDrawColor[0]);//white
                        ret = SCI_SUCCESS;
                        break;
                        case DRAW_COLOUR_RED:
                          LCD_Clear(MAIN_LCD_ID, iDrawColor[2]);//red
                        ret = SCI_SUCCESS;
                        break;
                        case DRAW_COLOUR_GREEN:
                          LCD_Clear(MAIN_LCD_ID, iDrawColor[3]);//green
                        ret = SCI_SUCCESS;
                        break;
                        case DRAW_COLOUR_BLUE:
                         LCD_Clear(MAIN_LCD_ID, iDrawColor[4]);//blue
                        ret = SCI_SUCCESS;
                        break;
                        case DRAW_COLOUR_BLACK:
                          LCD_Clear(MAIN_LCD_ID, iDrawColor[1]);//black
                        ret = SCI_SUCCESS;
                        break;
                        case DISABLE_LCD_KEYLOCK://disable lcd key lock
                        MMIDEFAULT_TurnOnLCDBackLight();
                        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                         ret = SCI_SUCCESS;
                        break;
                        case ENABLE_LCD_KEYLOCK://restore lcd keylock
                        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                        ret = SCI_SUCCESS;
                        break;
                        default://error commandtg
                        ret = SCI_FAILURE;
                        break;
            }
        }
        break;
        default:
	 	ret = SCI_FAILURE;
        break;
        }

    *dest_len_ptr = sizeof(MSG_HEAD_T);
    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
    SCI_ASSERT(SCI_NULL != *dest_ptr);
    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

	((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
	((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ret;

    return 1;
}

#ifdef MAINLCM_INTERFACE_SPI	//LCM_INTERFACE_SPI mengzhao
LOCAL uint32 HandleSpi_LCDAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
	uint8  ret = 0;
	DIAG_MSG_LCD_T  *msg_head = (DIAG_MSG_LCD_T *)src_ptr;
	uint32    j = 0;

    switch((AUTOTEST_LCD_E)(msg_head->cmmand))
    {
        case LCD_WRITE_START:
            {
        		LCD_EnterSleep(MAINLCD_ID, SCI_FALSE);
        		GPIO_SetLcdBackLight(SCI_TRUE);

        		for(j=0;j<32;j++)
        		{
        		  ret = LCM_SendData(msg_head->data,MAINLCD_ID);
        		}
               *dest_len_ptr = sizeof(MSG_HEAD_T);
    		}
            break;
        case LCD_WRITE_END:
    		{
    		   LCD_Close();
    		   GPIO_SetLcdBackLight(SCI_FALSE);
               *dest_len_ptr = sizeof(MSG_HEAD_T);
    		}
            break;

        case LCD_READ_START:
        case LCD_READ_END:
            ret = SCI_SUCCESS;
    		break;

    		default:
    			ret = SCI_FAILURE;
    		    break;
    }

    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
    SCI_ASSERT(SCI_NULL != *dest_ptr);

    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

    ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ret;
    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
    return 1;
}
#endif

LOCAL uint32 HandleAntennaTest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    uint32 rc = 1;
    DIAG_MSG_ANTTEST_T  *msg_head = (DIAG_MSG_ANTTEST_T *)src_ptr;
#if 1
    rc = CFW_CfgSetAntenna(&msg_head->data, sizeof(DIAG_MSG_ANTTEST_DATA_T));
#endif
    *dest_len_ptr = sizeof(MSG_HEAD_T) + sizeof(rc);
    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
    SCI_ASSERT(SCI_NULL != *dest_ptr);
    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
    *(uint32 *)((MSG_HEAD_T *)(*dest_ptr) + 1) = rc;

    return 1;
}

LOCAL uint32 HandleDevice_MiscAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{

	BOOLEAN         flag = SCI_SUCCESS;
	uint8           value = SCI_SUCCESS;
	int32           vbatTemp = 0;
	uint8           vbatFromat[4] = {0};
	uint32          i;
	LOCAL PROD_PARAM_T tprod_param = {0};      //used for product parameters

	
	DIAG_MSG_MISC_T  *msg_head = (DIAG_MSG_MISC_T *)src_ptr;

	if(msg_head->set_data >=100)
	{
		msg_head->set_data =100;
	}

	SCI_TRACE_LOW("HandleDevice_MiscAutotest dev_mode=%d", msg_head->dev_mode);

	switch((AUTOTEST_ANA_DEV_ID_E)(msg_head->dev_mode))
	{
		case AUTOTEST_ANA_DEV_ID_LCM_BL:
			GPIO_SetLcdBackLight (SCI_TRUE);
			ANA_SetDevValule(ANA_DEV_ID_LCM_BL, msg_head->set_data);
		break;

		case AUTOTEST_ANA_DEV_ID_KPD_BL:
			ANA_SetDevValule(ANA_DEV_ID_KPD_BL, msg_head->set_data);
		break;

		case AUTOTEST_ANA_DEV_ID_VIBRATOR:
#if !defined(PRODUCT_CONFIG_uis8910ff_mpphone) && !defined(PRODUCT_CONFIG_uis8910ff_zxf)
			ANA_SetDevValule(ANA_DEV_ID_VIBRATOR, msg_head->set_data);
#else
		{
			extern void MMIAPISET_SetVibratorEx(BOOLEAN is_open, MMISET_ALL_RING_TYPE_E  ring_type, BOOLEAN is_period, uint32 duration);
			BOOLEAN is_open = FALSE;

            		if (msg_head->set_data == 100)
            		{
                		is_open = TRUE;
            		}
			else 
			{
				is_open = FALSE;
			}
            		MMIAPISET_SetVibratorEx(is_open,MMISET_RING_TYPE_NONE,TRUE,1000);
        	}
#endif
            break;
        case AUTOTEST_ANA_DEV_ID_HEADSET:
			if(GPIO_CheckHeadsetStatus())
			{
				value = SCI_FAILURE;
			}
			break;

        case AUTOTEST_ANA_DEV_ID_FLASH_LIGHT:
			ANA_SetDevValule(ANA_DEV_ID_FLASH_LIGHT, msg_head->set_data);
        break;

        case AUTOTEST_ANA_DEV_ID_BAT_TEMP:
			//vbatTemp = CHGMNG_GetVbatTempVolValue();
			vbatTemp = CHGMNG_GetNTC_TempVolValue(); /* W217 NTC */

			/* 工具计算方式: float fTemp=(szRecv[0]+szRecv[1]+szRecv[2]+szRecv[3]-250)/10.0 摄氏度 */
			vbatTemp = (vbatTemp * 10) + 250;

			/* 最小温度 -25 */
			SCI_MEMSET(vbatFromat, 0x0, sizeof(vbatFromat));
			for  (i = 0; i < 4; i++)
			{
				if ( vbatTemp > 0xFF )
				{
					vbatFromat[i] = 0xFF;
					vbatTemp -= 0xFF;
				}
				else
				{
					vbatFromat[i] = vbatTemp;
					break;
				}
			}

        break;
        case AUTOTEST_ANA_DEV_ID_THERMAL:
                THERMAL_Ctrl_Set(msg_head->set_data);
                SCI_TRACE_LOW("Thermal Ctrl Value =%d", msg_head->set_data);
                EFS_NvitemRead(NV_PROD_PARAM, sizeof(tprod_param), (uint8 *)(&tprod_param));
                SCI_TRACE_LOW("Thermal Ctrl Value =%d, tprod_param.chr_param.reseved[0] = %d", msg_head->set_data, tprod_param.chr_param.reseved[0]);
                if(tprod_param.chr_param.reseved[0] != msg_head->set_data)
                {
                    tprod_param.chr_param.reseved[0] = msg_head->set_data;
                    EFS_NvitemWrite(NV_PROD_PARAM, sizeof(tprod_param), (uint8 *)(&tprod_param), TRUE);
                    SCI_TRACE_LOW("Thermal Ctrl Value is update success = %d", tprod_param.chr_param.reseved[0]);
                }
                 //EFS_NvitemWrite(NV_PROD_PARAM, sizeof(t_prod_param), (uint8 *)(&t_prod_param), 0);
        break;
        default:
            flag = SCI_FAILURE;
            break;
    }

    if (AUTOTEST_ANA_DEV_ID_BAT_TEMP == (AUTOTEST_ANA_DEV_ID_E)(msg_head->dev_mode))
    {
        *dest_len_ptr = sizeof(MSG_HEAD_T) + sizeof(vbatFromat);
        *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
        SCI_ASSERT(SCI_NULL != *dest_ptr);
        SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
        ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
        ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = SCI_SUCCESS;
         SCI_MEMCPY(*dest_ptr+sizeof(MSG_HEAD_T),vbatFromat, sizeof(vbatFromat));

        SCI_TRACE_LOW("HandleDevice_MiscAutotest vbatFromat=0x%x0x%x0x%x0x%x", vbatFromat[0],vbatFromat[1],vbatFromat[2],vbatFromat[3]);

    }
    else if(flag == SCI_SUCCESS)
    {
        *dest_len_ptr = sizeof(MSG_HEAD_T)+1;
        *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
        SCI_ASSERT(SCI_NULL != *dest_ptr);
        SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
        ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
        ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = SCI_SUCCESS;
         SCI_MEMSET(*dest_ptr+sizeof(MSG_HEAD_T),(uint8)value,1);
    }
    else
    {
        *dest_len_ptr = sizeof(MSG_HEAD_T);
        *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
        SCI_ASSERT(SCI_NULL != *dest_ptr);
        SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
        ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = SCI_FAILURE;
        ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
    }
    return 1;
}

#if 0
LOCAL void Autodev_PM_GPIOInit (void)
{
    int i = 0;
    uint16 gpio_num;

    // Enable all GPIO pages
    for (;;)
    {
        if (autodev_pm_gpio_map[i].gpio_num == PM_INVALID_SHORT_VAL)
        {
            break;
        }

        gpio_num  = autodev_pm_gpio_map[i].gpio_num;

  		SCI_TRACE_LOW("Autodev_PM_Init: gpio_num:%d,  %d,  %d",gpio_num,autodev_pm_gpio_map[i].dir,autodev_pm_gpio_map[i].default_val);

        if (autodev_pm_gpio_map[i].dir == PM_OUTPUT)
        {
            // Output only
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be OUTPUT.(1)
            GPIO_SetDirection (gpio_num,SCI_TRUE);
            // MASK it.(0)
            GPIO_DisableIntCtl (gpio_num);
            // Set default value
            if (0xFFFF != autodev_pm_gpio_map[i].default_val)
            {
                GPIO_SetValue (gpio_num,autodev_pm_gpio_map[i].default_val);
            }
        }
        else
        {
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be INPUT.(0)
            GPIO_SetDirection (gpio_num,SCI_FALSE);
        }
        i++;
    }
}

PUBLIC void Autodev_PM_Init (void)
{
    int i = 0;

    for (;;)
    {
        // Check if search to end
        if (autodev_pm_func[i].addr == PM_INVALID_VAL)
        {
            break;
        }

  		SCI_TRACE_LOW("Autodev_PM_Init: i:%d,  %d",i,ANA_IS_ANA_REG (autodev_pm_func[i].addr));

        if (!ANA_IS_ANA_REG (autodev_pm_func[i].addr))
        {
            CHIP_REG_SET (autodev_pm_func[i].addr,  autodev_pm_func[i].value);
        }
        else
        {
            ANA_REG_SET (autodev_pm_func[i].addr,   autodev_pm_func[i].value);
        }
        i++;
    }

    Autodev_PM_GPIOInit();
}

#endif

#if defined(PLATFORM_UWS6121E)
uint8 AutoTest_GetRealGpioId(uint8 virt_id)
{
    uint8 i;
    uint8 id_autotest_pm_func = 0xFF;

    for (i = 0; i < NUM_AUTOTST_PM_PINFUNC_T; i++)
    {
        if (autodev_pm_func[i].pin_num == virt_id)
        {
            id_autotest_pm_func = i;
            break;
        }
    }
    return id_autotest_pm_func;
}
#endif

LOCAL uint32 HandleGpioAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
	uint8 value = 0;
	uint8 result = SCI_SUCCESS;

    GPIO_INFO_T gpio_info;

	MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
	DIAG_MSG_GPIO_T  *MSG_HEAD = (DIAG_MSG_GPIO_T *)src_ptr;
#if defined(PLATFORM_UWS6121E)
       AUTOTST_PM_PINFUNC_T *p_pm_pinfuc = NULL;
       if (((AUTOTEST_GPIO_E)(MSG_HEAD->command) == GPIO_GET_VALUE) || ((AUTOTEST_GPIO_E)(MSG_HEAD->command) == GPIO_SET_VALUE))
       {
        	if(MSG_HEAD->gpio_id > PIN_NUM_MAX)
        	{
        		result = SCI_FAILURE;
        		goto Fail;
        	}
              value = AutoTest_GetRealGpioId(MSG_HEAD->gpio_id);
              if (0xFF == value)
              {
        		result = SCI_FAILURE;
        		goto Fail;
              }
              p_pm_pinfuc = &autodev_pm_func[value];
              //need open some power such as  LCD \ CAMERA
              if ((MSG_HEAD->gpio_id >=36) && (MSG_HEAD->gpio_id <=41))      //LCD
              {
                  ANA_REG_OR(ANA_LDO_VOSEL4, 0x1c00);    //lcd 1v8
                  ANA_REG_AND(ANA_POWER_PD_SW1, ~BIT_8);  //lcd open
              }
              if ((MSG_HEAD->gpio_id >=44) && (MSG_HEAD->gpio_id <=47))      //CAMERA
              {
                  ANA_REG_OR(ANA_LDO_VOSEL1, 0x7);    //camera 1v8
                  ANA_REG_AND(ANA_POWER_PD_SW1, ~BIT_11);  //camera open
              }
       }

	switch((AUTOTEST_GPIO_E)(MSG_HEAD->command))
	{
        case GPIO_GET_VALUE:
        {
            if (p_pm_pinfuc->gpio_port == 0xFF)    //this pin is not gpio,can not get value
            {
                  result = SCI_FAILURE;
                  break;
            }
            // Config it to be INPUT.(0)
            GPIO_SetDirection (p_pm_pinfuc->gpio_port, SCI_FALSE);
            // pinmap it to gpio
            if (p_pm_pinfuc->addr != PM_INVALID_VAL)   //addr == PM_INVALID_VAL, only single func=gpio,do not write any addr to map gpio
            {
                CHIP_REG_SET(p_pm_pinfuc->addr, p_pm_pinfuc->pin_sel);
            }
	    //get value from gpio
            value = GPIO_GetValue(p_pm_pinfuc->gpio_port);
            break;
        }
        case GPIO_SET_VALUE:
        {
              if (p_pm_pinfuc->addr != PM_INVALID_VAL)
              {
                  if (p_pm_pinfuc->gpio_port == 0xFF)    //this pin is not gpio,but can force output 1/0
                  {
                        result = SCI_FAILURE;
                  break;
                  }
                  else
                  {
                        // Output only
                        GPIO_SetValue(p_pm_pinfuc->gpio_port, MSG_HEAD->gpio_value); // set value
                        // pinmap it to gpio
                        CHIP_REG_SET(p_pm_pinfuc->addr, p_pm_pinfuc->pin_sel);
                        // Config it to be OUTPUT.(1)
                        GPIO_SetDirection (p_pm_pinfuc->gpio_port, SCI_TRUE);
                  }
              }
              else   //addr == PM_INVALID_VAL, only gpio,do not write any addr to map gpio
              {
                    if (p_pm_pinfuc->gpio_port == 0xFF)    //this is error
                    {
                        SCI_PASSERT(0, ("autotest gpio config set is err\n"));    /*assert verified*/
                    }
                    else
                    {
                        // Output only
                        GPIO_SetValue(p_pm_pinfuc->gpio_port, MSG_HEAD->gpio_value); // set value
                        // Config it to be OUTPUT.(1)
                        GPIO_SetDirection (p_pm_pinfuc->gpio_port, SCI_TRUE);
                    }
              }

            break;
        }
        case GPIO_SETBACK:
            {
                extern PUBLIC void PM_Init (void);
                PM_Init();
            }
		   break;

        default:
            result = SCI_FAILURE;
          break;
	}
#else
       if (((AUTOTEST_GPIO_E)(MSG_HEAD->command) != GPIOEX_KB_GET_VALUE) && ((AUTOTEST_GPIO_E)(MSG_HEAD->command) != GPIOEX_KB_SET_VALUE))
       {
        	if(MSG_HEAD->gpio_id > PIN_NUM_MAX)
        	{
        		result = SCI_FAILURE;
        		goto Fail;
        	}
              //Autodev_PM_Init();
              GPIO_PHY_GetBaseInfo (MSG_HEAD->gpio_id, &gpio_info);
       }

	switch((AUTOTEST_GPIO_E)(MSG_HEAD->command))
	{
	#if 0
        case GPIO_GET_VALUE:
            if (GPIO_PHY_GetDirection (&gpio_info)||(!GPIO_PHY_GetDataMask (&gpio_info)))
            {
                result = SCI_FAILURE;
                break;
            }
            value = GPIO_GetValue(MSG_HEAD->gpio_id);
        	break;

        case GPIO_SET_VALUE:
            if ((!GPIO_PHY_GetDirection (&gpio_info))||(!GPIO_PHY_GetDataMask (&gpio_info)))
            {
                result = SCI_FAILURE;
                break;
            }
            GPIO_SetValue(MSG_HEAD->gpio_id, MSG_HEAD->gpio_value); // set value
        	break;
	#endif
        case GPIO_GET_VALUE:
	        // Check if search to end
        if (autodev_pm_func[MSG_HEAD->gpio_id].addr == PM_INVALID_VAL)
        {
             result = SCI_FAILURE;
            break;
        }

        if (!ANA_IS_ANA_REG (autodev_pm_func[MSG_HEAD->gpio_id].addr))
        {
            CHIP_REG_SET (autodev_pm_func[MSG_HEAD->gpio_id].addr, PIN_I_EN | autodev_pm_func[MSG_HEAD->gpio_id].value);/*lint !e737 -e737*/
        }
        else
        {
            ANA_REG_SET (autodev_pm_func[MSG_HEAD->gpio_id].addr, PIN_I_EN | autodev_pm_func[MSG_HEAD->gpio_id].value);
        }

	      // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (MSG_HEAD->gpio_id,SCI_TRUE);
            // Config it to be INPUT.(0)
            GPIO_SetDirection (MSG_HEAD->gpio_id,SCI_FALSE);

  //          if (GPIO_PHY_GetDirection (&gpio_info)||(!GPIO_PHY_GetDataMask (&gpio_info)))
  //          {
  //              result = SCI_FAILURE;
  //              break;
  //          }
            value = GPIO_GetValue(MSG_HEAD->gpio_id);
        	break;

        case GPIO_SET_VALUE:
		        // Check if search to end
        if (autodev_pm_func[MSG_HEAD->gpio_id].addr == PM_INVALID_VAL)
        {
             result = SCI_FAILURE;
            break;
        }
        if (!ANA_IS_ANA_REG (autodev_pm_func[MSG_HEAD->gpio_id].addr))
        {
            CHIP_REG_SET (autodev_pm_func[MSG_HEAD->gpio_id].addr, PIN_O_EN | autodev_pm_func[MSG_HEAD->gpio_id].value);
        }
        else
        {
            ANA_REG_SET (autodev_pm_func[MSG_HEAD->gpio_id].addr, PIN_O_EN | autodev_pm_func[MSG_HEAD->gpio_id].value);
        }

            // Output only
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (MSG_HEAD->gpio_id,SCI_TRUE);
            // Config it to be OUTPUT.(1)
            GPIO_SetDirection (MSG_HEAD->gpio_id,SCI_TRUE);
            // MASK it.(0)
            GPIO_DisableIntCtl (MSG_HEAD->gpio_id);

//            if ((!GPIO_PHY_GetDirection (&gpio_info))||(!GPIO_PHY_GetDataMask (&gpio_info)))
//            {
//                result = SCI_FAILURE;
//                break;
//            }
            GPIO_SetValue(MSG_HEAD->gpio_id, MSG_HEAD->gpio_value); // set value
        	break;
        case GPIOEX_KB_GET_VALUE:
        {
            result = PM_GPIOKeyPinCtrl(1, MSG_HEAD->gpio_id, &value);
            break;
        }
        case GPIOEX_KB_SET_VALUE:
        {
            result = PM_GPIOKeyPinCtrl(0, MSG_HEAD->gpio_id, &(MSG_HEAD->gpio_value));
            break;
        }
        case GPIO_SETBACK:
            {
                extern PUBLIC void PM_Init (void);
                PM_Init();
            }
		   break;

        default:
            result = SCI_FAILURE;
          break;
	}
#endif
	Fail:
	if(SCI_FAILURE == result)
	{
        msg_head->subtype = SCI_FAILURE;
        ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
	}
	else
	{
        if (((AUTOTEST_GPIO_E)(MSG_HEAD->command) == GPIO_GET_VALUE) || ((AUTOTEST_GPIO_E)(MSG_HEAD->command) == GPIOEX_KB_GET_VALUE))
        {
            *dest_len_ptr = sizeof(MSG_HEAD_T)+1;
        }
        else
        {
            *dest_len_ptr = sizeof(MSG_HEAD_T);
        }
    	*dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
    	SCI_ASSERT(SCI_NULL != *dest_ptr);
        SCI_MEMCPY(*dest_ptr, MSG_HEAD, sizeof(MSG_HEAD_T));
        ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
        ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = result;
        if (((AUTOTEST_GPIO_E)(MSG_HEAD->command) == GPIO_GET_VALUE) || ((AUTOTEST_GPIO_E)(MSG_HEAD->command) == GPIOEX_KB_GET_VALUE))
        {
            SCI_MEMSET(*dest_ptr+sizeof(MSG_HEAD_T),value,1);
        }
    }
    
    return 1;

}

#if !defined(CAMERA_SUPPORT_NONE)
/******************************************************************************/
// Description: set sensor output mode
// Author:
// Input:    mode
// Output:   none
// Return:   none
//
// Note:
/******************************************************************************/
PUBLIC void AutoTest_SetSensorOutPutMode(SENSOR_OUTPUT_MODE_E mode)
{
    s_auto_test_sensor_output_mode = mode;
}

/******************************************************************************/
// Description: get sensor output mode:
// Author:
// Input:    none
// Output:   none
// Return:      s_auto_test_sensor_output_mode
//
// Note:
/******************************************************************************/
PUBLIC  SENSOR_OUTPUT_MODE_E AutoTest_GetSensorOutPutMode(void)    //false: parallel sensor ;true: SPI sensor;
{
    return s_auto_test_sensor_output_mode;
}

/******************************************************************************
// Description:     assign the dc capture  buffer
// Author:
// Input:           none
// Output:          none
// Return:          SCI_SUCCESS
// Note:            none
******************************************************************************/
PUBLIC uint32 AutoTest_SensorMalloc(void)
{
    if(ext_buf_addr == NULL)
    {
        ext_buf_addr=(uint32)SCI_ALLOC(CAM_EXT_BUF_LEN+256);
        if(NULL == ext_buf_addr)
        {
        	SCI_TRACE_LOW("david autotest malloc fail!");
            return SCI_FAILURE;
        }
		SCI_TRACE_LOW("david autotest malloc ok");
        ext_buf_addr_align=DC_RIGHT_TRUNK(ext_buf_addr,256);
    }
    return SCI_SUCCESS;
}

/******************************************************************************
// Description:     Close the sensor
// Author:
// Input:           none
// Output:          none
//
// Return:          DCAMERA_OP_SUCCESS
//
// Note:            none
******************************************************************************/
PUBLIC uint32 AutoTest_SensorClose(void)
{
   uint32 ret=SCI_FAILURE;
   if(NULL!=ext_buf_addr)
    {
        SCI_FREE(ext_buf_addr);  /*lint !e64 */
        ext_buf_addr_align=NULL;
        ext_buf_addr=NULL;
    }
     if(SENSOR_OUTPUT_MODE_CCIR601_8BIT==s_auto_test_sensor_output_mode)
    {
        ret= _Parallel_Sensor_PowerOn(FALSE);
    }
    else if(SENSOR_OUTPUT_MODE_SPI_1BIT==s_auto_test_sensor_output_mode)
    {
        ret=  _Spi_Sensor_PowerOn(FALSE);
    }
    else
    {
        return SCI_FAILURE;
    }
    AutoTest_SetSensorOutPutMode(SENSOR_OUTPUT_MODE_MAX);
    return ret;
}

/******************************************************************************/
// Description: init sensor interface
// Author:
// Input:      none
// Output:     none
// Return:     SCI_SUCCESS
//
// Note:
/******************************************************************************/
PUBLIC  uint32  AutoTest_SensorInit(void)
{
    //DISABLE_AHB_SLEEP

    if(SENSOR_OUTPUT_MODE_CCIR601_8BIT==s_auto_test_sensor_output_mode)
    {
        return _Parallel_Sensor_PowerOn(TRUE);
    }
    else if(SENSOR_OUTPUT_MODE_SPI_2BIT==s_auto_test_sensor_output_mode)
    {
        return  _Spi_Sensor_PowerOn(TRUE);
    }
    else
    {
        return SCI_FAILURE;
    }
}
/******************************************************************************
// Description:     Get the sensor data  after snapshot
// Author:
// Input:
// Output:          cap_img_addr
//                  cap_size
// Return:          SCI_SUCCESS ok
//                  SCI_ERROR fail
// Note:            none
******************************************************************************/

PUBLIC uint32 AutoTest_GetSensorData(DCAMERA_YUV_CHN_ADDR_T* cap_img_addr, DCAMERA_SIZE_T* cap_size_t)
{
    uint32 dcam_ret = SCI_SUCCESS;
    DCAMERA_EXT_GET_DATA_T get_sensor_data = {0x00};

    DCAMERA_EXT_GET_DATA_T_PTR get_data_ptr = &get_sensor_data;
    DCAMERA_YUV_CHN_ADDR_T rtn_image_addr = {0x00};
    DCAMERA_SIZE_T rtn_size = {0x00};

    get_data_ptr->data_format = DCAMERA_DATA_TYPE_YUV422;
    get_data_ptr->is_display = SCI_FALSE;

    get_data_ptr->buf_addr = ext_buf_addr_align;
    get_data_ptr->buf_len = CAM_EXT_BUF_LEN;

    get_data_ptr->img_size.w = 120;//320;//480
    get_data_ptr->img_size.h = 120;//240;//640

    dcam_ret = DCAMERA_ExtGetData(get_data_ptr, &rtn_image_addr, &rtn_size);
    SCI_TRACE_LOW("DCAMERA_ExtGetData %d",dcam_ret);

    if(DCAMERA_OP_SUCCESS!=dcam_ret )
    {
        return SCI_FAILURE;
    }

    *cap_img_addr = rtn_image_addr;
    *cap_size_t = rtn_size;

    return dcam_ret;
}


/******************************************************************************
// Description:     pc sends the cmds to parallel sernsor to get data  and return ack
// Author:
// Input:           *src_ptr
//                  src_len
// Output:          *dest_ptr
//                  *dest_len_ptr
// Return:          1
//
// Note:            none
******************************************************************************/
#if 0
LOCAL uint32 HandleParallel_CameraAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    uint8  cmmand = 0;
    uint32 dcam_ret=SCI_SUCCESS;
    wchar photo_path_ptr[256]={0};
    uint16 photo_len;
    uint32 sfs_handle;

	DCAMERA_PREVIEW_PARAM_T param;
	uint32 ret = 0;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    cmmand = *(uint8 *)(msg_head + 1);
	switch(cmmand)
	{

	   case 0x01:

           dcam_ret= DCAMERA_Open(DCAMERA_OP_NORMAL);
		if(SCI_SUCCESS == dcam_ret)
		{
                msg_head->subtype = ATCMD_SUCCESS;
		}
		else
		{
		  msg_head->subtype = ATCMD_BADCOMMAND;
		}
	        ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
	       break;

	    case 0x02:
            AutoTest_SensorMalloc();
            dcam_ret=AutoTest_GetSensorData(&cap_image_addr,&cap_size);

	     if(SCI_SUCCESS == dcam_ret)
            {
                *dest_len_ptr = sizeof(MSG_HEAD_T)+ CAM_SEND_DATA_LENGTH*2;
                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);

                SCI_ASSERT(SCI_NULL != *dest_ptr);
                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

                SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T), (uint8 *)cap_image_addr.y_chn_addr, CAM_SEND_DATA_LENGTH);
                SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T)+CAM_SEND_DATA_LENGTH, (uint8 *)cap_image_addr.u_chn_addr, CAM_SEND_DATA_LENGTH);

                ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
            }
		else
            {
                *dest_len_ptr = sizeof(MSG_HEAD_T);
                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                SCI_ASSERT(SCI_NULL != *dest_ptr);

                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

                ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADCOMMAND;
            }
	        break;

	    case 0x03:
            if(NULL!=ext_buf_addr)
            {
                SCI_FREE(ext_buf_addr);  /*lint !e64 */
                ext_buf_addr_align=NULL;
                ext_buf_addr=NULL;
            }
            dcam_ret=DCAMERA_Close();
			hal_GoudaOvlLayerActive(0);
            if(SCI_SUCCESS == dcam_ret)
		{
               msg_head->subtype = ATCMD_SUCCESS;
		}
		else
		{
		  msg_head->subtype = ATCMD_BADCOMMAND;
		}
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
	       break;
            case 0x4:
#ifdef CAMERA_SUPPORT
                photo_len =MMIDC_GetphotoFullPath(photo_path_ptr);//dc mode
#endif
                sfs_handle = SFS_CreateFile(photo_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
                if((0 != photo_len)&&(NULL != sfs_handle))
                {
                     if(SFS_ERROR_NONE == SFS_SetFilePointer(sfs_handle, 0, SFS_SEEK_BEGIN))
                     {
                           uint32  fileSize = 0;
                           uint32  read_size = 0;

                           SFS_GetFileSize(sfs_handle, &fileSize);
                           SCI_TRACE_LOW("the file size=%d",fileSize);
                           if(fileSize>100*1024)
                           {
                                *dest_len_ptr = sizeof(MSG_HEAD_T);
                                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                                SCI_ASSERT(SCI_NULL != *dest_ptr);
                                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                                ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADLENGTH;
                                SFS_CloseFile(sfs_handle);
                                break;
                           }
                           *dest_len_ptr = sizeof(MSG_HEAD_T)+fileSize;
                           *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                           SCI_ASSERT(SCI_NULL != *dest_ptr);
                           SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                           ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                           if(SFS_ERROR_NONE == SFS_ReadFile(sfs_handle, *dest_ptr+sizeof(MSG_HEAD_T), fileSize, &read_size, NULL))
                           {
                               ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = SCI_SUCCESS;
                           }
                           else
                           {

                               ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADPARAMETER;
                           }
                            SCI_TRACE_LOW("the file size=%d,read_size=%d",fileSize,read_size);
                            SFS_CloseFile(sfs_handle);
                            break;
                     }

                }
                if(sfs_handle)
				{
                    SFS_CloseFile(sfs_handle);
                }
                *dest_len_ptr = sizeof(MSG_HEAD_T);
                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                SCI_ASSERT(SCI_NULL != *dest_ptr);
                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADPARAMETER;
                break;

			//Start Preview For uix8910
			case 0x05:

				memset(&param, 0, sizeof(DCAMERA_PREVIEW_PARAM_T));
				param.lcd_id = 0;
				param.lcd_block_id = 1;
				param.disp_rect.x = 0;
				param.disp_rect.y = 0;
				param.disp_rect.w = 240;
				param.disp_rect.h = 320;
				param.target_rect.x = 0;
				param.target_rect.y = 0;
				param.target_rect.w = 240;
				param.target_rect.h = 320;
				param.rotation_mode = DCAMERA_ROTATION_90;
				param.disp_mode   = DCAMERA_DISP_FULLSCREEN;
				param.capture_mode = DCAMERA_CAPTURE_NORMAL;
				param.panorama_dir = DC_PANORAMA_HORIZONTAL_LEFT;
				param.dc_atv_callback = NULL;

				ret = DCAMERA_StartPreview(&param);
				if(0 == ret)
				{
					hal_GoudaOvlLayerNoActive(0);
					msg_head->subtype = ATCMD_SUCCESS;
				}
				else
				{
					msg_head->subtype = ATCMD_BADCOMMAND;
				}
				ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);

				break;

			//Stop Preview For uix8910
			case 0x06:

			hal_GoudaOvlLayerActive(0);
			ret = DCAMERA_StopPreview();
			if(0 == ret)
			{
				msg_head->subtype = ATCMD_SUCCESS;
			}
			else
			{
				msg_head->subtype = ATCMD_BADCOMMAND;
			}
			ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);

			break;

    	    default:
             msg_head->subtype = ATCMD_BADCOMMAND;
             ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
	         break;

	}
	return 1;
}
#endif
LOCAL uint32 HandleParallel_CameraAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
	uint8 cmmand = 0;
	MSG_HEAD_T *msg_head = (MSG_HEAD_T *)src_ptr;

	uint32 ldo_camret = SCI_SUCCESS;
	cmmand = *(uint8 *)(msg_head + 1);
	SCI_TRACE_LOW("HandleParallel_CameraAutotest: LDO CAMA cmmand = 0x%x",cmmand);
	switch (cmmand) {
	case 0x01:
		ldo_camret = LDO_SetVoltValue (LDO_LDO_CAMD, 1800);
		RTN_IF_ERR(ldo_camret);
		ldo_camret = LDO_TurnOnLDO (LDO_LDO_CAMD);
		RTN_IF_ERR(ldo_camret);
		ldo_camret = LDO_SetVoltValue (LDO_LDO_CAMA, 2800);
		RTN_IF_ERR(ldo_camret);
		ldo_camret = LDO_TurnOnLDO (LDO_LDO_CAMA);
		SCI_TRACE_LOW("HandleParallel_CameraAutotest: LDO CAMA Open!ret = %d",ldo_camret);
		break;
	case 0x03:
		ldo_camret = LDO_TurnOffLDO (LDO_LDO_CAMA);
		RTN_IF_ERR(ldo_camret);
		ldo_camret = LDO_TurnOffLDO (LDO_LDO_CAMD);
		SCI_TRACE_LOW("HandleParallel_CameraAutotest: LDO CAMA Close!ret = %d",ldo_camret);
		break;
	default:
		ldo_camret= ATCMD_BADCOMMAND;
		break;
	}
	exit:
		if (SCI_SUCCESS == ldo_camret)
			msg_head->subtype = ATCMD_SUCCESS;
		else
			msg_head->subtype = ATCMD_BADCOMMAND;
		ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
	return 1;
}



/******************************************************************************
// Description:      pc sends the cmds to spi sernsor to get data  and return ack
// Author:
// Input:           *src_ptr
//                  src_len
// Output:          *dest_ptr
//                  *dest_len_ptr
// Return:          SCI_SUCCESS ok
//                   1
// Note:            none
******************************************************************************/
LOCAL uint32 HandleSPI_CameraAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    uint8  cmmand = 0;
    uint32 dcam_ret=SCI_SUCCESS;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    cmmand = *(uint8 *)(msg_head + 1);

    switch(cmmand)
        {
    		 case 0x01:
                AutoTest_SetSensorOutPutMode(SENSOR_OUTPUT_MODE_SPI_2BIT);
    			dcam_ret=AutoTest_SensorInit();

    			if(SCI_ERROR!=dcam_ret)
    			{
    	          msg_head->subtype = ATCMD_SUCCESS;
    			}
    			else
    			{
    			  msg_head->subtype = ATCMD_BADCOMMAND;
    			}
    	        ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
    	       break;

    	    case 0x02:
                AutoTest_SensorMalloc();
                dcam_ret=AutoTest_GetSensorData(&cap_image_addr,&cap_size);

    		    if(SCI_SUCCESS == dcam_ret)
                {
                    *dest_len_ptr = sizeof(MSG_HEAD_T)+ CAM_SEND_DATA_LENGTH*2;
                    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);

                    SCI_ASSERT(SCI_NULL != *dest_ptr);
                    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

                    SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T), (uint8 *)cap_image_addr.y_chn_addr, CAM_SEND_DATA_LENGTH);
                    SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T)+CAM_SEND_DATA_LENGTH, (uint8 *)cap_image_addr.u_chn_addr, CAM_SEND_DATA_LENGTH);

                    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                    ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
                }
    			else
                {
                    *dest_len_ptr = sizeof(MSG_HEAD_T);
                    *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                    SCI_ASSERT(SCI_NULL != *dest_ptr);

                    SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));

                    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                    ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADCOMMAND;
                }
	        break;

	    case 0x03:
            if(NULL!=ext_buf_addr)
            {
                SCI_FREE(ext_buf_addr);  /*lint !e64 */
                ext_buf_addr_align=NULL;
                ext_buf_addr=NULL;
            }

            dcam_ret=DCAMERA_Close();
            if(SCI_SUCCESS == dcam_ret)
			{
            	msg_head->subtype = ATCMD_SUCCESS;
			}
			else
			{
		  		msg_head->subtype = ATCMD_BADCOMMAND;
			}
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);

	       break;

    	   default:
                msg_head->subtype = ATCMD_BADCOMMAND;
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                break;
        }
   return 1;
}
#endif

#ifdef VIDEO_RECORD_SUPPORT
LOCAL uint32 HandleVIDEO_Autotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    uint16 list_item_index=0;
    wchar videofullpath[256] = {0};
    uint16 lenFileName = 0;
    uint32 sfs_handle;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    DIAG_MSG_VIDEO_T *MSG_HEAD = (DIAG_MSG_VIDEO_T *)src_ptr;
    switch(MSG_HEAD->cmmand)
    {
        case 0x01:   //reserved for open vedio
           break;
        case 0x02:// get vedio
            //get video data
#ifdef CAMERA_SUPPORT
           lenFileName = MMIDC_GetVedioFullPath(videofullpath);
#endif
          SCI_TRACE_LOW("savepath,thepathlength=%d",lenFileName);

           sfs_handle = SFS_CreateFile(videofullpath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
           if((0 == lenFileName)&&(0 != sfs_handle))
           {
               if(SFS_ERROR_NONE == SFS_SetFilePointer(sfs_handle, 0, SFS_SEEK_BEGIN))
               {
                   uint32  fileSize = 0;
                   uint32  read_size = 0;

                   SFS_GetFileSize(sfs_handle, &fileSize);
                   SCI_TRACE_LOW("the file size=%d",fileSize);
                   if(fileSize>VIDEO_BUF_LEN)
                   {
                        *dest_len_ptr = sizeof(MSG_HEAD_T);
                        *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                        SCI_ASSERT(SCI_NULL != *dest_ptr);
                        SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                        ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                        ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADLENGTH;
                        SFS_CloseFile(sfs_handle);
                        break;
                   }
                   *dest_len_ptr = sizeof(MSG_HEAD_T)+fileSize;
                   *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                   SCI_ASSERT(SCI_NULL != *dest_ptr);
                   SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                    ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                   if(SFS_ERROR_NONE == SFS_ReadFile(sfs_handle, *dest_ptr+sizeof(MSG_HEAD_T), fileSize, &read_size, NULL))
                   {
                       ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = SCI_SUCCESS;
                   }
                   else
                   {

                       ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADPARAMETER;
                   }
                    SCI_TRACE_LOW("the file size=%d,read_size=%d",fileSize,read_size);
                    SFS_CloseFile(sfs_handle);
                    break;
               }
           }
           if(sfs_handle){
            SFS_CloseFile(sfs_handle);
            }
           *dest_len_ptr = sizeof(MSG_HEAD_T);
           *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
           SCI_ASSERT(SCI_NULL != *dest_ptr);
           SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
           ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
           ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADPARAMETER;
            break;

    }

	return 1;

}
#endif

#if !defined(ATV_SUPPORT_NONE)
PUBLIC uint32 AutoTest_ATVMalloc(void)
{
    if(ext_buf_addr == NULL)
    {
        ext_buf_addr=(uint32)SCI_ALLOC(ATV_EXT_BUF_LEN+256);
        if(NULL == ext_buf_addr)
        {
            return SCI_FAILURE;
        }
        ext_buf_addr_align=DC_RIGHT_TRUNK(ext_buf_addr,256);
    }
    return SCI_SUCCESS;
}

PUBLIC  uint32  AutoTest_AtvInit(DCAMERA_OP_MODE_E op_mode)
{
    //DISABLE_AHB_SLEEP
    if(DCAMERA_OP_SUCCESS == DCAMERA_Open(op_mode))
    return SCI_SUCCESS;
    else
    return SCI_FAILURE;
}

PUBLIC uint32 AutoTest_GetATVData(DCAMERA_YUV_CHN_ADDR_T* cap_img_addr,DCAMERA_SIZE_T* cap_size_t)
{
    uint32 ret = SCI_SUCCESS;

    DCAMERA_EXT_GET_DATA_T get_sensor_data = {0x00};
    DCAMERA_EXT_GET_DATA_T_PTR get_data_ptr = &get_sensor_data;
    DCAMERA_YUV_CHN_ADDR_T rtn_image_addr = {0x00};
    DCAMERA_SIZE_T rtn_size = {0x00};

    get_data_ptr->data_format = DCAMERA_DATA_TYPE_YUV422;
    get_data_ptr->is_display = SCI_FALSE;

    get_data_ptr->buf_addr = ext_buf_addr_align;
    get_data_ptr->buf_len = ATV_EXT_BUF_LEN;

    get_data_ptr->img_size.w = 120;  //as default!
    get_data_ptr->img_size.h = 160;


    ret = DCAMERA_ExtGetData(get_data_ptr, &rtn_image_addr, &rtn_size);

    SCI_TRACE_LOW("AutoTest_GetATVData %d",ret);

    if(DCAMERA_OP_SUCCESS!=ret)
    {
        return SCI_FAILURE;
    }

    *cap_img_addr = rtn_image_addr;
    *cap_size_t = rtn_size;

    return ret;
}


PUBLIC  uint32 AutoTest_CloseATV(void)
{
    if(NULL!=ext_buf_addr)
    {
        SCI_FREE(ext_buf_addr); /*lint !e64 */
        ext_buf_addr_align=NULL;
        ext_buf_addr=NULL;
    }
    DCAMERA_Close();
    AutoTest_SetSensorOutPutMode(SENSOR_OUTPUT_MODE_MAX);
    return DCAMERA_OP_SUCCESS;
}
/******************************************************************************
// Description:     pc sends the cmds to atv to get the data  and return ack
// Author:
// Input:           *src_ptr
//                  src_len
// Output:          *dest_ptr
//                  *dest_len_ptr
// Return:         1
//
// Note:            none
******************************************************************************/
PUBLIC uint32 HandleATV_Autotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )

{
    uint32 atv_ret = SCI_SUCCESS;
	uint16 atv_channel = 0;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    DIAG_MSG_ATV_T *MSG_HEAD = (DIAG_MSG_ATV_T *)src_ptr;

    switch(MSG_HEAD->cmmand)
    {
        case 0x01:
            atv_channel = MSG_HEAD->atv_channel;
            if((SCI_SUCCESS == AutoTest_AtvInit(DCAMERA_OP_ATV))
               &&(DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_ATV_REGION, ATV_REGION_CHINA, PNULL))
               &&(DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_ATV_CHN, atv_channel, PNULL)) //nmiapi.c,get the atv_channel number
               /*&&(DCAMERA_OP_SUCCESS == DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_ATV_VOLUME,10,PNULL))*/
               )
                {
                   // AUDIO_HAL_Open(AUDIO_FUN_LINEIN|AUDIO_FUN_PLAYBACK,8000);
                   // ARMVB_SetPgaLevel(0,AUDIO_ALL_CHAN,0x008C,AUDIO_LINEIN | AUDIO_HP);
                   // ARMVB_DrvOutSwitch(0,AUDIO_HP, SCI_TRUE);
                    atv_ret = SCI_SUCCESS;
                }
                else
                {
                    atv_ret = SCI_FAILURE;
                }

            *dest_len_ptr = sizeof(MSG_HEAD_T);
            *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
            SCI_ASSERT(SCI_NULL != *dest_ptr);
            SCI_MEMCPY(*dest_ptr, MSG_HEAD, sizeof(MSG_HEAD_T));
            ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
            ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = atv_ret;
            break;

        case 0x02:
            AutoTest_ATVMalloc();
            atv_ret = AutoTest_GetATVData(&cap_image_addr,&cap_size);

		    if(SCI_SUCCESS == atv_ret)
            {
                *dest_len_ptr = sizeof(MSG_HEAD_T)+ ATV_SEND_DATA_LENGTH;
                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                SCI_ASSERT(SCI_NULL != *dest_ptr);
                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T), (uint8 *)cap_image_addr.y_chn_addr, ATV_SEND_DATA_LENGTH);
                ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
            }
			else
            {
                *dest_len_ptr = sizeof(MSG_HEAD_T);
                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                SCI_ASSERT(SCI_NULL != *dest_ptr);
                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
                ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADCOMMAND;
            }
            break;

        case 0x03:
            AutoTest_CloseATV();

            *dest_len_ptr = sizeof(MSG_HEAD_T);
            *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
            SCI_ASSERT(SCI_NULL != *dest_ptr);

            SCI_MEMCPY(*dest_ptr, MSG_HEAD, sizeof(MSG_HEAD_T));

            ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
            ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_SUCCESS;
            break;

        default:
            *dest_len_ptr = sizeof(MSG_HEAD_T);
            *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
            SCI_ASSERT(SCI_NULL != *dest_ptr);

            SCI_MEMCPY(*dest_ptr, MSG_HEAD, sizeof(MSG_HEAD_T));

            ((MSG_HEAD_T  *)(*dest_ptr))->len = *dest_len_ptr;
            ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = ATCMD_BADCOMMAND;
            break;
	}

	return 1;
}
#endif

#if !defined(WIFI_SUPPORT_NONE)
LOCAL uint32 HandleWifiAutotest(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
)
{
    uint32 i = 0, n = 0;
    uint8  command = 0;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    struct wifi_scan_res *res = PNULL;
    struct wifi_bss_info *bss = PNULL;
	WIFISCAN_REQ scan_req = {0};
    char bssidrssi[10] = {0};

    command = *(uint8 *)(msg_head + 1);
    SCI_TRACE_LOW("HandleWifiAutotest command 0x%x",command);
    switch(command)
    {
        case WIFI_OPEN:
        {
			Is_WifiScanStart = SCI_FALSE;
            wifi_sta_test_init();
            wifi_sta_open();

            //SCI_SLEEP(500);
            msg_head->subtype = SCI_SUCCESS;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            break;
        }

        case WIFI_SCAN:
        {
			//TBD
			scan_req.wswitch = 1;
			scan_req.nChNum = 13;
			scan_req.nMaxNodeNum = 20;
			scan_req.nCh[0] = 1;
			scan_req.nCh[1] = 2;
			scan_req.nCh[2] = 3;
			scan_req.nCh[3] = 4;
			scan_req.nCh[4] = 5;
			scan_req.nCh[5] = 6;
			scan_req.nCh[6] = 7;
			scan_req.nCh[7] = 8;
			scan_req.nCh[8] = 9;
			scan_req.nCh[9] = 10;
			scan_req.nCh[10] = 11;
			scan_req.nCh[11] = 12;
			scan_req.nCh[12] = 13;
			//TBD

            wifi_sta_scan_start(scan_req);
			Is_WifiScanStart = SCI_TRUE;

            SCI_SLEEP(500);

            msg_head->subtype = SCI_SUCCESS;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            break;
        }

        case WIFI_READ:
        {
            if (SCI_FALSE == Is_WifiScanStart)
			{
                msg_head->subtype = SCI_ERROR;
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                break;
            }

            res = wifi_sta_scan_results();
			*dest_len_ptr = sizeof(MSG_HEAD_T) + (res->nr_bss)*WIFI_MAC_ADDR_LEN;
			//*dest_len_ptr = sizeof(MSG_HEAD_T) + (res->nr_bss)*sizeof(WIFISUPP_BSSID_T);
			SCI_TRACE_LOW("WIFIAutotest WIFI_READ len %d.", *dest_len_ptr);

			*dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
			SCI_ASSERT(SCI_NULL != *dest_ptr);
			msg_head->len = *dest_len_ptr;
			if (res->nr_bss > 0)
			{
				SCI_TRACE_LOW("WIFIAutotest WIFI_READ SUCCESS.");
				msg_head->subtype = SCI_SUCCESS;
			}
			else
			{
				SCI_TRACE_LOW("WIFIAutotest WIFI_READ FAIL.");
				msg_head->subtype = SCI_FAILURE;
			}
			SCI_MEMCPY((void *)(*dest_ptr), (void *)msg_head, sizeof(MSG_HEAD_T));
			for (i = 0; i < res->nr_bss; i++)
			{
				bss = &res->bsses[i];
				SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T) + (i)*WIFI_MAC_ADDR_LEN, bss->bssid, WIFI_MAC_ADDR_LEN);
			}

            break;
        }

        case WIFI_CLOSE:
        {
			wifi_sta_scan_stop();

            wifi_sta_close();

            Is_WifiScanStart = SCI_FALSE;
            msg_head->subtype = SCI_SUCCESS;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            break;
        }

        case WIFI_RSSI:
        {
            if (SCI_FALSE == Is_WifiScanStart)
			{
                msg_head->subtype = SCI_ERROR;
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
                break;
            }

			res = wifi_sta_scan_results();

			*dest_len_ptr = sizeof(MSG_HEAD_T) + (res->nr_bss)*10;
			SCI_TRACE_LOW("WIFIAutotest WIFI_RSSI len %d.", *dest_len_ptr);

			*dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
			SCI_ASSERT(SCI_NULL != *dest_ptr);
			msg_head->len = *dest_len_ptr;
			if (res->nr_bss > 0)
			{
				SCI_TRACE_LOW("WIFIAutotest WIFI_RSSI SUCCESS.");
				msg_head->subtype = SCI_SUCCESS;
			}
			else
			{
				SCI_TRACE_LOW("WIFIAutotest WIFI_RSSI FAIL.");
				msg_head->subtype = SCI_FAILURE;
			}
			SCI_MEMCPY((void *)(*dest_ptr), (void *)msg_head, sizeof(MSG_HEAD_T));
			for (i = 0; i < res->nr_bss; i++)
			{
				bss = &res->bsses[i];
				SCI_MEMSET(bssidrssi, 0 , 10);
				SCI_MEMCPY(bssidrssi,bss->bssid,WIFI_MAC_ADDR_LEN);
				bssidrssi[6] = bss->rssi;
				SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T) + (i)*10, bssidrssi,10);
			}
            break;
        }

        default :
            msg_head->subtype = SCI_SUCCESS;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            break;
    }

    return 1;
}
#endif

LOCAL uint32 Handle_Autotest_Check(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
	MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;
    msg_head->subtype = ATCMD_SUCCESS;
    ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
	return 1;
}

/********************************************************************************
NAME:          ReadUITestResultNV
DESCRIPTION:
PARAM IN:
PARAM OUT:
AUTHOR:
DATE:
********************************************************************************/
LOCAL int32 AutotestReadNV( uint8* buffer)
{
    uint8 nvitem[64] = {0};
    uint32 nv_length = 64;
    uint32 return_code = SCI_SUCCESS ;

    return_code = PROD_ReadEngModeTestResult((void*)nvitem, nv_length);

    //SCI_TRACE_LOW:"MMIENG_UITESTWIN ReadUITestResultNV  return_code = %d"
    if(return_code == SCI_SUCCESS)
    {
        SCI_MEMCPY(buffer,nvitem+sizeof(nvitem)-AUTOTEST_NVRAM_LEN,AUTOTEST_NVRAM_LEN);  //default the last 8 items of engine_test_result  is use.......
    }

    return return_code;
}

/********************************************************************************
NAME:          WriteUITestResultNV
DESCRIPTION:
PARAM IN:
PARAM OUT:
AUTHOR:
DATE:
********************************************************************************/
LOCAL int32 AutotestWriteNV(uint8* buffer)
{
    uint8 nvitem[64] ={ 0};
    uint32 nv_length = 64;
    uint32 return_code = SCI_SUCCESS ;

    return_code = PROD_ReadEngModeTestResult((void*)nvitem, nv_length);
    //SCI_TRACE_LOW:"MMIENG_UITESTWIN WriteUITestResultNV return_code =  %d"
    if(SCI_SUCCESS == return_code )
    {
        SCI_MEMCPY((uint8*)(nvitem+64-AUTOTEST_NVRAM_LEN), buffer, AUTOTEST_NVRAM_LEN);
        return_code = PROD_WriteEngModeTestResult((void*)nvitem, nv_length);
    }
    return return_code;
}

LOCAL uint32 Autotest_Result_Read(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
 {
    uint8 result = 0;
    MSG_HEAD_T *msg_head = (MSG_HEAD_T *)src_ptr;
    uint8 read_data[AUTOTEST_NVRAM_LEN+1];
    result = AutotestReadNV(read_data);
    if(SCI_SUCCESS == result)
    {
        *dest_len_ptr = sizeof(MSG_HEAD_T) + AUTOTEST_NVRAM_LEN;
        *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
        SCI_ASSERT(SCI_NULL != *dest_ptr);
        msg_head->len = *dest_len_ptr;
        msg_head->subtype = SCI_SUCCESS;
        SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T));
        SCI_MEMCPY(*dest_ptr + sizeof(MSG_HEAD_T), read_data, AUTOTEST_NVRAM_LEN);
     }
     else
     {
        msg_head->subtype = SCI_FAILURE;
        ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
     }
 	 return 1;
 }

LOCAL uint32 Autotest_Result_Write(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
 {
    uint8 result = SCI_SUCCESS;
    uint8 read_data[AUTOTEST_NVRAM_LEN+1]={0};
    MSG_HEAD_T *msg_head = (MSG_HEAD_T *)src_ptr;
    SCI_MEMCPY(read_data, (uint8*)(src_ptr+sizeof(MSG_HEAD_T)), sizeof(DIAG_MSG_NVRAM_T)-sizeof(MSG_HEAD_T));
    result = AutotestWriteNV((uint8*)read_data);

    if(SCI_SUCCESS != result)
    {
        result = SCI_FAILURE;
    }
    msg_head->subtype = result;
    ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
 	return 1;
 }

/*****************************************************************************/
//  Description:    this function is used to init the autotest_device's callback func array.
//
//	Global resource dependence:
//  Author:         Jackey.Ling
//  Note:
/*****************************************************************************/

PUBLIC void DIAG_InitAutotestCallback( void )
{
    int  type = 0;

    for( type = 0; type < DEVICE_AUTOTEST_MAX_F; type++ )
    {
        s_diag_autotest_callback[ type ] = NULL;
    }
}

/*****************************************************************************/
//  Description:    this function is used to register the autotest_device's callback func.
//
//	Global resource dependence:
//  Author:         Jackey.Ling
//  Note:
/*****************************************************************************/

PUBLIC void DIAG_RegisterAutotestCallback( uint32  type,  DIAG_AUTOTEST_CALLBACK  routine )
{
    if( type < DEVICE_AUTOTEST_MAX_F )
        s_diag_autotest_callback[ type ] = routine;
}

/*****************************************************************************/
//  Description:    this function is used to unregister the autotest_device's callback func.
//
//	Global resource dependence:
//  Author:         Jackey.Ling
//  Note:
/*****************************************************************************/

PUBLIC void DIAG_UnRegisterAutotestCallback( uint32  type )
{
    if( type < DEVICE_AUTOTEST_MAX_F )
        s_diag_autotest_callback[ type ] = NULL;
}


/*****************************************************************************/
//  Description:    this function is used to handle the autotest_device's callback func.
//
//	Global resource dependence:
//  Author:         Jackey.Ling
//  Note:
/*****************************************************************************/

LOCAL uint32 HandleDeviceAutotest( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    uint8        cmd_subtype;
    MSG_HEAD_T  *msg_head;

    msg_head    = (MSG_HEAD_T *) src_ptr;
    cmd_subtype = msg_head->subtype;

    if( cmd_subtype < DEVICE_AUTOTEST_MAX_F )
    {
        if( NULL != s_diag_autotest_callback[cmd_subtype] )
        {
            if( s_diag_autotest_callback[cmd_subtype]( dest_ptr, dest_len_ptr, src_ptr, src_len ) )// has been handled.
            {
                return  1;
            }
        }
    }
    return 0;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
