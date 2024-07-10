/******************************************************************************
 ** File Name:    Sensor_SP0A38.c                                             *
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
#ifndef _SP0A38_C_
#define _SP0A38_C_

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
#define SP0A38_I2C_ADDR_W               0x42
#define SP0A38_I2C_ADDR_R               0x43

/**---------------------------------------------------------------------------*
     SP0A38 SUPPORT 10 SERIAL MODE:
     SPI, 
     CCIR656_1BIT, CCIR656_2BIT, CCIR656_4BIT
     PACKET_1BIT, PACKET_2BIT, PACKET_4BIT
     PACKET_DDR_1BIT, PACKET_DDR_2BIT, PACKET_DDR_4BIT
 **---------------------------------------------------------------------------*/

//#define SP0A38_OUTPUT_MODE_CCIR601_8BIT
//#define SP0A38_OUTPUT_MODE_SPI
//#define SP0A38_OUTPUT_MODE_CCIR656_1BIT
//#define SP0A38_OUTPUT_MODE_CCIR656_2BIT
//#define SP0A38_OUTPUT_MODE_CCIR656_4BIT
//#define SP0A38_OUTPUT_MODE_PACKET_1BIT
//#define SP0A38_OUTPUT_MODE_PACKET_2BIT
//#define SP0A38_OUTPUT_MODE_PACKET_4BIT
//#define SP0A38_OUTPUT_MODE_PACKET_DDR_1BIT
#define SP0A38_OUTPUT_MODE_PACKET_DDR_2BIT 
//#define SP0A38_OUTPUT_MODE_PACKET_DDR_4BIT

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void   SP0A38_WriteReg(uint8 subaddr, uint8 data );
LOCAL uint8  SP0A38_ReadReg(uint8 subaddr);
LOCAL void   SP0A38_Write_Group_Regs(SENSOR_REG_T* sensor_reg_ptr );
LOCAL uint32 SP0A38_Power_On(uint32 power_on);
LOCAL uint32 SP0A38_Identify(uint32 param);
LOCAL uint32 SP0A38_Set_Brightness(uint32 level);
LOCAL uint32 SP0A38_Set_Contrast(uint32 level);
LOCAL uint32 SP0A38_Set_Image_Effect(uint32 effect_type);
LOCAL uint32 SP0A38_Set_Ev(uint32 level);
LOCAL uint32 SP0A38_Set_Anti_Flicker(uint32 mode);
LOCAL uint32 SP0A38_Set_Preview_Mode(uint32 preview_mode);
LOCAL uint32 SP0A38_Set_AWB(uint32 mode);
LOCAL uint32 SP0A38_Set_Video_Mode(uint32 mode);
LOCAL uint32 SP0A38_Before_Snapshot(uint32 para);
LOCAL uint32 SP0A38_After_Snapshot(uint32 para);
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
#ifdef SP0A38_OUTPUT_MODE_CCIR601_8BIT

#else
__align(4) const SENSOR_REG_T SP0A38_YUV_640X480[] = 
{
//;ae setting  fix 6fps
    {0xfd,0x00},
  {0x03,0x00},
  {0x04,0x5a},
  {0x05,0x00},
  {0x06,0x00},
  {0x07,0x00},
  {0x08,0x00},
  {0x09,0x00},
  {0x0a,0xef},
  {0xfd,0x01},
  {0xf7,0x0f},
  {0x02,0x21},
  {0x03,0x01},
  {0x06,0x0f},
  {0x07,0x00},
  {0x08,0x01},
  {0x09,0x00},
  {0xfd,0x02},
  {0xbe,0xef},
  {0xbf,0x01},
  {0xd0,0xef},
  {0xd1,0x01},

//config setting
  {0xfd,0x00},
  {0x30,0x01},//0x01 0x02
  {0x1b,0x30},
  {0x1e,0x94},
  {0x1c,0x1f},
  {0xe7,0x03},
  {0x2c,0x1d},//0x1d //04 15
  {0x2d,0x40},
  {0x2e,0xf9},/* 2bit ddr mode */
  {0xfd,0x01},
  {0x32,0x15},
  {0x33,0xef},
  {0x34,0xef},
};
#endif

LOCAL SENSOR_REG_TAB_INFO_T s_SP0A38_resolution_Tab_YUV[]=
{   
    { ADDR_AND_LEN_OF_ARRAY(SP0A38_YUV_640X480), 640, 480,  24, SENSOR_IMAGE_FORMAT_YUV422 },
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

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_SP0A38_ioctl_func_tab = 
{
// Internal 
    PNULL,
    SP0A38_Power_On, 
    PNULL,
    SP0A38_Identify,
    PNULL,
    PNULL,
// Custom function
    PNULL,
    PNULL,

// External
    PNULL, 
    PNULL, //SP0A38_Set_Mirror,
    PNULL, //SP0A38_Set_Flip,

    SP0A38_Set_Brightness,
    SP0A38_Set_Contrast,
    PNULL, //SP0A38_Set_Sharpness,
    PNULL, //SP0A38_Set_saturation,
    SP0A38_Set_Preview_Mode, 

    SP0A38_Set_Image_Effect, 
    PNULL,//SP0A38_Before_Snapshot,
    PNULL,//SP0A38_After_Snapshot,

    PNULL,

    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,

    PNULL,
    PNULL,

    SP0A38_Set_AWB,

    PNULL,

    PNULL, 
    SP0A38_Set_Ev,

    PNULL, 
    PNULL, 
    PNULL,

    PNULL,
    PNULL,
    SP0A38_Set_Anti_Flicker,
    SP0A38_Set_Video_Mode,

    PNULL,
    PNULL
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC const SENSOR_INFO_T g_SP0A38_yuv_info =
{
    SP0A38_I2C_ADDR_W,              // salve i2c write address
    SP0A38_I2C_ADDR_R,              // salve i2c read address
    
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
    0x01, 0x0A,                     // supply two code to identify sensor.
    0x02, 0x10,                     // for Example: index = 0-> Device id, index = 1 -> version id                                          
                                            
    SENSOR_AVDD_2800MV,             // voltage of avdd  
    
    640,                            // max width of source image
    480,                            // max height of source image
    "SP0A38",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_SP0A38_resolution_Tab_YUV,    // point to resolution table information structure
    &s_SP0A38_ioctl_func_tab,       // point to ioctl function table
            
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
#ifdef SP0A38_OUTPUT_MODE_SPI
    SENSOR_OUTPUT_MODE_SPI_1BIT,
#elif defined(SP0A38_OUTPUT_MODE_CCIR656_1BIT)
    SENSOR_OUTPUT_MODE_CCIR656_1BIT,
#elif defined(SP0A38_OUTPUT_MODE_CCIR656_2BIT)
    SENSOR_OUTPUT_MODE_CCIR656_2BIT,
#elif defined(SP0A38_OUTPUT_MODE_CCIR656_4BIT)
    SENSOR_OUTPUT_MODE_CCIR656_4BIT,
#elif defined(SP0A38_OUTPUT_MODE_PACKET_1BIT)
    SENSOR_OUTPUT_MODE_PACKET_1BIT, 
#elif defined(SP0A38_OUTPUT_MODE_PACKET_2BIT)
    SENSOR_OUTPUT_MODE_PACKET_2BIT, 
#elif defined(SP0A38_OUTPUT_MODE_PACKET_4BIT)
    SENSOR_OUTPUT_MODE_PACKET_4BIT, 
#elif defined(SP0A38_OUTPUT_MODE_PACKET_DDR_1BIT)
    SENSOR_OUTPUT_MODE_PACKET_DDR_1BIT, 
#elif defined(SP0A38_OUTPUT_MODE_PACKET_DDR_2BIT)
    SENSOR_OUTPUT_MODE_PACKET_DDR_2BIT, 
#elif defined(SP0A38_OUTPUT_MODE_PACKET_DDR_4BIT)
    SENSOR_OUTPUT_MODE_PACKET_DDR_4BIT, 
#else
    SENSOR_OUTPUT_MODE_CCIR601_8BIT, 
#endif
    SENSOR_OUTPUT_ENDIAN_BIG
};

/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void SP0A38_WriteReg(uint8 subaddr, uint8 data )
{
    Sensor_WriteReg(subaddr, data);
}

LOCAL uint8 SP0A38_ReadReg(uint8 subaddr)
{
    uint8 value = 0;
    
    value = Sensor_ReadReg( subaddr);
    
    return value;
}

LOCAL void SP0A38_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

}

LOCAL uint32 SP0A38_Power_On(uint32 power_on)
{
   // SENSOR_AVDD_VAL_E dvdd_val=g_SP0A38_yuv_info.dvdd_val;
   // SENSOR_AVDD_VAL_E avdd_val=g_SP0A38_yuv_info.avdd_val;
    //SENSOR_AVDD_VAL_E iovdd_val=g_SP0A38_yuv_info.iovdd_val;  
  //  uint32 power_down=g_SP0A38_yuv_info.power_down_level;
    
    
    //SCI_TRACE_LOW("SENSOR: _SP0A38_On(1:on, 0:off): %d", power_on);    
    
    return SCI_SUCCESS;
}

LOCAL uint32 SP0A38_Identify(uint32 param)
{
    uint8 reg[2] = {0x01, 0x02};
    uint8 value[2] = {0x0a, 0x10};
    uint8 ret = 0;
    uint8 err_cnt = 0;
    uint32 i = 0;
    uint32 nLoop = 1000;
    
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = SP0A38_ReadReg(reg[i]);    
        
     	SCI_TRACE_LOW("SENSOR: SP0A38 Read reg0x00 = %x");
        
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
                 SCI_TRACE_LOW("SP0A38 identify Fail");
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

    SCI_TRACE_LOW("SP0A38 Identify ok");

    return (uint32)SCI_SUCCESS;
}

__align(4) const SENSOR_REG_T SP0A38_brightness_tab[][2]=
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

LOCAL uint32 SP0A38_Set_Brightness(uint32 level)
{

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0A38_brightness_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/ 
    
    SP0A38_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("SP0A38_Set_Brightness: level = %d");
    
    return 0;
}

__align(4) const SENSOR_REG_T SP0A38_contrast_tab[][2]=
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

LOCAL uint32 SP0A38_Set_Contrast(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0A38_contrast_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    
    SP0A38_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("SP0A38_Set_Contrast: level = %d");
    
    return 0;
}

__align(4) const SENSOR_REG_T SP0A38_image_effect_tab[][4]= 
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

LOCAL uint32 SP0A38_Set_Image_Effect(uint32 effect_type)
{
   
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0A38_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 

    SP0A38_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("SP0A38_Set_Image_Effect: effect_type = %d");
    
    return 0;
}

__align(4) const SENSOR_REG_T SP0A38_ev_tab[][4]=
{   
    {{0xfe, 0x01},{0x33, 0x30},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x38},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x40},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x48},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x50},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x58},{0xfe, 0x00},{0xff, 0xff}},
    {{0xfe, 0x01},{0x33, 0x60},{0xfe, 0x00},{0xff, 0xff}},  
};

LOCAL uint32 SP0A38_Set_Ev(uint32 level)
{

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0A38_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    SCI_ASSERT(level < 7);    /*assert verified*/ 
 
    SP0A38_Write_Group_Regs(sensor_reg_ptr );

    SCI_TRACE_LOW("SP0A38_Set_Ev: level = %d");

    return 0;
}

LOCAL uint32 SP0A38_Set_Anti_Flicker(uint32 mode)
{ 
    switch(mode)
    {
		case DCAMERA_FLICKER_50HZ:

		break;

		case DCAMERA_FLICKER_60HZ:

		break;

		default:
		break;
    }
      
    SCI_TRACE_LOW("SP0A38_Set_Anti_Flicker-mode=%d");
    
    return 0;
}

LOCAL uint32 SP0A38_Set_Preview_Mode(uint32 preview_mode)
{
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            break;
        }
        default:
        {
            break;
        }
        
    }
	SCI_TRACE_LOW("SP0A38_Set_Preview_Mode: level = %d");
	return 0;
}

__align(4) const SENSOR_REG_T SP0A38_awb_tab[][5]=
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

LOCAL uint32 SP0A38_Set_AWB(uint32 mode)
{
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0A38_awb_tab[mode];

    SCI_ASSERT(mode < 7);    /*assert verified*/ 
    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/ 
    
    SP0A38_Write_Group_Regs(sensor_reg_ptr);
    
    SCI_TRACE_LOW("SP0A38_Set_AWB: mode = %d");
    
    return 0;
}

__align(4) const SENSOR_REG_T SP0A38_video_mode_nor_tab[][4]=
{
    // normal mode 
    {{0xfe , 0x01},{0x33 , 0x00},{0xfe , 0x00},{0xff , 0xff}},    
    //vodeo mode
    {{0xfe , 0x01},{0x33 , 0x00},{0xfe , 0x00},{0xff , 0xff}},
    // UPCC  mode
    {{0xfe , 0x01},{0x33 , 0x00},{0xfe , 0x00},{0xff , 0xff}}
};    

LOCAL uint32 SP0A38_Set_Video_Mode(uint32 mode)
{
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)SP0A38_video_mode_nor_tab[mode];

    if(PNULL == sensor_reg_ptr||mode > DCAMERA_MODE_MAX)
    {
        return SCI_ERROR;
    }

    SP0A38_Write_Group_Regs(sensor_reg_ptr );

    SCI_TRACE_LOW("SP0A38_Set_Video_Mode =%d");

    return 0;
}

LOCAL uint32 SP0A38_Before_Snapshot(uint32 para)
{

    return 0;
}

LOCAL uint32 SP0A38_After_Snapshot(uint32 para)
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

#endif
