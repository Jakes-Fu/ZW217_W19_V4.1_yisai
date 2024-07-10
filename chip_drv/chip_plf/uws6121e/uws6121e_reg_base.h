/******************************************************************************
 ** File Name:      uws6121e_reg_base.h                                         *
 ** Author:         Yong.Li                                                   *
 ** DATE:           07/28/2011                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                 Register address map for the sc6600d chip                 *
 **                 Reference to the SC6531EFM control register document         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07-28-2011     Yong.Li          Create                                    *
 ******************************************************************************/
#ifndef _UWS6121E_REG_BASE_H_
#define _UWS6121E_REG_BASE_H_

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/*----------Peripheral Address Space------------*/
#define INTC_BASE                       0x80000000
#define INTC0_BASE                      INTC_BASE
#define INTC1_BASE                      0x80001000
#define TIMER_CTL_BASE                  0x81000000  //Timer0 (RTC)
#define BACKUP_TIMER_CTL_BASE           0x81001000  //Timer3 (RTC)
#define SYSTIMER_BASE                   0x81003000  //System cnt timer0

#define ADI_BASE                0x50308000  //ADI master
#define ANA_PWM_BASE			0x82001000
#define ANA_INTC_BASE			0x400
#define WDG_BASE				0x480
#define ANA_CAL_BASE			0x040
#define ANA_AUD_IF_BASE			0x140
#define ANA_PIN_CTL_BASE		0x7C0
#define ANA_PTEST_BASE			0x500
#define RTC_BASE				0x600
#define ANA_EIC_BASE			0x500
#define ANA_AUD_DIG_BASE		0x400
#define ADC_BASE				0x100
#define ANA_AUD_BASE			0x800
#define ANA_GLB_REG_BASE		0xC00

#define EFUSE_REG_BASE          (0x200)
#define ARM_VBC_BASE                    0x82003000
#define ARM_UART0_BASE        (0x51700000)
#define ARM_UART1_BASE        (0x51400000)
#define ARM_UART2_BASE        (0x51401000)
#define ZSP_UART_BASE           (0x04400000)
#define ARM_UART3_BASE        ZSP_UART_BASE
#define ARM_UART4_BASE        (0x04401000)
#define ARM_UART5_BASE        (0x04402000)
//#define ARM_UART2_BASE                  0x85000000
#define I2C_BASE                        0x04807000
#define I2C0_BASE                       I2C_BASE
#define I2C1_BASE                       0x04808000
#define I2C2_BASE                       0x51504000
#define KPD_BASE                        0x87000000
#define PWM_BASE                        0x88000000
#define FM_REG_BASE                     0x88002000
#define SIM0_BASE                       0x89000000
#define SIM1_BASE                       SIM0_BASE
#define GPIO_BASE                       0x51703000//REG_GPIO1_BASE
#define REG_GPIO1_BASE                  0x51703000
#define REG_GPIO2_BASE                  0x51503000
#define EIC_BASE                        0x8A001000
#define AUDIO_BASE                      0x8A002000
#define AUDIFD_BASE                     0x8A003000
#define GLB_REG_BASE                    0x8B000000  //Global Registers
#define PIN_CTL_BASE                    0x8C000000
#define GEA_BASE                        0x8D100000
#define IIS_BASE                        0x8F000000
#define PCM_CTL_BASE                    IIS_BASE

#define INT_REG_BASE                    INTC_BASE
#define INT0_REG_BASE                   INTC0_BASE
#define INT1_REG_BASE                   INTC1_BASE
#define AON_REG_BASE                    0x8D200000
#define SMC_REG_BASE                    0x20000000
#define EXT_MEM_CTL_BASE                SMC_REG_BASE
#define DMA_REG_BASE                    0x090c0000
#define DMA_CHA_CTL_BASE                0x20101000
#define DMA_REQ_CID_BASE                0x20102000
#define DCAM_REG_BASE                   0x20200000

#define BUS_MONx_CTL_BASE               0x20400000
#define BUS_MON0_CTL_BASE               0x20400000
#define BUS_MON_CTL_BASE                BUS_MON0_CTL_BASE

#define AHB_REG_BASE                    0x20500000

#define SDIO_BASE_ADDR                  0x20700000
#define SDIO0_BASE_ADDR                 SDIO_BASE_ADDR
#define SDIO1_BASE_ADDR                 0x20700100

#define LCM_REG_BASE                    0x20800000

#define SFC_REG_BASE                    0x20A00000

#define MIDI_REG_BASE                   0x20B00000

#define LCDC_REG_BASE                   0x20D00000

#define LZMA_REG_BASE                   0x20E00000
#define LZMA0_REG_BASE                  LZMA_REG_BASE
#define LZMA1_REG_BASE                  0x20F00000

#define RF_ISRAM_BASE                    0x50000000
#define LCM_SPACE_BASE                  0x60000000

#define SPI0_BASE                       0x04008000
#define SPI0_REG_BASE                   SPI0_BASE
#define SPI1_BASE                       0x5150C000
#define SPI1_REG_BASE                   SPI1_BASE
#define SPI2_BASE                       0x08814000
#define SPI2_REG_BASE                   SPI2_BASE

#define TIMER4_BASE_ADDR                0x0880E000
#define MCHK_BASE                       0x6C000000

#define USB_REG_BASE                    0x09040000

#define ROT_REG_BASE                    0x98000400
#define REG_ROTATION_REG_BASE           0x98000200

#define CHIP_TYPE                       0x205003FC

//Memory Address Map.
#define INTER_RAM_BEGIN                 0x00800000    //Internal ram1
#define INTER_RAM_END                     0x0083FFFF    //The address of the last byte
#define INTER_RAM_SIZE                  (0x40000)      // 2K IRAM1 for critical code&data

#define BCPU_SRAM_BEGIN                 0x40080000    //bbsram
#define BCPU_SRAM_END                     0x4009FFFF    //The address of the last byte
#define BCPU_SRAM_SIZE                  (0x20000)      // 

#define INTER_BT_MEM_BEGIN              0x40002800    //Internal ram switch with BT memory
#define INTER_BT_MEM_END                0x40007FFF    //The address of the last byte
#define INTER_BT_MEM_SIZE               (0x5800)      // 56k switch ram with BT memory

#define INTER_MEM_SXRAM2_BEGIN          0x40019000    //Internal Shared Memory.
#define INTER_MEM_SXRAM2_END            0x4001AFFF    //The address of the last byte
#define INTER_MEM_SXRAM2_SIZE           (0x2000)      //

#define INTER_MEM_XRAM_BEGIN            0x4001B000    //Internal Shared Memory.
#define INTER_MEM_XRAM_END              0x40022FFF    //The address of the last byte
#define INTER_MEM_XRAM_SIZE             (0x8000)      //

#define INTER_MEM_PRAM_BEGIN            0x40023000    //Internal Shared Memory.
#define INTER_MEM_PRAM_END              0x4002EFFF    //The address of the last byte
#define INTER_MEM_PRAM_SIZE             (0xC000)      //

#define SHARE_MEM_BEGIN                 0x30000000    // 2K*16bits
#define SHARE_MEM_END                   0x30000FFF    //
#define SHARE_MEM_SIZE                  (0x1000)      //

//Shared with DSP
#define SHARE_MEM0_BEGIN                SHARE_MEM_BEGIN
#define SHARE_MEM0_END                  SHARE_MEM_END
#define SHARE_MEM0_SIZE                 SHARE_MEM_SIZE

//Shared with BT
#define SHARE_MEM1_BEGIN                0x30100000    // 2K*16bits
#define SHARE_MEM1_END                  0x301003FF    //
#define SHARE_MEM1_SIZE                 (0x400)      //

#define DSP_MEM_BEGIN                   0xFFFFFFFF    //DSP memory base address.
#define DSP_MEM_LEN                     0x00020040    //length unit:byte.

//CONTROL REGISTER
#define EXTERNAL_MEM_CTL_BEGIN          (SMC_REG_BASE + 0x00)    //External Memory Control registers.
#define EXTERNAL_MEM_CTL_END            (SMC_REG_BASE + 0x5C)

#define DMA_GEN_CTL_BEGIN               (DMA_REG_BASE + 0x00)    //DMA General Control registers.
#define DMA_GEN_CTL_END                 (DMA_REG_BASE + 0x20)

#define DMA_CHA_CTL_BEGIN               (DMA_CHA_CTL_BASE + 0x000)    //DMA Channel Control registers.
#define DMA_CHA_CTL_END                 (DMA_CHA_CTL_BASE + 0x7FC)

#define DMA_REQ_CID_BEGIN               (DMA_REQ_CID_BASE + 0x00)    //DMA REQ CID registers.
#define DMA_REQ_CID_END                 (DMA_REQ_CID_BASE + 0x7C)

#define DCAM_CTL_BEGIN                  (DCAM_REG_BASE + 0x0000)    //DCAM Control registers. 
#define DCAM_CTL_END                    (DCAM_REG_BASE + 0x3FFC)    //the begin address of the last word

#define USB_CTL_BEGIN                   (USB_REG_BASE + 0x000)    //USB Device Space.                           
#define USB_CTL_END                     (USB_REG_BASE + 0x19C)    //the begin address of the last word

#define ROTATION_CTL_BEGIN                   (ROT_REG_BASE + 0x00)    //Rotation Device Space.                           
#define ROTATION_CTL_END                     (ROT_REG_BASE + 0x3C)    //the begin address of the last word

#define BUS_MON0_CTL_BEGIN              (BUS_MON0_CTL_BASE + 0x00)    //Bus Monitor 0 Control registers.              
#define BUS_MON0_CTL_END                (BUS_MON0_CTL_BASE + 0x28)    //The address of the last byte

#define AHB_GEN_CTL_BEGIN               (AHB_REG_BASE + 0x000)    //Bus Monitor Control registers.
#define AHB_GEN_CTL_END                 (AHB_REG_BASE + 0x120)

#define CHIP_ID_BEGIN                   (AHB_REG_BASE + 0x3FC)    //CHIP ID registers.
#define CHIP_ID_END                     (AHB_REG_BASE + 0x3FC)

#define SDIO_SLOT0_CTL_BEGIN            (SDIO0_BASE_ADDR + 0x00)    //SDIO slot0 Control Registers
#define SDIO_SLOT0_CTL_END              (SDIO0_BASE_ADDR + 0xFC)

#define SDIO_SLOT1_CTL_BEGIN            (SDIO1_BASE_ADDR + 0x00)    //SDIO slot1 Control Registers
#define SDIO_SLOT1_CTL_END              (SDIO1_BASE_ADDR + 0xFC)

#define LCM_CTL_BEGIN                   (LCM_REG_BASE + 0x00)    //LCM Control Registers
#define LCM_CTL_END                     (LCM_REG_BASE + 0x54)

#define SFC_CTL_BEGIN                   (SFC_REG_BASE + 0x000)    //SFC Registers
#define SFC_CTL_END                     (SFC_REG_BASE + 0x20C)

#define MIDI_CTL_BEGIN                  (MIDI_REG_BASE + 0x00)    //MIDI Registers
#define MIDI_CTL_END                    (MIDI_REG_BASE + 0x50)

#define LZMA_CTL_BEGIN                  (LZMA_REG_BASE + 0x00)    //LZMA Registers
#define LZMA_CTL_END                    (LZMA_REG_BASE + 0x44)

#define LCDC_CTL_BEGIN                  (LCDC_REG_BASE + 0x000)    //LCDC Control Registers                      
#define LCDC_CTL_END                    (LCDC_REG_BASE + 0x11C)    //length unit:word.

#define LCM_SPACE_BEGIN                 (LCM_SPACE_BASE + 0x0000000)    //LCM Space              
#define LCM_SPACE_END                   (LCM_SPACE_BASE + 0x1FFFFFF)    //length unit:word.

#define SPI0_CTL_BEGIN                  (SPI0_BASE + 0x00)    //SPI0 Control Registers              
#define SPI0_CTL_END                    (SPI0_BASE + 0x74)    //length unit:word.

/*
 SC6531EFM MCU APB memory address mapping
*/
#define INT_CTL_BEGIN                   (INTC_BASE + 0x00)    //Interrupt Control Registers                 
#define INT_CTL_END                     (INTC_BASE + 0x38)    //the begin address of the last word

#define INT1_CTL_BEGIN                   (INTC_BASE + 0x1000)    //Interrupt Control Registers                 
#define INT1_CTL_END                     (INT1_CTL_BEGIN + 0x38)    //the begin address of the last word

#define TIMER_CNT_BEGIN                 (TIMER_CTL_BASE + 0x00)   //tiemr counter Registers
#define TIMER_CNT_END                   (TIMER_CTL_BASE + 0x4C)    //the begin address of the last word

#define BACKUP_TIMER_CNT_BEGIN          (BACKUP_TIMER_CTL_BASE + 0x00)    //Backup tiemr counter Registers
#define BACKUP_TIMER_CNT_END            (BACKUP_TIMER_CTL_BASE + 0x4C)    //the begin address of the last word

#define SYS_CNT_BEGIN                   (SYSTIMER_BASE + 0x00)    //SYS tiemr counter Registers
#define SYS_CNT_END                     (SYSTIMER_BASE + 0x0C)    //the begin address of the last word

#define ADI_CTL_BEGIN                   (ADI_BASE + 0x00)    //adi master control registers
#define ADI_CTL_END                     (ADI_BASE + 0x20)

#define ANA_GLB_BEGIN                   (ANA_GLB_REG_BASE + 0x000)    //analog global control registers
#define ANA_GLB_END                     (ANA_GLB_REG_BASE + 0x1BC)

#define EFUSE_CTL_BEGIN                 (EFUSE_REG_BASE + 0x00)    //EFUSE control registers
#define EFUSE_CTL_END                   (EFUSE_REG_BASE + 0x20)

#define VOICE_BAND_CODEC_BEGIN          (ARM_VBC_BASE + 0x000)    //VOICE BAND control registers
#define VOICE_BAND_CODEC_END            (ARM_VBC_BASE + 0x1C0)

#define UART0_CTL_BEGIN                 (ARM_UART0_BASE + 0x00)    //UART0 Control Registers
#define UART0_CTL_END                   (ARM_UART0_BASE + 0x30)    //the begin address of the last word

#define UART1_CTL_BEGIN                 (ARM_UART1_BASE + 0x00)    //UART1 Control Registers, share with DSP
#define UART1_CTL_END                   (ARM_UART1_BASE + 0x30)    //the begin address of the last word

#define SIM0_CTL_BEGIN                  (SIM0_BASE + 0x00)    //SIMCARD Control Registers
#define SIM0_CTL_END                    (SIM0_BASE + 0x6C)    //the begin address of the last word

//length need verification
#define I2C_CTL_BEGIN                   (I2C_BASE + 0x00)    //I2C Control Registers
#define I2C_CTL_END                     (I2C_BASE + 0x14)    //the begin address of the last word

//length need verification
#define I2C1_CTL_BEGIN                  (I2C1_BASE + 0x00)    //I2C Control Registers
#define I2C1_CTL_END                    (I2C1_BASE + 0x14)    //the begin address of the last word

#define KEYPAD_CTL_BEGIN                (KPD_BASE + 0x00)    //Keypad Control Registers        
#define KEYPAD_CTL_END                  (KPD_BASE + 0x38)    //the begin address of the last word

#define PWM_CTL_BEGIN                   (PWM_BASE + 0x00)    //Keypad Control Registers
#define PWM_CTL_END                     (PWM_BASE + 0x10)    //the begin address of the last word

#define FM_CTL_BEGIN                    FM_REG_BASE    //FM Control Registers
#define FM_CTL_END                     (FM_REG_BASE + 0x0fff)    //the begin address of the last word

#define RTC_CTL_BEGIN                  (RTC_BASE + 0x00)    //RTC Control Registers
#define RTC_CTL_END                    (RTC_BASE + 0x7F)   //the begin address of the last word

#define WATDOG_CTL_BEGIN               (WDG_BASE + 0x00)   //watchdog Control Registers
#define WATDOG_CTL_END                 (WDG_BASE + 0x3F)    //the begin address of the last word

#define AUXADC_CTL_BEGIN               (ADC_BASE + 0x00)
#define AUXADC_CTL_END                 (ADC_BASE + 0x7F)

#define GPIO_CTL_BEGIN                 (GPIO_BASE + 0x000)    //GPIO Control Registers    ///digital die
#define GPIO_CTL_END                   (GPIO_BASE + 0x3A8)

#define EIC_CTL_BEGIN                  (EIC_BASE + 0x00)    //GPIO Control Registers   ///digital die
#define EIC_CTL_END                    (EIC_BASE + 0x5C)

#define AUD_CTL_BEGIN                  (AUDIO_BASE + 0x00)    //AUD Control Registers   ///digital die
#define AUD_CTL_END                    (AUDIO_BASE + 0x28)

#define GLOBAL_CTL_BEGIN               (GLB_REG_BASE + 0x000)    //GLOBAL Control Registers
#define GLOBAL_CTL_END                 (GLB_REG_BASE + 0x388)    //the begin address of the last word   0x240

#define CHIPPIN_CTL_BEGIN              (PIN_CTL_BASE + 0x000)    //ChipPin Control Registers
#define CHIPPIN_CTL_END                (PIN_CTL_BASE + 0x2E4)

#define GEA_CTL_BEGIN                  (GEA_BASE + 0x1000)    //GEA Control Registers, share with dsp
#define GEA_CTL_END                    (GEA_BASE + 0x1034)

#define IIS_CTL_BEGIN                  (IIS_BASE + 0x00)    //IIS Control Registers, share with dsp
#define IIS_CTL_END                    (IIS_BASE + 0x3C)

#define SPI1_CTL_BEGIN                 (SPI1_BASE + 0x00)    //SPI1 Control Registers
#define SPI1_CTL_END                   (SPI1_BASE + 0x74)

#define AON_CTL_BEGIN               (AON_REG_BASE + 0x000)    //CLK Control Registers
#define AON_CTL_END                 (AON_REG_BASE + 0x100)

#define TIMER4_HWTIMER_CURVAL_L       (TIMER4_BASE_ADDR + 0x1C)
#define TIMER4_HWTIMER_CURVAL_H       (TIMER4_BASE_ADDR + 0x20)

/*
  A-die reg base for module in ARM side
*/
#define ANA_REG_BASE                    0x000
#define ADIE_ARM_ADDR_START      (ANA_REG_BASE + 0x0000)
#define ADIE_ARM_ADDR_END        (ANA_REG_BASE + 0x1FFF)

#define ADIE_PWM_REG_START       	(ANA_PWM_BASE + 0x00)	
#define ADIE_PWM_REG_END       	(ANA_PWM_BASE + 0x1F)

#define ADIE_INT_CTRL_START      (ANA_INTC_BASE + 0x00)
#define ADIE_INT_CTRL_END        (ANA_INTC_BASE + 0x1F)

#define ADIE_WDG_REG_START      	(WDG_BASE + 0x00)
#define ADIE_WDG_REG_END         	(WDG_BASE + 0x3F)

#define ADIE_CAL_REG_START       	(ANA_CAL_BASE + 0x00)
#define ADIE_CAL_REG_END           	(ANA_CAL_BASE + 0x3F)

#define ADIE_AUD_IF_REG_START     (ANA_AUD_IF_BASE + 0x00)
#define ADIE_AUD_IF_REG_END         (ANA_AUD_IF_BASE + 0x3F)

#define ADIE_PIN_REG_START       	(ANA_PIN_CTL_BASE + 0x00)
#define ADIE_PIN_REG_END         	(ANA_PIN_CTL_BASE + 0x3F)

#define ADIE_PTEST_REG_START       	(ANA_PTEST_BASE + 0x00)
#define ADIE_PTEST_REG_END           	(ANA_PTEST_BASE + 0x3F)

#define ADIE_RTC_REG_START       	(RTC_BASE + 0x00)
#define ADIE_RTC_REG_END         	(RTC_BASE + 0x7F)

#define ADIE_EIC_REG_START       	(ANA_EIC_BASE + 0x00)
#define ADIE_EIC_REG_END        	 	(ANA_EIC_BASE + 0x7F)

#define ADIE_AUD_DIG_REG_START     (ANA_AUD_DIG_BASE + 0x00)
#define ADIE_AUD_DIG_REG_END         (ANA_AUD_DIG_BASE + 0x7F)

#define ADIE_ADC_REG_START      (ADC_BASE + 0x00)
#define ADIE_ADC_REG_END          (ADC_BASE + 0x7F)

#define ADIE_AUDIO_REG_START     	(ANA_AUD_BASE + 0x00)
#define ADIE_AUDIO_REG_END       	(ANA_AUD_BASE + 0x7F)

#define ADIE_GLB_REG_START       	(ANA_GLB_REG_BASE + 0x000)
#define ADIE_GLB_REG_END           	(ANA_GLB_REG_BASE + 0x1BC)

#define ADIE_RESERVED_START      (ADI_BASE + 0x16C0)
#define ADIE_RESERVED_END        (ADI_BASE + 0x1FFF)


/*
reg base for modem side 
*/

#define MODEM_GIC_START     (0x08201000)
#define MODEM_GIC_END         (0x08201000+0x00001100)

#define MODEM_IRQ_START     (0x08800000)
#define MODEM_IRQ_END         (0x08800000+0x0000004C)

#define MODEM_IRQ1_START     (0x08800800)
#define MODEM_IRQ1_END         (0x08800800+0x0000004C)

#define MODEM_TIMER1_START     (0x08808000)
#define MODEM_TIMER1_END         (0x08808000+0x00000100)

#define MODEM_TIMER2_START     (0x08809000)
#define MODEM_TIMER2_END         (0x08809000+0x00000100)

#define MODEM_TIMER4_START     (0x0880E000)
#define MODEM_TIMER4_END         (0x0880E000+0x00000100)

#define MODEM_SCI1_START     (0x08814000)
#define MODEM_SCI1_END         (0x08814000+0x00000100)

#define MODEM_SCI2_START     (0x08815000)
#define MODEM_SCI2_END         (0x08815000+0x00000100)

#define MODEM_F8_START     (0x09000000)
#define MODEM_F8_END         (0x09000000+0x00000200)

#define MODEM_AXIDMA_START     (0x020C0000)
#define MODEM_AXIDMA_END         (0x020C0000+0x00000400)

#define MODEM_SPIFLASH_START     (0x00880000)
#define MODEM_SPIFLASH_END         (0x00880000 +0x100)

#define MODEM_SPIFLASH1_START     (0x00881000)
#define MODEM_SPIFLASH1_END         (0x00881000 +0x100)

#define MODEM_EFS_START                 (0x705C0000)
#define MODEM_EFS_END                     (0x70800000)

#define MODEM_GPIO_START                 (0x51703000)
#define MODEM_GPIO_END                     (0x51703100)

#define MODEM_GPIO2_START                  (0x51503000)
#define MODEM_GPIO2_END                  (0x51503100)

#define BT_RISCV_RAM_START     (0x10040000)
#define BT_RISCV_RAM_END         (0x10040000+0x0000A000)

#define BT_RISCV_EM_START     (0x14110000)
#define BT_RISCV_EM_END         (0x14110000+0x00006000)


#define MODEM_ZITCM_START     (0x21400000)
#define MODEM_ZITCM_END         (0x21400000+0x00004000)

#define MODEM_ZDTCM_START     (0x21500000)
#define MODEM_ZDTCM_END         (0x21500000+0x00004000)

#define MODEM_ZAXIDMA_START     (0x22000000)
#define MODEM_ZAXIDMA_END         (0x22000000+0x00000400)

#define MODEM_RXTX_START     (0x25000000)
#define MODEM_RXTX_END         (0x25000000+0x00020000)

#define MODEM_RFADRAM_START     (0x25100000)
#define MODEM_RFADRAM_END         (0x25100000+0x00003000)

#define MODEM_COEFF_START     (0x25200000)
#define MODEM_COEFF_END         (0x25200000+0x00000200)

#define MODEM_LDTC_START     (0x25300000)
#define MODEM_LDTC_END         (0x25300000+0x00024000)

#define MODEM_OTDOA_START     (0x25400000)
#define MODEM_OTDOA_END         (0x25400000+0x00060000)

#define MODEM_MEASPWR_START     (0x25500000)
#define MODEM_MEASPWR_END         (0x25500000+0x000A0000)

#define MODEM_IDDET_START     (0x25600000)
#define MODEM_IDDET_END         (0x25600000+0x0000C000)

#define MODEM_ULFFT_START     (0x25700000)
#define MODEM_ULFFT_END         (0x25700000+0x00003400)

#define MODEM_PUSCH_START     (0x25800000)
#define MODEM_PUSCH_END         (0x25800000+0x00038000)

#define MODEM_ULPCDCI_START     (0x25900000)
#define MODEM_ULPCDCI_END         (0x25900000+0x00000200)

#define MODEM_DLFFT_START     (0x25A00000)
#define MODEM_DLFFT_END         (0x25A00000+0x00016000)

#define MODEM_CSIRS_START     (0x25B00000)
#define MODEM_CSIRS_END         (0x25B00000+0x0000D000)

#define MODEM_LDTC1_START     (0x26000000)
#define MODEM_LDTC1_END         (0x26000000+0x007203A0)

#define MODEM_RXCAP_START     (0x27000000)
#define MODEM_RXCAP_END         (0x27000000+0x00017000)


#define MODEM_BBAPB_START     (0x40000000)
#define MODEM_BBAPB_END         (0x40000000+0x00020000)

#define MODEM_BCPUROM_START     (0x400A0000)
#define MODEM_BCPUROM_END         (0x400C8000)

#define MODEM_RISCV_START     (0x50000000)
#define MODEM_RISCV_END         (0x50000000+0x0000C000)

#define MODEM_RFREG_START     (0x50030000)
#define MODEM_RFREG_END         (0x50030000+0x00005100)

#define MODEM_SYSREG_START     (0x50080000)
#define MODEM_SYSREG_END         (0x50080064)

#define MODEM_CLKRST_START     (0x50081000)
#define MODEM_CLKRST_END         (0x500810A8)

#define MODEM_MONITOR_START     (0x50083000)
#define MODEM_MONITOR_END         (0x50083044)

#define MODEM_WD_START     (0x50084000)
#define MODEM_WD_END         (0x50084020)


#define MODEM_IDLE_START     (0x50090000)
#define MODEM_IDLE_END         (0x500902A4)

#define MODEM_PWRCTRL_START     (0x500A0000)
#define MODEM_PWRCTRL_END         (0x500A00D8)

#define MODEM_SYSCTRL_START     (0x50100000)
#define MODEM_SYSCTRL_END         (0x5010024C)


#define MODEM_MAILBOX_START     (0x50104000)
#define MODEM_MAILBOX_END         (0x5010467C)

#define MODEM_TIMER3_START     (0x50105000)
#define MODEM_TIMER3_END         (0x50105000+0x00000100)

#define MODEM_ANALOG_REG_START     (0x50109000)
#define MODEM_ANALOG_REG_END         (0x5010923C)



#endif  //_SC6531EFM_REG_BASE_H_
