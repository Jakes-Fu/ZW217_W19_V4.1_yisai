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


#ifndef _SYS_CTRL_H_
#define _SYS_CTRL_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'sys_ctrl'."
#endif


#include "globals.h"
#include "uix8910_module_ver_extern.h"

// =============================================================================
//  MACROS
// =============================================================================

// ============================================================================
// CPU_ID_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// CPU IDs
    A5CPU                                       = 0x00000000
} CPU_ID_T;


// ============================================================================
// SYS_AXI_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Sys Axi Clks IDs
    SYS_AXI_CLK_ID_IMEM                         = 0x00000000,
    SYS_AXI_CLK_ID_LZMA                         = 0x00000001,
    SYS_AXI_CLK_ID_REV0                         = 0x00000002,
    SYS_AXI_CLK_ID_REV1                         = 0x00000003,
    SYS_AXI_CLK_ID_REV2                         = 0x00000004,
    SYS_AXI_CLK_ID_SDMMC1                       = 0x00000005,
    SYS_AXI_CLK_ID_ALWAYS                       = 0x00000006
} SYS_AXI_CLKS_T;

/// reserved base number
#define NB_SYS_AXI_CLK_REV                      (2)
/// auto clock enable number
#define NB_SYS_AXI_CLK_AEN                      (6)
#define NB_SYS_AXI_CLK_EN                       (7)
#define NB_SYS_AXI_CLK                          (7)

// ============================================================================
// SYS_AHB_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Sys Ahb Clks IDs
    SYS_AHB_CLK_ID_GOUDA                        = 0x00000000,
    SYS_AHB_CLK_ID_GGE                          = 0x00000001,
    SYS_AHB_CLK_ID_GEA3                         = 0x00000002,
    SYS_AHB_CLK_ID_LZMA                         = 0x00000003,
    SYS_AHB_CLK_ID_F8                           = 0x00000004,
    SYS_AHB_CLK_ID_AXIDMA                       = 0x00000005,
    SYS_AHB_CLK_ID_USBC                         = 0x00000006,
    SYS_AHB_CLK_ID_LCD                          = 0x00000007,
    SYS_AHB_CLK_ID_AES                          = 0x00000008,
    SYS_AHB_CLK_ID_USB11                        = 0x00000009,
    SYS_AHB_CLK_ID_ALWAYS                       = 0x0000000A
} SYS_AHB_CLKS_T;

/// reserved base number
#define NB_SYS_AHB_CLK_REV                      (3)
/// auto clock enable number
#define NB_SYS_AHB_CLK_AEN                      (6)
#define NB_SYS_AHB_CLK_EN                       (11)
#define NB_SYS_AHB_CLK                          (11)

// ============================================================================
// AP_APB_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Sys Apb Clks IDs
    AP_APB_CLK_ID_CONF                          = 0x00000000,
    AP_APB_CLK_ID_MOD_CAMERA                    = 0x00000001,
    AP_APB_CLK_ID_I2C1                          = 0x00000002,
    AP_APB_CLK_ID_I2C3                          = 0x00000003,
    AP_APB_CLK_ID_IFC                           = 0x00000004,
    AP_APB_CLK_ID_IFC_CH0                       = 0x00000005,
    AP_APB_CLK_ID_IFC_CH1                       = 0x00000006,
    AP_APB_CLK_ID_IFC_CH2                       = 0x00000007,
    AP_APB_CLK_ID_IFC_CH3                       = 0x00000008,
    AP_APB_CLK_ID_IFC_CH4                       = 0x00000009,
    AP_APB_CLK_ID_IFC_CH5                       = 0x0000000A,
    AP_APB_CLK_ID_IFC_CH6                       = 0x0000000B,
    AP_APB_CLK_ID_IFC_CHDBG                     = 0x0000000C,
    AP_APB_CLK_ID_GOUDA                         = 0x0000000D,
    AP_APB_CLK_ID_SDMMC2                        = 0x0000000E,
    AP_APB_CLK_ID_SPI1                          = 0x0000000F,
    AP_APB_CLK_ID_SPI2                          = 0x00000010,
    AP_APB_CLK_ID_SCID1                         = 0x00000011,
    AP_APB_CLK_ID_SCID2                         = 0x00000012,
    AP_APB_CLK_ID_SCI1                          = 0x00000013,
    AP_APB_CLK_ID_SCI2                          = 0x00000014,
    AP_APB_CLK_ID_CAMERA                        = 0x00000015,
    AP_APB_CLK_ID_PAGESPY                       = 0x00000016,
    AP_APB_CLK_ID_LZMA                          = 0x00000017,
    AP_APB_CLK_ID_REV2                          = 0x00000018,
    AP_APB_CLK_ID_TIMER1                        = 0x00000019,
    AP_APB_CLK_ID_TIMER2                        = 0x0000001A,
    AP_APB_CLK_ID_TIMER4                        = 0x0000001B,
    AP_APB_CLK_ID_LCD                           = 0x0000001C,
    AP_APB_CLK_ID_CQIRQ                         = 0x0000001D,
    AP_APB_CLK_ID_ALWAYS                        = 0x0000001E
} AP_APB_CLKS_T;

/// auto clock enable number
#define NB_AP_APB_CLK_AEN                       (23)
#define NB_AP_APB_CLK_EN                        (31)
#define NB_AP_APB_CLK                           (31)

// ============================================================================
// AIF_APB_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Aif Apb Clks IDs
    AIF_APB_CLK_ID_CONF                         = 0x00000000,
    AIF1_APB_CLK_ID_AIF                         = 0x00000001,
    AIF2_APB_CLK_ID_AIF                         = 0x00000002,
    AIF_APB_CLK_ID_IFC                          = 0x00000003,
    AIF_APB_CLK_ID_IFC_CH0                      = 0x00000004,
    AIF_APB_CLK_ID_IFC_CH1                      = 0x00000005,
    AIF_APB_CLK_ID_IFC_CH2                      = 0x00000006,
    AIF_APB_CLK_ID_IFC_CH3                      = 0x00000007,
    AIF_APB_CLK_ID_REV0                         = 0x00000008,
    AIF_APB_CLK_ID_REV1                         = 0x00000009,
    AIF_APB_CLK_ID_REV2                         = 0x0000000A,
    AIF_APB_CLK_ID_AUD_2AD                      = 0x0000000B,
    AIF_APB_CLK_ID_ALWAYS                       = 0x0000000C
} AIF_APB_CLKS_T;

/// reserved base number
#define NB_AIF_APB_CLK_REV                      (8)
/// auto clock enable number
#define NB_AIF_APB_CLK_AEN                      (12)
#define NB_AIF_APB_CLK_EN                       (13)
#define NB_AIF_APB_CLK                          (13)

// ============================================================================
// AON_AHB_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Aon Ahb Clks IDs
    AON_AHB_CLK_ID_BB_SYSCTRL                   = 0x00000000,
    AON_AHB_CLK_ID_BB_REV0                      = 0x00000001,
    AON_AHB_CLK_ID_BB_REV1                      = 0x00000002,
    AON_AHB_CLK_ID_BB_REV2                      = 0x00000003,
    AON_AHB_CLK_ID_CSSYS                        = 0x00000004,
    AON_AHB_CLK_ID_ALWAYS                       = 0x00000005
} AON_AHB_CLKS_T;

/// reserved base number
#define NB_AON_AHB_CLK_REV                      (1)
/// auto clock enable number
#define NB_AON_AHB_CLK_AEN                      (5)
#define NB_AON_AHB_CLK_EN                       (6)
#define NB_AON_AHB_CLK                          (6)

// ============================================================================
// AON_APB_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Aon Apb Clks IDs
    AON_APB_CLK_ID_CONF                         = 0x00000000,
    AON_APB_CLK_ID_DBG_HOST                     = 0x00000001,
    AON_APB_CLK_ID_DBG_UART                     = 0x00000002,
    AON_APB_CLK_ID_IFC                          = 0x00000003,
    AON_APB_CLK_ID_IFC_CH0                      = 0x00000004,
    AON_APB_CLK_ID_IFC_CH1                      = 0x00000005,
    AON_APB_CLK_ID_IFC_CH2                      = 0x00000006,
    AON_APB_CLK_ID_IFC_CH3                      = 0x00000007,
    AON_APB_CLK_ID_IFC_CHDBG                    = 0x00000008,
    AON_APB_CLK_ID_REV0                         = 0x00000009,
    AON_APB_CLK_ID_REV1                         = 0x0000000A,
    AON_APB_CLK_ID_PWM                          = 0x0000000B,
    AON_APB_CLK_ID_I2C2                         = 0x0000000C,
    AON_APB_CLK_ID_ANA_REG                      = 0x0000000D,
    AON_APB_CLK_ID_SPINLOCK                     = 0x0000000E,
    AON_APB_CLK_ID_LPS_GSM                      = 0x0000000F,
    AON_APB_CLK_ID_LPS_NB                       = 0x00000010,
    AON_APB_CLK_ID_EFUSE                        = 0x00000011,
    AON_APB_CLK_ID_REV2                         = 0x00000012,
    AON_APB_CLK_ID_VAD                          = 0x00000013,
    AON_APB_CLK_ID_REV3                         = 0x00000014,
    AON_APB_CLK_ID_REV4                         = 0x00000015,
    AON_APB_CLK_ID_GPIO                         = 0x00000016,
    AON_APB_CLK_ID_TIMER3                       = 0x00000017,
    AON_APB_CLK_ID_KEYPAD                       = 0x00000018,
    AON_APB_CLK_ID_AHB2APB_ADI                  = 0x00000019,
    AON_APB_CLK_ID_ADI                          = 0x0000001A,
    AON_APB_CLK_ID_LVDS                         = 0x0000001B,
    AON_APB_CLK_ID_ALWAYS                       = 0x0000001C
} AON_APB_CLKS_T;

/// auto clock enable number
#define NB_AON_APB_CLK_AEN                      (20)
#define NB_AON_APB_CLK_EN                       (29)
#define NB_AON_APB_CLK                          (29)

// ============================================================================
// RF_AHB_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    RF_AHB_CLK_ID_REV0                          = 0x00000000,
    RF_AHB_CLK_ID_ALWAYS                        = 0x00000001
} RF_AHB_CLKS_T;

/// auto clock enable number
#define NB_RF_AHB_CLK_AEN                       (1)
#define NB_RF_AHB_CLK_EN                        (2)
#define NB_RF_AHB_CLK                           (2)

// ============================================================================
// RF_APB_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    RF_APB_CLK_ID_REV0                          = 0x00000000,
    RF_APB_CLK_ID_ALWAYS                        = 0x00000001
} RF_APB_CLKS_T;

/// auto clock enable number
#define NB_RF_APB_CLK_AEN                       (1)
#define NB_RF_APB_CLK_EN                        (2)
#define NB_RF_APB_CLK                           (2)

// ============================================================================
// OTHERS_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Other Clks IDs
    OC_ID_HOST_UART                             = 0x00000000,
    OC_ID_BCK1                                  = 0x00000001,
    OC_ID_BCK2                                  = 0x00000002,
    OC_ID_UART1                                 = 0x00000003,
    OC_ID_UART2                                 = 0x00000004,
    OC_ID_UART3                                 = 0x00000005,
    OC_ID_AP_A5                                 = 0x00000006,
    OC_ID_CP_A5                                 = 0x00000007,
    OC_ID_GPIO                                  = 0x00000008,
    OC_ID_USBPHY                                = 0x00000009,
    OC_ID_PIX                                   = 0x0000000A,
    OC_ID_CLK_OUT                               = 0x0000000B,
    OC_ID_ISP                                   = 0x0000000C,
    OC_ID_SYS_SPIFLASH                          = 0x0000000D,
    OC_ID_SYS_SPIFLASH_ALWAYS                   = 0x0000000E,
    OC_ID_SYS_SPIFLASH1                         = 0x0000000F,
    OC_ID_SYS_SPIFLASH1_ALWAYS                  = 0x00000010,
    OC_ID_SPIFLASH                              = 0x00000011,
    OC_ID_SPIFLASH1                             = 0x00000012,
    OC_ID_SPICAM                                = 0x00000013,
    OC_ID_CAM                                   = 0x00000014,
    OC_ID_PSRAM_CONF                            = 0x00000015,
    OC_ID_PSRAM_DMC                             = 0x00000016,
    OC_ID_PSRAM_PAGESPY                         = 0x00000017,
    OC_ID_PSRAM_ALWAYS                          = 0x00000018
} OTHERS_CLKS_T;

#define NB_CLK_OTHER_AEN                        (8)
/// System Spiflash Domain Clock ID Base
#define OC_ID_SYS_SPIFLASH_BASE                 (13)
/// Psram Ctrl Domain Clock ID Base
#define OC_ID_PSRAM_BASE                        (21)
#define NB_CLK_OTHER_EN                         (25)
#define NB_CLK_OTHER                            (25)

// ============================================================================
// OTHERS_CLKS_1_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Other Clks 1 IDs
    OC_ID_GGE_CLK32K                            = 0x00000000,
    OC_ID_GGE_26M                               = 0x00000001,
    OC_ID_NB_61P44M                             = 0x00000002,
    OC_ID_BT_FM_CLK32K                          = 0x00000003,
    OC_ID_BT_FM_26M                             = 0x00000004,
    OC_ID_PMIC_CLK32K                           = 0x00000005,
    OC_ID_PMIC_26M                              = 0x00000006,
    OC_ID_CP_CLK32K                             = 0x00000007,
    OC_ID_CP_26M                                = 0x00000008,
    OC_ID_ZSP_UART                              = 0x00000009,
    OC_ID_CP_BBLTE                              = 0x0000000A,
    OC_ID_494M_LTE                              = 0x0000000B,
    OC_ID_REV0                                  = 0x0000000C,
    OC_ID_REV1                                  = 0x0000000D,
    OC_ID_REV2                                  = 0x0000000E,
    OC_ID_REV3                                  = 0x0000000F,
    OC_ID_SDM_26M                               = 0x00000010,
    OC_ID_LPS_GSM                               = 0x00000011,
    OC_ID_LPS_NB                                = 0x00000012,
    OC_ID_EFUSE_26M                             = 0x00000013,
    OC_ID_USB_ADP_32K                           = 0x00000014,
    OC_ID_USB_UTMI_48M                          = 0x00000015,
    OC_ID_AP_26M                                = 0x00000016,
    OC_ID_AP_32K                                = 0x00000017,
    OC_ID_MIPIDSI                               = 0x00000018,
    OC_ID_AHB_BTFM                              = 0x00000019,
    OC_ID_VAD                                   = 0x0000001A,
    OC_ID_USB11_48M                             = 0x0000001B,
    OC_ID_TRNG_CLKEN                            = 0x0000001C,
    OC_ID_CORESIGHT                             = 0x0000001D,
    OC_ID_ADI                                   = 0x0000001E
} OTHERS_CLKS_1_T;

#define NB_CLK_OTHER_1_AEN                      (16)
#define NB_CLK_OTHER_1_EN                       (31)
#define NB_CLK_OTHER_1                          (31)

// ============================================================================
// PSRAM_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Psram Clks IDs
    PSRAM_CLK_ID_CONF                           = 0x00000000,
    PSRAM_CLK_ID_DMC                            = 0x00000001,
    PSRAM_CLK_ID_PAGESPY                        = 0x00000002,
    PSRAM_CLK_ID_ALWAYS                         = 0x00000003
} PSRAM_CLKS_T;

#define NB_CLK_PSRAM                            (4)

// ============================================================================
// SYS_SPIFLASH_CLKS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Sys Spiflash Clks IDs
    SPIFLASH_CLK_ID_SPIFLASH                    = 0x00000000,
    SPIFLASH_CLK_ID_ALWAYS                      = 0x00000001,
/// Sys Spiflash1 Clks IDs
    SPIFLASH_CLK_ID_SPIFLASH1                   = 0x00000002,
    SPIFLASH1_CLK_ID_ALWAYS                     = 0x00000003
} SYS_SPIFLASH_CLKS_T;


// ============================================================================
// SYS_AXI_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Sys Axi Rst IDs
    SYS_AXI_RST_ID_SYS                          = 0x00000000,
    SYS_AXI_RST_ID_IMEM                         = 0x00000001,
    SYS_AXI_RST_ID_LZMA                         = 0x00000002,
    SYS_AXI_RST_ID_REV0                         = 0x00000003,
    SYS_AXI_RST_ID_REV1                         = 0x00000004,
    SYS_AXI_RST_ID_REV2                         = 0x00000005,
    SYS_AXI_RST_ID_REV3                         = 0x00000006
} SYS_AXI_RST_T;

#define NB_SYS_AXI_RST                          (7)

// ============================================================================
// SYS_AHB_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Sys Ahb Rst IDs
    SYS_AHB_RST_ID_SYS                          = 0x00000000,
    SYS_AHB_RST_ID_GOUDA                        = 0x00000001,
    SYS_AHB_RST_ID_GGE                          = 0x00000002,
    SYS_AHB_RST_ID_GEA3                         = 0x00000003,
    SYS_AHB_RST_ID_USBC                         = 0x00000004,
    SYS_AHB_RST_ID_BTFM                         = 0x00000005,
    SYS_AHB_RST_ID_GIC400                       = 0x00000006,
    SYS_AHB_RST_ID_F8                           = 0x00000007,
    SYS_AHB_RST_ID_AXIDMA                       = 0x00000008,
    SYS_AHB_RST_ID_LZMA                         = 0x00000009,
    SYS_AHB_RST_ID_LCD                          = 0x0000000A,
    SYS_AHB_RST_ID_AES                          = 0x0000000B,
    SYS_AHB_RST_ID_USB11                        = 0x0000000C
} SYS_AHB_RST_T;

#define NB_SYS_AHB_RST                          (13)

// ============================================================================
// AP_APB_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Sys Apb Rst IDs
    AP_APB_RST_ID_SYS                           = 0x00000000,
    AP_APB_RST_ID_CAMERA                        = 0x00000001,
    AP_APB_RST_ID_I2C1                          = 0x00000002,
    AP_APB_RST_ID_I2C3                          = 0x00000003,
    AP_APB_RST_ID_IFC                           = 0x00000004,
    AP_APB_RST_ID_IMEM                          = 0x00000005,
    AP_APB_RST_ID_SDMMC1                        = 0x00000006,
    AP_APB_RST_ID_SDMMC2                        = 0x00000007,
    AP_APB_RST_ID_SPI1                          = 0x00000008,
    AP_APB_RST_ID_SPI2                          = 0x00000009,
    AP_APB_RST_ID_UART2                         = 0x0000000A,
    AP_APB_RST_ID_UART3                         = 0x0000000B,
    AP_APB_RST_ID_ZSP_UART                      = 0x0000000C,
    AP_APB_RST_ID_REV0                          = 0x0000000D,
    AP_APB_RST_ID_REV1                          = 0x0000000E,
    AP_APB_RST_ID_SCI1                          = 0x0000000F,
    AP_APB_RST_ID_SCI2                          = 0x00000010,
    AP_APB_RST_ID_PAGESPY                       = 0x00000011,
    AP_APB_RST_ID_LZMA                          = 0x00000012,
    AP_APB_RST_ID_PSRAM                         = 0x00000013,
    AP_APB_RST_ID_TIMER1                        = 0x00000014,
    AP_APB_RST_ID_TIMER2                        = 0x00000015,
    AP_APB_RST_ID_TIMER4                        = 0x00000016,
    AP_APB_RST_ID_GOUDA                         = 0x00000017,
    AP_APB_RST_ID_CQIRQ                         = 0x00000018
} AP_APB_RST_T;

#define NB_AP_APB_RST                           (25)

// ============================================================================
// AIF_APB_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Aif Apb Rst IDs
    AIF1_APB_RST_ID_SYS                         = 0x00000000,
    AIF2_APB_RST_ID_SYS                         = 0x00000001,
    AIF1_APB_RST_ID_AIF                         = 0x00000002,
    AIF2_APB_RST_ID_AIF                         = 0x00000003,
    AIF_APB_RST_ID_IFC                          = 0x00000004,
    AIF_APB_RST_ID_AUD_2AD                      = 0x00000005
} AIF_APB_RST_T;

#define NB_AIF_APB_RST                          (6)

// ============================================================================
// AON_AHB_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Aon Ahb Rst IDs
    AON_AHB_RST_ID_SYS                          = 0x00000000,
    AON_AHB_RST_ID_REV0                         = 0x00000001,
    AON_AHB_RST_ID_REV1                         = 0x00000002,
    AON_AHB_RST_ID_REV2                         = 0x00000003,
    AON_AHB_RST_ID_CSSYS                        = 0x00000004
} AON_AHB_RST_T;

#define NB_AON_AHB_RST                          (5)

// ============================================================================
// AON_APB_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Aon Apb Rst IDs
    AON_APB_RST_ID_SYS                          = 0x00000000,
    AON_APB_RST_ID_CALENDAR                     = 0x00000001,
    AON_APB_RST_ID_GPIO                         = 0x00000002,
    AON_APB_RST_ID_IFC                          = 0x00000003,
    AON_APB_RST_ID_KEYPAD                       = 0x00000004,
    AON_APB_RST_ID_PWM                          = 0x00000005,
    AON_APB_RST_ID_REV0                         = 0x00000006,
    AON_APB_RST_ID_REV1                         = 0x00000007,
    AON_APB_RST_ID_TIMER3                       = 0x00000008,
    AON_APB_RST_ID_I2C2                         = 0x00000009,
    AON_APB_RST_ID_ANA_REG                      = 0x0000000A,
    AON_APB_RST_ID_SPINLOCK                     = 0x0000000B,
    AON_APB_RST_ID_LPS_GSM                      = 0x0000000C,
    AON_APB_RST_ID_LPS_NB                       = 0x0000000D,
    AON_APB_RST_ID_EFUSE                        = 0x0000000E,
    AON_APB_RST_ID_AHB2APB_ADI                  = 0x0000000F,
    AON_APB_RST_ID_ADI                          = 0x00000010,
    AON_APB_RST_ID_LVDS                         = 0x00000011,
    AON_APB_RST_ID_UART1                        = 0x00000012,
    AON_APB_RST_ID_VAD                          = 0x00000013
} AON_APB_RST_T;

#define NB_AON_APB_RST                          (20)

// ============================================================================
// RF_AHB_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Rf Ahb Rst IDs
    RF_AHB_RST_ID_SYS                           = 0x00000000,
    RF_AHB_RST_ID_RF_REV0                       = 0x00000001
} RF_AHB_RST_T;

#define NB_RF_AHB_RST                           (2)

// ============================================================================
// RF_APB_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Rf Apb Rst IDs
    RF_APB_RST_ID_SYS                           = 0x00000000,
    RF_APB_RST_ID_REV0                          = 0x00000001
} RF_APB_RST_T;

#define NB_RF_APB_RST                           (2)

// ============================================================================
// APCPU_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// APCPU Rst IDs
    APCPU_RST_ID_SYS                            = 0x00000000,
    APCPU_RST_ID_CORE                           = 0x00000001,
    APCPU_RST_ID_DBG                            = 0x00000002,
    APCPU_RST_ID_GIC400                         = 0x00000003,
    GLOBAL_SOFT_RST                             = 0x00000004
} APCPU_RST_T;

#define NB_APCPU_RST                            (4)

// ============================================================================
// CPCPU_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// CPCPU Rst IDs
    CPCPU_RST_ID_SYS                            = 0x00000000,
    CPCPU_RST_ID_CORE                           = 0x00000001,
    CPCPU_RST_ID_DBG                            = 0x00000002
} CPCPU_RST_T;

#define NB_CPCPU_RST                            (3)

// ============================================================================
// BBLTE_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// BBlte Rst IDs
    BBLTE_RST_ID_SYS                            = 0x00000000,
    BBLTE_RST_ID_REV0                           = 0x00000001
} BBLTE_RST_T;

#define NB_BBLTE_RST                            (2)

// ============================================================================
// OTHERS_RST_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Other Rsts IDs
    RSTO_ID_BCK1                                = 0x00000000,
    RSTO_ID_BCK2                                = 0x00000001,
    RSTO_ID_DBG_HOST                            = 0x00000002,
    RSTO_ID_GPIO                                = 0x00000003,
    RSTO_ID_UART1                               = 0x00000004,
    RSTO_ID_UART2                               = 0x00000005,
    RSTO_ID_UART3                               = 0x00000006,
    RSTO_ID_USBC                                = 0x00000007,
    RSTO_ID_WDTIMER0                            = 0x00000008,
    RSTO_ID_WDTIMER1                            = 0x00000009,
    RSTO_ID_WDTIMER2                            = 0x0000000A,
    RSTO_ID_SPIFLASH                            = 0x0000000B,
    RSTO_ID_SPIFLASH_SYS                        = 0x0000000C,
    RSTO_ID_SPIFLASH1                           = 0x0000000D,
    RSTO_ID_SPIFLASH1_SYS                       = 0x0000000E,
    RSTO_ID_PSRAM_DMC                           = 0x0000000F,
    RSTO_ID_PSRAM_SYS                           = 0x00000010,
    RSTO_ID_PSRAM_PAGESPY                       = 0x00000011,
    RSTO_ID_VAD                                 = 0x00000012,
    RSTO_ID_PIX                                 = 0x00000013,
    RSTO_ID_SDM_26M                             = 0x00000014,
    RSTO_ID_WDTIMER4                            = 0x00000015,
    RSTO_ID_LPS_GSM                             = 0x00000016,
    RSTO_ID_LPS_NB                              = 0x00000017,
    RSTO_ID_EFUSE                               = 0x00000018,
    RSTO_ID_USB_ADP_32K                         = 0x00000019,
    RSTO_ID_MIPIDSI                             = 0x0000001A,
    RSTO_ID_MIPIDSI_PHY                         = 0x0000001B,
    RSTO_ID_AUD_2AD                             = 0x0000001C,
    RSTO_ID_USB11_48M                           = 0x0000001D,
    RSTO_ID_ZSP_UART                            = 0x0000001E,
    RSTO_ID_CORESIGHT                           = 0x0000001F
} OTHERS_RST_T;

#define NB_RSTO                                 (32)
/// For REG_DBG protect lock/unlock value
#define SYS_CTRL_PROTECT_LOCK                   (0XA50000)
#define SYS_CTRL_PROTECT_UNLOCK                 (0XA50001)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SYS_CTRL_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_SYS_CTRL_BASE          0x05100000
#else
#define REG_SYS_CTRL_BASE          0x50100000
#endif

typedef volatile struct
{
    /// <strong>This register is used to Lock and Unlock the protected
    /// registers.</strong>
    REG32                          REG_DBG;                      //0x00000000
    /// This register is protected.
    REG32                          SYS_AXI_RST_SET;              //0x00000004
    REG32                          SYS_AXI_RST_CLR;              //0x00000008
    /// This register is protected.
    REG32                          SYS_AHB_RST_SET;              //0x0000000C
    REG32                          SYS_AHB_RST_CLR;              //0x00000010
    /// This register is protected.
    REG32                          AP_APB_RST_SET;               //0x00000014
    REG32                          AP_APB_RST_CLR;               //0x00000018
    /// This register is protected.
    REG32                          AIF_APB_RST_SET;              //0x0000001C
    REG32                          AIF_APB_RST_CLR;              //0x00000020
    /// This register is protected.
    REG32                          AON_AHB_RST_SET;              //0x00000024
    REG32                          AON_AHB_RST_CLR;              //0x00000028
    /// This register is protected.
    REG32                          AON_APB_RST_SET;              //0x0000002C
    REG32                          AON_APB_RST_CLR;              //0x00000030
    /// This register is protected.
    REG32                          RF_AHB_RST_SET;               //0x00000034
    REG32                          RF_AHB_RST_CLR;               //0x00000038
    /// This register is protected.
    REG32                          RF_APB_RST_SET;               //0x0000003C
    REG32                          RF_APB_RST_CLR;               //0x00000040
    /// This register is protected.
    REG32                          APCPU_RST_SET;                //0x00000044
    REG32                          APCPU_RST_CLR;                //0x00000048
    /// This register is protected.
    REG32                          CPCPU_RST_SET;                //0x0000004C
    REG32                          CPCPU_RST_CLR;                //0x00000050
    /// This register is protected.
    REG32                          BBLTE_RST_SET;                //0x00000054
    REG32                          BBLTE_RST_CLR;                //0x00000058
    /// This register is protected.
    REG32                          OTHERS_RST_SET;               //0x0000005C
    REG32                          OTHERS_RST_CLR;               //0x00000060
    REG32                          CLK_SYS_AXI_MODE;             //0x00000064
    REG32                          CLK_SYS_AXI_ENABLE;           //0x00000068
    /// This register is protected.
    REG32                          CLK_SYS_AXI_DISABLE;          //0x0000006C
    REG32                          CLK_SYS_AHB_MODE;             //0x00000070
    REG32                          CLK_SYS_AHB_ENABLE;           //0x00000074
    /// This register is protected.
    REG32                          CLK_SYS_AHB_DISABLE;          //0x00000078
    REG32                          CLK_AP_APB_MODE;              //0x0000007C
    REG32                          CLK_AP_APB_ENABLE;            //0x00000080
    /// This register is protected.
    REG32                          CLK_AP_APB_DISABLE;           //0x00000084
    REG32                          CLK_AIF_APB_MODE;             //0x00000088
    REG32                          CLK_AIF_APB_ENABLE;           //0x0000008C
    /// This register is protected.
    REG32                          CLK_AIF_APB_DISABLE;          //0x00000090
    REG32                          CLK_AON_AHB_MODE;             //0x00000094
    REG32                          CLK_AON_AHB_ENABLE;           //0x00000098
    /// This register is protected.
    REG32                          CLK_AON_AHB_DISABLE;          //0x0000009C
    REG32                          CLK_AON_APB_MODE;             //0x000000A0
    REG32                          CLK_AON_APB_ENABLE;           //0x000000A4
    /// This register is protected.
    REG32                          CLK_AON_APB_DISABLE;          //0x000000A8
    REG32                          CLK_RF_AHB_MODE;              //0x000000AC
    REG32                          CLK_RF_AHB_ENABLE;            //0x000000B0
    /// This register is protected.
    REG32                          CLK_RF_AHB_DISABLE;           //0x000000B4
    REG32                          CLK_RF_APB_MODE;              //0x000000B8
    REG32                          CLK_RF_APB_ENABLE;            //0x000000BC
    /// This register is protected.
    REG32                          CLK_RF_APB_DISABLE;           //0x000000C0
    REG32                          CLK_OTHERS_MODE;              //0x000000C4
    REG32                          CLK_OTHERS_ENABLE;            //0x000000C8
    /// This register is protected.
    REG32                          CLK_OTHERS_DISABLE;           //0x000000CC
    REG32                          CLK_OTHERS_1_MODE;            //0x000000D0
    REG32                          CLK_OTHERS_1_ENABLE;          //0x000000D4
    /// This register is protected.
    REG32                          CLK_OTHERS_1_DISABLE;         //0x000000D8
    /// Register protected by Write_Unlocked_H.
    REG32                          Pll_Ctrl;                     //0x000000DC
    /// This register is protected.
    REG32                          Sel_Clock;                    //0x000000E0
    REG32 Reserved_000000E4;                    //0x000000E4
    /// This register is protected.
    REG32                          Cfg_Clk_Out;                  //0x000000E8
    REG32 Reserved_000000EC;                    //0x000000EC
    REG32                          Cfg_Clk_AudioBCK1_Div;        //0x000000F0
    REG32                          Cfg_Clk_AudioBCK2_Div;        //0x000000F4
    REG32                          Cfg_Clk_Uart[4];              //0x000000F8
    REG32                          Cfg_Clk_PWM;                  //0x00000108
    REG32                          Cfg_Clk_Dbg_Div;              //0x0000010C
    REG32                          Cfg_Clk_Camera_Out;           //0x00000110
    REG32                          Reset_Cause;                  //0x00000114
    /// This register is protected.
    REG32                          WakeUp;                       //0x00000118
    /// This register is protected.
    REG32                          Ignore_Charger;               //0x0000011C
    REG32 Reserved_00000120[2];                 //0x00000120
    REG32                          CFG_PLL_SPIFLASH_DIV;         //0x00000128
    REG32                          CFG_PLL_SPIFLASH1_DIV;        //0x0000012C
    REG32                          CFG_PLL_MEM_BRIDGE_DIV;       //0x00000130
    REG32                          CFG_DBG_CLK_SOURCE_SEL;       //0x00000134
    REG32                          CFG_CLK_494M_LTE_DIV;         //0x00000138
    REG32                          CFG_PLL_ISP_DIV;              //0x0000013C
    REG32                          CFG_PLL_PIX_DIV;              //0x00000140
    REG32                          CFG_PLL_SYS_AXI_DIV;          //0x00000144
    REG32                          CFG_PLL_SYS_AHB_APB_DIV;      //0x00000148
    REG32                          CFG_PLL_SYS_AHB_BTFM_DIV;     //0x0000014C
    REG32                          CFG_PLL_CSI_DIV;              //0x00000150
    REG32                          CFG_PLL_SYS_SPIFLASH_DIV;     //0x00000154
    REG32                          CFG_PLL_CP_DIV;               //0x00000158
    REG32                          CFG_PLL_AP_CPU_DIV;           //0x0000015C
    REG32                          CFG_AP_CPU_ACLKEN_DIV;        //0x00000160
    REG32                          CFG_AP_CPU_DBGEN_DIV;         //0x00000164
    REG32                          CFG_PLL_CP_CPU_DIV;           //0x00000168
    REG32                          CFG_CP_CPU_ACLKEN_DIV;        //0x0000016C
    REG32                          CFG_CP_CPU_DBGEN_DIV;         //0x00000170
    REG32                          CFG_TRNG_CLKEN_DIV;           //0x00000174
    REG32                          CFG_CORESIGHT_DIV;            //0x00000178
    REG32                          CFG_USB11_48M_DIV;            //0x0000017C
    REG32                          CFG_PLL_USB;                  //0x00000180
    REG32                          CFG_VAD_DIV;                  //0x00000184
    /// This register is ahb master protect cfg.
    REG32                          Cfg_HMPROT;                   //0x00000188
    /// This register is cq memory cfg.
    REG32                          Cfg_MEM_CQ;                   //0x0000018C
    /// This register is a5_top_wrap/axidma/cp_a5_top/f8/gea3_wrap/lzma/sys_imem
    /// mem cfg.
    REG32                          Cfg_ARM_SYS_CFG_MEM;          //0x00000190
    /// This register is audio mem cfg.
    REG32                          Cfg_AUDIO_CFG_MEM;            //0x00000194
    /// This register is lcd/gouda mem cfg.
    REG32                          Cfg_LCD_CFG_MEM;              //0x00000198
    /// This register is camera mem cfg.
    REG32                          Cfg_CAM_CFG_MEM;              //0x0000019C
    /// This register is peri(sdmmc/uart/usbc etc.) mem cfg.
    REG32                          Cfg_PERI_CFG_MEM;             //0x000001A0
    /// This register is aon sys mem cfg.
    REG32                          Cfg_AON_SYS_CFG_MEM;          //0x000001A4
    /// This register is rf sys mem cfg.
    REG32                          Cfg_RF_SYS_CFG_MEM;           //0x000001A8
    /// This register is coresight mem cfg.
    REG32                          Cfg_CORESIGHT_CFG_MEM;        //0x000001AC
    /// This register is vad mem cfg.
    REG32                          Cfg_VAD_CFG_MEM;              //0x000001B0
    /// This register is for audio i2s mux ,aif load_position etc. config.
    REG32                          Cfg_AIF_CFG;                  //0x000001B4
    /// This register is limit_en_spi,,clk_freq cfg.
    REG32                          Cfg_MISC_CFG;                 //0x000001B8
    /// This register is misc cfg.
    REG32                          Cfg_MISC1_CFG;                //0x000001BC
    /// This register set lp related config.
    REG32                          Cfg_FORCE_LP_MODE_LP;         //0x000001C0
    REG32 Reserved_000001C4[4];                 //0x000001C4
    /// This register is reserved.
    REG32                          Cfg_Reserve;                  //0x000001D4
    /// This register is reserved.
    REG32                          Cfg_Reserve1;                 //0x000001D8
    /// This register is reserved.
    REG32                          Cfg_Reserve2;                 //0x000001DC
    /// This register is reserved.
    REG32                          Cfg_Reserve3;                 //0x000001E0
    /// This register is reserved.
    REG32                          Cfg_Reserve4;                 //0x000001E4
    /// This register is reserved.
    REG32                          Cfg_Reserve5;                 //0x000001E8
    /// This register is reserved.
    REG32                          Cfg_Reserve6;                 //0x000001EC
    /// This register is reserved.
    REG32                          Cfg_Reserve7;                 //0x000001F0
    /// This register is reserved.
    REG32                          Cfg_Reserve8;                 //0x000001F4
    /// This register is reserved.
    REG32                          Cfg_Reserve9;                 //0x000001F8
    /// This register is reserved.
    REG32                          Cfg_Reserve10;                //0x000001FC
    /// This register is reserved.
    REG32                          Cfg_Reserve11;                //0x00000200
    /// This register is reserved.
    REG32                          Cfg_Reserve12;                //0x00000204
    /// This register is reserved.
    REG32                          Cfg_Reserve13;                //0x00000208
    /// This register is reserved.
    REG32                          Cfg_Reserve14;                //0x0000020C
    /// This register is reserved.
    REG32                          Cfg_Reserve15;                //0x00000210
    /// This register is reserved.
    REG32                          Cfg_Reserve16;                //0x00000214
    /// This register is reserved.
    REG32                          Cfg_Reserve1_in;              //0x00000218
    /// This register is reserved.
    REG32                          Cfg_Reserve2_in;              //0x0000021C
    /// This register is reserved.
    REG32                          Cfg_Reserve3_in;              //0x00000220
    /// This register is reserved.
    REG32                          Cfg_Reserve4_in;              //0x00000224
    /// This register is reserved.
    REG32                          Cfg_Reserve5_in;              //0x00000228
    /// This register is reserved.
    REG32                          Cfg_Reserve6_in;              //0x0000022C
    /// This register is reserved.
    REG32                          Cfg_Reserve7_in;              //0x00000230
    /// This register is for CHIP_ID(METAL_ID[11:0],BOND_ID[15:12]),PROD[31:16]
    REG32                          Cfg_CHIP_PROD_ID;             //0x00000234
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_WCN_A5_GGE;           //0x00000238
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_AXIDMA_CPA5_F8;       //0x0000023C
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_LCDC_LZMA_GOUDA;      //0x00000240
    /// This register is for BUS QOS config.
    REG32                          Cfg_QOS_LTE_USB;              //0x00000244
    /// This register is for merge mem awqos/arqos QOS config.
    REG32                          Cfg_QOS_MERGE_MEM;            //0x00000248
    /// This register is for bcpu break point debug.
    REG32                          Cfg_BCPU_DBG_BKP;             //0x0000024C
} HWP_SYS_CTRL_T;

#define hwp_sysCtrl                ((HWP_SYS_CTRL_T*) REG_ACCESS_ADDRESS(REG_SYS_CTRL_BASE))


//REG_DBG
typedef union {
    REG32 v;
    struct {
        REG32 scratch : 16; // [15:0]
        REG32 __29_16 : 14;
        REG32 write_unlock_status : 1; // [30], read only
        REG32 write_unlock : 1; // [31], read only
    } b;
} REG_SYS_CTRL_REG_DBG_T;

//SYS_AXI_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_sys_axi_rst_id_sys : 1; // [0], write set
        REG32 set_sys_axi_rst_id_imem : 1; // [1], write set
        REG32 set_sys_axi_rst_id_lzma : 1; // [2], write set
        REG32 set_sys_axi_rst_id_rev0 : 1; // [3], write set
        REG32 set_sys_axi_rst_id_rev1 : 1; // [4], write set
        REG32 set_sys_axi_rst_id_rev2 : 1; // [5], write set
        REG32 set_sys_axi_rst_id_rev3 : 1; // [6], write set
        REG32 __31_7 : 25;
    } b;
} REG_SYS_CTRL_SYS_AXI_RST_SET_T;

//SYS_AXI_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_sys_axi_rst_id_sys : 1; // [0], write clear
        REG32 clr_sys_axi_rst_id_imem : 1; // [1], write clear
        REG32 clr_sys_axi_rst_id_lzma : 1; // [2], write clear
        REG32 clr_sys_axi_rst_id_rev0 : 1; // [3], write clear
        REG32 clr_sys_axi_rst_id_rev1 : 1; // [4], write clear
        REG32 clr_sys_axi_rst_id_rev2 : 1; // [5], write clear
        REG32 clr_sys_axi_rst_id_rev3 : 1; // [6], write clear
        REG32 __31_7 : 25;
    } b;
} REG_SYS_CTRL_SYS_AXI_RST_CLR_T;

//SYS_AHB_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_sys_ahb_rst_id_sys : 1; // [0], write set
        REG32 set_sys_ahb_rst_id_gouda : 1; // [1], write set
        REG32 set_sys_ahb_rst_id_gge : 1; // [2], write set
        REG32 set_sys_ahb_rst_id_gea3 : 1; // [3], write set
        REG32 set_sys_ahb_rst_id_usbc : 1; // [4], write set
        REG32 set_sys_ahb_rst_id_btfm : 1; // [5], write set
        REG32 set_sys_ahb_rst_id_gic400 : 1; // [6], write set
        REG32 set_sys_ahb_rst_id_f8 : 1; // [7], write set
        REG32 set_sys_ahb_rst_id_axidma : 1; // [8], write set
        REG32 set_sys_ahb_rst_id_lzma : 1; // [9], write set
        REG32 set_sys_ahb_rst_id_lcd : 1; // [10], write set
        REG32 set_sys_ahb_rst_id_aes : 1; // [11], write set
        REG32 set_sys_ahb_rst_id_usb11 : 1; // [12], write set
        REG32 __31_13 : 19;
    } b;
} REG_SYS_CTRL_SYS_AHB_RST_SET_T;

//SYS_AHB_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_sys_ahb_rst_id_sys : 1; // [0], write clear
        REG32 clr_sys_ahb_rst_id_gouda : 1; // [1], write clear
        REG32 clr_sys_ahb_rst_id_gge : 1; // [2], write clear
        REG32 clr_sys_ahb_rst_id_gea3 : 1; // [3], write clear
        REG32 clr_sys_ahb_rst_id_usbc : 1; // [4], write clear
        REG32 clr_sys_ahb_rst_id_btfm : 1; // [5], write clear
        REG32 clr_sys_ahb_rst_id_gic400 : 1; // [6], write clear
        REG32 clr_sys_ahb_rst_id_f8 : 1; // [7], write clear
        REG32 clr_sys_ahb_rst_id_axidma : 1; // [8], write clear
        REG32 clr_sys_ahb_rst_id_lzma : 1; // [9], write clear
        REG32 clr_sys_ahb_rst_id_lcd : 1; // [10], write clear
        REG32 clr_sys_ahb_rst_id_aes : 1; // [11], write clear
        REG32 clr_sys_ahb_rst_id_usb11 : 1; // [12], write clear
        REG32 __31_13 : 19;
    } b;
} REG_SYS_CTRL_SYS_AHB_RST_CLR_T;

//AP_APB_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_ap_apb_rst_id_sys : 1; // [0], write set
        REG32 set_ap_apb_rst_id_camera : 1; // [1], write set
        REG32 set_ap_apb_rst_id_i2c1 : 1; // [2], write set
        REG32 set_ap_apb_rst_id_i2c3 : 1; // [3], write set
        REG32 set_ap_apb_rst_id_ifc : 1; // [4], write set
        REG32 set_ap_apb_rst_id_imem : 1; // [5], write set
        REG32 set_ap_apb_rst_id_sdmmc1 : 1; // [6], write set
        REG32 set_ap_apb_rst_id_sdmmc2 : 1; // [7], write set
        REG32 set_ap_apb_rst_id_spi1 : 1; // [8], write set
        REG32 set_ap_apb_rst_id_spi2 : 1; // [9], write set
        REG32 set_ap_apb_rst_id_uart2 : 1; // [10], write set
        REG32 set_ap_apb_rst_id_uart3 : 1; // [11], write set
        REG32 set_ap_apb_rst_id_zsp_uart : 1; // [12], write set
        REG32 set_ap_apb_rst_id_rev0 : 1; // [13], write set
        REG32 set_ap_apb_rst_id_rev1 : 1; // [14], write set
        REG32 set_ap_apb_rst_id_sci1 : 1; // [15], write set
        REG32 set_ap_apb_rst_id_sci2 : 1; // [16], write set
        REG32 set_ap_apb_rst_id_pagespy : 1; // [17], write set
        REG32 set_ap_apb_rst_id_lzma : 1; // [18], write set
        REG32 set_ap_apb_rst_id_psram : 1; // [19], write set
        REG32 set_ap_apb_rst_id_timer1 : 1; // [20], write set
        REG32 set_ap_apb_rst_id_timer2 : 1; // [21], write set
        REG32 set_ap_apb_rst_id_timer4 : 1; // [22], write set
        REG32 set_ap_apb_rst_id_gouda : 1; // [23], write set
        REG32 set_ap_apb_rst_id_cqirq : 1; // [24], write set
        REG32 __31_25 : 7;
    } b;
} REG_SYS_CTRL_AP_APB_RST_SET_T;

//AP_APB_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_ap_apb_rst_id_sys : 1; // [0], write clear
        REG32 clr_ap_apb_rst_id_camera : 1; // [1], write clear
        REG32 clr_ap_apb_rst_id_i2c1 : 1; // [2], write clear
        REG32 clr_ap_apb_rst_id_i2c3 : 1; // [3], write clear
        REG32 clr_ap_apb_rst_id_ifc : 1; // [4], write clear
        REG32 clr_ap_apb_rst_id_imem : 1; // [5], write clear
        REG32 clr_ap_apb_rst_id_sdmmc1 : 1; // [6], write clear
        REG32 clr_ap_apb_rst_id_sdmmc2 : 1; // [7], write clear
        REG32 clr_ap_apb_rst_id_spi1 : 1; // [8], write clear
        REG32 clr_ap_apb_rst_id_spi2 : 1; // [9], write clear
        REG32 clr_ap_apb_rst_id_uart2 : 1; // [10], write clear
        REG32 clr_ap_apb_rst_id_uart3 : 1; // [11], write clear
        REG32 clr_ap_apb_rst_id_zsp_uart : 1; // [12], write clear
        REG32 clr_ap_apb_rst_id_rev0 : 1; // [13], write clear
        REG32 clr_ap_apb_rst_id_rev1 : 1; // [14], write clear
        REG32 clr_ap_apb_rst_id_sci1 : 1; // [15], write clear
        REG32 clr_ap_apb_rst_id_sci2 : 1; // [16], write clear
        REG32 clr_ap_apb_rst_id_pagespy : 1; // [17], write clear
        REG32 clr_ap_apb_rst_id_lzma : 1; // [18], write clear
        REG32 clr_ap_apb_rst_id_psram : 1; // [19], write clear
        REG32 clr_ap_apb_rst_id_timer1 : 1; // [20], write clear
        REG32 clr_ap_apb_rst_id_timer2 : 1; // [21], write clear
        REG32 clr_ap_apb_rst_id_timer4 : 1; // [22], write clear
        REG32 clr_ap_apb_rst_id_gouda : 1; // [23], write clear
        REG32 clr_ap_apb_rst_id_cqirq : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_SYS_CTRL_AP_APB_RST_CLR_T;

//AIF_APB_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_aif1_apb_rst_id_sys : 1; // [0], write set
        REG32 set_aif2_apb_rst_id_sys : 1; // [1], write set
        REG32 set_aif1_apb_rst_id_aif : 1; // [2], write set
        REG32 set_aif2_apb_rst_id_aif : 1; // [3], write set
        REG32 set_aif_apb_rst_id_ifc : 1; // [4], write set
        REG32 set_aif_apb_rst_id_aud_2ad : 1; // [5], write set
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_AIF_APB_RST_SET_T;

//AIF_APB_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_aif1_apb_rst_id_sys : 1; // [0], write clear
        REG32 clr_aif2_apb_rst_id_sys : 1; // [1], write clear
        REG32 clr_aif1_apb_rst_id_aif : 1; // [2], write clear
        REG32 clr_aif2_apb_rst_id_aif : 1; // [3], write clear
        REG32 clr_aif_apb_rst_id_ifc : 1; // [4], write clear
        REG32 clr_aif_apb_rst_id_aud_2ad : 1; // [5], write clear
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_AIF_APB_RST_CLR_T;

//AON_AHB_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_aon_ahb_rst_id_sys : 1; // [0], write set
        REG32 set_aon_ahb_rst_id_rev0 : 1; // [1], write set
        REG32 set_aon_ahb_rst_id_rev1 : 1; // [2], write set
        REG32 set_aon_ahb_rst_id_rev2 : 1; // [3], write set
        REG32 set_aon_ahb_rst_id_cssys : 1; // [4], write set
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_AON_AHB_RST_SET_T;

//AON_AHB_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_aon_ahb_rst_id_sys : 1; // [0], write clear
        REG32 clr_aon_ahb_rst_id_rev0 : 1; // [1], write clear
        REG32 clr_aon_ahb_rst_id_rev1 : 1; // [2], write clear
        REG32 clr_aon_ahb_rst_id_rev2 : 1; // [3], write clear
        REG32 clr_aon_ahb_rst_id_cssys : 1; // [4], write clear
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_AON_AHB_RST_CLR_T;

//AON_APB_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_aon_apb_rst_id_sys : 1; // [0], write set
        REG32 set_aon_apb_rst_id_calendar : 1; // [1], write set
        REG32 set_aon_apb_rst_id_gpio : 1; // [2], write set
        REG32 set_aon_apb_rst_id_ifc : 1; // [3], write set
        REG32 set_aon_apb_rst_id_keypad : 1; // [4], write set
        REG32 set_aon_apb_rst_id_pwm : 1; // [5], write set
        REG32 set_aon_apb_rst_id_rev0 : 1; // [6], write set
        REG32 set_aon_apb_rst_id_rev1 : 1; // [7], write set
        REG32 set_aon_apb_rst_id_timer3 : 1; // [8], write set
        REG32 set_aon_apb_rst_id_i2c2 : 1; // [9], write set
        REG32 set_aon_apb_rst_id_ana_reg : 1; // [10], write set
        REG32 set_aon_apb_rst_id_spinlock : 1; // [11], write set
        REG32 set_aon_apb_rst_id_lps_gsm : 1; // [12], write set
        REG32 set_aon_apb_rst_id_lps_nb : 1; // [13], write set
        REG32 set_aon_apb_rst_id_efuse : 1; // [14], write set
        REG32 set_aon_apb_rst_id_ahb2apb_adi : 1; // [15], write set
        REG32 set_aon_apb_rst_id_adi : 1; // [16], write set
        REG32 set_aon_apb_rst_id_lvds : 1; // [17], write set
        REG32 set_aon_apb_rst_id_uart1 : 1; // [18], write set
        REG32 set_aon_apb_rst_id_vad : 1; // [19], write set
        REG32 __31_20 : 12;
    } b;
} REG_SYS_CTRL_AON_APB_RST_SET_T;

//AON_APB_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_aon_apb_rst_id_sys : 1; // [0], write clear
        REG32 clr_aon_apb_rst_id_calendar : 1; // [1], write clear
        REG32 clr_aon_apb_rst_id_gpio : 1; // [2], write clear
        REG32 clr_aon_apb_rst_id_ifc : 1; // [3], write clear
        REG32 clr_aon_apb_rst_id_keypad : 1; // [4], write clear
        REG32 clr_aon_apb_rst_id_pwm : 1; // [5], write clear
        REG32 clr_aon_apb_rst_id_rev0 : 1; // [6], write clear
        REG32 clr_aon_apb_rst_id_rev1 : 1; // [7], write clear
        REG32 clr_aon_apb_rst_id_timer3 : 1; // [8], write clear
        REG32 clr_aon_apb_rst_id_i2c2 : 1; // [9], write clear
        REG32 clr_aon_apb_rst_id_ana_reg : 1; // [10], write clear
        REG32 clr_aon_apb_rst_id_spinlock : 1; // [11], write clear
        REG32 clr_aon_apb_rst_id_lps_gsm : 1; // [12], write clear
        REG32 clr_aon_apb_rst_id_lps_nb : 1; // [13], write clear
        REG32 clr_aon_apb_rst_id_efuse : 1; // [14], write clear
        REG32 clr_aon_apb_rst_id_ahb2apb_adi : 1; // [15], write clear
        REG32 clr_aon_apb_rst_id_adi : 1; // [16], write clear
        REG32 clr_aon_apb_rst_id_lvds : 1; // [17], write clear
        REG32 clr_aon_apb_rst_id_uart1 : 1; // [18], write clear
        REG32 clr_aon_apb_rst_id_vad : 1; // [19], write clear
        REG32 __31_20 : 12;
    } b;
} REG_SYS_CTRL_AON_APB_RST_CLR_T;

//RF_AHB_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_rf_ahb_rst_id_sys : 1; // [0], write set
        REG32 set_rf_ahb_rst_id_rf_rev0 : 1; // [1], write set
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_RF_AHB_RST_SET_T;

//RF_AHB_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_rf_ahb_rst_id_sys : 1; // [0], write clear
        REG32 clr_rf_ahb_rst_id_rf_rev0 : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_RF_AHB_RST_CLR_T;

//RF_APB_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_rf_apb_rst_id_sys : 1; // [0], write set
        REG32 set_rf_apb_rst_id_rev0 : 1; // [1], write set
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_RF_APB_RST_SET_T;

//RF_APB_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_rf_apb_rst_id_sys : 1; // [0], write clear
        REG32 clr_rf_apb_rst_id_rev0 : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_RF_APB_RST_CLR_T;

//APCPU_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_apcpu_rst_id_sys : 1; // [0], write set
        REG32 set_apcpu_rst_id_core : 1; // [1], write set
        REG32 set_apcpu_rst_id_dbg : 1; // [2], write set
        REG32 set_apcpu_rst_id_gic400 : 1; // [3], write set
        REG32 set_global_soft_rst : 1; // [4], write set
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_APCPU_RST_SET_T;

//APCPU_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_apcpu_rst_id_sys : 1; // [0], write clear
        REG32 clr_apcpu_rst_id_core : 1; // [1], write clear
        REG32 clr_apcpu_rst_id_dbg : 1; // [2], write clear
        REG32 clr_apcpu_rst_id_gic400 : 1; // [3], write clear
        REG32 clr_global_soft_rst : 1; // [4], write clear
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_APCPU_RST_CLR_T;

//CPCPU_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_cpcpu_rst_id_sys : 1; // [0], write set
        REG32 set_cpcpu_rst_id_core : 1; // [1], write set
        REG32 set_cpcpu_rst_id_dbg : 1; // [2], write set
        REG32 set_3 : 1; // [3], write set
        REG32 __31_4 : 28;
    } b;
} REG_SYS_CTRL_CPCPU_RST_SET_T;

//CPCPU_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_cpcpu_rst_id_sys : 1; // [0], write clear
        REG32 clr_cpcpu_rst_id_core : 1; // [1], write clear
        REG32 clr_cpcpu_rst_id_dbg : 1; // [2], write clear
        REG32 clr_3 : 1; // [3], write clear
        REG32 __31_4 : 28;
    } b;
} REG_SYS_CTRL_CPCPU_RST_CLR_T;

//BBLTE_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_bblte_rst_id_sys : 1; // [0], write set
        REG32 set_bblte_rst_id_rev0 : 1; // [1], write set
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_BBLTE_RST_SET_T;

//BBLTE_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_bblte_rst_id_sys : 1; // [0], write clear
        REG32 clr_bblte_rst_id_rev0 : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_BBLTE_RST_CLR_T;

//OTHERS_RST_SET
typedef union {
    REG32 v;
    struct {
        REG32 set_rsto_id_bck1 : 1; // [0], write set
        REG32 set_rsto_id_bck2 : 1; // [1], write set
        REG32 set_rsto_id_dbg_host : 1; // [2], write set
        REG32 set_rsto_id_gpio : 1; // [3], write set
        REG32 set_rsto_id_uart1 : 1; // [4], write set
        REG32 set_rsto_id_uart2 : 1; // [5], write set
        REG32 set_rsto_id_uart3 : 1; // [6], write set
        REG32 set_rsto_id_usbc : 1; // [7], write set
        REG32 set_rsto_id_wdtimer0 : 1; // [8], write set
        REG32 set_rsto_id_wdtimer1 : 1; // [9], write set
        REG32 set_rsto_id_wdtimer2 : 1; // [10], write set
        REG32 set_rsto_id_spiflash : 1; // [11], write set
        REG32 set_rsto_id_spiflash_sys : 1; // [12], write set
        REG32 set_rsto_id_spiflash1 : 1; // [13], write set
        REG32 set_rsto_id_spiflash1_sys : 1; // [14], write set
        REG32 set_rsto_id_psram_dmc : 1; // [15], write set
        REG32 set_rsto_id_psram_sys : 1; // [16], write set
        REG32 set_rsto_id_psram_pagespy : 1; // [17], write set
        REG32 set_rsto_id_vad : 1; // [18], write set
        REG32 set_rsto_id_pix : 1; // [19], write set
        REG32 set_rsto_id_sdm_26m : 1; // [20], write set
        REG32 set_rsto_id_wdtimer4 : 1; // [21], write set
        REG32 set_rsto_id_lps_gsm : 1; // [22], write set
        REG32 set_rsto_id_lps_nb : 1; // [23], write set
        REG32 set_rsto_id_efuse : 1; // [24], write set
        REG32 set_rsto_id_usb_adp_32k : 1; // [25], write set
        REG32 set_rsto_id_mipidsi : 1; // [26], write set
        REG32 set_rsto_id_mipidsi_phy : 1; // [27], write set
        REG32 set_rsto_id_aud_2ad : 1; // [28], write set
        REG32 set_rsto_id_usb11_48m : 1; // [29], write set
        REG32 set_rsto_id_zsp_uart : 1; // [30], write set
        REG32 set_rsto_id_coresight : 1; // [31], write set
    } b;
} REG_SYS_CTRL_OTHERS_RST_SET_T;

//OTHERS_RST_CLR
typedef union {
    REG32 v;
    struct {
        REG32 clr_rsto_id_bck1 : 1; // [0], write clear
        REG32 clr_rsto_id_bck2 : 1; // [1], write clear
        REG32 clr_rsto_id_dbg_host : 1; // [2], write clear
        REG32 clr_rsto_id_gpio : 1; // [3], write clear
        REG32 clr_rsto_id_uart1 : 1; // [4], write clear
        REG32 clr_rsto_id_uart2 : 1; // [5], write clear
        REG32 clr_rsto_id_uart3 : 1; // [6], write clear
        REG32 clr_rsto_id_usbc : 1; // [7], write clear
        REG32 clr_rsto_id_wdtimer0 : 1; // [8], write clear
        REG32 clr_rsto_id_wdtimer1 : 1; // [9], write clear
        REG32 clr_rsto_id_wdtimer2 : 1; // [10], write clear
        REG32 clr_rsto_id_spiflash : 1; // [11], write clear
        REG32 clr_rsto_id_spiflash_sys : 1; // [12], write clear
        REG32 clr_rsto_id_spiflash1 : 1; // [13], write clear
        REG32 clr_rsto_id_spiflash1_sys : 1; // [14], write clear
        REG32 clr_rsto_id_psram_dmc : 1; // [15], write clear
        REG32 clr_rsto_id_psram_sys : 1; // [16], write clear
        REG32 clr_rsto_id_psram_pagespy : 1; // [17], write clear
        REG32 clr_rsto_id_vad : 1; // [18], write clear
        REG32 clr_rsto_id_pix : 1; // [19], write clear
        REG32 clr_rsto_id_sdm_26m : 1; // [20], write clear
        REG32 clr_rsto_id_wdtimer4 : 1; // [21], write clear
        REG32 clr_rsto_id_lps_gsm : 1; // [22], write clear
        REG32 clr_rsto_id_lps_nb : 1; // [23], write clear
        REG32 clr_rsto_id_efuse : 1; // [24], write clear
        REG32 clr_rsto_id_usb_adp_32k : 1; // [25], write clear
        REG32 clr_rsto_id_mipidsi : 1; // [26], write clear
        REG32 clr_rsto_id_mipidsi_phy : 1; // [27], write clear
        REG32 clr_rsto_id_aud_2ad : 1; // [28], write clear
        REG32 clr_rsto_id_usb11_48m : 1; // [29], write clear
        REG32 clr_rsto_id_zsp_uart : 1; // [30], write clear
        REG32 clr_rsto_id_coresight : 1; // [31], write clear
    } b;
} REG_SYS_CTRL_OTHERS_RST_CLR_T;

//CLK_SYS_AXI_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_sys_axi_clk_id_imem : 1; // [0]
        REG32 mode_sys_axi_clk_id_lzma : 1; // [1]
        REG32 mode_sys_axi_clk_id_rev0 : 1; // [2]
        REG32 mode_sys_axi_clk_id_rev1 : 1; // [3]
        REG32 mode_sys_axi_clk_id_rev2 : 1; // [4]
        REG32 mode_sys_axi_clk_id_sdmmc1 : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_CLK_SYS_AXI_MODE_T;

//CLK_SYS_AXI_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_sys_axi_clk_id_imem : 1; // [0], write set
        REG32 enable_sys_axi_clk_id_lzma : 1; // [1], write set
        REG32 enable_sys_axi_clk_id_rev0 : 1; // [2], write set
        REG32 enable_sys_axi_clk_id_rev1 : 1; // [3], write set
        REG32 enable_sys_axi_clk_id_rev2 : 1; // [4], write set
        REG32 enable_sys_axi_clk_id_sdmmc1 : 1; // [5], write set
        REG32 enable_sys_axi_clk_id_always : 1; // [6], write set
        REG32 __31_7 : 25;
    } b;
} REG_SYS_CTRL_CLK_SYS_AXI_ENABLE_T;

//CLK_SYS_AXI_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_sys_axi_clk_id_imem : 1; // [0], write clear
        REG32 disable_sys_axi_clk_id_lzma : 1; // [1], write clear
        REG32 disable_sys_axi_clk_id_rev0 : 1; // [2], write clear
        REG32 disable_sys_axi_clk_id_rev1 : 1; // [3], write clear
        REG32 disable_sys_axi_clk_id_rev2 : 1; // [4], write clear
        REG32 disable_sys_axi_clk_id_sdmmc1 : 1; // [5], write clear
        REG32 disable_sys_axi_clk_id_always : 1; // [6], write clear
        REG32 __31_7 : 25;
    } b;
} REG_SYS_CTRL_CLK_SYS_AXI_DISABLE_T;

//CLK_SYS_AHB_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_sys_ahb_clk_id_gouda : 1; // [0]
        REG32 mode_sys_ahb_clk_id_gge : 1; // [1]
        REG32 mode_sys_ahb_clk_id_gea3 : 1; // [2]
        REG32 mode_sys_ahb_clk_id_lzma : 1; // [3]
        REG32 mode_sys_ahb_clk_id_f8 : 1; // [4]
        REG32 mode_sys_ahb_clk_id_axidma : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_CLK_SYS_AHB_MODE_T;

//CLK_SYS_AHB_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_sys_ahb_clk_id_gouda : 1; // [0], write set
        REG32 enable_sys_ahb_clk_id_gge : 1; // [1], write set
        REG32 enable_sys_ahb_clk_id_gea3 : 1; // [2], write set
        REG32 enable_sys_ahb_clk_id_lzma : 1; // [3], write set
        REG32 enable_sys_ahb_clk_id_f8 : 1; // [4], write set
        REG32 enable_sys_ahb_clk_id_axidma : 1; // [5], write set
        REG32 enable_sys_ahb_clk_id_usbc : 1; // [6], write set
        REG32 enable_sys_ahb_clk_id_lcd : 1; // [7], write set
        REG32 enable_sys_ahb_clk_id_aes : 1; // [8], write set
        REG32 enable_sys_ahb_clk_id_usb11 : 1; // [9], write set
        REG32 enable_sys_ahb_clk_id_always : 1; // [10], write set
        REG32 __31_11 : 21;
    } b;
} REG_SYS_CTRL_CLK_SYS_AHB_ENABLE_T;

//CLK_SYS_AHB_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_sys_ahb_clk_id_gouda : 1; // [0], write clear
        REG32 disable_sys_ahb_clk_id_gge : 1; // [1], write clear
        REG32 disable_sys_ahb_clk_id_gea3 : 1; // [2], write clear
        REG32 disable_sys_ahb_clk_id_lzma : 1; // [3], write clear
        REG32 disable_sys_ahb_clk_id_f8 : 1; // [4], write clear
        REG32 disable_sys_ahb_clk_id_axidma : 1; // [5], write clear
        REG32 disable_sys_ahb_clk_id_usbc : 1; // [6], write clear
        REG32 disable_sys_ahb_clk_id_lcd : 1; // [7], write clear
        REG32 disable_sys_ahb_clk_id_aes : 1; // [8], write clear
        REG32 disable_sys_ahb_clk_id_usb11 : 1; // [9], write clear
        REG32 disable_sys_ahb_clk_id_always : 1; // [10], write clear
        REG32 __31_11 : 21;
    } b;
} REG_SYS_CTRL_CLK_SYS_AHB_DISABLE_T;

//CLK_AP_APB_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_ap_apb_clk_id_conf : 1; // [0]
        REG32 mode_ap_apb_clk_id_mod_camera : 1; // [1]
        REG32 mode_ap_apb_clk_id_i2c1 : 1; // [2]
        REG32 mode_ap_apb_clk_id_i2c3 : 1; // [3]
        REG32 mode_ap_apb_clk_id_ifc : 1; // [4]
        REG32 mode_ap_apb_clk_id_ifc_ch0 : 1; // [5]
        REG32 mode_ap_apb_clk_id_ifc_ch1 : 1; // [6]
        REG32 mode_ap_apb_clk_id_ifc_ch2 : 1; // [7]
        REG32 mode_ap_apb_clk_id_ifc_ch3 : 1; // [8]
        REG32 mode_ap_apb_clk_id_ifc_ch4 : 1; // [9]
        REG32 mode_ap_apb_clk_id_ifc_ch5 : 1; // [10]
        REG32 mode_ap_apb_clk_id_ifc_ch6 : 1; // [11]
        REG32 mode_ap_apb_clk_id_ifc_chdbg : 1; // [12]
        REG32 mode_ap_apb_clk_id_gouda : 1; // [13]
        REG32 mode_ap_apb_clk_id_sdmmc2 : 1; // [14]
        REG32 mode_ap_apb_clk_id_spi1 : 1; // [15]
        REG32 mode_ap_apb_clk_id_spi2 : 1; // [16]
        REG32 mode_ap_apb_clk_id_scid1 : 1; // [17]
        REG32 mode_ap_apb_clk_id_scid2 : 1; // [18]
        REG32 mode_ap_apb_clk_id_sci1 : 1; // [19]
        REG32 mode_ap_apb_clk_id_sci2 : 1; // [20]
        REG32 mode_ap_apb_clk_id_camera : 1; // [21]
        REG32 mode_ap_apb_clk_id_pagespy : 1; // [22]
        REG32 __31_23 : 9;
    } b;
} REG_SYS_CTRL_CLK_AP_APB_MODE_T;

//CLK_AP_APB_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_ap_apb_clk_id_conf : 1; // [0], write set
        REG32 enable_ap_apb_clk_id_mod_camera : 1; // [1], write set
        REG32 enable_ap_apb_clk_id_i2c1 : 1; // [2], write set
        REG32 enable_ap_apb_clk_id_i2c3 : 1; // [3], write set
        REG32 enable_ap_apb_clk_id_ifc : 1; // [4], write set
        REG32 enable_ap_apb_clk_id_ifc_ch0 : 1; // [5], write set
        REG32 enable_ap_apb_clk_id_ifc_ch1 : 1; // [6], write set
        REG32 enable_ap_apb_clk_id_ifc_ch2 : 1; // [7], write set
        REG32 enable_ap_apb_clk_id_ifc_ch3 : 1; // [8], write set
        REG32 enable_ap_apb_clk_id_ifc_ch4 : 1; // [9], write set
        REG32 enable_ap_apb_clk_id_ifc_ch5 : 1; // [10], write set
        REG32 enable_ap_apb_clk_id_ifc_ch6 : 1; // [11], write set
        REG32 enable_ap_apb_clk_id_ifc_chdbg : 1; // [12], write set
        REG32 enable_ap_apb_clk_id_gouda : 1; // [13], write set
        REG32 enable_ap_apb_clk_id_sdmmc2 : 1; // [14], write set
        REG32 enable_ap_apb_clk_id_spi1 : 1; // [15], write set
        REG32 enable_ap_apb_clk_id_spi2 : 1; // [16], write set
        REG32 enable_ap_apb_clk_id_scid1 : 1; // [17], write set
        REG32 enable_ap_apb_clk_id_scid2 : 1; // [18], write set
        REG32 enable_ap_apb_clk_id_sci1 : 1; // [19], write set
        REG32 enable_ap_apb_clk_id_sci2 : 1; // [20], write set
        REG32 enable_ap_apb_clk_id_camera : 1; // [21], write set
        REG32 enable_ap_apb_clk_id_pagespy : 1; // [22], write set
        REG32 enable_ap_apb_clk_id_lzma : 1; // [23], write set
        REG32 enable_ap_apb_clk_id_rev2 : 1; // [24], write set
        REG32 enable_ap_apb_clk_id_timer1 : 1; // [25], write set
        REG32 enable_ap_apb_clk_id_timer2 : 1; // [26], write set
        REG32 enable_ap_apb_clk_id_timer4 : 1; // [27], write set
        REG32 enable_ap_apb_clk_id_lcd : 1; // [28], write set
        REG32 enable_ap_apb_clk_id_cqirq : 1; // [29], write set
        REG32 enable_ap_apb_clk_id_always : 1; // [30], write set
        REG32 __31_31 : 1;
    } b;
} REG_SYS_CTRL_CLK_AP_APB_ENABLE_T;

//CLK_AP_APB_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_ap_apb_clk_id_conf : 1; // [0], write clear
        REG32 disable_ap_apb_clk_id_mod_camera : 1; // [1], write clear
        REG32 disable_ap_apb_clk_id_i2c1 : 1; // [2], write clear
        REG32 disable_ap_apb_clk_id_i2c3 : 1; // [3], write clear
        REG32 disable_ap_apb_clk_id_ifc : 1; // [4], write clear
        REG32 disable_ap_apb_clk_id_ifc_ch0 : 1; // [5], write clear
        REG32 disable_ap_apb_clk_id_ifc_ch1 : 1; // [6], write clear
        REG32 disable_ap_apb_clk_id_ifc_ch2 : 1; // [7], write clear
        REG32 disable_ap_apb_clk_id_ifc_ch3 : 1; // [8], write clear
        REG32 disable_ap_apb_clk_id_ifc_ch4 : 1; // [9], write clear
        REG32 disable_ap_apb_clk_id_ifc_ch5 : 1; // [10], write clear
        REG32 disable_ap_apb_clk_id_ifc_ch6 : 1; // [11], write clear
        REG32 disable_ap_apb_clk_id_ifc_chdbg : 1; // [12], write clear
        REG32 disable_ap_apb_clk_id_gouda : 1; // [13], write clear
        REG32 disable_ap_apb_clk_id_sdmmc2 : 1; // [14], write clear
        REG32 disable_ap_apb_clk_id_spi1 : 1; // [15], write clear
        REG32 disable_ap_apb_clk_id_spi2 : 1; // [16], write clear
        REG32 disable_ap_apb_clk_id_scid1 : 1; // [17], write clear
        REG32 disable_ap_apb_clk_id_scid2 : 1; // [18], write clear
        REG32 disable_ap_apb_clk_id_sci1 : 1; // [19], write clear
        REG32 disable_ap_apb_clk_id_sci2 : 1; // [20], write clear
        REG32 disable_ap_apb_clk_id_camera : 1; // [21], write clear
        REG32 disable_ap_apb_clk_id_pagespy : 1; // [22], write clear
        REG32 disable_ap_apb_clk_id_lzma : 1; // [23], write clear
        REG32 disable_ap_apb_clk_id_rev2 : 1; // [24], write clear
        REG32 disable_ap_apb_clk_id_timer1 : 1; // [25], write clear
        REG32 disable_ap_apb_clk_id_timer2 : 1; // [26], write clear
        REG32 disable_ap_apb_clk_id_timer4 : 1; // [27], write clear
        REG32 disable_ap_apb_clk_id_lcd : 1; // [28], write clear
        REG32 disable_ap_apb_clk_id_cqirq : 1; // [29], write clear
        REG32 disable_ap_apb_clk_id_always : 1; // [30], write clear
        REG32 __31_31 : 1;
    } b;
} REG_SYS_CTRL_CLK_AP_APB_DISABLE_T;

//CLK_AIF_APB_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_aif_apb_clk_id_conf : 1; // [0]
        REG32 mode_aif1_apb_clk_id_aif : 1; // [1]
        REG32 mode_aif2_apb_clk_id_aif : 1; // [2]
        REG32 mode_aif_apb_clk_id_ifc : 1; // [3]
        REG32 mode_aif_apb_clk_id_ifc_ch0 : 1; // [4]
        REG32 mode_aif_apb_clk_id_ifc_ch1 : 1; // [5]
        REG32 mode_aif_apb_clk_id_ifc_ch2 : 1; // [6]
        REG32 mode_aif_apb_clk_id_ifc_ch3 : 1; // [7]
        REG32 mode_aif_apb_clk_id_rev0 : 1; // [8]
        REG32 mode_aif_apb_clk_id_rev1 : 1; // [9]
        REG32 mode_aif_apb_clk_id_rev2 : 1; // [10]
        REG32 mode_aif_apb_clk_id_aud_2ad : 1; // [11]
        REG32 __31_12 : 20;
    } b;
} REG_SYS_CTRL_CLK_AIF_APB_MODE_T;

//CLK_AIF_APB_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_aif_apb_clk_id_conf : 1; // [0], write set
        REG32 enable_aif1_apb_clk_id_aif : 1; // [1], write set
        REG32 enable_aif2_apb_clk_id_aif : 1; // [2], write set
        REG32 enable_aif_apb_clk_id_ifc : 1; // [3], write set
        REG32 enable_aif_apb_clk_id_ifc_ch0 : 1; // [4], write set
        REG32 enable_aif_apb_clk_id_ifc_ch1 : 1; // [5], write set
        REG32 enable_aif_apb_clk_id_ifc_ch2 : 1; // [6], write set
        REG32 enable_aif_apb_clk_id_ifc_ch3 : 1; // [7], write set
        REG32 enable_aif_apb_clk_id_rev0 : 1; // [8], write set
        REG32 enable_aif_apb_clk_id_rev1 : 1; // [9], write set
        REG32 enable_aif_apb_clk_id_rev2 : 1; // [10], write set
        REG32 enable_aif_apb_clk_id_aud_2ad : 1; // [11], write set
        REG32 enable_aif_apb_clk_id_always : 1; // [12], write set
        REG32 __31_13 : 19;
    } b;
} REG_SYS_CTRL_CLK_AIF_APB_ENABLE_T;

//CLK_AIF_APB_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_aif_apb_clk_id_conf : 1; // [0], write clear
        REG32 disable_aif1_apb_clk_id_aif : 1; // [1], write clear
        REG32 disable_aif2_apb_clk_id_aif : 1; // [2], write clear
        REG32 disable_aif_apb_clk_id_ifc : 1; // [3], write clear
        REG32 disable_aif_apb_clk_id_ifc_ch0 : 1; // [4], write clear
        REG32 disable_aif_apb_clk_id_ifc_ch1 : 1; // [5], write clear
        REG32 disable_aif_apb_clk_id_ifc_ch2 : 1; // [6], write clear
        REG32 disable_aif_apb_clk_id_ifc_ch3 : 1; // [7], write clear
        REG32 disable_aif_apb_clk_id_rev0 : 1; // [8], write clear
        REG32 disable_aif_apb_clk_id_rev1 : 1; // [9], write clear
        REG32 disable_aif_apb_clk_id_rev2 : 1; // [10], write clear
        REG32 disable_aif_apb_clk_id_aud_2ad : 1; // [11], write clear
        REG32 disable_aif_apb_clk_id_always : 1; // [12], write clear
        REG32 __31_13 : 19;
    } b;
} REG_SYS_CTRL_CLK_AIF_APB_DISABLE_T;

//CLK_AON_AHB_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_aon_ahb_clk_id_bb_sysctrl : 1; // [0]
        REG32 mode_aon_ahb_clk_id_bb_rev0 : 1; // [1]
        REG32 mode_aon_ahb_clk_id_bb_rev1 : 1; // [2]
        REG32 mode_aon_ahb_clk_id_bb_rev2 : 1; // [3]
        REG32 mode_aon_ahb_clk_id_cssys : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CLK_AON_AHB_MODE_T;

//CLK_AON_AHB_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_aon_ahb_clk_id_bb_sysctrl : 1; // [0], write set
        REG32 enable_aon_ahb_clk_id_bb_rev0 : 1; // [1], write set
        REG32 enable_aon_ahb_clk_id_bb_rev1 : 1; // [2], write set
        REG32 enable_aon_ahb_clk_id_bb_rev2 : 1; // [3], write set
        REG32 enable_aon_ahb_clk_id_cssys : 1; // [4], write set
        REG32 enable_aon_ahb_clk_id_always : 1; // [5], write set
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_CLK_AON_AHB_ENABLE_T;

//CLK_AON_AHB_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_aon_ahb_clk_id_bb_sysctrl : 1; // [0], write clear
        REG32 disable_aon_ahb_clk_id_bb_rev0 : 1; // [1], write clear
        REG32 disable_aon_ahb_clk_id_bb_rev1 : 1; // [2], write clear
        REG32 disable_aon_ahb_clk_id_bb_rev2 : 1; // [3], write clear
        REG32 disable_aon_ahb_clk_id_cssys : 1; // [4], write clear
        REG32 disable_aon_ahb_clk_id_always : 1; // [5], write clear
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_CLK_AON_AHB_DISABLE_T;

//CLK_AON_APB_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_aon_apb_clk_id_conf : 1; // [0]
        REG32 mode_aon_apb_clk_id_dbg_host : 1; // [1]
        REG32 mode_aon_apb_clk_id_dbg_uart : 1; // [2]
        REG32 mode_aon_apb_clk_id_ifc : 1; // [3]
        REG32 mode_aon_apb_clk_id_ifc_ch0 : 1; // [4]
        REG32 mode_aon_apb_clk_id_ifc_ch1 : 1; // [5]
        REG32 mode_aon_apb_clk_id_ifc_ch2 : 1; // [6]
        REG32 mode_aon_apb_clk_id_ifc_ch3 : 1; // [7]
        REG32 mode_aon_apb_clk_id_ifc_chdbg : 1; // [8]
        REG32 mode_aon_apb_clk_id_rev0 : 1; // [9]
        REG32 mode_aon_apb_clk_id_rev1 : 1; // [10]
        REG32 mode_aon_apb_clk_id_pwm : 1; // [11]
        REG32 mode_aon_apb_clk_id_i2c2 : 1; // [12]
        REG32 mode_aon_apb_clk_id_ana_reg : 1; // [13]
        REG32 mode_aon_apb_clk_id_spinlock : 1; // [14]
        REG32 mode_aon_apb_clk_id_lps_gsm : 1; // [15]
        REG32 mode_aon_apb_clk_id_lps_nb : 1; // [16]
        REG32 mode_aon_apb_clk_id_efuse : 1; // [17]
        REG32 mode_aon_apb_clk_id_rev2 : 1; // [18]
        REG32 mode_aon_apb_clk_id_vad : 1; // [19]
        REG32 __31_20 : 12;
    } b;
} REG_SYS_CTRL_CLK_AON_APB_MODE_T;

//CLK_AON_APB_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_aon_apb_clk_id_conf : 1; // [0], write set
        REG32 enable_aon_apb_clk_id_dbg_host : 1; // [1], write set
        REG32 enable_aon_apb_clk_id_dbg_uart : 1; // [2], write set
        REG32 enable_aon_apb_clk_id_ifc : 1; // [3], write set
        REG32 enable_aon_apb_clk_id_ifc_ch0 : 1; // [4], write set
        REG32 enable_aon_apb_clk_id_ifc_ch1 : 1; // [5], write set
        REG32 enable_aon_apb_clk_id_ifc_ch2 : 1; // [6], write set
        REG32 enable_aon_apb_clk_id_ifc_ch3 : 1; // [7], write set
        REG32 enable_aon_apb_clk_id_ifc_chdbg : 1; // [8], write set
        REG32 enable_aon_apb_clk_id_rev0 : 1; // [9], write set
        REG32 enable_aon_apb_clk_id_rev1 : 1; // [10], write set
        REG32 enable_aon_apb_clk_id_pwm : 1; // [11], write set
        REG32 enable_aon_apb_clk_id_i2c2 : 1; // [12], write set
        REG32 enable_aon_apb_clk_id_ana_reg : 1; // [13], write set
        REG32 enable_aon_apb_clk_id_spinlock : 1; // [14], write set
        REG32 enable_aon_apb_clk_id_lps_gsm : 1; // [15], write set
        REG32 enable_aon_apb_clk_id_lps_nb : 1; // [16], write set
        REG32 enable_aon_apb_clk_id_efuse : 1; // [17], write set
        REG32 enable_aon_apb_clk_id_rev2 : 1; // [18], write set
        REG32 enable_aon_apb_clk_id_vad : 1; // [19], write set
        REG32 enable_aon_apb_clk_id_rev3 : 1; // [20], write set
        REG32 enable_aon_apb_clk_id_rev4 : 1; // [21], write set
        REG32 enable_aon_apb_clk_id_gpio : 1; // [22], write set
        REG32 enable_aon_apb_clk_id_timer3 : 1; // [23], write set
        REG32 enable_aon_apb_clk_id_keypad : 1; // [24], write set
        REG32 enable_aon_apb_clk_id_ahb2apb_adi : 1; // [25], write set
        REG32 enable_aon_apb_clk_id_adi : 1; // [26], write set
        REG32 enable_aon_apb_clk_id_lvds : 1; // [27], write set
        REG32 enable_aon_apb_clk_id_always : 1; // [28], write set
        REG32 __31_29 : 3;
    } b;
} REG_SYS_CTRL_CLK_AON_APB_ENABLE_T;

//CLK_AON_APB_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_aon_apb_clk_id_conf : 1; // [0], write clear
        REG32 disable_aon_apb_clk_id_dbg_host : 1; // [1], write clear
        REG32 disable_aon_apb_clk_id_dbg_uart : 1; // [2], write clear
        REG32 disable_aon_apb_clk_id_ifc : 1; // [3], write clear
        REG32 disable_aon_apb_clk_id_ifc_ch0 : 1; // [4], write clear
        REG32 disable_aon_apb_clk_id_ifc_ch1 : 1; // [5], write clear
        REG32 disable_aon_apb_clk_id_ifc_ch2 : 1; // [6], write clear
        REG32 disable_aon_apb_clk_id_ifc_ch3 : 1; // [7], write clear
        REG32 disable_aon_apb_clk_id_ifc_chdbg : 1; // [8], write clear
        REG32 disable_aon_apb_clk_id_rev0 : 1; // [9], write clear
        REG32 disable_aon_apb_clk_id_rev1 : 1; // [10], write clear
        REG32 disable_aon_apb_clk_id_pwm : 1; // [11], write clear
        REG32 disable_aon_apb_clk_id_i2c2 : 1; // [12], write clear
        REG32 disable_aon_apb_clk_id_ana_reg : 1; // [13], write clear
        REG32 disable_aon_apb_clk_id_spinlock : 1; // [14], write clear
        REG32 disable_aon_apb_clk_id_lps_gsm : 1; // [15], write clear
        REG32 disable_aon_apb_clk_id_lps_nb : 1; // [16], write clear
        REG32 disable_aon_apb_clk_id_efuse : 1; // [17], write clear
        REG32 disable_aon_apb_clk_id_rev2 : 1; // [18], write clear
        REG32 disable_aon_apb_clk_id_vad : 1; // [19], write clear
        REG32 disable_aon_apb_clk_id_rev3 : 1; // [20], write clear
        REG32 disable_aon_apb_clk_id_rev4 : 1; // [21], write clear
        REG32 disable_aon_apb_clk_id_gpio : 1; // [22], write clear
        REG32 disable_aon_apb_clk_id_timer3 : 1; // [23], write clear
        REG32 disable_aon_apb_clk_id_keypad : 1; // [24], write clear
        REG32 disable_aon_apb_clk_id_ahb2apb_adi : 1; // [25], write clear
        REG32 disable_aon_apb_clk_id_adi : 1; // [26], write clear
        REG32 disable_aon_apb_clk_id_lvds : 1; // [27], write clear
        REG32 disable_aon_apb_clk_id_always : 1; // [28], write clear
        REG32 __31_29 : 3;
    } b;
} REG_SYS_CTRL_CLK_AON_APB_DISABLE_T;

//CLK_RF_AHB_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_clk_rf_ahb : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_SYS_CTRL_CLK_RF_AHB_MODE_T;

//CLK_RF_AHB_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_rf_ahb_clk_id_rev0 : 1; // [0], write set
        REG32 enable_rf_ahb_clk_id_always : 1; // [1], write set
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_CLK_RF_AHB_ENABLE_T;

//CLK_RF_AHB_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_rf_ahb_clk_id_rev0 : 1; // [0], write clear
        REG32 disable_rf_ahb_clk_id_always : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_CLK_RF_AHB_DISABLE_T;

//CLK_RF_APB_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_clk_rf_apb : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_SYS_CTRL_CLK_RF_APB_MODE_T;

//CLK_RF_APB_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_rf_apb_clk_id_rev0 : 1; // [0], write set
        REG32 enable_rf_apb_clk_id_always : 1; // [1], write set
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_CLK_RF_APB_ENABLE_T;

//CLK_RF_APB_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_rf_apb_clk_id_rev0 : 1; // [0], write clear
        REG32 disable_rf_apb_clk_id_always : 1; // [1], write clear
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_CLK_RF_APB_DISABLE_T;

//CLK_OTHERS_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_oc_id_host_uart : 1; // [0]
        REG32 mode_oc_id_bck1 : 1; // [1]
        REG32 mode_oc_id_bck2 : 1; // [2]
        REG32 mode_oc_id_uart1 : 1; // [3]
        REG32 mode_oc_id_uart2 : 1; // [4]
        REG32 mode_oc_id_uart3 : 1; // [5]
        REG32 mode_oc_id_ap_a5 : 1; // [6]
        REG32 mode_oc_id_cp_a5 : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_SYS_CTRL_CLK_OTHERS_MODE_T;

//CLK_OTHERS_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_oc_id_host_uart : 1; // [0], write set
        REG32 enable_oc_id_bck1 : 1; // [1], write set
        REG32 enable_oc_id_bck2 : 1; // [2], write set
        REG32 enable_oc_id_uart1 : 1; // [3], write set
        REG32 enable_oc_id_uart2 : 1; // [4], write set
        REG32 enable_oc_id_uart3 : 1; // [5], write set
        REG32 enable_oc_id_ap_a5 : 1; // [6], write set
        REG32 enable_oc_id_cp_a5 : 1; // [7], write set
        REG32 enable_oc_id_gpio : 1; // [8], write set
        REG32 enable_oc_id_usbphy : 1; // [9], write set
        REG32 enable_oc_id_pix : 1; // [10], write set
        REG32 enable_oc_id_clk_out : 1; // [11], write set
        REG32 enable_oc_id_isp : 1; // [12], write set
        REG32 enable_oc_id_sys_spiflash : 1; // [13], write set
        REG32 enable_oc_id_sys_spiflash_always : 1; // [14], write set
        REG32 enable_oc_id_sys_spiflash1 : 1; // [15], write set
        REG32 enable_oc_id_sys_spiflash1_always : 1; // [16], write set
        REG32 enable_oc_id_spiflash : 1; // [17], write set
        REG32 enable_oc_id_spiflash1 : 1; // [18], write set
        REG32 enable_oc_id_spicam : 1; // [19], write set
        REG32 enable_oc_id_cam : 1; // [20], write set
        REG32 enable_oc_id_psram_conf : 1; // [21], write set
        REG32 enable_oc_id_psram_dmc : 1; // [22], write set
        REG32 enable_oc_id_psram_pagespy : 1; // [23], write set
        REG32 enable_oc_id_psram_always : 1; // [24], write set
        REG32 __31_25 : 7;
    } b;
} REG_SYS_CTRL_CLK_OTHERS_ENABLE_T;

//CLK_OTHERS_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_oc_id_host_uart : 1; // [0], write clear
        REG32 disable_oc_id_bck1 : 1; // [1], write clear
        REG32 disable_oc_id_bck2 : 1; // [2], write clear
        REG32 disable_oc_id_uart1 : 1; // [3], write clear
        REG32 disable_oc_id_uart2 : 1; // [4], write clear
        REG32 disable_oc_id_uart3 : 1; // [5], write clear
        REG32 disable_oc_id_ap_a5 : 1; // [6], write clear
        REG32 disable_oc_id_cp_a5 : 1; // [7], write clear
        REG32 disable_oc_id_gpio : 1; // [8], write clear
        REG32 disable_oc_id_usbphy : 1; // [9], write clear
        REG32 disable_oc_id_pix : 1; // [10], write clear
        REG32 disable_oc_id_clk_out : 1; // [11], write clear
        REG32 disable_oc_id_isp : 1; // [12], write clear
        REG32 disable_oc_id_sys_spiflash : 1; // [13], write clear
        REG32 disable_oc_id_sys_spiflash_always : 1; // [14], write clear
        REG32 disable_oc_id_sys_spiflash1 : 1; // [15], write clear
        REG32 disable_oc_id_sys_spiflash1_always : 1; // [16], write clear
        REG32 disable_oc_id_spiflash : 1; // [17], write clear
        REG32 disable_oc_id_spiflash1 : 1; // [18], write clear
        REG32 disable_oc_id_spicam : 1; // [19], write clear
        REG32 disable_oc_id_cam : 1; // [20], write clear
        REG32 disable_oc_id_psram_conf : 1; // [21], write clear
        REG32 disable_oc_id_psram_dmc : 1; // [22], write clear
        REG32 disable_oc_id_psram_pagespy : 1; // [23], write clear
        REG32 disable_oc_id_psram_always : 1; // [24], write clear
        REG32 __31_25 : 7;
    } b;
} REG_SYS_CTRL_CLK_OTHERS_DISABLE_T;

//CLK_OTHERS_1_MODE
typedef union {
    REG32 v;
    struct {
        REG32 mode_oc_id_gge_clk32k : 1; // [0]
        REG32 mode_oc_id_gge_26m : 1; // [1]
        REG32 mode_oc_id_nb_61p44m : 1; // [2]
        REG32 mode_oc_id_bt_fm_clk32k : 1; // [3]
        REG32 mode_oc_id_bt_fm_26m : 1; // [4]
        REG32 mode_oc_id_pmic_clk32k : 1; // [5]
        REG32 mode_oc_id_pmic_26m : 1; // [6]
        REG32 mode_oc_id_cp_clk32k : 1; // [7]
        REG32 mode_oc_id_cp_26m : 1; // [8]
        REG32 mode_oc_id_zsp_uart : 1; // [9]
        REG32 mode_oc_id_cp_bblte : 1; // [10]
        REG32 mode_oc_id_494m_lte : 1; // [11]
        REG32 mode_oc_id_rev0 : 1; // [12]
        REG32 mode_oc_id_rev1 : 1; // [13]
        REG32 mode_oc_id_rev2 : 1; // [14]
        REG32 mode_oc_id_rev3 : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_SYS_CTRL_CLK_OTHERS_1_MODE_T;

//CLK_OTHERS_1_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 enable_oc_id_gge_clk32k : 1; // [0], write set
        REG32 enable_oc_id_gge_26m : 1; // [1], write set
        REG32 enable_oc_id_nb_61p44m : 1; // [2], write set
        REG32 enable_oc_id_bt_fm_clk32k : 1; // [3], write set
        REG32 enable_oc_id_bt_fm_26m : 1; // [4], write set
        REG32 enable_oc_id_pmic_clk32k : 1; // [5], write set
        REG32 enable_oc_id_pmic_26m : 1; // [6], write set
        REG32 enable_oc_id_cp_clk32k : 1; // [7], write set
        REG32 enable_oc_id_cp_26m : 1; // [8], write set
        REG32 enable_oc_id_zsp_uart : 1; // [9], write set
        REG32 enable_oc_id_cp_bblte : 1; // [10], write set
        REG32 enable_oc_id_494m_lte : 1; // [11], write set
        REG32 enable_oc_id_rev0 : 1; // [12], write set
        REG32 enable_oc_id_rev1 : 1; // [13], write set
        REG32 enable_oc_id_rev2 : 1; // [14], write set
        REG32 enable_oc_id_rev3 : 1; // [15], write set
        REG32 enable_oc_id_sdm_26m : 1; // [16], write set
        REG32 enable_oc_id_lps_gsm : 1; // [17], write set
        REG32 enable_oc_id_lps_nb : 1; // [18], write set
        REG32 enable_oc_id_efuse_26m : 1; // [19], write set
        REG32 enable_oc_id_usb_adp_32k : 1; // [20], write set
        REG32 enable_oc_id_usb_utmi_48m : 1; // [21], write set
        REG32 enable_oc_id_ap_26m : 1; // [22], write set
        REG32 enable_oc_id_ap_32k : 1; // [23], write set
        REG32 enable_oc_id_mipidsi : 1; // [24], write set
        REG32 enable_oc_id_ahb_btfm : 1; // [25], write set
        REG32 enable_oc_id_vad : 1; // [26], write set
        REG32 enable_oc_id_usb11_48m : 1; // [27], write set
        REG32 enable_oc_id_trng_clken : 1; // [28], write set
        REG32 enable_oc_id_coresight : 1; // [29], write set
        REG32 enable_oc_id_adi : 1; // [30], write set
        REG32 __31_31 : 1;
    } b;
} REG_SYS_CTRL_CLK_OTHERS_1_ENABLE_T;

//CLK_OTHERS_1_DISABLE
typedef union {
    REG32 v;
    struct {
        REG32 disable_oc_id_gge_clk32k : 1; // [0], write clear
        REG32 disable_oc_id_gge_26m : 1; // [1], write clear
        REG32 disable_oc_id_nb_61p44m : 1; // [2], write clear
        REG32 disable_oc_id_bt_fm_clk32k : 1; // [3], write clear
        REG32 disable_oc_id_bt_fm_26m : 1; // [4], write clear
        REG32 disable_oc_id_pmic_clk32k : 1; // [5], write clear
        REG32 disable_oc_id_pmic_26m : 1; // [6], write clear
        REG32 disable_oc_id_cp_clk32k : 1; // [7], write clear
        REG32 disable_oc_id_cp_26m : 1; // [8], write clear
        REG32 disable_oc_id_zsp_uart : 1; // [9], write clear
        REG32 disable_oc_id_cp_bblte : 1; // [10], write clear
        REG32 disable_oc_id_494m_lte : 1; // [11], write clear
        REG32 disable_oc_id_rev0 : 1; // [12], write clear
        REG32 disable_oc_id_rev1 : 1; // [13], write clear
        REG32 disable_oc_id_rev2 : 1; // [14], write clear
        REG32 disable_oc_id_rev3 : 1; // [15], write clear
        REG32 disable_oc_id_sdm_26m : 1; // [16], write clear
        REG32 disable_oc_id_lps_gsm : 1; // [17], write clear
        REG32 disable_oc_id_lps_nb : 1; // [18], write clear
        REG32 disable_oc_id_efuse_26m : 1; // [19], write clear
        REG32 disable_oc_id_usb_adp_32k : 1; // [20], write clear
        REG32 disable_oc_id_usb_utmi_48m : 1; // [21], write clear
        REG32 disable_oc_id_ap_26m : 1; // [22], write clear
        REG32 disable_oc_id_ap_32k : 1; // [23], write clear
        REG32 disable_oc_id_mipidsi : 1; // [24], write clear
        REG32 disable_oc_id_ahb_btfm : 1; // [25], write clear
        REG32 disable_oc_id_vad : 1; // [26], write clear
        REG32 disable_oc_id_usb11_48m : 1; // [27], write clear
        REG32 disable_oc_id_trng_clken : 1; // [28], write clear
        REG32 disable_oc_id_coresight : 1; // [29], write clear
        REG32 disable_oc_id_adi : 1; // [30], write clear
        REG32 __31_31 : 1;
    } b;
} REG_SYS_CTRL_CLK_OTHERS_1_DISABLE_T;

//Pll_Ctrl
typedef union {
    REG32 v;
    struct {
        REG32 pll_enable : 1; // [0]
        REG32 __3_1 : 3;
        REG32 pll_lock_reset : 1; // [4]
        REG32 __7_5 : 3;
        REG32 pll_bypass_lock : 1; // [8]
        REG32 __11_9 : 3;
        REG32 pll_clk_fast_enable : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_SYS_CTRL_PLL_CTRL_T;

//Sel_Clock
typedef union {
    REG32 v;
    struct {
        REG32 slow_sel_rf : 1; // [0]
        REG32 sys_sel_fast : 1; // [1]
        REG32 osc_32k_26m_div32k_sel : 1; // [2]
        REG32 pll_disable_lps : 1; // [3]
        REG32 rf_detected : 1; // [4], read only
        REG32 rf_detect_bypass : 1; // [5]
        REG32 rf_detect_reset : 1; // [6]
        REG32 rf_selected_l : 1; // [7], read only
        REG32 pll_locked : 1; // [8], read only
        REG32 fast_selected_l : 1; // [9], read only
        REG32 soft_sel_spiflash : 1; // [10]
        REG32 soft_sel_mem_bridge : 1; // [11]
        REG32 bblte_clk_pll_sel : 1; // [12]
        REG32 camera_clk_pll_sel : 1; // [13]
        REG32 usb_pll_locked_h : 1; // [14], read only
        REG32 bb26m_sel : 1; // [15]
        REG32 soft_sel_spiflash1 : 1; // [16]
        REG32 apll_locked_h : 1; // [17], read only
        REG32 mempll_locked_h : 1; // [18], read only
        REG32 audiopll_locked_h : 1; // [19], read only
        REG32 bbpll2_locked_h : 1; // [20], read only
        REG32 bbpll1_locked_h : 1; // [21], read only
        REG32 usbpll_locked_h : 1; // [22], read only
        REG32 __31_23 : 9;
    } b;
} REG_SYS_CTRL_SEL_CLOCK_T;

//Cfg_Clk_Out
typedef union {
    REG32 v;
    struct {
        REG32 clkout_divider : 5; // [4:0]
        REG32 clkout_dbg_sel : 1; // [5]
        REG32 clkout_enable : 1; // [6]
        REG32 clkout_update : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_SYS_CTRL_CFG_CLK_OUT_T;

//Cfg_Clk_AudioBCK1_Div
typedef union {
    REG32 v;
    struct {
        REG32 audiobck1_divider : 11; // [10:0]
        REG32 audiobck1_update : 1; // [11]
        REG32 __31_12 : 20;
    } b;
} REG_SYS_CTRL_CFG_CLK_AUDIOBCK1_DIV_T;

//Cfg_Clk_AudioBCK2_Div
typedef union {
    REG32 v;
    struct {
        REG32 audiobck2_divider : 11; // [10:0]
        REG32 audiobck2_update : 1; // [11]
        REG32 __31_12 : 20;
    } b;
} REG_SYS_CTRL_CFG_CLK_AUDIOBCK2_DIV_T;

//Cfg_Clk_Uart
typedef union {
    REG32 v;
    struct {
        REG32 uart_divider : 24; // [23:0]
        REG32 uart_divider_update : 1; // [24]
        REG32 uart_sel_pll : 1; // [25]
        REG32 __31_26 : 6;
    } b;
} REG_SYS_CTRL_CFG_CLK_UART_T;

//Cfg_Clk_PWM
typedef union {
    REG32 v;
    struct {
        REG32 pwm_divider : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_SYS_CTRL_CFG_CLK_PWM_T;

//Cfg_Clk_Dbg_Div
typedef union {
    REG32 v;
    struct {
        REG32 clk_dbg_divider : 11; // [10:0]
        REG32 __31_11 : 21;
    } b;
} REG_SYS_CTRL_CFG_CLK_DBG_DIV_T;

//Cfg_Clk_Camera_Out
typedef union {
    REG32 v;
    struct {
        REG32 clk_camera_out_en : 1; // [0]
        REG32 clk_camera_div_src_sel : 1; // [1]
        REG32 clk_camera_out_div : 11; // [12:2]
        REG32 clk_camera_div_update : 1; // [13]
        REG32 clk_spi_cam_en : 1; // [14]
        REG32 clk_spi_cam_pol : 1; // [15]
        REG32 clk_spi_cam_sel : 1; // [16]
        REG32 clk_spi_cam_div : 11; // [27:17]
        REG32 clk_spi_cam_div_update : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_SYS_CTRL_CFG_CLK_CAMERA_OUT_T;

//Reset_Cause
typedef union {
    REG32 v;
    struct {
        REG32 watchdog_reset_1 : 1; // [0]
        REG32 watchdog_reset_2 : 1; // [1]
        REG32 watchdog_reset_3 : 1; // [2]
        REG32 watchdog_reset_4 : 1; // [3]
        REG32 watchdog_reset_rf : 1; // [4]
        REG32 watchdog_reset_gge : 1; // [5]
        REG32 watchdog_reset_zsp : 1; // [6]
        REG32 globalsoft_reset : 1; // [7], read only
        REG32 hostdebug_reset : 1; // [8], read only
        REG32 watchdog_reset_cp : 1; // [9]
        REG32 __11_10 : 2;
        REG32 alarmcause : 1; // [12], read only
        REG32 __15_13 : 3;
        REG32 boot_mode : 6; // [21:16]
        REG32 sw_boot_mode : 6; // [27:22]
        REG32 __30_28 : 3;
        REG32 fonctional_test_mode : 1; // [31], read only
    } b;
} REG_SYS_CTRL_RESET_CAUSE_T;

//WakeUp
typedef union {
    REG32 v;
    struct {
        REG32 force_wakeup : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_SYS_CTRL_WAKEUP_T;

//Ignore_Charger
typedef union {
    REG32 v;
    struct {
        REG32 ignore_charger : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_SYS_CTRL_IGNORE_CHARGER_T;

//CFG_PLL_SPIFLASH_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_spiflash_div : 4; // [3:0]
        REG32 cfg_pll_spiflash_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_SPIFLASH_DIV_T;

//CFG_PLL_SPIFLASH1_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_spiflash1_div : 4; // [3:0]
        REG32 cfg_pll_spiflash1_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_SPIFLASH1_DIV_T;

//CFG_PLL_MEM_BRIDGE_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_mem_bridge_div : 4; // [3:0]
        REG32 cfg_pll_mem_bridge_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_MEM_BRIDGE_DIV_T;

//CFG_DBG_CLK_SOURCE_SEL
typedef union {
    REG32 v;
    struct {
        REG32 cfg_dbg_clk_source_sel : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_CFG_DBG_CLK_SOURCE_SEL_T;

//CFG_CLK_494M_LTE_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_clk_494m_lte_apll_div : 4; // [3:0]
        REG32 cfg_clk_494m_lte_apll_div_update : 1; // [4]
        REG32 cfg_clksw_494m_lte_usbphy_sel : 1; // [5]
        REG32 cfg_clksw_494m_clk_sel : 1; // [6]
        REG32 cfg_clk_96m_26m_sel : 1; // [7]
        REG32 cfg_clk_96m_div : 23; // [30:8]
        REG32 cfg_clk_96m_div_update : 1; // [31]
    } b;
} REG_SYS_CTRL_CFG_CLK_494M_LTE_DIV_T;

//CFG_PLL_ISP_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_isp_div : 12; // [11:0]
        REG32 cfg_pll_isp_div_update : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_SYS_CTRL_CFG_PLL_ISP_DIV_T;

//CFG_PLL_PIX_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_pix_div : 7; // [6:0]
        REG32 cfg_pll_pix_div_update : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_SYS_CTRL_CFG_PLL_PIX_DIV_T;

//CFG_PLL_SYS_AXI_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_sys_axi_div : 4; // [3:0]
        REG32 cfg_pll_sys_axi_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_SYS_AXI_DIV_T;

//CFG_PLL_SYS_AHB_APB_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_ap_apb_div : 4; // [3:0]
        REG32 cfg_pll_ap_apb_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_SYS_AHB_APB_DIV_T;

//CFG_PLL_SYS_AHB_BTFM_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_ahb_btfm_div : 4; // [3:0]
        REG32 cfg_pll_ahb_btfm_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_SYS_AHB_BTFM_DIV_T;

//CFG_PLL_CSI_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_csi_div : 7; // [6:0]
        REG32 cfg_pll_csi_div_update : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_SYS_CTRL_CFG_PLL_CSI_DIV_T;

//CFG_PLL_SYS_SPIFLASH_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_sys_spiflash_div : 4; // [3:0]
        REG32 cfg_pll_sys_spiflash_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_SYS_SPIFLASH_DIV_T;

//CFG_PLL_CP_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_cp_div : 4; // [3:0]
        REG32 cfg_pll_cp_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_CP_DIV_T;

//CFG_PLL_AP_CPU_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_ap_cpu_div : 4; // [3:0]
        REG32 cfg_pll_ap_cpu_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_AP_CPU_DIV_T;

//CFG_AP_CPU_ACLKEN_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_ap_cpu_aclken_div : 3; // [2:0]
        REG32 cfg_ap_cpu_aclken_div_update : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_SYS_CTRL_CFG_AP_CPU_ACLKEN_DIV_T;

//CFG_AP_CPU_DBGEN_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_ap_cpu_dbgen_div : 3; // [2:0]
        REG32 cfg_ap_cpu_dbgen_div_update : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_SYS_CTRL_CFG_AP_CPU_DBGEN_DIV_T;

//CFG_PLL_CP_CPU_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_cp_cpu_div : 4; // [3:0]
        REG32 cfg_pll_cp_cpu_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_PLL_CP_CPU_DIV_T;

//CFG_CP_CPU_ACLKEN_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_cp_cpu_aclken_div : 3; // [2:0]
        REG32 cfg_cp_cpu_aclken_div_update : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_SYS_CTRL_CFG_CP_CPU_ACLKEN_DIV_T;

//CFG_CP_CPU_DBGEN_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_cp_cpu_dbgen_div : 3; // [2:0]
        REG32 cfg_cp_cpu_dbgen_div_update : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_SYS_CTRL_CFG_CP_CPU_DBGEN_DIV_T;

//CFG_TRNG_CLKEN_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_trng_clken_div : 5; // [4:0]
        REG32 cfg_trng_clken_div_update : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_CFG_TRNG_CLKEN_DIV_T;

//CFG_CORESIGHT_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_coresight_div : 4; // [3:0]
        REG32 cfg_coresight_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_CORESIGHT_DIV_T;

//CFG_USB11_48M_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_usb11_48m_div : 5; // [4:0]
        REG32 cfg_usb11_48m_div_update : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_SYS_CTRL_CFG_USB11_48M_DIV_T;

//CFG_PLL_USB
typedef union {
    REG32 v;
    struct {
        REG32 cfg_pll_usb_reset : 1; // [0]
        REG32 cfg_pll_usb_rev : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_SYS_CTRL_CFG_PLL_USB_T;

//CFG_VAD_DIV
typedef union {
    REG32 v;
    struct {
        REG32 cfg_vad_div : 4; // [3:0]
        REG32 cfg_vad_div_update : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_SYS_CTRL_CFG_VAD_DIV_T;

//Cfg_HMPROT
typedef union {
    REG32 v;
    struct {
        REG32 hprot_bbdma : 4; // [3:0]
        REG32 hprot_aif_ifc : 4; // [7:4]
        REG32 hprot_gouda : 4; // [11:8]
        REG32 hprot_ap_ifc : 4; // [15:12]
        REG32 hprot_usb : 4; // [19:16]
        REG32 hprot_sys_aon_ifc : 4; // [23:20]
        REG32 hsprot_psram_ctrl : 4; // [27:24]
        REG32 __31_28 : 4;
    } b;
} REG_SYS_CTRL_CFG_HMPROT_T;

//Cfg_AIF_CFG
typedef union {
    REG32 v;
    struct {
        REG32 aif1_load_pos : 6; // [5:0]
        REG32 aif2_load_pos : 6; // [11:6]
        REG32 aif1_sel : 3; // [14:12]
        REG32 aif2_sel : 3; // [17:15]
        REG32 i2s1_sel : 3; // [20:18]
        REG32 i2s2_sel : 3; // [23:21]
        REG32 i2s3_sel : 3; // [26:24]
        REG32 i2s1_bck_lrck_oen : 1; // [27]
        REG32 i2s2_bck_lrck_oen : 1; // [28]
        REG32 i2s3_bck_lrck_oen : 1; // [29]
        REG32 __31_30 : 2;
    } b;
} REG_SYS_CTRL_CFG_AIF_CFG_T;

//Cfg_MISC_CFG
typedef union {
    REG32 v;
    struct {
        REG32 limit_en_spi1 : 1; // [0]
        REG32 limit_en_spi2 : 1; // [1]
        REG32 limit_en_spi3 : 1; // [2]
        REG32 cfgsdisable_gic400 : 1; // [3]
        REG32 aif1_i2s_bck_sel : 3; // [6:4]
        REG32 __7_7 : 1;
        REG32 aif2_i2s_bck_sel : 3; // [10:8]
        REG32 wcn_uart_out_sel : 1; // [11]
        REG32 ap_uart_out_sel : 1; // [12]
        REG32 cfg_mode_lp : 1; // [13]
        REG32 cfg_force_lp : 1; // [14]
        REG32 cfg_number_lp : 16; // [30:15]
        REG32 bcpu_stall_ack : 1; // [31], read only
    } b;
} REG_SYS_CTRL_CFG_MISC_CFG_T;

//Cfg_MISC1_CFG
typedef union {
    REG32 v;
    struct {
        REG32 debug_mon_sel : 5; // [4:0]
        REG32 iomux_clk_force_on : 1; // [5]
        REG32 ap_rst_ctrl : 1; // [6]
        REG32 gge_rst_ctrl : 1; // [7]
        REG32 btfm_rst_ctrl : 1; // [8]
        REG32 ap_clk_ctrl : 1; // [9]
        REG32 gge_clk_ctrl : 1; // [10]
        REG32 btfm_clk_ctrl : 1; // [11]
        REG32 bbpll1_enable : 1; // [12]
        REG32 bbpll2_enable : 1; // [13]
        REG32 mempll_enable : 1; // [14]
        REG32 usbpll_enable : 1; // [15]
        REG32 audiopll_enable : 1; // [16]
        REG32 apll_zsp_clk_sel : 1; // [17]
        REG32 apll_bbpll2_clk_sel : 1; // [18]
        REG32 reg_gic400_aruser_sel : 1; // [19]
        REG32 reg_gic400_aruser_dbg : 1; // [20]
        REG32 reg_gic400_awuser_sel : 1; // [21]
        REG32 reg_gic400_awuser_dbg : 1; // [22]
        REG32 lvds_wcn_rfdig_sel : 1; // [23]
        REG32 lvds_rfdig_rf_bb_sel : 1; // [24]
        REG32 wcn_osc_en_ctrl : 1; // [25]
        REG32 vad_clk_force_on : 1; // [26]
        REG32 vad_clk_pn_sel : 1; // [27]
        REG32 aud_sclk_o_pn_sel : 1; // [28]
        REG32 aon_lp_rst_ctrl : 1; // [29]
        REG32 aon_lp_clk_ctrl : 1; // [30]
        REG32 dump_path : 1; // [31]
    } b;
} REG_SYS_CTRL_CFG_MISC1_CFG_T;

//Cfg_FORCE_LP_MODE_LP
typedef union {
    REG32 v;
    struct {
        REG32 cfg_force_lp_a5 : 1; // [0]
        REG32 cfg_force_lp_ahb : 1; // [1]
        REG32 cfg_force_lp_connect : 1; // [2]
        REG32 cfg_force_lp_cp_a5 : 1; // [3]
        REG32 cfg_force_lp_psram : 1; // [4]
        REG32 cfg_force_lp_spiflash : 1; // [5]
        REG32 cfg_force_lp_spiflash1 : 1; // [6]
        REG32 cfg_mode_lp_a5 : 1; // [7]
        REG32 cfg_mode_lp_ahb : 1; // [8]
        REG32 cfg_mode_lp_connect : 1; // [9]
        REG32 cfg_mode_lp_cp_a5 : 1; // [10]
        REG32 cfg_mode_lp_psram : 1; // [11]
        REG32 cfg_mode_lp_spiflash : 1; // [12]
        REG32 cfg_mode_lp_spiflash1 : 1; // [13]
        REG32 cfg_mode_lp_ahb_merge : 1; // [14]
        REG32 force_lp_ahb_merge : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_SYS_CTRL_CFG_FORCE_LP_MODE_LP_T;

//Cfg_Reserve
typedef union {
    REG32 v;
    struct {
        REG32 wd_rst_mode : 1; // [0]
        REG32 sys_bb_side : 1; // [1]
        REG32 cam_out0_sel : 1; // [2]
        REG32 vpu_clk_en : 1; // [3]
        REG32 wd_1_rst_en : 1; // [4]
        REG32 wd_2_rst_en : 1; // [5]
        REG32 wd_3_rst_en : 1; // [6]
        REG32 wd_4_rst_en : 1; // [7]
        REG32 wd_rf_rst_en : 1; // [8]
        REG32 wd_gge_rst_en : 1; // [9]
        REG32 wd_zsp_rst_en : 1; // [10]
        REG32 wd_cp_rst_en : 1; // [11]
        REG32 dmc_phy_rst_en : 1; // [12]
        REG32 hmprot_wcn_peri : 4; // [16:13]
        REG32 hmprot_wcn_mem : 4; // [20:17]
        REG32 hmprot_aes : 4; // [24:21]
        REG32 reserve : 7; // [31:25]
    } b;
} REG_SYS_CTRL_CFG_RESERVE_T;

//Cfg_CHIP_PROD_ID
typedef union {
    REG32 v;
    struct {
        REG32 metal_id : 12; // [11:0], read only
        REG32 bond_id : 4; // [15:12], read only
        REG32 prod_id : 16; // [31:16], read only
    } b;
} REG_SYS_CTRL_CFG_CHIP_PROD_ID_T;

//Cfg_QOS_WCN_A5_GGE
typedef union {
    REG32 v;
    struct {
        REG32 wcn_mem_arqos : 5; // [4:0]
        REG32 wcn_mem_awqos : 5; // [9:5]
        REG32 gge_arqos : 5; // [14:10]
        REG32 gge_awqos : 5; // [19:15]
        REG32 a5_arqos : 5; // [24:20]
        REG32 a5_awqos : 5; // [29:25]
        REG32 __31_30 : 2;
    } b;
} REG_SYS_CTRL_CFG_QOS_WCN_A5_GGE_T;

//Cfg_QOS_AXIDMA_CPA5_F8
typedef union {
    REG32 v;
    struct {
        REG32 axidma_arqos : 5; // [4:0]
        REG32 axidma_awqos : 5; // [9:5]
        REG32 cp_a5_arqos : 5; // [14:10]
        REG32 cp_a5_awqos : 5; // [19:15]
        REG32 f8_arqos : 5; // [24:20]
        REG32 f8_awqos : 5; // [29:25]
        REG32 __31_30 : 2;
    } b;
} REG_SYS_CTRL_CFG_QOS_AXIDMA_CPA5_F8_T;

//Cfg_QOS_LCDC_LZMA_GOUDA
typedef union {
    REG32 v;
    struct {
        REG32 lcdc_arqos : 5; // [4:0]
        REG32 lcdc_awqos : 5; // [9:5]
        REG32 lzma_arqos : 5; // [14:10]
        REG32 lzma_awqos : 5; // [19:15]
        REG32 gouda_arqos : 5; // [24:20]
        REG32 gouda_awqos : 5; // [29:25]
        REG32 __31_30 : 2;
    } b;
} REG_SYS_CTRL_CFG_QOS_LCDC_LZMA_GOUDA_T;

//Cfg_QOS_LTE_USB
typedef union {
    REG32 v;
    struct {
        REG32 lte_arqos : 5; // [4:0]
        REG32 lte_awqos : 5; // [9:5]
        REG32 usb_arqos : 5; // [14:10]
        REG32 usb_awqos : 5; // [19:15]
        REG32 __31_20 : 12;
    } b;
} REG_SYS_CTRL_CFG_QOS_LTE_USB_T;

//Cfg_QOS_MERGE_MEM
typedef union {
    REG32 v;
    struct {
        REG32 merge_mem_awqos : 5; // [4:0]
        REG32 merge_mem_arqos : 5; // [9:5]
        REG32 __31_10 : 22;
    } b;
} REG_SYS_CTRL_CFG_QOS_MERGE_MEM_T;

//Cfg_BCPU_DBG_BKP
typedef union {
    REG32 v;
    struct {
        REG32 bcpu_bkpt_addr : 28; // [27:0]
        REG32 bcpu_bkpt_mode : 2; // [29:28]
        REG32 bcpu_bkpt_en : 1; // [30]
        REG32 bcpu_stalled_w1c : 1; // [31]
    } b;
} REG_SYS_CTRL_CFG_BCPU_DBG_BKP_T;


//REG_DBG
#define SYS_CTRL_SCRATCH(n)        (((n)&0xFFFF)<<0)
#define SYS_CTRL_WRITE_UNLOCK_STATUS (1<<30)
#define SYS_CTRL_WRITE_UNLOCK      (1<<31)

//SYS_AXI_RST_SET
#define SYS_CTRL_SET_SYS_AXI_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_SYS_AXI_RST_ID_IMEM (1<<1)
#define SYS_CTRL_SET_SYS_AXI_RST_ID_LZMA (1<<2)
#define SYS_CTRL_SET_SYS_AXI_RST_ID_REV0 (1<<3)
#define SYS_CTRL_SET_SYS_AXI_RST_ID_REV1 (1<<4)
#define SYS_CTRL_SET_SYS_AXI_RST_ID_REV2 (1<<5)
#define SYS_CTRL_SET_SYS_AXI_RST_ID_REV3 (1<<6)
#define SYS_CTRL_SET_SYS_AXI_RST(n) (((n)&0x7F)<<0)
#define SYS_CTRL_SET_SYS_AXI_RST_MASK (0x7F<<0)
#define SYS_CTRL_SET_SYS_AXI_RST_SHIFT (0)

//SYS_AXI_RST_CLR
#define SYS_CTRL_CLR_SYS_AXI_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_SYS_AXI_RST_ID_IMEM (1<<1)
#define SYS_CTRL_CLR_SYS_AXI_RST_ID_LZMA (1<<2)
#define SYS_CTRL_CLR_SYS_AXI_RST_ID_REV0 (1<<3)
#define SYS_CTRL_CLR_SYS_AXI_RST_ID_REV1 (1<<4)
#define SYS_CTRL_CLR_SYS_AXI_RST_ID_REV2 (1<<5)
#define SYS_CTRL_CLR_SYS_AXI_RST_ID_REV3 (1<<6)
#define SYS_CTRL_CLR_SYS_AXI_RST(n) (((n)&0x7F)<<0)
#define SYS_CTRL_CLR_SYS_AXI_RST_MASK (0x7F<<0)
#define SYS_CTRL_CLR_SYS_AXI_RST_SHIFT (0)

//SYS_AHB_RST_SET
#define SYS_CTRL_SET_SYS_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_GOUDA (1<<1)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_GGE (1<<2)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_GEA3 (1<<3)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_USBC (1<<4)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_BTFM (1<<5)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_GIC400 (1<<6)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_F8 (1<<7)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_AXIDMA (1<<8)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_LZMA (1<<9)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_LCD (1<<10)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_AES (1<<11)
#define SYS_CTRL_SET_SYS_AHB_RST_ID_USB11 (1<<12)
#define SYS_CTRL_SET_SYS_AHB_RST(n) (((n)&0x1FFF)<<0)
#define SYS_CTRL_SET_SYS_AHB_RST_MASK (0x1FFF<<0)
#define SYS_CTRL_SET_SYS_AHB_RST_SHIFT (0)

//SYS_AHB_RST_CLR
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_GOUDA (1<<1)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_GGE (1<<2)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_GEA3 (1<<3)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_USBC (1<<4)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_BTFM (1<<5)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_GIC400 (1<<6)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_F8 (1<<7)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_AXIDMA (1<<8)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_LZMA (1<<9)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_LCD (1<<10)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_AES (1<<11)
#define SYS_CTRL_CLR_SYS_AHB_RST_ID_USB11 (1<<12)
#define SYS_CTRL_CLR_SYS_AHB_RST(n) (((n)&0x1FFF)<<0)
#define SYS_CTRL_CLR_SYS_AHB_RST_MASK (0x1FFF<<0)
#define SYS_CTRL_CLR_SYS_AHB_RST_SHIFT (0)

//AP_APB_RST_SET
#define SYS_CTRL_SET_AP_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_AP_APB_RST_ID_CAMERA (1<<1)
#define SYS_CTRL_SET_AP_APB_RST_ID_I2C1 (1<<2)
#define SYS_CTRL_SET_AP_APB_RST_ID_I2C3 (1<<3)
#define SYS_CTRL_SET_AP_APB_RST_ID_IFC (1<<4)
#define SYS_CTRL_SET_AP_APB_RST_ID_IMEM (1<<5)
#define SYS_CTRL_SET_AP_APB_RST_ID_SDMMC1 (1<<6)
#define SYS_CTRL_SET_AP_APB_RST_ID_SDMMC2 (1<<7)
#define SYS_CTRL_SET_AP_APB_RST_ID_SPI1 (1<<8)
#define SYS_CTRL_SET_AP_APB_RST_ID_SPI2 (1<<9)
#define SYS_CTRL_SET_AP_APB_RST_ID_UART2 (1<<10)
#define SYS_CTRL_SET_AP_APB_RST_ID_UART3 (1<<11)
#define SYS_CTRL_SET_AP_APB_RST_ID_ZSP_UART (1<<12)
#define SYS_CTRL_SET_AP_APB_RST_ID_REV0 (1<<13)
#define SYS_CTRL_SET_AP_APB_RST_ID_REV1 (1<<14)
#define SYS_CTRL_SET_AP_APB_RST_ID_SCI1 (1<<15)
#define SYS_CTRL_SET_AP_APB_RST_ID_SCI2 (1<<16)
#define SYS_CTRL_SET_AP_APB_RST_ID_PAGESPY (1<<17)
#define SYS_CTRL_SET_AP_APB_RST_ID_LZMA (1<<18)
#define SYS_CTRL_SET_AP_APB_RST_ID_PSRAM (1<<19)
#define SYS_CTRL_SET_AP_APB_RST_ID_TIMER1 (1<<20)
#define SYS_CTRL_SET_AP_APB_RST_ID_TIMER2 (1<<21)
#define SYS_CTRL_SET_AP_APB_RST_ID_TIMER4 (1<<22)
#define SYS_CTRL_SET_AP_APB_RST_ID_GOUDA (1<<23)
#define SYS_CTRL_SET_AP_APB_RST_ID_CQIRQ (1<<24)
#define SYS_CTRL_SET_AP_APB_RST(n) (((n)&0x1FFFFFF)<<0)
#define SYS_CTRL_SET_AP_APB_RST_MASK (0x1FFFFFF<<0)
#define SYS_CTRL_SET_AP_APB_RST_SHIFT (0)

//AP_APB_RST_CLR
#define SYS_CTRL_CLR_AP_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_AP_APB_RST_ID_CAMERA (1<<1)
#define SYS_CTRL_CLR_AP_APB_RST_ID_I2C1 (1<<2)
#define SYS_CTRL_CLR_AP_APB_RST_ID_I2C3 (1<<3)
#define SYS_CTRL_CLR_AP_APB_RST_ID_IFC (1<<4)
#define SYS_CTRL_CLR_AP_APB_RST_ID_IMEM (1<<5)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC1 (1<<6)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC2 (1<<7)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SPI1 (1<<8)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SPI2 (1<<9)
#define SYS_CTRL_CLR_AP_APB_RST_ID_UART2 (1<<10)
#define SYS_CTRL_CLR_AP_APB_RST_ID_UART3 (1<<11)
#define SYS_CTRL_CLR_AP_APB_RST_ID_ZSP_UART (1<<12)
#define SYS_CTRL_CLR_AP_APB_RST_ID_REV0 (1<<13)
#define SYS_CTRL_CLR_AP_APB_RST_ID_REV1 (1<<14)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SCI1 (1<<15)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SCI2 (1<<16)
#define SYS_CTRL_CLR_AP_APB_RST_ID_PAGESPY (1<<17)
#define SYS_CTRL_CLR_AP_APB_RST_ID_LZMA (1<<18)
#define SYS_CTRL_CLR_AP_APB_RST_ID_PSRAM (1<<19)
#define SYS_CTRL_CLR_AP_APB_RST_ID_TIMER1 (1<<20)
#define SYS_CTRL_CLR_AP_APB_RST_ID_TIMER2 (1<<21)
#define SYS_CTRL_CLR_AP_APB_RST_ID_TIMER4 (1<<22)
#define SYS_CTRL_CLR_AP_APB_RST_ID_GOUDA (1<<23)
#define SYS_CTRL_CLR_AP_APB_RST_ID_CQIRQ (1<<24)
#define SYS_CTRL_CLR_AP_APB_RST(n) (((n)&0x1FFFFFF)<<0)
#define SYS_CTRL_CLR_AP_APB_RST_MASK (0x1FFFFFF<<0)
#define SYS_CTRL_CLR_AP_APB_RST_SHIFT (0)

//AIF_APB_RST_SET
#define SYS_CTRL_SET_AIF1_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_AIF2_APB_RST_ID_SYS (1<<1)
#define SYS_CTRL_SET_AIF1_APB_RST_ID_AIF (1<<2)
#define SYS_CTRL_SET_AIF2_APB_RST_ID_AIF (1<<3)
#define SYS_CTRL_SET_AIF_APB_RST_ID_IFC (1<<4)
#define SYS_CTRL_SET_AIF_APB_RST_ID_AUD_2AD (1<<5)
#define SYS_CTRL_SET_AIF_APB_RST(n) (((n)&0x3F)<<0)
#define SYS_CTRL_SET_AIF_APB_RST_MASK (0x3F<<0)
#define SYS_CTRL_SET_AIF_APB_RST_SHIFT (0)

//AIF_APB_RST_CLR
#define SYS_CTRL_CLR_AIF1_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_AIF2_APB_RST_ID_SYS (1<<1)
#define SYS_CTRL_CLR_AIF1_APB_RST_ID_AIF (1<<2)
#define SYS_CTRL_CLR_AIF2_APB_RST_ID_AIF (1<<3)
#define SYS_CTRL_CLR_AIF_APB_RST_ID_IFC (1<<4)
#define SYS_CTRL_CLR_AIF_APB_RST_ID_AUD_2AD (1<<5)
#define SYS_CTRL_CLR_AIF_APB_RST(n) (((n)&0x3F)<<0)
#define SYS_CTRL_CLR_AIF_APB_RST_MASK (0x3F<<0)
#define SYS_CTRL_CLR_AIF_APB_RST_SHIFT (0)

//AON_AHB_RST_SET
#define SYS_CTRL_SET_AON_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_AON_AHB_RST_ID_REV0 (1<<1)
#define SYS_CTRL_SET_AON_AHB_RST_ID_REV1 (1<<2)
#define SYS_CTRL_SET_AON_AHB_RST_ID_REV2 (1<<3)
#define SYS_CTRL_SET_AON_AHB_RST_ID_CSSYS (1<<4)
#define SYS_CTRL_SET_AON_AHB_RST(n) (((n)&31)<<0)
#define SYS_CTRL_SET_AON_AHB_RST_MASK (31<<0)
#define SYS_CTRL_SET_AON_AHB_RST_SHIFT (0)

//AON_AHB_RST_CLR
#define SYS_CTRL_CLR_AON_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_REV0 (1<<1)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_REV1 (1<<2)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_REV2 (1<<3)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_CSSYS (1<<4)
#define SYS_CTRL_CLR_AON_AHB_RST(n) (((n)&31)<<0)
#define SYS_CTRL_CLR_AON_AHB_RST_MASK (31<<0)
#define SYS_CTRL_CLR_AON_AHB_RST_SHIFT (0)

//AON_APB_RST_SET
#define SYS_CTRL_SET_AON_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_AON_APB_RST_ID_CALENDAR (1<<1)
#define SYS_CTRL_SET_AON_APB_RST_ID_GPIO (1<<2)
#define SYS_CTRL_SET_AON_APB_RST_ID_IFC (1<<3)
#define SYS_CTRL_SET_AON_APB_RST_ID_KEYPAD (1<<4)
#define SYS_CTRL_SET_AON_APB_RST_ID_PWM (1<<5)
#define SYS_CTRL_SET_AON_APB_RST_ID_REV0 (1<<6)
#define SYS_CTRL_SET_AON_APB_RST_ID_REV1 (1<<7)
#define SYS_CTRL_SET_AON_APB_RST_ID_TIMER3 (1<<8)
#define SYS_CTRL_SET_AON_APB_RST_ID_I2C2 (1<<9)
#define SYS_CTRL_SET_AON_APB_RST_ID_ANA_REG (1<<10)
#define SYS_CTRL_SET_AON_APB_RST_ID_SPINLOCK (1<<11)
#define SYS_CTRL_SET_AON_APB_RST_ID_LPS_GSM (1<<12)
#define SYS_CTRL_SET_AON_APB_RST_ID_LPS_NB (1<<13)
#define SYS_CTRL_SET_AON_APB_RST_ID_EFUSE (1<<14)
#define SYS_CTRL_SET_AON_APB_RST_ID_AHB2APB_ADI (1<<15)
#define SYS_CTRL_SET_AON_APB_RST_ID_ADI (1<<16)
#define SYS_CTRL_SET_AON_APB_RST_ID_LVDS (1<<17)
#define SYS_CTRL_SET_AON_APB_RST_ID_UART1 (1<<18)
#define SYS_CTRL_SET_AON_APB_RST_ID_VAD (1<<19)
#define SYS_CTRL_SET_AON_APB_RST(n) (((n)&0xFFFFF)<<0)
#define SYS_CTRL_SET_AON_APB_RST_MASK (0xFFFFF<<0)
#define SYS_CTRL_SET_AON_APB_RST_SHIFT (0)

//AON_APB_RST_CLR
#define SYS_CTRL_CLR_AON_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_AON_APB_RST_ID_CALENDAR (1<<1)
#define SYS_CTRL_CLR_AON_APB_RST_ID_GPIO (1<<2)
#define SYS_CTRL_CLR_AON_APB_RST_ID_IFC (1<<3)
#define SYS_CTRL_CLR_AON_APB_RST_ID_KEYPAD (1<<4)
#define SYS_CTRL_CLR_AON_APB_RST_ID_PWM (1<<5)
#define SYS_CTRL_CLR_AON_APB_RST_ID_REV0 (1<<6)
#define SYS_CTRL_CLR_AON_APB_RST_ID_REV1 (1<<7)
#define SYS_CTRL_CLR_AON_APB_RST_ID_TIMER3 (1<<8)
#define SYS_CTRL_CLR_AON_APB_RST_ID_I2C2 (1<<9)
#define SYS_CTRL_CLR_AON_APB_RST_ID_ANA_REG (1<<10)
#define SYS_CTRL_CLR_AON_APB_RST_ID_SPINLOCK (1<<11)
#define SYS_CTRL_CLR_AON_APB_RST_ID_LPS_GSM (1<<12)
#define SYS_CTRL_CLR_AON_APB_RST_ID_LPS_NB (1<<13)
#define SYS_CTRL_CLR_AON_APB_RST_ID_EFUSE (1<<14)
#define SYS_CTRL_CLR_AON_APB_RST_ID_AHB2APB_ADI (1<<15)
#define SYS_CTRL_CLR_AON_APB_RST_ID_ADI (1<<16)
#define SYS_CTRL_CLR_AON_APB_RST_ID_LVDS (1<<17)
#define SYS_CTRL_CLR_AON_APB_RST_ID_UART1 (1<<18)
#define SYS_CTRL_CLR_AON_APB_RST_ID_VAD (1<<19)
#define SYS_CTRL_CLR_AON_APB_RST(n) (((n)&0xFFFFF)<<0)
#define SYS_CTRL_CLR_AON_APB_RST_MASK (0xFFFFF<<0)
#define SYS_CTRL_CLR_AON_APB_RST_SHIFT (0)

//RF_AHB_RST_SET
#define SYS_CTRL_SET_RF_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RF_REV0 (1<<1)
#define SYS_CTRL_SET_RF_AHB_RST(n) (((n)&3)<<0)
#define SYS_CTRL_SET_RF_AHB_RST_MASK (3<<0)
#define SYS_CTRL_SET_RF_AHB_RST_SHIFT (0)

//RF_AHB_RST_CLR
#define SYS_CTRL_CLR_RF_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RF_REV0 (1<<1)
#define SYS_CTRL_CLR_RF_AHB_RST(n) (((n)&3)<<0)
#define SYS_CTRL_CLR_RF_AHB_RST_MASK (3<<0)
#define SYS_CTRL_CLR_RF_AHB_RST_SHIFT (0)

//RF_APB_RST_SET
#define SYS_CTRL_SET_RF_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_RF_APB_RST_ID_REV0 (1<<1)
#define SYS_CTRL_SET_RF_APB_RST(n) (((n)&3)<<0)
#define SYS_CTRL_SET_RF_APB_RST_MASK (3<<0)
#define SYS_CTRL_SET_RF_APB_RST_SHIFT (0)

//RF_APB_RST_CLR
#define SYS_CTRL_CLR_RF_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_RF_APB_RST_ID_REV0 (1<<1)
#define SYS_CTRL_CLR_RF_APB_RST(n) (((n)&3)<<0)
#define SYS_CTRL_CLR_RF_APB_RST_MASK (3<<0)
#define SYS_CTRL_CLR_RF_APB_RST_SHIFT (0)

//APCPU_RST_SET
#define SYS_CTRL_SET_APCPU_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_APCPU_RST_ID_CORE (1<<1)
#define SYS_CTRL_SET_APCPU_RST_ID_DBG (1<<2)
#define SYS_CTRL_SET_APCPU_RST_ID_GIC400 (1<<3)
#define SYS_CTRL_SET_GLOBAL_SOFT_RST (1<<4)
#define SYS_CTRL_SET_APCPU_RST(n)  (((n)&31)<<0)
#define SYS_CTRL_SET_APCPU_RST_MASK (31<<0)
#define SYS_CTRL_SET_APCPU_RST_SHIFT (0)

//APCPU_RST_CLR
#define SYS_CTRL_CLR_APCPU_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_APCPU_RST_ID_CORE (1<<1)
#define SYS_CTRL_CLR_APCPU_RST_ID_DBG (1<<2)
#define SYS_CTRL_CLR_APCPU_RST_ID_GIC400 (1<<3)
#define SYS_CTRL_CLR_GLOBAL_SOFT_RST (1<<4)
#define SYS_CTRL_CLR_APCPU_RST(n)  (((n)&31)<<0)
#define SYS_CTRL_CLR_APCPU_RST_MASK (31<<0)
#define SYS_CTRL_CLR_APCPU_RST_SHIFT (0)

//CPCPU_RST_SET
#define SYS_CTRL_SET_CPCPU_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_CPCPU_RST_ID_CORE (1<<1)
#define SYS_CTRL_SET_CPCPU_RST_ID_DBG (1<<2)
#define SYS_CTRL_SET_3             (1<<3)
#define SYS_CTRL_SET_CPCPU_RST(n)  (((n)&15)<<0)
#define SYS_CTRL_SET_CPCPU_RST_MASK (15<<0)
#define SYS_CTRL_SET_CPCPU_RST_SHIFT (0)

//CPCPU_RST_CLR
#define SYS_CTRL_CLR_CPCPU_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_CPCPU_RST_ID_CORE (1<<1)
#define SYS_CTRL_CLR_CPCPU_RST_ID_DBG (1<<2)
#define SYS_CTRL_CLR_3             (1<<3)
#define SYS_CTRL_CLR_CPCPU_RST(n)  (((n)&15)<<0)
#define SYS_CTRL_CLR_CPCPU_RST_MASK (15<<0)
#define SYS_CTRL_CLR_CPCPU_RST_SHIFT (0)

//BBLTE_RST_SET
#define SYS_CTRL_SET_BBLTE_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_BBLTE_RST_ID_REV0 (1<<1)
#define SYS_CTRL_SET_BBLTE_RST(n)  (((n)&3)<<0)
#define SYS_CTRL_SET_BBLTE_RST_MASK (3<<0)
#define SYS_CTRL_SET_BBLTE_RST_SHIFT (0)

//BBLTE_RST_CLR
#define SYS_CTRL_CLR_BBLTE_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_BBLTE_RST_ID_REV0 (1<<1)
#define SYS_CTRL_CLR_BBLTE_RST(n)  (((n)&3)<<0)
#define SYS_CTRL_CLR_BBLTE_RST_MASK (3<<0)
#define SYS_CTRL_CLR_BBLTE_RST_SHIFT (0)

//OTHERS_RST_SET
#define SYS_CTRL_SET_RSTO_ID_BCK1  (1<<0)
#define SYS_CTRL_SET_RSTO_ID_BCK2  (1<<1)
#define SYS_CTRL_SET_RSTO_ID_DBG_HOST (1<<2)
#define SYS_CTRL_SET_RSTO_ID_GPIO  (1<<3)
#define SYS_CTRL_SET_RSTO_ID_UART1 (1<<4)
#define SYS_CTRL_SET_RSTO_ID_UART2 (1<<5)
#define SYS_CTRL_SET_RSTO_ID_UART3 (1<<6)
#define SYS_CTRL_SET_RSTO_ID_USBC  (1<<7)
#define SYS_CTRL_SET_RSTO_ID_WDTIMER0 (1<<8)
#define SYS_CTRL_SET_RSTO_ID_WDTIMER1 (1<<9)
#define SYS_CTRL_SET_RSTO_ID_WDTIMER2 (1<<10)
#define SYS_CTRL_SET_RSTO_ID_SPIFLASH (1<<11)
#define SYS_CTRL_SET_RSTO_ID_SPIFLASH_SYS (1<<12)
#define SYS_CTRL_SET_RSTO_ID_SPIFLASH1 (1<<13)
#define SYS_CTRL_SET_RSTO_ID_SPIFLASH1_SYS (1<<14)
#define SYS_CTRL_SET_RSTO_ID_PSRAM_DMC (1<<15)
#define SYS_CTRL_SET_RSTO_ID_PSRAM_SYS (1<<16)
#define SYS_CTRL_SET_RSTO_ID_PSRAM_PAGESPY (1<<17)
#define SYS_CTRL_SET_RSTO_ID_VAD   (1<<18)
#define SYS_CTRL_SET_RSTO_ID_PIX   (1<<19)
#define SYS_CTRL_SET_RSTO_ID_SDM_26M (1<<20)
#define SYS_CTRL_SET_RSTO_ID_WDTIMER4 (1<<21)
#define SYS_CTRL_SET_RSTO_ID_LPS_GSM (1<<22)
#define SYS_CTRL_SET_RSTO_ID_LPS_NB (1<<23)
#define SYS_CTRL_SET_RSTO_ID_EFUSE (1<<24)
#define SYS_CTRL_SET_RSTO_ID_USB_ADP_32K (1<<25)
#define SYS_CTRL_SET_RSTO_ID_MIPIDSI (1<<26)
#define SYS_CTRL_SET_RSTO_ID_MIPIDSI_PHY (1<<27)
#define SYS_CTRL_SET_RSTO_ID_AUD_2AD (1<<28)
#define SYS_CTRL_SET_RSTO_ID_USB11_48M (1<<29)
#define SYS_CTRL_SET_RSTO_ID_ZSP_UART (1<<30)
#define SYS_CTRL_SET_RSTO_ID_CORESIGHT (1<<31)
#define SYS_CTRL_SET_OTHERS_RST(n) (((n)&0xFFFFFFFF)<<0)
#define SYS_CTRL_SET_OTHERS_RST_MASK (0xFFFFFFFF<<0)
#define SYS_CTRL_SET_OTHERS_RST_SHIFT (0)

//OTHERS_RST_CLR
#define SYS_CTRL_CLR_RSTO_ID_BCK1  (1<<0)
#define SYS_CTRL_CLR_RSTO_ID_BCK2  (1<<1)
#define SYS_CTRL_CLR_RSTO_ID_DBG_HOST (1<<2)
#define SYS_CTRL_CLR_RSTO_ID_GPIO  (1<<3)
#define SYS_CTRL_CLR_RSTO_ID_UART1 (1<<4)
#define SYS_CTRL_CLR_RSTO_ID_UART2 (1<<5)
#define SYS_CTRL_CLR_RSTO_ID_UART3 (1<<6)
#define SYS_CTRL_CLR_RSTO_ID_USBC  (1<<7)
#define SYS_CTRL_CLR_RSTO_ID_WDTIMER0 (1<<8)
#define SYS_CTRL_CLR_RSTO_ID_WDTIMER1 (1<<9)
#define SYS_CTRL_CLR_RSTO_ID_WDTIMER2 (1<<10)
#define SYS_CTRL_CLR_RSTO_ID_SPIFLASH (1<<11)
#define SYS_CTRL_CLR_RSTO_ID_SPIFLASH_SYS (1<<12)
#define SYS_CTRL_CLR_RSTO_ID_SPIFLASH1 (1<<13)
#define SYS_CTRL_CLR_RSTO_ID_SPIFLASH1_SYS (1<<14)
#define SYS_CTRL_CLR_RSTO_ID_PSRAM_DMC (1<<15)
#define SYS_CTRL_CLR_RSTO_ID_PSRAM_SYS (1<<16)
#define SYS_CTRL_CLR_RSTO_ID_PSRAM_PAGESPY (1<<17)
#define SYS_CTRL_CLR_RSTO_ID_VAD   (1<<18)
#define SYS_CTRL_CLR_RSTO_ID_PIX   (1<<19)
#define SYS_CTRL_CLR_RSTO_ID_SDM_26M (1<<20)
#define SYS_CTRL_CLR_RSTO_ID_WDTIMER4 (1<<21)
#define SYS_CTRL_CLR_RSTO_ID_LPS_GSM (1<<22)
#define SYS_CTRL_CLR_RSTO_ID_LPS_NB (1<<23)
#define SYS_CTRL_CLR_RSTO_ID_EFUSE (1<<24)
#define SYS_CTRL_CLR_RSTO_ID_USB_ADP_32K (1<<25)
#define SYS_CTRL_CLR_RSTO_ID_MIPIDSI (1<<26)
#define SYS_CTRL_CLR_RSTO_ID_MIPIDSI_PHY (1<<27)
#define SYS_CTRL_CLR_RSTO_ID_AUD_2AD (1<<28)
#define SYS_CTRL_CLR_RSTO_ID_USB11_48M (1<<29)
#define SYS_CTRL_CLR_RSTO_ID_ZSP_UART (1<<30)
#define SYS_CTRL_CLR_RSTO_ID_CORESIGHT (1<<31)
#define SYS_CTRL_CLR_OTHERS_RST(n) (((n)&0xFFFFFFFF)<<0)
#define SYS_CTRL_CLR_OTHERS_RST_MASK (0xFFFFFFFF<<0)
#define SYS_CTRL_CLR_OTHERS_RST_SHIFT (0)

//CLK_SYS_AXI_MODE
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_IMEM_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_IMEM_MANUAL (1<<0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_IMEM_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_IMEM_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_LZMA_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_LZMA_MANUAL (1<<1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_LZMA_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_LZMA_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV0_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV0_MANUAL (1<<2)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV0_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV1_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV1_MANUAL (1<<3)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV1_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV2_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV2_MANUAL (1<<4)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV2_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_SDMMC1_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_SDMMC1_MANUAL (1<<5)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_SDMMC1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_SDMMC1_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_SYS_AXI(n) (((n)&0x3F)<<0)
#define SYS_CTRL_MODE_CLK_SYS_AXI_MASK (0x3F<<0)
#define SYS_CTRL_MODE_CLK_SYS_AXI_SHIFT (0)

//CLK_SYS_AXI_ENABLE
#define SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_IMEM (1<<0)
#define SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_LZMA (1<<1)
#define SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_REV0 (1<<2)
#define SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_REV1 (1<<3)
#define SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_REV2 (1<<4)
#define SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_SDMMC1 (1<<5)
#define SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_ALWAYS (1<<6)
#define SYS_CTRL_ENABLE_CLK_SYS_AXI(n) (((n)&0x7F)<<0)
#define SYS_CTRL_ENABLE_CLK_SYS_AXI_MASK (0x7F<<0)
#define SYS_CTRL_ENABLE_CLK_SYS_AXI_SHIFT (0)

//CLK_SYS_AXI_DISABLE
#define SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_IMEM (1<<0)
#define SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_LZMA (1<<1)
#define SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_REV0 (1<<2)
#define SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_REV1 (1<<3)
#define SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_REV2 (1<<4)
#define SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_SDMMC1 (1<<5)
#define SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_ALWAYS (1<<6)
#define SYS_CTRL_DISABLE_CLK_SYS_AXI(n) (((n)&0x7F)<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_AXI_MASK (0x7F<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_AXI_SHIFT (0)

//CLK_SYS_AHB_MODE
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GOUDA_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GOUDA_MANUAL (1<<0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GOUDA_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GOUDA_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GGE_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GGE_MANUAL (1<<1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GGE_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GGE_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GEA3_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GEA3_MANUAL (1<<2)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GEA3_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GEA3_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_LZMA_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_LZMA_MANUAL (1<<3)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_LZMA_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_LZMA_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_F8_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_F8_MANUAL (1<<4)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_F8_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_F8_V_MANUAL (1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_AXIDMA_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_AXIDMA_MANUAL (1<<5)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_AXIDMA_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_AXIDMA_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_SYS_AHB(n) (((n)&0x3F)<<0)
#define SYS_CTRL_MODE_CLK_SYS_AHB_MASK (0x3F<<0)
#define SYS_CTRL_MODE_CLK_SYS_AHB_SHIFT (0)

//CLK_SYS_AHB_ENABLE
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GOUDA (1<<0)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GGE (1<<1)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GEA3 (1<<2)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_LZMA (1<<3)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_F8 (1<<4)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_AXIDMA (1<<5)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_USBC (1<<6)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_LCD (1<<7)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_AES (1<<8)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_USB11 (1<<9)
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_ALWAYS (1<<10)
#define SYS_CTRL_ENABLE_CLK_SYS_AHB(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_ENABLE_CLK_SYS_AHB_MASK (0x7FF<<0)
#define SYS_CTRL_ENABLE_CLK_SYS_AHB_SHIFT (0)

//CLK_SYS_AHB_DISABLE
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GOUDA (1<<0)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GGE (1<<1)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GEA3 (1<<2)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_LZMA (1<<3)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_F8 (1<<4)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_AXIDMA (1<<5)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_USBC (1<<6)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_LCD (1<<7)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_AES (1<<8)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_USB11 (1<<9)
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_ALWAYS (1<<10)
#define SYS_CTRL_DISABLE_CLK_SYS_AHB(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_AHB_MASK (0x7FF<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_AHB_SHIFT (0)

//CLK_AP_APB_MODE
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CONF_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CONF_MANUAL (1<<0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CONF_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CONF_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_MOD_CAMERA_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_MOD_CAMERA_MANUAL (1<<1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_MOD_CAMERA_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_MOD_CAMERA_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C1_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C1_MANUAL (1<<2)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C1_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C3_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C3_MANUAL (1<<3)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C3_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C3_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_MANUAL (1<<4)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH0_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH0_MANUAL (1<<5)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH0_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH1_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH1_MANUAL (1<<6)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH1_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH2_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH2_MANUAL (1<<7)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH2_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH3_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH3_MANUAL (1<<8)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH3_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH3_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH4_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH4_MANUAL (1<<9)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH4_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH4_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH5_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH5_MANUAL (1<<10)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH5_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH5_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH6_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH6_MANUAL (1<<11)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH6_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH6_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CHDBG_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CHDBG_MANUAL (1<<12)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CHDBG_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CHDBG_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_GOUDA_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_GOUDA_MANUAL (1<<13)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_GOUDA_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_GOUDA_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SDMMC2_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SDMMC2_MANUAL (1<<14)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SDMMC2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SDMMC2_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI1_AUTOMATIC (0<<15)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI1_MANUAL (1<<15)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI1_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI2_AUTOMATIC (0<<16)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI2_MANUAL (1<<16)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI2_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID1_AUTOMATIC (0<<17)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID1_MANUAL (1<<17)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID1_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID2_AUTOMATIC (0<<18)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID2_MANUAL (1<<18)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCID2_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI1_AUTOMATIC (0<<19)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI1_MANUAL (1<<19)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI1_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI2_AUTOMATIC (0<<20)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI2_MANUAL (1<<20)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SCI2_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CAMERA_AUTOMATIC (0<<21)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CAMERA_MANUAL (1<<21)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CAMERA_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CAMERA_V_MANUAL (1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_PAGESPY_AUTOMATIC (0<<22)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_PAGESPY_MANUAL (1<<22)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_PAGESPY_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_PAGESPY_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_AP_APB(n) (((n)&0x7FFFFF)<<0)
#define SYS_CTRL_MODE_CLK_AP_APB_MASK (0x7FFFFF<<0)
#define SYS_CTRL_MODE_CLK_AP_APB_SHIFT (0)

//CLK_AP_APB_ENABLE
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_MOD_CAMERA (1<<1)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_I2C1 (1<<2)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_I2C3 (1<<3)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC (1<<4)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH0 (1<<5)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH1 (1<<6)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH2 (1<<7)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH3 (1<<8)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH4 (1<<9)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH5 (1<<10)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH6 (1<<11)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CHDBG (1<<12)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_GOUDA (1<<13)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SDMMC2 (1<<14)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SPI1 (1<<15)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SPI2 (1<<16)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SCID1 (1<<17)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SCID2 (1<<18)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SCI1 (1<<19)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SCI2 (1<<20)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_CAMERA (1<<21)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_PAGESPY (1<<22)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_LZMA (1<<23)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_REV2 (1<<24)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_TIMER1 (1<<25)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_TIMER2 (1<<26)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_TIMER4 (1<<27)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_LCD (1<<28)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_CQIRQ (1<<29)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_ALWAYS (1<<30)
#define SYS_CTRL_ENABLE_CLK_AP_APB(n) (((n)&0x7FFFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_AP_APB_MASK (0x7FFFFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_AP_APB_SHIFT (0)

//CLK_AP_APB_DISABLE
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_MOD_CAMERA (1<<1)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_I2C1 (1<<2)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_I2C3 (1<<3)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC (1<<4)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH0 (1<<5)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH1 (1<<6)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH2 (1<<7)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH3 (1<<8)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH4 (1<<9)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH5 (1<<10)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH6 (1<<11)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CHDBG (1<<12)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_GOUDA (1<<13)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SDMMC2 (1<<14)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SPI1 (1<<15)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SPI2 (1<<16)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SCID1 (1<<17)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SCID2 (1<<18)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SCI1 (1<<19)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SCI2 (1<<20)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_CAMERA (1<<21)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_PAGESPY (1<<22)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_LZMA (1<<23)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_REV2 (1<<24)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_TIMER1 (1<<25)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_TIMER2 (1<<26)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_TIMER4 (1<<27)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_LCD (1<<28)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_CQIRQ (1<<29)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_ALWAYS (1<<30)
#define SYS_CTRL_DISABLE_CLK_AP_APB(n) (((n)&0x7FFFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_AP_APB_MASK (0x7FFFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_AP_APB_SHIFT (0)

//CLK_AIF_APB_MODE
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_CONF_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_CONF_MANUAL (1<<0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_CONF_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_CONF_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF1_APB_CLK_ID_AIF_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AIF1_APB_CLK_ID_AIF_MANUAL (1<<1)
#define SYS_CTRL_MODE_AIF1_APB_CLK_ID_AIF_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF1_APB_CLK_ID_AIF_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF2_APB_CLK_ID_AIF_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AIF2_APB_CLK_ID_AIF_MANUAL (1<<2)
#define SYS_CTRL_MODE_AIF2_APB_CLK_ID_AIF_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF2_APB_CLK_ID_AIF_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_MANUAL (1<<3)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH0_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH0_MANUAL (1<<4)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH0_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH1_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH1_MANUAL (1<<5)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH1_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH2_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH2_MANUAL (1<<6)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH2_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH3_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH3_MANUAL (1<<7)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH3_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH3_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV0_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV0_MANUAL (1<<8)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV0_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV1_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV1_MANUAL (1<<9)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV1_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV2_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV2_MANUAL (1<<10)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV2_V_MANUAL (1)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_AUD_2AD_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_AUD_2AD_MANUAL (1<<11)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_AUD_2AD_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_AUD_2AD_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_AIF_APB(n) (((n)&0xFFF)<<0)
#define SYS_CTRL_MODE_CLK_AIF_APB_MASK (0xFFF<<0)
#define SYS_CTRL_MODE_CLK_AIF_APB_SHIFT (0)

//CLK_AIF_APB_ENABLE
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_ENABLE_AIF1_APB_CLK_ID_AIF (1<<1)
#define SYS_CTRL_ENABLE_AIF2_APB_CLK_ID_AIF (1<<2)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_IFC (1<<3)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_IFC_CH0 (1<<4)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_IFC_CH1 (1<<5)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_IFC_CH2 (1<<6)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_IFC_CH3 (1<<7)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_REV0 (1<<8)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_REV1 (1<<9)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_REV2 (1<<10)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_AUD_2AD (1<<11)
#define SYS_CTRL_ENABLE_AIF_APB_CLK_ID_ALWAYS (1<<12)
#define SYS_CTRL_ENABLE_CLK_AIF_APB(n) (((n)&0x1FFF)<<0)
#define SYS_CTRL_ENABLE_CLK_AIF_APB_MASK (0x1FFF<<0)
#define SYS_CTRL_ENABLE_CLK_AIF_APB_SHIFT (0)

//CLK_AIF_APB_DISABLE
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_DISABLE_AIF1_APB_CLK_ID_AIF (1<<1)
#define SYS_CTRL_DISABLE_AIF2_APB_CLK_ID_AIF (1<<2)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_IFC (1<<3)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_IFC_CH0 (1<<4)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_IFC_CH1 (1<<5)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_IFC_CH2 (1<<6)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_IFC_CH3 (1<<7)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_REV0 (1<<8)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_REV1 (1<<9)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_REV2 (1<<10)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_AUD_2AD (1<<11)
#define SYS_CTRL_DISABLE_AIF_APB_CLK_ID_ALWAYS (1<<12)
#define SYS_CTRL_DISABLE_CLK_AIF_APB(n) (((n)&0x1FFF)<<0)
#define SYS_CTRL_DISABLE_CLK_AIF_APB_MASK (0x1FFF<<0)
#define SYS_CTRL_DISABLE_CLK_AIF_APB_SHIFT (0)

//CLK_AON_AHB_MODE
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_SYSCTRL_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_SYSCTRL_MANUAL (1<<0)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_SYSCTRL_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_SYSCTRL_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV0_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV0_MANUAL (1<<1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV0_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV1_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV1_MANUAL (1<<2)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV1_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV2_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV2_MANUAL (1<<3)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV2_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_CSSYS_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_CSSYS_MANUAL (1<<4)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_CSSYS_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_CSSYS_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_AON_AHB(n) (((n)&31)<<0)
#define SYS_CTRL_MODE_CLK_AON_AHB_MASK (31<<0)
#define SYS_CTRL_MODE_CLK_AON_AHB_SHIFT (0)

//CLK_AON_AHB_ENABLE
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_SYSCTRL (1<<0)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_REV0 (1<<1)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_REV1 (1<<2)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_REV2 (1<<3)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_CSSYS (1<<4)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_ALWAYS (1<<5)
#define SYS_CTRL_ENABLE_CLK_AON_AHB(n) (((n)&0x3F)<<0)
#define SYS_CTRL_ENABLE_CLK_AON_AHB_MASK (0x3F<<0)
#define SYS_CTRL_ENABLE_CLK_AON_AHB_SHIFT (0)

//CLK_AON_AHB_DISABLE
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_SYSCTRL (1<<0)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_REV0 (1<<1)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_REV1 (1<<2)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_REV2 (1<<3)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_CSSYS (1<<4)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_ALWAYS (1<<5)
#define SYS_CTRL_DISABLE_CLK_AON_AHB(n) (((n)&0x3F)<<0)
#define SYS_CTRL_DISABLE_CLK_AON_AHB_MASK (0x3F<<0)
#define SYS_CTRL_DISABLE_CLK_AON_AHB_SHIFT (0)

//CLK_AON_APB_MODE
#define SYS_CTRL_MODE_AON_APB_CLK_ID_CONF_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_CONF_MANUAL (1<<0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_CONF_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_CONF_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_HOST_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_HOST_MANUAL (1<<1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_HOST_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_HOST_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_UART_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_UART_MANUAL (1<<2)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_UART_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_UART_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_MANUAL (1<<3)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH0_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH0_MANUAL (1<<4)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH0_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH1_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH1_MANUAL (1<<5)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH1_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH2_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH2_MANUAL (1<<6)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH2_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH3_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH3_MANUAL (1<<7)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH3_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH3_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CHDBG_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CHDBG_MANUAL (1<<8)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CHDBG_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CHDBG_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV0_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV0_MANUAL (1<<9)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV0_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV1_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV1_MANUAL (1<<10)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV1_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_MANUAL (1<<11)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_I2C2_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_I2C2_MANUAL (1<<12)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_I2C2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_I2C2_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_ANA_REG_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_ANA_REG_MANUAL (1<<13)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_ANA_REG_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_ANA_REG_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SPINLOCK_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SPINLOCK_MANUAL (1<<14)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SPINLOCK_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SPINLOCK_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_GSM_AUTOMATIC (0<<15)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_GSM_MANUAL (1<<15)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_GSM_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_GSM_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_NB_AUTOMATIC (0<<16)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_NB_MANUAL (1<<16)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_NB_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_NB_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_EFUSE_AUTOMATIC (0<<17)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_EFUSE_MANUAL (1<<17)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_EFUSE_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_EFUSE_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV2_AUTOMATIC (0<<18)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV2_MANUAL (1<<18)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_REV2_V_MANUAL (1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_VAD_AUTOMATIC (0<<19)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_VAD_MANUAL (1<<19)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_VAD_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_VAD_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_AON_APB(n) (((n)&0xFFFFF)<<0)
#define SYS_CTRL_MODE_CLK_AON_APB_MASK (0xFFFFF<<0)
#define SYS_CTRL_MODE_CLK_AON_APB_SHIFT (0)

//CLK_AON_APB_ENABLE
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_DBG_HOST (1<<1)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_DBG_UART (1<<2)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC (1<<3)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH0 (1<<4)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH1 (1<<5)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH2 (1<<6)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH3 (1<<7)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CHDBG (1<<8)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_REV0 (1<<9)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_REV1 (1<<10)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_PWM (1<<11)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_I2C2 (1<<12)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_ANA_REG (1<<13)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_SPINLOCK (1<<14)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_LPS_GSM (1<<15)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_LPS_NB (1<<16)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_EFUSE (1<<17)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_REV2 (1<<18)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_VAD (1<<19)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_REV3 (1<<20)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_REV4 (1<<21)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_GPIO (1<<22)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_TIMER3 (1<<23)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_KEYPAD (1<<24)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_AHB2APB_ADI (1<<25)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_ADI (1<<26)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_LVDS (1<<27)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_ALWAYS (1<<28)
#define SYS_CTRL_ENABLE_CLK_AON_APB(n) (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_AON_APB_MASK (0x1FFFFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_AON_APB_SHIFT (0)

//CLK_AON_APB_DISABLE
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_DBG_HOST (1<<1)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_DBG_UART (1<<2)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC (1<<3)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH0 (1<<4)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH1 (1<<5)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH2 (1<<6)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH3 (1<<7)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CHDBG (1<<8)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_REV0 (1<<9)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_REV1 (1<<10)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_PWM (1<<11)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_I2C2 (1<<12)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_ANA_REG (1<<13)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_SPINLOCK (1<<14)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_LPS_GSM (1<<15)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_LPS_NB (1<<16)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_EFUSE (1<<17)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_REV2 (1<<18)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_VAD (1<<19)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_REV3 (1<<20)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_REV4 (1<<21)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_GPIO (1<<22)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_TIMER3 (1<<23)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_KEYPAD (1<<24)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_AHB2APB_ADI (1<<25)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_ADI (1<<26)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_LVDS (1<<27)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_ALWAYS (1<<28)
#define SYS_CTRL_DISABLE_CLK_AON_APB(n) (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_AON_APB_MASK (0x1FFFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_AON_APB_SHIFT (0)

//CLK_RF_AHB_MODE
#define SYS_CTRL_MODE_CLK_RF_AHB_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_CLK_RF_AHB_MANUAL (1<<0)
#define SYS_CTRL_MODE_CLK_RF_AHB_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_CLK_RF_AHB_V_MANUAL (1)

//CLK_RF_AHB_ENABLE
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_REV0 (1<<0)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_ALWAYS (1<<1)
#define SYS_CTRL_ENABLE_CLK_RF_AHB(n) (((n)&3)<<0)
#define SYS_CTRL_ENABLE_CLK_RF_AHB_MASK (3<<0)
#define SYS_CTRL_ENABLE_CLK_RF_AHB_SHIFT (0)

//CLK_RF_AHB_DISABLE
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_REV0 (1<<0)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_ALWAYS (1<<1)
#define SYS_CTRL_DISABLE_CLK_RF_AHB(n) (((n)&3)<<0)
#define SYS_CTRL_DISABLE_CLK_RF_AHB_MASK (3<<0)
#define SYS_CTRL_DISABLE_CLK_RF_AHB_SHIFT (0)

//CLK_RF_APB_MODE
#define SYS_CTRL_MODE_CLK_RF_APB_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_CLK_RF_APB_MANUAL (1<<0)
#define SYS_CTRL_MODE_CLK_RF_APB_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_CLK_RF_APB_V_MANUAL (1)

//CLK_RF_APB_ENABLE
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_REV0 (1<<0)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_ALWAYS (1<<1)
#define SYS_CTRL_ENABLE_CLK_RF_APB(n) (((n)&3)<<0)
#define SYS_CTRL_ENABLE_CLK_RF_APB_MASK (3<<0)
#define SYS_CTRL_ENABLE_CLK_RF_APB_SHIFT (0)

//CLK_RF_APB_DISABLE
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_REV0 (1<<0)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_ALWAYS (1<<1)
#define SYS_CTRL_DISABLE_CLK_RF_APB(n) (((n)&3)<<0)
#define SYS_CTRL_DISABLE_CLK_RF_APB_MASK (3<<0)
#define SYS_CTRL_DISABLE_CLK_RF_APB_SHIFT (0)

//CLK_OTHERS_MODE
#define SYS_CTRL_MODE_OC_ID_HOST_UART_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_OC_ID_HOST_UART_MANUAL (1<<0)
#define SYS_CTRL_MODE_OC_ID_HOST_UART_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_HOST_UART_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_BCK1_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_OC_ID_BCK1_MANUAL (1<<1)
#define SYS_CTRL_MODE_OC_ID_BCK1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_BCK1_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_BCK2_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_OC_ID_BCK2_MANUAL (1<<2)
#define SYS_CTRL_MODE_OC_ID_BCK2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_BCK2_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_UART1_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_OC_ID_UART1_MANUAL (1<<3)
#define SYS_CTRL_MODE_OC_ID_UART1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_UART1_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_UART2_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_OC_ID_UART2_MANUAL (1<<4)
#define SYS_CTRL_MODE_OC_ID_UART2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_UART2_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_UART3_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_OC_ID_UART3_MANUAL (1<<5)
#define SYS_CTRL_MODE_OC_ID_UART3_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_UART3_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_AP_A5_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_OC_ID_AP_A5_MANUAL (1<<6)
#define SYS_CTRL_MODE_OC_ID_AP_A5_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_AP_A5_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_CP_A5_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_OC_ID_CP_A5_MANUAL (1<<7)
#define SYS_CTRL_MODE_OC_ID_CP_A5_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_CP_A5_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_OTHERS(n) (((n)&0xFF)<<0)
#define SYS_CTRL_MODE_CLK_OTHERS_MASK (0xFF<<0)
#define SYS_CTRL_MODE_CLK_OTHERS_SHIFT (0)

//CLK_OTHERS_ENABLE
#define SYS_CTRL_ENABLE_OC_ID_HOST_UART (1<<0)
#define SYS_CTRL_ENABLE_OC_ID_BCK1 (1<<1)
#define SYS_CTRL_ENABLE_OC_ID_BCK2 (1<<2)
#define SYS_CTRL_ENABLE_OC_ID_UART1 (1<<3)
#define SYS_CTRL_ENABLE_OC_ID_UART2 (1<<4)
#define SYS_CTRL_ENABLE_OC_ID_UART3 (1<<5)
#define SYS_CTRL_ENABLE_OC_ID_AP_A5 (1<<6)
#define SYS_CTRL_ENABLE_OC_ID_CP_A5 (1<<7)
#define SYS_CTRL_ENABLE_OC_ID_GPIO (1<<8)
#define SYS_CTRL_ENABLE_OC_ID_USBPHY (1<<9)
#define SYS_CTRL_ENABLE_OC_ID_PIX  (1<<10)
#define SYS_CTRL_ENABLE_OC_ID_CLK_OUT (1<<11)
#define SYS_CTRL_ENABLE_OC_ID_ISP  (1<<12)
#define SYS_CTRL_ENABLE_OC_ID_SYS_SPIFLASH (1<<13)
#define SYS_CTRL_ENABLE_OC_ID_SYS_SPIFLASH_ALWAYS (1<<14)
#define SYS_CTRL_ENABLE_OC_ID_SYS_SPIFLASH1 (1<<15)
#define SYS_CTRL_ENABLE_OC_ID_SYS_SPIFLASH1_ALWAYS (1<<16)
#define SYS_CTRL_ENABLE_OC_ID_SPIFLASH (1<<17)
#define SYS_CTRL_ENABLE_OC_ID_SPIFLASH1 (1<<18)
#define SYS_CTRL_ENABLE_OC_ID_SPICAM (1<<19)
#define SYS_CTRL_ENABLE_OC_ID_CAM  (1<<20)
#define SYS_CTRL_ENABLE_OC_ID_PSRAM_CONF (1<<21)
#define SYS_CTRL_ENABLE_OC_ID_PSRAM_DMC (1<<22)
#define SYS_CTRL_ENABLE_OC_ID_PSRAM_PAGESPY (1<<23)
#define SYS_CTRL_ENABLE_OC_ID_PSRAM_ALWAYS (1<<24)
#define SYS_CTRL_ENABLE_CLK_OTHERS(n) (((n)&0x1FFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_OTHERS_MASK (0x1FFFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_OTHERS_SHIFT (0)

//CLK_OTHERS_DISABLE
#define SYS_CTRL_DISABLE_OC_ID_HOST_UART (1<<0)
#define SYS_CTRL_DISABLE_OC_ID_BCK1 (1<<1)
#define SYS_CTRL_DISABLE_OC_ID_BCK2 (1<<2)
#define SYS_CTRL_DISABLE_OC_ID_UART1 (1<<3)
#define SYS_CTRL_DISABLE_OC_ID_UART2 (1<<4)
#define SYS_CTRL_DISABLE_OC_ID_UART3 (1<<5)
#define SYS_CTRL_DISABLE_OC_ID_AP_A5 (1<<6)
#define SYS_CTRL_DISABLE_OC_ID_CP_A5 (1<<7)
#define SYS_CTRL_DISABLE_OC_ID_GPIO (1<<8)
#define SYS_CTRL_DISABLE_OC_ID_USBPHY (1<<9)
#define SYS_CTRL_DISABLE_OC_ID_PIX (1<<10)
#define SYS_CTRL_DISABLE_OC_ID_CLK_OUT (1<<11)
#define SYS_CTRL_DISABLE_OC_ID_ISP (1<<12)
#define SYS_CTRL_DISABLE_OC_ID_SYS_SPIFLASH (1<<13)
#define SYS_CTRL_DISABLE_OC_ID_SYS_SPIFLASH_ALWAYS (1<<14)
#define SYS_CTRL_DISABLE_OC_ID_SYS_SPIFLASH1 (1<<15)
#define SYS_CTRL_DISABLE_OC_ID_SYS_SPIFLASH1_ALWAYS (1<<16)
#define SYS_CTRL_DISABLE_OC_ID_SPIFLASH (1<<17)
#define SYS_CTRL_DISABLE_OC_ID_SPIFLASH1 (1<<18)
#define SYS_CTRL_DISABLE_OC_ID_SPICAM (1<<19)
#define SYS_CTRL_DISABLE_OC_ID_CAM (1<<20)
#define SYS_CTRL_DISABLE_OC_ID_PSRAM_CONF (1<<21)
#define SYS_CTRL_DISABLE_OC_ID_PSRAM_DMC (1<<22)
#define SYS_CTRL_DISABLE_OC_ID_PSRAM_PAGESPY (1<<23)
#define SYS_CTRL_DISABLE_OC_ID_PSRAM_ALWAYS (1<<24)
#define SYS_CTRL_DISABLE_CLK_OTHERS(n) (((n)&0x1FFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_OTHERS_MASK (0x1FFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_OTHERS_SHIFT (0)

//CLK_OTHERS_1_MODE
#define SYS_CTRL_MODE_OC_ID_GGE_CLK32K_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_OC_ID_GGE_CLK32K_MANUAL (1<<0)
#define SYS_CTRL_MODE_OC_ID_GGE_CLK32K_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_GGE_CLK32K_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_GGE_26M_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_OC_ID_GGE_26M_MANUAL (1<<1)
#define SYS_CTRL_MODE_OC_ID_GGE_26M_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_GGE_26M_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_NB_61P44M_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_OC_ID_NB_61P44M_MANUAL (1<<2)
#define SYS_CTRL_MODE_OC_ID_NB_61P44M_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_NB_61P44M_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_BT_FM_CLK32K_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_OC_ID_BT_FM_CLK32K_MANUAL (1<<3)
#define SYS_CTRL_MODE_OC_ID_BT_FM_CLK32K_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_BT_FM_CLK32K_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_BT_FM_26M_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_OC_ID_BT_FM_26M_MANUAL (1<<4)
#define SYS_CTRL_MODE_OC_ID_BT_FM_26M_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_BT_FM_26M_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_PMIC_CLK32K_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_OC_ID_PMIC_CLK32K_MANUAL (1<<5)
#define SYS_CTRL_MODE_OC_ID_PMIC_CLK32K_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_PMIC_CLK32K_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_PMIC_26M_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_OC_ID_PMIC_26M_MANUAL (1<<6)
#define SYS_CTRL_MODE_OC_ID_PMIC_26M_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_PMIC_26M_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_CP_CLK32K_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_OC_ID_CP_CLK32K_MANUAL (1<<7)
#define SYS_CTRL_MODE_OC_ID_CP_CLK32K_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_CP_CLK32K_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_CP_26M_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_OC_ID_CP_26M_MANUAL (1<<8)
#define SYS_CTRL_MODE_OC_ID_CP_26M_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_CP_26M_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_ZSP_UART_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_OC_ID_ZSP_UART_MANUAL (1<<9)
#define SYS_CTRL_MODE_OC_ID_ZSP_UART_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_ZSP_UART_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_CP_BBLTE_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_OC_ID_CP_BBLTE_MANUAL (1<<10)
#define SYS_CTRL_MODE_OC_ID_CP_BBLTE_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_CP_BBLTE_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_494M_LTE_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_OC_ID_494M_LTE_MANUAL (1<<11)
#define SYS_CTRL_MODE_OC_ID_494M_LTE_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_494M_LTE_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_REV0_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_OC_ID_REV0_MANUAL (1<<12)
#define SYS_CTRL_MODE_OC_ID_REV0_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_REV0_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_REV1_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_OC_ID_REV1_MANUAL (1<<13)
#define SYS_CTRL_MODE_OC_ID_REV1_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_REV1_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_REV2_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_OC_ID_REV2_MANUAL (1<<14)
#define SYS_CTRL_MODE_OC_ID_REV2_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_REV2_V_MANUAL (1)
#define SYS_CTRL_MODE_OC_ID_REV3_AUTOMATIC (0<<15)
#define SYS_CTRL_MODE_OC_ID_REV3_MANUAL (1<<15)
#define SYS_CTRL_MODE_OC_ID_REV3_V_AUTOMATIC (0)
#define SYS_CTRL_MODE_OC_ID_REV3_V_MANUAL (1)
#define SYS_CTRL_MODE_CLK_OTHERS_1(n) (((n)&0xFFFF)<<0)
#define SYS_CTRL_MODE_CLK_OTHERS_1_MASK (0xFFFF<<0)
#define SYS_CTRL_MODE_CLK_OTHERS_1_SHIFT (0)

//CLK_OTHERS_1_ENABLE
#define SYS_CTRL_ENABLE_OC_ID_GGE_CLK32K (1<<0)
#define SYS_CTRL_ENABLE_OC_ID_GGE_26M (1<<1)
#define SYS_CTRL_ENABLE_OC_ID_NB_61P44M (1<<2)
#define SYS_CTRL_ENABLE_OC_ID_BT_FM_CLK32K (1<<3)
#define SYS_CTRL_ENABLE_OC_ID_BT_FM_26M (1<<4)
#define SYS_CTRL_ENABLE_OC_ID_PMIC_CLK32K (1<<5)
#define SYS_CTRL_ENABLE_OC_ID_PMIC_26M (1<<6)
#define SYS_CTRL_ENABLE_OC_ID_CP_CLK32K (1<<7)
#define SYS_CTRL_ENABLE_OC_ID_CP_26M (1<<8)
#define SYS_CTRL_ENABLE_OC_ID_ZSP_UART (1<<9)
#define SYS_CTRL_ENABLE_OC_ID_CP_BBLTE (1<<10)
#define SYS_CTRL_ENABLE_OC_ID_494M_LTE (1<<11)
#define SYS_CTRL_ENABLE_OC_ID_REV0 (1<<12)
#define SYS_CTRL_ENABLE_OC_ID_REV1 (1<<13)
#define SYS_CTRL_ENABLE_OC_ID_REV2 (1<<14)
#define SYS_CTRL_ENABLE_OC_ID_REV3 (1<<15)
#define SYS_CTRL_ENABLE_OC_ID_SDM_26M (1<<16)
#define SYS_CTRL_ENABLE_OC_ID_LPS_GSM (1<<17)
#define SYS_CTRL_ENABLE_OC_ID_LPS_NB (1<<18)
#define SYS_CTRL_ENABLE_OC_ID_EFUSE_26M (1<<19)
#define SYS_CTRL_ENABLE_OC_ID_USB_ADP_32K (1<<20)
#define SYS_CTRL_ENABLE_OC_ID_USB_UTMI_48M (1<<21)
#define SYS_CTRL_ENABLE_OC_ID_AP_26M (1<<22)
#define SYS_CTRL_ENABLE_OC_ID_AP_32K (1<<23)
#define SYS_CTRL_ENABLE_OC_ID_MIPIDSI (1<<24)
#define SYS_CTRL_ENABLE_OC_ID_AHB_BTFM (1<<25)
#define SYS_CTRL_ENABLE_OC_ID_VAD  (1<<26)
#define SYS_CTRL_ENABLE_OC_ID_USB11_48M (1<<27)
#define SYS_CTRL_ENABLE_OC_ID_TRNG_CLKEN (1<<28)
#define SYS_CTRL_ENABLE_OC_ID_CORESIGHT (1<<29)
#define SYS_CTRL_ENABLE_OC_ID_ADI  (1<<30)
#define SYS_CTRL_ENABLE_CLK_OTHERS_1(n) (((n)&0x7FFFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_OTHERS_1_MASK (0x7FFFFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_OTHERS_1_SHIFT (0)

//CLK_OTHERS_1_DISABLE
#define SYS_CTRL_DISABLE_OC_ID_GGE_CLK32K (1<<0)
#define SYS_CTRL_DISABLE_OC_ID_GGE_26M (1<<1)
#define SYS_CTRL_DISABLE_OC_ID_NB_61P44M (1<<2)
#define SYS_CTRL_DISABLE_OC_ID_BT_FM_CLK32K (1<<3)
#define SYS_CTRL_DISABLE_OC_ID_BT_FM_26M (1<<4)
#define SYS_CTRL_DISABLE_OC_ID_PMIC_CLK32K (1<<5)
#define SYS_CTRL_DISABLE_OC_ID_PMIC_26M (1<<6)
#define SYS_CTRL_DISABLE_OC_ID_CP_CLK32K (1<<7)
#define SYS_CTRL_DISABLE_OC_ID_CP_26M (1<<8)
#define SYS_CTRL_DISABLE_OC_ID_ZSP_UART (1<<9)
#define SYS_CTRL_DISABLE_OC_ID_CP_BBLTE (1<<10)
#define SYS_CTRL_DISABLE_OC_ID_494M_LTE (1<<11)
#define SYS_CTRL_DISABLE_OC_ID_REV0 (1<<12)
#define SYS_CTRL_DISABLE_OC_ID_REV1 (1<<13)
#define SYS_CTRL_DISABLE_OC_ID_REV2 (1<<14)
#define SYS_CTRL_DISABLE_OC_ID_REV3 (1<<15)
#define SYS_CTRL_DISABLE_OC_ID_SDM_26M (1<<16)
#define SYS_CTRL_DISABLE_OC_ID_LPS_GSM (1<<17)
#define SYS_CTRL_DISABLE_OC_ID_LPS_NB (1<<18)
#define SYS_CTRL_DISABLE_OC_ID_EFUSE_26M (1<<19)
#define SYS_CTRL_DISABLE_OC_ID_USB_ADP_32K (1<<20)
#define SYS_CTRL_DISABLE_OC_ID_USB_UTMI_48M (1<<21)
#define SYS_CTRL_DISABLE_OC_ID_AP_26M (1<<22)
#define SYS_CTRL_DISABLE_OC_ID_AP_32K (1<<23)
#define SYS_CTRL_DISABLE_OC_ID_MIPIDSI (1<<24)
#define SYS_CTRL_DISABLE_OC_ID_AHB_BTFM (1<<25)
#define SYS_CTRL_DISABLE_OC_ID_VAD (1<<26)
#define SYS_CTRL_DISABLE_OC_ID_USB11_48M (1<<27)
#define SYS_CTRL_DISABLE_OC_ID_TRNG_CLKEN (1<<28)
#define SYS_CTRL_DISABLE_OC_ID_CORESIGHT (1<<29)
#define SYS_CTRL_DISABLE_OC_ID_ADI (1<<30)
#define SYS_CTRL_DISABLE_CLK_OTHERS_1(n) (((n)&0x7FFFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_OTHERS_1_MASK (0x7FFFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_OTHERS_1_SHIFT (0)

//Pll_Ctrl
#define SYS_CTRL_PLL_ENABLE        (1<<0)
#define SYS_CTRL_PLL_ENABLE_MASK   (1<<0)
#define SYS_CTRL_PLL_ENABLE_SHIFT  (0)
#define SYS_CTRL_PLL_ENABLE_POWER_DOWN (0<<0)
#define SYS_CTRL_PLL_ENABLE_ENABLE (1<<0)
#define SYS_CTRL_PLL_ENABLE_V_POWER_DOWN (0)
#define SYS_CTRL_PLL_ENABLE_V_ENABLE (1)
#define SYS_CTRL_PLL_LOCK_RESET    (1<<4)
#define SYS_CTRL_PLL_LOCK_RESET_MASK (1<<4)
#define SYS_CTRL_PLL_LOCK_RESET_SHIFT (4)
#define SYS_CTRL_PLL_LOCK_RESET_RESET (0<<4)
#define SYS_CTRL_PLL_LOCK_RESET_NO_RESET (1<<4)
#define SYS_CTRL_PLL_LOCK_RESET_V_RESET (0)
#define SYS_CTRL_PLL_LOCK_RESET_V_NO_RESET (1)
#define SYS_CTRL_PLL_BYPASS_LOCK   (1<<8)
#define SYS_CTRL_PLL_BYPASS_LOCK_MASK (1<<8)
#define SYS_CTRL_PLL_BYPASS_LOCK_SHIFT (8)
#define SYS_CTRL_PLL_BYPASS_LOCK_PASS (0<<8)
#define SYS_CTRL_PLL_BYPASS_LOCK_BYPASS (1<<8)
#define SYS_CTRL_PLL_BYPASS_LOCK_V_PASS (0)
#define SYS_CTRL_PLL_BYPASS_LOCK_V_BYPASS (1)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE (1<<12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_MASK (1<<12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_SHIFT (12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE (1<<12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE (0<<12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_V_ENABLE (1)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_V_DISABLE (0)

//Sel_Clock
#define SYS_CTRL_SLOW_SEL_RF_OSCILLATOR (1<<0)
#define SYS_CTRL_SLOW_SEL_RF_RF    (0<<0)
#define SYS_CTRL_SLOW_SEL_RF_V_OSCILLATOR (1)
#define SYS_CTRL_SLOW_SEL_RF_V_RF  (0)
#define SYS_CTRL_SYS_SEL_FAST_SLOW (1<<1)
#define SYS_CTRL_SYS_SEL_FAST_FAST (0<<1)
#define SYS_CTRL_SYS_SEL_FAST_V_SLOW (1)
#define SYS_CTRL_SYS_SEL_FAST_V_FAST (0)
#define SYS_CTRL_OSC_32K_26M_DIV32K_SEL (1<<2)
#define SYS_CTRL_PLL_DISABLE_LPS_DISABLE (1<<3)
#define SYS_CTRL_PLL_DISABLE_LPS_ENABLE (0<<3)
#define SYS_CTRL_PLL_DISABLE_LPS_V_DISABLE (1)
#define SYS_CTRL_PLL_DISABLE_LPS_V_ENABLE (0)
#define SYS_CTRL_RF_DETECTED_OK    (1<<4)
#define SYS_CTRL_RF_DETECTED_NO    (0<<4)
#define SYS_CTRL_RF_DETECTED_V_OK  (1)
#define SYS_CTRL_RF_DETECTED_V_NO  (0)
#define SYS_CTRL_RF_DETECT_BYPASS  (1<<5)
#define SYS_CTRL_RF_DETECT_RESET   (1<<6)
#define SYS_CTRL_RF_SELECTED_L     (1<<7)
#define SYS_CTRL_PLL_LOCKED        (1<<8)
#define SYS_CTRL_PLL_LOCKED_MASK   (1<<8)
#define SYS_CTRL_PLL_LOCKED_SHIFT  (8)
#define SYS_CTRL_PLL_LOCKED_LOCKED (1<<8)
#define SYS_CTRL_PLL_LOCKED_NOT_LOCKED (0<<8)
#define SYS_CTRL_PLL_LOCKED_V_LOCKED (1)
#define SYS_CTRL_PLL_LOCKED_V_NOT_LOCKED (0)
#define SYS_CTRL_FAST_SELECTED_L   (1<<9)
#define SYS_CTRL_FAST_SELECTED_L_MASK (1<<9)
#define SYS_CTRL_FAST_SELECTED_L_SHIFT (9)
#define SYS_CTRL_SOFT_SEL_SPIFLASH (1<<10)
#define SYS_CTRL_SOFT_SEL_MEM_BRIDGE (1<<11)
#define SYS_CTRL_BBLTE_CLK_PLL_SEL (1<<12)
#define SYS_CTRL_CAMERA_CLK_PLL_SEL (1<<13)
#define SYS_CTRL_USB_PLL_LOCKED_H  (1<<14)
#define SYS_CTRL_BB26M_SEL         (1<<15)
#define SYS_CTRL_SOFT_SEL_SPIFLASH1 (1<<16)
#define SYS_CTRL_APLL_LOCKED_H     (1<<17)
#define SYS_CTRL_MEMPLL_LOCKED_H   (1<<18)
#define SYS_CTRL_AUDIOPLL_LOCKED_H (1<<19)
#define SYS_CTRL_BBPLL2_LOCKED_H   (1<<20)
#define SYS_CTRL_BBPLL1_LOCKED_H   (1<<21)
#define SYS_CTRL_USBPLL_LOCKED_H   (1<<22)

//Cfg_Clk_Out
#define SYS_CTRL_CLKOUT_DIVIDER(n) (((n)&31)<<0)
#define SYS_CTRL_CLKOUT_DBG_SEL    (1<<5)
#define SYS_CTRL_CLKOUT_ENABLE     (1<<6)
#define SYS_CTRL_CLKOUT_UPDATE     (1<<7)

//Cfg_Clk_AudioBCK1_Div
#define SYS_CTRL_AUDIOBCK1_DIVIDER(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_AUDIOBCK1_UPDATE  (1<<11)

//Cfg_Clk_AudioBCK2_Div
#define SYS_CTRL_AUDIOBCK2_DIVIDER(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_AUDIOBCK2_UPDATE  (1<<11)

//Cfg_Clk_Uart
#define SYS_CTRL_UART_DIVIDER(n)   (((n)&0xFFFFFF)<<0)
#define SYS_CTRL_UART_DIVIDER_MASK (0xFFFFFF<<0)
#define SYS_CTRL_UART_DIVIDER_SHIFT (0)
#define SYS_CTRL_UART_DIVIDER_UPDATE (1<<24)
#define SYS_CTRL_UART_SEL_PLL      (1<<25)

//Cfg_Clk_PWM
#define SYS_CTRL_PWM_DIVIDER(n)    (((n)&0xFF)<<0)

//Cfg_Clk_Dbg_Div
#define SYS_CTRL_CLK_DBG_DIVIDER(n) (((n)&0x7FF)<<0)

//Cfg_Clk_Camera_Out
#define SYS_CTRL_CLK_CAMERA_OUT_EN_DISABLE (0<<0)
#define SYS_CTRL_CLK_CAMERA_OUT_EN_ENABLE (1<<0)
#define SYS_CTRL_CLK_CAMERA_OUT_EN_V_DISABLE (0)
#define SYS_CTRL_CLK_CAMERA_OUT_EN_V_ENABLE (1)
#define SYS_CTRL_CLK_CAMERA_DIV_SRC_SEL_26_MHZ (0<<1)
#define SYS_CTRL_CLK_CAMERA_DIV_SRC_SEL_156_MHZ (1<<1)
#define SYS_CTRL_CLK_CAMERA_DIV_SRC_SEL_V_26_MHZ (0)
#define SYS_CTRL_CLK_CAMERA_DIV_SRC_SEL_V_156_MHZ (1)
#define SYS_CTRL_CLK_CAMERA_OUT_DIV(n) (((n)&0x7FF)<<2)
#define SYS_CTRL_CLK_CAMERA_DIV_UPDATE (1<<13)
#define SYS_CTRL_CLK_SPI_CAM_EN_DISABLE (0<<14)
#define SYS_CTRL_CLK_SPI_CAM_EN_ENABLE (1<<14)
#define SYS_CTRL_CLK_SPI_CAM_EN_V_DISABLE (0)
#define SYS_CTRL_CLK_SPI_CAM_EN_V_ENABLE (1)
#define SYS_CTRL_CLK_SPI_CAM_POL   (1<<15)
#define SYS_CTRL_CLK_SPI_CAM_SEL   (1<<16)
#define SYS_CTRL_CLK_SPI_CAM_DIV(n) (((n)&0x7FF)<<17)
#define SYS_CTRL_CLK_SPI_CAM_DIV_UPDATE (1<<28)

//Reset_Cause
#define SYS_CTRL_WATCHDOG_RESET_1_HAPPENED (1<<0)
#define SYS_CTRL_WATCHDOG_RESET_1_NO (0<<0)
#define SYS_CTRL_WATCHDOG_RESET_1_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_1_V_NO (0)
#define SYS_CTRL_WATCHDOG_RESET_2_HAPPENED (1<<1)
#define SYS_CTRL_WATCHDOG_RESET_2_NO (0<<1)
#define SYS_CTRL_WATCHDOG_RESET_2_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_2_V_NO (0)
#define SYS_CTRL_WATCHDOG_RESET_3_HAPPENED (1<<2)
#define SYS_CTRL_WATCHDOG_RESET_3_NO (0<<2)
#define SYS_CTRL_WATCHDOG_RESET_3_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_3_V_NO (0)
#define SYS_CTRL_WATCHDOG_RESET_4_HAPPENED (1<<3)
#define SYS_CTRL_WATCHDOG_RESET_4_NO (0<<3)
#define SYS_CTRL_WATCHDOG_RESET_4_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_4_V_NO (0)
#define SYS_CTRL_WATCHDOG_RESET_RF_HAPPENED (1<<4)
#define SYS_CTRL_WATCHDOG_RESET_RF_NO (0<<4)
#define SYS_CTRL_WATCHDOG_RESET_RF_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_RF_V_NO (0)
#define SYS_CTRL_WATCHDOG_RESET_GGE_HAPPENED (1<<5)
#define SYS_CTRL_WATCHDOG_RESET_GGE_NO (0<<5)
#define SYS_CTRL_WATCHDOG_RESET_GGE_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_GGE_V_NO (0)
#define SYS_CTRL_WATCHDOG_RESET_ZSP_HAPPENED (1<<6)
#define SYS_CTRL_WATCHDOG_RESET_ZSP_NO (0<<6)
#define SYS_CTRL_WATCHDOG_RESET_ZSP_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_ZSP_V_NO (0)
#define SYS_CTRL_GLOBALSOFT_RESET_HAPPENED (1<<7)
#define SYS_CTRL_GLOBALSOFT_RESET_NO (0<<7)
#define SYS_CTRL_GLOBALSOFT_RESET_V_HAPPENED (1)
#define SYS_CTRL_GLOBALSOFT_RESET_V_NO (0)
#define SYS_CTRL_HOSTDEBUG_RESET_HAPPENED (1<<8)
#define SYS_CTRL_HOSTDEBUG_RESET_NO (0<<8)
#define SYS_CTRL_HOSTDEBUG_RESET_V_HAPPENED (1)
#define SYS_CTRL_HOSTDEBUG_RESET_V_NO (0)
#define SYS_CTRL_WATCHDOG_RESET_CP_HAPPENED (1<<9)
#define SYS_CTRL_WATCHDOG_RESET_CP_NO (0<<9)
#define SYS_CTRL_WATCHDOG_RESET_CP_V_HAPPENED (1)
#define SYS_CTRL_WATCHDOG_RESET_CP_V_NO (0)
#define SYS_CTRL_ALARMCAUSE_HAPPENED (1<<12)
#define SYS_CTRL_ALARMCAUSE_NO     (0<<12)
#define SYS_CTRL_ALARMCAUSE_V_HAPPENED (1)
#define SYS_CTRL_ALARMCAUSE_V_NO   (0)
#define SYS_CTRL_BOOT_MODE(n)      (((n)&0x3F)<<16)
#define SYS_CTRL_BOOT_MODE_MASK    (0x3F<<16)
#define SYS_CTRL_BOOT_MODE_SHIFT   (16)
#define SYS_CTRL_SW_BOOT_MODE(n)   (((n)&0x3F)<<22)
#define SYS_CTRL_SW_BOOT_MODE_MASK (0x3F<<22)
#define SYS_CTRL_SW_BOOT_MODE_SHIFT (22)
#define SYS_CTRL_FONCTIONAL_TEST_MODE (1<<31)

//WakeUp
#define SYS_CTRL_FORCE_WAKEUP      (1<<0)

//Ignore_Charger
#define SYS_CTRL_IGNORE_CHARGER    (1<<0)

//CFG_PLL_SPIFLASH_DIV
#define SYS_CTRL_CFG_PLL_SPIFLASH_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_SPIFLASH_DIV_UPDATE (1<<4)

//CFG_PLL_SPIFLASH1_DIV
#define SYS_CTRL_CFG_PLL_SPIFLASH1_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_SPIFLASH1_DIV_UPDATE (1<<4)

//CFG_PLL_MEM_BRIDGE_DIV
#define SYS_CTRL_CFG_MEM_BRIDGE_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_MEM_BRIDGE_DIV_UPDATE (1<<4)

//CFG_DBG_CLK_SOURCE_SEL
#define SYS_CTRL_CFG_DBG_CLK_SOURCE_SEL(n) (((n)&0x3F)<<0)

//CFG_CLK_494M_LTE_DIV
#define SYS_CTRL_CFG_CLK_494M_LTE_APLL_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_CLK_494M_LTE_APLL_DIV_UPDATE (1<<4)
#define SYS_CTRL_CFG_CLKSW_494M_LTE_USBPHY_SEL (1<<5)
#define SYS_CTRL_CFG_CLKSW_494M_CLK_SEL (1<<6)
#define SYS_CTRL_CFG_CLK_96M_26M_SEL (1<<7)
#define SYS_CTRL_CFG_CLK_96M_DIV(n) (((n)&0x7FFFFF)<<8)
#define SYS_CTRL_CFG_CLK_96M_DIV_UPDATE (1<<31)

//CFG_PLL_ISP_DIV
#define SYS_CTRL_CFG_PLL_ISP_DIV(n) (((n)&0xFFF)<<0)
#define SYS_CTRL_CFG_PLL_ISP_DIV_UPDATE (1<<12)

//CFG_PLL_PIX_DIV
#define SYS_CTRL_CFG_PLL_PIX_DIV(n) (((n)&0x7F)<<0)
#define SYS_CTRL_CFG_PLL_PIX_DIV_UPDATE (1<<7)

//CFG_PLL_SYS_AXI_DIV
#define SYS_CTRL_CFG_PLL_SYS_AXI_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_SYS_AXI_DIV_UPDATE (1<<4)

//CFG_PLL_SYS_AHB_APB_DIV
#define SYS_CTRL_CFG_PLL_AP_APB_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_AP_APB_DIV_UPDATE (1<<4)

//CFG_PLL_SYS_AHB_BTFM_DIV
#define SYS_CTRL_CFG_PLL_AHB_BTFM_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_AHB_BTFM_DIV_UPDATE (1<<4)

//CFG_PLL_CSI_DIV
#define SYS_CTRL_CFG_PLL_CSI_DIV(n) (((n)&0x7F)<<0)
#define SYS_CTRL_CFG_PLL_CSI_DIV_UPDATE (1<<7)

//CFG_PLL_SYS_SPIFLASH_DIV
#define SYS_CTRL_CFG_PLL_SYS_SPIFLASH_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_SYS_SPIFLASH_DIV_UPDATE (1<<4)

//CFG_PLL_CP_DIV
#define SYS_CTRL_CFG_PLL_CP_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_CP_DIV_UPDATE (1<<4)

//CFG_PLL_AP_CPU_DIV
#define SYS_CTRL_CFG_PLL_AP_CPU_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_AP_CPU_DIV_UPDATE (1<<4)

//CFG_AP_CPU_ACLKEN_DIV
#define SYS_CTRL_CFG_AP_CPU_ACLKEN_DIV(n) (((n)&7)<<0)
#define SYS_CTRL_CFG_AP_CPU_ACLKEN_DIV_UPDATE (1<<3)

//CFG_AP_CPU_DBGEN_DIV
#define SYS_CTRL_CFG_AP_CPU_DBGEN_DIV(n) (((n)&7)<<0)
#define SYS_CTRL_CFG_AP_CPU_DBGEN_DIV_UPDATE (1<<3)

//CFG_PLL_CP_CPU_DIV
#define SYS_CTRL_CFG_PLL_CP_CPU_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_PLL_CP_CPU_DIV_UPDATE (1<<4)

//CFG_CP_CPU_ACLKEN_DIV
#define SYS_CTRL_CFG_CP_CPU_ACLKEN_DIV(n) (((n)&7)<<0)
#define SYS_CTRL_CFG_CP_CPU_ACLKEN_DIV_UPDATE (1<<3)

//CFG_CP_CPU_DBGEN_DIV
#define SYS_CTRL_CFG_CP_CPU_DBGEN_DIV(n) (((n)&7)<<0)
#define SYS_CTRL_CFG_CP_CPU_DBGEN_DIV_UPDATE (1<<3)

//CFG_TRNG_CLKEN_DIV
#define SYS_CTRL_CFG_TRNG_CLKEN_DIV(n) (((n)&31)<<0)
#define SYS_CTRL_CFG_TRNG_CLKEN_DIV_UPDATE (1<<5)

//CFG_CORESIGHT_DIV
#define SYS_CTRL_CFG_CORESIGHT_DIV(n) (((n)&15)<<0)
#define SYS_CTRL_CFG_CORESIGHT_DIV_UPDATE (1<<4)

//CFG_USB11_48M_DIV
#define SYS_CTRL_CFG_USB11_48M_DIV(n) (((n)&31)<<0)
#define SYS_CTRL_CFG_USB11_48M_DIV_UPDATE (1<<5)

//CFG_PLL_USB
#define SYS_CTRL_CFG_PLL_USB_RESET (1<<0)
#define SYS_CTRL_CFG_PLL_USB_REV   (1<<1)

//CFG_VAD_DIV
#define SYS_CTRL_CFG_VAD_DIV(n)    (((n)&15)<<0)
#define SYS_CTRL_CFG_VAD_DIV_UPDATE (1<<4)

//Cfg_HMPROT
#define SYS_CTRL_HPROT_BBDMA(n)    (((n)&15)<<0)
#define SYS_CTRL_HPROT_AIF_IFC(n)  (((n)&15)<<4)
#define SYS_CTRL_HPROT_GOUDA(n)    (((n)&15)<<8)
#define SYS_CTRL_HPROT_AP_IFC(n)   (((n)&15)<<12)
#define SYS_CTRL_HPROT_USB(n)      (((n)&15)<<16)
#define SYS_CTRL_HPROT_SYS_AON_IFC(n) (((n)&15)<<20)
#define SYS_CTRL_HSPROT_PSRAM_CTRL(n) (((n)&15)<<24)

//Cfg_MEM_CQ
#define SYS_CTRL_MEM_EMA_CQ(n)     (((n)&0xFFFFFFFF)<<0)

//Cfg_ARM_SYS_CFG_MEM
#define SYS_CTRL_ARM_SYS_CFG_MEM(n) (((n)&0xFFFFFFFF)<<0)

//Cfg_AUDIO_CFG_MEM
#define SYS_CTRL_AUDIO_CFG_MEM(n)  (((n)&0xFFFFFFFF)<<0)

//Cfg_LCD_CFG_MEM
#define SYS_CTRL_LCD_CFG_MEM(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_CAM_CFG_MEM
#define SYS_CTRL_CAM_CFG_MEM(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_PERI_CFG_MEM
#define SYS_CTRL_PERI_CFG_MEM(n)   (((n)&0xFFFFFFFF)<<0)

//Cfg_AON_SYS_CFG_MEM
#define SYS_CTRL_AON_SYS_CFG_MEM(n) (((n)&0xFFFFFFFF)<<0)

//Cfg_RF_SYS_CFG_MEM
#define SYS_CTRL_RF_SYS_CFG_MEM(n) (((n)&0xFFFFFFFF)<<0)

//Cfg_CORESIGHT_CFG_MEM
#define SYS_CTRL_CORESIGHT_CFG_MEM(n) (((n)&0xFFFFFFFF)<<0)

//Cfg_VAD_CFG_MEM
#define SYS_CTRL_VAD_CFG_MEM(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_AIF_CFG
#define SYS_CTRL_AIF1_LOAD_POS(n)  (((n)&0x3F)<<0)
#define SYS_CTRL_AIF2_LOAD_POS(n)  (((n)&0x3F)<<6)
#define SYS_CTRL_AIF1_SEL(n)       (((n)&7)<<12)
#define SYS_CTRL_AIF2_SEL(n)       (((n)&7)<<15)
#define SYS_CTRL_I2S1_SEL(n)       (((n)&7)<<18)
#define SYS_CTRL_I2S2_SEL(n)       (((n)&7)<<21)
#define SYS_CTRL_I2S3_SEL(n)       (((n)&7)<<24)
#define SYS_CTRL_I2S1_BCK_LRCK_OEN (1<<27)
#define SYS_CTRL_I2S2_BCK_LRCK_OEN (1<<28)
#define SYS_CTRL_I2S3_BCK_LRCK_OEN (1<<29)

//Cfg_MISC_CFG
#define SYS_CTRL_LIMIT_EN_SPI1     (1<<0)
#define SYS_CTRL_LIMIT_EN_SPI2     (1<<1)
#define SYS_CTRL_LIMIT_EN_SPI3     (1<<2)
#define SYS_CTRL_CFGSDISABLE_GIC400 (1<<3)
#define SYS_CTRL_AIF1_I2S_BCK_SEL(n) (((n)&7)<<4)
#define SYS_CTRL_AIF2_I2S_BCK_SEL(n) (((n)&7)<<8)
#define SYS_CTRL_WCN_UART_OUT_SEL  (1<<11)
#define SYS_CTRL_AP_UART_OUT_SEL   (1<<12)
#define SYS_CTRL_CFG_MODE_LP       (1<<13)
#define SYS_CTRL_CFG_FORCE_LP      (1<<14)
#define SYS_CTRL_CFG_NUMBER_LP(n)  (((n)&0xFFFF)<<15)
#define SYS_CTRL_BCPU_STALL_ACK    (1<<31)

//Cfg_MISC1_CFG
#define SYS_CTRL_DEBUG_MON_SEL(n)  (((n)&31)<<0)
#define SYS_CTRL_IOMUX_CLK_FORCE_ON (1<<5)
#define SYS_CTRL_AP_RST_CTRL       (1<<6)
#define SYS_CTRL_GGE_RST_CTRL      (1<<7)
#define SYS_CTRL_BTFM_RST_CTRL     (1<<8)
#define SYS_CTRL_AP_CLK_CTRL       (1<<9)
#define SYS_CTRL_GGE_CLK_CTRL      (1<<10)
#define SYS_CTRL_BTFM_CLK_CTRL     (1<<11)
#define SYS_CTRL_BBPLL1_ENABLE     (1<<12)
#define SYS_CTRL_BBPLL2_ENABLE     (1<<13)
#define SYS_CTRL_MEMPLL_ENABLE     (1<<14)
#define SYS_CTRL_USBPLL_ENABLE     (1<<15)
#define SYS_CTRL_AUDIOPLL_ENABLE   (1<<16)
#define SYS_CTRL_APLL_ZSP_CLK_SEL  (1<<17)
#define SYS_CTRL_APLL_BBPLL2_CLK_SEL (1<<18)
#define SYS_CTRL_REG_GIC400_ARUSER_SEL (1<<19)
#define SYS_CTRL_REG_GIC400_ARUSER_DBG (1<<20)
#define SYS_CTRL_REG_GIC400_AWUSER_SEL (1<<21)
#define SYS_CTRL_REG_GIC400_AWUSER_DBG (1<<22)
#define SYS_CTRL_LVDS_WCN_RFDIG_SEL (1<<23)
#define SYS_CTRL_LVDS_RFDIG_RF_BB_SEL (1<<24)
#define SYS_CTRL_WCN_OSC_EN_CTRL   (1<<25)
#define SYS_CTRL_VAD_CLK_FORCE_ON  (1<<26)
#define SYS_CTRL_VAD_CLK_PN_SEL    (1<<27)
#define SYS_CTRL_AUD_SCLK_O_PN_SEL (1<<28)
#define SYS_CTRL_AON_LP_RST_CTRL   (1<<29)
#define SYS_CTRL_AON_LP_CLK_CTRL   (1<<30)
#define SYS_CTRL_DUMP_PATH         (1<<31)

//Cfg_FORCE_LP_MODE_LP
#define SYS_CTRL_CFG_FORCE_LP_A5   (1<<0)
#define SYS_CTRL_CFG_FORCE_LP_AHB  (1<<1)
#define SYS_CTRL_CFG_FORCE_LP_CONNECT (1<<2)
#define SYS_CTRL_CFG_FORCE_LP_CP_A5 (1<<3)
#define SYS_CTRL_CFG_FORCE_LP_PSRAM (1<<4)
#define SYS_CTRL_CFG_FORCE_LP_SPIFLASH (1<<5)
#define SYS_CTRL_CFG_FORCE_LP_SPIFLASH1 (1<<6)
#define SYS_CTRL_CFG_MODE_LP_A5    (1<<7)
#define SYS_CTRL_CFG_MODE_LP_AHB   (1<<8)
#define SYS_CTRL_CFG_MODE_LP_CONNECT (1<<9)
#define SYS_CTRL_CFG_MODE_LP_CP_A5 (1<<10)
#define SYS_CTRL_CFG_MODE_LP_PSRAM (1<<11)
#define SYS_CTRL_CFG_MODE_LP_SPIFLASH (1<<12)
#define SYS_CTRL_CFG_MODE_LP_SPIFLASH1 (1<<13)
#define SYS_CTRL_CFG_MODE_LP_AHB_MERGE (1<<14)
#define SYS_CTRL_FORCE_LP_AHB_MERGE (1<<15)

//Cfg_Reserve
#define SYS_CTRL_WD_RST_MODE       (1<<0)
#define SYS_CTRL_SYS_BB_SIDE       (1<<1)
#define SYS_CTRL_CAM_OUT0_SEL      (1<<2)
#define SYS_CTRL_VPU_CLK_EN        (1<<3)
#define SYS_CTRL_WD_1_RST_EN       (1<<4)
#define SYS_CTRL_WD_2_RST_EN       (1<<5)
#define SYS_CTRL_WD_3_RST_EN       (1<<6)
#define SYS_CTRL_WD_4_RST_EN       (1<<7)
#define SYS_CTRL_WD_RF_RST_EN      (1<<8)
#define SYS_CTRL_WD_GGE_RST_EN     (1<<9)
#define SYS_CTRL_WD_ZSP_RST_EN     (1<<10)
#define SYS_CTRL_WD_CP_RST_EN      (1<<11)
#define SYS_CTRL_DMC_PHY_RST_EN    (1<<12)
#define SYS_CTRL_HMPROT_WCN_PERI(n) (((n)&15)<<13)
#define SYS_CTRL_HMPROT_WCN_MEM(n) (((n)&15)<<17)
#define SYS_CTRL_HMPROT_AES(n)     (((n)&15)<<21)
#define SYS_CTRL_RESERVE(n)        (((n)&0x7F)<<25)

//Cfg_Reserve1
#define SYS_CTRL_RESERVE1(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve2
#define SYS_CTRL_RESERVE2(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve3
#define SYS_CTRL_RESERVE3(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve4
#define SYS_CTRL_RESERVE4(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve5
#define SYS_CTRL_RESERVE5(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve6
#define SYS_CTRL_RESERVE6(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve7
#define SYS_CTRL_RESERVE7(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve8
#define SYS_CTRL_RESERVE8(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve9
#define SYS_CTRL_RESERVE9(n)       (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve10
#define SYS_CTRL_RESERVE10(n)      (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve11
#define SYS_CTRL_RESERVE11(n)      (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve12
#define SYS_CTRL_RESERVE12(n)      (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve13
#define SYS_CTRL_RESERVE13(n)      (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve14
#define SYS_CTRL_RESERVE14(n)      (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve15
#define SYS_CTRL_RESERVE15(n)      (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve16
#define SYS_CTRL_RESERVE16(n)      (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve1_in
#define SYS_CTRL_RESERVE1_IN(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve2_in
#define SYS_CTRL_RESERVE2_IN(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve3_in
#define SYS_CTRL_RESERVE3_IN(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve4_in
#define SYS_CTRL_RESERVE4_IN(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve5_in
#define SYS_CTRL_RESERVE5_IN(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve6_in
#define SYS_CTRL_RESERVE6_IN(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_Reserve7_in
#define SYS_CTRL_RESERVE7_IN(n)    (((n)&0xFFFFFFFF)<<0)

//Cfg_CHIP_PROD_ID
#define SYS_CTRL_METAL_ID(n)       (((n)&0xFFF)<<0)
#define SYS_CTRL_BOND_ID(n)        (((n)&15)<<12)
#define SYS_CTRL_PROD_ID(n)        (((n)&0xFFFF)<<16)

//Cfg_QOS_WCN_A5_GGE
#define SYS_CTRL_WCN_MEM_ARQOS(n)  (((n)&31)<<0)
#define SYS_CTRL_WCN_MEM_AWQOS(n)  (((n)&31)<<5)
#define SYS_CTRL_GGE_ARQOS(n)      (((n)&31)<<10)
#define SYS_CTRL_GGE_AWQOS(n)      (((n)&31)<<15)
#define SYS_CTRL_A5_ARQOS(n)       (((n)&31)<<20)
#define SYS_CTRL_A5_AWQOS(n)       (((n)&31)<<25)

//Cfg_QOS_AXIDMA_CPA5_F8
#define SYS_CTRL_AXIDMA_ARQOS(n)   (((n)&31)<<0)
#define SYS_CTRL_AXIDMA_AWQOS(n)   (((n)&31)<<5)
#define SYS_CTRL_CP_A5_ARQOS(n)    (((n)&31)<<10)
#define SYS_CTRL_CP_A5_AWQOS(n)    (((n)&31)<<15)
#define SYS_CTRL_F8_ARQOS(n)       (((n)&31)<<20)
#define SYS_CTRL_F8_AWQOS(n)       (((n)&31)<<25)

//Cfg_QOS_LCDC_LZMA_GOUDA
#define SYS_CTRL_LCDC_ARQOS(n)     (((n)&31)<<0)
#define SYS_CTRL_LCDC_AWQOS(n)     (((n)&31)<<5)
#define SYS_CTRL_LZMA_ARQOS(n)     (((n)&31)<<10)
#define SYS_CTRL_LZMA_AWQOS(n)     (((n)&31)<<15)
#define SYS_CTRL_GOUDA_ARQOS(n)    (((n)&31)<<20)
#define SYS_CTRL_GOUDA_AWQOS(n)    (((n)&31)<<25)

//Cfg_QOS_LTE_USB
#define SYS_CTRL_LTE_ARQOS(n)      (((n)&31)<<0)
#define SYS_CTRL_LTE_AWQOS(n)      (((n)&31)<<5)
#define SYS_CTRL_USB_ARQOS(n)      (((n)&31)<<10)
#define SYS_CTRL_USB_AWQOS(n)      (((n)&31)<<15)

//Cfg_QOS_MERGE_MEM
#define SYS_CTRL_MERGE_MEM_AWQOS(n) (((n)&31)<<0)
#define SYS_CTRL_MERGE_MEM_ARQOS(n) (((n)&31)<<5)

//Cfg_BCPU_DBG_BKP
#define SYS_CTRL_BCPU_BKPT_ADDR(n) (((n)&0xFFFFFFF)<<0)
#define SYS_CTRL_BCPU_BKPT_MODE(n) (((n)&3)<<28)
#define SYS_CTRL_BCPU_BKPT_EN      (1<<30)
#define SYS_CTRL_BCPU_STALLED_W1C  (1<<31)





#endif

