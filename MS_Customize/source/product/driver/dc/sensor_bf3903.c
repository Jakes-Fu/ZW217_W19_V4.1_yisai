/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_BF3903.c                                           *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor BF3903. 
 **                                                          
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **2011.07.06     january             BYD_BME
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
//#include "i2c_drv.h"
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
/*lint -save -e785 */
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
#define BF3903_I2C_ADDR_W       0xDC
#define BF3903_I2C_ADDR_R       0xDD

/**---------------------------------------------------------------------------*
     BF3903 SUPPORT 5 SERIAL MODE:
     CCIR601_8BIT, SPI, CCIR656_2BIT, CCIR656_4BIT
 **---------------------------------------------------------------------------*/

#define BF3903_OUTPUT_MODE_CCIR601_8BIT
//#define BF3903_OUTPUT_MODE_SPI
//#define BF3903_OUTPUT_MODE_CCIR656_2BIT
//#define BF3903_OUTPUT_MODE_CCIR656_4BIT

  
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_BF3903_ae_awb_enable(uint32 ae_enable, uint32 awb_enable);
LOCAL uint32 set_BF3903_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 BF3903_Identify(uint32 param);
LOCAL uint32 BF3903_BeforeSnapshot(uint32 param);
LOCAL uint32 BF3903_After_Snapshot(uint32 param);

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
LOCAL uint32 BF3903_set_work_mode(uint32 mode);

LOCAL uint32 set_BF3903_ev(uint32 level);
LOCAL uint32 set_BF3903_awb(uint32 mode);
LOCAL uint32 set_BF3903_anti_flicker(uint32 mode);
LOCAL uint32 set_BF3903_video_mode(uint32 mode);

LOCAL uint32 _BF3903_Power_On(uint32 power_on);

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/


__align(4) const SENSOR_REG_T BF3903_YUV_Setting[]=
{
    //BF3903 initial
//  {0x12 , 0x80},
//  {0xff , 0xff},
    {0x15 , 0x08},
//  {0x09 , 0x00},
    {0x09 , 0x02},
    {0x13 , 0x00},
    {0x01 , 0x11},
    {0x02 , 0x22},
    {0x13 , 0x07},
    {0x2f , 0x86},
    {0x35 , 0x3e},
    {0x65 , 0x38},
    {0x66 , 0x38},
    {0x36 , 0x45},
    {0x6e , 0x20}, 
    {0x72 , 0x2f},
    {0x73 , 0x2f},
    {0x74 , 0x28},
    {0x75 , 0x25},
    {0x69 , 0x1a},
    {0x76 , 0x90},
    {0x79 , 0x2d},
    {0x7a , 0x28},
    {0x7e , 0x1a},
    {0x82 , 0x30},
    {0x83 , 0x55},        
    {0x84 , 0x50},
    {0x85 , 0x6c},
    {0x86 , 0x90},
    {0x89 , 0x92},
    {0x3b , 0x60},
    {0x3c , 0x30},
    {0x39 , 0x8a},
    {0x3f , 0x8a},       
    {0x24 , 0x80},
    {0x4d , 0x0f},
    {0X40 , 0X68},
    {0X41 , 0X60},
    {0X42 , 0X54},
    {0X43 , 0X46},
    {0X44 , 0X3e},
    {0X45 , 0X39},
    {0X46 , 0X35},         
    {0X47 , 0X31},
    {0X48 , 0X2e},
    {0X49 , 0X2b},
    {0X4b , 0X29},
    {0X4c , 0X27},
    {0X4e , 0X24},
    {0X4f , 0X22},
    {0X50 , 0X20},         
    {0x5a , 0x16},
    {0x51 , 0x1C},
    {0x52 , 0x1b},
    {0x53 , 0x80},
    {0x54 , 0x72},
    {0x57 , 0x92},
    {0x58 , 0x41},
    {0x5a , 0x96},         
    {0x51 , 0x0e},
    {0x52 , 0x0b},
    {0x53 , 0x59},
    {0x54 , 0x45},
    {0x57 , 0x5f},
    {0x58 , 0x1e},
    {0x5b , 0xc2},
    {0x5c , 0x2a},         
    {0xb0 , 0xe0},
    {0xb1 , 0xc8},
    {0xb2 , 0xb8},
    {0xb3 , 0x4c},
    {0xb4 , 0x60},
    {0x6a , 0x81},
    {0x23 , 0x66},
    {0xa2 , 0x0b},         
    {0xa3 , 0x26},
    {0xa4 , 0x09},
    {0xa5 , 0x23},
    {0xa6 , 0x06},
    {0xa7 , 0x99},
    {0xa8 , 0x1a},
    {0xae , 0x57},
    {0xa9 , 0x50},
    {0xaa , 0x50},
    {0xab , 0x18},
    {0xd1 , 0x00},
    {0xd2 , 0x78},
    {0x12 , 0x00},
    {0x8a , 0x4c},
    
#ifdef BF3903_OUTPUT_MODE_SPI
    {0x1b , 0x06},
    {0x11 , 0x30},
    {0x6b , 0x01},
    {0x62 , 0x81},
    {0x15 , 0x10},
    {0x08 , 0x80},

    {0xda , 0x23},
    {0xdb , 0x7f},
    {0xdc , 0x00},
    {0xdd , 0x7a},
    {0x6b , 0x01},
    {0x4a , 0x0e},

    {0x0a , 0x21},
    {0x10 , 0x21},
    {0x3d , 0x33},

    {0x17 , 0x00},
    {0x18 , 0x3c},
    {0x19 , 0x00},
    {0x1a , 0x50},
    {0x03 , 0x00},
    {0x8a , 0x3b},
#elif defined(BF3903_OUTPUT_MODE_CCIR656_2BIT)
    {0x62 , 0x02},
    {0x6b , 0x42},
    {0x1b , 0x06},
    {0x11 , 0x10},
    {0x15 , 0x10},
    {0x08 , 0x80},
#elif defined(BF3903_OUTPUT_MODE_CCIR656_4BIT)
    {0x62 , 0x03},
    {0x6b , 0x42},
    {0x1b , 0x80},//0x80},
    {0x11 , 0xb0},//0x90},
    {0x15 , 0x10},
    {0x08 , 0x80},
#else //BF3903_OUTPUT_MODE_CCIR601_8BIT
    {0x62 , 0x00},
    {0x6b , 0x02},
    {0x1b , 0x80},
    {0x11 , 0x80},//0x10
    {0x17 , 0x00},
    {0x18 , 0xa0},
    {0x19 , 0x00},
    {0x1a , 0x78},
    {0x03 , 0x00},
#endif  
    //mirror and flip
    {0x1e , 0x30},
    
};


LOCAL SENSOR_REG_TAB_INFO_T s_BF3903_resolution_Tab_YUV[]=
{   
    // COMMON INIT
#ifdef BF3903_OUTPUT_MODE_SPI
    {ADDR_AND_LEN_OF_ARRAY(BF3903_YUV_Setting),240,320,24,SENSOR_IMAGE_FORMAT_YUV422},
#else
    {ADDR_AND_LEN_OF_ARRAY(BF3903_YUV_Setting),640,480,24,SENSOR_IMAGE_FORMAT_YUV422},
#endif

    // YUV422 PREVIEW 1
#ifdef BF3903_OUTPUT_MODE_SPI
    { PNULL, 0, 240, 320, 24, SENSOR_IMAGE_FORMAT_YUV422},
#else
    { PNULL, 0, 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
#endif
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    
    // YUV422 PREVIEW 2 
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0}

};


LOCAL SENSOR_IOCTL_FUNC_TAB_T s_BF3903_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    _BF3903_Power_On,
    PNULL,
    BF3903_Identify,

    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

    // External
    set_BF3903_ae_enable,
    set_hmirror_enable,
    set_vmirror_enable,

    set_brightness,
    set_contrast,
    set_sharpness,
    set_saturation,

    set_preview_mode,   
    set_image_effect,

    PNULL,                //BF3903_BeforeSnapshot,
    PNULL,             //BF3903_After_Snapshot,
    PNULL,

    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate,
    PNULL,
    PNULL,
    set_BF3903_awb,
    PNULL,
    PNULL,
    set_BF3903_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_BF3903_anti_flicker,
    set_BF3903_video_mode,
    PNULL,
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_BF3903_yuv_info =
{
    BF3903_I2C_ADDR_W,              // salve i2c write address
    BF3903_I2C_ADDR_R,              // salve i2c read address
    
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
    
    SENSOR_LOW_PULSE_RESET,         // reset pulse level
    20,                             // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code
    0xFC, 0x38,                     // supply two code to identify sensor.
    0xFD, 0x03,                     // for Example: index = 0-> Device id, index = 1 -> version id  
                                    
    SENSOR_AVDD_2800MV,             // voltage of avdd  

#ifdef BF3903_OUTPUT_MODE_SPI
    240,                            // max width of source image
    320,                            // max height of source image
#else
    640,                            // max width of source image
    480,                            // max height of source image
#endif  
    "BF3903",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_BF3903_resolution_Tab_YUV,    // point to resolution table information structure
    &s_BF3903_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor  
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1800MV,                      // dvdd
    4,                     // skip frame num before preview 
    3,                      // skip frame num before capture
    0,                      // deci frame num during preview    
    2,                       // deci frame num during video preview
    0,
    0,
    0,
    0,
#ifdef BF3903_OUTPUT_MODE_SPI
    SENSOR_OUTPUT_MODE_SPI
#elif defined(BF3903_OUTPUT_MODE_CCIR656_2BIT)
    SENSOR_OUTPUT_MODE_CCIR656_2BIT
#elif defined(BF3903_OUTPUT_MODE_CCIR656_4BIT)
    SENSOR_OUTPUT_MODE_CCIR656_4BIT
#else
    SENSOR_OUTPUT_MODE_CCIR601_8BIT  
#endif
};


/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void BF3903_WriteReg( uint8  subaddr, uint8 data )
{
    Sensor_WriteReg(subaddr, data);
    //SCI_TRACE_LOW:"SENSOR: BF3903_WriteReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_413_112_2_18_0_30_33_710,(uint8*)"dd", subaddr, data);
}

LOCAL uint8 BF3903_ReadReg( uint8  subaddr)
{
    uint8 value = 0;
    
    value =Sensor_ReadReg( subaddr);
    //SCI_TRACE_LOW:"BF3903_ReadReg: subaddr:%x,value:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_421_112_2_18_0_30_33_711,(uint8*)"dd",subaddr,value);
    
    return value;
}


LOCAL uint32 BF3903_Identify(uint32 param)
{
    uint32 i;
    uint32 nLoop;
    uint8 ret;
    uint32 err_cnt = 0;
    uint8 reg[2] = {0xfC, 0xfD};
    uint8 value[2] = {0x38, 0x03};

    //SCI_TRACE_LOW:"BF3903_Identify: start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_436_112_2_18_0_30_33_712,(uint8*)"");

    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = BF3903_ReadReg(reg[i]);
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
                while(nLoop--)
                continue;
            }
        }
        err_cnt = 0;
        i++;
    }

    //SCI_TRACE_LOW:"BF3903_Identify: it is BF3903"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_460_112_2_18_0_30_33_713,(uint8*)"");
    
    return (uint32)SCI_SUCCESS;
}

LOCAL uint32 set_BF3903_ae_enable(uint32 enable)
{
    unsigned char ae_value;

    ae_value=BF3903_ReadReg(0x13);

    if(0x00==enable)
    {
        ae_value&=0xfa;
        BF3903_WriteReg(0x13,ae_value);
    }
    else if(0x01==enable)
    {
        ae_value|=0x05;
        BF3903_WriteReg(0x13,ae_value);
    }

    //SCI_TRACE_LOW:"SENSOR: set_ae_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_482_112_2_18_0_30_33_714,(uint8*)"d", enable);

    return 0;
}


LOCAL uint32 set_BF3903_ae_awb_enable(uint32 ae_enable, uint32 awb_enable)
{
    unsigned char ae_value;

    ae_value=BF3903_ReadReg(0x13);

    if(0x00==ae_enable)
    {
        ae_value&=0xfa;
    }
    else if(0x01==ae_enable)
    {
        ae_value|=0x05;
    }

    if(0x00==awb_enable)
    {
        ae_value&=0xfd;
    }
    else if(0x01==awb_enable)
    {
        ae_value|=0x02;
    }        

    BF3903_WriteReg(0x13,ae_value);

    //SCI_TRACE_LOW:"set_BF3903_ae_awb_enable: set_ae_awb_enable: ae=%d awb=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_514_112_2_18_0_30_34_715,(uint8*)"dd", ae_enable, awb_enable);

    return 0;
}


void set_BF3903_shutter(uint16 shutter)
{            
    BF3903_WriteReg(0x8c,(shutter&0xFF00)>>8);
    BF3903_WriteReg(0x8D,shutter&0xFF);
}   /* set_BF3903_shutter */


uint16 get_BF3903_shutter(void)
{
    uint8 temp_reg1, temp_reg2;
    uint16 shutter;

    temp_reg1=BF3903_ReadReg(0x8c);
    //Delayms(10); //modify by ancky 
    SCI_Sleep(10);
    temp_reg2=BF3903_ReadReg(0x8d);
    
    shutter=((temp_reg1&0xff)<<8)|(temp_reg2);

    return shutter;
}   /* get_BF3903_shutter */


LOCAL uint32 set_hmirror_enable(uint32 enable)
{
 
    
    return 0;
}


LOCAL uint32 set_vmirror_enable(uint32 enable)
{

    
    return 0;
}


__align(4) const SENSOR_REG_T BF3903_ev_tab[][2]=
{
    {{0x55, 0xd8}, {0xff,0xff}},//-3
    {{0x55, 0xb8}, {0xff,0xff}},//-2
    {{0x55, 0x98}, {0xff,0xff}},//-1
    {{0x55, 0x00}, {0xff,0xff}},//0
    {{0x55, 0x18}, {0xff,0xff}},//+1
    {{0x55, 0x38}, {0xff,0xff}},//+2
    {{0x55, 0x58}, {0xff,0xff}},//+3
};

    
LOCAL uint32 set_BF3903_ev(uint32 level)
{
    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3903_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 
    SCI_ASSERT(level < 7);       /*assert verified*/ 

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    
    //SCI_TRACE_LOW:"set_BF3903_ev: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_584_112_2_18_0_30_34_716,(uint8*)"d", level);

    return 0;
}


__align(4) const SENSOR_REG_T BF3903_anti_flicker_mode_tab[][3]=
{
     //50Hz:
    { 
        {0x80,0xd6},
    #ifdef BF3903_OUTPUT_MODE_SPI
        {0x8a,0x3b},
    #else
        {0x8a,0x4c},
    #endif
        {0xff,0xff}
    },
    
     //60Hz:
    {   
        {0x80,0xd4},
    #ifdef BF3903_OUTPUT_MODE_SPI
        {0x8b,0x31},
    #else
        {0x8b,0x3f},
    #endif
        {0xff,0xff}
    },

};

/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_BF3903_anti_flicker(uint32 mode)
{//24m->65.6 us 12m->131us
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3903_anti_flicker_mode_tab[mode];
    SCI_ASSERT(mode < 2);   /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"set_BF3903_anti_flicker: set_banding_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_633_112_2_18_0_30_34_717,(uint8*)"d", mode);
    
    return 0;
}


/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//       
/******************************************************************************/
__align(4) const SENSOR_REG_T BF3903_video_mode_nor_tab[][6]=
{
    // normal mode 14fps
    {
       {0x92,0x00},{0x93,0x00},{0x2a,0x00},{0x2b,0x00},{0x89,0x5d},{0xff, 0xff}
    },    
    //video mode 10fps
    {
       {0x92,0x00},{0x93,0x00},{0x2a,0x00},{0x2b,0x00},{0x89,0x5d},{0xff, 0xff}      
    },
    // UPCC  mode 14fps
    {
       {0x92,0x00},{0x93,0x00},{0x2a,0x00},{0x2b,0x00},{0x89,0x5d}, {0xff, 0xff}      
    }  
};    


LOCAL uint32 set_BF3903_video_mode(uint32 mode)
{
   
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3903_video_mode_nor_tab[mode];

 
    SCI_ASSERT(mode<=DCAMERA_MODE_MAX);   /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"SENSOR: set_BF3903_video_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_675_112_2_18_0_30_34_718,(uint8*)"d", mode);
    return 0;
}


/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T BF3903_awb_tab[][5]=

{
     //AUTO:
    {
    {0x13, 0x07},
    {0x01, 0x15},
    {0x02, 0x24},
    {0x6a, 0x81},
    {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
    {0x13, 0x05},
    {0x01, 0x1f},
    {0x02, 0x15},
    {0x6a, 0x01},
    {0xff, 0xff}        
    },
     //U30:
    {
    {0x13, 0x05},
    {0x01, 0x12},
    {0x02, 0x19},
    {0x6a, 0x81},
    {0xff, 0xff}  
    },
    //CWF:
    {
    {0x13, 0x05},
    {0x01, 0x1b},
    {0x02, 0x19},
    {0x6a, 0x81},
    {0xff, 0xff}            
    },  
    //FLUORESCENT:
    {
    {0x13, 0x05},
    {0x01, 0x1a},
    {0x02, 0x1e},
    {0x6a, 0x01},
    {0xff, 0xff}          
    },    
    //SUN:
    {
    {0x13, 0x05},
    {0x01, 0x13},
    {0x02, 0x26},
    {0x6a, 0x01},
    {0xff, 0xff}           
    },
    //CLOUD
    {
    {0x13, 0x05},
    {0x01, 0x10},
    {0x02, 0x28},
    {0x6a, 0x01},
    {0xff, 0xff}            
    },  
};


LOCAL uint32 set_BF3903_awb(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3903_awb_tab[mode];

    SCI_ASSERT(mode < 7);   /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"set_BF3903_awb: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_760_112_2_18_0_30_34_719,(uint8*)"d", mode);
    
    return 0;
}


/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T BF3903_brightness_tab[][3]=
{
    {{0x24, 0x48},  {0xff,0xff}, {0xff,0xff}},
    {{0x24, 0x58},  {0xff,0xff}, {0xff,0xff}},
    {{0x24, 0x60},  {0xff,0xff}, {0xff,0xff}},
    {{0x24, 0x78},  {0xff,0xff}, {0xff,0xff}},
    {{0x24, 0x88},  {0xff,0xff}, {0xff,0xff}},
    {{0x24, 0x98},  {0xff,0xff}, {0xff,0xff}},
    {{0x24, 0xa0},  {0xff,0xff}, {0xff,0xff}},
};


LOCAL uint32 set_brightness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3903_brightness_tab[level];

    SCI_ASSERT(level < 7);   /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"BF3903 set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_796_112_2_18_0_30_34_720,(uint8*)"d", level);
    
    return 0;
}


__align(4) const SENSOR_REG_T BF3903_contrast_tab[][2]=
{
    {{0x56, 0x10}, {0xff,0xff}},
    {{0x56, 0x20}, {0xff,0xff}},
    {{0x56, 0x30}, {0xff,0xff}},
    {{0x56, 0x40}, {0xff,0xff}},
    {{0x56, 0x50}, {0xff,0xff}},
    {{0x56, 0x60}, {0xff,0xff}},
    {{0x56, 0x70}, {0xff,0xff}},

};


LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)BF3903_contrast_tab[level];

    SCI_ASSERT(level < 7 );   /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(20);
    //SCI_TRACE_LOW:"BF3903: set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_830_112_2_18_0_30_34_721,(uint8*)"d", level);

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
    //SCI_TRACE_LOW:"BF3903 set_preview_mode: preview_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_858_112_2_18_0_30_34_722,(uint8*)"d", preview_mode);
        
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            BF3903_set_work_mode(0);
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            BF3903_set_work_mode(1);
            break;
        }
        case DCAMERA_ENVIRONMENT_SUNNY:
        {
            BF3903_set_work_mode(0);
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
__align(4) const SENSOR_REG_T BF3903_image_effect_tab[][7]=
{
    {
        {0x70, 0x0f},
        {0xb4, 0x60},
        {0x98, 0x02},
        {0x69, 0x1a},
        {0x67, 0x80}, 
        {0x68 ,0x80}, 
        {0xff,0xff}
    },//effect Normal
    {
        {0x70, 0x0f},
        {0xb4, 0x00},
        {0x98, 0x82},
        {0x69, 0x20},
        {0x67, 0x80}, 
        {0x68 ,0x80}, 
        {0xff,0xff}
    },//effect BLACKWHITE
    {
        {0x70, 0x0f},
        {0xb4, 0x00},
        {0x98, 0x82},
        {0x69, 0x20},
        {0x67, 0x80}, 
        {0x68 ,0xc0}, 
        {0xff,0xff}
    },// effect RED
    {
        {0x70, 0x0f},
        {0xb4, 0x00},
        {0x98, 0x82},
        {0x69, 0x20},
        {0x67, 0x40}, 
        {0x68 ,0x40}, 
        {0xff,0xff}
    },// effect GREEN
    {
        {0x70, 0x0f},
        {0xb4, 0x00},
        {0x98, 0x82},
        {0x69, 0x20},   
        {0x67, 0xc0}, 
        {0x68 ,0x80}, 
        {0xff,0xff}
    },// effect  BLUE
    {
        {0x70, 0x0f},
        {0xb4, 0x00},
        {0x98, 0x82},
        {0x69, 0x20},
        {0x67, 0x40}, 
        {0x68 ,0x80}, 
        {0xff,0xff}
    },//effect ANTIQUE
    {
        {0x70, 0x0f},
        {0xb4, 0x60},
        {0x98, 0x02},
        {0x69, 0x40},
        {0x67, 0x80}, 
        {0x68 ,0x80}, 
        {0xff,0xff}
    },// effect NEGATIVE
    {
        {0x70, 0x0f},
        {0xb4, 0x00},
        {0x98, 0x82},
        {0x69, 0x20},
        {0x67, 0x60}, 
        {0x68 ,0x98}, 
        {0xff,0xff}
    },// effect  sepia
};

    
LOCAL uint32 set_image_effect(uint32 effect_type)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3903_image_effect_tab[effect_type];

    
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"BF3903 set_image_effect: effect_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_984_112_2_18_0_30_34_723,(uint8*)"d", effect_type);
    
    return 0;
}


LOCAL uint32 BF3903_After_Snapshot(uint32 param)
{
    
    BF3903_WriteReg(0x13, 0x07);
    BF3903_WriteReg(0x11, 0x80);
    

    SCI_Sleep(400);

    return 0;
    
}


LOCAL uint32 BF3903_BeforeSnapshot(uint32 param)
{
    uint16 shutter;

    BF3903_WriteReg(0x13,0x02);

    shutter=get_BF3903_shutter();

    BF3903_WriteReg(0x11, 0x90); 

    shutter=shutter>>1;
    if(shutter<1)
    shutter=1;

    //write shutter
    set_BF3903_shutter(shutter);
    SCI_Sleep(200);     //400   

    return 0;
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
    return 0;
    
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
//      mode 0:normal;   1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T BF3903_mode_tab[][3]=
{
    {{0X89,0X5d}, {0xff,0xff}, {0xff,0xff}},// normal   min 9fps
    {{0X89,0Xa5}, {0xff,0xff}, {0xff,0xff}},// night  // min 5 fps
};

LOCAL uint32 BF3903_set_work_mode(uint32 mode)
{
    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3903_mode_tab[mode];

    SCI_ASSERT(mode <= 1);    /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3903_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"BF3903 set_work_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_1095_112_2_18_0_30_35_724,(uint8*)"d", mode);
    return 0;
}

LOCAL uint32 _BF3903_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E       dvdd_val=g_BF3903_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E       avdd_val=g_BF3903_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E       iovdd_val=g_BF3903_yuv_info.iovdd_val;  
    BOOLEAN                 power_down=(BOOLEAN )g_BF3903_yuv_info.power_down_level;        
//    BOOLEAN                 reset_level=g_BF3903_yuv_info.reset_pulse_level;
//    uint32                  reset_width=g_BF3903_yuv_info.reset_pulse_width;        
    
    if(SCI_TRUE==power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
        
        GPIO_SetSensorPwdn(!power_down);        /*lint !e18 !e718 !e730*/

        // Open Mclk in default frequency
        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);   
        
        SCI_Sleep(20);
    }
    else
    {
        GPIO_SetSensorPwdn(power_down);

        Sensor_SetMCLK(SENSOR_DISABLE_MCLK);           

        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);        
    }
    
    //SCI_TRACE_LOW:"SENSOR: _BF3903_Power_On(1:on, 0:off): %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3903_1128_112_2_18_0_30_35_725,(uint8*)"d", power_on);
    
    return SCI_SUCCESS;
}

/*lint -restore */

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
