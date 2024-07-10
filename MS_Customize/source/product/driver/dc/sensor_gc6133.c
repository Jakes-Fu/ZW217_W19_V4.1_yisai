/******************************************************************************
 ** File Name:    Sensor_GC6133.c                                         *
 ** Author:         Haydn_He                                                 *
 ** Date:           2012-11-16	                                         *
 ** Platform:       SP6530
 ** Copyright:    Spreadtrum All Rights Reserved.        *
 ** Description:   implementation of digital camera register interface       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log$
 ** DATE             NAME             DESCRIPTION                                 *
 ** 2012-11-16   Haydn            Fristly Released      
 *******************************************************************************/
#ifndef _GC6133_C_

#define _GC6133_C_
#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
//#include "i2c_drv.h"
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
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define GC6133_I2C_ADDR_W    			0x80
#define GC6133_I2C_ADDR_R    			0x81
#define GC6133_I2C_ACK					0x0
LOCAL BOOLEAN  	bl_GC_50Hz_GC6133		= FALSE;

//#define IMAGE_NORMAL 
#define IMAGE_H_MIRROR 
//#define IMAGE_V_MIRROR 
//#define IMAGE_HV_MIRROR 

#ifdef IMAGE_NORMAL
#define MIRROR 0x54
#define AD_NUM 0x04
#define BLK_VAL 0x46
#endif

#ifdef IMAGE_H_MIRROR
#define MIRROR 0x55
#define AD_NUM 0x08
#define BLK_VAL 0x46
#endif

#ifdef IMAGE_V_MIRROR
#define MIRROR 0x5e
#define AD_NUM 0x04
#define BLK_VAL 0x30
#endif

#ifdef IMAGE_HV_MIRROR
#define MIRROR 0x5f
#define AD_NUM 0x08
#define BLK_VAL 0x30
#endif

//#define GC6133_LOAD_FROM_T_FLASH // add by tim
#ifdef GC6133_LOAD_FROM_T_FLASH  
#include "sfs.h"
#include "ffs.h"
LOCAL uint32 Load_GC6133_RegTab_From_T_Flash(void);
#endif

////extern unsigned int sjtc_camera_pix8;

uint8 g_gc6133_indenty_id[2] = {0};
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 GC6133_Power_On(uint32 power_on);
LOCAL uint32 GC6133_Identify(uint32 param);
LOCAL void   GC6133_WriteReg( uint8  subaddr, uint8 data );
LOCAL uint8  GC6133_ReadReg( uint8  subaddr);
LOCAL void   GC6133_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr );
LOCAL void   GC6133_Delayms (uint32 ms);
//LOCAL uint32 Set_GC6133_Mirror(uint32 level);
//LOCAL uint32 Set_GC6133_Flip(uint32 level);
LOCAL uint32 Set_GC6133_Brightness(uint32 level);
LOCAL uint32 Set_GC6133_Contrast(uint32 level);
LOCAL uint32 Set_GC6133_Image_Effect(uint32 effect_type);
LOCAL uint32 Set_GC6133_Ev(uint32 level);
LOCAL uint32 Set_GC6133_Anti_Flicker(uint32 mode);
LOCAL uint32 Set_GC6133_Preview_Mode(uint32 preview_mode);
LOCAL uint32 Set_GC6133_AWB(uint32 mode);

LOCAL uint32 Set_GC6133_Video_Mode(uint32 mode);
LOCAL uint32 GC6133_GetPclkTab(uint32 param);
/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/
__align(4) const SENSOR_REG_T GC6133_YUV_240X320[] = 
{
	/////////////////////////////////////////////////////
	//////////////////////   SYS   //////////////////////
	/////////////////////////////////////////////////////
	{0xfe, 0xa0},
	{0xfe, 0xa0},
	{0xfe, 0xa0},
	{0xf6, 0x00},
	{0xfa, 0x11},
	{0xfc, 0x12}, //clock enable
	{0xfe, 0x00}, //Page select
	
	{0x49, 0x70},  //AWB r gain
	{0x4a, 0x40},  //AWB g gain
	{0x4b, 0x5d},  //AWB b gain
	/////////////////////////////////////////////////////
	////////////////   ANALOG & CISCTL   ////////////////
	/////////////////////////////////////////////////////
	{0x03, 0x00},
	{0x04, 0xfa},
	{0x01, 0x41}, //hb
	{0x02, 0x12}, //vb
	{0x0f, 0x01},
	{0x0d, 0x30},
	{0x12, 0xc8},
	{0x14, MIRROR}, //dark CFA
	{0x15, 0x32}, //1:sdark 0:ndark
	{0x16, AD_NUM},
	{0x17, 0x19},
	{0x1d, 0xb9},
	{0x1f, 0x3f}, //PAD_drv
	{0x7a, 0x00},
	{0x7b, 0x14},
	{0x7d, 0x36},
	{0xfe, 0x10}, //cisctl rst [4]
	/////////////////////////////////////////////////////
	//////////////////////   ISP   //////////////////////
	/////////////////////////////////////////////////////
	{0x20, 0x7e},
	{0x22, 0xb8},
	{0x24, 0x54}, //output_format
	{0x26, 0x87}, //[5]Y_switch [4]UV_switch [2]skin_en

	{0x39, 0x00},//crop window
	{0x3a, 0x80},
	{0x3b, 0x01}, //width
	{0x3c, 0x40},
	{0x3e, 0xf0},//height
	/////////////////////////////////////////////////////
	//////////////////////   BLK   //////////////////////
	/////////////////////////////////////////////////////
	{0x2a, 0x2f},
	{0x37, BLK_VAL}, //[4:0]blk_select_row

	/////////////////////////////////////////////////////
	//////////////////////   GAIN   /////////////////////
	/////////////////////////////////////////////////////
	{0x3f, 0x18}, //global gain 20160901

	/////////////////////////////////////////////////////
	//////////////////////   DNDD   /////////////////////
	/////////////////////////////////////////////////////
	{0x52, 0x4b}, //4f 
	{0x53, 0x81}, 
	{0x54, 0x43},
	{0x56, 0x78}, 
	{0x57, 0xaa},
	{0x58, 0xff},
	/////////////////////////////////////////////////////
	//////////////////////   ASDE   /////////////////////
	/////////////////////////////////////////////////////
	{0x5b, 0x60}, //dd&ee th
	{0x5c, 0x80}, //60/OT_th
	{0xab, 0x28},
	{0xac, 0xb5},
	/////////////////////////////////////////////////////
	/////////////////////   INTPEE   ////////////////////
	/////////////////////////////////////////////////////
	{0x60, 0x45},
	{0x62, 0x68},
	{0x63, 0x13}, //edge effect
	{0x64, 0x43}, 
	/////////////////////////////////////////////////////
	//////////////////////   CC   ///////////////////////
	/////////////////////////////////////////////////////
	{0x65, 0x13}, //Y
	{0x66, 0x26},
	{0x67, 0x07},
	{0x68, 0xf5}, //Cb
	{0x69, 0xea},
	{0x6a, 0x21},
	{0x6b, 0x21}, //Cr
	{0x6c, 0xe4},
	{0x6d, 0xfb},

	/////////////////////////////////////////////////////
	//////////////////////   YCP   //////////////////////
	/////////////////////////////////////////////////////
	{0x81, 0x36}, //cb 30
	{0x82, 0x36}, //cr 30
	{0x83, 0x48}, //luma contrast
	{0x8d, 0x78}, //edge dec sa
	{0x8e, 0x25}, //autogray

	/////////////////////////////////////////////////////
	//////////////////////   AEC   //////////////////////
	/////////////////////////////////////////////////////
	{0x90, 0x38},//20160901
	{0x9d, 0x32}, //flicker step
	{0x9e, 0x61}, //[7:4]margin
	{0x9f, 0xf4}, //exp_level 3f2
	{0xa3, 0x28}, //pregain
	{0xa4, 0x01}, 

	/////////////////////////////////////////////////////
	//////////////////////   AWB   //////////////////////
	/////////////////////////////////////////////////////
	{0xb1, 0x1e}, //Y_to_C_diff
	{0xb3, 0x20}, //C_max
	{0xbd, 0x70}, //R_limit
	{0xbe, 0x58}, //G_limit
	{0xbf, 0xa0}, //B_limit

	{0xfe, 0x00},
	{0x43, 0xa8},
	{0xb0, 0xf2}, 
	{0xb5, 0x40},
	{0xb8, 0x05},
	{0xba, 0x60},

	/////////////////////////////////////////////////////
	////////////////////   Banding   ////////////////////
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	//////////////////////   SPI   //////////////////////
	/////////////////////////////////////////////////////
	{0xfe, 0x02},
	{0x01, 0x01}, //SPI MASTER enable
	{0x02, 0x82}, //ddr_mode  //80   for SC8910 
	{0x03, 0x20},  //1-wire
	{0x04, 0x20},  //[4] master_outformat
	{0x0a, 0x00},  //Data ID, 0x00-YUV422, 0x01-RGB565
	{0x13, 0x10},
	{0x24, 0x00},  //[1]sck_always [0]BT656   
	{0x28, 0x03}, //clock_div_spi
	{0xfe, 0x00},
	//////////////////////////////////////////
	//////////////////////output////////////////
	/////////////////////////////////////////
	{0x22, 0xf8}, //open awb
	{0xf1, 0x03}, //output enable
};

LOCAL SENSOR_REG_TAB_INFO_T s_GC6133_resolution_Tab_YUV[]=
{   
    { ADDR_AND_LEN_OF_ARRAY(GC6133_YUV_240X320), 240, 320,  24, SENSOR_IMAGE_FORMAT_YUV422 },
    // YUV422 PREVIEW 1
    { PNULL,                    0,   240,    320,    24   ,   SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    

    // YUV422 PREVIEW 2 
     { PNULL,                   0,      0,  0 ,         0,         0     },  //For BCR Preview 
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      }

};
LOCAL uint32 GC6133_set_flash_enable(uint32 enable)
{
	SCI_TRACE_LOW("david sensor set flash enable. %d", enable);
    return 0;
}

#ifdef GC6133_LOAD_FROM_T_FLASH
LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC6133_ioctl_func_tab = 
{
// Internal 
    PNULL,
    GC6133_Power_On,    
    PNULL,
    GC6133_Identify,
    
    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    GC6133_GetPclkTab,

// External
     PNULL,//set_ae_enable,
    PNULL,          //Set_GC6133_Hmirror, //set_hmirror_enable,
    PNULL,          //Set_GC6133_Vmirror, //set_vmirror_enable,
    PNULL,//Set_GC6133_Brightness, 

    PNULL,//Set_GC6133_Contrast,
    PNULL, 
    PNULL, 
    Set_GC6133_Preview_Mode,   //15
    
    PNULL,//Set_GC6133_Image_Effect,    //16  
    PNULL,//GC6133_Before_Snapshot,
    PNULL,//GC6133_After_Snapshot,
    PNULL,
    
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    
    PNULL,//Set_GC6133_AWB,   //28   
    PNULL, 
    PNULL, 
    PNULL,//Set_GC6133_Ev,   //31
    
    PNULL, 
    PNULL, 
    PNULL,     
    PNULL,
    
    PNULL,
    PNULL,//Set_GC6133_Anti_Flicker,
    PNULL,//Set_GC6133_Video_Mode,    
     PNULL,
};
#else
LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC6133_ioctl_func_tab = 
{
// Internal 
    PNULL,
	GC6133_Power_On, 
    PNULL,
    GC6133_Identify,
    PNULL,
    PNULL,
    
    PNULL,
    GC6133_GetPclkTab,

// External
    PNULL, 
    PNULL, //Set_GC6133_Mirror,
    PNULL, //Set_GC6133_Flip,
    
    Set_GC6133_Brightness,
    Set_GC6133_Contrast,
    PNULL, 
    PNULL, 
    Set_GC6133_Preview_Mode, 
    
    Set_GC6133_Image_Effect, 
    PNULL,//GC6133_Before_Snapshot,
    PNULL,//GC6133_After_Snapshot,
	GC6133_set_flash_enable,

    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    Set_GC6133_AWB,    
    PNULL,
    PNULL, 
    Set_GC6133_Ev, 
    PNULL, 
    PNULL, 
    PNULL,
    
    PNULL,
    PNULL,
    Set_GC6133_Anti_Flicker,
    Set_GC6133_Video_Mode,   
    PNULL,
    PNULL,
};
#endif
 

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
PUBLIC const SENSOR_INFO_T g_gc6133_yuv_info =
{
	GC6133_I2C_ADDR_W,				// salve i2c write address
	GC6133_I2C_ADDR_R, 				// salve i2c read address
	
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
	
	SENSOR_LOW_PULSE_RESET,		// reset pulse level
	100,								// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	2,								// count of identify code 
	0xf0, 0xba,                     // supply two code to identify sensor.
	0xf0, 0xba,                     // for Example: index = 0-> Device id, index = 1 -> version id                                          
											
	SENSOR_AVDD_2800MV,				// voltage of avdd	
	
	240,							// max width of source image
	320,							// max height of source image
	"GC6133",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_UYVY,	// pattern of input image form sensor;			

	s_GC6133_resolution_Tab_YUV,	// point to resolution table information structure
	&s_GC6133_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_1800MV,                     // iovdd
	SENSOR_AVDD_UNUSED,                      // dvdd
	4,                     // skip frame num before preview 
	3,                     // skip frame num before capture		
	0,                     // deci frame num during preview;		
	2,                     // deci frame num during video preview;
	0,                     // threshold enable
	0,                     // threshold mode
	0,                     // threshold start postion	
	0,                     // threshold end postion 
	SENSOR_OUTPUT_MODE_CCIR656_1BIT,
	SENSOR_OUTPUT_ENDIAN_BIG
};

/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void GC6133_WriteReg( uint8  subaddr, uint8 data )
{
	
	#ifndef	_USE_DSP_I2C_
		uint8 cmd[2];
		cmd[0]	=	subaddr;
		cmd[1]	=	data;		
		//I2C_WriteCmdArr(GC6133_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg(subaddr, data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW("SENSOR: GC6133_WriteReg reg/value(%x,%x) !!", subaddr, data);

}

LOCAL uint8 GC6133_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
		//I2C_WriteCmdArr(GC6133_I2C_ADDR_W, &subaddr, 1, SCI_TRUE);
		//I2C_ReadCmd(GC6133_I2C_ADDR_R, &value, SCI_TRUE);
		value =Sensor_ReadReg( subaddr);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif

	//SCI_TRACE_LOW("SENSOR: GC6133_ReadReg reg/value(%x,%x) !!", subaddr, value);
    
	return value;
}


LOCAL void GC6133_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

}

LOCAL void GC6133_Delayms (uint32 ms)	
{
	uint32 t1, t2;
	
	t1 = t2 = SCI_GetTickCount ();
	
	do{
		t2 = SCI_GetTickCount ();
	}while (t2 < (t1+ms));
}


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL uint32 GC6133_Power_On(uint32 power_on)
{
	SENSOR_AVDD_VAL_E		dvdd_val=g_gc6133_yuv_info.dvdd_val;
	SENSOR_AVDD_VAL_E		avdd_val=g_gc6133_yuv_info.avdd_val;
	SENSOR_AVDD_VAL_E		iovdd_val=g_gc6133_yuv_info.iovdd_val;	
	BOOLEAN 				power_down=g_gc6133_yuv_info.power_down_level;		
	
	if(SCI_TRUE==power_on)
	{
		Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
		//Sensor_SetVoltage(dvdd_val, SENSOR_AVDD_1800MV,SENSOR_AVDD_1800MV );
		SCI_Sleep(20);
		//Sensor_SetVoltage(dvdd_val, SENSOR_AVDD_2500MV,SENSOR_AVDD_2500MV );
		//SCI_Sleep(20);
		//Sensor_SetVoltage(dvdd_val, SENSOR_AVDD_2800MV, SENSOR_AVDD_2800MV);	
		// Open Mclk in default frequency
		Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);	// Default MCLK=12Mhz
		SCI_Sleep(100);
		if(SENSOR_MAIN==Sensor_GetCurId())
		{
			GPIO_SetFrontSensorPwdn(power_down);	
						
			GPIO_SetSensorPwdn(!power_down);
			SCI_Sleep(20);
			GPIO_SetSensorPwdn(power_down);
			SCI_Sleep(20);
			GPIO_SetSensorPwdn(!power_down);		
		}
		else
		{
			GPIO_SetSensorPwdn(power_down);
			GPIO_SetFrontSensorPwdn(!power_down);
			SCI_Sleep(20);
			GPIO_SetFrontSensorPwdn(power_down);	
			SCI_Sleep(20);
			GPIO_SetFrontSensorPwdn(!power_down);		
		}
	}
	else
	{
		GPIO_SetSensorPwdn(power_down);
		GPIO_SetFrontSensorPwdn(power_down);
		SCI_Sleep(20);

		Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);		  
		SCI_Sleep(20);

		Sensor_SetMCLK(SENSOR_DISABLE_MCLK);		   
	}
	
	SCI_TRACE_LOW("set_GC6133_Power_On");
	return SCI_SUCCESS;
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
LOCAL uint32 GC6133_Identify(uint32 param)
{
    uint8 reg[2]    = {0xf0, 0xf0};
    uint8 value[2]  = {0xba, 0xba};
    uint8 ret       = 0;
    uint32 i;
    uint8   err_cnt = 0;
    uint32 nLoop = 1000;
	
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = GC6133_ReadReg(reg[i]);    
	  g_gc6133_indenty_id[i] = ret;	
	 SCI_TRACE_LOW( "SENSOR: GC6133 Read reg0x00 = %x",ret );
		
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
	             SCI_TRACE_LOW( "GC6133 Fail" );
	             return SCI_ERROR;
            }
            else
            {
                while(nLoop--);
                continue;
            }
        }
        err_cnt = 0;
        i++;
    }

	SCI_TRACE_LOW( "GC6133 Identify" );
    ////sjtc_camera_pix8=1;
    return (uint32)SCI_SUCCESS;

}


LOCAL SENSOR_TRIM_T s_GC6133_Pclk_Tab[]=
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

LOCAL uint32 GC6133_GetPclkTab(uint32 param)
{
    return (uint32)s_GC6133_Pclk_Tab;
}


__align(4) const SENSOR_REG_T GC6133_brightness_tab[][2]=
{
	{{0x85, 0x01},   {0xff,0xff},},
	{{0x85, 0x03},   {0xff,0xff},},
	{{0x85, 0x05},   {0xff,0xff},},
	{{0x85, 0x06},   {0xff,0xff},},
	{{0x85, 0x0c},   {0xff,0xff},},
	{{0x85, 0x14},   {0xff,0xff},},
	{{0x85, 0x20},   {0xff,0xff},},
};

LOCAL uint32 Set_GC6133_Brightness(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6133_brightness_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    GC6133_Write_Group_Regs(sensor_reg_ptr);
	
    SCI_TRACE_LOW("set_GC6133_brightness: level = %d", level);
    
    return 0;
}



__align(4) const SENSOR_REG_T GC6133_contrast_tab[][2]=
{
	{{0x83,0x38}, {0xff,0xff}},
	{{0x83,0x40}, {0xff,0xff}},
	{{0x83,0x44}, {0xff,0xff}},
	{{0x83,0x48}, {0xff,0xff}},
	{{0x83,0x4a}, {0xff,0xff}},
	{{0x83,0x50}, {0xff,0xff}},
	{{0x83,0x54}, {0xff,0xff}},  
};

LOCAL uint32 Set_GC6133_Contrast(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6133_contrast_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    GC6133_Write_Group_Regs(sensor_reg_ptr);
	
    SCI_TRACE_LOW("set_GC6133_contrast: level = %d", level);
    
    return 0;
}


__align(4) const SENSOR_REG_T GC6133_image_effect_tab[][4]=	
{
	// effect normal
	{ {0x23,0x00}, {0x8a,0x00}, {0x8b,0x00}, {0xff,0xff} },
	//effect BLACKWHITE
	{ {0x23,0x02}, {0x8a,0x00}, {0x8b,0x00}, {0xff,0xff} },
	// effect RED pink
	{ {0x23,0x02}, {0x8a,0x10}, {0x8b,0x50}, {0xff,0xff} },
	// effect GREEN
	{ {0x23,0x02}, {0x8a,0xe0}, {0x8b,0xe0}, {0xff,0xff} },
	// effect  BLUE
	{ {0x23,0x02}, {0x8a,0x50}, {0x8b,0xe0}, {0xff,0xff} },
	// effect  YELLOW
	{{0x23,0x02}, {0x8a,0x80}, {0x8b,0x20}, {0xff,0xff}  },  
	// effect NEGATIVE
	{ {0x23,0x01}, {0x8a,0x00}, {0x8b,0x00}, {0xff,0xff} },    
	//effect ANTIQUE
	{{0x23,0x02}, {0x8a,0xd0}, {0x8b,0x28}, {0xff,0xff} },
};

LOCAL uint32 Set_GC6133_Image_Effect(uint32 effect_type)
{
   
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6133_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

    GC6133_Write_Group_Regs(sensor_reg_ptr);
	
    SCI_TRACE_LOW("set_GC6133_image_effect: effect_type = %d", effect_type);
    
    return 0;
}



__align(4) const SENSOR_REG_T GC6133_ev_tab[][2]=
{   
    {{0x92, 0x28},{0xff, 0xff}},
    {{0x92, 0x30},{0xff, 0xff}},
    {{0x92, 0x38},{0xff, 0xff}},
    {{0x92, 0x3e},{0xff, 0xff}},
    {{0x92, 0x48},{0xff, 0xff}},
    {{0x92, 0x50},{0xff, 0xff}},
    {{0x92, 0x58},{0xff, 0xff}},  
};

LOCAL uint32 Set_GC6133_Ev(uint32 level)
{
  
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6133_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    SCI_ASSERT(level < 7);
 
    GC6133_Write_Group_Regs(sensor_reg_ptr );

    SCI_TRACE_LOW("set_GC6133_ev: level = %d", level);

    return 0;
}

/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
LOCAL uint32 Set_GC6133_Anti_Flicker(uint32 mode)
{ 
	switch(mode)
	{
	case DCAMERA_FLICKER_50HZ:

		bl_GC_50Hz_GC6133 = TRUE;
		GC6133_WriteReg(0xfe, 0x00);
		GC6133_WriteReg(0x01, 0x41); //hb
		GC6133_WriteReg(0x02, 0x12); //vb
		GC6133_WriteReg(0x0f, 0x01);
		GC6133_WriteReg(0x9d, 0x32); //step
		GC6133_WriteReg(0x9e, 0x61); //[7:4]margin  10fps
		GC6133_WriteReg(0x9f, 0xf4); //1f4

	break;

	case DCAMERA_FLICKER_60HZ:

		bl_GC_50Hz_GC6133 = FALSE;
		GC6133_WriteReg(0xfe, 0x00);
		GC6133_WriteReg(0x01, 0x3c); //hb
		GC6133_WriteReg(0x02, 0x04); //vb
		GC6133_WriteReg(0x0f, 0x01);
		GC6133_WriteReg(0x9d, 0x2a); //step
		GC6133_WriteReg(0x9e, 0x61); //[7:4]margin 10fps
		GC6133_WriteReg(0x9f, 0xce); 
	break;

	    default:
	    break;
    }
      
    SCI_TRACE_LOW("set_GC6133_anti_flicker-mode=%d",mode);
    OS_TickDelay(100); 
	
    return 0;
}

LOCAL uint32 Set_GC6133_Preview_Mode(uint32 preview_mode)
{
/*
		if(SENSOR_SUB==Sensor_GetCurId())
		{
				GC6133_WriteReg(0x14, 0x55);
		}
		else
		{
				GC6133_WriteReg(0x14, 0x54);
		}
*/
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		case DCAMERA_ENVIRONMENT_NIGHT:
		default:
			break;
		
	}
	SCI_TRACE_LOW("set_GC6133_preview_mode: level = %d", preview_mode);
#ifdef GC6133_LOAD_FROM_T_FLASH  
 	Load_GC6133_RegTab_From_T_Flash();
#endif	
	return 0;
}


__align(4) const SENSOR_REG_T GC6133_awb_tab[][5]=
{
    //AUTO
    {
        {0x22, 0xf8},    // the reg value is not written here, rewrite in set_GC6133_awb();
        {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
        {0x22, 0xb8},    // Disable AWB 
        {0x49, 0x48},
        {0x4a, 0x40},
        {0x4b, 0x5c},
        {0xff, 0xff} 
    },
    //U30
    {
        {0x22, 0xb8},   // Disable AWB 
        {0x49, 0x40},
        {0x4a, 0x54},
        {0x4b, 0x70},
        {0xff, 0xff} 
    },  
    //CWF  //
    {
        {0x22, 0xb8},   // Disable AWB 
        {0x49, 0x40},
        {0x4a, 0x54},
        {0x4b, 0x70},
        {0xff, 0xff} 
    },    
    //FLUORESCENT:
    {
        {0x22, 0xb8},   // Disable AWB 
        {0x49, 0x40},
        {0x4a, 0x42}, 
        {0x4b, 0x50},
        {0xff, 0xff} 
    },
    //SUN:
    {
        {0x22, 0xb8},   // Disable AWB 
        {0x49, 0x50},
        {0x4a, 0x45},
        {0x4b, 0x40},
        {0xff, 0xff} 
    },
    //CLOUD:
    {
        {0x22, 0xb8},   // Disable AWB 
        {0x49, 0x5a},
        {0x4a, 0x42},
        {0x4b, 0x40},
        {0xff, 0xff} 
    },
};

LOCAL uint32 Set_GC6133_AWB(uint32 mode)
{
	
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6133_awb_tab[mode];

	SCI_ASSERT(mode < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	GC6133_Write_Group_Regs(sensor_reg_ptr);
	
	SCI_TRACE_LOW("set_GC6133_awb_mode: mode = %d", mode);
	
	return 0;
}


/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//		 
/******************************************************************************/   
LOCAL uint32 Set_GC6133_Video_Mode(uint32 mode)
{
	
	SCI_ASSERT(mode <=DCAMERA_MODE_MAX);

	if(1==mode ||2==mode )
	{
		if(bl_GC_50Hz_GC6133 == TRUE)  // is 50hz in video
		{
		GC6133_WriteReg(0xfe, 0x00);
		GC6133_WriteReg(0x01, 0x41); //hb
		GC6133_WriteReg(0x02, 0x12); //vb
		GC6133_WriteReg(0x0f, 0x01);
		GC6133_WriteReg(0x9d, 0x32); //step
		GC6133_WriteReg(0x9e, 0x61); //[7:4]margin  10fps
		GC6133_WriteReg(0x9f, 0xf4); 
	}
	else  // is 60hz in video mode
	{
		GC6133_WriteReg(0xfe, 0x00);
		GC6133_WriteReg(0x01, 0x3c); //hb
		GC6133_WriteReg(0x02, 0x04); //vb
		GC6133_WriteReg(0x0f, 0x01);
		GC6133_WriteReg(0x9d, 0x2a); //step
		GC6133_WriteReg(0x9e, 0x61); //[7:4]margin 10fps
		GC6133_WriteReg(0x9f, 0xce); 
	}
	SCI_TRACE_LOW("set_GC6133_video_mode=%d",mode);
	}	
	
	return 0;
}

#ifdef GC6133_LOAD_FROM_T_FLASH  
__align(4) static SENSOR_REG_T GC6133_YUV_Init_Reg[1000] = {{0x00,0x00},}; 
//#define READ_BUFFER_SIZE  (274*12)    //must be divisible by 12
LOCAL uint32 Load_GC6133_RegTab_From_T_Flash(void)
{
    SFS_HANDLE    file_handle = 0;
    FFS_ERROR_E   ffs_err = FFS_NO_ERROR;
  //  char *file_name = "C:\\GC6133_Initialize_Setting.Bin";
   	static wchar unicode_file_name[256] = {0};
    int regNum = 0; //uint32 regNo = 0;
    int i = 0;  //uint32 i = 0;
    uint8 *curr_ptr = NULL;
    uint8 *buffer_ptr = NULL;  //char buffer_ptr[READ_BUFFER_SIZE] = {0};
    uint32 read_size = 0;
	uint32 file_size = 0;
    uint8 func_ind[3] = {0};  /* REG or DLY */
   // for(i=0;i<25;i++)
       // unicode_file_name[i] = file_name[i];

	SCI_MEM16CPY(unicode_file_name,L"E:\\GC6133_Initialize_Setting.Bin",sizeof(L"E:\\GC6133_Initialize_Setting.Bin"));
   

	/* Search the setting file in all of the user disk. */
	#if 0
	curr_ptr = (uint8 *)unicode_file_name;
	while (file_handle == 0)
    {
        if ((*curr_ptr >= 'c' && *curr_ptr <= 'z') || (*curr_ptr >= 'C' && *curr_ptr <= 'Z'))
        {
			file_handle = SFS_CreateFile((const uint16 *)unicode_file_name, 0x0030|SFS_MODE_READ, NULL, NULL);	//	FFS_MODE_OPEN_EXISTING
            if (file_handle > 0)
            {
                break; /* Find the setting file. */
            }
            *curr_ptr = *curr_ptr + 1;
        }
        else
        {
            break ;
        }
    }
	#else
	file_handle = SFS_CreateFile((const uint16 *)unicode_file_name, 0x0030|SFS_MODE_READ, NULL, NULL);	//	FFS_MODE_OPEN_EXISTING
	#endif
	
	if(file_handle == 0) //read file error
    {
		SCI_TRACE_LOW("!!! Warning, Can't find the initial setting file!!!");
		return SCI_ERROR;
    }


	SFS_GetFileSize(file_handle,&file_size);
	if(file_size < 10)
	{
		SCI_TRACE_LOW("!!! Warning, Invalid setting file!!!");
		return SCI_ERROR;
	}
	
	buffer_ptr =SCI_ALLOCA(file_size);///
	
	if (buffer_ptr == NULL)
	{
    	SCI_TRACE_LOW("!!! Warning, Memory not enoughed...");
    	return SCI_ERROR;        /* Memory not enough */
	}
    ffs_err = SFS_ReadFile(file_handle, buffer_ptr, file_size, &read_size, NULL);
	curr_ptr = buffer_ptr;
    if(SFS_NO_ERROR == ffs_err)
    {
		while(curr_ptr < (buffer_ptr + read_size))
        {     
        	while ((*curr_ptr == ' ') || (*curr_ptr == '\t')) /* Skip the Space & TAB */
        		curr_ptr++;
			if (((*curr_ptr) == '/') && ((*(curr_ptr + 1)) == '*'))
   			{
   	    		 while (!(((*curr_ptr) == '*') && ((*(curr_ptr + 1)) == '/')))
       		 	{
            		curr_ptr++;    /* Skip block comment code. */
       		 	}
        		while (!((*curr_ptr == 0x0D) && (*(curr_ptr+1) == 0x0A)))
        		{
            		curr_ptr++;
        		}
        		curr_ptr += 2;            /* Skip the enter line */
        		continue ;
    		}
			if (((*curr_ptr) == '/') && ((*(curr_ptr+1)) == '/'))   /* Skip // block comment code. */
    		{
        		while (!((*curr_ptr == 0x0D) && (*(curr_ptr+1) == 0x0A)))
        		{
            		curr_ptr++;
        		}
        		curr_ptr += 2;            /* Skip the enter line */
        		continue ;
   			}

			/* This just content one enter line. */
    		if (((*curr_ptr) == 0x0D) && ((*(curr_ptr + 1)) == 0x0A))
    		{
        		curr_ptr += 2;
        		continue ;
    		}
			SCI_MEMCPY(func_ind, curr_ptr,3);
			curr_ptr += 4;  
			if (strcmp((const char *)func_ind, "REG") == 0)    /* REG */
			{
            GC6133_YUV_Init_Reg[regNum].reg_addr = (uint16)strtol(curr_ptr, NULL, 16);
            curr_ptr += 5;
            GC6133_YUV_Init_Reg[regNum].reg_value = (uint16)strtol(curr_ptr, NULL, 16);
            //strNum += 5;
			
			}
			regNum++;	
			/* Skip to next line directly. */
    		while (!((*curr_ptr == 0x0D) && (*(curr_ptr+1) == 0x0A)))
    		{
        		curr_ptr++;
    		}		
            curr_ptr += 2;    
        }
    }
	GC6133_YUV_Init_Reg[regNum].reg_addr = 0xff;
	GC6133_YUV_Init_Reg[regNum].reg_value =0xff;
	
	SCI_TRACE_LOW("%d register read...", i);
 
	SCI_FREE(buffer_ptr);
 	buffer_ptr = NULL;	
    ffs_err = SFS_CloseFile( file_handle);
	file_handle = NULL;

	SCI_TRACE_LOW("Start apply initialize setting.");
    /* Start apply the initial setting to sensor. */
	SCI_ASSERT(PNULL != GC6133_YUV_Init_Reg);
    GC6133_Write_Group_Regs(GC6133_YUV_Init_Reg);

	return SCI_SUCCESS;
}
#endif
#endif

