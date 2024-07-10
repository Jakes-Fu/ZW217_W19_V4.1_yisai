/***************************************************************************************
** File Name:      lcdc_common.h                                                   *
** DATE:           27/09/2005                                                          *
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:    this file defines macro and declare internal interfaces of LCDC     *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                 *
** 27/09/2005              Create.                                                     *
****************************************************************************************/
#ifndef _LCDC_COMMON_H_
#define _LCDC_COMMON_H_

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/
#include "sci_types.h"
/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                                              *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


#define GAMMA_SEL 1
#define LCDC_UPDATE_PIXELS_MIN  64

/****************************************************************************************/
// LCM REGISTER ADDRESS MACRO DEFINE
/****************************************************************************************/
#define LCM_INTSOURCE_ADDR          0x60001c84
/****************************************************************************************/


/****************************************************************************************/
// YUV TO RGB CONVERTION MATRIX
/****************************************************************************************/
#define TV_PAL_SEL_MATRIX_A1            0x20e
#define TV_PAL_SEL_MATRIX_A2            0xf68
#define TV_PAL_SEL_MATRIX_A3            0x1c2
#define TV_PAL_SEL_MATRIX_B1            0x408
#define TV_PAL_SEL_MATRIX_B2            0xed6
#define TV_PAL_SEL_MATRIX_B3            0xe88
#define TV_PAL_SEL_MATRIX_C1            0x0c9
#define TV_PAL_SEL_MATRIX_C2            0x1c1
#define TV_PAL_SEL_MATRIX_C3            0xfb7

#define TV_NTSC_SEL_1_MATRIX_A1         0x20e
#define TV_NTSC_SEL_1_MATRIX_A2         0x1ff
#define TV_NTSC_SEL_1_MATRIX_A3         0x122
#define TV_NTSC_SEL_1_MATRIX_B1         0x408
#define TV_NTSC_SEL_1_MATRIX_B2         0xf59
#define TV_NTSC_SEL_1_MATRIX_B3         0xdf6
#define TV_NTSC_SEL_1_MATRIX_C1         0x0c9
#define TV_NTSC_SEL_1_MATRIX_C2         0xea8
#define TV_NTSC_SEL_1_MATRIX_C3         0x0e9

#define TV_NTSC_SEL_0_MATRIX_A1         0x107
#define TV_NTSC_SEL_0_MATRIX_A2         0x115
#define TV_NTSC_SEL_0_MATRIX_A3         0x063
#define TV_NTSC_SEL_0_MATRIX_B1         0x204
#define TV_NTSC_SEL_0_MATRIX_B2         0xfac
#define TV_NTSC_SEL_0_MATRIX_B3         0xf54
#define TV_NTSC_SEL_0_MATRIX_C1         0x065
#define TV_NTSC_SEL_0_MATRIX_C2         0xefb
#define TV_NTSC_SEL_0_MATRIX_C3         0x091
/****************************************************************************************/
//SC8800H AHB_CTL0 REGISTER MACRO DEFINE
/****************************************************************************************/
#define SC8800H_AHB_CTL0_BASE   0x20900200
#define SC8800H_NFC_MEM_DLY      0x8b000058
#define SC8800H_LCDC_ENA_BIT        3
#define SC8800H_TVE_ENA_BIT     4
#define SC8800H_LCM_ENA_BIT     9
/****************************************************************************************/
//SC8800H AHB_SOFT_RST REGISTER MACRO DEFINE
/****************************************************************************************/
#define SC8800H_AHB_SOFT_RST_ADDR   0x20900210
#define SC8800H_LCDC_SOFT_RST_BIT   3
#define SC8800H_TVE_SOFT_RST_BIT        4
#define SC8800H_NLC_SOFT_RST_BIT        5
/****************************************************************************************/
//SC8800H SPI RERIGSTER ADDRESS MACRO DEFINE
/****************************************************************************************/
#define SC8800H_LCDC_SPI            0x8f000000
/****************************************************************************************/

/****************************************************************************************/
// IRQ INTERRUPT ENABLE REGISTER¡¡
/****************************************************************************************/
#define SC8800H_LCDC_IRQ_BIT        28

/****************************************************************************************/
// GLOBAL CONTROL REGISTER ADDRESS MACRO DEFINE
/****************************************************************************************/
#define GLB_GEN1_REG_ADDR       0x8b000018
#define GLB_GEN4_REG_ADDR       0x8b000060
#define GLB_VPLL_MN_REG_ADDR    0x8b000068
#define GLB_LDO_CTL3_REG_ADDR   0x8b00006c
//#define V_PLLMN_WE_BIT            20
/****************************************************************************************/

/****************************************************************************************/
// CHIP PIN SELECT REGISTER ADDRESS MACRO DEFINE
/****************************************************************************************/
#define CHIP_PIN_SEL_REG2_ADDR      0x8c000008
#define CHIP_PIN_SEL_REG4_ADDR      0x8c000010
#define CHIP_PIN_SEL_REG5_ADDR      0x8c000014
#define CHIP_PIN_SEL_REG6_ADDR      0x8c000018
#define CHIP_PIN_SEL_REG7_ADDR      0x8c00001c
#define SHIP_PIN_SEL_REG15_ADDR     0x8c00003c
#define CHIP_PIN_SEL_REG18_ADDR     0x8c000048
#define CHIP_PIN_SEL_REG22_ADDR     0x8c000058
#define CHIP_PIN_SEL_REG23_ADDR     0x8c00005c
#define CHIP_PIN_SEL_REG24_ADDR     0x8c000060
#define CHIP_PIN_DRV_REG22_ADDR     0x8c0000bc
#define CHIP_PIN_DRV_REG23_ADDR     0x8c0000c0
#define CHIP_PIN_DRV_REG24_ADDR     0x8c0000c4

/****************************************************************************************/
//SC8800H SPI REGISTER ADDRESS MACRO
/****************************************************************************************/
#define SC8800H_UART_IEN            0x8f000010
#define SC8800H_UART_CTL0       0x8f000018
/****************************************************************************************/
// LCD COLOR CORRECTION MATRIX
/****************************************************************************************/
#define COLORMATRIX_A1  0x100
#define COLORMATRIX_A2  0x0
#define COLORMATRIX_A3  0x0
#define COLORMATRIX_B1  0x0
#define COLORMATRIX_B2  0x100
#define COLORMATRIX_B3  0x0
#define COLORMATRIX_C1  0x0
#define COLORMATRIX_C2  0x0
#define COLORMATRIX_C3  0x100

/****************************************************************************************/
// MACRO
/****************************************************************************************/
#define LCD_BLOCK_NUM   6               //LCD display block total number
/****************************************************************************************/
//INTERNAL INTERFACE DECLARE
/****************************************************************************************/
typedef union _LCDC_RGB_LCD_CTRL_U
{
    struct RGB_LCD_CTRL_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint res_1       :1; //1bit,ScalingUp,for LCD,bypass scalingUP mode
        volatile uint hsync_pol   :1; //1bit,0:LCD_HSYNC is valid when it is 1'b1;1:LCD_HSYNC is valid when it is 1'b0
        volatile uint vsync_pol   :1; //1bit,0:LCD_VSYNC is valid when it is 1'b1;1:LCD_VSYNC is valid when it is 1'b0
        volatile uint enable_pol  :1; //1bit,0:LCD_Enable is valid when it is 1'b1;1:LCD_Enable is valid when it is 1'b0
        volatile uint lcd_bpp     :2; //2bits,Bit Per Pixel;00¡êo888 01¡êo666;10: 565  11: Rsved
        volatile uint trans_cycle :1; //1bits,0: 1 cycle per Pixel;1: 3 cycle per Pixel
        volatile uint rb_format   :1; //1bits,0: pixel output sequence is RGB  1: output sequence is BGR
        volatile uint res         :24;//bit31~bit8
#else
        volatile uint res         :24;//bit31~bit8
        volatile uint rb_format   :1; //1bits,0: pixel output sequence is RGB  1: output sequence is BGR
        volatile uint trans_cycle :1; //1bits,0: 1 cycle per Pixel;1: 3 cycle per Pixel
        volatile uint lcd_bpp     :2; //2bits,Bit Per Pixel;00¡êo888 01¡êo666;10: 565  11: Rsved
        volatile uint enable_pol  :1; //1bit,0:LCD_Enable is valid when it is 1'b1;1:LCD_Enable is valid when it is 1'b0
        volatile uint vsync_pol   :1; //1bit,0:LCD_VSYNC is valid when it is 1'b1;1:LCD_VSYNC is valid when it is 1'b0
        volatile uint hsync_pol   :1; //1bit,0:LCD_HSYNC is valid when it is 1'b1;1:LCD_HSYNC is valid when it is 1'b0
        volatile uint res_1       :1; //1bit,ScalingUp,for LCD,bypass scalingUP mode
#endif
    } mBits;
    volatile uint32 value;
} LCDC_RGB_LCD_CTRL_U;

typedef struct _LCDC_RGB_SPEC_T_tag
{
    //  LCDC_RGB_LCD_TIMING0_U  timing0;
    //  LCDC_RGB_LCD_TIMING1_U  timing1;
    LCDC_RGB_LCD_CTRL_U     lcd_ctrl;
} LCDC_RGB_SPEC_T;

typedef struct _LCDC_RGB_INT_ENA_T_tag
{
    uint16 end_of_frame;  //1bit:0:disable;1:enable
    uint16 start_of_frame;//1bit:0:disable;1:enable
} LCDC_RGB_INT_ENA_T;

typedef union _LCDC_RGB_LCD_TIMING0_U
{
    struct RGB_TIMING0_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint bfw :8;//bit7~bit0,beginning of frame horizontal sync clock wait count - 1
        volatile uint efw :8;//bit15~bit8,end of frame horizontal sync clock wait count - 1
        volatile uint vsw :8;//bit23~vit16,Vertical Sync Pulse Width - 1
        volatile uint res :8;//bit31~bit24,reserved
#else
        volatile uint res :8;//bit31~bit24,reserved
        volatile uint vsw :8;//bit23~vit16,Vertical Sync Pulse Width - 1
        volatile uint efw :8;//bit15~bit8,end of frame horizontal sync clock wait count - 1
        volatile uint bfw :8;//bit7~bit0,beginning of frame horizontal sync clock wait count - 1
#endif
    } mBits;
    volatile uint32 value;
} LCDC_RGB_LCD_TIMING0_U;

typedef union _LCDC_RGB_LCD_TIMING1_U
{
    struct RGB_TIMING1_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint blw :8;//8bits,beginning of line pixel cloxk wait count - 1
        volatile uint elw :8;//8bits,end of line pixel cloxk wait count - 1
        volatile uint hsw :8;//8bits,Horizontal sync Pulse width - 1
        volatile uint res :8;//bit31~bit24,reserved
#else
        volatile uint res :8;//bit31~bit24,reserved
        volatile uint hsw :8;//8bits,Horizontal sync Pulse width - 1
        volatile uint elw :8;//8bits,end of line pixel cloxk wait count - 1
        volatile uint blw :8;//8bits,beginning of line pixel cloxk wait count - 1

#endif
    } mBits;
    volatile uint32 value;
} LCDC_RGB_LCD_TIMING1_U;

typedef struct _LCDC_RGB_INFO_T_tag
{
    LCDC_RGB_LCD_TIMING0_U  timing0;
    LCDC_RGB_LCD_TIMING1_U  timing1;
    LCDC_RGB_LCD_CTRL_U     lcd_ctrl;
} LCDC_RGB_INFO_T;

typedef union _LCDC_MCU_LCD_PARAMETER0_U
{
    struct MCU_PARAMETER0_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint whpw    :4; //Write High Pulse Width
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rcss    :2; //Read CS Status
        volatile uint res     :12;//reserved
#else
        volatile uint res     :12;//reserved
        volatile uint rcss    :2; //Read CS Status
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint whpw    :4; //Write High Pulse Width
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_PARAMETER0_U;

typedef union _LCDC_MCU_LCD_PARAMETER1_U
{
    struct MCU_PARAMETER1_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint whpw    :4; //Write High Pulse Width
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rcss    :2; //Read CS Status
        volatile uint res     :12;//reserved
#else
        volatile uint res     :12;//reserved
        volatile uint rcss    :2; //Read CS Status
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint whpw    :4; //Write High Pulse Width
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_PARAMETER1_U;

typedef union _LCDC_MCU_MODESEL_U
{
    struct MCU_MODESEL_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint modesel0    :1; //ModeSel0:0: 8080;1: 6800
        volatile uint modesel1    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint res         :30;//reserved
#else
        volatile uint res         :30;//reserved
        volatile uint modesel1    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel0    :1; //ModeSel0:0: 8080;1: 6800
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_MODESEL_U;

typedef union _LCDC_MCU_RGBMODE_U
{
    struct MCU_RGBMODE_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint cs0_rgb_mode    :2; //CS0 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs1_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint res             :28;//reserved
#else
        volatile uint res             :28;//reserved
        volatile uint cs1_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs0_rgb_mode    :2; //CS0 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_RGBMODE_U;

typedef struct _LCDC_MCU_SPEC_T_tag
{
    LCDC_MCU_LCD_PARAMETER0_U parameter0;
    LCDC_MCU_LCD_PARAMETER1_U parameter1;
    LCDC_MCU_LCD_MODESEL_U    modesel;
    LCDC_MCU_LCD_RGBMODE_U    rgbmode;
} LCDC_MCU_SPEC_T;

typedef struct _LCDC_MCU_INFO_T_tag
{
    LCDC_MCU_LCD_PARAMETER0_U parameter0;
    LCDC_MCU_LCD_PARAMETER1_U parameter1;
    LCDC_MCU_LCD_MODESEL_U    modesel;
    LCDC_MCU_LCD_RGBMODE_U    rgbmode;
    uint32                    lcd_cd[2];//0:command is 0,data is 1,1:command is 1,data is 0
    uint32                    lcd_sum;  //lcd total number
} LCDC_MCU_INFO_T;

void lcdc_rgb_init (uint32 mode);
void lcdc_rgb_reset (void);
int32 lcdc_rgb_configure (LCDC_RGB_SPEC_T *prgb_lcd_spec);
int32 lcdc_rgb_int_enable (LCDC_RGB_INT_ENA_T *pInt);
int32 lcdc_rgb_get_info (LCDC_RGB_INFO_T *ptrInfo);
void lcdc_rgb_send_cmd_data (uint32 cmd,uint32 data);
int32 lcdc_rgb_transcmd (uint16 lcd_width,uint16 lcd_height);
void lcdc_rgb_isr_handle (void);
void lcdc_rgb_send_cmd (uint32 cmd);
int32 lcdc_rgb_read_lcdindex (uint32 *pData,uint32 cmd);
void lcdc_mcu_init (uint32 mode);
void lcdc_mcu_reset (uint32 delay_ms);
int32 lcdc_mcu_configure (LCDC_MCU_SPEC_T *pmcu_lcd_spec);
//void lcdc_init_blk(void);
void lcdc_mcu_int_enable (uint32 osdInt);
int32 lcdc_mcu_send_cmd_data (uint32 cmd,uint32 data,uint32 lcd_id);
int32 lcdc_mcu_send_cmd (uint32 cmd,uint32 lcd_id);
int32 lcdc_mcu_send_data (uint32 data,uint32 lcd_id);
int32 lcdc_mcu_transcmd (uint16 lcd_width, uint16 lcd_height,uint32 lcd_cs);
void lcdc_mcu_isr_handle (void);
int32 lcdc_mcu_get_info (LCDC_MCU_INFO_T *ptrInfo);
int32 lcdc_mcu_remain_datacounter (uint32 *pDataNum);
int32 lcdc_mcu_read_lcdindex (uint32 *pData,uint32 lcd_index,uint32 lcd_id);


/**------------------------------------------------------------------------------------*
 **                             Compiler Flag                                          *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif//_LCDC_COMMON_H

    //end of lcdc_common.h


