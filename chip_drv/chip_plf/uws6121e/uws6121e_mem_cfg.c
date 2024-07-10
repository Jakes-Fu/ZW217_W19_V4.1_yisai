/******************************************************************************
 ** File Name:    sc6531efm_mem_cfg.c                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         07/27/2010                                                  *
 ** Copyright:    2010  Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **07/27/2010     Steve.zhan      Create.                                     *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "sci_types.h"
#include "uws6121e_reg_base.h"
#include "../../../MS_Ref/export/inc/iram_mgr.h"
#include "uws6121e_mem_cfg.h"
#include "mem_dump.h"
#include "mem_cfg.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
    
/******************************************************/
/******************start dump ip reg*******************/
/******************************************************/
#define IP_REG_AP_IRAM_START                (0x00100000)
#define IP_REG_AP_IRAM_END                  (0x00100000+0x4000)
#define IP_REG_SPIFLASH1_REG_START          (0x02000000)
#define IP_REG_SPIFLASH1_REG_END            (0x02000000+0xc4)
#define IP_REG_SPIFLASH2_REG_START          (0x02040000)
#define IP_REG_SPIFLASH2_REG_END            (0x02040000+0xc4)
#define IP_REG_GOUDA_START                  (0x02080000)
#define IP_REG_GOUDA_END                    (0x02080000+0x2e40)
#define IP_REG_AP_AXIDMA_START              (0x020c0000)
#define IP_REG_AP_AXIDMA_END                (0x020c0000+0x32c)
#define IP_REG_USB_START                    (0x02100000)
#define IP_REG_USB_END                      (0x02100000+0x358)
#define IP_REG_EMMC_START                   (0x04006000)
#define IP_REG_EMMC_END                     (0x04006000+0x290)
#define IP_REG_SPI1_START                   (0x04008000)
#define IP_REG_SPI1_END                     (0x04008000+0x7c)
#define IP_REG_SDMMC_START                  (0x04403000)
#define IP_REG_SDMMC_END                    (0x04403000+0x844)
#define IP_REG_CAMERA_START                 (0x04404000)
#define IP_REG_CAMERA_END                   (0x04404000+0xf5c)
#define IP_REG_AP_IFC_START                 (0x04405000)
#define IP_REG_AP_IFC_END                   (0x04405000+0xd8)
#define IP_REG_LZMA_START                   (0x04800000)
#define IP_REG_LZMA_END                     (0x04800000+0x4c)
#define IP_REG_AP_IMEM_START                (0x04801000)
#define IP_REG_AP_IMEM_END                  (0x04801000+0x40)
#define IP_REG_AP_BUSMON_START              (0x04802000)
#define IP_REG_AP_BUSMON_END                (0x04802000+0x9c)
#define IP_REG_APB_REG_START                (0x04803000)
#define IP_REG_APB_REG_END                  (0x04803000+0x92C)
#define IP_REG_GOUDA_REG_START              (0x04804000)
#define IP_REG_GOUDA_REG_END                (0x04804000+0xa4)
#define IP_REG_TIMER1_START                 (0x04805000)
#define IP_REG_TIMER1_END                   (0x04805000+0x28)
#define IP_REG_TIMER2_START                 (0x04806000)
#define IP_REG_TIMER2_END                   (0x04806000+0x3c)
#define IP_REG_TIMER5_START                 (0x04806800)
#define IP_REG_TIMER5_END                   (0x04806800+0x3c)
#define IP_REG_I2C1_START                   (0x04807000)
#define IP_REG_I2C1_END                     (0x04807000+0x14)
#define IP_REG_I2C2_START                   (0x04808000)
#define IP_REG_I2C2_END                     (0x04808000+0x14)
#define IP_REG_GPT3_START                   (0x04809000)
#define IP_REG_GPT3_END                     (0x04809000+0x44)
#define IP_REG_AP_CLK_START                 (0x0480a000)
#define IP_REG_AP_CLK_END                   (0x0480a000+0x104)
#define IP_REG_CP_ROM_START                 (0x10000000)
#define IP_REG_CP_ROM_END                   (0x10000000+0x80000)
#define IP_REG_CP_IRAM_START                (0x10100000)
#define IP_REG_CP_IRAM_END                  (0x10100000+0x34000)
#define IP_REG_F8_REG_START                 (0x12000000)
#define IP_REG_F8_REG_END                   (0x12000000+0x40)
#define IP_REG_CP_AXIDMA_START              (0x12040000)
#define IP_REG_CP_AXIDMA_END                (0x12040000+0x32c)
#define IP_REG_FREQ_BIAS_START              (0x12080000)
#define IP_REG_FREQ_BIAS_END                (0x12080000+0xf4)
#define IP_REG_FREQ_BIAS_RAM_START          (0x12080400)
#define IP_REG_FREQ_BIAS_RAM_END            (0x12080400+0x0400)
#define IP_REG_GLB_REG_START                (0x120c0000)
#define IP_REG_GLB_REG_END                  (0x120c0000+0xc8)
#define IP_REG_SCI1_START                   (0x14000000)
#define IP_REG_SCI1_END                     (0x14000000+0x34)
#define IP_REG_SCI2_START                   (0x14001000)
#define IP_REG_SCI2_END                     (0x14001000+0x34)
#define IP_REG_CP_IFC_START                 (0x14002000)
#define IP_REG_CP_IFC_END                   (0x14002000+0x60)
#define IP_REG_CP_IMEM_START                (0x14003000)
#define IP_REG_CP_IMEM_END                  (0x14003000+0xc0)
#define IP_REG_CP_BUSMON_START              (0x14004000)
#define IP_REG_CP_BUSMON_END                (0x14004000+0x9c)
#define IP_REG_TIMER3_START                 (0x14006000)
#define IP_REG_TIMER3_END                   (0x14006000+0x28)
#define IP_REG_TIMER4_START                 (0x14007000)
#define IP_REG_TIMER4_END                   (0x14007000+0x3c)
#define IP_REG_WLAN_11B_START               (0x14008000)
#define IP_REG_WLAN_11B_END                 (0x14008000+0xf4)
#define IP_REG_CP_IRQ0_START                (0x14009000)
#define IP_REG_CP_IRQ0_END                  (0x14009000+0x150)
#define IP_REG_CP_IRQ1_START                (0x1400a000)
#define IP_REG_CP_IRQ1_END                  (0x1400a000+0x150)
#define IP_REG_TXRX_START                   (0x18000000)
#define IP_REG_TXRX_END                     (0x18000000+0x1c008)
#define IP_REG_RFAD_START                   (0x18100000)
#define IP_REG_RFAD_END                     (0x18100000+0x2004)
#define IP_REG_COEFF_START                  (0x18200000)
#define IP_REG_COEFF_END                    (0x18200000+0x18)
#define IP_REG_MEASPWR_START                (0x18500000)
#define IP_REG_MEASPWR_END                  (0x18500000+0x38004)
#define IP_REG_IDDET_START                  (0x18600000)
#define IP_REG_IDDET_END                    (0x18600000+0xd004)
#define IP_REG_ULDFT_START                  (0x18700000)
#define IP_REG_ULDFT_END                    (0x18700000+0x3804)
#define IP_REG_PUSCH_START                  (0x18800000)
#define IP_REG_PUSCH_END                    (0x18800000+0x30004)
#define IP_REG_ULPCDCI_START                (0x18900000)
#define IP_REG_ULPCDCI_END                  (0x18900000+0x60)
#define IP_REG_DLFFT_START                  (0x18a00000)
#define IP_REG_DLFFT_END                    (0x18a00000+0x100)
#define IP_REG_SCIRS_START                  (0x18b00000)
#define IP_REG_SCIRS_END                    (0x18b00000+0xc004)
#define IP_REG_HSDL_START                   (0x18c00000)
#define IP_REG_HSDL_END                     (0x18c00000+0x310)
#define IP_REG_RXCAPT_START                 (0x1a000000)
#define IP_REG_RXCAPT_END                   (0x1a000000+0x16004)
#define IP_REG_RF_ISRAM_START               (0x50000000)
#define IP_REG_RF_ISRAM_END                 (0x50000000+0x8000)
#define IP_REG_RF_DSRAM_START               (0x50008000)
#define IP_REG_RF_DSRAM_END                 (0x50008000+0x4000)
#define IP_REG_RISCV_DEBUG_START            (0x50020000)
#define IP_REG_RISCV_DEBUG_END              (0x50020000+0x7c44)
#define IP_REG_RISCV_INTC_START             (0x50028000)
#define IP_REG_RISCV_INTC_END               (0x50028000+0x10)
#define IP_REG_RISCV_SLEEP_START            (0x5002c000)
#define IP_REG_RISCV_SLEEP_END              (0x5002c000+0x8)
#define IP_REG_RF_INTERFACE_REG_START       (0x50030000)
#define IP_REG_RF_INTERFACE_REG_END         (0x50030000+0x360)
#define IP_REG_RF_ANA_REG_START             (0x50031000)
#define IP_REG_RF_ANA_REG_END               (0x50031000+0x198)
#define IP_REG_DFE_START                    (0x50032000)
#define IP_REG_DFE_END                      (0x50032000+0x658)
#define IP_REG_TX_DLPF_START                (0x50033000)
#define IP_REG_TX_DLPF_END                  (0x50033000+0xb8)
#define IP_REG_RTC1_START                   (0x50034000)
#define IP_REG_RTC1_END                     (0x50034000+0xdc)
#define IP_REG_RF_SYS_CTRL_START            (0x50035000)
#define IP_REG_RF_SYS_CTRL_END              (0x50035000+0x15c)
#define IP_REG_LTE_GNSS_BITMAP_START        (0x50036000)
#define IP_REG_LTE_GNSS_BITMAP_END          (0x50036000+0xac)
#define IP_REG_RX_DLPF_START                (0x50037000)
#define IP_REG_RX_DLPF_END                  (0x50037000+0xb8)
#define IP_REG_RFFE_START                   (0x50038000)
#define IP_REG_RFFE_END                     (0x50038000+0x20)
#define IP_REG_WATCHDOG_START               (0x50039000)
#define IP_REG_WATCHDOG_END                 (0x50039000+0x1000)
#define IP_REG_TIMER0_START                 (0x5003a000)
#define IP_REG_TIMER0_END                   (0x5003a000+0x1000)
#define IP_REG_TEST_TSEN_START              (0x5003b000)
#define IP_REG_TEST_TSEN_END                (0x5003b000+0x54)
#define IP_REG_AON_IRAM_START               (0x50800000)
#define IP_REG_AON_IRAM_END                 (0x50800000+0x14000)
#define IP_REG_SPINLOCK_START               (0x51000000)
#define IP_REG_SPINLOCK_END                 (0x51000000+0x1000)
#define IP_REG_ADI_MST_START                (0x51100000)
#define IP_REG_ADI_MST_END                  (0x51100000+0x60)
#define IP_REG_EFUSE_START                  (0x51200000)
#define IP_REG_EFUSE_END                    (0x51200000+0x84)
#define IP_REG_TZPC_START                   (0x51300000)
#define IP_REG_TZPC_END                     (0x51300000+0x70)
#define IP_REG_AIF_START                    (0x5140a000)
#define IP_REG_AIF_END                      (0x5140a000+0x1c)
#define IP_REG_AON_IFC_START                (0x5140e000)
#define IP_REG_AON_IFC_END                  (0x5140e000+0x88)
#define IP_REG_DBG_HOST_START               (0x5140f000)
#define IP_REG_DBG_HOST_END                 (0x5140f000+0x1c)
#define IP_REG_SYS_CTRL_START               (0x51500000)
#define IP_REG_SYS_CTRL_END                 (0x51500000+0x8d4)
#define IP_REG_ANA_WRAP1_START              (0x51501000)
#define IP_REG_ANA_WRAP1_END                (0x51501000+0x870)
#define IP_REG_MON_CTRL_START               (0x51502000)
#define IP_REG_MON_CTRL_END                 (0x51502000+0x48)
#define IP_REG_GPIO2_START                  (0x51503000)
#define IP_REG_GPIO2_END                    (0x51503000+0x50)
#define IP_REG_I2C3_START                   (0x51504000)
#define IP_REG_I2C3_END                     (0x51504000+0x14)
#define IP_REG_SCC_TOP_START                (0x51505000)
#define IP_REG_SCC_TOP_END                  (0x51505000+0x34)
#define IP_REG_SYSMAIL_START                (0x51506000)
#define IP_REG_SYSMAIL_END                  (0x51506000+0x680)
#define IP_REG_IDLE_TIMER_START             (0x51507000)
#define IP_REG_IDLE_TIMER_END               (0x51507000+0x3c4)
#define IP_REG_AON_CLK_PRE_START            (0x51508000)
#define IP_REG_AON_CLK_PRE_END              (0x51508000+0x54)
#define IP_REG_AON_CLK_CORE_START           (0x51508800)
#define IP_REG_AON_CLK_CORE_END             (0x51508800+0x1a4)
#define IP_REG_AUD_2AD_START                (0x5150a000)
#define IP_REG_AUD_2AD_END                  (0x5150a000+0x6c)
#define IP_REG_GPT2_START                   (0x5150b000)
#define IP_REG_GPT2_END                     (0x5150b000+0x54)
#define IP_REG_SPI2_START                   (0x5150c000)
#define IP_REG_SPI2_END                     (0x5150c000+0x7c)
#define IP_REG_GPT1_START                   (0x5150d000)
#define IP_REG_GPT1_END                     (0x5150d000+0x44)
#define IP_REG_DJTAG_CFG_START              (0x5150e000)
#define IP_REG_DJTAG_CFG_END                (0x5150e000+0x20)
#define IP_REG_ANA_WAP2_START               (0x5150f000)
#define IP_REG_ANA_WAP2_END                 (0x5150f000+0x818)
#define IP_REG_IOMUX_START                  (0x51510000)
#define IP_REG_IOMUX_END                    (0x51510000+0x5b8)
#define IP_REG_DMC_START                    (0x51600000)
#define IP_REG_DMC_END                      (0x51600000+0x1000)
#define IP_REG_PSRAM_PHY_START              (0x51601000)
#define IP_REG_PSRAM_PHY_END                (0x51601000+0x730)
#define IP_REG_PAGESPY_START                (0x51602000)
#define IP_REG_PAGESPY_END                  (0x51602000+0x200)
#define IP_REG_PUB_APB_REG_START            (0x51603000)
#define IP_REG_PUB_APB_REG_END              (0x51603000+0x228)
#define IP_REG_IDLE_LPS_START               (0x51702000)
#define IP_REG_IDLE_LPS_END                 (0x51702000+0x340)
#define IP_REG_GPIO1_START                  (0x51703000)
#define IP_REG_GPIO1_END                    (0x51703000+0x50)
#define IP_REG_APB_REG1_START               (0x51705000)
#define IP_REG_APB_REG1_END                 (0x51705000+0x88C)
#define IP_REG_KEYPAD_START                 (0x51706000)
#define IP_REG_KEYPAD_END                   (0x51706000+0x1c)
#define IP_REG_PWRCTRL_START                (0x51707000)
#define IP_REG_PWRCTRL_END                  (0x51707000+0x85C)
#define IP_REG_RTC_TIMER_START              (0x51708000)
#define IP_REG_RTC_TIMER_END                (0x51708000+0x44)
#define IP_REG_ANA_WARAP3_START             (0x51709000)
#define IP_REG_ANA_WARAP3_END               (0x51709000+0x8)
#define IP_REG_LPS_IFC_START                (0x5170e000)
#define IP_REG_LPS_IFC_END                  (0x5170e000+0x88)
#define IP_REG_DAP_START                    (0x51800000)
#define IP_REG_DAP_END                      (0x51800000+0x6c)
#define IP_REG_AP_QOS_START                 (0x52042100)
#define IP_REG_AP_QOS_END                   (0x52042100+0x3c)
#define IP_REG_CP_QOS_START                 (0x52043100)
#define IP_REG_CP_QOS_END                   (0x52043100+0x3c)
#define IP_REG_AON_QOS_START                (0x52044100)
#define IP_REG_AON_QOS_END                  (0x52044100+0x3c)
#define IP_REG_GNSS_QOS_START               (0x52045100)
#define IP_REG_GNSS_QOS_END                 (0x52045100+0x3c)
#define IP_REG_GNSS_AHB_START               (0x1c000000)
#define IP_REG_GNSS_AHB_END                 (0x1c000000+0x32fc)
#define IP_REG_GNSS_CLK_RF_START            (0x1c010000)
#define IP_REG_GNSS_CLK_RF_END              (0x1c010000+0x0048)
#define IP_REG_RFT_START                    (0x1c020000)
#define IP_REG_RFT_END                      (0x1c020000+0x0144)
#define IP_REG_GNSS_SPI_START               (0x1c040000)
#define IP_REG_GNSS_SPI_END                 (0x1c040000+0x0014)
#define IP_REG_PPS_START                    (0x1c050000)
#define IP_REG_PPS_END                      (0x1c050000+0x0040)
#define IP_REG_GNSS_GLB_REG_START           (0x1cc00000)
#define IP_REG_GNSS_GLB_REG_END             (0x1cc00000+0x006c)
#define IP_REG_PP_START                     (0x1cc10000)
#define IP_REG_PP_END                       (0x1cc10000+0x0358)
#define IP_REG_AE_FIFO0_START               (0x1cd00000)
#define IP_REG_AE_FIFO0_END                 (0x1cd00000+0x0070)
#define IP_REG_AXI_START                    (0x1cd02000)
#define IP_REG_AXI_END                      (0x1cd02000+0x0088)
#define IP_REG_AE_FIFO1_START               (0x1cd04000)
#define IP_REG_AE_FIFO1_END                 (0x1cd04000+0x0070)
#define IP_REG_AE_REG_START                 (0x1cd10000)
#define IP_REG_AE_REG_END                   (0x1cd10000+0x1000)
#define IP_REG_TE_FIFO0_START               (0x1ce00000)
#define IP_REG_TE_FIFO0_END                 (0x1ce00000+0x0068)
#define IP_REG_TE_FIFO1_START               (0x1ce04000)
#define IP_REG_TE_FIFO1_END                 (0x1ce04000+0x0068)
#define IP_REG_TE_REG_START                 (0x1ce0c000)
#define IP_REG_TE_REG_END                   (0x1ce0c000+0x0ffc)
#define IP_REG_VITEBI_START                 (0x1cf0c000)
#define IP_REG_VITEBI_END                   (0x1cf0c000+0x0018)
#define IP_REG_RTC_START                    (0x1cf14000)
#define IP_REG_RTC_END                      (0x1cf14000+0x0040)
#define IP_REG_DATA2RAM_START               (0x1cf18000)
#define IP_REG_DATA2RAM_END                 (0x1cf18000+0x001c)
#define IP_REG_LDPC_START                   (0x1cf2c000)
#define IP_REG_LDPC_END                     (0x1cf2c000+0x001c)
#define IP_REG_UART1A_START                 (0x51700000)
#define IP_REG_UART1A_END                   (0x51700000+0x8)
#define IP_REG_UART1B_START                 (0x5170000c)
#define IP_REG_UART1B_END                   (0x5170000c+0x20)
#define IP_REG_UART2A_START                 (0x51400000)
#define IP_REG_UART2A_END                   (0x51400000+0x8)
#define IP_REG_UART2B_START                 (0x5140000c)
#define IP_REG_UART2B_END                   (0x5140000c+0x20)
#define IP_REG_UART3A_START                 (0x51401000)
#define IP_REG_UART3A_END                   (0x51401000+0x8)
#define IP_REG_UART3B_START                 (0x5140100c)
#define IP_REG_UART3B_END                   (0x5140100c+0x20)
#define IP_REG_UART4A_START                 (0x04400000)
#define IP_REG_UART4A_END                   (0x04400000+0x8)
#define IP_REG_UART4B_START                 (0x0440000c)
#define IP_REG_UART4B_END                   (0x0440000c+0x20)
#define IP_REG_UART5A_START                 (0x04401000)
#define IP_REG_UART5A_END                   (0x04401000+0x8)
#define IP_REG_UART5B_START                 (0x0440100c)
#define IP_REG_UART5B_END                   (0x0440100c+0x20)
#define IP_REG_UART6A_START                 (0x04402000)
#define IP_REG_UART6A_END                   (0x04402000+0x8)
#define IP_REG_UART6B_START                 (0x0440200c)
#define IP_REG_UART6B_END                   (0x0440200c+0x20)
#define IP_REG_DBG_UARTA_START              (0x51402000)
#define IP_REG_DBG_UARTA_END                (0x51402000+0x8)
#define IP_REG_DBG_UARTB_START              (0x5140200c)
#define IP_REG_DBG_UARTB_END                (0x5140200c+0x10)
#define IP_REG_LDTCL_1_START                (0x19000000)
#define IP_REG_LDTCL_1_END                  (0x19000000+0x300)
#define IP_REG_LDTCL_2_START                (0x19100000)
#define IP_REG_LDTCL_2_END                  (0x19100000+0x70000)
#define IP_REG_LDTCL_3_START                (0x19200000)
#define IP_REG_LDTCL_3_END                  (0x19200000+0x40000)
#define IP_REG_LDTCL_4_START                (0x19300000)
#define IP_REG_LDTCL_4_END                  (0x19300000+0x10000)
#define IP_REG_LDTCL_5_START                (0x19400000)
#define IP_REG_LDTCL_5_END                  (0x19400000+0x40000)
#define IP_REG_LDTCL_6_START                (0x19500000)
#define IP_REG_LDTCL_6_END                  (0x19500000+0x30000)
#define IP_REG_LDTCL_7_START                (0x19600000)
#define IP_REG_LDTCL_7_END                  (0x19600000+0x20000)
#define IP_REG_LDTCL_8_START                (0x19700000)
#define IP_REG_LDTCL_8_END                  (0x19700000+0x30000)
    
/****************************************************************/
/************************end dump ip reg*************************/
/****************************************************************/



/**----------------------------------------------------------------------------*
**                        Local variable definition                           **
**----------------------------------------------------------------------------*/
const MEMORY_ACCESS_INFO_T s_memory_acc_table[ ] =
{
    /*------------------------------------------------------------------------------------------------------------
    *start_address              end_address                 acc_condtion                                memory name
    ----------------------------------------------------------------------------------------------------------------*/
    {0x00000000,          0xFFFFFFFF, (READ_ACC|WRITE_ACC|BYTE_ACC|WORD_ACC|DWORD_ACC) },             //the RAM memory
};


LOCAL const IRAM_AREA g_init_iram_area[] = {0x40000000, 0x00002800, SCI_NULL, IRAM_ATTRIBUTE_NORMAL};

/*
    On Chip RAM for DSP 
*/
LOCAL const IRAM_AREA g_iram_dsp[] = {
                                        {0x4001B000, 0x00008000, SCI_NULL, IRAM_ATTRIBUTE_DSP},
                                        {0x40023000, 0x0000C000, SCI_NULL, IRAM_ATTRIBUTE_DSP}
                                     };


// layout the const to global var, so that it can be modifed 
PUBLIC const CTL_REG_INFO_T g_CtlRegInfoTab[]=
{
    {IP_REG_SPIFLASH1_REG_START, IP_REG_SPIFLASH1_REG_END},
    {IP_REG_SPIFLASH2_REG_START, IP_REG_SPIFLASH2_REG_END},
    {IP_REG_GOUDA_START, IP_REG_GOUDA_END},
    {IP_REG_AP_AXIDMA_START, IP_REG_AP_AXIDMA_END},
    {IP_REG_EMMC_START, IP_REG_EMMC_END},
    {IP_REG_SPI1_START, IP_REG_SPI1_END},
    {IP_REG_SDMMC_START, IP_REG_SDMMC_END},
    {IP_REG_CAMERA_START, IP_REG_CAMERA_END},
    {IP_REG_AP_IFC_START, IP_REG_AP_IFC_END},
    {IP_REG_LZMA_START, IP_REG_LZMA_END},
    {IP_REG_AP_IMEM_START, IP_REG_AP_IMEM_END},
    {IP_REG_AP_BUSMON_START, IP_REG_AP_BUSMON_END},
    {IP_REG_GOUDA_REG_START, IP_REG_GOUDA_REG_END},
    {IP_REG_TIMER1_START, IP_REG_TIMER1_END},
    {IP_REG_TIMER2_START, IP_REG_TIMER2_END},
    {IP_REG_TIMER5_START, IP_REG_TIMER5_END},
    {IP_REG_I2C1_START, IP_REG_I2C1_END},
    {IP_REG_I2C2_START, IP_REG_I2C2_END},
    {IP_REG_GPT3_START, IP_REG_GPT3_END},
    {IP_REG_AP_CLK_START, IP_REG_AP_CLK_END},
    {IP_REG_SCI1_START, IP_REG_SCI1_END},
    {IP_REG_SCI2_START, IP_REG_SCI2_END},
    {IP_REG_TIMER3_START, IP_REG_TIMER3_END},
    {IP_REG_TIMER4_START, IP_REG_TIMER4_END},
    {IP_REG_RISCV_DEBUG_START, IP_REG_RISCV_DEBUG_END},
    {IP_REG_RISCV_INTC_START, IP_REG_RISCV_INTC_END},
    {IP_REG_RISCV_SLEEP_START, IP_REG_RISCV_SLEEP_END},
    {IP_REG_RF_INTERFACE_REG_START, IP_REG_RF_INTERFACE_REG_END},
    {IP_REG_RF_ANA_REG_START, IP_REG_RF_ANA_REG_END},
    {IP_REG_DFE_START, IP_REG_DFE_END},
    {IP_REG_TX_DLPF_START, IP_REG_TX_DLPF_END},
    {IP_REG_RTC1_START, IP_REG_RTC1_END},
    {IP_REG_RF_SYS_CTRL_START, IP_REG_RF_SYS_CTRL_END},
    {IP_REG_LTE_GNSS_BITMAP_START, IP_REG_LTE_GNSS_BITMAP_END},
    {IP_REG_RX_DLPF_START, IP_REG_RX_DLPF_END},
    {IP_REG_RFFE_START, IP_REG_RFFE_END},
    {IP_REG_WATCHDOG_START, IP_REG_WATCHDOG_END},
    {IP_REG_TIMER0_START, IP_REG_TIMER0_END},
    {IP_REG_TEST_TSEN_START, IP_REG_TEST_TSEN_END},
    {IP_REG_GPIO2_START, IP_REG_GPIO2_END},
    {IP_REG_I2C3_START, IP_REG_I2C3_END},
    {IP_REG_GPIO1_START, IP_REG_GPIO1_END},
    {IP_REG_AP_QOS_START, IP_REG_AP_QOS_END},
    {IP_REG_RTC_START, IP_REG_RTC_END},
    {IP_REG_TXRX_START, IP_REG_TXRX_END},
    {IP_REG_RFAD_START, IP_REG_RFAD_END},
    {IP_REG_COEFF_START, IP_REG_COEFF_END},
    {IP_REG_MEASPWR_START, IP_REG_MEASPWR_END},
    {IP_REG_IDDET_START, IP_REG_IDDET_END},
    {IP_REG_ULDFT_START, IP_REG_ULDFT_END},
    {IP_REG_PUSCH_START, IP_REG_PUSCH_END},
    {IP_REG_ULPCDCI_START, IP_REG_ULPCDCI_END},
    {IP_REG_DLFFT_START, IP_REG_DLFFT_END},
    {IP_REG_SCIRS_START, IP_REG_SCIRS_END},
    {IP_REG_HSDL_START, IP_REG_HSDL_END},
    {IP_REG_LDTCL_1_START, IP_REG_LDTCL_1_END},
    {IP_REG_LDTCL_2_START, IP_REG_LDTCL_2_END},
    {IP_REG_LDTCL_3_START, IP_REG_LDTCL_3_END},
    {IP_REG_LDTCL_4_START, IP_REG_LDTCL_4_END},
    {IP_REG_LDTCL_5_START, IP_REG_LDTCL_5_END},
    {IP_REG_LDTCL_6_START, IP_REG_LDTCL_6_END},
    {IP_REG_LDTCL_7_START, IP_REG_LDTCL_7_END},
    {IP_REG_LDTCL_8_START, IP_REG_LDTCL_8_END},
    {IP_REG_RXCAPT_START, IP_REG_RXCAPT_END},
    {IP_REG_RF_ISRAM_START, IP_REG_RF_ISRAM_END},
    {IP_REG_RF_DSRAM_START, IP_REG_RF_DSRAM_END},
    {IP_REG_IOMUX_START, IP_REG_IOMUX_END},
    {IP_REG_DMC_START, IP_REG_DMC_END},
    {IP_REG_PSRAM_PHY_START, IP_REG_PSRAM_PHY_END},
    {IP_REG_PAGESPY_START, IP_REG_PAGESPY_END},
    {IP_REG_PUB_APB_REG_START, IP_REG_PUB_APB_REG_END},
    {IP_REG_IDLE_LPS_START, IP_REG_IDLE_LPS_END},
    {IP_REG_APB_REG_START, IP_REG_APB_REG_END},
    {IP_REG_KEYPAD_START, IP_REG_KEYPAD_END},
    {IP_REG_PWRCTRL_START, IP_REG_PWRCTRL_END},
    {IP_REG_RTC_TIMER_START, IP_REG_RTC_TIMER_END},
    {IP_REG_ANA_WARAP3_START, IP_REG_ANA_WARAP3_END},
    {IP_REG_LPS_IFC_START, IP_REG_LPS_IFC_END},
    {IP_REG_DAP_START, IP_REG_DAP_END},
    {IP_REG_WLAN_11B_START, IP_REG_WLAN_11B_END},
    {IP_REG_EFUSE_START, IP_REG_EFUSE_END},
    {IP_REG_AIF_START, IP_REG_AIF_END},
    {IP_REG_DBG_HOST_START, IP_REG_DBG_HOST_END},
    {IP_REG_SYS_CTRL_START, IP_REG_SYS_CTRL_END},
    {IP_REG_ANA_WRAP1_START, IP_REG_ANA_WRAP1_END},
    {IP_REG_MON_CTRL_START, IP_REG_MON_CTRL_END},
    {IP_REG_SCC_TOP_START, IP_REG_SCC_TOP_END},
    {IP_REG_SYSMAIL_START, IP_REG_SYSMAIL_END},
    {IP_REG_IDLE_TIMER_START, IP_REG_IDLE_TIMER_END},
    {IP_REG_AUD_2AD_START, IP_REG_AUD_2AD_END},
    {IP_REG_GPT2_START, IP_REG_GPT2_END},
    {IP_REG_SPI2_START, IP_REG_SPI2_END},
    {IP_REG_GPT1_START, IP_REG_GPT1_END},
    {IP_REG_DJTAG_CFG_START, IP_REG_DJTAG_CFG_END},
    {IP_REG_ANA_WAP2_START, IP_REG_ANA_WAP2_END},
    {IP_REG_GNSS_QOS_START, IP_REG_GNSS_QOS_END},
    {IP_REG_GNSS_AHB_START, IP_REG_GNSS_AHB_END},
    {IP_REG_GNSS_CLK_RF_START, IP_REG_GNSS_CLK_RF_END},
    {IP_REG_RFT_START, IP_REG_RFT_END},
    {IP_REG_UART1A_START, IP_REG_UART1A_END},
    {IP_REG_UART1B_START, IP_REG_UART1B_END},
    {IP_REG_UART2A_START, IP_REG_UART2A_END},
    {IP_REG_UART2B_START, IP_REG_UART2B_END},
    {IP_REG_UART3A_START, IP_REG_UART3A_END},
    {IP_REG_UART3B_START, IP_REG_UART3B_END},
    {IP_REG_UART4A_START, IP_REG_UART4A_END},
    {IP_REG_UART4B_START, IP_REG_UART4B_END},
    {IP_REG_UART5A_START, IP_REG_UART5A_END},
    {IP_REG_UART5B_START, IP_REG_UART5B_END},
    {IP_REG_UART6A_START, IP_REG_UART6A_END},
    {IP_REG_UART6B_START, IP_REG_UART6B_END},
    {IP_REG_DBG_UARTA_START, IP_REG_DBG_UARTA_END},
    {IP_REG_DBG_UARTB_START, IP_REG_DBG_UARTB_END},
    {IP_REG_APB_REG1_START, IP_REG_APB_REG1_END},

    {IP_REG_SPINLOCK_START, IP_REG_SPINLOCK_END},
    {IP_REG_ADI_MST_START, IP_REG_ADI_MST_END},
    {IP_REG_AON_IFC_START, IP_REG_AON_IFC_END},
    {IP_REG_AON_CLK_PRE_START, IP_REG_AON_CLK_PRE_END},
    {IP_REG_AON_CLK_CORE_START, IP_REG_AON_CLK_CORE_END},
    {IP_REG_AON_QOS_START, IP_REG_AON_QOS_END},

    {IP_REG_F8_REG_START, IP_REG_F8_REG_END},
    {IP_REG_CP_AXIDMA_START, IP_REG_CP_AXIDMA_END},
    {IP_REG_GLB_REG_START, IP_REG_GLB_REG_END},
    {IP_REG_CP_IFC_START, IP_REG_CP_IFC_END},
    {IP_REG_CP_IMEM_START, IP_REG_CP_IMEM_END},
    {IP_REG_CP_BUSMON_START, IP_REG_CP_BUSMON_END},
    {IP_REG_CP_IRQ0_START, IP_REG_CP_IRQ0_END},
    {IP_REG_CP_IRQ1_START, IP_REG_CP_IRQ1_END},
    {IP_REG_CP_QOS_START, IP_REG_CP_QOS_END},

    {IP_REG_GNSS_SPI_START, IP_REG_GNSS_SPI_END},
    {IP_REG_PPS_START, IP_REG_PPS_END},
    {IP_REG_GNSS_GLB_REG_START, IP_REG_GNSS_GLB_REG_END},
    {IP_REG_AE_FIFO0_START, IP_REG_AE_FIFO0_END},
    {IP_REG_AXI_START, IP_REG_AXI_END},
    {IP_REG_AE_FIFO1_START, IP_REG_AE_FIFO1_END},
    {IP_REG_AE_REG_START, IP_REG_AE_REG_END},
    {IP_REG_TE_FIFO0_START, IP_REG_TE_FIFO0_END},
    {IP_REG_TE_FIFO1_START, IP_REG_TE_FIFO1_END},
    {IP_REG_TE_REG_START, IP_REG_TE_REG_END},
    {IP_REG_VITEBI_START, IP_REG_VITEBI_END},
    {IP_REG_DATA2RAM_START, IP_REG_DATA2RAM_END},
    {IP_REG_LDPC_START, IP_REG_LDPC_END},
    {PHY_FLASH_START_ADDR, PHY_FLASH_END_ADDR},

    {0xffffffff,0xffffffff}
};

//PUBLIC const IP_REG_INFO_T s_DumpRegInfoTab = {"dumpreg", g_CtlRegInfoTab};

PUBLIC const CTL_MEM_INFO_T g_CtlMemInfoTab[]=
{
//16M RAM	
	{PHY_RAM_BASE_ADDR, MEM_RAM_END_ADDR, MEM_RAM_SIZE },
//64K AP_ROM
	//{PHY_IROM_BASE_ADDR, MEM_IROM_END_ADDR, MEM_IROM_SIZE},
//16K AP_IRAM	
	{PHY_IRAM_BASE_ADDR,MEM_IRAM_END_ADDR,MEM_IRAM_SIZE},
    {MEM_CP_ROM_BASE_ADDR,MEM_CP_ROM_END_ADDR,MEM_CP_ROM_SIZE},
//16KB CP IRAM
	{PHY_CP_IRAM_BASE_ADDR,MEM_CP_IRAM_END_ADDR,MEM_CP_IRAM_SIZE},
//80KB AON IRAM
	{PHY_AON_IRAM_BASE_ADDR,MEM_AON_IRAM_END_ADDR,MEM_AON_IRAM_SIZE},

	{0xffffffff,0xffffffff,0}
};

/**----------------------------------------------------------------------------*
**                        Global variable definition                          **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC MEMORY_ACCESS_INFO_T *Mem_getAccTable (uint32 *pSize)
{
    *pSize = sizeof (s_memory_acc_table) /sizeof (s_memory_acc_table[0]);

    return (MEMORY_ACCESS_INFO_T *) s_memory_acc_table;
}

/*****************************************************************************/
//  Description:    Get IRAM information used by DSP
//  Global resource dependence:
//  Author:
//  Note:           
/*****************************************************************************/
PUBLIC IRAM_AREA* MEM_GetIRAMDsp (uint32 *count)
{
    // return registered area num by input param
    *count = sizeof(g_iram_dsp[0]) /sizeof (IRAM_AREA);

    return ( (IRAM_AREA *) g_iram_dsp);
}

/*****************************************************************************/
//  Description:    Get IRAM information used by ARM
//  Global resource dependence:
//  Author:
//  Note:           return num by input param and return the const array pointer
/*****************************************************************************/
PUBLIC IRAM_AREA *GetIRAMSet (uint32 *count)
{
    // return registered area num by input param
    *count = sizeof (g_init_iram_area) /sizeof (IRAM_AREA);

    return ( (IRAM_AREA *) g_init_iram_area);
}

/*****************************************************************************/
// Description :    This function is used to get the number of control registers group.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 CHIP_GetNumCtlRegGrp (void)
{
    uint32 CtlRegInfoTabSize = sizeof (g_CtlRegInfoTab) / sizeof (g_CtlRegInfoTab[0]);
    return CtlRegInfoTabSize - 1;
}

/*****************************************************************************/
// Description :    This function is used to get the infomation of control
//                  registers,including the base address,the length and the
//                  name of External Memory Control Registers and AHB General
//                  Control Registers,DMA Control Register,Digital Camera Space,
//                  USB Device Space,NAND Flash Control and LCM Control Space,
//                  Peripheral Address Space.
// Global resource dependence :g_CtlRegInfoTab
// Author :         kevin.wang
// Note : the unit of length is a word(32bit).
/*****************************************************************************/
PUBLIC void const *CHIP_GetInfoOfCtlReg (void)
{
    return (void *) g_CtlRegInfoTab;
}

/*
PUBLIC void const *CHIP_GetInfoOfDumpReg (void)
{
    return (void *) &s_DumpRegInfoTab;
}
*/

/*****************************************************************************/
// Description :    This function is used to get the number of control memory group.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 CHIP_GetNumCtlMemGrp (void)
{
    uint32 CtlMemInfoTabSize = sizeof (g_CtlMemInfoTab) / sizeof (g_CtlMemInfoTab[0]);
    return CtlMemInfoTabSize;
}

/*****************************************************************************/
// Description :    This function is used to get the infomation of control
//                  memory group
// Global resource dependence :g_CtlMemInfoTab
// Author :         kevin.wang
// Note : the unit of length is a word(32bit).
/*****************************************************************************/
PUBLIC void const *CHIP_GetInfoOfCtlMem (void)
{
    return (void *) g_CtlMemInfoTab;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
