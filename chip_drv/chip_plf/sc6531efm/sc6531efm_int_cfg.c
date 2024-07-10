/******************************************************************************
 ** File Name:    sc6531efm_int_cfg.c                                            *
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
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "sci_types.h"
#include "chip_plf_export.h"
#include "sc6531efm_int_cfg.h"
#include "mem_dump.h"
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
#ifdef NUM_FIQ_IRQ
#undef NUM_FIQ_IRQ
#define NUM_FIQ_IRQ  64
#endif

/**----------------------------------------------------------------------------*
**                        Global variable definition                                                                          **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   config interrupt module array
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note: the map_bit is interrupt status bit.
/*****************************************************************************/
PUBLIC uint32 MAP_TO_BIT_UNIT[ISR_UNIT_NUM][ONE_UNIT_MAX_NUMBER] = //order by logicNum
{
    {
        (1UL << 0) , (1UL << 1) , (1UL << 2) , (1UL << 3) ,
        (1UL << 4) , (1UL << 5) , (1UL << 6) , (1UL << 7) , (1UL << 8) ,
        (1UL << 9) , (1UL << 10), (1UL << 11), (1UL << 12),
        (1UL << 13), (1UL << 14), (1UL << 15),
        (1UL << 16), (1UL << 17), (1UL << 18),
        (1UL << 19), (1UL << 20), (1UL << 21),
        (1UL << 22), (1UL << 23), (1UL << 24),
        (1UL << 25), (1UL << 26), (1UL << 27),
        (1UL << 28), (1UL << 29), (1UL << 30), (1UL << 31),
    },//the unit root .== ISR_UNIT_0

};/*lint !e785 confirmed by xuepeng*/
/*****************************************************************************/
//  Description:   config interrupt module array
//  Dependency:
//  Author:         Steve.Zhan
//  Note: You should config some datas in this array, ref below.
//typedef struct
//{
//    uint32 intLogicNum;//should config
//    uint32 IntRegValueIndex;//should config
//    uint32 IntRegBitMask;
//
//    void (*Handlefunc)(uint32);
//    void (* callback)(void *);
//    void (*isrDefaultFunc)(uint32);//should config
//}PRI_INDEX_GET_STATUS_T;
//
/*****************************************************************************/
PUBLIC PRI_INDEX_GET_STATUS_T irq_isr_array[MAX_ISR_NUM] = //order by priority.
{
    {TB_DSP_INT,             ISR_UNIT_0,  INTCTL_DSP_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_TIMER0_INT,          ISR_UNIT_0,  INTCTL_TIMER0_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_TIMER3_INT,          ISR_UNIT_0,  INTCTL_TIMER0_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_USB_INT,             ISR_UNIT_1,  INTCTL_USB_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_DMA_INT,             ISR_UNIT_0,  INTCTL_DMA_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_SIM0_INT,            ISR_UNIT_0,  INTCTL_SIM0_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_SDIO_INT,            ISR_UNIT_1,  INTCTL_SDIO_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_GPIO_EICDBC_INT,     ISR_UNIT_0,  INTCTL_GPIO_EICDBC_IRQ,     NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_EIC_SYNC_ASYNC_INT,  ISR_UNIT_0,  INTCTL_EIC_SYNC_ASYNC_IRQ,  NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_ANA_EIC_INT,         ISR_UNIT_2,  ANA_EIC_IRQ,                NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_TIMER2_INT,          ISR_UNIT_0,  INTCTL_TIMER2_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_TIMER5_INT,          ISR_UNIT_0,  INTCTL_TIMER5_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_FM_INT,              ISR_UNIT_1,  INTCTL_FM_IRQ,              NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_ZIP0_INT,            ISR_UNIT_1,  INTCTL_ZIP0_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_RF_MASTER_INT,            ISR_UNIT_1,  INTCTL_RF_MASTER_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_DCAM_INT,            ISR_UNIT_1,  INTCTL_DCAM_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_LCDC_INT,            ISR_UNIT_0,  INTCTL_LCDC_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_RTC_INT,             ISR_UNIT_2,  ANA_RTC_IRQ,                NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_UART0_INT,           ISR_UNIT_0,  INTCTL_UART0_IRQ,           NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_SPECIAL_LATCH_INT,   ISR_UNIT_0,  INTCTL_SPECIAL_LATCH_IRQ,   NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_KPD_INT,             ISR_UNIT_0,  INTCTL_KPD_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    //end FREQ.used
    {TB_SFC_INT ,            ISR_UNIT_1,  INTCTL_SFC_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_VBC_INT ,            ISR_UNIT_1,  INTCTL_VBC_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_ADI_INT,             ISR_UNIT_1,  INTCTL_ADI_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_IIS_INT,             ISR_UNIT_1,  INTCTL_IIS_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_SPI0_INT,            ISR_UNIT_0,  INTCTL_SPI0_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_SPI1_INT,            ISR_UNIT_0,  INTCTL_SPI1_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
#ifndef FPGA_VERIFICATION_DUAL_LZMA                                                 
    {TB_SOFT_INT,            ISR_UNIT_0,  INTCTL_SOFT_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
#endif                                                                              
    {TB_TIMER1_INT,          ISR_UNIT_0,  INTCTL_TIMER1_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_TIMER4_INT,          ISR_UNIT_0,  INTCTL_TIMER1_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_I2C0_INT,            ISR_UNIT_0,  INTCTL_I2C_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_I2C1_INT,            ISR_UNIT_0,  INTCTL_I2C_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_UART1_INT,           ISR_UNIT_0,  INTCTL_UART1_IRQ,           NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_AUD_INT,  	         ISR_UNIT_0,  INTCTL_AUD_IRQ,	          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_ADC_INT,             ISR_UNIT_2,  ANA_ADC_IRQ,                NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_GEA_INT,             ISR_UNIT_0,  INTCTL_GEA_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SYST_INT,            ISR_UNIT_0,  INTCTL_SYST_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_BM0_INT,             ISR_UNIT_0,  INTCTL_BM0_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_CM4_TO_ARM9_INT0,             ISR_UNIT_0,  INTCTL_CM4_TO_ARM9_IRQ0,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_CM4_TO_ARM9_INT1,             ISR_UNIT_0,  INTCTL_CM4_TO_ARM9_IRQ1,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_CM4_TO_ARM9_INT2,             ISR_UNIT_0,  INTCTL_CM4_TO_ARM9_IRQ2,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_WDG_INT,             ISR_UNIT_2,  ANA_WDG_IRQ,                NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_COMMRX_INT,          ISR_UNIT_0,  INTCTL_COMMRX,              NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_COMMTX_INT,          ISR_UNIT_0,  INTCTL_COMMTX,              NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_ARM7_INT,            ISR_UNIT_1,  INTCTL_ARM7_IRQ,            NULL,   NULL, INT_PHY_IrqHandleDefaultFunc},
    {TB_ANA_INT,             ISR_UNIT_1,  INTCTL_ANA_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},

#ifdef FPGA_VERIFICATION_DUAL_LZMA                                                  
    {TB_SOFT_INT,            ISR_UNIT_0,  INTCTL_SOFT_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
#endif
    {TB_AUDPROTECT_INT,             ISR_UNIT_2,  ANA_AUDPROTECT_IRQ,                NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_CAL_INT,             ISR_UNIT_2,  ANA_CAL_IRQ,                NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_ARM7_WDG_INT,             ISR_UNIT_1,  INTCTL_ARM7_WDG_IRQ,                NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL},
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL},
};//lint !e785

PUBLIC PRI_INDEX_GET_STATUS_T fiq_isr_array[MAX_ISR_NUM] = //order by priority.
{
    {TB_DSP_INT,             ISR_UNIT_0,  INTCTL_DSP_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_TIMER1_INT,          ISR_UNIT_0,  INTCTL_TIMER1_IRQ,          NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_TIMER4_INT,          ISR_UNIT_0,  INTCTL_TIMER1_IRQ,          NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_VBC_INT ,            ISR_UNIT_1,  INTCTL_VBC_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_BM0_INT,             ISR_UNIT_0,  INTCTL_BM0_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_FM_INT,              ISR_UNIT_1,  INTCTL_FM_IRQ,              NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_ZIP0_INT,            ISR_UNIT_1,  INTCTL_ZIP0_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_RF_MASTER_INT,            ISR_UNIT_1,  INTCTL_RF_MASTER_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_ADI_INT,             ISR_UNIT_1,  INTCTL_ADI_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_USB_INT,             ISR_UNIT_1,  INTCTL_USB_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SFC_INT ,            ISR_UNIT_1,  INTCTL_SFC_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SOFT_INT,            ISR_UNIT_0,  INTCTL_SOFT_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SDIO_INT,            ISR_UNIT_1,  INTCTL_SDIO_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_TIMER0_INT,          ISR_UNIT_0,  INTCTL_TIMER0_IRQ,          NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_TIMER3_INT,          ISR_UNIT_0,  INTCTL_TIMER0_IRQ,          NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_TIMER2_INT,          ISR_UNIT_0,  INTCTL_TIMER2_IRQ,          NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_TIMER5_INT,          ISR_UNIT_0,  INTCTL_TIMER2_IRQ,          NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_GPIO_EICDBC_INT,     ISR_UNIT_0,  INTCTL_GPIO_EICDBC_IRQ,     NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_EIC_SYNC_ASYNC_INT,  ISR_UNIT_0,  INTCTL_EIC_SYNC_ASYNC_IRQ,  NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_KPD_INT,             ISR_UNIT_0,  INTCTL_KPD_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_I2C0_INT,            ISR_UNIT_0,  INTCTL_I2C0_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_I2C1_INT,            ISR_UNIT_0,  INTCTL_I2C0_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SIM0_INT,            ISR_UNIT_0,  INTCTL_SIM0_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_UART0_INT,           ISR_UNIT_0,  INTCTL_UART0_IRQ,           NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_UART1_INT,           ISR_UNIT_0,  INTCTL_UART1_IRQ,           NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_GEA_INT,             ISR_UNIT_0,  INTCTL_GEA_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SYST_INT,            ISR_UNIT_0,  INTCTL_SYST_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SPI0_INT,            ISR_UNIT_0,  INTCTL_SPI0_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SPI1_INT,            ISR_UNIT_0,  INTCTL_SPI1_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_DMA_INT,             ISR_UNIT_0,  INTCTL_DMA_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_SPECIAL_LATCH_INT,   ISR_UNIT_0,  INTCTL_SPECIAL_LATCH_IRQ,   NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_DCAM_INT,            ISR_UNIT_1,  INTCTL_DCAM_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_IIS_INT,             ISR_UNIT_1,  INTCTL_IIS_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_LCDC_INT,            ISR_UNIT_0,  INTCTL_LCDC_IRQ,            NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_COMMRX_INT,          ISR_UNIT_0,  INTCTL_COMMRX,              NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_COMMTX_INT,          ISR_UNIT_0,  INTCTL_COMMTX,              NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_ANA_INT,             ISR_UNIT_1,  INTCTL_ANA_IRQ,             NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_ARM7_WDG_INT,             ISR_UNIT_1,  INTCTL_ARM7_WDG_IRQ,                NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
    {NULL,                   NULL,        NULL,                       NULL,  NULL,  NULL                        },
};//lint !e785

PUBLIC const char *const fiq_irq_name[NUM_FIQ_IRQ]=
{
    "SPECIAL",
    "TB_SOFT_INT",
    "TB_COMMRX_INT",
    "TB_COMMTX_INT",
    "TB_TIMER0_INT",
    "TB_TIMER1_INT",
    "TB_TIMER2_INT",
    "TB_TIMER3_INT",
    "TB_TIMER4_INT",
    "TB_TIMER5_INT",
    "TB_GPIO_INT",
    "TB_EIC_INT",
    "TB_KPD_INT",
    "TB_I2C0_INT",
    "TB_I2C1_INT",
    "NO_DEFINE_INT",
    "TB_AUD_INT",
    "TB_SIM0_INT",
    "TB_UART0_INT",
    "TB_UART1_INT",
    "NO_DEFINE_INT",
    "TB_DSP_INT",
    "TB_LCDC_INT",
    "TB_GEA_INT",
    "TB_SYST_INT",
    "TB_SPI0_INT",
    "TB_SPI1_INT",
    "TB_BM0_INT",
    "TB_CM42ARM2_INT",
    "TB_CM42ARM1_INT",
    "TB_CM42ARM0_INT",
    "TB_DMA_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "TB_VBC_INT",
    "TB_ADI_INT",
    "TB_ANA_INT",
    "TB_USB_INT",
    "TB_DCAM_INT",
    "TB_IIS_INT",
    "TB_SDIO_INT",
    "TB_ARM7_INT",
    "TB_ARM7_TMR_INT",
    "NO_DEFINE_INT",
    "TB_FM_INT",
    "TB_ZIP0_INT",
    "TB_RF_MASTER_INT",
    "TB_SFC_INT",
    "TB_ARM7_WDG_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
    "NO_DEFINE_INT",
};

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to get the name of FIQ/IRQ.
// Global resource dependence :fiq_irq_name
// Author :         kevin.wang
// Note :
/*****************************************************************************/

PUBLIC const char  **CHIP_GetNameOfFiqIrq (void)
{
    return (const char **) fiq_irq_name;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
