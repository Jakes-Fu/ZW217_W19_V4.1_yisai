/******************************************************************************
 ** File Name:      dsp_phy_v0.c                                              *
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
#include "dal_power.h"
#include "isr_drvapi.h"
#include "dsp_drvapi.h"
#include "tb_hal.h"
#include "chip_plf_export.h"
#include "boot_drvapi.h"
#include "dsp_drvapi.h"
#include "chip_internal.h"
#include "dsp_phy.h"

/*lint -esym(551, s_dsp_once_in_exception) */
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
//used to recode if the dsp is both in exception status this time and last time
LOCAL BOOLEAN s_dsp_in_exception_status = FALSE;
//used to recode if the dsp is in exception status once
LOCAL BOOLEAN s_dsp_once_in_exception   = FALSE;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
const uint16 dsp_dl_secure_data_sc6800h[] = 
{
    0x0000,//nop
    0x0000,//nop
    0x4180,//br
    0x0000 //pc=0000
};
/*****************************************************************************
 ** Description :   Get Dsp share memory physics addresss.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetDSPShareMemPhyBaseAddr(void)
{  
    return 0;//function is empyt
}
/*****************************************************************************
 ** Description :   Get Dsp share memory virtual addresss.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetDSPShareMemVirBaseAddr(void)
{
    return 0;//function is empyt
}
/*****************************************************************************
 ** Description :   GET ARM/DSP Share Memory BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetShareMemBaseAddr(void)
{
    return SHARE_MEM_BEGIN;
}
/*****************************************************************************
 ** Description :   Get Interal On-Chip RAM SPACE BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetSwitchMemBaseAddr(void)
{
    return 0x40000800;
}
/*****************************************************************************
 ** Description :   get DSP-Side SPACE BASE ADDRESS
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC uint32 DSP_PHY_GetAudioInterruptBaseAddr(void)
{
    return 0;//function is empyt
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
        CHIP_REG_OR (GR_SOFT_RST, SWRST_MCU_DSP_RST);//reset the dsp    
    }
    else
    {
        CHIP_REG_AND (GR_SOFT_RST, ~ SWRST_MCU_DSP_RST);//release the ds
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
PUBLIC void DSP_PHY_DspClockEnable(BOOLEAN is_enable)
{
    switch(clk_dsp_type)
    {
        case CLK_TYPE_DSP_CLK_AUTO_EN:
            if (is_enable)
            {        
                CHIP_REG_OR (GR_MISC0, MISC0_DSP_CLK_AUTO_EN);
            }
            else
            {
                CHIP_REG_AND (GR_MISC0, ~ MISC0_DSP_CLK_AUTO_EN);
            }
            break;    
        case CLK_TYPE_ALL_CLK_EN:
            //function is empty
            break;
        case CLK_TYPE_ROM_CLK_EN:
           //function is empty
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
    //function is empty
}

/*****************************************************************************
 ** Description :   soft reset DSP Z-bus Accelerators
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_ZBUSAcceleratorReset(void)
{
    //function is empyt
}
/*****************************************************************************
 ** Description :   Enables the use of external z-space memory.
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
    CHIP_REG_AND (GR_MISC0, ~ MISC0_DSP_STRAP_BIT_MASK);
    CHIP_REG_OR  (GR_MISC0, strap_mode << MISC0_DSP_STRAP_BIT_SHIFT);
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
            CHIP_REG_OR (GR_IRQ, APB_MCU_IRQ_EN);
            break;
        case DSP_INT_FRQ0:
            //function is empyt
            break;
        case DSP_INT_IRQ1:
            //function is empyt
            break;
        case DSP_INT_FRQ1:
            //function is empyt
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
            CHIP_REG_OR (GR_IRQ, APB_DSP_IRQ_CLR);
            break;
        case DSP_INT_FRQ0:
            CHIP_REG_OR (GR_IRQ, APB_DSP_FIQ_CLR);
            break;
        case DSP_INT_IRQ1:
            //function is empyt
            break;
        case DSP_INT_FRQ1:
            //function is empyt
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
    BOOLEAN int_flag = 0;
    
    switch(int_type)
    {
        case DSP_INT_IRQ0:
            int_flag = (CHIP_REG_GET (GR_IRQ) & APB_DSP_IRQ_EN) >> 2;
            break;
        case DSP_INT_FRQ0:
            int_flag = (CHIP_REG_GET (GR_IRQ) & APB_DSP_FIQ_EN) >> 3;
            break;
        case DSP_INT_IRQ1:
            //function is empty
            break;
        case DSP_INT_FRQ1:
            //function is empty
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
PUBLIC void DSP_PHY_IRAMSwitchtoARM(BOOLEAN is_enable)
{
    if (is_enable)
    {
        CHIP_REG_OR (GR_MISC0, MISC0_XRAM0_ARM_EN);
    }
    else
    {
        CHIP_REG_AND (GR_MISC0, MISC0_XRAM0_ARM_EN);
    }
}

/*****************************************************************************
 ** Description :   Set DSP BOOT VECTOR
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_SetDspBootVector(uint32 addr)
{
    CHIP_REG_SET (GR_ARM_BOOT_ADDR, addr & 0xffff);
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
        CHIP_REG_OR (GR_ARM_BOOT_ADDR, ARM_BOOT_EN);
    }
    else
    {
        CHIP_REG_AND (GR_ARM_BOOT_ADDR, ~ ARM_BOOT_EN);
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
    CHIP_REG_OR (INT_IRQ_EN, INTCTL_DSP_IRQ);
}
/*****************************************************************************
 ** Description :   set DSP interrupt response function
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_RegisterHandle (TB_ISR callback)
{
    ISR_RegHandler (TB_DSP_INT, callback);
}

/*****************************************************************************
 ** Description :   remove DSP interrupt response function
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_UnregisterHandle (void)
{
    ISR_UnRegHandler (TB_DSP_INT);
}

/*****************************************************************************
 ** Description :   Enable/Disable ARM enables CEVAX matrix to sleep.
 ** Global resource dependence :
 ** Author :        windy.wang
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_ARM_DSLEEP_EN (BOOLEAN is_enable);
{
    //function is empty
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
    //function is empty
}

/*****************************************************************************/
//Description:   This function is used to check if dsp is in exception status
//Author:
//Note:         
//Parameter:
/*****************************************************************************/
LOCAL BOOLEAN _DSP_PHY_IsDSPException(void)
{
    BOOLEAN ret = FALSE;
    
    if (APB_STATUS_CLK_QBC_EB != (CHIP_REG_GET(GR_STATUS) & APB_STATUS_CLK_QBC_EB))        
    {
        ret = TRUE;//dsp schedule is in sleep mode, this is a exception status
    }

    return ret;
}
/*****************************************************************************
 ** Description :   This function is used to check the dsp status
 ** Global resource dependence :
 ** Author :        Yuhua.Shi
 ** Note :    
 *****************************************************************************/
PUBLIC void DSP_PHY_ExceptionCheck(void)
{
    //When startup by power button, charger, battery-in, reset(RESET_MODE) and the other startup mode(like NORMAL_MODE for CFT, auto-test and the others),
    //It will check the dsp status.
    if (CheckRTCState () & BIT_4)
    {
        //if startup by alarm, don't check dsp status because in alarm startup window that no need the call function.
        //And then if user press the power key to power on the system in alarm startup window, the system will do a watchdog reset.
        //It will check dsp status later.
        return;
    }
 
    //get the dsp status now
    if(_DSP_PHY_IsDSPException())
    {   
        //get the dsp status last time
        if(CHIP_PHY_GetWDGHWFlag(TYPE_DSPEXCEPTION))
        {
            //this time and last time, the dsp is both in exception status, so return directly, not to reset again.
            s_dsp_in_exception_status = TRUE;
            return;
        }
        else
        {
            //dsp is in exception now, so call dsp exception handle
            DSP_CTRL_DownLoadCode((const uint16*)dsp_dl_secure_data_sc6800h, (sizeof(dsp_dl_secure_data_sc6800h)/(sizeof(uint16))));
            
            //record the dsp exception status
            CHIP_PHY_SetWDGHWFlag(TYPE_DSPEXCEPTION, TRUE);
  
            //reset the whole system
            HAL_MCUReset();
        }
    }
    else
    {
        if(CHIP_PHY_GetWDGHWFlag(TYPE_DSPEXCEPTION))
        {
            s_dsp_once_in_exception = TRUE;
            //clear the dsp exception status
            CHIP_PHY_SetWDGHWFlag(TYPE_DSPEXCEPTION, FALSE);
        }
    }
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of dsp_phy_v0.c
