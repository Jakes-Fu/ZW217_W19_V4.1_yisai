/******************************************************************************
 ** File Name:      chip_phy_sc6531efm.c                                          *
 ** Author:                                                                   *
 ** DATE:           01/27/2016                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/27/2016                      Create.                                   *
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
#define ADI_ARMREG_ADDR_MASK (0xFFF)//[11:0]
#define ADI_ARMREG_FLAG_MASK (0<<12)

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void)
{
    uint32 id0 =  REG32 (AON_PROJ_ID0);//0x3632_0000->62
    uint32 id1 =  REG32 (AON_PROJ_ID1);//0x5343_3635->SC65
    uint32 id2 =  REG32 (AON_VER_ID);//0x0 0r 0x1
    uint32 chip_id = 0;
    CHIP_TYPE_E chip_type = CHIP_TYPE_MAX;

    id1 = ((id1 & 0x0F0F) & 0xFF) |((((id1 & 0x0F0F) & 0xFF00) >> 8)<<4);//0x65
    id0 = (((id0 & 0x0F0F0000)>>16) & 0xFF) |(((((id0 & 0x0F0F0000)>>16) & 0xFF00) >> 8)<<4);//0x62
    chip_id = (((id1 << 8) + id0)<<16) + id2;//0x65620000 or 0x65620001

    switch (chip_id)
    {
        case 0x65300000:
        case 0x6530C000:
            chip_type = SC6530;
            break;

        case 0x65310000:
		case 0x65310001:	
            chip_type = SC6531;
            break;

        case 0x65620000:
            chip_type = SC6531EFM;
            break;

        case 0x65620001:
            chip_type = SC6531EFM_AB;
            break;

        default:
#ifdef FPGA_SUPPORT_SC6531EFM
            chip_type = SC6531EFM;
#endif
            break;
    }
	
    return chip_type;
}

/*****************************************************************************/
// Description :    This function is used get the chip type ex
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_EX_E CHIP_GetChipTypeEx (void)
{
    uint32 chip_id_ex = 0;
    uint32 chip_id_ex_temp = 0;
    CHIP_TYPE_EX_E chip_type_ex = CHIP_TYPE_EX_MAX;

    chip_id_ex = ((*(volatile uint32 *)0x205000E0 & 0x1) ? *(volatile uint32 *)0x00015FFC : *(volatile uint32 *)0x10015FFC);//rom space last word
    chip_id_ex_temp = chip_id_ex & (BIT_1|BIT_0);
    switch (chip_id_ex_temp)
    {
        case 0x0:
            chip_type_ex = CHIP_TYPE_EX_AA_AB;
            break;
        case 0x3:
            chip_type_ex = CHIP_TYPE_EX_AC;
            break;
        default:
            chip_type_ex = CHIP_TYPE_EX_MAX;
            break;
    }

    return chip_type_ex;
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
uint32 CHIP_GetArmClk (void)
{
#ifdef FPGA_VERIFICATION
    return  ARM_CLK_36M;
#else
    uint32 arm_clk = ARM_CLK_26M;

    volatile uint32 clk_reg = REG32 (APB_CLK_CFG3);

    switch ((clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT)
    {
    case 0:
        arm_clk = ARM_CLK_26M;
        break;
    case 1:
        arm_clk = ARM_CLK_104M;
        break;
    case 2:
        arm_clk = ARM_CLK_208M;//CPLL
        break;
    case 3:
        arm_clk = ARM_CLK_208M;//APLL
        break;
    default:
        break;
    }	

    return arm_clk;
#endif
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void)
{
#ifdef FPGA_VERIFICATION
    return  ARM_CLK_36M;
#else
	volatile uint32 reg_val = REG32 (APB_CLK_CFG3);
	return (reg_val & CLK_MCU_RATIO) ? ARM_CLK_104M : (CHIP_GetArmClk() >> 1);
#endif
}

/*****************************************************************************/
// Description :    This function is used to init MCU clock.
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
PUBLIC void CHIP_PHY_MCU_CLK_Init(void)
{
    uint32 j;

    j = REG32 (APB_CLK_CFG3);

    //set ARM clk and AHB clk , don't gate.
    j &=  ~CLK_MCU_RATIO;
    REG32 (APB_CLK_CFG3) = j;

    //set CLK_ARM to 208MHz
    j &= ~ (CLK_MCU_SEL_MSK);      
    j |= 0x3 << CLK_MCU_SEL_SHIFT;//0x0:26m, 0x1:104m_cpll, 0x2:208m_cpll, 0x3:208m_apll         

    REG32 (APB_CLK_CFG3) = j;

    for (j=0; j<100; j++) {}

    //set SFC CLK to 104MHz
    j = REG32 (CGM_SFC_2X_CFG);
    j &=  ~CGM_SFC_2X_SEL_MSK;
    j |= 0x2 << CGM_SFC_2X_SEL_SHIFT;//0x2:156m_apll, 0x3:166m4_cpll, 0x4:208m_apll, 0x5:208m_cpll   
    REG32 (CGM_SFC_2X_CFG) = j;

    j = REG32 (CGM_SFC_1X_CFG);
    j &=  ~CGM_SFC_1X_DIV_MSK;
    j |= 0x1 << CGM_SFC_1X_DIV_SHIFT;  
    REG32 (CGM_SFC_1X_CFG) = j;

    for (j=0; j<100; j++) {}
	
}

/*****************************************************************************/
// Description :    This function is used to init chip.
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
PUBLIC void Chip_Init(void)
{
    CHIP_PHY_MCU_CLK_Init();
    
    //(*(volatile uint32 *)0x20500080) = 0xfffff;
	
    SMC_Init();

}

/*****************************************************************************/
//  Description:    The function is used to get the Init ADI.
//  Global resource dependence:
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC void CHIP_InitAdi (void)
{
	uint32 i = 100;
	
	//enable adi module
	*(volatile uint32 *)APB_EB0_SET = ADI_EB;

	//reset adi module
	*(volatile uint32 *)APB_RST0_SET = ADI_SOFT_RST;

	while(i--);
	
	*(volatile uint32 *)APB_RST0_CLR = ADI_SOFT_RST;

	i = 100;
	while(i--);
}

PUBLIC uint32 CHIP_PHY_GetANAReg (uint32 addr)
{
    uint32 adi_rd_data;

    // Set read command
    * (volatile uint32 *) ADI_ARM_RD_CMD = ( (addr) &ADI_ARMREG_ADDR_MASK) |
                                           ADI_ARMREG_FLAG_MASK;

    // Wait read operation complete, RD_data[31] will be cleared after the read
    // operation complete
    do
    {
        adi_rd_data = * (volatile uint32 *) ADI_ARM_RD_DATA;
    }
    while (adi_rd_data & BIT_31);

    return ( (adi_rd_data & 0x0000FFFF));
}

PUBLIC void CHIP_PHY_SetANAReg (uint32 addr, uint32 value)
{
    for (;;)             ////ADI_wait_fifo_empty
    {
        if ( ( (* (volatile uint32 *) ADI_ARM_STS) & ADI_STS_FIFO_FULL_MASK)  == 0)
        {
            break;
        }
    }

    CHIP_REG_SET (addr, value);

	while(((*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_EMPTY_MASK) == 0)
	{
		;
	}
}

PUBLIC void CHIP_LDOSFPDEnAfterRst(void)
{
	uint16 tmp_val = 0;

	//software reset ldo sf power down enable
	tmp_val = CHIP_PHY_GetANAReg(ANA_SWRST_CTRL1);
	tmp_val |= BIT_2;
	CHIP_PHY_SetANAReg((ANA_SWRST_CTRL1), tmp_val);
	
	tmp_val = CHIP_PHY_GetANAReg(ANA_SWRST_CTRL0);
	tmp_val |= (BIT_6 | BIT_9 | BIT_10);
	CHIP_PHY_SetANAReg((ANA_SWRST_CTRL0), tmp_val);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
