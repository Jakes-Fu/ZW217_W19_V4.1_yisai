/******************************************************************************
 ** File Name:    Sensor_GC0310.c                                             *
 ** Author:       Eddy.wei                                                    *
 ** Date:         2016/05/02                                                  *
 ** Platform:     SC6531EFM                                                      *
 ** Copyright:    Spreadtrum All Rights Reserved.                             *
 ** Description:  implementation of digital camera register interface         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE             NAME             DESCRIPTION                             *
 ** 2016/05/02   Ededy.wei       Fristly Released                             *
 ******************************************************************************/
#ifndef _GC0310_C_
#define _GC0310_C_

#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
//#include "i2c_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "gpio_prod_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*lint -save -e785 */
/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define GC0310_I2C_ADDR_W               0x42
#define GC0310_I2C_ADDR_R               0x43

/**---------------------------------------------------------------------------*
     GC0310 SUPPORT 10 SERIAL MODE:
     SPI, 
     CCIR656_1BIT, CCIR656_2BIT, CCIR656_4BIT
     PACKET_1BIT, PACKET_2BIT, PACKET_4BIT
     PACKET_DDR_1BIT, PACKET_DDR_2BIT, PACKET_DDR_4BIT
 **---------------------------------------------------------------------------*/


//#define GC0310_OUTPUT_MODE_CCIR656_2BIT
#define GC0310_OUTPUT_MODE_PACKET_DDR_2BIT 

//#define GC0310_LOAD_FROM_T_FLASH 

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void   GC0310_WriteReg(uint8 subaddr, uint8 data );
LOCAL uint8  GC0310_ReadReg(uint8 subaddr);
LOCAL void   GC0310_Write_Group_Regs(SENSOR_REG_T* sensor_reg_ptr );
LOCAL uint32 GC0310_Power_On(uint32 power_on);
LOCAL uint32 GC0310_Identify(uint32 param);
LOCAL uint32 GC0310_Set_Brightness(uint32 level);
LOCAL uint32 GC0310_Set_Contrast(uint32 level);
LOCAL uint32 GC0310_Set_Image_Effect(uint32 effect_type);
LOCAL uint32 GC0310_Set_Ev(uint32 level);
LOCAL uint32 GC0310_Set_Anti_Flicker(uint32 mode);
LOCAL uint32 GC0310_Set_Preview_Mode(uint32 preview_mode);
LOCAL uint32 GC0310_Set_AWB(uint32 mode);
LOCAL uint32 GC0310_Set_Video_Mode(uint32 mode);
LOCAL uint32 GC0310_Before_Snapshot(uint32 para);
LOCAL uint32 GC0310_After_Snapshot(uint32 para);
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 s_preview_mode = DCAMERA_ENVIRONMENT_NORMAL;
LOCAL uint32 s_anti_flicker = DCAMERA_FLICKER_50HZ;

#ifdef GC0310_LOAD_FROM_T_FLASH  
#include "sfs.h"
#include "ffs.h"
LOCAL uint32 Load_GC0310_RegTab_From_T_Flash(void);
#endif

        
#if defined(GC0310_OUTPUT_MODE_CCIR656_2BIT)
__align(4) const SENSOR_REG_T GC0310_YUV_640X480[] = 
{
	//GC0310_OUTPUT_MODE_CCIR656_2BIT	
	/////////////////////////////////////////////////
	/////////////////   system reg  /////////////////
	/////////////////////////////////////////////////
	{0xfe,0xf0},
	{0xfe,0xf0},
	{0xfe,0x00},

	{0xfc,0x16}, //4e 
	{0xfc,0x16}, //4e // [0]apwd [6]regf_clk_gate 
	{0xf2,0x07}, //sync output
	{0xf3,0x83}, //ff//1f//01 data output
	{0xf5,0x07}, //sck_dely

	{0xf7,0x88}, //f8//fd   88
	{0xf8,0x00},           // 00
	{0xf9,0x4f}, //0f//01   4d
	{0xfa,0x32}, //32

	{0xfc,0xce},
	{0xfd,0x00},
	/////////////////////////////////////////////////
	/////////////////   CISCTL reg  /////////////////
	/////////////////////////////////////////////////
	{0x00,0x2f}, 
	{0x01,0x0f}, 
	{0x02,0x04},

	{0x03,0x02},
	{0x04,0x12},

	{0x09,0x00}, 
	{0x0a,0x00}, 
	{0x0b,0x00}, 
	{0x0c,0x04}, 
	{0x0d,0x01}, 
	{0x0e,0xe8}, 
	{0x0f,0x02}, 
	{0x10,0x88}, 
	{0x16,0x00},
	{0x17,0x14},
	{0x18,0x1a}, 
	{0x19,0x14}, 
	{0x1b,0x48},
	{0x1c,0x1c},
	{0x1e,0x6b},
	{0x1f,0x28},
	{0x20,0x8b},//0x89 travis20140801
	{0x21,0x49},
	{0x22,0xb0},
	{0x23,0x04}, 
	{0x24,0xff}, 
	{0x34,0x20}, 


	/////////////////////////////////////////////////
	////////////////////   BLK   ////////////////////
	/////////////////////////////////////////////////
	{0x26,0x23},
	{0x28,0xff},
	{0x29,0x00},
	{0x32,0x00},
	{0x33,0x10}, 
	{0x37,0x20},
	{0x38,0x10},
	{0x47,0x80},
	{0x4e,0x66},
	{0xa8,0x02},
	{0xa9,0x80},


	/////////////////////////////////////////////////
	//////////////////   ISP reg  ///////////////////
	/////////////////////////////////////////////////
	{0x40,0xff},
	{0x41,0x21},
	{0x42,0xcf},
	{0x44,0x02},
	{0x45,0xa8}, 
	{0x46,0x02}, //sync
	{0x4a,0x11},
	{0x4b,0x01},
	{0x4c,0x20},
	{0x4d,0x05},
	{0x4f,0x01},
	{0x50,0x01},
	{0x55,0x01},
	{0x56,0xe0},
	{0x57,0x02},
	{0x58,0x80},
	
	/////////////////////////////////////////////////
	///////////////////   GAIN   ////////////////////
	/////////////////////////////////////////////////
	{0x70,0x70},
	{0x5a,0x84},
	{0x5b,0xc9},
	{0x5c,0xed},
	{0x77,0x74},
	{0x78,0x40},
	{0x79,0x5f}, 


	///////////////////////////////////////////////// 
	///////////////////   DNDD  /////////////////////
	///////////////////////////////////////////////// 
	{0x82,0x14}, 
	{0x83,0x0b},
	{0x89,0xf0},
	///////////////////////////////////////////////// 
	//////////////////   EEINTP  ////////////////////
	///////////////////////////////////////////////// 
	{0x8f,0xaa},
	{0x90,0x8c},
	{0x91,0x90},
	{0x92,0x03},
	{0x93,0x03},
	{0x94,0x05},
	{0x95,0x65}, 
	{0x96,0xf0}, 
	///////////////////////////////////////////////// 
	/////////////////////  ASDE  ////////////////////
	///////////////////////////////////////////////// 
	{0xfe,0x00},

	{0x9a,0x20},
	{0x9b,0x80},
	{0x9c,0x40},
	{0x9d,0x80},
	 
	{0xa1,0x30},
 	{0xa2,0x32},
	{0xa4,0x30},
	{0xa5,0x30},
	{0xaa,0x10}, 
	{0xac,0x22},
	 

	/////////////////////////////////////////////////
	///////////////////   GAMMA   ///////////////////
	/////////////////////////////////////////////////
	{0xfe,0x00},//default
	{0xbf,0x08},
	{0xc0,0x16},
	{0xc1,0x28},
	{0xc2,0x41},
	{0xc3,0x5a},
	{0xc4,0x6c},
	{0xc5,0x7a},
	{0xc6,0x96},
	{0xc7,0xac},
	{0xc8,0xbc},
	{0xc9,0xc9},
	{0xca,0xd3},
	{0xcb,0xdd},
	{0xcc,0xe5},
	{0xcd,0xf1},
	{0xce,0xfa},
	{0xcf,0xff},
	
/* 
	{0xfe,0x00},//big gamma
	{0xbf,0x08},
	{0xc0,0x1d},
	{0xc1,0x34},
	{0xc2,0x4b},
	{0xc3,0x60},
	{0xc4,0x73},
	{0xc5,0x85},
	{0xc6,0x9f},
	{0xc7,0xb5},
	{0xc8,0xc7},
	{0xc9,0xd5},
	{0xca,0xe0},
	{0xcb,0xe7},
	{0xcc,0xec},
	{0xcd,0xf4},
	{0xce,0xfa},
	{0xcf,0xff},
*/	

/*
	{0xfe,0x00},//small gamma
	{0xbf,0x08},
	{0xc0,0x18},
	{0xc1,0x2c},
	{0xc2,0x41},
	{0xc3,0x59},
	{0xc4,0x6e},
	{0xc5,0x81},
	{0xc6,0x9f},
	{0xc7,0xb5},
	{0xc8,0xc7},
	{0xc9,0xd5},
	{0xca,0xe0},
	{0xcb,0xe7},
	{0xcc,0xec},
	{0xcd,0xf4},
	{0xce,0xfa},
	{0xcf,0xff},
*/
	/////////////////////////////////////////////////
	///////////////////   YCP  //////////////////////
	/////////////////////////////////////////////////
	{0xd0,0x40}, 
	{0xd1,0x34}, 
	{0xd2,0x34}, 
	{0xd3,0x40}, 
	{0xd6,0xf2}, 
	{0xd7,0x1b}, 
	{0xd8,0x18}, 
	{0xdd,0x03}, 

	/////////////////////////////////////////////////
	////////////////////   AEC   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01},
	{0x05,0x30},
	{0x06,0x75},
	{0x07,0x40},
	{0x08,0xb0},
	{0x0a,0xc5},
	{0x0b,0x11},
	{0x0c,0x00},
	{0x12,0x52},
	{0x13,0x38},
	{0x18,0x95},
	{0x19,0x96},
	{0x1f,0x20}, 
	{0x20,0xc0}, 
	{0x3e,0x40}, 
	{0x3f,0x57}, 
	{0x40,0x7d}, 

	{0x03,0x60}, 

	{0x44,0x02}, 
	/////////////////////////////////////////////////
	////////////////////   AWB   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01},
	{0x1c,0x91}, 
	{0x21,0x15}, 
	{0x50,0x80},
	{0x56,0x04},
	{0x59,0x08}, 
	{0x5b,0x02},
	{0x61,0x8d}, 
	{0x62,0xa7}, 
	{0x63,0xd0}, 
	{0x65,0x06},
	{0x66,0x06}, 
	{0x67,0x84}, 
	{0x69,0x08}, 
	{0x6a,0x25}, 
	{0x6b,0x01}, 
	{0x6c,0x00}, 
	{0x6d,0x02}, 
	{0x6e,0xf0}, 
	{0x6f,0x80}, 
	{0x76,0x80}, 
	{0x78,0xaf}, 
	{0x79,0x75},
	{0x7a,0x40},
	{0x7b,0x50},
	{0x7c,0x0c}, 

	{0x90,0xc9},//stable AWB 
	{0x91,0xbe},
	{0x92,0xe2},
	{0x93,0xc9},
	{0x95,0x1b},
	{0x96,0xe2},
	{0x97,0x49},
	{0x98,0x1b},
	{0x9a,0x49},
	{0x9b,0x1b},
	{0x9c,0xc3},
	{0x9d,0x49},
	{0x9f,0xc7},
	{0xa0,0xc8},
	{0xa1,0x00},
	{0xa2,0x00},
	{0x86,0x00},
	{0x87,0x00},
	{0x88,0x00},
	{0x89,0x00},
	{0xa4,0xb9},
	{0xa5,0xa0},
	{0xa6,0xba},
	{0xa7,0x92},
	{0xa9,0xba},
	{0xaa,0x80},
	{0xab,0x9d},
	{0xac,0x7f},
	{0xae,0xbb},
	{0xaf,0x9d},
	{0xb0,0xc8},
	{0xb1,0x97},
	{0xb3,0xb7},
	{0xb4,0x7f},
	{0xb5,0x00},
	{0xb6,0x00},
	{0x8b,0x00},
	{0x8c,0x00},
	{0x8d,0x00},
	{0x8e,0x00},
	{0x94,0x55},
	{0x99,0xa6},
	{0x9e,0xaa},
	{0xa3,0x0a},
	{0x8a,0x00},
	{0xa8,0x55},
	{0xad,0x55},
	{0xb2,0x55},
	{0xb7,0x05},
	{0x8f,0x00},
	{0xb8,0xcb},
	{0xb9,0x9b},  
	
/*	 
	{0xa4,0xb9}, //default AWB
	{0xa5,0xa0},
	{0x90,0xc9}, 
	{0x91,0xbe},                                 
	{0xa6,0xb8}, 
	{0xa7,0x95}, 
	{0x92,0xe6}, 
	{0x93,0xca},                                
	{0xa9,0xbc}, 
	{0xaa,0x95}, 
	{0x95,0x23}, 
	{0x96,0xe7},                                  
	{0xab,0x9d}, 
	{0xac,0x80},
	{0x97,0x43}, 
	{0x98,0x24},                                
	{0xae,0xb7}, 
	{0xaf,0x9e}, 
	{0x9a,0x43},
	{0x9b,0x24},                                 
	{0xb0,0xc8}, 
	{0xb1,0x97},
	{0x9c,0xc4}, 
	{0x9d,0x44},                               
	{0xb3,0xb7}, 
	{0xb4,0x7f},
	{0x9f,0xc7},
	{0xa0,0xc8},                               
	{0xb5,0x00}, 
	{0xb6,0x00},
	{0xa1,0x00},
	{0xa2,0x00},                         
	{0x86,0x60},
	{0x87,0x08},
	{0x88,0x00},
	{0x89,0x00},
	{0x8b,0xde},
	{0x8c,0x80},
	{0x8d,0x00},
	{0x8e,0x00},                               
	{0x94,0x55},
	{0x99,0xa6},
	{0x9e,0xaa},
	{0xa3,0x0a},
	{0x8a,0x0a},
	{0xa8,0x55},
	{0xad,0x55},
	{0xb2,0x55},
	{0xb7,0x05},
	{0x8f,0x05},                              
	{0xb8,0xcc},
	{0xb9,0x9a},
*/  

	/////////////////////////////////////////////////
	////////////////////  CC ////////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01},
	
	{0xd0,0x38},//skin red
	{0xd1,0x00},
	{0xd2,0x02},
	{0xd3,0x04},
	{0xd4,0x38},
	{0xd5,0x12},	
/*                     
	{0xd0,0x38},//skin white
	{0xd1,0xfd},
	{0xd2,0x06},
	{0xd3,0xf0},
	{0xd4,0x40},
	{0xd5,0x08},
*/
	
/*                       
	{0xd0,0x38},//guodengxiang
	{0xd1,0xf8},
	{0xd2,0x06},
	{0xd3,0xfd},
	{0xd4,0x40},
	{0xd5,0x00},	
*/
	{0xd6,0x30},
	{0xd7,0x00},
	{0xd8,0x0a},
	{0xd9,0x16},
	{0xda,0x39},
	{0xdb,0xf8},
	/////////////////////////////////////////////////
	////////////////////   LSC   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01}, 
	{0xc1,0x3c},
	{0xc2,0x50},
	{0xc3,0x00},
	{0xc4,0x40},
	{0xc5,0x30},
	{0xc6,0x30},
	{0xc7,0x10},
	{0xc8,0x00},
	{0xc9,0x00},
	{0xdc,0x20},
	{0xdd,0x10},
	{0xdf,0x00}, 
	{0xde,0x00}, 


	/////////////////////////////////////////////////
	///////////////////  Histogram  /////////////////
	/////////////////////////////////////////////////
	{0x01,0x10}, 
	{0x0b,0x31}, 
	{0x0e,0x50}, 
	{0x0f,0x0f}, 
	{0x10,0x6e}, 
	{0x12,0xa0}, 
	{0x15,0x60}, 
	{0x16,0x60}, 
	{0x17,0xe0}, 


	/////////////////////////////////////////////////
	//////////////   Measure Window   ///////////////
	/////////////////////////////////////////////////
	{0xcc,0x0c}, 
	{0xcd,0x10}, 
	{0xce,0xa0}, 
	{0xcf,0xe6}, 


	/////////////////////////////////////////////////
	/////////////////   dark sun   //////////////////
	/////////////////////////////////////////////////
	{0x45,0xf7},
	{0x46,0xff}, 
	{0x47,0x15},
	{0x48,0x03}, 
	{0x4f,0x60}, 
                                
	/////////////////////////////////////////////////
	///////////////////  banding  ///////////////////
	/////////////////////////////////////////////////
	{0xfe,0x00},
	{0x05,0x01},
	{0x06,0x12}, //HB
	{0x07,0x00},
	{0x08,0x1c}, //VB
	{0xfe,0x01},
	{0x25,0x00}, //step 
	{0x26,0x1f}, 
	{0x27,0x01}, //6fps
	{0x28,0xf0},  
	{0x29,0x01}, //6fps
	{0x2a,0xf0}, 
	{0x2b,0x01}, //6fps
	{0x2c,0xf0}, 
	{0x2d,0x03}, //3.3fps
	{0x2e,0xe0},
	{0x3c,0x20},
	/////////////////////  SPI   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x03},
	{0x01,0x00},
	{0x02,0x00},
	{0x10,0x00},
	{0x15,0x00},
	{0x17,0x00}, //01//03
	{0x04,0x10},//fifo full level
	{0x05,0x00},
	{0x40,0x00},

	{0x52,0x02}, //a2 //a0//80//00

	{0x53,0x24}, //24
	{0x54,0x20},
	{0x55,0x20}, //QQ//01
	{0x59,0x1f},//hechang
	{0x5a,0x00}, //00 //yuv 
	{0x5b,0x80},
	{0x5c,0x02},
	{0x5d,0xe0},
	{0x5e,0x01},
	{0x51,0x03},
	{0x60,0xb6},
	{0x61,0x80},
	{0x62,0x9d},
	{0x63,0xb6},
	{0x64,0x07},
	{0x65,0xff},
	{0x66,0x00},
	{0x67,0x00},
	{0xfe,0x00},
};             	     
#elif defined(GC0310_OUTPUT_MODE_PACKET_DDR_2BIT)
__align(4) const SENSOR_REG_T GC0310_YUV_640X480[] = 
{
	   //GC0310_OUTPUT_MODE_PACKET_DDR_2BIT
	//VGA setting	
	/////////////////////////////////////////////////
	/////////////////   system reg  /////////////////
	/////////////////////////////////////////////////
	{0xfe,0xf0},
	{0xfe,0xf0},
	{0xfe,0x00},

	{0xfc,0x16}, //4
	{0xfc,0x16}, //4 
	{0xf2,0x07}, //sync output
	{0xf3,0x83}, //ff//1f//01 data output
	{0xf5,0x07}, //sck_dely

	{0xf7,0x89}, //f8//fd   88
	{0xf8,0x01},           // 00
	{0xf9,0x4f}, //0f//01   4d
	{0xfa,0x11}, //32

	{0xfc,0xce},
	{0xfd,0x00},
	/////////////////////////////////////////////////
	/////////////////   CISCTL reg  /////////////////
	/////////////////////////////////////////////////
	{0x00,0x2f}, 
	{0x01,0x0f}, 
	{0x02,0x04},

	{0x03,0x02},
	{0x04,0x12},

	{0x09,0x00}, 
	{0x0a,0x00}, 
	{0x0b,0x00}, 
	{0x0c,0x04}, 
	{0x0d,0x01}, 
	{0x0e,0xe8}, 
	{0x0f,0x02}, 
	{0x10,0x88}, 
	{0x16,0x00},
	{0x17,0x14},
	{0x18,0x1a}, 
	{0x19,0x14}, 
	{0x1b,0x48},
	{0x1e,0x6b},
	{0x1f,0x28},
	{0x20,0x8b},//0x89 travis20140801
	{0x21,0x49},
	{0x22,0xb0},
	{0x23,0x04}, 
	{0x24,0xff}, 
	{0x34,0x20}, 
	/////////////////////////////////////////////////
	////////////////////   BLK   ////////////////////
	/////////////////////////////////////////////////
	{0x26,0x23},
	{0x28,0xff},
	{0x29,0x00},
	{0x33,0x10}, 
	{0x37,0x20},
	{0x38,0x10},
	{0x47,0x80},
	{0x4e,0x66},
	{0xa8,0x02},
	{0xa9,0x80},
	/////////////////////////////////////////////////
	//////////////////   ISP reg  ///////////////////
	/////////////////////////////////////////////////
	{0x40,0xff},
	{0x41,0x21},
	{0x42,0xcf},
	{0x44,0x02},
	{0x45,0xa8}, 
	{0x46,0x02}, //sync
	{0x4a,0x11},
	{0x4b,0x01},
	{0x4c,0x20},
	{0x4d,0x05},
	{0x4f,0x01},
	{0x50,0x01},
	{0x55,0x01},
	{0x56,0xe0},
	{0x57,0x02},
	{0x58,0x80},
	
	/////////////////////////////////////////////////
	///////////////////   GAIN   ////////////////////
	/////////////////////////////////////////////////
	{0x70,0x70},
	{0x5a,0x84},
	{0x5b,0xc9},
	{0x5c,0xed},
	{0x77,0x74},
	{0x78,0x40},
	{0x79,0x5f}, 


	///////////////////////////////////////////////// 
	///////////////////   DNDD  /////////////////////
	///////////////////////////////////////////////// 
	{0x82,0x14}, 
	{0x83,0x0b},
	{0x89,0xf0},
	///////////////////////////////////////////////// 
	//////////////////   EEINTP  ////////////////////
	///////////////////////////////////////////////// 
	{0x8f,0xaa},
	{0x90,0x8c},
	{0x91,0x90},
	{0x92,0x03},
	{0x93,0x03},
	{0x94,0x05},
	{0x95,0x65}, 
	{0x96,0xf0}, 
	///////////////////////////////////////////////// 
	/////////////////////  ASDE  ////////////////////
	///////////////////////////////////////////////// 
	{0xfe,0x00},

	{0x9a,0x20},
	{0x9b,0x80},
	{0x9c,0x40},
	{0x9d,0x80},
	 
	{0xa1,0x30},
 	{0xa2,0x32},
	{0xa4,0x30},
	{0xa5,0x30},
	{0xaa,0x10}, 
	{0xac,0x22},
	 

	/////////////////////////////////////////////////
	///////////////////   GAMMA   ///////////////////
	/////////////////////////////////////////////////
	{0xfe,0x00},//default
	{0xbf,0x08},
	{0xc0,0x16},
	{0xc1,0x28},
	{0xc2,0x41},
	{0xc3,0x5a},
	{0xc4,0x6c},
	{0xc5,0x7a},
	{0xc6,0x96},
	{0xc7,0xac},
	{0xc8,0xbc},
	{0xc9,0xc9},
	{0xca,0xd3},
	{0xcb,0xdd},
	{0xcc,0xe5},
	{0xcd,0xf1},
	{0xce,0xfa},
	{0xcf,0xff},
	
/* 
	{0xfe,0x00},//big gamma
	{0xbf,0x08},
	{0xc0,0x1d},
	{0xc1,0x34},
	{0xc2,0x4b},
	{0xc3,0x60},
	{0xc4,0x73},
	{0xc5,0x85},
	{0xc6,0x9f},
	{0xc7,0xb5},
	{0xc8,0xc7},
	{0xc9,0xd5},
	{0xca,0xe0},
	{0xcb,0xe7},
	{0xcc,0xec},
	{0xcd,0xf4},
	{0xce,0xfa},
	{0xcf,0xff},
*/	

/*
	{0xfe,0x00},//small gamma
	{0xbf,0x08},
	{0xc0,0x18},
	{0xc1,0x2c},
	{0xc2,0x41},
	{0xc3,0x59},
	{0xc4,0x6e},
	{0xc5,0x81},
	{0xc6,0x9f},
	{0xc7,0xb5},
	{0xc8,0xc7},
	{0xc9,0xd5},
	{0xca,0xe0},
	{0xcb,0xe7},
	{0xcc,0xec},
	{0xcd,0xf4},
	{0xce,0xfa},
	{0xcf,0xff},
*/
	/////////////////////////////////////////////////
	///////////////////   YCP  //////////////////////
	/////////////////////////////////////////////////
	{0xd0,0x40}, 
	{0xd1,0x34}, 
	{0xd2,0x34}, 
	{0xd3,0x40}, 
	{0xd6,0xf2}, 
	{0xd7,0x1b}, 
	{0xd8,0x18}, 
	{0xdd,0x03}, 

	/////////////////////////////////////////////////
	////////////////////   AEC   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01},
	{0x05,0x30},
	{0x06,0x75},
	{0x07,0x40},
	{0x08,0xb0},
	{0x0a,0xc5},
	{0x0b,0x11},
	{0x0c,0x00},
	{0x12,0x52},
	{0x13,0x38},
	{0x18,0x95},
	{0x19,0x96},
	{0x1f,0x20}, 
	{0x20,0xc0}, 
	{0x3e,0x40}, 
	{0x3f,0x57}, 
	{0x40,0x7d}, 

	{0x03,0x60}, 

	{0x44,0x02}, 
	/////////////////////////////////////////////////
	////////////////////   AWB   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01},
	{0x1c,0x91}, 
	{0x21,0x15}, 
	{0x50,0x80},
	{0x56,0x04},
	{0x59,0x08}, 
	{0x5b,0x02},
	{0x61,0x8d}, 
	{0x62,0xa7}, 
	{0x63,0xd0}, 
	{0x65,0x06},
	{0x66,0x06}, 
	{0x67,0x84}, 
	{0x69,0x08}, 
	{0x6a,0x25}, 
	{0x6b,0x01}, 
	{0x6c,0x00}, 
	{0x6d,0x02}, 
	{0x6e,0xf0}, 
	{0x6f,0x80}, 
	{0x76,0x80}, 
	{0x78,0xaf}, 
	{0x79,0x75},
	{0x7a,0x40},
	{0x7b,0x50},
	{0x7c,0x0c}, 

	{0x90,0xc9},//stable AWB 
	{0x91,0xbe},
	{0x92,0xe2},
	{0x93,0xc9},
	{0x95,0x1b},
	{0x96,0xe2},
	{0x97,0x49},
	{0x98,0x1b},
	{0x9a,0x49},
	{0x9b,0x1b},
	{0x9c,0xc3},
	{0x9d,0x49},
	{0x9f,0xc7},
	{0xa0,0xc8},
	{0xa1,0x00},
	{0xa2,0x00},
	{0x86,0x00},
	{0x87,0x00},
	{0x88,0x00},
	{0x89,0x00},
	{0xa4,0xb9},
	{0xa5,0xa0},
	{0xa6,0xba},
	{0xa7,0x92},
	{0xa9,0xba},
	{0xaa,0x80},
	{0xab,0x9d},
	{0xac,0x7f},
	{0xae,0xbb},
	{0xaf,0x9d},
	{0xb0,0xc8},
	{0xb1,0x97},
	{0xb3,0xb7},
	{0xb4,0x7f},
	{0xb5,0x00},
	{0xb6,0x00},
	{0x8b,0x00},
	{0x8c,0x00},
	{0x8d,0x00},
	{0x8e,0x00},
	{0x94,0x55},
	{0x99,0xa6},
	{0x9e,0xaa},
	{0xa3,0x0a},
	{0x8a,0x00},
	{0xa8,0x55},
	{0xad,0x55},
	{0xb2,0x55},
	{0xb7,0x05},
	{0x8f,0x00},
	{0xb8,0xcb},
	{0xb9,0x9b},  
	
/*	 
	{0xa4,0xb9}, //default AWB
	{0xa5,0xa0},
	{0x90,0xc9}, 
	{0x91,0xbe},                                 
	{0xa6,0xb8}, 
	{0xa7,0x95}, 
	{0x92,0xe6}, 
	{0x93,0xca},                                
	{0xa9,0xbc}, 
	{0xaa,0x95}, 
	{0x95,0x23}, 
	{0x96,0xe7},                                  
	{0xab,0x9d}, 
	{0xac,0x80},
	{0x97,0x43}, 
	{0x98,0x24},                                
	{0xae,0xb7}, 
	{0xaf,0x9e}, 
	{0x9a,0x43},
	{0x9b,0x24},                                 
	{0xb0,0xc8}, 
	{0xb1,0x97},
	{0x9c,0xc4}, 
	{0x9d,0x44},                               
	{0xb3,0xb7}, 
	{0xb4,0x7f},
	{0x9f,0xc7},
	{0xa0,0xc8},                               
	{0xb5,0x00}, 
	{0xb6,0x00},
	{0xa1,0x00},
	{0xa2,0x00},                         
	{0x86,0x60},
	{0x87,0x08},
	{0x88,0x00},
	{0x89,0x00},
	{0x8b,0xde},
	{0x8c,0x80},
	{0x8d,0x00},
	{0x8e,0x00},                               
	{0x94,0x55},
	{0x99,0xa6},
	{0x9e,0xaa},
	{0xa3,0x0a},
	{0x8a,0x0a},
	{0xa8,0x55},
	{0xad,0x55},
	{0xb2,0x55},
	{0xb7,0x05},
	{0x8f,0x05},                              
	{0xb8,0xcc},
	{0xb9,0x9a},
*/  

	/////////////////////////////////////////////////
	////////////////////  CC ////////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01},
	
	{0xd0,0x38},//skin red
	{0xd1,0x00},
	{0xd2,0x02},
	{0xd3,0x04},
	{0xd4,0x38},
	{0xd5,0x12},	
/*                     
	{0xd0,0x38},//skin white
	{0xd1,0xfd},
	{0xd2,0x06},
	{0xd3,0xf0},
	{0xd4,0x40},
	{0xd5,0x08},
*/
	
/*                       
	{0xd0,0x38},//guodengxiang
	{0xd1,0xf8},
	{0xd2,0x06},
	{0xd3,0xfd},
	{0xd4,0x40},
	{0xd5,0x00},	
*/
	{0xd6,0x30},
	{0xd7,0x00},
	{0xd8,0x0a},
	{0xd9,0x16},
	{0xda,0x39},
	{0xdb,0xf8},
	/////////////////////////////////////////////////
	////////////////////   LSC   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x01}, 
	{0xc1,0x3c},
	{0xc2,0x50},
	{0xc3,0x00},
	{0xc4,0x40},
	{0xc5,0x30},
	{0xc6,0x30},
	{0xc7,0x10},
	{0xc8,0x00},
	{0xc9,0x00},
	{0xdc,0x20},
	{0xdd,0x10},
	{0xdf,0x00}, 
	{0xde,0x00}, 


	/////////////////////////////////////////////////
	///////////////////  Histogram  /////////////////
	/////////////////////////////////////////////////
	{0x01,0x10}, 
	{0x0b,0x31}, 
	{0x0e,0x50}, 
	{0x0f,0x0f}, 
	{0x10,0x6e}, 
	{0x12,0xa0}, 
	{0x15,0x60}, 
	{0x16,0x60}, 
	{0x17,0xe0}, 


	/////////////////////////////////////////////////
	//////////////   Measure Window   ///////////////
	/////////////////////////////////////////////////
	{0xcc,0x0c}, 
	{0xcd,0x10}, 
	{0xce,0xa0}, 
	{0xcf,0xe6}, 


	/////////////////////////////////////////////////
	/////////////////   dark sun   //////////////////
	/////////////////////////////////////////////////
	{0x45,0xf7},
	{0x46,0xff}, 
	{0x47,0x15},
	{0x48,0x03}, 
	{0x4f,0x60}, 
                                
	/////////////////////////////////////////////////
	///////////////////  banding  ///////////////////
	/////////////////////////////////////////////////
	{0xfe,0x00},
	{0x05,0x01}, 
	{0x06,0x32}, //HB
	{0x07,0x00}, 
	{0x08,0x70}, //VB
	{0xfe,0x01}, 
	{0x25,0x00}, //step
	{0x26,0x78}, 
	{0x27,0x02}, //20fps
	{0x28,0x58}, 
	{0x29,0x03}, //14.2fps
	{0x2a,0x48}, 
	{0x2b,0x04}, //10fps
	{0x2c,0xb0}, 
	{0x2d,0x07}, //6.25fps
	{0x2e,0x80}, 
	{0x3c,0x20},
	/////////////////////  SPI   ////////////////////
	/////////////////////////////////////////////////
	{0xfe,0x03},
	{0x01,0x00},
	{0x02,0x00},
	{0x10,0x00},
	{0x15,0x00},
	{0x17,0x00}, //01//03
	{0x04,0x10},
	{0x05,0x00},
	{0x40,0x00},


	/*{0x52,0x20}, //a2 //a0//80//00 //22

	{0x53,0x24}, //24
	{0x54,0x20},
	{0x55,0x20}, //QQ//01
	{0x59,0x1f},//hechang
	{0x5a,0x40}, //00 //yuv 
	{0x5b,0x80},
	{0x5c,0x02},
	{0x5d,0xe0},
	{0x5e,0x01},
	{0x51,0x03},
	{0x64,0x06},
	{0xfe,0x00},*/

	{0x52,0xba},//{0x52,0xba},
	{0x53,0x24},
	{0x54,0x20},
	{0x55,0x20},//{0x55,0x00},
	{0x59,0x1f},//20210322 hechang  20190627 scaler output error
	{0x5a,0x40}, //00 //yuv
	{0x5b,0x80},
	{0x5c,0x02},
	{0x5d,0xe0},
	{0x5e,0x01},
	{0x51,0x03},
	{0x64,0x04},
	{0xfe,0x00},  
};
#endif

LOCAL SENSOR_REG_TAB_INFO_T s_GC0310_resolution_Tab_YUV[]=
{   
    { ADDR_AND_LEN_OF_ARRAY(GC0310_YUV_640X480), 640, 480,  24, SENSOR_IMAGE_FORMAT_YUV422 },
    // YUV422 PREVIEW 1
    { PNULL,                    0,   640,    480,    24   ,   SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    

    // YUV422 PREVIEW 2 
     { PNULL,                   0,      0,  0 ,         0,         0     }, 
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      }

};

#ifdef GC0310_LOAD_FROM_T_FLASH  
LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC0310_ioctl_func_tab = 
{
// Internal 
    PNULL,
    GC0310_Power_On, 
    PNULL,
    GC0310_Identify,
    PNULL,
    PNULL,
// Custom function
    PNULL,
    PNULL,

// External
    PNULL, 
    PNULL, //GC0310_Set_Mirror,
    PNULL, //GC0310_Set_Flip,

    PNULL,//GC0310_Set_Brightness,
    PNULL,//GC0310_Set_Contrast,
    PNULL, //GC0310_Set_Sharpness,
    PNULL, //GC0310_Set_saturation,
    GC0310_Set_Preview_Mode, 

    PNULL,//GC0310_Set_Image_Effect, 
    PNULL,//GC0310_Before_Snapshot,
    PNULL,//GC0310_After_Snapshot,

    PNULL,

    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,

    PNULL,
    PNULL,

    PNULL,//GC0310_Set_AWB,

    PNULL,

    PNULL, 
    PNULL,//GC0310_Set_Ev,

    PNULL, 
    PNULL, 
    PNULL,

    PNULL,
    PNULL,
    PNULL,//GC0310_Set_Anti_Flicker,
    PNULL,//GC0310_Set_Video_Mode,

    PNULL,
    PNULL
};
#else
LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC0310_ioctl_func_tab = 
{
// Internal 
    PNULL,
    GC0310_Power_On, 
    PNULL,
    GC0310_Identify,
    PNULL,
    PNULL,
// Custom function
    PNULL,
    PNULL,

// External
    PNULL, 
    PNULL, //GC0310_Set_Mirror,
    PNULL, //GC0310_Set_Flip,

    GC0310_Set_Brightness,
    GC0310_Set_Contrast,
    PNULL, //GC0310_Set_Sharpness,
    PNULL, //GC0310_Set_saturation,
    GC0310_Set_Preview_Mode, 

    GC0310_Set_Image_Effect, 
    PNULL,//GC0310_Before_Snapshot,
    PNULL,//GC0310_After_Snapshot,

    PNULL,

    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,

    PNULL,
    PNULL,

    GC0310_Set_AWB,

    PNULL,

    PNULL, 
    GC0310_Set_Ev,

    PNULL, 
    PNULL, 
    PNULL,

    PNULL,
    PNULL,
    GC0310_Set_Anti_Flicker,
    GC0310_Set_Video_Mode,

    PNULL,
    PNULL
};
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC const SENSOR_INFO_T g_GC0310_yuv_info =
{
    GC0310_I2C_ADDR_W,              // salve i2c write address
    GC0310_I2C_ADDR_R,              // salve i2c read address
    
    0,                              // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                    // other bit: reseved
    SENSOR_HW_SIGNAL_PCLK_P|\
    SENSOR_HW_SIGNAL_VSYNC_N|\
    SENSOR_HW_SIGNAL_HSYNC_N,       // bit0: 0:negative; 1:positive -> polarily of pixel clock
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
        
    7,                              // bit[0:7]: count of step in brightness, contrast, sharpness, saturation
                                    // bit[8:31] reseved
    
    SENSOR_LOW_PULSE_RESET,     // reset pulse level
    100,                                // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,          // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code 
    0xf0, 0xA3,                     // supply two code to identify sensor.
    0xf1, 0x10,                     // for Example: index = 0-> Device id, index = 1 -> version id                                          
                                            
    SENSOR_AVDD_2800MV,             // voltage of avdd  
    
    640,                            // max width of source image
    480,                            // max height of source image
    "GC0310",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_GC0310_resolution_Tab_YUV,    // point to resolution table information structure
    &s_GC0310_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor  
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_CLOSED,                      // dvdd
    2,                     // skip frame num before preview 
    2,                     // skip frame num before capture     
    0,                     // deci frame num during preview;        
    2,                     // deci frame num during video preview;
    0,                     // threshold enable
    0,                     // threshold mode
    0,                     // threshold start postion   
    0,                     // threshold end postion 

#if  defined (GC0310_OUTPUT_MODE_CCIR656_2BIT)
    SENSOR_OUTPUT_MODE_CCIR656_2BIT,
#elif defined(GC0310_OUTPUT_MODE_PACKET_DDR_2BIT)
    SENSOR_OUTPUT_MODE_PACKET_DDR_2BIT, 
#else
    SENSOR_OUTPUT_MODE_CCIR656_2BIT, 
#endif
    SENSOR_OUTPUT_ENDIAN_LITTLE
};

/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void GC0310_WriteReg(uint8 subaddr, uint8 data )
{
    Sensor_WriteReg(subaddr, data);
}

LOCAL uint8 GC0310_ReadReg(uint8  subaddr)
{
    uint8 value = 0;
    
    value = Sensor_ReadReg( subaddr);
    
    return value;
}

LOCAL void GC0310_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

}

LOCAL uint32 GC0310_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val=g_GC0310_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val=g_GC0310_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val=g_GC0310_yuv_info.iovdd_val;  
    uint32 power_down=g_GC0310_yuv_info.power_down_level;
    
    if(SCI_TRUE == power_on)
    {
		GPIO_SetSensorPwdn(!power_down); /*lint !e730 -e730*/
		SCI_Sleep(2);
		Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
		SCI_Sleep(2);
        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);// Default MCLK=12Mhz
		SCI_Sleep(2);
		GPIO_SetSensorPwdn(power_down);
		SCI_Sleep(4);
		GPIO_SetSensorPwdn(!power_down); 
    }
    else
    {
		GPIO_SetSensorPwdn(power_down);
		SCI_Sleep(2);
		Sensor_SetMCLK(SENSOR_DISABLE_MCLK);
		SCI_Sleep(2);
		Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);
		SCI_Sleep(2);
		GPIO_SetSensorPwdn(!power_down);
    }
    
    //SCI_TRACE_LOW("SENSOR: _GC0310_On(1:on, 0:off): %d", power_on);    
    
    return SCI_SUCCESS;
}

LOCAL uint32 GC0310_Identify(uint32 param)
{
    uint8 reg[2] = {0xf0, 0xf1};
    uint8 value[2] = {0xa3, 0x10};
    uint8 ret = 0;
    uint8 err_cnt = 0;
    uint32 i = 0;
    uint32 nLoop = 1000;
    
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = GC0310_ReadReg(reg[i]);    
        
     	SCI_TRACE_LOW("SENSOR: GC0310 Read reg0x00 = %x");
        
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
                 SCI_TRACE_LOW("GC0310 identify Fail");
                 return SCI_ERROR;
            }
            else
            {
                while(nLoop--)
                {
                    ;
                }
                continue;
            }
        }
        err_cnt = 0;
        i++;
    }

    SCI_TRACE_LOW("GC0310 Identify ok");

    return (uint32)SCI_SUCCESS;
}

__align(4) const SENSOR_REG_T GC0310_brightness_tab[][2]=
{
       {        
        {0xd5, 0xd0},   {0xff,0xff},
    },

    {
        {0xd5, 0xe0},   {0xff,0xff},
    },

    {
        {0xd5, 0xf0},   {0xff,0xff},
    },

    {
        {0xd5, 0x00},   {0xff,0xff},
    },

    {
        {0xd5, 0x20},   {0xff,0xff},
    },

    {
        {0xd5, 0x30},   {0xff,0xff},
    },

    {
        {0xd5, 0x40},   {0xff,0xff},
    },
};

LOCAL uint32 GC0310_Set_Brightness(uint32 level)
{

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC0310_brightness_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 
    
    GC0310_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("GC0310_Set_Brightness: level = %d", level);
    
    return 0;
}

__align(4) const SENSOR_REG_T GC0310_contrast_tab[][2]=
{
    {
        {0xd3,0x34},    {0xff,0xff},
    },

    {
        {0xd3,0x38},    {0xff,0xff}, 
    },

    {
        {0xd3,0x3d},    {0xff,0xff}, 
    },

    {
        {0xd3,0x40},    {0xff,0xff},
    },

    {
        {0xd3,0x44},    {0xff,0xff}, 
    },

    {
        {0xd3,0x48},    {0xff,0xff}, 
    },

    {
        {0xd3,0x50},    {0xff,0xff},
    },                   
};

LOCAL uint32 GC0310_Set_Contrast(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC0310_contrast_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    
    GC0310_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("GC0310_Set_Contrast: level = %d", level);
    
    return 0;
}

__align(4) const SENSOR_REG_T GC0310_image_effect_tab[][4]= 
{
    // effect normal
    {
        {0x43,0x00}, {0xda,0x00}, {0xdb,0x00}, {0xff,0xff}
    },
    //effect BLACKWHITE
    {
        {0x43,0x02}, {0xda,0x00}, {0xdb,0x00}, {0xff,0xff}
    },
    // effect RED pink
    {
        {0x43,0x02}, {0xda,0x10}, {0xdb,0x50}, {0xff,0xff}
    },
    // effect GREEN
    {
        {0x43,0x02}, {0xda,0xc0}, {0xdb,0xc0}, {0xff,0xff}
    },
    // effect  BLUE
    {
        {0x43,0x02}, {0xda,0x50}, {0xdb,0xe0}, {0xff,0xff}
    },
    // effect  YELLOW
    {
        {0x43,0x02}, {0xda,0x80}, {0xdb,0x20}, {0xff,0xff}
    },  
    // effect NEGATIVE
    {        
        {0x43,0x01}, {0xda,0x00}, {0xdb,0x00}, {0xff,0xff}
    },    
    //effect ANTIQUE
    {
        {0x43,0x02}, {0xda,0xd2}, {0xdb,0x28}, {0xff,0xff}
    },
};

LOCAL uint32 GC0310_Set_Image_Effect(uint32 effect_type)
{
   
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC0310_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 

    GC0310_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("GC0310_Set_Image_Effect: effect_type = %d", effect_type);
    
    return 0;
}

__align(4) const SENSOR_REG_T GC0310_ev_normal_tab[][4]=
{   
    {{0xfe, 0x01},{0x13, 0x18},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x20},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x28},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x38},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x40},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x48},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x50},{0xfe, 0x00},{0xff, 0xff}},  
};

__align(4) const SENSOR_REG_T GC0310_ev_night_tab[][4]=
{   
    {{0xfe, 0x01},{0x13, 0x38},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x40},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x48},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x50},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x58},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x60},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x13, 0x68},{0xfe, 0x00},{0xff, 0xff}},  
};
LOCAL uint32 GC0310_Set_Ev(uint32 level)
{

    SENSOR_REG_T* sensor_reg_ptr;

    if (s_preview_mode != DCAMERA_ENVIRONMENT_NIGHT) {
        sensor_reg_ptr = (SENSOR_REG_T*)GC0310_ev_normal_tab[level];
    } else {
        sensor_reg_ptr = (SENSOR_REG_T*)GC0310_ev_night_tab[level];
    }
    
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    SCI_ASSERT(level < 7);    /*assert verified*/ 
 
    GC0310_Write_Group_Regs(sensor_reg_ptr );

    SCI_TRACE_LOW("GC0310_Set_Ev: level = %d", level);

    return 0;
}
#if  defined (GC0310_OUTPUT_MODE_CCIR656_2BIT)
__align(4) const SENSOR_REG_T GC0310_anti_flicker_tab[][19]=
{   
    {//50hz
		{0xfe,0x00},
		{0x05,0x01},
		{0x06,0x12}, //HB
		{0x07,0x00},
		{0x08,0x1c}, //VB
		{0xfe,0x01},
		{0x25,0x00}, //step 
		{0x26,0x1f}, 
		{0x27,0x01}, //6fps
		{0x28,0xf0},  
		{0x29,0x01}, //6fps
		{0x2a,0xf0}, 
		{0x2b,0x01}, //6fps
		{0x2c,0xf0}, 
		{0x2d,0x03}, //3.3fps
		{0x2e,0xe0},
		{0x3c,0x20},
        {0xfe,0x00},
        {0xff,0xff}
     },
     {//60Hz
        {0xfe,0x00},
        {0x05,0x01},
        {0x06,0x32}, //HB
        {0x07,0x00},
        {0x08,0x0c}, //VB
        {0xfe,0x01},
        {0x25,0x00}, //step 
        {0x26,0x19}, 
        {0x27,0x01}, //6fps
        {0x28,0xf4},  
        {0x29,0x01}, //6fps
        {0x2a,0xf4}, 
        {0x2b,0x01}, //6fps
        {0x2c,0xf4}, 
        {0x2d,0x03}, //3.25fps
        {0x2e,0x20},
        {0xfe,0x00},
        {0xff,0xff}
     },
};
#elif defined(GC0310_OUTPUT_MODE_PACKET_DDR_2BIT)
__align(4) const SENSOR_REG_T GC0310_anti_flicker_tab[][19]=
{   
    {//50hz
		{0xfe,0x00},
		{0x05,0x01}, 
		{0x06,0x32}, //HB
		{0x07,0x00}, 
		{0x08,0x70}, //VB
		{0xfe,0x01}, 
		{0x25,0x00}, //step
		{0x26,0x78}, 
		{0x27,0x02}, //20fps
		{0x28,0x58}, 
		{0x29,0x03}, //14.2fps
		{0x2a,0x48}, 
		{0x2b,0x04}, //10fps
		{0x2c,0xb0}, 
		{0x2d,0x07}, //6.25fps
		{0x2e,0x80}, 
		{0x3c,0x20},
        {0xfe,0x00},
        {0xff,0xff}
     },
     {//60Hz
        {0xfe,0x00},
        {0x05,0x01},
        {0x06,0x32}, //HB
        {0x07,0x00},
        {0x08,0x6a}, //VB
        {0xfe,0x01},
        {0x25,0x00}, //step 
        {0x26,0x6c}, 
        {0x27,0x02}, //20fps
        {0x28,0x52},  
        {0x29,0x03}, //15fps
        {0x2a,0x18}, 
        {0x2b,0x04}, //10fps
        {0x2c,0xa4}, 
        {0x2d,0x09}, //5fps
        {0x2e,0x48},
        {0xfe,0x00},
        {0xff,0xff}
     },
};
#endif
LOCAL uint32 GC0310_Set_Anti_Flicker(uint32 mode)
{ 
    SENSOR_REG_T* sensor_reg_ptr;
    s_anti_flicker = mode;
    switch(mode)
    {
        case DCAMERA_FLICKER_50HZ:
            sensor_reg_ptr = (SENSOR_REG_T*)GC0310_anti_flicker_tab[mode];
        break;

        case DCAMERA_FLICKER_60HZ:
            sensor_reg_ptr = (SENSOR_REG_T*)GC0310_anti_flicker_tab[mode];
        break;

        default:
            sensor_reg_ptr = (SENSOR_REG_T*)GC0310_anti_flicker_tab[mode];
        break;
    }
      
    GC0310_Write_Group_Regs(sensor_reg_ptr );

    SCI_TRACE_LOW("GC0310_Set_Anti_Flicker: level = %d", mode);
    return 0;
}

__align(4) const SENSOR_REG_T GC0310_preview_tab[][4]=
{   
    {{0xfe, 0x01},{0x3c, 0x20},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x3c, 0x30},{0xfe, 0x00},{0xff, 0xff}},
};
LOCAL uint32 GC0310_Set_Preview_Mode(uint32 preview_mode)
{
    SENSOR_REG_T* sensor_reg_ptr;
    s_preview_mode = preview_mode;
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            sensor_reg_ptr = (SENSOR_REG_T*)GC0310_preview_tab[preview_mode];
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            sensor_reg_ptr = (SENSOR_REG_T*)GC0310_preview_tab[preview_mode];
            break;
        }
        default:
        {
            sensor_reg_ptr = (SENSOR_REG_T*)GC0310_preview_tab[DCAMERA_ENVIRONMENT_NORMAL];
            break;
        }
        
    }

    GC0310_Write_Group_Regs(sensor_reg_ptr );

#ifdef GC0310_LOAD_FROM_T_FLASH  
 	Load_GC0310_RegTab_From_T_Flash();
#endif	

    SCI_TRACE_LOW("GC0310_Set_Preview_Mode: level = %d", preview_mode);
    return 0;
}

__align(4) const SENSOR_REG_T GC0310_awb_tab[][5]=
{
    //AUTO
    {
        {0x77, 0x57},    // the reg value is not written here, rewrite in set_GC6113_awb();
        {0x78, 0x4b}, 
        {0x79, 0x45}, 
        {0x42, 0xcf}, 
        {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
        {0x42, 0xcd},   // Disable AWB 
        {0x77, 0x48},    
        {0x78, 0x40},
        {0x79, 0x5c},
        {0xff, 0xff} 
    },
    //U30
    {
        {0x42, 0xcd},   // Disable AWB 
        {0x77, 0x40},    
        {0x78, 0x54},
        {0x79, 0x70},
        {0xff, 0xff} 
    },
    //CWF  //
    {
        {0x42, 0xcd},   // Disable AWB 
        {0x77, 0x40},    
        {0x78, 0x54},
        {0x79, 0x70},
        {0xff, 0xff} 
    },    
    //FLUORESCENT:
    {
        {0x42, 0xcd},   // Disable AWB 
        {0x77, 0x40},    
        {0x78, 0x42},
        {0x79, 0x50},
        {0xff, 0xff} 
    },
    //SUN:
    {
        {0x42, 0xcd},   // Disable AWB 
        {0x77, 0x50},    
        {0x78, 0x45},
        {0x79, 0x40},
        {0xff, 0xff} 
    },
    //CLOUD:
    {
        {0x42, 0xcd},   // Disable AWB 
        {0x77, 0x5a},    
        {0x78, 0x42},
        {0x79, 0x40},
        {0xff, 0xff} 
    },
};

LOCAL uint32 GC0310_Set_AWB(uint32 mode)
{
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC0310_awb_tab[mode];

    SCI_ASSERT(mode < 7);    /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    
    GC0310_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("GC0310_Set_AWB: mode = %d", mode);
    
    return 0;
}

LOCAL uint32 GC0310_Set_Video_Mode(uint32 mode)
{
    SENSOR_REG_T* sensor_reg_ptr = PNULL;

    if(PNULL == sensor_reg_ptr || mode > DCAMERA_MODE_MAX)
    {
        return SCI_ERROR;
    }

    if (mode == DCAMERA_VIDEO_MODE || mode == DCAMERA_UPCC_MODE) {
       sensor_reg_ptr = (SENSOR_REG_T*)GC0310_anti_flicker_tab[s_anti_flicker];
       GC0310_Write_Group_Regs(sensor_reg_ptr );
    }    

    SCI_TRACE_LOW("GC0310_Set_Video_Mode =%d", mode);

    return 0;
}

LOCAL uint32 GC0310_Before_Snapshot(uint32 para)
{

    return 0;
}

LOCAL uint32 GC0310_After_Snapshot(uint32 para)
{

    return 0;
}

/*lint -restore */

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#ifdef GC0310_LOAD_FROM_T_FLASH  
__align(4) static SENSOR_REG_T GC0310_YUV_Init_Reg[1000] = {{0x00,0x00},}; 
//#define READ_BUFFER_SIZE  (274*12)    //must be divisible by 12
LOCAL uint32 Load_GC0310_RegTab_From_T_Flash(void)
{
    SFS_HANDLE    file_handle = 0;
    FFS_ERROR_E   ffs_err = FFS_NO_ERROR;
  //  char *file_name = "C:\\GC0310_SERIAL_Initialize_Setting.Bin";
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

	SCI_MEM16CPY(unicode_file_name,L"E:\\GC0310_SERIAL_Initialize_Setting.Bin",sizeof(L"E:\\GC0310_SERIAL_Initialize_Setting.Bin"));
   

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
            GC0310_YUV_Init_Reg[regNum].reg_addr = (uint16)strtol(curr_ptr, NULL, 16);
            curr_ptr += 5;
            GC0310_YUV_Init_Reg[regNum].reg_value = (uint16)strtol(curr_ptr, NULL, 16);
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
	GC0310_YUV_Init_Reg[regNum].reg_addr = 0xff;
	GC0310_YUV_Init_Reg[regNum].reg_value =0xff;
	
	SCI_TRACE_LOW("%d register read...", i);
 
	SCI_FREE(buffer_ptr);
 	buffer_ptr = NULL;	
    ffs_err = SFS_CloseFile( file_handle);
	file_handle = NULL;

	SCI_TRACE_LOW("Start apply initialize setting.");
    /* Start apply the initial setting to sensor. */
	SCI_ASSERT(PNULL != GC0310_YUV_Init_Reg);
    GC0310_Write_Group_Regs(GC0310_YUV_Init_Reg);

	return SCI_SUCCESS;
}
#endif

#endif
