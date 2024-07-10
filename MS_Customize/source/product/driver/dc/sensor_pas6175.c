/******************************************************************************
 ** Copyright (c) 
 ** File Name:		sensor_PAS6175.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor PAS6175. 
 ** 														 
 ******************************************************************************

 ******************************************************************************
 ** 					   Edit History 									  *
 ** ------------------------------------------------------------------------- *
 ** DATE		   NAME 			DESCRIPTION 							  *
 ** 	  
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 ** 						Dependencies									  *
 **---------------------------------------------------------------------------*/
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "gpio_prod_api.h"

/**---------------------------------------------------------------------------*
 ** 						Compiler Flag									  *
 **---------------------------------------------------------------------------*/
#ifdef	 __cplusplus
	extern	 "C" 
	{
#endif
/**---------------------------------------------------------------------------*
 ** 					Extern Function Declaration 						  *
 **---------------------------------------------------------------------------*/
extern uint32 OS_TickDelay(uint32 ticks);

/**---------------------------------------------------------------------------*
 ** 						Const variables 								  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 						   Macro Define
 **---------------------------------------------------------------------------*/
#define PAS6175_I2C_ADDR_W		0x80
#define PAS6175_I2C_ADDR_R		0x81

#define SENSOR_GAIN_SCALE		16

 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_PAS6175_ae_awb_enable(uint32 ae_enable, uint32 awb_enable);
LOCAL uint32 set_PAS6175_ae_enable(uint32 enable);
PUBLIC uint32 PAS6175_hmirror_enable(uint32 enable);   //
PUBLIC uint32 PAS6175_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 PAS6175_Identify(uint32 param);
LOCAL uint32 PAS6175_BeforeSnapshot(uint32 param);
LOCAL uint32 PAS6175_After_Snapshot(uint32 param);

LOCAL uint32 set_brightness(uint32 level);
LOCAL uint32 set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
LOCAL uint32 set_image_effect(uint32 effect_type);

LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);
LOCAL uint32 PAS6175_set_work_mode(uint32 mode);

LOCAL uint32 set_PAS6175_ev(uint32 level);
LOCAL uint32 set_PAS6175_awb(uint32 mode);
LOCAL uint32 set_PAS6175_anti_flicker(uint32 mode);
LOCAL uint32 set_PAS6175_video_mode(uint32 mode);
LOCAL uint32 _PAS6175_Power_On(uint32 power_on);


LOCAL uint32 s_preview_mode;

LOCAL uint8 Lpf_LSB = 0;
LOCAL uint8 Lpf_MSB = 0;
LOCAL uint8 Ny_LSB	= 0;
LOCAL uint8 Ny_MSB	= 0;

/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/

/*lint -save -e533 */
__align(4) const SENSOR_REG_T PAS6175_YUV_240X320[]=
{
	//PAS6175_MTK_SOP_HSIN_20100811_NORMAL.set
	{0xEF,0x00},
	{0x04,0x00},
	{0x07,0xC8},
	{0x08,0x50},
	{0x09,0x00},
	{0x0A,0xFF},
	{0x0C,0x00},
	{0x0F,0x3A},
	{0x11,0x4E},
	{0x13,0x30}, //min_lpf=560 for 20 fps
	{0x14,0x22},
	{0x15,0x3A},
	{0x17,0x42},
	{0x19,0x3F},
	{0x1A,0x80},
	{0x1B,0x3F},
	{0x1C,0x80},
	{0x1F,0x50},
	{0x20,0x28},
	{0x21,0x02},
	{0x22,0x78},
	{0x23,0x30},//э到堵翴, 20101112, Angelo
	{0x24,0x02},
	{0x25,0x8C},
	{0x26,0x00},
	{0x27,0x00},
	{0x29,0x01},
	{0x2A,0x11},
	{0x2B,0x20},
	{0x2C,0x3A},
	{0x2D,0x64},
	{0x2E,0x75},
	{0x2F,0x84},
	{0x30,0x92},
	{0x31,0x9F},
	{0x32,0xB5},
	{0x33,0xC7},
	{0x34,0xD5},
	{0x35,0xE0},
	{0x36,0xEF},
	{0x37,0xF8},
	{0x38,0xFD},
	{0x39,0x03},
	{0x45,0x00},
	{0x46,0x00},
	{0x47,0x30},
	{0x48,0x16},
	{0x49,0x10},
	{0x4A,0x17},
	{0x4B,0x7E},
	{0x4C,0x80},
	{0x4D,0xF3},
	{0x4E,0xDC},
	{0x4F,0xF7},
	{0x50,0x1E},
	{0x51,0x14},
	{0x52,0x09},
	{0x53,0x46},
	{0x54,0xFF},
	{0x55,0x28},
	{0x56,0x19},
	{0x57,0x01},
	{0x58,0x06},
	{0x59,0x04},
	{0x5A,0x0A},
	{0x5B,0x02},
	{0x63,0x30},
	{0x64,0x0A},
	{0x65,0x30},
	{0x66,0x03},
	{0x67,0x12}, // AE freq for 8Mhz
	{0x68,0x7A}, // AE freq for 8Mhz
	{0x6B,0x07},  // AE min stage
	{0x6C,0x14},
	{0x6D,0x28},
	{0x6F,0x9B},
	{0x70,0x00},
	{0x72,0x01},
	{0x73,0x02},
	{0x74,0x40},
	{0x75,0x02},
	{0x76,0x07},
	{0x78,0x01},
	{0x79,0x0C},
	{0x7B,0x03},
	{0x7D,0x00},
	{0x7E,0x00},
	{0x7F,0xFF},
	{0x80,0xFE},
	{0x81,0x12},
	{0x82,0x6F},
	{0x84,0x2C},
	{0x86,0x14},
	{0x87,0x44},
	{0x88,0x01},
	{0x89,0x00},
	{0x8A,0x00},
	{0x8B,0x00},
	{0x8C,0xE0},
	{0x8E,0x00},
	{0x8F,0x00},
	{0x90,0x00},
	{0x91,0x00},
	{0x92,0x00},
	{0x93,0x00},
	{0x94,0x00},
	{0x95,0x00},//Sensor CLK Delay
	{0x97,0x10},
	{0x99,0x44},
	{0x9A,0x40},
	{0x9B,0x44},
	{0x9C,0x40},
	{0x9D,0x46},
	{0x9E,0x40},
	{0x9F,0x46},
	{0xA0,0x40},
	{0xA1,0x60},
	{0xA2,0x50},
	{0xA3,0x5E},
	{0xA4,0x50},
	{0xA5,0x46},
	{0xA6,0x50},
	{0xAA,0x10},
	{0xE0,0xF4},//244
	{0xE1,0x00},
	{0xE2,0x44},//324
	{0xE3,0x01},
	{0xE6,0x00},
	{0xE9,0x87},
	{0xEA,0x00},
	{0xEB,0x00},
	{0xED,0x00},
	{0xEE,0x3F},
	{0xED,0x01},
	    
	{0xEF,0x01},
	{0x02,0x03}, //24/4=8 Mhz
	{0x03,0x64}, // Novsize=100
	{0x04,0x54},
	{0x05,0x01},
	{0x06,0x0F},
	{0x07,0x11},
	{0x08,0x0F},
	{0x09,0x15},
	{0x0C,0x00},
	{0x0D,0x27},
	{0x0E,0x0F},
	{0x0F,0x00},
	{0x10,0x12},
	{0x11,0x00},
	{0x12,0x20},
	{0x17,0x00},
	{0x18,0x00},
	{0x19,0x00},
	{0x1A,0x00},
	{0x1B,0xFF},
	{0x1C,0x00},
	{0x1D,0x4F},
	{0x1E,0x01},
	{0x20,0x14},
	{0x21,0x00},
	{0x22,0x08},
	{0x23,0x00},
	{0x24,0x05},
	{0x28,0x08},
	{0x2A,0x03},
	{0x2B,0x1F},
	{0x2C,0x2C},  // Cmd_SHS_Dn from 0x26 to 0x2C chris 0913
	{0x2D,0x0F},
	{0x2E,0x1E},
	{0x2F,0x1F},
	{0x30,0x21},
	{0x33,0x16},
	{0x34,0x30},
	{0x35,0x00},
	{0x36,0x00},
	{0x37,0x00},
	{0x38,0x00},
	{0x39,0x0A},
	{0x3B,0x04},
	{0x3C,0x04},
	{0x3D,0x04},
	{0x3E,0x04},
	{0x41,0x06},
	{0x42,0x00},
	{0x43,0x82},
	{0x44,0x0C},
	{0x45,0x43},
	{0x48,0x31},
	{0x49,0x11},
	{0x4A,0x00},
	{0x4B,0x18},
	{0x50,0x03},
	{0x51,0x04},
	{0x52,0x39},
	{0x53,0x02},
	{0x58,0x00},
	{0x5D,0x00},
	{0x5E,0x3F},
	{0x62,0x35},
	{0x63,0x7F},
	{0x64,0x34},
	{0x66,0x06},
	{0x67,0x18},
	{0x68,0x07},
	{0x69,0x01},
	{0x76,0x00},
	{0x77,0x20},
	{0x78,0x0F},
	{0x7A,0x0F},
	{0x7C,0x03},
	{0x7D,0x0F},
	{0x7E,0x00},
	{0x80,0x00},  // nov
	{0x82,0x00},
	{0x91,0x00},
	{0x9F,0x00},
	{0xA1,0x04},
	{0xA2,0xC0},
	{0xA4,0x00},
	{0xA5,0x00},
	{0xA6,0x00},
	{0xA7,0x08},
	{0xA8,0x0F},
	{0xA9,0x00},
	{0xAA,0xAA},
	{0xAB,0x55},
	{0xAC,0x66},
	{0xB0,0x05},
	{0xB1,0x3F},
	{0xB2,0xCA},
	{0xB4,0x0C},
	{0xB6,0x0E},
	{0xBC,0x25},
	{0xBE,0x26},
	{0x11,0x01},
	    
	{0xEF,0x02},
	{0x00,0x00},
	{0x18,0x88},
	{0x19,0xC6},
	{0x22,0x08},
	{0x24,0xFF},//э到堵翴, 20101112, Angelo
	{0x25,0x20},//э到堵翴, 20101112, Angelo
	{0x26,0x12},
	{0x27,0x06},
	{0x2A,0x80},
	{0x2B,0x00},
	{0x2C,0x14},//э到堵翴, 20101112, Angelo
	{0x2D,0x07},//э到堵翴, 20101112, Angelo
	{0x2E,0x40},//э到堵翴, 20101112, Angelo
	{0x2F,0x14},
	{0x30,0x10},
	{0x36,0x02},
	{0x37,0x86},
	{0x38,0x91},
	{0x39,0x10},
	{0x3A,0x8E},
	{0x3B,0x20},
	{0x3C,0x03},
	{0x3E,0x33},
	{0x3F,0x2F},
	{0x40,0x30},
	{0x41,0x21},
	{0x42,0x0C},
	{0x43,0x40},
	{0x44,0x4C},
	{0x45,0x44},
	{0x46,0x44},
	{0x47,0x00},
	{0x50,0x3C},
	{0x51,0x02},
	{0x52,0x0F},
	{0x53,0x03},
	{0x54,0x01},
	{0x55,0x00},
	{0x56,0x01},//э到堵翴, 20101112, Angelo
	{0x57,0x01},//э到堵翴, 20101112, Angelo
	{0x58,0x08},//э到堵翴, 20101112, Angelo
	{0x59,0x00},
	{0x5A,0x06},//э到堵翴, 20101112, Angelo
	{0x5B,0x0A},//э到堵翴, 20101112, Angelo
	{0x5C,0x06},//э到堵翴, 20101112, Angelo
	{0x5D,0x02},
	{0x5E,0x02},
	{0x5F,0x0A},
	{0x60,0x12},
	{0x61,0x00},
	{0x62,0x01},
	{0x63,0x0F},
	{0x64,0x01},
	{0x65,0x48},//э到堵翴, 20101112, Angelo
	{0x66,0xFF},
	{0x69,0x0C},
	{0x6A,0x00},
	{0x9B,0x00},
	{0x9C,0xF2},
	{0x9D,0x01},
	{0x9E,0x40},
	{0x9F,0x00},
	{0xA0,0x00},
	{0xA1,0x00},
	{0xA2,0x00},
	{0xA3,0x00},
	{0xA4,0x02},
	{0xA5,0x00},
	{0xA6,0x02},
	{0xA8,0x00},
	{0xA9,0x20},
	{0xAA,0x00},
	{0xAB,0xF0},
	{0xAD,0x00},
	{0xAE,0x00},
	{0xB0,0x00},
	{0xB1,0x00},
	{0xB2,0x00},
	{0xB4,0x00},
	{0xB5,0xF0},
	{0xB6,0x01},
	{0xB7,0x40},
	{0xB8,0x00},
	{0xB9,0x00},
	{0xBA,0x00},
	{0xBB,0x00},
	{0xBC,0x00},
	{0xBD,0x01},
	{0xBE,0x00},
	{0xBF,0x04},//For SC6600L platform only, don't modify. Angelo 2010/06/23
	{0xC0,0x00},
	{0xC1,0x00},//For SC6600L platform only, don't modify. Angelo 2010/06/23
	{0xC2,0x00},
	{0xC4,0x00},
	{0xC5,0x00},
	{0xC6,0x01},
	{0xC7,0x00},
	{0xC8,0x00},
	{0xC9,0x00},
	{0xCA,0x00},
	{0xCB,0x00},
	{0xCC,0x00},
	{0xCD,0x00},
	{0xCE,0x00},
	{0xCF,0x00},
	{0xD0,0x00},
	{0xD1,0x00},
	{0xD2,0x00},
	{0xD3,0x00},
	{0xD4,0x00},
	{0xD5,0x00},
	{0xD6,0x00},
	{0xD7,0x00},
	{0xD8,0x00},
	{0xD9,0x00},
	{0xDA,0x00},
	{0xDB,0x00},
	{0xDC,0x00},
	{0xDD,0x00},
	{0xDE,0x00},
	{0xDF,0x00},
	{0xE0,0x04},
	{0xE3,0x00},
	{0xE4,0x00},
	{0xE5,0x00},
	{0xE6,0x00},
	{0xEB,0x00},
	{0x00,0x01},

	{0xef,0x02}, //  QVGA 244x324 setting
	{0xa3,0x01},
	{0xa4,0x02},
	{0xa5,0x00},
	{0xa6,0x10},
	{0xa8,0x00},
	{0xa9,0x20},
	{0xaa,0x00},
	{0xab,0xF4},
	{0x9b,0x00},
	{0x9c,0xF4},
	{0x9d,0x01},
	{0x9e,0x44},
	{0xb4,0x00},
	{0xb5,0xF4},
	{0xb6,0x01},
	{0xb7,0x44},
	{0xb8,0x00},
	{0xb9,0x00},
	{0xba,0x00},
	{0xbb,0x00},
	{0xb0,0x00},
	{0xb1,0x00},
	{0xb2,0x00},
	{0x00,0x01},

	{0xef,0x00},
	{0x66,0x13},
	{0xed,0x01},
	
	{0xff,0xff},// Table end	
};

LOCAL SENSOR_REG_TAB_INFO_T s_PAS6175_resolution_Tab_YUV[]=
{	
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(PAS6175_YUV_240X320), 240, 320, 24, SENSOR_IMAGE_FORMAT_YUV422},
	// YUV422 PREVIEW  1
    { PNULL,	0, 240, 320, 24, SENSOR_IMAGE_FORMAT_YUV422},
	{ PNULL,	0, 0,	0, 0, 0},
    { PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0}
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_PAS6175_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    _PAS6175_Power_On,
    PNULL,
    PAS6175_Identify,

    PNULL,			// write register
    PNULL,			// read  register	
    PNULL,
    PNULL,

    // External
    set_PAS6175_ae_enable,
    PAS6175_hmirror_enable,   //
    PAS6175_vmirror_enable,

    set_brightness,  // need to do
    set_contrast,  // need to do
    set_sharpness,
    set_saturation,

    set_preview_mode,	
    set_image_effect,

    PNULL,//PAS6175_BeforeSnapshot, 
    PNULL, //PAS6175_After_Snapshot,

    PNULL,

    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate,	
    PNULL,
    PNULL,
    set_PAS6175_awb,
    PNULL,
    PNULL,
    set_PAS6175_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_PAS6175_anti_flicker,
    set_PAS6175_video_mode,
    PNULL,
};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_PAS6175_yuv_info =
{
	PAS6175_I2C_ADDR_W,				// salve i2c write address
	PAS6175_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_P|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_N,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved											
											
	// preview mode
	SENSOR_ENVIROMENT_NORMAL|\
	SENSOR_ENVIROMENT_NIGHT|\
	SENSOR_ENVIROMENT_SUNNY,		
	
	// image effect
	SENSOR_IMAGE_EFFECT_NORMAL|\
	SENSOR_IMAGE_EFFECT_BLACKWHITE|\
	SENSOR_IMAGE_EFFECT_RED|\
	SENSOR_IMAGE_EFFECT_GREEN|\
	SENSOR_IMAGE_EFFECT_BLUE|\
	SENSOR_IMAGE_EFFECT_YELLOW|\
	SENSOR_IMAGE_EFFECT_NEGATIVE|\
	SENSOR_IMAGE_EFFECT_CANVAS,
	
	// while balance mode
	0,
		
	7,								// bit[0:7]: count of step in brightness, contrast, sharpness, saturation
									// bit[8:31] reseved
	
	SENSOR_LOW_PULSE_RESET,			// reset pulse level
	20,								// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	1,								// count of identify code
	0x00,0x61,						// supply two code to identify sensor.
	0x01,0x75,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	240,							// max width of source image
	320,							// max height of source image
	"PAS6175",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_UYVY,	// pattern of input image form sensor;			

	s_PAS6175_resolution_Tab_YUV,	// point to resolution table information structure
	&s_PAS6175_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	4,                     // skip frame num before preview 
	3,                      // skip frame num before capture
	0,                      // deci frame num during preview	
	2                       // deci frame num during video preview
};
/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void PAS6175_WriteReg( uint8  subaddr, uint8 data )
{
	
	#ifndef	_USE_DSP_I2C_
		uint8 cmd[2];
		cmd[0]	=	subaddr;
		cmd[1]	=	data;		
		I2C_WriteCmdArr(PAS6175_I2C_ADDR_W, cmd, 2, SCI_TRUE);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW("SENSOR: PAS6175_WriteReg reg/value(%x,%x) !!", subaddr, data);

}

LOCAL uint8 PAS6175_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
		I2C_WriteCmdArr(PAS6175_I2C_ADDR_W, &subaddr, 1, SCI_TRUE);
		I2C_ReadCmd(PAS6175_I2C_ADDR_R, &value, SCI_TRUE);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif

	//SCI_TRACE_LOW("SENSOR: PAS6175_ReadReg reg/value(%x,%x) !!", subaddr, value);
    
	return value;
}


LOCAL uint32 PAS6175_Identify(uint32 param)
{
	uint32 i;
	uint32 nLoop;
	uint8 ret;
	uint32 err_cnt = 0;
	uint8 reg[2] 	= {0x00,0x01};
	uint8 value[2] 	= {0x61,0x75};

	for(i = 0; i<2; i++)
	{
		nLoop = 1000;
		PAS6175_WriteReg(0xef,0x00);
		ret = PAS6175_ReadReg(reg[i]);
		if( ret != value[i])
		{
			err_cnt++;
			if(err_cnt>3)
			{
				return SCI_ERROR;
			}
			else
			{
				//Masked by frank.yang,SCI_Sleep() will cause a  Assert when called in boot precedure
				//SCI_Sleep(10);
				while(nLoop--);
				continue;
			}
		}
        err_cnt = 0;
	}

	//SCI_TRACE_LOW("SENSOR: PAS6175_Identify: it is PAS6175");
	return (uint32)SCI_SUCCESS;
}

LOCAL uint32 _PAS6175_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val=g_PAS6175_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val=g_PAS6175_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val=g_PAS6175_yuv_info.iovdd_val;  
    BOOLEAN power_down=g_PAS6175_yuv_info.power_down_level;	    
    BOOLEAN reset_level=g_PAS6175_yuv_info.reset_pulse_level;
    uint32 reset_width=g_PAS6175_yuv_info.reset_pulse_width;	    
    
    if(SCI_TRUE==power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
        
        GPIO_SetSensorPwdn(!power_down);

        // Open Mclk in default frequency
        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);    // Default MCLK=12Mhz
        
        SCI_Sleep(20);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel(!reset_level);
        SCI_Sleep(100);
    }
    else
    {
        GPIO_SetSensorPwdn(power_down);

        Sensor_SetMCLK(SENSOR_DISABLE_MCLK);           

        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);        
    }
    
    //SCI_TRACE_LOW("SENSOR: _PAS6175_Power_On(1:on, 0:off): %d", power_on);    
    
    return SCI_SUCCESS;
}

LOCAL uint32 set_PAS6175_ae_enable(uint32 enable)
{
	unsigned char Reg_vlaue = 0;
	
	PAS6175_WriteReg(0xef,0x00);
	Reg_vlaue = PAS6175_ReadReg(0x66);

	if(enable == 1)
		Reg_vlaue |= 0x10;
	else
		Reg_vlaue &= 0xef;

	PAS6175_WriteReg(0x66,Reg_vlaue);	

	//SCI_TRACE_LOW("SENSOR: set_ae_enable: enable = %d", enable);
	return 0;
}


LOCAL uint32 set_PAS6175_awb_enable(uint32 enable)
{
	unsigned char Reg_vlaue = 0;
	
	PAS6175_WriteReg(0xef,0x00);
	Reg_vlaue = PAS6175_ReadReg(0x72);

	if(enable == 1)
		Reg_vlaue |= 0x01;
	else
		Reg_vlaue &= 0xfe;
	
	PAS6175_WriteReg(0x72,Reg_vlaue);	
	
	//SCI_TRACE_LOW("SENSOR: set_awb_enable: enable = %d", enable);
	return 0;
	
}


LOCAL uint32 set_PAS6175_ae_awb_enable(uint32 ae_enable, uint32 awb_enable)
{
	set_PAS6175_ae_enable(ae_enable);
	set_PAS6175_awb_enable(awb_enable);

    //SCI_TRACE_LOW("SENSOR: set_ae_awb_enable: ae=%d awb=%d", ae_enable, awb_enable);

	return 0;
}



//LOCAL uint32 set_hmirror_enable(uint32 enable)
PUBLIC uint32 PAS6175_hmirror_enable(uint32 enable)   //wangfang modify on 20100628
{
	unsigned char Reg_value = 0;
	PAS6175_WriteReg(0xef,0x01);
	Reg_value = PAS6175_ReadReg(0x21);

	if(enable == 1)
		Reg_value |= 0x04;
	else
		Reg_value &= 0xfb;
	
	PAS6175_WriteReg(0x21,Reg_value);
	PAS6175_WriteReg(0x11,0x01);
	
	//SCI_TRACE_LOW("set_hmirror_enable: enable = %d", enable);
	
	return 0;
}


//LOCAL uint32 set_vmirror_enable(uint32 enable)
PUBLIC uint32 PAS6175_vmirror_enable(uint32 enable)   //wangfang modify on 20100628
{
	unsigned char Reg_value = 0;
	PAS6175_WriteReg(0xef,0x01);
	Reg_value = PAS6175_ReadReg(0x21);

	if(enable == 1)
		Reg_value |= 0x08;
	else
		Reg_value &= 0xf7;
	
	PAS6175_WriteReg(0x21,Reg_value);
	PAS6175_WriteReg(0x11,0x01);

	//SCI_TRACE_LOW("set_vmirror_enable: enable = %d", enable);
	
	return 0;
}

__align(4) const SENSOR_REG_T PAS6175_ev_tab[][4]=
{
    {{0xef,0x00},{0x6f, 95},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x6f,110},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x6f,135},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x6f,150},{0xed,0x01},{0xff,0xff}},  // normal
	{{0xef,0x00},{0x6f,165},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x6f,180},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x6f,205},{0xed,0x01},{0xff,0xff}}
};

LOCAL uint32 set_PAS6175_ev(uint32 level)
{
    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    SCI_ASSERT(level < 7);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

	//SCI_TRACE_LOW("SENSOR: set_ev: level = %d", level);

    return 0;
}
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_PAS6175_anti_flicker(uint32 mode)
{//24m->65.6 us 12m->131us
    uint8 BANDING_60HZ = 0;

    switch(mode)
    {
        case DCAMERA_FLICKER_50HZ:
				BANDING_60HZ = 0;
            break;
            
        case DCAMERA_FLICKER_60HZ:
				BANDING_60HZ = 1;
            break;
            
        default:
            break;
    }

	PAS6175_WriteReg(0xef,0x00);
	PAS6175_WriteReg(0x66,((PAS6175_ReadReg(0x66)&0xFE)| BANDING_60HZ));
	PAS6175_WriteReg(0xed,0x01);

    OS_TickDelay(100); 
    //SCI_TRACE_LOW("SENSOR: set_banding_mode: mode = %d", mode);

    return 0;
}
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//		 
/******************************************************************************/
__align(4) const SENSOR_REG_T PAS6175_video_mode_nand_tab[][4]=
{
    // normal mode
    {
        {0x2d,0x00},{0x2e,0x00},{0x14,0x18},{0xff,0xff}
    },    
    //vodeo mode
    {
        {0x2d,0x00},{0x2e,0x00},{0x14,0x38},{0xff,0xff}      
    },
    //upcc mode
    {
        {0xff,0xff},{0x2e,0x00},{0x14,0x28},{0xff,0xff}                      
    }
};
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//		 
/******************************************************************************/
__align(4) const SENSOR_REG_T PAS6175_video_mode_nor_tab[][6]= //Angelo, Need modify
{
    // normal mode
    {
        {0x2d,0x00},{0x2e,0x00},{0x14,0x18},{0x92,0x00}, {0x93,0x00}, {0xff,0xff}
    }, 

    #ifdef CHIP_DRV_SC6600L
    //vodeo mode
    {// 30fps (for l page 10fps, for brust 15fps)
        {0x2d,0x00},{0x2e,0x00},{0x14,0x28}, {0x92,0x00}, {0x93,0x00}, {0xff,0xff}      
    },
    #else
     //vodeo mode
    {
        {0x2d,0x00},{0x2e,0x00},{0x14,0x38}, {0x92,0x68}, {0x93,0x01}, {0xff,0xff}      
    },
    #endif

    //upcc mode
    {
    	{0xff,0xff},{0x2e,0x00},{0x14,0x38}, {0x92,0x68}, {0x93,0x01}, {0xff,0xff}       
    }
};    

LOCAL uint32 set_PAS6175_video_mode(uint32 mode)
{
    uint8 data=0x00;
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;

  //  SCI_ASSERT(mode <= 1);
    SCI_ASSERT(mode<=DCAMERA_MODE_MAX);

#if 0
    if(CHIP_DetectMemType())
        sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_video_mode_nand_tab[mode];
    else
        sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_video_mode_nor_tab[mode];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
#endif

    PAS6175_WriteReg(0xef,0x01);          
    switch(mode)
    {
        case DCAMERA_NORMAL_MODE:
     //       PAS6175_WriteReg(0x02,0x04);                  
            break;

        case DCAMERA_VIDEO_MODE:     
      //      PAS6175_WriteReg(0x02,0x04);      
            break;
        default :
            break;
    }    
    PAS6175_WriteReg(0x11,0x01);          
#if 0
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
    	Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
#endif
    //SCI_Sleep(500);

    //SCI_TRACE_LOW("SENSOR: set_video_mode: mode = %d", mode);
    return 0;
}

/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) const SENSOR_REG_T PAS6175_awb_tab[][12]=
{
    //AUTO
    {
		{0xef,0x00},
		{0x72,0x01},
		{0xed,0x01},
		{0x80,0xfe},
		{0xed,0x01},
		{0xef,0x01},
		{0x06,0x0F},
		{0x07,0x13},
		{0x08,0x0F},
		{0x09,0x16},
		{0x11,0x01},		
		{0xff,0xff}				
	},    
    //INCANDESCENCE:
    {
		{0xef,0x00},
		{0x72,0x10},
		{0xed,0x01},
		{0x80,0x86},
		{0xed,0x01},
		{0xef,0x01},
		{0x06,0x0c},
		{0x07,0x1c},
		{0x08,0x0c},
		{0x09,0x09},
		{0x11,0x01},					
		{0xff,0xff} 	
    },
    //U30 //need modify
    {
		{0xef,0x00},
		{0x72,0x10},
		{0xed,0x01},
		{0x80,0x86},
		{0xed,0x01},
		{0xef,0x01},
		{0x06,0x10},
		{0x07,0x11},
		{0x08,0x10},
		{0x09,0x14},
		{0x11,0x01},					
		{0xff,0xff}  
    },  
    //CWF //need modify
    {
		{0xef,0x00},
		{0x72,0x10},
		{0xed,0x01},
		{0x80,0x86},
		{0xed,0x01},
		{0xef,0x01},
		{0x06,0x10},
		{0x07,0x11},
		{0x08,0x10},
		{0x09,0x0d},
		{0x11,0x01},					
		{0xff,0xff}  
    },    
    //FLUORESCENT:
    {
		{0xef,0x00},
		{0x72,0x10},
		{0xed,0x01},
		{0x80,0x86},
		{0xed,0x01},
		{0xef,0x01},
		{0x06,0x10},
		{0x07,0x15},
		{0x08,0x10},
		{0x09,0x19},
		{0x11,0x01},					
		{0xff,0xff}
    },
    //SUN:
    {
		{0xef,0x00},
		{0x72,0x10},
		{0xed,0x01},
		{0x80,0x86},
		{0xed,0x01},
		{0xef,0x01},
		{0x06,0x0C},
		{0x07,0x0A},
		{0x08,0x0C},
		{0x09,0x16},
		{0x11,0x01},					
		{0xff,0xff}   
    },
    //CLOUD:
    {
		{0xef,0x00},
		{0x72,0x10},
		{0xed,0x01},
		{0x80,0x86},
		{0xed,0x01},
		{0xef,0x01},
		{0x06,0x0c},
		{0x07,0x0e},
		{0x08,0x0c},
		{0x09,0x14},
		{0x11,0x01},				    
        {0xff,0xff}            
    }
};

LOCAL uint32 set_PAS6175_awb(uint32 mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_awb_tab[mode];

	SCI_ASSERT(mode < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		PAS6175_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW("SENSOR: set_awb_mode: mode = %d", mode);
	
	return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T PAS6175_brightness_tab[][6]=//Angelo, Need modify
{
	{{0xef,0x00},{0x94,0x01},{0x93,0x09},{0x91,226},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x94,0x01},{0x93,0x09},{0x91,236},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x94,0x01},{0x93,0x09},{0x91,246},{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x94,0x01},{0x93,0x09},{0x91,0  },{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x94,0x01},{0x93,0x09},{0x91,10 },{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x94,0x01},{0x93,0x09},{0x91,25 },{0xed,0x01},{0xff,0xff}},
	{{0xef,0x00},{0x94,0x01},{0x93,0x09},{0x91,40 },{0xed,0x01},{0xff,0xff}}
};

LOCAL uint32 set_brightness(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_brightness_tab[level];

	SCI_ASSERT(level < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		PAS6175_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW("SENSOR: set_brightness: level = %d", level);
	
	return 0;
}

__align(4) const SENSOR_REG_T PAS6175_contrast_tab[][4]=//Angelo, Need modify
{
	{{0xef,0x02},{0x65,0x40},{0x00,0x01},{0xff,0xff}},
	{{0xef,0x02},{0x65,0x42},{0x00,0x01},{0xff,0xff}},
	{{0xef,0x02},{0x65,0x44},{0x00,0x01},{0xff,0xff}},
	{{0xef,0x02},{0x65,0x46},{0x00,0x01},{0xff,0xff}},
	{{0xef,0x02},{0x65,0x49},{0x00,0x01},{0xff,0xff}},    
	{{0xef,0x02},{0x65,0x4C},{0x00,0x01},{0xff,0xff}},
	{{0xef,0x02},{0x65,0x4F},{0x00,0x01},{0xff,0xff}}
	
};

LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_contrast_tab[level];

    SCI_ASSERT(level < 7 );
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        PAS6175_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    OS_TickDelay(20);
    //SCI_TRACE_LOW("SENSOR: set_contrast: level = %d", level);

    return 0;
}


LOCAL uint32 set_sharpness(uint32 level)
{
	return 0;
}


LOCAL uint32 set_saturation(uint32 level)
{
	return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/

LOCAL uint32 set_preview_mode(uint32 preview_mode)
{
	//SCI_TRACE_LOW("SENSOR: set_preview_mode: preview_mode = %d", preview_mode);
	
	s_preview_mode = preview_mode;
	
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{
			PAS6175_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			PAS6175_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			PAS6175_set_work_mode(0);
			break;
		}
		default:
		{
			break;
		}
			
	}
	
	SCI_Sleep(250);
	
	return 0;
}


/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) const SENSOR_REG_T PAS6175_image_effect_tab[][7]=
{
    // effect normal
    {
        {0xef,0x00},
		{0x8f,0x00},
		{0x90,0x00},
		{0x93,0x00},
		{0x94,0x00},
		{0xed,0x01},
		{0xff,0xff}
    },
    //effect BLACKWHITE //CAM_EFFECT_ENC_GRAYSCALE
    {
        {0xef,0x00},
		{0x8f,0x00},
		{0x90,0x00},
		{0x93,0x00},
		{0x94,0x01},
		{0xed,0x01},
		{0xff,0xff}
    },
    // effect RED
    {

        {0xef,0x00},
		{0x8f,0x78},
		{0x90,0xB4},
		{0x93,0x03},
		{0x94,0x01},
		{0xed,0x01},
		{0xff,0xff}
    
    },
    // effect GREEN
    {
        {0xef,0x00},
		{0x8f,0x69},
		{0x90,0x71},
		{0x93,0x03},
		{0x94,0x01},
		{0xed,0x01},
		{0xff,0xff}
    },
    // effect  BLUE
    {
       {0xef,0x00},
		{0x8f,0x97},
		{0x90,0x84},
		{0x93,0x03},
		{0x94,0x01},
		{0xed,0x01},
		{0xff,0xff}
    },
    // effect  YELLOW
    {
        {0xef,0x00},
		{0x8f,0x00},
		{0x90,0xAA},
		{0x93,0x03},
		{0x94,0x01},
		{0xed,0x01},
		{0xff,0xff}
    },  
    // effect NEGATIVE // CAM_EFFECT_ENC_COLORINV
    {	     
        {0xef,0x00},
		{0x8f,0x00},
		{0x90,0x00},
		{0x93,0x02},
		{0x94,0x01},
		{0xed,0x01},
		{0xff,0xff}
    },   
    //effect ANTIQUE //CAM_EFFECT_ENC_SEPIA
    {
	   	{0xef,0x00},
		{0x8f,0x64},
		{0x90,0x8D},
		{0x93,0x03},
		{0x94,0x01},
		{0xed,0x01},
		{0xff,0xff}
    },
};

	
LOCAL uint32 set_image_effect(uint32 effect_type)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_image_effect_tab[effect_type];

	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		PAS6175_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//SCI_TRACE_LOW("SENSOR: set_image_effect: effect_type = %d", effect_type);
    
	return 0;
}

LOCAL uint32 PAS6175_After_Snapshot(uint32 param)
{
    //SCI_TRACE_LOW("SENSOR: PAS6175_After_Snapshot");  

    return 0;
    
}

LOCAL uint32 PAS6175_BeforeSnapshot(uint32 param)
{
    //SCI_TRACE_LOW("SENSOR: PAS6175_BeforeSnapshot");  

	return 0;
}

LOCAL uint32 read_ev_value(uint32 value)
{
	return 0;
}

LOCAL uint32 write_ev_value(uint32 exposure_value)
{
	
	return 0;	
}

LOCAL uint32 read_gain_value(uint32 value)
{
	return 0;
}

LOCAL uint32 write_gain_value(uint32 gain_value)
{

	
	return 0;
}

LOCAL uint32 read_gain_scale(uint32 value)
{
	return SENSOR_GAIN_SCALE;
	
}


LOCAL uint32 set_frame_rate(uint32 param)
    
{
	
	
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;	 1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T PAS6175_mode_tab[][11]=
{
	//Normal Mode
	{
		{0xef,0x00},
		{0x6c,0x14},
		{0x6d,0x20},
		{0xed,0x01},
		{0xef,0x02},
		{0x2f,0x14},
		{0x30,0x10},
		{0x36,0x02},        
	//	{0x5f,0x09},		
		{0x00,0x01},
		{0xff,0xff}
	}, 

	//Night Mode
	{
		{0xef,0x00},
		{0x6c,0x1B},
		{0x6d,0x22},
		{0xed,0x01},
		{0xef,0x02},
		{0x2f,0x1B},
		{0x30,0x14},
		{0x36,0x04},        
	//	{0x5f,0x07},		
		{0x00,0x01},
		{0xff,0xff}
	}
};

LOCAL uint32 PAS6175_set_work_mode(uint32 mode)
{
    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)PAS6175_mode_tab[mode];

    SCI_ASSERT(mode <= 1);
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    switch(mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL:

            break;

        case DCAMERA_ENVIRONMENT_NIGHT:

            break;

        default :
            break;
    }    

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
	    Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW("SENSOR: set_work_mode: mode = %d", mode);
    return 0;
}
