/******************************************************************************
 ** File Name:    Sensor_GC6113.c                                         *
 ** Author:         Mormo_Hui                                                 *
 ** Date:           2011/06/14                                            *
 ** Platform:       SP6610&6620
 ** Copyright:    Spreadtrum All Rights Reserved.        *
 ** Description:   implementation of digital camera register interface       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE             NAME             DESCRIPTION                                 *
 ** 2011/06/14   Mormo       Fristly Released                               *
 ******************************************************************************/
#ifndef _GC6113_C_
#define _GC6113_C_

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
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define GC6113_I2C_ADDR_W               0x80
#define GC6113_I2C_ADDR_R               0x81
#define GC6113_I2C_ACK                  0x0
LOCAL BOOLEAN   bl_GC_50Hz_GC6113       = FALSE;

/**---------------------------------------------------------------------------*
     GC6113 SUPPORT 5 SERIAL MODE:
     CCIR601_8BIT, SPI, CCIR656_2BIT, CCIR656_4BIT, ONE2ONE(GC6113&GC6103)
 **---------------------------------------------------------------------------*/

//#define GC6113_OUTPUT_MODE_CCIR601_8BIT
//#define GC6113_OUTPUT_MODE_SPI
#define GC6113_OUTPUT_MODE_CCIR656_1BIT
//#define GC6113_OUTPUT_MODE_CCIR656_2BIT
//#define GC6113_OUTPUT_MODE_CCIR656_4BIT
//#define GC6113_OUTPUT_MODE_PACKET_1BIT
//#define GC6113_OUTPUT_MODE_PACKET_2BIT
//#define GC6113_OUTPUT_MODE_PACKET_4BIT
//#define GC6113_OUTPUT_MODE_ONE2ONE


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 GC6113_Power_On(uint32 power_on);
LOCAL uint32 GC6113_Identify(uint32 param);
LOCAL void   GC6113_WriteReg( uint8  subaddr, uint8 data );
LOCAL uint8  GC6113_ReadReg( uint8  subaddr);
LOCAL void   GC6113_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr );
LOCAL void   GC6113_Delayms (uint32 ms);
//LOCAL uint32 Set_GC6113_Mirror(uint32 level);
//LOCAL uint32 Set_GC6113_Flip(uint32 level);
LOCAL uint32 Set_GC6113_Brightness(uint32 level);
LOCAL uint32 Set_GC6113_Contrast(uint32 level);
LOCAL uint32 Set_GC6113_Image_Effect(uint32 effect_type);
LOCAL uint32 Set_GC6113_Ev(uint32 level);
LOCAL uint32 Set_GC6113_Anti_Flicker(uint32 mode);
LOCAL uint32 Set_GC6113_Preview_Mode(uint32 preview_mode);
LOCAL uint32 Set_GC6113_AWB(uint32 mode);
LOCAL uint32 GC6113_AE_AWB_Enable(uint32 ae_enable, uint32 awb_enable);
LOCAL uint32 GC6113_Before_Snapshot(uint32 para);
LOCAL uint32 GC6113_After_Snapshot(uint32 para);
LOCAL uint32 Set_GC6113_Video_Mode(uint32 mode);
LOCAL uint32 GC6113_GetPclkTab(uint32 param);
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
#ifdef GC6113_OUTPUT_MODE_CCIR601_8BIT

#else
__align(4) const SENSOR_REG_T GC6113_YUV_240X320[] = 
{
    {0xfe, 0x80},
    {0xfc, 0x12}, //clk_en
    
    {0xfa, 0x00}, // Cis clock config 1/1

    {0xfe, 0x00},
    {0x09, 0x01}, //[8]cis_win_height  486,1e6
    {0x0a, 0x48}, //[7:0]cis_win_height
    {0x0b, 0x00}, //[9:8]cis_win_width 646,286
    {0x0c, 0xf8}, //[7:0]cis_win_width
    {0x12, 0x2a}, //sh_delay
    {0x14, 0x10}, 
    {0x16, 0x06}, //[3:0]number of a/d pipe stages
    
    {0x1c, 0x49},
    {0x1d, 0x95},
    {0x20, 0x7f},
    {0x21, 0x38},
    {0x22, 0x73},
    {0x50, 0x14},
    /********************************************/
    /******************* BLK ********************/
    /********************************************/
    {0x30, 0xf7},
    {0x39, 0x09},  // exp_rate_darkc
    {0x3a, 0x18},
    {0x3b, 0x20},
    {0x3c, 0x22},
    {0x3d, 0x22},
    /********************************************/
    /******************* LSC ********************/
    /********************************************/
    {0x88, 0x20},
    {0x8b, 0x24},
    {0x8c, 0x20},
    {0x8d, 0x18},

    /********************************************/
    /******************* DNDD ********************/
    /********************************************/
    {0x62, 0x02},  // b_base
    {0x64, 0x05},

    /********************************************/
    /******************* ASDE********************/
    /********************************************/
    {0x69, 0x38},  //[5]auto_disable_dn when b<2, [4] 1->enhance dn when dark, 0->decrease dn when dark,
                      //[3:0]c_slope
    {0x6a, 0x88},  // [7:4]b_slope , [3:0]n_slope
    {0x6e, 0x50},  // ee1_effect,  ee1_slope
    {0x6f, 0x40},  // ee2_effect,  ee2_slope


    /********************************************/
    /******************* EEINTP ********************/
    /********************************************/
    {0x72, 0x6c},  


    /********************************************/
    /******************* AWB ********************/
    /********************************************/
    {0xfe, 0x01},   
    {0x00, 0xf5},       
    {0x02, 0x1a},   
    {0x03, 0x20},   
    {0x04, 0x10},   
    {0x05, 0x08},
    {0x06, 0x20},   
    {0x08, 0x20},   
    {0x0a, 0x40},   
    {0x11, 0x3f},   
    {0x12, 0x72},   
    {0x13, 0x11},   
    {0x14, 0x42},   //awb_r_5_gain
    {0x15, 0x43},   //awb_b_5_gain
    {0x16, 0xc2},     //sinT
    {0x17, 0xa6},   //cosT
    
    {0xc0, 0x08},   
    {0xc1, 0x06},   
    {0xc2, 0x3e},   
    {0xc3, 0x4e},   

    /********************************************/
    /******************* ABS ********************/
    /********************************************/
    {0xb0, 0x02},
    {0xb1, 0x01},
    {0xb2, 0x00},   
    {0xb3, 0x10},   //  y strech limit

    /********************************************/
    /******************* YCP ********************/
    /********************************************/
    {0xfe, 0x00},
    {0xb1, 0x38},
    {0xb2, 0x38},

    /*
    {0xb3, 0x40},
    {0xbe, 0x21},
    {0x93, 0x48},
    {0x94, 0xf6},
    {0x95, 0xf8},
    {0x96, 0x00}, 
    {0x97, 0x50}, 
    {0x98, 0xfa},
    {0x9d, 0x03},
    */
    
    /********************************************/
    /*******************Gamma********************/
    /********************************************/
    {0x9F, 0x0E},          
    {0xA0, 0x1C},  
    {0xA1, 0x34},
    {0xA2, 0x48},
    {0xA3, 0x5A},
    {0xA4, 0x6B},
    {0xA5, 0x7B},
    {0xA6, 0x95},
    {0xA7, 0xAB},
    {0xA8, 0xBF},
    {0xA9, 0xCE},
    {0xAA, 0xD9},
    {0xAB, 0xE4},
    {0xAC, 0xEC},
    {0xAD, 0xF7},
    {0xAE, 0xFd},
    {0xAF, 0xFF},

    /********************************************/
    /******************* AEC ********************/
    /********************************************/
    {0xfe, 0x00},
    {0xe5, 0x01},  //AEC enable
    {0xfe, 0x01},  
    {0x30, 0x0b},
    {0x31, 0x10},
    {0x32, 0x00},
    {0x33, 0x48}, // Y target
    {0x3b, 0x92},
    {0x3c, 0xa5},

    {0xfe, 0x00},
    {0x01, 0xb2},  //hb[7:0]
    {0x02, 0x60},  //vb[7:0]
    {0x0f, 0x07},  //vb[11:8] , hb[11:8] 

    {0xfe, 0x01},
    {0x42, 0x00},   //step[11:8]
    {0x43, 0x35},   //step[7:0]

    {0x44, 0x01},   // level_0  12.5fps
    {0x45, 0xa8},  
    {0x46, 0x02},   // level_1  8.33fps
    {0x47, 0x7c},
    {0x48, 0x03},   // level_2  6.25fps
    {0x49, 0x50},
    {0x4a, 0x04},   // level_3  5.00fps
    {0x4b, 0x24},
    
    {0x4c, 0x10},  // exp_level
    
    {0x4d, 0x02},  // min_exp
    {0x4e, 0xc0},  //aec post gain limit
    {0x4f, 0x40},  //aec pre gain limit
    
    /********************************************/
    /******************* SPI ********************/
    /********************************************/
    {0xfe, 0x02},
    {0x01, 0x03},
    {0x02, 0x82},   //0x80->0x02, LSB & Falling edge sample
    
#ifdef GC6113_OUTPUT_MODE_SPI
    {0x03, 0x00},   //signal wire
#elif defined(GC6113_OUTPUT_MODE_CCIR656_1BIT)||defined(GC6113_OUTPUT_MODE_PACKET_1BIT)
    {0x03, 0x20},  //singel wire
#elif defined(GC6113_OUTPUT_MODE_CCIR656_2BIT)||defined(GC6113_OUTPUT_MODE_PACKET_2BIT)
    {0x03, 0x25},  //two wire
#else   //GC6113_OUTPUT_MODE_CCIR656_4BIT & GC6113_OUTPUT_MODE_PACKET_4BIT
    {0x03, 0x2a},  //four wire
    {0xfd, 0x01},  //four wire
#endif  

    {0x04, 0x20},  //[4] master_outformat
    {0x05, 0x00},  
    {0x09, 0x00},  
    {0x0a, 0x00},  //Data ID, 0x00-YUV422, 0x01-RGB565
    {0x13, 0xf0},

#ifdef GC6113_OUTPUT_MODE_SPI
    {0x24, 0x00},       //spi mode      
#elif defined(GC6113_OUTPUT_MODE_CCIR656_1BIT)||defined(GC6113_OUTPUT_MODE_CCIR656_2BIT)||defined(GC6113_OUTPUT_MODE_CCIR656_4BIT)
    {0x25, 0xff},  //head sync code
    {0x26, 0x00},
    {0x27, 0x00},
        
    //SYNC code
    {0x20, 0xb6},  //frame_start -- not need
    {0x21, 0x80},  //line sync start
    {0x22, 0x9d},  //line sync end
    {0x23, 0xb6},  //frame end

    {0x24, 0x03},       //bt656 mode
#else //GC6113_OUTPUT_MODE_PACKET_2BIT & GC6113_OUTPUT_MODE_PACKET_2BIT & GC6113_OUTPUT_MODE_PACKET_4BIT
    {0x25, 0xff},  //head sync code
    {0x26, 0xff},
    {0x27, 0xff},

    {0x24, 0x00},       //packet mode
#endif

    /********************************************/
    /******************* OUT ********************/
    /********************************************/
    {0xfe, 0x00},
    {0x1f, 0x15},  // bit[7:6] driving SCK current
    {0x1d, 0x95},  // bit[3:2] driving SDO_0 current
    {0x24, 0xa2},

    
#ifdef GC6113_OUTPUT_MODE_SPI
    {0xf0, 0x19}, //output_en
#elif defined(GC6113_OUTPUT_MODE_CCIR656_1BIT)||defined(GC6113_OUTPUT_MODE_PACKET_1BIT)
    {0xf0, 0x11},
#elif defined(GC6113_OUTPUT_MODE_CCIR656_2BIT)||defined(GC6113_OUTPUT_MODE_PACKET_2BIT)
    {0xf0, 0x13}, //output_en
#else
    {0xf0, 0x1f}, //output_en
#endif

//-----------------Updated By Mormo 2011/05/11 Start-------------//
    {0xfe, 0x00},
    {0x60, 0xe7}, 
    {0x6b, 0x55},   
//-----------------Updated By Mormo 2011/05/11 End-------------//


//-----------------Updated By Mormo 2011/05/19 Start-------------//
    {0xfe, 0x00},
    // daemon add start 
    {0x2e, 0xc0},  // open in_buf 
    {0x46, 0x80}, 
    {0x48, 0x02},  //Crop Start X
    {0x49, 0x01}, 
    {0x4a, 0x40}, 
    {0x4b, 0x00}, 
    {0x4c, 0xf0}, 
    // daemon add end 
//-----------------Updated By Mormo 2011/05/19 End-------------//

//-----------------Updated By Mormo 2011/05/31 Start-------------//
    {0xfe, 0x00},
    {0x2e, 0x40},  // close in_buf 
    {0x08, 0x04},
    {0x48, 0x00},  //Crop Start X 0x00
//-----------------Updated By Mormo 2011/05/31 End-------------//
};
#endif


LOCAL SENSOR_REG_TAB_INFO_T s_GC6113_resolution_Tab_YUV[]=
{   
    { ADDR_AND_LEN_OF_ARRAY(GC6113_YUV_240X320), 240, 320,  24, SENSOR_IMAGE_FORMAT_YUV422 },
    // YUV422 PREVIEW 1
    { PNULL,                    0,   240,    320,    24   ,   SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    { PNULL,                    0,      0,  0  ,        0,        0      },   
    

    // YUV422 PREVIEW 2 
     { PNULL,                   0,      0,  0 ,         0,         0     }, 
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      },   
     { PNULL,                   0,      0,  0  ,        0,        0      }

};


LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC6113_ioctl_func_tab = 
{
// Internal 
    PNULL,
    GC6113_Power_On, 
    PNULL,
    GC6113_Identify,
    PNULL,
    PNULL,
    
    PNULL,
    GC6113_GetPclkTab,

// External
    PNULL, 
    PNULL, //Set_GC6113_Mirror,
    PNULL, //Set_GC6113_Flip,
    
    Set_GC6113_Brightness,
    Set_GC6113_Contrast,
    PNULL, 
    PNULL, 
    Set_GC6113_Preview_Mode, 
    
    Set_GC6113_Image_Effect, 
    PNULL,//GC6113_Before_Snapshot,
    PNULL,//GC6113_After_Snapshot,

    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    Set_GC6113_AWB,    
    PNULL,
    PNULL, 
    Set_GC6113_Ev, 
    PNULL, 
    PNULL, 
    PNULL,
    
    PNULL,
    PNULL,
    Set_GC6113_Anti_Flicker,
    Set_GC6113_Video_Mode,   
    PNULL
};

 

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC const SENSOR_INFO_T g_GC6113_yuv_info =
{
    GC6113_I2C_ADDR_W,              // salve i2c write address
    GC6113_I2C_ADDR_R,              // salve i2c read address
    
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
    
    SENSOR_LOW_LEVEL_PWDN,          // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code 
    0x00, 0xb8,                     // supply two code to identify sensor.
    0x00, 0xb8,                     // for Example: index = 0-> Device id, index = 1 -> version id                                          
                                            
    SENSOR_AVDD_2800MV,             // voltage of avdd  
    
    240,                            // max width of source image
    320,                            // max height of source image
    "GC6113",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_GC6113_resolution_Tab_YUV,    // point to resolution table information structure
    &s_GC6113_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor  
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1800MV,                      // dvdd
    4,                     // skip frame num before preview 
    3,                     // skip frame num before capture     
    0,                     // deci frame num during preview;        
    2,                     // deci frame num during video preview;
    0,                     // threshold enable
    0,                     // threshold mode
    0,                     // threshold start postion   
    0,                     // threshold end postion 
#ifdef GC6113_OUTPUT_MODE_SPI
    SENSOR_OUTPUT_MODE_SPI_1BIT,
#elif defined(GC6113_OUTPUT_MODE_CCIR656_1BIT)
    SENSOR_OUTPUT_MODE_CCIR656_1BIT,
#elif defined(GC6113_OUTPUT_MODE_CCIR656_2BIT)
    SENSOR_OUTPUT_MODE_CCIR656_2BIT,
#elif defined(GC6113_OUTPUT_MODE_CCIR656_4BIT)
    SENSOR_OUTPUT_MODE_CCIR656_4BIT,
#elif defined(GC6113_OUTPUT_MODE_PACKET_4BIT)
    SENSOR_OUTPUT_MODE_PACKET_4BIT, 
#elif defined(GC6113_OUTPUT_MODE_PACKET_2BIT)
    SENSOR_OUTPUT_MODE_PACKET_2BIT, 
#elif defined(GC6113_OUTPUT_MODE_PACKET_1BIT)
    SENSOR_OUTPUT_MODE_PACKET_1BIT, 
#else 
    SENSOR_OUTPUT_MODE_CCIR601_8BIT, 
#endif
    SENSOR_OUTPUT_ENDIAN_BIG
};

/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void GC6113_WriteReg( uint8  subaddr, uint8 data )
{
    Sensor_WriteReg(subaddr, data);
    //SCI_TRACE_LOW:"GC6113_WriteReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_517_112_2_18_0_30_52_819,(uint8*)"dd", subaddr, data);
}

LOCAL uint8 GC6113_ReadReg( uint8  subaddr)
{
    uint8 value = 0;
    
    value =Sensor_ReadReg( subaddr);
    //SCI_TRACE_LOW:"GC6113_ReadReg: subaddr:%x,value:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_525_112_2_18_0_30_52_820,(uint8*)"dd",subaddr,value);
    
    return value;
}


LOCAL void GC6113_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

}

LOCAL void GC6113_Delayms (uint32 ms)   
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
LOCAL uint32 GC6113_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E dvdd_val=g_GC6113_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E avdd_val=g_GC6113_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E iovdd_val=g_GC6113_yuv_info.iovdd_val;  
    BOOLEAN reset_level=g_GC6113_yuv_info.reset_pulse_level;
    uint32 reset_width=g_GC6113_yuv_info.reset_pulse_width;     
    
    if(SCI_TRUE==power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
        
        // Open Mclk in default frequency
        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);    // Default MCLK=12Mhz
        Sensor_SetResetLevel(!reset_level);     /*lint !e730*/
        SCI_Sleep(20);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel(!reset_level);     /*lint !e730*/
        SCI_Sleep(100);
    }
    else
    {
        GC6113_WriteReg(0xfc,0x01);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(20);

        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);        
        SCI_Sleep(20);
        
        Sensor_SetMCLK(SENSOR_DISABLE_MCLK);           
    }
    
    //SCI_TRACE_LOW("SENSOR: _GC6113_On(1:on, 0:off): %d", power_on);    
    
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
LOCAL uint32 GC6113_Identify(uint32 param)
{
    uint8 reg[2]    = {0x00, 0x00};
    uint8 value[2]  = {0xb8, 0xb8};
    uint8 ret       = 0;
    uint32 i;
    uint8   err_cnt = 0;
    uint32 nLoop = 1000;

    GC6113_WriteReg(0xfc,0x12);
    
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = GC6113_ReadReg(reg[i]);    
        
     //SCI_TRACE_LOW:"SENSOR: GC6113 Read reg0x00 = %x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_620_112_2_18_0_30_52_821,(uint8*)"d",ret );
        
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
                 //SCI_TRACE_LOW:"GC6113 Fail"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_627_112_2_18_0_30_52_822,(uint8*)"" );
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

    //SCI_TRACE_LOW:"GC6113 Identify"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_640_112_2_18_0_30_52_823,(uint8*)"" );
    return (uint32)SCI_SUCCESS;
}


LOCAL SENSOR_TRIM_T s_GC6113_Pclk_Tab[]=
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

LOCAL uint32 GC6113_GetPclkTab(uint32 param)
{
    return (uint32)s_GC6113_Pclk_Tab;
}


__align(4) const SENSOR_REG_T GC6113_brightness_tab[][2]=
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

LOCAL uint32 Set_GC6113_Brightness(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6113_brightness_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 
    
    GC6113_Write_Group_Regs(sensor_reg_ptr);
    
    //SCI_TRACE_LOW:"set_GC6113_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_709_112_2_18_0_30_52_824,(uint8*)"d", level);
    
    return 0;
}



__align(4) const SENSOR_REG_T GC6113_contrast_tab[][2]=
{
    {
        {0xb3,0x34},    {0xff,0xff},
    },

    {
        {0xb3,0x38},    {0xff,0xff}, 
    },

    {
        {0xb3,0x3d},    {0xff,0xff}, 
    },

    {
        {0xb3,0x40},    {0xff,0xff},
    },

    {
        {0xb3,0x44},    {0xff,0xff}, 
    },

    {
        {0xb3,0x48},    {0xff,0xff}, 
    },

    {
        {0xb3,0x50},    {0xff,0xff},
    },                   
};

LOCAL uint32 Set_GC6113_Contrast(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6113_contrast_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    
    GC6113_Write_Group_Regs(sensor_reg_ptr);
    
    //SCI_TRACE_LOW:"set_GC6113_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_756_112_2_18_0_30_52_825,(uint8*)"d", level);
    
    return 0;
}


__align(4) const SENSOR_REG_T GC6113_image_effect_tab[][4]= 
{
    // effect normal
    {
        {0x23,0x00}, {0xba,0x00}, {0xbb,0x00}, {0xff,0xff}
    },
    //effect BLACKWHITE
    {
        {0x23,0x02}, {0xba,0x00}, {0xbb,0x00}, {0xff,0xff}
    },
    // effect RED pink
    {
        {0x23,0x02}, {0xba,0x10}, {0xbb,0x50}, {0xff,0xff}
    },
    // effect GREEN
    {
        {0x23,0x02}, {0xba,0xe0}, {0xbb,0xe0}, {0xff,0xff}
    },
    // effect  BLUE
    {
        {0x23,0x02}, {0xba,0x50}, {0xbb,0xe0}, {0xff,0xff}
    },
    // effect  YELLOW
    {
        {0x23,0x02}, {0xba,0x80}, {0xbb,0x20}, {0xff,0xff}
    },  
    // effect NEGATIVE
    {        
        {0x23,0x01}, {0xba,0x00}, {0xbb,0x00}, {0xff,0xff}
    },    
    //effect ANTIQUE
    {
        {0x23,0x02}, {0xba,0xd0}, {0xbb,0x28}, {0xff,0xff}
    },
};

LOCAL uint32 Set_GC6113_Image_Effect(uint32 effect_type)
{
   
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6113_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 

    GC6113_Write_Group_Regs(sensor_reg_ptr);
    
    //SCI_TRACE_LOW:"set_GC6113_image_effect: effect_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_807_112_2_18_0_30_52_826,(uint8*)"d", effect_type);
    
    return 0;
}



__align(4) const SENSOR_REG_T GC6113_ev_tab[][4]=
{   
    {{0xfe, 0x01},{0x33, 0x30},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x38},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x40},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x48},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x50},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x58},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x60},{0xfe, 0x00},{0xff, 0xff}},  
};

LOCAL uint32 Set_GC6113_Ev(uint32 level)
{
  
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6113_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    SCI_ASSERT(level < 7);    /*assert verified*/ 
 
    GC6113_Write_Group_Regs(sensor_reg_ptr );

    //SCI_TRACE_LOW:"set_GC6113_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_835_112_2_18_0_30_52_827,(uint8*)"d", level);

    return 0;
}

/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 Set_GC6113_Anti_Flicker(uint32 mode)
{ 
    switch(mode)
    {
    case DCAMERA_FLICKER_50HZ:

        bl_GC_50Hz_GC6113 = TRUE;
        GC6113_WriteReg(0xfe, 0x00);
        GC6113_WriteReg(0x01, 0xb2);  //hb[7:0]
        GC6113_WriteReg(0x02, 0x60);  //vb[7:0]
        GC6113_WriteReg(0x0f, 0x07);  //vb[11:8] , hb[11:8] 

        GC6113_WriteReg(0xfe, 0x01);
        GC6113_WriteReg(0x42, 0x00);   //step[11:8]
        GC6113_WriteReg(0x43, 0x35);   //step[7:0]

        GC6113_WriteReg(0x44, 0x01);   // level_0  12.5fps
        GC6113_WriteReg(0x45, 0xa8);  
        GC6113_WriteReg(0x46, 0x02);   // level_1  8.33fps
        GC6113_WriteReg(0x47, 0x7c);
        GC6113_WriteReg(0x48, 0x03);   // level_2  6.25fps
        GC6113_WriteReg(0x49, 0x50);
        GC6113_WriteReg(0x4a, 0x04);   // level_3  5.00fps
        GC6113_WriteReg(0x4b, 0x24);

        GC6113_WriteReg(0xfe, 0x00);
    break;

    case DCAMERA_FLICKER_60HZ:

        bl_GC_50Hz_GC6113 = FALSE;

        GC6113_WriteReg(0xfe, 0x00);
        GC6113_WriteReg(0x01, 0xf0);  //hb[7:0]
        GC6113_WriteReg(0x02, 0x66);  //vb[7:0]
        GC6113_WriteReg(0x0f, 0x07);  //vb[11:8] , hb[11:8] 

        GC6113_WriteReg(0xfe, 0x01);
        GC6113_WriteReg(0x42, 0x00);   //step[11:8]
        GC6113_WriteReg(0x43, 0x2b);   //step[7:0]

        GC6113_WriteReg(0x44, 0x01);   // level_0  11.99fps
        GC6113_WriteReg(0x45, 0xae);  
        GC6113_WriteReg(0x46, 0x02);   // level_1  7.99fps
        GC6113_WriteReg(0x47, 0x85);
        GC6113_WriteReg(0x48, 0x03);   // level_2  6.31fps
        GC6113_WriteReg(0x49, 0x31);
        GC6113_WriteReg(0x4a, 0x04);   // level_3  4.99fps
        GC6113_WriteReg(0x4b, 0x08);

        GC6113_WriteReg(0xfe, 0x00);
    break;

        default:
        break;
    }
      
    //SCI_TRACE_LOW:"set_GC6113_anti_flicker-mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_902_112_2_18_0_30_52_828,(uint8*)"d",mode);
    OS_TickDelay(100); 
    
    return 0;
}

LOCAL uint32 Set_GC6113_Preview_Mode(uint32 preview_mode)
{
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            GC6113_WriteReg(0xfe, 0x01);
            GC6113_WriteReg(0x4c, 0x10); //12.5fps
            GC6113_WriteReg(0xfe, 0x00);
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            GC6113_WriteReg(0xfe, 0x01);
            GC6113_WriteReg(0x4c, 0x30);//5fps
            GC6113_WriteReg(0xfe, 0x00);    
            break;
        }
        default:
        {
            break;
        }
        
    }
      //SCI_TRACE_LOW:"set_GC6113_preview_mode: level = %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_932_112_2_18_0_30_53_829,(uint8*)"d", preview_mode);
      return 0;
}


__align(4) const SENSOR_REG_T GC6113_awb_tab[][5]=
{
    //AUTO
    {
        {0x22, 0x73},    // the reg value is not written here, rewrite in set_GC6113_awb();
        {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
        {0x22, 0x71},    // Disable AWB 
        {0x5a, 0x48},
        {0x5b, 0x40},
        {0x5c, 0x5c},
        {0xff, 0xff} 
    },
    //U30
    {
        {0x22, 0x71},   // Disable AWB 
        {0x5a, 0x40},
        {0x5b, 0x54},
        {0x5c, 0x70},
        {0xff, 0xff} 
    },  
    //CWF  //
    {
        {0x22, 0x71},   // Disable AWB 
        {0x5a, 0x40},
        {0x5b, 0x54},
        {0x5c, 0x70},
        {0xff, 0xff} 
    },    
    //FLUORESCENT:
    {
        {0x22, 0x71},   // Disable AWB 
        {0x5a, 0x40},
        {0x5b, 0x42}, 
        {0x5c, 0x50},
        {0xff, 0xff} 
    },
    //SUN:
    {
        {0x22, 0x71},   // Disable AWB 
        {0x5a, 0x50},
        {0x5b, 0x45},
        {0x5c, 0x40},
        {0xff, 0xff} 
    },
    //CLOUD:
    {
        {0x22, 0x71},   // Disable AWB 
        {0x5a, 0x5a},
        {0x5b, 0x42},
        {0x5c, 0x40},
        {0xff, 0xff} 
    },
};

LOCAL uint32 Set_GC6113_AWB(uint32 mode)
{
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GC6113_awb_tab[mode];

    SCI_ASSERT(mode < 7);    /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    
    GC6113_Write_Group_Regs(sensor_reg_ptr);
    
    //SCI_TRACE_LOW:"set_GC6113_awb_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_1004_112_2_18_0_30_53_830,(uint8*)"d", mode);
    
    return 0;
}

LOCAL uint32 GC6113_AE_AWB_Enable(uint32 ae_enable, uint32 awb_enable)
{
    uint16 ae_value = 0 , awb_value=0;

    awb_value = GC6113_ReadReg(0x22);

    if(0x01==ae_enable)
    {
        ae_value |= 0x01;
    }
    else if(0x00==ae_enable)
    {
        ae_value &=0xfe;
    }

    if(0x01==awb_enable)
    {
        awb_value |=0x02;
    }
    else if(0x00==awb_enable)
    {
        awb_value &= 0xfd;
    }   
    
    GC6113_WriteReg(0xe5, ae_value);
    GC6113_WriteReg(0x22, awb_value);

       //SCI_TRACE_LOW:"SENSOR: set_ae_awb_enable: ae_enable = %d  awb_enable = %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_1036_112_2_18_0_30_53_831,(uint8*)"dd", ae_enable ,awb_enable);
    return 0;
}


LOCAL uint32 GC6113_Before_Snapshot(uint32 para)
{
    uint8 reg_val = 0;
    uint16 exposal_time=0x00;

    GC6113_AE_AWB_Enable(0x00,0x00);   // close aec awb

    reg_val = GC6113_ReadReg(0x04);
    exposal_time=reg_val&0x00ff;
    reg_val = GC6113_ReadReg(0x03);
    exposal_time|=((reg_val&0x00ff)<<0x08);

    if(exposal_time<1)
    {
        exposal_time=1;
    }

    reg_val=exposal_time&0x00ff;
    GC6113_WriteReg(0x04, reg_val);
    reg_val=(exposal_time&0xff00)>>8;
    GC6113_WriteReg(0x03, reg_val);

    GC6113_Delayms(400);    
    
    //SCI_TRACE_LOW:"SENSOR_GC6113: Before Snapshot"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_1065_112_2_18_0_30_53_832,(uint8*)"");

    return 0;
}



LOCAL uint32 GC6113_After_Snapshot(uint32 para)
{
    GC6113_AE_AWB_Enable(0x01,0x01);   // Open aec awb
    
    GC6113_Delayms(400);
    
    //SCI_TRACE_LOW:"SENSOR_GC6113: After Snapshot"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_1078_112_2_18_0_30_53_833,(uint8*)"");

    return 0;
}


 
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//       
/******************************************************************************/   
LOCAL uint32 Set_GC6113_Video_Mode(uint32 mode)
{
    
    SCI_ASSERT(mode <=DCAMERA_MODE_MAX);    /*assert verified*/ 

    if(1==mode ||2==mode )
    {
        if(bl_GC_50Hz_GC6113 == TRUE)  // is 50hz in video
        {
        GC6113_WriteReg(0xfe, 0x00);
        GC6113_WriteReg(0x01, 0xb2);  //hb[7:0]
        GC6113_WriteReg(0x02, 0x60);  //vb[7:0]
        GC6113_WriteReg(0x0f, 0x07);  //vb[11:8] , hb[11:8] 

        GC6113_WriteReg(0xfe, 0x01);
        GC6113_WriteReg(0x42, 0x00);   //step[11:8]
        GC6113_WriteReg(0x43, 0x35);   //step[7:0]

        GC6113_WriteReg(0x44, 0x01);   // level_0  12.5fps
        GC6113_WriteReg(0x45, 0xa8);  
        GC6113_WriteReg(0x46, 0x02);   // level_1  8.33fps
        GC6113_WriteReg(0x47, 0x7c);
        GC6113_WriteReg(0x48, 0x03);   // level_2  6.25fps
        GC6113_WriteReg(0x49, 0x50);
        GC6113_WriteReg(0x4a, 0x04);   // level_3  5.00fps
        GC6113_WriteReg(0x4b, 0x24);

        GC6113_WriteReg(0xfe, 0x00);

    }
    else  // is 60hz in video mode
    {
        GC6113_WriteReg(0xfe, 0x00);
        GC6113_WriteReg(0x01, 0xf0);  //hb[7:0]
        GC6113_WriteReg(0x02, 0x66);  //vb[7:0]
        GC6113_WriteReg(0x0f, 0x07);  //vb[11:8] , hb[11:8] 

        GC6113_WriteReg(0xfe, 0x01);
        GC6113_WriteReg(0x42, 0x00);   //step[11:8]
        GC6113_WriteReg(0x43, 0x2b);   //step[7:0]

        GC6113_WriteReg(0x44, 0x01);   // level_0  11.99fps
        GC6113_WriteReg(0x45, 0xae);  
        GC6113_WriteReg(0x46, 0x02);   // level_1  7.99fps
        GC6113_WriteReg(0x47, 0x85);
        GC6113_WriteReg(0x48, 0x03);   // level_2  6.31fps
        GC6113_WriteReg(0x49, 0x31);
        GC6113_WriteReg(0x4a, 0x04);   // level_3  4.99fps
        GC6113_WriteReg(0x4b, 0x08);

        GC6113_WriteReg(0xfe, 0x00);
    }
    //SCI_TRACE_LOW:"set_GC6113_video_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC6113_1142_112_2_18_0_30_53_834,(uint8*)"d",mode);
    }   
    
    return 0;
}

/*lint -restore */

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
