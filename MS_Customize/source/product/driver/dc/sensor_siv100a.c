/******************************************************************************
 ** File Name:      SIV100A.c                                         *
 ** Author:        	cloud6                                             	  *
 ** Date:           07/09/2007                                            *
 ** Copyright:      XUNRUI. All Rights Reserved.        *
 ** Description:    implementation of digital camera register interface       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **                                  *
 ******************************************************************************/

#ifndef _SIV100A_C_
#define _SIV100A_C_

#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"


/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define SIV100A_I2C_ADDR_W    			0x66
#define SIV100A_I2C_ADDR_R    			0x67
#define SIV100A_I2C_ACK				0x0

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
#define    SIV100A_DELAY_MS(ms)            {\
                                              uint32 end = SCI_GetTickCount() + ms;\
                                              while (SCI_GetTickCount() < end)\
                                              ;\
                                          }
                                           
 


/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
// OV give the parameters

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/


__align(4) const SENSOR_REG_T siv100a_YVU_640X480[] = 
{
	{0x40,	0x00},
	{0x60,	0x00},
	{0x04,	0x00},	//Sensor Control Register
	{0x05,	0x07},	//VGA mode
	{0x10,	0x0b},
	{0x11,	0x05},
	{0x12,	0x0a},
	{0x18, 	0x00},
	{0x19, 	0x7c},
	{0x20,	0x00},	//50Hz for 24MHz
	{0x21,	0x9c},
	{0x22,	0x03},
	{0x34,	0x7d},
	{0x23,	0x00},
	{0x24,	0x9c},
	{0x25,	0x03},
	{0x35,	0x7d},
	{0x33,	0x14},	//Frame rate control
	{0x40,	0x9c},	//Auto Exposure
	{0x41,	0x8a},	//brighness.8a is default value//70
	{0x43,	0xc0},
	{0x44,	0x40},
	{0x45,	0x20},
	{0x46,	0x0a},
	{0x47,	0x15},
	{0x48,	0x16},
	{0x49,	0x0b},
	{0x4a,	0x53},
	{0x4b,	0xc1},
	{0x4c,	0x0c},
	{0x4e,	0x97},
	{0x4f,	0x8a},
	{0x5a,	0x00},
	{0x60,	0x90},	//Auto White Balance
	{0x61,	0xa3},
	{0x62,	0x11},
	{0x63,	0x82},
	{0x64,	0x80},
	{0x65,	0xdf},
	{0x66,	0x70},
	{0x67,	0xdf},
	{0x68,	0x78},
	{0x69,	0x30},
	{0x6a,	0x30},
	{0x6b,	0x80},
	{0x6c,	0x01},
	{0x6d,	0xe0},
	{0x6e,	0x30},
	{0x6f,	0xc0},
	{0x70,	0x80},
	{0x71,	0x32},
	{0x72,	0x32},
	{0x73,	0x80},
	{0x74,	0x80},
	{0x75,	0x40},
	{0x80,	0xaf},
	{0x81,	0x0f},	//Signal Output Control
	{0x82,	0x35},	//Output Format Control
	{0x83,	0x00},
	{0x85,	0x81},//81
	{0x86,	0x00},
	{0x87,	0x21},//21
	{0x88,	0xff},//0f
	{0x89,	0x0c},//0c
	{0x8d,	0xff},//7c
	{0x90,	0x80},
	{0x91,	0x80},
	{0x92,	0x80},
	{0x93,	0x80},
	{0xa0,	0x40},	//BLC
	{0xa5,	0x14},
	{0xa6,	0x14}, 
	{0xa7,	0x14},
	{0xa8,	0x14},
	{0xb0,	0x88},	//Shading Control		B0~BC		
	{0xb1,	0xff},
	{0xb2,	0xff},
	{0xb3,	0xff},
	{0xb4,	0xff},
	{0xb5,	0xcd},
	{0xb6,	0xca},
	{0xb7,	0x77},
	{0xb8,	0x88},
	{0xb9,	0x66},
	{0xba,	0x86},
	{0xbb,	0x9d},
	{0xbc,	0x8b},
	{0xc0,	0x00},	//Gammma	Control		C0~CF
	{0xc1,	0x0b},
	{0xc2,	0x14},
	{0xc3,	0x22},
	{0xc4,	0x3d},
	{0xc5,	0x54},
	{0xc6,	0x6a},
	{0xc7,	0x7c},
	{0xc8,	0x8d},
	{0xc9,	0x9c},
	{0xca,	0xaa},
	{0xcb,	0xc4},
	{0xcc,	0xd9},
	{0xcd,	0xed},
	{0xce,	0xf7},
	{0xcf,	0xff},
	{0xd0,	0x3d},	//Color Matrix		D0~D8
	{0xd1,	0xc6},
	{0xd2,	0xfd},
	{0xd3,	0x10},
	{0xd4,	0x1d},
	{0xd5,	0x13},
	{0xd6,	0xf3},
	{0xd7,	0xbd},
	{0xd8,	0x50},
	{0xd9,	0xb4},	//Sharpness Control  94
	{0xda,	0x10},
	{0xdb,	0x21},
	{0xdc,	0xf0},
	{0xe0,	0x12},	//Contrast Control
	{0xe1,	0x14},	//Cr Color Saturation 
	{0xe2,	0x14},	//Cb Color Saturation
	{0xe3,	0x84},	//Brightness Control  84
	{0xe4,	0xff},
	{0xe5,	0x00},
	{0xe6,	0xff},
	{0xe7,	0x00},
	{0xe8,	0xff},
	{0xe9,	0x00},
	{0xea,	0x21},
	{0xeb,	0x28},
	{0x03,	0x05},
	{0x7a,	0xb0},
	{0x7b,	0xb0}, 
	{0x7c,	0x80},  
	{0xff,	0xff},

};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL uint8 SIV100A_WriteReg( uint8  subaddr, uint8 data )
{
	
	#ifndef	_USE_DSP_I2C_
		//uint8 cmd[2];
		//cmd[0]	=	subaddr;
		//cmd[1]	=	data;
		
		//I2C_WriteCmdArr(SIV100A_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg( subaddr,  data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW:"SENSOR: SIV100A_WriteReg reg/value(%x,%x) !!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_204_112_2_18_0_32_33_1330,(uint8*)"dd", subaddr, data);

    return 0;

}

LOCAL uint8 SIV100A_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
		//I2C_WriteCmdArr(SIV100A_I2C_ADDR_W, &subaddr, 1, SCI_TRUE);
		//I2C_ReadCmd(SIV100A_I2C_ADDR_R, &value, SCI_TRUE);
		value=Sensor_ReadReg(subaddr);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif
	
	return value;
}

/******************************************************************************/
// Description: sensor probe function
// Author:     benny.zou
// Input:      none
// Output:     result
// Return:     0           successful
//             others      failed
// Note:       this function only to check whether sensor is work, not identify 
//              whitch it is!!
/******************************************************************************/
LOCAL uint32 SIV100A_Identify(uint32 param)//(void)
{
    uint8 reg[2]    = {0x01, 0x02};
    uint8 value[2]  = {0x0c, 0x10};
    uint8 ret       = 0;
    uint32 i;
    uint8 err_cnt = 0;
    uint32 nLoop = 1000;
    
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = SIV100A_ReadReg(reg[i]);    
		
		//SCI_TRACE_LOW:"SIV100A read ret is %X"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_247_112_2_18_0_32_33_1331,(uint8*)"d",ret );
		
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
            	//SCI_TRACE_LOW:"SIV100A Fail"
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_254_112_2_18_0_32_33_1332,(uint8*)"" );
                return 0xFF;
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

     //SCI_TRACE_LOW:"SIV100A succ"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_269_112_2_18_0_32_33_1333,(uint8*)"" );
//    s_sensor_ID = SENSOR_SIV100A;
    return SCI_SUCCESS; 
}

__align(4) const SENSOR_REG_T siv100a_brightness_tab[][2]=
{
    {
            {0xe3, 0xb0},
            {0xFF,0xFF},
    },
    
    {
            {0xe3, 0xa0},
            {0xFF,0xFF},
    },

    {
            {0xe3, 0x90},
            {0xFF,0xFF},
    },

    {
            {0xe3, 0x84},
            {0xFF,0xFF},    
    },

    {
            {0xe3, 0x10},
            {0xFF,0xFF},
    },

    {
            {0xe3, 0x20},
            {0xFF,0xFF},
    },

    {
            {0xe3, 0x30},
            {0xFF,0xFF},
    }
};
LOCAL uint32 set_brightness(uint32 level)
{
    uint16 i;
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv100a_brightness_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_323_112_2_18_0_32_34_1334,(uint8*)"d", level);
    
    return 0;
}




__align(4) const SENSOR_REG_T siv100a_contrast_tab[][2]=
{
    {
            {0xe0, 0x02},
            {0xFF, 0xFF},
    },

    {
            {0xe0, 0x08},
            {0xFF, 0xFF},
    },

    {
            {0xe0, 0x0d},
            {0xFF, 0xFF},
    },

    {
            {0xe0, 0x12},
            {0xFF, 0xFF},
    },

    {
            {0xe0, 0x18},
            {0xFF, 0xFF},
    },    
    {
            {0xe0, 0x1d},
            {0xFF, 0xFF},
    },    
    {
            {0xe0, 0x22},
            {0xFF, 0xFF},
    }, 
};

LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;    
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv100a_contrast_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_379_112_2_18_0_32_34_1335,(uint8*)"d", level);
    
    return 0;
}


LOCAL uint32 Sensor_Write_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    return 0;
}

__align(4) const SENSOR_REG_T siv100a_mode_tab[][1]=
{
	//normal
    {
            {0xFF, 0xFF},
    },
	//night
    {
            {0xFF, 0xFF},
    }
};

LOCAL uint32 set_preview_mode(uint32 preview_mode)
{
    SENSOR_REG_T *reg_ptr = (SENSOR_REG_T *)siv100a_mode_tab[preview_mode];
    
    //SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_412_112_2_18_0_32_34_1336,(uint8*)"d", preview_mode);
    
    Sensor_Write_Regs( reg_ptr );
    
    return 0;
}




__align(4) const SENSOR_REG_T siv100a_image_effect_tab[][1]=
{
    // effect normal
    {
        { 0xff, 0xff},
    },
    // effect BLACKWHITE
    {
        {0xff, 0xff},
    },
    // effect RED 
    {
        {0xff, 0xff},
    },

    // effect GREEN 
    {
        {0xff, 0xff},
    },

    // effect  BLUE
    {
        {0xff, 0xff},
    },
    // effect yellow
    {
         {0xff, 0xff},
    },
    // effect sepia
    {
         {0xff, 0xff},
    },
    
    // effect NEGATIVE
    {
        {0xff, 0xff},
    },

};

LOCAL uint32 set_image_effect(uint32 effect_type)
{
    uint16 i;
    
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv100a_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"-----------set_image_effect: effect_type = %d------------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100A_476_112_2_18_0_32_34_1337,(uint8*)"d", effect_type);
    
    return 0;
}

LOCAL SENSOR_REG_TAB_INFO_T s_siv100a_resolution_Tab_YUV[]=
{
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(siv100a_YVU_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
	
	// YUV422 PREVIEW 1	
	{PNULL, 0, 640, 480,24, SENSOR_IMAGE_FORMAT_YUV422},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0}
};


LOCAL SENSOR_IOCTL_FUNC_TAB_T s_siv100a_ioctl_func_tab = 
{
// Internal 
    PNULL,
    PNULL,
    PNULL,
    SIV100A_Identify,
    
    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

// External
    PNULL, //set_ae_enable,
    PNULL, //set_hmirror_enable,
    PNULL, //set_vmirror_enable,
    
    set_brightness,
    set_contrast,
    PNULL, //set_sharpness,
    PNULL, //set_saturation,
    
    set_preview_mode,   
    set_image_effect,

    PNULL, //ov9655_BeforeSnapshot,
    PNULL,

    PNULL,
    
    PNULL, //read_ev_value,
    PNULL, //write_ev_value,
    PNULL, //read_gain_value,
    PNULL, //write_gain_value,
    PNULL, //read_gain_scale,
    PNULL, //set_frame_rate,
    
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


PUBLIC SENSOR_INFO_T g_SIV100A_yuv_info =
{
	SIV100A_I2C_ADDR_W,				// salve i2c write address
	SIV100A_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_P|\
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
	
	SENSOR_LOW_PULSE_RESET,		// reset pulse level
	20,								// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	2,								// count of identify code
	0x01, 0x0c,						// supply two code to identify sensor.
	0x02, 0x10,						// for Example: index = 0-> Device id, index = 1 -> version id											
											
	SENSOR_AVDD_2800MV,				// voltage of avdd	
	
	640,							// max width of source image
	480,							// max height of source image
	"SIV100A",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;			

	s_siv100a_resolution_Tab_YUV,	// point to resolution table information structure
	&s_siv100a_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	

	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	4,                     // skip frame num before preview 
	3,                      // skip frame num before capture
	0,                      // deci frame num during preview	
	2,                      // deci frame num during video preview

	0,	// threshold enable		
	0,  // threshold mode 	
	0,	// threshold start postion	
	0,	// threshold end postion 
	0
	
}; 


#endif

