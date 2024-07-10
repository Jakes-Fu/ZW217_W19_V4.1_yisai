/******************************************************************************
 ** File Name:      chip_phy_v5.c                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/14/2002     Richard.Yang     Create.                                   *
 ** 09/16/2003     Xueliang.Wang    Modify CR4013                             *
 ** 08/23/2004     JImmy.Jia        Modify for SC6600D                        *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "chip_plf_export.h"
#include "chip_drvapi.h"
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
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

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
#ifdef CHIP_VER_6530
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
#elif defined(CHIP_VER_6531)
    volatile uint32 clk_reg = REG32 (APB_CLK_CFG3);

    switch ((clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT)
    {
    case 0:
        arm_clk = ARM_CLK_26M;
        break;
    case 1:
        arm_clk = ARM_CLK_208M;
        break;
    case 2:
        arm_clk = ARM_CLK_249_6M;
        break;
    case 3:
        arm_clk = ARM_CLK_312M;
        break;
    default:
        break;
    }
#elif define(CHIP_VER_6531EFM)
    volatile uint32 clk_reg = REG32(APB_CLK_CFG3);

    switch ((clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT)
    {
    //CPLL
    case 0:
        arm_clk = ARM_CLK_26M;
        break;
    case 1:
        arm_clk = ARM_CLK_104M;
        break;
    case 2:
        arm_clk = ARM_CLK_208M;
        break;
    //APLL    
    case 3:
        arm_clk = ARM_CLK_208M;
        break;
    default:
        break;
    }   
#endif
    return arm_clk;
#endif
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
#ifdef FPGA_VERIFICATION
    return  ARM_CLK_36M;
#else
#ifdef CHIP_VER_6530
    volatile uint32 reg_val = REG32 (APB_CLK_CFG0);

    return (reg_val & CLK_AHB_DIV2_EN) ? (CHIP_GetArmClk() >> 1) : CHIP_GetArmClk();
#elif defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
    volatile uint32 reg_val = REG32 (APB_CLK_CFG3);

    return (reg_val & CLK_MCU_RATIO) ? ARM_CLK_104M : (CHIP_GetArmClk() >> 1);

#endif
#endif
}



PUBLIC void CHIP_PHY_MCU_CLK_Init(void)
{
#if (defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM))
    uint32 j;

    j = REG32 (APB_CLK_CFG3);

    //set ARM clk and AHB clk ratio to be 2
    j &=  ~CLK_MCU_RATIO;
    REG32 (APB_CLK_CFG3) = j;

    //set CLK_ARM to 312MHz
    j &= ~ (CLK_MCU_SEL_MSK);      
    j |= 0x3 << CLK_MCU_SEL_SHIFT;        

    REG32 (APB_CLK_CFG3) = j;

    for (j=0; j<100; j++) {}

    /*SFC CLK init*/

    {
        j = REG32 (APB_CLK_CFG1);

        j &=  ~CLK_SFC_SEL_MSK;
        j |= 0x1 << CLK_SFC_SEL_SHIFT;  
        REG32 (APB_CLK_CFG1) = j;

        for (j=0; j<100; j++) {}
    }
#endif
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
