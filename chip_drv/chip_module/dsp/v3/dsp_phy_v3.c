/******************************************************************************
 ** File Name:      dsp_phy_v3.c                                              *
 ** Author:         windy.Wang                                                *
 ** DATE:           26/12/2011                                                *
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
#include "sc_reg.h"
#include "dsp_drvapi.h"
#include "chip_plf_export.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*****************************************************************************
 ** Description :   Get Dsp share memory physics addresss.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetDSPShareMemPhyBaseAddr(void)
{  
    return DSP_SHAREMEM_PHY_BASE_ADDR;
}

/*****************************************************************************
 ** Description :   Get Dsp share memory virtual addresss.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetDSPShareMemVirBaseAddr(void)
{
    return DSP_SHAREMEM_VIR_BASE_ADDR;
}

/*****************************************************************************
 ** Description :   GET ARM/DSP Share Memory BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetShareMemBaseAddr(void)
{
    return DSP_SHMEM_BASE;
}

/*****************************************************************************
 ** Description :   Get Interal On-Chip RAM SPACE BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetSwitchMemBaseAddr(void)
{
    return 0;//function is empyt
}

/*****************************************************************************
 ** Description :   get DSP-Side SPACE BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetAudioInterruptBaseAddr(void)
{
    return 0x10132000;
}

/*****************************************************************************
 ** Description :   DSP in RESET/RELEASE MODE
 **                 enable: hold dsp
 **                 disable: release dsp
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_DspinResetMode(BOOLEAN is_enable)
{
    if (is_enable)
    {
        CHIP_REG_AND (AHB_DSP_RESET ,~ SWRST_MCU_DSP_RST);//reset the dsp     
    }
    else
    {
        CHIP_REG_OR (AHB_DSP_RESET , SWRST_MCU_DSP_RST);//release the dsp
    }
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
PUBLIC void DSP_PHY_DspClockEnable(CLK_DSP_TYPE_E clk_dsp_type, BOOLEAN is_enable)
{
    switch(clk_dsp_type)
    {
        case CLK_TYPE_DSP_CLK_AUTO_EN:
            //function is empty
            break;
        case CLK_TYPE_ALL_CLK_EN:
            if(is_enable)
            {
                CHIP_REG_OR(GR_PCTL, GR_PCTL_ALL_CLK_EN);
            }
            else
            {
                CHIP_REG_AND(GR_PCTL, ~ GR_PCTL_ALL_CLK_EN);
            }
            break;
        case CLK_TYPE_ROM_CLK_EN:
            if(is_enable)
            {
                CHIP_REG_OR(GR_PCTL, GR_PCTL_ROM_CLK_EN);
            }
            else
            {
                CHIP_REG_AND(GR_PCTL, ~ GR_PCTL_ROM_CLK_EN);
            }
			break;           
        default:
            break;
    }
}

/*****************************************************************************
 ** Description :   UART1 owner SEL, Owner selection for UART 1
 **                 1: uart1 controled by dsp
 **                 0: uart1 controled by arm
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_UART1_MUX_SEL_DSP_EN(BOOLEAN is_enable)
{
    if(is_enable)
    {
        CHIP_REG_OR(GR_PCTL, MISC0_UART1_MUX_SEL);
    }
    else
    {
        CHIP_REG_AND(GR_PCTL, ~ MISC0_UART1_MUX_SEL);
    }  
}

/*****************************************************************************
 ** Description :   soft reset DSP Z-bus Accelerators
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_ZBUSAcceleratorReset(void)
{
    uint32 i;
    
    CHIP_REG_OR (GR_GEN0, GEN0_MCU_DSP_RST);
    
    for(i=0; i<100; i++)
    {
    ;
    }
    
    CHIP_REG_AND (GR_GEN0, ~ GEN0_MCU_DSP_RST);
}
/*****************************************************************************
 ** Description :   Enables/Disable the use of external z-space memory.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_ZSpaceMemoryEnable(BOOLEAN is_enable)
{
    //function is empyt
}

/*****************************************************************************
 ** Description :   Set strapping mode for dsp boot
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_SetStrapMode(DSP_TEAKLITE_STRAP strap_mode)
{
    //function is empyt
}

/*****************************************************************************
 ** Description :   ARM Generate an interrupt to DSP,
 **                 intetrupt type would be IRQ0/IRQ1/FRQ0/FRQ1
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_SetMcuIrqInt(DSP_INT_TYPE_E int_type )
{
    switch(int_type)
    {
        case DSP_INT_IRQ0:
            CHIP_REG_OR (GR_IRQ, IRQ_MCU_IRQ0);
            break;
        case DSP_INT_FRQ0:
            CHIP_REG_OR (GR_IRQ, IRQ_MCU_FRQ0);
            break;
        case DSP_INT_IRQ1:
            CHIP_REG_OR (GR_IRQ, IRQ_MCU_IRQ1);
            break;
        case DSP_INT_FRQ1:
            CHIP_REG_OR (GR_IRQ, IRQ_MCU_FRQ1);
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
PUBLIC void DSP_PHY_ClrDspIrqInt(DSP_INT_TYPE_E int_type )
{
    switch(int_type)
    {
        case DSP_INT_IRQ0:
            CHIP_REG_OR (GR_ICLR, ICLR_DSP_IRQ0_CLR);
            break;
        case DSP_INT_FRQ0:
            CHIP_REG_OR (GR_ICLR, ICLR_DSP_FRQ0_CLR);
            break;
        case DSP_INT_IRQ1:
            CHIP_REG_OR (GR_ICLR, ICLR_DSP_IRQ1_CLR);
            break;
        case DSP_INT_FRQ1:
            CHIP_REG_OR (GR_ICLR, ICLR_DSP_FRQ1_CLR);
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
PUBLIC BOOLEAN DSP_PHY_GetDspIrqSts(DSP_INT_TYPE_E int_type )
{
    return 0;//sc8800g has no dsp interrupt status
}

/*****************************************************************************
 ** Description :   Internal RAM is control by ARM/DSP
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_IRAMSwitchtoARM(BOOLEAN is_enable)
{
    //function is empyt
}

/*****************************************************************************
 ** Description :   DSP BOOT VECTOR
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_SetDspBootVector(uint32 addr)
{
    CHIP_REG_SET (AHB_DSP_BOOT_VECTOR, addr);
}

/*****************************************************************************
 ** Description :   Enable/Disable DSP to BOOT
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_DspBootEnable(BOOLEAN is_enable)
{
    if (is_enable)
    {
        CHIP_REG_OR (AHB_DSP_BOOT_EN, ARM_BOOT_EN);
    }
    else
    {
        CHIP_REG_AND (AHB_DSP_BOOT_EN, ~ ARM_BOOT_EN);
    }
}

/*****************************************************************************
 ** Description :   Enable ARM to response DSP IRQ or FIQ Interrupt
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_DSPIRQFIQINTEnable(void)
{
    CHIP_REG_OR (INT_IRQ_EN, TB_DSP_INT1);
}

/*****************************************************************************
 ** Description :   set DSP interrupt response function
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_RegisterHandle (TB_ISR callback)
{
    //function is empyt
}

/*****************************************************************************
 ** Description :   remove DSP interrupt response function
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_UnregisterHandle (void)
{
    //function is empyt
}

/*****************************************************************************
 ** Description :   ARM enables CEVAX matrix to sleep.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_ARM_DSLEEP_EN (BOOLEAN is_enable)
{
    if(is_enable)
    {
        CHIP_REG_OR (AHB_CTL1, AHB_CTRL1_ARM_DAHB_SLEEP_EN);
    }
    else
    {
        CHIP_REG_AND (AHB_CTL1, ~ AHB_CTRL1_ARM_DAHB_SLEEP_EN);
    }
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
PUBLIC void DSP_PHY_ARM_TO_DSP_ASHB_EN (BOOLEAN is_enable)
{
    if(is_enable)
    {
        CHIP_REG_OR (DSP_BOOT_EN, ASHB_ARMTODSP_EN);
    }
    else
    {
        CHIP_REG_AND (DSP_BOOT_EN, ~ ASHB_ARMTODSP_EN);
    }
}

/*****************************************************************************
 ** Description :   This function is used to check the dsp status
 ** Global resource dependence :
 ** Author :        Yuhua.Shi
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_ExceptionCheck(void)
{
    //function is empyt
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of __cplusplus
