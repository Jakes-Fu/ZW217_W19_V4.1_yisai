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
#include "uix8910_int_cfg.h"
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
#define NUM_FIQ_IRQ  10
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
    {TB_TIMER3_OS_INT,          ISR_UNIT_1,  INTCTL_TIMER3_OS_IRQ,       NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_DMA_INT,                ISR_UNIT_0,  INTCTL_AXIDMA_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_UART1_INT,              ISR_UNIT_0,  INTCTL_UART1_IRQ,           NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_UART0_INT,              ISR_UNIT_0,  INTCTL_UART0_IRQ,           NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_MAILBOX_ARM_AP_INT,     ISR_UNIT_1,  INTCTL_SYSMAIL_AP_IRQ,      NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_KPD_INT,                ISR_UNIT_1,  INTCTL_KPD_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_GOUDA_INT,              ISR_UNIT_0,  INTCTL_GOUDA_IRQ,           NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_CAMERA_INT,             ISR_UNIT_0,  INTCTL_DCAM_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_LCDC_INT,               ISR_UNIT_0,  INTCTL_LCDC_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_USB_INT,                ISR_UNIT_0,  INTCTL_USB_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_GPIO_EICDBC_INT,        ISR_UNIT_1,  INTCTL_GPIO_EICDBC_IRQ,     NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_I2C0_INT,               ISR_UNIT_0,  INTCTL_I2C_IRQ,             NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_SDIO_INT,               ISR_UNIT_0,  INTCTL_SDIO_IRQ,            NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_SDIO2_INT,              ISR_UNIT_0,  INTCTL_SDIO2_IRQ,           NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_RTC_INT,                ISR_UNIT_3,  INTCTL_ANA_RTC_IRQ,         NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
    {TB_ANA_EIC_INT,            ISR_UNIT_3,  INTCTL_ANA_EIC_IRQ,         NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
};

PUBLIC PRI_INDEX_GET_STATUS_T fiq_isr_array[MAX_ISR_NUM] = //order by priority.
{
    {TB_TIMER3_OS_INT,       ISR_UNIT_1,  INTCTL_TIMER3_OS_IRQ,       NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_DMA_INT,             ISR_UNIT_0,  INTCTL_AXIDMA_IRQ,          NULL,  NULL,  INT_PHY_FiqHandleDefaultFunc},
    {TB_UART0_INT,           ISR_UNIT_0,  INTCTL_UART0_IRQ,          NULL,  NULL,  INT_PHY_IrqHandleDefaultFunc},
};

PUBLIC const char *const fiq_irq_name[NUM_FIQ_IRQ]=
{
    "TB_TIMER3OS_INT",
    "TB_AXIDMA_INT",
    "TB_UART1_INT"
    "TB_UART0_INT"
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
