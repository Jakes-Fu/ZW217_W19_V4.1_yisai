#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "sensor_cfg.h"

#define MT9M112_I2C_ADDR_W    	0xBA
#define MT9M112_I2C_ADDR_R    	0xBB

#define SENSOR_GAIN_SCALE		16

LOCAL uint32 mt9m112_write_reg(uint32 param);
LOCAL uint32 mt9m112_read_reg(uint32 param);
LOCAL uint32 mt9m112_set_ae_enable(uint32 enable);
LOCAL uint32 mt9m112_set_hmirror_enable(uint32 enable);
LOCAL uint32 mt9m112_set_vmirror_enable(uint32 enable);
LOCAL uint32 mt9m112_set_preview_mode(uint32 preview_mode);
LOCAL uint32 MT9M112_Identify(uint32 param);
LOCAL uint32 MT9M112_BeforeSnapshot(uint32 param);

LOCAL uint32 mt9m112_set_brightness(uint32 level);
LOCAL uint32 mt9m112_set_contrast(uint32 level);
LOCAL uint32 mt9m112_set_sharpness(uint32 level);
LOCAL uint32 mt9m112_set_saturation(uint32 level);
LOCAL uint32 mt9m112_set_image_effect(uint32 effect_type);

LOCAL uint32 mt9m112_read_ev_value(uint32 value);
LOCAL uint32 mt9m112_write_ev_value(uint32 exposure_value);
LOCAL uint32 mt9m112_read_gain_value(uint32 value);
LOCAL uint32 mt9m112_write_gain_value(uint32 gain_value);
LOCAL uint32 mt9m112_read_gain_scale(uint32 value);
LOCAL uint32 mt9m112_set_frame_rate(uint32 param);


#if 0
/*lint -save -e533 */
__align(4)const SENSOR_REG_T mt9m112_RGB_COMMON[]=
{
	{0xf0, 0x1},		// page 0
	{0x3a, 0x0200},
	{0x3a, 0x1200},
	{0x9b, 0x0200},
	{0x9b, 0x1200},
	{0x6,  0x200c},
	{0xf0, 0x0},
	{0xd,  0x108},
};


__align(4) const SENSOR_REG_T mt9m112_RGB_1280X1024[]=
{
	{0xf0,0x0},
	{0xc8,0xf0b},
};

__align(4)const SENSOR_REG_T mt9m112_RGB_640X480[] =
{
	{0xf0,0x0},
};



LOCAL SENSOR_REG_TAB_INFO_T s_MT9M112_resolution_Tab_RAWRGB[]=
{
	// COMMON INIT	
	{ ADDR_AND_LEN_OF_ARRAY(mt9m112_RGB_COMMON), 		0, 	  	0,	0, 0, 0, 0,12 },
		
	// RAWRGB PREVIEW 1
	{ ADDR_AND_LEN_OF_ARRAY(mt9m112_RGB_640X480), 		640, 	480, 0, 0, 640, 480,	24 },
	{ ADDR_AND_LEN_OF_ARRAY(mt9m112_RGB_1280X1024),		1280,	1024, 0, 0, 1280, 1024,	24 },	
	{ PNULL,					0, 		0,	0,							0 },
	{ PNULL,					0, 		0,	0,							0 },
	//{ PNULL,					0, 		0,	0,							0 },


	// RAWRGB PREVIEW 2	
	//{ ADDR_AND_LEN_OF_ARRAY(mt9m112_RGB_1280X1024),		1280,	1024,	24 },	
	{ PNULL,					0, 		0,	0,							0 },
	{ PNULL,					0, 		0,	0,							0 },
	{ PNULL,					0, 		0,	0,							0 },
	{ PNULL,					0, 		0,	0,							0 }


};
#endif
__align(4) const SENSOR_REG_T mt9m112_YUV_COMMON[]=
{
	{0xf0,0x0},

	//{0x21,0x40c},
	//{SENSOR_WRITE_DELAY,0x14},		// delay 20ms
};

__align(4) const SENSOR_REG_T mt9m112_YUV_1280X1024[]=
{
	{0xf0,0x0},
	{0xc8,0xf0b},
};

__align(4)const SENSOR_REG_T mt9m112_YUV_640X480[]=
{
	{0xf0,0x0},
	{0xd,  0x108},
	
};


LOCAL SENSOR_REG_TAB_INFO_T s_MT9M112_resolution_Tab_YUV[]=
{	
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(mt9m112_YUV_COMMON), 0, 0, 12, 0},	
	
	// YUV422 PREVIEW 1
	{ADDR_AND_LEN_OF_ARRAY(mt9m112_YUV_640X480), 640, 480,	24,0 },
	{ADDR_AND_LEN_OF_ARRAY(mt9m112_YUV_1280X1024), 1280, 1024, 24, 0},	
	{PNULL,	0, 0,	0, 0, 0},
	{PNULL,	0, 0,	0, 0, 0},
	

	// YUV422 PREVIEW 2 
	{PNULL,	0, 0,	0, 0, 0},
	{PNULL,	0, 0,	0, 0, 0},
	{PNULL,	0, 0,	0, 0, 0},
	{PNULL,	0, 0,	0, 0, 0}
};


LOCAL SENSOR_IOCTL_FUNC_TAB_T s_MT9M112_ioctl_func_tab = 
{
// Internal 
	PNULL,
	PNULL,
	PNULL,
	MT9M112_Identify,
	
	mt9m112_write_reg,			// write register
	mt9m112_read_reg,			// read  register	
	PNULL,
	PNULL,

// External
	mt9m112_set_ae_enable,
	mt9m112_set_hmirror_enable,
	mt9m112_set_vmirror_enable,
	
	mt9m112_set_brightness,
	mt9m112_set_contrast,
	mt9m112_set_sharpness,
	mt9m112_set_saturation,
	
	mt9m112_set_preview_mode,	
	mt9m112_set_image_effect,

	MT9M112_BeforeSnapshot,
	PNULL,

	PNULL,
	
	mt9m112_read_ev_value,
	mt9m112_write_ev_value,
	mt9m112_read_gain_value,
	mt9m112_write_gain_value,
	mt9m112_read_gain_scale,
	mt9m112_set_frame_rate,
	
	PNULL,
	PNULL,
	
        PNULL,
        PNULL,
        PNULL,
        PNULL,    
        PNULL,
        PNULL,
        PNULL,
        PNULL,   
        PNULL,
        PNULL,
        PNULL,
        PNULL	
	
};



PUBLIC SENSOR_INFO_T g_mt9m112_yuv_info =
{
	MT9M112_I2C_ADDR_W,				// salve i2c write address
	MT9M112_I2C_ADDR_R, 			// salve i2c read address
	
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
									
	SENSOR_LOW_PULSE_RESET,	// reset pulse level
	20,							// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,		// power down level
	
	0,	
	0xff, 0x1c48,				// product id
	0xff, 0x1c48,

	SENSOR_AVDD_2800MV,			// avdd voltage	
	
		
	1280,
	1024,
	"MT9M112",					

	SENSOR_IMAGE_FORMAT_YUV422,			// image format
	SENSOR_IMAGE_PATTERN_YUV422_UYVY,	// image pattern

	s_MT9M112_resolution_Tab_YUV,
	&s_MT9M112_ioctl_func_tab,
	
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	0,                     // skip frame num before preview 
	0,                      // skip frame num before capture	
	0,                      // deci frame num during preview	
	0,                      // deci frame num during video preview

	0,	// threshold enable		
	0,  // threshold mode 	
	0,	// threshold start postion	
	0,	// threshold end postion 
	0
	
};

LOCAL uint32 mt9m112_write_reg(uint32 param)
{
	uint8 	cmd[3];
	uint8   temp[2];
	uint16  high;
	uint16  low;
	uint8 	subaddr;
	uint16 	write_data;
	uint16  read_data;
	uint32	i;
	//16bit data,分两次写入，先高后低
	cmd[0] = (uint8)((param>>16)&0xff);
	cmd[1] = (uint8)((param>>8)&0xff);
	cmd[2] = (uint8)(param&0xff);
	
	subaddr 	= cmd[0];
	write_data	= (uint16)(param&0xffff);
	
	#if 0
	if(SENSOR_WRITE_DELAY != subaddr)
	{
		I2C_WriteCmdArr(MT9M112_I2C_ADDR_W, cmd, 3, SCI_TRUE);
	    
	    //SCI_TRACE_LOW:"reg: 0x%x, write_val: 0x%x"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_285_112_2_18_0_31_7_895,(uint8*)"dd",cmd[0],(cmd[1]<<8)+cmd[2]);
	}
	else
	{
		if((write_data) > 0x80)
		{
			SCI_Sleep(write_data);			
		}
		else
		{
		   	OS_TickDelay(write_data);
	    }
	    	
	    	//SCI_TRACE_LOW:"SENSOR: Delay %d ms"
	    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_298_112_2_18_0_31_7_896,(uint8*)"d", write_data);
	}
	#else
		//I2C_WriteCmdArr(MT9M112_I2C_ADDR_W, cmd, 3, SCI_TRUE);
            Sensor_WriteReg(subaddr, write_data);
	    //SCI_TRACE_LOW:"reg: 0x%x, write_val: 0x%x"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_303_112_2_18_0_31_7_897,(uint8*)"dd",cmd[0],(cmd[1]<<8)+cmd[2]);
	#endif
	//read back
	if(SENSOR_IMAGE_FORMAT_YUV422 == Sensor_GetSensorFormat())
		{
		    for( i = 0; i < 0x1000; i++ ); 
			
		    //I2C_WriteCmd(MT9M112_I2C_ADDR_W, cmd[0], SCI_TRUE);    

		    //I2C_ReadCmdArr(MT9M112_I2C_ADDR_R, &temp[0], 2, SCI_TRUE); 
                 read_data=Sensor_ReadReg(subaddr);
	
			//read_data = (temp[0]<<8)+temp[1];
			//high = (temp[0])<<8;
			//low  = temp[1];
		    
		    if(write_data != read_data)
		   	{
		    	//SCI_TRACE_LOW:"ERROR reg: 0x%x, write: 0x%x, read: 0x%x, temp[0]: 0x%x, temp[1]: 0x%x"
		    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_321_112_2_18_0_31_7_898,(uint8*)"ddddd", cmd[0], write_data, read_data, temp[0], temp[1]);
		   	
		   	}
	   	}
	
	return SENSOR_OP_SUCCESS;
	
}

LOCAL uint32 mt9m112_read_reg(uint32 param)
{
	uint8   temp[2];
	uint32  read_data;
	read_data = Sensor_ReadReg(param);
	//I2C_WriteCmd(MT9M112_I2C_ADDR_W, param, SCI_TRUE);    

	//I2C_ReadCmdArr(MT9M112_I2C_ADDR_R, &temp[0], 2, SCI_TRUE); 
	
	//read_data = (temp[0]<<8)+temp[1];
	
	//SCI_TRACE_LOW:"MT9M112 read_reg reg: 0x%x, val: 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_341_112_2_18_0_31_7_899,(uint8*)"dd", param, read_data);
	
	return read_data;

}


LOCAL uint32 mt9m112_set_ae_enable(uint32 enable)
{
	//SCI_TRACE_LOW:"set_ae_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_350_112_2_18_0_31_7_900,(uint8*)"d", enable);

	return 0;
}

LOCAL uint32 mt9m112_set_hmirror_enable(uint32 enable)
{
	//SCI_TRACE_LOW:"set_hmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_357_112_2_18_0_31_7_901,(uint8*)"d", enable);

	return 0;
}

LOCAL uint32 mt9m112_set_vmirror_enable(uint32 enable)
{
	//SCI_TRACE_LOW:"set_vmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_364_112_2_18_0_31_7_902,(uint8*)"d", enable);

	return 0;
}

LOCAL uint32 mt9m112_read_ev_value(uint32 value)
{
	uint32 read_exposure = 0;
	
	mt9m112_write_reg(0xf00002);
	
	read_exposure = mt9m112_read_reg(0x62);
	
	//SCI_TRACE_LOW:"read_ae_value: %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_377_112_2_18_0_31_7_903,(uint8*)"d", read_exposure);
	return read_exposure;
}

LOCAL uint32 mt9m112_write_ev_value(uint32 exposure_value)
{
	//SCI_TRACE_LOW:"write_ae_value: %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_383_112_2_18_0_31_7_904,(uint8*)"d", exposure_value);
	
	mt9m112_write_reg(0xf00002);
	mt9m112_write_reg((0x62<<16)+(exposure_value&0xffff));
	return 0;
}

LOCAL uint32 mt9m112_read_gain_value(uint32 value)
{
	//SCI_TRACE_LOW:"read_gain_value: "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_392_112_2_18_0_31_7_905,(uint8*)"");
	return 0;
}

LOCAL uint32 mt9m112_write_gain_value(uint32 gain_value)
{
	//SCI_TRACE_LOW:"write_gain_value: %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_398_112_2_18_0_31_7_906,(uint8*)"d", gain_value);
	return 0;

}

LOCAL uint32 mt9m112_read_gain_scale(uint32 value)
{
	return SENSOR_GAIN_SCALE;
}

LOCAL uint32 mt9m112_set_preview_mode(uint32 preview_mode)
{
	//SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_410_112_2_18_0_31_7_907,(uint8*)"d", preview_mode);

	return SCI_SUCCESS;
}

__align(4) const SENSOR_REG_T mt9m112_brightness_tab[][4]=
{
	{
		{0xff, 0xffff},{0xff, 0xffff},{0xff, 0xffff},	{0xff, 0xffff},
	},
};

__align(4) const SENSOR_REG_T mt9m112_contrast_tab[][34]=
{
	{
		{0x3D, 0x12},{0x7c, 0x04},{0x7d, 0x09},{0x7e, 0x14},{0x7f, 0x28},
		{0x80, 0x32},{0x81, 0x3C},{0x82, 0x46},{0x83, 0x4F},{0x84, 0x58},
		{0x85, 0x61},{0x86, 0x73},{0x87, 0x85},{0x88, 0xA5},{0x89, 0xc5},
		{0x8a, 0xDD},{0x6c, 0x40},{0x6d, 0x50},{0x6e, 0x58},{0x6f, 0x50},
		{0x70, 0x50},{0x71, 0x50},{0x72, 0x50},{0x73, 0x48},{0x74, 0x48},
		{0x75, 0x48},{0x76, 0x48},{0x77, 0x48},{0x78, 0x40},{0x79, 0x40},
		{0x7a, 0x30},{0x7b, 0x2E},{0x3D, 0x92},{0xFF, 0xFF},
	}, 
};


__align(4) const SENSOR_REG_T mt9m112_image_effect_tab[][4]=
{
	{
		{0xff, 0xffff},{0xff, 0xffff},{0xff, 0xffff},	{0xff, 0xffff},
	},
};


__align(4) const SENSOR_REG_T mt9m112_saturation_tab[][12]=
{
	{
		{0x4f, 0x0f}, {0x50, 0x0f}, {0x51, 0x01}, {0x52, 0x05}, {0x53, 0x0a},
		{0x54, 0x0e}, {0x55, 0x40}, {0x56, 0x40}, {0x57, 0x40}, {0x58, 0x0d},
		{0xff, 0xff}, {0xff, 0xff},
	},
};

__align(4) const SENSOR_REG_T mt9m112_sharpness_tab[][2]=
{
	{
		{0xff,0xffff},{0xff,0xffff},
	},
};
/*lint -restore */

LOCAL uint32 mt9m112_set_brightness(uint32 level)
{
	//SCI_TRACE_LOW:"set_brightness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_462_112_2_18_0_31_7_908,(uint8*)"d", level);
	
	return 0;
}

LOCAL uint32 mt9m112_set_contrast(uint32 level)
{
	//SCI_TRACE_LOW:"set_contrast: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_469_112_2_18_0_31_7_909,(uint8*)"d", level);
	
	return 0;
}

LOCAL uint32 mt9m112_set_sharpness(uint32 level)
{
	//SCI_TRACE_LOW:"set_sharpness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_476_112_2_18_0_31_7_910,(uint8*)"d", level);
	
	return 0;
}

LOCAL uint32 mt9m112_set_saturation(uint32 level)
{
	//SCI_TRACE_LOW:"set_saturation: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_483_112_2_18_0_31_7_911,(uint8*)"d", level);
	
	return 0;
}

LOCAL uint32 mt9m112_set_image_effect(uint32 effect_type)
{
	//SCI_TRACE_LOW:"-----------set_image_effect: effect_type = %d------------"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_490_112_2_18_0_31_7_912,(uint8*)"d", effect_type);
	
	return 0;
}

LOCAL uint32 mt9m112_set_frame_rate(uint32 param)
{
 	//SCI_TRACE_LOW:"set_frame_rate: param = %d"
 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_497_112_2_18_0_31_7_913,(uint8*)"d", param);
 	
 	return 0;
}

LOCAL uint32 MT9M112_Identify(uint32 param)
{
	#define MT9M112_CHIP_VERSION_REG	0xff
	#define MT9M112_CHIP_VERSION_VAL	0x148C
	uint16 cmd[2];
	uint32 read_data;
	//1st step set page address 0
	cmd[0] = 0xf0;
	cmd[1] = 0x0;
	mt9m112_write_reg((cmd[1]<<8)+cmd[1]);
	
	//2nd step read chip_version register
	read_data = mt9m112_read_reg(MT9M112_CHIP_VERSION_REG);
	
	if(MT9M112_CHIP_VERSION_VAL == read_data)
	{
		//SCI_TRACE_LOW:"this sensor is MT9M112"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_518_112_2_18_0_31_8_914,(uint8*)"");
		return 0;
	}
	else
	{
		//SCI_TRACE_LOW:"this sensor is not MT9M112"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_523_112_2_18_0_31_8_915,(uint8*)"");
		return 0xff;
	}
}

LOCAL void Sensor_Set_ChangeMega_MT9M112(void) 
{
	//SCI_TRACE_LOW:"Sensor_Set_ChangeMega_MT9M112"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_530_112_2_18_0_31_8_916,(uint8*)"");
}

LOCAL uint32 MT9M112_BeforeSnapshot(uint32 param)
{
	SENSOR_MODE_E sensor_mode = (SENSOR_MODE_E)param;
	
	//SCI_TRACE_LOW:"MT9M112_BeforeSnapshot: param = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_MT9M112_537_112_2_18_0_31_8_917,(uint8*)"d", param);
	
	if(SENSOR_MODE_SNAPSHOT_ONE_FIRST == sensor_mode)
	{
		Sensor_Set_ChangeMega_MT9M112();
	}

	return 0;
}
