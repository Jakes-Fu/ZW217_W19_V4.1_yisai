/******************************************************************************
 ** File Name:    Tiger_reg_int.h                                             *
 ** Author:       ryan.liao                                                   *
 ** DATE:         03/08/2012                                                  *
 ** Copyright:    2012 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 6/14/2008     Yuhua.Shi       Create.                                     *
 ******************************************************************************/
#ifndef _SHARK_REG_INT_H_
#define _SHARK_REG_INT_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "sprd_reg.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define INTC0_REG_BASE			CTL_BASE_INTC0

#define INT_IRQ_BASE             (INTC0_REG_BASE + 0x0000)
#define INT_IRQ_STS              (INTC0_REG_BASE + 0x0000) //Interrupt status after masked by irq_enable.
#define INT_IRQ_RAW_STS          (INTC0_REG_BASE + 0x0004) //Interrupt status from different interrupt source.
#define INT_IRQ_EN               (INTC0_REG_BASE + 0x0008) //Enable bits  for the corresponding interrupt sources. 
#define INT_IRQ_DISABLE          (INTC0_REG_BASE + 0x000C)
#define INT_IRQ_SOFT             (INTC0_REG_BASE + 0x0010)
#define INT_IRQ_TEST_SRC         (INTC0_REG_BASE + 0x0014)
#define INT_IRQ_TEST_SEL         (INTC0_REG_BASE + 0x0018)

#define INT_FIQ_STS              (INTC0_REG_BASE + 0x0020)
#define INT_FIQ_RAW_STS          (INTC0_REG_BASE + 0x0024)
#define INT_FIQ_EN               (INTC0_REG_BASE + 0x0028)
#define INT_FIQ_DISABLE          (INTC0_REG_BASE + 0x002C)
#define INT_FIQ_SOFT             (INTC0_REG_BASE + 0x0030)
#define INT_FIQ_TEST_SRC         (INTC0_REG_BASE + 0x0034)
#define INT_FIQ_TEST_SEL         (INTC0_REG_BASE + 0x0038)

#define CTL_BIT(_x)              (1<< ((_x) & 0x1f))


#define INT_NUM_EIC_LAT		0
#define INT_NUM_TIMER0 		2
#define INT_NUM_TIMER1		3
#define INT_NUM_TIMER2 		4
#define INT_NUM_SYSTMR 		5
#define INT_NUM_GPIO 		6
#define INT_NUM_WDG			7
#define INT_NUM_UART0		8
#define INT_NUM_UART1		9
#define INT_NUM_IIS			10
#define INT_NUM_EIC_SYNC	11
#define INT_NUM_EIC_ASYNC	12
#define INT_NUM_EIC_DBNC	13
#define INT_NUM_RF_MASTER	14
#define INT_NUM_DMA 		15
#define INT_NUM_BUSMON		16
#define INT_NUM_SDIO0		17
#define INT_NUM_MDM			18
#define INT_NUM_BT			19
#define INT_NUM_FM			20
#define INT_NUM_WIFI		21
#define INT_NUM_SPI_FPGA	22
#define INT_NUM_TIMER3 		23
#define INT_NUM_TIMER4 		24
#define INT_NUM_TIMER5 		25
#define INT_NUM_TIMER6 		26
#define INT_NUM_TIMER7 		27
#define INT_NUM_TIMER8 		28
#define INT_NUM_TIMER9 		29

#define INT_NUM_MAX			32

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
