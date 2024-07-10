/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_GC2015.c                                           *
 ** Author: Leo lee                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor GC2015. 
 **                                                          
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2011.3.21    Leo            Update
 ** 2011.3.23      Leo          Update shutter,setting
 ** 2011.6.21      Leo          Update debug mode
 ** 2012.2.28    Leo            48Mclk
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
#include "gpio_prod_api.h"

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

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define GC2015_I2C_ADDR_W       0x60
#define GC2015_I2C_ADDR_R       0x61
//#define SENSOR_WRITE_DELAY      0xffff
#define GC2015_DEBUG
#define GC2015_48MHz_MCLK

#define GC2015_SET_PAGE0 Sensor_WriteReg(0xfe , 0x00) 
#define GC2015_SET_PAGE1 Sensor_WriteReg(0xfe , 0x01) 

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 GC2015_Power_On(uint32 power_on);
LOCAL uint32 GC2015_Identify(uint32 param);
LOCAL void   GC2015_WriteReg( uint8  subaddr, uint8 data );
LOCAL uint8  GC2015_ReadReg( uint8  subaddr);
LOCAL uint32 GC2015_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr );
LOCAL uint32 GC2015_set_ae_enable(uint8 enable);
LOCAL uint32 GC2015_set_awb_enable(uint8 enable);
LOCAL void   GC2015_set_shutter(void);
LOCAL uint32 GC2015_before_snapshot(uint32 param);
LOCAL uint32 GC2015_after_snapshot(uint32 param);
LOCAL uint32 GC2015_set_hmirror_enable(uint32 enable);
LOCAL uint32 GC2015_set_vmirror_enable(uint32 enable);
LOCAL uint32 GC2015_set_brightness(uint32 level);
LOCAL uint32 GC2015_set_contrast(uint32 level);
LOCAL uint32 GC2015_set_preview_mode(uint32 preview_mode);
LOCAL uint32 GC2015_set_image_effect(uint32 effect_type);
LOCAL uint32 GC2015_set_ev(uint32 level);
LOCAL uint32 GC2015_set_gamma(uint32 level);
LOCAL uint32 GC2015_set_awb(uint32 mode);
LOCAL uint32 GC2015_set_anti_flicker(uint32 mode);
LOCAL uint32 GC2015_set_video_mode(uint32 mode);
LOCAL uint32 GC2015_Check_Image_Format_Support(uint32 param);
LOCAL uint32 GC2015_Change_Image_Format(uint32 param);
LOCAL uint32 GC2015_set_flash_enable(uint32 enable);
/**---------------------------------------------------------------------------*
 **                         Global Variables                                 *
 **---------------------------------------------------------------------------*/

LOCAL uint32 hb_add = 0;//add your HB here when capture

LOCAL uint32  mirror_setting = 0;     //mirror flip setting 

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
__align(4) const SENSOR_REG_T GC2015_YUV_COMMON[] = 
{
    {0xfe , 0x80},
    {0xfe , 0x80},
    {0xfe , 0x80},

    {0xfe , 0x00},
    {0x45 , 0x00},
    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////preview capture switch ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //preview
    {0x02 , 0x01}, //preview mode
    {0x2a , 0xca}, //[7]col_binning , 0x[6]even skip
    {0x48 , 0x40},//manual gain
    {0x59 , 0x55}, //[7:4]row sample , 0x[3:0]col sample 
    {0x5a , 0x06},
    {0x5b , 0x01},
    {0x5c , 0x23},
    {0x5d , 0x00},
    {0x5e , 0x00},
    {0x5f , 0x01},
    {0x60 , 0x23},
    {0x61 , 0x00},
    {0x62 , 0x00},

    {0xfe , 0x01}, //page1

    {0x0a , 0x05}, //c_big_win_x0
    {0x0b , 0x05}, //c_big_win_y0
    {0x0c , 0xc6}, //c_big_win_x0
    {0x0d , 0x4a}, //c_big_win_y0

    {0xb0 , 0x13}, //[4]Y_LSC_en [3]lsc_compensate [2]signed_b4 [1:0]pixel array select
    {0xb1 , 0x20}, //P_LSC_red_b2
    {0xb2 , 0x20}, //P_LSC_green_b2
    {0xb3 , 0x20}, //P_LSC_blue_b2
    {0xb4 , 0x20}, //P_LSC_red_b4
    {0xb5 , 0x20}, //P_LSC_green_b4
    {0xb6 , 0x20}, //P_LSC_blue_b4
    {0xb7 , 0x00}, //P_LSC_compensate_b2
    {0xb8 , 0x80}, //P_LSC_row_center , 0x344 , 0x (600/2-100)/2=100
    {0xb9 , 0x80}, //P_LSC_col_center , 0x544 , 0x (800/2-200)/2=100

    {0xba , 0x13}, //[4]Y_LSC_en [3]lsc_compensate [2]signed_b4 [1:0]pixel array select
    {0xbb , 0x20}, //P_LSC_red_b2
    {0xbc , 0x20}, //P_LSC_green_b2
    {0xbd , 0x20}, //P_LSC_blue_b2
    {0xbe , 0x20}, //P_LSC_red_b4
    {0xbf , 0x20}, //P_LSC_green_b4
    {0xc0 , 0x20}, //P_LSC_blue_b4
    {0xc1 , 0x00}, //P_LSC_compensate_b2
    {0xc2 , 0x80}, //P_LSC_row_center , 0x344 , 0x (600/2-100)/2=100
    {0xc3 , 0x80}, //P_LSC_col_center , 0x544 , 0x (800/2-200)/2=100

    {0xfe , 0x00}, //page0
    /////////////////////////////////////////////////////////////////
    //////////////////////////analog configure /////////////////////////
    /////////////////////////////////////////////////////////////////
//  {0x29 , 0x00}, //cisctl mode 1              //conflict with mirror/flip function, so mask
    {0x2b , 0x06}, //cisctl mode 3
    {0x32 , 0x0c}, //analog mode 1
    {0x33 , 0x0f}, //analog mode 2
    {0x34 , 0x00}, //[6:4]da_rsg
    {0x35 , 0x88}, //Vref_A25
    //{0x37 , 0x16}, //driver current
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////  ISP Related  //////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x40 , 0xff},
    {0x41 , 0x24}, //[5]skin_detection enable [2]auto_gray , 0x[1]y_gamma
    {0x42 , 0x76}, //[7]auto_sa [6]auto_ee [5]auto_dndd [4]auto_lsc [3]na [2]abs , 0x[1]awb
    {0x4b , 0xea}, //[1]AWB_gain_mode , 0x1:at pregain 0:at postgain
    {0x4d , 0x03}, //[1]inbf_en
    {0x4f , 0x01}, //AEC enable

    ///////////////////////////////////////////////////
    ///////////////////////////  BLK  //////////////////
    ///////////////////////////////////////////////////
    {0x63 , 0x77}, //BLK mode 1
    {0x66 , 0x00}, //BLK global offset
    {0x6d , 0x04},
    {0x6e , 0x18}, //BLK offset submode,offset G
    {0x6f , 0x10}, //BLK dark current G
    {0x70 , 0x18}, //BLK offset ratio RB
    {0x71 , 0x10}, //BLK dark current RB
    {0x73 , 0x03}, //BLK offset ratio R

    ///////////////////////////////////////////////////
    ///////////////////////////  DNDD /////////////////
    ///////////////////////////////////////////////////
    {0x80 , 0x07}, //[7]dn_inc_or_dec [4]zero_weight_mode [3]share [2]c_weight_adap [1]dn_lsc_mode [0]dn_b
    {0x82 , 0x08}, //DN lilat b base

    ///////////////////////////////////////////////////
    /////////////////////////// EEINTP ////////////////
    ///////////////////////////////////////////////////
    {0x8a , 0x7c},//need slim edge,can set as 3c
    {0x8c , 0x02},
    {0x8e , 0x02},
    {0x8f , 0x48}, //INTPEE edge effect
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// CC_t      ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xb0 , 0x44},
    {0xb1 , 0xfe},
    {0xb2 , 0x00},
    {0xb3 , 0xf8},
    {0xb4 , 0x48},
    {0xb5 , 0xf8},
    {0xb6 , 0x00},
    {0xb7 , 0x04},
    {0xb8 , 0x00},
        
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////  GAMMA    ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    //RGB_GAMMA

    //Gamma(1)
    
    {0xbf , 0x0e},
    {0xc0 , 0x1c},
    {0xc1 , 0x34},
    {0xc2 , 0x48},
    {0xc3 , 0x5a},
    {0xc4 , 0x6b},
    {0xc5 , 0x7b},
    {0xc6 , 0x95},
    {0xc7 , 0xab},
    {0xc8 , 0xbf},
    {0xc9 , 0xce},
    {0xca , 0xd9},
    {0xcb , 0xe4},
    {0xcc , 0xec},
    {0xcd , 0xf7},
    {0xce , 0xfd},
    {0xcf , 0xff},
    
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// YCP_t     ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xd1 , 0x38},//saturation
    {0xd2 , 0x38},//saturation
    {0xde , 0x21},//auto_gray

    ///////////////////////////////////////////////////
    /////////////////////////// ASDE //////////////////
    ///////////////////////////////////////////////////
    {0x98 , 0x30},
    {0x99 , 0xf0},
    {0x9b , 0x00},
    
    ///////////////////////////////////////////////////
    /////////////////////////// AEC ///////////////////
    ///////////////////////////////////////////////////
    {0xfe , 0x01},//page 1
    {0x10 , 0x45}, //AEC mode 1
    {0x11 , 0x32}, //[7]fix target
    {0x13 , 0x60},
    {0x17 , 0x00},
    {0x1c , 0x96},
    {0x1e , 0x11},
    {0x21 , 0xc0}, //max_post_gain
    {0x22 , 0x60}, //max_pre_gain
    {0x2d , 0x06}, //P_N_AEC_exp_level_1[12:8]
    {0x2e , 0x00}, //P_N_AEC_exp_level_1[7:0]
    {0x1e , 0x32},
    
    ///////////////////////////////////////////////////
    ///////////////////////////  AWB //////////////////
    ///////////////////////////////////////////////////
    {0x57 , 0x40}, //number limit
    {0x5d , 0x44}, //
    {0x5c , 0x35}, //show mode,close dark_mode
    {0x5e , 0x29}, //close color temp
    {0x5f , 0x50}, 
    {0x60 , 0x50},
    {0x65 , 0xc0}, 
    ///////////////////////////////////////////////////
    ///////////////////////////  ABS // ///////////////
    ///////////////////////////////////////////////////
    {0x80 , 0x82},
    {0x81 , 0x00},
    {0x83 , 0x00},//ABS Y stretch limit

    ///////////////////////////////////////////////////
    ///////////////////////////  OUT //////////////////
    ///////////////////////////////////////////////////
    {0xfe , 0x00},//page 0
    {0x46 , 0x03}, //sync mode
    {0x44 , 0xa2}, //YUV
    {0x45 , 0x0f}, //output enable
    {0xfe , 0x01},
    //{0x33 , 0x20},    
    {0x34 , 0x20},
    {0xfe , 0x00},
    {0x43 , 0x00},
    {0x42 , 0x74},
    {0x7a , 0x5f},
    {0x7b , 0x40},
    {0x7c , 0x47},
    {0x42 , 0x76},
    {0xd5 , 0x00},

    #ifdef GC2015_48MHz_MCLK
    {0xfe , 0x00},
    {0x2c , 0x01},
    {0x32 , 0x2c},//analog mode 1
    {0x33 , 0x08},//analog mode 2
    {0x34 , 0x10},//[6:4]da_rsg
    #endif
    {SENSOR_WRITE_DELAY, 200},
};

__align(4) const SENSOR_REG_T GC2015_YUV_1600X1200[] = 
{
    {0x48 , 0x50},
    {0x02 , 0x00}, //capture mode
    {0x2a , 0x0a}, //[6]even skip
    {0x59 , 0x11}, //[7:4]row sample , 0x[3:0]col sample 
    {0x5a , 0x06},
};

__align(4) const SENSOR_REG_T GC2015_YUV_1280X960[] = 
{
    {0x48 , 0x50},
    {0x02 , 0x00}, //capture mode
    {0x2a , 0x0a}, //[6]even skip
    {0x59 , 0x55}, //[7:4]row sample , 0x[3:0]col sample 
    {0x5a , 0x06},
    {0x5b , 0x01},
    {0x5c , 0x23},
    {0x5f , 0x01},
    {0x60 , 0x23},
};

__align(4) const SENSOR_REG_T GC2015_YUV_640X480[] = 
{
    {0x48 , 0x40},
    {0x02 , 0x01}, //preview mode
    {0x2a , 0xca}, //[7]col_binning , 0x[6]even skip
    {0x59 , 0x55}, //[7:4]row sample , 0x[3:0]col sample 
    {0x5a , 0x06},
    {0x5b , 0x01},
    {0x5c , 0x23},
    {0x5f , 0x01},
    {0x60 , 0x23},
};

LOCAL SENSOR_REG_TAB_INFO_T s_GC2015_resolution_Tab_YUV[]=
{   
    // COMMON INIT
    { ADDR_AND_LEN_OF_ARRAY(GC2015_YUV_COMMON), 640, 480, 48, SENSOR_IMAGE_FORMAT_YUV422}, 
    
    
    // YUV422 PREVIEW 1 
    { ADDR_AND_LEN_OF_ARRAY(GC2015_YUV_640X480), 640, 480, 48, SENSOR_IMAGE_FORMAT_YUV422},
    { ADDR_AND_LEN_OF_ARRAY(GC2015_YUV_1280X960), 1280, 960, 48, SENSOR_IMAGE_FORMAT_YUV422},
    { ADDR_AND_LEN_OF_ARRAY(GC2015_YUV_1600X1200), 1600, 1200, 48,SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL,    0, 0,   0, 0, 0},
    
    // YUV422 PREVIEW 2 
    { PNULL,    0, 0,   0, 0, 0},
    { PNULL,    0, 0,   0, 0, 0},
    { PNULL,    0, 0,   0, 0, 0},
    { PNULL,    0, 0,   0, 0, 0} 
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC2015_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    GC2015_Power_On,
    PNULL, 
    GC2015_Identify,

    PNULL,// write register
    PNULL,// read  register 
    PNULL,
    PNULL,

    // External
    PNULL,
    GC2015_set_hmirror_enable,
    GC2015_set_vmirror_enable,

    GC2015_set_brightness,
    GC2015_set_contrast,
    PNULL,
    PNULL,

    GC2015_set_preview_mode,    
    GC2015_set_image_effect,

    GC2015_before_snapshot,
    GC2015_after_snapshot,
    GC2015_set_flash_enable,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    GC2015_set_awb,
    PNULL,
    PNULL,
    GC2015_set_ev,
    GC2015_Check_Image_Format_Support,
    GC2015_Change_Image_Format,
    PNULL,
    PNULL,
    PNULL,
    GC2015_set_anti_flicker,
    GC2015_set_video_mode,
    PNULL,
    PNULL
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 PUBLIC const SENSOR_INFO_T g_GC2015_yuv_info =
{
    GC2015_I2C_ADDR_W,              // salve i2c write address
    GC2015_I2C_ADDR_R,              // salve i2c read address
    
    SENSOR_I2C_REG_8BIT | SENSOR_I2C_VAL_8BIT,      // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                    // other bit: reseved
    SENSOR_HW_SIGNAL_PCLK_P|\
    SENSOR_HW_SIGNAL_VSYNC_P|\
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
    100,                                // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code
    0x0000, 0x20,                       // supply two code to identify sensor.
    0x0001, 0x05,                       // for Example: index = 0-> Device id, index = 1 -> version id                                          
                                            
    SENSOR_AVDD_2800MV,             // voltage of avdd  
    
    1600,                           // max width of source image
    1200,                           // max height of source image
    "GC2015",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,SENSOR_IMAGE_FORMAT_MAX
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_GC2015_resolution_Tab_YUV,    // point to resolution table information structure
    &s_GC2015_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1500MV,                      // dvdd
    1,                     // skip frame num before preview 
    2,                      // skip frame num before capture    
    0,
    0,
    0,
    0,
    0,
    0,
    SENSOR_OUTPUT_MODE_CCIR601_8BIT,
    0
};

/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
 
PUBLIC uint32 GC2015_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E       dvdd_val=g_GC2015_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E       avdd_val=g_GC2015_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E       iovdd_val=g_GC2015_yuv_info.iovdd_val;  
    BOOLEAN                 power_down=g_GC2015_yuv_info.power_down_level;      
    BOOLEAN                 reset_level=g_GC2015_yuv_info.reset_pulse_level;
    uint32                  reset_width=g_GC2015_yuv_info.reset_pulse_width;        

    if(SCI_TRUE == power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
        GPIO_SetSensorPwdn(!power_down);        GPIO_SetSensorPwdn(!power_down);                /*lint !e18*//*lint !e718*//*lint !e730*/
        // Open Mclk in default frequency
        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);   
        SCI_Sleep(10);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel(!reset_level);             /*lint !e730*/
        SCI_Sleep(10);
    }
    else
    {
        GPIO_SetSensorPwdn(power_down);
        Sensor_SetMCLK(SENSOR_DISABLE_MCLK);           
        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);        
    }

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"set_GC2015_Power_On"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_530_112_2_18_0_30_49_799,(uint8*)"");
#endif
    
    return SCI_SUCCESS;
}


/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/

LOCAL uint32 GC2015_Identify(uint32 param)
{
#define GC2015_ID_H_VALUE   0x20
#define GC2015_ID_L_VALUE   0x05
        
    uint8 id_h_value = 0;
    uint8 id_l_value = 0;

    id_h_value = GC2015_ReadReg(0x0000);
    id_l_value = GC2015_ReadReg(0x0001);

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"GC2015_Identify-id_h_value %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_554_112_2_18_0_30_49_800,(uint8*)"d", id_h_value);
    //SCI_TRACE_LOW:"GC2015_Identify-id_l_value %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_555_112_2_18_0_30_49_801,(uint8*)"d", id_l_value);
#endif

    if((GC2015_ID_H_VALUE == id_h_value) && (GC2015_ID_L_VALUE == id_l_value))
    {
        SCI_TRACE_LOW("It Is GC2015 Sensor !");
        return 0;
    }
    else
    {
        return 1;
    }
    
}
/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL void GC2015_WriteReg( uint8  subaddr, uint8 data )
{
    
    #ifndef _USE_DSP_I2C_
        //uint8 cmd[2];
        //cmd[0]    =   subaddr;
        //cmd[1]    =   data;
        
        Sensor_WriteReg(subaddr, data);
    #else
        DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
    #endif
#ifdef GC2015_DEBUG
    SCI_TRACE_LOW("GC2015 WriteReg reg/value(%x,%x) ", subaddr, data);
#endif
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint8 GC2015_ReadReg( uint8  subaddr)
{
    uint8 value = 0;
    
    #ifndef _USE_DSP_I2C_
        value =Sensor_ReadReg( subaddr);
    #else
        value = (uint16)DSENSOR_IICRead((uint16)subaddr);
    #endif
#ifdef GC2015_DEBUG
    SCI_TRACE_LOW("GC2015 ReadReg reg/value(%x,%x) " ,subaddr,value);
#endif
    return value;
}
/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 GC2015_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        GC2015_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
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
LOCAL uint32 GC2015_set_hmirror_enable(uint32 enable)
{
    uint8 value;
    value = Sensor_ReadReg(0x29);

    if(enable)
    {
        mirror_setting = mirror_setting | 0x1;
    }
    else
    {
        mirror_setting = mirror_setting & 0x0E;
    }
    
    value = ((value & 0xfc) |(mirror_setting&0x3));
    Sensor_WriteReg(0x29, value);
    
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 GC2015_set_vmirror_enable(uint32 enable)
{
    uint8 value;
    value = Sensor_ReadReg(0x29);

    if(enable)
    {
        mirror_setting = mirror_setting | 0x2;
    }
    else
    {
        mirror_setting = mirror_setting & 0x0D;
    }

    value = ((value & 0xfc) |(mirror_setting&0x3));
    Sensor_WriteReg(0x29, value);

    return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T GC2015_brightness_tab[][2]=
{
    {{0xd5 , 0xa0},{0xff , 0xff}},
    {{0xd5 , 0xc0},{0xff , 0xff}},
    {{0xd5 , 0xe0},{0xff , 0xff}},
    {{0xd5 , 0x00},{0xff , 0xff}},
    {{0xd5 , 0x20},{0xff , 0xff}},
    {{0xd5 , 0x40},{0xff , 0xff}},
    {{0xd5 , 0x60},{0xff , 0xff}}
};

LOCAL uint32 GC2015_set_brightness(uint32 level)
{

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2015_brightness_tab[level];


    if(PNULL == sensor_reg_ptr)
    {
        return SCI_ERROR;
    }

    GC2015_Write_Group_Regs(sensor_reg_ptr);
    
#ifdef GC2015_DEBUG
        //SCI_TRACE_LOW:"set_GC2015_brightness: level = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_704_112_2_18_0_30_49_805,(uint8*)"d", level);
#endif
        
    return 0;

}
/******************************************************************************/
// Description: set contrast
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
__align(4) const SENSOR_REG_T GC2015_contrast_tab[][2]=
{
    {{0xd3,0x34}, {0xff,0xff}},
    {{0xd3,0x38}, {0xff,0xff}},
    {{0xd3,0x3c}, {0xff,0xff}},
    {{0xd3,0x40}, {0xff,0xff}},
    {{0xd3,0x44}, {0xff,0xff}},
    {{0xd3,0x48}, {0xff,0xff}},
    {{0xd3,0x4c}, {0xff,0xff}}                   
};

LOCAL uint32 GC2015_set_contrast(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2015_contrast_tab[level];


    if(PNULL == sensor_reg_ptr)
    {
        return SCI_ERROR;
    }
    
    GC2015_Write_Group_Regs(sensor_reg_ptr);
#ifdef GC2015_DEBUG
    SCI_TRACE_LOW("set_GC2015_contrast: level = %d", level);
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
LOCAL uint32 GC2015_set_preview_mode(uint32 preview_mode)
{
    GC2015_set_gamma(1);
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            GC2015_WriteReg(0xfe , 0x01);
            GC2015_WriteReg(0x33 , 0x00);
            GC2015_WriteReg(0xfe , 0x00);
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            GC2015_WriteReg(0xfe , 0x01);
            GC2015_WriteReg(0x33 , 0x60);
            GC2015_WriteReg(0xfe , 0x00);
            break;
        }
        default:
        {
            break;
        }
    } 

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"set_GC2015_preview_mode: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_769_112_2_18_0_30_49_807,(uint8*)"d", preview_mode);
#endif

    SCI_Sleep(100);

    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T GC2015_image_effect_tab[][4]= 
{
    //Normal
    {{0x43 , 0x00},{0xff , 0xff},{0xff , 0xff},{0xff , 0xff}},
    //BLACK&WHITE
    {{0x43 , 0x02},{0xda , 0x00},{0xdb , 0x00},{0xff , 0xff}},
    //RED
    {{0x43 , 0x02},{0xda , 0x10},{0xdb , 0x50},{0xff , 0xff}},
    //GREEN
    {{0x43 , 0x02},{0xda , 0xc0},{0xdb , 0xc0},{0xff , 0xff}},
    //BLUE
    {{0x43 , 0x02},{0xda , 0x50},{0xdb , 0xe0},{0xff , 0xff}},
    //YELLOW
    {{0x43 , 0x02},{0xda , 0x80},{0xdb , 0x20},{0xff , 0xff}},
    //NEGATIVE
    {{0x43 , 0x01},{0xff , 0xff},{0xff , 0xff},{0xff , 0xff}},
    //SEPIA
    {{0x43 , 0x02},{0xda , 0xd0},{0xdb , 0x28},{0xff , 0xff}}
};

LOCAL uint32 GC2015_set_image_effect(uint32 effect_type)
{
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2015_image_effect_tab[effect_type];

    if(PNULL == sensor_reg_ptr)
    {
        return SCI_ERROR;
    }
    
    GC2015_Write_Group_Regs(sensor_reg_ptr);

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"set_GC2015_image_effect: effect_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_811_112_2_18_0_30_49_808,(uint8*)"d", effect_type);
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
LOCAL uint32 GC2015_before_snapshot(uint32 sensor_snapshot_mode)
{
    GC2015_set_ae_enable(0x00);
//    Sensor_SetMode(sensor_snapshot_mode);
    
    switch(sensor_snapshot_mode)
    {   
        case SENSOR_MODE_PREVIEW_ONE:   //VGA
            {
#ifdef GC2015_DEBUG
            //SCI_TRACE_LOW:"Capture VGA Size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_832_112_2_18_0_30_49_809,(uint8*)"");
#endif
            break;
            }
        case SENSOR_MODE_SNAPSHOT_ONE_FIRST:     // 1.3 M
        case SENSOR_MODE_SNAPSHOT_ONE_SECOND:   // 2 M.
            {
            Sensor_SetMode(sensor_snapshot_mode);
                
#ifdef GC2015_DEBUG
            //SCI_TRACE_LOW:"Capture 1.3M&2M Size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_840_112_2_18_0_30_49_810,(uint8*)"");
#endif
            GC2015_set_shutter();
            GC2015_WriteReg(0x6e , 0x19); //BLK offset submode,offset R
            GC2015_WriteReg(0x6f , 0x10);
            GC2015_WriteReg(0x70 , 0x19);
            GC2015_WriteReg(0x71 , 0x10);
            SCI_Sleep(100);
            break;
            }
        default:
            break;
    }
    

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"SENSOR_GC2015: Before Snapshot mode = sensor_snapshot_mode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_855_112_2_18_0_30_49_811,(uint8*)"");
#endif


    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/
LOCAL uint32 GC2015_after_snapshot(uint32 para)
{   
    Sensor_SetMode(para);

//    GC2015_Change_Image_Format(SENSOR_IMAGE_FORMAT_YUV422);

    GC2015_set_ae_enable(0x01);
    GC2015_WriteReg(0xfa , 0x00); //cancel clk div
    GC2015_WriteReg(0x6e , 0x18); //BLK offset submode,offset R
    GC2015_WriteReg(0x6f , 0x10);
    GC2015_WriteReg(0x70 , 0x18);
    GC2015_WriteReg(0x71 , 0x10);   
//    SCI_Sleep(100);

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"SENSOR_GC2015: After Snapshot"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_881_112_2_18_0_30_50_812,(uint8*)"");
#endif

    return 0;
}

/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T GC2015_awb_tab[][5]=
{
    //Auto
    {
        {0x7a , 0x56},
        {0x7b , 0x40},
        {0x7c , 0x4a},
        {0x42 , 0x76},
        {0xff , 0xff}
    },

    //Office
    {
        {0x42 , 0x74},
        {0x7a , 0x48},
        {0x7b , 0x40},
        {0x7c , 0x5c},
        {0xff , 0xff}
    },
    
    //U30  //not use
    {
        {0x42 , 0x74},
        {0x7a , 0x40},
        {0x7b , 0x54},
        {0x7c , 0x70},    
        {0xff , 0xff}
    },  
    
    //CWF  //not use
    {
        {0x42 , 0x74},
        {0x7a , 0x40},
        {0x7b , 0x54},
        {0x7c , 0x70},          
        {0xff , 0xff}
    }, 
    
    //HOME
    {
        {0x42 , 0x74},
        {0x7a , 0x40},
        {0x7b , 0x42},
        {0x7c , 0x50},    
        {0xff, 0xff}
    },  
    
    //SUN:
    {
        {0x42 , 0x74},
        {0x7a , 0x50},
        {0x7b , 0x45},
        {0x7c , 0x40},        
        {0xff , 0xff}
    },
    
    //CLOUD:
    {
        {0x42 , 0x74},
        {0x7a , 0x5a},
        {0x7b , 0x42},
        {0x7c , 0x40},    
        {0xff , 0xff}
    }
};

LOCAL uint32 GC2015_set_awb(uint32 mode)
{
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2015_awb_tab[mode];

    if(PNULL == sensor_reg_ptr||mode >= 7)
    {
        return SCI_ERROR;
    }
    
    GC2015_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_Sleep(100); 

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"set_GC2015_awb_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_970_112_2_18_0_30_50_813,(uint8*)"d", mode);
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
__align(4) const SENSOR_REG_T GC2015_ev_tab[][4]=
{   
    {{0xfe , 0x01},{0x13 , 0x70},{0xfe , 0x00},{0xff , 0xff}},
    {{0xfe , 0x01},{0x13 , 0x80},{0xfe , 0x00},{0xff , 0xff}},
    {{0xfe , 0x01},{0x13 , 0x90},{0xfe , 0x00},{0xff , 0xff}},
    {{0xfe , 0x01},{0x13 , 0xa0},{0xfe , 0x00},{0xff , 0xff}},
    {{0xfe , 0x01},{0x13 , 0xb0},{0xfe , 0x00},{0xff , 0xff}},
    {{0xfe , 0x01},{0x13 , 0xc0},{0xfe , 0x00},{0xff , 0xff}},
    {{0xfe , 0x01},{0x13 , 0xd0},{0xfe , 0x00},{0xff , 0xff}}
};

LOCAL uint32 GC2015_set_ev(uint32 level)
{
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2015_ev_tab[level];


    if(PNULL == sensor_reg_ptr||level >= 7)
    {
        return SCI_ERROR;
    }

    GC2015_Write_Group_Regs(sensor_reg_ptr );

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"set_GC2015_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_1002_112_2_18_0_30_50_814,(uint8*)"d", level);
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
LOCAL uint32 GC2015_set_gamma(uint32 level)
{
    switch(level)
    {
        case 1:                                             //smallest gamma curve
            Sensor_WriteReg( 0xbF, 0x0B ); 
            Sensor_WriteReg( 0xc0, 0x16 ); 
            Sensor_WriteReg( 0xc1, 0x29 ); 
            Sensor_WriteReg( 0xc2, 0x3C ); 
            Sensor_WriteReg( 0xc3, 0x4F ); 
            Sensor_WriteReg( 0xc4, 0x5F ); 
            Sensor_WriteReg( 0xc5, 0x6F ); 
            Sensor_WriteReg( 0xc6, 0x8A ); 
            Sensor_WriteReg( 0xc7, 0x9F ); 
            Sensor_WriteReg( 0xc8, 0xB4 ); 
            Sensor_WriteReg( 0xc9, 0xC6 ); 
            Sensor_WriteReg( 0xcA, 0xD3 ); 
            Sensor_WriteReg( 0xcB, 0xDD );  
            Sensor_WriteReg( 0xcC, 0xE5 );  
            Sensor_WriteReg( 0xcD, 0xF1 ); 
            Sensor_WriteReg( 0xcE, 0xFA ); 
            Sensor_WriteReg( 0xcF, 0xFF );  
            break;
        case 2:         
            Sensor_WriteReg( 0xbF, 0x0E ); 
            Sensor_WriteReg( 0xc0, 0x1C ); 
            Sensor_WriteReg( 0xc1, 0x34 ); 
            Sensor_WriteReg( 0xc2, 0x48 ); 
            Sensor_WriteReg( 0xc3, 0x5A ); 
            Sensor_WriteReg( 0xc4, 0x6B ); 
            Sensor_WriteReg( 0xc5, 0x7B ); 
            Sensor_WriteReg( 0xc6, 0x95 ); 
            Sensor_WriteReg( 0xc7, 0xAB ); 
            Sensor_WriteReg( 0xc8, 0xBF );
            Sensor_WriteReg( 0xc9, 0xCE ); 
            Sensor_WriteReg( 0xcA, 0xD9 ); 
            Sensor_WriteReg( 0xcB, 0xE4 );  
            Sensor_WriteReg( 0xcC, 0xEC ); 
            Sensor_WriteReg( 0xcD, 0xF7 ); 
            Sensor_WriteReg( 0xcE, 0xFD ); 
            Sensor_WriteReg( 0xcF, 0xFF ); 
        break;
        case 3:
            Sensor_WriteReg( 0xbF, 0x10 ); 
            Sensor_WriteReg( 0xc0, 0x20 ); 
            Sensor_WriteReg( 0xc1, 0x38 ); 
            Sensor_WriteReg( 0xc2, 0x4E ); 
            Sensor_WriteReg( 0xc3, 0x63 ); 
            Sensor_WriteReg( 0xc4, 0x76 ); 
            Sensor_WriteReg( 0xc5, 0x87 ); 
            Sensor_WriteReg( 0xc6, 0xA2 ); 
            Sensor_WriteReg( 0xc7, 0xB8 ); 
            Sensor_WriteReg( 0xc8, 0xCA ); 
            Sensor_WriteReg( 0xc9, 0xD8 ); 
            Sensor_WriteReg( 0xcA, 0xE3 ); 
            Sensor_WriteReg( 0xcB, 0xEB ); 
            Sensor_WriteReg( 0xcC, 0xF0 ); 
            Sensor_WriteReg( 0xcD, 0xF8 ); 
            Sensor_WriteReg( 0xcE, 0xFD ); 
            Sensor_WriteReg( 0xcF, 0xFF ); 

            break;
        case 4:
            Sensor_WriteReg( 0xbF, 0x14 ); 
            Sensor_WriteReg( 0xc0, 0x28 ); 
            Sensor_WriteReg( 0xc1, 0x44 ); 
            Sensor_WriteReg( 0xc2, 0x5D ); 
            Sensor_WriteReg( 0xc3, 0x72 ); 
            Sensor_WriteReg( 0xc4, 0x86 ); 
            Sensor_WriteReg( 0xc5, 0x95 ); 
            Sensor_WriteReg( 0xc6, 0xB1 ); 
            Sensor_WriteReg( 0xc7, 0xC6 ); 
            Sensor_WriteReg( 0xc8, 0xD5 ); 
            Sensor_WriteReg( 0xc9, 0xE1 ); 
            Sensor_WriteReg( 0xcA, 0xEA ); 
            Sensor_WriteReg( 0xcB, 0xF1 ); 
            Sensor_WriteReg( 0xcC, 0xF5 ); 
            Sensor_WriteReg( 0xcD, 0xFB ); 
            Sensor_WriteReg( 0xcE, 0xFE ); 
            Sensor_WriteReg( 0xcF, 0xFF );
        break;
        case 5:                             // largest gamma curve
            Sensor_WriteReg( 0xbF, 0x15 ); 
            Sensor_WriteReg( 0xc0, 0x2A ); 
            Sensor_WriteReg( 0xc1, 0x4A ); 
            Sensor_WriteReg( 0xc2, 0x67 ); 
            Sensor_WriteReg( 0xc3, 0x79 ); 
            Sensor_WriteReg( 0xc4, 0x8C ); 
            Sensor_WriteReg( 0xc5, 0x9A ); 
            Sensor_WriteReg( 0xc6, 0xB3 ); 
            Sensor_WriteReg( 0xc7, 0xC5 ); 
            Sensor_WriteReg( 0xc8, 0xD5 ); 
            Sensor_WriteReg( 0xc9, 0xDF ); 
            Sensor_WriteReg( 0xcA, 0xE8 ); 
            Sensor_WriteReg( 0xcB, 0xEE ); 
            Sensor_WriteReg( 0xcC, 0xF3 ); 
            Sensor_WriteReg( 0xcD, 0xFA ); 
            Sensor_WriteReg( 0xcE, 0xFD ); 
            Sensor_WriteReg( 0xcF, 0xFF );
            break;
        default:
        break;
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
LOCAL uint32 GC2015_Check_Image_Format_Support(uint32 param)
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
        SCI_PASSERT(0, ("GC2015 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param));/*assert verified*/
        break;
    }

    return ret_val;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 GC2015_Change_Image_Format(uint32 param)
{
    SENSOR_REG_TAB_INFO_T st_yuv422_reg_table_info = { ADDR_AND_LEN_OF_ARRAY(GC2015_YUV_COMMON),0,0,0,0};
    uint32 ret_val = SCI_ERROR;

    switch(param)
    {
        case SENSOR_IMAGE_FORMAT_YUV422:
            ret_val = Sensor_SendRegTabToSensor(&st_yuv422_reg_table_info);
            break;

        case SENSOR_IMAGE_FORMAT_JPEG:
            ret_val = SCI_ERROR;
            break;

        default:
                   break;
    }

    return ret_val;
}
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
LOCAL uint32 GC2015_set_anti_flicker(uint32 mode)
{
    switch(mode)
    {
        case DCAMERA_FLICKER_50HZ:
            GC2015_WriteReg(0x05 , 0x01);//HB
            GC2015_WriteReg(0x06 , 0x0f);
            GC2015_WriteReg(0x07 , 0x00);//VB
            GC2015_WriteReg(0x08 , 0x30);

            GC2015_WriteReg(0xfe , 0x01);

            GC2015_WriteReg(0x29 , 0x00);//Anti Step
            GC2015_WriteReg(0x2a , 0x9e);

            GC2015_WriteReg(0x2b , 0x04);//Level0 12.5fps
            GC2015_WriteReg(0x2c , 0xf0);
            GC2015_WriteReg(0x2d , 0x06);//Level1 10.00fps
            GC2015_WriteReg(0x2e , 0x2c);
            GC2015_WriteReg(0x2f , 0x07);//Level2 8.00fps
            GC2015_WriteReg(0x30 , 0x68);
            GC2015_WriteReg(0x31 , 0x0c);//Level3 5.00fps
            GC2015_WriteReg(0x32 , 0x58);

            GC2015_WriteReg(0xfe , 0x00);
        break;

        case DCAMERA_FLICKER_60HZ:
            GC2015_WriteReg(0x05 , 0x01);//HB
            GC2015_WriteReg(0x06 , 0x0d);
            GC2015_WriteReg(0x07 , 0x00);//VB
            GC2015_WriteReg(0x08 , 0x68);

            GC2015_WriteReg(0xfe , 0x01);

            GC2015_WriteReg(0x29 , 0x00);//Anti Step
            GC2015_WriteReg(0x2a , 0x84);

            GC2015_WriteReg(0x2b , 0x05);//Level0 12.0fps
            GC2015_WriteReg(0x2c , 0x28);
            GC2015_WriteReg(0x2d , 0x06);//Level1 10.00fps
            GC2015_WriteReg(0x2e , 0x30);
            GC2015_WriteReg(0x2f , 0x07);//Level2 8.00fps
            GC2015_WriteReg(0x30 , 0xbc);
            GC2015_WriteReg(0x31 , 0x0c);//Level3 5.00fps
            GC2015_WriteReg(0x32 , 0x60); 
            
            GC2015_WriteReg(0xfe , 0x00);
        break;

        default:
        return 0;
    }

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"set_GC2015_anti_flicker-mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_1230_112_2_18_0_30_50_815,(uint8*)"d",mode);
#endif

    SCI_Sleep(200); 

    return 0;
}

/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T GC2015_video_mode_nor_tab[][4]=
{
    // normal mode 
    {{0xfe , 0x01},{0x33 , 0x00},{0xfe , 0x00},{0xff , 0xff}},    
    //vodeo mode
    {{0xfe , 0x01},{0x33 , 0x00},{0xfe , 0x00},{0xff , 0xff}},
    // UPCC  mode
    {{0xfe , 0x01},{0x33 , 0x00},{0xfe , 0x00},{0xff , 0xff}}
};    

LOCAL uint32 GC2015_set_video_mode(uint32 mode)
{
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC2015_video_mode_nor_tab[mode];

    if(PNULL == sensor_reg_ptr||mode > DCAMERA_MODE_MAX)
    {
        return SCI_ERROR;
    }


    GC2015_Write_Group_Regs(sensor_reg_ptr );

#ifdef GC2015_DEBUG
    //SCI_TRACE_LOW:"set_GC2015_video_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_1263_112_2_18_0_30_50_816,(uint8*)"d",mode);
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
LOCAL void GC2015_set_shutter(void)
{
    uint32 hb_ori, hb_total,shutter;
    uint32 temp_reg, pv_shutter, pv_extra_shutter;      // banding_step,
    uint16 temp_reg1, temp_reg2;
    
    /* Backup the preview mode last shutter & sensor gain. */
    temp_reg1 = GC2015_ReadReg(0x03);
    temp_reg2 = GC2015_ReadReg(0x04);
    pv_shutter = (temp_reg1 << 8) | (temp_reg2 & 0xFF);
    
    /* Backup the preview mode last shutter & sensor gain. */
    pv_extra_shutter = 0;
    
    shutter = pv_shutter + pv_extra_shutter;
    /*Set HB start*/
    
    /*The HB must < 0xFFF*/
    //hb_ori = (GC2015_ReadReg(0x12)<<8 )|( GC2015_ReadReg(0x13));
    hb_ori = 298;
    hb_total = hb_ori + hb_add;

#ifdef GC2015_48MHz_MCLK
    if(hb_total > 0xfff)
    {
        GC2015_WriteReg(0x12 , 0x0f);
        GC2015_WriteReg(0x13 , 0xff); 
        temp_reg = shutter * (1702 + hb_ori ) *10  / ( 18 * 4095);
        
    }
    else
    {
        GC2015_WriteReg(0x12 , (hb_total>>8)&0xff);
        GC2015_WriteReg(0x13 , hb_total&0xff); 
        temp_reg = shutter * (1702 + hb_ori ) * 10  / ( 18 * (1702 + hb_ori + hb_add));
    }
    //temp_reg = shutter * 10 / 11; 
#else
    if(hb_total > 0xfff)
    {
        GC2015_WriteReg(0x12 , 0x0f);
        GC2015_WriteReg(0x13 , 0xff); 
        temp_reg = shutter * (1702 + hb_ori ) *10  / ( 18 * 4095);
        
    }
    else
    {
        GC2015_WriteReg(0x12 , (hb_total>>8)&0xff);
        GC2015_WriteReg(0x13 , hb_total&0xff); 
        temp_reg = shutter * (1702 + hb_ori ) * 10  / ( 18 * (1702 + hb_ori + hb_add));
    }
    //temp_reg = shutter * 10 / 11; 
#endif
    /*Set HB end*/

    /*Set Shutter start*/
    if(temp_reg < 1) temp_reg = 1;
    
    GC2015_WriteReg(0x03 , (temp_reg>>8)&0xff);        
    GC2015_WriteReg(0x04 , temp_reg&0xff); 
    /*Set Shutter end*/
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 GC2015_set_ae_enable(uint8 enable)
{
    uint8 ae_tamp;
    
    ae_tamp = GC2015_ReadReg(0x4f);
    if(0x01 == enable)
    {
        GC2015_WriteReg(0x4f,ae_tamp|0x01);
    }
    else
    {
        GC2015_WriteReg(0x4f,ae_tamp&0xfe);
    }
#ifdef GC2015_DEBUG 
    //SCI_TRACE_LOW:"SENSOR: set_ae_enable: ae_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_1356_112_2_18_0_30_50_817,(uint8*)"d", enable);
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
LOCAL uint32 GC2015_set_awb_enable(uint8 enable)
{
    uint8 awb_tamp;

    awb_tamp = GC2015_ReadReg(0x42);
    if(0x01 == enable)
    {
       GC2015_WriteReg(0x42,awb_tamp|0x02);
    }
    else
    {
       GC2015_WriteReg(0x42,awb_tamp&0xfd);
    }
#ifdef GC2015_DEBUG
   //SCI_TRACE_LOW:"SENSOR: set_awb_enable: awb_enable = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC2015_1380_112_2_18_0_30_50_818,(uint8*)"d", enable);
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
LOCAL uint32 GC2015_set_flash_enable(uint32 enable)
{
#ifdef DC_FLASH_SUPPORT
    GPIO_SetFlashLight(enable);
#endif
    return 0;
}

