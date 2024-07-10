/******************************************************************************
 ** Copyright (c)
 ** File Name:		sensor_bf30a2.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor bf30a2.
 **
 ******************************************************************************

 ******************************************************************************
 ** 					   Edit History 									  *
 ** ------------------------------------------------------------------------- *
 ** DATE		   NAME 			DESCRIPTION 							  *
 **2011.12.05     january             BYD_BME
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 ** 						Dependencies									  *
 **---------------------------------------------------------------------------*/
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
 ** 						Compiler Flag									  *
 **---------------------------------------------------------------------------*/
#ifdef	 __cplusplus
	extern	 "C"
	{
#endif
/*lint -save -e785 */
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
#define bf30a2_I2C_ADDR_W		0xDC
#define bf30a2_I2C_ADDR_R		0xDD

/**---------------------------------------------------------------------------*
     bf30a2 SUPPORT 5 SERIAL MODE:
      SPI, CCIR656_2BIT, CCIR656_4BIT
 **---------------------------------------------------------------------------*/

//#define bf30a2_OUTPUT_MODE_SPI
  #define bf30a2_OUTPUT_MODE_CCIR656_1BIT
//#define bf30a2_OUTPUT_MODE_CCIR656_2BIT
//#define bf30a2_OUTPUT_MODE_CCIR656_4BIT
//#define bf30a2_OUTPUT_MODE_PACKET_4BIT
//#define bf30a2_OUTPUT_MODE_PACKET_2BIT
//#define bf30a2_OUTPUT_MODE_PACKET_1BIT




/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 bf30a2_set_ae_awb_enable(uint32 ae_enable, uint32 awb_enable);
LOCAL uint32 bf30a2_set_ae_enable(uint32 enable);
LOCAL uint32 bf30a2_set_hmirror_enable(uint32 enable);
LOCAL uint32 bf30a2_set_vmirror_enable(uint32 enable);
LOCAL uint32 bf30a2_set_preview_mode(uint32 preview_mode);
LOCAL uint32 bf30a2_Identify(uint32 param);
LOCAL uint32 bf30a2_BeforeSnapshot(uint32 param);
LOCAL uint32 bf30a2_After_Snapshot(uint32 param);

LOCAL uint32 bf30a2_set_brightness(uint32 level);
LOCAL uint32 bf30a2_set_contrast(uint32 level);
LOCAL uint32 bf30a2_set_sharpness(uint32 level);
LOCAL uint32 bf30a2_set_saturation(uint32 level);
LOCAL uint32 bf30a2_set_image_effect(uint32 effect_type);

LOCAL uint32 bf30a2_read_ev_value(uint32 value);
LOCAL uint32 bf30a2_write_ev_value(uint32 exposure_value);
LOCAL uint32 bf30a2_read_gain_value(uint32 value);
LOCAL uint32 bf30a2_write_gain_value(uint32 gain_value);
LOCAL uint32 bf30a2_read_gain_scale(uint32 value);
LOCAL uint32 bf30a2_set_frame_rate(uint32 param);
LOCAL uint32 bf30a2_set_work_mode(uint32 mode);

LOCAL uint32 set_bf30a2_ev(uint32 level);
LOCAL uint32 set_bf30a2_awb(uint32 mode);
LOCAL uint32 set_bf30a2_anti_flicker(uint32 mode);
LOCAL uint32 set_bf30a2_video_mode(uint32 mode);
LOCAL uint32 _bf30a2_Power_On(uint32 power_on);
LOCAL uint32 bf30a2_GetPclkTab(uint32 param);


/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/


__align(4) const SENSOR_REG_T bf30a2_YUV_Setting[]=
{
	//BF3A02 SPI ZX V1 20200319
//XCLK:24M; SPICLK: 24M; MCLK:3M
//Width:240; Height: 320
//行长：294  帧长：340
//Max fps: 15fps
	{0xcf,0xf0},//0xb0驱动能力
	{0x12,0x40},//zx:0x10 mtk:0x20 rda:0x40
	{0x6b,0x70},
	{0x15,0x10},
	{0x00,0x40},
	{0x04,0x00},
	{0x06,0x26},
	{0x08,0x07},
	{0x1c,0x12},
	{0x1e,0x26},
	{0x1f,0x01},
	{0x20,0x20},
	{0x21,0x20},
	{0x36,0x21},
	{0x37,0x13},
	{0xca,0x03},
	{0xcb,0x22},
	{0xcc,0x49},
	{0xcd,0x4c},
	{0xce,0x72},
	//{0xcf,0xf0},
	{0xa0,0x8e},
	{0x01,0x1b},
	{0x02,0x1d},
	{0x13,0x08},
	{0x87,0x13},
	{0x8b,0x08},
	{0x70,0x1f},
	{0x71,0x43},
	{0x72,0x0a},
	{0x73,0x62},
	{0x74,0xa2},
	{0x75,0xbf},
	{0x76,0x02},
	{0x77,0xcc},
	{0x40,0x32},
	{0x41,0x28},
	{0x42,0x26},
	{0x43,0x1d},
	{0x44,0x1a},
	{0x45,0x14},
	{0x46,0x11},
	{0x47,0x0f},
	{0x48,0x0e},
	{0x49,0x0d},
	{0x4B,0x0c},
	{0x4C,0x0b},
	{0x4E,0x0a},
	{0x4F,0x09},
	{0x50,0x09},
	{0x24,0x50},
	{0x25,0x36},
	{0x80,0x33},
	{0x81,0x20},
	{0x82,0x40},
	{0x83,0x30},
	{0x84,0x50},
	{0x85,0x30},
	{0x86,0xD8},
	{0x89,0x45},
	{0x8a,0x33},
	{0x8f,0x81},
	{0x91,0xff},
	{0x92,0x08},
	{0x94,0x82},
	{0x95,0x8d},
	{0x9a,0x20},
	{0xf0,0x8f},
	{0x51,0x06},
	{0x52,0x25},
	{0x53,0x2b},
	{0x54,0x0F},
	{0x57,0x2A},
	{0x58,0x22},
	{0x59,0x2c},
	{0x23,0x33},
	{0xa1,0x93},
	{0xa2,0x0f},
	{0xa3,0x2a},
	{0xa4,0x08},
	{0xa5,0x26},
	{0xa7,0x80},
	{0xa8,0x80},
	{0xa9,0x1e},
	{0xaa,0x19},
	{0xab,0x18},
	{0xae,0x50},
	{0xaf,0x04},
	{0xc8,0x10},
	{0xc9,0x15},
	{0xd3,0x0c},
	{0xd4,0x16},
	{0xee,0x06},
	{0xef,0x04},
	{0x55,0x34},
	{0x56,0x90},
	{0xb1,0x98},
	{0xb2,0x98},
	{0xb3,0xc4},
	{0xb4,0x0C},
	{0xa0,0x8f},
	{0x13,0x07},
};


LOCAL SENSOR_REG_TAB_INFO_T s_bf30a2_resolution_Tab_YUV[]=
{
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(bf30a2_YUV_Setting),240,320,24,SENSOR_IMAGE_FORMAT_YUV422},

	// YUV422 PREVIEW 1
	{ PNULL, 0, 240, 320, 24, SENSOR_IMAGE_FORMAT_YUV422},

	{ PNULL,					0,		0,	0,							0 , 0},
	{ PNULL,					0,		0,	0,							0 , 0},
	{ PNULL,					0,		0,	0,							0 , 0},

	// YUV422 PREVIEW 2
	{ PNULL,					0,		0,	0,							0 , 0},
	{ PNULL,					0,		0,	0,							0 , 0},
	{ PNULL,					0,		0,	0,							0 , 0},
	{ PNULL,					0,		0,	0,							0 , 0}

};


LOCAL SENSOR_IOCTL_FUNC_TAB_T s_bf30a2_ioctl_func_tab =
{
    // Internal
    PNULL,
    _bf30a2_Power_On,
    PNULL,
    bf30a2_Identify,

    PNULL,			// write register
    PNULL,			// read  register
    PNULL,
    bf30a2_GetPclkTab,

    // External
    PNULL,//   bf30a2_set_ae_enable,
    bf30a2_set_hmirror_enable,
    bf30a2_set_vmirror_enable,

    bf30a2_set_brightness,
    bf30a2_set_contrast,
    bf30a2_set_sharpness,
    bf30a2_set_saturation,

    bf30a2_set_preview_mode,
    bf30a2_set_image_effect,

    PNULL,                //bf30a2_BeforeSnapshot,
    PNULL,             //bf30a2_After_Snapshot,
    PNULL,

    bf30a2_read_ev_value,
    bf30a2_write_ev_value,
    bf30a2_read_gain_value,
    bf30a2_write_gain_value,
    bf30a2_read_gain_scale,
    bf30a2_set_frame_rate,
    PNULL,
    PNULL,
    set_bf30a2_awb,
    PNULL,
    PNULL,
    PNULL,//set_bf30a2_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_bf30a2_anti_flicker,
    set_bf30a2_video_mode,
    PNULL,

};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC const SENSOR_INFO_T g_bf30a2_yuv_info =
{
	bf30a2_I2C_ADDR_W,				// salve i2c write address
	bf30a2_I2C_ADDR_R, 				// salve i2c read address

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

	2,								// count of identify code
	0xFC, 0x3b,						// supply two code to identify sensor.
	0xFD, 0x02,						// for Example: index = 0-> Device id, index = 1 -> version id

	SENSOR_AVDD_2800MV,				// voltage of avdd

	240,							// max width of source image
	320,							// max height of source image
	"bf30a2",						// name of sensor

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;

	s_bf30a2_resolution_Tab_YUV,	// point to resolution table information structure
	&s_bf30a2_ioctl_func_tab,		// point to ioctl function table

	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor

	SENSOR_AVDD_1800MV,                     // iovdd

	SENSOR_AVDD_1800MV,                      // dvdd
	4,                     // skip frame num before preview   ///4
	3,                      // skip frame num before capture
	0,                      // deci frame num during preview
	0,                       // deci frame num during video preview
	0,
	0,
	0,
	0,
#ifdef bf30a2_OUTPUT_MODE_SPI
		SENSOR_OUTPUT_MODE_SPI_1BIT,
#elif defined(bf30a2_OUTPUT_MODE_CCIR656_1BIT)
		SENSOR_OUTPUT_MODE_CCIR656_1BIT,
#elif defined(bf30a2_OUTPUT_MODE_CCIR656_2BIT)
		SENSOR_OUTPUT_MODE_CCIR656_2BIT,
#elif defined(bf30a2_OUTPUT_MODE_CCIR656_4BIT)
		SENSOR_OUTPUT_MODE_CCIR656_4BIT,
#elif defined(bf30a2_OUTPUT_MODE_PACKET_4BIT)
		SENSOR_OUTPUT_MODE_PACKET_4BIT,
#elif defined(bf30a2_OUTPUT_MODE_PACKET_2BIT)
			SENSOR_OUTPUT_MODE_PACKET_2BIT,
#elif defined(bf30a2_OUTPUT_MODE_PACKET_1BIT)
		SENSOR_OUTPUT_MODE_PACKET_1BIT,
#else
      SENSOR_OUTPUT_MODE_CCIR601_8BIT,
#endif
   SENSOR_OUTPUT_ENDIAN_BIG
};


/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void bf30a2_WriteReg( uint8  subaddr, uint8 data )
{
	Sensor_WriteReg(subaddr, data);
	SCI_TRACE_LOW("bf30a2_WriteReg reg/value(%x,%x)", subaddr, data);
}


LOCAL uint8 bf30a2_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	value =Sensor_ReadReg( subaddr);
	SCI_TRACE_LOW("bf30a2_ReadReg subaddr/value(%x,%x)",subaddr,value);

	return value;
}


LOCAL uint32 bf30a2_Identify(uint32 param)
{
	uint32 i;
	uint32 nLoop;
	uint8 ret;
	uint32 err_cnt = 0;
	uint8 reg[2] = {0xfC, 0xfD};
	uint8 value[2] = {0x3b, 0x02};

	SCI_TRACE_LOW("bf30a2_Identify:start");

	for(i = 0; i<2; )
	{
		nLoop = 1000;
		ret = bf30a2_ReadReg(reg[i]);
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
				while(nLoop--);/*lint !e722*/
				continue;
			}
		}
        err_cnt = 0;
		i++;
	}

	SCI_TRACE_LOW("bf30a2_Identify:it is bf30a2");

	return (uint32)SCI_SUCCESS;
}

LOCAL SENSOR_TRIM_T s_bf30a2_Pclk_Tab[]=
{
	// COMMON INIT
	{0, 0, 0, 0,  6},

	// YUV422 PREVIEW 1
	{0, 0, 0, 0,  6},
	{0, 0, 0, 0,  6},
	{0, 0, 0, 0,  6},
	{0, 0, 0, 0,  6},

	// YUV422 PREVIEW 2
	{0, 0, 0, 0,  0},
	{0, 0, 0, 0,  0},
	{0, 0, 0, 0,  0},
	{0, 0, 0, 0,  0}
};

LOCAL uint32 bf30a2_GetPclkTab(uint32 param)
{
    return (uint32)s_bf30a2_Pclk_Tab;
}

LOCAL uint32 bf30a2_set_ae_enable(uint32 enable)
{
    unsigned char ae_value;

    ae_value=bf30a2_ReadReg(0x13);

    if(0x00==enable)
    {
        ae_value&=(~0x01);
        bf30a2_WriteReg(0x13,ae_value);
    }
    else if(0x01==enable)
    {
        ae_value|=0x01;
        bf30a2_WriteReg(0x13,ae_value);
    }

	SCI_TRACE_LOW("bf30a2_set_ae_enable:%d", enable);

	return 0;
}


LOCAL uint32 bf30a2_set_ae_awb_enable(uint32 ae_enable, uint32 awb_enable)
{
    unsigned char ae_value,awb_value;

	ae_value = bf30a2_ReadReg(0x13);
	awb_value = bf30a2_ReadReg(0xa0);

    if(0x00==ae_enable)
    {
        ae_value&=(~0x01);
    }
    else if(0x01==ae_enable)
    {
        ae_value|=0x01;
    }

    if(0x00==awb_enable)
    {
        awb_value&=~0x01;
    }
    else if(0x01==awb_enable)
    {
        awb_value|=0x01;
    }
	bf30a2_WriteReg(0x13,ae_value);
	bf30a2_WriteReg(0xa0,awb_value);
    SCI_TRACE_LOW("bf30a2_set_ae_awb_enable:ae=%d awb=%d", ae_enable, awb_enable);

	return 0;
}


void set_bf30a2_shutter(uint16 shutter)
{

}


uint16 get_bf30a2_shutter(void)
{

	return 0;
}


LOCAL uint32 bf30a2_set_hmirror_enable(uint32 enable)
{

	return 0;
}


LOCAL uint32 bf30a2_set_vmirror_enable(uint32 enable)
{

	return 0;
}


__align(4) const SENSOR_REG_T bf30a2_ev_tab[][2]=
{
	{{0x24,0x1f},{0xff,0xff}},
	{{0x24,0x2f},{0xff,0xff}},
	{{0x24,0x3f},{0xff,0xff}},
	{{0x24,0x50},{0xff,0xff}},
	{{0x24,0x5f},{0xff,0xff}},
	{{0x24,0x6f},{0xff,0xff}},
	{{0x24,0x7f},{0xff,0xff}},
};


LOCAL uint32 set_bf30a2_ev(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    SCI_ASSERT(level < 7);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("set_bf30a2_ev:level=%d", level);
    return 0;
}

__align(4) const SENSOR_REG_T bf30a2_anti_flicker_mode_tab[][2]=
{
	{
		{0x8a,0x33},
		{0xff,0xff}
	},
	{
		{0x8a,0x33},
		{0xff,0xff}
	},

};


/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence:
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_bf30a2_anti_flicker(uint32 mode)
{//24m->65.6 us 12m->131us

	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_anti_flicker_mode_tab[mode];
	SCI_ASSERT(mode < 2);
	SCI_ASSERT(PNULL != sensor_reg_ptr);

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

    OS_TickDelay(100);

    SCI_TRACE_LOW("set_bf30a2_anti_flicker:mode=%d", mode);
	return 0;
}


/******************************************************************************/
// Description: set video mode
// Global resource dependence:
// Author:
// Note:
//
/******************************************************************************/
__align(4) const SENSOR_REG_T bf30a2_video_mode_nor_tab[][2]=
{
    // normal mode 14fps
    {
			{0xf0,0x8f},{0xff, 0xff}
    },
    //video mode 10fps
    {
			{0xf0,0x8f},{0xff, 0xff}
    },
    // UPCC  mode 14fps
    {
		  {0xf0,0x8f}, {0xff, 0xff}
    }
};


LOCAL uint32 set_bf30a2_video_mode(uint32 mode)
{
  /*
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_video_mode_nor_tab[mode];


    SCI_ASSERT(mode<=DCAMERA_MODE_MAX);
    SCI_ASSERT(PNULL != sensor_reg_ptr);
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
    	bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("SENSOR: set_bf30a2_video_mode: mode = %d", mode);
*/
    return 0;
}


/******************************************************************************/
// Description: set wb mode
// Global resource dependence:
// Author:
// Note:
//
/******************************************************************************/
__align(4) const SENSOR_REG_T bf30a2_awb_tab[][5]=

{
     //AUTO:
    {
			{0xa0,0x8f},
			{0xa1,0x93},
			{0xff,0xff},
			{0xff,0xff},
			{0xff,0xff}
    },
    //INCANDESCENCE:
    {
			{0xa0,0x8e},
			{0xa1,0x91},
			{0x01,0x2a},
			{0x02,0x0a},
			{0xff,0xff}
    },
     //U30:
    {
			{0xa0,0x8e},
			{0xa1,0x91},
			{0x01,0x24},
			{0x02,0x10},
			{0xff,0xff}
	},
    //CWF:
    {
			{0xa0,0x8e},
			{0xa1,0x91},
			{0x01,0x20},
			{0x02,0x18},
			{0xff,0xff}
    },
    //FLUORESCENT:
    {
			{0xa0,0x8e},
			{0xa1,0x91},
			{0x01,0x13},
			{0x02,0x1e},
			{0xff,0xff}
    },
    //SUN:
    {
			{0xa0,0x8e},
			{0xa1,0x91},
			{0x01,0x0f},
			{0x02,0x22},
			{0xff,0xff}
    },
    //CLOUD
    {
			{0xa0,0x8e},
			{0xa1,0x91},
			{0x01,0x0b},
			{0x02,0x2c},
			{0xff,0xff}
    },
};


LOCAL uint32 set_bf30a2_awb(uint32 mode)
{

	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_awb_tab[mode];

	SCI_ASSERT(mode < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100);
	SCI_TRACE_LOW("set_bf30a2_awb:mode=%d", mode);

	return 0;
}


/******************************************************************************/
// Description: set brightness
// Global resource dependence:
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T bf30a2_brightness_tab[][2]=
{
	{{0x24,0x1f},{0xff,0xff}},
	{{0x24,0x2f},{0xff,0xff}},
	{{0x24,0x3f},{0xff,0xff}},
	{{0x24,0x50},{0xff,0xff}},
	{{0x24,0x5f},{0xff,0xff}},
	{{0x24,0x6f},{0xff,0xff}},
	{{0x24,0x7f},{0xff,0xff}},
};


LOCAL uint32 bf30a2_set_brightness(uint32 level)
{

	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_brightness_tab[level];

	SCI_ASSERT(level < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	OS_TickDelay(100);

	SCI_TRACE_LOW("bf30a2_set_brightness:level=%d", level);

	return 0;
}


__align(4) const SENSOR_REG_T bf30a2_contrast_tab[][2]=
{

		{{0x56,0x50},{0xff,0xff}},
		{{0x56,0x60},{0xff,0xff}},
		{{0x56,0x70},{0xff,0xff}},
		{{0x56,0x90},{0xff,0xff}},
		{{0x56,0xa3},{0xff,0xff}},
		{{0x56,0xb3},{0xff,0xff}},
		{{0x56,0xc3},{0xff,0xff}},


};


LOCAL uint32 bf30a2_set_contrast(uint32 level)
{

    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_contrast_tab[level];

    SCI_ASSERT(level < 7 );
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    OS_TickDelay(20);
    SCI_TRACE_LOW("bf30a2_set_contrast:level=%d", level);
    return 0;
}


LOCAL uint32 bf30a2_set_sharpness(uint32 level)
{

	return 0;
}


LOCAL uint32 bf30a2_set_saturation(uint32 level)
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
LOCAL uint32 bf30a2_set_preview_mode(uint32 preview_mode)
{
	SCI_TRACE_LOW("bf30a2_set_preview_mode:preview_mode=%d", preview_mode);

	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL:
		{
			bf30a2_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			bf30a2_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			bf30a2_set_work_mode(0);
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
__align(4) const SENSOR_REG_T bf30a2_image_effect_tab[][5]=
{
	{
		{0x15,0x10},
		{0x0b,0x00},
		{0x67,0x80},
		{0x68,0x80},
		{0xff,0xff}
	},//effect Normal
	{
		{0x15,0x10},
		{0x0b,0x20},
		{0x67,0x80},
		{0x68,0x80},
		{0xff,0xff}

	},//effect BLACKWHITE
	{
		{0x15,0x10},
		{0x0b,0x20},
		{0x67,0x60},
		{0x68,0xe0},
		{0xff,0xff}

	},// effect RED
	{
		{0x15,0x10},
		{0x0b,0x20},
		{0x67,0x60},
		{0x68,0x60},
		{0xff,0xff}

	},// effect GREEN
	{
		{0x15,0x10},
		{0x0b,0x20},
		{0x67,0xa0},
		{0x68,0x60},
		{0xff,0xff}

	},// effect  BLUE
	{
		{0x15,0x10},
		{0x0b,0x20},
		{0x67,0x50},
		{0x68,0x90},
		{0xff,0xff}

	},//effect ANTIQUE
	{
		{0x15,0x12},
		{0x0b,0x00},
		{0x67,0x00},
		{0x68,0x00},
		{0xff,0xff}

	},// effect NEGATIVE
	{
		{0x15,0x10},
		{0x0b,0x20},
		{0x67,0x60},
		{0x68,0x98},
		{0xff,0xff}

	},// effect  sepia
};


LOCAL uint32 bf30a2_set_image_effect(uint32 effect_type)
{

	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_image_effect_tab[effect_type];


	SCI_ASSERT(PNULL != sensor_reg_ptr);

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	SCI_TRACE_LOW("bf30a2_set_image_effect:effect_type=%d", effect_type);

	return 0;
}


LOCAL uint32 bf30a2_After_Snapshot(uint32 param)
{

    return 0;

}


LOCAL uint32 bf30a2_BeforeSnapshot(uint32 param)
{

	return 0;
}


LOCAL uint32 bf30a2_read_ev_value(uint32 value)
{
	return 0;
}


LOCAL uint32 bf30a2_write_ev_value(uint32 exposure_value)
{

	return 0;
}


LOCAL uint32 bf30a2_read_gain_value(uint32 value)
{


	return 0;
}


LOCAL uint32 bf30a2_write_gain_value(uint32 gain_value)
{


	return 0;
}


LOCAL uint32 bf30a2_read_gain_scale(uint32 value)
{
	return 0;

}


LOCAL uint32 bf30a2_set_frame_rate(uint32 param)

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
__align(4) const SENSOR_REG_T bf30a2_mode_tab[][3]=
{

	{{0xf0,0x8f}, {0x86,0xd8}, {0xff,0xff}},//cc
	{{0xf0,0x94}, {0x86,0xd8}, {0xff,0xff}},

};

LOCAL uint32 bf30a2_set_work_mode(uint32 mode)
{

    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bf30a2_mode_tab[mode];

    SCI_ASSERT(mode <= 1);
    SCI_ASSERT(PNULL != sensor_reg_ptr);
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        bf30a2_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("bf30a2_set_work_mode:mode=%d", mode);

	return 0;
}

LOCAL uint32 _bf30a2_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E		dvdd_val=g_bf30a2_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E		avdd_val=g_bf30a2_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E		iovdd_val=g_bf30a2_yuv_info.iovdd_val;
    BOOLEAN 				power_down=(BOOLEAN )g_bf30a2_yuv_info.power_down_level;
    BOOLEAN 				reset_level=g_bf30a2_yuv_info.reset_pulse_level;
    uint32 				    reset_width=g_bf30a2_yuv_info.reset_pulse_width;

    if(SCI_TRUE==power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);

        GPIO_SetSensorPwdn(!power_down);/*lint !e718*/

        // Open Mclk in default frequency
        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);

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

    SCI_TRACE_LOW("_bf30a2_Power_On(1:on,0:off):%d", power_on);

    return SCI_SUCCESS;
}

/*lint -restore */

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


