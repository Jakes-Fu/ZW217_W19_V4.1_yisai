/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_NT99250.c                                          *
 ** Author:                                                   *
 ** DATE:           2010.07.07                                                *
 ** Description:   This file contains driver for sensor NT99250
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
#include "sci_api.h"

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
#define NT99250_I2C_ADDR_W 0x6C
#define NT99250_I2C_ADDR_R 0x6D

 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 NT99250_set_ae_enable(uint32 enable);
LOCAL uint32 NT99250_set_hmirror_enable(uint32 enable);
LOCAL uint32 NT99250_set_vmirror_enable(uint32 enable);
LOCAL uint32 NT99250_set_preview_mode(uint32 preview_mode);
LOCAL uint32 NT99250_Identify(uint32 param);
LOCAL uint32 NT99250_BeforeSnapshot(uint32 param);
    
LOCAL uint32 NT99250_set_brightness(uint32 level);
LOCAL uint32 NT99250_set_contrast(uint32 level);
LOCAL uint32 NT99250_set_sharpness(uint32 level);
LOCAL uint32 NT99250_set_saturation(uint32 level);
LOCAL uint32 NT99250_set_image_effect(uint32 effect_type);
LOCAL uint32 NT99250_set_work_mode(uint32 mode);
LOCAL uint32 NT99250_chang_image_format(uint32 param);
LOCAL uint32 NT99250_check_image_format_support(uint32 param);
LOCAL uint32 NT99250_after_snapshot(uint32 param);

LOCAL uint32 set_NT99250_ev(uint32 level);
LOCAL uint32 set_NT99250_awb(uint32 mode);
LOCAL uint32 set_NT99250_anti_flicker(uint32 mode);
LOCAL uint32 set_NT99250_video_mode(uint32 mode); 
//LOCAL uint32 NT99250_zoom(uint32 level);
LOCAL uint32 _NT99250_GetResolutionTrimTab(uint32 param);

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 s_change_mode=SENSOR_MODE_COMMON_INIT;
LOCAL uint32 s_preview_mode;
LOCAL uint32 s_image_effect = 0;
LOCAL uint32 work_mode = 0;
LOCAL uint32 reg3012 = 0x01;
LOCAL uint32 reg3013 = 0x80;
LOCAL uint32 reg301d = 0x00;

/*lint -save -e533 */
__align(4)  const SENSOR_REG_T NT99250_YUV_COMMON[]=
{

    {0x32F0, 0x00},
    {0x301e, 0x00},
    {0x301f, 0x25},

    {0x3270, 0x00},
    {0x3271, 0x0C},
    {0x3272, 0x17},
    {0x3273, 0x2E},
    {0x3274, 0x46},
    {0x3275, 0x5D},
    {0x3276, 0x85},
    {0x3277, 0xA2},
    {0x3278, 0xBA},
    {0x3279, 0xCD},
    {0x327A, 0xE9},
    {0x327B, 0xF6},
    {0x327C, 0xFB},
    {0x327D, 0xFE},
    {0x327E, 0xFF},

    {0x3302, 0x00},
    {0x3303, 0x46},
    {0x3304, 0x00},
    {0x3305, 0xA9},
    {0x3306, 0x00},
    {0x3307, 0x0F},
    {0x3308, 0x07},
    {0x3309, 0xD9},
    {0x330A, 0x06},
    {0x330B, 0xFC},
    {0x330C, 0x01},
    {0x330D, 0x2C},
    {0x330E, 0x01},
    {0x330F, 0x3A},
    {0x3310, 0x06},
    {0x3311, 0xED},
    {0x3312, 0x07},
    {0x3313, 0xD9},


    {0x3290, 0x01},
    {0x3291, 0x40},
    {0x3296, 0x01},
    {0x3297, 0x60},

    {0x3102, 0x0b},
    {0x3103, 0x46},
    {0x3105, 0x33},
    {0x3107, 0x32},
    {0x310A, 0x03},
    {0x310B, 0x18},
    {0x310f, 0x08},
    {0x3110, 0x03},
    {0x3113, 0x0F},
    {0x3119, 0x17},
    {0x3114, 0x03},
    {0x3117, 0x0F},
    {0x3118, 0x01},
    {0x3044, 0x02},
    {0x3045, 0xD0},
    {0x3046, 0x02},
    {0x3047, 0xD0},
    {0x3048, 0x02},
    {0x3049, 0xD0},
    {0x304a, 0x02},
    {0x304b, 0xD0},
    {0x303e, 0x02},
    {0x303f, 0x2B},
    {0x3052, 0x80},
    {0x3053, 0x00},
    {0x305f, 0x52},
    {0x3058, 0x01},
    {0x3080, 0x80},
    {0x3081, 0x80},
    {0x3082, 0x80},
    {0x3083, 0x40},
    {0x3084, 0x80},
    {0x3085, 0x40},

    {0x3068, 0x01},
    {0x3069, 0x01},
    {0x306b, 0x01},
    {0x306c, 0x01},
    {0x306d, 0x01},
    {0x306e, 0x01},       
          
    //Lsc             
    {0x3262, 0x0B},
    {0x3263, 0x0B},
    {0x3264, 0x0B},
    {0x3250, 0x01},
    {0x3251, 0xB0},
    {0x3252, 0x01},
    {0x3253, 0xB0},
    {0x3254, 0x01},
    {0x3255, 0xB0},
    {0x3265, 0x27},
    {0x3266, 0x00},

    //edge table
    {0x3326, 0x02},
    {0x3327, 0x0A},
    {0x3328, 0x0A},
    {0x3329, 0x06},
    {0x332A, 0x06},
    {0x332B, 0x1C},
    {0x332C, 0x1C},
    {0x332D, 0x00},
    {0x332E, 0x1D},
    {0x332F, 0x1F},
          
    //Nr              
    {0x32d8, 0x3F},
    {0x32d9, 0x28},
    {0x3320, 0x10},
    {0x3323, 0x30},
    {0x3324, 0x07},
    {0x3338, 0x03},
    {0x3339, 0x10},
    {0x333A, 0x30},
    {0x3330, 0x00},
    {0x3331, 0x0A},
    {0x3332, 0xA0},
    {0x3333, 0xFF},
    {0x3334, 0xFF},
    {0x3300, 0x20},
    {0x3301, 0x28},
          
          
    //AE              
    {0x32bc, 0x40},
    {0x32bd, 0x05},
    {0x32be, 0x05},
    {0x32bf, 0x6A},
    {0x32C0, 0x22},
    {0x32cd, 0x01},
    {0x32d3, 0x13},
    {0x32d7, 0x82},
    //{0x3077, 0x00},
    {0x32c3, 0x0A},
    {0x32c5, 0x20},
    {0x32bb, 0x1b},
    {0x3012, 0x01},
    {0x3013, 0x80},
          
    //Chroma          
    {0x32f6, 0x0d},
    {0x32f9, 0x33},
    {0x32fA, 0x33}                                  
};


__align(4) SENSOR_REG_T NT99250_YUV_1600X1200[]=
{
    {0x3200, 0x3e},
    {0x3201, 0x0f},
    {0x3022, 0x24},
    {0x3023, 0x24},
    {0x3002, 0x00},
    {0x3003, 0x04},
    {0x3004, 0x00},
    {0x3005, 0x04},
    {0x3006, 0x06},
    {0x3007, 0x43},
    {0x3008, 0x04},
    {0x3009, 0xb3},
    {0x300a, 0x09},
    {0x300b, 0xc4},
    {0x300c, 0x07},
    {0x300d, 0xa7},
    {0x300e, 0x06},
    {0x300f, 0x40},
    {0x3010, 0x04},
    {0x3011, 0xb0},
    {0x3021, 0x02},
    {0x3060, 0x01}
};

__align(4) SENSOR_REG_T NT99250_YUV_640X480_Video[]=          
{
// Mclk=24 Pclk=56  30~30fps  for Video  Preview
    {0x3300,0x28}, 
    {0x3301,0x70},
    {0x3320,0x40}, 
    {0x3324,0x07}, 
    {0x3325,0x5F}, 
    {0x3326,0x02}, 
    {0x3331,0x0b}, 
    {0x3332,0x80}, 
    {0x3200,0x3e}, 
    {0x3201,0x3f},
    {0x32e0,0x02}, 
    {0x32e1,0x80}, 
    {0x32e2,0x01}, 
    {0x32e3,0xe0}, 
    {0x32e4,0x01}, 
    {0x32e5,0x00}, 
    {0x32e6,0x00}, 
    {0x32e7,0x00}, 
    {0x301e,0x00},  
    {0x301f,0x27}, 
    {0x3022,0x24}, 
    {0x3023,0x64}, 
    {0x3002,0x00}, 
    {0x3003,0xa4}, 
    {0x3004,0x00}, 
    {0x3005,0x7c}, 
    {0x3006,0x05}, 
    {0x3007,0xa3}, 
    {0x3008,0x04}, 
    {0x3009,0x3b}, 
    {0x300a,0x06}, 
    {0x300b,0x80}, 
    {0x300c,0x02}, 
    {0x300d,0x30}, 
    {0x300e,0x05}, 
    {0x300f,0x00}, 
    {0x3010,0x01}, 
    {0x3011,0xe0}, 
    {0x32bb,0x0b}, 
    {0x32bc,0x4b}, 
    {0x32c1,0x22}, 
    {0x32c2,0x30}, 
    {0x32c8,0xa8}, 
    {0x32c9,0x8c}, 
    {0x32c4,0x00}, 
    {0x3201,0x7f}, 
    {0x3021,0x06}, 
    {0x3060,0x01}
};    

__align(4) SENSOR_REG_T NT99250_YUV_640X480[]=          
{
// Mclk=24 Pclk=56  20~30fps  for  Preview
    {0x3300,0x28}, 
    {0x3301,0x70}, 
    {0x3320,0x40}, 
    {0x3324,0x07}, 
    {0x3325,0x5F}, 
    {0x3326,0x02}, 
    {0x3331,0x0b}, 
    {0x3332,0x80}, 
    {0x3200,0x3e}, 
    {0x3201,0x3f},
    {0x32e0,0x02}, 
    {0x32e1,0x80}, 
    {0x32e2,0x01}, 
    {0x32e3,0xe0}, 
    {0x32e4,0x01}, 
    {0x32e5,0x00}, 
    {0x32e6,0x00}, 
    {0x32e7,0x00}, 
    {0x301e,0x00},                
    {0x301f,0x27}, 
    {0x3022,0x24}, 
    {0x3023,0x64}, 
    {0x3002,0x00}, 
    {0x3003,0xa4}, 
    {0x3004,0x00}, 
    {0x3005,0x7c}, 
    {0x3006,0x05}, 
    {0x3007,0xa3}, 
    {0x3008,0x04}, 
    {0x3009,0x3b}, 
    {0x300a,0x06}, 
    {0x300b,0x80}, 
    {0x300c,0x02}, 
    {0x300d,0x30}, 
    {0x300e,0x05}, 
    {0x300f,0x00}, 
    {0x3010,0x01}, 
    {0x3011,0xe0}, 
    {0x32bb,0x0b}, 
    {0x32bc,0x4b}, 
    {0x32c1,0x23}, 
    {0x32c2,0x49}, 
    {0x32c8,0xa8}, 
    {0x32c9,0x8c}, 
    {0x32c4,0x00}, 
    {0x3201,0x7f}, 
    {0x3021,0x02}, 
    {0x3060,0x01}
};    

LOCAL SENSOR_REG_TAB_INFO_T s_NT99250_resolution_Tab_YUV[]=
{   
    // COMMON INIT
    { ADDR_AND_LEN_OF_ARRAY(NT99250_YUV_COMMON), 0, 0, 24, SENSOR_IMAGE_FORMAT_YUV422}, 

    // YUV422 PREVIEW 1 
    { ADDR_AND_LEN_OF_ARRAY(NT99250_YUV_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},      
    { ADDR_AND_LEN_OF_ARRAY(NT99250_YUV_1600X1200), 1600, 1200, 24,SENSOR_IMAGE_FORMAT_YUV422}, 
    { PNULL,    0, 0,   0, 0, 0},
    { PNULL,    0, 0,   0, 0, 0},

    // YUV422 PREVIEW 2 
    { PNULL,    0, 0,   0, 0, 0},
    { PNULL,    0, 0,   0, 0, 0},
    { PNULL,    0, 0,   0, 0, 0},
    { PNULL,    0, 0,   0, 0, 0} 
};

LOCAL SENSOR_TRIM_T s_NT99250_Resolution_Trim_Tab[]=
{    
    // COMMON INIT
    {0, 0, 0, 0, 0, 56},

    // YUV422 PREVIEW 1    
    {0, 0, 640, 480, 68, 56},
    {0, 0, 1600, 1200, 122, 56},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    
    // YUV422 PREVIEW 2 
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_NT99250_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    PNULL,
    PNULL,
    NT99250_Identify,

    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    _NT99250_GetResolutionTrimTab,

    // External
    NT99250_set_ae_enable,
    NT99250_set_hmirror_enable,
    NT99250_set_vmirror_enable,

    NT99250_set_brightness,
    NT99250_set_contrast,
    PNULL,//NT99250_set_sharpness,
    PNULL,//NT99250_set_saturation,

    NT99250_set_preview_mode,   
    NT99250_set_image_effect,

    NT99250_BeforeSnapshot,
    NT99250_after_snapshot,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_NT99250_awb,
    PNULL,
    PNULL,
    set_NT99250_ev,
    NT99250_check_image_format_support,
    NT99250_chang_image_format,
    PNULL,
    PNULL,
    PNULL,
    set_NT99250_anti_flicker,
    PNULL,

    PNULL,
    PNULL
};


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_NT99250_yuv_info =
{
    NT99250_I2C_ADDR_W,             // salve i2c write address
    NT99250_I2C_ADDR_R,                 // salve i2c read address
    
    SENSOR_I2C_REG_16BIT,           // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit1: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
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
    20,                             // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code
    0x3000, 0x25,                       // supply two code to identify sensor.
    0x3001, 0x01,                       // for Example: index = 0-> Device id, index = 1 -> version id                                          
                                            
    SENSOR_AVDD_2800MV,             // voltage of avdd  
    
    1600,                           // max width of source image
    1200,                           // max height of source image
    "NT99250",                      // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,SENSOR_IMAGE_FORMAT_MAX
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T

    SENSOR_IMAGE_PATTERN_YUV422_UYVY,   // pattern of input image form sensor;          

    s_NT99250_resolution_Tab_YUV,   // point to resolution table information structure
    &s_NT99250_ioctl_func_tab,      // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,              // extend information about sensor
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1500MV,                     // dvdd
    3,                     // skip frame num before preview 
    2,                     // skip frame num before capture
    0,                     // deci frame num during preview;    
    0,                    // deci frame num during video preview;
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
/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 NT99250_set_ae_enable(uint32 enable)
{

    return 0;
}
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99250_banding_flicker_tab[][2]=
{
    // 50HZ
    {
        {0x32bb, 0x0b},{0xffff,0xff} 
    },    
    //60HZ
    {
        {0x32bb, 0x03},{0xffff,0xff}       
    },
};   

LOCAL uint32 set_NT99250_anti_flicker(uint32 mode)
{//107 us
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;


    if(mode > 1)
    {
            return 0;
    }
    sensor_reg_ptr = (SENSOR_REG_T*)NT99250_banding_flicker_tab[mode];

    SCI_ASSERT(PNULL != sensor_reg_ptr);  /*assert to do*/

    for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr)|| (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_Sleep(100); 
    SCI_TRACE_LOW("SENSOR: set_NT99250_flicker: 0x%x", mode);

    return 0;
}
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author: Tim.zhu
// Note:
//        
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T NT99250_video_mode_tab[][50]=
{
    {//nor mode
        {0x3300,0x28},
        {0x3301,0x70},
        {0x3320,0x40},
        {0x3324,0x07},
        {0x3325,0x5F},
        {0x3326,0x02},
        {0x3331,0x0b},
        {0x3332,0x80},
        {0x3200,0x3e},
        {0x3201,0x3f},
        {0x32e0,0x02},
        {0x32e1,0x80},
        {0x32e2,0x01},
        {0x32e3,0xe0},
        {0x32e4,0x01},
        {0x32e5,0x00},
        {0x32e6,0x00},
        {0x32e7,0x00},
        {0x301e,0x00},
        {0x301f,0x27},
        {0x3022,0x24},
        {0x3023,0x64},
        {0x3002,0x00},
        {0x3003,0xa4},
        {0x3004,0x00},
        {0x3005,0x7c},
        {0x3006,0x05},
        {0x3007,0xa3},
        {0x3008,0x04},
        {0x3009,0x3b},
        {0x300a,0x06},
        {0x300b,0x80},
        {0x300c,0x02},
        {0x300d,0x30},
        {0x300e,0x05},
        {0x300f,0x00},
        {0x3010,0x01},
        {0x3011,0xe0},
        {0x32bb,0x0b},
        {0x32bc,0x4b},
        {0x32c1,0x22},
        {0x32c2,0x30},
        {0x32c8,0xa8},
        {0x32c9,0x8c},
        {0x32c4,0x00},
        {0x3201,0x7f},
        {0x3021,0x02},
        {0x3060,0x01},         
        {0xffff,0xff}

    },                  /*lint !e785*/
    {//video mode fix 30fps
        {0x3300,0x28},
        {0x3301,0x70},
        {0x3320,0x40},
        {0x3324,0x07},
        {0x3325,0x5F},
        {0x3326,0x02},
        {0x3331,0x0b},
        {0x3332,0x80},
        {0x3200,0x3e},
        {0x3201,0x3f},
        {0x32e0,0x02},
        {0x32e1,0x80},
        {0x32e2,0x01},
        {0x32e3,0xe0},
        {0x32e4,0x01},
        {0x32e5,0x00},
        {0x32e6,0x00},
        {0x32e7,0x00},
        {0x301e,0x00},
        {0x301f,0x27},
        {0x3022,0x24},
        {0x3023,0x64},
        {0x3002,0x00},
        {0x3003,0xa4},
        {0x3004,0x00},
        {0x3005,0x7c},
        {0x3006,0x05},
        {0x3007,0xa3},
        {0x3008,0x04},
        {0x3009,0x3b},
        {0x300a,0x06},
        {0x300b,0x80},
        {0x300c,0x02},
        {0x300d,0x30},
        {0x300e,0x05},
        {0x300f,0x00},
        {0x3010,0x01},
        {0x3011,0xe0},
        {0x32bb,0x0b},
        {0x32bc,0x4b},
        {0x32c1,0x22},
        {0x32c2,0x30},
        {0x32c8,0xa8},
        {0x32c9,0x8c},
        {0x32c4,0x00},
        {0x3201,0x7f},
        {0x3021,0x02},
        {0x3060,0x01},           
        {0xffff,0xff}
    },                      /*lint !e785*/
    {//upcc mode fix 30fps
        {0x3300,0x28},
        {0x3301,0x70},
        {0x3320,0x40},
        {0x3324,0x07},
        {0x3325,0x5F},
        {0x3326,0x02},
        {0x3331,0x0b},
        {0x3332,0x80},
        {0x3200,0x3e},
        {0x3201,0x3f},
        {0x32e0,0x02},
        {0x32e1,0x80},
        {0x32e2,0x01},
        {0x32e3,0xe0},
        {0x32e4,0x01},
        {0x32e5,0x00},
        {0x32e6,0x00},
        {0x32e7,0x00},
        {0x301e,0x00},
        {0x301f,0x27},
        {0x3022,0x24},
        {0x3023,0x64},
        {0x3002,0x00},
        {0x3003,0xa4},
        {0x3004,0x00},
        {0x3005,0x7c},
        {0x3006,0x05},
        {0x3007,0xa3},
        {0x3008,0x04},
        {0x3009,0x3b},
        {0x300a,0x06},
        {0x300b,0x80},
        {0x300c,0x02},
        {0x300d,0x30},
        {0x300e,0x05},
        {0x300f,0x00},
        {0x3010,0x01},
        {0x3011,0xe0},
        {0x32bb,0x0b},
        {0x32bc,0x4b},
        {0x32c1,0x22},
        {0x32c2,0x30},
        {0x32c8,0xa8},
        {0x32c9,0x8c},
        {0x32c4,0x00},
        {0x3201,0x7f},
        {0x3021,0x02},
        {0x3060,0x01},            
        {0xffff,0xff}
    }                   /*lint !e785*/
}; 
LOCAL uint32 set_NT99250_video_mode(uint32 mode)
{
    SENSOR_REG_T_PTR sensor_reg_ptr=(SENSOR_REG_T_PTR)NT99250_video_mode_tab[mode];
    uint16 i=0x00;
    
    SCI_ASSERT(PNULL!=sensor_reg_ptr);  /*assert to do*/
    
    for(i=0x00; (0xffff!=sensor_reg_ptr[i].reg_addr)||(0xff!=sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("SENSOR: set_NT99250_video_mode = 0x%02x", mode);
    return 0;
}

/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/

__align(4) const SENSOR_REG_T NT99250_awb_tab[][5]=

{
    //AUTO
    {
        {0x3201, 0x7F},
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff},
        {0xffff, 0xff}        
    },    
    //OFFICE:
    {
        {0x3290, 0x01},
        {0x3291, 0x30},
        {0x3296, 0x01},
        {0x3297, 0xCB},
        {0xffff, 0xff}
    },
        //U30
    {
        {0x3290, 0x01},
        {0x3291, 0xC8},
        {0x3296, 0x01},       
        {0x3297, 0x92},    
        {0xffff, 0xff}            
    },  
    //CWF
    {
        {0x3290, 0x01},
        {0x3291, 0x70},
        {0x3296, 0x01},
        {0x3297, 0xff},
        {0xffff, 0xff},  
    },    
    //HOME
    {
        {0x3290, 0x01},
        {0x3291, 0x00},
        {0x3296, 0x02},
        {0x3297, 0x30},
        {0xffff, 0xff}
    },  
    
    //SUN:
    {
        {0x3290, 0x01},
        {0x3291, 0x38},
        {0x3296, 0x01},
        {0x3297, 0x68},
        {0xffff, 0xff}
    },
    //CLOUD:
    {
        {0x3290, 0x01},
        {0x3291, 0x51},
        {0x3296, 0x01},
        {0x3297, 0x00},
        {0xffff, 0xff}
    }
};

LOCAL uint32 set_NT99250_awb(uint32 mode)
{
    uint16 i;
    uint8 temp;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99250_awb_tab[mode];

    if(mode >= DCAMERA_WB_MODE_MAX)
        return SENSOR_OP_PARAM_ERR;

    temp = Sensor_ReadReg(0x3201);
    if(mode != DCAMERA_WB_MODE_AUTO)
        Sensor_WriteReg(0x3201, temp & (~0x10));
    else
        Sensor_WriteReg(0x3201, temp | 0x10);
    
    SCI_ASSERT(PNULL != sensor_reg_ptr);  /*assert to do*/
    
    for(i = 0;(0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    SCI_TRACE_LOW("SENSOR: set_awb_mode: mode = %d", mode);

    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:yangbin
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99250_ev_tab[][2]=
{

    {{0x32F2, 0x50},{0xffff, 0xff}},
    {{0x32F2, 0x60},{0xffff, 0xff}},
    {{0x32F2, 0x70},{0xffff, 0xff}},
    {{0x32F2, 0x80},{0xffff, 0xff}},
    {{0x32F2, 0x90},{0xffff, 0xff}},
    {{0x32F2, 0xA0},{0xffff, 0xff}},
    {{0x32F2, 0xB0},{0xffff, 0xff}} //for nt99250
};

LOCAL uint32 set_NT99250_ev(uint32 level)
{
    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99250_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);  /*assert to do*/
    SCI_ASSERT(level < 7);  /*assert to do*/

    for(i = 0; i < 1 /*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 

    SCI_TRACE_LOW("SENSOR: set_ev: level = %d", level);

    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 NT99250_set_hmirror_enable(uint32 enable)
{

    uint16 reg04;

    reg04 = Sensor_ReadReg(0x3022);
    
    if (enable)
    {
        reg04 = (reg04 | 0x02);//reg04 = (reg04 | 0x02);
        Sensor_WriteReg(0x3022, reg04);
    }
    else
    {
        reg04 = (reg04 & (~(0x02)));//reg04 = (reg04 & (~(0x02)));
        Sensor_WriteReg(0x3022, reg04);
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
LOCAL uint32 NT99250_set_vmirror_enable(uint32 enable)
{
    uint16 reg04;
    
    reg04 = Sensor_ReadReg(0x3022);
    
    if (enable)
    {
        reg04 = (reg04 | 0x01);
        Sensor_WriteReg(0x3022, reg04);
    }
    else
    {
        reg04 = (reg04 & (~(0x01)));
        Sensor_WriteReg(0x3022, reg04);
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
LOCAL uint32 NT99250_set_preview_mode(uint32 preview_mode)
{
    SCI_TRACE_LOW("set_preview_mode: preview_mode = %d", preview_mode);
    
    s_preview_mode = preview_mode;
    
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            NT99250_set_work_mode(0);
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            NT99250_set_work_mode(1);
            break;
        }
        case DCAMERA_ENVIRONMENT_SUNNY:
        {
            NT99250_set_work_mode(0);
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

LOCAL uint32 _NT99250_GetResolutionTrimTab(uint32 param)
{
    return (uint32)s_NT99250_Resolution_Trim_Tab;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 NT99250_Identify(uint32 param)
{
#define NT99250_ID_H_VALUE  0x25
#define NT99250_ID_L_VALUE  0x01
        
    uint8 id_h_value = 0;
    uint8 id_l_value = 0;
    uint32 ret_value = 0xFF;

    id_h_value = Sensor_ReadReg(0x3000);
    SCI_TRACE_LOW("NT99250_Identify-id_h_value %d", id_h_value);
    
    id_l_value = Sensor_ReadReg(0x3001);
    SCI_TRACE_LOW("NT99250_Identify-id_l_value %d", id_l_value);

    if((NT99250_ID_H_VALUE == id_h_value) && (NT99250_ID_L_VALUE == id_l_value))
    {
            
        ret_value = 0;
        SCI_TRACE_LOW("It Is NT99250 Sensor !");    
    }

    return ret_value;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
#define CAPTURE_MAX_GAIN16                  128
#define NT99250_CAPTURE_MAXIMUM_SHUTTER     1200


#define PV_PERIOD_PIXEL_NUMS 1190
//#define PV_Dummy_Pixels  0


#define  FULL_PERIOD_PIXEL_NUMS 1922
#define  g_Capture_Dummy_Pixels 0
#define  FULL_EXPOSURE_LIMITATION 1234
#define g_Capture_Dummy_Lines 0
#define g_Capture_PCLK_Frequency 36

uint16 read_NT99250_gain(void)
{
    uint16  temp_reg;
   uint16 sensor_gain;
   
  temp_reg= (Sensor_ReadReg(0x301C) << 8) + Sensor_ReadReg(0x301D);  
   
   
    sensor_gain=(16+(temp_reg&0x0F));
   if(temp_reg&0x10)
      sensor_gain<<=1;
   if(temp_reg&0x20)
      sensor_gain<<=1;   
   if(temp_reg&0x40)
      sensor_gain<<=1;     
   if(temp_reg&0x80)
      sensor_gain<<=1;
      
   return sensor_gain;
}  /* read_OV2650_gain */

LOCAL uint16 NT99250_read_shutter(void)
{
    uint8 temp_reg1, temp_reg2;
    uint16 temp_reg;                // extra_exp_lines;

    temp_reg1 = Sensor_ReadReg(0x3013);
    temp_reg2 = Sensor_ReadReg(0x3012);
    temp_reg = (temp_reg1 & 0xFF) | (temp_reg2 << 8);
    
    return temp_reg ;
}    /* NT99250_read_shutter */

LOCAL void NT99250_write_shutter(uint16 shutter)
{
    // set extra exposure line
    Sensor_WriteReg(0x3012, (shutter >> 8) & 0xff);
    Sensor_WriteReg(0x3013, shutter & 0xFF); // EXVTS[b15~b8]
    Sensor_WriteReg(0x3060,0x01);

}    /* NT99250_write_shutter */

LOCAL void NT99250_write_gain(uint16 gain)
{
    Sensor_WriteReg(0x301c,(gain >>8) & 0xff);
    Sensor_WriteReg(0x301d,(gain & 0xff));
    Sensor_WriteReg(0x3060,0x01);
}  /* NT99250_write_gain */

LOCAL void NT99250_ExposureTrans(SENSOR_MODE_E snapshot_mode)
{
    uint16 sensor_gain , PVExposureLine = 0 , CPExposureLine = 0;
    uint16 temp_reg, temp1, maxExposureLine;
//    uint8 temp_reg1=0, temp_reg2=0;

    temp1 = Sensor_ReadReg(0x3201);
    Sensor_WriteReg(0x3201, temp1&0xCF); //close AE,AWB
    
    reg3012 = Sensor_ReadReg(0x3012);
    reg3013 = Sensor_ReadReg(0x3013);
    reg301d = Sensor_ReadReg(0x301d);
    
    temp_reg = (Sensor_ReadReg(0x301c) << 8) | Sensor_ReadReg(0x301d);
    sensor_gain = 1;            
    
    if (temp_reg&0x10)
        sensor_gain<<=1;
    
    if (temp_reg&0x20)
        sensor_gain<<=1;
    
    if (temp_reg&0x40)
        sensor_gain<<=1;
    #if 0
    if (temp_reg&0x80)
        sensor_gain<<=1;
    #endif
    maxExposureLine = ((Sensor_ReadReg(0x32c1)&0x0f)<< 8 )| Sensor_ReadReg(0x32c2);  
    PVExposureLine = NT99250_read_shutter();
  
    // OutDoor Setting
    if(temp_reg < 0x02) 
    {
       //Sensor_WriteReg(0x3331,0x08);
    }
    
    // Low Light Setting
    else if((maxExposureLine == PVExposureLine)&&(Sensor_ReadReg(0x301d)>= 0x18))
    {                                                                                                                                                   
       //Sensor_WriteReg(0x3301,0x50);
    }

    Sensor_SetMode(snapshot_mode);
    
    NT99250_write_gain(temp_reg&0xFF0F);              
    
    CPExposureLine = (PVExposureLine*sensor_gain);  
    NT99250_write_shutter(CPExposureLine);
    
}  /* NT99250_read_NT99250_gain */


LOCAL uint32 NT99250_BeforeSnapshot(uint32 param)
{
    uint32 rtn=SCI_SUCCESS;
    if(SENSOR_MODE_PREVIEW_ONE>=param)
    {
        s_change_mode=SENSOR_MODE_PREVIEW_ONE;
        return rtn;
    }

    s_change_mode=param;

    NT99250_ExposureTrans(param);

    return rtn;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99250_sharpness_tab[][2]=
{

    {{0x3301, 0x08},{0xffff, 0xff}},
    {{0x3301, 0x18},{0xffff, 0xff}},
    {{0x3301, 0x28},{0xffff, 0xff}},
    {{0x3301, 0x50},{0xffff, 0xff}},
    {{0x3301, 0x78},{0xffff, 0xff}}
};

LOCAL uint32 NT99250_set_sharpness(uint32 level)
{
#if 0
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99250_sharpness_tab[level];
    
    SCI_ASSERT(level <= 5 );   /*assert to do*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);  /*assert to do*/
    
    for(i = 0; i < 2/*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/ ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_TRACE_LOW("set_sharpness: level = %d", level);
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
__align(4) const SENSOR_REG_T NT99250_saturation_tab[][4]=
{
    // level 0
    { 
        {0x32f1, 0x05},{0x32f3, 0x40},{0xFFFF, 0xFF},{0xFFFF, 0xFF}
    },
    //level 1
    { 
        {0x32f1, 0x05},{0x32f3, 0x60},{0xFFFF, 0xFF},{0xFFFF, 0xFF}
 
    },
    //level 2
    { 
        {0x32f1, 0x05},{0x32f3, 0x80},{0xFFFF, 0xFF},{0xFFFF, 0xFF}

    },
    //level 3
    { 
        {0x32f1, 0x05},{0x32f3, 0xA0},{0xFFFF, 0xFF},{0xFFFF, 0xFF}
 
    },
    //level 4
    { 
        {0x32f1, 0x05},{0x32f3, 0xC0},{0xFFFF, 0xFF},{0xFFFF, 0xFF}
 
    }
};
LOCAL uint32 NT99250_set_saturation(uint32 level)
{
#if 0
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99250_saturation_tab[level];
    
    SCI_ASSERT(level <= 8 );   /*assert to do*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);  /*assert to do*/
    
    for(i = 0; i < 4/*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/ ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_TRACE_LOW("set_saturation: level = %d", level);
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
__align(4) const SENSOR_REG_T NT99250_image_effect_tab[][4]=
{
    // effect normal
    {
        {0x32f1, 0x00},{0xffff, 0xff},{0xffff,0xff},{0xffff,0xff}

    },
    //effect BLACKWHITE
    {
        {0x32f1, 0x01},{0xffff, 0xff},{0xffff,0xff},{0xffff,0xff}

    },
    // effect RED
    {
        {0x32f1, 0x05},{0x32f4, 0x80},{0x32f5, 0xf0},{0xffff,0xff}

    },
        
    // effect GREEN
    {
        {0x32f1, 0x05},{0x32f4, 0x60},{0x32f5, 0x20},{0xffff,0xff} 

    },
        
    // effect  BLUE
    {
        {0x32f1, 0x05},{0x32f4, 0xf0},{0x32f5, 0x80},{0xffff,0xff}

    },
    //effect YELLOW
    {
         {0x32f1, 0x05},{0x32f4,0x20},{0x32f5,0xa0},{0xffff,0xff}
    },
    // effect NEGATIVE
    {
        {0x32f1, 0x03},{0xffff, 0xff},{0xffff,0xff},{0xffff,0xff}

    },
    // effect CANVAS ANTIQUE
    {
        {0x32f1, 0x02},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff}
    }
};
LOCAL uint32 NT99250_set_image_effect(uint32 effect_type)
{
    uint16          i = 0;            //reg_value
    SENSOR_REG_T*   sensor_reg_ptr = (SENSOR_REG_T*)NT99250_image_effect_tab[effect_type];

    if(effect_type >= DCAMERA_EFFECT_MAX)   
        return SENSOR_OP_PARAM_ERR;
    
    s_image_effect = effect_type;
    
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert to do*/
    
    for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {   
        {
            Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
        }
    }
    SCI_TRACE_LOW("set_image_effect: effect_type = %d", effect_type);
    return 0;
}


/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/

__align(4) const SENSOR_REG_T NT99250_brightness_tab[][5]=
{
    {//level 1
        {0x32f2, 0x50},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff} ,{0xffff,0xff}             
    },
    {//level 2
        {0x32f2, 0x60},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff} ,{0xffff,0xff} 
    },
    {//level 3
         {0x32f2, 0x70},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff} ,{0xffff,0xff} 
    },
    {//level 4
         {0x32f2, 0x80},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff} ,{0xffff,0xff} 
    },
    {//level 5
         {0x32f2, 0x90},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff} ,{0xffff,0xff} 
    },
    {//level 6
         {0x32f2, 0xA0},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff} ,{0xffff,0xff} 
    },                             
    {//level 7
         {0x32f2, 0xB0},{0xffff,0xff},{0xffff,0xff},{0xffff,0xff} ,{0xffff,0xff}    
    }        
};

LOCAL uint32 NT99250_set_brightness(uint32 level)
{
    uint16          i = 0;                    //reg_value
    SENSOR_REG_T*   sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)NT99250_brightness_tab[level];

    if(level >= 7)
        return SENSOR_OP_PARAM_ERR;
    
    for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        {
            Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
        }
    }
    SCI_Sleep(100); 
    SCI_TRACE_LOW("set_brightness: level = %d", level);
    return 0;
}

/******************************************************************************/
// Description: set contrast
// Global resource dependence: 
// Author:
// Note:
//      level must smaller than 9
/******************************************************************************/
__align(4) const SENSOR_REG_T NT99250_contrast_tab[][16]=
{
    {//level 1
        {0x3270,0x00},
        {0x3271,0x1F},
        {0x3272,0x2D},
        {0x3273,0x3F},
        {0x3274,0x4E},
        {0x3275,0x5A},
        {0x3276,0x6E},
        {0x3277,0x7F},
        {0x3278,0x8E},
        {0x3279,0x9C},
        {0x327A,0xB4},
        {0x327B,0xC9},
        {0x327C,0xDD},
        {0x327D,0xEE},
        {0x327E,0xFF},
        {0xFFFF,0xFF}   
    },
    {//level 2 1.8
        {0x3270,0x00},
        {0x3271,0x19},
        {0x3272,0x25},
        {0x3273,0x36},
        {0x3274,0x44},
        {0x3275,0x50},
        {0x3276,0x64},
        {0x3277,0x76},
        {0x3278,0x85},
        {0x3279,0x94},
        {0x327A,0xAD},
        {0x327B,0xC4},
        {0x327C,0xD9},
        {0x327D,0xED},
        {0x327E,0xFF},
        {0xFFFF,0xFF} 
    },
    {//level 3 //1.6
        {0x3270,0x00},
        {0x3271,0x12},
        {0x3272,0x1D},
        {0x3273,0x2D},
        {0x3274,0x3A},
        {0x3275,0x45},
        {0x3276,0x59},
        {0x3277,0x6B},
        {0x3278,0x7B},
        {0x3279,0x8A},
        {0x327A,0xA5},
        {0x327B,0xBE},
        {0x327C,0xD5},
        {0x327D,0xEB},
        {0x327E,0xFF},
        {0xFFFF,0xFF}
    },
    {//level 4 Gamma3
        {0x3270,0x00},
        {0x3271,0x0B},
        {0x3272,0x16},
        {0x3273,0x2B},
        {0x3274,0x3F},
        {0x3275,0x51},
        {0x3276,0x72},
        {0x3277,0x8F},
        {0x3278,0xA7},
        {0x3279,0xBC},
        {0x327A,0xDC},
        {0x327B,0xF0},
        {0x327C,0xFA},
        {0x327D,0xFE},
        {0x327E,0xFF},
        {0xFFFF,0xFF} 
    },
    {//level 5
        {0x3270,0x00},
        {0x3271,0x0C},
        {0x3272,0x18},
        {0x3273,0x32},
        {0x3274,0x44},
        {0x3275,0x54},
        {0x3276,0x70},
        {0x3277,0x88},
        {0x3278,0x9D},
        {0x3279,0xB0},
        {0x327A,0xCF},
        {0x327B,0xE2},
        {0x327C,0xEF},
        {0x327D,0xF7},
        {0x327E,0xFF},
        {0xFFFF,0xFF} 
    },
    {//level 6
        {0x3270,0x00},
        {0x3271,0x05},
        {0x3272,0x11},
        {0x3273,0x28},
        {0x3274,0x43},
        {0x3275,0x5C},
        {0x3276,0x7E},
        {0x3277,0x9B},
        {0x3278,0xB3},
        {0x3279,0xC6},
        {0x327A,0xD9},
        {0x327B,0xE7},
        {0x327C,0xF6},
        {0x327D,0xFF},
        {0x327E,0xFF},
        {0xFFFF,0xFF} 
    },
    {//level 7
        {0x3270,0x00},
        {0x3271,0x05},
        {0x3272,0x0E},
        {0x3273,0x1E},
        {0x3274,0x3A},
        {0x3275,0x50},
        {0x3276,0x73},
        {0x3277,0x95},
        {0x3278,0xAD},
        {0x3279,0xC7},
        {0x327A,0xDE},
        {0x327B,0xEB},
        {0x327C,0xF9},
        {0x327D,0xFF},
        {0x327E,0xFF},
        {0xFFFF,0xFF}
    }
};
LOCAL uint32 NT99250_set_contrast(uint32 level)
{
    uint16          i = 0;            //reg_value
    SENSOR_REG_T*   sensor_reg_ptr;
    sensor_reg_ptr = (SENSOR_REG_T*)NT99250_contrast_tab[level];
    
    if(level >= 7)
        return SENSOR_OP_PARAM_ERR;

    for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {       
        {
            Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
        }
    }
    SCI_Sleep(20);
    SCI_TRACE_LOW("set_contrast: level = %d", level);
    return 0;
}



/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/


__align(4) const SENSOR_REG_T NT99250_work_mode_tab[][2]=
{
    {//normal 
        {0x301e, 0x00},     
        {0xffff, 0xff}
    },

    {//night 
        {0x301e, 0x04},     
        {0xffff, 0xff}
    }
};
/*lint -restore */

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/
LOCAL uint32 NT99250_set_work_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)NT99250_work_mode_tab[mode];
    
    if(mode > 1)
        return SENSOR_OP_PARAM_ERR;


    work_mode = mode;

    for(i = 0; (0xFFFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_TRACE_LOW("Sensor I2C read: 0x301e = %04x", Sensor_ReadReg(0x301e));   // for NT99250 debug
    SCI_TRACE_LOW("set_work_mode: mode = %d", mode);
    return 0;
}

LOCAL uint32 NT99250_after_snapshot(uint32 param)
{
   if(SENSOR_MODE_PREVIEW_ONE<s_change_mode)
    {
       Sensor_WriteReg(0x3012,reg3012);
       Sensor_WriteReg(0x3013,reg3013);
       Sensor_WriteReg(0x301d,reg301d);
       Sensor_WriteReg(0x3060,0x01);
       s_change_mode=SENSOR_MODE_PREVIEW_ONE;
    }

    return SCI_SUCCESS;
}

LOCAL uint32 NT99250_chang_image_format(uint32 param)
{
    SENSOR_REG_TAB_INFO_T st_yuv422_reg_table_info = { ADDR_AND_LEN_OF_ARRAY(NT99250_YUV_COMMON),0,0,0,0};
    uint32 ret_val = SCI_ERROR;

    switch(param){
    case SENSOR_IMAGE_FORMAT_YUV422:
        ret_val = Sensor_SendRegTabToSensor(&st_yuv422_reg_table_info);
        break;

    case SENSOR_IMAGE_FORMAT_JPEG:
        ret_val = SCI_ERROR;//Sensor_SendRegTabToSensor(&st_jpeg_reg_table_info);
        break;

    default:
//        SCI_PASSERT(0, ("NT99250 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param)); /*assert to do*/
        break;
    }

    return ret_val;
}

LOCAL uint32 NT99250_check_image_format_support(uint32 param) 
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
//        SCI_PASSERT(0, ("NT99250 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param));  /*assert to do*/
        break;
    }

    return ret_val;
}

/*lint -restore */ 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

