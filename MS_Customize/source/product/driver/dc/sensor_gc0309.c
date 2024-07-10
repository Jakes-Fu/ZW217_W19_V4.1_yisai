/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_GC0309.c                                           *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor GC0309. 
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
#define GC0309_I2C_ADDR_W       0x42//0x60
#define GC0309_I2C_ADDR_R       0x43//0x61

#define SENSOR_GAIN_SCALE       16

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_gc0309_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 GC0309_Identify(uint32 param);
/*
LOCAL uint32 GC0309_BeforeSnapshot(uint32 param);
LOCAL uint32 GC0309_After_Snapshot(uint32 param);
*/
LOCAL uint32 set_brightness(uint32 level);
LOCAL uint32 set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
LOCAL uint32 set_image_effect(uint32 effect_type);

LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);
LOCAL uint32 GC0309_set_work_mode(uint32 mode);

LOCAL uint32 set_gc0309_ev(uint32 level);
LOCAL uint32 set_gc0309_awb(uint32 mode);
LOCAL uint32 set_gc0309_anti_flicker(uint32 mode);
LOCAL uint32 set_gc0309_video_mode(uint32 mode);
LOCAL uint32 GC0309_set_flash_enable(uint32 enable);

LOCAL BOOLEAN gc_enter_effect = SCI_FALSE;
LOCAL uint32  mirror_setting = 0;     //mirror flip setting 


/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/

__align(4) const SENSOR_REG_T gc0309_YUV_640X480[]=
{
    {0xfe,0x80},
        
    {0xfe,0x00}, 
    
    {0x1a,0x16},    
    {0xd2,0x10},
    {0x22,0x55}, 

    {0x5a,0x56}, 
    {0x5b,0x40},
    {0x5c,0x4a},            

    {0x22,0x57}, 
        
    {0x01,0xfa}, 
    {0x02,0x70}, 
    {0x0f,0x01}, 

    {0xe2,0x00}, 
    {0xe3,0x64}, 

    {0x03,0x01}, 
    {0x04,0x2c}, 

    
    {0x01,0x6a}, 
    {0x02,0x70}, 
    {0x0f,0x00},

    {0xe2,0x00}, 
    {0xe3,0x96}, 
        
    {0xe4,0x02}, 
    {0xe5,0x58}, 
    {0xe6,0x04}, 
    {0xe7,0xb0}, 
    {0xe8,0x07}, 
    {0xe9,0x08}, 
    {0xea,0x0b}, 
    {0xeb,0xb8}, 
    

    {0x05,0x00},
    {0x06,0x00},
    {0x07,0x00}, 
    {0x08,0x00}, 
    {0x09,0x01}, 
    {0x0a,0xe8}, 
    {0x0b,0x02}, 
    {0x0c,0x88}, 
    {0x0d,0x02}, 
    {0x0e,0x02}, 
    {0x10,0x22}, 
    {0x11,0x0d}, 
    {0x12,0x2a}, 
    {0x13,0x00}, 
    {0x14,0x10},    
    {0x15,0x0a}, 
    {0x16,0x05}, 
    {0x17,0x01}, 

    {0x1b,0x03}, 
    {0x1c,0xc1}, 
    {0x1d,0x08}, 
    {0x1e,0x20},
    {0x1f,0x16}, 

    {0x20,0xff}, 
    {0x21,0xf8}, 
    {0x24,0xa2}, 
    {0x25,0x0f},
    //output sync_mode
    {0x26,0x03}, 
    {0x2f,0x01}, 
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// grab_t ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x30,0xf7}, 
    {0x31,0x40},
    {0x32,0x00}, 
    {0x39,0x04}, 
    {0x3a,0x20}, 
    {0x3b,0x20}, 
    {0x3c,0x02}, 
    {0x3d,0x02}, 
    {0x3e,0x02},
    {0x3f,0x02}, 
    
    //gain
    {0x50,0x24}, 
    
    {0x53,0x82}, 
    {0x54,0x80}, 
    {0x55,0x80}, 
    {0x56,0x82}, 
    
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// LSC_t  ////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x8b,0x20}, 
    {0x8c,0x20}, 
    {0x8d,0x20}, 
    {0x8e,0x10}, 
    {0x8f,0x10}, 
    {0x90,0x10}, 
    {0x91,0x3c}, 
    {0x92,0x50}, 
    {0x5d,0x12}, 
    {0x5e,0x1a}, 
    {0x5f,0x24}, 
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// DNDD_t  ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x60,0x07}, 
    {0x61,0x0e}, 
    {0x62,0x0c}, 
    {0x64,0x03}, 
    {0x66,0xe8}, 
    {0x67,0x86}, 
    {0x68,0xa2}, 
    
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// asde_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x69,0x20}, 
    {0x6a,0x0f}, 
    {0x6b,0x00}, 
    {0x6c,0x53}, 
    {0x6d,0x83}, 
    {0x6e,0xac}, 
    {0x6f,0xac}, 
    {0x70,0x15}, 
    {0x71,0x33}, 
    /////////////////////////////////////////////////////////////////////
    /////////////////////////// eeintp_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x72,0xdc},  
    {0x73,0x80},  
    //for high resolution in light scene
    {0x74,0x02}, 
    {0x75,0x3f}, 
    {0x76,0x02}, 
    {0x77,0x54}, 
    {0x78,0x88}, 
    {0x79,0x81}, 
    {0x7a,0x81}, 
    {0x7b,0x22}, 
    {0x7c,0xff},
    
    
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////CC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0x93,0x45}, 
    {0x94,0x00}, 
    {0x95,0x00}, 
    {0x96,0x00}, 
    {0x97,0x45}, 
    {0x98,0xf0}, 
    {0x9c,0x00}, 
    {0x9d,0x03}, 
    {0x9e,0x00}, 

    
    
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////YCP_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xb1,0x40}, 
    {0xb2,0x40}, 
    {0xb8,0x20}, 
    {0xbe,0x36}, 
    {0xbf,0x00}, 
    /////////////////////////////////////////////////////////////////////
    ///////////////////////////AEC_t///////////////////////////////
    /////////////////////////////////////////////////////////////////////
    {0xd0,0xc9},  
    {0xd1,0x10},  
//  {0xd2,0x90},  
    {0xd3,0x80},  
    {0xd5,0xf2}, 
    {0xd6,0x16},  
    {0xdb,0x92}, 
    {0xdc,0xa5},  
    {0xdf,0x23},   
    {0xd9,0x00},  
    {0xda,0x00},  
    {0xe0,0x09},

    {0xec,0x20},  
    {0xed,0x04},  
    {0xee,0xa0},  
    {0xef,0x40},  
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////GAMMA//////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    //Y_gamma
    {0xc0,0x00},
    {0xc1,0x0B},
    {0xc2,0x15},
    {0xc3,0x27},
    {0xc4,0x39},
    {0xc5,0x49},
    {0xc6,0x5A},
    {0xc7,0x6A},
    {0xc8,0x89},
    {0xc9,0xA8},
    {0xca,0xC6},
    {0xcb,0xE3},
    {0xcc,0xFF},

    /////////////////////////////////////////////////////////////////
    /////////////////////////// ABS_t ///////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf0,0x02},
    {0xf1,0x01},
    {0xf2,0x00}, 
    {0xf3,0x30}, 
    
    /////////////////////////////////////////////////////////////////
    /////////////////////////// Measure Window ///////////////////////
    /////////////////////////////////////////////////////////////////
    {0xf7,0x04}, 
    {0xf8,0x02}, 
    {0xf9,0x9f},
    {0xfa,0x78},

    //---------------------------------------------------------------
    {0xfe,0x01},

    /////////////////////////////////////////////////////////////////
    ///////////////////////////AWB_p/////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {0x00,0xf5}, 
    //{0x01,0x0a},  
    {0x02,0x1a}, 
    {0x0a,0xa0}, 
    {0x0b,0x60}, 
    {0x0c,0x08},
    {0x0e,0x4c}, 
    {0x0f,0x39}, 
    {0x11,0x3f}, 
    {0x12,0x72}, 
    {0x13,0x13}, 
    {0x14,0x42},  
    {0x15,0x43}, 
    {0x16,0xc2}, 
    {0x17,0xa8}, 
    {0x18,0x18},  
    {0x19,0x40},  
    {0x1a,0xd0}, 
    {0x1b,0xf5},  

    {0x70,0x40}, 
    {0x71,0x58},  
    {0x72,0x30},  
    {0x73,0x48},  
    {0x74,0x20},  
    {0x75,0x60},  
    
    {0xfe,0x00},

    {0xd2,0x90},  // Open AEC at last


    /////////////////////////////////////////////////////////////////
    ///////////////////////////For AE Update//////////////////////////
    /////////////////////////////////////////////////////////////////
    //* 2010/05/11 Daemon update
    {0x8b,0x22},   // lsc r 
    {0x71,0x43},   // auto sat limit
    
    //cc
    {0x93,0x48}, 
    {0x94,0x00}, 
    {0x95,0x05}, 
    {0x96,0xe8}, 
    {0x97,0x40}, 
    {0x98,0xf8}, 
    {0x9c,0x00}, 
    {0x9d,0x00}, 
    {0x9e,0x00}, 
    
    {0xd0,0xcb},  // aec before gamma
    {0xd3,0x50},  // ae target

    //* 2010/07/21 Mormo update
    {0x31,0x60}, 
    
    {0x1c,0x49}, 
    {0x1d,0x98}, 
    {0x10,0x26}, 
    {0x1a,0x26},  

    
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////GAMMA//////////////////////////////////
    ///////////////////////////////////////////////////////////////////

    //-----------GAMMA Select(5)---------------//
    {0x9F , 0x15},
    {0xA0 , 0x2A},
    {0xA1 , 0x4A},
    {0xA2 , 0x67},
    {0xA3 , 0x79},
    {0xA4 , 0x8C},
    {0xA5 , 0x9A},
    {0xA6 , 0xB3},
    {0xA7 , 0xC5},
    {0xA8 , 0xD5},
    {0xA9 , 0xDF},
    {0xAA , 0xE8},
    {0xAB , 0xEE},
    {0xAC , 0xF3},
    {0xAD , 0xFA},
    {0xAE , 0xFD},
    {0xAF , 0xFF}, 

    /*GC0309_GAMMA_Select,
    1:                                             //smallest gamma curve
    {0x9F , 0x0B},
    {0xA0 , 0x16},
    {0xA1 , 0x29},
    {0xA2 , 0x3C},
    {0xA3 , 0x4F},
    {0xA4 , 0x5F},
    {0xA5 , 0x6F},
    {0xA6 , 0x8A},
    {0xA7 , 0x9F},
    {0xA8 , 0xB4}, 
    {0xA9 , 0xC6},
    {0xAA , 0xD3},
    {0xAB , 0xDD},
    {0xAC , 0xE5},
    {0xAD , 0xF1},
    {0xAE , 0xFA},
    {0xAF , 0xFF},  

    2:          
    {0x9F , 0x0E},
    {0xA0 , 0x1C},
    {0xA1 , 0x34},
    {0xA2 , 0x48},
    {0xA3 , 0x5A},
    {0xA4 , 0x6B},
    {0xA5 , 0x7B},
    {0xA6 , 0x95},
    {0xA7 , 0xAB},
    {0xA8 , 0xBF},
    {0xA9 , 0xCE},
    {0xAA , 0xD9},
    {0xAB , 0xE4},
    {0xAC , 0xEC},
    {0xAD , 0xF7},
    {0xAE , 0xFD},
    {0xAF , 0xFF},

    3:
    {0x9F , 0x10},
    {0xA0 , 0x20},
    {0xA1 , 0x38},
    {0xA2 , 0x4E},
    {0xA3 , 0x63},
    {0xA4 , 0x76},
    {0xA5 , 0x87},
    {0xA6 , 0xA2},
    {0xA7 , 0xB8},
    {0xA8 , 0xCA},
    {0xA9 , 0xD8},
    {0xAA , 0xE3},
    {0xAB , 0xEB},
    {0xAC , 0xF0},
    {0xAD , 0xF8},
    {0xAE , 0xFD},
    {0xAF , 0xFF},

    4:
    {0x9F , 0x14},
    {0xA0 , 0x28},
    {0xA1 , 0x44},
    {0xA2 , 0x5D},
    {0xA3 , 0x72},
    {0xA4 , 0x86},
    {0xA5 , 0x95},
    {0xA6 , 0xB1},
    {0xA7 , 0xC6},
    {0xA8 , 0xD5},
    {0xA9 , 0xE1},
    {0xAA , 0xEA},
    {0xAB , 0xF1},
    {0xAC , 0xF5},
    {0xAD , 0xFB},
    {0xAE , 0xFE},
    {0xAF , 0xFF},

    5:                              //largest gamma curve
    {0x9F , 0x15},
    {0xA0 , 0x2A},
    {0xA1 , 0x4A},
    {0xA2 , 0x67},
    {0xA3 , 0x79},
    {0xA4 , 0x8C},
    {0xA5 , 0x9A},
    {0xA6 , 0xB3},
    {0xA7 , 0xC5},
    {0xA8 , 0xD5},
    {0xA9 , 0xDF},
    {0xAA , 0xE8},
    {0xAB , 0xEE},
    {0xAC , 0xF3},
    {0xAD , 0xFA},
    {0xAE , 0xFD},
    {0xAF , 0xFF}, */
    //-----------GAMMA Select End--------------//




    //-------------H_V_Switch(4)---------------//
    {0x14 , 0x11},

    /*GC0309_H_V_Switch,

    1:  // normal
    {0x14 , 0x10},

    2:  // IMAGE_H_MIRROR
    {0x14 , 0x11},

    3:  // IMAGE_V_MIRROR
    {0x14 , 0x12},

    4:  // IMAGE_HV_MIRROR
    {0x14 , 0x13},
    */          
    //-------------H_V_Select End--------------//

    {SENSOR_WRITE_DELAY, 50},
};



LOCAL SENSOR_REG_TAB_INFO_T s_GC0309_resolution_Tab_YUV[]=
{   
    { ADDR_AND_LEN_OF_ARRAY(gc0309_YUV_640X480),    640,    480, 24,  SENSOR_IMAGE_FORMAT_YUV422 },
    // YUV422 PREVIEW 1
    { PNULL,                    0,   640,    480,    24   ,        SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    

    // YUV422 PREVIEW 2 
     { PNULL,                   0,      0,  0 ,        0,         0     },  //For BCR Preview 
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      }

};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC0309_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    PNULL, 
    PNULL,
    GC0309_Identify,

    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

    // External
    set_gc0309_ae_enable,
    set_hmirror_enable,
    set_vmirror_enable,

    set_brightness,
    set_contrast,
    set_sharpness,
    set_saturation,

    set_preview_mode,   
    set_image_effect,

    PNULL,  //  GC0309_BeforeSnapshot,
    PNULL,      //GC0309_After_Snapshot,

    GC0309_set_flash_enable,

    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate, 
    PNULL,
    PNULL,
    set_gc0309_awb,
    PNULL,
    PNULL,
    set_gc0309_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_gc0309_anti_flicker,
    set_gc0309_video_mode,
    PNULL,
    PNULL
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 PUBLIC const SENSOR_INFO_T g_GC0309_yuv_info =
{
    GC0309_I2C_ADDR_W,              // salve i2c write address
    GC0309_I2C_ADDR_R,              // salve i2c read address
    
    0|\
    SENSOR_I2C_FREQ_100,                                // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                    // other bit: reseved
    SENSOR_HW_SIGNAL_PCLK_N|\
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
    20,                             // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code
    0x00, 0xa0,                     // supply two code to identify sensor.
    0x00, 0xa0,                     // for Example: index = 0-> Device id, index = 1 -> version id  
                                    
    SENSOR_AVDD_2800MV,             // voltage of avdd  

    640,                            // max width of source image
    480,                            // max height of source image
    "GC0309",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_GC0309_resolution_Tab_YUV,    // point to resolution table information structure
    &s_GC0309_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor  
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1800MV,                      // dvdd
    3,
    3,
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
LOCAL void GC0309_WriteReg( uint8  subaddr, uint8 data )
{
    
    #ifndef _USE_DSP_I2C_
        //uint8 cmd[2];
        //cmd[0]    =   subaddr;
        //cmd[1]    =   data;       
        //I2C_WriteCmdArr(GC0309_I2C_ADDR_W, cmd, 2, SCI_TRUE);
        Sensor_WriteReg(subaddr, data);
    #else
        DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
    #endif

    //SCI_TRACE_LOW:"SENSOR: GC0309_WriteReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_690_112_2_18_0_30_46_779,(uint8*)"dd", subaddr, data);

}

LOCAL uint8 GC0309_ReadReg( uint8  subaddr)
{
    uint8 value = 0;
    
    #ifndef _USE_DSP_I2C_
    //I2C_WriteCmdArrNoStop(GC0309_I2C_ADDR_W, &subaddr, 1,SCI_TRUE);
    //I2C_ReadCmd(GC0309_I2C_ADDR_R, &value, SCI_TRUE);
    value =Sensor_ReadReg( subaddr);
    #else
        value = (uint16)DSENSOR_IICRead((uint16)subaddr);
    #endif

    //SCI_TRACE_LOW:"SENSOR: GC0309_ReadReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_706_112_2_18_0_30_46_780,(uint8*)"dd", subaddr, value);
    
    return value;
}

LOCAL uint32 GC0309_Identify(uint32 param)
{
#define GC0309_PID_VALUE    0xa0    
#define GC0309_PID_ADDR     0x00
#define GC0309_VER_VALUE    0xa0    
#define GC0309_VER_ADDR     0x00    

    uint32 i;
    uint32 nLoop;
    uint8 ret;
    uint32 err_cnt = 0;
    uint8 reg[2]    = {0x00, 0x00};
    uint8 value[2]  = {0xa0, 0xa0};

    //SCI_TRACE_LOW:"GC0309_Identify"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_725_112_2_18_0_30_46_781,(uint8*)"");
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = GC0309_ReadReg(reg[i]);
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
                //SCI_TRACE_LOW:"It is not GC0309"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_735_112_2_18_0_30_46_782,(uint8*)"");
                return SCI_ERROR;
            }
            else
            {
                //Masked by frank.yang,SCI_Sleep() will cause a  Assert when called in boot precedure
                //SCI_Sleep(10);
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

    //SCI_TRACE_LOW:"GC0309_Identify: it is GC0309"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_753_112_2_18_0_30_46_783,(uint8*)"");
    
    return (uint32)SCI_SUCCESS;
}

LOCAL uint32 set_gc0309_ae_enable(uint32 enable)
{
    //SCI_TRACE_LOW:"set_gc0309_ae_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_760_112_2_18_0_30_46_784,(uint8*)"d", enable);
    return 0;
}


LOCAL uint32 set_hmirror_enable(uint32 enable)
{
    uint16 reg0x0101;
    
    reg0x0101 = GC0309_ReadReg(0x14);

     //to be opposite because it's fore camera
    if(enable)
    {
        mirror_setting = mirror_setting & 0x0E;
    }
    else
    {
        mirror_setting = mirror_setting | 0x1;     
    }

    reg0x0101 = (reg0x0101& 0xfc) | (mirror_setting&0x3);
    GC0309_WriteReg(0x14, reg0x0101);

    //SCI_TRACE_LOW:"set_gc0309_hmirror_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_783_112_2_18_0_30_46_785,(uint8*)"d", enable);

    return 0;
}


LOCAL uint32 set_vmirror_enable(uint32 enable)
{
    uint16 reg0x0101;
    
    reg0x0101 = GC0309_ReadReg(0x14);

    if(enable)
    {
        mirror_setting = mirror_setting | 0x2;
    }
    else
    {
        mirror_setting = mirror_setting & 0x0d;
    }

    reg0x0101 = (reg0x0101& 0xfc) | (mirror_setting&0x3);    

    GC0309_WriteReg(0x14, reg0x0101);

    //SCI_TRACE_LOW:"set_gc0309_vmirror_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_808_112_2_18_0_30_46_786,(uint8*)"d", enable);

    return 0;   
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T gc0309_brightness_tab[][2]=
{
        {       
            {0xb5, 0xd0},   {0xff,0xff},
        },
    
        {
            {0xb5, 0xe0},   {0xff,0xff},
        },
    
        {
            {0xb5, 0xf0},   {0xff,0xff},
        },
    
        {
            {0xb5, 0x00},   {0xff,0xff},
        },
    
        {
            {0xb5, 0x20},   {0xff,0xff},
        },
    
        {
            {0xb5, 0x30},   {0xff,0xff},
        },
    
        {
            {0xb5, 0x40},   {0xff,0xff},
        },
};


LOCAL uint32 set_brightness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc0309_brightness_tab[level];

    SCI_ASSERT(level < 7);    /*assert to do*/
//  SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        GC0309_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
//  OS_TickDelay(100); 
    //SCI_TRACE_LOW:"set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_863_112_2_18_0_30_47_787,(uint8*)"d", level);
    
    return 0;
}

__align(4) const SENSOR_REG_T GC0309_ev_tab[][3]=
{   
    {{0xd3, 0x48}, {0xb5, 0xd0},{0xff, 0xff}},
    {{0xd3, 0x50}, {0xb5, 0xe0},{0xff, 0xff}},
    {{0xd3, 0x58}, {0xb5, 0xf0},{0xff, 0xff}},
    {{0xd3, 0x60}, {0xb5, 0x10},{0xff, 0xff}},
    {{0xd3, 0x68}, {0xb5, 0x20},{0xff, 0xff}},
    {{0xd3, 0x70}, {0xb5, 0x30},{0xff, 0xff}},
    {{0xd3, 0x78}, {0xb5, 0x40},{0xff, 0xff}},
    
};


LOCAL uint32 set_gc0309_ev(uint32 level)
{
    uint16 i; 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC0309_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert to do*/
//    SCI_ASSERT(level < 7);    /*assert verified*/
 
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) ||(0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        GC0309_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    
    //SCI_TRACE_LOW:"SENSOR: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_895_112_2_18_0_30_47_788,(uint8*)"d", level);

    return 0;
}

/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_gc0309_anti_flicker(uint32 param )
{
    switch (param)
    {
        case DCAMERA_FLICKER_50HZ:
            GC0309_WriteReg(0x01, 0x6a);    
            GC0309_WriteReg(0x02, 0x70); 
            GC0309_WriteReg(0x0f, 0x00);
            GC0309_WriteReg(0xe2,0x00);
            GC0309_WriteReg(0xe3,0x96);
            GC0309_WriteReg(0xe4,0x02);    
            GC0309_WriteReg(0xe5,0x58);    
            GC0309_WriteReg(0xe6,0x04);    
            GC0309_WriteReg(0xe7,0xb0);    
            GC0309_WriteReg(0xe8,0x05);    
            GC0309_WriteReg(0xe9,0xdc);    
            GC0309_WriteReg(0xea,0x09);    
            GC0309_WriteReg(0xeb,0x60);    
            //SCI_TRACE_LOW:"DEBUG_gc0309 anti flick 50HZ!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_923_112_2_18_0_30_47_789,(uint8*)"");
            break;

        case DCAMERA_FLICKER_60HZ:
            GC0309_WriteReg(0x01,0xa8);      
            GC0309_WriteReg(0x02,0x5c);    
            GC0309_WriteReg(0x0f,0x00);
            GC0309_WriteReg(0xe2,0x00);
            GC0309_WriteReg(0xe3,0x74);
            GC0309_WriteReg(0xe4,0x02);    
            GC0309_WriteReg(0xe5,0x44);    
            GC0309_WriteReg(0xe6,0x04);    
            GC0309_WriteReg(0xe7,0x14);    
            GC0309_WriteReg(0xe8,0x05);    
            GC0309_WriteReg(0xe9,0x70);    
            GC0309_WriteReg(0xea,0x08);    
            GC0309_WriteReg(0xeb,0x28);  
            //SCI_TRACE_LOW:"DEBUG_gc0309 anti flick 60HZ!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_940_112_2_18_0_30_47_790,(uint8*)"");
            break;

        default:
            break;
    }

    return 0;
}

/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//       
/******************************************************************************/
__align(4) const SENSOR_REG_T GC0309_video_mode_nor_tab[][2]=
{
    // normal mode      25 fps
    {
    {0xec,0x00},
    {0xff,0xff}, 
    },    
    //vodeo mode     25 fps
    {
    {0xec,0x00},
    {0xff,0xff} 
    },
   // UPCC  mode      25 fps
    {
    {0xec,0x00},
    {0xff,0xff},
    }   
};     

LOCAL uint32 set_gc0309_video_mode(uint32 mode)
{
    //uint8 data=0x00;
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;
  //  uint8 tempregval = 0;

    sensor_reg_ptr = (SENSOR_REG_T*)GC0309_video_mode_nor_tab[mode];

    if(mode > DCAMERA_MODE_MAX ||PNULL == sensor_reg_ptr)
    {
        return SCI_ERROR;
    }

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        GC0309_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"SENSOR: set_video_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_991_112_2_18_0_30_47_791,(uint8*)"d", mode);
    return 0;
}


/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T gc0309_awb_tab[][6]=

{

     //AUTO
    {
        {0x41, 0x3d},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
        {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x80},
        {0xff, 0xff},
        {0xff, 0xff}         
    },
    //U30 ?
    {
       {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff},
        {0xff, 0xff}      
    },  
    //CWF ?
    {
        {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff},
        {0xff, 0xff}            
    },    
    //FLUORESCENT:
    {
        {0x41, 0x39},
        {0xca, 0x50},
        {0xcb, 0x40},
        {0xcc, 0x70},
        {0xff, 0xff},
        {0xff, 0xff}          
    },
    //SUN:
    {
        {0x41, 0x39},
        {0xca, 0x5a},
        {0xcb, 0x40},
        {0xcc, 0x58},
        {0xff, 0xff},
        {0xff, 0xff}           
    },
    //CLOUD:
    {
        {0x41, 0x39},
        {0xca, 0x68},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff},
        {0xff, 0xff}            
    },
};

/*
// enum: preview while balance mode
typedef enum
{
    DCAMERA_WB_MODE_AUTO = 0x00,            //自动
    DCAMERA_WB_MODE_INCANDESCENCE,      //白炽灯
    DCAMERA_WB_MODE_U30,                //商用光源
    DCAMERA_WB_MODE_CWF,                //冷荧光
    DCAMERA_WB_MODE_FLUORESCENT,        //日光灯
    DCAMERA_WB_MODE_SUN,                //晴天
    DCAMERA_WB_MODE_CLOUD,              //阴天
    DCAMERA_WB_MODE_MAX
}DCAMERA_PARAM_WB_MODE_E;
*/
__align(4) const SENSOR_REG_T GC0309_awb_tab[][5]=
    {
        //AUTO
        {
            {0x5a, 0x4c}, 
            {0x5b, 0x40}, 
            {0x5c, 0x4a},
            {0x22, 0x57},    // the reg value is not written here, rewrite in set_GC0309_awb();
            {0xff, 0xff}
        },    
        //INCANDESCENCE:
        {
            {0x22, 0x55},    // Disable AWB 
            {0x5a, 0x48},
            {0x5b, 0x40},
            {0x5c, 0x5c},
            {0xff, 0xff} 
        },
        //dummy now
        {
            {0x22, 0x55},    // Disable AWB 
            {0x5a, 0x48},
            {0x5b, 0x40},
            {0x5c, 0x5c},
            {0xff, 0xff} 
        },
        //dummy now
        {
            {0x22, 0x55},    // Disable AWB 
            {0x5a, 0x48},
            {0x5b, 0x40},
            {0x5c, 0x5c},
            {0xff, 0xff} 
        },
        //FLUORESCENT:
        {
            {0x22, 0x55},   // Disable AWB 
            {0x5a, 0x40},
            {0x5b, 0x42}, 
            {0x5c, 0x50},
            {0xff, 0xff} 
        },
        //SUN:
        {
            {0x22, 0x55},   // Disable AWB 
            {0x5a, 0x5a},
            {0x5b, 0x40},
            {0x5c, 0x58},
            {0xff, 0xff} 
        },
        //CLOUD:
        {
            {0x22, 0x55},   // Disable AWB 
            {0x5a, 0x68}, 
            {0x5b, 0x40},
            {0x5c, 0x50},
            {0xff, 0xff} 
        },
    };
    
    LOCAL uint32 set_gc0309_awb(uint32 mode)
    {
        uint16 i;
        
        SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC0309_awb_tab[mode];

        SCI_ASSERT(mode < 7);    /*assert verified*/
//      SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/
        
        SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/
        for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
        {
            GC0309_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
        }

//      OS_TickDelay(100); 
        //SCI_TRACE_LOW:"SENSOR: set_awb_mode: mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_1159_112_2_18_0_30_47_792,(uint8*)"d", mode);
        
        return 0;
}

__align(4) const SENSOR_REG_T gc0309_contrast_tab[][2]=
{
        {
            {0xb3,0x28},    {0xff,0xff},
        },
    
        {
            {0xb3,0x30},    {0xff,0xff}, 
        },
    
        {
            {0xb3,0x38},    {0xff,0xff}, 
        },
    
        {
            {0xb3,0x40},    {0xff,0xff},
        },
    
        {
            {0xb3,0x48},    {0xff,0xff}, 
        },
    
        {
            {0xb3,0x50},    {0xff,0xff}, 
        },
    
        {
            {0xb3,0x58},    {0xff,0xff},
        },                   
};


LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)gc0309_contrast_tab[level];

    SCI_ASSERT(level < 7 );    /*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        GC0309_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
//    OS_TickDelay(20);
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_1211_112_2_18_0_30_47_793,(uint8*)"d", level);
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
    //SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_1238_112_2_18_0_30_47_794,(uint8*)"d", preview_mode);
    
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            GC0309_WriteReg(0xec, 0x00);        
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            GC0309_WriteReg(0xec, 0x30);        
            break;
        }
        default:
        {
            break;
        }
    } 

    //SCI_TRACE_LOW:"set_GC0309_preview_mode: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_1256_112_2_18_0_30_47_795,(uint8*)"d", preview_mode);

    return 0;
     
}
/*
typedef enum
{
    DCAMERA_EFFECT_NORMAL = 0x00,
    DCAMERA_EFFECT_BLACKWHITE,          // 黑白
    DCAMERA_EFFECT_RED,                 // 偏红
    DCAMERA_EFFECT_GREEN,               // 偏绿
    DCAMERA_EFFECT_BLUE,                // 偏蓝
    DCAMERA_EFFECT_YELLOW,              // 偏黄
    DCAMERA_EFFECT_NEGATIVE,            // 底片
    DCAMERA_EFFECT_CANVAS,              // 帆布
    DCAMERA_EFFECT_RELIEVOS,            // 浮雕 
    DCAMERA_EFFECT_MAX
}DCAMERA_PARAM_EFFECT_E;
*/

__align(4) const SENSOR_REG_T GC0309_image_effect_tab[][11]=    
{
    // effect normal
    {
        {0x23,0x00}, {0x2d,0x0a}, {0x20,0x7f}, {0xd2,0x90}, {0x73,0x00}, {0x77,0x78},
        {0xb3,0x42}, {0xb4,0x80}, {0xba,0x00}, {0xbb,0x00}, {0xff,0xff}
    },
    //effect BLACKWHITE
    {
        {0x23,0x02}, {0x2d,0x0a}, {0x20,0x7f}, {0xd2,0x90}, {0x73,0x00},  
        {0xb3,0x40},    {0xb4,0x80}, {0xba,0x00}, {0xbb,0x00}, {0xff,0xff}, {0xff,0xff}
    },
    // effect RED pink
    {
        //TODO: later work
        {0x23,0x02},{0x2d,0x0a},{0x20,0x7f},{0xd2,0x90},{0x77,0x88},
            {0xb3,0x40},{0xb4,0x80},{0xba,0x10},{0xbb,0x50},{0xff, 0xff}, {0xff,0xff}
    },
    // effect GREEN
    {
        {0x23,0x02},{0x2d,0x0a},{0x20,0x7f},{0xd2,0x90},{0x77,0x88},
            {0xb3,0x40},{0xb4,0x80},{0xba,0xc0},{0xbb,0xc0},{0xff, 0xff}, {0xff,0xff}
    },
    // effect  BLUE
    {
        {0x23,0x02},{0x2d,0x0a},{0x20,0x7f},{0xd2,0x90},{0x73,0x00},
        {0xb3,0x40},{0xb4,0x80},{0xba,0x50},{0xbb,0xe0},{0xff, 0xff}, {0xff,0xff}
    },
    // effect  YELLOW
    {
        //TODO:later work
        {0x23,0x02},{0x2d,0x0a},{0x20,0x7f},{0xd2,0x90},{0x77,0x88},
            {0xb3,0x40},{0xb4,0x80},{0xba,0x80},{0xbb,0x20},{0xff, 0xff}, {0xff,0xff}

    },  
    // effect NEGATIVE
    {        
        {0x23,0x01},{0x2d,0x0a},{0x20,0x7f},{0xd2,0x90},{0x73,0x00},
        {0xb3,0x40},{0xb4,0x80},{0xba,0x00},{0xbb,0x00},{0xff, 0xff}, {0xff,0xff}
    },    
    //effect ANTIQUE
    {
        {0x23,0x02},{0x2d,0x0a},{0x20,0x7f},{0xd2,0x90},{0x73,0x00},
        {0xb3,0x40},{0xb4,0x80},{0xba,0xd0},{0xbb,0x28},{0xff, 0xff}, {0xff,0xff}
    },
};
LOCAL uint32 set_image_effect(uint32 effect_type)
{
    uint16 i;
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC0309_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"-----------set_image_effect: effect_type = %d------------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_1335_112_2_18_0_30_47_796,(uint8*)"d", effect_type);
    
    return 0;
}

/*
LOCAL uint32 GC0309_After_Snapshot(uint32 param)
{

    Sensor_SetMCLK(24);
    
    GC0309_WriteReg(0x41,GC0309_ReadReg(0x41) | 0xf7);
    SCI_Sleep(200);
    return 0;
    
}

LOCAL uint32 GC0309_BeforeSnapshot(uint32 param)
{

    uint16 shutter = 0x00;
    uint16 temp_reg = 0x00;
    uint16 temp_r =0x00;
    uint16 temp_g =0x00;
    uint16 temp_b =0x00;    
    BOOLEAN b_AEC_on;
    

    //SCI_TRACE_LOW:"GC0309_BeforeSnapshot "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_1362_112_2_18_0_30_48_797,(uint8*)"");
        if(GC0309_ReadReg(0X41)  & 0x08 == 0x08)  //AEC on
            b_AEC_on = SCI_TRUE;
        else
            b_AEC_on = SCI_FALSE;

    temp_reg = GC0309_ReadReg(0xdb);
    temp_r = GC0309_ReadReg(0xcd);
    temp_g = GC0309_ReadReg(0xce);
    temp_b = GC0309_ReadReg(0xcf);

    shutter = (GC0309_ReadReg(0x03)<<8)  | (GC0309_ReadReg(0x04)&0x00ff) ;
    shutter = shutter /2;

    if(b_AEC_on)
        GC0309_WriteReg(0x41,GC0309_ReadReg(0x41) & 0xc5); //0x01);
    SCI_Sleep(300); 

///12m
    Sensor_SetMCLK(12);
    
    GC0309_WriteReg(0x03,shutter/256);
    GC0309_WriteReg(0x04,shutter & 0x00ff); 
    //SCI_TRACE_LOW("GC0309_BeforeSnapshot, temp_r=%x,temp_reg=%x, final = %x ",temp_r,temp_reg, temp_r*temp_reg/ 0x80);    

    temp_r = (temp_r*temp_reg) / 0x80;
    temp_g = (temp_g*temp_reg) / 0x80;
    temp_b = (temp_b*temp_reg) / 0x80;
    if(b_AEC_on)
    {
        GC0309_WriteReg(0xcd, temp_r);
        GC0309_WriteReg(0xce, temp_g);
        GC0309_WriteReg(0xcf , temp_b);
    }
    //SCI_TRACE_LOW("GC0309_BeforeSnapshot, temp_r=%x,temp_g=%x, temp_b = %x ",temp_r,temp_g,temp_b);    

    SCI_Sleep(300); 
        return 0;
    
}
*/
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
    //GC0309_WriteReg( 0xd8, uint8 data );
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T gc0309_mode_tab[][8]=
{
    //LCD的GAMMA值需要细调，不然会有一圈圈的光晕
    //Fps 12.5 YUV open auto frame function, 展讯的jpeg编码不行太大和太快，因此将帧率限制在12.5fps
    {/*{0xa0, 0x50},*/{0xec, 0x00},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},},
    //Fps 12.5->3.125 YUV open auto frame function
    {/*{0xa0, 0x40},*/{0xec, 0x30},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},{0xFF, 0xFF},},
};

LOCAL uint32 GC0309_set_work_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc0309_mode_tab[mode];

    SCI_ASSERT(mode <= 1);    /*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        GC0309_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"set_work_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0309_1469_112_2_18_0_30_48_798,(uint8*)"d", mode);
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 GC0309_set_flash_enable(uint32 enable)
{
#ifdef DC_FLASH_SUPPORT
    GPIO_SetFlashLight(enable);
#endif
    return 0;
}
