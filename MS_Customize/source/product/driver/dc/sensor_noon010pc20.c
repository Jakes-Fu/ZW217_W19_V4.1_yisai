/******************************************************************************
 ** Copyright (c) 
 ** File Name:		sensor_noon010pc20.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor noon010pc20. 
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
#define NOON010PC20_I2C_ADDR_W		0x60
#define NOON010PC20_I2C_ADDR_R		0x61

#define SENSOR_GAIN_SCALE		16

#define NOON010PC20_SIGNAL_PCLK_PHASE               0x00

#define NOON010PC20_SWITCH_PAGE_ID           0x03
#define NOON010PC20_PAGE0_ID                        0x00
#define NOON010PC20_PAGE1_ID                        0x01
#define NOON010PC20_PAGE2_ID                        0x02
#define NOON010PC20_PAGE3_ID                        0x03
#define NOON010PC20_PAGE4_ID                        0x04

 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_noon010pc20_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 noon010pc20_set_preview_mode(uint32 preview_mode);
LOCAL uint32 noon010pc20_Identify(uint32 param);
LOCAL uint32 noon010pc20_BeforeSnapshot(uint32 param);
LOCAL uint32 noon010pc20_After_Snapshot(uint32 param);

LOCAL uint32 noon010pc20_set_brightness(uint32 level);
LOCAL uint32 noon010pc20_set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
LOCAL uint32 noon010pc20_set_image_effect(uint32 effect_type);

LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);
LOCAL uint32 noon010pc20_set_work_mode(uint32 mode);
LOCAL uint32 noon010pc20_ev_compensation(uint32 ev_mode);
LOCAL uint32 noon010pc20_set_flicker_elimination(uint32 flicker_mode);
LOCAL uint32 noon010pc20_set_iso(uint32 iso_type);
LOCAL uint32 noon010pc20_get_ev(uint32 param);
LOCAL uint32 noon010pc20_set_wb_mode(uint32 wb_mode);

LOCAL uint32 s_preview_mode;




/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/


__align(4) const SENSOR_REG_T noon010pc20_YUV_640X480[]=
{
    //BEGIN
    //PAGE0
    {0x01, 0x51},
    {0x01, 0x53},
    {0x01, 0x51},
    {0x03, 0x03},
    {0x10, 0x0c},
    {0x03, 0x04},
    {0x10, 0x69},

    //Page0//
    {0x03, 0x00}, //Page 0
    {0x10, 0x00}, //18}, //Sub Sampling{1/2:176x144}
    {0x11, 0x94}, // 1frame skip when capture
    {0x12, 0x04}, //05}, //clock divider 1/2 {Pclk: 24MHz} {Max Frame rate Fps}
    {0x13, 0x00},
    {0x14, 0x01},
    {0x15, 0x00},
    {0x16, 0x01},

    {0x17, 0x00},
    {0x18, 0xf0},
        
    {0x19, 0x01},
    {0x1a, 0x40},
        
    {0x1b, 0x00},
    {0x1c, 0x6c},
     
    {0x1d, 0x01},
    {0x1e, 0xFF},
      
    {0x1f, 0x09},
    {0x20, 0xaf},
    {0x21, 0x02},
    {0x22, 0x0a},
    {0x23, 0x0a},
    {0x24, 0x0a},
    {0x25, 0x00},
    {0x26, 0x00},
    {0x27, 0x00},
    {0x28, 0x20},
    {0x2c, 0x08},
    {0x2e, 0x00},
    {0x40, 0x22},
    {0x41, 0x77},
    {0x42, 0x87},
    {0x43, 0x36},
    {0x4a, 0x00},
    {0x4b, 0x01},//05
    {0x72, 0x21},
    {0x75, 0x00},
    {0x80, 0x3c},
    {0x84, 0x02},
    {0x85, 0x18},
    {0x86, 0x02},
    {0x87, 0x18},
    {0x88, 0x02},
    {0x89, 0x1a},
    {0x8a, 0x02},
    {0x8b, 0x1a},
    {0xa1, 0x01},
    {0xa2, 0x04},
    {0xa3, 0x36},

    //Page1//
    {0x03, 0x01},
    {0x10, 0x01}, //[B:[1][0]] {0:VYUY,1:UYVY,2:YVYU,3:YUYV} need to be tunned
    {0x11, 0x03},
    {0x12, 0x30},
    {0x14, 0x00}, //20071227 0x80
    {0x19, 0x85},
    {0x1a, 0x00},
    {0x1d, 0x80},
    {0x1e, 0x80},
    {0x20, 0x0f},
    {0x21, 0xc4}, 
    {0x22, 0xc4},     

    //cmc matrix//
    {0x30, 0x0f},
    {0x36, 0x3f},

    {0x38, 0x51},
    {0x39, 0x0c},
    {0x3a, 0x05},
    {0x3b, 0x0a},
    {0x3c, 0x55},
    {0x3d, 0x0a},
    {0x3e, 0x01},
    {0x3f, 0x21},
    {0x40, 0x63},

    //gamma 20071227//
    {0x60, 0x01},
    {0x61, 0x10},
    {0x62, 0x14},
    {0x63, 0x1d},
    {0x64, 0x33},
    {0x65, 0x5c},
    {0x66, 0x7a},
    {0x67, 0x91},
    {0x68, 0xa4},
    {0x69, 0xb3},
    {0x6a, 0xc3},
    {0x6b, 0xce},
    {0x6c, 0xdf},
    {0x6d, 0xec},
    {0x6e, 0xf7},
    {0x6f, 0xff},

    //edge//
    {0x90, 0x0d},
    {0x91, 0xdd},
    {0x93, 0xa2}, //42}, //Control of edge gain
    {0x94, 0x20},
    {0x96, 0x41},
    {0x97, 0x75},
    {0x99, 0x30},

    //lpf//
    {0xa0, 0x1d},
    {0xa2, 0x03},
    {0xa3, 0x08},
    {0xa4, 0x08},
    {0xb1, 0x01},

    //dpc//
    {0xc0, 0x03},
    {0xc2, 0x44},
    {0xc3, 0x88},
    {0xc4, 0x30},
    {0xc5, 0x10},
    {0xc6, 0x08},

    //lens shading//
    {0xd0, 0x05},
    {0xd1, 0x80},
    {0xd2, 0x80},
    {0xd3, 0x70},
    {0xd4, 0x50},
    {0xd5, 0x52},
    {0xd6, 0x58},
    {0xd7, 0x50},

    //Page2//
    {0x03, 0x02},
    {0x50, 0x00},
    {0x51, 0x3e}, //3E}, //for 24MHz
    {0x52, 0x34}, //34}, //for 24MHz
    {0x53, 0x02},
    {0x54, 0x20},

    //Page3//
    {0x03, 0x03},
    {0x10, 0x0c},
    {0x11, 0x80},
    {0x12, 0x30},
    {0x13, 0xa5},
    {0x14, 0x30},
    {0x15, 0x51},
    {0x16, 0x23},
    {0x18, 0x7e},
    {0x1c, 0x38},
    {0x1d, 0x33},
    {0x1e, 0x20},
    {0x26, 0xc0},
    {0x2c, 0x40},
    {0x2d, 0x06},
    //setting for 24MHz//
    {0x33, 0x00}, // EXP Normal 30fps
    {0x34, 0x2b}, 
    {0x35, 0xf2}, 

    {0x36, 0x00},
    {0x37, 0x20},

    {0x38, 0x00},// EXP Max 3fps
    {0x39, 0xaf},
    {0x3a, 0xc8},
    {0x3b, 0x0e}, // EXP 100
    {0x3c, 0xa6},
    {0x3d, 0x0c}, // EXP 120
    {0x3e, 0x35},
    //setting for 24MHz//    
    {0x41, 0x00},
    {0x42, 0xc3},
    {0x43, 0x50},
    {0x47, 0x01},
    {0x48, 0x9a},
    {0x49, 0x45},
    {0x4a, 0x78},
    {0x4b, 0xd7},

    //pga gain//
    {0x50, 0x10},
    {0x51, 0x10},
    {0x52, 0x60},
    {0x53, 0x10},
    {0x54, 0x10},
    {0x55, 0x2d}, //31},
    {0x56, 0x80},

    {0x57, 0x60},
    {0x58, 0xc0},

    {0x59, 0x20},
    {0x5a, 0x1a},
    {0x5b, 0x18},
    {0x5c, 0x16},
    {0x5d, 0x15},
    {0x5e, 0x14},
    {0x5f, 0x13},
    {0x60, 0x12},
    {0x61, 0x14},
    {0x62, 0x28},
    {0x63, 0x54},

    //--------------------------Page4//
    {0x03, 0x04},
    {0x10, 0x69},
    {0x11, 0x34},
    {0x12, 0x00},
    {0x13, 0x03},
    {0x19, 0x55},
    {0x20, 0x13},
    {0x21, 0x88},
    {0x22, 0x88},
    {0x23, 0xd8},
    {0x24, 0x88},
    {0x25, 0x66},
    {0x28, 0x88},
    {0x29, 0x66},
    {0x2c, 0x09},
    {0x30, 0xd0},
    {0x31, 0xa1},
    {0x32, 0xa4},
    {0x40, 0x3c},
    {0x41, 0x20},
    {0x42, 0x20}, //52},
    {0x43, 0x48},
    {0x44, 0x21},
    {0x45, 0x56},
    {0x46, 0x22},
    {0x47, 0x46},
    {0x48, 0x38},
    {0x49, 0x2e},
    {0x4a, 0x1c},
    {0x4b, 0x08},
    {0x4d, 0x04},
    {0x4e, 0x60},

    {0x4f, 0x5e},
    {0x50, 0x5b},
    {0x51, 0x53},
    {0x52, 0x48},
    {0x53, 0x42},
    {0x54, 0x3c},
    {0x55, 0x28},
    {0x56, 0x22},
    {0x57, 0x20},
    {0x58, 0x1f},
    {0x59, 0x1e},
    {0x5a, 0x1c},
    {0x5b, 0x05},
    {0x60, 0x80},
    {0x61, 0x80},
    {0x64, 0x30},
    {0x65, 0x48},
    {0x66, 0x48},
    {0x67, 0x40},

    {0x03, 0x03},
    {0x10, 0x9c},
    {0x03, 0x04},
    {0x10, 0xe6},
    {0x01, 0x50},
        
   // {0xffff, 0xffff},    
};

LOCAL SENSOR_REG_TAB_INFO_T s_noon010pc20_resolution_Tab_YUV[]=
{
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(noon010pc20_YUV_640X480), 320, 240, 12, SENSOR_IMAGE_FORMAT_YUV422},
	
	// YUV422 PREVIEW 1	
	{PNULL, 0, 320, 240,12, SENSOR_IMAGE_FORMAT_YUV422},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0}
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_noon010pc20_ioctl_func_tab = 
{
// 1: Internal IOCTL function
	PNULL,      // use to reset sensor 
	PNULL,      // Power on/off sensor selected by input parameter(0:off,1:on) 
	PNULL,      // enter sleep
	PNULL,//noon010pc20_Identify,    // identify sensor: 0 -> successful ; others -> fail	
	PNULL,      // write register
	PNULL,      // read  register	
// Custom function
	PNULL,
	PNULL,
// 2: External IOCTL function
	PNULL,                   // enable auto exposure
	PNULL,          // enable horizontal mirror
	PNULL,          // enable vertical mirror 	
	noon010pc20_set_brightness,                  // set brightness 	0: auto; other: the appointed level
	noon010pc20_set_contrast,                    // set contrast     0: auto; other: the appointed level  
	PNULL,               // set sharpness    0: auto; other: the appointed level
	PNULL,                  // set saturation   0: auto; other: the appointed level		
	noon010pc20_set_preview_mode,        // set preview mode : 0: normal mode; 1: night mode; 2: sunny mode
	noon010pc20_set_image_effect,        // set image effect
	PNULL,          // do something before do snapshort
	noon010pc20_After_Snapshot,      // do something after do snapshort                           	
	PNULL,      // 1: open flash; 0: close falsh                                                                                                           
	PNULL,      // return AE value
	PNULL,      // input AE value
	PNULL,      // return GAIN value
	PNULL,      // input GAIN value
	PNULL,      // return GAIN scale (for ov9650, 16)
	PNULL,      // set sensor frame rate based on current clock
	PNULL,      // input 1: enable; input 0: disable
	PNULL,      //return value: return 0 -> focus ok, other value -> lose focus
	noon010pc20_set_wb_mode,      // set while balance mode
	PNULL,      // get snapshot skip frame num from customer, input SENSOR_MODE_E paramter
	noon010pc20_set_iso,      // set ISO level					 0: auto; other: the appointed level
	noon010pc20_ev_compensation,      // Set exposure compensation	 0: auto; other: the appointed level
	noon010pc20_get_ev,      // Get the current frame lum
	noon010pc20_set_flicker_elimination,      // Set banding flicker compensation	 0: 50Hz; 1: 60Hz 
	// CUSTOMER FUNCTION	                      
	PNULL,  	// function 3 for custumer to configure                      
	PNULL	// function 4 for custumer to configure 
};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_noon010pc20_yuv_info =
{
	NOON010PC20_I2C_ADDR_W,				// salve i2c write address
	NOON010PC20_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_P|\
	NOON010PC20_SIGNAL_PCLK_PHASE,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved	
									// bit5~7: ccir delay sel
											
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
        SENSOR_WB_MODE_AUTO|\
        SENSOR_WB_MODE_INCANDESCENCE|\
        SENSOR_WB_MODE_U30|\
        SENSOR_WB_MODE_CWF|\
        SENSOR_WB_MODE_FLUORESCENT|\
        SENSOR_WB_MODE_SUN|\
        SENSOR_WB_MODE_CLOUD,

	// bit[0:7]: count of step in brightness, contrast, sharpness, saturation	
	// bit[8:16]: ISO type
	// bit[17:23]: EV compenation level
	// bit[24:32]: banding flicker elimination
        0,
	
	SENSOR_LOW_PULSE_RESET,			// reset pulse level
	20,								// reset pulse width(ms)
	
	SENSOR_LOW_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	1,								// count of identify code
	0x04, 0x79,						// supply two code to identify sensor.
	0x0B, 0x04,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	640,							// max width of source image
	480,							// max height of source image
	"NOON010PC20",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_UYVY,	// pattern of input image form sensor;			

	s_noon010pc20_resolution_Tab_YUV,	// point to resolution table information structure
	&s_noon010pc20_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	
};
/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void noon010pc20_WriteReg( uint8  subaddr, uint8 data )
{
	
	#ifndef	_USE_DSP_I2C_
		//uint8 cmd[2];
		//cmd[0]	=	subaddr;
		//cmd[1]	=	data;
		
		//I2C_WriteCmdArr(NOON010PC20_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg( subaddr,  data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW:"SENSOR: NOON010PC20_WriteReg 0x%x=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_528_112_2_18_0_31_15_918,(uint8*)"dd", subaddr, data);

}

LOCAL uint8 noon010pc20_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
		//I2C_WriteCmdArr(NOON010PC20_I2C_ADDR_W, &subaddr, 2, SCI_TRUE);
		//I2C_ReadCmd(NOON010PC20_I2C_ADDR_R, &value, SCI_TRUE);
		value=Sensor_ReadReg(subaddr);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif
	//SCI_TRACE_LOW:"SENSOR: NOON010PC20_WriteReg 0x%x=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_543_112_2_18_0_31_15_919,(uint8*)"dd", subaddr, value);
	return value;
}


LOCAL uint32 noon010pc20_Identify(uint32 param)
{
    uint8 ret;
    uint8 reg[1] 	= {0x04};
    uint8 value[1] 	= {0x79};

    //SCI_TRACE_LOW:"NOON010PC20_Identify: it is NOON010PC20"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_554_112_2_18_0_31_15_920,(uint8*)"");

    noon010pc20_WriteReg(NOON010PC20_SWITCH_PAGE_ID, NOON010PC20_PAGE0_ID);    
    
    ret = noon010pc20_ReadReg(reg[0]);  

    if(value[0]!=ret)
    {
        //SCI_TRACE_LOW:"This is not NOON010PC20 sensor"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_562_112_2_18_0_31_15_921,(uint8*)"");
        return SCI_ERROR;
    }

    //SCI_TRACE_LOW:"NOON010PC20_Identify: it is NOON010PC20"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_566_112_2_18_0_31_15_922,(uint8*)"");

    return (uint32)SCI_SUCCESS;
    
}



LOCAL uint32 set_noon010pc20_ae_enable(uint32 enable)
{
	return 0;
}



LOCAL uint32 set_hmirror_enable(uint32 enable)
{
 

	//SCI_TRACE_LOW:"set_hmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_585_112_2_18_0_31_15_923,(uint8*)"d", enable);
		

	return 0;
}




LOCAL uint32 set_vmirror_enable(uint32 enable)
{

	//SCI_TRACE_LOW:"set_vmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_597_112_2_18_0_31_15_924,(uint8*)"d", enable);
	

	return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc20_brightness_tab[][2]=
{
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
};


LOCAL uint32 noon010pc20_set_brightness(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_brightness_tab[level];

	SCI_ASSERT(level < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		noon010pc20_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW:"set_brightness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_633_112_2_18_0_31_15_925,(uint8*)"d", level);
	
	return 0;
}

__align(4) const SENSOR_REG_T noon010pc20_contrast_tab[][2]=
{
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},	
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
};


LOCAL uint32 noon010pc20_set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_contrast_tab[level];

    SCI_ASSERT(level < 7 );
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        noon010pc20_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    OS_TickDelay(20);
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_665_112_2_18_0_31_15_926,(uint8*)"d", level);

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

LOCAL uint32 noon010pc20_set_preview_mode(uint32 preview_mode)
{
	//SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_693_112_2_18_0_31_15_927,(uint8*)"d", preview_mode);
	
	s_preview_mode = preview_mode;
	
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{
			noon010pc20_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			noon010pc20_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			noon010pc20_set_work_mode(0);
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
 __align(4) const SENSOR_REG_T noon010pc20_image_effect_tab[][10]=
{//Tim.zhu@20080602 for tim.zhu_cr116992	
    { // normal

        {0xff,0xff},
    },

    { //white black                           
   
        {0xff, 0xff},       
    },

    { // red

        {0xff, 0xff},
    },

    {   // green

        {0xff, 0xff},
    },

    { // blue
     
        {0xff, 0xff},
    },

    { // yellow

        {0xff, 0xff},
    },        

    {  // negative                             

        {0xff, 0xff},
    }, 

    { // antique

        {0xff, 0xff},       
    },

};		

LOCAL uint32 noon010pc20_set_image_effect(uint32 effect_type)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_image_effect_tab[effect_type];
	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		noon010pc20_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//SCI_TRACE_LOW:"set_image_effect: effect_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_787_112_2_18_0_31_15_928,(uint8*)"d", effect_type);
    
	return 0;
}
#ifdef PRODUCT_DRIVER_HIMALAYA
/******************************************************************************/
// Description: change the sensor setting to preview mode
// Global resource dependence: 
// Author: Tim.zhu
// Note:
//
/******************************************************************************/
LOCAL uint32 noon010pc20_After_Snapshot(uint32 param)
{
    
    return 0;
    
}

#else
LOCAL uint32 noon010pc20_After_Snapshot(uint32 param)
{

    return 0;   
}



LOCAL uint32 noon010pc20_BeforeSnapshot(uint32 param)
{

    
    return 0;
    
}
#endif

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
//		mode 0:normal;   1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc20_iso_tab[][3]=
{
	//IS0 atuto
	{
		{0xff, 0xff}
	},
	//IS0 low
	{
		{0xff, 0xff}
	},
	//IS0 mid
	{
		{0xff, 0xff}
	},
	//IS0 high
	{
		 {0xff, 0xff}
	},	
};

LOCAL uint32 noon010pc20_set_iso(uint32 iso_type)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_iso_tab[iso_type];

	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"noon010pc20_set_iso: iso_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_898_112_2_18_0_31_16_929,(uint8*)"d", iso_type);

	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		white blance 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc20_wb_mode_tab[][5]=
{

    {//auto
        {0xff, 0xff},
    },

    {//sunny

        {0xff, 0xff},
    },

    {//cloudy

        {0xff, 0xff},
    },

    {//office

        {0xff, 0xff},
    },

    {//home

        {0xff, 0xff},
    },

};

LOCAL uint32 noon010pc20_set_wb_mode(uint32 wb_mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_wb_mode_tab[wb_mode];
    if(wb_mode>4)
    {
        return 0;
    }
	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"OV2640_set_wb_mode: wb_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_953_112_2_18_0_31_16_930,(uint8*)"d", wb_mode);
    
	return 0; 
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;   1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc20_flicker_mode_tab[][5]=
{
	//svga 50Hz
	{
	    {0xff, 0xff}	
	},
	//svga 60Hz
	{
	    {0xff, 0xff}	
	}
};

LOCAL uint32 noon010pc20_set_flicker_elimination(uint32 flicker_mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_flicker_mode_tab[flicker_mode];

	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"noon010pc20_set_flicker_elimination_mode: flicker_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_987_112_2_18_0_31_16_931,(uint8*)"d", flicker_mode);
    
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		ev mode 0:-3 -2 -1 0 1 2 3 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc20_ev_compensation_tab[][3]=
{

	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}}    
};

LOCAL uint32 noon010pc20_ev_compensation(uint32 ev_mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_ev_compensation_tab[ev_mode];
	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"noon010pc20_ev_compensation: ev_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_1021_112_2_18_0_31_16_932,(uint8*)"d", ev_mode);
    
	return 0;
}

LOCAL uint32 noon010pc20_get_ev(uint32 param)
{

    return 0x00;
     
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;	 1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc20_mode_tab[][5]=
{
    {{0xff,0xff}}, // normal
    {{0xff,0xff}} // night
};

LOCAL uint32 noon010pc20_set_work_mode(uint32 mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc20_mode_tab[mode];

	SCI_ASSERT(mode <= 1);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"set_work_mode: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC20_1057_112_2_18_0_31_16_933,(uint8*)"d", mode);
	return 0;
}

