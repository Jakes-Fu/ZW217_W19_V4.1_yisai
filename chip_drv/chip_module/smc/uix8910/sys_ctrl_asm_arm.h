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


#ifndef _SYS_CTRL_ASM_ARM_H_
#define _SYS_CTRL_ASM_ARM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

//#include "globals_asm_arm.h"

/// CPU IDs
#define A5CPU                                   (0)
/// Sys Axi Clks IDs
#define SYS_AXI_CLK_ID_IMEM                     (0)
#define SYS_AXI_CLK_ID_LZMA                     (1)
/// reserved base number
#define NB_SYS_AXI_CLK_REV                      (2)
#define SYS_AXI_CLK_ID_REV0                     (2)
#define SYS_AXI_CLK_ID_REV1                     (3)
#define SYS_AXI_CLK_ID_REV2                     (4)
#define SYS_AXI_CLK_ID_SDMMC1                   (5)
/// auto clock enable number
#define NB_SYS_AXI_CLK_AEN                      (6)
#define SYS_AXI_CLK_ID_ALWAYS                   (6)
#define NB_SYS_AXI_CLK_EN                       (7)
#define NB_SYS_AXI_CLK                          (7)
/// Sys Ahb Clks IDs
#define SYS_AHB_CLK_ID_GOUDA                    (0)
#define SYS_AHB_CLK_ID_GGE                      (1)
#define SYS_AHB_CLK_ID_GEA3                     (2)
/// reserved base number
#define NB_SYS_AHB_CLK_REV                      (3)
#define SYS_AHB_CLK_ID_LZMA                     (3)
#define SYS_AHB_CLK_ID_F8                       (4)
#define SYS_AHB_CLK_ID_AXIDMA                   (5)
/// auto clock enable number
#define NB_SYS_AHB_CLK_AEN                      (6)
#define SYS_AHB_CLK_ID_USBC                     (6)
#define SYS_AHB_CLK_ID_LCD                      (7)
#define SYS_AHB_CLK_ID_ALWAYS                   (8)
#define NB_SYS_AHB_CLK_EN                       (9)
#define NB_SYS_AHB_CLK                          (9)
/// Sys Apb Clks IDs
#define AP_APB_CLK_ID_CONF                      (0)
#define AP_APB_CLK_ID_CAMERA                    (1)
#define AP_APB_CLK_ID_I2C1                      (2)
#define AP_APB_CLK_ID_I2C2                      (3)
#define AP_APB_CLK_ID_I2C3                      (4)
#define AP_APB_CLK_ID_IFC                       (5)
#define AP_APB_CLK_ID_IFC_CH0                   (6)
#define AP_APB_CLK_ID_IFC_CH1                   (7)
#define AP_APB_CLK_ID_IFC_CH2                   (8)
#define AP_APB_CLK_ID_IFC_CH3                   (9)
#define AP_APB_CLK_ID_IFC_CH4                   (10)
#define AP_APB_CLK_ID_IFC_CH5                   (11)
#define AP_APB_CLK_ID_IFC_CH6                   (12)
#define AP_APB_CLK_ID_IFC_CHDBG                 (13)
#define AP_APB_CLK_ID_REV2                      (14)
#define AP_APB_CLK_ID_SDMMC2                    (15)
#define AP_APB_CLK_ID_SPI1                      (16)
#define AP_APB_CLK_ID_SPI2                      (17)
#define AP_APB_CLK_ID_SPI3                      (18)
#define AP_APB_CLK_ID_UART1                     (19)
#define AP_APB_CLK_ID_UART2                     (20)
#define AP_APB_CLK_ID_UART3                     (21)
/// reserved base number
#define NB_AP_APB_CLK_REV                       (22)
#define AP_APB_CLK_ID_REV0                      (22)
#define AP_APB_CLK_ID_REV1                      (23)
#define AP_APB_CLK_ID_PAGESPY                   (24)
/// auto clock enable number
#define NB_AP_APB_CLK_AEN                       (25)
#define AP_APB_CLK_ID_LZMA                      (25)
#define AP_APB_CLK_ID_PSRAM                     (26)
#define AP_APB_CLK_ID_TIMER1                    (27)
#define AP_APB_CLK_ID_TIMER2                    (28)
#define AP_APB_CLK_ID_TIMER4                    (29)
#define AP_APB_CLK_ID_LCD                       (30)
#define AP_APB_CLK_ID_ALWAYS                    (31)
#define NB_AP_APB_CLK_EN                        (32)
#define NB_AP_APB_CLK                           (32)
/// Aif Apb Clks IDs
#define AIF_APB_CLK_ID_CONF                     (0)
#define AIF1_APB_CLK_ID_AIF                     (1)
#define AIF2_APB_CLK_ID_AIF                     (2)
#define AIF_APB_CLK_ID_IFC                      (3)
#define AIF_APB_CLK_ID_IFC_CH0                  (4)
#define AIF_APB_CLK_ID_IFC_CH1                  (5)
#define AIF_APB_CLK_ID_IFC_CH2                  (6)
#define AIF_APB_CLK_ID_IFC_CH3                  (7)
/// reserved base number
#define NB_AIF_APB_CLK_REV                      (8)
#define AIF_APB_CLK_ID_REV0                     (8)
#define AIF_APB_CLK_ID_REV1                     (9)
#define AIF_APB_CLK_ID_REV2                     (10)
#define AIF_APB_CLK_ID_AUD_2AD                  (11)
/// auto clock enable number
#define NB_AIF_APB_CLK_AEN                      (12)
#define AIF_APB_CLK_ID_ALWAYS                   (12)
#define NB_AIF_APB_CLK_EN                       (13)
#define NB_AIF_APB_CLK                          (13)
/// Aon Ahb Clks IDs
#define AON_AHB_CLK_ID_BB_SYSCTRL               (0)
/// reserved base number
#define NB_AON_AHB_CLK_REV                      (1)
#define AON_AHB_CLK_ID_BB_REV0                  (1)
#define AON_AHB_CLK_ID_BB_REV1                  (2)
#define AON_AHB_CLK_ID_BB_REV2                  (3)
#define AON_AHB_CLK_ID_BB_REV3                  (4)
/// auto clock enable number
#define NB_AON_AHB_CLK_AEN                      (5)
#define AON_AHB_CLK_ID_ALWAYS                   (5)
#define NB_AON_AHB_CLK_EN                       (6)
#define NB_AON_AHB_CLK                          (6)
/// Aon Apb Clks IDs
#define AON_APB_CLK_ID_CONF                     (0)
#define AON_APB_CLK_ID_DBG_HOST                 (1)
#define AON_APB_CLK_ID_DBG_UART                 (2)
#define AON_APB_CLK_ID_IFC                      (3)
#define AON_APB_CLK_ID_IFC_CH0                  (4)
#define AON_APB_CLK_ID_IFC_CH1                  (5)
#define AON_APB_CLK_ID_IFC_CH2                  (6)
#define AON_APB_CLK_ID_IFC_CH3                  (7)
#define AON_APB_CLK_ID_IFC_CH4                  (8)
#define AON_APB_CLK_ID_IFC_CH5                  (9)
#define AON_APB_CLK_ID_IFC_CH6                  (10)
#define AON_APB_CLK_ID_IFC_CH7                  (11)
#define AON_APB_CLK_ID_IFC_CHDBG                (12)
#define AON_APB_CLK_ID_SCID1                    (13)
#define AON_APB_CLK_ID_SCID2                    (14)
#define AON_APB_CLK_ID_PWM                      (15)
#define AON_APB_CLK_ID_I2C                      (16)
/// reserved base number
#define NB_AON_APB_CLK_REV                      (17)
#define AON_APB_CLK_ID_ANA_REG                  (17)
#define AON_APB_CLK_ID_SPINLOCK                 (18)
#define AON_APB_CLK_ID_LPS                      (19)
#define AON_APB_CLK_ID_EFUSE                    (20)
/// auto clock enable number
#define NB_AON_APB_CLK_AEN                      (21)
#define AON_APB_CLK_ID_SCI1                     (21)
#define AON_APB_CLK_ID_SCI2                     (22)
#define AON_APB_CLK_ID_GPIO                     (23)
#define AON_APB_CLK_ID_TIMER3                   (24)
#define AON_APB_CLK_ID_KEYPAD                   (25)
#define AON_APB_CLK_ID_AHB2APB_ADI              (26)
#define AON_APB_CLK_ID_ADI                      (27)
#define AON_APB_CLK_ID_LVDS                     (28)
#define AON_APB_CLK_ID_ALWAYS                   (29)
#define NB_AON_APB_CLK_EN                       (30)
#define NB_AON_APB_CLK                          (30)
/// Rf Ahb Clks IDs
#define RF_AHB_CLK_ID_RF_RAM                    (0)
#define RF_AHB_CLK_ID_RISCV                     (1)
#define RF_AHB_CLK_ID_RISCV_APB                 (2)
#define RF_AHB_CLK_ID_RISCV_JTAG                (3)
#define RF_AHB_CLK_ID_SPI2AHB                   (4)
#define RF_AHB_CLK_ID_RF_APB                    (5)
#define RF_AHB_CLK_ID_REV0                      (6)
#define RF_AHB_CLK_ID_REV1                      (7)
#define RF_AHB_CLK_ID_REV2                      (8)
#define RF_AHB_CLK_ID_REV3                      (9)
/// auto clock enable number
#define NB_RF_AHB_CLK_AEN                       (10)
#define RF_AHB_CLK_ID_ALWAYS                    (10)
#define NB_RF_AHB_CLK_EN                        (11)
#define NB_RF_AHB_CLK                           (11)
/// Rf Apb Clks IDs
#define RF_APB_CLK_ID_CONF                      (0)
#define RF_APB_CLK_ID_IRQ_CTRL                  (1)
#define RF_APB_CLK_ID_TIMER0                    (2)
#define RF_APB_CLK_ID_UART0                     (3)
#define RF_APB_CLK_ID_REV0                      (4)
#define RF_APB_CLK_ID_REV1                      (5)
#define RF_APB_CLK_ID_REV2                      (6)
#define RF_APB_CLK_ID_REV3                      (7)
/// auto clock enable number
#define NB_RF_APB_CLK_AEN                       (8)
#define RF_APB_CLK_ID_ALWAYS                    (8)
#define NB_RF_APB_CLK_EN                        (9)
#define NB_RF_APB_CLK                           (9)
/// Other Clks IDs
#define OC_ID_HOST_UART                         (0)
#define OC_ID_BCK1                              (1)
#define OC_ID_BCK2                              (2)
#define OC_ID_UART1                             (3)
#define OC_ID_UART2                             (4)
#define OC_ID_UART3                             (5)
#define OC_ID_AP_A5                             (6)
#define OC_ID_CP_A5                             (7)
#define NB_CLK_OTHER_AEN                        (8)
#define OC_ID_GPIO                              (8)
#define OC_ID_USBPHY                            (9)
#define OC_ID_PIX                               (10)
#define OC_ID_CLK_OUT                           (11)
#define OC_ID_ISP                               (12)
/// System Spiflash Domain Clock ID Base
#define OC_ID_SYS_SPIFLASH_BASE                 (13)
#define OC_ID_SYS_SPIFLASH                      (13)
#define OC_ID_SYS_SPIFLASH_ALWAYS               (14)
#define OC_ID_SYS_SPIFLASH1                     (15)
#define OC_ID_SYS_SPIFLASH1_ALWAYS              (16)
#define OC_ID_SPIFLASH                          (17)
#define OC_ID_SPIFLASH1                         (18)
#define OC_ID_SPICAM                            (19)
#define OC_ID_CAM                               (20)
/// Psram Ctrl Domain Clock ID Base
#define OC_ID_PSRAM_BASE                        (21)
#define OC_ID_PSRAM_CONF                        (21)
#define OC_ID_PSRAM_DMC                         (22)
#define OC_ID_PSRAM_PAGESPY                     (23)
#define OC_ID_PSRAM_ALWAYS                      (24)
#define NB_CLK_OTHER_EN                         (25)
#define NB_CLK_OTHER                            (25)
/// Other Clks 1 IDs
#define OC_ID_GGE_CLK32K                        (0)
#define OC_ID_GGE_26M                           (1)
#define OC_ID_BT_FM_CLK32K                      (2)
#define OC_ID_BT_FM_26M                         (3)
#define OC_ID_PMIC_CLK32K                       (4)
#define OC_ID_PMIC_26M                          (5)
#define OC_ID_CP_CLK32K                         (6)
#define OC_ID_CP_26M                            (7)
#define OC_ID_CP_96M                            (8)
#define OC_ID_CP_BBLTE                          (9)
#define OC_ID_494M_LTE                          (10)
#define OC_ID_REV0                              (11)
#define OC_ID_REV1                              (12)
#define OC_ID_REV2                              (13)
#define OC_ID_REV3                              (14)
#define NB_CLK_OTHER_1_AEN                      (15)
#define OC_ID_SDM_26M                           (15)
#define OC_ID_LPS                               (16)
#define OC_ID_EFUSE_26M                         (17)
#define OC_ID_USB_ADP_32K                       (18)
#define OC_ID_USB_UTMI_48M                      (19)
#define OC_ID_AP_26M                            (20)
#define OC_ID_AP_32K                            (21)
#define OC_ID_MIPIDSI                           (22)
#define OC_ID_AHB_BTFM                          (23)
#define NB_CLK_OTHER_1_EN                       (24)
#define NB_CLK_OTHER_1                          (24)
/// Psram Clks IDs
#define PSRAM_CLK_ID_CONF                       ((OC_ID_PSRAM_CONF-OC_ID_PSRAM_BASE))
#define PSRAM_CLK_ID_DMC                        ((OC_ID_PSRAM_DMC-OC_ID_PSRAM_BASE))
#define PSRAM_CLK_ID_PAGESPY                    ((OC_ID_PSRAM_PAGESPY-OC_ID_PSRAM_BASE))
#define PSRAM_CLK_ID_ALWAYS                     ((OC_ID_PSRAM_ALWAYS-OC_ID_PSRAM_BASE))
#define NB_CLK_PSRAM                            (4)
/// Sys Spiflash Clks IDs
#define SPIFLASH_CLK_ID_SPIFLASH                ((OC_ID_SYS_SPIFLASH-OC_ID_SYS_SPIFLASH_BASE))
#define SPIFLASH_CLK_ID_ALWAYS                  ((OC_ID_SYS_SPIFLASH_ALWAYS-OC_ID_SYS_SPIFLASH_BASE))
/// Sys Spiflash1 Clks IDs
#define SPIFLASH_CLK_ID_SPIFLASH1               ((OC_ID_SYS_SPIFLASH1-OC_ID_SYS_SPIFLASH_BASE))
#define SPIFLASH1_CLK_ID_ALWAYS                 ((OC_ID_SYS_SPIFLASH1_ALWAYS-OC_ID_SYS_SPIFLASH_BASE))
/// Sys Axi Rst IDs
#define SYS_AXI_RST_ID_SYS                      (0)
#define SYS_AXI_RST_ID_IMEM                     (1)
#define SYS_AXI_RST_ID_LZMA                     (2)
#define SYS_AXI_RST_ID_REV0                     (3)
#define SYS_AXI_RST_ID_REV1                     (4)
#define SYS_AXI_RST_ID_REV2                     (5)
#define SYS_AXI_RST_ID_REV3                     (6)
#define NB_SYS_AXI_RST                          (7)
/// Sys Ahb Rst IDs
#define SYS_AHB_RST_ID_SYS                      (0)
#define SYS_AHB_RST_ID_GOUDA                    (1)
#define SYS_AHB_RST_ID_GGE                      (2)
#define SYS_AHB_RST_ID_GEA3                     (3)
#define SYS_AHB_RST_ID_USBC                     (4)
#define SYS_AHB_RST_ID_BTFM                     (5)
#define SYS_AHB_RST_ID_GIC400                   (6)
#define SYS_AHB_RST_ID_F8                       (7)
#define SYS_AHB_RST_ID_AXIDMA                   (8)
#define SYS_AHB_RST_ID_LZMA                     (9)
#define SYS_AHB_RST_ID_LCD                      (10)
#define NB_SYS_AHB_RST                          (11)
/// Sys Apb Rst IDs
#define AP_APB_RST_ID_SYS                       (0)
#define AP_APB_RST_ID_CAMERA                    (1)
#define AP_APB_RST_ID_I2C1                      (2)
#define AP_APB_RST_ID_I2C2                      (3)
#define AP_APB_RST_ID_I2C3                      (4)
#define AP_APB_RST_ID_IFC                       (5)
#define AP_APB_RST_ID_IMEM                      (6)
#define AP_APB_RST_ID_SDMMC1                    (7)
#define AP_APB_RST_ID_SDMMC2                    (8)
#define AP_APB_RST_ID_SPI1                      (9)
#define AP_APB_RST_ID_SPI2                      (10)
#define AP_APB_RST_ID_SPI3                      (11)
#define AP_APB_RST_ID_UART1                     (12)
#define AP_APB_RST_ID_UART2                     (13)
#define AP_APB_RST_ID_UART3                     (14)
#define AP_APB_RST_ID_REV0                      (15)
#define AP_APB_RST_ID_REV1                      (16)
#define AP_APB_RST_ID_REV2                      (17)
#define AP_APB_RST_ID_REV3                      (18)
#define AP_APB_RST_ID_PAGESPY                   (19)
#define AP_APB_RST_ID_LZMA                      (20)
#define AP_APB_RST_ID_PSRAM                     (21)
#define AP_APB_RST_ID_TIMER1                    (22)
#define AP_APB_RST_ID_TIMER2                    (23)
#define AP_APB_RST_ID_TIMER4                    (24)
#define AP_APB_RST_ID_GOUDA                     (25)
#define NB_AP_APB_RST                           (26)
/// Aif Apb Rst IDs
#define AIF1_APB_RST_ID_SYS                     (0)
#define AIF2_APB_RST_ID_SYS                     (1)
#define AIF1_APB_RST_ID_AIF                     (2)
#define AIF2_APB_RST_ID_AIF                     (3)
#define AIF_APB_RST_ID_IFC                      (4)
#define AIF_APB_RST_ID_AUD_2AD                  (5)
#define NB_AIF_APB_RST                          (6)
/// Aon Ahb Rst IDs
#define AON_AHB_RST_ID_SYS                      (0)
#define AON_AHB_RST_ID_REV0                     (1)
#define AON_AHB_RST_ID_REV1                     (2)
#define AON_AHB_RST_ID_REV2                     (3)
#define AON_AHB_RST_ID_REV3                     (4)
#define NB_AON_AHB_RST                          (5)
/// Aon Apb Rst IDs
#define AON_APB_RST_ID_SYS                      (0)
#define AON_APB_RST_ID_CALENDAR                 (1)
#define AON_APB_RST_ID_GPIO                     (2)
#define AON_APB_RST_ID_IFC                      (3)
#define AON_APB_RST_ID_KEYPAD                   (4)
#define AON_APB_RST_ID_PWM                      (5)
#define AON_APB_RST_ID_SCI1                     (6)
#define AON_APB_RST_ID_SCI2                     (7)
#define AON_APB_RST_ID_TIMER3                   (8)
#define AON_APB_RST_ID_I2C                      (9)
#define AON_APB_RST_ID_ANA_REG                  (10)
#define AON_APB_RST_ID_SPINLOCK                 (11)
#define AON_APB_RST_ID_LPS                      (12)
#define AON_APB_RST_ID_EFUSE                    (13)
#define AON_APB_RST_ID_AHB2APB_ADI              (14)
#define AON_APB_RST_ID_ADI                      (15)
#define AON_APB_RST_ID_LVDS                     (16)
#define NB_AON_APB_RST                          (17)
/// Rf Ahb Rst IDs
#define RF_AHB_RST_ID_SYS                       (0)
#define RF_AHB_RST_ID_RF_RAM                    (1)
#define RF_AHB_RST_ID_RISCV                     (2)
#define RF_AHB_RST_ID_RISCV_APB                 (3)
#define RF_AHB_RST_ID_RISCV_DBG                 (4)
#define RF_AHB_RST_ID_RISCV_JTAG                (5)
#define RF_AHB_RST_ID_SPI2AHB                   (6)
#define RF_AHB_RST_ID_RF_APB                    (7)
#define RF_AHB_RST_ID_RF_REV0                   (8)
#define RF_AHB_RST_ID_RF_REV1                   (9)
#define RF_AHB_RST_ID_RF_REV2                   (10)
#define RF_AHB_RST_ID_RF_REV3                   (11)
#define NB_RF_AHB_RST                           (12)
/// Rf Apb Rst IDs
#define RF_APB_RST_ID_SYS                       (0)
#define RF_APB_RST_ID_IRQ_CTRL                  (1)
#define RF_APB_RST_ID_TIMER0                    (2)
#define RF_APB_RST_ID_UART0                     (3)
#define RF_APB_RST_ID_REV0                      (4)
#define RF_APB_RST_ID_REV1                      (5)
#define RF_APB_RST_ID_REV2                      (6)
#define RF_APB_RST_ID_REV3                      (7)
#define NB_RF_APB_RST                           (8)
/// APCPU Rst IDs
#define APCPU_RST_ID_SYS                        (0)
#define APCPU_RST_ID_CORE                       (1)
#define APCPU_RST_ID_DBG                        (2)
#define APCPU_RST_ID_GIC400                     (3)
#define NB_APCPU_RST                            (4)
#define GLOBAL_SOFT_RST                         (4)
/// CPCPU Rst IDs
#define CPCPU_RST_ID_SYS                        (0)
#define CPCPU_RST_ID_CORE                       (1)
#define CPCPU_RST_ID_DBG                        (2)
#define NB_CPCPU_RST                            (3)
/// BBlte Rst IDs
#define BBLTE_RST_ID_SYS                        (0)
#define BBLTE_RST_ID_REV0                       (1)
#define NB_BBLTE_RST                            (2)
/// Other Rsts IDs
#define RSTO_ID_BCK1                            (0)
#define RSTO_ID_BCK2                            (1)
#define RSTO_ID_DBG_HOST                        (2)
#define RSTO_ID_GPIO                            (3)
#define RSTO_ID_UART1                           (4)
#define RSTO_ID_UART2                           (5)
#define RSTO_ID_UART3                           (6)
#define RSTO_ID_USBC                            (7)
#define RSTO_ID_WDTIMER0                        (8)
#define RSTO_ID_WDTIMER1                        (9)
#define RSTO_ID_WDTIMER2                        (10)
#define RSTO_ID_SPIFLASH                        (11)
#define RSTO_ID_SPIFLASH_SYS                    (12)
#define RSTO_ID_SPIFLASH1                       (13)
#define RSTO_ID_SPIFLASH1_SYS                   (14)
#define RSTO_ID_PSRAM_DMC                       (15)
#define RSTO_ID_PSRAM_SYS                       (16)
#define RSTO_ID_PSRAM_PAGESPY                   (17)
#define RSTO_ID_PSRAM_APB                       (18)
#define RSTO_ID_PIX                             (19)
#define RSTO_ID_SDM_26M                         (20)
#define RSTO_ID_WDTIMER4                        (21)
#define RSTO_ID_RF_WDTIMER                      (22)
#define RSTO_ID_LPS                             (23)
#define RSTO_ID_EFUSE                           (24)
#define RSTO_ID_USB_ADP_32K                     (25)
#define RSTO_ID_MIPIDSI                         (26)
#define RSTO_ID_MIPIDSI_PHY                     (27)
#define RSTO_ID_AUD_2AD                         (28)
#define NB_RSTO                                 (29)
/// For REG_DBG protect lock/unlock value
#define SYS_CTRL_PROTECT_LOCK                   (0XA50000)
#define SYS_CTRL_PROTECT_UNLOCK                 (0XA50001)

//==============================================================================
// Cpu_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Axi_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Ahb_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Ap_Apb_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aif_Apb_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aon_Ahb_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aon_Apb_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Rf_Ahb_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Rf_Apb_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Others_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Others_Clks_1
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Psram_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Spiflash_Clks
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Axi_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Ahb_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Ap_Apb_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aif_Apb_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aon_Ahb_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aon_Apb_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Rf_Ahb_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Rf_Apb_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Apcpu_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Cpcpu_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Bblte_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Others_Rst
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// sys_ctrl
//------------------------------------------------------------------------------
///
//==============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_SYS_CTRL_BASE          0x05100000
#else
#define REG_SYS_CTRL_BASE          0x50100000
#endif

#define REG_SYS_CTRL_REG_DBG          0x00000000
#define REG_SYS_CTRL_SYS_AXI_RST_SET    0x00000004
#define REG_SYS_CTRL_SYS_AXI_RST_CLR    0x00000008
#define REG_SYS_CTRL_SYS_AHB_RST_SET    0x0000000C
#define REG_SYS_CTRL_SYS_AHB_RST_CLR    0x00000010
#define REG_SYS_CTRL_AP_APB_RST_SET    0x00000014
#define REG_SYS_CTRL_AP_APB_RST_CLR    0x00000018
#define REG_SYS_CTRL_AIF_APB_RST_SET    0x0000001C
#define REG_SYS_CTRL_AIF_APB_RST_CLR    0x00000020
#define REG_SYS_CTRL_AON_AHB_RST_SET    0x00000024
#define REG_SYS_CTRL_AON_AHB_RST_CLR    0x00000028
#define REG_SYS_CTRL_AON_APB_RST_SET    0x0000002C
#define REG_SYS_CTRL_AON_APB_RST_CLR    0x00000030
#define REG_SYS_CTRL_RF_AHB_RST_SET    0x00000034
#define REG_SYS_CTRL_RF_AHB_RST_CLR    0x00000038
#define REG_SYS_CTRL_RF_APB_RST_SET    0x0000003C
#define REG_SYS_CTRL_RF_APB_RST_CLR    0x00000040
#define REG_SYS_CTRL_APCPU_RST_SET    0x00000044
#define REG_SYS_CTRL_APCPU_RST_CLR    0x00000048
#define REG_SYS_CTRL_CPCPU_RST_SET    0x0000004C
#define REG_SYS_CTRL_CPCPU_RST_CLR    0x00000050
#define REG_SYS_CTRL_BBLTE_RST_SET    0x00000054
#define REG_SYS_CTRL_BBLTE_RST_CLR    0x00000058
#define REG_SYS_CTRL_OTHERS_RST_SET    0x0000005C
#define REG_SYS_CTRL_OTHERS_RST_CLR    0x00000060
#define REG_SYS_CTRL_CLK_SYS_AXI_MODE    0x00000064
#define REG_SYS_CTRL_CLK_SYS_AXI_ENABLE    0x00000068
#define REG_SYS_CTRL_CLK_SYS_AXI_DISABLE    0x0000006C
#define REG_SYS_CTRL_CLK_SYS_AHB_MODE    0x00000070
#define REG_SYS_CTRL_CLK_SYS_AHB_ENABLE    0x00000074
#define REG_SYS_CTRL_CLK_SYS_AHB_DISABLE    0x00000078
#define REG_SYS_CTRL_CLK_AP_APB_MODE    0x0000007C
#define REG_SYS_CTRL_CLK_AP_APB_ENABLE    0x00000080
#define REG_SYS_CTRL_CLK_AP_APB_DISABLE    0x00000084
#define REG_SYS_CTRL_CLK_AIF_APB_MODE    0x00000088
#define REG_SYS_CTRL_CLK_AIF_APB_ENABLE    0x0000008C
#define REG_SYS_CTRL_CLK_AIF_APB_DISABLE    0x00000090
#define REG_SYS_CTRL_CLK_AON_AHB_MODE    0x00000094
#define REG_SYS_CTRL_CLK_AON_AHB_ENABLE    0x00000098
#define REG_SYS_CTRL_CLK_AON_AHB_DISABLE    0x0000009C
#define REG_SYS_CTRL_CLK_AON_APB_MODE    0x000000A0
#define REG_SYS_CTRL_CLK_AON_APB_ENABLE    0x000000A4
#define REG_SYS_CTRL_CLK_AON_APB_DISABLE    0x000000A8
#define REG_SYS_CTRL_CLK_RF_AHB_MODE    0x000000AC
#define REG_SYS_CTRL_CLK_RF_AHB_ENABLE    0x000000B0
#define REG_SYS_CTRL_CLK_RF_AHB_DISABLE    0x000000B4
#define REG_SYS_CTRL_CLK_RF_APB_MODE    0x000000B8
#define REG_SYS_CTRL_CLK_RF_APB_ENABLE    0x000000BC
#define REG_SYS_CTRL_CLK_RF_APB_DISABLE    0x000000C0
#define REG_SYS_CTRL_CLK_OTHERS_MODE    0x000000C4
#define REG_SYS_CTRL_CLK_OTHERS_ENABLE    0x000000C8
#define REG_SYS_CTRL_CLK_OTHERS_DISABLE    0x000000CC
#define REG_SYS_CTRL_CLK_OTHERS_1_MODE    0x000000D0
#define REG_SYS_CTRL_CLK_OTHERS_1_ENABLE    0x000000D4
#define REG_SYS_CTRL_CLK_OTHERS_1_DISABLE    0x000000D8
#define REG_SYS_CTRL_PLL_CTRL         0x000000DC
#define REG_SYS_CTRL_SEL_CLOCK        0x000000E0
#define REG_SYS_CTRL_CFG_CLK_OUT      0x000000E8
#define REG_SYS_CTRL_CFG_CLK_AUDIOBCK1_DIV    0x000000F0
#define REG_SYS_CTRL_CFG_CLK_AUDIOBCK2_DIV    0x000000F4
#define REG_SYS_CTRL_CFG_CLK_UART_0    0x000000F8
#define REG_SYS_CTRL_CFG_CLK_UART_1    0x000000FC
#define REG_SYS_CTRL_CFG_CLK_UART_2    0x00000100
#define REG_SYS_CTRL_CFG_CLK_UART_3    0x00000104
#define REG_SYS_CTRL_CFG_CLK_PWM      0x00000108
#define REG_SYS_CTRL_CFG_CLK_AUXCLK    0x0000010C
#define REG_SYS_CTRL_CFG_CLK_DBG_DIV    0x00000110
#define REG_SYS_CTRL_CFG_CLK_CAMERA_OUT    0x00000114
#define REG_SYS_CTRL_RESET_CAUSE      0x00000118
#define REG_SYS_CTRL_WAKEUP           0x0000011C
#define REG_SYS_CTRL_IGNORE_CHARGER    0x00000120
#define REG_SYS_CTRL_CFG_PLL_SPIFLASH_DIV    0x0000012C
#define REG_SYS_CTRL_CFG_PLL_SPIFLASH1_DIV    0x00000130
#define REG_SYS_CTRL_CFG_PLL_MEM_BRIDGE_DIV    0x00000134
#define REG_SYS_CTRL_CFG_DBG_CLK_SOURCE_SEL    0x00000138
#define REG_SYS_CTRL_CFG_CLK_494M_LTE_DIV    0x0000013C
#define REG_SYS_CTRL_CFG_PLL_ISP_DIV    0x00000140
#define REG_SYS_CTRL_CFG_PLL_PIX_DIV    0x00000144
#define REG_SYS_CTRL_CFG_PLL_SYS_AXI_DIV    0x00000148
#define REG_SYS_CTRL_CFG_PLL_SYS_AHB_APB_DIV    0x0000014C
#define REG_SYS_CTRL_CFG_PLL_SYS_AHB_BTFM_DIV    0x00000150
#define REG_SYS_CTRL_CFG_PLL_CSI_DIV    0x00000154
#define REG_SYS_CTRL_CFG_PLL_SYS_SPIFLASH_DIV    0x00000158
#define REG_SYS_CTRL_CFG_PLL_CP_DIV    0x0000015C
#define REG_SYS_CTRL_CFG_PLL_AP_CPU_DIV    0x00000160
#define REG_SYS_CTRL_CFG_AP_CPU_ACLKEN_DIV    0x00000164
#define REG_SYS_CTRL_CFG_AP_CPU_DBGEN_DIV    0x00000168
#define REG_SYS_CTRL_CFG_PLL_CP_CPU_DIV    0x0000016C
#define REG_SYS_CTRL_CFG_CP_CPU_ACLKEN_DIV    0x00000170
#define REG_SYS_CTRL_CFG_CP_CPU_DBGEN_DIV    0x00000174
#define REG_SYS_CTRL_CFG_PLL_USB      0x00000178
#define REG_SYS_CTRL_CFG_HMPROT       0x0000017C
#define REG_SYS_CTRL_CFG_MEM_BBDMA    0x00000180
#define REG_SYS_CTRL_CFG_IMEMROM      0x00000184
#define REG_SYS_CTRL_CFG_AIF_CFG      0x00000188
#define REG_SYS_CTRL_CFG_MISC_CFG     0x0000018C
#define REG_SYS_CTRL_CFG_MISC1_CFG    0x00000190
#define REG_SYS_CTRL_CFG_IRQ_CLEAR    0x00000194
#define REG_SYS_CTRL_CFG_IRQ_MASK     0x00000198
#define REG_SYS_CTRL_CFG_STATUS       0x0000019C
#define REG_SYS_CTRL_CFG_FORCE_LP_MODE_LP    0x000001A0
#define REG_SYS_CTRL_CFG_RESERVE      0x000001B4
#define REG_SYS_CTRL_CFG_RESERVE1     0x000001B8
#define REG_SYS_CTRL_CFG_RESERVE2     0x000001BC
#define REG_SYS_CTRL_CFG_RESERVE3     0x000001C0
#define REG_SYS_CTRL_CFG_RESERVE4     0x000001C4
#define REG_SYS_CTRL_CFG_RESERVE5     0x000001C8
#define REG_SYS_CTRL_CFG_RESERVE6     0x000001CC
#define REG_SYS_CTRL_CFG_RESERVE7     0x000001D0
#define REG_SYS_CTRL_CFG_RESERVE8     0x000001D4
#define REG_SYS_CTRL_CFG_RESERVE9     0x000001D8
#define REG_SYS_CTRL_CFG_RESERVE10    0x000001DC
#define REG_SYS_CTRL_CFG_RESERVE11    0x000001E0
#define REG_SYS_CTRL_CFG_RESERVE12    0x000001E4
#define REG_SYS_CTRL_CFG_RESERVE13    0x000001E8
#define REG_SYS_CTRL_CFG_RESERVE14    0x000001EC
#define REG_SYS_CTRL_CFG_RESERVE15    0x000001F0
#define REG_SYS_CTRL_CFG_RESERVE16    0x000001F4
#define REG_SYS_CTRL_CFG_RESERVE1_IN    0x000001F8
#define REG_SYS_CTRL_CFG_RESERVE2_IN    0x000001FC
#define REG_SYS_CTRL_CFG_RESERVE3_IN    0x00000200
#define REG_SYS_CTRL_CFG_RESERVE4_IN    0x00000204
#define REG_SYS_CTRL_CFG_RESERVE5_IN    0x00000208
#define REG_SYS_CTRL_CFG_RESERVE6_IN    0x0000020C
#define REG_SYS_CTRL_CFG_RESERVE7_IN    0x00000210
#define REG_SYS_CTRL_CFG_CHIP_PROD_ID    0x00000214
#define REG_SYS_CTRL_CFG_QOS_WCN_A5_GGE    0x00000218
#define REG_SYS_CTRL_CFG_QOS_AXIDMA_CPA5_F8    0x0000021C
#define REG_SYS_CTRL_CFG_QOS_LCDC_LZMA_GOUDA    0x00000220
#define REG_SYS_CTRL_CFG_QOS_LTE_USB    0x00000224

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
#define SYS_CTRL_SET_SYS_AHB_RST(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_SET_SYS_AHB_RST_MASK (0x7FF<<0)
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
#define SYS_CTRL_CLR_SYS_AHB_RST(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_CLR_SYS_AHB_RST_MASK (0x7FF<<0)
#define SYS_CTRL_CLR_SYS_AHB_RST_SHIFT (0)

//AP_APB_RST_SET
#define SYS_CTRL_SET_AP_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_AP_APB_RST_ID_CAMERA (1<<1)
#define SYS_CTRL_SET_AP_APB_RST_ID_I2C1 (1<<2)
#define SYS_CTRL_SET_AP_APB_RST_ID_I2C2 (1<<3)
#define SYS_CTRL_SET_AP_APB_RST_ID_I2C3 (1<<4)
#define SYS_CTRL_SET_AP_APB_RST_ID_IFC (1<<5)
#define SYS_CTRL_SET_AP_APB_RST_ID_IMEM (1<<6)
#define SYS_CTRL_SET_AP_APB_RST_ID_SDMMC1 (1<<7)
#define SYS_CTRL_SET_AP_APB_RST_ID_SDMMC2 (1<<8)
#define SYS_CTRL_SET_AP_APB_RST_ID_SPI1 (1<<9)
#define SYS_CTRL_SET_AP_APB_RST_ID_SPI2 (1<<10)
#define SYS_CTRL_SET_AP_APB_RST_ID_SPI3 (1<<11)
#define SYS_CTRL_SET_AP_APB_RST_ID_UART1 (1<<12)
#define SYS_CTRL_SET_AP_APB_RST_ID_UART2 (1<<13)
#define SYS_CTRL_SET_AP_APB_RST_ID_UART3 (1<<14)
#define SYS_CTRL_SET_AP_APB_RST_ID_REV0 (1<<15)
#define SYS_CTRL_SET_AP_APB_RST_ID_REV1 (1<<16)
#define SYS_CTRL_SET_AP_APB_RST_ID_REV2 (1<<17)
#define SYS_CTRL_SET_AP_APB_RST_ID_REV3 (1<<18)
#define SYS_CTRL_SET_AP_APB_RST_ID_PAGESPY (1<<19)
#define SYS_CTRL_SET_AP_APB_RST_ID_LZMA (1<<20)
#define SYS_CTRL_SET_AP_APB_RST_ID_PSRAM (1<<21)
#define SYS_CTRL_SET_AP_APB_RST_ID_TIMER1 (1<<22)
#define SYS_CTRL_SET_AP_APB_RST_ID_TIMER2 (1<<23)
#define SYS_CTRL_SET_AP_APB_RST_ID_TIMER4 (1<<24)
#define SYS_CTRL_SET_AP_APB_RST_ID_GOUDA (1<<25)
#define SYS_CTRL_SET_AP_APB_RST(n) (((n)&0x3FFFFFF)<<0)
#define SYS_CTRL_SET_AP_APB_RST_MASK (0x3FFFFFF<<0)
#define SYS_CTRL_SET_AP_APB_RST_SHIFT (0)

//AP_APB_RST_CLR
#define SYS_CTRL_CLR_AP_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_AP_APB_RST_ID_CAMERA (1<<1)
#define SYS_CTRL_CLR_AP_APB_RST_ID_I2C1 (1<<2)
#define SYS_CTRL_CLR_AP_APB_RST_ID_I2C2 (1<<3)
#define SYS_CTRL_CLR_AP_APB_RST_ID_I2C3 (1<<4)
#define SYS_CTRL_CLR_AP_APB_RST_ID_IFC (1<<5)
#define SYS_CTRL_CLR_AP_APB_RST_ID_IMEM (1<<6)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC1 (1<<7)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC2 (1<<8)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SPI1 (1<<9)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SPI2 (1<<10)
#define SYS_CTRL_CLR_AP_APB_RST_ID_SPI3 (1<<11)
#define SYS_CTRL_CLR_AP_APB_RST_ID_UART1 (1<<12)
#define SYS_CTRL_CLR_AP_APB_RST_ID_UART2 (1<<13)
#define SYS_CTRL_CLR_AP_APB_RST_ID_UART3 (1<<14)
#define SYS_CTRL_CLR_AP_APB_RST_ID_REV0 (1<<15)
#define SYS_CTRL_CLR_AP_APB_RST_ID_REV1 (1<<16)
#define SYS_CTRL_CLR_AP_APB_RST_ID_REV2 (1<<17)
#define SYS_CTRL_CLR_AP_APB_RST_ID_REV3 (1<<18)
#define SYS_CTRL_CLR_AP_APB_RST_ID_PAGESPY (1<<19)
#define SYS_CTRL_CLR_AP_APB_RST_ID_LZMA (1<<20)
#define SYS_CTRL_CLR_AP_APB_RST_ID_PSRAM (1<<21)
#define SYS_CTRL_CLR_AP_APB_RST_ID_TIMER1 (1<<22)
#define SYS_CTRL_CLR_AP_APB_RST_ID_TIMER2 (1<<23)
#define SYS_CTRL_CLR_AP_APB_RST_ID_TIMER4 (1<<24)
#define SYS_CTRL_CLR_AP_APB_RST_ID_GOUDA (1<<25)
#define SYS_CTRL_CLR_AP_APB_RST(n) (((n)&0x3FFFFFF)<<0)
#define SYS_CTRL_CLR_AP_APB_RST_MASK (0x3FFFFFF<<0)
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
#define SYS_CTRL_SET_AON_AHB_RST_ID_REV3 (1<<4)
#define SYS_CTRL_SET_AON_AHB_RST(n) (((n)&31)<<0)
#define SYS_CTRL_SET_AON_AHB_RST_MASK (31<<0)
#define SYS_CTRL_SET_AON_AHB_RST_SHIFT (0)

//AON_AHB_RST_CLR
#define SYS_CTRL_CLR_AON_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_REV0 (1<<1)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_REV1 (1<<2)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_REV2 (1<<3)
#define SYS_CTRL_CLR_AON_AHB_RST_ID_REV3 (1<<4)
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
#define SYS_CTRL_SET_AON_APB_RST_ID_SCI1 (1<<6)
#define SYS_CTRL_SET_AON_APB_RST_ID_SCI2 (1<<7)
#define SYS_CTRL_SET_AON_APB_RST_ID_TIMER3 (1<<8)
#define SYS_CTRL_SET_AON_APB_RST_ID_I2C (1<<9)
#define SYS_CTRL_SET_AON_APB_RST_ID_ANA_REG (1<<10)
#define SYS_CTRL_SET_AON_APB_RST_ID_SPINLOCK (1<<11)
#define SYS_CTRL_SET_AON_APB_RST_ID_LPS (1<<12)
#define SYS_CTRL_SET_AON_APB_RST_ID_EFUSE (1<<13)
#define SYS_CTRL_SET_AON_APB_RST_ID_AHB2APB_ADI (1<<14)
#define SYS_CTRL_SET_AON_APB_RST_ID_ADI (1<<15)
#define SYS_CTRL_SET_AON_APB_RST_ID_LVDS (1<<16)
#define SYS_CTRL_SET_AON_APB_RST(n) (((n)&0x1FFFF)<<0)
#define SYS_CTRL_SET_AON_APB_RST_MASK (0x1FFFF<<0)
#define SYS_CTRL_SET_AON_APB_RST_SHIFT (0)

//AON_APB_RST_CLR
#define SYS_CTRL_CLR_AON_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_AON_APB_RST_ID_CALENDAR (1<<1)
#define SYS_CTRL_CLR_AON_APB_RST_ID_GPIO (1<<2)
#define SYS_CTRL_CLR_AON_APB_RST_ID_IFC (1<<3)
#define SYS_CTRL_CLR_AON_APB_RST_ID_KEYPAD (1<<4)
#define SYS_CTRL_CLR_AON_APB_RST_ID_PWM (1<<5)
#define SYS_CTRL_CLR_AON_APB_RST_ID_SCI1 (1<<6)
#define SYS_CTRL_CLR_AON_APB_RST_ID_SCI2 (1<<7)
#define SYS_CTRL_CLR_AON_APB_RST_ID_TIMER3 (1<<8)
#define SYS_CTRL_CLR_AON_APB_RST_ID_I2C (1<<9)
#define SYS_CTRL_CLR_AON_APB_RST_ID_ANA_REG (1<<10)
#define SYS_CTRL_CLR_AON_APB_RST_ID_SPINLOCK (1<<11)
#define SYS_CTRL_CLR_AON_APB_RST_ID_LPS (1<<12)
#define SYS_CTRL_CLR_AON_APB_RST_ID_EFUSE (1<<13)
#define SYS_CTRL_CLR_AON_APB_RST_ID_AHB2APB_ADI (1<<14)
#define SYS_CTRL_CLR_AON_APB_RST_ID_ADI (1<<15)
#define SYS_CTRL_CLR_AON_APB_RST_ID_LVDS (1<<16)
#define SYS_CTRL_CLR_AON_APB_RST(n) (((n)&0x1FFFF)<<0)
#define SYS_CTRL_CLR_AON_APB_RST_MASK (0x1FFFF<<0)
#define SYS_CTRL_CLR_AON_APB_RST_SHIFT (0)

//RF_AHB_RST_SET
#define SYS_CTRL_SET_RF_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RF_RAM (1<<1)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RISCV (1<<2)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RISCV_APB (1<<3)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RISCV_DBG (1<<4)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RISCV_JTAG (1<<5)
#define SYS_CTRL_SET_RF_AHB_RST_ID_SPI2AHB (1<<6)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RF_APB (1<<7)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RF_REV0 (1<<8)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RF_REV1 (1<<9)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RF_REV2 (1<<10)
#define SYS_CTRL_SET_RF_AHB_RST_ID_RF_REV3 (1<<11)
#define SYS_CTRL_SET_RF_AHB_RST(n) (((n)&0xFFF)<<0)
#define SYS_CTRL_SET_RF_AHB_RST_MASK (0xFFF<<0)
#define SYS_CTRL_SET_RF_AHB_RST_SHIFT (0)

//RF_AHB_RST_CLR
#define SYS_CTRL_CLR_RF_AHB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RF_RAM (1<<1)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RISCV (1<<2)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RISCV_APB (1<<3)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RISCV_DBG (1<<4)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RISCV_JTAG (1<<5)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_SPI2AHB (1<<6)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RF_APB (1<<7)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RF_REV0 (1<<8)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RF_REV1 (1<<9)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RF_REV2 (1<<10)
#define SYS_CTRL_CLR_RF_AHB_RST_ID_RF_REV3 (1<<11)
#define SYS_CTRL_CLR_RF_AHB_RST(n) (((n)&0xFFF)<<0)
#define SYS_CTRL_CLR_RF_AHB_RST_MASK (0xFFF<<0)
#define SYS_CTRL_CLR_RF_AHB_RST_SHIFT (0)

//RF_APB_RST_SET
#define SYS_CTRL_SET_RF_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_SET_RF_APB_RST_ID_IRQ_CTRL (1<<1)
#define SYS_CTRL_SET_RF_APB_RST_ID_TIMER0 (1<<2)
#define SYS_CTRL_SET_RF_APB_RST_ID_UART0 (1<<3)
#define SYS_CTRL_SET_RF_APB_RST_ID_REV0 (1<<4)
#define SYS_CTRL_SET_RF_APB_RST_ID_REV1 (1<<5)
#define SYS_CTRL_SET_RF_APB_RST_ID_REV2 (1<<6)
#define SYS_CTRL_SET_RF_APB_RST_ID_REV3 (1<<7)
#define SYS_CTRL_SET_RF_APB_RST(n) (((n)&0xFF)<<0)
#define SYS_CTRL_SET_RF_APB_RST_MASK (0xFF<<0)
#define SYS_CTRL_SET_RF_APB_RST_SHIFT (0)

//RF_APB_RST_CLR
#define SYS_CTRL_CLR_RF_APB_RST_ID_SYS (1<<0)
#define SYS_CTRL_CLR_RF_APB_RST_ID_IRQ_CTRL (1<<1)
#define SYS_CTRL_CLR_RF_APB_RST_ID_TIMER0 (1<<2)
#define SYS_CTRL_CLR_RF_APB_RST_ID_UART0 (1<<3)
#define SYS_CTRL_CLR_RF_APB_RST_ID_REV0 (1<<4)
#define SYS_CTRL_CLR_RF_APB_RST_ID_REV1 (1<<5)
#define SYS_CTRL_CLR_RF_APB_RST_ID_REV2 (1<<6)
#define SYS_CTRL_CLR_RF_APB_RST_ID_REV3 (1<<7)
#define SYS_CTRL_CLR_RF_APB_RST(n) (((n)&0xFF)<<0)
#define SYS_CTRL_CLR_RF_APB_RST_MASK (0xFF<<0)
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
#define SYS_CTRL_SET_RSTO_ID_PSRAM_APB (1<<18)
#define SYS_CTRL_SET_RSTO_ID_PIX   (1<<19)
#define SYS_CTRL_SET_RSTO_ID_SDM_26M (1<<20)
#define SYS_CTRL_SET_RSTO_ID_WDTIMER4 (1<<21)
#define SYS_CTRL_SET_RSTO_ID_RF_WDTIMER (1<<22)
#define SYS_CTRL_SET_RSTO_ID_LPS   (1<<23)
#define SYS_CTRL_SET_RSTO_ID_EFUSE (1<<24)
#define SYS_CTRL_SET_RSTO_ID_USB_ADP_32K (1<<25)
#define SYS_CTRL_SET_RSTO_ID_MIPIDSI (1<<26)
#define SYS_CTRL_SET_RSTO_ID_MIPIDSI_PHY (1<<27)
#define SYS_CTRL_SET_RSTO_ID_AUD_2AD (1<<28)
#define SYS_CTRL_SET_OTHERS_RST(n) (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_SET_OTHERS_RST_MASK (0x1FFFFFFF<<0)
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
#define SYS_CTRL_CLR_RSTO_ID_PSRAM_APB (1<<18)
#define SYS_CTRL_CLR_RSTO_ID_PIX   (1<<19)
#define SYS_CTRL_CLR_RSTO_ID_SDM_26M (1<<20)
#define SYS_CTRL_CLR_RSTO_ID_WDTIMER4 (1<<21)
#define SYS_CTRL_CLR_RSTO_ID_RF_WDTIMER (1<<22)
#define SYS_CTRL_CLR_RSTO_ID_LPS   (1<<23)
#define SYS_CTRL_CLR_RSTO_ID_EFUSE (1<<24)
#define SYS_CTRL_CLR_RSTO_ID_USB_ADP_32K (1<<25)
#define SYS_CTRL_CLR_RSTO_ID_MIPIDSI (1<<26)
#define SYS_CTRL_CLR_RSTO_ID_MIPIDSI_PHY (1<<27)
#define SYS_CTRL_CLR_RSTO_ID_AUD_2AD (1<<28)
#define SYS_CTRL_CLR_OTHERS_RST(n) (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_CLR_OTHERS_RST_MASK (0x1FFFFFFF<<0)
#define SYS_CTRL_CLR_OTHERS_RST_SHIFT (0)

//CLK_SYS_AXI_MODE
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_IMEM_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_IMEM_MANUAL (1<<0)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_LZMA_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_LZMA_MANUAL (1<<1)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV0_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV0_MANUAL (1<<2)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV1_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV1_MANUAL (1<<3)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV2_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_REV2_MANUAL (1<<4)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_SDMMC1_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_SYS_AXI_CLK_ID_SDMMC1_MANUAL (1<<5)
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
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GGE_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GGE_MANUAL (1<<1)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GEA3_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_GEA3_MANUAL (1<<2)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_LZMA_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_LZMA_MANUAL (1<<3)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_F8_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_F8_MANUAL (1<<4)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_AXIDMA_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_SYS_AHB_CLK_ID_AXIDMA_MANUAL (1<<5)
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
#define SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_ALWAYS (1<<8)
#define SYS_CTRL_ENABLE_CLK_SYS_AHB(n) (((n)&0x1FF)<<0)
#define SYS_CTRL_ENABLE_CLK_SYS_AHB_MASK (0x1FF<<0)
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
#define SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_ALWAYS (1<<8)
#define SYS_CTRL_DISABLE_CLK_SYS_AHB(n) (((n)&0x1FF)<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_AHB_MASK (0x1FF<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_AHB_SHIFT (0)

//CLK_AP_APB_MODE
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CONF_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CONF_MANUAL (1<<0)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CAMERA_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_CAMERA_MANUAL (1<<1)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C1_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C1_MANUAL (1<<2)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C2_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C2_MANUAL (1<<3)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C3_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_I2C3_MANUAL (1<<4)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_MANUAL (1<<5)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH0_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH0_MANUAL (1<<6)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH1_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH1_MANUAL (1<<7)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH2_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH2_MANUAL (1<<8)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH3_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH3_MANUAL (1<<9)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH4_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH4_MANUAL (1<<10)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH5_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH5_MANUAL (1<<11)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH6_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CH6_MANUAL (1<<12)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CHDBG_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_IFC_CHDBG_MANUAL (1<<13)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_REV2_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_REV2_MANUAL (1<<14)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SDMMC2_AUTOMATIC (0<<15)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SDMMC2_MANUAL (1<<15)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI1_AUTOMATIC (0<<16)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI1_MANUAL (1<<16)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI2_AUTOMATIC (0<<17)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI2_MANUAL (1<<17)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI3_AUTOMATIC (0<<18)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_SPI3_MANUAL (1<<18)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_UART1_AUTOMATIC (0<<19)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_UART1_MANUAL (1<<19)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_UART2_AUTOMATIC (0<<20)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_UART2_MANUAL (1<<20)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_UART3_AUTOMATIC (0<<21)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_UART3_MANUAL (1<<21)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_REV0_AUTOMATIC (0<<22)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_REV0_MANUAL (1<<22)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_REV1_AUTOMATIC (0<<23)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_REV1_MANUAL (1<<23)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_PAGESPY_AUTOMATIC (0<<24)
#define SYS_CTRL_MODE_AP_APB_CLK_ID_PAGESPY_MANUAL (1<<24)
#define SYS_CTRL_MODE_CLK_AP_APB(n) (((n)&0x1FFFFFF)<<0)
#define SYS_CTRL_MODE_CLK_AP_APB_MASK (0x1FFFFFF<<0)
#define SYS_CTRL_MODE_CLK_AP_APB_SHIFT (0)

//CLK_AP_APB_ENABLE
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_CAMERA (1<<1)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_I2C1 (1<<2)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_I2C2 (1<<3)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_I2C3 (1<<4)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC (1<<5)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH0 (1<<6)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH1 (1<<7)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH2 (1<<8)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH3 (1<<9)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH4 (1<<10)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH5 (1<<11)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CH6 (1<<12)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_IFC_CHDBG (1<<13)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_REV2 (1<<14)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SDMMC2 (1<<15)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SPI1 (1<<16)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SPI2 (1<<17)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_SPI3 (1<<18)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_UART1 (1<<19)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_UART2 (1<<20)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_UART3 (1<<21)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_REV0 (1<<22)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_REV1 (1<<23)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_PAGESPY (1<<24)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_LZMA (1<<25)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_PSRAM (1<<26)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_TIMER1 (1<<27)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_TIMER2 (1<<28)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_TIMER4 (1<<29)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_LCD (1<<30)
#define SYS_CTRL_ENABLE_AP_APB_CLK_ID_ALWAYS (1<<31)
#define SYS_CTRL_ENABLE_CLK_AP_APB(n) (((n)&0xFFFFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_AP_APB_MASK (0xFFFFFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_AP_APB_SHIFT (0)

//CLK_AP_APB_DISABLE
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_CAMERA (1<<1)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_I2C1 (1<<2)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_I2C2 (1<<3)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_I2C3 (1<<4)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC (1<<5)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH0 (1<<6)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH1 (1<<7)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH2 (1<<8)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH3 (1<<9)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH4 (1<<10)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH5 (1<<11)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CH6 (1<<12)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_IFC_CHDBG (1<<13)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_REV2 (1<<14)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SDMMC2 (1<<15)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SPI1 (1<<16)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SPI2 (1<<17)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_SPI3 (1<<18)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_UART1 (1<<19)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_UART2 (1<<20)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_UART3 (1<<21)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_REV0 (1<<22)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_REV1 (1<<23)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_PAGESPY (1<<24)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_LZMA (1<<25)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_PSRAM (1<<26)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_TIMER1 (1<<27)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_TIMER2 (1<<28)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_TIMER4 (1<<29)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_LCD (1<<30)
#define SYS_CTRL_DISABLE_AP_APB_CLK_ID_ALWAYS (1<<31)
#define SYS_CTRL_DISABLE_CLK_AP_APB(n) (((n)&0xFFFFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_AP_APB_MASK (0xFFFFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_AP_APB_SHIFT (0)

//CLK_AIF_APB_MODE
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_CONF_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_CONF_MANUAL (1<<0)
#define SYS_CTRL_MODE_AIF1_APB_CLK_ID_AIF_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AIF1_APB_CLK_ID_AIF_MANUAL (1<<1)
#define SYS_CTRL_MODE_AIF2_APB_CLK_ID_AIF_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AIF2_APB_CLK_ID_AIF_MANUAL (1<<2)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_MANUAL (1<<3)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH0_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH0_MANUAL (1<<4)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH1_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH1_MANUAL (1<<5)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH2_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH2_MANUAL (1<<6)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH3_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_IFC_CH3_MANUAL (1<<7)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV0_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV0_MANUAL (1<<8)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV1_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV1_MANUAL (1<<9)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV2_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_REV2_MANUAL (1<<10)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_AUD_2AD_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_AIF_APB_CLK_ID_AUD_2AD_MANUAL (1<<11)
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
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV0_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV0_MANUAL (1<<1)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV1_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV1_MANUAL (1<<2)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV2_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV2_MANUAL (1<<3)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV3_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AON_AHB_CLK_ID_BB_REV3_MANUAL (1<<4)
#define SYS_CTRL_MODE_CLK_AON_AHB(n) (((n)&31)<<0)
#define SYS_CTRL_MODE_CLK_AON_AHB_MASK (31<<0)
#define SYS_CTRL_MODE_CLK_AON_AHB_SHIFT (0)

//CLK_AON_AHB_ENABLE
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_SYSCTRL (1<<0)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_REV0 (1<<1)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_REV1 (1<<2)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_REV2 (1<<3)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_BB_REV3 (1<<4)
#define SYS_CTRL_ENABLE_AON_AHB_CLK_ID_ALWAYS (1<<5)
#define SYS_CTRL_ENABLE_CLK_AON_AHB(n) (((n)&0x3F)<<0)
#define SYS_CTRL_ENABLE_CLK_AON_AHB_MASK (0x3F<<0)
#define SYS_CTRL_ENABLE_CLK_AON_AHB_SHIFT (0)

//CLK_AON_AHB_DISABLE
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_SYSCTRL (1<<0)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_REV0 (1<<1)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_REV1 (1<<2)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_REV2 (1<<3)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_BB_REV3 (1<<4)
#define SYS_CTRL_DISABLE_AON_AHB_CLK_ID_ALWAYS (1<<5)
#define SYS_CTRL_DISABLE_CLK_AON_AHB(n) (((n)&0x3F)<<0)
#define SYS_CTRL_DISABLE_CLK_AON_AHB_MASK (0x3F<<0)
#define SYS_CTRL_DISABLE_CLK_AON_AHB_SHIFT (0)

//CLK_AON_APB_MODE
#define SYS_CTRL_MODE_AON_APB_CLK_ID_CONF_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_CONF_MANUAL (1<<0)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_HOST_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_HOST_MANUAL (1<<1)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_UART_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_DBG_UART_MANUAL (1<<2)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_MANUAL (1<<3)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH0_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH0_MANUAL (1<<4)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH1_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH1_MANUAL (1<<5)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH2_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH2_MANUAL (1<<6)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH3_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH3_MANUAL (1<<7)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH4_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH4_MANUAL (1<<8)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH5_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH5_MANUAL (1<<9)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH6_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH6_MANUAL (1<<10)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH7_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CH7_MANUAL (1<<11)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CHDBG_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_IFC_CHDBG_MANUAL (1<<12)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SCID1_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SCID1_MANUAL (1<<13)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SCID2_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SCID2_MANUAL (1<<14)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_AUTOMATIC (0<<15)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_PWM_MANUAL (1<<15)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_I2C_AUTOMATIC (0<<16)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_I2C_MANUAL (1<<16)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_ANA_REG_AUTOMATIC (0<<17)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_ANA_REG_MANUAL (1<<17)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SPINLOCK_AUTOMATIC (0<<18)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_SPINLOCK_MANUAL (1<<18)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_AUTOMATIC (0<<19)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_LPS_MANUAL (1<<19)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_EFUSE_AUTOMATIC (0<<20)
#define SYS_CTRL_MODE_AON_APB_CLK_ID_EFUSE_MANUAL (1<<20)
#define SYS_CTRL_MODE_CLK_AON_APB(n) (((n)&0x1FFFFF)<<0)
#define SYS_CTRL_MODE_CLK_AON_APB_MASK (0x1FFFFF<<0)
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
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH4 (1<<8)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH5 (1<<9)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH6 (1<<10)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CH7 (1<<11)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_IFC_CHDBG (1<<12)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_SCID1 (1<<13)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_SCID2 (1<<14)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_PWM (1<<15)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_I2C (1<<16)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_ANA_REG (1<<17)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_SPINLOCK (1<<18)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_LPS (1<<19)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_EFUSE (1<<20)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_SCI1 (1<<21)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_SCI2 (1<<22)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_GPIO (1<<23)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_TIMER3 (1<<24)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_KEYPAD (1<<25)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_AHB2APB_ADI (1<<26)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_ADI (1<<27)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_LVDS (1<<28)
#define SYS_CTRL_ENABLE_AON_APB_CLK_ID_ALWAYS (1<<29)
#define SYS_CTRL_ENABLE_CLK_AON_APB(n) (((n)&0x3FFFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_AON_APB_MASK (0x3FFFFFFF<<0)
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
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH4 (1<<8)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH5 (1<<9)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH6 (1<<10)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CH7 (1<<11)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_IFC_CHDBG (1<<12)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_SCID1 (1<<13)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_SCID2 (1<<14)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_PWM (1<<15)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_I2C (1<<16)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_ANA_REG (1<<17)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_SPINLOCK (1<<18)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_LPS (1<<19)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_EFUSE (1<<20)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_SCI1 (1<<21)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_SCI2 (1<<22)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_GPIO (1<<23)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_TIMER3 (1<<24)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_KEYPAD (1<<25)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_AHB2APB_ADI (1<<26)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_ADI (1<<27)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_LVDS (1<<28)
#define SYS_CTRL_DISABLE_AON_APB_CLK_ID_ALWAYS (1<<29)
#define SYS_CTRL_DISABLE_CLK_AON_APB(n) (((n)&0x3FFFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_AON_APB_MASK (0x3FFFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_AON_APB_SHIFT (0)

//CLK_RF_AHB_MODE
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RF_RAM_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RF_RAM_MANUAL (1<<0)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RISCV_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RISCV_MANUAL (1<<1)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RISCV_APB_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RISCV_APB_MANUAL (1<<2)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RISCV_JTAG_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RISCV_JTAG_MANUAL (1<<3)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_SPI2AHB_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_SPI2AHB_MANUAL (1<<4)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RF_APB_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_RF_APB_MANUAL (1<<5)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV0_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV0_MANUAL (1<<6)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV1_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV1_MANUAL (1<<7)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV2_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV2_MANUAL (1<<8)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV3_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_RF_AHB_CLK_ID_REV3_MANUAL (1<<9)
#define SYS_CTRL_MODE_CLK_RF_AHB(n) (((n)&0x3FF)<<0)
#define SYS_CTRL_MODE_CLK_RF_AHB_MASK (0x3FF<<0)
#define SYS_CTRL_MODE_CLK_RF_AHB_SHIFT (0)

//CLK_RF_AHB_ENABLE
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_RF_RAM (1<<0)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_RISCV (1<<1)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_RISCV_APB (1<<2)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_RISCV_JTAG (1<<3)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_SPI2AHB (1<<4)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_RF_APB (1<<5)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_REV0 (1<<6)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_REV1 (1<<7)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_REV2 (1<<8)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_REV3 (1<<9)
#define SYS_CTRL_ENABLE_RF_AHB_CLK_ID_ALWAYS (1<<10)
#define SYS_CTRL_ENABLE_CLK_RF_AHB(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_ENABLE_CLK_RF_AHB_MASK (0x7FF<<0)
#define SYS_CTRL_ENABLE_CLK_RF_AHB_SHIFT (0)

//CLK_RF_AHB_DISABLE
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_RF_RAM (1<<0)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_RISCV (1<<1)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_RISCV_APB (1<<2)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_RISCV_JTAG (1<<3)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_SPI2AHB (1<<4)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_RF_APB (1<<5)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_REV0 (1<<6)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_REV1 (1<<7)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_REV2 (1<<8)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_REV3 (1<<9)
#define SYS_CTRL_DISABLE_RF_AHB_CLK_ID_ALWAYS (1<<10)
#define SYS_CTRL_DISABLE_CLK_RF_AHB(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_DISABLE_CLK_RF_AHB_MASK (0x7FF<<0)
#define SYS_CTRL_DISABLE_CLK_RF_AHB_SHIFT (0)

//CLK_RF_APB_MODE
#define SYS_CTRL_MODE_RF_APB_CLK_ID_CONF_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_CONF_MANUAL (1<<0)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_IRQ_CTRL_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_IRQ_CTRL_MANUAL (1<<1)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_TIMER0_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_TIMER0_MANUAL (1<<2)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_UART0_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_UART0_MANUAL (1<<3)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV0_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV0_MANUAL (1<<4)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV1_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV1_MANUAL (1<<5)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV2_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV2_MANUAL (1<<6)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV3_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_RF_APB_CLK_ID_REV3_MANUAL (1<<7)
#define SYS_CTRL_MODE_CLK_RF_APB(n) (((n)&0xFF)<<0)
#define SYS_CTRL_MODE_CLK_RF_APB_MASK (0xFF<<0)
#define SYS_CTRL_MODE_CLK_RF_APB_SHIFT (0)

//CLK_RF_APB_ENABLE
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_IRQ_CTRL (1<<1)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_TIMER0 (1<<2)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_UART0 (1<<3)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_REV0 (1<<4)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_REV1 (1<<5)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_REV2 (1<<6)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_REV3 (1<<7)
#define SYS_CTRL_ENABLE_RF_APB_CLK_ID_ALWAYS (1<<8)
#define SYS_CTRL_ENABLE_CLK_RF_APB(n) (((n)&0x1FF)<<0)
#define SYS_CTRL_ENABLE_CLK_RF_APB_MASK (0x1FF<<0)
#define SYS_CTRL_ENABLE_CLK_RF_APB_SHIFT (0)

//CLK_RF_APB_DISABLE
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_CONF (1<<0)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_IRQ_CTRL (1<<1)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_TIMER0 (1<<2)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_UART0 (1<<3)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_REV0 (1<<4)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_REV1 (1<<5)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_REV2 (1<<6)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_REV3 (1<<7)
#define SYS_CTRL_DISABLE_RF_APB_CLK_ID_ALWAYS (1<<8)
#define SYS_CTRL_DISABLE_CLK_RF_APB(n) (((n)&0x1FF)<<0)
#define SYS_CTRL_DISABLE_CLK_RF_APB_MASK (0x1FF<<0)
#define SYS_CTRL_DISABLE_CLK_RF_APB_SHIFT (0)

//CLK_OTHERS_MODE
#define SYS_CTRL_MODE_OC_ID_HOST_UART_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_OC_ID_HOST_UART_MANUAL (1<<0)
#define SYS_CTRL_MODE_OC_ID_BCK1_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_OC_ID_BCK1_MANUAL (1<<1)
#define SYS_CTRL_MODE_OC_ID_BCK2_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_OC_ID_BCK2_MANUAL (1<<2)
#define SYS_CTRL_MODE_OC_ID_UART1_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_OC_ID_UART1_MANUAL (1<<3)
#define SYS_CTRL_MODE_OC_ID_UART2_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_OC_ID_UART2_MANUAL (1<<4)
#define SYS_CTRL_MODE_OC_ID_UART3_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_OC_ID_UART3_MANUAL (1<<5)
#define SYS_CTRL_MODE_OC_ID_AP_A5_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_OC_ID_AP_A5_MANUAL (1<<6)
#define SYS_CTRL_MODE_OC_ID_CP_A5_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_OC_ID_CP_A5_MANUAL (1<<7)
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
#define SYS_CTRL_MODE_OC_ID_GGE_26M_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_OC_ID_GGE_26M_MANUAL (1<<1)
#define SYS_CTRL_MODE_OC_ID_BT_FM_CLK32K_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_OC_ID_BT_FM_CLK32K_MANUAL (1<<2)
#define SYS_CTRL_MODE_OC_ID_BT_FM_26M_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_OC_ID_BT_FM_26M_MANUAL (1<<3)
#define SYS_CTRL_MODE_OC_ID_PMIC_CLK32K_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_OC_ID_PMIC_CLK32K_MANUAL (1<<4)
#define SYS_CTRL_MODE_OC_ID_PMIC_26M_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_OC_ID_PMIC_26M_MANUAL (1<<5)
#define SYS_CTRL_MODE_OC_ID_CP_CLK32K_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_OC_ID_CP_CLK32K_MANUAL (1<<6)
#define SYS_CTRL_MODE_OC_ID_CP_26M_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_OC_ID_CP_26M_MANUAL (1<<7)
#define SYS_CTRL_MODE_OC_ID_CP_96M_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_OC_ID_CP_96M_MANUAL (1<<8)
#define SYS_CTRL_MODE_OC_ID_CP_BBLTE_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_OC_ID_CP_BBLTE_MANUAL (1<<9)
#define SYS_CTRL_MODE_OC_ID_494M_LTE_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_OC_ID_494M_LTE_MANUAL (1<<10)
#define SYS_CTRL_MODE_OC_ID_REV0_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_OC_ID_REV0_MANUAL (1<<11)
#define SYS_CTRL_MODE_OC_ID_REV1_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_OC_ID_REV1_MANUAL (1<<12)
#define SYS_CTRL_MODE_OC_ID_REV2_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_OC_ID_REV2_MANUAL (1<<13)
#define SYS_CTRL_MODE_OC_ID_REV3_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_OC_ID_REV3_MANUAL (1<<14)
#define SYS_CTRL_MODE_CLK_OTHERS_1(n) (((n)&0x7FFF)<<0)
#define SYS_CTRL_MODE_CLK_OTHERS_1_MASK (0x7FFF<<0)
#define SYS_CTRL_MODE_CLK_OTHERS_1_SHIFT (0)

//CLK_OTHERS_1_ENABLE
#define SYS_CTRL_ENABLE_OC_ID_GGE_CLK32K (1<<0)
#define SYS_CTRL_ENABLE_OC_ID_GGE_26M (1<<1)
#define SYS_CTRL_ENABLE_OC_ID_BT_FM_CLK32K (1<<2)
#define SYS_CTRL_ENABLE_OC_ID_BT_FM_26M (1<<3)
#define SYS_CTRL_ENABLE_OC_ID_PMIC_CLK32K (1<<4)
#define SYS_CTRL_ENABLE_OC_ID_PMIC_26M (1<<5)
#define SYS_CTRL_ENABLE_OC_ID_CP_CLK32K (1<<6)
#define SYS_CTRL_ENABLE_OC_ID_CP_26M (1<<7)
#define SYS_CTRL_ENABLE_OC_ID_CP_96M (1<<8)
#define SYS_CTRL_ENABLE_OC_ID_CP_BBLTE (1<<9)
#define SYS_CTRL_ENABLE_OC_ID_494M_LTE (1<<10)
#define SYS_CTRL_ENABLE_OC_ID_REV0 (1<<11)
#define SYS_CTRL_ENABLE_OC_ID_REV1 (1<<12)
#define SYS_CTRL_ENABLE_OC_ID_REV2 (1<<13)
#define SYS_CTRL_ENABLE_OC_ID_REV3 (1<<14)
#define SYS_CTRL_ENABLE_OC_ID_SDM_26M (1<<15)
#define SYS_CTRL_ENABLE_OC_ID_LPS  (1<<16)
#define SYS_CTRL_ENABLE_OC_ID_EFUSE_26M (1<<17)
#define SYS_CTRL_ENABLE_OC_ID_USB_ADP_32K (1<<18)
#define SYS_CTRL_ENABLE_OC_ID_USB_UTMI_48M (1<<19)
#define SYS_CTRL_ENABLE_OC_ID_AP_26M (1<<20)
#define SYS_CTRL_ENABLE_OC_ID_AP_32K (1<<21)
#define SYS_CTRL_ENABLE_OC_ID_MIPIDSI (1<<22)
#define SYS_CTRL_ENABLE_OC_ID_AHB_BTFM (1<<23)
#define SYS_CTRL_ENABLE_CLK_OTHERS_1(n) (((n)&0xFFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_OTHERS_1_MASK (0xFFFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_OTHERS_1_SHIFT (0)

//CLK_OTHERS_1_DISABLE
#define SYS_CTRL_DISABLE_OC_ID_GGE_CLK32K (1<<0)
#define SYS_CTRL_DISABLE_OC_ID_GGE_26M (1<<1)
#define SYS_CTRL_DISABLE_OC_ID_BT_FM_CLK32K (1<<2)
#define SYS_CTRL_DISABLE_OC_ID_BT_FM_26M (1<<3)
#define SYS_CTRL_DISABLE_OC_ID_PMIC_CLK32K (1<<4)
#define SYS_CTRL_DISABLE_OC_ID_PMIC_26M (1<<5)
#define SYS_CTRL_DISABLE_OC_ID_CP_CLK32K (1<<6)
#define SYS_CTRL_DISABLE_OC_ID_CP_26M (1<<7)
#define SYS_CTRL_DISABLE_OC_ID_CP_96M (1<<8)
#define SYS_CTRL_DISABLE_OC_ID_CP_BBLTE (1<<9)
#define SYS_CTRL_DISABLE_OC_ID_494M_LTE (1<<10)
#define SYS_CTRL_DISABLE_OC_ID_REV0 (1<<11)
#define SYS_CTRL_DISABLE_OC_ID_REV1 (1<<12)
#define SYS_CTRL_DISABLE_OC_ID_REV2 (1<<13)
#define SYS_CTRL_DISABLE_OC_ID_REV3 (1<<14)
#define SYS_CTRL_DISABLE_OC_ID_SDM_26M (1<<15)
#define SYS_CTRL_DISABLE_OC_ID_LPS (1<<16)
#define SYS_CTRL_DISABLE_OC_ID_EFUSE_26M (1<<17)
#define SYS_CTRL_DISABLE_OC_ID_USB_ADP_32K (1<<18)
#define SYS_CTRL_DISABLE_OC_ID_USB_UTMI_48M (1<<19)
#define SYS_CTRL_DISABLE_OC_ID_AP_26M (1<<20)
#define SYS_CTRL_DISABLE_OC_ID_AP_32K (1<<21)
#define SYS_CTRL_DISABLE_OC_ID_MIPIDSI (1<<22)
#define SYS_CTRL_DISABLE_OC_ID_AHB_BTFM (1<<23)
#define SYS_CTRL_DISABLE_CLK_OTHERS_1(n) (((n)&0xFFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_OTHERS_1_MASK (0xFFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_OTHERS_1_SHIFT (0)

//Pll_Ctrl
#define SYS_CTRL_PLL_ENABLE        (1<<0)
#define SYS_CTRL_PLL_ENABLE_MASK   (1<<0)
#define SYS_CTRL_PLL_ENABLE_SHIFT  (0)
#define SYS_CTRL_PLL_ENABLE_POWER_DOWN (0<<0)
#define SYS_CTRL_PLL_ENABLE_ENABLE (1<<0)
#define SYS_CTRL_PLL_LOCK_RESET    (1<<4)
#define SYS_CTRL_PLL_LOCK_RESET_MASK (1<<4)
#define SYS_CTRL_PLL_LOCK_RESET_SHIFT (4)
#define SYS_CTRL_PLL_LOCK_RESET_RESET (0<<4)
#define SYS_CTRL_PLL_LOCK_RESET_NO_RESET (1<<4)
#define SYS_CTRL_PLL_BYPASS_LOCK   (1<<8)
#define SYS_CTRL_PLL_BYPASS_LOCK_MASK (1<<8)
#define SYS_CTRL_PLL_BYPASS_LOCK_SHIFT (8)
#define SYS_CTRL_PLL_BYPASS_LOCK_PASS (0<<8)
#define SYS_CTRL_PLL_BYPASS_LOCK_BYPASS (1<<8)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE (1<<12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_MASK (1<<12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_SHIFT (12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE (1<<12)
#define SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE (0<<12)

//Sel_Clock
#define SYS_CTRL_SLOW_SEL_RF_OSCILLATOR (1<<0)
#define SYS_CTRL_SLOW_SEL_RF_RF    (0<<0)
#define SYS_CTRL_SYS_SEL_FAST_SLOW (1<<1)
#define SYS_CTRL_SYS_SEL_FAST_FAST (0<<1)
#define SYS_CTRL_OSC_32K_26M_DIV32K_SEL (1<<2)
#define SYS_CTRL_PLL_DISABLE_LPS_DISABLE (1<<3)
#define SYS_CTRL_PLL_DISABLE_LPS_ENABLE (0<<3)
#define SYS_CTRL_RF_DETECTED_OK    (1<<4)
#define SYS_CTRL_RF_DETECTED_NO    (0<<4)
#define SYS_CTRL_RF_DETECT_BYPASS  (1<<5)
#define SYS_CTRL_RF_DETECT_RESET   (1<<6)
#define SYS_CTRL_RF_SELECTED_L     (1<<7)
#define SYS_CTRL_PLL_LOCKED        (1<<8)
#define SYS_CTRL_PLL_LOCKED_MASK   (1<<8)
#define SYS_CTRL_PLL_LOCKED_SHIFT  (8)
#define SYS_CTRL_PLL_LOCKED_LOCKED (1<<8)
#define SYS_CTRL_PLL_LOCKED_NOT_LOCKED (0<<8)
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

//Cfg_Clk_Auxclk
#define SYS_CTRL_AUXCLK_EN_DISABLE (0<<0)
#define SYS_CTRL_AUXCLK_EN_ENABLE  (1<<0)

//Cfg_Clk_Dbg_Div
#define SYS_CTRL_CLK_DBG_DIVIDER(n) (((n)&0x7FF)<<0)

//Cfg_Clk_Camera_Out
#define SYS_CTRL_CLK_CAMERA_OUT_EN_DISABLE (0<<0)
#define SYS_CTRL_CLK_CAMERA_OUT_EN_ENABLE (1<<0)
#define SYS_CTRL_CLK_CAMERA_DIV_SRC_SEL_26_MHZ (0<<1)
#define SYS_CTRL_CLK_CAMERA_DIV_SRC_SEL_156_MHZ (1<<1)
#define SYS_CTRL_CLK_CAMERA_OUT_DIV(n) (((n)&0x7FF)<<2)
#define SYS_CTRL_CLK_CAMERA_DIV_UPDATE (1<<13)
#define SYS_CTRL_CLK_SPI_CAM_EN_DISABLE (0<<14)
#define SYS_CTRL_CLK_SPI_CAM_EN_ENABLE (1<<14)
#define SYS_CTRL_CLK_SPI_CAM_POL   (1<<15)
#define SYS_CTRL_CLK_SPI_CAM_SEL   (1<<16)
#define SYS_CTRL_CLK_SPI_CAM_DIV(n) (((n)&0x7FF)<<17)
#define SYS_CTRL_CLK_SPI_CAM_DIV_UPDATE (1<<28)

//Reset_Cause
#define SYS_CTRL_WATCHDOG_RESET_1_HAPPENED (1<<0)
#define SYS_CTRL_WATCHDOG_RESET_1_NO (0<<0)
#define SYS_CTRL_WATCHDOG_RESET_2_HAPPENED (1<<1)
#define SYS_CTRL_WATCHDOG_RESET_2_NO (0<<1)
#define SYS_CTRL_WATCHDOG_RESET_3_HAPPENED (1<<2)
#define SYS_CTRL_WATCHDOG_RESET_3_NO (0<<2)
#define SYS_CTRL_WATCHDOG_RESET_4_HAPPENED (1<<3)
#define SYS_CTRL_WATCHDOG_RESET_4_NO (0<<3)
#define SYS_CTRL_WATCHDOG_RESET_RF_HAPPENED (1<<4)
#define SYS_CTRL_WATCHDOG_RESET_RF_NO (0<<4)
#define SYS_CTRL_WATCHDOG_RESET_GGE_HAPPENED (1<<5)
#define SYS_CTRL_WATCHDOG_RESET_GGE_NO (0<<5)
#define SYS_CTRL_WATCHDOG_RESET_ZSP_HAPPENED (1<<6)
#define SYS_CTRL_WATCHDOG_RESET_ZSP_NO (0<<6)
#define SYS_CTRL_GLOBALSOFT_RESET_HAPPENED (1<<7)
#define SYS_CTRL_GLOBALSOFT_RESET_NO (0<<7)
#define SYS_CTRL_HOSTDEBUG_RESET_HAPPENED (1<<8)
#define SYS_CTRL_HOSTDEBUG_RESET_NO (0<<8)
#define SYS_CTRL_WATCHDOG_RESET_CP_HAPPENED (1<<9)
#define SYS_CTRL_WATCHDOG_RESET_CP_NO (0<<9)
#define SYS_CTRL_ALARMCAUSE_HAPPENED (1<<12)
#define SYS_CTRL_ALARMCAUSE_NO     (0<<12)
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

//CFG_PLL_USB
#define SYS_CTRL_CFG_PLL_USB_RESET (1<<0)
#define SYS_CTRL_CFG_PLL_USB_REV   (1<<1)

//Cfg_HMPROT
#define SYS_CTRL_HPROT_BBDMA(n)    (((n)&15)<<0)
#define SYS_CTRL_HPROT_AIF_IFC(n)  (((n)&15)<<4)
#define SYS_CTRL_HPROT_GOUDA(n)    (((n)&15)<<8)
#define SYS_CTRL_HPROT_AP_IFC(n)   (((n)&15)<<12)
#define SYS_CTRL_HPROT_USB(n)      (((n)&15)<<16)
#define SYS_CTRL_HPROT_SYS_AON_IFC(n) (((n)&15)<<20)
#define SYS_CTRL_HSPROT_PSRAM_CTRL(n) (((n)&15)<<24)

//Cfg_MEM_BBDMA
#define SYS_CTRL_MEM_EMA_BBDMA(n)  (((n)&0xFFFFFFFF)<<0)

//Cfg_IMEMROM
#define SYS_CTRL_MEM_EMA_IMEMROM(n) (((n)&0xFFFFFFFF)<<0)

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

//Cfg_MISC1_CFG
#define SYS_CTRL_DEBUG_MON_SEL(n)  (((n)&15)<<0)
#define SYS_CTRL_IOMUX_CLK_FORCE_ON (1<<4)
#define SYS_CTRL_AP_RST_CTRL       (1<<5)
#define SYS_CTRL_GGE_RST_CTRL      (1<<6)
#define SYS_CTRL_BTFM_RST_CTRL     (1<<7)
#define SYS_CTRL_AP_CLK_CTRL       (1<<8)
#define SYS_CTRL_GGE_CLK_CTRL      (1<<9)
#define SYS_CTRL_BTFM_CLK_CTRL     (1<<10)
#define SYS_CTRL_BBPLL1_ENABLE     (1<<11)
#define SYS_CTRL_BBPLL2_ENABLE     (1<<12)
#define SYS_CTRL_MEMPLL_ENABLE     (1<<13)
#define SYS_CTRL_USBPLL_ENABLE     (1<<14)
#define SYS_CTRL_AUDIOPLL_ENABLE   (1<<15)
#define SYS_CTRL_APLL_ZSP_CLK_SEL  (1<<16)
#define SYS_CTRL_APLL_BBPLL2_CLK_SEL (1<<17)
#define SYS_CTRL_REG_GIC400_ARUSER_SEL (1<<18)
#define SYS_CTRL_REG_GIC400_ARUSER_DBG (1<<19)
#define SYS_CTRL_REG_GIC400_AWUSER_SEL (1<<20)
#define SYS_CTRL_REG_GIC400_AWUSER_DBG (1<<21)
#define SYS_CTRL_LVDS_WCN_RFDIG_SEL (1<<22)
#define SYS_CTRL_LVDS_RFDIG_RF_BB_SEL (1<<23)
#define SYS_CTRL_WCN_OSC_EN_CTRL   (1<<24)

//Cfg_IRQ_CLEAR
#define SYS_CTRL_RFBUSY_BB_CLEAR_R (1<<0)
#define SYS_CTRL_RFBUSY_BB_CLEAR_F (1<<1)

//Cfg_IRQ_MASK
#define SYS_CTRL_RFBUSY_BB_MASK_R  (1<<0)
#define SYS_CTRL_RFBUSY_BB_MASK_F  (1<<1)

//Cfg_STATUS
#define SYS_CTRL_RFBUSY_BB_STATUS  (1<<0)

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

//Cfg_Reserve
#define SYS_CTRL_UART1_EN          (1<<0)
#define SYS_CTRL_I2S_EN            (1<<1)
#define SYS_CTRL_AUX_REQ_EN        (1<<2)
#define SYS_CTRL_WD_RST_MODE       (1<<3)
#define SYS_CTRL_SYS_BB_SIDE       (1<<4)
#define SYS_CTRL_CAM_OUT0_SEL      (1<<5)
#define SYS_CTRL_VPU_CLK_EN        (1<<6)
#define SYS_CTRL_WD_1_RST_EN       (1<<7)
#define SYS_CTRL_WD_2_RST_EN       (1<<8)
#define SYS_CTRL_WD_3_RST_EN       (1<<9)
#define SYS_CTRL_WD_4_RST_EN       (1<<10)
#define SYS_CTRL_WD_RF_RST_EN      (1<<11)
#define SYS_CTRL_WD_GGE_RST_EN     (1<<12)
#define SYS_CTRL_WD_ZSP_RST_EN     (1<<13)
#define SYS_CTRL_WD_CP_RST_EN      (1<<14)
#define SYS_CTRL_DMC_PHY_RST_EN    (1<<15)
#define SYS_CTRL_HMPROT_WCN_PERI(n) (((n)&15)<<16)
#define SYS_CTRL_HMPROT_WCN_MEM(n) (((n)&15)<<20)
#define SYS_CTRL_HSPROT_GEA3(n)    (((n)&15)<<24)
#define SYS_CTRL_RESERVE(n)        (((n)&15)<<28)

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




#endif
