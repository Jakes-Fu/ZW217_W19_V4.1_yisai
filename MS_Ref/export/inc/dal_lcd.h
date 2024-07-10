/******************************************************************************
 ** File Name:      dal_lcd.h                                                 *
 ** Author:         Jim zhang                                                 *
 ** DATE:           06/03/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the color LCD API function for up layer  *
 **                 1. LCD driver support a LCD buffer according to lcd size  *
 **                    to hold the pixel color. Up layer can get the buffer   *
 **                    pointer and operate on the LCD buffer.                 *
 **                 2. LCD driver is only responsible of copying the pixel    *
 **                    color in LCD buffer to LCD (hardware).                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/03/2004     Jim.zhang        Update file used for lcd api for up layer *
 ** 10/22/2004     Jim.zhang        Delete some api.                          *
 ******************************************************************************/

#ifndef _DAL_LCD_H_
#define _DAL_LCD_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "ref_outport.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/
//LCD id enum struct
#define MAIN_LCD_ID                     0
#define SUB_LCD_ID                      1
#define MAX_LCM_PARAM_VALUE             ((0x3ee << 10)|(0x3ee))

#define LCD_SUPPORT_MAX                 2

//resolution of lcd data
//0:RGB888; 1:RGB666; 2:RGB565;3:RGB555;4:GREY,5:YUV422;6:YUV420;7:YUV400
#define  LCD_RESOLUTION_RGB888          0
#define  LCD_RESOLUTION_RGB666          1
#define  LCD_RESOLUTION_RGB565          2
#define  LCD_RESOLUTION_RGB555          3
#define  LCD_RESOLUTION_GREY            4
#define  LCD_RESOLUTION_YUV422          5
#define  LCD_RESOLUTION_YUV420          6
#define  LCD_RESOLUTION_YUV400          7
#define  LCD_RESOLUTION_YUV420_3PLANE          8


/**---------------------------------------------------------------------------*
 **                         enum defines.
 **---------------------------------------------------------------------------*/
typedef enum
{
    ERR_LCD_NONE = 0,               // Success,no error
    ERR_LCD_FUNC_NOT_SUPPORT,       // LCD not support this function
    ERR_LCD_PARAMETER_WRONG,        // Parameter is wrong
    ERR_LCD_POINTER_NULL,           // Input pointer is PNULL
    ERR_LCD_OPERATE_FAIL,           // Operate fail
    ERR_LCD_NOT_FOUND,              // LCD not found
    ERR_LCD_SELPIN_WRONG,           // LCD SELPIN WRONG
    ERR_LCD_SPEC_WRONG              // LCD SPEC WRONG
} ERR_LCD_E;

typedef enum
{
    LCD_ID_0 = 0,
    LCD_ID_1    ,
    LCD_ID_2    ,
    LCD_ID_3    ,
    LCD_ID_4    ,
    LCD_ID_5    ,
    LCD_ID_6    ,
    LCD_ID_7    ,
    TV_ID       ,                   //be used for TVOUT
    MAX_LCD_ID                      //Reserved, can not be used by user.
} LCD_ID_E;

typedef enum
{
    DATA_SIZE_BYTE = 0,             //0:DATA_SIZE_BYTE
    DATA_SIZE_HALFWORD,             //1:DATA_SIZE_HALFWORD
    DATA_SIZE_WORD                  //2:DATA_SIZE_WORD
} LCD_DATA_SIZE_E;

typedef enum
{
    LCD_RGB = 0,
    LCD_MCU
} LCD_MODE_E;

typedef enum
{
    BUS_MODE_8080   = 0,
    BUS_MODE_6800 ,
    BUS_MODE_SPI
} LCD_BUS_MODE_E;

typedef enum
{
    WIDTH_16  =0,
    WIDTH_18 ,
    WIDTH_8  ,
    WIDTH_9
} LCD_BUS_WIDTH_E;

// LCD Contrller type
typedef enum
{
    LCD_CTRL_MODULE_LCDC = 0   ,    //LCDC
    LCD_CTRL_MODULE_LCM        ,    //LCM
    LCD_CTRL_MODULE_LCDC_LCM0,      //LCDC+LCM0
    LCD_CTRL_MODULE_EMC,
    LCD_CTRL_GPIO,
    LCD_CTRL_MODULE_SPI             //spi    
} LCD_CTRL_MODULE_E;

typedef enum
{
    LCD_ANGLE_0   ,
    LCD_ANGLE_90  ,
    LCD_ANGLE_180 ,
    LCD_ANGLE_270
} LCD_ANGLE_E;

typedef enum
{
    LCD_DIRECT_NORMAL = 0x00,
    LCD_DIRECT_ROT_90,      // Rotation 90
    LCD_DIRECT_ROT_180,     // Rotation 180
    LCD_DIRECT_ROT_270,     // Rotation 270
    LCD_DIRECT_MIR_H,       // Mirror horizontal
    LCD_DIRECT_MIR_V,       // Mirror vertical
    LCD_DIRECT_MIR_HV,      // Mirror horizontal and vertical
    LCD_DIRECT_MAX
} LCD_DIRECT_E;

//Brush LCD mode
typedef enum
{
    LCD_BRUSH_MODE_SYNC = 0,
    LCD_BRUSH_MODE_ASYNC
} LCD_BRUSH_MODE_E;

typedef enum
{
    CMD_LEVEL   = 0,                //command:0, data:1. nomal
    DATA_LEVEL  = 1             //command:1, data:0. 
} LCD_CD_LEVEL_E; 

/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

//Block Configuration
typedef struct blockcfg_tag
{
    uint16 start_x;
    uint16 start_y;
    uint16 end_x;
    uint16 end_y;
    uint16 colorkey;
    uint16 width;                //real data width in SDRAM
    uint8  resolution;           //0:RGB888; 1:RGB666; 2:RGB565(RGB555 if OSD)
    uint8  type;                 //0:OSD; 1:image
    uint8  priority;             //0~7, high:7,low:0
    uint8  alpha;
    uint8  colorkey_en;
    uint8  alpha_sel;            //0-pixel alpha;1-block alpha
    uint8  endian_switch;        //support for 6800H
    uint8  set_endian_flag;
    uint32 grey_rgb;             //RGB888 value 
    uint32 rotation;
    uint32 rb_switch;
    uint32 blend_mode;
    uint32 rgb_expand_mode;
} BLOCKCFG_T; 

typedef struct lcd_info_tag
{
    uint32  r_bitmask;              // R(Red color) bit mask
    uint32  g_bitmask;              // G(Green color) bit mask
    uint32  b_bitmask;              // B(Black color) bit mask
    uint16  bits_per_pixel;         // bit number of one pixel
    uint16  lcd_width;              // lcd width
    uint16  lcd_height;             // lcd height.
    uint16  contrast_min;           // min value of contrast
    uint16  contrast_max;           // max value of contrast
    uint16  contrast_defaut;        // default value of contrast
    void    *lcdbuff_ptr;           // lcd buffer pointer
    uint16  is_use_fmark;           // 0: don't use fmark 1:use fmark
    uint16  fmark_pol;              //0:HIGH, 1:LOW
} LCD_INFO_T;

// lcd information struct, which will be useful for upper layer.
typedef union _LCD_TIMING_U
{
    struct RGB_LCD_TIMING_tag
    {
        uint32 bfw;    //Beginning-of-Frame Horizontal Sync Clock Wait Count-1
        uint32 efw;    //End-of-Frame Horizontal Sync Clock Wait Count-1
        uint32 vsw;    //Vertical Sync Pulse Width - 1
        uint32 blw;    //Beginning-of-Line Pixel Clock Wait Count -1
        uint32 elw;    //End-of-Line Pixel Clock Wait Count -1
        uint32 hsw;    //Horizontal Sync Pulse Width - 1
    } rgb_mode_timing;
    struct MCU_LCD_TIMING_tag
    {
        uint32 rcss;// CS setup time for LCM read (optional)
        uint32 rlpw;// low pulse width for LCM read (according spec)
        uint32 rhpw;// high pulse width for LCM read (according spec)
        uint32 wcss;// CS setup time for LCM write  (optional)
        uint32 wlpw;// low pulse width for LCM write (according spec)
        uint32 whpw;// high pulse width for LCM write (according spec)
    } mcu_mode_timing;
    struct SPI_LCD_TIMING_tag
    {
        uint32 freq;          // clk frequency support (unit:Khz)       
        uint32 cpol;          // CPOL: 0--SPI_CLK_IDLE_LOW,   1--SPI_CLK_IDLE_HIGH
        uint32 cpha;          // CPHA: 0--SPI_SAMPLING_RISING,1--SPI_SAMPLING_FALLING  
        uint32 tx_bitlen;     // tx bit length: 8/16bits for init_code process. 
        uint32 null0;    
        uint32 null1;         // 
    } spi_mode_timing; 
    
} LCD_TIMING_U;

typedef struct lcd_operations_tag
{
    ERR_LCD_E (*lcd_Init) (void);
    ERR_LCD_E (*lcd_EnterSleep) (BOOLEAN is_sleep);
    ERR_LCD_E (*lcd_SetContrast) (uint16 contrast);
    ERR_LCD_E (*lcd_SetBrightness) (uint16 brightness);
    ERR_LCD_E (*lcd_SetDisplayWindow) (uint16 left, uint16 top, uint16 right, uint16 bottom);
    ERR_LCD_E (*lcd_InvalidateRect) (uint16 left, uint16 top, uint16 right, uint16 bottom);
    ERR_LCD_E (*lcd_Invalidate) (void);
    void (*lcd_Close) (void);
    ERR_LCD_E (*lcd_RotationInvalidateRect) (uint16 left, uint16 top, uint16 right, uint16 bottom, LCD_ANGLE_E angle);
    ERR_LCD_E (*lcd_SetBrushDirection) (LCD_DIRECT_E direct_type);
    void (*lcd_Rst) (void);
    uint32 (*lcd_readid)(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);
} LCD_OPERATIONS_T;

typedef struct lcd_spec_tag
{
    uint32                  width;
    uint32                  height;
    LCD_MODE_E              mode;       // rgb or mcu
    LCD_BUS_MODE_E          bus_mode;
    LCD_BUS_WIDTH_E         bus_width;
    LCD_TIMING_U            *timing;    // lcd read/write timing parameter
    LCD_OPERATIONS_T        *operation;
    uint16                  is_use_fmark;
    uint16                  fmark_pol;//0:HIGH, 1:LOW
} LCD_SPEC_T, *LCD_SPEC_T_PTR;

typedef struct
{
    uint16                      cs_pin;     // cs pin connected with LCM
    uint16                      cd_pin;     // cd pin connected with LCM
    LCD_CTRL_MODULE_E           controller;
    LCD_SPEC_T_PTR              spec_ptr;
} LCD_SPEC_INFO_T,*LCD_SPEC_INFO_T_PTR;

typedef struct _lcd_simple_init_param_tag
{
    uint32              lcd_id;
    uint32              cs_id;
    uint32              interface_type;  // 0: 8080;1: 6800
    uint32              cd_pol;
    uint32              lcd_bits;        //0£º16bit ;1£º18bit;2: 8bit;3: 9bit
    LCD_TIMING_U        timing;
    LCD_CTRL_MODULE_E   controller;
} LCD_SIMPLE_INIT_PARAM_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    The function used to initlize the lcd. If have dual lcd,
//                  sub lcd will also  be initlized by this function.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:           If have many lcd, this function will init all lcd.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Init (void);

/*****************************************************************************/
//  Description:    The function is used to close lcd when power off.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:           When power off , call this function. If have many lcd,
//                  this function will close all lcd.
/*****************************************************************************/
PUBLIC void LCD_Close (void);
/*****************************************************************************/
//  Description:    Get the lcd basic information..
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC  ERR_LCD_E  LCD_GetInfo (
                                    LCD_ID_E  lcd_id,           //id of lcd to operate
                                    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
                                   );

/*****************************************************************************/
//  Description:    Enter/Exit sleep mode to control power consume.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         error code.
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E   LCD_EnterSleep ( 
                                         LCD_ID_E  lcd_id,     
                                         BOOLEAN is_sleep   //SCI_TRUE:enter sleep: SCI_FALSE:exit sleep mode
                                        );

/*****************************************************************************/
//  Description: Update LCD Timing
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_UpdateTiming (uint32 ahb_clk);


/*****************************************************************************/
//  Description:  display text on the main lcd.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DisplayText (uint16 x, uint16 y, char *text, uint16 len);

/*****************************************************************************/
//  Description:  clear the whole lcd to one color.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_Clear (
                            LCD_ID_E lcd_id,     //id of lcd to operate.
                            uint32 color        //color to fill the whole lcd.
                         );

/*****************************************************************************/
//  Description:  Get LCD number int the system.
//  Global resource dependence:
//  Author: Younger.Yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 LCD_GetUsedNum (void);
/*****************************************************************************/
// Description: send command to lcd
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/*****************************************************************************/
PUBLIC int32 LCD_SendCmd (uint32 cmd,uint32 lcd_id);
/*****************************************************************************/
// Description: send display data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/*****************************************************************************/
PUBLIC int32 LCD_SendData (uint32 lcd_data,uint32 lcd_id);
/*****************************************************************************/
// Description: send the command and data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/*****************************************************************************/
PUBLIC int32 LCD_SendCmdData (uint32 cmd,uint32 lcd_data,uint32 lcd_id);
/*****************************************************************************/
// Description:Read data from lcm
// Global resource dependence: NONE
// Author: Younger.yang
// Note:
/*****************************************************************************/
PUBLIC uint32 LCD_ReadRegVal (uint32 lcd_id,uint32 reg_addr);
/*****************************************************************************/
//  Description:    LCD delay
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Delayms (uint32 ms);
/*****************************************************************************/
// Description: LCD initiliazation
// Global resource dependence: NONE
// Author: Jianping.Wang
// Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SimpleInit (LCD_SIMPLE_INIT_PARAM_T *param_ptr);
/*****************************************************************************/
//Description: LCD initialization when assert
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
PUBLIC void LCD_Reset (void);
/*****************************************************************************/
//Description: is support fresh lcd by dma
//Global resource dependence:
//Author: Eddy.wei
/*****************************************************************************/
PUBLIC BOOLEAN LCDC_GetDMAInvalidateSupport(void);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_DAL_LCD_H
