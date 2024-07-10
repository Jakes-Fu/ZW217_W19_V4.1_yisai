/******************************************************************************
 ** File Name:      sensor_drv.h                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           04/20/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of sensor*
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/20/2006     Liangwen.Zhen     Create.                                  *
 ******************************************************************************/

#ifndef _SENSOR_DRV_H_
#define _SENSOR_DRV_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h"
#include "os_api.h"
#include "sensor_atv.h"
#include "jpeg_interface.h"


/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
 
#define NUMBER_OF_ARRAY(a)                  (sizeof(a)/sizeof(a[0]))
#define ADDR_AND_LEN_OF_ARRAY(a)            (SENSOR_REG_T*)a, NUMBER_OF_ARRAY(a)


#define SENSOR_DISABLE_MCLK                 0       // MHZ
#define SENSOR_DEFALUT_MCLK                 12      // MHZ

#define SENSOR_LOW_PULSE_RESET              0x00
#define SENSOR_HIGH_PULSE_RESET             0x01

#define SENSOR_RESET_PULSE_WIDTH_DEFAULT    20          // ms
#define SENSOR_RESET_PULSE_WIDTH_MAX        200     // ms

#define SENSOR_LOW_LEVEL_PWDN               0x00
#define SENSOR_HIGH_LEVEL_PWDN              0x01

#define SENSOR_IDENTIFY_CODE_COUNT          0x02

// Image effect
#define SENSOR_IMAGE_EFFECT_NORMAL          (0x01 << 0)     
#define SENSOR_IMAGE_EFFECT_BLACKWHITE      (0x01 << 1)     //  black/white
#define SENSOR_IMAGE_EFFECT_RED             (0x01 << 2)     // red
#define SENSOR_IMAGE_EFFECT_GREEN           (0x01 << 3)     // green
#define SENSOR_IMAGE_EFFECT_BLUE            (0x01 << 4)     // blue
#define SENSOR_IMAGE_EFFECT_YELLOW          (0x01 << 5)     // yellow
#define SENSOR_IMAGE_EFFECT_NEGATIVE        (0x01 << 6)     // negative
#define SENSOR_IMAGE_EFFECT_CANVAS          (0x01 << 7)     // Sepia
#define SENSOR_IMAGE_EFFECT_RELIEVOS        (0x01 << 8)     // Embossment
#define SENSOR_IMAGE_EFFECT_AQUA            (0x01 << 9)     // Aqua

// While balance mode
#define SENSOR_WB_MODE_AUTO                 (0x01 << 0)     //auto
#define SENSOR_WB_MODE_INCANDESCENCE        (0x01 << 1)     //incandescent
#define SENSOR_WB_MODE_U30                  (0x01 << 2)     //u30
#define SENSOR_WB_MODE_CWF                  (0x01 << 3)     //cwf
#define SENSOR_WB_MODE_FLUORESCENT          (0x01 << 4)     //fluoresecent
#define SENSOR_WB_MODE_SUN                  (0x01 << 5)     //daylight
#define SENSOR_WB_MODE_CLOUD                (0x01 << 6)     //cloudy

// Preview mode
#define SENSOR_ENVIROMENT_NORMAL            (0x01 << 0)     // 室内环境
#define SENSOR_ENVIROMENT_NIGHT             (0x01 << 1)     // 夜间或着弱光环境�?
#define SENSOR_ENVIROMENT_SUNNY             (0x01 << 2)     // 室外或着强光环境�?
#define SENSOR_ENVIROMENT_SPORTS            (0x01 << 3)     // 运动模式
#define SENSOR_ENVIROMENT_LANDSCAPE         (0x01 << 4)     // 风景模式
#define SENSOR_ENVIROMENT_PORTRAIT          (0x01 << 5)     // 人物/肖像
#define SENSOR_ENVIROMENT_PORTRAIT_NIGHT    (0x01 << 6)     // 夜间人物/肖像
#define SENSOR_ENVIROMENT_BACKLIGHT         (0x01 << 7)     // 逆光拍摄
#define SENSOR_ENVIROMENT_MARCO             (0x01 << 8)     // 微距

#define SENSOR_ENVIROMENT_MANUAL            (0x01 << 30)    // 手动模式
#define SENSOR_ENVIROMENT_AUTO              (0x01 << 31)    // 自动模式

// YUV PATTERN
#define SENSOR_IMAGE_PATTERN_YUV422_YUYV    0x00
#define SENSOR_IMAGE_PATTERN_YUV422_YVYU    0x01
#define SENSOR_IMAGE_PATTERN_YUV422_UYVY    0x02
#define SENSOR_IMAGE_PATTERN_YUV422_VYUY    0x03
// RAW RGB BAYER
#define SENSOR_IMAGE_PATTERN_RAWRGB_GR      0x00
#define SENSOR_IMAGE_PATTERN_RAWRGB_R       0x01
#define SENSOR_IMAGE_PATTERN_RAWRGB_B       0x02
#define SENSOR_IMAGE_PATTERN_RAWRGB_GB      0x03

// I2C REG/VAL BIT count
#define SENSOR_I2C_VAL_8BIT                 0x00
#define SENSOR_I2C_VAL_16BIT                0x01
#define SENSOR_I2C_REG_8BIT                 (0x00 << 1)
#define SENSOR_I2C_REG_16BIT                (0x01 << 1)
#define SENSOR_I2C_CUSTOM                   (0x01 << 2)

// I2C ACK/STOP BIT count
#define SENSOR_I2C_ACK_BIT          (0x00 << 3)
#define SENSOR_I2C_NOACK_BIT        (0x01 << 3)
#define SENSOR_I2C_STOP_BIT         (0x01 << 4)
#define SENSOR_I2C_NOSTOP_BIT       (0x00 << 4)

// I2C FEEQ BIT count
#define SENSOR_I2C_FREQ_MASK     (7 << 5)
#define SENSOR_I2C_FREQ_20       (0x01 << 5)
#define SENSOR_I2C_FREQ_50       (0x02 << 5)
#define SENSOR_I2C_FREQ_100      (0x00 << 5)
#define SENSOR_I2C_FREQ_200      (0x03 << 5)
#define SENSOR_I2C_FREQ_400      (0x04 << 5)

// Hardward signal polarity
#define SENSOR_HW_SIGNAL_PCLK_N             0x00
#define SENSOR_HW_SIGNAL_PCLK_P             0x01
#define SENSOR_HW_SIGNAL_VSYNC_N            (0x00 << 2)
#define SENSOR_HW_SIGNAL_VSYNC_P            (0x01 << 2)
#define SENSOR_HW_SIGNAL_HSYNC_N            (0x00 << 4)
#define SENSOR_HW_SIGNAL_HSYNC_P            (0x01 << 4)

// sensor focus mode
#define SENSOR_FOCUS_TRIG 0x01
#define SENSOR_FOCUS_ZONE (0x01<<1)

// sensor exposure mode
#define SENSOR_EXPOSURE_AUTO 0x01
#define SENSOR_EXPOSURE_ZONE (0x01<<1)

#define SENSOR_NO_SUPPORT 0x00
#define SENSOR_IOCTRL_PARAM_LENGTH     8          
#define SENSOR_WRITE_DELAY             0xffff
#define SENSOR_IOCTL_FUNC_NOT_REGISTER 0xffffffff

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
// enum: return value about sensor operation
typedef enum
{
  SENSOR_OP_SUCCESS = SCI_SUCCESS,
  SENSOR_OP_PARAM_ERR,
  SENSOR_OP_STATUS_ERR,
  SENSOR_OP_ERR,  
  SENSOR_OP_MAX = 0xFFFF
}ERR_SENSOR_E;

// enum: sensor id main img sensor sub img senosr or analog tv
typedef enum
{
    SENSOR_MAIN = 0,
    SENSOR_SUB,
    SENSOR_ATV,
    SENSOR_ID_MAX
}SENSOR_ID_E;

// enum: sensor type img sensor or analog tv
typedef enum
{
    SENSOR_TYPE_NONE = 0x00,
    SENSOR_TYPE_IMG_SENSOR,
    SENSOR_TYPE_ATV,
    SENSOR_TYPE_MAX
}SENSOR_TYPE_E;

typedef enum
{       
    SENSOR_OUTPUT_MODE_CCIR601_8BIT = 0,    
    SENSOR_OUTPUT_MODE_CCIR601_4BIT,        
    SENSOR_OUTPUT_MODE_CCIR601_2BIT,
    SENSOR_OUTPUT_MODE_CCIR601_1BIT,
    SENSOR_OUTPUT_MODE_CCIR656_8BIT,    
    SENSOR_OUTPUT_MODE_CCIR656_4BIT,
    SENSOR_OUTPUT_MODE_CCIR656_2BIT,
    SENSOR_OUTPUT_MODE_CCIR656_1BIT,    
    SENSOR_OUTPUT_MODE_SPI_8BIT,
    SENSOR_OUTPUT_MODE_SPI_4BIT,
    SENSOR_OUTPUT_MODE_SPI_2BIT,
    SENSOR_OUTPUT_MODE_SPI_1BIT,    
    SENSOR_OUTPUT_MODE_PACKET_8BIT,    
    SENSOR_OUTPUT_MODE_PACKET_4BIT,
    SENSOR_OUTPUT_MODE_PACKET_2BIT,
    SENSOR_OUTPUT_MODE_PACKET_1BIT,
    SENSOR_OUTPUT_MODE_PACKET_DDR_8BIT,    
    SENSOR_OUTPUT_MODE_PACKET_DDR_4BIT,
    SENSOR_OUTPUT_MODE_PACKET_DDR_2BIT,
    SENSOR_OUTPUT_MODE_PACKET_DDR_1BIT,
    SENSOR_OUTPUT_MODE_OV_SPI_8BIT,
    SENSOR_OUTPUT_MODE_OV_SPI_4BIT,    
    SENSOR_OUTPUT_MODE_OV_SPI_2BIT,
    SENSOR_OUTPUT_MODE_OV_SPI_1BIT,    
    SENSOR_OUTPUT_MODE_MAX
} SENSOR_OUTPUT_MODE_E;

typedef enum
{       
    SENSOR_OUTPUT_ENDIAN_BIG = 0,   
    SENSOR_OUTPUT_ENDIAN_LITTLE,    
    SENSOR_OUTPUT_ENDIAN_MAX
} SENSOR_OUTPUT_ENDIAN_E;

// enum: AVDD value about BB LDO output
typedef enum
{
    SENSOR_AVDD_3300MV=0,
    SENSOR_AVDD_3000MV,
    SENSOR_AVDD_2800MV,
    SENSOR_AVDD_2500MV,
    SENSOR_AVDD_1800MV,
    SENSOR_AVDD_1500MV,
    SENSOR_AVDD_1300MV, 
    SENSOR_AVDD_1200MV, 
    SENSOR_AVDD_CLOSED,
    SENSOR_AVDD_UNUSED
}SENSOR_AVDD_VAL_E;

// enum: mclk of chip output
typedef enum
{
    SENSOR_MCLK_12M=12,
    SENSOR_MCLK_13M=13,
    SENSOR_MCLK_24M=24,
    SENSOR_MCLK_26M=26,
    SENSOR_MCLK_MAX
}SENSOR_M_CLK_E;


// preview one match some range resolution snapshot
// preview two match other range resolution snapshot
// so the smallest resolution of snapshot one overlap with the biggest resolution of two,
// or the biggest resolution of snapshot one overlap with the smallest resolution of two.
typedef enum
{       
    // COMMON INIT
    SENSOR_MODE_COMMON_INIT = 0,    

    // PREVIEW ONE
    SENSOR_MODE_PREVIEW_ONE,        
    SENSOR_MODE_SNAPSHOT_ONE_FIRST,
    SENSOR_MODE_SNAPSHOT_ONE_SECOND,
    SENSOR_MODE_SNAPSHOT_ONE_THIRD,

    // PREVIEW TWO  
    SENSOR_MODE_PREVIEW_TWO,
    SENSOR_MODE_SNAPSHOT_TWO_FIRST,
    SENSOR_MODE_SNAPSHOT_TWO_SECOND,
    SENSOR_MODE_SNAPSHOT_TWO_THIRD,

    SENSOR_MODE_MAX,

	SENSOR_MODE_RECORD_REVIEW = 10   //UIX8910 录像预览320*240
} SENSOR_MODE_E;

// enum: image format about sensor output
typedef enum
{
    SENSOR_IMAGE_FORMAT_YUV422 = 0,
    SENSOR_IMAGE_FORMAT_YUV420,
    SENSOR_IMAGE_FORMAT_RAW,
    SENSOR_IMAGE_FORMAT_RGB565,
    SENSOR_IMAGE_FORMAT_RGB666,
    SENSOR_IMAGE_FORMAT_RGB888,
    SENSOR_IMAGE_FORMAT_CCIR656,
    SENSOR_IMAGE_FORMAT_JPEG,
    
    SENSOR_IMAGE_FORMAT_MAX
}SENSOR_IMAGE_FORMAT;

// enum: sensor I2c port select
typedef enum
{
    SENSOR_I2C_PORT_ZERO = 0,
    SENSOR_I2C_PORT_ONE,
    SENSOR_I2C_PORT_TWO,
    SENSOR_I2C_PORT_THREE,
    SENSOR_I2C_PORT_FOUR,
    SENSOR_I2C_PORT_FIVE,
    SENSOR_I2C_PORT_SIX,
    SENSOR_I2C_PORT_SEVEN,
    
    SENSOR_I2C_PORT_MAX
}SENSOR_I2C_PORT_E;

// enum: Sensor IOCTL command
typedef enum
{
    // Internal Command (count = 8)
    SENSOR_IOCTL_RESET = 0,         // use to reset sensor
    SENSOR_IOCTL_POWER,         // Power on/off sensor selected by input parameter(0:off,1:on)
    SENSOR_IOCTL_ENTER_SLEEP,       // enter sleep
    SENSOR_IOCTL_IDENTIFY,          // identify
    SENSOR_IOCTL_WRITE_REG,     // write register value
    SENSOR_IOCTL_READ_REG,          // read register value
    SENSOR_IOCTL_ATV,       // set function 1 for custumer to configure
    SENSOR_IOCTL_GET_TRIM,          // get img sensor trim size

    // External Command (count = 18)
    SENSOR_IOCTL_AE_ENABLE,     // enable auto exposure
    SENSOR_IOCTL_HMIRROR_ENABLE,    // enable horizontal mirror
    SENSOR_IOCTL_VMIRROR_ENABLE,    // enable vertical mirror

    SENSOR_IOCTL_BRIGHTNESS,        // set brightness
    SENSOR_IOCTL_CONTRAST,          // set contrast
    SENSOR_IOCTL_SHARPNESS,     // set sharpness
    SENSOR_IOCTL_SATURATION,        // set saturation
    SENSOR_IOCTL_PREVIEWMODE,       // set preview mode

    SENSOR_IOCTL_IMAGE_EFFECT,      // set image effect
    SENSOR_IOCTL_BEFORE_SNAPSHOT,   // do something before do snapshort
    SENSOR_IOCTL_AFTER_SNAPSHOT,    // do something after do snapshort   

    SENSOR_IOCTL_FLASH,         // control to open / close flash

    SENSOR_IOCTL_READ_EV,           // read AE value from sensor register
    SENSOR_IOCTL_WRITE_EV,          // write AE value to sensor register
    SENSOR_IOCTL_READ_GAIN,     // read GAIN value from sensor register
    SENSOR_IOCTL_WRITE_GAIN,        // write GAIN value to sensor register
    SENSOR_IOCTL_READ_GAIN_SCALE,  // read GAIN scale (sensor dependable, refer to sensor spec)
    SENSOR_IOCTL_SET_FRAME_RATE,    // set sensor frame rate based on current clock

    SENSOR_IOCTL_AF_ENABLE,     // enable auto focus function
    SENSOR_IOCTL_AF_GET_STATUS, // get auto focus status

    SENSOR_IOCTL_SET_WB_MODE,       // set while balance mode

    SENSOR_IOCTL_GET_SKIP_FRAME,    // get snapshot skip frame num from customer

    SENSOR_IOCTL_ISO,               // set ISO mode
    SENSOR_IOCTL_EXPOSURE_COMPENSATION, // Set exposure compensation

    SENSOR_IOCTL_CHECK_IMAGE_FORMAT_SUPPORT, // check whether image format is support
    SENSOR_IOCTL_CHANGE_IMAGE_FORMAT, //change sensor image format
    SENSOR_IOCTL_ZOOM,                  //change sensor output window and size

    SENSOR_IOCTL_GET_EXIF,      // set function for get image sensor exif info
    SENSOR_IOCTL_EXT_FUNC,      // set ext function of img sensor

    SENSOR_IOCTL_ANTI_BANDING_FLICKER, // Set anti banding flicker mode
    SENSOR_IOCTL_VIDEO_MODE, // Set video mode

    SENSOR_IOCTL_PICK_JPEG_STREAM,
    SENSOR_IOCTL_METER_MODE,
    SENSOR_IOCTL_TEST_PATTERN_MODE,
    SENSOR_IOCTL_MAX
}SENSOR_IOCTL_CMD_E;

// enum: Sensor EXTEND focus command
typedef enum
{
    SENSOR_EXT_FOCUS_NONE=0x00,
    SENSOR_EXT_FOCUS_TRIG,
    SENSOR_EXT_FOCUS_ZONE,
    SENSOR_EXT_FOCUS_MAX
}SENSOR_EXT_FOCUS_CMD_E;

// enum: Sensor EXTEND exposure command
typedef enum
{
    SENSOR_EXT_EXPOSURE_NONE=0x00,
    SENSOR_EXT_EXPOSURE_AUTO,    
    SENSOR_EXT_EXPOSURE_ZONE,
    SENSOR_EXT_EXPOSURE_MAX
}SENSOR_EXT_EXPOSURE_CMD_E;

// enum: Sensor EXTEND function command
typedef enum
{
    SENSOR_EXT_FUNC_NONE=0x00,
    SENSOR_EXT_FUNC_INIT,
    SENSOR_EXT_FOCUS_START,
    SENSOR_EXT_EXPOSURE_START,
    SENSOR_EXT_FUNC_MAX
}SENSOR_EXT_FUNC_CMD_E;

// enum: Sensor exif command
typedef enum
{
    SENSOR_EXIF_CTRL_EXPOSURETIME=0x00,
    SENSOR_EXIF_CTRL_FNUMBER,
    SENSOR_EXIF_CTRL_EXPOSUREPROGRAM,   
    SENSOR_EXIF_CTRL_SPECTRALSENSITIVITY,   
    SENSOR_EXIF_CTRL_ISOSPEEDRATINGS,
    SENSOR_EXIF_CTRL_OECF,  
    SENSOR_EXIF_CTRL_SHUTTERSPEEDVALUE,
    SENSOR_EXIF_CTRL_APERTUREVALUE,
    SENSOR_EXIF_CTRL_BRIGHTNESSVALUE,
    SENSOR_EXIF_CTRL_EXPOSUREBIASVALUE,
    SENSOR_EXIF_CTRL_MAXAPERTUREVALUE,
    SENSOR_EXIF_CTRL_SUBJECTDISTANCE,
    SENSOR_EXIF_CTRL_METERINGMODE,
    SENSOR_EXIF_CTRL_LIGHTSOURCE,   
    SENSOR_EXIF_CTRL_FLASH,
    SENSOR_EXIF_CTRL_FOCALLENGTH,
    SENSOR_EXIF_CTRL_SUBJECTAREA,
    SENSOR_EXIF_CTRL_FLASHENERGY,
    SENSOR_EXIF_CTRL_SPATIALFREQUENCYRESPONSE,
    SENSOR_EXIF_CTRL_FOCALPLANEXRESOLUTION,
    SENSOR_EXIF_CTRL_FOCALPLANEYRESOLUTION,
    SENSOR_EXIF_CTRL_FOCALPLANERESOLUTIONUNIT,
    SENSOR_EXIF_CTRL_SUBJECTLOCATION,
    SENSOR_EXIF_CTRL_EXPOSUREINDEX,
    SENSOR_EXIF_CTRL_SENSINGMETHOD,
    SENSOR_EXIF_CTRL_FILESOURCE,
    SENSOR_EXIF_CTRL_SCENETYPE,
    SENSOR_EXIF_CTRL_CFAPATTERN,
    SENSOR_EXIF_CTRL_CUSTOMRENDERED,
    SENSOR_EXIF_CTRL_EXPOSUREMODE,
    SENSOR_EXIF_CTRL_WHITEBALANCE,
    SENSOR_EXIF_CTRL_DIGITALZOOMRATIO,
    SENSOR_EXIF_CTRL_FOCALLENGTHIN35MMFILM,
    SENSOR_EXIF_CTRL_SCENECAPTURETYPE,  
    SENSOR_EXIF_CTRL_GAINCONTROL,
    SENSOR_EXIF_CTRL_CONTRAST,
    SENSOR_EXIF_CTRL_SATURATION,
    SENSOR_EXIF_CTRL_SHARPNESS,
    SENSOR_EXIF_CTRL_DEVICESETTINGDESCRIPTION,
    SENSOR_EXIF_CTRL_SUBJECTDISTANCERANGE,
    SENSOR_EXIF_CTRL_MAX,
}SENSOR_EXIF_CTRL_E;

typedef enum
{
    SENSOR_METER_MODE_FULL = 0,
    SENSOR_METER_MODE_CENTER,        
    SENSOR_METER_MODE_SPOT,
    SENSOR_METER_MODE_MATRIX,
    SENSOR_METER_MODE_MAX = 0xFF
}SENSOR_METER_MODE_E; 

typedef uint32 (*SENSOR_IOCTL_FUNC_PTR)(uint32 param);
// struct: Sensor IOCTL function
typedef struct sensor_ioctl_func_tab_tag
{
    // 1: Internal IOCTL function
    uint32 (*reset)           (uint32 param);   // use to reset sensor                                      
    uint32 (*power)           (uint32 param);   // Power on/off sensor selected by input parameter(0:off,1:on)  
    uint32 (*enter_sleep)     (uint32 param);   // enter sleep  
    uint32 (*identify)        (uint32 param);   // identify sensor: 0 -> successful ; others -> fail
    uint32 (*write_reg)       (uint32 param);   // [31:16] register address; [15:0] register value
    uint32 (*read_reg)        (uint32 param);   // input value[15:0]: register address; return value[15:0]: register value
    // Custom function
    uint32 (*cus_func_1)      (uint32 param);   // function 1 for atv ioctl                      
    uint32 (*get_trim)        (uint32 param);   // function 2 for img sensor trim 

    // 2: External IOCTL function   
    uint32 (*set_rot)         (uint32 param);   // enable auto exposure                                         
    uint32 (*hmirror_enable)  (uint32 param);   // enable horizontal mirror                                     
    uint32 (*vmirror_enable)  (uint32 param);   // enable vertical mirror                                       
                                                                                            
    uint32 (*set_brightness)  (uint32 param);   // set brightness   0: auto; other: the appointed level                                                
    uint32 (*set_contrast)    (uint32 param);   // set contrast     0: auto; other: the appointed level                                             
    uint32 (*set_sharpness)   (uint32 param);   // set sharpness    0: auto; other: the appointed level                                             
    uint32 (*set_saturation)  (uint32 param);   // set saturation   0: auto; other: the appointed level                                             
    uint32 (*set_preview_mode)(uint32 param);   // set preview mode : 0: normal mode; 1: night mode; 2: sunny mode                                               
                                                                                            
    uint32 (*set_image_effect)(uint32 param);   // set image effect                                             
    uint32 (*before_snapshort)(uint32 param);   // do something before do snapshort                             
    uint32 (*after_snapshort) (uint32 param);   // do something after do snapshort                                  

    uint32 (*flash)           (uint32 param);   // 1: open flash; 0: close falsh
                                                                                                
    uint32 (*read_ae_value)   (uint32 param);   // return AE value
    uint32 (*write_ae_value)  (uint32 param);   // input AE value
    uint32 (*read_gain_value) (uint32 param);   // return GAIN value
    uint32 (*write_gain_value)(uint32 param);   // input GAIN value
    uint32 (*read_gain_scale) (uint32 param);   // return GAIN scale (for ov9650, 16)
    uint32 (*set_frame_rate)  (uint32 param);   // set sensor frame rate based on current clock

    uint32 (*af_enable)       (uint32 param);   // input 1: enable; input 0: disable
    uint32 (*af_get_status)   (uint32 param);   // return value: return 0 -> focus ok, other value -> lose focus

    uint32 (*set_wb_mode)     (uint32 param);   // set while balance mode

    uint32 (*get_skip_frame)  (uint32 param);   // get snapshot skip frame num from customer, input SENSOR_MODE_E paramter

    uint32 (*set_iso)         (uint32 param);   // set ISO level                     0: auto; other: the appointed level
    uint32 (*set_exposure_compensation)(uint32 param); // Set exposure compensation  0: auto; other: the appointed level

    uint32 (*check_image_format_support)(uint32 param); // check whether image format is support
    uint32 (*change_image_format)(uint32 param); //change sensor image format according to param
    uint32 (*set_zoom)(uint32 param); //change sensor image format according to param

    // CUSTOMER FUNCTION                          
    uint32 (*get_exif)          (uint32 param); // function get sensor exif
    uint32 (*ext_func)          (uint32 param); // function for img sensor extern feature
    uint32 (*set_anti_banding_flicker)(uint32 param); // Set anti banding flicker    0: 50hz;1: 60  
    uint32 (*set_video_mode)(uint32 param); // set video mode

    uint32 (*pick_jpeg_stream)(uint32 param);   // pick out the jpeg stream from given buffer
    uint32 (*set_meter_mode)(uint32 param);   // set meter mode
    uint32 (*set_test_pattern_mode)(uint32 param); // set test pattern mode
    
}SENSOR_IOCTL_FUNC_TAB_T, *SENSOR_IOCTL_FUNC_TAB_T_PTR;

typedef struct _atv_info_tag
{
    uint32 atv_cur_region;
    uint32 atv_cur_chn;
    uint32 cur_volume_level;
} ATV_INFO_T, *ATV_INFO_T_PTR;

// struct: Information about sensor register
typedef struct sensor_reg_tag
{
    uint16 reg_addr;                        // address of sensor register
    uint16 reg_value;                       // value of sensor register
    
}SENSOR_REG_T, *SENSOR_REG_T_PTR;

typedef struct sensor_trim_tag
{
    uint16    trim_start_x;
    uint16    trim_start_y;
    uint16    trim_width;
    uint16    trim_height;
    uint32    line_time;
    uint32    pclk;
}SENSOR_TRIM_T, *SENSOR_TRIM_T_PTR;

typedef struct _sensor_rect_tag 
{
    uint16  x;
    uint16  y;
    uint16  w;
    uint16  h;
}SENSOR_RECT_T, *SENSOR_RECT_T_PTR;

typedef struct _sensor_ext_fun_tag 
{
    uint32 cmd;
    uint32 param;
    SENSOR_RECT_T zone;
}SENSOR_EXT_FUN_T, *SENSOR_EXT_FUN_T_PTR;

// struct: Information about sensor register table 
typedef struct sensor_reg_tab_info_tag
{
    SENSOR_REG_T_PTR    sensor_reg_tab_ptr; // poiter of sensor register table
    uint32              reg_count;          // count of registers in table
    uint16              width;              // width of resolution
    uint16              height;             // height of resolution
    uint32              xclk_to_sensor;     // unit : MHz ->0: default value
    SENSOR_IMAGE_FORMAT image_format;       // image format is valid when set to SENSOR_IMAGE_FORMAT_MAX in SENSOR_INFO_T
                                            // or else image format is invalid
}SENSOR_REG_TAB_INFO_T, *SENSOR_REG_TAB_INFO_T_PTR;

// struct: Information about Sensor operation mode 
typedef struct sensor_mode_info_tag
{
    SENSOR_MODE_E       mode;
    uint16              width;
    uint16              height;
    uint16              trim_start_x;
    uint16              trim_start_y;
    uint16              trim_width;
    uint16              trim_height;
    uint32              line_time;
    uint32              pclk;
    SENSOR_IMAGE_FORMAT image_format;
    
}SENSOR_MODE_INFO_T, *SENSOR_MODE_INFO_T_PTR;

// struct: Information about rawrgb
typedef struct sensor_raw_info_tag
{
    uint32              res;    
}SENSOR_RAW_INFO_T, *SENSOR_RAW_INFO_T_PTR;

// struct: Information about sensor extend
typedef struct sensor_extend_info_tag
{
    uint32 focus_mode;
    uint32 exposure_mode;
}SENSOR_EXTEND_INFO_T, *SENSOR_EXTEND_INFO_T_PTR;

// struct: Information about currect register img sensor
typedef struct sensor_register_tag
{
    uint32 img_sensor_num;
    uint8 cur_id;
    uint8 is_register[SENSOR_ID_MAX];
}SENSOR_REGISTER_INFO_T, *SENSOR_REGISTER_INFO_T_PTR;

//struct: source MCLK and APB setting table struct
typedef struct SN_MCLK
{    
    uint32  clock;
    uint32  src;
}SN_MCLK;

// struct: Information about sensor export to other module
typedef struct sensor_exp_info_tag
{
    SENSOR_OUTPUT_MODE_E  output_mode;  //sensor output mode
    SENSOR_OUTPUT_ENDIAN_E output_endian;
    SENSOR_IMAGE_FORMAT image_format;   // define in SENSOR_IMAGE_FORMAT_E enum
    uint32 image_pattern;               // pattern of input image form sensor   

    uint8 pclk_polarity;                // 0:negative; 1:positive -> polarily of pixel clock    
    uint8 vsync_polarity;               // 0:negative; 1:positive -> polarily of vertical synchronization signal
    uint8 hsync_polarity;               // 0:negative; 1:positive -> polarily of horizontal synchronization signal
    uint8 pclk_delay;

    uint16 source_width_max;        // max width of source image
    uint16 source_height_max;       // max height of source image

    uint32 environment_mode;        // environment mode type that sensor can support
    uint32 image_effect;            // image effect type that sensor can support
    uint32 wb_mode;                 // while balance type that sensor can support       
    uint32 step_count;              // bit[0:7]: count of step in brightness, contrast, sharpness, saturation

    SENSOR_MODE_INFO_T sensor_mode_info[SENSOR_MODE_MAX+1];     
    SENSOR_IOCTL_FUNC_TAB_T_PTR ioctl_func_ptr; // point to ioctl function table    
    SENSOR_RAW_INFO_T_PTR raw_info_ptr;
    SENSOR_EXTEND_INFO_T_PTR ext_info_ptr;
    uint32 preview_skip_num;        // skip frame num before preview;
    uint32 capture_skip_num;        // skip frame num before capture;       
    uint32 preview_deci_num;        // deci frame num during preview;   
    uint32 video_preview_deci_num;  // deci frame num during preview;
    uint16 threshold_eb;    // threshold enable
    uint16 threshold_mode;  // threshold mode
    uint16 threshold_start; // threshold start postion  
    uint16 threshold_end;   // threshold end postion 
}SENSOR_EXP_INFO_T, *SENSOR_EXP_INFO_T_PTR;

// struct: Information about sensor from customer

typedef struct sensor_info_tag
{
    // addr
    uint8 salve_i2c_addr_w;     // salve i2c write address
    uint8 salve_i2c_addr_r;     // salve i2c read address

    uint8 reg_addr_value_bits;  // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                // bit1: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                // other bit: reseved

    uint8 hw_signal_polarity;   // bit0: 0:negative; 1:positive -> polarily of pixel clock
                                // bit2: 0:negative; 1:positive -> polarily of vertical synchronization signal
                                // bit4: 0:negative; 1:positive -> polarily of horizontal synchronization signal
                                // other bit: reseved

    uint32 environment_mode;    // environment mode type that sensor can support
    uint32 image_effect;        // image effect type that sensor can support
    uint32 wb_mode;             // while balance type that sensor can support       
    uint32 step_count;          // bit[0:7]: count of step in brightness, contrast, sharpness, saturation
                                // bit[8:15] count of step in ISO
                                // bit[16:23] count of step in exposure compensation
                                // bit[24:31] reseved

    uint16 reset_pulse_level;   // 1: high level valid; 0: low level valid
    uint16 reset_pulse_width;   // Unit: ms. Less than 200ms

    uint32 power_down_level;    // 1: high level valid; 0: low level valid

    uint32 identify_count;      // count of identify code
    SENSOR_REG_T identify_code[SENSOR_IDENTIFY_CODE_COUNT];     
                                // supply two code to identify sensor. 
                                // for Example: index = 0-> Device id, index = 1 -> version id

    SENSOR_AVDD_VAL_E avdd_val; // voltage of avdd  

    uint16 source_width_max;    // max width of source image
    uint16 source_height_max;   // max height of source image
    const char* name;           // name of sensor

    SENSOR_IMAGE_FORMAT image_format;   // define in SENSOR_IMAGE_FORMAT_E enum,
                                        // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    uint32 image_pattern;               // pattern of input image from sensor;          

    SENSOR_REG_TAB_INFO_T_PTR resolution_tab_info_ptr;  // point to resolution table information structure  
    SENSOR_IOCTL_FUNC_TAB_T_PTR ioctl_func_tab_ptr;     // point to ioctl function table        
    SENSOR_RAW_INFO_T_PTR raw_info_ptr;                 // information and table about Rawrgb sensor
    SENSOR_EXTEND_INFO_T_PTR ext_info_ptr;              // extend information about sensor  
    SENSOR_AVDD_VAL_E iovdd_val;    // voltage of iovdd 
    SENSOR_AVDD_VAL_E dvdd_val;     // voltage of dvdd  
    uint32 preview_skip_num;        // skip frame num before preview;
    uint32 capture_skip_num;        // skip frame num before capture;   
    uint32 preview_deci_num;        // deci frame num during preview;       
    uint32 video_preview_deci_num;  // deci frame num during video preview;
    uint16 threshold_eb;            // threshold enable
    uint16 threshold_mode;          // threshold mode
    uint16 threshold_start;         // threshold start postion  
    uint16 threshold_end;           // threshold end postion 
    SENSOR_OUTPUT_MODE_E output_mode;
    SENSOR_OUTPUT_ENDIAN_E output_endian;
}SENSOR_INFO_T;

typedef  int (*SENSOR_OPEN_FUNC_PTR)(int);

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 
//------ To Sensor Module
 
/*****************************************************************************/
//  Description:    This function is used to write value to sensor register    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_WriteReg( uint16  subaddr, uint16 data );

/*****************************************************************************/
//  Description:    This function is used to get iic handler
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_GetI2cHnadler(void);

/*****************************************************************************/
//  Description:    This function is used to read value from sensor register     
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint16 Sensor_ReadReg(uint16 subaddr);

/*****************************************************************************/
//  Description:    This function is used to send a table of register to sensor    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_SendRegTabToSensor(SENSOR_REG_TAB_INFO_T * sensor_reg_tab_info_ptr   );

//------ To Digital Camera Module

/*****************************************************************************/
//  Description:    This function is used to initialize Sensor function    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_Init(void);

/*****************************************************************************/
//  Description:    This function is used to check if sensor has been init    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_IsInit(void);

/*****************************************************************************/
//  Description:    This function is used to Open sensor function    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_Open(void);

/*****************************************************************************/
//  Description:    This function is used to set work-mode    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_SetMode(SENSOR_MODE_E mode);

/*****************************************************************************/
//  Description:    This function is used to control sensor    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_Ioctl(uint32 cmd, uint32 arg);

/*****************************************************************************/
//  Description:    This function is used to Get sensor information    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_EXP_INFO_T* Sensor_GetInfo( void );

/*****************************************************************************/
//  Description:    This function is used to Close sensor function    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC ERR_SENSOR_E Sensor_Close(void) ;

/*****************************************************************************/
//  Description:    This function is used to power down sensor     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_PowerDown(BOOLEAN power_down);
/*****************************************************************************/
//  Description:    This function is used to set AVDD
//  Author:         Liangwen.Zhen
//  Note:           Open AVDD on one special voltage or Close it
/*****************************************************************************/
PUBLIC void Sensor_SetVoltage(SENSOR_AVDD_VAL_E dvdd_val, SENSOR_AVDD_VAL_E avdd_val, SENSOR_AVDD_VAL_E iodd_val);

/*****************************************************************************/
//  Description:    This function is used to power down front sensor     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_PowerDownFront(BOOLEAN power_down);

/*****************************************************************************/
//  Description:    This function is used to power down sensor     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN Sensor_SetResetLevel(BOOLEAN plus_level);

/*****************************************************************************/
//  Description:    This function is used to power on sensor and select xclk    
//  Author:         Liangwen.Zhen
//  Note:           1.Unit: MHz 2. if mclk equal 0, close main clock to sensor
/*****************************************************************************/
PUBLIC void Sensor_SetMCLK(uint32 mclk);

/*****************************************************************************/
//  Description:    This function is used to power on sensor and select xclk    
//  Author:         Liangwen.Zhen
//  Note:           1.Unit: MHz 2. if mclk equal 0, close main clock to sensor
/*****************************************************************************/
PUBLIC BOOLEAN  Sensor_IsOpen(void) ;

/*****************************************************************************/
//  Description:    This function is used to set sensor id
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_SetCurId(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used to get sensor id
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_ID_E Sensor_GetCurId(void);

/*****************************************************************************/
//  Description:    This function is Get the num of register img senosr
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_REGISTER_INFO_T_PTR Sensor_GetRegisterInfo(void);

/*****************************************************************************/
//  Description: Delete Mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          sm    -  Mutex
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void ImgSensor_DeleteMutex(void);

/*****************************************************************************/
//  Description: Get Mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          sm    -  mutex
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void ImgSensor_GetMutex(void);
/*****************************************************************************/
//  Description: Put mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          sm    -  mutex
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void ImgSensor_GetMutex(void);

/*****************************************************************************/
//  Description: Put mutex
//  Global resource dependence:
//  Author: Tim.zhu
//  Note:
//      input:
//          sm    -  mutex
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void ImgSensor_PutMutex(void);

/*****************************************************************************/
//  Description:    This function is used to set sensor type    
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_SetSensorType(SENSOR_TYPE_E sensor_type);

/*****************************************************************************/
//  Description:    This function is used to get sensor type    
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC SENSOR_TYPE_E _Sensor_GetSensorType(void) ;

/*****************************************************************************/
//  Description:    This function is used to set sensor info
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_SetInfo(void);

/*****************************************************************************/
//  Description:    This function is used to set sensor exif info    
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC uint32 Sensor_SetSensorExifInfo(SENSOR_EXIF_CTRL_E cmd ,uint32 param);

/*****************************************************************************/
//  Description:    This function is used to get sensor exif info    
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC EXIF_SPEC_PIC_TAKING_COND_T* Sensor_GetSensorExifInfo(void);

/*****************************************************************************/
//  Description:    This function is used to get sensor info    
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_GetTable(SENSOR_ID_E sensor_id,SENSOR_INFO_T **table_ptr);

/*****************************************************************************/
//  Description:    This function is used for ccir pclk source   
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirPclkSrc(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used for ccir D0 
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirD0Src(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used for ccir vsync 
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirVsyncSrc(SENSOR_ID_E sensor_id);

/*****************************************************************************/
//  Description:    This function is used for ccir d1 
//  Author:         Eddy.wei
//  Note:           
/*****************************************************************************/
PUBLIC uint8 Sensor_GetCcirD1Src(SENSOR_ID_E sensor_id);

/*****************************************************************************
//  Description:    This function is to set sensor index value   
//  Author:         xi.zhang
//  Note:           
*****************************************************************************/
PUBLIC void Sensor_SetIdentifiedState(BOOLEAN if_identified, uint8 *sensor_index);

/*****************************************************************************
//  Description:    This function is to get sensor index value   
//  Author:         xi.zhang
//  Note:           
*****************************************************************************/
PUBLIC void Sensor_GetIdentifiedState(SENSOR_ID_E sensor_id, uint8 *p_sensor_index);

/*****************************************************************************
//  Description:    Check if there exist camera   
//  Author:         xi.zhang
//  Note:           
*****************************************************************************/
PUBLIC BOOLEAN Sensor_CheckIfHaveCamera(void);

/*****************************************************************************
//  Description:    restore the sensor state from intial operation   
//  Author:         xi.zhang
//  Note:           
*****************************************************************************/
PUBLIC void Sensor_RestoreStateFromInit(void);


PUBLIC int sensor_powerup();
PUBLIC void Sensor_SetCallback(SENSOR_OPEN_FUNC_PTR pFunc);

PUBLIC int32 Sensor_I2cInit() ;
void Sensor_WriteRegtest( uint16  subaddr, uint16 data );
uint16 Sensor_ReadRegtest(uint16 subaddr);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}

#endif

#endif  // _SENSOR_DRV_H_

// End of sensor_drv.h
