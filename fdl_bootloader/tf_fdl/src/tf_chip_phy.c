/******************************************************************************
 ** File Name:      tf_chip_phy.c                                             *
 ** Author:         Fei Zhang                                              *
 ** DATE:           06/07/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/11/2011     Fei.Zhang        Create.                                   *                     *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_drvapi.h"
#include "chip_plf_export.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 chip_id = 0;
LOCAL CHIP_TYPE_E chip_type = CHIP_TYPE_MAX;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetAPBClk (void)
{
    return ARM_CLK_26M;
}

/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void)
{
#ifdef PLATFORM_SC6530
	#ifdef FPGA_SUPPORT_SC6530
	    return SC6530;
	#else
	    return chip_type;
	#endif	
#else	
	CHIP_InitHWChipID();
    return chip_type;
#endif	   
}

/*****************************************************************************/
//  Description:    The function is used to return chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetHWChipID (void)   // return chip ID number.
{
    return chip_id;
}

#ifdef PLATFORM_SC8800G
uint32 CHIP_GetMPllClk (void)
{
    return (
               CHIP_GetAPBClk()
               / ( ( (* (volatile uint32 *) GR_MPLL_MN) & 0x003F0000) >>16) \
               * ( (* (volatile uint32 *) GR_MPLL_MN) & 0x00000FFF));
}

LOCAL uint32 CHIP_GetMcuSelClk()
{

    volatile uint32 clk_mcu_sel = REG32 (AHB_ARM_CLK);
    uint32 clk_mcu = 0;
    clk_mcu_sel = (clk_mcu_sel >> 23) & 0x3;

    switch (clk_mcu_sel)
    {
        case 0:
            clk_mcu = (0 == (REG32 (GR_GEN3) & 0x3)) ?
                      (CHIP_GetMPllClk()) :                     // 8800G2
                      ( (1 == (REG32 (GR_GEN3) & 0x3)) ?
                        (256000000) :                             // 8801G1
                        (192000000));                             // 8802G1
            break;
        case 1:
            clk_mcu = 153600000;
            break;
        case 2:
            clk_mcu = 64000000;
            break;
        case 3:
            clk_mcu = 26000000;
            break;
        default:
            // can't go there
            break;
    }

    return clk_mcu;
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
    volatile uint32 ahb_div = REG32 (AHB_ARM_CLK);
#ifdef FPGA_VERIFICATION
    return 36000000;
#else
    ahb_div = (ahb_div >> 4) & 7;
    ahb_div = ahb_div + 1;

    if (REG32 (AHB_ARM_CLK) &BIT_30)
    {
        ahb_div = ahb_div << 1;
    }

    if (REG32 (AHB_ARM_CLK) &BIT_31)
    {
        ahb_div=ahb_div<<1;
    }

    return (CHIP_GetMcuSelClk() / ahb_div);
#endif
}

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC void CHIP_InitHWChipID (void)
{
    uint32 id =  * (uint32 *) (CHIP_TYPE);

    chip_id = id;

    switch (chip_id)
    {
        case 0x88000000:
            chip_type = SC8800G1;
            break;
        case 0x88000002:
            chip_type = (0 == (REG32 (GR_GEN3) & 0x3)) ?
                        (SC8800G2) :                     // 8800G2
                        ( (1 == (REG32 (GR_GEN3) & 0x3)) ?
                          (SC8801G2) :                     // 8801G2
                        ( (2 == (REG32 (GR_GEN3) & 0x3)) ?  
                          (SC8802G2):                     // 8802G2
                          (SC6810)));                     // SC6810
            break;
        default:
            break;
    }

    return;
}

PUBLIC void TF_CHIP_ResetMCU (void)
{
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    WDG_TimerInit ();

    ANA_REG_OR (ANA_AGEN, (AGEN_WDG_EN | AGEN_RTC_WDG_EN));
    
    while (i--) {}    

    WDG_ResetMCU ();
    
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 500)
        {
            tick2 = SCI_GetTickCount();
        }
    }
}

#define HWRST_ADDR     (DMA_REG_BASE + 0x400 + 31 * 0x20 + 8)  //src addr reg in chn 31
#define HW_RST_MASK       0x1F

LOCAL uint32 TF_CHIP_PHY_GetHwRstAddr (void)
{
    // Returns a DWORD of IRAM shared with DCAM
    return HWRST_ADDR;
}

PUBLIC void TF_CHIP_PHY_ResetHWFlag (uint32 val)
{
    //set WDG Mon to 0xF
    ANA_REG_AND (ANA_HWRST_STATUS, ~0xF);
    ANA_REG_OR (ANA_HWRST_STATUS, (val&0xF));

    CHIP_REG_AND (TF_CHIP_PHY_GetHwRstAddr(), ~HW_RST_MASK);
    CHIP_REG_OR (TF_CHIP_PHY_GetHwRstAddr(), (val & HW_RST_MASK));
}

#endif
#ifdef PLATFORM_SC6800H

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC void CHIP_InitHWChipID (void)
{
    uint32 id =  REG32 (CHIP_TYPE);

    chip_id = id;

    switch (chip_id)
    {
        case 0x68000000:
            chip_type = SC6800H;
            break;
        case 0x68030000:
            chip_type = SC6800H3;
            break;
        default:
            break;
    }

    return;
}



uint32 CHIP_GetMPllClk (void)
{
    return (
               CHIP_GetAPBClk()
               / ( ( (REG32 (GR_MPLL_MN)) & 0xFFFF0000) >>16) \
               * ( (REG32 (GR_MPLL_MN)) & 0x0000FFFF));
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetArmClk (void)
{

    volatile uint32 clk_reg = REG32 (AHB_CLK_CFG0);

    if (clk_reg & BIT_0)
    {
        uint32 clk_arm_div = (clk_reg>> 1) & 0x3;
        if(SC6800H == CHIP_GetChipType())
        {
            return ( (CHIP_GetMPllClk() >> 1) / (clk_arm_div+1)) ;
        }
        else
        {
            return ( (CHIP_GetMPllClk() >> 2) / (clk_arm_div+1)) ;
        }
    }
    else
    {
        return  ARM_CLK_26M;
    }
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
    volatile uint32 reg_val = REG32 (AHB_CLK_CFG0);

    if (reg_val & BIT_0)
    {
        return (reg_val & BIT_3) ? (CHIP_GetArmClk() >> 1) : CHIP_GetArmClk();
    }
    else
    {
        return ARM_CLK_26M;
    }
}

LOCAL uint32 TF_CHIP_PHY_GetHwRstAddr (void)
{
    return GR_HWRST;
}

PUBLIC void TF_CHIP_ResetMCU (void)
{
    // This loop is very important to let the reset process work well on V3 board
    // @Richard
    uint32 i = 10000;

    WDG_TimerInit ();

    CHIP_REG_OR (GR_GEN0,  GEN0_WDG_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_WDG_RTC_EN);
    
    while (i--){}

    WDG_ResetMCU();
    
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 500)
        {
            tick2 = SCI_GetTickCount();
        }
    }
}

PUBLIC void TF_CHIP_PHY_ResetHWFlag (uint32 val)
{
    CHIP_REG_AND (TF_CHIP_PHY_GetHwRstAddr (), ~0xFFFF);
    CHIP_REG_OR (TF_CHIP_PHY_GetHwRstAddr (), (val&0xFFFF));
}

#endif
#ifdef PLATFORM_SC6530
uint32 CHIP_GetMPllClk (void)
{
    return (CHIP_GetAPBClk() / 2) * ( (* (volatile uint32 *) APB_PLL_CTL2) & 0x0000FFFF);
}
/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetArmClk (void)
{
#ifdef FPGA_VERIFICATION
    return  ARM_CLK_36M;
#else
    uint32 arm_clk = ARM_CLK_26M;
    volatile uint32 clk_reg = REG32 (APB_CLK_CFG0);

    switch ((clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT)
    {
    case 0:
        arm_clk = ARM_CLK_26M;
        break;
    case 1:
        arm_clk = ARM_CLK_208M;
        break;
    case 2:
        arm_clk = ARM_CLK_104M;
        break;
    case 3:
        arm_clk = ARM_CLK_156M;
        break;
    default:
        break;
    }

    return arm_clk;
#endif
}
uint32 CHIP_GetAhbClk (void)
{
#ifdef CHIP_VER_6531
    volatile uint32 reg_val = REG32 (APB_CLK_CFG3);

	if(reg_val & CLK_MCU_RATIO)
	{
		return ARM_CLK_104M;
	}
	else
	{
		return (CHIP_GetArmClk() >> 1);
	}
#else
    volatile uint32 reg_val = REG32 (APB_CLK_CFG0);

    return (reg_val & CLK_AHB_DIV2_EN) ? (CHIP_GetArmClk() >> 1) : CHIP_GetArmClk();

#endif
}
LOCAL uint32 TF_CHIP_PHY_GetHwRstAddr (void)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	return ANA_POR_PIN_RST_MONITOR;
#else
	return ANA_HW_RST2;
#endif
}

LOCAL void CHIP_PHY_SetHwRstFlag (uint32 val)
{
    //ANA_REG_AND (HW_RST_ADDR, ~HW_RST_MASK);
   // ANA_REG_OR (HW_RST_ADDR, (val&HW_RST_MASK));
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
   ANA_REG_AND (ANA_POR_PIN_RST_MONITOR, ~0x1F);
    ANA_REG_OR (ANA_POR_PIN_RST_MONITOR, (val&0x1F));
#else
   ANA_REG_AND (ANA_HW_RST2, ~0x1F);
    ANA_REG_OR (ANA_HW_RST2, (val&0x1F));
#endif
}

PUBLIC void TF_CHIP_PHY_ResetHWFlag (uint32 val)
{


#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    ANA_REG_AND (ANA_WDG_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_WDG_RST_MONITOR, (0x1FFF&0xFFFF));
    ANA_REG_AND (ANA_POR_PIN_RST_MONITOR, ~0xFFFF);
    ANA_REG_OR (ANA_POR_PIN_RST_MONITOR, (0x1FFF&0xFFFF));
#else
    ANA_REG_AND (ANA_HW_RST0, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST0, (0x1FFF&0xFFFF));
    ANA_REG_AND (ANA_HW_RST1, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST1, (0x1FFF&0xFFFF));
    ANA_REG_AND (ANA_HW_RST2, ~0xFFFF);
    ANA_REG_OR (ANA_HW_RST2, (0x1FFF&0xFFFF));
#endif
    CHIP_REG_AND (TF_CHIP_PHY_GetHwRstAddr (), ~HWRST1_REG_MSK);
    CHIP_REG_OR (TF_CHIP_PHY_GetHwRstAddr (), ((0x1FFF&0xFF) << HWRST1_REG_SHIFT));	
    CHIP_PHY_SetHwRstFlag (8);

}
PUBLIC void TF_CHIP_ResetMCU (void)
{
    uint32 i = 10000;

    WDG_TimerInit ();
    //CHIP_REG_OR (APB_EB0_SET,  WDG_EB_SET);
   // CHIP_REG_OR (APB_EB0_SET,  WDG_RTC_EB_SET);
    
    while (i--){}
    WDG_ResetMCU();
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();
        while ( (tick2 - tick1) < 500)
        {
            tick2 = SCI_GetTickCount();
        }
    }
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

