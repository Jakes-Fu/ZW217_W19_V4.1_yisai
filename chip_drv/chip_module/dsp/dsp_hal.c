/******************************************************************************
 ** File Name:      dsp_hal.c                                                 *
 ** Author:         windy.Wang                                                *
 ** DATE:           11/26/2011                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic charging and power manage     *
 **                 operation process.                                        *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                  *
 ** 26/12/2011      windy.Wang       Create.                                  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "isr_drvapi.h"
#include "dsp_drvapi.h"
#include "dsp_phy.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************
 ** Description :   Get Dsp share memory physics addresss.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_HAL_GetDSPShareMemPhyBaseAddr(void)
{
    return DSP_PHY_GetDSPShareMemPhyBaseAddr();
}

/*****************************************************************************
 ** Description :   Get Dsp share memory virtual addresss.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_HAL_GetDSPShareMemVirBaseAddr(void)
{
    return DSP_PHY_GetDSPShareMemVirBaseAddr();
}
/*****************************************************************************
 ** Description :   GET ARM/DSP Share Memory BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_HAL_GetShareMemBaseAddr(void)
{
    return DSP_PHY_GetShareMemBaseAddr();
}
/*****************************************************************************
 ** Description :   Get Interal On-Chip RAM SPACE BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_HAL_GetSwitchMemBaseAddr(void)
{
    return DSP_PHY_GetSwitchMemBaseAddr();
}
/*****************************************************************************
 ** Description :   get DSP-Side SPACE BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_HAL_GetAudioInterruptBaseAddr(void)
{
    return DSP_PHY_GetAudioInterruptBaseAddr();
}
/*****************************************************************************
 ** Description :   DSP in RESET/RELEASE MODE
 **                 enable: hold dsp
 **                 disable: release dsp
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_DspinResetMode(BOOLEAN is_enable)
{
    DSP_PHY_DspinResetMode(is_enable);
}
/*****************************************************************************
 ** Description :   CLK_TYPE_DSP Force to open/close clock of DSP
 **                 CLK_TYPE_ALL_CLK_EN :All clock enable. When this bit is 1, 
 **                                 all clock gatings are disabled. This bit 
 **                                 is for debug use, and do not set this bit 
 **                                 in normal mode
 **                 CLK_TYPE_ROM_CLK_EN:     Enable automatic access to ROM, 
 **                                 active high. After boot, set this bit
 **                                 to high.
 **                                 0: force clock free running for ROM
 **                                 1: clock enabled automatically
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_DspClockEnable(CLK_DSP_TYPE_E clk_dsp_type, BOOLEAN is_enable)
{
    DSP_PHY_DspClockEnable(clk_dsp_type,is_enable);
}
/*****************************************************************************
 ** Description :   UART1 owner SEL, Owner selection for UART 1
 **                 1: uart1 controled by dsp
 **                 0: uart1 controled by arm
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_UART1_MUX_SEL_DSP_EN(BOOLEAN is_enable)
{
    DSP_PHY_UART1_MUX_SEL_DSP_EN(is_enable);
}

/*****************************************************************************
 ** Description :   soft reset DSP Z-bus Accelerators
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_ZBUSAcceleratorReset(void)
{
    DSP_PHY_ZBUSAcceleratorReset();
}
/*****************************************************************************
 ** Description :   Enables/Disable the use of external z-space memory.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_ZSpaceMemoryEnable(BOOLEAN is_enable)
{
    DSP_PHY_ZSpaceMemoryEnable(is_enable);
}
/*****************************************************************************
 ** Description :   Set strapping mode for dsp boot
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_SetStrapMode(DSP_TEAKLITE_STRAP strap_mode)
{
    DSP_PHY_SetStrapMode(strap_mode);
}
/*****************************************************************************
 ** Description :   ARM Generate an interrupt to DSP,
 **                 intetrupt type would be IRQ0/IRQ1/FRQ0/FRQ1
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_SetMcuIrqInt(DSP_INT_TYPE_E int_type )
{
    switch(int_type)
    {
        case DSP_INT_IRQ0:
            DSP_PHY_SetMcuIrqInt(DSP_INT_IRQ0);
            break;
        case DSP_INT_FRQ0:
            DSP_PHY_SetMcuIrqInt(DSP_INT_FRQ0);
            break;
        case DSP_INT_IRQ1:
            DSP_PHY_SetMcuIrqInt(DSP_INT_IRQ1);
            break;
        case DSP_INT_FRQ1:
            DSP_PHY_SetMcuIrqInt(DSP_INT_FRQ1);
            break;
    }
}
/*****************************************************************************
 ** Description :   Clear DSP Interrupt from DSP
 **                 intetrupt type would be IRQ0/IRQ1/FRQ0/FRQ1
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_ClrDspIrqInt(DSP_INT_TYPE_E int_type )
{
    switch(int_type)
    {
        case DSP_INT_IRQ0:
            DSP_PHY_ClrDspIrqInt(DSP_INT_IRQ0);
            break;
        case DSP_INT_FRQ0:
            DSP_PHY_ClrDspIrqInt(DSP_INT_FRQ0);
            break;
        case DSP_INT_IRQ1:
            DSP_PHY_ClrDspIrqInt(DSP_INT_IRQ1);
            break;
        case DSP_INT_FRQ1:
            DSP_PHY_ClrDspIrqInt(DSP_INT_FRQ1);
            break;
    }
}
/*****************************************************************************
 ** Description :   GET DSP interrupt STATUS
 **                 intetrupt type would be IRQ0/IRQ1/FRQ0/FRQ1
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC BOOLEAN DSP_HAL_GetDspIrqSts(DSP_INT_TYPE_E int_type)
{
    BOOLEAN int_flag = 0;
    
    switch(int_type)
    {
        case DSP_INT_IRQ0:
            int_flag =  DSP_PHY_GetDspIrqSts(DSP_INT_IRQ0);
            break;
        case DSP_INT_FRQ0:
            int_flag = DSP_PHY_GetDspIrqSts(DSP_INT_FRQ0);
            break;
        case DSP_INT_IRQ1:
            int_flag = DSP_PHY_GetDspIrqSts(DSP_INT_IRQ1);
            break;
        case DSP_INT_FRQ1:
            int_flag = DSP_PHY_GetDspIrqSts(DSP_INT_FRQ1);
            break;
    }
    
    return int_flag;
}
/*****************************************************************************
 ** Description :   Enable/Disable Internal RAM is control by ARM/DSP
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_IRAMSwitchtoARM(BOOLEAN is_enable)
{
    DSP_PHY_IRAMSwitchtoARM(is_enable);
}
/*****************************************************************************
 ** Description :   Set DSP BOOT VECTOR
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_SetDspBootVector(uint32 addr)
{
    DSP_PHY_SetDspBootVector(addr);
}
/*****************************************************************************
 ** Description :   Enable/Disable DSP to BOOT
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_DspBootEnable(BOOLEAN is_enable)
{
    DSP_PHY_DspBootEnable(is_enable);
}
/*****************************************************************************
 ** Description :   Enable ARM to response DSP IRQ or FIQ Interrupt
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_DSPIRQFIQINTEnable(void)
{
    DSP_PHY_DSPIRQFIQINTEnable();
}
/*****************************************************************************
 ** Description :   set DSP interrupt response function
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_RegisterHandle (TB_ISR callback)
{
    DSP_PHY_RegisterHandle(callback);
}
/*****************************************************************************
 ** Description :   remove DSP interrupt response function
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_UnregisterHandle (void)
{
    DSP_PHY_UnregisterHandle ();
}
/*****************************************************************************
 ** Description :   Enable/Disable ARM enables CEVAX matrix to sleep.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_ARM_DSLEEP_EN (BOOLEAN is_enable)
{
    DSP_PHY_ARM_DSLEEP_EN(is_enable);
}
/*****************************************************************************
 ** Description :   ARM access DSP ASHB bridge auto enable, active high.
 **                 1'b0 : ARM to DSP async bridge closed;
 **                 1'b1 : ARM to DSP async bridge automatically enable with
 **                        ARM sleep/wakeup;
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_ARM_TO_DSP_ASHB_EN (BOOLEAN is_enable)
{
    DSP_HAL_ARM_TO_DSP_ASHB_EN(is_enable);
}
/*****************************************************************************
 ** Description :   This function is used to check the dsp status
 ** Global resource dependence :
 ** Author :        Yuhua.Shi
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_HAL_ExceptionCheck(void)
{
    DSP_PHY_ExceptionCheck();
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  //__cplusplus
