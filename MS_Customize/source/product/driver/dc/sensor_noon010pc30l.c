/******************************************************************************
 ** Copyright (c) 
 ** File Name:		sensor_noon010pc30l.c 										  *
 ** Author: 		chris.shen										  *
 ** DATE:			2010/11/25										  *
 ** Description:   This file contains driver for sensor noon010pc30l. 
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
#define NOON010PC30L_I2C_ADDR_W		0x60
#define NOON010PC30L_I2C_ADDR_R		0x61

#define SENSOR_GAIN_SCALE		16

#define NOON010PC30L_SIGNAL_PCLK_PHASE               0x00

#define NOON010PC30L_SWITCH_PAGE_ID           0x03 // modify this by different sensor chris.shen 20101126
#define NOON010PC30L_PAGE0_ID                        0x00
#define NOON010PC30L_PAGE1_ID                        0x01
#define NOON010PC30L_PAGE2_ID                        0x02
#define NOON010PC30L_PAGE3_ID                        0x03
#define NOON010PC30L_PAGE4_ID                        0x04

 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_noon010pc30l_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 noon010pc30l_set_preview_mode(uint32 preview_mode);
LOCAL uint32 noon010pc30l_Identify(uint32 param);
LOCAL uint32 noon010pc30l_BeforeSnapshot(uint32 param);
LOCAL uint32 noon010pc30l_After_Snapshot(uint32 param);

LOCAL uint32 noon010pc30l_set_brightness(uint32 level);
LOCAL uint32 noon010pc30l_set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
LOCAL uint32 noon010pc30l_set_image_effect(uint32 effect_type);

LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);
LOCAL uint32 noon010pc30l_set_work_mode(uint32 mode);
LOCAL uint32 noon010pc30l_ev_compensation(uint32 ev_mode);
LOCAL uint32 noon010pc30l_set_flicker_elimination(uint32 flicker_mode);
LOCAL uint32 noon010pc30l_set_iso(uint32 iso_type);
LOCAL uint32 noon010pc30l_get_ev(uint32 param);
LOCAL uint32 noon010pc30l_set_wb_mode(uint32 wb_mode);
LOCAL uint32 _noon010pc30l_Power_On(uint32 power_on);
LOCAL uint32 _noon010pc30l_Reset(uint32 param);
LOCAL uint32 noon010pc30l_set_meter_mode(uint32 param);
    
LOCAL uint32 s_preview_mode;




/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/

// init seq 
__align(4) const SENSOR_REG_T noon010pc30l_YUV_320X240[]=
{
    /*==========================================================*/
    /*   CIF CAMERA TUNING                                                    */
    /*   ¨ù©ød ©ø?¡ì¢¯?                                                                    */
    /*   MCLK 6Mhz, Div 1, PCLK 6Mhz                                       */
    /*   VSync(HIGH), PCLK Data Catch (Falling Edge)                */
    /*   20090416 : fixed 15fps                                                 */
    /*==========================================================*/
    /*==================================================*/
    /* 0	Initial Setting                             */
    /*==================================================*/
    {0x03, 0x00},	
    {0x01, 0xf1},
    {0x01, 0xf3},
    {0x01, 0xf1},
    {0x03, 0x03},
    {0x10, 0x1c},
    {0x03, 0x04},
    {0x10, 0x69},

    {0x03, 0x00},
    {0x10, 0x00},
    {0x11, 0x95},
    {0x12, 0x24},
    {0x13, 0x00},
    {0x14, 0x02},
    {0x15, 0x00},
    {0x16, 0x06},
    {0x17, 0x01},
    {0x18, 0x20},
    {0x19, 0x01},
    {0x1a, 0x60},
    {0x1b, 0x00},
    {0x1c, 0x7C}, 
    {0x1d, 0x00},
    {0x1e, 0x14},

    {0x1f, 0x09},
    {0x20, 0xaf},
    {0x21, 0xc2},
    {0x22, 0x04},
    {0x23, 0x02},
    {0x24, 0x02},
    {0x25, 0x01},
    {0x26, 0x00},
    {0x27, 0x01},
    {0x28, 0x20},
    {0x2c, 0x08},
    {0x2e, 0xb7},
    {0x2f, 0x00},

    {0x40, 0xbb},
    {0x41, 0x77},
    {0x42, 0x87},
    {0x43, 0x36},
    {0x4a, 0x00},
    {0x4b, 0x05},
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

    {0x91, 0x08},
    {0x92, 0x14},
    {0x93, 0x08},
    {0x94, 0x14},
    {0x95, 0x33},
    {0x96, 0x47},
    {0x97, 0x33},
    {0x98, 0x47},
    {0x99, 0x18},
    {0x9a, 0x2c},
    {0x9b, 0x4a},
    {0x9c, 0x5e},
    {0xc4, 0x02},

    {0xa1, 0x01},
    {0xa2, 0x04},
    {0xa3, 0x36},

    {0x03, 0x01},
    {0x10, 0x01},
    {0x11, 0x03},
    {0x12, 0x30},
    {0x14, 0x00}, 
    {0x19, 0x81},
    {0x1a, 0x29},
    {0x1d, 0x80},
    {0x1e, 0x80},
    {0x20, 0x19},
    {0x21, 0x90},
    {0x22, 0x80},

    {0x30, 0x0f},
    {0x33, 0x3c},
    {0x35, 0x2c},
    {0x36, 0x3f},

    {0x38, 0x79},
    {0x39, 0x39},
    {0x3a, 0x00},
    {0x3b, 0x11},
    {0x3c, 0x65},
    {0x3d, 0x14},
    {0x3e, 0x01},
    {0x3f, 0x33},
    {0x40, 0x74},

    {0x41, 0x00},
    {0x42, 0x00},
    {0x43, 0x00},
    {0x44, 0x8b},
    {0x45, 0x07},
    {0x46, 0x04},
    {0x47, 0x9c},
    {0x48, 0x89},
    {0x49, 0x25},

    {0x50, 0x1D},
    {0x51, 0x80},
    {0x52, 0x44},
    {0x53, 0x24},

    //Tuning AWB //
    //Gamma_Registe
    {0x03,0x01},
    {0x61,0x06},
    {0x62,0x15},
    {0x63,0x28},
    {0x64,0x41},
    {0x65,0x60},
    {0x66,0x7a},
    {0x67,0x90},
    {0x68,0xa3},
    {0x69,0xaf},
    {0x6a,0xbd},
    {0x6b,0xca},
    {0x6c,0xdd},
    {0x6d,0xec},
    {0x6e,0xf7},
    {0x6f,0xff},

    {0x90, 0x19},
    {0x91, 0x51},
    {0x93, 0x64}, 
    {0x95, 0x40}, 
    {0x96, 0x01}, 
    {0x99, 0x20},

    {0xa0, 0x11},
    {0xa1, 0x13}, 
    {0xa2, 0x1f},
    {0xa3, 0x20},
    {0xa4, 0x06},
    {0xb1, 0x24},
    {0xb2, 0x83}, 
    {0xb3, 0x53}, 
    {0xb4, 0xa3},
    {0xb5, 0x05}, 

    {0xc0, 0x1D}, 
    {0xc2, 0x44},
    {0xc3, 0x88},
    {0xc4, 0x30},
    {0xc5, 0x10},
    {0xc6, 0x08},

    {0xd0, 0x05},
    {0xd1, 0x80},
    {0xd2, 0x80},
    {0xd3, 0x6a},
    {0xd4, 0x4d},
    {0xd5, 0x58},
    {0xd6, 0x58},
    {0xd7, 0x50},

    {0xf8, 0x01},
    {0xf9, 0x00},
    {0xfa, 0xaa},
    {0xfb, 0xcc},
    {0xfc, 0xf7},

    {0x03, 0x02},
    {0x50, 0x00},
    {0x51, 0x1e}, 
    {0x52, 0x19},
    {0x53, 0x02},
    {0x54, 0x20},

    {0x03, 0x03},
    {0x10, 0x9c},
    {0x11, 0x81},
    {0x12, 0x30},
    {0x13, 0xa5},
    {0x14, 0x3a},
    {0x15, 0x51},
    {0x16, 0x33},
    {0x18, 0x7e},
    {0x1c, 0x3e}, 
    {0x1d, 0x34},
    {0x1e, 0x30}, 
    {0x26, 0x95}, 
    {0x2c, 0x00}, 
    {0x2d, 0x02},

    {0x33, 0x00}, 
    {0x34, 0x2b}, 
    {0x35, 0xf2}, 
    {0x36, 0x00},
    {0x37, 0x20},
    {0x38, 0x00},
    {0x39, 0x49},
    {0x3a, 0x3e},
    {0x3b, 0x0e},
    {0x3c, 0xa6},
    {0x3d, 0x0c}, 
    {0x3e, 0x35},

    {0x41, 0x00},
    {0x42, 0x5c},
    {0x43, 0xc6},

    {0x50, 0x24},
    {0x51, 0x14},
    {0x52, 0xff},
    {0x53, 0x29},
    {0x54, 0x24},
    {0x55, 0x59}, 
    {0x57, 0x90}, 
    {0x58, 0xc8}, 

    {0x59, 0x3e},
    {0x5a, 0x35},
    {0x5b, 0x31},
    {0x5c, 0x2e},
    {0x5d, 0x2c},
    {0x5e, 0x2b},
    {0x5f, 0x2a},
    {0x60, 0x29},
    {0x61, 0x14},

    {0x70, 0x58},
    {0x71, 0x55},
    {0x72, 0x45},
    {0x73, 0x88},
    {0x78, 0xf0}, 
    {0x79, 0x88},

    {0x03, 0x04},
    {0x10, 0x78}, 
    {0x11, 0x2E}, 
    {0x12, 0x20}, 
    {0x13, 0x85},
    {0x19, 0x55},
    {0x20, 0x11},
    {0x21, 0x88},
    {0x22, 0x88},
    {0x23, 0xf3},
    {0x24, 0x66}, 
    {0x25, 0x33}, 
    {0x28, 0xaa}, 
    {0x29, 0x44}, 
    {0x2c, 0x09}, 
    {0x30, 0xa2}, 
    {0x31, 0xa1},
    {0x32, 0xa4},
    {0x40, 0x29},
    {0x41, 0x20},
    {0x42, 0x3e},

    {0x43, 0x50},
    {0x44, 0x22},
    {0x45, 0x55},
    {0x46, 0x21},

    {0x47, 0x45},
    {0x48, 0x30},
    {0x49, 0x40},
    {0x4a, 0x33},

    {0x4b, 0x0a},
    {0x4d, 0x24},
    {0x4e, 0x53},

    {0x4f, 0x52}, 
    {0x50, 0x4f},
    {0x51, 0x4c}, 
    {0x52, 0x49}, 
    {0x53, 0x45}, 
    {0x54, 0x42}, 
    {0x55, 0x3d}, 
    {0x56, 0x26}, 
    {0x57, 0x20}, 
    {0x58, 0x1b}, 
    {0x59, 0x18}, 
    {0x5a, 0x16}, 

    {0x5b, 0x07},

    {0x60, 0x80}, 
    {0x61, 0x80}, 
    {0x64, 0x30},
    {0x65, 0x48},

    {0x03, 0x03},
    {0x10, 0x9c},
    {0x03, 0x04},
    {0x10, 0xf6},
    {0x01, 0xf0},
    {0xff, 0xff},    
};

__align(4) const SENSOR_REG_T noon010pc30l_cameramode_tab[]=
{

    {0xff, 0xff},    
};

__align(4) const SENSOR_REG_T noon010pc30l_camcordermode_tab[]=
{
    {0xff, 0xff},    

};


LOCAL SENSOR_REG_TAB_INFO_T s_noon010pc30l_resolution_Tab_YUV[]=
{
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(noon010pc30l_YUV_320X240), 320, 240, SENSOR_MCLK_12M, SENSOR_IMAGE_FORMAT_YUV422},
	
	// YUV422 PREVIEW 1	
	{ADDR_AND_LEN_OF_ARRAY(noon010pc30l_cameramode_tab), 320, 240,SENSOR_MCLK_12M, SENSOR_IMAGE_FORMAT_YUV422},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0}
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_noon010pc30l_ioctl_func_tab = 
{
// 1: Internal IOCTL function
	_noon010pc30l_Reset,//PNULL,      // use to reset sensor 
	_noon010pc30l_Power_On,      // Power on/off sensor selected by input parameter(0:off,1:on) 
	PNULL,      // enter sleep
	noon010pc30l_Identify,    // identify sensor: 0 -> successful ; others -> fail	
	PNULL,      // write register
	PNULL,      // read  register	
// Custom function
	PNULL,
	PNULL,
// 2: External IOCTL function
	PNULL,                   // enable auto exposure
	PNULL,          // enable horizontal mirror
	PNULL,          // enable vertical mirror 	
	noon010pc30l_set_brightness,                  // set brightness 	0: auto; other: the appointed level
	PNULL,//noon010pc30l_set_contrast,                    // set contrast     0: auto; other: the appointed level  
	PNULL,               // set sharpness    0: auto; other: the appointed level
	PNULL,                  // set saturation   0: auto; other: the appointed level		
	noon010pc30l_set_preview_mode,        // set preview mode : 0: normal mode; 1: night mode; 2: sunny mode
	noon010pc30l_set_image_effect,        // set image effect
	PNULL,          // do something before do snapshort
	noon010pc30l_After_Snapshot,      // do something after do snapshort                           	
	PNULL,      // 1: open flash; 0: close falsh                                                                                                           
	PNULL,      // return AE value
	PNULL,      // input AE value
	PNULL,      // return GAIN value
	PNULL,      // input GAIN value
	PNULL,      // return GAIN scale (for ov9650, 16)
	PNULL,      // set sensor frame rate based on current clock
	PNULL,      // input 1: enable; input 0: disable
	PNULL,      //return value: return 0 -> focus ok, other value -> lose focus
	noon010pc30l_set_wb_mode,      // set while balance mode
	PNULL,      // get snapshot skip frame num from customer, input SENSOR_MODE_E paramter
	PNULL,//noon010pc30l_set_iso,      // set ISO level					 0: auto; other: the appointed level
	noon010pc30l_ev_compensation,      // Set exposure compensation	 0: auto; other: the appointed level
	noon010pc30l_get_ev,      // check image format
	PNULL,    //change image format
	PNULL,    //zoom
	PNULL,    // func_3,
	PNULL,    // func_4,
	PNULL,//noon010pc30l_set_flicker_elimination,      // Set banding flicker compensation	 0: 50Hz; 1: 60Hz 
	PNULL,  	// video mode                    
	PNULL,	// piuk up jpeg stream
//	noon010pc30l_set_meter_mode
};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_noon010pc30l_yuv_info =
{
	NOON010PC30L_I2C_ADDR_W,				// salve i2c write address
	NOON010PC30L_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_P|\
	SENSOR_HW_SIGNAL_HSYNC_P|\
	NOON010PC30L_SIGNAL_PCLK_PHASE,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved	
									// bit5~7: ccir delay sel
											
	// preview mode
	SENSOR_ENVIROMENT_NORMAL|\
	SENSOR_ENVIROMENT_NIGHT,
			
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
	
	0,								// count of identify code
	0x04, 0x79,						// supply two code to identify sensor.
	0x0B, 0x04,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	320,							// max width of source image
	240,							// max height of source image
	"NOON010PC30L",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_UYVY,	// pattern of input image form sensor;			

	s_noon010pc30l_resolution_Tab_YUV,	// point to resolution table information structure
	&s_noon010pc30l_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_1800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	4,                     // skip frame num before preview 
	1,                      // skip frame num before capture
	0,                      // deci frame num during preview	
	2,                       // deci frame num during video preview
	0,
	0,
	0,
	0,
	0	
};

LOCAL uint32 _noon010pc30l_Reset(uint32 param)
{
    return (uint32)SCI_SUCCESS;    
}

LOCAL uint32 _noon010pc30l_Power_On(uint32 power_on)
{
    //BOOLEAN 				power_down;		
    SENSOR_AVDD_VAL_E		dvdd_val;
    SENSOR_AVDD_VAL_E		avdd_val;
    SENSOR_AVDD_VAL_E		iovdd_val;    

    //power_down = (BOOLEAN)g_noon010pc30l_yuv_info.power_down_level;
    dvdd_val   = g_noon010pc30l_yuv_info.dvdd_val;
    avdd_val   = g_noon010pc30l_yuv_info.avdd_val;
    iovdd_val   = g_noon010pc30l_yuv_info.iovdd_val;
	
	//SCI_TRACE_LOW:"SENSOR: _noon010pc30l_Power_On  power_on =0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_596_112_2_18_0_31_17_934,(uint8*)"d", power_on);
	
	if(power_on)
        {			
////////////////////////////////////////////////////			
            GPIO_SetFrontSensorPwdn(!power_on);
	    GPIO_SetSensorPower(!power_on);
	    GPIO_SetFrontSensorReset(!power_on);
            // Open power
            Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);

            
	    GPIO_SetSensorPower(power_on);
	    OS_TickDelay(5); 
	    GPIO_SetFrontSensorPwdn(power_on);		
            // Open Mclk in default frequency
            Sensor_SetMCLK(SENSOR_MCLK_12M);    
            // Reset sensor
            OS_TickDelay(10); 

	    //GPIO_SetMainSensorReset(power_on);		
	    GPIO_SetFrontSensorReset(power_on);
        }
        else
        {
            GPIO_SetFrontSensorReset(0);
            //GPIO_SetMainSensorReset(0);
	    OS_TickDelay(10); 
	    Sensor_SetMCLK(SENSOR_DISABLE_MCLK);				

            // Power down sensor and maybe close DVDD, DOVDD
            GPIO_SetFrontSensorPwdn(0);
	    GPIO_SetSensorPower(0);
		
            Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);

            // Close Mclk
           	
        }
}
/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void noon010pc30l_WriteReg( uint8  subaddr, uint8 data )
{
	
	#ifndef	_USE_DSP_I2C_
		//uint8 cmd[2];
		//cmd[0]	=	subaddr;
		//cmd[1]	=	data;
		
		//I2C_WriteCmdArr(NOON010PC30L_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg( subaddr,  data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW:"SENSOR: NOON010PC30L_WriteReg 0x%x=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_652_112_2_18_0_31_18_935,(uint8*)"dd", subaddr, data);

}

LOCAL uint8 noon010pc30l_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
		//I2C_WriteCmdArr(NOON010PC30L_I2C_ADDR_W, &subaddr, 2, SCI_TRUE);
		//I2C_ReadCmd(NOON010PC30L_I2C_ADDR_R, &value, SCI_TRUE);
		value=Sensor_ReadReg(subaddr);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif
	//SCI_TRACE_LOW:"SENSOR: noon010pc30l_ReadReg 0x%x=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_667_112_2_18_0_31_18_936,(uint8*)"dd", subaddr, value);
	return value;
}


LOCAL uint32 noon010pc30l_Identify(uint32 param) 
{
    uint8 ret = 0;
    uint8 reg[1] 	= {0x04};
    uint8 value[1] 	= {0x86};
    // bst chris.shen  read the ASCI version 
    //return (uint32)SCI_SUCCESS; // bst chris.shen temp add for debug 20101125
    
    //SCI_TRACE_LOW:"NOON010PC30L_Identify: it is NOON010PC30L ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_680_112_2_18_0_31_18_937,(uint8*)"d", ret);

    noon010pc30l_WriteReg(NOON010PC30L_SWITCH_PAGE_ID, NOON010PC30L_PAGE0_ID);    
    
    ret = noon010pc30l_ReadReg(reg[0]);  
    //SCI_TRACE_LOW:"NOON010PC30L_Identify: it is NOON010PC30L 0x%x after read reg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_685_112_2_18_0_31_18_938,(uint8*)"d", ret);
#if 0
    if(value[0]!=ret)
    {
        //SCI_TRACE_LOW:"This is not NOON010PC30L sensor"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_689_112_2_18_0_31_18_939,(uint8*)"");
        return SCI_ERROR;
    }

    //SCI_TRACE_LOW:"NOON010PC30L_Identify: it is NOON010PC30L"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_693_112_2_18_0_31_18_940,(uint8*)"");
#endif
return (uint32)SCI_SUCCESS;
    
}



LOCAL uint32 set_noon010pc30l_ae_enable(uint32 enable)
{
	return 0;
}



LOCAL uint32 set_hmirror_enable(uint32 enable)
{
 

	//SCI_TRACE_LOW:"set_hmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_712_112_2_18_0_31_18_941,(uint8*)"d", enable);
		

	return 0;
}




LOCAL uint32 set_vmirror_enable(uint32 enable)
{

	//SCI_TRACE_LOW:"set_vmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_724_112_2_18_0_31_18_942,(uint8*)"d", enable);
	

	return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc30l_brightness_tab[][5]=
{
	{		

		{0xff, 0xff}
	},
	{		

		{0xff, 0xff}
	},
	{

		{0xff, 0xff}
	},
	
	{//level 4

		{0xff, 0xff}
	},
	
	{

		{0xff, 0xff}		
	},
	
	{

		{0xff, 0xff}		
	},
	
	{

		{0xff, 0xff}		
	},
};


LOCAL uint32 noon010pc30l_set_brightness(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_brightness_tab[level];

	SCI_ASSERT(level < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		noon010pc30l_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW:"set_brightness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_785_112_2_18_0_31_18_943,(uint8*)"d", level);
	
	return 0;
}

__align(4) const SENSOR_REG_T noon010pc30l_contrast_tab[][2]=
{
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},	
	{{0xff,0xff}},
	{{0xff,0xff}},
	{{0xff,0xff}},
};


LOCAL uint32 noon010pc30l_set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_contrast_tab[level];

    SCI_ASSERT(level < 7 );
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        noon010pc30l_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    OS_TickDelay(20);
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_817_112_2_18_0_31_18_944,(uint8*)"d", level);

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

LOCAL uint32 noon010pc30l_set_preview_mode(uint32 preview_mode)
{
	//SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_845_112_2_18_0_31_18_945,(uint8*)"d", preview_mode);
	
	s_preview_mode = preview_mode;
	
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{
			noon010pc30l_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			noon010pc30l_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			noon010pc30l_set_work_mode(0);
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
 __align(4) const SENSOR_REG_T noon010pc30l_image_effect_tab[][10]=
{//Tim.zhu@20080602 for tim.zhu_cr116992	
    { // normal
{0x03, 0x01},
{0x11, 0x03},
{0x12, 0x30},

        {0xff,0xff},
    },

    { //white black                           
    {0x03, 0x01},
{0x11, 0x03},
{0x12, 0x33},
{0x1d, 0x70},
{0x1e, 0x98},
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
{0x03, 0x01},
{0x11, 0x03},
{0x12, 0x33},
{0x1d, 0x80},
{0x1e, 0x80},
        {0xff, 0xff},
    },        

    {  // negative                             
{0x03, 0x01},
{0x11, 0x03},
{0x12, 0x38},

        {0xff, 0xff},
    }, 

    { // antique
{0x03, 0x01},
{0x11, 0x03},
{0x12, 0x33},
{0x13, 0x00},
{0x1d, 0xb0},
{0x1e, 0x40},
{0xff, 0xff},       
    },

};		

LOCAL uint32 noon010pc30l_set_image_effect(uint32 effect_type)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_image_effect_tab[effect_type];
	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		noon010pc30l_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//SCI_TRACE_LOW:"set_image_effect: effect_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_958_112_2_18_0_31_18_946,(uint8*)"d", effect_type);
    
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
LOCAL uint32 noon010pc30l_After_Snapshot(uint32 param)
{
    
    return 0;
    
}

#else
LOCAL uint32 noon010pc30l_After_Snapshot(uint32 param)
{

    return 0;   
}



LOCAL uint32 noon010pc30l_BeforeSnapshot(uint32 param)
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
__align(4) const SENSOR_REG_T noon010pc30l_iso_tab[][3]=
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

LOCAL uint32 noon010pc30l_set_iso(uint32 iso_type)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_iso_tab[iso_type];

	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"noon010pc30l_set_iso: iso_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_1069_112_2_18_0_31_18_947,(uint8*)"d", iso_type);

	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		white blance 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc30l_wb_mode_tab[][16]=
{

    {//auto
{0x03, 0x04},
{0x10, 0x76}, // AWB Off
{0x11, 0x2E},
{0x40, 0x29}, // RGAIN
{0x41, 0x20}, // GGAIN
{0x42, 0x3e}, // BGAIN
{0x43, 0x56}, // RMAX
{0x44, 0x20}, // RMIN
{0x45, 0x60}, // BMAX
{0x46, 0x22}, // BMIN
{0x10, 0xE6}, // AWB On
        {0xff, 0xff},
    },

    {//sunny
{0x03, 0x04},
{0x10, 0x76}, // AWB Off
{0x11, 0x28},
{0x40, 0x47}, // RGAIN
{0x41, 0x20}, // GGAIN
{0x42, 0x28}, // BGAIN
{0x43, 0x50}, // RMAX
{0x44, 0x40}, // RMIN
{0x45, 0x30}, // BMAX
{0x46, 0x20}, // BMIN
{0x10, 0xE6}, // AWB On
        {0xff, 0xff},
    },

    {//cloudy
{0x03, 0x04},
{0x10, 0x76}, // AWB Off
{0x11, 0x28},
{0x40, 0x43}, // RGAIN
{0x41, 0x20}, // GGAIN
{0x42, 0x28}, // BGAIN
{0x43, 0x5A}, // RMAX
{0x44, 0x53}, // RMIN
{0x45, 0x30}, // BMAX
{0x46, 0x25}, // BMIN
{0x10, 0xE6}, // AWB On 
        {0xff, 0xff},
    },

    {//office
{0x03, 0x04},
{0x10, 0x76}, // AWB Off
{0x11, 0x28},
{0x40, 0x21}, // RGAIN
{0x41, 0x20}, // GGAIN
{0x42, 0x47}, // BGAIN
{0x43, 0x29}, // RMAX
{0x44, 0x19}, // RMIN
{0x45, 0x53}, // BMAX
{0x46, 0x40}, // BMIN
{0x10, 0xE6}, // AWB On
        {0xff, 0xff},
    },

    {//home
{0x03, 0x04},
{0x10, 0x76}, // AWB Off
{0x11, 0x28},
{0x40, 0x37}, // RGAIN
{0x41, 0x20}, // GGAIN
{0x42, 0x4c}, // BGAIN
{0x43, 0x3F}, // RMAX
{0x44, 0x30}, // RMIN
{0x45, 0x54}, // BMAX
{0x46, 0x45}, // BMIN
{0x10, 0xE6}, // AWB On         {0xff, 0xff},
    },

};

LOCAL uint32 noon010pc30l_set_wb_mode(uint32 wb_mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_wb_mode_tab[wb_mode];
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
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_1174_112_2_18_0_31_19_948,(uint8*)"d", wb_mode);
    
	return 0; 
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;   1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc30l_flicker_mode_tab[][5]=
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

LOCAL uint32 noon010pc30l_set_flicker_elimination(uint32 flicker_mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_flicker_mode_tab[flicker_mode];

	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"noon010pc30l_set_flicker_elimination_mode: flicker_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_1208_112_2_18_0_31_19_949,(uint8*)"d", flicker_mode);
    
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		ev mode 0:-3 -2 -1 0 1 2 3 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc30l_ev_compensation_tab[][3]=
{

	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}},
	{{0xff, 0xff}}    
};

LOCAL uint32 noon010pc30l_ev_compensation(uint32 ev_mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_ev_compensation_tab[ev_mode];
	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"noon010pc30l_ev_compensation: ev_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_1242_112_2_18_0_31_19_950,(uint8*)"d", ev_mode);
    
	return 0;
}

LOCAL uint32 noon010pc30l_get_ev(uint32 param)
{

    return 0x00;
     
}

LOCAL uint32 noon010pc30l_set_meter_mode(uint32 param)
{

    // the param is a pointer to a uint32 array, param[0] means meter mode, 0---FULL meter, 1----Center meter, 2----Spot meter,
    // if param[0] equals 2,then the following 4 members from param[1] to param[4] desricbe the rectangle of spot meter.

    
    // add code here
    uint32  *p_para = (uint32*)param;

    if(NULL == p_para)
        return 0xff;
    
    switch(p_para[0])
    {
        case 0: //Full meter

            break;
        case 1: //center meter

            break;
        case 2: // spot meter
            break;

        default:
            return 0xff;
    }

	//SCI_TRACE_LOW:"noon010pc30l_set_meter_mode: mode = %d, rect(%d,%d,%d,%d)"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_1282_112_2_18_0_31_19_951,(uint8*)"ddddd", p_para[0],p_para[1],p_para[2],p_para[3],p_para[4]);
        
    return 0x00;
     
}
/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;	 1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T noon010pc30l_mode_tab[][64]=
{
    	{

{0xff,0xff},
}, // normal

{
    /* Scene Night */
{0xff,0xff},
} // night

};

LOCAL uint32 noon010pc30l_set_work_mode(uint32 mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)noon010pc30l_mode_tab[mode];

	SCI_ASSERT(mode <= 1);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"set_work_mode: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_NOON010PC30L_1319_112_2_18_0_31_19_952,(uint8*)"d", mode);
	return 0;
}

