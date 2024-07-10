/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_SP0838.c                                           *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor SP0838. 
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
#define SP0838_I2C_ADDR_W       0x30//0x60
#define SP0838_I2C_ADDR_R       0x31//0x61

#define SENSOR_GAIN_SCALE       16

 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_SP0838_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 SP0838_Identify(uint32 param);
/*
LOCAL uint32 SP0838_BeforeSnapshot(uint32 param);
LOCAL uint32 SP0838_After_Snapshot(uint32 param);
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
LOCAL uint32 SP0838_set_work_mode(uint32 mode);

LOCAL uint32 set_SP0838_ev(uint32 level);
LOCAL uint32 set_SP0838_awb(uint32 mode);
LOCAL uint32 set_SP0838_anti_flicker(uint32 mode);
LOCAL uint32 set_SP0838_video_mode(uint32 mode);

LOCAL BOOLEAN gc_enter_effect = SCI_FALSE;

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL void SP0838_Delayms (uint32 ms)   
{
    uint32 t1, t2;
    
    t1 = t2 = SCI_GetTickCount ();
    
    do{
        t2 = SCI_GetTickCount ();
    }while (t2 < (t1+ms));
}
__align(4) const SENSOR_REG_T SP0838_YUV_640X480[]=
{
{0xfd,0x00},
{0x1B,0x02},
{0x27,0xe8},
{0x28,0x0B},
{0x32,0x00},
{0x22,0xc0},
{0x26,0x10},
{0x31,0x70},
{0x5f,0x11},

{0xfd,0x01},
{0x25,0x1a},
{0x26,0xfb},
{0x28,0x61},
{0x29,0x49},
{0x31,0x60},
{0x32,0x18},
{0x4d,0xdc},
{0x4e,0x6b},
{0x41,0x8c},
{0x42,0x66},
{0x55,0xff},
{0x56,0x00},
{0x59,0x82},
{0x5a,0x00},
{0x5d,0xff},
{0x5e,0x6f},
{0x57,0xff},
{0x58,0x00},
{0x5b,0xff},
{0x5c,0xa8},
{0x5f,0x75},
{0x60,0x00},
{0x2d,0x00},
{0x2e,0x00},
{0x2f,0x00},
{0x30,0x00},
{0x33,0x00},
{0x34,0x00},
{0x37,0x00},
{0x38,0x00},

{0xfd,0x00},
{0x33,0xff},
{0x51,0x3f},
{0x52,0x09},
{0x53,0x00},
{0x54,0x00},
{0x55,0x10},
{0x57,0x40},
{0x58,0x40},
{0x59,0x10},
{0x56,0x71},
{0x5a,0x02},
{0x5b,0x02},
{0x5c,0x20},

{0x65,0x06},
{0x66,0x01},
{0x67,0x03},
{0x68,0xc6},
{0x69,0x7f},
{0x6a,0x01},
{0x6b,0x06},
{0x6c,0x01},
{0x6d,0x03},
{0x6e,0xc6},
{0x6f,0x7f},
{0x70,0x01},
{0x71,0x0a},
{0x72,0x10},
{0x73,0x02},
{0x74,0xc3},
{0x75,0x7f},
{0x76,0x01},

{0xcb,0x07},
{0xcc,0x04},
{0xce,0xff},
{0xcf,0x10},
{0xd0,0x20},
{0xd1,0x00},
{0xd2,0x1c},
{0xd3,0x16},
{0xd4,0x00},
{0xd6,0x1c},
{0xd7,0x16},
{0xdd,0x70}, 
{0xde,0x88}, 
{0xdf,0x80},

{0x7f,0x8c},    
{0x80,0xf4},
{0x81,0x00},
{0x82,0xed},
{0x83,0xa6},
{0x84,0xed},
{0x85,0xfa},
{0x86,0xba},
{0x87,0xcc},
{0x88,0x0c},
{0x89,0x33},
{0x8a,0x0f},

{0x8b,0x00}, 
{0x8c,0x1a},
{0x8d,0x29},
{0x8e,0x41},
{0x8f,0x62},
{0x90,0x7c},
{0x91,0x90},
{0x92,0xa2},
{0x93,0xaf},
{0x94,0xba},
{0x95,0xc4},
{0x96,0xce},
{0x97,0xd6},
{0x98,0xdd},
{0x99,0xe4},
{0x9a,0xea},
{0x9b,0xf1},
{0xfd,0x01},
{0x8d,0xf8},
{0x8e,0xff},

{0xfd,0x00},
{0xca,0xcf},
{0xd8,0x75},
{0xd9,0x75},
{0xda,0x75},
{0xdb,0x55},

{0xb9,0x00}, 
{0xba,0x04},
{0xbb,0x08},
{0xbc,0x10},
{0xbd,0x20},
{0xbe,0x30},
{0xbf,0x40},
{0xc0,0x50},
{0xc1,0x60},
{0xc2,0x70},
{0xc3,0x80},
{0xc4,0x90},
{0xc5,0xA0},
{0xc6,0xB0},
{0xc7,0xC0},
{0xc8,0xD0},
{0xc9,0xE0},
{0xfd,0x01},
{0x89,0xf0},
{0x8a,0xff},

{0xfd,0x00},
{0xe8,0x30},
{0xe9,0x30},
{0xea,0x40},
{0xf4,0x1b},
{0xf5,0x80},
{0xf7,0x78},
{0xf8,0x68},
{0xf9,0x68},
{0xfa,0x58},
{0xfd,0x01},
{0x09,0x31},
{0x0a,0x85},
{0x0b,0x0b},
{0x14,0x20},
{0x15,0x0f},

{0xfd,0x00},
{0x05,0x0 },
{0x06,0x0 },
{0x09,0x1 },
{0x0a,0x76},
{0xf0,0x62},
{0xf1,0x0 },
{0xf2,0x5f},
{0xf5,0x78},
{0xfd,0x01},
{0x00,0xb8},
{0x0f,0x60},
{0x16,0x60},
{0x17,0xa8},
{0x18,0xb0},
{0x1b,0x60},
{0x1c,0xb0},
{0xb4,0x20},
{0xb5,0x3a},
{0xb6,0x5e},
{0xb9,0x40},
{0xba,0x4f},
{0xbb,0x47},
{0xbc,0x45},
{0xbd,0x43},
{0xbe,0x42},
{0xbf,0x42},
{0xc0,0x42},
{0xc1,0x41},
{0xc2,0x41},
{0xc3,0x41},
{0xc4,0x41},
{0xc5,0x41},
{0xc6,0x41},
{0xca,0x70},
{0xcb,0x10},

{0xfd,0x00},
{0x32,0x15},
{0x34,0x26},
{0x35,0x40},
{0xff,0xff},
};


LOCAL SENSOR_REG_TAB_INFO_T s_SP0838_resolution_Tab_YUV[]=
{
    // COMMON INIT
    {ADDR_AND_LEN_OF_ARRAY(SP0838_YUV_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
    
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

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_SP0838_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    PNULL,
    PNULL,
    SP0838_Identify,

    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

    // External
    set_SP0838_ae_enable,
    set_hmirror_enable,
    set_vmirror_enable,

    set_brightness,
    set_contrast,
    set_sharpness,
    set_saturation,

    set_preview_mode,   
    set_image_effect,

    PNULL,  //  SP0838_BeforeSnapshot,
    PNULL,      //SP0838_After_Snapshot,

    PNULL,

    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate, 
    PNULL,
    PNULL,
    set_SP0838_awb,
    PNULL,
    PNULL,
    set_SP0838_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_SP0838_anti_flicker,
    set_SP0838_video_mode,
    PNULL,
    PNULL
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_SP0838_yuv_info =
{
    SP0838_I2C_ADDR_W,              // salve i2c write address
    SP0838_I2C_ADDR_R,              // salve i2c read address
    
    0,                              // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
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
    100,                                // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code
    0x02, 0x27,                     // supply two code to identify sensor.
    0x02, 0x27,                     // for Example: index = 0-> Device id, index = 1 -> version id  
                                    
    SENSOR_AVDD_2800MV,             // voltage of avdd  

    640,                            // max width of source image
    480,                            // max height of source image
    "SP0838",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_SP0838_resolution_Tab_YUV,    // point to resolution table information structure
    &s_SP0838_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor  
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1800MV,                      // dvdd
    3,
    0,
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
LOCAL void SP0838_WriteReg( uint8  subaddr, uint8 data )
{
    
    #ifndef _USE_DSP_I2C_
        //uint8 cmd[2];
        //cmd[0]    =   subaddr;
        //cmd[1]    =   data;       
        //I2C_WriteCmdArr(SP0838_I2C_ADDR_W, cmd, 2, SCI_TRUE);
        Sensor_WriteReg(subaddr, data);
    #else
        DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
    #endif

    //SCI_TRACE_LOW:"SENSOR: SP0838_WriteReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_469_112_2_18_0_32_40_1375,(uint8*)"dd", subaddr, data);

}

LOCAL uint8 SP0838_ReadReg( uint8  subaddr)
{
    uint8 value = 0;
    
    #ifndef _USE_DSP_I2C_
    //I2C_WriteCmdArrNoStop(SP0838_I2C_ADDR_W, &subaddr, 1,SCI_TRUE);
    //I2C_ReadCmd(SP0838_I2C_ADDR_R, &value, SCI_TRUE);
    value =Sensor_ReadReg( subaddr);
    #else
        value = (uint16)DSENSOR_IICRead((uint16)subaddr);
    #endif

    //SCI_TRACE_LOW:"SENSOR: SP0838_ReadReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_485_112_2_18_0_32_40_1376,(uint8*)"dd", subaddr, value);
    
    return value;
}


LOCAL uint32 SP0838_Identify(uint32 param)
{
#if 0 // by riyang.xie
#define SP0838_PID_VALUE    0x27    
#define SP0838_PID_ADDR     0x02
#define SP0838_VER_VALUE    0x27    
#define SP0838_VER_ADDR     0x02    

    uint32 i;
    uint32 nLoop;
    uint8 ret;
    uint32 err_cnt = 0;
    uint8 reg[2]    = {0x02, 0x02};
    uint8 value[2]  = {0x27, 0x27};

    //SCI_TRACE_LOW:"SP0838_Identify"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_506_112_2_18_0_32_40_1377,(uint8*)"");
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = SP0838_ReadReg(reg[i]);
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
                //SCI_TRACE_LOW:"It is not SP0838"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_516_112_2_18_0_32_40_1378,(uint8*)"");
                return SCI_ERROR;
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

    //SCI_TRACE_LOW:"SP0838_Identify: it is SP0838"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_531_112_2_18_0_32_40_1379,(uint8*)"");
    
    return (uint32)SCI_SUCCESS;
#else
    uint16  iden_reg_val = 0;
    uint32  ret = SENSOR_OP_ERR;

    iden_reg_val = SP0838_ReadReg(0x02);
    if(iden_reg_val == 0x27)
    {
            ret = SENSOR_OP_SUCCESS;
    }
    
    //SCI_TRACE_LOW:"SP0838_Identify: ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_544_112_2_18_0_32_40_1380,(uint8*)"d", ret);
    return ret;

#endif
}

LOCAL uint32 set_SP0838_ae_enable(uint32 enable)
{
    //SCI_TRACE_LOW:"set_SP0838_ae_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_552_112_2_18_0_32_40_1381,(uint8*)"d", enable);
    return 0;
}


LOCAL uint32 set_hmirror_enable(uint32 enable)
{
 
    //SCI_TRACE_LOW:"set_hmirror_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_560_112_2_18_0_32_40_1382,(uint8*)"d", enable);
    
    return 0;
}


LOCAL uint32 set_vmirror_enable(uint32 enable)
{

    //SCI_TRACE_LOW:"set_vmirror_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_569_112_2_18_0_32_40_1383,(uint8*)"d", enable);
    
    return 0;
}
/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T SP0838_brightness_tab[][4]=
{
//default 0xdc ,0x00
    {
        {0xfd,0x00},
        {0xdc,0xd0},
        {0xfd,0x00},
        {0xff,0xff},
    },

    {
        {0xfd,0x00},
        {0xdc,0xe0},
        {0xfd,0x00},
        {0xff,0xff},
    },

    {
        {0xfd,0x00},
        {0xdc,0xf0},
        {0xfd,0x00},
        {0xff,0xff},
    },

    {
        {0xfd,0x00},
        {0xdc,0x00},
        {0xfd,0x00}, //normal
        {0xff,0xff},
    },

    {
        {0xfd,0x00},
        {0xdc,0x10},
        {0xfd,0x00},
        {0xff,0xff},
    },

    {
        {0xfd,0x00},
        {0xdc,0x20},
        {0xfd,0x00},
        {0xff,0xff},
    },
    {       
        {0xfd,0x00},
        {0xdc,0x30},
        {0xfd,0x00},
        {0xff,0xff},
    },
};


LOCAL uint32 set_brightness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0838_brightness_tab[level];

    SCI_ASSERT(level < 7);/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        SP0838_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_644_112_2_18_0_32_41_1384,(uint8*)"d", level);
    
    return 0;
}

__align(4) const SENSOR_REG_T SP0838_ev_tab[][6]=
{   
    {//case 1
        {0xfd,0x00},    
        {0xf7,0x60},
        {0xf8,0x50},
        {0xdc,0xd0},
        {0xfd,0x00},
        {0xff,0xff},    
    },
    {//case 2
        {0xfd,0x00},    
        {0xf7,0x68}, 
        {0xf8,0x58}, 
        {0xdc,0xe0}, 
        {0xfd,0x00},
        {0xff,0xff},    
    },
    {//case 3
        {0xfd,0x00},    
        {0xf7,0x70}, 
        {0xf8,0x60}, 
        {0xdc,0xf0}, 
        {0xfd,0x00},
        {0xff,0xff},    
    },
    {//case 4
        {0xfd,0x00},    
        {0xf7,0x78},
        {0xf8,0x68},
        {0xdc,0x00}, //normal
        {0xfd,0x00},
        {0xff,0xff},    
    },
    {//case 5
        {0xfd,0x00},    
        {0xf7,0x80}, 
        {0xf8,0x70}, 
        {0xdc,0x10},   
        {0xfd,0x00},
        {0xff,0xff},    
    },
    {//case 6
        {0xfd,0x00},    
        {0xf7,0x90}, 
        {0xf8,0x80}, 
        {0xdc,0x20}, 
        {0xfd,0x00},
        {0xff,0xff},    
    },
    {//case 7
        {0xfd,0x00},    
        {0xf7,0xa0}, 
        {0xf8,0x90}, 
        {0xdc,0x30}, 
        {0xfd,0x00},
        {0xff,0xff},    
    },
};


LOCAL uint32 set_SP0838_ev(uint32 level)
{
    uint16 i; 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0838_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    SCI_ASSERT(level < 7);/*assert verified*/
 
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) ||(0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        SP0838_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    
    //SCI_TRACE_LOW:"SENSOR: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_724_112_2_18_0_32_41_1385,(uint8*)"d", level);

    return 0;
}

/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_SP0838_anti_flicker(uint32 param )
{ 
 #if 1
     switch(param)
    {
        case DCAMERA_FLICKER_50HZ:          
        SP0838_WriteReg(0xfd, 0x00); 
        SP0838_WriteReg(0xf0, 0x62); 
        SP0838_WriteReg(0xf2, 0x5f); 
        SP0838_WriteReg(0xf5, 0x78); 
        SP0838_WriteReg(0xfd, 0x01); 
        SP0838_WriteReg(0x00, 0xb8); 
        SP0838_WriteReg(0x0f, 0x60); 
        SP0838_WriteReg(0x16, 0x60); 
        SP0838_WriteReg(0x17, 0xa8); 
        SP0838_WriteReg(0x18, 0xb0); 
        SP0838_WriteReg(0x1b, 0x60);    
        SP0838_WriteReg(0x1c, 0xb0);    
        SP0838_WriteReg(0xb4, 0x20); 
        SP0838_WriteReg(0xb5, 0x3a); 
        SP0838_WriteReg(0xb6, 0x5e); 
        SP0838_WriteReg(0xcb, 0x10);    
        SP0838_WriteReg(0xfd, 0x00); 
        break;
            
        case DCAMERA_FLICKER_60HZ:
        SP0838_WriteReg(0xfd, 0x00);  
        SP0838_WriteReg(0xf0, 0x51);  
        SP0838_WriteReg(0xf2, 0x5c);  
        SP0838_WriteReg(0xf5, 0x75); 
        SP0838_WriteReg(0xfd, 0x01);  
        SP0838_WriteReg(0x00, 0xb9);  
        SP0838_WriteReg(0x0f, 0x5d);  
        SP0838_WriteReg(0x16, 0x5d); 
        SP0838_WriteReg(0x17, 0xa9);                    
        SP0838_WriteReg(0x18, 0xb1); 
        SP0838_WriteReg(0x1b, 0x5d); 
        SP0838_WriteReg(0x1c, 0xb1); 
        SP0838_WriteReg(0xb4, 0x21);  
        SP0838_WriteReg(0xb5, 0x3d);  
        SP0838_WriteReg(0xb6, 0x4d);  
        SP0838_WriteReg(0xcb, 0x14);        
        SP0838_WriteReg(0xfd, 0x00);  
        break;
            
        default:
            break;
    }
    
    SP0838_Delayms(100); 
#endif
    return 0;
}

/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//       
/******************************************************************************/
__align(4) const SENSOR_REG_T SP0838_video_mode_nor_tab[][38]=
{
    // normal mode 20~6fps
    {
    {0xfd,0x00},
    {0x05,0x0 },
    {0x06,0x0 },
    {0x09,0x1 },
    {0x0a,0x76},
    {0xf0,0x62},
    {0xf1,0x0 },
    {0xf2,0x5f},
    {0xf5,0x78},
    {0xfd,0x01},
    {0x00,0xb8},
    {0x0f,0x60},
    {0x16,0x60},
    {0x17,0xa8},
    {0x18,0xb0},
    {0x1b,0x60},
    {0x1c,0xb0},
    {0xb4,0x20},
    {0xb5,0x3a},
    {0xb6,0x5e},
    {0xb9,0x40},
    {0xba,0x4f},
    {0xbb,0x47},
    {0xbc,0x45},
    {0xbd,0x43},
    {0xbe,0x42},
    {0xbf,0x42},
    {0xc0,0x42},
    {0xc1,0x41},
    {0xc2,0x41},
    {0xc3,0x41},
    {0xc4,0x41},
    {0xc5,0x41},
    {0xc6,0x41},
    {0xca,0x70},
    {0xcb,0x10},
    {0xfd,0x00},
    {0xFF, 0xFF},

    },    
    //vodeo mode      fix 10fps
    {
    {0xfd,0x00},
    {0x05,0x0 },
    {0x06,0x0 },
    {0x09,0x6 },
    {0x0a,0x3e},
    {0xf0,0x31},
    {0xf1,0x0 },
    {0xf2,0x51},
    {0xf5,0x6a},
    {0xfd,0x01},
    {0x00,0xa0},
    {0x0f,0x52},
    {0x16,0x52},
    {0x17,0x90},
    {0x18,0x98},
    {0x1b,0x52},
    {0x1c,0x98},
    {0xb4,0x21},
    {0xb5,0x2f},
    {0xb6,0x2f},
    {0xb9,0x40},
    {0xba,0x4f},
    {0xbb,0x47},
    {0xbc,0x45},
    {0xbd,0x43},
    {0xbe,0x42},
    {0xbf,0x42},
    {0xc0,0x42},
    {0xc1,0x41},
    {0xc2,0x41},
    {0xc3,0x70},
    {0xc4,0x41},
    {0xc5,0x41},
    {0xc6,0x41},
    {0xca,0x70},
    {0xcb,0xa },
    {0xfd,0x00},
    {0xFF, 0xFF},

    },
   // UPCC  mode    20~6fps
    {
    {0xfd,0x00},
    {0x05,0x0 },
    {0x06,0x0 },
    {0x09,0x1 },
    {0x0a,0x76},
    {0xf0,0x62},
    {0xf1,0x0 },
    {0xf2,0x5f},
    {0xf5,0x78},
    {0xfd,0x01},
    {0x00,0xb8},
    {0x0f,0x60},
    {0x16,0x60},
    {0x17,0xa8},
    {0x18,0xb0},
    {0x1b,0x60},
    {0x1c,0xb0},
    {0xb4,0x20},
    {0xb5,0x3a},
    {0xb6,0x5e},
    {0xb9,0x40},
    {0xba,0x4f},
    {0xbb,0x47},
    {0xbc,0x45},
    {0xbd,0x43},
    {0xbe,0x42},
    {0xbf,0x42},
    {0xc0,0x42},
    {0xc1,0x41},
    {0xc2,0x41},
    {0xc3,0x41},
    {0xc4,0x41},
    {0xc5,0x41},
    {0xc6,0x41},
    {0xca,0x70},
    {0xcb,0x10},
    {0xfd,0x00},
    {0xFF, 0xFF},
    }  
};    

LOCAL uint32 set_SP0838_video_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;

    SCI_ASSERT(mode <=DCAMERA_MODE_MAX);/*assert verified*/


    if(CHIP_DetectMemType())
       
            sensor_reg_ptr = (SENSOR_REG_T*)SP0838_video_mode_nor_tab[mode];/*lint !e662*/

    else
       
            sensor_reg_ptr = (SENSOR_REG_T*)SP0838_video_mode_nor_tab[mode];/*lint !e662*/


    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        SP0838_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value); 
    }
    return 0;
}


/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
#if 0 // by riyang.xie
__align(4) const SENSOR_REG_T SP0838_awb_tab[][6]=

{
 #if 0
     //AUTO
    {
        {0x41, 0x3d},
        {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
        {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x80},
        {0xff, 0xff}         
    },
    //U30 ?
    {
       {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff}      
    },  
    //CWF ?
    {
        {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff}            
    },    
    //FLUORESCENT:
    {
        {0x41, 0x39},
        {0xca, 0x50},
        {0xcb, 0x40},
        {0xcc, 0x70},
        {0xff, 0xff}          
    },
    //SUN:
    {
        {0x41, 0x39},
        {0xca, 0x5a},
        {0xcb, 0x40},
        {0xcc, 0x58},
        {0xff, 0xff}           
    },
    //CLOUD:
    {
        {0x41, 0x39},
        {0xca, 0x68},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff}            
    },
    #endif
    {0xff,0xff}
};
#endif

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
__align(4) const SENSOR_REG_T SP0838_awb_tab[][7]=
    {

        //AUTO
        {
            {0xfd,0x01},                      
            {0x28,0x61},                          
            {0x29,0x49},                      
            {0xfd,0x00},  // AUTO 3000K~7000K 
            {0x32,0x15},   
            {0xfd,0x00},  
            {0xff,0xff} 
        },    
        //INCANDESCENCE:
        {
            {0xfd,0x00},  //2800K~3000K  
            {0x32,0x05},                 
            {0xfd,0x01},                 
            {0x28,0x41},                     
            {0x29,0x71},                     
            {0xfd,0x00}, 
            {0xff,0xff} 
        },
        //U30 //???????
        {
            {0xfd,0x00},//4000K
            {0x32,0x05},       
            {0xfd,0x01},       
            {0x28,0x57},         
            {0x29,0x66},         
            {0xfd,0x00},     
            {0xff,0xff} 
        },  
        //CWF  //???????
        {
            {0xfd,0x00},//4000K
            {0x32,0x05},       
            {0xfd,0x01},       
            {0x28,0x57},         
            {0x29,0x66},         
            {0xfd,0x00},     
            {0xff,0xff} 
        },    
        //FLUORESCENT:
        {
            {0xfd,0x00},  //4200K~5000K   
            {0x32,0x05},                  
            {0xfd,0x01},                  
            {0x28,0x5a},                      
            {0x29,0x62},                      
            {0xfd,0x00},
            {0xff,0xff} 
        },
        //SUN:
        {
            {0xfd,0x00}, //6500K   
            {0x32,0x05},           
            {0xfd,0x01},           
            {0x28,0x6b},             
            {0x29,0x48},             
            {0xfd,0x00},  
            {0xff,0xff} 
        },
                //CLOUD:
        {
            {0xfd,0x00},  //7000K 
            {0x32,0x05},          
            {0xfd,0x01},          
            {0x28,0x71},            
            {0x29,0x41},            
            {0xfd,0x00},
            {0xff,0xff} 
        },  
};
    
    LOCAL uint32 set_SP0838_awb(uint32 mode)
    {
        uint16 i;
        
        SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0838_awb_tab[mode];
    
        SCI_ASSERT(mode < 7);/*assert verified*/
        SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
        // hill 0408
        for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
        {
            SP0838_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
        }
        SCI_Sleep(100); 
        //SCI_TRACE_LOW:"SENSOR: set_awb_mode: mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_1119_112_2_18_0_32_42_1386,(uint8*)"d", mode);
        
        return 0;
}

__align(4) const SENSOR_REG_T SP0838_contrast_tab[][4]=
{
//default 0xde ,0xa0
    {
        {0xfd, 0x00},
        {0xde, 0x50},
        {0xfd, 0x00},
        {0xff, 0xff}
    },

    {
        {0xfd, 0x00},
        {0xde, 0x60},
        {0xfd, 0x00},
        {0xff, 0xff}
    },

    {
        {0xfd, 0x00},
        {0xde, 0x70},
        {0xfd, 0x00},
        {0xff, 0xff}
    },

    {
        {0xfd, 0x00},
        {0xde, 0x88},//normal
        {0xfd, 0x00},
        {0xff, 0xff}
    },

    {
        {0xfd, 0x00},
        {0xde, 0x90},
        {0xfd, 0x00},
        {0xff, 0xff}
    },

    {
        {0xfd, 0x00},
        {0xde, 0xa0},
        {0xfd, 0x00},
        {0xff, 0xff}
    },

    {
        {0xfd, 0x00},
        {0xde, 0xb0},
        {0xfd, 0x00},
        {0xff, 0xff}
    }, 
};


LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)SP0838_contrast_tab[level];

    SCI_ASSERT(level < 7 );/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        SP0838_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(20);
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_1193_112_2_18_0_32_42_1387,(uint8*)"d", level);
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_1220_112_2_18_0_32_42_1388,(uint8*)"d", preview_mode);
    switch (preview_mode)
    {
    
        case DCAMERA_ENVIRONMENT_NORMAL: //fix 15fps
        { 
        SP0838_WriteReg(0xfd, 0x01);            
        SP0838_WriteReg(0x14, 0x20);
        SP0838_WriteReg(0x15, 0x0f);    
    
        SP0838_WriteReg(0xfd,0x00);
        SP0838_WriteReg(0x05,0x0 );
        SP0838_WriteReg(0x06,0x0 );
        SP0838_WriteReg(0x09,0x1 );
        SP0838_WriteReg(0x0a,0x76);
        SP0838_WriteReg(0xf0,0x62);
        SP0838_WriteReg(0xf1,0x0 );
        SP0838_WriteReg(0xf2,0x5f);
        SP0838_WriteReg(0xf5,0x78);
        SP0838_WriteReg(0xfd,0x01);
        SP0838_WriteReg(0x00,0xb8);
        SP0838_WriteReg(0x0f,0x60);
        SP0838_WriteReg(0x16,0x60);
        SP0838_WriteReg(0x17,0xa8);
        SP0838_WriteReg(0x18,0xb0);
        SP0838_WriteReg(0x1b,0x60);
        SP0838_WriteReg(0x1c,0xb0);
        SP0838_WriteReg(0xb4,0x20);
        SP0838_WriteReg(0xb5,0x3a);
        SP0838_WriteReg(0xb6,0x5e);
        SP0838_WriteReg(0xb9,0x40);
        SP0838_WriteReg(0xba,0x4f);
        SP0838_WriteReg(0xbb,0x47);
        SP0838_WriteReg(0xbc,0x45);
        SP0838_WriteReg(0xbd,0x43);
        SP0838_WriteReg(0xbe,0x42);
        SP0838_WriteReg(0xbf,0x42);
        SP0838_WriteReg(0xc0,0x42);
        SP0838_WriteReg(0xc1,0x41);
        SP0838_WriteReg(0xc2,0x41);
        SP0838_WriteReg(0xc3,0x41);
        SP0838_WriteReg(0xc4,0x41);
        SP0838_WriteReg(0xc5,0x41);
        SP0838_WriteReg(0xc6,0x41);
        SP0838_WriteReg(0xca,0x70);
        SP0838_WriteReg(0xcb,0x10);
        SP0838_WriteReg(0xfd,0x00);

        break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT://4
        {
        SP0838_WriteReg(0xfd, 0x01);        
        SP0838_WriteReg(0x14, 0x20);
        SP0838_WriteReg(0x15, 0x1f);    
        

        SP0838_WriteReg(0xfd,0x00);
        SP0838_WriteReg(0x05,0x0 );
        SP0838_WriteReg(0x06,0x0 );
        SP0838_WriteReg(0x09,0x1 );
        SP0838_WriteReg(0x0a,0x76);
        SP0838_WriteReg(0xf0,0x62);
        SP0838_WriteReg(0xf1,0x0 );
        SP0838_WriteReg(0xf2,0x5f);
        SP0838_WriteReg(0xf5,0x78);
        SP0838_WriteReg(0xfd,0x01);
        SP0838_WriteReg(0x00,0xb8);
        SP0838_WriteReg(0x0f,0x60);
        SP0838_WriteReg(0x16,0x60);
        SP0838_WriteReg(0x17,0xa8);
        SP0838_WriteReg(0x18,0xb0);
        SP0838_WriteReg(0x1b,0x60);
        SP0838_WriteReg(0x1c,0xb0);
        SP0838_WriteReg(0xb4,0x20);
        SP0838_WriteReg(0xb5,0x3a);
        SP0838_WriteReg(0xb6,0x5e);
        SP0838_WriteReg(0xb9,0x40);
        SP0838_WriteReg(0xba,0x4f);
        SP0838_WriteReg(0xbb,0x47);
        SP0838_WriteReg(0xbc,0x45);
        SP0838_WriteReg(0xbd,0x43);
        SP0838_WriteReg(0xbe,0x42);
        SP0838_WriteReg(0xbf,0x42);
        SP0838_WriteReg(0xc0,0x42);
        SP0838_WriteReg(0xc1,0x41);
        SP0838_WriteReg(0xc2,0x41);
        SP0838_WriteReg(0xc3,0x41);
        SP0838_WriteReg(0xc4,0x41);
        SP0838_WriteReg(0xc5,0x41);
        SP0838_WriteReg(0xc6,0x41);
        SP0838_WriteReg(0xca,0x70);
        SP0838_WriteReg(0xcb,0x10);
        SP0838_WriteReg(0xfd,0x00);

        break;
        }
        case DCAMERA_ENVIRONMENT_SUNNY://fix 15fps
        {
        SP0838_WriteReg(0xfd, 0x01);            
        SP0838_WriteReg(0x14, 0x20);
        SP0838_WriteReg(0x15, 0x0f);    

        SP0838_WriteReg(0xfd,0x00);
        SP0838_WriteReg(0x05,0x0 );
        SP0838_WriteReg(0x06,0x0 );
        SP0838_WriteReg(0x09,0x1 );
        SP0838_WriteReg(0x0a,0x76);
        SP0838_WriteReg(0xf0,0x62);
        SP0838_WriteReg(0xf1,0x0 );
        SP0838_WriteReg(0xf2,0x5f);
        SP0838_WriteReg(0xf5,0x78);
        SP0838_WriteReg(0xfd,0x01);
        SP0838_WriteReg(0x00,0xb8);
        SP0838_WriteReg(0x0f,0x60);
        SP0838_WriteReg(0x16,0x60);
        SP0838_WriteReg(0x17,0xa8);
        SP0838_WriteReg(0x18,0xb0);
        SP0838_WriteReg(0x1b,0x60);
        SP0838_WriteReg(0x1c,0xb0);
        SP0838_WriteReg(0xb4,0x20);
        SP0838_WriteReg(0xb5,0x3a);
        SP0838_WriteReg(0xb6,0x5e);
        SP0838_WriteReg(0xb9,0x40);
        SP0838_WriteReg(0xba,0x4f);
        SP0838_WriteReg(0xbb,0x47);
        SP0838_WriteReg(0xbc,0x45);
        SP0838_WriteReg(0xbd,0x43);
        SP0838_WriteReg(0xbe,0x42);
        SP0838_WriteReg(0xbf,0x42);
        SP0838_WriteReg(0xc0,0x42);
        SP0838_WriteReg(0xc1,0x41);
        SP0838_WriteReg(0xc2,0x41);
        SP0838_WriteReg(0xc3,0x41);
        SP0838_WriteReg(0xc4,0x41);
        SP0838_WriteReg(0xc5,0x41);
        SP0838_WriteReg(0xc6,0x41);
        SP0838_WriteReg(0xca,0x70);
        SP0838_WriteReg(0xcb,0x10);
        SP0838_WriteReg(0xfd,0x00);

        break;
        }
        default:
        {
            break;
        }


    } 
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

__align(4) const SENSOR_REG_T SP0838_image_effect_tab[][6]= 
{

    // effect normal
    {
        {0xfd, 0x00},
        {0x62, 0x00},
        {0x63, 0x80},
        {0x64, 0x80},
        {0xfd, 0x00},
        {0xff, 0xff}
    },
    //effect BLACKWHITE
    {
        {0xfd, 0x00},
        {0x62, 0x40},
        {0x63, 0x80},
        {0x64, 0x80},
        {0xfd, 0x00},
        {0xff, 0xff}
    },
    // effect RED pink
    {
        {0xfd, 0x00},
        {0x62, 0x20},
        {0x63, 0xc0},
        {0x64, 0x70},//0x20
        {0xfd, 0x00},
        {0xff, 0xff}
    },
    // effect GREEN
    {
        {0xfd, 0x00},
        {0x62, 0x20},
        {0x63, 0x20},
        {0x64, 0x20},
        {0xfd, 0x00},
        {0xff, 0xff}
    },
    // effect  BLUE
    {
        {0xfd, 0x00},
        {0x62, 0x20},
        {0x63, 0x20},
        {0x64, 0xf0},
        {0xfd, 0x00},
        {0xff, 0xff}
    },
    // effect  YELLOW
    {
        //TODO:later work
        {0xfd, 0x00},
        {0x62, 0x20},
        {0x63, 0xa0},
        {0x64, 0x10},
        {0xfd, 0x00},
        {0xff, 0xff}

    },  
    // effect NEGATIVE
    {        
        {0xfd, 0x00},
        {0x62, 0x10},
        {0x63, 0x80},
        {0x64, 0x80},
        {0xfd, 0x00},
        {0xff, 0xff}
    },    
    //effect ANTIQUE
    {
        {0xfd, 0x00},
        {0x62, 0x20},
        {0x63, 0xa0},
        {0x64, 0x50},
        {0xfd, 0x00},
        {0xff, 0xff}
    },
};
LOCAL uint32 set_image_effect(uint32 effect_type)
{
    uint16 i;
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0838_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"-----------set_image_effect: effect_type = %d------------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_1477_112_2_18_0_32_42_1389,(uint8*)"d", effect_type);
    
    return 0;
}

/*
LOCAL uint32 SP0838_After_Snapshot(uint32 param)
{

    Sensor_SetMCLK(24);
    
    SP0838_WriteReg(0x41,SP0838_ReadReg(0x41) | 0xf7);
    SCI_Sleep(200);
    return 0;
    
}

LOCAL uint32 SP0838_BeforeSnapshot(uint32 param)
{

    uint16 shutter = 0x00;
    uint16 temp_reg = 0x00;
    uint16 temp_r =0x00;
    uint16 temp_g =0x00;
    uint16 temp_b =0x00;    
    BOOLEAN b_AEC_on;
    

    //SCI_TRACE_LOW:"SP0838_BeforeSnapshot "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_1504_112_2_18_0_32_42_1390,(uint8*)"");
        if(SP0838_ReadReg(0X41)  & 0x08 == 0x08)  //AEC on
            b_AEC_on = SCI_TRUE;
        else
            b_AEC_on = SCI_FALSE;

    temp_reg = SP0838_ReadReg(0xdb);
    temp_r = SP0838_ReadReg(0xcd);
    temp_g = SP0838_ReadReg(0xce);
    temp_b = SP0838_ReadReg(0xcf);

    shutter = (SP0838_ReadReg(0x03)<<8)  | (SP0838_ReadReg(0x04)&0x00ff) ;
    shutter = shutter /2;

    if(b_AEC_on)
        SP0838_WriteReg(0x41,SP0838_ReadReg(0x41) & 0xc5); //0x01);
    SCI_Sleep(300); 

///12m
    Sensor_SetMCLK(12);
    
    SP0838_WriteReg(0x03,shutter/256);
    SP0838_WriteReg(0x04,shutter & 0x00ff); 
    //SCI_TRACE_LOW("SP0838_BeforeSnapshot, temp_r=%x,temp_reg=%x, final = %x ",temp_r,temp_reg, temp_r*temp_reg/ 0x80);    

    temp_r = (temp_r*temp_reg) / 0x80;
    temp_g = (temp_g*temp_reg) / 0x80;
    temp_b = (temp_b*temp_reg) / 0x80;
    if(b_AEC_on)
    {
        SP0838_WriteReg(0xcd, temp_r);
        SP0838_WriteReg(0xce, temp_g);
        SP0838_WriteReg(0xcf , temp_b);
    }
    //SCI_TRACE_LOW("SP0838_BeforeSnapshot, temp_r=%x,temp_g=%x, temp_b = %x ",temp_r,temp_g,temp_b);    

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
    //SP0838_WriteReg( 0xd8, uint8 data );
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T SP0838_mode_tab[][5]=
{
#if  0
    //LCD的GAMMA值需要细调，不然会有一圈圈的光晕
    //Fps 12.5 YUV open auto frame function, 展讯的jpeg编码不行太大和太快，因此将帧率限制在12.5fps
    {/*{0xa0, 0x50},*/{0xec, 0x20},{0xFF, 0xFF},},
    //Fps 12.5->3.125 YUV open auto frame function
    {/*{0xa0, 0x40},*/{0xd8, 0x30},{0xFF, 0xFF},},
    
    {0xff,0xff}
#endif

     {    
        {0xfd,0x01},     
        {0x14,0x20},
        {0x15,0x0f},
        {0xfd,0x00},     
        {0xff,0xff},
        
    },
    //night
    {    
        {0xfd,0x01},     
        {0x14,0x20},
        {0x15,0x1f},
        {0xfd,0x00},     
        {0xff,0xff},
    }
};

LOCAL uint32 SP0838_set_work_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0838_mode_tab[mode];

    SCI_ASSERT(mode <= 1);/*assert verified*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);/*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        SP0838_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"set_work_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SP0838_1632_112_2_18_0_32_43_1391,(uint8*)"d", mode);
    return 0;
}
