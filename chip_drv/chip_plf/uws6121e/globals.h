/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'globals'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
/// AHB Address bus size
#define NB_BITS_ADDR                            (32)
/// AXI Address bus size of DMA_AP
#define AP_NB_BITS_ADDR                         (32)
/// System AON Apb Bus Configuration
#define AON_NB_BITS_PSEL                        (7)
#define AON_NB_BITS_PADDR                       (12)
#define AON_APB_STEP                            (EXP2(AON_NB_BITS_PADDR))
#define AON_APB_ID_IFC_BASE                     (16)

// ============================================================================
// AON_APB_ID_T
// -----------------------------------------------------------------------------
/// 8910m sys aon apb module id
// =============================================================================
typedef enum
{
    AON_APB_ID_SYS_CTRL                         = 0x00000000,
    AON_APB_ID_LVDS                             = 0x00000001,
    AON_APB_ID_GSM_LPS                          = 0x00000002,
    AON_APB_ID_I2C_M2                           = 0x00000003,
    AON_APB_ID_MAILBOX                          = 0x00000004,
    AON_APB_ID_TIMER_3                          = 0x00000005,
    AON_APB_ID_KEYPAD                           = 0x00000006,
    AON_APB_ID_GPIO_1                           = 0x00000007,
    AON_APB_ID_PWM                              = 0x00000008,
    AON_APB_ID_ANALOG_REG                       = 0x00000009,
    AON_APB_ID_AON_IFC                          = 0x0000000A,
    AON_APB_ID_NB_LPS                           = 0x0000000B,
    AON_APB_ID_IOMUX                            = 0x0000000C,
    AON_APB_ID_SPINLOCK                         = 0x0000000D,
    AON_APB_ID_EFUSE                            = 0x0000000E,
/// Last of AON Normal slave
    AON_APB_ID_NORMAL_LAST                      = 0x0000000F,
    AON_APB_ID_DEBUG_UART                       = 0x00000010,
    AON_APB_ID_17_RESERVED                      = 0x00000011,
    AON_APB_ID_VAD                              = 0x00000012,
/// The debug host is placed at last PSEL127 in the IFC
    AON_APB_ID_DEBUG_HOST                       = 0x0000007F
} AON_APB_ID_T;

/// Num of System Aon Apb Slaves except Debug Host
#define AON_NB_PSEL                             (19)
/// Debug host slave id used for ifc channel.
#define DEBUG_HOST_SLAVE_ID                     (AON_APB_ID_DEBUG_HOST)
/// System Apb Bus Configuration
#define AP_NB_BITS_PSEL                         (7)
#define AP_NB_BITS_PADDR                        (12)
#define AP_APB_STEP                             (EXP2(AP_NB_BITS_PADDR))
#define AP_APB_ID_IFC_BASE                      (16)

// ============================================================================
// AP_APB_ID_T
// -----------------------------------------------------------------------------
/// 8910m sys apb module id
// =============================================================================
typedef enum
{
    AP_APB_ID_IRQH                              = 0x00000000,
    AP_APB_ID_LPDDR_PSRAM_CTRL                  = 0x00000001,
    AP_APB_ID_PAGESPY                           = 0x00000002,
    AP_APB_ID_DMC_CTRL                          = 0x00000003,
    AP_APB_ID_SYSIMEM                           = 0x00000004,
    AP_APB_ID_LZMA                              = 0x00000005,
    AP_APB_ID_GOUDA                             = 0x00000006,
    AP_APB_ID_LCDC                              = 0x00000007,
    AP_APB_ID_TIMER_1                           = 0x00000008,
    AP_APB_ID_TIMER_2                           = 0x00000009,
    AP_APB_ID_I2C_M1                            = 0x0000000A,
    AP_APB_ID_I2C_M3                            = 0x0000000B,
    AP_APB_ID_12_RESERVED                       = 0x0000000C,
    AP_APB_ID_AUD_2AD                           = 0x0000000D,
    AP_APB_ID_TIMER_4                           = 0x0000000E,
    AP_APB_ID_AP_IFC                            = 0x0000000F,
/// Last of Sys APB Normal slave
    AP_APB_ID_NORMAL_LAST                       = 0x0000000F,
    AP_APB_ID_SDMMC1                            = 0x00000010,
    AP_APB_ID_SDMMC2                            = 0x00000011,
    AP_APB_ID_SPI_1                             = 0x00000012,
    AP_APB_ID_SPI_2                             = 0x00000013,
    AP_APB_ID_SCI_1                             = 0x00000014,
    AP_APB_ID_SCI_2                             = 0x00000015,
    AP_APB_ID_ZSP_UART                          = 0x00000016,
    AP_APB_ID_UART_2                            = 0x00000017,
    AP_APB_ID_UART_3                            = 0x00000018,
    AP_APB_ID_CAMERA                            = 0x00000019,
    AP_APB1_ID_AIF                              = 0x0000001A,
    AP_APB1_ID_AIF2                             = 0x0000001B
} AP_APB_ID_T;

/// Num of System Apb Slaves
#define AP_NB_PSEL                              (28)
/// System Ahb Bus Configuration
#define SYS_NB_BITS_HADDR                       (18)
#define SYS_AHB_STEP                            (EXP2(SYS_NB_BITS_HADDR))

// ============================================================================
// SYS_AHB_ID_T
// -----------------------------------------------------------------------------
/// 8910m sys ahb module id
// =============================================================================
typedef enum
{
    SYS_AHB_ID_F8                               = 0x00000001,
    SYS_AHB_ID_USBC                             = 0x00000002,
    SYS_AHB_ID_GOUDA                            = 0x00000003,
    SYS_AHB_ID_AXIDMA                           = 0x00000004,
    SYS_AHB_ID_GEA3                             = 0x00000005,
    SYS_AHB_ID_AES                              = 0x00000006,
    SYS_AHB_ID_USB11                            = 0x00000007
} SYS_AHB_ID_T;

/// Num of System Ahb Slaves
#define NB_SYS_AHB_SLAVES                       (7)
/// Aif Apb Bus Configuration
#define AIF_NB_BITS_PSEL                        (5)
#define AIF_NB_BITS_PADDR                       (12)
#define AIF_APB_STEP                            (EXP2(AIF_NB_BITS_PADDR))

// ============================================================================
// AIF_APB_ID_T
// -----------------------------------------------------------------------------
/// 8910m aif apb module id
// =============================================================================
typedef enum
{
    AIF_APB_ID_AIF                              = 0x00000007
} AIF_APB_ID_T;

/// Num of Aif Apb Slaves
#define AIF_NB_PSEL                             (8)
/// Aif slave id used for ifc channel.
#define AIF_SLAVE_ID                            (AIF_APB_ID_AIF)
#define AP_APB1_NB_BITS_PADDR                   (AP_NB_BITS_PADDR)
#define AP_APB1_NB_BITS_PSEL                    (AP_NB_BITS_PSEL)
#define AUDIO_IFC_APB_STEP                      (EXP2(8))
#define AP_NB_DMA_REQ_WIDTH                     (5)

// ============================================================================
// AP_IFC_REQUEST_IDS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    AP_APB_DMA_ID_SDMMC1_TX                     = 0x00000000,
    AP_APB_DMA_ID_SDMMC1_RX                     = 0x00000001,
    AP_APB_DMA_ID_SDMMC2_TX                     = 0x00000002,
    AP_APB_DMA_ID_SDMMC2_RX                     = 0x00000003,
    AP_APB_DMA_ID_SPI_1_TX                      = 0x00000004,
    AP_APB_DMA_ID_SPI_1_RX                      = 0x00000005,
    AP_APB_DMA_ID_SPI_2_TX                      = 0x00000006,
    AP_APB_DMA_ID_SPI_2_RX                      = 0x00000007,
    AP_APB_DMA_ID_SCI_1_TX                      = 0x00000008,
    AP_APB_DMA_ID_SCI_1_RX                      = 0x00000009,
    AP_APB_DMA_ID_SCI_2_TX                      = 0x0000000A,
    AP_APB_DMA_ID_SCI_2_RX                      = 0x0000000B,
    AP_APB_DMA_ID_FREE_TX                       = 0x00000012,
    AP_APB_DMA_ID_CAMERA_RX                     = 0x00000013
} AP_IFC_REQUEST_IDS_T_s;//bringup_tmp

/// Num of sys ifc dma req
#define AP_NB_DMA_REQ                           (20)

// ============================================================================
// SYS_AXI_DMA_REQUEST_IDS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    SYS_AXI_DMA_ID_UART_1_RX                    = 0x00000000,
    SYS_AXI_DMA_ID_UART_1_TX                    = 0x00000001,
    SYS_AXI_DMA_ID_UART_2_RX                    = 0x00000002,
    SYS_AXI_DMA_ID_UART_2_TX                    = 0x00000003,
    SYS_AXI_DMA_ID_UART_3_RX                    = 0x00000004,
    SYS_AXI_DMA_ID_UART_3_TX                    = 0x00000005,
    SYS_AXI_DMA_ID_ZBUSMON                      = 0x00000006,
    SYS_AXI_DMA_ID_ZSP_UART                     = 0x00000007
} SYS_AXI_DMA_REQUEST_IDS_T;

#define AON_NB_DMA_REQ_WIDTH                    (5)

// ============================================================================
// AON_IFC_REQUEST_IDS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    AON_APB_DMA_ID_TX_DEBUG_UART                = 0x00000000,
    AON_APB_DMA_ID_RX_DEBUG_UART                = 0x00000001,
    AON_APB_DMA_ID_RX_VAD                       = 0x00000005
} AON_IFC_REQUEST_IDS_T;

/// Num of aon ifc dma req
#define AON_NB_DMA_REQ                          (6)

// ============================================================================
// SYS_IRQ_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if 0
typedef enum
{
/// System IRQ IDs
    SYS_IRQ_ID_PAGE_SPY                         = 0x00000000,
    SYS_IRQ_ID_IMEM                             = 0x00000001,
    SYS_IRQ_ID_TIMER_1                          = 0x00000002,
    SYS_IRQ_ID_TIMER_1_OS                       = 0x00000003,
    SYS_IRQ_ID_TIMER_2                          = 0x00000004,
    SYS_IRQ_ID_TIMER_2_OS                       = 0x00000005,
    SYS_IRQ_ID_TIMER_4                          = 0x00000006,
    SYS_IRQ_ID_TIMER_4_OS                       = 0x00000007,
    SYS_IRQ_ID_I2C_M1                           = 0x00000008,
    SYS_IRQ_ID_AIF_APB_0                        = 0x00000009,
    SYS_IRQ_ID_AIF_APB_1                        = 0x0000000A,
    SYS_IRQ_ID_AIF_APB_2                        = 0x0000000B,
    SYS_IRQ_ID_AIF_APB_3                        = 0x0000000C,
    SYS_IRQ_ID_AUD_2AD                          = 0x0000000D,
    SYS_IRQ_ID_SDMMC1                           = 0x0000000E,
    SYS_IRQ_ID_SDMMC2                           = 0x0000000F,
    SYS_IRQ_ID_SPI_1                            = 0x00000010,
    SYS_IRQ_ID_SPI_2                            = 0x00000011,
    SYS_IRQ_ID_ZSP_UART                         = 0x00000012,
    SYS_IRQ_ID_UART_2                           = 0x00000013,
    SYS_IRQ_ID_UART_3                           = 0x00000014,
    SYS_IRQ_ID_CAMERA                           = 0x00000015,
    SYS_IRQ_ID_LZMA                             = 0x00000016,
    SYS_IRQ_ID_GOUDA                            = 0x00000017,
    SYS_IRQ_ID_F8                               = 0x00000018,
    SYS_IRQ_ID_USBC                             = 0x00000019,
    SYS_IRQ_ID_USB11                            = 0x0000001A,
    SYS_IRQ_ID_AXIDMA                           = 0x0000001B,
    SYS_IRQ_ID_AXIDMA_1_SECURITY                = 0x0000001C,
    SYS_IRQ_ID_AXIDMA_1_UNSECURITY              = 0x0000001D,
    SYS_IRQ_ID_PMU_APCPU                        = 0x0000001E,
    SYS_IRQ_ID_31_RESERVED                      = 0x0000001F,
    SYS_IRQ_ID_LCD                              = 0x00000020,
    SYS_IRQ_ID_SPIFLASH                         = 0x00000021,
    SYS_IRQ_ID_SPIFLASH1                        = 0x00000022,
    SYS_IRQ_ID_GPRS_0                           = 0x00000023,
    SYS_IRQ_ID_GPRS_1                           = 0x00000024,
    SYS_IRQ_ID_DMC                              = 0x00000025,
    SYS_IRQ_ID_AES                              = 0x00000026,
    SYS_IRQ_ID_CTI_APCPU                        = 0x00000027,
    SYS_IRQ_ID_AP_TZ_SLV                        = 0x00000028,
    SYS_IRQ_ID_AP_TZ_MEM                        = 0x00000029,
    SYS_IRQ_ID_I2C_M3                           = 0x0000002A,
    SYS_IRQ_ID_GSM_LPS                          = 0x0000002B,
    SYS_IRQ_ID_I2C_M2                           = 0x0000002C,
    SYS_IRQ_ID_TIMER_3                          = 0x0000002D,
    SYS_IRQ_ID_TIMER_3_OS                       = 0x0000002E,
    SYS_IRQ_ID_KEYPAD                           = 0x0000002F,
    SYS_IRQ_ID_GPIO_1                           = 0x00000030,
    SYS_IRQ_ID_DEBUG_UART                       = 0x00000031,
    SYS_IRQ_ID_SCI_1                            = 0x00000032,
    SYS_IRQ_ID_SCI_2                            = 0x00000033,
    SYS_IRQ_ID_ADI                              = 0x00000034,
    SYS_IRQ_ID_UART_1                           = 0x00000035,
    SYS_IRQ_ID_VAD                              = 0x00000036,
    SYS_IRQ_ID_VAD_PULSE                        = 0x00000037,
    SYS_IRQ_ID_AON_TZ                           = 0x00000038,
    SYS_IRQ_ID_NB_LPS                           = 0x00000039,
    SYS_IRQ_ID_CP_IDLE_H                        = 0x0000003A,
    SYS_IRQ_ID_CP_IDLE_2_H                      = 0x0000003B,
    SYS_IRQ_ID_MAILBOX_ARM_AP                   = 0x0000003C,
    SYS_IRQ_ID_LTEM1_FRAME                      = 0x0000003D,
    SYS_IRQ_ID_LTEM2_FRAME                      = 0x0000003E,
    SYS_IRQ_ID_RFSPI_CONFLICT                   = 0x0000003F,
    SYS_IRQ_ID_CP_WD_RESET                      = 0x00000040,
    SYS_IRQ_ID_GSM_FRAME                        = 0x00000041,
    SYS_IRQ_ID_PWRCTRL_TIMEOUT                  = 0x00000042,
    SYS_IRQ_ID_NB_FRAME                         = 0x00000043,
    SYS_IRQ_ID_ZSP_AXIDMA                       = 0x00000044,
    SYS_IRQ_ID_ZSP_BUSMON                       = 0x00000045,
    SYS_IRQ_ID_ZSP_WD                           = 0x00000046,
    SYS_IRQ_ID_71_RESERVED                      = 0x00000047,
    SYS_IRQ_ID_GGE_FINT                         = 0x00000048,
    SYS_IRQ_ID_GGE_TCU_0                        = 0x00000049,
    SYS_IRQ_ID_GGE_TCU_1                        = 0x0000004A,
    SYS_IRQ_ID_NB_FINT                          = 0x0000004B,
    SYS_IRQ_ID_NB_TCU_0                         = 0x0000004C,
    SYS_IRQ_ID_NB_TCU_1                         = 0x0000004D,
    SYS_IRQ_ID_NB_TCU_SYNC                      = 0x0000004E,
    SYS_IRQ_ID_GGE_COMP_INT                     = 0x0000004F,
    SYS_IRQ_ID_GGE_NB_RX_DSP                    = 0x00000050,
    SYS_IRQ_ID_GGE_NB_RX_MCU                    = 0x00000051,
    SYS_IRQ_ID_GGE_NB_TX_DSP                    = 0x00000052,
    SYS_IRQ_ID_GGE_NB_ACC_DSP                   = 0x00000053,
    SYS_IRQ_ID_84_RESERVED                      = 0x00000054,
    SYS_IRQ_ID_WCN_WLAN                         = 0x00000055,
    SYS_IRQ_ID_WCN_SOFT                         = 0x00000056,
    SYS_IRQ_ID_WCN_WDT_RST                      = 0x00000057,
    SYS_IRQ_ID_WCN_AWAKE                        = 0x00000058,
    SYS_IRQ_ID_RF_WDG_RST                       = 0x00000059,
    SYS_IRQ_ID_LVDS                             = 0x0000005A
} SYS_IRQ_ID_T;

/// Num of System IRQS
#define NB_SYS_IRQ                              (91)
#endif
typedef enum
{
/// System IRQ IDs
	SYS_IRQ_ID_AP_IMEM						=0x00000000,
	SYS_IRQ_ID_SPIFLASH1						=0x00000001,
	SYS_IRQ_ID_SPIFLASH2						=0x00000002,
	SYS_IRQ_ID_GOUDA							=0x00000003,
	SYS_IRQ_ID_AP_AXIDMA						=0x00000004,
	SYS_IRQ_ID_AP_AXIDMA_SECURITY			=0x00000005,
	SYS_IRQ_ID_AP_AXIDMA_UNSECURITY		=0x00000006,
	SYS_IRQ_ID_USBC							=0x00000007,
	SYS_IRQ_ID_MED							=0x00000008,
	SYS_IRQ_ID_CE_PUB							=0x00000009,
	SYS_IRQ_ID_CE_SEC							=0x0000000A,
	SYS_IRQ_ID_UART4							=0x0000000B,
	SYS_IRQ_ID_UART5							=0x0000000C,
	SYS_IRQ_ID_UART6							=0x0000000D,
	SYS_IRQ_ID_SPI1							=0x0000000E,
	SYS_IRQ_ID_SDMMC							=0x0000000F,
	SYS_IRQ_ID_CAMERA						=0x00000010,
	SYS_IRQ_ID_LZMA							=0x00000011,
	SYS_IRQ_ID_AP_BUSMON						=0x00000012,
	SYS_IRQ_ID_EMMC							=0x00000013,
	SYS_IRQ_ID_TIMER1							=0x00000014,
	SYS_IRQ_ID_TIMER1_OS						=0x00000015,
	SYS_IRQ_ID_TIMER2							=0x00000016,
	SYS_IRQ_ID_TIMER2_OS						=0x00000017,
	SYS_IRQ_ID_I2C1							=0x00000018,
	SYS_IRQ_ID_I2C2							=0x00000019,
	SYS_IRQ_ID_GPT3_0							=0x0000001A,
	SYS_IRQ_ID_GPT3_1							=0x0000001B,
	SYS_IRQ_ID_GPT3_2							=0x0000001C,
	SYS_IRQ_ID_GPT3_3							=0x0000001D,
	SYS_IRQ_ID_GPT3_4							=0x0000001E,
	SYS_IRQ_ID_GPT3_5							=0x0000001F,
	SYS_IRQ_ID_GPT3_6							=0x00000020,
	SYS_IRQ_ID_PMU_APCPU						=0x00000021,
	SYS_IRQ_ID_CLK_MNT32K					=0x00000022,
	SYS_IRQ_ID_CLK_MNT26M					=0x00000023,
	SYS_IRQ_ID_SFW_AP_IFC					=0x00000024,
	SYS_IRQ_ID_SFW_AP_AHB					=0x00000025,
	SYS_IRQ_ID_MFW_AP_IMEM					=0x00000026,
	SYS_IRQ_ID_MFW_SPIFLASH1				=0x00000027,
	SYS_IRQ_ID_MFW_SPIFLASH2				=0x00000028,
	SYS_IRQ_ID_AP_WD_RST						=0x00000029,
	SYS_IRQ_ID_AP_TZPC_RST					=0x0000002A,
	SYS_IRQ_ID_ADIMST							=0x0000002B,
	SYS_IRQ_ID_IDLE_LPS						=0x0000002C,
	SYS_IRQ_ID_GPIO1							=0x0000002D,
	SYS_IRQ_ID_KEYPAD							=0x0000002E,
	SYS_IRQ_ID_GPT1							=0x0000002F,
	SYS_IRQ_ID_GPIO2							=0x00000030,
	SYS_IRQ_ID_I2C3							=0x00000031,
	SYS_IRQ_ID_MAILBOX_ARM_AP				=0x00000032,
	SYS_IRQ_ID_AUD_2AD						=0x00000033,
	SYS_IRQ_ID_GPT2_0							=0x00000034,
	SYS_IRQ_ID_GPT2_1							=0x00000035,
	SYS_IRQ_ID_GPT2_2							=0x00000036,
	SYS_IRQ_ID_UART1							=0x00000037,
	SYS_IRQ_ID_UART2							=0x00000038,
	SYS_IRQ_ID_UART3							=0x00000039,
	SYS_IRQ_ID_SPI2							=0x0000003A,
	SYS_IRQ_ID_DEBUG_UART					=0x0000003B,
	SYS_IRQ_ID_DEBUG_HOST					=0x0000003C,
	SYS_IRQ_ID_AIF_IFC0						=0x0000003D,
	SYS_IRQ_ID_AIF_IFC1						=0x0000003E,
	SYS_IRQ_ID_SFW_LPS_IFC					=0x0000003F,
	SYS_IRQ_ID_SFW_AON_IFC					=0x00000040,
	SYS_IRQ_ID_SFW_AON_AHB					=0x00000041,
	SYS_IRQ_ID_MFW_AON_IMEM_INT			=0x00000042,
	SYS_IRQ_ID_MSTFLT_AON_CP				=0x00000043,
	SYS_IRQ_ID_MSTFLT_AON_RF					=0x00000044,
	SYS_IRQ_ID_CP_IDLE_H						=0x00000045,
	SYS_IRQ_ID_CP_IDLE2_H					=0x00000046,
	SYS_IRQ_ID_LTEM1_FRAME					=0x00000047,
	SYS_IRQ_ID_LTEM2_FRAME					=0x00000048,
	SYS_IRQ_ID_LTEM3_FRAME					=0x00000049,
	SYS_IRQ_ID_RC26M_CALIB					=0x0000004A,
	SYS_IRQ_ID_PWRCTRL						=0x0000004B,
	SYS_IRQ_ID_PMIC							=0x0000004C,
	SYS_IRQ_ID_MFW_AON_IMEM_RST			=0x0000004D,
	SYS_IRQ_ID_RTC_TIMER						=0x0000004E,
	SYS_IRQ_ID_CP_IRQ							=0x0000004F,
	SYS_IRQ_ID_CP_FIQ							=0x00000050,
	SYS_IRQ_ID_CP_BUSMON						=0x00000051,
	SYS_IRQ_ID_CP_WD_RST						=0x00000052,
	SYS_IRQ_ID_FREQ_BIAS						=0x00000053,
	SYS_IRQ_ID_CP_WIFI						=0x00000054,
	SYS_IRQ_ID_PPS							=0x00000055,
	SYS_IRQ_ID_RFT_TOP						=0x00000056,
	SYS_IRQ_ID_GNSS_BB						=0x00000057,
	SYS_IRQ_ID_GNSS_BB_EXCEPT				=0x00000058,
	SYS_IRQ_ID_GNSS_NOMASK_NEXT_MEASINT	=0x00000059,
	SYS_IRQ_ID_RF_WD_RST						=0x0000005A,
	SYS_IRQ_ID_DMC							=0x0000005B,
	SYS_IRQ_ID_PAGE_SPY						=0x0000005C,
	SYS_IRQ_ID_MFW_PUB_INT					=0x0000005D,
	SYS_IRQ_ID_MFW_PUB_RST					=0x0000005E,
	SYS_IRQ_ID_TIMER5							=0x0000005F,
	SYS_IRQ_ID_TIMER5_OS						=0x00000060,

	SYS_IRQ_ID_SCI								=0x00000061,
	SYS_IRQ_ID_LCD							=0x00000062,
	SYS_IRQ_ID_WCN_AWAKE					=0x00000063,
	SYS_IRQ_ID_WCN_WLAN						=0x00000064,

} SYS_IRQ_ID_T;
#define NB_SYS_IRQ		(97)
#define NB_BITS_ADDR		(32)
//#define NB_SYS_IRQ                              (91)
// ============================================================================
// CP_IRQ_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// IRQ IDs For CP CPU
    CP_IRQ_ID_PAGE_SPY                          = 0x00000000,
    CP_IRQ_ID_TIMER_1                           = 0x00000001,
    CP_IRQ_ID_TIMER_1_OS                        = 0x00000002,
    CP_IRQ_ID_UART_2                            = 0x00000003,
    CP_IRQ_ID_UART_3                            = 0x00000004,
    CP_IRQ_ID_F8                                = 0x00000005,
    CP_IRQ_ID_AXIDMA                            = 0x00000006,
    CP_IRQ_ID_ZSP_UART                          = 0x00000007,
    CP_IRQ_ID_CTI_CPCPU                         = 0x00000008,
    CP_IRQ_ID_PMU_CPCPU                         = 0x00000009,
    CP_IRQ_ID_UART_1                            = 0x0000000A,
    CP_IRQ_ID_LPS_GSM                           = 0x0000000B,
    CP_IRQ_ID_LPS_NB                            = 0x0000000C,
    CP_IRQ_ID_SCI_1                             = 0x0000000D,
    CP_IRQ_ID_SCI_2                             = 0x0000000E,
    CP_IRQ_ID_VAD                               = 0x0000000F,
    CP_IRQ_ID_VAD_PULSE                         = 0x00000010,
    CP_IRQ_ID_CP_IDLE_H                         = 0x00000011,
    CP_IRQ_ID_CP_IDLE_2_H                       = 0x00000012,
    CP_IRQ_ID_MAILBOX_ARM_CP                    = 0x00000013,
    CP_IRQ_ID_LTEM1_FRAME                       = 0x00000014,
    CP_IRQ_ID_LTEM2_FRAME                       = 0x00000015,
    CP_IRQ_ID_RFSPI_CONFLICT                    = 0x00000016,
    CP_IRQ_ID_GSM_FRAME                         = 0x00000017,
    CP_IRQ_ID_NB_FRAME                          = 0x00000018,
    CP_IRQ_ID_PWRCTRL_TIMEOUT                   = 0x00000019,
    CP_IRQ_ID_LTE_COEFF                         = 0x0000001A,
    CP_IRQ_ID_LTE_CSIRS                         = 0x0000001B,
    CP_IRQ_ID_LTE_DLFFT                         = 0x0000001C,
    CP_IRQ_ID_LTE_DLFFT_ERR                     = 0x0000001D,
    CP_IRQ_ID_LTE_IDDET                         = 0x0000001E,
    CP_IRQ_ID_LTE_LDTC1_CTRL                    = 0x0000001F,
    CP_IRQ_ID_LTE_LDTC1_DATA                    = 0x00000020,
    CP_IRQ_ID_LTE_LDTC1_VIT                     = 0x00000021,
    CP_IRQ_ID_LTE_LDTC                          = 0x00000022,
    CP_IRQ_ID_LTE_MEASPWR                       = 0x00000023,
    CP_IRQ_ID_LTE_PUSCH                         = 0x00000024,
    CP_IRQ_ID_LTE_RX_TRACE                      = 0x00000025,
    CP_IRQ_ID_LTE_TXRX                          = 0x00000026,
    CP_IRQ_ID_LTE_TX_TRACE                      = 0x00000027,
    CP_IRQ_ID_LTE_ULDFT                         = 0x00000028,
    CP_IRQ_ID_LTE_ULDFT_ERR                     = 0x00000029,
    CP_IRQ_ID_LTE_OTDOA                         = 0x0000002A,
    CP_IRQ_ID_LTE_PBMEAS                        = 0x0000002B,
    CP_IRQ_ID_LTE_RXCAPT                        = 0x0000002C,
    CP_IRQ_ID_LTE_ULPCDCI                       = 0x0000002D,
    CP_IRQ_ID_LTE_CORR                          = 0x0000002E,
    CP_IRQ_ID_ZSP_AXIDMA                        = 0x0000002F,
    CP_IRQ_ID_ZSP_BUSMON                        = 0x00000030,
    CP_IRQ_ID_ZSP_WD                            = 0x00000031,
    CP_IRQ_ID_ZSP_AUD_DFT                       = 0x00000032,
    CP_IRQ_ID_GGE_FINT                          = 0x00000033,
    CP_IRQ_ID_GGE_TCU_0                         = 0x00000034,
    CP_IRQ_ID_GGE_TCU_1                         = 0x00000035,
    CP_IRQ_ID_NB_FINT                           = 0x00000036,
    CP_IRQ_ID_NB_TCU_0                          = 0x00000037,
    CP_IRQ_ID_NB_TCU_1                          = 0x00000038,
    CP_IRQ_ID_GGE_NB_RX_DSP                     = 0x00000039,
    CP_IRQ_ID_GGE_NB_RX_MCU                     = 0x0000003A,
    CP_IRQ_ID_GGE_NB_TX_DSP                     = 0x0000003B,
    CP_IRQ_ID_GGE_NB_ACC_DSP                    = 0x0000003C,
    CP_IRQ_ID_NB_TCU_SYNC                       = 0x0000003D,
    CP_IRQ_ID_TIMER_3                           = 0x0000003E,
    CP_IRQ_ID_OTHERS                            = 0x0000003F
} CP_IRQ_ID_T;

/// Num of IRQS For CP CPU
#define NB_CP_IRQ                               (64)

// ============================================================================
// CP_OTHER_IRQ_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Other IRQ IDs For CP CPU
    CP_IRQ_ID_IMEM                              = 0x00000000,
    CP_IRQ_ID_TIMER_2                           = 0x00000001,
    CP_IRQ_ID_TIMER_2_OS                        = 0x00000002,
    CP_IRQ_ID_TIMER_4                           = 0x00000003,
    CP_IRQ_ID_TIMER_4_OS                        = 0x00000004,
    CP_IRQ_ID_I2C_M1                            = 0x00000005,
    CP_IRQ_ID_AIF_APB_0                         = 0x00000006,
    CP_IRQ_ID_AIF_APB_1                         = 0x00000007,
    CP_IRQ_ID_AIF_APB_2                         = 0x00000008,
    CP_IRQ_ID_AIF_APB_3                         = 0x00000009,
    CP_IRQ_ID_AUD_2AD                           = 0x0000000A,
    CP_IRQ_ID_SDMMC1                            = 0x0000000B,
    CP_IRQ_ID_SDMMC2                            = 0x0000000C,
    CP_IRQ_ID_SPI_1                             = 0x0000000D,
    CP_IRQ_ID_SPI_2                             = 0x0000000E,
    CP_IRQ_ID_CAMERA                            = 0x0000000F,
    CP_IRQ_ID_LZMA                              = 0x00000010,
    CP_IRQ_ID_GOUDA                             = 0x00000011,
    CP_IRQ_ID_USBC                              = 0x00000012,
    CP_IRQ_ID_USB11                             = 0x00000013,
    CP_IRQ_ID_AXIDMA_1_SECURITY                 = 0x00000014,
    CP_IRQ_ID_AXIDMA_1_UNSECURITY               = 0x00000015,
    CP_IRQ_ID_LCD                               = 0x00000016,
    CP_IRQ_ID_SPIFLASH                          = 0x00000017,
    CP_IRQ_ID_SPIFLASH1                         = 0x00000018,
    CP_IRQ_ID_GPRS_0                            = 0x00000019,
    CP_IRQ_ID_GPRS_1                            = 0x0000001A,
    CP_IRQ_ID_DMC                               = 0x0000001B,
    CP_IRQ_ID_AES                               = 0x0000001C,
    CP_IRQ_ID_AP_TZ_SLV                         = 0x0000001D,
    CP_IRQ_ID_AP_TZ_MEM                         = 0x0000001E,
    CP_IRQ_ID_I2C_M3                            = 0x0000001F,
    CP_IRQ_ID_GGE_COMP_INT                      = 0x00000020,
    CP_IRQ_ID_I2C_M2                            = 0x00000021,
    CP_IRQ_ID_TIMER_3_OS                        = 0x00000022,
    CP_IRQ_ID_KEYPAD                            = 0x00000023,
    CP_IRQ_ID_GPIO_1                            = 0x00000024,
    CP_IRQ_ID_DEBUG_UART                        = 0x00000025,
    CP_IRQ_ID_ADI                               = 0x00000026,
    CP_IRQ_ID_AON_TZ                            = 0x00000027,
    CP_IRQ_ID_CP_WD_RESET                       = 0x00000028
} CP_OTHER_IRQ_ID_T;

/// Num of Other IRQS For CP CPU
#define NB_CP_OTHER_IRQ                         (41)
/// Below is for compatibility to inherited design and for rtl compiling pass
#define RF_SPI_SLAVE_ID                         (22)
#define SYS_NB_MASTERS                          (9)
#define BB_NB_MASTERS                           (6)
#define BB_NB_SLAVES                            (NB_SYS_AHB_SLAVES)
#define BB_NB_BITS_PSEL                         (AIF_NB_BITS_PSEL)
#define BB_NB_BITS_PADDR                        (AIF_NB_BITS_PADDR)
#define BB_NB_PSEL                              (AIF_NB_PSEL)
#define RF_SLAVE_ID                             (6)
#define BB_SYS_BITS_PADDR                       (12)
#define BB_SYS_NB_BITS_PSEL                     (4)
#define BB_SYS_STEP                             (EXP2(BB_SYS_BITS_PADDR))

// ============================================================================
// BB_SYSCTRL_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    BB_SYSCTRL_ID_SYSREG                        = 0x00000000,
    BB_SYSCTRL_ID_CLKRST                        = 0x00000001,
    BB_SYSCTRL_ID_2_RESERVED                    = 0x00000002,
    BB_SYSCTRL_ID_MONITOR                       = 0x00000003,
    BB_SYSCTRL_ID_CP_WD_TIMER                   = 0x00000004,
    BB_SYSCTRL_ID_IDLE                          = 0x00000010,
    BB_SYSCTRL_ID_UART1                         = 0x00000018,
    BB_SYSCTRL_ID_PWRCTRL                       = 0x00000020
} BB_SYSCTRL_ID_T;

#define CP_ZSP_SYS_PADDR                        (12)
#define CP_ZSP_NB_BITS_PSEL                     (12)
#define CP_ZSP_SYS_STEP                         (EXP2(CP_ZSP_SYS_PADDR))

// ============================================================================
// CP_ZSP_SYS_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    CP_ZSP_SYS_ID_ZSP_AXIDMA                    = 0x00000000,
    CP_ZSP_SYS_ID_ZSP_AUD_DFT                   = 0x00001000,
    CP_ZSP_SYS_ID_ZSP_WD                        = 0x00001001,
    CP_ZSP_SYS_ID_ZSP_IRQH                      = 0x00001002,
    CP_ZSP_SYS_ID_BUSMON                        = 0x00001003
} CP_ZSP_SYS_ID_T;

#define CP_LTE_SYS_PADDR                        (20)
#define CP_LTE_NB_BITS_PSEL                     (4)
#define CP_LTE_SYS_STEP                         (EXP2(CP_LTE_SYS_PADDR))

// ============================================================================
// CP_LTE_SYS_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    CP_LTE_SYS_ID_TXRX                          = 0x00000000,
    CP_LTE_SYS_ID_RFAD                          = 0x00000001,
    CP_LTE_SYS_ID_COEFF                         = 0x00000002,
    CP_LTE_SYS_ID_LDTC                          = 0x00000003,
    CP_LTE_SYS_ID_OTDOA                         = 0x00000004,
    CP_LTE_SYS_ID_MEASPWR                       = 0x00000005,
    CP_LTE_SYS_ID_IDDET                         = 0x00000006,
    CP_LTE_SYS_ID_ULDFT                         = 0x00000007,
    CP_LTE_SYS_ID_PUSCH                         = 0x00000008,
    CP_LTE_SYS_ID_ULPCDCI                       = 0x00000009,
    CP_LTE_SYS_ID_DLFFT                         = 0x0000000A,
    CP_LTE_SYS_ID_CSIRS                         = 0x0000000B,
    CP_LTE_SYS_ID_LDTC1                         = 0x00000010,
    CP_LTE_SYS_ID_RXCAPT                        = 0x00000020
} CP_LTE_SYS_ID_T;

/// GGE System Apb Bus Configuration
#define GGE_SYS_NB_BITS_PSEL                    (5)
#define GGE_SYS_NB_BITS_PADDR                   (12)
#define GGE_SYS_APB_STEP                        (EXP2(GGE_SYS_NB_BITS_PADDR))

// ============================================================================
// GGE_SYS_APB_ID_T
// -----------------------------------------------------------------------------
/// 8910m gge sys apb module id
// =============================================================================
typedef enum
{
    GGE_SYS_APB_ID_0_RSV                        = 0x00000000,
    GGE_SYS_APB_ID_TCU_GSM                      = 0x00000001,
    GGE_SYS_APB_ID_TCU_NB                       = 0x00000002,
    GGE_SYS_APB_ID_BB_DMA                       = 0x00000003,
    GGE_SYS_APB_ID_4_RSV                        = 0x00000004,
    GGE_SYS_APB_ID_WDT                          = 0x00000005,
    GGE_SYS_APB_ID_GGE_IFC                      = 0x00000006,
    GGE_SYS_APB_ID_7_RSV                        = 0x00000007,
    GGE_SYS_APB_ID_8_RSV                        = 0x00000008,
    GGE_SYS_APB_ID_9_RSV                        = 0x00000009,
    GGE_SYS_APB_ID_10_RSV                       = 0x0000000A,
    GGE_SYS_APB_ID_11_RSV                       = 0x0000000B,
    GGE_SYS_APB_ID_12_RSV                       = 0x0000000C,
    GGE_SYS_APB_ID_13_RSV                       = 0x0000000D,
    GGE_SYS_APB_ID_RFSPI_GSM                    = 0x0000000E,
    GGE_SYS_APB_ID_RFSPI_NB                     = 0x0000000F
} GGE_SYS_APB_ID_T;

/// Num of GGE System Apb Slaves
#define GGE_SYS_APB_NB_PSEL                     (16)
#define GGE_SYS_NB_DMA_REQ_WIDTH                (5)
/// Num of gge ifc dma req
#define GGE_SYS_NB_DMA_REQ                      (0)
/// GGE System Apb Bus Configuration
#define GGE_BB_NB_BITS_PSEL                     (5)
#define GGE_BB_NB_BITS_PADDR                    (12)
#define GGE_BB_APB_STEP                         (EXP2(GGE_BB_NB_BITS_PADDR))

// ============================================================================
// GGE_BB_APB_ID_T
// -----------------------------------------------------------------------------
/// 8910m gge bb apb module id
// =============================================================================
typedef enum
{
    GGE_BB_APB_ID_XCOR                          = 0x00000000,
    GGE_BB_APB_ID_CORDIC                        = 0x00000001,
    GGE_BB_APB_ID_ITLV                          = 0x00000002,
    GGE_BB_APB_ID_VITAC                         = 0x00000003,
    GGE_BB_APB_ID_EXCOR                         = 0x00000004,
    GGE_BB_APB_ID_CHOLK                         = 0x00000005,
    GGE_BB_APB_ID_CIPHER                        = 0x00000006,
    GGE_BB_APB_ID_EVITAC                        = 0x00000007,
    GGE_BB_APB_ID_CP2                           = 0x00000008,
    GGE_BB_APB_ID_BCPU_DBG                      = 0x00000009,
    GGE_BB_APB_ID_BCPU_CORE                     = 0x0000000A,
    GGE_BB_APB_ID_ROM                           = 0x0000000B,
    GGE_BB_APB_ID_RF_IF                         = 0x0000000C,
    GGE_BB_APB_ID_IRQ                           = 0x0000000D,
    GGE_BB_APB_ID_SYSCTRL                       = 0x0000000E,
    GGE_BB_APB_ID_A53                           = 0x0000000F,
    GGE_BB_APB_ID_NB_CTRL                       = 0x00000010,
    GGE_BB_APB_ID_NB_COMMON                     = 0x00000011,
    GGE_BB_APB_ID_NB_INTC                       = 0x00000012,
    GGE_BB_APB_ID_NB_CELL_SEARCH                = 0x00000013,
    GGE_BB_APB_ID_NB_FFT_RSRP                   = 0x00000014,
    GGE_BB_APB_ID_NB_VITERBI                    = 0x00000015,
    GGE_BB_APB_ID_NB_MEAS                       = 0x00000016,
    GGE_BB_APB_ID_NB_DS_BSEL                    = 0x00000017,
    GGE_BB_APB_ID_NB_TX_PUSCH_ENC               = 0x00000018,
    GGE_BB_APB_ID_NB_TX_CHSC                    = 0x00000019,
    GGE_BB_APB_ID_NB_TX_FRONTEND                = 0x0000001A
} GGE_BB_APB_ID_T;

/// Num of GGE Baseband Apb Slaves
#define GGE_BB_APB_NB_PSEL                      (27)

// ============================================================================
// GGE_BB_IRQ_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// GGE IRQ IDs
    GGE_BB_IRQ_ID_TCU0_GSM                      = 0x00000000,
    GGE_BB_IRQ_ID_TCU1_GSM                      = 0x00000001,
    GGE_BB_IRQ_ID_FRAME_GSM                     = 0x00000002,
    GGE_BB_IRQ_ID_TCU0_NB                       = 0x00000003,
    GGE_BB_IRQ_ID_TCU1_NB                       = 0x00000004,
    GGE_BB_IRQ_ID_FRAME_NB                      = 0x00000005,
    GGE_BB_IRQ_ID_RFSPI_CONFLICT                = 0x00000006,
    GGE_BB_IRQ_ID_A53                           = 0x00000007,
    GGE_BB_IRQ_ID_RFIF_DBG_2G                   = 0x00000008,
    GGE_BB_IRQ_ID_RFIF_DBG_NB                   = 0x00000009,
    GGE_BB_IRQ_ID_RFIF_RX                       = 0x0000000A,
    GGE_BB_IRQ_ID_RFIF_TX                       = 0x0000000B,
    GGE_BB_IRQ_ID_EVITAC                        = 0x0000000C,
    GGE_BB_IRQ_ID_EXCOR                         = 0x0000000D,
    GGE_BB_IRQ_ID_ITLV                          = 0x0000000E,
    GGE_BB_IRQ_ID_VITAC                         = 0x0000000F,
    GGE_BB_IRQ_ID_XCOR                          = 0x00000010,
    GGE_BB_IRQ_ID_CHOLK                         = 0x00000011,
    GGE_BB_IRQ_ID_RF_SPI_GSM                    = 0x00000012,
    GGE_BB_IRQ_ID_MAILBOX                       = 0x00000013,
    GGE_BB_IRQ_ID_TCU_SYNC                      = 0x00000014,
    GGE_BB_IRQ_ID_NB_RX                         = 0x00000015,
    GGE_BB_IRQ_ID_NB_TX                         = 0x00000016,
    GGE_BB_IRQ_ID_NB_ACC                        = 0x00000017,
    GGE_BB_IRQ_ID_AIF0                          = 0x00000018,
    GGE_BB_IRQ_ID_AIF1                          = 0x00000019,
    GGE_BB_IRQ_ID_AIF2                          = 0x0000001A,
    GGE_BB_IRQ_ID_AIF3                          = 0x0000001B,
    GGE_BB_IRQ_ID_RF_SPI_NB                     = 0x0000001C,
    GGE_BB_IRQ_ID_BB_DMA                        = 0x0000001D
} GGE_BB_IRQ_ID_T;

/// Number of GGE Pulse IRQ
#define NB_GGE_BB_IRQ_PULSE                     (8)
/// Num of Gge IRQS
#define NB_GGE_BB_IRQ                           (30)
/// Number of GGE BB Level IRQ
#define NB_GGE_BB_IRQ_LEVEL                     (NB_GGE_BB_IRQ - NB_GGE_BB_IRQ_PULSE)
#define BB_SYMBOL_SIZE                          (13)
#define VITAC_MULT_SIZE                         (14)
#define BB_SRAM_ADDR_WIDTH                      (13)
/// System RF Apb Bus Configuration
#define RF_NB_BITS_PADDR                        (12)
#define RF_APB_STEP                             (EXP2(RF_NB_BITS_PADDR))

// ============================================================================
// RF_APB_ID_T
// -----------------------------------------------------------------------------
/// 8910m rf apb module id
// =============================================================================
typedef enum
{
    RF_APB_ID_CTRL_1                            = 0x00000000,
    RF_APB_ID_CTRL_2                            = 0x00000001,
    RF_APB_ID_DFE                               = 0x00000002,
    RF_APB_ID_ET                                = 0x00000003,
    RF_APB_ID_RTC                               = 0x00000004,
    RF_APB_ID_SYS_CTRL                          = 0x00000005,
    RF_APB_ID_TIMER_1                           = 0x00000006,
    RF_APB_ID_TIMER_2                           = 0x00000007,
    RF_APB_ID_TIMER_3                           = 0x00000008,
    RF_APB_ID_UART                              = 0x00000009,
    RF_APB_ID_WATCHDOG                          = 0x0000000A,
    RF_APB_ID_TSEN_ADC                          = 0x0000000B,
    RF_APB_ID_RFFE                              = 0x0000000C
} RF_APB_ID_T;

/// Num of RF Apb Slaves
#define RF_NB_PSEL                              (13)
/// WCN System Apb Bus Configuration
#define WCN_SYS_NB_BITS_PSEL                    (4)
#define WCN_SYS_NB_BITS_PADDR                   (12)
#define WCN_SYS_APB_STEP                        (EXP2(WCN_SYS_NB_BITS_PADDR))

// ============================================================================
// WCN_SYS_APB_ID_T
// -----------------------------------------------------------------------------
/// 8910m wcn sys apb module id
// =============================================================================
typedef enum
{
    WCN_SYS_APB_ID_SYS_CTRL                     = 0x00000000,
    WCN_SYS_APB_ID_DBM                          = 0x00000001,
    WCN_SYS_APB_ID_SYS_IFC                      = 0x00000002,
    WCN_SYS_APB_ID_BT_CORE                      = 0x00000003,
    WCN_SYS_APB_ID_UART                         = 0x00000004,
    WCN_SYS_APB_ID_RF_IF                        = 0x00000005,
    WCN_SYS_APB_ID_MODEM                        = 0x00000006,
    WCN_SYS_APB_ID_WLAN                         = 0x00000007,
    WCN_SYS_APB_ID_WDT                          = 0x00000008,
    WCN_SYS_APB_ID_TRAP                         = 0x00000009,
    WCN_SYS_APB_ID_SYSTICK                      = 0x0000000A,
    WCN_SYS_APB_ID_COMREGS_WCN                  = 0x0000000B,
    WCN_SYS_APB_ID_COMREGS_AP                   = 0x0000000C
} WCN_SYS_APB_ID_T;

/// Num of WCN System Apb Slaves
#define WCN_SYS_APB_NB_PSEL                     (13)

// =============================================================================
//  TYPES
// =============================================================================



#endif

