/******************************************************************************
 ** Copyright (c) 
 ** File Name:		sensor_NT99040.c 										  *
 ** Author: 		yangbin @hisense											  *
 ** DATE:			2010.07.07												  *
 ** Description:   This file contains driver for sensor NT99040
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
 

#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "sci_api.h"

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
#define NT99040_I2C_ADDR_W		0x42
#define NT99040_I2C_ADDR_R			0x43

 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 NT99040_set_ae_enable(uint32 enable);
LOCAL uint32 NT99040_set_hmirror_enable(uint32 enable);
LOCAL uint32 NT99040_set_vmirror_enable(uint32 enable);
LOCAL uint32 NT99040_set_preview_mode(uint32 preview_mode);
LOCAL uint32 NT99040_Identify(uint32 param);
LOCAL uint32 NT99040_BeforeSnapshot(uint32 param);
	
LOCAL uint32 NT99040_set_brightness(uint32 level);
LOCAL uint32 NT99040_set_contrast(uint32 level);
LOCAL uint32 NT99040_set_sharpness(uint32 level);
LOCAL uint32 NT99040_set_saturation(uint32 level);
LOCAL uint32 NT99040_set_image_effect(uint32 effect_type);
LOCAL uint32 NT99040_set_work_mode(uint32 mode);
LOCAL uint32 NT99040_chang_image_format(uint32 param);
LOCAL uint32 NT99040_check_image_format_support(uint32 param);
LOCAL uint32 NT99040_after_snapshot(uint32 param);

LOCAL uint32 set_NT99040_ev(uint32 level);
LOCAL uint32 set_NT99040_awb(uint32 mode);
LOCAL uint32 set_NT99040_anti_flicker(uint32 mode);
//LOCAL uint32 set_NT99040_video_mode(uint32 mode); 
LOCAL uint32 NT99040_zoom(uint32 level);

/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/
LOCAL uint32 s_preview_mode;
LOCAL uint32 s_image_effect = 0;


LOCAL uint32 work_mode = 0;


/*lint -save -e533 */
__align(4)  const SENSOR_REG_T NT99040_YUV_COMMON[]=
{
	{0x32F0, 0x00},

//Gamma 1
/*	{0x3270, 0x00},
	{0x3271, 0x12},
	{0x3272, 0x22},
	{0x3273, 0x32},
	{0x3274, 0x44},
	{0x3275, 0x55},
	{0x3276, 0x6e},
	{0x3277, 0x88},
	{0x3278, 0x9d},
	{0x3279, 0xb1},
	{0x327A, 0xd0},
	{0x327B, 0xe1},
	{0x327C, 0xee},
	{0x327D, 0xf7},
	{0x327E, 0xff},
*/
//Gamma 2
/*	{0x3270, 0x00},
	{0x3271, 0x04},
	{0x3272, 0x0c},
	{0x3273, 0x23},
	{0x3274, 0x3f},
	{0x3275, 0x62},
	{0x3276, 0x7d},
	{0x3277, 0x9b},
	{0x3278, 0xb5},
	{0x3279, 0xca},
	{0x327A, 0xe0},
	{0x327B, 0xed},
	{0x327C, 0xfa},
	{0x327D, 0xff},
	{0x327E, 0xff},
*/
//Gamma 3
	{0x3270, 0x00},
	{0x3271, 0x0a},
	{0x3272, 0x26},
	{0x3273, 0x49},
	{0x3274, 0x63},
	{0x3275, 0x7f},
	{0x3276, 0x9a},
	{0x3277, 0xb4},
	{0x3278, 0xcb}, 
	{0x3279, 0xe0},
	{0x327A, 0xed},
	{0x327B, 0xf5},
	{0x327C, 0xfa},
	{0x327D, 0xFd},
	{0x327E, 0xFF},
	
//Gamma 4
/*	{0x3270, 0x0A},
	{0x3271, 0x2a},
	{0x3272, 0x4c},
	{0x3273, 0x68},
	{0x3274, 0x82},
	{0x3275, 0x9b},
	{0x3276, 0xb4},
	{0x3277, 0xcf},
	{0x3278, 0xe0},
	{0x3279, 0xef},
	{0x327A, 0xf5},
	{0x327B, 0xf9},
	{0x327C, 0xfb},
	{0x327D, 0xFd},
	{0x327E, 0xFF},
*/	
//----------------Gamma end --------------------

//Lens Sading+++++++++++++++++
	{0x3210,0x32},
	{0x3211,0x24},
	{0x3212,0xfe},
	{0x3213,0x78},
	{0x3214,0x1d},
	{0x3215,0x78},
	{0x3216,0xff},
	{0x3217,0x3c},
	{0x3218,0x40},
	{0x3219,0x78},
	{0x321a,0xfe},
	{0x321b,0x08},
	{0x321c,0x25},
	{0x321d,0xe4},
	{0x321e,0xff},
	{0x321f,0x04},	              
	{0x3220,0x41},
	{0x3221,0x74},
	{0x3222,0xfe},
	{0x3223,0x08},
	{0x3224,0x25},
	{0x3225,0xa4},
	{0x3226,0xff},
	{0x3227,0x04},
	{0x3228,0x34},
	{0x3229,0x98},
	{0x322a,0xfe},
	{0x322b,0x68},
	{0x322c,0x1e},
	{0x322d,0x48},
	{0x322e,0xff},
	{0x322f,0x34},      
	{0x3238,0x00},
	{0x3239,0x00},
	{0x323a,0x00},
	{0x323b,0x28},
	{0x323c,0x00},
	{0x323d,0x00},
	{0x323e,0x34},
	{0x323f,0x00},
	{0x3240,0x00},
	{0x3241,0x38},
	{0x3242,0x00},
	{0x3243,0x00},
	{0x3244,0x2c},
//Lens Sading--------------------	  	
	              
	{0x32f6,0x0f},
//   Color correction +++++++++++++++++++++++	              
	{0x3300,0x78},    //5:0 Noise reduction     7:6 Edge Enhancement[9:8]
	{0x3301,0x80},    //7:0 Edge Enhancement[7:0]
//   Color correction ---------------------------

/*	{0x3320,0x28},
	{0x3323,0x32},
	{0x3324,0x00},
	{0x3331,0x04},
	{0x3332,0x10},
*/
	{0x3320,0x30},
	{0x3323,0x32},
	{0x3324,0x04},
	{0x3331,0x06},
	{0x3332,0x80},


	
//   Color correction +++++++++++++++++++++++	              
//color matrix1
/*	{0x3302,0x07},
	{0x3303,0xf8},
	{0x3304,0x00},
	{0x3305,0xd7},
	{0x3306,0x00},
	{0x3307,0x30},
	{0x3308,0x07},
	{0x3309,0xd9},
	{0x330a,0x07},
	{0x330b,0x58},
	{0x330c,0x00},
	{0x330d,0xce},
	{0x330e,0x01},
	{0x330f,0x0d},
	{0x3310,0x06},
	{0x3311,0xf9},
	{0x3312,0x07},
	{0x3313,0xf9},
*/
//color matrix2
	{0x3302,0x07},
	{0x3303,0xed},
	{0x3304,0x00},
	{0x3305,0xbc},
	{0x3306,0x00},
	{0x3307,0x55},
	{0x3308,0x07},
	{0x3309,0x92},
	{0x330a,0x07},
	{0x330b,0x3b},
	{0x330c,0x01},
	{0x330d,0x32},
	{0x330e,0x01},
	{0x330f,0x34},
	{0x3310,0x06},
	{0x3311,0xc7},
	{0x3312,0x00},
	{0x3313,0x03},
//   Color correction --------------------------	      

//AWB+++++++++++
	{0x329b,0x00},
	{0x32a1,0x01},
	{0x32a2,0x10},
	{0x32a3,0x01},
	{0x32a4,0xae},
	{0x32a5,0x01},
	{0x32a6,0x2d},
	{0x32a7,0x01},
	{0x32a8,0x7f},
//AWB--------------

// AE++++++++++++
	{0x32b0,0x00},
	{0x32b1,0x90},
	{0x32bf,0x52},
	{0x32c8,0x49},
	{0x32c9,0x3d},
	{0x32cd,0x06},
	{0x32d3,0x13},
	              
	{0x32bb,0x0b},
	{0x32bc,0x55},
	{0x32bd,0x04},
	{0x32be,0x04},
	              
	{0x32c1,0x29},
	{0x32c2,0x30},
	{0x32c3,0x18},
	{0x32c5,0x18},
//AE----------------
	              
	{0x3102,0x09},
	{0x3110,0x00},
	{0x3113,0x00},
	{0x3114,0x07},
	              
	{0x3200,0x1e},
	{0x3201,0x2f},
	              
	{0x303f,0x22},
	{0x3044,0x02},
	{0x3045,0xe8},
	{0x3046,0x02},
	{0x3047,0xe4},
	{0x3048,0x02},
	{0x3049,0xe4},
	{0x304a,0x02},
	{0x304b,0xe8},
	// OB ++++
	{0x3051,0x3f},
	{0x3052,0x10},
	{0x3053,0x3a},
	// OB ----
	{0x3057,0xb6},
	{0x3022,0x02},   // bit 1: mirro   bit 0:flip
	{0x3003,0x02},
	{0x3007,0x81},
	{0x300b,0x30},
	{0x300c,0x01},
	{0x300d,0xec},
	{0x3024,0x0a},
	              
	{0x33f8,0x01},
	              
	{0x306a,0x80},
	{0x306b,0x50},
	{0x306d,0x43},


};

//pclk = 36MHz @24MHz MCLK
//7.5fps
__align(4) SENSOR_REG_T NT99040_YUV_1600X1200[]=
{
	{0x3fff, 0xff},

};

__align(4) SENSOR_REG_T NT99040_YUV_1280X960[]=
{
	{0x3fff, 0xff},

};
//18MHz@24MHz MCLK
//15fps
__align(4) SENSOR_REG_T NT99040_YUV_640X480[]=
  //thomas 24M MCLK  input ,15fps ,18M PCLK 
{
	              
	{0x3021,0x06},
		
//WB set default +++++
       {0x3290, 0x01},
	{0x3291, 0x80},
	{0x3296, 0x01},
	{0x3297, 0x90},
//WB set default ----

	{0x32c1,0x29},
	{0x32c2,0x30},
	{0x32c5,0x22},
	{0x3012,0x09},
	{0x3013,0x30},
	{0x301d,0x10},
	{0x3060,0x01},  //active all
	
	{0x3201,0x3f},


};



__align(4) const SENSOR_REG_T NT99040_yuv422_mode[]=
{   
    {0x3fff, 0xff},  

};

LOCAL SENSOR_REG_TAB_INFO_T s_NT99040_resolution_Tab_YUV[]=
{	
	// COMMON INIT
	{ ADDR_AND_LEN_OF_ARRAY(NT99040_YUV_COMMON), 0, 0, 24, SENSOR_IMAGE_FORMAT_YUV422}, 
	
	// YUV422 PREVIEW 1 
	{ ADDR_AND_LEN_OF_ARRAY(NT99040_YUV_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},

	{ ADDR_AND_LEN_OF_ARRAY(NT99040_YUV_1280X960), 1280, 960,24, SENSOR_IMAGE_FORMAT_YUV422},
    
	{ ADDR_AND_LEN_OF_ARRAY(NT99040_YUV_1600X1200), 1600, 1200, 24,SENSOR_IMAGE_FORMAT_YUV422}, //yangbin
	{ PNULL,	0, 0,	0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0},
	{ PNULL,	0, 0,	0, 0, 0} 
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_NT99040_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    PNULL,
    PNULL,
    NT99040_Identify,

    PNULL,			// write register
    PNULL,			// read  register	
    PNULL,
    PNULL,

    // External
    NT99040_set_ae_enable,
    NT99040_set_hmirror_enable,
    NT99040_set_vmirror_enable,

    NT99040_set_brightness,
    NT99040_set_contrast,
    PNULL,//OV2655_set_sharpness,
    PNULL,//OV2655_set_saturation,

    NT99040_set_preview_mode,	
    NT99040_set_image_effect,

    NT99040_BeforeSnapshot,
    NT99040_after_snapshot,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_NT99040_awb,
    PNULL,
    PNULL,
    set_NT99040_ev,
    NT99040_check_image_format_support,
    NT99040_chang_image_format,
    PNULL,
    PNULL,
    PNULL,
    set_NT99040_anti_flicker,
    PNULL,

    PNULL,
};


/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_NT99040_yuv_info =
{
	NT99040_I2C_ADDR_W,				// salve i2c write address
	NT99040_I2C_ADDR_R, 				// salve i2c read address
	
	SENSOR_I2C_REG_16BIT,			// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit1: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_P,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
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
	0x3000, 0x04,						// supply two code to identify sensor.
	0x3001, 0x00,						// for Example: index = 0-> Device id, index = 1 -> version id											
											
	SENSOR_AVDD_2800MV,				// voltage of avdd	
	
	640,							// max width of source image
	480,							// max height of source image
	"NT99040",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,SENSOR_IMAGE_FORMAT_MAX
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T

	SENSOR_IMAGE_PATTERN_YUV422_UYVY,	// pattern of input image form sensor;			

	s_NT99040_resolution_Tab_YUV,	// point to resolution table information structure
	&s_NT99040_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,				// extend information about sensor
	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_UNUSED,//SENSOR_AVDD_1300MV,                      // dvdd
	0,                     // skip frame num before preview 
	0,                     // skip frame num before capture
	0,                     // deci frame num during preview;	
	0,                    // deci frame num during video preview;
	0,                    // threshold enable(only analog TV)	
	0,                    // threshold mode 0 fix mode 1 auto mode	
	0,                    // threshold start postion
	0,                     // threshold end postion
	0
	
};

/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
LOCAL uint32 NT99040_set_ae_enable(uint32 enable)
{

	return 0;
}
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99040_banding_flicker_tab[][2]=
{
    // 50HZ
    {
        {0x32c8, 0x49},{0xffff,0xff} 
    },    
    //60HZ
    {
        {0x32c9, 0x3d},{0xffff,0xff}       
    },
};   

LOCAL uint32 set_NT99040_anti_flicker(uint32 mode)
{//107 us
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;


    if(mode > 1)
    {
    	    return 0;
    }
    sensor_reg_ptr = (SENSOR_REG_T*)NT99040_banding_flicker_tab[mode];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr)|| (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    OS_TickDelay(100); 
    //SCI_TRACE_LOW:"SENSOR: set_NT99040_flicker: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_579_112_2_18_0_31_20_953,(uint8*)"d", mode);

    return 0;
}
/*
LOCAL uint32 set_NT99040_video_mode(uint32 mode)
{
    return 0;
}
*/
/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/

__align(4) const SENSOR_REG_T NT99040_awb_tab[][5]=

{
    //AUTO
    {
        {0xffff, 0xff},  
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff}        
    },    
    //OFFICE:
    {
        {0x3290, 0x01},
        {0x3291, 0x38},
        {0x3296, 0x01},
        {0x3297, 0x78},
        {0xffff, 0xff}
    },
        //U30
    {
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff},       
        {0xffff, 0xff},    
        {0xffff, 0xff},             
    },  
    //CWF
    {
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff},  
    },    
    //HOME
    {
        {0x3290, 0x01},
        {0x3291, 0x24},
        {0x3296, 0x01},
        {0x3297, 0x78},
        {0xffff, 0xff}
    },  
    
    //SUN:
    {
        {0x3290, 0x01},
        {0x3291, 0x38},
        {0x3296, 0x01},
        {0x3297, 0x68},
        {0xffff, 0xff}
    },
    //CLOUD:
    {
        {0x3290, 0x01},
        {0x3291, 0x48},
        {0x3296, 0x01},
        {0x3297, 0x58},
        {0xffff, 0xff}
    }
};

LOCAL uint32 set_NT99040_awb(uint32 mode)
{
	uint16 i;
	uint8 temp;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99040_awb_tab[mode];

	if(mode >= DCAMERA_WB_MODE_MAX)
		return SENSOR_OP_PARAM_ERR;

	temp = Sensor_ReadReg(0x3201);
	if(mode != DCAMERA_WB_MODE_AUTO)
		Sensor_WriteReg(0x3201, temp & (~0x10));
	else
		Sensor_WriteReg(0x3201, temp | 0x10);
	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0;(0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW:"SENSOR: set_awb_mode: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_678_112_2_18_0_31_21_954,(uint8*)"d", mode);

	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:yangbin
// Note:
//		
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99040_ev_tab[][2]=
{

	{{0x32F2, 0x50},{0xffff, 0xff}},
	{{0x32F2, 0x60},{0xffff, 0xff}},
	{{0x32F2, 0x70},{0xffff, 0xff}},
	{{0x32F2, 0x80},{0xffff, 0xff}},
	{{0x32F2, 0x90},{0xffff, 0xff}},
	{{0x32F2, 0xA0},{0xffff, 0xff}},
	{{0x32F2, 0xB0},{0xffff, 0xff}}	//for NT99040

	
};

LOCAL uint32 set_NT99040_ev(uint32 level)
{
#if	1
	uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99040_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    SCI_ASSERT(level < 7);

    for(i = 0; i < 1 /*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    OS_TickDelay(100); 

    //SCI_TRACE_LOW:"SENSOR: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_717_112_2_18_0_31_21_955,(uint8*)"d", level);
#endif
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
LOCAL uint32 NT99040_set_hmirror_enable(uint32 enable)
{

	uint16 reg04;

	reg04 = Sensor_ReadReg(0x3022);
	
	if (enable)
	{
		reg04 = (reg04 & (~(0x02)));//reg04 = (reg04 | 0x02);
		Sensor_WriteReg(0x3022, reg04);
	}
	else
	{
		reg04 = (reg04 | 0x02);//reg04 = (reg04 & (~(0x02)));
		Sensor_WriteReg(0x3022, reg04);
	}
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
LOCAL uint32 NT99040_set_vmirror_enable(uint32 enable)
{
	uint16 reg04;
	
	reg04 = Sensor_ReadReg(0x3022);
	
	if (enable)
	{
		reg04 = (reg04 | 0x01);
		Sensor_WriteReg(0x3022, reg04);
	}
	else
	{
		reg04 = (reg04 & (~(0x01)));
		Sensor_WriteReg(0x3022, reg04);
	}
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
LOCAL uint32 NT99040_set_preview_mode(uint32 preview_mode)
{
	//SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_778_112_2_18_0_31_21_956,(uint8*)"d", preview_mode);
	
	s_preview_mode = preview_mode;
	
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{
			NT99040_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			NT99040_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			NT99040_set_work_mode(0);
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
LOCAL uint32 NT99040_Identify(uint32 param)
{
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
#define CAPTURE_MAX_GAIN16 					128
#define NT99040_CAPTURE_MAXIMUM_SHUTTER		500


#define PV_PERIOD_PIXEL_NUMS 480
//#define PV_Dummy_Pixels  0


#define  FULL_PERIOD_PIXEL_NUMS 1922
#define  g_Capture_Dummy_Pixels 0
#define  FULL_EXPOSURE_LIMITATION 1234
#define g_Capture_Dummy_Lines 0
#define g_Capture_PCLK_Frequency 36

uint16 read_NT99040_gain(void)
{
    uint16  temp_reg;
   uint16 sensor_gain;
   
  temp_reg= (Sensor_ReadReg(0x301C) << 8) + Sensor_ReadReg(0x301D);  
   
   
    sensor_gain=(16+(temp_reg&0x0F));
   if(temp_reg&0x10)
      sensor_gain<<=1;
   if(temp_reg&0x20)
      sensor_gain<<=1;
      
   if(temp_reg&0x40)
      sensor_gain<<=1;
      
   if(temp_reg&0x80)
      sensor_gain<<=1;
      
   return sensor_gain;
}  /* read_OV2650_gain */


LOCAL void NT99040_CalculateExposureGain(SENSOR_MODE_E sensor_preview_mode, SENSOR_MODE_E sensor_snapshot_mode)

#if 1
{ 
  uint16 PV_Line_Width;
    uint16 Capture_Line_Width;
    uint16 Capture_Maximum_Shutter;
    uint16 Capture_Exposure;
    uint16 Capture_Gain16;
    uint16 Capture_Banding_Filter;
    uint32 Gain_Exposure=0;

  uint8   	Reg0x3000,Reg0x3002,Reg0x3003;
    uint32  	Shutter;
    uint16  	tmp1,tmp2;
    uint32 		Gain_Mul = 0;
	uint16 g_PV_Gain16 = 0;

    uint16	g_PV_Shutter = 0;
	uint16 g_PV_Extra_Lines = 0;
	uint16	g_PV_Dummy_Pixels = 0;
	uint16	g_Capture_Max_Gain16 = 8*16;

	uint16 g_Capture_Gain16 = 0;
       uint16 g_Capture_Extra_Lines = 0;  
    
        uint16 g_Capture_Shutter = 0;


    if(sensor_snapshot_mode < SENSOR_MODE_SNAPSHOT_ONE_FIRST) //less than 640X480
    {
         return ;
    }


   Sensor_WriteReg(0x3201, Sensor_ReadReg(0x3201) & (~0x30));
 

//Stop Preview
//Stop AE/AG
//Read back preview shutter
    Reg0x3002 = Sensor_ReadReg(0x3012);
    Reg0x3003 = Sensor_ReadReg(0x3013);

    
    //SCI_TRACE_LOW:"Reg0x3012=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_909_112_2_18_0_31_21_957,(uint8*)"d", Reg0x3002);
    //SCI_TRACE_LOW:"Reg0x3013=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_910_112_2_18_0_31_21_958,(uint8*)"d", Reg0x3003);

   
    tmp1=(uint16)Reg0x3002;
    tmp1 <<=8;
    tmp2=(uint16)Reg0x3003;
    Shutter = (uint16)(tmp1 |tmp2);

    Reg0x3000 = Sensor_ReadReg(0x301d);

    Sensor_SetMode(sensor_snapshot_mode);


	
    if(0 == work_mode) 
    {
		//Sensor_WriteReg(0x3011,  0x01);  //match with the colck change with night mode    
    }
    else
    {
	    //Sensor_WriteReg(0x3011,  0x03);  //match with the colck change with night mode    
		
    }
#if 1

    if(Reg0x3000 & 0x40) //4//8x~16x gain
    {
    		Reg0x3000 = Reg0x3000 & 0x3f;
		Shutter = Shutter * 2;	
    }
    else if(Reg0x3000 & 0x20) //4//4x~8x gain
    {
    		Reg0x3000 = Reg0x3000 & 0x1f;
		Shutter = Shutter * 2;	
    }
    else if(Reg0x3000 & 0x10)//4  //2x~4x gain
    {
    		Reg0x3000 = Reg0x3000 & 0x0f;
		Shutter = Shutter * 2;	
    }    

#endif
	
    //Shutter = Shutter *1190 *360/18/1922/12;

    if(Shutter > 1234)
    	{
//    		g_Capture_Extra_Lines = Shutter - 1234;
//		Shutter = 1234; 	
    	}		

    if(0 == Shutter)
    {
    	    Shutter = 1;
    }

   
    g_Capture_Shutter = Shutter;
    Sensor_WriteReg(0x3013,  g_Capture_Shutter&0xff);
    Sensor_WriteReg(0x3012, (g_Capture_Shutter >>8)&0xff);

//    Sensor_WriteReg(0x302e,  g_Capture_Extra_Lines&0xff);
//    Sensor_WriteReg(0x302d, (g_Capture_Extra_Lines >>8)&0xff);

    Sensor_WriteReg(0x301d, Reg0x3000);



	if(0 == work_mode) 
	{
	 	SCI_Sleep(100);
	}    
	else
	{
	   	SCI_SLEEP(200);
	}



}
#else
{
    uint16 PV_Line_Width;
    uint16 Capture_Line_Width;
    uint16 Capture_Maximum_Shutter;
    uint16 Capture_Exposure;
    uint16 Capture_Gain16;
    uint16 Capture_Banding_Filter;
    uint32 Gain_Exposure=0;

  uint8   	Reg0x3000,Reg0x3002,Reg0x3003;
    uint32  	Shutter;
    uint16  	tmp1,tmp2;
    uint32 		Gain_Mul = 0;
	uint16 g_PV_Gain16 = 0;

    uint16	g_PV_Shutter = 0;
	uint16 g_PV_Extra_Lines = 0;
	uint16	g_PV_Dummy_Pixels = 0;
	uint16	g_Capture_Max_Gain16 = 8*16;

	uint16 g_Capture_Gain16 = 0;
       uint16 g_Capture_Extra_Lines = 0;  
    
        uint16 g_Capture_Shutter = 0;


    if(sensor_snapshot_mode < SENSOR_MODE_SNAPSHOT_ONE_FIRST) //less than 640X480
    {
         return ;
    }


   Sensor_WriteReg(0x3013, 0xf0);
 

//Stop Preview
//Stop AE/AG
//Read back preview shutter
    Reg0x3002 = Sensor_ReadReg(0x3002);
    Reg0x3003 = Sensor_ReadReg(0x3003);

   
    tmp1=(uint16)Reg0x3002;
    tmp1 <<=8;
    tmp2=(uint16)Reg0x3003;
    Shutter = (uint16)(tmp1 |tmp2);

    Reg0x3000 = read_OV2650_gain();//Sensor_ReadReg(0x3000);

    g_PV_Shutter = Shutter;
   
//////////////////////////////////////////////////////////////////

    Sensor_SetMode(sensor_snapshot_mode);


    PV_Line_Width = PV_PERIOD_PIXEL_NUMS + g_PV_Dummy_Pixels;   

    Capture_Line_Width = FULL_PERIOD_PIXEL_NUMS + g_Capture_Dummy_Pixels;
    Capture_Maximum_Shutter = FULL_EXPOSURE_LIMITATION + g_Capture_Dummy_Lines;

    Capture_Banding_Filter =(g_Capture_PCLK_Frequency*100000/100/(2*Capture_Line_Width)+1)/2*2;

    /*   Gain_Exposure = g_PV_Gain16*(g_PV_Shutter+g_PV_Extra_Lines)*PV_Line_Width/g_Preview_PCLK_Frequency/Capture_Line_Width*g_Capture_PCLK_Frequency
    ;*/


    //SCI_TRACE_LOW:"Reg0x3000=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1058_112_2_18_0_31_21_959,(uint8*)"d", Reg0x3000);
    //SCI_TRACE_LOW:"g_PV_Shutter=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1059_112_2_18_0_31_21_960,(uint8*)"d", g_PV_Shutter);
    //SCI_TRACE_LOW:"g_PV_Extra_Lines=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1060_112_2_18_0_31_22_961,(uint8*)"d", g_PV_Extra_Lines);
    //SCI_TRACE_LOW:"PV_Line_Width=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1061_112_2_18_0_31_22_962,(uint8*)"d", PV_Line_Width);
    //SCI_TRACE_LOW:"Capture_Line_Width=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1062_112_2_18_0_31_22_963,(uint8*)"d", Capture_Line_Width);
   

#if 1
	g_PV_Gain16 = Reg0x3000 ;// read_OV2650_gain();
    
    Gain_Exposure = g_PV_Gain16;
    ///////////////////////
    Gain_Exposure *=(g_PV_Shutter+g_PV_Extra_Lines);
    Gain_Exposure *=PV_Line_Width;  //970
    //   Gain_Exposure /=g_Preview_PCLK_Frequency;
    Gain_Exposure= Gain_Exposure/Capture_Line_Width;//1940

    Gain_Exposure = Gain_Exposure  * 36 / 30 /2;
	
    //Gain_Exposure = Gain_Exposure*4/3;// for clock   //Wonder 20090622
#else




	g_PV_Gain16 = Reg0x3000 ;// read_OV2650_gain();
    
    Gain_Exposure = g_PV_Gain16;
    ///////////////////////
    Gain_Exposure *=(g_PV_Shutter);
    Gain_Exposure *=PV_Line_Width;  //970
    //   Gain_Exposure /=g_Preview_PCLK_Frequency;
    Gain_Exposure /=Capture_Line_Width;//1940
    //Gain_Exposure = Gain_Exposure*4/3;// for clock   //Wonder 20090622
#endif


    //SCI_TRACE_LOW:"PV_Line_Width=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1095_112_2_18_0_31_22_964,(uint8*)"d", PV_Line_Width);
    //SCI_TRACE_LOW:"Capture_Line_Width=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1096_112_2_18_0_31_22_965,(uint8*)"d", Capture_Line_Width);
    //SCI_TRACE_LOW:"Capture_Maximum_Shutter=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1097_112_2_18_0_31_22_966,(uint8*)"d", Capture_Maximum_Shutter);
    //SCI_TRACE_LOW:"Capture_Banding_Filter=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1098_112_2_18_0_31_22_967,(uint8*)"d", Capture_Banding_Filter);
    //SCI_TRACE_LOW:"g_PV_Gain16=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1099_112_2_18_0_31_22_968,(uint8*)"d", g_PV_Gain16);
    //SCI_TRACE_LOW:"Gain_Exposure=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1100_112_2_18_0_31_22_969,(uint8*)"d", Gain_Exposure);

    //redistribute gain and exposure
    if (Gain_Exposure < Capture_Banding_Filter * 16)     // Exposure < 1/100/120
    {

    	//SCI_TRACE_LOW:"if (Gain_Exposure < Capture_Banding_Filter * 16)"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1106_112_2_18_0_31_22_970,(uint8*)"");
    
       if(Gain_Exposure<16){//exposure line smaller than 2 lines and gain smaller than 0x08 
            Gain_Exposure = Gain_Exposure*4;     
            Capture_Exposure = 1;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2/4;

            //SCI_TRACE_LOW:"Gain_Exposure<16 Capture_Gain16=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1113_112_2_18_0_31_22_971,(uint8*)"d", Capture_Gain16);
        }
        else
        {
            Capture_Exposure = Gain_Exposure /16;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2;

            //SCI_TRACE_LOW:"Gain_Exposure<16 else---- Capture_Gain16=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1120_112_2_18_0_31_22_972,(uint8*)"d", Capture_Gain16);
        }
    }
    else 
    {

    		//SCI_TRACE_LOW:"if (Gain_Exposure < Capture_Banding_Filter * 16)  else--------"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1126_112_2_18_0_31_22_973,(uint8*)"");
    		
        if (Gain_Exposure > Capture_Maximum_Shutter * 16) // Exposure > Capture_Maximum_Shutter
        {
           
            Capture_Exposure = Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2;
            if (Capture_Gain16 > g_Capture_Max_Gain16) 
            {
                // gain reach maximum, insert extra line
                Capture_Exposure = Gain_Exposure*1.1/g_Capture_Max_Gain16;
                
                // Exposure = n/100/120
                Capture_Exposure = Capture_Exposure/Capture_Banding_Filter;
                Capture_Exposure =Capture_Exposure * Capture_Banding_Filter;
                Capture_Gain16 = ((Gain_Exposure *4)/ Capture_Exposure+3)/4;

		   //SCI_TRACE_LOW:"Gain_Exposure<16 Capture_Gain16=%d"
		   SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1143_112_2_18_0_31_22_974,(uint8*)"d", Capture_Gain16);
            }
        }
        else  // 1/100 < Exposure < Capture_Maximum_Shutter, Exposure = n/100/120
        {
            Capture_Exposure = Gain_Exposure/16/Capture_Banding_Filter;
            Capture_Exposure = Capture_Exposure * Capture_Banding_Filter;
            Capture_Gain16 = (Gain_Exposure*2 +1) / Capture_Exposure/2;

           //SCI_TRACE_LOW:"Gain_Exposure<16 else---- Capture_Gain16=%d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1152_112_2_18_0_31_22_975,(uint8*)"d", Capture_Gain16);
        }
    }
    
    g_Capture_Gain16 = Capture_Gain16;
    g_Capture_Extra_Lines = (Capture_Exposure > Capture_Maximum_Shutter)?
            (Capture_Exposure - Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter):0;     
    
    g_Capture_Shutter = Capture_Exposure - g_Capture_Extra_Lines;


    Sensor_WriteReg(0x3003,  g_Capture_Shutter&0xff);
    Sensor_WriteReg(0x3002, (g_Capture_Shutter >>8)&0xff);

    Sensor_WriteReg(0x302e,  g_Capture_Extra_Lines&0xff);
    Sensor_WriteReg(0x302d, (g_Capture_Extra_Lines >>8)&0xff);

    Sensor_WriteReg(0x3000, g_Capture_Gain16);



     SCI_Sleep(1000);
}
#endif

LOCAL uint32 NT99040_BeforeSnapshot(uint32 param)
{
    uint32  preview_mode = (param >= SENSOR_MODE_PREVIEW_TWO) ? \
                            SENSOR_MODE_PREVIEW_TWO:SENSOR_MODE_PREVIEW_ONE;
    
	NT99040_CalculateExposureGain(preview_mode, param);
	return 0;
}


/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99040_sharpness_tab[][2]=    //no use
{
	//weakest
	{{0x3306, 0x08},{0x3371, 0x00}},
	{{0x3306, 0x08},{0x3371, 0x01}},
	{{0x3306, 0x08},{0x3371, 0x02}},
	{{0x3306, 0x08},{0x3371, 0x03}},
	//strongest
	{{0x3306, 0x08},{0x3371, 0x04}}
};

LOCAL uint32 NT99040_set_sharpness(uint32 level)
{
#if	0
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99040_sharpness_tab[level];
	
	SCI_ASSERT(level <= 5 );
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; i < 2/*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/ ; i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//SCI_TRACE_LOW:"set_sharpness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1216_112_2_18_0_31_22_976,(uint8*)"d", level);
#endif	
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99040_saturation_tab[][4]=   //no use
{
	// level 0
	{ 
		{0x3301, 0xff},{0x3391, 0x02},{0x3394, 0x70},{0x3395, 0x70} 
	},
	//level 1
	{ 
		{0x3301, 0xff},{0x3391, 0x02},{0x3394, 0x50},{0x3395, 0x50} 
 
	},
	//level 2
	{ 
		{0x3301, 0xff},{0x3391, 0x02},{0x3394, 0x40},{0x3395, 0x40} 

	},
	//level 3
	{ 
		{0x3301, 0xff},{0x3391, 0x02},{0x3394, 0x30},{0x3395, 0x30}
 
	},
	//level 4
	{ 
		{0x3301, 0xff},{0x3391, 0x02},{0x3394, 0x20},{0x3395, 0x20}
 
	}
};
LOCAL uint32 NT99040_set_saturation(uint32 level)
{
#if	0
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99040_saturation_tab[level];
	
	SCI_ASSERT(level <= 8 );
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; i < 4/*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/ ; i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//SCI_TRACE_LOW:"set_saturation: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1266_112_2_18_0_31_22_977,(uint8*)"d", level);
#endif	
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99040_image_effect_tab[][4]=
{
	// effect normal
	{
		{0x32f1, 0x00},{0xffff, 0xff},{0xffff,0xff},{0xffff,0xff}

	},
	//effect BLACKWHITE
	{
		{0x32f1, 0x01},{0x32f6, 0x0f},{0xffff,0xff},{0xffff,0xff}

	},
	// effect RED
	{
		{0x3391, 0x18},{0x3396, 0x80},{0x3397, 0xc0},{0x32f6, 0x0f}

	},
		
	// effect GREEN
	{
		{0x32f1, 0x05},{0x32f4, 0x60},{0x32f5, 0x20},{0x32f6, 0x0f} 

	},
		
	// effect  BLUE
	{
		{0x32f1, 0x05},{0x32f4, 0xf0},{0x32f5, 0x80},{0x32f6, 0x0f}

	},
	//effect YELLOW
	{
		{0x32f1, 0x05},{0x32f4, 0xf0},{0x32f5, 0x20},{0x32f6, 0x0f}

	},
	// effect NEGATIVE
	{
		{0x32f1, 0x03},{0x32f6, 0x0f},{0xffff,0xff},{0xffff,0xff}

	},
	// effect CANVAS ANTIQUE
	{
		{0x32f1, 0x05},{0x32f4, 0xf0},{0x32f5, 0x20},{0x32f6, 0x0f}
	}
};
LOCAL uint32 NT99040_set_image_effect(uint32 effect_type)
{
	uint16 			i, reg_value = 0;
	SENSOR_REG_T* 	sensor_reg_ptr = (SENSOR_REG_T*)NT99040_image_effect_tab[effect_type];

	if(effect_type >= DCAMERA_EFFECT_MAX)	
		return SENSOR_OP_PARAM_ERR;
	
	s_image_effect = effect_type;
	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
/*	
		if(sensor_reg_ptr[i].reg_addr == 0x3391)
		{
			reg_value = Sensor_ReadReg(sensor_reg_ptr[i].reg_addr) & 0x03;
			reg_value |= (sensor_reg_ptr[i].reg_value & 0xF8);
			Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, reg_value);
		}
		else
*/		
		{
			Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
		}
	}
	//SCI_TRACE_LOW:"set_image_effect: effect_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1346_112_2_18_0_31_22_978,(uint8*)"d", effect_type);
	return 0;
}


/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/

__align(4) const SENSOR_REG_T NT99040_brightness_tab[][5]=
{
    {//level 1
        {0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x49},{0x339a, 0x30},{0xffff,0xff}	        
    },
    {//level 2
        {0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x49},{0x339a, 0x20},{0xffff,0xff}
    },
    {//level 3
        {0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x49},{0x339a, 0x10},{0xffff,0xff}
    },
    {//level 4
        {0x3301, 0xff},{0x3391, 0x00},{0x3390, 0x41},{0x339a, 0x00},{0xffff,0xff}
    },
    {//level 5
        {0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x41},{0x339a, 0x10},{0xffff,0xff}
    },
    {//level 6
        {0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x41},{0x339a, 0x20},{0xffff,0xff}
    },                             
    {//level 7
        {0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x41},{0x339a, 0x30},{0xffff,0xff}	
    }        
};

LOCAL uint32 NT99040_set_brightness(uint32 level)
{
	uint16 			i, reg_value = 0;
	SENSOR_REG_T* 	sensor_reg_ptr;

	sensor_reg_ptr = (SENSOR_REG_T*)NT99040_brightness_tab[level];

	if(level >= 7)
		return SENSOR_OP_PARAM_ERR;
	
	for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
/*	
		if(sensor_reg_ptr[i].reg_addr == 0x3391 ) 
		{
			reg_value = NT99040_image_effect_tab[s_image_effect][0].reg_value;
			reg_value |= sensor_reg_ptr[i].reg_value;
			Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, reg_value);
			//SCI_TRACE_LOW:"set_brightness: reg3391 = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1399_112_2_18_0_31_22_979,(uint8*)"d", reg_value);

		}
		
		else
*/
		{
			Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
		}
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW:"set_brightness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1410_112_2_18_0_31_22_980,(uint8*)"d", level);
	return 0;
}

/******************************************************************************/
// Description: set contrast
// Global resource dependence: 
// Author:
// Note:
//		level must smaller than 9
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99040_contrast_tab[][6]=
{
	{//level 1
		{0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x45},{0x3398, 0x14},{0x3399, 0x14},{0xffff,0xff} 
	},
	{//level 2
		{0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x45},{0x3398, 0x18},{0x3399, 0x18},{0xffff,0xff} 
	},
	{//level 3
		{0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x45},{0x3398, 0x1c},{0x3399, 0x1c},{0xffff,0xff} 
	},
	{//level 4
		{0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x45},{0x3398, 0x20},{0x3399, 0x20},{0xffff,0xff} 
	},
	{//level 5
		{0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x45},{0x3398, 0x24},{0x3399, 0x24},{0xffff,0xff} 
	},
	{//level 6
		{0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x45},{0x3398, 0x28},{0x3399, 0x28},{0xffff,0xff} 
	},
	{//level 7
		{0x3301, 0xff},{0x3391, 0x04},{0x3390, 0x45},{0x3398, 0x2c},{0x3399, 0x2c},{0xffff,0xff} 
	}
};
LOCAL uint32 NT99040_set_contrast(uint32 level)
{
	uint16 			i, reg_value = 0;
	SENSOR_REG_T* 	sensor_reg_ptr;
	sensor_reg_ptr = (SENSOR_REG_T*)NT99040_contrast_tab[level];
	
	if(level >= 7)
		return SENSOR_OP_PARAM_ERR;

	for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
/*	
		if(sensor_reg_ptr[i].reg_addr == 0x3391 ) 
		{
			reg_value = NT99040_image_effect_tab[s_image_effect][0].reg_value;
			reg_value |= sensor_reg_ptr[i].reg_value;
			Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, reg_value);
			//SCI_TRACE_LOW:"set_contrast: reg3391 = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1459_112_2_18_0_31_22_981,(uint8*)"d", reg_value);

		}
		else
*/		
		{
			Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
		}
	}
	OS_TickDelay(20);
	//SCI_TRACE_LOW:"set_contrast: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1469_112_2_18_0_31_22_982,(uint8*)"d", level);
	return 0;
}



/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;   1:night 
/******************************************************************************/


__align(4) const SENSOR_REG_T NT99040_work_mode_tab[][2]=
{
	
	{//normal fix 30fps
	     {0x301e, 0x00},    //   pclk 24M
            {0xffff, 0xff}
            
	},
	{//night 15fps
	     {0x301e, 0x01},    //    pclk 12M
            {0xffff, 0xff}
	}
};
/*lint -restore */

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;	 1:night 
/******************************************************************************/
LOCAL uint32 NT99040_set_work_mode(uint32 mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99040_work_mode_tab[mode];
	
	if(mode > 1)
		return SENSOR_OP_PARAM_ERR;


	work_mode = mode;

	for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	SCI_TRACE_LOW("Sensor I2C read: 0x301e = %04x", Sensor_ReadReg(0x301e));   // for NT99040 debug
	//SCI_TRACE_LOW:"set_work_mode: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NT99040_1517_112_2_18_0_31_23_983,(uint8*)"d", mode);
	return 0;
}

LOCAL uint32 NT99040_after_snapshot(uint32 param)
{
    //Sensor_SendRegTabToSensor(&s_OV3640_resolution_Tab_YUV[0]);
    NT99040_chang_image_format(SENSOR_IMAGE_FORMAT_YUV422);
    
    return SCI_SUCCESS;
}

LOCAL uint32 NT99040_chang_image_format(uint32 param)
{
    SENSOR_REG_TAB_INFO_T st_yuv422_reg_table_info = { ADDR_AND_LEN_OF_ARRAY(NT99040_YUV_COMMON),0,0,0,0};
    uint32 ret_val = SCI_ERROR;

    switch(param){
    case SENSOR_IMAGE_FORMAT_YUV422:
        ret_val = Sensor_SendRegTabToSensor(&st_yuv422_reg_table_info);
        break;

    case SENSOR_IMAGE_FORMAT_JPEG:
        ret_val = SCI_ERROR;//Sensor_SendRegTabToSensor(&st_jpeg_reg_table_info);
        break;

    default:
        SCI_PASSERT(0, ("NT99040 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param));
        break;
    }

    return ret_val;
}

LOCAL uint32 NT99040_check_image_format_support(uint32 param) 
{
    uint32 ret_val = SCI_ERROR;

    switch(param){
    case SENSOR_IMAGE_FORMAT_YUV422:
        ret_val = SCI_SUCCESS;
        break;

    case SENSOR_IMAGE_FORMAT_JPEG:
        ret_val = SCI_ERROR;//SCI_SUCCESS;
        break;

    default:
        SCI_PASSERT(0, ("NT99040 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param));
        break;
    }

    return ret_val;
}






