/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_BF3703.c                                           *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor BF3703. 
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

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define BF3703_I2C_ADDR_W       0xDC
#define BF3703_I2C_ADDR_R       0xDD

#define SENSOR_GAIN_SCALE       16
 
#define BF3703_COM11 0x3b
#define PLL_ADDR    0x11
#define AE_ENABLE 0x13

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_BF3703_ae_awb_enable(uint32 ae_enable, uint32 awb_enable);
LOCAL uint32 set_BF3703_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 BF3703_Identify(uint32 param);
LOCAL uint32 BF3703_BeforeSnapshot(uint32 param);
LOCAL uint32 BF3703_After_Snapshot(uint32 param);

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
LOCAL uint32 BF3703_set_work_mode(uint32 mode);

LOCAL uint32 set_BF3703_ev(uint32 level);
LOCAL uint32 set_BF3703_awb(uint32 mode);
LOCAL uint32 set_BF3703_anti_flicker(uint32 mode);
LOCAL uint32 set_BF3703_video_mode(uint32 mode);

#if 0 // by riyang.xie
LOCAL uint32 _BF3703_Power_On(BOOLEAN power_on);
#endif
// LOCAL uint32 s_preview_mode;

#if defined(CMOS_SENSOR_FLASH_SUPPORT)
extern uint32 GPIO_OpenSensorFlash(BOOLEAN open);
#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/


__align(4) const SENSOR_REG_T BF3703_YUV_640X480[]=
{

    {SENSOR_WRITE_DELAY, 0x20},//delay 100ms
    {SENSOR_WRITE_DELAY, 0x20},//delay 100ms
    {SENSOR_WRITE_DELAY, 0x20},//delay 100ms
    {SENSOR_WRITE_DELAY, 0x20},//delay 100ms
    {0x12 , 0x00},
    {0x12 , 0x80},
    {0xff , 0xff},
    
    {0x15, 0x00},
    {0x3a, 0x00},
    
    {0x11 , 0x80},
    {0x13 , 0x00},
    {0x01 , 0x15},
    {0x02 , 0x24},
    {0x8c , 0x02},//01 :devided by 2  02 :devided by 1
    {0x8d , 0x64},//32: devided by 2  64 :devided by 1
    {0x87 , 0x18},
    {0x13 , 0x07},  
    
    //black level  , 对上电偏绿有改善 , 如果需要请使用

    {0x05 , 0x1f}, 
    {0x06 , 0x60}, 
    {0x14 , 0x1f}, 
    {0x27 , 0x03}, 
    {0x06 , 0xe0}, 


    //lens shading
    {0x35 , 0x68}, 
    {0x65 , 0x68}, 
    {0x66 , 0x62}, 
    {0x36 , 0x05}, 
    {0x37 , 0xf6}, 
    {0x38 , 0x46}, 
    {0x9b , 0xf6}, 
    {0x9c , 0x46}, 
    {0xbc , 0x01}, 
    {0xbd , 0xf6}, 
    {0xbe , 0x46}, 

    //AE
    {0x82 , 0x14}, 
    {0x83 , 0x23}, 
    {0x9a , 0x23}, //the same as 0x83
    {0x84 , 0x1a}, 
    {0x85 , 0x20}, 
    {0x89 , 0x04}, //02 :devided by 2  04 :devided by 1
    {0x8a , 0x08}, //04: devided by 2  05 :devided by 1
    {0x86 , 0x28}, 
    {0x96 , 0xa6}, //AE speed
    {0x97 , 0x0c}, //AE speed
    {0x98 , 0x18}, //AE speed
    //AE target
    {0x24 , 0x78}, 
    {0x25 , 0x88}, 
    {0x94 , 0x0a}, //INT_OPEN  
    {0x80 , 0x55}, 

    //denoise 
    {0x70 , 0x6f}, //denoise
    {0x72 , 0x4f}, //denoise
    {0x73 , 0x2f}, //denoise
    {0x74 , 0x27}, //denoise
    {0x7a , 0x4e}, //denoise in  low light , 0x8e\0x4e\0x0e
    {0x7b , 0x28}, //the same as 0x86

    //black level
    {0X1F , 0x20}, //G target
    {0X22 , 0x20}, //R target
    {0X26 , 0x20}, //B target
    //模拟部分参数
    {0X16 , 0x00}, //如果觉得黑色物体不够黑，有点偏红，将0x16写为0x03会有点改善 
    {0xbb , 0x20},   // deglitch  对xclk整形
    {0xeb , 0x30}, 
    {0xf5 , 0x21}, 
    {0xe1 , 0x3c}, 
    {0xbb , 0x20}, 
    {0X2f , 0X66}, 
    {0x06 , 0xe0},    

    //anti black sun spot
    {0x61 , 0xd3}, //0x61[3]=0 black sun disable
    {0x79 , 0x48}, //0x79[7]=0 black sun disable

    //Gamma

    {0x3b , 0x60}, //auto gamma offset adjust in    low light  
    {0x3c , 0x20}, //auto gamma offset adjust in    low light  
    {0x56 , 0x40}, 
    {0x39 , 0x80},   
    //gamma1
    {0x3f , 0xb0}, 
    {0X40 , 0X88}, 
    {0X41 , 0X74}, 
    {0X42 , 0X5E}, 
    {0X43 , 0X4c}, 
    {0X44 , 0X44}, 
    {0X45 , 0X3E}, 
    {0X46 , 0X39}, 
    {0X47 , 0X35}, 
    {0X48 , 0X31}, 
    {0X49 , 0X2E}, 
    {0X4b , 0X2B}, 
    {0X4c , 0X29}, 
    {0X4e , 0X25}, 
    {0X4f , 0X22}, 
    {0X50 , 0X1F}, 

    /*gamma2  过曝过度好，高亮度
    {0x3f , 0xb0}, 
    {0X40 , 0X9b}, 
    {0X41 , 0X88}, 
    {0X42 , 0X6e}, 
    {0X43 , 0X59}, 
    {0X44 , 0X4d}, 
    {0X45 , 0X45}, 
    {0X46 , 0X3e}, 
    {0X47 , 0X39}, 
    {0X48 , 0X35}, 
    {0X49 , 0X31}, 
    {0X4b , 0X2e}, 
    {0X4c , 0X2b}, 
    {0X4e , 0X26}, 
    {0X4f , 0X23}, 
    {0X50 , 0X1F}, 
    */
    /*//gamma3 清晰亮丽 灰阶分布好
    {0X3f , 0Xb0}, 
    {0X40 , 0X60}, 
    {0X41 , 0X60}, 
    {0X42 , 0X66}, 
    {0X43 , 0X57}, 
    {0X44 , 0X4c}, 
    {0X45 , 0X43}, 
    {0X46 , 0X3c}, 
    {0X47 , 0X37}, 
    {0X48 , 0X33}, 
    {0X49 , 0X2f}, 
    {0X4b , 0X2c}, 
    {0X4c , 0X29}, 
    {0X4e , 0X25}, 
    {0X4f , 0X22}, 
    {0X50 , 0X20}, 

    //gamma 4   low noise   
    {0X3f , 0Xa8}, 
    {0X40 , 0X48}, 
    {0X41 , 0X54}, 
    {0X42 , 0X4E}, 
    {0X43 , 0X44}, 
    {0X44 , 0X3E}, 
    {0X45 , 0X39}, 
    {0X46 , 0X34}, 
    {0X47 , 0X30}, 
    {0X48 , 0X2D}, 
    {0X49 , 0X2A}, 
    {0X4b , 0X28}, 
    {0X4c , 0X26}, 
    {0X4e , 0X22}, 
    {0X4f , 0X20}, 
    {0X50 , 0X1E}, 
    */    

    //color matrix
    {0x51 , 0x08}, 
    {0x52 , 0x3a}, 
    {0x53 , 0x32}, 
    {0x54 , 0x12}, 
    {0x57 , 0x7f}, 
    {0x58 , 0x6d}, 
    {0x59 , 0x50}, 
    {0x5a , 0x5d}, 
    {0x5b , 0x0d}, 
    {0x5D , 0x95}, 
    {0x5C , 0x0e}, 

    /* 

    //                                
    {0x51 , 0x00}, 
    {0x52 , 0x15}, 
    {0x53 , 0x15}, 
    {0x54 , 0x12}, 
    {0x57 , 0x7d}, 
    {0x58 , 0x6a}, 
    {0x59 , 0x5c}, 
    {0x5a , 0x87}, 
    {0x5b , 0x2b}, 
    {0x5D , 0x95}, 
    {0x5C , 0x0e}, 
    //  

    //艳丽
    {0x51 , 0x0d}, 
    {0x52 , 0x2b}, 
    {0x53 , 0x1e}, 
    {0x54 , 0x15}, 
    {0x57 , 0x92}, 
    {0x58 , 0x7d}, 
    {0x59 , 0x5f}, 
    {0x5a , 0x74}, 
    {0x5b , 0x15}, 
    {0x5c , 0x0e}, 
    {0x5d , 0x95}, //0x5c[3:0] low light color coefficient，smaller  , lower noise

    //适中
    {0x51 , 0x08}, 
    {0x52 , 0x0E}, 
    {0x53 , 0x06}, 
    {0x54 , 0x12}, 
    {0x57 , 0x82}, 
    {0x58 , 0x70}, 
    {0x59 , 0x5C}, 
    {0x5a , 0x77}, 
    {0x5b , 0x1B}, 
    {0x5c , 0x0e}, //0x5c[3:0] low light color coefficient，smaller  , lower noise
    {0x5d , 0x95}, 


    //color 淡
    {0x51 , 0x03}, 
    {0x52 , 0x0d}, 
    {0x53 , 0x0b}, 
    {0x54 , 0x14}, 
    {0x57 , 0x59}, 
    {0x58 , 0x45}, 
    {0x59 , 0x41}, 
    {0x5a , 0x5f}, 
    {0x5b , 0x1e}, 
    {0x5c , 0x0e}, //0x5c[3:0] low light color coefficient，smaller  , lower noise
    {0x5d , 0x95}, 
    */

    {0x60 , 0x20}, //color open in low light 
    //AWB
    {0x6a , 0x81}, 
    {0x23 , 0x77}, //Green gain
    {0xa0 , 0x03}, 

    {0xa1 , 0X31}, 
    {0xa2 , 0X0e}, 
    {0xa3 , 0X26}, 
    {0xa4 , 0X0d}, 
    {0xa5 , 0x26}, 
    {0xa6 , 0x06}, 
    {0xa7 , 0x80}, //BLUE Target
    {0xa8 , 0x7c}, //RED Target
    {0xa9 , 0x28}, 
    {0xaa , 0x28}, 
    {0xab , 0x28}, 
    {0xac , 0x3c}, 
    {0xad , 0xf0}, 
    {0xc8 , 0x18}, 
    {0xc9 , 0x20}, 
    {0xca , 0x17}, 
    {0xcb , 0x1f}, 
    {0xaf , 0x00},    
    {0xc5 , 0x18},  
    {0xc6 , 0x00}, 
    {0xc7 , 0x20},  
    {0xae , 0x83}, 
    {0xcc , 0x3c}, 
    {0xcd , 0x90}, 
    {0xee , 0x4c}, // P_TH

    // color saturation
    {0xb0 , 0xd0}, 
    {0xb1 , 0xc0}, 
    {0xb2 , 0xa8}, 
    {0xb3 , 0x8a}, 

    //anti webcamera banding
    {0x9d , 0x4c}, 

    //switch direction
    {0x1e , 0x00}, //00:normal  10:IMAGE_V_MIRROR   20:IMAGE_H_MIRROR  30:IMAGE_HV_MIRROR
    


    //set max 15fps
    {0x92 , 0xa0},
    {0x93 , 0x00},
    {0x2a , 0x10},
    {0x2b , 0x88},

    {0X8e , 0X03}, 
    {0X8f , 0Xfc},  //set min  10fps

    {0x20 , 0x40},
    {0x09 , 0xc0},
};




LOCAL SENSOR_REG_TAB_INFO_T s_BF3703_resolution_Tab_YUV[]=
{   
    // COMMON INIT

    {ADDR_AND_LEN_OF_ARRAY(BF3703_YUV_640X480),640,480,24,SENSOR_IMAGE_FORMAT_YUV422},

    // YUV422 PREVIEW 1
    { PNULL, 0, 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    
    // YUV422 PREVIEW 2 
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0},
    { PNULL,                    0,      0,  0,                          0 , 0}

};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_BF3703_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    PNULL,//_BF3703_Power_On,
    PNULL,
    BF3703_Identify,

    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

    // External
    set_BF3703_ae_enable,
    set_hmirror_enable,
    set_vmirror_enable,

    set_brightness,
    set_contrast,
    set_sharpness,
    set_saturation,

    set_preview_mode,   
    set_image_effect,

       PNULL,                //BF3703_BeforeSnapshot,
       PNULL,             //BF3703_After_Snapshot,

#if defined(CMOS_SENSOR_FLASH_SUPPORT) //added by txy
    GPIO_OpenSensorFlash,
#else
    PNULL,
#endif

    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate,
    PNULL,
    PNULL,
    set_BF3703_awb,
    PNULL,
    PNULL,
    set_BF3703_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
       PNULL,
    set_BF3703_anti_flicker,
    set_BF3703_video_mode,
    PNULL,
    PNULL
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_bf3703_yuv_info =
{
    BF3703_I2C_ADDR_W,              // salve i2c write address
    BF3703_I2C_ADDR_R,              // salve i2c read address
    
    0,                              // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
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
    20,                             // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code
    0xFC, 0x37,                     // supply two code to identify sensor.
    0xFD, 0x03,                     // for Example: index = 0-> Device id, index = 1 -> version id  
                                    
    SENSOR_AVDD_2800MV,             // voltage of avdd  

    640,                            // max width of source image
    480,                            // max height of source image
    "BF3703",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_BF3703_resolution_Tab_YUV,    // point to resolution table information structure
    &s_BF3703_ioctl_func_tab,       // point to ioctl function table
            
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
    0,
    0
};
/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
 
LOCAL void BF3703_WriteReg( uint8  subaddr, uint8 data )
{
    Sensor_WriteReg(subaddr, data);
    //SCI_TRACE_LOW:"SENSOR: BF3703_WriteReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_546_112_2_18_0_30_31_693,(uint8*)"dd", subaddr, data);
}

LOCAL uint8 BF3703_ReadReg( uint8  subaddr)
{
    uint8 value = 0;
    
    value =Sensor_ReadReg( subaddr);
    //SCI_TRACE_LOW:"BF3703_ReadReg: subaddr:%x,value:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_554_112_2_18_0_30_31_694,(uint8*)"dd",subaddr,value);
    
    return value;
}


LOCAL uint32 BF3703_Identify(uint32 param)
{
#if 0 // by riyang.xie
    uint32 i;
    uint32 nLoop;
    uint8 ret;
    uint32 err_cnt = 0;
    uint8 reg[2] = {0xfC, 0xfD};
    uint8 value[2] = {0x37, 0x03};

    //SCI_TRACE_LOW:"BF3703_Identify: start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_570_112_2_18_0_30_31_695,(uint8*)"");

    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = BF3703_ReadReg(reg[i]);
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
            
                while(nLoop--);
                continue;
            }
        }
        err_cnt = 0;
        i++;
    }

    //SCI_TRACE_LOW:"BF3703_Identify: it is BF3703"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_595_112_2_18_0_30_31_696,(uint8*)"");
    
    return (uint32)SCI_SUCCESS;
#else
    uint16  iden_reg_val = 0;
    uint32  ret = SENSOR_OP_ERR;

    iden_reg_val = BF3703_ReadReg(0xfc);
    if(iden_reg_val == 0x37)
    {
            iden_reg_val = BF3703_ReadReg(0xfd);
            if(iden_reg_val == 0x03)
                   ret = SENSOR_OP_SUCCESS;
    }
    
    //SCI_TRACE_LOW:"BF3703_Identify: ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_610_112_2_18_0_30_31_697,(uint8*)"d", ret);
    return ret;

#endif
}

LOCAL uint32 set_BF3703_ae_enable(uint32 enable)
{
    unsigned char ae_value;

        ae_value=BF3703_ReadReg(AE_ENABLE);

        if(0x00==enable)
        {
            ae_value&=0xfa;
            BF3703_WriteReg(AE_ENABLE,ae_value);
        }
        else if(0x01==enable)
        {
            ae_value|=0x05;
            BF3703_WriteReg(AE_ENABLE,ae_value);
        }

    //SCI_TRACE_LOW:"SENSOR: set_ae_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_633_112_2_18_0_30_31_698,(uint8*)"d", enable);

    return 0;
}


LOCAL uint32 set_BF3703_ae_awb_enable(uint32 ae_enable, uint32 awb_enable)
{
        unsigned char ae_value;

        ae_value=BF3703_ReadReg(AE_ENABLE);

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

        BF3703_WriteReg(AE_ENABLE,ae_value);

        //SCI_TRACE_LOW:"set_BF3703_ae_awb_enable: set_ae_awb_enable: ae=%d awb=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_665_112_2_18_0_30_31_699,(uint8*)"dd", ae_enable, awb_enable);

    return 0;
}

void set_BF3703_shutter(uint16 shutter)
{            
    BF3703_WriteReg(0x8c,(shutter&0xFF00)>>8);
    BF3703_WriteReg(0x8D,shutter&0xFF);
}   /* set_BF3703_shutter */

uint16 get_BF3703_shutter(void)
{
    uint8 temp_reg1, temp_reg2;
    uint16 shutter;

    temp_reg1=BF3703_ReadReg(0x8c);
    //Delayms(10); //modify by ancky 
    SCI_Sleep(10);
    temp_reg2=BF3703_ReadReg(0x8d);
    
    shutter=((temp_reg1&0xff)<<8)|(temp_reg2);

    return shutter;
}   /* get_BF3703_shutter */


LOCAL uint32 set_hmirror_enable(uint32 enable)
{
 
    
    return 0;
}




LOCAL uint32 set_vmirror_enable(uint32 enable)
{

    
    return 0;
}


__align(4) const SENSOR_REG_T BF3703_ev_tab[][2]=
{
       {{0x55, 0xf8}, {0xff,0xff}},//-4
    {{0x55, 0xd8}, {0xff,0xff}},//-3
    {{0x55, 0xb8}, {0xff,0xff}},//-2
    {{0x55, 0x98}, {0xff,0xff}},//-1
    {{0x55, 0x00}, {0xff,0xff}},//0
    {{0x55, 0x18}, {0xff,0xff}},//+1
    {{0x55, 0x38}, {0xff,0xff}},//+2
    {{0x55, 0x58}, {0xff,0xff}},//+3
    {{0x55, 0x78}, {0xff,0xff}}//+4
};



    
LOCAL uint32 set_BF3703_ev(uint32 level)
{
    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3703_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    SCI_ASSERT(level < 7);/*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    
    //SCI_TRACE_LOW:"set_BF3703_ev: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_739_112_2_18_0_30_31_700,(uint8*)"d", level);

    return 0;
}
__align(4) const SENSOR_REG_T BF3703_anti_flicker_mode_tab[][3]=
{
      //50Hz:
        { {0x80,0x55},{0x9d,0x66},{0xff,0xff}},
    
     
        {{0x80,0x54},{0x9e,0x55},{0xff,0xff}},

};
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_BF3703_anti_flicker(uint32 mode)
{//24m->65.6 us 12m->131us
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3703_anti_flicker_mode_tab[mode];
    SCI_ASSERT(mode < 2);/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"set_BF3703_anti_flicker: set_banding_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_769_112_2_18_0_30_31_701,(uint8*)"d", mode);
    
    return 0;
}

/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//       
/******************************************************************************/
__align(4) const SENSOR_REG_T BF3703_video_mode_nor_tab[][7]=
{
    // normal mode 14fps
    {
       {0x92,0xa0},{0x93,0x00},{0x2a,0x10},{0x2b,0x88},{0x8e,0x03},{0x8f,0xfc},{0xff, 0xff}
    },    
    //vodeo mode 10fps
    {
       {0x92,0xf0},{0x93,0x01},{0x2a,0x10},{0x2b,0x88},{0x8e,0x04},{0x8f,0xfb},  {0xff, 0xff}      
    },
    // UPCC  mode 14fps
    {
       {0x92,0xa0},{0x93,0x00},{0x2a,0x10},{0x2b,0x88},{0x8e,0x03},{0x8f,0xfc},{0xff, 0xff}
    }  
};    


#define BF3703_video_mode_wqvga_nand_tab BF3703_video_mode_nor_tab

LOCAL uint32 set_BF3703_video_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3703_video_mode_nor_tab[mode];

 
   SCI_ASSERT(mode<=DCAMERA_MODE_MAX);/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"SENSOR: set_BF3703_video_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_811_112_2_18_0_30_31_702,(uint8*)"d", mode);
    return 0;
}

/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) const SENSOR_REG_T BF3703_awb_tab[][6]=

{
     //AUTO:
    {
    {0x13, 0x07},
    {0x01, 0x15},
    {0x02, 0x24},
    {0x6a, 0x81},
    {0x23, 0x66},   
    {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
    {0x13, 0x05},
    {0x01, 0x1f},
    {0x02, 0x15},
    {0x6a, 0x81},
    {0x23, 0x66},   
    {0xff, 0xff}        
    },
     //U30:
    {
    {0x13, 0x05},
    {0x01, 0x12},
    {0x02, 0x19},
    {0x6a, 0x81},
    {0x23, 0x66},   
    {0xff, 0xff}    
    },
    //CWF:
    {
    {0x13, 0x05},
    {0x01, 0x1b},
    {0x02, 0x19},
    {0x6a, 0x81},
    {0x23, 0x66},   
    {0xff, 0xff}            
    },  
    //FLUORESCENT:
    {
    {0x13, 0x05},
    {0x01, 0x24},
    {0x02, 0x1e},
    {0x6a, 0x81},
    {0x23, 0x66},   
    {0xff, 0xff}          
    },    
    //SUN:
    {
    {0x13, 0x05},
    {0x01, 0x54},
    {0x02, 0x5c},
    {0x6a, 0x81},
    {0x23, 0x66},   
    {0xff, 0xff}           
    },
    //CLOUD
    {
    {0x13, 0x05},
    {0x01, 0x10},
    {0x02, 0x28},
    {0x6a, 0x81},
    {0x23, 0x66},   
    {0xff, 0xff}            
    },  
};

LOCAL uint32 set_BF3703_awb(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3703_awb_tab[mode];

    SCI_ASSERT(mode < 7);/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"set_BF3703_awb: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_901_112_2_18_0_30_32_703,(uint8*)"d", mode);
    
    return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T BF3703_brightness_tab[][3]=
{
    {{0x24, 0x38},  {0x25, 0x48}, {0xff,0xff}},
    {{0x24, 0x48},  {0x25, 0x58}, {0xff,0xff}},
    {{0x24, 0x58},  {0x25, 0x68}, {0xff,0xff}},
    {{0x24, 0x60},  {0x25, 0x78}, {0xff,0xff}},
    {{0x24, 0x78},  {0x25, 0x88}, {0xff,0xff}},
    {{0x24, 0x88},  {0x25, 0x98}, {0xff,0xff}},
    {{0x24, 0x98},  {0x25, 0xa8}, {0xff,0xff}},
    {{0x24, 0xa0},  {0x25, 0xb0}, {0xff,0xff}},
    {{0x24, 0xb8},  {0x25, 0xc8}, {0xff,0xff}},
};


LOCAL uint32 set_brightness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3703_brightness_tab[level];

    SCI_ASSERT(level < 7);/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"BF3703 set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_938_112_2_18_0_30_32_704,(uint8*)"d", level);
    
    return 0;
}


__align(4) const SENSOR_REG_T BF3703_contrast_tab[][2]=
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

    sensor_reg_ptr = (SENSOR_REG_T*)BF3703_contrast_tab[level];

    SCI_ASSERT(level < 7 );/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(20);
    //SCI_TRACE_LOW:"BF3703: set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_971_112_2_18_0_30_32_705,(uint8*)"d", level);

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
    //SCI_TRACE_LOW:"BF3703 set_preview_mode: preview_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_999_112_2_18_0_30_32_706,(uint8*)"d", preview_mode);
    
//  s_preview_mode = preview_mode;
    
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            BF3703_set_work_mode(0);
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            BF3703_set_work_mode(1);
            break;
        }
        case DCAMERA_ENVIRONMENT_SUNNY:
        {
            BF3703_set_work_mode(0);
            break;
        }
        default:
        {
            break;
        }
            
    }
    
    SCI_Sleep(250);
#if defined (FP880XG2_DUALSIM_PROJECT) 
    if(SENSOR_MAIN==Sensor_GetCurId())
    {
    BF3703_WriteReg(0x1e, 0x00);
    }
    else if(SENSOR_SUB==Sensor_GetCurId())
    {
    BF3703_WriteReg(0x1e, 0x20);
    }
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
__align(4) const SENSOR_REG_T BF3703_image_effect_tab[][7]=
{
    {{0x13, 0x07},{0x80, 0x45},{0x76, 0x00},{0x69, 0x00},{0x67, 0x80}, {0x68 ,0x80}, {0xff,0xff}},//effect Normal
    {{0x13, 0x07},{0x80, 0x45},{0x76, 0x00},{0x69, 0x20},{0x67, 0x80}, {0x68 ,0x80}, {0xff,0xff}},//effect BLACKWHITE
    {{0x13, 0x07},{0x80, 0x45},{0x76, 0x00},{0x69, 0x20},{0x67, 0x80}, {0x68 ,0xc0}, {0xff,0xff}},// effect RED
    {{0x13, 0x07},{0x80, 0x45},{0x76, 0x00},{0x69, 0x20},{0x67, 0x40}, {0x68 ,0x40}, {0xff,0xff}},// effect GREEN
    {{0x13, 0x07},{0x80, 0x45},{0x76, 0x00},{0x69, 0x20},{0x67, 0xc0}, {0x68 ,0x80}, {0xff,0xff}},// effect  BLUE
    {{0x13, 0x07},{0x80, 0x45},{0x76, 0x00},{0x69, 0x20},{0x67, 0x40}, {0x68 ,0x80}, {0xff,0xff}},//effect ANTIQUE
    {{0x13, 0x07},{0x80, 0x45},{0x76, 0x00},{0x69, 0x40},{0x67, 0x80}, {0x68 ,0x80}, {0xff,0xff}},// effect NEGATIVE
    {{0x13, 0x07},{0x80, 0xc5},{0x76, 0x00},{0x69, 0x20},{0x67, 0x60}, {0x68 ,0x98}, {0xff,0xff}},// effect  sepia
};

    
LOCAL uint32 set_image_effect(uint32 effect_type)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3703_image_effect_tab[effect_type];

    
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"BF3703 set_image_effect: effect_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_1072_112_2_18_0_30_32_707,(uint8*)"d", effect_type);
    
    return 0;
}

LOCAL uint32 BF3703_After_Snapshot(uint32 param)
{
    
    BF3703_WriteReg(0x13, 0x07);
    //BF3703_WriteReg(0x1e, 0x30);
    BF3703_WriteReg(0x11, 0x80);
    //BF3703_WriteReg(0x09, 0x01);  
    

    SCI_Sleep(400);

    return 0;
    
}

LOCAL uint32 BF3703_BeforeSnapshot(uint32 param)
{

    uint16 shutter;

    
       BF3703_WriteReg(0x20, 0x40); 
    BF3703_WriteReg(0x09, 0xc0);    

    //read shutter


    BF3703_WriteReg(0x13,0xe2);

    shutter=get_BF3703_shutter();

    BF3703_WriteReg(0x11, 0x90); 

    shutter=shutter>>1;
    if(shutter<1)
    shutter=1;

    //write shutter
    set_BF3703_shutter(shutter);
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
    return SENSOR_GAIN_SCALE;
    
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
__align(4) const SENSOR_REG_T BF3703_mode_tab[][3]=
{
       {{0X8e,0X03}, {0X8f,0Xfc}, {0xff,0xff}},// normal
    {{0X8e,0X07}, {0X8f,0Xf8},{0xff,0xff}},// night  // min 5 fps
};

LOCAL uint32 BF3703_set_work_mode(uint32 mode)
{
    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)BF3703_mode_tab[mode];

    SCI_ASSERT(mode <= 1);/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
    BF3703_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"BF3703 set_work_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_1185_112_2_18_0_30_32_708,(uint8*)"d", mode);
    return 0;
}

#if 0 // by riyang.xie
LOCAL uint32 _BF3703_Power_On(BOOLEAN power_on)
{
    SENSOR_AVDD_VAL_E       dvdd_val=g_bf3703_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E       avdd_val=g_bf3703_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E       iovdd_val=g_bf3703_yuv_info.iovdd_val;  
    BOOLEAN                 power_down=(BOOLEAN )g_bf3703_yuv_info.power_down_level;        
    BOOLEAN                 reset_level=g_bf3703_yuv_info.reset_pulse_level;
    uint32              reset_width=g_bf3703_yuv_info.reset_pulse_width;        
    
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
    
    //SCI_TRACE_LOW:"SENSOR: _BF3703_Power_On(1:on, 0:off): %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_BF3703_1226_112_2_18_0_30_32_709,(uint8*)"d", power_on);
    
    return SCI_SUCCESS;
}

#endif
