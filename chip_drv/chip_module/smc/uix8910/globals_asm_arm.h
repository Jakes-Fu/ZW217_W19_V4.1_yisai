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


#ifndef _GLOBALS_ASM_ARM_H_
#define _GLOBALS_ASM_ARM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !


/// AHB Address bus size
#define NB_BITS_ADDR                            (32)
/// AXI Address bus size of DMA_AP
#define AP_NB_BITS_ADDR                         (32)
/// System AON Apb Bus Configuration
#define AON_NB_BITS_PSEL                        (7)
#define AON_NB_BITS_PADDR                       (12)
#define AON_APB_STEP                            (EXP2(AON_NB_BITS_PADDR))
#define AON_APB_ID_IFC_BASE                     (16)
#define AON_APB_ID_SYS_CTRL                     (0)
#define AON_APB_ID_LVDS                         (1)
#define AON_APB_ID_LPS                          (2)
#define AON_APB_ID_I2C_M4                       (3)
#define AON_APB_ID_CALENDAR                     (4)
#define AON_APB_ID_TIMER_3                      (5)
#define AON_APB_ID_KEYPAD                       (6)
#define AON_APB_ID_GPIO_1                       (7)
#define AON_APB_ID_PWM                          (8)
#define AON_APB_ID_ANALOG_REG                   (9)
#define AON_APB_ID_AON_IFC                      (10)
#define AON_APB_ID_GPIO_2                       (11)
#define AON_APB_ID_IOMUX                        (12)
#define AON_APB_ID_SPINLOCK                     (13)
#define AON_APB_ID_EFUSE                        (14)
/// Last of AON Normal slave
#define AON_APB_ID_NORMAL_LAST                  (AON_APB_ID_IFC_BASE-1)
#define AON_APB_ID_DEBUG_UART                   (AON_APB_ID_IFC_BASE)
#define AON_APB_ID_RSV_17                       (17)
#define AON_APB_ID_RSV_18                       (18)
#define AON_APB_ID_SCI_1                        (19)
#define AON_APB_ID_SCI_2                        (20)
/// Num of System Aon Apb Slaves except Debug Host
#define AON_NB_PSEL                             (21)
/// The debug host is placed at last PSEL127 in the IFC
#define AON_APB_ID_DEBUG_HOST                   (EXP2(AON_NB_BITS_PSEL)-1)
/// Debug host slave id used for ifc channel.
#define DEBUG_HOST_SLAVE_ID                     (AON_APB_ID_DEBUG_HOST)
/// System Apb Bus Configuration
#define AP_NB_BITS_PSEL                         (7)
#define AP_NB_BITS_PADDR                        (12)
#define AP_APB_STEP                             (EXP2(AP_NB_BITS_PADDR))
#define AP_APB_ID_IFC_BASE                      (16)
#define AP_APB_ID_0_RESERVED                    (0)
#define AP_APB_ID_LPDDR_PSRAM_CTRL              (1)
#define AP_APB_ID_PAGESPY                       (2)
#define AP_APB_ID_DMC_CTRL                      (3)
#define AP_APB_ID_SYSIMEM                       (4)
#define AP_APB_ID_LZMA                          (5)
#define AP_APB_ID_GOUDA                         (6)
#define AP_APB_ID_LCDC                          (7)
#define AP_APB_ID_TIMER_1                       (8)
#define AP_APB_ID_TIMER_2                       (9)
#define AP_APB_ID_I2C_M1                        (10)
#define AP_APB_ID_I2C_M2                        (11)
#define AP_APB_ID_I2C_M3                        (12)
#define AP_APB_ID_AUD_2AD                       (13)
#define AP_APB_ID_TIMER_4                       (14)
#define AP_APB_ID_AP_IFC                        (15)
/// Last of Sys APB Normal slave
#define AP_APB_ID_NORMAL_LAST                   (AP_APB_ID_IFC_BASE-1)
#define AP_APB_ID_SDMMC1                        (AP_APB_ID_IFC_BASE)
#define AP_APB_ID_SDMMC2                        (17)
#define AP_APB_ID_SPI_1                         (18)
#define AP_APB_ID_SPI_2                         (19)
#define AP_APB_ID_SPI_3                         (20)
#define AP_APB_ID_SPI_RESERVED                  (21)
#define AP_APB_ID_UART_1                        (22)
#define AP_APB_ID_UART_2                        (23)
#define AP_APB_ID_UART_3                        (24)
#define AP_APB_ID_CAMERA                        (25)
#define AP_APB1_ID_AIF                          (26)
#define AP_APB1_ID_AIF2                         (27)
/// Num of System Apb Slaves
#define AP_NB_PSEL                              (28)
/// System Ahb Bus Configuration
#define SYS_NB_BITS_HADDR                       (18)
#define SYS_AHB_STEP                            (EXP2(SYS_NB_BITS_HADDR))
#define SYS_AHB_ID_F8                           (1)
#define SYS_AHB_ID_USBC                         (2)
#define SYS_AHB_ID_GOUDA                        (3)
#define SYS_AHB_ID_AXIDMA                       (4)
/// Num of System Ahb Slaves
#define NB_SYS_AHB_SLAVES                       (5)
#define SYS_AHB_ID_GEA3                         (5)
/// Aif Apb Bus Configuration
#define AIF_NB_BITS_PSEL                        (5)
#define AIF_NB_BITS_PADDR                       (12)
#define AIF_APB_STEP                            (EXP2(AIF_NB_BITS_PADDR))
#define AIF_APB_ID_AIF                          (7)
/// Num of Aif Apb Slaves
#define AIF_NB_PSEL                             (8)
/// Aif slave id used for ifc channel.
#define AIF_SLAVE_ID                            (AIF_APB_ID_AIF)
#define AP_APB1_NB_BITS_PADDR                   (AP_NB_BITS_PADDR)
#define AP_APB1_NB_BITS_PSEL                    (AP_NB_BITS_PSEL)
#define AUDIO_IFC_APB_STEP                      (EXP2(8))
#define AP_NB_DMA_REQ_WIDTH                     (5)
#define AP_APB_DMA_ID_SDMMC1_TX                 ((AP_APB_ID_SDMMC1-AP_APB_ID_IFC_BASE)*2+0)
#define AP_APB_DMA_ID_SDMMC1_RX                 ((AP_APB_ID_SDMMC1-AP_APB_ID_IFC_BASE)*2+1)
#define AP_APB_DMA_ID_SDMMC2_TX                 ((AP_APB_ID_SDMMC2-AP_APB_ID_IFC_BASE)*2+0)
#define AP_APB_DMA_ID_SDMMC2_RX                 ((AP_APB_ID_SDMMC2-AP_APB_ID_IFC_BASE)*2+1)
#define AP_APB_DMA_ID_SPI_1_TX                  ((AP_APB_ID_SPI_1-AP_APB_ID_IFC_BASE)*2+0)
#define AP_APB_DMA_ID_SPI_1_RX                  ((AP_APB_ID_SPI_1-AP_APB_ID_IFC_BASE)*2+1)
#define AP_APB_DMA_ID_SPI_2_TX                  ((AP_APB_ID_SPI_2-AP_APB_ID_IFC_BASE)*2+0)
#define AP_APB_DMA_ID_SPI_2_RX                  ((AP_APB_ID_SPI_2-AP_APB_ID_IFC_BASE)*2+1)
#define AP_APB_DMA_ID_SPI_3_TX                  ((AP_APB_ID_SPI_3-AP_APB_ID_IFC_BASE)*2+0)
#define AP_APB_DMA_ID_SPI_3_RX                  ((AP_APB_ID_SPI_3-AP_APB_ID_IFC_BASE)*2+1)
#define AP_APB_DMA_ID_10_RESERVED               ((AP_APB_ID_SPI_RESERVED-AP_APB_ID_IFC_BASE)*2+0)
#define AP_APB_DMA_ID_11_RESERVED               ((AP_APB_ID_SPI_RESERVED-AP_APB_ID_IFC_BASE)*2+1)
#define AP_APB_DMA_ID_FREE_TX                   ((AP_APB_ID_CAMERA-AP_APB_ID_IFC_BASE)*2+0)
#define AP_APB_DMA_ID_CAMERA_RX                 ((AP_APB_ID_CAMERA-AP_APB_ID_IFC_BASE)*2+1)
/// Num of sys ifc dma req
#define AP_NB_DMA_REQ                           (20)
#define SYS_AXI_DMA_ID_UART_1_RX                (0)
#define SYS_AXI_DMA_ID_UART_1_TX                (1)
#define SYS_AXI_DMA_ID_UART_2_RX                (2)
#define SYS_AXI_DMA_ID_UART_2_TX                (3)
#define SYS_AXI_DMA_ID_UART_3_RX                (4)
#define SYS_AXI_DMA_ID_UART_3_TX                (5)
#define SYS_AXI_DMA_ID_ZBUSMON                  (6)
#define AON_NB_DMA_REQ_WIDTH                    (5)
#define AON_APB_DMA_ID_TX_DEBUG_UART            ((AON_APB_ID_DEBUG_UART-AON_APB_ID_IFC_BASE)*2+0)
#define AON_APB_DMA_ID_RX_DEBUG_UART            ((AON_APB_ID_DEBUG_UART-AON_APB_ID_IFC_BASE)*2+1)
#define AON_APB_DMA_ID_TX_SCI_1                 ((AON_APB_ID_SCI_1-AON_APB_ID_IFC_BASE)*2+0)
#define AON_APB_DMA_ID_RX_SCI_1                 ((AON_APB_ID_SCI_1-AON_APB_ID_IFC_BASE)*2+1)
#define AON_APB_DMA_ID_TX_SCI_2                 ((AON_APB_ID_SCI_2-AON_APB_ID_IFC_BASE)*2+0)
#define AON_APB_DMA_ID_RX_SCI_2                 ((AON_APB_ID_SCI_2-AON_APB_ID_IFC_BASE)*2+1)
/// Num of aon ifc dma req
#define AON_NB_DMA_REQ                          (10)
/// System IRQ IDs
#define SYS_IRQ_ID_PAGE_SPY                     (0)
#define SYS_IRQ_ID_IMEM                         (1)
#define SYS_IRQ_ID_TIMER_1                      (2)
#define SYS_IRQ_ID_TIMER_1_OS                   (3)
#define SYS_IRQ_ID_TIMER_2                      (4)
#define SYS_IRQ_ID_TIMER_2_OS                   (5)
#define SYS_IRQ_ID_TIMER_4                      (6)
#define SYS_IRQ_ID_TIMER_4_OS                   (7)
#define SYS_IRQ_ID_I2C_M1                       (8)
#define SYS_IRQ_ID_I2C_M2                       (9)
#define SYS_IRQ_ID_I2C_M3                       (10)
#define SYS_IRQ_ID_AIF_APB_0                    (11)
#define SYS_IRQ_ID_AIF_APB_1                    (12)
#define SYS_IRQ_ID_AIF_APB_2                    (13)
#define SYS_IRQ_ID_AIF_APB_3                    (14)
#define SYS_IRQ_ID_SDMMC1                       (15)
#define SYS_IRQ_ID_SDMMC2                       (16)
#define SYS_IRQ_ID_SPI_1                        (17)
#define SYS_IRQ_ID_SPI_2                        (18)
#define SYS_IRQ_ID_SPI_3                        (19)
#define SYS_IRQ_ID_UART_1                       (20)
#define SYS_IRQ_ID_UART_2                       (21)
#define SYS_IRQ_ID_UART_3                       (22)
#define SYS_IRQ_ID_CAMERA                       (23)
#define SYS_IRQ_ID_LZMA                         (24)
#define SYS_IRQ_ID_GOUDA                        (25)
#define SYS_IRQ_ID_F8                           (26)
#define SYS_IRQ_ID_USBC                         (27)
#define SYS_IRQ_ID_AXIDMA                       (28)
#define SYS_IRQ_ID_PMU_APCPU                    (29)
#define SYS_IRQ_ID_PMU_CPCPU                    (30)
#define SYS_IRQ_ID_LCD                          (31)
#define SYS_IRQ_ID_LPS                          (32)
#define SYS_IRQ_ID_I2C_M4                       (33)
#define SYS_IRQ_ID_TIMER_3                      (34)
#define SYS_IRQ_ID_TIMER_3_OS                   (35)
#define SYS_IRQ_ID_KEYPAD                       (36)
#define SYS_IRQ_ID_GPIO_1                       (37)
#define SYS_IRQ_ID_GPIO_2                       (38)
#define SYS_IRQ_ID_DEBUG_UART                   (39)
#define SYS_IRQ_ID_SCI_1                        (40)
#define SYS_IRQ_ID_SCI_2                        (41)
#define SYS_IRQ_ID_ADI                          (42)
#define SYS_IRQ_ID_CP_IDLE_2_H                  (43)
#define SYS_IRQ_ID_MAILBOX_ARM_AP               (44)
#define SYS_IRQ_ID_MAILBOX_ARM_CP               (45)
#define SYS_IRQ_ID_LTEM1_FRAME                  (46)
#define SYS_IRQ_ID_LTEM2_FRAME                  (47)
#define SYS_IRQ_ID_CP_IDLE_H                    (48)
#define SYS_IRQ_ID_RFSPI_CONFLICT               (49)
#define SYS_IRQ_ID_CP_WD_RESET                  (50)
#define SYS_IRQ_ID_GSM_FRAME                    (51)
#define SYS_IRQ_ID_PWRCTRL_TIMEOUT              (52)
#define SYS_IRQ_ID_LTE_COEFF                    (53)
#define SYS_IRQ_ID_LTE_CSIRS                    (54)
#define SYS_IRQ_ID_LTE_DLFFT                    (55)
#define SYS_IRQ_ID_LTE_DLFFT_ERR                (56)
#define SYS_IRQ_ID_LTE_IDDET                    (57)
#define SYS_IRQ_ID_LTE_LDTC1_CTRL               (58)
#define SYS_IRQ_ID_LTE_LDTC1_DATA               (59)
#define SYS_IRQ_ID_LTE_LDTC1_VIT                (60)
#define SYS_IRQ_ID_LTE_LDTC                     (61)
#define SYS_IRQ_ID_LTE_MEASPWR                  (62)
#define SYS_IRQ_ID_LTE_PUSCH                    (63)
#define SYS_IRQ_ID_LTE_RFMODEL                  (64)
#define SYS_IRQ_ID_LTE_RX_TRACE                 (65)
#define SYS_IRQ_ID_LTE_TXRX                     (66)
#define SYS_IRQ_ID_LTE_TX_TRACE                 (67)
#define SYS_IRQ_ID_LTE_ULDFT                    (68)
#define SYS_IRQ_ID_LTE_ULDFT_ERR                (69)
#define SYS_IRQ_ID_LTE_ULPCDCI                  (70)
#define SYS_IRQ_ID_LTE_CORR                     (71)
#define SYS_IRQ_ID_ZSP_AXIDMA                   (72)
#define SYS_IRQ_ID_ZSP_BUSMON                   (73)
#define SYS_IRQ_ID_ZSP_WD                       (74)
#define SYS_IRQ_ID_GGE_TCU_SYNC                 (75)
#define SYS_IRQ_ID_GGE_FINT                     (76)
#define SYS_IRQ_ID_GGE_TCU_0                    (77)
#define SYS_IRQ_ID_GGE_TCU_1                    (78)
#define SYS_IRQ_ID_GGE_COMP_INT                 (79)
#define SYS_IRQ_ID_WCN_WLAN                     (80)
#define SYS_IRQ_ID_WCN_SOFT                     (81)
#define SYS_IRQ_ID_WCN_WDT_RST                  (82)
#define SYS_IRQ_ID_WCN_AWAKE                    (83)
#define SYS_IRQ_ID_RF_WDG_RST                   (84)
#define SYS_IRQ_ID_SPIFLASH                     (85)
#define SYS_IRQ_ID_GGE_NB_RX_DSP                (86)
#define SYS_IRQ_ID_GGE_NB_RX_MCU                (87)
#define SYS_IRQ_ID_GGE_NB_TX_DSP                (88)
#define SYS_IRQ_ID_GGE_NB_ACC_DSP               (89)
#define SYS_IRQ_ID_GPRS_0                       (90)
#define SYS_IRQ_ID_GPRS_1                       (91)
#define SYS_IRQ_ID_DMC                          (92)
#define SYS_IRQ_ID_AXIDMA_1                     (93)
#define SYS_IRQ_ID_AUD_2AD                      (94)
#define SYS_IRQ_ID_SPIFLASH1                    (95)
/// Num of System IRQS
#define NB_SYS_IRQ                              (96)
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
#define BB_SYSCTRL_ID_SYSREG                    (0)
#define BB_SYSCTRL_ID_CLKRST                    (1)
#define BB_SYSCTRL_ID_MAILBOX                   (2)
#define BB_SYSCTRL_ID_MONITOR                   (3)
#define BB_SYSCTRL_ID_CP_WD_TIMER               (4)
#define BB_SYSCTRL_ID_IDLE                      (EXP2(BB_SYS_NB_BITS_PSEL))
#define BB_SYSCTRL_ID_PWRCTRL                   (EXP2(BB_SYS_NB_BITS_PSEL)+BB_SYSCTRL_ID_IDLE)
#define CP_ZSP_SYS_PADDR                        (12)
#define CP_ZSP_NB_BITS_PSEL                     (12)
#define CP_ZSP_SYS_STEP                         (EXP2(CP_ZSP_SYS_PADDR))
#define CP_ZSP_SYS_ID_ZSP_AXIDMA                (0)
#define CP_ZSP_SYS_ID_ZSP_UART                  (EXP2(CP_ZSP_NB_BITS_PSEL))
#define CP_ZSP_SYS_ID_ZSP_WD                    (4097)
#define CP_ZSP_SYS_ID_ZSP_IRQH                  (4098)
#define CP_ZSP_SYS_ID_BUSMON                    (4099)
#define CP_LTE_SYS_PADDR                        (20)
#define CP_LTE_NB_BITS_PSEL                     (4)
#define CP_LTE_SYS_STEP                         (EXP2(CP_LTE_SYS_PADDR))
#define CP_LTE_SYS_ID_TXRX                      (0)
#define CP_LTE_SYS_ID_RFAD                      (1)
#define CP_LTE_SYS_ID_COEFF                     (2)
#define CP_LTE_SYS_ID_LDTC                      (3)
#define CP_LTE_SYS_ID_MEASPWR                   (5)
#define CP_LTE_SYS_ID_IDDET                     (6)
#define CP_LTE_SYS_ID_ULDFT                     (7)
#define CP_LTE_SYS_ID_PUSCH                     (8)
#define CP_LTE_SYS_ID_ULPCDCI                   (9)
#define CP_LTE_SYS_ID_DLFFT                     (10)
#define CP_LTE_SYS_ID_CSIRS                     (11)
#define CP_LTE_SYS_ID_LDTC1                     (EXP2(CP_LTE_NB_BITS_PSEL))
/// GGE System Apb Bus Configuration
#define GGE_SYS_NB_BITS_PSEL                    (5)
#define GGE_SYS_NB_BITS_PADDR                   (12)
#define GGE_SYS_APB_STEP                        (EXP2(GGE_SYS_NB_BITS_PADDR))
#define GGE_SYS_APB_ID_0_RSV                    (0)
#define GGE_SYS_APB_ID_TCU                      (1)
#define GGE_SYS_APB_ID_2_RSV                    (2)
#define GGE_SYS_APB_ID_3_RSV                    (3)
#define GGE_SYS_APB_ID_4_RSV                    (4)
#define GGE_SYS_APB_ID_WDT                      (5)
#define GGE_SYS_APB_ID_GGE_IFC                  (6)
#define GGE_SYS_APB_ID_7_RSV                    (7)
#define GGE_SYS_APB_ID_8_RSV                    (8)
#define GGE_SYS_APB_ID_9_RSV                    (9)
#define GGE_SYS_APB_ID_10_RSV                   (10)
#define GGE_SYS_APB_ID_11_RSV                   (11)
#define GGE_SYS_APB_ID_12_RSV                   (12)
#define GGE_SYS_APB_ID_13_RSV                   (13)
#define GGE_SYS_APB_ID_RFSPI                    (14)
/// Num of GGE System Apb Slaves
#define GGE_SYS_APB_NB_PSEL                     (15)
#define GGE_SYS_NB_DMA_REQ_WIDTH                (5)
/// Num of gge ifc dma req
#define GGE_SYS_NB_DMA_REQ                      (0)
/// GGE System Apb Bus Configuration
#define GGE_BB_NB_BITS_PSEL                     (5)
#define GGE_BB_NB_BITS_PADDR                    (12)
#define GGE_BB_APB_STEP                         (EXP2(GGE_BB_NB_BITS_PADDR))
#define GGE_BB_APB_ID_XCOR                      (0)
#define GGE_BB_APB_ID_CORDIC                    (1)
#define GGE_BB_APB_ID_ITLV                      (2)
#define GGE_BB_APB_ID_VITAC                     (3)
#define GGE_BB_APB_ID_EXCOR                     (4)
#define GGE_BB_APB_ID_CHOLK                     (5)
#define GGE_BB_APB_ID_CIPHER                    (6)
#define GGE_BB_APB_ID_EVITAC                    (7)
#define GGE_BB_APB_ID_CP2                       (8)
#define GGE_BB_APB_ID_BCPU_CORE                 (9)
#define GGE_BB_APB_ID_BCPU_DBG                  (10)
#define GGE_BB_APB_ID_ROM                       (11)
#define GGE_BB_APB_ID_RF_IF                     (12)
#define GGE_BB_APB_ID_IRQ                       (13)
#define GGE_BB_APB_ID_SYSCTRL                   (14)
#define GGE_BB_APB_ID_A53                       (15)
#define GGE_BB_APB_ID_NB_CTRL                   (16)
#define GGE_BB_APB_ID_NB_COMMON                 (17)
#define GGE_BB_APB_ID_NB_INTC                   (18)
#define GGE_BB_APB_ID_NB_CELL_SEARCH            (19)
#define GGE_BB_APB_ID_NB_FFT_RSRP               (20)
#define GGE_BB_APB_ID_NB_VITERBI                (21)
#define GGE_BB_APB_ID_NB_MEAS                   (22)
#define GGE_BB_APB_ID_NB_DS_BSEL                (23)
#define GGE_BB_APB_ID_NB_TX_PUSCH_ENC           (24)
#define GGE_BB_APB_ID_NB_TX_CHSC                (25)
#define GGE_BB_APB_ID_NB_TX_FRONTEND            (26)
/// Num of GGE Baseband Apb Slaves
#define GGE_BB_APB_NB_PSEL                      (27)
/// GGE IRQ IDs
#define GGE_BB_IRQ_ID_TCU0                      (0)
#define GGE_BB_IRQ_ID_TCU1                      (1)
#define GGE_BB_IRQ_ID_FRAME                     (2)
#define GGE_BB_IRQ_ID_A53                       (3)
/// Number of GGE Pulse IRQ
#define NB_GGE_BB_IRQ_PULSE                     (4)
#define GGE_BB_IRQ_ID_4_RSV                     (4)
#define GGE_BB_IRQ_ID_5_RSV                     (5)
#define GGE_BB_IRQ_ID_RFIF_DBG_2G               (6)
#define GGE_BB_IRQ_ID_RFIF_DBG_NB               (7)
#define GGE_BB_IRQ_ID_RFIF_RX                   (8)
#define GGE_BB_IRQ_ID_RFIF_TX                   (9)
#define GGE_BB_IRQ_ID_EVITAC                    (10)
#define GGE_BB_IRQ_ID_EXCOR                     (11)
#define GGE_BB_IRQ_ID_ITLV                      (12)
#define GGE_BB_IRQ_ID_VITAC                     (13)
#define GGE_BB_IRQ_ID_XCOR                      (14)
#define GGE_BB_IRQ_ID_CHOLK                     (15)
#define GGE_BB_IRQ_ID_16_RSV                    (16)
#define GGE_BB_IRQ_ID_MAILBOX                   (17)
#define GGE_BB_IRQ_ID_TCU_SYNC                  (18)
#define GGE_BB_IRQ_ID_NB_RX                     (19)
#define GGE_BB_IRQ_ID_NB_TX                     (20)
#define GGE_BB_IRQ_ID_NB_ACC                    (21)
/// Num of Gge IRQS
#define NB_GGE_BB_IRQ                           (22)
/// Number of GGE BB Level IRQ
#define NB_GGE_BB_IRQ_LEVEL                     (NB_GGE_BB_IRQ - NB_GGE_BB_IRQ_PULSE)
#define BB_SYMBOL_SIZE                          (13)
#define VITAC_MULT_SIZE                         (14)
#define BB_SRAM_ADDR_WIDTH                      (13)
/// System RF Apb Bus Configuration
#define RF_NB_BITS_PADDR                        (12)
#define RF_APB_STEP                             (EXP2(RF_NB_BITS_PADDR))
#define RF_APB_ID_CTRL_1                        (0)
#define RF_APB_ID_CTRL_2                        (1)
#define RF_APB_ID_DFE_1                         (2)
#define RF_APB_ID_DFE_2                         (3)
#define RF_APB_ID_RTC                           (4)
#define RF_APB_ID_SYS_CTRL                      (5)
#define RF_APB_ID_TIMER_1                       (6)
#define RF_APB_ID_TIMER_2                       (7)
#define RF_APB_ID_TIMER_3                       (8)
#define RF_APB_ID_UART                          (9)
#define RF_APB_ID_WATCHDOG                      (10)
#define RF_APB_ID_TSEN_ADC                      (11)
#define RF_APB_ID_RFFE                          (12)
/// Num of RF Apb Slaves
#define RF_NB_PSEL                              (13)
/// WCN System Apb Bus Configuration
#define WCN_SYS_NB_BITS_PSEL                    (4)
#define WCN_SYS_NB_BITS_PADDR                   (12)
#define WCN_SYS_APB_STEP                        (EXP2(WCN_SYS_NB_BITS_PADDR))
#define WCN_SYS_APB_ID_SYS_CTRL                 (0)
#define WCN_SYS_APB_ID_DBM                      (1)
#define WCN_SYS_APB_ID_SYS_IFC                  (2)
#define WCN_SYS_APB_ID_BT_CORE                  (3)
#define WCN_SYS_APB_ID_UART                     (4)
#define WCN_SYS_APB_ID_RF_IF                    (5)
#define WCN_SYS_APB_ID_MODEM                    (6)
#define WCN_SYS_APB_ID_WLAN                     (7)
#define WCN_SYS_APB_ID_WDT                      (8)
#define WCN_SYS_APB_ID_TRAP                     (9)
#define WCN_SYS_APB_ID_SYSTICK                  (10)
#define WCN_SYS_APB_ID_COMREGS_WCN              (11)
#define WCN_SYS_APB_ID_COMREGS_AP               (12)
/// Num of WCN System Apb Slaves
#define WCN_SYS_APB_NB_PSEL                     (13)

//==============================================================================
// Aon_Apb_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Ap_Apb_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Ahb_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aif_Apb_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Ap_Ifc_Request_IDs
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Axi_DMA_Request_IDs
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Aon_Ifc_Request_IDs
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Sys_Irq_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// BB_SYSCTRL_ID
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// CP_ZSP_SYS_ID
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// CP_LTE_SYS_ID
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Gge_Sys_Apb_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Gge_Ifc_Request_IDs
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Gge_Bb_Apb_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Gge_Bb_Irq_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Rf_Apb_Id
//------------------------------------------------------------------------------
///
//==============================================================================



//==============================================================================
// Wcn_Sys_Apb_Id
//------------------------------------------------------------------------------
///
//==============================================================================




#endif
