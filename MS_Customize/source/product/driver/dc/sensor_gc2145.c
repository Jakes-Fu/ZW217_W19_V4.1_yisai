/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_GC2145.c                                           *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor GC2145. 
 **                                                          
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **       
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                     Extern Function Declaration                           *
 **---------------------------------------------------------------------------*/
//extern uint32 OS_TickDelay(uint32 ticks);
/*
extern void Sensor_SetMCLK(uint32 mclk);
*/
//extern ERR_I2C_E I2C_WriteCmdArrNoStop(uint8 addr, uint8 *pCmd, uint32 len, BOOLEAN ack_en);

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define GC2145_I2C_ADDR_W       0x78
#define GC2145_I2C_ADDR_R       0x79

#define SENSOR_GAIN_SCALE       16



/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_gc2145_ae_enable(uint32 enable);
//LOCAL uint32 set_hmirror_enable(uint32 enable);
//LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 GC2145_Identify(uint32 param);
LOCAL uint32 set_brightness(uint32 level);
LOCAL uint32 set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
LOCAL uint32 set_image_effect(uint32 effect_type);
LOCAL uint32 GC2145_After_Snapshot(uint32 param);
LOCAL uint32 GC2145_BeforeSnapshot(uint32 param);
	
LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);
LOCAL uint32 GC2145_set_work_mode(uint32 mode);

LOCAL uint32 set_gc2145_ev(uint32 level);
LOCAL uint32 set_gc2145_awb(uint32 mode);
LOCAL uint32 set_gc2145_anti_flicker(uint32 mode);
LOCAL uint32 set_gc2145_video_mode(uint32 mode);
LOCAL uint32 GC2145_Power_On(uint32 power_on);

LOCAL BOOLEAN gc_enter_effect = SCI_FALSE;
LOCAL uint32  mirror_setting = 0;     //mirror flip setting 

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/

__align(4) const SENSOR_REG_T GC2145_YUV_COMMON[]=
{
	{0xfe , 0xf0},
 	{0xfe , 0xf0},
 	{0xfe , 0xf0},
 	{0xfc , 0x06},
 	{0xf6 , 0x00},
 	{0xf7 , 0x1d},
 	{0xf8 , 0x84},
 	{0xfa , 0x00},
 	{0xf9 , 0xfe},
	{0xf2 , 0x00},
	/////////////////////////////////////////////////
	//////////////////ISP reg//////////////////////
	////////////////////////////////////////////////////
	{0xfe , 0x00},
	{0x03 , 0x04},
	{0x04 , 0xe2},
	{0x09 , 0x00},
	{0x0a , 0x00},
	{0x0b , 0x00},
	{0x0c , 0x00},
	{0x0d , 0x04},
	{0x0e , 0xc0},
	{0x0f , 0x06},
	{0x10 , 0x52},
	{0x12 , 0x2e},
	{0x17 , 0x14}, //mirror
	{0x18 , 0x22},
	{0x19 , 0x0e},
	{0x1a , 0x01},
	{0x1b , 0x4b},
	{0x1c , 0x07},
	{0x1d , 0x10},
	{0x1e , 0x88},
	{0x1f , 0x78},
	{0x20 , 0x03},
	{0x21 , 0x40},
	{0x22 , 0xa0}, 
	{0x24 , 0x16},
	{0x25 , 0x01},
	{0x26 , 0x10},
	{0x2d , 0x60},
	{0x30 , 0x01},
	{0x31 , 0x90},
	{0x33 , 0x06},
	{0x34 , 0x01},
	/////////////////////////////////////////////////
	//////////////////ISP reg////////////////////
	/////////////////////////////////////////////////
	{0xfe , 0x00},
	{0x80 , 0x7f},
	{0x81 , 0x26},
	{0x82 , 0xfa},
	{0x83 , 0x00},
	{0x84 , 0x02}, 
	{0x86 , 0x02},
	{0x88 , 0x03},
	{0x89 , 0x03},
	{0x85 , 0x08}, 
	{0x8a , 0x00},
	{0x8b , 0x00},
	{0xb0 , 0x55},
	{0xc3 , 0x00},
	{0xc4 , 0x80},
	{0xc5 , 0x90},
	{0xc6 , 0x3b},
	{0xc7 , 0x46},
	{0xec , 0x06},
	{0xed , 0x04},
	{0xee , 0x60},
	{0xef , 0x90},
	{0xb6 , 0x01},
	{0x90 , 0x01},
	{0x91 , 0x00},
	{0x92 , 0x00},
	{0x93 , 0x00},
	{0x94 , 0x00},
	{0x95 , 0x04},
	{0x96 , 0xb0},
	{0x97 , 0x06},
	{0x98 , 0x40},
	/////////////////////////////////////////
	/////////// BLK ////////////////////////
	/////////////////////////////////////////
	{0xfe , 0x00},
	{0x40 , 0x42},
	{0x41 , 0x00},
	{0x43 , 0x5b}, 
	{0x5e , 0x00}, 
	{0x5f , 0x00},
	{0x60 , 0x00}, 
	{0x61 , 0x00}, 
	{0x62 , 0x00},
	{0x63 , 0x00}, 
	{0x64 , 0x00}, 
	{0x65 , 0x00}, 
	{0x66 , 0x20},
	{0x67 , 0x20}, 
	{0x68 , 0x20}, 
	{0x69 , 0x20}, 
	{0x76 , 0x00},                                  
	{0x6a , 0x08}, 
	{0x6b , 0x08}, 
	{0x6c , 0x08}, 
	{0x6d , 0x08}, 
	{0x6e , 0x08}, 
	{0x6f , 0x08}, 
	{0x70 , 0x08}, 
	{0x71 , 0x08},   
	{0x76 , 0x00},
	{0x72 , 0xf0},
	{0x7e , 0x3c},
	{0x7f , 0x00},
	{0xfe , 0x02},
	{0x48 , 0x15},
	{0x49 , 0x00},
	{0x4b , 0x0b},
	{0xfe , 0x00},
	////////////////////////////////////////
	/////////// AEC ////////////////////////
	////////////////////////////////////////
	{0xfe , 0x01},
	{0x01 , 0x04},
	{0x02 , 0xc0},
	{0x03 , 0x04},
	{0x04 , 0x90},
	{0x05 , 0x30},
	{0x06 , 0x90},
	{0x07 , 0x30},
	{0x08 , 0x80},
	{0x09 , 0x00},
	{0x0a , 0x82},
	{0x0b , 0x11},
	{0x0c , 0x10},
	{0x11 , 0x10},
	{0x13 , 0x7b},
	{0x17 , 0x00},
	{0x1c , 0x11},
	{0x1e , 0x61},
	{0x1f , 0x35},
	{0x20 , 0x40},
	{0x22 , 0x40},
	{0x23 , 0x20},
	{0xfe , 0x02},
	{0x0f , 0x04},
	{0xfe , 0x01},
	{0x12 , 0x35},
	{0x15 , 0xb0},
	{0x10 , 0x31},
	{0x3e , 0x28},
	{0x3f , 0xb0},
	{0x40 , 0x90},
	{0x41 , 0x0f},
	
	/////////////////////////////
	//////// INTPEE /////////////
	/////////////////////////////
	{0xfe , 0x02},
	{0x90 , 0x6c},
	{0x91 , 0x03},
	{0x92 , 0xcb},
	{0x94 , 0x33},
	{0x95 , 0x84},
	{0x97 , 0x65},
	{0xa2 , 0x11},
	{0xfe , 0x00},
	/////////////////////////////
	//////// DNDD///////////////
	/////////////////////////////
	{0xfe , 0x02},
	{0x80 , 0xc1},
	{0x81 , 0x08},
	{0x82 , 0x05},
	{0x83 , 0x08},
	{0x84 , 0x0a},
	{0x86 , 0xf0},
	{0x87 , 0x50},
	{0x88 , 0x15},
	{0x89 , 0xb0},
	{0x8a , 0x30},
	{0x8b , 0x10},
	/////////////////////////////////////////
	/////////// ASDE ////////////////////////
	/////////////////////////////////////////
	{0xfe , 0x01},
	{0x21 , 0x04},
	{0xfe , 0x02},
	{0xa3 , 0x50},
	{0xa4 , 0x20},
	{0xa5 , 0x40},
	{0xa6 , 0x80},
	{0xab , 0x40},
	{0xae , 0x0c},
	{0xb3 , 0x46},
	{0xb4 , 0x64},
	{0xb6 , 0x38},
	{0xb7 , 0x01},
	{0xb9 , 0x2b},
	{0x3c , 0x04},
	{0x3d , 0x15},
	{0x4b , 0x06},
	{0x4c , 0x20},
	{0xfe , 0x00},
	/////////////////////////////////////////
	/////////// GAMMA   ////////////////////////
	/////////////////////////////////////////
	
	///////////////////gamma1////////////////////
#if 1
	{0xfe , 0x02},
	{0x10 , 0x09},
	{0x11 , 0x0d},
	{0x12 , 0x13},
	{0x13 , 0x19},
	{0x14 , 0x27},
	{0x15 , 0x37},
	{0x16 , 0x45},
	{0x17 , 0x53},
	{0x18 , 0x69},
	{0x19 , 0x7d},
	{0x1a , 0x8f},
	{0x1b , 0x9d},
	{0x1c , 0xa9},
	{0x1d , 0xbd},
	{0x1e , 0xcd},
	{0x1f , 0xd9},
	{0x20 , 0xe3},
	{0x21 , 0xea},
	{0x22 , 0xef},
	{0x23 , 0xf5},
	{0x24 , 0xf9},
	{0x25 , 0xff},
#else                               
	{0xfe , 0x02},
	{0x10 , 0x0a},
	{0x11 , 0x12},
	{0x12 , 0x19},
	{0x13 , 0x1f},
	{0x14 , 0x2c},
	{0x15 , 0x38},
	{0x16 , 0x42},
	{0x17 , 0x4e},
	{0x18 , 0x63},
	{0x19 , 0x76},
	{0x1a , 0x87},
	{0x1b , 0x96},
	{0x1c , 0xa2},
	{0x1d , 0xb8},
	{0x1e , 0xcb},
	{0x1f , 0xd8},
	{0x20 , 0xe2},
	{0x21 , 0xe9},
	{0x22 , 0xf0},
	{0x23 , 0xf8},
	{0x24 , 0xfd},
	{0x25 , 0xff},
	{0xfe , 0x00},     
#endif 
	{0xfe , 0x00},     
	{0xc6 , 0x20},
	{0xc7 , 0x2b},
	///////////////////gamma2////////////////////
#if 1
	{0xfe , 0x02},
	{0x26 , 0x0f},
	{0x27 , 0x14},
	{0x28 , 0x19},
	{0x29 , 0x1e},
	{0x2a , 0x27},
	{0x2b , 0x33},
	{0x2c , 0x3b},
	{0x2d , 0x45},
	{0x2e , 0x59},
	{0x2f , 0x69},
	{0x30 , 0x7c},
	{0x31 , 0x89},
	{0x32 , 0x98},
	{0x33 , 0xae},
	{0x34 , 0xc0},
	{0x35 , 0xcf},
	{0x36 , 0xda},
	{0x37 , 0xe2},
	{0x38 , 0xe9},
	{0x39 , 0xf3},
	{0x3a , 0xf9},
	{0x3b , 0xff},
#else
	////Gamma outdoor
	{0xfe , 0x02},
	{0x26 , 0x17},
	{0x27 , 0x18},
	{0x28 , 0x1c},
	{0x29 , 0x20},
	{0x2a , 0x28},
	{0x2b , 0x34},
	{0x2c , 0x40},
	{0x2d , 0x49},
	{0x2e , 0x5b},
	{0x2f , 0x6d},
	{0x30 , 0x7d},
	{0x31 , 0x89},
	{0x32 , 0x97},
	{0x33 , 0xac},
	{0x34 , 0xc0},
	{0x35 , 0xcf},
	{0x36 , 0xda},
	{0x37 , 0xe5},
	{0x38 , 0xec},
	{0x39 , 0xf8},
	{0x3a , 0xfd},
	{0x3b , 0xff},
#endif
	/////////////////////////////////////////////// 
	///////////YCP /////////////////////// 
	/////////////////////////////////////////////// 
	{0xfe , 0x02},
	{0xd1 , 0x32},
	{0xd2 , 0x32},
	{0xd3 , 0x40},
	{0xd6 , 0xf0},
	{0xd7 , 0x10},
	{0xd8 , 0xda},
	{0xdd , 0x14},
	{0xde , 0x86},
	{0xed , 0x80},
	{0xee , 0x00},
	{0xef , 0x3f},
	{0xd8 , 0xd8},
	///////////////////abs/////////////////
	{0xfe , 0x01},
	{0x9f , 0x40},
	/////////////////////////////////////////////
	//////////////////////// LSC ///////////////
	//////////////////////////////////////////
	{0xfe , 0x01},
	{0xc2 , 0x14},
	{0xc3 , 0x0d},
	{0xc4 , 0x0c},
	{0xc8 , 0x15},
	{0xc9 , 0x0d},
	{0xca , 0x0a},
	{0xbc , 0x24},
	{0xbd , 0x10},
	{0xbe , 0x0b},
	{0xb6 , 0x25},
	{0xb7 , 0x16},
	{0xb8 , 0x15},
	{0xc5 , 0x00},
	{0xc6 , 0x00},
	{0xc7 , 0x00},
	{0xcb , 0x00},
	{0xcc , 0x00},
	{0xcd , 0x00},
	{0xbf , 0x07},
	{0xc0 , 0x00},
	{0xc1 , 0x00},
	{0xb9 , 0x00},
	{0xba , 0x00},
	{0xbb , 0x00},
	{0xaa , 0x01},
	{0xab , 0x01},
	{0xac , 0x00},
	{0xad , 0x05},
	{0xae , 0x06},
	{0xaf , 0x0e},
	{0xb0 , 0x0b},
	{0xb1 , 0x07},
	{0xb2 , 0x06},
	{0xb3 , 0x17},
	{0xb4 , 0x0e},
	{0xb5 , 0x0e},
	{0xd0 , 0x09},
	{0xd1 , 0x00},
	{0xd2 , 0x00},
	{0xd6 , 0x08},
	{0xd7 , 0x00},
	{0xd8 , 0x00},
	{0xd9 , 0x00},
	{0xda , 0x00},
	{0xdb , 0x00},
	{0xd3 , 0x0a},
	{0xd4 , 0x00},
	{0xd5 , 0x00},
	{0xa4 , 0x00},
	{0xa5 , 0x00},
	{0xa6 , 0x77},
	{0xa7 , 0x77},
	{0xa8 , 0x77},
	{0xa9 , 0x77},
	{0xa1 , 0x80},
	{0xa2 , 0x80},
	                             
	{0xfe , 0x01},
	{0xdf , 0x0d},
	{0xdc , 0x25},
	{0xdd , 0x30},
	{0xe0 , 0x77},
	{0xe1 , 0x80},
	{0xe2 , 0x77},
	{0xe3 , 0x90},
	{0xe6 , 0x90},
	{0xe7 , 0xa0},
	{0xe8 , 0x90},
	{0xe9 , 0xa0},                             
	{0xfe , 0x00},
	///////////////////////////////////////////////
	/////////// AWB////////////////////////
	///////////////////////////////////////////////
	{0xfe , 0x01}, 
	{0x4f , 0x00},
	{0x4f , 0x00},
	{0x4b , 0x01},
	{0x4f , 0x00},
	                                
	{0x4c , 0x01}, // D75
	{0x4d , 0x71},
	{0x4e , 0x01},
	{0x4c , 0x01},
	{0x4d , 0x91},
	{0x4e , 0x01},
	{0x4c , 0x01},
	{0x4d , 0x70},
	{0x4e , 0x01},
	{0x4c , 0x01}, // D65
	{0x4d , 0x90},
	{0x4e , 0x02}, 
	{0x4c , 0x01},
	{0x4d , 0xb0},
	{0x4e , 0x02},
	{0x4c , 0x01},
	{0x4d , 0x8f},
	{0x4e , 0x02},
	{0x4c , 0x01},
	{0x4d , 0x6f},
	{0x4e , 0x02},
	{0x4c , 0x01},
	{0x4d , 0xaf},
	{0x4e , 0x02},                      
	{0x4c , 0x01},
	{0x4d , 0xd0},
	{0x4e , 0x02},
	{0x4c , 0x01},
	{0x4d , 0xf0},
	{0x4e , 0x02},
	{0x4c , 0x01},
	{0x4d , 0xcf},
	{0x4e , 0x02},
	{0x4c , 0x01},
	{0x4d , 0xef},
	{0x4e , 0x02},                             
	{0x4c , 0x01},//D50
	{0x4d , 0x6e},
	{0x4e , 0x03},
	{0x4c , 0x01}, 
	{0x4d , 0x8e},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0xae},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0xce},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x4d},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x6d},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x8d},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0xad},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0xcd},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x4c},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x6c},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x8c},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0xac},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0xcc},
	{0x4e , 0x03},                           
	{0x4c , 0x01},
	{0x4d , 0xcb},
	{0x4e , 0x03},                           
	{0x4c , 0x01},
	{0x4d , 0x4b},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x6b},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0x8b},
	{0x4e , 0x03},
	{0x4c , 0x01},
	{0x4d , 0xab},
	{0x4e , 0x03},                            
	{0x4c , 0x01},//CWF
	{0x4d , 0x8a},
	{0x4e , 0x04},
	{0x4c , 0x01},
	{0x4d , 0xaa},
	{0x4e , 0x04},
	{0x4c , 0x01},
	{0x4d , 0xca},
	{0x4e , 0x04},
	{0x4c , 0x01},
	{0x4d , 0xca},
	{0x4e , 0x04},
	{0x4c , 0x01},
	{0x4d , 0xc9},
	{0x4e , 0x04},
	{0x4c , 0x01},
	{0x4d , 0x8a},
	{0x4e , 0x04},
	{0x4c , 0x01},
	{0x4d , 0x89},
	{0x4e , 0x04},
	{0x4c , 0x01},
	{0x4d , 0xa9},
	{0x4e , 0x04},                            
	{0x4c , 0x02},//tl84
	{0x4d , 0x0b},
	{0x4e , 0x05},
	{0x4c , 0x02},
	{0x4d , 0x0a},
	{0x4e , 0x05},                           
	{0x4c , 0x01},
	{0x4d , 0xeb},
	{0x4e , 0x05},                           
	{0x4c , 0x01},
	{0x4d , 0xea},
	{0x4e , 0x05},                             
	{0x4c , 0x02},
	{0x4d , 0x09},
	{0x4e , 0x05},
	{0x4c , 0x02},
	{0x4d , 0x29},
	{0x4e , 0x05},                           
	{0x4c , 0x02},
	{0x4d , 0x2a},
	{0x4e , 0x05},                            
	{0x4c , 0x02},
	{0x4d , 0x4a},
	{0x4e , 0x05},
	//{0x4c , 0x02}, //A
	//{0x4d , 0x6a},
	//{0x4e , 0x06},
	{0x4c , 0x02}, 
	{0x4d , 0x8a},
	{0x4e , 0x06}, 
	{0x4c , 0x02},
	{0x4d , 0x49},
	{0x4e , 0x06},
	{0x4c , 0x02},
	{0x4d , 0x69},
	{0x4e , 0x06},
	{0x4c , 0x02},
	{0x4d , 0x89},
	{0x4e , 0x06},
	{0x4c , 0x02},
	{0x4d , 0xa9},
	{0x4e , 0x06}, 
	{0x4c , 0x02},
	{0x4d , 0x48},
	{0x4e , 0x06},
	{0x4c , 0x02},
	{0x4d , 0x68},
	{0x4e , 0x06},
	{0x4c , 0x02},
	{0x4d , 0x69},
	{0x4e , 0x06}, 
	{0x4c , 0x02},//H
	{0x4d , 0xca},
	{0x4e , 0x07},
	{0x4c , 0x02},
	{0x4d , 0xc9},
	{0x4e , 0x07},
	{0x4c , 0x02},
	{0x4d , 0xe9},
	{0x4e , 0x07},
	{0x4c , 0x03},
	{0x4d , 0x09},
	{0x4e , 0x07},
	{0x4c , 0x02},
	{0x4d , 0xc8},
	{0x4e , 0x07},
	{0x4c , 0x02},
	{0x4d , 0xe8},
	{0x4e , 0x07},
	{0x4c , 0x02},
	{0x4d , 0xa7},
	{0x4e , 0x07},
	{0x4c , 0x02},
	{0x4d , 0xc7},
	{0x4e , 0x07},
	{0x4c , 0x02},
	{0x4d , 0xe7},
	{0x4e , 0x07},
	{0x4c , 0x03},
	{0x4d , 0x07},
	{0x4e , 0x07},
	                             
	{0x4f , 0x01},
	{0x50 , 0x80},
	{0x51 , 0xa8},
	{0x52 , 0x47},
	{0x53 , 0x38},
	{0x54 , 0xc7},
	{0x56 , 0x0e},
	{0x58 , 0x08},
	{0x5b , 0x00},
	{0x5c , 0x74},
	{0x5d , 0x8b},
	{0x61 , 0xdb},
	{0x62 , 0xb8},
	{0x63 , 0x86},
	{0x64 , 0xc0},
	{0x65 , 0x04},
	{0x67 , 0xa8},
	{0x68 , 0xb0},
	{0x69 , 0x00},
	{0x6a , 0xa8},
	{0x6b , 0xb0},
	{0x6c , 0xaf},
	{0x6d , 0x8b},
	{0x6e , 0x50},
	{0x6f , 0x18},
	{0x73 , 0xf0},
	{0x70 , 0x0d},
	{0x71 , 0x60},
	{0x72 , 0x80},
	{0x74 , 0x01},
	{0x75 , 0x01},
	{0x7f , 0x0c},
	{0x76 , 0x70},
	{0x77 , 0x58},
	{0x78 , 0xa0},
	{0x79 , 0x5e},
	{0x7a , 0x54},
	{0x7b , 0x58},
	{0xfe , 0x00},
	//////////////////////////////////////////
	///////////CC////////////////////////
	//////////////////////////////////////////
	{0xfe , 0x02},
	{0xc0 , 0x01},                                   
	{0xc1 , 0x44},
	{0xc2 , 0xfd},
	{0xc3 , 0x04},
	{0xc4 , 0xF0},
	{0xc5 , 0x48},
	{0xc6 , 0xfd},
	{0xc7 , 0x46},
	{0xc8 , 0xfd},
	{0xc9 , 0x02},
	{0xca , 0xe0},
	{0xcb , 0x45},
	{0xcc , 0xec},                         
	{0xcd , 0x48},
	{0xce , 0xf0},
	{0xcf , 0xf0},
	{0xe3 , 0x0c},
	{0xe4 , 0x4b},
	{0xe5 , 0xe0},
	//////////////////////////////////////////
	///////////ABS ////////////////////
	//////////////////////////////////////////
	{0xfe , 0x01},
	{0x9f , 0x40},
	{0xfe , 0x00}, 
	//////////////////////////////////////
	///////////  OUTPUT   ////////////////
	//////////////////////////////////////
	{0xfe, 0x00},
	{0xf2, 0x0f},
	///////////////dark sun////////////////////
	{0xfe , 0x02},
	{0x40 , 0xbf},
	{0x46 , 0xcf},
	{0xfe , 0x00},
	
	//////////////frame rate 50Hz/////////
	{0xfe , 0x00},
	{0x05 , 0x01},
	{0x06 , 0x56},
	{0x07 , 0x00},
	{0x08 , 0x32},
	{0xfe , 0x01},
	{0x25 , 0x00},
	{0x26 , 0xfa}, 
	{0x27 , 0x04}, 
	{0x28 , 0xe2}, //20fps 
	{0x29 , 0x06}, 
	{0x2a , 0xd6}, //14fps 
	{0x2b , 0x07}, 
	{0x2c , 0xd0}, //12fps
	{0x2d , 0x0b}, 
	{0x2e , 0xb8}, //8fps
	{0xfe , 0x00},
};

__align(4) const SENSOR_REG_T GC2145_YUV_800x600[]=
{
	{0xfe , 0x00},
	{0xb6 , 0x01},//add aec_en		
	{0xfa , 0x00},
	{0xfd , 0x01}, 
	//// crop window              
	{0xfe , 0x00},
	{0x99 , 0x11},  
	{0x9a , 0x06},
	{0x9b , 0x00},
	{0x9c , 0x00},
	{0x9d , 0x00},
	{0x9e , 0x00},
	{0x9f , 0x00},
	{0xa0 , 0x00},  
	{0xa1 , 0x00},
	{0xa2  ,0x00},
	{0x90 , 0x01}, 
	{0x91 , 0x00},
	{0x92 , 0x00},
	{0x93 , 0x00},
	{0x94 , 0x00},
	{0x95 , 0x02},
	{0x96 , 0x58},
	{0x97 , 0x03},
	{0x98 , 0x20},
	//// AWB                      
	{0xfe , 0x00},
	{0xec , 0x02}, 
	{0xed , 0x02},
	{0xee , 0x30},
	{0xef , 0x48},
	{0xfe , 0x02},
	{0x9d , 0x0b},
	{0xfe , 0x01},
	{0x74 , 0x00}, 
	//// AEC                      
	{0xfe , 0x01},
	{0x01 , 0x04},
	{0x02 , 0x60},
	{0x03 , 0x02},
	{0x04 , 0x48},
	{0x05 , 0x18},
	{0x06 , 0x50},
	{0x07 , 0x10},
	{0x08 , 0x38},
	{0x0a , 0x80}, 
	{0x21 , 0x04},
	{0xfe, 0x00},
	{0x20, 0x03},
	{0xfe, 0x00},
};

__align(4) const SENSOR_REG_T GC2145_YUV_1280x960[]=
{
	///1280X960//////
	{0xfe , 0x00},
	{0xfa , 0x11},
	{0xfd , 0x00},
	{0x1c , 0x05},
	//// crop window
	{0xfe , 0x00},
	{0x99 , 0x55},  
	{0x9a , 0x06},
	{0x9b , 0x00},
	{0x9c , 0x00},
	{0x9d , 0x01},
	{0x9e , 0x23},
	{0x9f , 0x00},
	{0xa0 , 0x00},  
	{0xa1 , 0x01},
	{0xa2 , 0x23},
	{0x90 , 0x01},
	{0x91 , 0x00},
	{0x92 , 0x00},
	{0x93 , 0x00},
	{0x94 , 0x00},
	{0x95 , 0x03},
	{0x96 , 0xc0},
	{0x97 , 0x05},
	{0x98 , 0x00},

	//// AWB   
	{0xfe , 0x00},
	{0xec , 0x06},
	{0xed , 0x04},
	{0xee , 0x60},
	{0xef , 0x90},
	{0xfe , 0x01},
	{0x74 , 0x01},
	//// AEC	  
	{0xfe , 0x01},
	{0x01 , 0x04},
	{0x02 , 0xc0},
	{0x03 , 0x04},
	{0x04 , 0x90},
	{0x05 , 0x30},
	{0x06 , 0x90},
	{0x07 , 0x30},
	{0x08 , 0x80},
	{0x0a , 0x82}, 
	{0x21 , 0x15}, 
	{0xfe, 0x00},
	{0x20, 0x15},
	{0xfe, 0x00},
};
	
__align(4) const SENSOR_REG_T GC2145_YUV_1600x1200[]=
{
	{0xfe , 0x00},
	{0xfa , 0x11},
	{0xfd , 0x00},
	{0x1c , 0x05},
	//// crop window
	{0xfe , 0x00},
	{0x99 , 0x11},  
	{0x9a , 0x06},
	{0x9b , 0x00},
	{0x9c , 0x00},
	{0x9d , 0x00},
	{0x9e , 0x00},
	{0x9f , 0x00},
	{0xa0 , 0x00},  
	{0xa1 , 0x00},
	{0xa2  ,0x00},
	{0x90 , 0x01},
	{0x91 , 0x00},
	{0x92 , 0x00},
	{0x93 , 0x00},
	{0x94 , 0x00},
	{0x95 , 0x04},
	{0x96 , 0xb0},
	{0x97 , 0x06},
	{0x98 , 0x40},
	//// AWB   
	{0xfe , 0x00},
	{0xec , 0x06},
	{0xed , 0x04},
	{0xee , 0x60},
	{0xef , 0x90},
	{0xfe , 0x01},
	{0x74 , 0x01},
	//// AEC	  
	{0xfe , 0x01},
	{0x01 , 0x04},
	{0x02 , 0xc0},
	{0x03 , 0x04},
	{0x04 , 0x90},
	{0x05 , 0x30},
	{0x06 , 0x90},
	{0x07 , 0x30},
	{0x08 , 0x80},
	{0x0a , 0x82}, 
	{0x21 , 0x15}, 
	{0xfe, 0x00},
	{0x20, 0x15},
	{0xfe, 0x00},
};

LOCAL SENSOR_REG_TAB_INFO_T s_GC2145_resolution_Tab_YUV[]=
{   
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(GC2145_YUV_COMMON), 800, 600, 24, SENSOR_IMAGE_FORMAT_YUV422},
	// YUV422 PREVIEW 1	
	{ADDR_AND_LEN_OF_ARRAY(GC2145_YUV_800x600), 800, 600, 24, SENSOR_IMAGE_FORMAT_YUV422},
	{ADDR_AND_LEN_OF_ARRAY(GC2145_YUV_1280x960), 1280, 960, 24, SENSOR_IMAGE_FORMAT_YUV422},
	{ADDR_AND_LEN_OF_ARRAY(GC2145_YUV_1600x1200), 1600, 1200, 24, SENSOR_IMAGE_FORMAT_YUV422},
	{ PNULL,                    0,      0,  0  ,        0,        0      },   

	// YUV422 PREVIEW 2 
	{ PNULL,                   0,      0,  0 ,        0,         0     },  //For BCR Preview 
	{ PNULL,                   0,      0,  0  ,        0,        0      },   
	{ PNULL,                   0,      0,  0  ,        0,        0      },   
	{ PNULL,                   0,      0,  0  ,        0,        0      }
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC2145_ioctl_func_tab = 
{
	// Internal 
	PNULL,
	PNULL, 
	PNULL,
	GC2145_Identify,

	PNULL,          // write register
	PNULL,          // read  register   
	PNULL,
	PNULL,

	// External
	set_gc2145_ae_enable,
	PNULL,//set_hmirror_enable,
	PNULL,//set_vmirror_enable,

	set_brightness,
	set_contrast,
	set_sharpness,
	set_saturation,

	set_preview_mode,   
	set_image_effect,

	GC2145_BeforeSnapshot,
	GC2145_After_Snapshot,

	PNULL,

	read_ev_value,
	write_ev_value,
	read_gain_value,
	write_gain_value,
	read_gain_scale,
	set_frame_rate, 
	PNULL,
	PNULL,
	set_gc2145_awb,
	PNULL,
	PNULL,
	set_gc2145_ev,
	PNULL,
	PNULL,
	PNULL,
	PNULL,
	PNULL,
	set_gc2145_anti_flicker,
	set_gc2145_video_mode,
	PNULL,
	PNULL
};

/**---------------------------------------------------------------------------*
**                         Global Variables                                  *
**---------------------------------------------------------------------------*/
PUBLIC const SENSOR_INFO_T g_GC2145_yuv_info =
{
	GC2145_I2C_ADDR_W,              // salve i2c write address
	GC2145_I2C_ADDR_R,              // salve i2c read address

	0|\
	SENSOR_I2C_FREQ_100,                                // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
	                                // bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
	                                // other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_P,       // bit0: 0:negative; 1:positive -> polarily of pixel clock
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

	SENSOR_LOW_PULSE_RESET,         // reset pulse level
	100,                             // reset pulse width(ms)

	SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid

	2,                              // count of identify code
	0xf0, 0x21,                     // supply two code to identify sensor.
	0xf1, 0x45,                     // for Example: index = 0-> Device id, index = 1 -> version id  
	                                
	SENSOR_AVDD_2800MV,             // voltage of avdd  

	1600,                            // max width of source image
	1200,                            // max height of source image
	"GC2145",                       // name of sensor                                               

	SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
				                                // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

	s_GC2145_resolution_Tab_YUV,    // point to resolution table information structure
	&s_GC2145_ioctl_func_tab,       // point to ioctl function table
	        
	PNULL,                          // information and table about Rawrgb sensor
	PNULL,                          // extend information about sensor  
	SENSOR_AVDD_1800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	4,
	5,
	0,
	2,
	0,
	0,
	0,
	0,
	0,
	0
};
/**---------------------------------------------------------------------------*
**                             Function  Definitions
**---------------------------------------------------------------------------*/
LOCAL void GC2145_WriteReg( uint8  subaddr, uint8 data )
{
#ifndef _USE_DSP_I2C_
	//uint8 cmd[2];
	//cmd[0]    =   subaddr;
	//cmd[1]    =   data;       
	//I2C_WriteCmdArr(GC2145_I2C_ADDR_W, cmd, 2, SCI_TRUE);
	Sensor_WriteReg(subaddr, data);
#else
	DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
#endif
	SCI_TRACE_LOW("SENSOR: GC2145_WriteReg reg/value(%x,%x) !!", subaddr, data);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_690_112_2_18_0_30_46_779,(uint8*)"dd", subaddr, data);
}

LOCAL uint8 GC2145_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
#ifndef _USE_DSP_I2C_
	value =Sensor_ReadReg( subaddr);
#else
	value = (uint16)DSENSOR_IICRead((uint16)subaddr);
#endif
	SCI_TRACE_LOW("SENSOR: GC2145_ReadReg reg/value(%x,%x) !!", subaddr, value);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_706_112_2_18_0_30_46_780,(uint8*)"dd", subaddr, value);
	return value;
}

LOCAL uint32 GC2145_Identify(uint32 param)
{
#define GC2145_PID_VALUE    0xf0    
#define GC2145_PID_ADDR     0x21
#define GC2145_VER_VALUE    0xf1    
#define GC2145_VER_ADDR     0x45    

	uint32 i;
	uint32 nLoop;
	uint8 ret;
	uint32 err_cnt = 0;
	uint8 reg[2]    = {0xf0, 0xf1};
	uint8 value[2]  = {0x21, 0x45};
	GC2145_Power_On(TRUE);

	SCI_TRACE_LOW("GC2145_Identify");
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_725_112_2_18_0_30_46_781,(uint8*)"");
	for(i = 0; i<2; )
	{
		nLoop = 1000;
		ret = GC2145_ReadReg(reg[i]);
		if( ret != value[i])
		{
			err_cnt++;
			if(err_cnt>3)
			{
				SCI_TRACE_LOW("It is not GC2145");
				//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_735_112_2_18_0_30_46_782,(uint8*)"");
				return SCI_ERROR;
			}
			else
			{
				//Masked by frank.yang,SCI_Sleep() will cause a  Assert when called in boot precedure
				//SCI_Sleep(10);
				while(nLoop--)
				    { ;}
				continue;
			}
		}
		err_cnt = 0;
		i++;
	}

	SCI_TRACE_LOW("GC2145_Identify: it is GC2145");
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_753_112_2_18_0_30_46_783,(uint8*)"");

	return (uint32)SCI_SUCCESS;
}

LOCAL uint32 set_gc2145_ae_enable(uint32 enable)
{
	SCI_TRACE_LOW("set_gc2145_ae_enable: enable = %d", enable);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_760_112_2_18_0_30_46_784,(uint8*)"d", enable);
	return 0;
}


LOCAL void GC2145_set_shutter()
{
	uint16   shutter = 0 ;
	
	Sensor_WriteReg(0xfe,0x00);
	Sensor_WriteReg(0xb6,0x00);  //AEC CLOSE
	shutter = (Sensor_ReadReg(0x03)<<8 )|( Sensor_ReadReg(0x04));    

	shutter = shutter /2 ;
	
	if(shutter < 1)
	{
	   shutter = 1;
	}
	Sensor_WriteReg(0x03, (shutter >> 8)&0xff); /* Shutter */
  	Sensor_WriteReg(0x04, shutter&0xff);
	SCI_Sleep(300); 

}


/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T gc2145_brightness_tab[][4]=
{
	{{0xfe , 0x02},{0xd5 , 0xc0},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x02},{0xd5 , 0xe0},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x02},{0xd5 , 0xf0},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x02},{0xd5 , 0x00},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x02},{0xd5 , 0x10},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x02},{0xd5 , 0x20},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x02},{0xd5 , 0x30},{0xfe , 0x00},{0xff , 0xff}}
};

LOCAL uint32 set_brightness(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc2145_brightness_tab[level];

	SCI_ASSERT(level < 7);    /*assert to do*/
	//  SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		GC2145_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//  OS_TickDelay(100); 
	SCI_TRACE_LOW("set_brightness: level = %d", level);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_863_112_2_18_0_30_47_787,(uint8*)"d", level);

	return 0;
}

__align(4) const SENSOR_REG_T GC2145_ev_tab[][4]=
{   
 	{{0xfe , 0x01},{0x13 , 0x65},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x01},{0x13 , 0x70},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x01},{0x13 , 0x75},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x01},{0x13 , 0x7b},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x01},{0x13 , 0x80},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x01},{0x13 , 0x85},{0xfe , 0x00},{0xff , 0xff}},
	{{0xfe , 0x01},{0x13 , 0x90},{0xfe , 0x00},{0xff , 0xff}}
};


LOCAL uint32 set_gc2145_ev(uint32 level)
{
	uint16 i; 
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2145_ev_tab[level];

	SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert to do*/
	//    SCI_ASSERT(level < 7);    /*assert verified*/

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) ||(0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		GC2145_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	SCI_TRACE_LOW("SENSOR: set_ev: level = %d", level);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_895_112_2_18_0_30_47_788,(uint8*)"d", level);
	return 0;
}

/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_gc2145_anti_flicker(uint32 param )
{
	switch (param)
	{
		case DCAMERA_FLICKER_50HZ:
			
		GC2145_WriteReg(0x05 , 0x01);//hb
		GC2145_WriteReg(0x06 , 0x56);
		GC2145_WriteReg(0x07 , 0x00);//vb
		GC2145_WriteReg(0x08 , 0x32);
		GC2145_WriteReg(0xfe , 0x01);
		GC2145_WriteReg(0x25 , 0x00);//step
		GC2145_WriteReg(0x26 , 0xfa);
		GC2145_WriteReg(0x27 , 0x04);//level1
		GC2145_WriteReg(0x28 , 0xe2);
		GC2145_WriteReg(0x29 , 0x06);//level2
		GC2145_WriteReg(0x2a , 0xd6);
		GC2145_WriteReg(0x2b , 0x07);////level3
		GC2145_WriteReg(0x2c , 0xd0);
		GC2145_WriteReg(0x2d , 0x0b);//level4 7.14fps
		GC2145_WriteReg(0x2e , 0xb8);
		GC2145_WriteReg(0xfe , 0x00);
			
		SCI_TRACE_LOW("DEBUG_gc2145 anti flick 50HZ!!!!");
		//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_923_112_2_18_0_30_47_789,(uint8*)"");
		break;

		case DCAMERA_FLICKER_60HZ:
			
		GC2145_WriteReg(0x05,0x01);//hb
		GC2145_WriteReg(0x06,0x58);
		GC2145_WriteReg(0x07,0x00);//vb
		GC2145_WriteReg(0x08,0x32);
		GC2145_WriteReg(0xfe,0x01);
		GC2145_WriteReg(0x25,0x00);//step
		GC2145_WriteReg(0x26,0xd0);
		GC2145_WriteReg(0x27 , 0x04);//level1
		GC2145_WriteReg(0x28 , 0xe0);
		GC2145_WriteReg(0x29 , 0x06);//level2
		GC2145_WriteReg(0x2a , 0x80);
		GC2145_WriteReg(0x2b , 0x08);//level3
		GC2145_WriteReg(0x2c , 0x20);
		GC2145_WriteReg(0x2d,0x0b);//level4
		GC2145_WriteReg(0x2e,0x60);
		GC2145_WriteReg(0xfe,0x00);		
			
		SCI_TRACE_LOW("DEBUG_gc2145 anti flick 60HZ!!!!");
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_940_112_2_18_0_30_47_790,(uint8*)"");
	break;

	default:
	break;
	}

	return 0;
}

LOCAL uint32 set_gc2145_video_mode(uint32 mode)
{
	SCI_TRACE_LOW("SENSOR: set_video_mode: mode = %d", mode);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_991_112_2_18_0_30_47_791,(uint8*)"d", mode);
	return 0;
}


/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T gc2145_awb_tab[][6]=
{
	//AUTO
	{{0xfe , 0x00},{0xb3 , 0x60},{0xb4 , 0x40},{0xb5 , 0x60},{0x82 , 0xfe},{0xff , 0xff}},    
	//INCANDESCENCE:
	{{0xfe , 0x00},{0x82 , 0xfc},{0xb3 , 0x50},{0xb4 , 0x40},{0xb5 , 0xa8},{0xff , 0xff}},
	//U30 ?
	{{0xfe , 0x00},{0x82 , 0xfc},{0xb3 , 0x50},{0xb4 , 0x40},{0xb5 , 0xa8},{0xff , 0xff}},  
	//CWF ?
	{{0xfe , 0x00},{0x82 , 0xfc},{0xb3 , 0x50},{0xb4 , 0x40},{0xb5 , 0xa8},{0xff , 0xff}},    
	//FLUORESCENT:
	{{0xfe , 0x00},{0x82 , 0xfc},{0xb3 , 0xa0},{0xb4 , 0x45},{0xb5 , 0x40},{0xff , 0xff}},
	//SUN:
	{{0xfe , 0x00},{0x82 , 0xfc},{0xb3 , 0x78},{0xb4 , 0x40},{0xb5 , 0x50},{0xff , 0xff}},
	//CLOUD:
	{{0xfe , 0x00},{0x82 , 0xfc},{0xb3 , 0x58},{0xb4 , 0x40},{0xb5 , 0x50},{0xff , 0xff}},
};

/*
// enum: preview while balance mode
typedef enum
{
    DCAMERA_WB_MODE_AUTO = 0x00,            //×Ô¶¯
    DCAMERA_WB_MODE_INCANDESCENCE,      //°×³ãµÆ
    DCAMERA_WB_MODE_U30,                //ÉÌÓÃ¹âÔ´
    DCAMERA_WB_MODE_CWF,                //ÀäÓ«¹â
    DCAMERA_WB_MODE_FLUORESCENT,        //ÈÕ¹âµÆ
    DCAMERA_WB_MODE_SUN,                //ÇçÌì
    DCAMERA_WB_MODE_CLOUD,              //ÒõÌì
    DCAMERA_WB_MODE_MAX
}DCAMERA_PARAM_WB_MODE_E;
*/
    
LOCAL uint32 set_gc2145_awb(uint32 mode)
{
	uint16 i;

	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc2145_awb_tab[mode];

	SCI_ASSERT(mode < 7);    /*assert verified*/
	//      SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

	SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		GC2145_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//      OS_TickDelay(100); 
	SCI_TRACE_LOW("SENSOR: set_awb_mode: mode = %d", mode);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_1159_112_2_18_0_30_47_792,(uint8*)"d", mode);

	return 0;
}

__align(4) const SENSOR_REG_T gc2145_contrast_tab[][4]=
{
	{{0xfe,0x02}, {0xd3,0x30}, {0xfe,0x00}, {0xff,0xff}},
	{{0xfe,0x02}, {0xd3,0x34}, {0xfe,0x00}, {0xff,0xff}},
	{{0xfe,0x02}, {0xd3,0x38}, {0xfe,0x00}, {0xff,0xff}},
	{{0xfe,0x02}, {0xd3,0x40}, {0xfe,0x00}, {0xff,0xff}},
	{{0xfe,0x02}, {0xd3,0x44}, {0xfe,0x00}, {0xff,0xff}},
	{{0xfe,0x02}, {0xd3,0x48}, {0xfe,0x00}, {0xff,0xff}},
	{{0xfe,0x02}, {0xd3,0x50}, {0xfe,0x00}, {0xff,0xff}}
};

LOCAL uint32 set_contrast(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr;

	sensor_reg_ptr = (SENSOR_REG_T*)gc2145_contrast_tab[level];

	SCI_ASSERT(level < 7 );    /*assert verified*/
	SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		GC2145_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	//    OS_TickDelay(20);
	SCI_TRACE_LOW("set_contrast: level = %d", level);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_1211_112_2_18_0_30_47_793,(uint8*)"d", level);
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
//      level  must smaller than 8
/******************************************************************************/

LOCAL uint32 set_preview_mode(uint32 preview_mode)
{
	SCI_TRACE_LOW("set_preview_mode: preview_mode = %d", preview_mode);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_1238_112_2_18_0_30_47_794,(uint8*)"d", preview_mode);

	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{

			GC2145_WriteReg(0xfe , 0x01);
			GC2145_WriteReg(0x3c , 0x40);
			GC2145_WriteReg(0xfe , 0x00);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			

			GC2145_WriteReg(0xfe , 0x01);
			GC2145_WriteReg(0x3c , 0x60);
			GC2145_WriteReg(0xfe , 0x00);
			break;
		}
		default:
		{
			break;
		}
	} 

	SCI_TRACE_LOW("set_GC2145_preview_mode: level = %d", preview_mode);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_1256_112_2_18_0_30_47_795,(uint8*)"d", preview_mode);
	return 0;
}

/*
typedef enum
{
    DCAMERA_EFFECT_NORMAL = 0x00,
    DCAMERA_EFFECT_BLACKWHITE,          // ºÚ°×
    DCAMERA_EFFECT_RED,                 // Æ«ºì
    DCAMERA_EFFECT_GREEN,               // Æ«ÂÌ
    DCAMERA_EFFECT_BLUE,                // Æ«À¶
    DCAMERA_EFFECT_YELLOW,              // Æ«»Æ
    DCAMERA_EFFECT_NEGATIVE,            // µ×Æ¬
    DCAMERA_EFFECT_CANVAS,              // ·«²¼
    DCAMERA_EFFECT_RELIEVOS,            // ¸¡µñ 
    DCAMERA_EFFECT_MAX
}DCAMERA_PARAM_EFFECT_E;
*/

__align(4) const SENSOR_REG_T GC2145_image_effect_tab[][2]=    
{
	//Normal
	{{0x83 , 0xe0},{0xff , 0xff}},
	//BLACK&WHITE
	{{0x83 , 0x12},{0xff , 0xff}},
	//RED
	{{0x83 , 0x12},{0xff , 0xff}},
	//GREEN
	{{0x83 , 0x52},{0xff , 0xff}},
	//BLUE
	{{0x83 , 0x62},{0xff , 0xff}},
	//YELLOW
	{{0x83 , 0x12},{0xff , 0xff}},
	//NEGATIVE
	{{0x83 , 0x01},{0xff , 0xff}},
	//SEPIA
	{{0x83 , 0x82},{0xff , 0xff}}
};
LOCAL uint32 set_image_effect(uint32 effect_type)
{
	uint16 i;

	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2145_image_effect_tab[effect_type];

	SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
	Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	SCI_TRACE_LOW("-----------set_image_effect: effect_type = %d------------", effect_type);
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_1335_112_2_18_0_30_47_796,(uint8*)"d", effect_type);

	return 0;
}

LOCAL uint32 GC2145_After_Snapshot(uint32 param)
{
    SCI_Sleep(200);
    return 0;
}

LOCAL uint32 GC2145_BeforeSnapshot(uint32 param)
{
	Sensor_SetMode(param);
        switch(param)
        {
                case SENSOR_MODE_PREVIEW_ONE:    //VGA
                        //printk("Capture VGA Size");
                break;

                case SENSOR_MODE_SNAPSHOT_ONE_FIRST:  
                case SENSOR_MODE_SNAPSHOT_ONE_SECOND:    // 2 M
                	{				
				GC2145_set_shutter();	 
				SCI_Sleep(300);
                        //printk("Capture 1.3M&2M Size");            
                	}
                default:
                        break;
        }

        //printk("SENSOR_GC2145: Before Snapshot");

        return 0;
    
}

LOCAL void GC2145_Delayms (uint32 ms)	
{
	uint32 t1, t2;
	
	t1 = t2 = SCI_GetTickCount ();
	
	do{
		t2 = SCI_GetTickCount ();
	}while (t2 < (t1+ms));
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
    //GC2145_WriteReg( 0xd8, uint8 data );
    return 0;
}
LOCAL uint32 GC2145_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E		dvdd_val=g_GC2145_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E		avdd_val=g_GC2145_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E		iovdd_val=g_GC2145_yuv_info.iovdd_val;  
    BOOLEAN 				power_down=g_GC2145_yuv_info.power_down_level;	    
    BOOLEAN 				reset_level=g_GC2145_yuv_info.reset_pulse_level;
    uint32 				reset_width=g_GC2145_yuv_info.reset_pulse_width;	    
    
    if(SCI_TRUE==power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
            if(SENSOR_MAIN==Sensor_GetCurId())
		{
	         GPIO_SetSensorPwdn(!power_down);
		  GPIO_SetFrontSensorPwdn(power_down);			
            	}
		else
		{
	         GPIO_SetSensorPwdn(power_down);
		  GPIO_SetFrontSensorPwdn(!power_down);	
		}
        // Open Mclk in default frequency
        Sensor_SetMCLK(24);   

	 Sensor_SetResetLevel(!reset_level);
        GC2145_Delayms(100);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel(!reset_level);
        GC2145_Delayms(100);
    }
    else
    {
        GPIO_SetSensorPwdn(power_down);

        Sensor_SetMCLK(SENSOR_DISABLE_MCLK);           

        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);        
    }
    
    SCI_TRACE_LOW("set_GC2145_Power_On");
    return SCI_SUCCESS;
}
/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T gc2145_mode_tab[][7]=
{
	{{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},},
	{{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},},
};

LOCAL uint32 GC2145_set_work_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc2145_mode_tab[mode];

    SCI_ASSERT(mode <= 1);    /*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        GC2145_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("set_work_mode: mode = %d", mode);
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2145_1469_112_2_18_0_30_48_798,(uint8*)"d", mode);
    return 0;
}

