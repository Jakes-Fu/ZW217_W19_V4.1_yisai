/******************************************************************************
 ** File Name:      int_phy_v0.c                                                  *
 ** Author:         steve.zhan                                             *
 ** DATE:           10/14/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2002     steve.zhan    Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "../int_phy.h"
//#include "isr_drvapi.h"

#include "task_monitor.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Data                                              *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
//define for irq
#if (ISR_UNIT_NUM == 1)
LOCAL uint32 IrqUnitRegAddr[ISR_UNIT_NUM] = {INT_IRQ_STS};
#elif (ISR_UNIT_NUM == 2)
#error "You must define Interrupt status register, and the Mask value of valid bits"
#elif (ISR_UNIT_NUM == 3)
#error "You must define Interrupt status register, and the Mask value of valid bits"
#elif (ISR_UNIT_NUM == 4)
#error "You must define Interrupt status register, and the Mask value of valid bits"
#endif

PUBLIC uint32 IrqUnitRegValue[ISR_UNIT_NUM] = {0};

//define for fiq
LOCAL uint32 FiqUnitRegAddr[ISR_UNIT_NUM] = { (uint32) INT_FIQ_STS};

PUBLIC uint32 FiqUnitRegValue[ISR_UNIT_NUM] = {0};

/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/, for
//       process default interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IrqHandleDefaultFunc (uint32 LogicBit)
{
    INT_PHY_IRQDisable (LogicBit);
    return (uint32) ISR_DONE;
}

/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/,
//  for process default  interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_FiqHandleDefaultFunc (uint32 LogicBit)
{
    INT_PHY_FIQDisable (LogicBit);
    return (uint32) ISR_DONE;
}
/*****************************************************************************/
//  Description:    This function is called when an IRQ happens. Is used to get
//                   interrupt registers status value.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
//  For example, the interrupt status register like this:
//
// intReg1:
//
//     bit00
//     bit01                       bit10
//     bit02                       bit11
//     bit03 ----> intReg2 bit12
//     bit04                       bit13
//
//     IrqUnitRegValue[0] = REG32(addr of intReg1);
//     IrqUnitRegValue[1] = (REG32(addr of intReg1) & (0b)01000) ? REG32(addr of intReg2) : 0;
//
/*****************************************************************************/

PUBLIC void INT_PHY_GetIrqIntStatus (void)
{
    int unitNum;

    for (unitNum = 0; unitNum < ISR_UNIT_NUM; ++unitNum)
    {
        IrqUnitRegValue[unitNum] = REG32 (IrqUnitRegAddr[unitNum]);
    }

    return;
}

PUBLIC void INT_PHY_GetFiqIntStatus (void)
{
    FiqUnitRegValue[0] = REG32 (FiqUnitRegAddr[0]);
}
/*****************************************************************************/
//  Description:    This function is called when system startup, only init. once.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_Init (void)
{
    INT_PHY_IRQDisableAll ();

    return;
}


/*****************************************************************************/
//  Description:    using interrupt ligic number to clear the interrupt.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQClear (uint32 logicNum)
{

}

/*****************************************************************************/
//  Description:    Enable certain interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQEnable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/

    INT_DISABLE_ARM_IRQ_FIQ();

    CHIP_REG_SET (INT_FIQ_DISABLE, irq_isr_array[i].IntRegBitMask);
    CHIP_REG_OR (INT_IRQ_EN, irq_isr_array[i].IntRegBitMask);

    INT_ENABLE_ARM_IRQ_FIQ();
}

/*****************************************************************************/
//  Description:    Disable certain interrupt source. It is IRQ interrupt safe.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQDisable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/

    CHIP_REG_SET (INT_IRQ_DISABLE, irq_isr_array[i].IntRegBitMask);

}

/*****************************************************************************/
//  Description:    Disable All interrupt source. It is IRQ interrupt safe.
//                       General use in system startup  and debug, and so on.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQDisableAll (void)
{
    CHIP_REG_SET (INT_IRQ_DISABLE, 0xffffffff);

    return ;
}

/*****************************************************************************/
//  Description:    Get IRQ status, get uint1..group
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetUint1Status()
{
    return REG32 (INT_IRQ_STS);
}

/*****************************************************************************/
//  Description:    Get IRQ IE state
//
//  Dependency:
//  Author:         Yong.Li
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetIEState(void)
{
    return REG32 (INT_IRQ_EN);
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQEnable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) fiq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/

    INT_DISABLE_ARM_IRQ_FIQ();

    CHIP_REG_SET (INT_IRQ_DISABLE, fiq_isr_array[i].IntRegBitMask);
    CHIP_REG_OR (INT_FIQ_EN, fiq_isr_array[i].IntRegBitMask);

    INT_ENABLE_ARM_IRQ_FIQ();

    return ;
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQDisable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) fiq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/


    CHIP_REG_SET (INT_FIQ_DISABLE, fiq_isr_array[i].IntRegBitMask);


    return ;
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQDisableAll (void)
{

    CHIP_REG_SET (INT_FIQ_DISABLE, 0xffffffff);

    return ;
}


PUBLIC void INT_PHYFiqDebug (void)
{
    extern uint32 _tx_pc_val_before_fiq;
    TM_SendTestPointRequest (_tx_pc_val_before_fiq, REG32 (GR_VB_CTL));
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
