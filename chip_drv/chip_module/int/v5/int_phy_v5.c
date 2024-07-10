/******************************************************************************
 ** File Name:      int_phy_v5.c                                                  *
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
#include "Adi_hal_internal.h"
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
PUBLIC uint32 IrqUnitRegAddrMask[ISR_UNIT_NUM] = {0xffffffff};
#elif (ISR_UNIT_NUM == 2)
uint32 IrqUnitRegAddr[ISR_UNIT_NUM] = {INT_IRQ_STS,ANA_INT_STATUS};
PUBLIC uint32 IrqUnitRegAddrMask[ISR_UNIT_NUM] = {0xffffffff,0x1F};
#elif (ISR_UNIT_NUM == 3)
#error "You must define Interrupt status register, and the Mask value of valid bits"
#elif (ISR_UNIT_NUM == 4)
#error "You must define Interrupt status register, and the Mask value of valid bits"
#endif

PUBLIC uint32 IrqUnitRegValue[ISR_UNIT_NUM] = {0};

//define for fiq
LOCAL uint32 FiqUnitRegAddr[ISR_UNIT_NUM] = {INT_FIQ_STS};/*lint !e785 confirmed by xuepeng*/

PUBLIC uint32 FiqUnitRegValue[ISR_UNIT_NUM] = {0};

typedef struct ana_sub_int_ctrl
{
    uint32  mod_int_ctrl_addr;
    uint16  int_msk;
} ANA_SUB_INT_CTRL_T;


static const ANA_SUB_INT_CTRL_T sub_int_ctrl[ANA_MAX_INT_NUM] =
{
#ifdef NEED_MODIFY
    {ADC_INT_EN,        ADC_IRQ_EN_BIT  },
    {0,                 0               },          ///ana gpio
    {ANA_RTC_INT_EN,    RTC_INT_ALL_MSK },
    {WDG_CTRL,  WDG_INT_EN_BIT  },
    {TPC_INT_EN,        TPC_INT_ALL_MSK }
#endif
    {0,                 0               }
};
/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/, for
//       process default interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IrqHandleDefaultFunc (uint32 LogicBit)
{

    CHIPDRV_DisableIRQINT (LogicBit);

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
    IrqUnitRegValue[0] = REG32 (IrqUnitRegAddr[0]);

    if (IrqUnitRegValue[0]& (1 << TB_ANA_INT))
    {
        IrqUnitRegValue[1] = ANA_REG_GET (IrqUnitRegAddr[1]);
        TM_SendTestPointRequest (0x15170001, IrqUnitRegValue[1]);
    }
    else
    {
        IrqUnitRegValue[1] = 0;
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

    ANA_REG_SET (ANA_INT_EN,0);

    CHIPDRV_EnableIRQINT (TB_ANA_INT);

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
#include "Adi_hal_internal.h"
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

    if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_0)
    {
        CHIP_REG_SET (INT_FIQ_DISABLE, irq_isr_array[i].IntRegBitMask);
        CHIP_REG_OR (INT_IRQ_EN, irq_isr_array[i].IntRegBitMask);
    }

    if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_1)
    {
        ANA_REG_OR (ANA_INT_EN, irq_isr_array[i].IntRegBitMask);
    }

    if (logicNum == TB_GPIO_INT)
    {
        ANA_REG_OR (ANA_INT_EN, ANA_GPIO_IRQ);
    }

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
    TX_IRQ_SAVE_AREA

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/
    TX_DISABLE_IRQ

    if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_0)
    {
        CHIP_REG_SET (INT_IRQ_DISABLE, irq_isr_array[i].IntRegBitMask);
    }

    if (irq_isr_array[i].IntRegValueIndex == ISR_UNIT_1)
    {
        ANA_REG_AND (ANA_INT_EN, ~irq_isr_array[i].IntRegBitMask);
    }

    if (logicNum == TB_GPIO_INT)
    {
        ANA_REG_AND (ANA_INT_EN, (~ANA_GPIO_IRQ));
    }

    TX_RESTORE_IRQ
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

    if(i >= MAX_ISR_NUM)
    {
        CHIP_DRV_PASSERT ( 0, ("logicNum=%d\n",logicNum));/*assert verified*/
    }
    else
    {
        INT_DISABLE_ARM_IRQ_FIQ();

        CHIP_REG_SET (INT_IRQ_DISABLE, fiq_isr_array[i].IntRegBitMask);
        CHIP_REG_OR (INT_FIQ_EN, fiq_isr_array[i].IntRegBitMask);

        INT_ENABLE_ARM_IRQ_FIQ();
    }
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

    if( i>= MAX_ISR_NUM)
    {
        CHIP_DRV_PASSERT ( 0, ("logicNum=%d\n",logicNum));/*assert verified*/
    }
    else
    {
        CHIP_REG_SET (INT_FIQ_DISABLE, fiq_isr_array[i].IntRegBitMask);
    }

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
    TM_SendTestPointRequest (_tx_pc_val_before_fiq, REG32 (APB_PERI_CTL0));
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
