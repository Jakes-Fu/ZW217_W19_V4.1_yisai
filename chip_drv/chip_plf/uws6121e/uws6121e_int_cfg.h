/******************************************************************************
 ** File Name:    sc6531efm_int_cfg.h                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010  Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.zhan      Create.                                     *
 ** 04/12/2012    Yong.Li         Modified.                                   *
 ******************************************************************************/
#ifndef _UWS6121E_INT_CFG_H_
#define _UWS6121E_INT_CFG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
 #include "isr_drvapi.h"
 #include "globals.h"
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
// D-die Interrupt Number
#define TB_PAGE_SPY_INT              SYS_IRQ_ID_PAGE_SPY
#define TB_IMEM_INT                  SYS_IRQ_ID_IMEM
#define TB_TIMER1_INT                SYS_IRQ_ID_TIMER1
#define TB_TIMER1_OS_INT             SYS_IRQ_ID_TIMER1_OS
#define TB_TIMER2_INT                SYS_IRQ_ID_TIMER2_OS
#define TB_TIMER2_OS_INT             SYS_IRQ_ID_TIMER2_OS
#define TB_TIMER3_INT                SYS_IRQ_ID_TIMER5
#define TB_TIMER3_OS_INT             SYS_IRQ_ID_TIMER5_OS
#define TB_TIMER4_INT                SYS_IRQ_ID_TIMER_4
#define TB_TIMER4_OS_INT             SYS_IRQ_ID_TIMER_4_OS
#define TB_GPIO_EICDBC_INT           SYS_IRQ_ID_GPIO2

#define TB_GPIO1_INT                 SYS_IRQ_ID_GPIO1
#define TB_DEBUG_UART_INT            SYS_IRQ_ID_DEBUG_UART
#define TB_EIC_SYNC_ASYNC_INT        0xB
#define TB_KPD_INT                   SYS_IRQ_ID_KEYPAD
#define TB_I2C0_INT                  SYS_IRQ_ID_I2C1
#define TB_I2C1_INT                  SYS_IRQ_ID_I2C2
#define TB_I2C2_INT                  SYS_IRQ_ID_I2C3
//#define TB_I2C3_INT                  SYS_IRQ_ID_I2C4
#define TB_AIF_APB0_INT              SYS_IRQ_ID_AIF_IFC0
#define TB_AIF_APB1_INT              SYS_IRQ_ID_AIF_IFC1
//#define TB_AIF_APB2_INT              SYS_IRQ_ID_AIF_APB_0
//#define TB_AIF_APB3_INT              SYS_IRQ_ID_AIF_APB_1
#define TB_AUD_INT                   0x10
#define TB_SIM0_INT                  SYS_IRQ_ID_SCI
#define TB_SIM1_INT                  SYS_IRQ_ID_SCI
#define TB_UART0_INT                 SYS_IRQ_ID_UART1
#define TB_UART1_INT                 SYS_IRQ_ID_UART2
#define TB_UART2_INT                 SYS_IRQ_ID_UART3
#define TB_ZSPUART_INT             SYS_IRQ_ID_UART4
#define TB_DSP_INT                   0x15
#define TB_LCDC_INT                  SYS_IRQ_ID_LCD
#define TB_GEA_INT                   0x17
#define TB_SYST_INT                  0x18
#define TB_SPI0_INT                  SYS_IRQ_ID_SPI1
#define TB_SPI1_INT                  SYS_IRQ_ID_SPI2
//#define TB_SPI2_INT                  SYS_IRQ_ID_SPI_3
#define TB_CAMERA_INT                SYS_IRQ_ID_CAMERA
#define TB_LZMA_INT                  SYS_IRQ_ID_LZMA
#define TB_GOUDA_INT                 SYS_IRQ_ID_GOUDA
#define TB_F8_INT                    SYS_IRQ_ID_F8
#define TB_USB_INT                   SYS_IRQ_ID_USBC
#define TB_PMU_APCPU_INT             SYS_IRQ_ID_PMU_APCPU
#define TB_LPS_INT                   SYS_IRQ_ID_LPS
#define TB_MAILBOX_ARM_AP_INT        SYS_IRQ_ID_MAILBOX_ARM_AP
#define TB_IDLE_LPS_INT				 SYS_IRQ_ID_IDLE_LPS
#define TB_WCN_AWAKE_INT		     SYS_IRQ_ID_WCN_AWAKE
#define TB_WCN_WLAN_INT              SYS_IRQ_ID_WCN_WLAN
#define TB_PMIC_INT                 SYS_IRQ_ID_PMIC
#define TB_BM0_INT                   0x1B
#define TB_CM4_TO_ARM9_INT2	0x1C
#define TB_CM4_TO_ARM9_INT1	0x1D
#define TB_CM4_TO_ARM9_INT0	0x1E
#define TB_DMA_INT                   SYS_IRQ_ID_AP_AXIDMA_SECURITY
#define TB_VBC_INT                   0x22
#define TB_ADI_INT                   SYS_IRQ_ID_ADI
#define TB_ANA_INT                   0x24
#define TB_DCAM_INT                  TB_CAMERA_INT
#define TB_IIS_INT                   0x27
#define TB_SDIO_INT                  SYS_IRQ_ID_SDMMC
#define TB_SDIO2_INT                 SYS_IRQ_ID_SDMMC
#define TB_ARM7_INT                  0x29
#define TB_ARM7_TIMER_INT            0x2A
#define TB_FM_INT                    0x2C
#define TB_ZIP0_INT                  0x2D
#define TB_RF_MASTER_INT                  0x2E
#define TB_SFC_INT                   0x2F
#define TB_ARM7_WDG_INT                   0x30

//A-die Interrupt Number(in uix8910 map to the end of sys IRQ)
#define TB_ADC_INT                (NB_SYS_IRQ)
#define TB_RTC_INT                (NB_SYS_IRQ + 1)
#define TB_WDG_INT                (NB_SYS_IRQ + 2)
#define TB_FGU_INT                (NB_SYS_IRQ + 3)
#define TB_ANA_EIC_INT            (NB_SYS_IRQ + 4)
#define TB_AUDPROTECT_INT         (NB_SYS_IRQ + 5)
#define TB_TMR_INT                (NB_SYS_IRQ + 6)
#define TB_CAL_INT                (NB_SYS_IRQ + 7)
#define TB_TYPEC_INT              (NB_SYS_IRQ + 8)

#define TB_UINT                      TB_SPECIAL_LATCH_INT
#define PCM_IRQ_INT                  TB_IIS_INT
#define TB_SIM_INT                   TB_SIM0_INT
#define TB_GPIO_INT                  TB_GPIO_EICDBC_INT
//#define TB_SIM1_INT                  TB_SIM0_INT
#define TB_MIDI_FM_ZIP_INT           TB_ZIP0_INT
#define TB_I2C_INT                   TB_I2C0_INT
#define TB_BM_INT                    TB_BM0_INT
#define TB_TIMER0_3_INT              TB_TIMER0_INT
#define TB_TIMER1_4_INT              TB_TIMER1_INT
#define TB_TIMER2_5_INT              TB_TIMER2_INT

#define TB_TIMER0_INT                0xFF     /*no*/
#define TB_SPECIAL_LATCH_INT         0xFF     /*no*/

#define INTCTL_TIMER1_OS_IRQ		(1<<5)
#define INTCTL_TIMER2_OS_IRQ          (1 << 5)
#define INTCTL_TIMER3_OS_IRQ          (1 << 3)
#define INTCTL_AXIDMA_IRQ             (1 << 0x1C)
//The corresponding bit of all INT_CTL registers.
#define INTCTL_SPECIAL_LATCH_IRQ        (1 << 0)
#define INTCTL_SOFT_IRQ                 (1 << 1)
#define INTCTL_COMMRX                   (1 << 2)
#define INTCTL_COMMTX                   (1 << 3)
#define INTCTL_TIMER0_3_IRQ             (1 << 4)
#define INTCTL_TIMER1_4_IRQ             (1 << 5)
#define INTCTL_GPIO_EICDBC_IRQ          (1 << 0x10)
#define INTCTL_GPIO_1_IRQ			(1<<6)
#define INTCTL_EIC_SYNC_ASYNC_IRQ       (1 << 7)
#define INTCTL_KPD_IRQ                  (1 << 0xF)
#define INTCTL_I2C_IRQ                  (1 << 8)
#define INTCTL_SIM0_IRQ                 (1 << 10)
#define INTCTL_SYSMAIL_AP_IRQ           (1 << 0x1C)
#define INTCTL_UART0_IRQ                (1 << 0x15)
#define INTCTL_UART1_IRQ                (1 << 0x13)
#define INTCTL_UART2_IRQ                (1 << 0x14)
#define INTCTL_DSP_IRQ                  (1 << 13)
#define INTCTL_LCDC_IRQ                 (1 << 0x0)
#define INTCTL_GEA_IRQ                  (1 << 15)
#define INTCTL_SYST_IRQ                 (1 << 16)
#define INTCTL_SPI0_IRQ                 (1 << 17)
#define INTCTL_SPI1_IRQ                 (1 << 18)
#define INTCTL_BM_IRQ                   (1 << 19)
#define INTCTL_GOUDA_IRQ                (1 << 17)
#define INTCTL_DMA_IRQ                  (1 << 20)
#define INTCTL_VBC_IRQ                  (1 << 21)
#define INTCTL_ADI_IRQ                  (1 << 22)
#define INTCTL_TIMER2_5_IRQ             (1 << 23)
#define INTCTL_ANA_IRQ                  (1 << 24)
#define INTCTL_USB_IRQ                  (1 << 0x19)
#define INTCTL_DCAM_IRQ                 (1 << 0x15)
#define INTCTL_IIS_IRQ                  (1 << 27)
#define INTCTL_IDLE_LPS_IRQ             (1 << 27)
#define INTCTL_SDIO_IRQ                 (1 << 0xE)
#define INTCTL_SDIO2_IRQ                (1 << 0xF)
#define INTCTL_RESERVED_IRQ             (1 << 29)
#define INTCTL_MIDI_FM_ZIP_IRQ          (1 << 30)
#define INTCTL_SFC_IRQ                  (1UL << 31)
#define INTCTL_WCN_AWAKE_IRQ            (1 << 24)//(1 << 26)
#define INTCTL_WCN_WLAN_IRQ             (1 << 21)
#define INTCTL_PAGESPY_IRQ            (1 << 0)

#define INTCTL_ANA_RTC_IRQ              (1 << 1)
#define INTCTL_ANA_WDG_IRQ              (1 << 2)
#define INTCTL_ANA_EIC_IRQ              (1 << 4)
#define INTCTL_PMIC_IRQ              (1 << 4)
#define INTCTL_SIM1_IRQ                 INTCTL_SIM0_IRQ
#define INTCTL_PCM_IRQ                  INTCTL_IIS_IRQ
//#define INTCTL_ICLR_ALL                 (0x4FF7FFFF)
#define INTCTL_SIM_IRQ                  INTCTL_SIM0_IRQ

#define ISR_UNIT_0  0 //root
#define ISR_UNIT_1  1
#define ISR_UNIT_2  2
#define ISR_UNIT_3  3

typedef void (* CALLBACK_T) (void *);

typedef struct
{
    uint32 intLogicNum;
    uint32 IntRegValueIndex;
    uint32 IntRegBitMask;

    TB_ISR Handlefunc;//uint32 (*Handlefunc) (uint32);
    CALLBACK_T callback;
    uint32 (*isrDefaultFunc) (uint32);
} PRI_INDEX_GET_STATUS_T;

//interrupt module configuration datas.
#define ONE_UNIT_MAX_NUMBER (32)
//#define ISR_UNIT_NUM        (2)
#define ISR_UNIT_NUM        (3)

#define ISR_UNIT_0_NUMBER   (13)
#define ISR_UNIT_1_NUMBER   (8)
#define ISR_UNIT_2_NUMBER   (2)
#define ISR_UNIT_3_NUMBER   (4)

#define MAX_ISR_NUM (ISR_UNIT_0_NUMBER + ISR_UNIT_1_NUMBER + ISR_UNIT_2_NUMBER + ISR_UNIT_3_NUMBER)
#ifdef FIQ_SUPPORT
#define MAX_FIQ_NUM MAX_ISR_NUM //27
#else
#define MAX_FIQ_NUM 3  
#endif

PUBLIC void INT_HAL_IRQEnable (uint32 logic_num);
PUBLIC void INT_HAL_IRQDisable (uint32 logic_num);
PUBLIC void INT_HAL_IRQDisableAll (void);
PUBLIC uint32 INT_HAL_IRQGetUint1Status (void);
PUBLIC void INT_HAL_FIQEnable (uint32 logicNum);
PUBLIC void INT_HAL_FIQDisable (uint32 logicNum);
PUBLIC void INT_HAL_FIQDisableAll (void);
PUBLIC void INT_HAL_DisableINT (void);


#define _CHIPDRV_EnableIRQINT(int_num) do { \
        INT_HAL_IRQEnable(int_num); \
    } while(0)

#define _CHIPDRV_DisableIRQINT(int_num) do { \
        INT_HAL_IRQDisable(int_num); \
    } while(0)

#define _CHIPDRV_DisableIRQINTALL() do { \
        INT_HAL_IRQDisableAll(); \
    } while(0)

#define _CHIPDRV_GetIRQStatus() INT_HAL_IRQGetUint1Status()


#define _CHIPDRV_EnableFIQINT(int_num) do { \
        INT_HAL_FIQEnable(int_num); \
    } while(0)

#define _CHIPDRV_DisableFIQINT(int_num) do { \
        INT_HAL_FIQDisable(int_num); \
    } while(0)

#define _CHIPDRV_DisableFIQINTALL() do { \
        INT_HAL_FIQDisableAll(); \
    } while(0)

#define _CHIPDRV_DisableINT() do { \
        INT_HAL_DisableINT(); \
    } while(0)

//end

extern uint32 MAP_TO_BIT_UNIT[ISR_UNIT_NUM][ONE_UNIT_MAX_NUMBER];


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                                                                               **
**----------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/,
//  for process default  interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IrqHandleDefaultFunc (uint32 bit);


/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/,
//  for process default  interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_FiqHandleDefaultFunc (uint32 bit);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
