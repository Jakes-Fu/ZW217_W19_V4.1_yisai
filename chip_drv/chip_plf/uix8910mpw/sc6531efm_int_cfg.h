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
#ifndef _SC6531EFM_INT_CFG_H_
#define _SC6531EFM_INT_CFG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
 #include "isr_drvapi.h"
 
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
#define TB_SPECIAL_LATCH_INT         0x0
#define TB_SOFT_INT                  0x1
#define TB_COMMRX_INT                0x2
#define TB_COMMTX_INT                0x3
#define TB_TIMER0_INT                0x4
#define TB_TIMER1_INT                0x5
#define TB_TIMER2_INT                0x6
#define TB_TIMER3_INT                0x7
#define TB_TIMER4_INT                0x8
#define TB_TIMER5_INT                0x9
#define TB_GPIO_EICDBC_INT           0xA
#define TB_EIC_SYNC_ASYNC_INT        0xB
#define TB_KPD_INT                   0xC
#define TB_I2C0_INT                  0xD
#define TB_I2C1_INT                  0xE
#define TB_AUD_INT                   0x10
#define TB_SIM0_INT                  0x11
#define TB_UART0_INT                 0x12
#define TB_UART1_INT                 0x13
#define TB_DSP_INT                   0x15
#define TB_LCDC_INT                  0x16
#define TB_GEA_INT                   0x17
#define TB_SYST_INT                  0x18
#define TB_SPI0_INT                  0x19
#define TB_SPI1_INT                  0x1A
#define TB_BM0_INT                   0x1B
#define TB_CM4_TO_ARM9_INT2	0x1C
#define TB_CM4_TO_ARM9_INT1	0x1D
#define TB_CM4_TO_ARM9_INT0	0x1E
#define TB_DMA_INT                   0x1F
#define TB_VBC_INT                   0x22
#define TB_ADI_INT                   0x23
#define TB_ANA_INT                   0x24
#define TB_USB_INT                   0x25
#define TB_DCAM_INT                  0x26
#define TB_IIS_INT                   0x27
#define TB_SDIO_INT                  0x28
#define TB_ARM7_INT                  0x29
#define TB_ARM7_TIMER_INT            0x2A
#define TB_FM_INT                    0x2C
#define TB_ZIP0_INT                  0x2D
#define TB_RF_MASTER_INT                  0x2E
#define TB_SFC_INT                   0x2F
#define TB_ARM7_WDG_INT                   0x30

//A-die Interrupt Number
#define TB_ADC_INT                0x40
#define TB_RTC_INT                0x42
#define TB_WDG_INT                0x43
#define TB_ANA_EIC_INT                0x45
#define TB_AUDPROTECT_INT             0x47
#define TB_CAL_INT       0x48

#define TB_UINT                      TB_SPECIAL_LATCH_INT
#define PCM_IRQ_INT                  TB_IIS_INT 
#define TB_SIM_INT                   TB_SIM0_INT 
#define TB_GPIO_INT                  TB_GPIO_EICDBC_INT
#define TB_SIM1_INT                  TB_SIM0_INT
#define TB_MIDI_FM_ZIP_INT           TB_ZIP0_INT
#define TB_I2C_INT                   TB_I2C0_INT
#define TB_BM_INT                    TB_BM0_INT
#define TB_TIMER0_3_INT              TB_TIMER0_INT
#define TB_TIMER1_4_INT              TB_TIMER1_INT
#define TB_TIMER2_5_INT              TB_TIMER2_INT

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

#define ISR_UNIT_0_NUMBER   (32)
#define ISR_UNIT_1_NUMBER   (17)
#define ISR_UNIT_2_NUMBER   (9)
#define ISR_UNIT_3_NUMBER   (0)

#define MAX_ISR_NUM (ISR_UNIT_0_NUMBER + ISR_UNIT_1_NUMBER + ISR_UNIT_2_NUMBER + ISR_UNIT_3_NUMBER)

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
