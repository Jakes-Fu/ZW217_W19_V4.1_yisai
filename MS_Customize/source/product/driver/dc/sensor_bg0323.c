/******************************************************************************
 ** Copyright (c) 
 ** File Name:		sensor_BG0323.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor BG0323. 
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
#define BG0323_I2C_ADDR_W		0xa4
#define BG0323_I2C_ADDR_R		0xa5

#define SENSOR_GAIN_SCALE		16
 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_bg0323_ae_awb_enable(uint32 ae_enable, uint32 awb_enable);
LOCAL uint32 set_bg0323_ae_enable(uint32 enable);
LOCAL uint32 set_bg0323_hmirror_enable(uint32 enable);
LOCAL uint32 set_bg0323_vmirror_enable(uint32 enable);
LOCAL uint32 set_bg0323_preview_mode(uint32 preview_mode);
LOCAL uint32 _bg0323_Power_On(uint32 power_on);
LOCAL uint32 BG0323_Identify(uint32 param);
LOCAL uint32 BG0323_BeforeSnapshot(uint32 param);
LOCAL uint32 BG0323_After_Snapshot(uint32 param);

LOCAL uint32 bg0323_set_brightness(uint32 level);
LOCAL uint32 bg0323_set_contrast(uint32 level);
LOCAL uint32 bg0323_set_sharpness(uint32 level);
LOCAL uint32 bg0323_set_saturation(uint32 level);
LOCAL uint32 bg0323_set_image_effect(uint32 effect_type);

LOCAL uint32 bg0323_read_ev_value(uint32 value);
LOCAL uint32 bg0323_write_ev_value(uint32 exposure_value);
LOCAL uint32 bg0323_read_gain_value(uint32 value);
LOCAL uint32 bg0323_write_gain_value(uint32 gain_value);
LOCAL uint32 bg0323_read_gain_scale(uint32 value);
LOCAL uint32 bg0323_set_frame_rate(uint32 param);
LOCAL uint32 BG0323_set_work_mode(uint32 mode);
LOCAL uint32 BG0323_set_whitebalance_mode(uint32 mode);
LOCAL uint32 set_bg0323_ev(uint32 level);
LOCAL uint32 set_bg0323_video_mode(uint32 mode);
/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/

/*lint -save -e533 */
__align(4) const SENSOR_REG_T bg0323_YUV_640X480[]=
{
       {0x1c,0x01},  //Softreset

	{0xf0,0x00},  //Page00

	{0x1f,0x00}, 
	{0x0a,0x80},  //MCLKC
	{0x21,0x03},  //READC_L   //00->3  modify by bg jeff  20100406    
	{0x2c,0x00}, 
	{0x2d,0x02}, 
	{0x2e,0x04}, 
	{0x2f,0x0a}, 
	{0x30,0x18}, 

	{0x33,0x0a}, 
	{0x34,0x01}, 
	{0x37,0x04}, 

	{0x1d,0x01},  //Restart

	{0xfa,0xff},  //ISPCTRL

	{0xf0,0x02},  //Page02

       {0x9c,0x40}, 
  // gamma 1.8
  #if 0
	{0x40,0x01},  //GCSLOPE01_H
	{0x41,0x96},  //GCSLOPE01_L
	{0x43,0xBF},  //GCSLOPE02
	{0x45,0x8c},
	{0x46,0x67},
	{0x47,0x55},
	{0x48,0x4e},
	{0x49,0x48},
	{0x4a,0x44},
	{0x4b,0x3e},
	{0x4c,0x39},
	{0x4d,0x35},
	{0x4e,0x32},
	{0x4f,0x2e},
	{0x50,0x2a},
	{0x51,0x27},
	{0x51,0x25},
 #endif 
  // gamma 1.4  add by bg jeff 20100406
  #if 1
	{0x40,0x00},  //GCSLOPE01_H
	{0x41,0xd2},  //GCSLOPE01_L
	{0x43,0x86},  //GCSLOPE02
	{0x45,0x6e},
	{0x46,0x5a},
	{0x47,0x50},
	{0x48,0x4c},
	{0x49,0x48},
	{0x4a,0x45},
	{0x4b,0x42},
	{0x4c,0x3e},
	{0x4d,0x3b},
	{0x4e,0x39},
	{0x4f,0x36},
	{0x50,0x33},
	{0x51,0x30},
	{0x51,0x2f},
 #endif 
	{0x53,0x00},  //GCBRIGHT
	{0x54,0x10},  //BRGAIN

	{0x56,0x27}, 
	{0x57,0x2c}, 
	{0x58,0x31}, 
	{0x59,0x36}, 
	{0x5a,0x3b}, 
	{0x5b,0x00}, 
	{0x5c,0x36}, 
	{0x5d,0x2c}, 
	{0x5e,0x36}, 

	{0xee,0x0b}, 
	{0xef,0xd0}, 

	{0x9b,0x0f},  //CCMODE
#if 0
 // contrast  
	{0x98,0x90},
	{0xc9,0x80},  //CONGAINADJ
	{0xba,0xa6},  //CONKHLMAX
	{0xbc,0xa3},  //CONKMAX
	{0xbd,0x90},  //CONKMIN
#endif 
  
#if 1
 // contrast  
	{0x98,0x90},
	{0xc9,0x80},  //CONGAINADJ
	{0xba,0x90},  //CONKHLMAX
	{0xbc,0x90},  //CONKMAX
	{0xbd,0x80},  //CONKMIN
#endif 
	{0x8c,0x4d},
       {0x8d,0x96},
       {0x8e,0x1d},
  	{0x8f,0xd5},
 	 {0x90,0xab},
 	 {0x91,0x80},
 	 {0x92,0x80},
 	 {0x93,0x95},
 	 {0x94,0xeb},
	{0x95,0x00},  //y offset 
	{0x96,0x80},  //u offset
	{0x97,0x80},  //v offset


#if 0
  	{0x80,0x72},  //CCMAT11
	{0x81,0xd2},
	{0x82,0xfc},
	{0x83,0xfc},
	{0x84,0x5e},
	{0x85,0xe6},
	{0x86,0x01},
	{0x87,0xfc}, 
	{0x88,0x43},  //CCMAT33
	{0xa9,0x00},  //CCMAT14
	{0xaa,0x00}, 
	{0xab,0x00},  //CCMAT34
#endif

#if 0
  	{0x80,0x53},  //CCMAT11
	{0x81,0xd6},
	{0x82,0x17},
	{0x83,0xe0},
	{0x84,0x7a},
	{0x85,0xe6},
	{0x86,0xef},
	{0x87,0xf3}, 
	{0x88,0x60},  //CCMAT33
	{0xa9,0x00},  //CCMAT14
	{0xaa,0x00}, 
	{0xab,0x00},  //CCMAT34
#endif

#if 1    // add by bg  jeff 20100406
  	{0x80,0x6e},  //CCMAT11
	{0x81,0xe1},
	{0x82,0xf1},
	{0x83,0xe8},
	{0x84,0x73},
	{0x85,0xe5},
	{0x86,0xf5},
	{0x87,0xe0}, 
	{0x88,0x6b},  //CCMAT33
	{0xa9,0x00},  //CCMAT14
	{0xaa,0x00}, 
	{0xab,0x00},  //CCMAT34
#endif 
	{0xad,0x70},  //SKINKRR
	{0xae,0x5a},  //SKINKGG
	{0xaf,0x49},  //SKINKBB

	{0xb6,0xa0},  //CSRCOEFH
	{0xb7,0xa0},  //CSGCOEFH
	{0xb8,0xa0},  //CSBCOEFH
	{0x89,0x90},  //CSRCOEFL
	{0x8a,0x90},  //CSGCOEFL
	{0x8b,0x90},  //CSBCOEFL
	{0x9d,0x60},  //CSDKCOEFL

      {0xc7,0x06},  //DSCALLIFLAG
	{0xc8,0x00},  //DSCALE	

	{0xc6,0x01},  //YWBCOREN
	{0xc3,0xe0},  //YWBTHRED
	{0xc4,0x0a},  //CBTHRED
	{0xc5,0x0a},  //CRTHRED

	{0xee,0x14},  //AMOUNT_SEL

	{0xef,0x00},  //SHOWFLAG

	{0xf0,0x03},  //PAGE03
	{0x00,0xed},  //AECCTRL ED->EC disable AEC 
	{0x01,0x01},  //AECCNT

	{0x02,0x84},

	{0x03,0x82},

      {0x05,0x4c},  //TEXPMAX
	{0x06,0xb2},
      {0x07,0x3e},
#if 0
	{0x08,0x02},  //GMAX
	{0x09,0x00}, 
	{0x0c,0x00},  //GAIN1
	{0x0d,0x80},
	{0x0e,0x00},  //GAIN2ADD
	{0x0f,0x90},
	{0x10,0x00},  //GAIN2
	{0x11,0xa0},
	{0x12,0x00},
	{0x13,0xb0},
	{0x14,0x00},
	{0x15,0xc0},
	{0x16,0x00},
	{0x17,0xd0},
	{0x18,0x00},
	{0x19,0xe0},
	{0x1a,0x00},
	{0x1b,0xf0},
	{0x1c,0x01},
	{0x1d,0x00},
	{0x1e,0x01},
	{0x1f,0x40},
	{0x20,0x01},
	{0x21,0x80},
	{0x22,0x01},
	{0x23,0xa0},
	{0x24,0x02},  //GAIN12
	{0x25,0x00},
#endif 
#if 1   // add by bg jeff 20100406
	{0x08,0x01},  //GMAX
	{0x09,0x40}, 
	{0x0c,0x00},  //GAIN1
	{0x0d,0x80},
	{0x0e,0x00},  //GAIN2ADD
	{0x0f,0x90},
	{0x10,0x00},  //GAIN2
	{0x11,0xa0},
	{0x12,0x00},
	{0x13,0xb0},
	{0x14,0x00},
	{0x15,0xc0},
	{0x16,0x00},
	{0x17,0xd0},
	{0x18,0x00},
	{0x19,0xe0},
	{0x1a,0x00},
	{0x1b,0xe0},
	{0x1c,0x00},
	{0x1d,0xf0},
	{0x1e,0x01},
	{0x1f,0x00},
	{0x20,0x01},
	{0x21,0x10},
	{0x22,0x01},
	{0x23,0x20},
	{0x24,0x01},  //GAIN12
	{0x25,0x40},
#endif 

	{0x0b,0x28},

	{0x27,0x1e},
	{0x28,0x50},
	{0x29,0x35},
	{0x2a,0x28},
	{0x2b,0x70},
	{0x2c,0x50},
	{0x2d,0x40},
	{0x2e,0x30},
	{0x2f,0x20},
	{0x30,0x10},
	{0x31,0x08},

	{0x49,0x20},
	{0x04,0x14},
	{0x48,0x09},

	{0x3a,0x03},

	{0x32,0x03},
	{0x33,0x03},

	{0x36,0x78},

	{0x40,0x02},
	{0x41,0x04},
	{0x42,0x0f},
	{0x43,0x24},
	{0x44,0x38},
	{0x45,0x4e},
	{0x46,0x01},
	{0x47,0xac},

	{0x38,0x01},

	{0xf0,0x00},
	{0x1d,0x02},


// ���·�ת��
	{0xf0,0x00},
	{0x21,0x00},
	{0x1d,0x02},
};


LOCAL SENSOR_REG_TAB_INFO_T s_BG0323_resolution_Tab_YUV[]=
{	
	// COMMON INIT
	
       { ADDR_AND_LEN_OF_ARRAY(bg0323_YUV_640X480),		640,	480,	24 ,0},
	// YUV422 PREVIEW 1
	{ ADDR_AND_LEN_OF_ARRAY(bg0323_YUV_640X480),		640,	480,	24 ,0},
	// YUV422 PREVIEW 1
    	{ NULL, 0, 640, 480, 24, 0},

	{ PNULL,					0,		0,	0,			0,				0 },
	{ PNULL,					0,		0,	0,			0,				0 },
	{ PNULL,					0,		0,	0,			0,				0 },
	
	// YUV422 PREVIEW 2 
	{ PNULL,					0,		0,	0,			0,				0 },
	{ PNULL,					0,		0,	0,			0,				0 },
	{ PNULL,					0,		0,	0,			0,				0 },
	{ PNULL,					0,		0,	0,			0,				0 }

};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_BG0323_ioctl_func_tab = 
{
// Internal 
	PNULL,
    	PNULL,//_bg0323_Power_On,
	PNULL,
	BG0323_Identify,
	
	PNULL,			// write register
	PNULL,			// read  register	
	PNULL,
	PNULL,
	
// External
	set_bg0323_ae_enable,
	set_bg0323_hmirror_enable,
	set_bg0323_vmirror_enable,
	
	bg0323_set_brightness,
	bg0323_set_contrast,
	bg0323_set_sharpness,
	bg0323_set_saturation,
	
	set_bg0323_preview_mode,	
	bg0323_set_image_effect,

	PNULL,//BG0323_BeforeSnapshot,
	PNULL,//BG0323_After_Snapshot,

	PNULL,
	
	bg0323_read_ev_value,
	bg0323_write_ev_value,
	bg0323_read_gain_value,
	bg0323_write_gain_value,
	bg0323_read_gain_scale,
	bg0323_set_frame_rate,
	
	PNULL,
	PNULL,
	BG0323_set_whitebalance_mode,
	PNULL, //get_skip_frame
	PNULL, //set_iso
	set_bg0323_ev, //set_exposure_compensation
	PNULL,//PNULL,   //check_image_format_support
	PNULL,//PNULL,   //change_image_format
	PNULL,////change sensor image format according to param
	PNULL,   //cus_func_3
	PNULL,   //cus_func_4
	PNULL,   //set_anti_banding_flicker
       set_bg0323_video_mode,   //set_video_mode
	PNULL,//PNULL,   //pick_jpeg_stream

};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_bg0323_yuv_info =
{
	BG0323_I2C_ADDR_W,				// salve i2c write address
	BG0323_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_P|\
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
	0x00, 0x03,						// supply two code to identify sensor.
	0x01, 0x23,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	640,							// max width of source image
	480,							// max height of source image
	"BG0323",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;			

	s_BG0323_resolution_Tab_YUV,	// point to resolution table information structure
	&s_BG0323_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	0,                     // skip frame num before preview 
	0,                      // skip frame num before capture
	0,                      // deci frame num during preview	
	2,                       // deci frame num during video preview

	0,	// threshold enable		
	0,  // threshold mode 	
	0,	// threshold start postion	
	0,	// threshold end postion 
	0
};
/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void BG0323_WriteReg( uint8  subaddr, uint8 data )
{
	
	#ifndef	_USE_DSP_I2C_
		//uint8 cmd[2];
		//cmd[0]	=	subaddr;
		//cmd[1]	=	data;
		
		//I2C_WriteCmdArr(BG0323_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg(subaddr, data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW:"SENSOR: BG0323_WriteReg reg/value(%x,%x) !!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_544_112_2_18_0_30_36_726,(uint8*)"dd", subaddr, data);

}

LOCAL uint8 BG0323_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
		//I2C_WriteCmdArr(BG0323_I2C_ADDR_W, &subaddr, 1, SCI_TRUE);
		//I2C_ReadCmd(BG0323_I2C_ADDR_R, &value, SCI_TRUE);
		value =Sensor_ReadReg(subaddr);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif
	
    //SCI_TRACE_LOW:"SENSOR: BG0323_ReadReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_560_112_2_18_0_30_36_727,(uint8*)"dd", subaddr, value);
	return value;
}


LOCAL uint32 BG0323_Identify(uint32 param)
{

	uint32 i;
	uint32 nLoop;
	uint8 ret;
	uint32 err_cnt = 0;
	uint8 reg[2] 	= {0x00, 0x01};
	uint8 value[2] 	= {0x03, 0x23};

	for(i = 0; i<2; )
	{
		nLoop = 1000;
		ret = BG0323_ReadReg(reg[i]);
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
		i++;
	}

	//SCI_TRACE_LOW:"BG0323_Identify: it is BG0323"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_598_112_2_18_0_30_36_728,(uint8*)"");
	
	return (uint32)SCI_SUCCESS;
}

LOCAL uint32 _bg0323_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E		dvdd_val=g_bg0323_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E		avdd_val=g_bg0323_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E		iovdd_val=g_bg0323_yuv_info.iovdd_val;  
    BOOLEAN 				power_down=g_bg0323_yuv_info.power_down_level;	    
    BOOLEAN 				reset_level=g_bg0323_yuv_info.reset_pulse_level;
    uint32 				reset_width=g_bg0323_yuv_info.reset_pulse_width;	    
    
    if(SCI_TRUE==power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
        
        GPIO_SetSensorPwdn(!power_down);

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

    //SCI_TRACE_LOW:"SENSOR: _bg0323_Power_On(1:on, 0:off): %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_636_112_2_18_0_30_36_729,(uint8*)"d", power_on);
    
    return SCI_SUCCESS;
}

LOCAL uint32 set_bg0323_ae_enable(uint32 enable)
{
   #define bg0323_AE_ENABLE 0x00
    unsigned char ae_value;
        BG0323_WriteReg(0xf0,0x03);
        ae_value=BG0323_ReadReg(bg0323_AE_ENABLE);

        if(0x00==enable)
        {
            ae_value&=0xfe;
            BG0323_WriteReg(bg0323_AE_ENABLE,ae_value);
        }
        else if(0x01==enable)
        {        	
            ae_value|=0x01;
            BG0323_WriteReg(bg0323_AE_ENABLE,ae_value);
        }
        BG0323_WriteReg(0x38,0x01);
	//SCI_TRACE_LOW:"set_ae_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_659_112_2_18_0_30_36_730,(uint8*)"d", enable);

	return 0;
}

__align(4) const SENSOR_REG_T bg0323_ev_tab[][6]=
{
/*
    {{0xf0, 0x03}, {0x02, 0x4a}, {0x02, 0x48}, {0x38, 0x01}, {0xf0, 0x00}, {0x1d, 0x02},{0xff, 0xff}},
    {{0xf0, 0x03}, {0x02, 0x5a}, {0x02, 0x58}, {0x38, 0x01}, {0xf0, 0x00}, {0x1d, 0x02},{0xff, 0xff}},         
    {{0xf0, 0x03}, {0x02, 0x6a}, {0x02, 0x68}, {0x38, 0x01}, {0xf0, 0x00}, {0x1d, 0x02},{0xff, 0xff}},           
    {{0xf0, 0x03}, {0x02, 0x7a}, {0x02, 0x78}, {0x38, 0x01}, {0xf0, 0x00}, {0x1d, 0x02},{0xff, 0xff}},          
    {{0xf0, 0x03}, {0x02, 0x8a}, {0x02, 0x88}, {0x38, 0x01}, {0xf0, 0x00}, {0x1d, 0x02},{0xff, 0xff}},           
    {{0xf0, 0x03}, {0x02, 0x9a}, {0x02, 0x98}, {0x38, 0x01}, {0xf0, 0x00}, {0x1d, 0x02},{0xff, 0xff}},         
    {{0xf0, 0x03}, {0x02, 0xaa}, {0x02, 0xa8}, {0x38, 0x01}, {0xf0, 0x00}, {0x1d, 0x02},{0xff, 0xff}},
 */

      {{0xf0,0x02},{0x54, 0x0a},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x0c},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x0e},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x10},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x20},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x30},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
};

LOCAL uint32 set_bg0323_ev(uint32 level)
{
    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bg0323_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    SCI_ASSERT(level < 7);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    
    //SCI_TraceLow:"SENSOR: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_697_112_2_18_0_30_36_731,(uint8*)"d", level);

    return 0;
}

__align(4) const SENSOR_REG_T bg0323_video_mode[][10]=
{
    // normal mode
    {
       {0xf0, 0x02},
	{0x9c, 0x40},    	
	{0x6f, 0x00},	
	{0xf0, 0x03},  //PAGE03
	{0x05, 0x4c},  //TEXPMAX
	{0x06, 0xb2},
       {0x07, 0x3e},
	{0x0f, 0x00},	 
	{0x1d, 0x02},
	{0xff, 0xff}
    },    
    //video mode
    {
        {0xf0, 0x02},
	 {0x9c, 0x60}, 		
	 {0x6f, 0x01},	
	 {0xf0, 0x03},  //PAGE03
	 {0x05, 0x4c},  //TEXPMAX
	 {0x06, 0xb2},
        {0x07, 0x9c},
	 {0xf0, 0x00},
	 {0x1d, 0x02},
	 {0xff, 0xff}      
    }
};    

LOCAL uint32 set_bg0323_video_mode(uint32 mode)
{
    uint8 data=0x00;
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;

    SCI_ASSERT(mode <= 1);   
    sensor_reg_ptr = (SENSOR_REG_T*)bg0323_video_mode[mode];      

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
     
    /*
    switch(mode)
    {
        case DCAMERA_NORMAL_MODE:
                          
            break;

        case DCAMERA_VIDEO_MODE:     
            
            break;

        default :
            break;
    }    
    */

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
    	BG0323_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TraceLow:"SENSOR: set_video_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_765_112_2_18_0_30_36_732,(uint8*)"d", mode);
    return 0;
}


LOCAL uint32 set_bg0323_ae_awb_enable(uint32 ae_enable, uint32 awb_enable)
{
#if 0
#define bg0323_AE_ENABLE 0x00
        unsigned char ae_value;

        BG0323_WriteReg(0xf0,0x03);
        ae_value=BG0323_ReadReg(bg0323_AE_ENABLE);

        if(0x00==ae_enable)
        {
            ae_value&=0xfe;
        }
        else if(0x01==ae_enable)
        {
            ae_value|=0x01;
        }

        if(0x00==awb_enable)
        {
            ae_value&=0xf7;
        }
        else if(0x01==awb_enable)
        {
            ae_value|=0x08;
        }        

        BG0323_WriteReg(bg0323_AE_ENABLE,ae_value);
        BG0323_WriteReg(0x38,0x01);
	 BG0323_WriteReg(0xf0,0x00);
	 BG0323_WriteReg(0x1d,0x02);
        //SCI_TRACE_LOW:"set_ae_awb_enable: ae=%d awb=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_801_112_2_18_0_30_37_733,(uint8*)"dd", ae_enable, awb_enable);
#endif 
	return 0;
}



LOCAL uint32 set_bg0323_hmirror_enable(uint32 enable)
{
 	BG0323_WriteReg(0xf0,0x00);
	BG0323_WriteReg(0x21,0x01);
	BG0323_WriteReg(0x1d,0x02);

	//SCI_TRACE_LOW:"set_bg0323_hmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_814_112_2_18_0_30_37_734,(uint8*)"d", enable);
		

	return 0;
}




LOCAL uint32 set_bg0323_vmirror_enable(uint32 enable)
{

	//SCI_TRACE_LOW:"set_bg0323_vmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_826_112_2_18_0_30_37_735,(uint8*)"d", enable);
	

	return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T bg0323_brightness_tab[][6]=
{
	{{0xf0,0x02},{0x54, 0x0a},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x0c},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x0e},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x00},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x10},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x20},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0x54, 0x10},{0x95,0x30},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
};


LOCAL uint32 bg0323_set_brightness(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bg0323_brightness_tab[level];

	SCI_ASSERT(level < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		BG0323_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW:"bg0323_set_brightness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_862_112_2_18_0_30_37_736,(uint8*)"d", level);
	
	return 0;
}


__align(4) const SENSOR_REG_T bg0323_contrast_tab[][5]=
{
	{{0xf0,0x02},{0xc9, 0x50},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0xc9, 0x60},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0xc9, 0x70},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0xc9, 0x80},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},	
	{{0xf0,0x02},{0xc9, 0x90},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0xc9, 0xa0},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
	{{0xf0,0x02},{0xc9, 0xb0},{0xf0,0x00},{0x1d,0x02},{0xff,0xff}},
};

LOCAL uint32 bg0323_set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)bg0323_contrast_tab[level];

    SCI_ASSERT(level < 7 );
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BG0323_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    OS_TickDelay(20);
    //SCI_TRACE_LOW:"bg0323_set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_894_112_2_18_0_30_37_737,(uint8*)"d", level);

    return 0;
}


LOCAL uint32 bg0323_set_sharpness(uint32 level)
{
	
	return 0;
}


LOCAL uint32 bg0323_set_saturation(uint32 level)
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

LOCAL uint32 set_bg0323_preview_mode(uint32 preview_mode)
{
	//SCI_TRACE_LOW:"set_bg0323_preview_mode: preview_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_922_112_2_18_0_30_37_738,(uint8*)"d", preview_mode);
	
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{
			BG0323_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			BG0323_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			BG0323_set_work_mode(0);
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
__align(4) const SENSOR_REG_T bg0323_image_effect_tab[][14]=
{

	  // effect normal
	  {
        {0xf0,0x02},
        {0x9c,0x40},
		  	{0x80,0x6e},  //CCMAT11
			{0x81,0xe1},
			{0x82,0xf1},
			{0x83,0xe8},
			{0x84,0x73},
			{0x85,0xe5},
			{0x86,0xf5},
			{0x87,0xe0}, 
			{0x88,0x6b},  //CCMAT33
			{0xf0,0x00},
			{0x1d,0x02},
        {0xff,0xff}
    },
    //effect BLACKWHITE
    {
        {0xf0,0x02},
        {0x9c,0x40},
			  {0x80,0x13},
			  {0x81,0x25},
			  {0x82,0x07},
			  {0x83,0x13},	
			  {0x84,0x25},
			  {0x85,0x07},
			  {0x86,0x13},
			  {0x87,0x25},
			  {0x88,0x07},	
			  {0xf0,0x00},
			  {0x1d,0x02},			
        {0xff,0xff}
    },
    // effect RED
    {   
    	  {0xf0,0x02}, 
    	  {0x9c,0x40},
			  {0x80,0x70},
			  {0x81,0x00},
			  {0x82,0x00},
			  {0x83,0x13},	
			  {0x84,0x25},
			  {0x85,0x07},
			  {0x86,0x13},
			  {0x87,0x25},
			  {0x88,0x07},
			  {0xf0,0x00},
			  {0x1d,0x02},

        {0xff,0xff}
    },  
    // effect GREEN
    {   
    	  {0xf0,0x02},
    	  {0x9c,0x40},
			  {0x80,0x13},
			  {0x81,0x25},
			  {0x82,0x07},
			  {0x83,0x00},	
			  {0x84,0x70},
			  {0x85,0x00},
			  {0x86,0x13},
			  {0x87,0x25},
			  {0x88,0x07},
			  {0xf0,0x00},
			  {0x1d,0x02},
        {0xff,0xff}
    },  
    // effect  BLUE
    { 
    	  {0xf0,0x02},  
        {0x9c,0x40},
			  {0x80,0x13},
			  {0x81,0x25},
			  {0x82,0x07},
			  {0x83,0x13},	
			  {0x84,0x25},
			  {0x85,0x07},
			  {0x86,0x00},
			  {0x87,0x00},
			  {0x88,0x70},
			  {0xf0,0x00},
			  {0x1d,0x02},
        {0xff,0xff}
    },
    // effect YELLOW
    {
    {0xf0,0x02},  
        {0x9c,0x40},
			  {0x80,0x70},
			  {0x81,0x00},
			  {0x82,0x00},
			  {0x83,0x00},	
			  {0x84,0x70},
			  {0x85,0x00},
			  {0x86,0x13},
			  {0x87,0x25},
			  {0x88,0x07},
			  {0xf0,0x00},
			  {0x1d,0x02},
        {0xff,0xff}
    },
    // effect NEGATIVE
    {	     
        {0xf0,0x02}, 
        {0x9c,0xc0},
		  	{0x80,0x6e},  //CCMAT11
			{0x81,0xe1},
			{0x82,0xf1},
			{0x83,0xe8},
			{0x84,0x73},
			{0x85,0xe5},
			{0x86,0xf5},
			{0x87,0xe0}, 
			{0x88,0x6b},  //CCMAT33
			{0xf0,0x00},
			{0x1d,0x02},
        {0xff,0xff}
    },
    //effect ANTIQUE
    {
    
          {0xf0,0x02},  
          {0x9c,0x40},

			  {0x80,0x19},
			  {0x81,0x30},
			  {0x82,0x09},
			  {0x83,0x10},	
			  {0x84,0x22},
			  {0x85,0x04},
			  {0x86,0x0b},
			  {0x87,0x16},
			  {0x88,0x05},
			  {0xf0,0x00},
			  {0x1d,0x02},

        {0xff,0xff}
    },
};


LOCAL uint32 bg0323_set_image_effect(uint32 effect_type)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bg0323_image_effect_tab[effect_type];

	
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		BG0323_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//SCI_TRACE_LOW:"bg0323_set_image_effect: effect_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_1117_112_2_18_0_30_37_739,(uint8*)"d", effect_type);
    
	return 0;
}

LOCAL uint32 BG0323_After_Snapshot(uint32 param)
{
       unsigned char ae_value;

       BG0323_WriteReg(0xf0,0x03);
  //     ae_value=BG0323_ReadReg(0x00)&0xfe;
//	BG0323_WriteReg(0x00,ae_value|0x01);
	BG0323_WriteReg(0x00,0xed);	
       BG0323_WriteReg(0x38,0x01);	
       BG0323_WriteReg(0xf0,0x00);
       BG0323_WriteReg(0x0a,0x80);
       BG0323_WriteReg(0x1d,0x01);


	  return 0;
    
}

LOCAL uint32 BG0323_BeforeSnapshot(uint32 param)
{
      unsigned char ae_value;
      unsigned char iExp1,iExp2;
      short     iExp,iBank;

      BG0323_WriteReg(0xf0,0x03);
  //    ae_value=BG0323_ReadReg(0x00);
  //    BG0323_WriteReg(0x00,ae_value&0xfe);
      BG0323_WriteReg(0x00,0xe0);	
      BG0323_WriteReg(0x38,0x01);	
      BG0323_WriteReg(0xf0,0x00);		
      iExp1=BG0323_ReadReg(0x0c);
      iExp2=BG0323_ReadReg(0x0d);
      iExp=(iExp1<<8)|(iExp2);
      iExp1=BG0323_ReadReg(0x10);
      iExp2=BG0323_ReadReg(0x11);
      iBank=(iExp1<<8)|(iExp2);

 //     BG0323_WriteReg(0x38,0x01);
//      BG0323_WriteReg(0xf0,0x00);
      BG0323_WriteReg(0x0a,0x81);
      SCI_Sleep(20);
      iExp=iExp/2;
	     if (iExp < 1)
	          iExp = 1;			 
	    if(iExp<486)			
		   iBank=1;			
	    else			
		   iBank=iExp-486+1;				
			
//	  BG0323_WriteReg(0xf0,0x00);	        
	  BG0323_WriteReg(0x0c,(iExp>>8)&0xff);
	  BG0323_WriteReg(0x0d,iExp & 0xff);
	  BG0323_WriteReg(0x10,(iBank>>8) &0xff);
	  BG0323_WriteReg(0x11,iBank & 0xff);
 

        BG0323_WriteReg(0x1d,0x01);
        SCI_Sleep(100);
		
//	SCI_TRACE_LOW("set_ae_enable: enable = %d", enable);	
    return 0;
    
}

LOCAL uint32 bg0323_read_ev_value(uint32 value)
{
	return 0;
}

LOCAL uint32 bg0323_write_ev_value(uint32 exposure_value)
{
	
	return 0;	
}

LOCAL uint32 bg0323_read_gain_value(uint32 value)
{

	
	return 0;
}

LOCAL uint32 bg0323_write_gain_value(uint32 gain_value)
{

	
	return 0;
}

LOCAL uint32 bg0323_read_gain_scale(uint32 value)
{
	return SENSOR_GAIN_SCALE;
	
}


LOCAL uint32 bg0323_set_frame_rate(uint32 param)
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
__align(4) const SENSOR_REG_T bg0323_mode_tab[][8]=
{
    {
       {0xf0,0x03},
	     {0x05,0x4c},
	     {0x06,0xb2},
	     {0x07,0x3e},
	     {0x38,0x01},
	     {0xf0,0x00},
	     {0x1d,0x02},
    	 {0xff,0xff}
    }, // normal
    {
       {0xf0,0x03},
	     {0x05,0x53},
	     {0x06,0x88},
	     {0x07,0x3e},
	     {0x38,0x01},
	     {0xf0,0x00},
	     {0x1d,0x02},
    	 {0xff,0xff}
    } // night
};

LOCAL uint32 BG0323_set_work_mode(uint32 mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bg0323_mode_tab[mode];

	SCI_ASSERT(mode <= 1);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"set_work_mode: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_1267_112_2_18_0_30_37_740,(uint8*)"d", mode);
	return 0;
}

__align(4) const SENSOR_REG_T bg0323_WB_mode_tab[][8]=
{
		//WHITE_BALANCE_AUTO
	{ 
		{0xf0,0x03},
		{0x00,0xed},
		{0x34,0x35},
		{0x35,0x78},
		{0x38,0x01},
		{0xf0,0x00},
	       {0x1d,0x02},
		{0xff,0xff}
	},

	//WHITE_BALANCE_INCANDESCENCE
	{ 
		{0xf0,0x03},
		{0x00,0xfd},
		{0x34,0x35},
		{0x35,0x78},
		{0x38,0x01},
		{0xf0,0x00},
	       {0x1d,0x02},
		{0xff,0xff}
  },
  
	//U30  
	{
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff}
	}, 
	
	//CWF  ����/
	{
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff},
		{0xff, 0xff}       
	},    
	
	//WHITE_BALANCE_FLUORESCENT
	{
		{0xf0,0x03},
              {0x00,0xfd},
		{0x34,0x48},
		{0x35,0x58},	
		{0x38,0x01},
		{0xf0,0x00},
	       {0x1d,0x02},		
		{0xff,0xff}
  },

	//WHITE_BALANCE_SUN
	{ 
		{0xf0,0x03},
              {0x00,0xfd},
              {0x34,0x57},
              {0x35,0x4f},	
              {0x38,0x01},
              {0xf0,0x00},
	       {0x1d,0x02},	
		{0xff,0xff}
  },

	//WHITE_BALANCE_CLOUDY
	{ 
		{0xf0,0x03},
              {0x00,0xfd},
		{0x34,0x4b},
		{0x35,0x49},
		{0x38,0x01},
		{0xf0,0x00},
	       {0x1d,0x02},	
		{0xff,0xff}
  }
};

LOCAL uint32 BG0323_set_whitebalance_mode(uint32 mode )
{
	uint16 i;
	uint16 temp;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)bg0323_WB_mode_tab[mode];

	SCI_ASSERT(mode <= 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);

		
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"BG0323_set_whitebalance_mode: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BG0323_1373_112_2_18_0_30_38_741,(uint8*)"d", mode);
	return 0;

}

