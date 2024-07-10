/******************************************************************************
 ** File Name:      int_phy.h                                                 *
 ** Author:         Steve.Zhan                                                *
 ** DATE:           07/20/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file declearate the prototype of the physical level  *
 **                 function of interrupt module.                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/20/2010     Steve.Zhan       Create.                                   *
 **                                                                           *
 ******************************************************************************/
#ifndef _INT_PHY_H_
#define _INT_PHY_H_

#include "tx_port_thumb.h"

#define INT_DISABLE_ARM_IRQ_FIQ() do { \
        SCI_DisableIRQ(); \
        SCI_DisableFIQ(); \
    }while (0)

#define INT_ENABLE_ARM_IRQ_FIQ() do { \
        SCI_RestoreFIQ(); \
        SCI_RestoreIRQ(); \
    }while (0)

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is called when an IRQ happens. Is used to get
//                  interrupt registers status value.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_GetIrqIntStatus (void);


/*****************************************************************************/
//  Description:    This function is called when an FIQ happens. Is used to get
//                  interrupt registers status value.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_GetFiqIntStatus (void);

/*****************************************************************************/
//  Description:    This function is called when system startup, only init. once.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_Init (void);
PUBLIC void INT_PHY_PMIC_INIT (void);

PUBLIC void INT_PHY_PMIC_DISABLE_ALL (void);
PUBLIC void INT_PHY_PMIC_ABLE_ALL (void);
PUBLIC void INT_PHY_PMIC_ADC_EN (void);
PUBLIC void INT_PHY_PMIC_RTC_EN (void);
PUBLIC void INT_PHY_PMIC_WDG_EN (void);
PUBLIC void INT_PHY_PMIC_EIC_EN (void);
PUBLIC void INT_PHY_PMIC_TMR_EN (void);
PUBLIC void INT_PHY_PMIC_CAL_EN (void);
PUBLIC void INT_PHY_PMIC_PSM_EN (void);

PUBLIC void INT_PHY_PMIC_ADC_UNEN (void);
PUBLIC void INT_PHY_PMIC_RTC_UNEN (void);
PUBLIC void INT_PHY_PMIC_WDG_UNEN (void);
PUBLIC void INT_PHY_PMIC_EIC_UNEN (void);
PUBLIC void INT_PHY_PMIC_TMR_UNEN (void);
PUBLIC void INT_PHY_PMIC_CAL_UNEN (void);
PUBLIC void INT_PHY_PMIC_PSM_UNEN (void);

PUBLIC void INT_PHY_PMIC_PCLK_RTC_EN (void);
PUBLIC void INT_PHY_PMIC_PCLK_WDG_EN (void);
PUBLIC void INT_PHY_PMIC_PCLK_EIC_EN (void);
PUBLIC void INT_PHY_PMIC_PCLK_PSM_TOPA_EN (void);
PUBLIC void INT_PHY_PMIC_PCLK_RTC_TOPA_EN (void);
PUBLIC void INT_PHY_PMIC_PCLK_IOMUX_EN (void);

PUBLIC void INT_PHY_PMIC_PCLK_RTC_UNEN (void);
PUBLIC void INT_PHY_PMIC_PCLK_WDG_UNEN (void);
PUBLIC void INT_PHY_PMIC_PCLK_EIC_UNEN (void);
PUBLIC void INT_PHY_PMIC_PCLK_PSM_TOPA_UNEN (void);
PUBLIC void INT_PHY_PMIC_PCLK_RTC_TOPA_UNEN (void);
PUBLIC void INT_PHY_PMIC_PCLK_IOMUX_UNEN (void);

PUBLIC void INT_PHY_PMIC_RTC_CLK_DISABLE_ALL (void);
PUBLIC void INT_PHY_PMIC_RTC_CLK_ABLE_ALL (void);
PUBLIC void INT_PHY_PMIC_RTC_ARCH_EN (void);
PUBLIC void INT_PHY_PMIC_RTC_RTC_EN (void);
PUBLIC void INT_PHY_PMIC_RTC_WDG_EN (void);
PUBLIC void INT_PHY_PMIC_RTC_EIC_EN (void);
PUBLIC void INT_PHY_PMIC_RTC_EFS_EN (void);
PUBLIC void INT_PHY_PMIC_RTC_ARCH_UNEN (void);
PUBLIC void INT_PHY_PMIC_RTC_RTC_UNEN (void);
PUBLIC void INT_PHY_PMIC_RTC_WDG_UNEN (void);
PUBLIC void INT_PHY_PMIC_RTC_EIC_UNEN (void);
PUBLIC void INT_PHY_PMIC_RTC_EFS_UNEN (void);
PUBLIC uint32 PMIC_ISR_PHY_GetMskIntSts( uint32 int_status,uint32 chan_id);
PUBLIC uint32 PMIC_ISR_PHY_SELESCT(uint32 chan_id);
PUBLIC void PMIC_HisrFunc (uint32 i, void *d);
PUBLIC ISR_EXE_T PMIC_Isr_Handle( uint32 data );

extern PRI_INDEX_GET_STATUS_T irq_isr_array[MAX_ISR_NUM];
extern PRI_INDEX_GET_STATUS_T fiq_isr_array[MAX_FIQ_NUM];
extern uint32 IrqUnitRegValue[ISR_UNIT_NUM];
extern uint32 FiqUnitRegValue[ISR_UNIT_NUM];





/*****************************************************************************/
//  Description:    using interrupt ligic number to clear the interrupt.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQClear (uint32 logic_num);


/*****************************************************************************/
//  Description:    Enable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQEnable (uint32 logic_num);

/*****************************************************************************/
//  Description:    Disable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQDisable (uint32 logic_num);

/*****************************************************************************/
//  Description:    Disable All interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQDisableAll (void);

/*****************************************************************************/
//  Description:    Get IRQ status
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IRQGetUint1Status (void);

/*****************************************************************************/
//  Description:    Get IRQ status, get uint0..group
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetUint0Status(void);
/*****************************************************************************/
//  Description:    Get IRQ IE state
//
//  Dependency:
//  Author:         Yong.Li
//  Note:           ;
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IRQGetIEState(void);

/*****************************************************************************/
//  Description:    Disable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_FIQDisable (uint32 logic_num);


/*****************************************************************************/
//  Description:    Disable the one interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_FIQEnable (uint32 logicNum);

/*****************************************************************************/
//  Description:    Disable All interrupt source FOR fiq.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_FIQDisableAll (void);

PUBLIC void INT_PHYFiqDebug (void);

#endif


