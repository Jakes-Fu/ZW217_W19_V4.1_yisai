/******************************************************************************
 ** File Name:    lcdc_reg_v5.h                                                *
 ** Author:       Eddy.Wei                                                     *
 ** DATE:         09/21/2011                                                   *
 ** Copyright:    2011 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                               *
 *******************************************************************************/
/*******************************************************************************
 **                   Edit    History                                          *
 **----------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                  *
 ** 09/21/2011    Eddy.Wei         Create.                                     *
 *******************************************************************************/
#ifndef _LCDC_REG_V5_H_
#define _LCDC_REG_V5_H_
/*-----------------------------------------------------------------------------*
 **                          Dependencies                                      *
 **----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
 **                          Compiler Flag                                     *
 **----------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
#define  REG_LCDC_REG_BASE  LCDC_REG_BASE
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**-----------------------------------------------------------------------------*/
#define REG_LCDC_CTRL                   (REG_LCDC_REG_BASE + 0x0000)
#define REG_LCDC_DISP_SIZE              (REG_LCDC_REG_BASE + 0x0004)
#define REG_LCDC_LCM_START              (REG_LCDC_REG_BASE + 0x0008)
#define REG_LCDC_LCM_SIZE               (REG_LCDC_REG_BASE + 0x000c)
#define REG_LCDC_BG_COLOR               (REG_LCDC_REG_BASE + 0x0010)
#define REG_LCDC_FIFO_STATUS            (REG_LCDC_REG_BASE + 0x0014)
#define REG_LCDC_SYNC_DELAY             (REG_LCDC_REG_BASE + 0x0018)

#define REG_IMG_CTRL                    (REG_LCDC_REG_BASE + 0x0020)
#define REG_IMG_Y_BASE_ADDR             (REG_LCDC_REG_BASE + 0x0024)
#define REG_IMG_UV_BASE_ADDR            (REG_LCDC_REG_BASE + 0x0028)
#define REG_IMG_SIZE_XY                 (REG_LCDC_REG_BASE + 0x002c)
#define REG_IMG_PITCH                   (REG_LCDC_REG_BASE + 0x0030)
#define REG_IMG_DISP_XY                 (REG_LCDC_REG_BASE + 0x0034)

#define REG_OSD1_CTRL                   (REG_LCDC_REG_BASE + 0x0050)
#define REG_OSD1_BASE_ADDR              (REG_LCDC_REG_BASE + 0x0054)
#define REG_OSD1_ALPHA_BASE_ADDR        (REG_LCDC_REG_BASE + 0x0058)
#define REG_OSD1_SIZE_XY                (REG_LCDC_REG_BASE + 0x005c)
#define REG_OSD1_PITCH                  (REG_LCDC_REG_BASE + 0x0060)
#define REG_OSD1_DISP_XY                (REG_LCDC_REG_BASE + 0x0064)
#define REG_OSD1_ALPHA                  (REG_LCDC_REG_BASE + 0x0068)
#define REG_OSD1_GREY_RGB               (REG_LCDC_REG_BASE + 0x006c)
#define REG_OSD1_CK                     (REG_LCDC_REG_BASE + 0x0070)

#define REG_OSD2_CTRL                   (REG_LCDC_REG_BASE + 0x0080)
#define REG_OSD2_BASE_ADDR              (REG_LCDC_REG_BASE + 0x0084)
#define REG_OSD2_SIZE_XY                (REG_LCDC_REG_BASE + 0x008c)
#define REG_OSD2_PITCH                  (REG_LCDC_REG_BASE + 0x0090)
#define REG_OSD2_DISP_XY                (REG_LCDC_REG_BASE + 0x0094)
#define REG_OSD2_ALPHA                  (REG_LCDC_REG_BASE + 0x0098)
#define REG_OSD2_GREY_RGB               (REG_LCDC_REG_BASE + 0x009c)
#define REG_OSD2_CK                     (REG_LCDC_REG_BASE + 0x00a0)

#define REG_OSD3_CTRL                   (REG_LCDC_REG_BASE + 0x00b0)
#define REG_OSD3_BASE_ADDR              (REG_LCDC_REG_BASE + 0x00b4)
#define REG_OSD3_SIZE_XY                (REG_LCDC_REG_BASE + 0x00bc)
#define REG_OSD3_PITCH                  (REG_LCDC_REG_BASE + 0x00c0)
#define REG_OSD3_DISP_XY                (REG_LCDC_REG_BASE + 0x00c4)
#define REG_OSD3_ALPHA                  (REG_LCDC_REG_BASE + 0x00c8)
#define REG_OSD3_GREY_RGB               (REG_LCDC_REG_BASE + 0x00cc)
#define REG_OSD3_CK                     (REG_LCDC_REG_BASE + 0x00d0)

#define REG_CAP_CTRL                    (REG_LCDC_REG_BASE + 0x00e0)
#define REG_CAP_BASE_ADDR               (REG_LCDC_REG_BASE + 0x00e4)
#define REG_CAP_START_XY                (REG_LCDC_REG_BASE + 0x00e8)
#define REG_CAP_SIZE_XY                 (REG_LCDC_REG_BASE + 0x00ec)
#define REG_CAP_PITCH                   (REG_LCDC_REG_BASE + 0x00f0)

#define REG_Y2R_CTRL                    (REG_LCDC_REG_BASE + 0x0100)
#define REG_Y2R_CONTRAST                (REG_LCDC_REG_BASE + 0x0104)
#define REG_Y2R_SATURATION              (REG_LCDC_REG_BASE + 0x0108)
#define REG_Y2R_BRIGHTNESS              (REG_LCDC_REG_BASE + 0x010c)

#define REG_LCDC_IRQ_EN                 (REG_LCDC_REG_BASE + 0x0110)
#define REG_LCDC_IRQ_CLR                (REG_LCDC_REG_BASE + 0x0114)
#define REG_LCDC_IRQ_STATUS             (REG_LCDC_REG_BASE + 0x0118)
#define REG_LCDC_IRQ_RAW                (REG_LCDC_REG_BASE + 0x011c)

//The corresponding bit of LCD_CNT register.
#define LCDCNT_BM_SERIAL_S8             0
#define LCDCNT_BM_SERIAL_I2C            1
#define LCDCNT_BM_8B_PARALLEL_8080      2
#define LCDCNT_BM_8B_PARALLEL_6800      3
#define LCDCNT_CB_MODE_EN               (1 << 4)        //if "1", enable MCU buffer mode. In this mode, MCU can continue send 8 commands to LCD. 
#define LCDCNT_INF_RST                  (1 << 5)        //lcd interface reset, "1" reset lcd interface, "0" normal mode.

//The corresponding bit of LCD_ICLR register.
#define LCDICLR_INT_PIN                 1               //Interrupt pin is high when LCD interface is not busy. MCU can write bit0 of register LCDINTCLR to clear interrupt pin when no data need to be sent. After MCU send new command to LCD, the interrupt pin is automatically set high by LCD interface.
#define LCDICLR_CB_WR_DOWN              (1 << 1)        //In cb_mode, if MCU send   command number smaller than 8, MCU can write this bit to enable LCD interface execute LCD command.  
#define LCDICLR_CB_RD_DOWN              (1 << 2)        //In cb_mode, after LCD interface execute MCU command, if MCU read back data number smaller than 8, MCU can write this bit to enable LCD interface receive new MCU command.  


/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
//LCD Interface Control.
typedef struct lcd_tag
{
    VOLATILE uint32 data;
    VOLATILE uint32 clk;
    VOLATILE uint32 cnt;
    VOLATILE uint32 sts;
    VOLATILE uint32 iclr;
} lcd_s;


#ifdef CHIP_ENDIAN_LITTLE

typedef union _lcdc_ctrl_tag
{
    struct _lcdc_ctrl_map
    {
        volatile unsigned int lcd_enable        :1; //[0] 0:LCDC Disable;1:LCDC Enable
        volatile unsigned int fmark_mode        :1; //[1] 0: fmark device; 1: non-fmark device
        volatile unsigned int fmark_pol         :1; //[2] 0: fmark valid at 1; 1: fmark valid at 0
        volatile unsigned int lcdc_run          :1; //[3] 0: stop; 1:run
        volatile unsigned int dither_en         :1; //[4] 0:disable; 1:enable
        volatile unsigned int rgb_mode          :3; //[7:5] rgn_mode,only valid in display mode;
        volatile unsigned int req_gap           :8; //[15:8] The interval between 2 AHB master requests for each client.
        volatile unsigned int rgb_expand_mode   :2; //[17:16] The mode RGB555, RGB565 or RGB666 expand to RGB888
        //00 use MSBs
        //01 use 0s
        //10 use LSBs
        //11 reserved
        volatile unsigned int reserved          :14; //[31:18] Reserved ;
    } mBits ;
    volatile unsigned int dwValue ;
} LCDC_CTRL_U;


typedef union _lcdc_disp_size_tag
{
    struct _lcdc_disp_size_map
    {
        volatile unsigned int disp_size_x       :12;  //[11:0] display window horizontal size, should be >0 and <4096
        volatile unsigned int reserved_2        :4;  //[15: 12] Reserved
        volatile unsigned int disp_size_y       :12;  //[27:16] display window vertical size, should be >0 and <4096
        volatile unsigned int reserved_1        :4;  //[31:28] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_DISP_SIZE_U;

typedef union _lcdc_lcm_start_tag
{
    struct _lcdc_lcm_start_map
    {
        volatile unsigned int lcm_start_x       :12;  //[11:0] LCM refresh window start X, should be >0 and <4096, guarantee the window in display region.
        volatile unsigned int reserved_2        :4;  //[15: 12] Reserved
        volatile unsigned int lcm_start_y       :12;  //[27:16] LCM refresh window start Y, should be >0 and <4096, guarantee the window in display region.
        volatile unsigned int reserved_1        :4;  //[31:28] Reserved
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_START_U;

typedef union _lcdc_lcm_size_tag
{
    struct _lcdc_lcm_size_map
    {
        volatile unsigned int lcm_size_x        :12;  //[11:0] LCM refresh window horizontal size, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_2        :4;  //[15: 12] Reserved
        volatile unsigned int lcm_size_y        :12;  //[27:16] LCM refresh window vertical size, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_1        :4;  //[31:28] Reserved
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_SIZE_U;

typedef union _lcdc_bg_color_tag
{
    struct _lcdc_bg_color_map
    {
        volatile unsigned int bg_b              :8; //[7:0] Background blue
        volatile unsigned int bg_g              :8; //[15:8] Background green
        volatile unsigned int bg_r              :8; //[23:16] Background red
        volatile unsigned int reserved          :8; //[31:24] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_BG_COLOR_U;

typedef union _lcdc_fifo_status_tag
{
    struct _lcdc_fifo_status_map
    {
        volatile unsigned int img_y_fifo_status     :2;  //[1:0] Image layer FIFO info. When image is YUV format, it is for Y data; when image is RGB format, it is for RGB data.
        //status, '1' for full; [0] - FIFO empty status, '1' for empty.
        volatile unsigned int img_uv_fifo_status    :2;  //[3:2] Image layer FIFO info. It is active when image is YUV422 and YUV420 format. It is for UV data.
        //[3] - FIFO full status, '1' for full; [2] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_alpha_fifo_status  :2; //[5:4] Osd1 layer FIFO info. It is active when osd1 is RGB565 format with pixel alpha.
        //[5] - FIFO full status, '1' for full; [4] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_fifo_status      :2;  //[7:6] Osd1 layer FIFO info. It is for osd1 layer data.
        //[7] - FIFO full status, '1' for full; [6] - FIFO empty status, '1' for empty.
        volatile unsigned int osd2_fifo_status      :2;  //[9:8] Osd2 layer FIFO info.It is for osd2 layer data.
        //[9] - FIFO full status, '1' for full; [8] - FIFO empty status, '1' for empty.
        volatile unsigned int y2r_fifo_status       :2;  //[11:10] YUV to RGB work FIFO
        //[11] - FIFO full status, '1' for full; [10] - FIFO empty status, '1' for empty.
        volatile unsigned int dither_fifo_status    :2;  //[13:12] Dithering output FIFO, it is shared by capture and display.
        //[13] - FIFO full status, '1' for full; [12]-FIFO empty status, '1' for empty.
        volatile unsigned int output_fifo_status    :2;  //[15:14] Cross domain FIFO;
        //[15] - FIFO full status, '1' for full; [14] - FIFO empty status, '1' for empty.
        volatile unsigned int reserved              :16;  //[31:16] reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_FIFO_STATUS_U;

typedef union _lcdc_sync_delay_tag
{
    struct _lcdc_sync_delay_map
    {
        volatile unsigned int sync_delay        :20;    //[19:0] External FMARK sync delay, it is HCLK unit.
        volatile unsigned int reserved          :12;    //[31:20] reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_SYNC_DELAY_U;

typedef union _img_ctrl_tag
{
    struct _img_ctrl_map
    {
        volatile unsigned int img_en            :1;     //[0] Image layer enable; 0: disable; 1: enable
        volatile unsigned int reserved2         :3;     //[3:1] reserved
        volatile unsigned int img_format        :4;     //[7:4] Image layer data format, it supports following ones:
        //0000-YUV422;
        //0001-YUV420;
        //0010-YUV400;
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        volatile unsigned int img_y_switch      :2;     //[9:8] Image layer data endian; 0: big endian(0123); 1: little endian(3210) ;2:swap(2301)
        volatile unsigned int img_rot           :3;     //[12:10] Image layer data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int img_uv_switch     :2;     //[14:13] Image layer data endian; 0: big endian(0123); 1: little endian(3210) ;2:swap(2301)
        volatile unsigned int img_rb_switch     :1;     //[15]R/B data switch,                                                                               
        volatile unsigned int reserved1         :16;    //[31:16]
    } mBits;
    volatile unsigned int dwValue;
} IMG_CTRL_U;

typedef union _img_y_base_addr_tag
{
    struct _img_y_base_addr_map
    {
        volatile unsigned int img_y_base_addr   :30;    //[29:0]  //When image is YUV format, it is Y data base address;
        volatile unsigned int reserved          :2;     //[31:30]
    } mBits;
    volatile unsigned int dwValue;
} IMG_Y_BASE_ADDR_U;

typedef union _img_uv_base_addr_tag
{
    struct _img_uv_base_addr_map
    {
        volatile unsigned int img_uv_base_addr  :30;    //[29:0]  //When image is YUV format, it is UV base address;
        volatile unsigned int reserved          :2;     //[31:30]
    } mBits;
    volatile unsigned int dwValue;
} IMG_UV_BASE_ADDR_U;

typedef union _img_size_xy_tag
{
    struct _img_size_xy_map
    {
        volatile unsigned int img_size_x        :12;  //[11:0] Image layer window size in X, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 12] Reserved
        volatile unsigned int img_size_y        :12;  //[27:16] Image layer window size in Y, should be >0 and <4096
        volatile unsigned int reserved_1        :4;   //[31:28] Reserved
    } mBits;
    volatile unsigned int dwValue;
} IMG_SIZE_XY_U;

typedef union _img_pitch_tag
{
    struct _img_pitch_map
    {
        volatile unsigned int img_pitch         :12;      //[11:0] Image layer data storage pitch, should be >0 and <4096.
        volatile unsigned int reserved          :20;      //[31:12] Reserved
    } mBits;
    volatile unsigned int dwValue;
} IMG_PITCH_U;

typedef union _img_disp_xy_tag
{
    struct _img_disp_xy_map
    {
        volatile unsigned int img_disp_x        :12;  //[9:0] Image layer window start X position in display plane, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 10] Reserved
        volatile unsigned int img_disp_y        :12;  //[27:16] Image layer window start Y position in display plane, should be >0 and <4096
        volatile unsigned int reserved_1        :4;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} IMG_DISP_XY_U;

typedef union _osd1_ctrl_tag
{
    struct _osd1_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :2;     //[3:2] OSD layer1 alpha selection, 0: pixel alpha; 1: block alpha; 2-combo alpha (pixel alpha * block alpha)
        volatile unsigned int blk_format        :4;     //[7:4] osd data format
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        //0111-GREY;
        volatile unsigned int blk_switch        :2;    //[9:8] block data endian, 0: big endian (0123); 1: little endian (3210); 2:swap(2301)
        volatile unsigned int blk_rot           :3;    //[12:10]OSD layer1 data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int blk_alpha_switch  :2;    //[14:13] block data endian, 0: big endian (0123); 1: little endian (3210);2:swap(2301)
        volatile unsigned int blk_rb_switch     :1;    //[15]R/B data switch,      
        volatile unsigned int blk_blend_mode    :1;    //[16]OSD1 layer blending mode,      
        volatile unsigned int reserved          :15;   //[31:17] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD1_CTRL_U;

typedef union _osd2_ctrl_tag
{
    struct _osd2_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :2;     //[3:2] OSD layer2 alpha selection, 0: pixel alpha; 1: block alpha;2:combo alpha (pixel alpha * block alpha)
        volatile unsigned int blk_format        :4;     //[7:4] osd data format
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        //0111-GREY;
        volatile unsigned int blk_switch        :2;     //[9:8] block data endian, 0: big endian (0123); 1: little endian (3210); 2:swap(2301)
        volatile unsigned int blk_rot           :3;     //[12:10]OSD layer data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int reserved_2        :2;    //[14:13] Reserved
        volatile unsigned int blk_rb_switch     :1;    //[15]R/B data switch,      
        volatile unsigned int blk_blend_mode    :1;    //[16]OSD1 layer blending mode,      
        volatile unsigned int reserved_1        :15;   //[31:17] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD2_CTRL_U;

typedef union _osd3_ctrl_tag
{
    struct _osd3_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :2;     //[3:2] OSD layer3 block1 alpha selection, 0: pixel alpha; 1: block alpha; 2-combo alpha (pixel alpha * block alpha)
        volatile unsigned int blk_format        :4;     //[7:4] osd data format, it supports following ones:
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        //0111-GREY;
        volatile unsigned int blk_switch        :2;     //[9:8] block data endian, 0: big endian (0123); 1: little endian (3210); 2:swap(2301)
        volatile unsigned int blk_rot           :3;     //[12:10]OSD layer data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int reserved_2        :2;    //[14:13] Reserved
        volatile unsigned int blk_rb_switch     :1;    //[15]R/B data switch,      
        volatile unsigned int blk_blend_mode    :1;    //[16]OSD1 layer blending mode,      
        volatile unsigned int reserved_1        :15;   //[31:17] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD3_CTRL_U;


typedef union _osd_colorkey_rgb_tag
{
    struct _osd_colorkey_map
    {
        volatile unsigned int osd_ck            :24;    //[23:0] Color-key value in OSD layer
        volatile unsigned int reserved          :8;     //[31:24] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_COLORKEY_U;


typedef union _osd_base_addr_ctrl_tag
{
    struct _osd_base_addr_map
    {
        volatile unsigned int blk_base_addr     :30;    //[29:0] block base address
        volatile unsigned int reserved          :2;     //[31:30] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_BASE_ADDR_U;

typedef union _osd_alpha_base_addr_ctrl_tag
{
    struct _osd_alpha_base_addr_map
    {
        volatile unsigned int blk_alpha_base_addr   :30;    //[29:0] block base address
        volatile unsigned int reserved              :2;     //[31:30] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_BASE_ADDR_U;

typedef union _osd_size_xy_tag
{
    struct _osd_size_xy_map
    {
        volatile unsigned int blk_size_x        :12;  //[11:0] Image layer window size in X, should be >0 and <1024
        volatile unsigned int reserved_2        :4;   //[15: 12] Reserved
        volatile unsigned int blk_size_y        :12;  //[27:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_1        :4;   //[31:28] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_SIZE_XY_U;

typedef union _osd_pitch_tag
{
    struct _osd_pitch_map
    {
        volatile unsigned int blk_pitch         :12;  //[11:0] Image layer data storage pitch, should be >0 and <4096.
        volatile unsigned int reserved          :20;  //[31:12] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_PITCH_U;

typedef union _osd_disp_xy_tag
{
    struct _osd_disp_xy_map
    {
        volatile unsigned int blk_disp_x    :12;  //[11:0] Image layer window start X position in display plane, should be >0 and <4096
        volatile unsigned int reserved_2    :4;   //[15: 12] Reserved
        volatile unsigned int blk_disp_y    :12;  //[27:16] Image layer window start Y position in display plane, should be >0 and <4096
        volatile unsigned int reserved_1    :4;   //[31:28] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_DISP_XY_U;

typedef union _osd_alpha_tag
{
    struct _osd_alpha_map
    {
        volatile unsigned int blk_alpha         :8;       //[7:0] Block alpha for block1 in OSD layer1, it is in 0~255
        volatile unsigned int reserved          :24;      //[31:8] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_U;

typedef union _osd_grey_rgb_tag
{
    struct _osd_grey_rgb_map
    {
        volatile unsigned int blk_grey_rgb      :24;    //[23:0] Constant RGB for GREY data format.
        volatile unsigned int reserved          :8;     //[31:24] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_GREY_RGB_U;

typedef union _lcdc_cap_ctrl_tag
{
    struct _lcdc_cap_ctrl_map
    {
        volatile unsigned int cap_en            :1;   //[0] Capture back data control, 0: capture data to LCM or SPI module, 1: apture data to memory.
        volatile unsigned int cap_format        :2;   //[2:1] data save format, it supports following ones:(it is active only when capture back into memory)
        //00: RGB888, 
        //01: rgb666, 
        //10: RGB565, 
        //11: reserved
        volatile unsigned int cap_switch        :2;   //[4:3] Capture endian, 0: big endian, 1: little endian; 2:swap(2301)
        volatile unsigned int cap_master_en     :1;   //[5] Capture endian, 0: share AHB master1 through arbiter, 1: use AHB master2 individual
        volatile unsigned int cap_master_mode   :2;   //[7:6] Capture master mode,, it supports following ones:
        //00-fixed address;;
        //01-increamenting address;
        //10-increamenting address with wrap8;
        //11-reserved;
        volatile unsigned int cap_rot           :3;   //[10:8]cap data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int reserved_2        :4;   //[14:11] Reserved
        volatile unsigned int cap_rb_switch     :1;   //[15]R/B data switch,      
        volatile unsigned int cap_mode          :1;   //[16]0- normal mode;1- capture only OSDs mode, it works with cap_en='1';
        volatile unsigned int reserved_1        :15;  //[31:17] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_CAP_CTRL_U;

typedef union _cap_base_addr_tag
{
    struct _cap_base_addr_map
    {
        volatile unsigned int base_addr         :30;    //[29:0] Capture base address
        volatile unsigned int reserved          :2;     //[31:30] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_BASE_ADDR_U;

typedef union _cap_size_xy_tag
{
    struct _cap_size_xy_map
    {
        volatile unsigned int cap_size_x        :12;  //[9:0] Image layer window size in X, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 10] Reserved
        volatile unsigned int cap_size_y        :12;  //[25:16] Image layer window size in Y, should be >0 and <4096
        volatile unsigned int reserved_1        :4;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_SIZE_XY_U;

typedef union _cap_pitch_tag
{
    struct _cap_pitch_map
    {
        volatile unsigned int cap_pitch         :12;      //[11:0] Image layer data storage pitch, should be >0 and <4096.
        volatile unsigned int reserved          :20;      //[31:12] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_PITCH_U;

typedef union _cap_start_xy_tag
{
    struct _cap_start_xy_map
    {
        volatile unsigned int cap_start_x       :12;  //[11:0] Image layer window start X position in display plane, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 12] Reserved
        volatile unsigned int cap_start_y       :12;  //[27:16] Image layer window start Y position in display plane, should be >0 and <4096
        volatile unsigned int reserved_1        :4;   //[31:28] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_START_XY_U;

typedef union _y2r_ctrl_tag
{
    struct _y2r_ctrl_map
    {
        volatile unsigned int upsample_mode     :1;     //[0] UV horizontal up-sampling mode, 0: duplicate, 1: average
        volatile unsigned int reserved          :31;    //[31:1] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CTRL_U;

typedef union _y2r_contrast_tag
{
    struct _y2r_contrast_map
    {
        volatile unsigned int contrast          :8;     //[7:0] Contrast config, 0~255
        volatile unsigned int reserved          :24;    //[31:8] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CONTRAST_U;

typedef union _y2r_saturation_tag
{
    struct _y2r_saturation_map
    {
        volatile unsigned int saturation        :8;     //[7:0] Saturation config, 0~255
        volatile unsigned int reserved          :24;    //[31:8] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_SATURATION_U;

typedef union _y2r_brightness_tag
{
    struct _y2r_brightness_map
    {
        volatile unsigned int brightness        :9;     //[8:0] Brightness config (S1.8), -256~255
        volatile unsigned int reserved          :23;    //[31:9] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_BRIGHTNESS_U;


typedef union _lcdc_irq_en_tag
{
    struct _lcdc_irq_en_map
    {
        volatile unsigned int irq_lcdc_done_en  :1;     //[0] Enable LCDC_DONE interrupt
        volatile unsigned int irq_fmark_en      :1;     //[1] Enable FMARK interrupt, the interrupt is for LCDC detect a FMARK input.
        volatile unsigned int reserved          :30;    //[31:2] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_EN_U;

typedef union _lcdc_irq_clr_tag
{
    struct _lcdc_irq_clr_map
    {
        volatile unsigned int irq_lcdc_done_clr  :1;     //[0] Write '1' to clear the DISP_DONE interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_fmark_clr      :1;     //[1] Write '1' to clear the FMARK interrupt bit, and itself is cleared by HW.
        volatile unsigned int reserved           :30;    //[31:2] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_CLR_U;


typedef union _lcdc_irq_status_tag
{
    struct _lcdc_irq_status_map
    {
        volatile unsigned int irq_lcdc_done_status  :1;     //[0] DISP_DONE interrupt status
        volatile unsigned int irq_fmark_status      :1;     //[1] FMARK interrupt status
        volatile unsigned int reserved              :30;    //[31:2] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_STATUS_U;

typedef union _lcdc_irq_raw_tag
{
    struct _lcdc_irq_raw_map
    {
        volatile unsigned int irq_lcdc_done_raw  :1;     //[0] DISP_DONE raw interrupt
        volatile unsigned int irq_fmark_raw      :1;     //[1] FMARK raw interrupt
        volatile unsigned int reserved           :30;    //[31:2] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_RAW_U;

#else  

typedef union _lcdc_ctrl_tag
{
    struct _lcdc_ctrl_map
    {
        volatile unsigned int reserved          :14; //[31:18] Reserved ;
        volatile unsigned int rgb_expand_mode   :2; //[17:16] The mode RGB555, RGB565 or RGB666 expand to RGB888
        //00 use MSBs
        //01 use 0s
        //10 use LSBs
        //11 reserved
        volatile unsigned int req_gap           :8; //[15:8] The interval between 2 AHB master requests for each client.
        volatile unsigned int rgb_mode          :3; //[7:5] rgn_mode,only valid in display mode;
        volatile unsigned int dither_en         :1; //[4] 0:disable; 1:enable
        volatile unsigned int lcdc_run          :1; //[3] 0: stop; 1:run
        volatile unsigned int fmark_pol         :1; //[2] 0: fmark valid at 1; 1: fmark valid at 0
        volatile unsigned int fmark_mode        :1; //[1] 0: fmark device; 1: non-fmark device
        volatile unsigned int lcd_enable        :1; //[0] 0:LCDC Disable;1:LCDC Enable
    } mBits ;
    volatile unsigned int dwValue ;
} LCDC_CTRL_U;


typedef union _lcdc_disp_size_tag
{
    struct _lcdc_disp_size_map
    {
        volatile unsigned int reserved_1        :4;  //[31:28] Reserved
        volatile unsigned int disp_size_y       :12;  //[27:16] display window vertical size, should be >0 and <4096
        volatile unsigned int reserved_2        :4;  //[15: 12] Reserved
        volatile unsigned int disp_size_x       :12;  //[11:0] display window horizontal size, should be >0 and <4096
    } mBits;
    volatile unsigned int dwValue;
} LCDC_DISP_SIZE_U;

typedef union _lcdc_lcm_start_tag
{
    struct _lcdc_lcm_start_map
    {
        volatile unsigned int reserved_1        :4;  //[31:28] Reserved
        volatile unsigned int lcm_start_y       :12;  //[27:16] LCM refresh window start Y, should be >0 and <4096, guarantee the window in display region.
        volatile unsigned int reserved_2        :4;  //[15: 12] Reserved
        volatile unsigned int lcm_start_x       :12;  //[11:0] LCM refresh window start X, should be >0 and <4096, guarantee the window in display region.
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_START_U;

typedef union _lcdc_lcm_size_tag
{
    struct _lcdc_lcm_size_map
    {
        volatile unsigned int reserved_1        :4;  //[31:28] Reserved
        volatile unsigned int lcm_size_y        :12;  //[27:16] LCM refresh window vertical size, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_2        :4;  //[15: 12] Reserved
        volatile unsigned int lcm_size_x        :12;  //[11:0] LCM refresh window horizontal size, should be >0 and <1024, guarantee the window in display region.
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_SIZE_U;

typedef union _lcdc_bg_color_tag
{
    struct _lcdc_bg_color_map
    {
        volatile unsigned int reserved          :8; //[31:24] Reserved
        volatile unsigned int bg_r              :8; //[23:16] Background red
        volatile unsigned int bg_g              :8; //[15:8] Background green
        volatile unsigned int bg_b              :8; //[7:0] Background blue
    } mBits;
    volatile unsigned int dwValue;
} LCDC_BG_COLOR_U;

typedef union _lcdc_fifo_status_tag
{
    struct _lcdc_fifo_status_map
    {
        volatile unsigned int reserved              :16;  //[31:16] reserved
        volatile unsigned int output_fifo_status    :2;  //[15:14] Cross domain FIFO;
        //[15] - FIFO full status, '1' for full; [14] - FIFO empty status, '1' for empty.
        volatile unsigned int dither_fifo_status    :2;  //[13:12] Dithering output FIFO, it is shared by capture and display.
        //[13] - FIFO full status, '1' for full; [12]-FIFO empty status, '1' for empty.
        volatile unsigned int y2r_fifo_status       :2;  //[11:10] YUV to RGB work FIFO
        //[11] - FIFO full status, '1' for full; [10] - FIFO empty status, '1' for empty.
        volatile unsigned int osd2_fifo_status      :2;  //[9:8] Osd2 layer FIFO info.It is for osd2 layer data.
        //[9] - FIFO full status, '1' for full; [8] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_fifo_status      :2;  //[7:6] Osd1 layer FIFO info. It is for osd1 layer data.
        //[7] - FIFO full status, '1' for full; [6] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_alpha_fifo_status  :2; //[5:4] Osd1 layer FIFO info. It is active when osd1 is RGB565 format with pixel alpha.
        //[5] - FIFO full status, '1' for full; [4] - FIFO empty status, '1' for empty.
        volatile unsigned int img_uv_fifo_status    :2;  //[3:2] Image layer FIFO info. It is active when image is YUV422 and YUV420 format. It is for UV data.
        //[3] - FIFO full status, '1' for full; [2] - FIFO empty status, '1' for empty.
        volatile unsigned int img_y_fifo_status     :2;  //[1:0] Image layer FIFO info. When image is YUV format, it is for Y data; when image is RGB format, it is for RGB data.
        //status, '1' for full; [0] - FIFO empty status, '1' for empty.
    } mBits;
    volatile unsigned int dwValue;
} LCDC_FIFO_STATUS_U;

typedef union _lcdc_sync_delay_tag
{
    struct _lcdc_sync_delay_map
    {
        volatile unsigned int reserved          :12;    //[31:20] reserved
        volatile unsigned int sync_delay        :20;    //[19:0] External FMARK sync delay, it is HCLK unit.
    } mBits;
    volatile unsigned int dwValue;
} LCDC_SYNC_DELAY_U;

typedef union _img_ctrl_tag
{
    struct _img_ctrl_map
    {
        volatile unsigned int reserved1         :16;    //[31:16]
        volatile unsigned int img_rb_switch     :1;     //[15]R/B data switch,                                                                               
        volatile unsigned int img_uv_switch     :2;     //[14:13] Image layer data endian; 0: big endian(0123); 1: little endian(3210) ;2:swap(2301)
        volatile unsigned int img_rot           :3;     //[12:10] Image layer data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int img_y_switch      :2;     //[9:8] Image layer data endian; 0: big endian(0123); 1: little endian(3210) ;2:swap(2301)
        volatile unsigned int img_format        :4;     //[7:4] Image layer data format, it supports following ones:
        //0000-YUV422;
        //0001-YUV420;
        //0010-YUV400;
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        volatile unsigned int reserved2         :3;     //[3:1] reserved
        volatile unsigned int img_en            :1;     //[0] Image layer enable; 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} IMG_CTRL_U;

typedef union _img_y_base_addr_tag
{
    struct _img_y_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30]
        volatile unsigned int img_y_base_addr   :30;    //[29:0]  //When image is YUV format, it is Y data base address;
    } mBits;
    volatile unsigned int dwValue;
} IMG_Y_BASE_ADDR_U;

typedef union _img_uv_base_addr_tag
{
    struct _img_uv_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30]
        volatile unsigned int img_uv_base_addr  :30;    //[29:0]  //When image is YUV format, it is UV base address;
    } mBits;
    volatile unsigned int dwValue;
} IMG_UV_BASE_ADDR_U;

typedef union _img_size_xy_tag
{
    struct _img_size_xy_map
    {
        volatile unsigned int reserved_1        :4;   //[31:28] Reserved
        volatile unsigned int img_size_y        :12;  //[27:16] Image layer window size in Y, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 12] Reserved
        volatile unsigned int img_size_x        :12;  //[11:0] Image layer window size in X, should be >0 and <4096
    } mBits;
    volatile unsigned int dwValue;
} IMG_SIZE_XY_U;

typedef union _img_pitch_tag
{
    struct _img_pitch_map
    {
        volatile unsigned int reserved          :20;      //[31:12] Reserved
        volatile unsigned int img_pitch         :12;      //[11:0] Image layer data storage pitch, should be >0 and <4096.
    } mBits;
    volatile unsigned int dwValue;
} IMG_PITCH_U;

typedef union _img_disp_xy_tag
{
    struct _img_disp_xy_map
    {
        volatile unsigned int reserved_1        :4;   //[31:26] Reserved
        volatile unsigned int img_disp_y        :12;  //[27:16] Image layer window start Y position in display plane, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 10] Reserved
        volatile unsigned int img_disp_x        :12;  //[9:0] Image layer window start X position in display plane, should be >0 and <4096
    } mBits;
    volatile unsigned int dwValue;
} IMG_DISP_XY_U;

typedef union _osd1_ctrl_tag
{
    struct _osd1_ctrl_map
    {
        volatile unsigned int reserved          :15;   //[31:17] Reserved
        volatile unsigned int blk_blend_mode    :1;    //[16]OSD1 layer blending mode,      
        volatile unsigned int blk_rb_switch     :1;    //[15]R/B data switch,      
        volatile unsigned int blk_alpha_switch  :2;    //[14:13] block data endian, 0: big endian (0123); 1: little endian (3210);2:swap(2301)
        volatile unsigned int blk_rot           :3;    //[12:10]OSD layer1 data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int blk_switch        :2;    //[9:8] block data endian, 0: big endian (0123); 1: little endian (3210); 2:swap(2301)
        volatile unsigned int blk_format        :4;     //[7:4] osd data format
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        //0111-GREY;
        volatile unsigned int blk_alpha_sel     :2;     //[3:2] OSD layer1 alpha selection, 0: pixel alpha; 1: block alpha; 2-combo alpha (pixel alpha * block alpha)
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD1_CTRL_U;

typedef union _osd2_ctrl_tag
{
    struct _osd2_ctrl_map
    {
        volatile unsigned int reserved_1        :15;   //[31:17] Reserved
        volatile unsigned int blk_blend_mode    :1;    //[16]OSD1 layer blending mode,      
        volatile unsigned int blk_rb_switch     :1;    //[15]R/B data switch,      
        volatile unsigned int reserved_2        :2;    //[14:13] Reserved
        volatile unsigned int blk_rot           :3;     //[12:10]OSD layer data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int blk_switch        :2;     //[9:8] block data endian, 0: big endian (0123); 1: little endian (3210); 2:swap(2301)
        volatile unsigned int blk_format        :4;     //[7:4] osd data format
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        //0111-GREY;
        volatile unsigned int blk_alpha_sel     :2;     //[3:2] OSD layer2 alpha selection, 0: pixel alpha; 1: block alpha;2:combo alpha (pixel alpha * block alpha)
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD2_CTRL_U;

typedef union _osd3_ctrl_tag
{
    struct _osd3_ctrl_map
    {
        volatile unsigned int reserved_1        :15;   //[31:17] Reserved
        volatile unsigned int blk_blend_mode    :1;    //[16]OSD1 layer blending mode,      
        volatile unsigned int blk_rb_switch     :1;    //[15]R/B data switch,      
        volatile unsigned int reserved_2        :2;    //[14:13] Reserved
        volatile unsigned int blk_rot           :3;     //[12:10]OSD layer data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int blk_switch        :2;     //[9:8] block data endian, 0: big endian (0123); 1: little endian (3210); 2:swap(2301)
        volatile unsigned int blk_format        :4;     //[7:4] osd data format, it supports following ones:
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        //0111-GREY;
        volatile unsigned int blk_alpha_sel     :2;     //[3:2] OSD layer3 block1 alpha selection, 0: pixel alpha; 1: block alpha; 2-combo alpha (pixel alpha * block alpha)
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD3_CTRL_U;


typedef union _osd_colorkey_rgb_tag
{
    struct _osd_colorkey_map
    {
        volatile unsigned int reserved          :8;     //[31:24] Reserved
        volatile unsigned int osd_ck            :24;    //[23:0] Color-key value in OSD layer
    } mBits;
    volatile unsigned int dwValue;
} OSD_COLORKEY_U;


typedef union _osd_base_addr_ctrl_tag
{
    struct _osd_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30] Reserved
        volatile unsigned int blk_base_addr     :30;    //[29:0] block base address
    } mBits;
    volatile unsigned int dwValue;
} OSD_BASE_ADDR_U;

typedef union _osd_alpha_base_addr_ctrl_tag
{
    struct _osd_alpha_base_addr_map
    {
        volatile unsigned int reserved              :2;     //[31:30] Reserved
        volatile unsigned int blk_alpha_base_addr   :30;    //[29:0] block base address
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_BASE_ADDR_U;

typedef union _osd_size_xy_tag
{
    struct _osd_size_xy_map
    {
        volatile unsigned int reserved_1        :4;   //[31:28] Reserved
        volatile unsigned int blk_size_y        :12;  //[27:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_2        :4;   //[15: 12] Reserved
        volatile unsigned int blk_size_x        :12;  //[11:0] Image layer window size in X, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} OSD_SIZE_XY_U;

typedef union _osd_pitch_tag
{
    struct _osd_pitch_map
    {
        volatile unsigned int reserved          :20;  //[31:12] Reserved
        volatile unsigned int blk_pitch         :12;  //[11:0] Image layer data storage pitch, should be >0 and <4096.
    } mBits;
    volatile unsigned int dwValue;
} OSD_PITCH_U;

typedef union _osd_disp_xy_tag
{
    struct _osd_disp_xy_map
    {
        volatile unsigned int reserved_1    :4;   //[31:28] Reserved
        volatile unsigned int blk_disp_y    :12;  //[27:16] Image layer window start Y position in display plane, should be >0 and <4096
        volatile unsigned int reserved_2    :4;   //[15: 12] Reserved
        volatile unsigned int blk_disp_x    :12;  //[11:0] Image layer window start X position in display plane, should be >0 and <4096
    } mBits;
    volatile unsigned int dwValue;
} OSD_DISP_XY_U;

typedef union _osd_alpha_tag
{
    struct _osd_alpha_map
    {
        volatile unsigned int reserved          :24;      //[31:8] Reserved
        volatile unsigned int blk_alpha         :8;       //[7:0] Block alpha for block1 in OSD layer1, it is in 0~255
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_U;

typedef union _osd_grey_rgb_tag
{
    struct _osd_grey_rgb_map
    {
        volatile unsigned int reserved          :8;     //[31:24] Reserved
        volatile unsigned int blk_grey_rgb      :24;    //[23:0] Constant RGB for GREY data format.
    } mBits;
    volatile unsigned int dwValue;
} OSD_GREY_RGB_U;

typedef union _lcdc_cap_ctrl_tag
{
    struct _lcdc_cap_ctrl_map
    {
        volatile unsigned int reserved_1        :15;  //[31:17] Reserved
        volatile unsigned int cap_mode          :1;   //[16]0- normal mode;1- capture only OSDs mode, it works with cap_en='1';
        volatile unsigned int cap_rb_switch     :1;   //[15]R/B data switch,      
        volatile unsigned int reserved_2        :4;   //[14:11] Reserved
        volatile unsigned int cap_rot           :3;   //[10:8]cap data rotationn, it supports following ones:
        //000-rotation 0;
        //001-rotation 90;
        //010-rotation 180;
        //011-rotation 270;
        //100-mirror;
        //101-flip;
        volatile unsigned int cap_master_mode   :2;   //[7:6] Capture master mode,, it supports following ones:
        //00-fixed address;;
        //01-increamenting address;
        //10-increamenting address with wrap8;
        //11-reserved;
        volatile unsigned int cap_master_en     :1;   //[5] Capture endian, 0: share AHB master1 through arbiter, 1: use AHB master2 individual
        volatile unsigned int cap_switch        :2;   //[4:3] Capture endian, 0: big endian, 1: little endian; 2:swap(2301)
        volatile unsigned int cap_format        :2;   //[2:1] data save format, it supports following ones:(it is active only when capture back into memory)
        //00: RGB888, 
        //01: rgb666, 
        //10: RGB565, 
        //11: reserved
        volatile unsigned int cap_en            :1;   //[0] Capture back data control, 0: capture data to LCM or SPI module, 1: apture data to memory.
    } mBits;
    volatile unsigned int dwValue;
} LCDC_CAP_CTRL_U;

typedef union _cap_base_addr_tag
{
    struct _cap_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30] Reserved
        volatile unsigned int base_addr         :30;    //[29:0] Capture base address
    } mBits;
    volatile unsigned int dwValue;
} CAP_BASE_ADDR_U;

typedef union _cap_size_xy_tag
{
    struct _cap_size_xy_map
    {
        volatile unsigned int reserved_1        :4;   //[31:26] Reserved
        volatile unsigned int cap_size_y        :12;  //[25:16] Image layer window size in Y, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 10] Reserved        
        volatile unsigned int cap_size_x        :12;  //[9:0] Image layer window size in X, should be >0 and <4096
    } mBits;
    volatile unsigned int dwValue;
} CAP_SIZE_XY_U;

typedef union _cap_pitch_tag
{
    struct _cap_pitch_map
    {
        volatile unsigned int reserved          :20;      //[31:12] Reserved
        volatile unsigned int cap_pitch         :12;      //[11:0] Image layer data storage pitch, should be >0 and <4096.
    } mBits;
    volatile unsigned int dwValue;
} CAP_PITCH_U;

typedef union _cap_start_xy_tag
{
    struct _cap_start_xy_map
    {
        volatile unsigned int reserved_1        :4;   //[31:28] Reserved
        volatile unsigned int cap_start_y       :12;  //[27:16] Image layer window start Y position in display plane, should be >0 and <4096
        volatile unsigned int reserved_2        :4;   //[15: 12] Reserved
        volatile unsigned int cap_start_x       :12;  //[11:0] Image layer window start X position in display plane, should be >0 and <4096
    } mBits;
    volatile unsigned int dwValue;
} CAP_START_XY_U;

typedef union _y2r_ctrl_tag
{
    struct _y2r_ctrl_map
    {
        volatile unsigned int reserved          :31;    //[31:1] Reserved
        volatile unsigned int upsample_mode     :1;     //[0] UV horizontal up-sampling mode, 0: duplicate, 1: average
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CTRL_U;

typedef union _y2r_contrast_tag
{
    struct _y2r_contrast_map
    {
        volatile unsigned int reserved          :24;    //[31:8] Reserved
        volatile unsigned int contrast          :8;     //[7:0] Contrast config, 0~255
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CONTRAST_U;

typedef union _y2r_saturation_tag
{
    struct _y2r_saturation_map
    {
        volatile unsigned int reserved          :24;    //[31:8] Reserved
        volatile unsigned int saturation        :8;     //[7:0] Saturation config, 0~255
    } mBits;
    volatile unsigned int dwValue;
} Y2R_SATURATION_U;

typedef union _y2r_brightness_tag
{
    struct _y2r_brightness_map
    {
        volatile unsigned int reserved          :23;    //[31:9] Reserved
        volatile unsigned int brightness        :9;     //[8:0] Brightness config (S1.8), -256~255
    } mBits;
    volatile unsigned int dwValue;
} Y2R_BRIGHTNESS_U;


typedef union _lcdc_irq_en_tag
{
    struct _lcdc_irq_en_map
    {
        volatile unsigned int reserved          :30;    //[31:2] Reserved
        volatile unsigned int irq_fmark_en      :1;     //[1] Enable FMARK interrupt, the interrupt is for LCDC detect a FMARK input.
        volatile unsigned int irq_lcdc_done_en  :1;     //[0] Enable LCDC_DONE interrupt
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_EN_U;

typedef union _lcdc_irq_clr_tag
{
    struct _lcdc_irq_clr_map
    {
        volatile unsigned int reserved           :30;    //[31:2] Reserved
        volatile unsigned int irq_fmark_clr      :1;     //[1] Write '1' to clear the FMARK interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_lcdc_done_clr  :1;     //[0] Write '1' to clear the DISP_DONE interrupt bit, and itself is cleared by HW.
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_CLR_U;


typedef union _lcdc_irq_status_tag
{
    struct _lcdc_irq_status_map
    {
        volatile unsigned int reserved              :30;    //[31:2] Reserved
        volatile unsigned int irq_fmark_status      :1;     //[1] FMARK interrupt status
        volatile unsigned int irq_lcdc_done_status  :1;     //[0] DISP_DONE interrupt status
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_STATUS_U;

typedef union _lcdc_irq_raw_tag
{
    struct _lcdc_irq_raw_map
    {
        volatile unsigned int reserved           :30;    //[31:2] Reserved
        volatile unsigned int irq_fmark_raw      :1;     //[1] FMARK raw interrupt
        volatile unsigned int irq_lcdc_done_raw  :1;     //[0] DISP_DONE raw interrupt
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_RAW_U;

#endif   
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
