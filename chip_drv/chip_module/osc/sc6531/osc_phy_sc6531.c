/******************************************************************************
 ** File Name:      osc_phy_v0.c                                                 *
 ** Author:         kui.wang                                                    *
 ** DATE:           08/21/2012                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                 Edit          History                                     *
 **---------------------------------------------------------------------------*
 ** DATE            NAME          DESCRIPTION                                 *
 **                                                                           *
 ******************************************************************************/
#include "sci_types.h"
#include "os_api.h"
#include "osc_drvapi.h"
#include "chip_plf_export.h"
#include "osc_reg_sc6531.h"
#include "adi_hal_internal.h"

#define MIN_CAL_COUNT               0x00000400
#define MAX_CAL_COUNT               0x00400000
#define OSC_REG_GET(addr)           ANA_REG_GET(addr)
#define OSC_REG_SET(addr,value)     ANA_REG_SET(addr,value)
#define OSC_REG_AND(addr,value)     ANA_REG_AND(addr,value)
#define OSC_REG_OR(addr,value)      ANA_REG_OR(addr,value)
/*****************************************************************************/
//  Description:    This function for osc delay
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OSC_Delay (uint32 ms)
{
    uint32 cur_time = SCI_GetTickCount();

    while ( (SCI_GetTickCount() - cur_time) < ms)
    {
    }
}
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableClkSource (CLOCK_SOURCE_E clk_src,BOOLEAN isEnable)
{
	if(isEnable)
	{
		switch(clk_src)
		{
			case CLOCK_SELF_32K:
				OSC_REG_AND(ANA_MISC_CTL8,~CLK_SELF_EN_CLR);//使能SELF 32K clock
				OSC_Delay(3);
				OSC_REG_OR(ANA_MISC_CTL8,CLK_SELF_EN_SET);
				OSC_Delay(3); 		
				break;
			case CLOCK_DCXO_32K:
				OSC_REG_AND(ANA_LDO_PD_SET0,~LDO_ABB_PD);		//打开DCXO 32K LDO
				OSC_Delay(3);
				OSC_REG_OR(ANA_LDO_PD_RST0,LDO_ABB_PD_RST);
				OSC_Delay(3);
				OSC_REG_AND(ANA_LDO_PD_SET0,~DCXO_PD_SET);
				OSC_Delay(3);
				OSC_REG_OR(ANA_LDO_PD_RST0,DCXO_PD_RST);
				OSC_Delay(3);
				break;						
			case CLOCK_EXTE_32K:
				OSC_REG_AND(ANA_XTL_CTL1,~OSC32K_PD_SET);//open external 32k crystal
				OSC_Delay(3);
				OSC_REG_OR(ANA_XTL_CTL1,OSC32K_PD_CLR);
				OSC_Delay(3);
				break;
			default:
				break;	
		}			
	}
	else
	{
		switch(clk_src)
		{
			case CLOCK_SELF_32K:
				OSC_REG_AND(ANA_MISC_CTL8,~CLK_SELF_EN_SET);//close self 32k clock
				OSC_Delay(3);
				OSC_REG_OR(ANA_MISC_CTL8,CLK_SELF_EN_CLR);
				OSC_Delay(3);
				break;
			case CLOCK_DCXO_32K:
				OSC_REG_AND(ANA_LDO_PD_RST0,~LDO_ABB_PD_RST);//close DCXO 32k LDO
				OSC_Delay(3);
				OSC_REG_OR(ANA_LDO_PD_SET0,LDO_ABB_PD);
				OSC_Delay(3);
				OSC_REG_AND(ANA_LDO_PD_RST0,~DCXO_PD_RST);
				OSC_Delay(3);
				OSC_REG_OR(ANA_LDO_PD_SET0,DCXO_PD_SET);
				OSC_Delay(3);
				break;						
			case CLOCK_EXTE_32K:
				OSC_REG_AND(ANA_XTL_CTL1,~OSC32K_PD_CLR);//close external 32k clock
				OSC_Delay(3);
				OSC_REG_OR(ANA_XTL_CTL1,OSC32K_PD_SET);
				OSC_Delay(3);
				break;
			default:
				break;	
		}		
	}
	//OSC_REG_OR(ANA_MISC_CTL9,CLK_32K_OUT_EN);
}
/*****************************************************************************/
//  Description:    This function is get current use source 32k clock
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_SOURCE_E OSC_PHY_GetCurrentClkSource (void)
{
    uint16 src_sel[4];
    src_sel[0] = (uint16)(OSC_REG_GET(ANA_MISC_CTL8) & CLK_SELF_SEL_SET);
    src_sel[1] = (uint16)(OSC_REG_GET(ANA_MISC_CTL8) & CLK_SELF_SEL_CLR);
    src_sel[2] = (uint16)(OSC_REG_GET(ANA_MISC_CTL9) & CLK32K_EXT_SEL_SET);
    src_sel[3] = (uint16)(OSC_REG_GET(ANA_MISC_CTL9) & CLK32K_EXT_SEL_CLR);

    if (src_sel[1])
    {
        if (src_sel[3])
        {
            return CLOCK_DCXO_32K;
        }
        else
        {
            if (src_sel[2])
            {
                return CLOCK_EXTE_32K;
            }
            else
            {
                return CLOCK_DCXO_32K;
            }
        }
    }
    else
    {
        if (src_sel[0])
        {
            if (src_sel[3])
            {
                return CLOCK_SELF_32K;
            }
            else
            {
                if (src_sel[2])
                {
                    return CLOCK_EXTE_32K;
                }
                else
                {
                    return CLOCK_SELF_32K;
                }
            }
        }
        else
        {
            if (src_sel[3])
            {
                return CLOCK_DCXO_32K;
            }
            else
            {
                if (src_sel[2])
                {
                    return CLOCK_EXTE_32K;
                }
                else
                {
                    return CLOCK_DCXO_32K;
                }
            }
        }
    }
}
/*****************************************************************************/
//  Description:    This function is switch clock 32k source
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SwitchClkSource (CLOCK_SOURCE_E clk_src)
{
	switch(clk_src)
	{
		case CLOCK_SELF_32K:
			OSC_REG_AND(ANA_MISC_CTL8,~CLK_SELF_SEL_CLR);//用K2开关选择self 32k
			OSC_Delay(3);
			OSC_REG_OR(ANA_MISC_CTL8,CLK_SELF_SEL_SET);
			OSC_Delay(3);
			OSC_REG_AND(ANA_MISC_CTL9,~CLK32K_EXT_SEL_SET);//用K3开关来控制不选择external 32k
			OSC_Delay(3);
			OSC_REG_OR(ANA_MISC_CTL9,CLK32K_EXT_SEL_CLR);
			OSC_Delay(3);			
			break;
		case CLOCK_DCXO_32K:
			OSC_REG_AND(ANA_MISC_CTL8,~CLK_SELF_SEL_SET);//用K2开关选择 DCXO 32k
			OSC_Delay(3);
			OSC_REG_OR(ANA_MISC_CTL8,CLK_SELF_SEL_CLR);
			OSC_Delay(3);
			OSC_REG_AND(ANA_MISC_CTL9,~CLK32K_EXT_SEL_SET);//用K3开关控制不选择external 32k
			OSC_Delay(3);
			OSC_REG_OR(ANA_MISC_CTL9,CLK32K_EXT_SEL_CLR);
			OSC_Delay(3);
			break;						
		case CLOCK_EXTE_32K:
			OSC_REG_AND(ANA_MISC_CTL9,~CLK32K_EXT_SEL_CLR);//用K3开关选择external 32k 
			OSC_Delay(3);
			OSC_REG_OR(ANA_MISC_CTL9,CLK32K_EXT_SEL_SET);
			OSC_Delay(3);
			break;
		default:
			break;	
	}	
}
/*****************************************************************************/
//  Description:    This function is can let dcxo go into low current mode
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_DCXO_ChangeMode (DCXO_WORKMODE_E dcxo_mode)
{
    switch(dcxo_mode)
    {
        case NORMALCURRENT_MODE:
            OSC_REG_OR(ANA_LDO_PD_RST0,DCXO_FRC_LOW_CURR_MODE_RST);
            break;
        case LOWCURRENT_MODE:
            OSC_REG_OR(ANA_LDO_PD_SET0,DCXO_FRC_LOW_CURR_MODE_SET);
            break;
        default:
            break;
    }

    OSC_Delay (3);
}
/*****************************************************************************/
//  Description:    This function is control self 32k is master by arm or dsp
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_SwitchMaster (OSC_MASTER_E mastermcu)
{
    switch (mastermcu)
    {
        case MASTER_ARM:
            OSC_REG_AND (ANA_MISC_CTL9,~SELF_OSC_CAL_MASTER_SEL);
            break;
        case MASTER_DSP:
            OSC_REG_OR (ANA_MISC_CTL9,SELF_OSC_CAL_MASTER_SEL);
            break;
        default:
            break;
    }

    OSC_Delay (3);
}
/*****************************************************************************/
//  Description:    This function is Enable calibrate self 32k
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_CaliEnable (BOOLEAN isEnable)
{
    if (isEnable)
    {
        OSC_REG_OR (ANA_MISC_CTL9,SELF_OSC_CAL_EB);
        OSC_Delay (3);
        OSC_REG_OR (ANA_MISC_CTL9,CLK_SELF_CAL_26M_EN);
        OSC_Delay (3);
        OSC_REG_AND (ANA_MISC_CTL9,~SELF_OSC_CAL_SOFT_RST);
        OSC_Delay (3);
    }
    else
    {
        OSC_REG_AND (ANA_MISC_CTL9,~SELF_OSC_CAL_EB);
        OSC_Delay (3);
        OSC_REG_AND (ANA_MISC_CTL9,~CLK_SELF_CAL_26M_EN);
        OSC_Delay (3);
        OSC_REG_OR (ANA_MISC_CTL9,SELF_OSC_CAL_SOFT_RST);
        OSC_Delay (3);
    }
}
/*****************************************************************************/
//  Description:    This function is set use how many count to count 26M
//  Global resource dependence:
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_SetCaliCycle (uint32 cycleCount)
{
    OSC_REG_SET (OSC_CAL_CYCLE_P1, ( (cycleCount & CAL_CYCLE_P1_MSK) >> CAL_CYCLE_P1_SHIFT));
    OSC_Delay (3);
    OSC_REG_SET (OSC_CAL_CYCLE_P0, (cycleCount & CAL_CYCLE_P0_MSK));
    OSC_Delay (3);
    OSC_REG_SET (OSC_CAL_START,CAL_START_BUSY);
    OSC_Delay (3);
}
/*****************************************************************************/
//  Description:    This function is get result of calibration
//  Global resource dependence:
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 OSC_PHY_SELF_GetCaliResult (void)
{
    uint32 cal_result = 0x0;

    while (OSC_REG_GET (OSC_CAL_START) & CAL_START_BUSY)
    {
    }

    cal_result = OSC_REG_GET (OSC_CAL_RESULT_P1);
    cal_result <<= CAL_RESULT_P1_SHIFT;
    cal_result |= OSC_REG_GET (OSC_CAL_RESULT_P0);

    return cal_result;
}
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence:
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_GetDivValue (uint32 m,uint32 n,DIV_PART_T *divider)
{
    uint32 num,dec,i,mod,temp_value = n,cnt = 0x0;

    SCI_PASSERT ( (n >= MIN_CAL_COUNT && n <= MAX_CAL_COUNT), ("cali value must be in range"));/*assert verified*/

    for (i=0;; i++)
    {
        temp_value >>= 1;

        if (!temp_value)
        {
            break;
        }

        cnt++;
    }

    num = 13*125*125* (1 << (cnt + 7 - 15));
    dec = m;

    divider->integer = num/dec - 1;

    mod = num - (divider->integer + 1) * dec;

    divider->fraction = 0x0;

    for (i=0; i<20; i++)
    {
        if ( (mod << 1) > m)
        {
            divider->fraction = (divider->fraction << 1) + 1;
            mod = (mod << 1) - m;

            if (mod == 0)
            {
                break;
            }
        }
        else
        {
            divider->fraction = (divider->fraction << 1) + 0;
            mod = mod << 1;
        }
    }

}
/*****************************************************************************/
//  Description:    This function is Enable Source 32k clock
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_SELF_SetDivIF (DIV_PART_T *divider)
{
    while (OSC_REG_GET (OSC_DIV_FAC_UPD) & OSC_DIV_FAC_UPD_EN)
    {
    }

    OSC_REG_AND (OSC_DIV_FAC_P1,~ (OSC_DIV_FAC_INT_MSK | OSC_DIV_FAC_FRAC_P1_MSK));
    OSC_Delay (3);
    OSC_REG_AND (OSC_DIV_FAC_P0,~OSC_DIV_FAC_FRAC_P0_MSK);
    OSC_Delay (3);
    OSC_REG_OR (OSC_DIV_FAC_P1, ( ( (divider->integer) << OSC_DIV_FAC_INT_SHIFT) & OSC_DIV_FAC_INT_MSK));
    OSC_Delay (3);
    OSC_REG_OR (OSC_DIV_FAC_P1, ( (divider->fraction >> OSC_DIV_FAC_FRAC_P1_SHIFT) & OSC_DIV_FAC_FRAC_P1_MSK));
    OSC_Delay (3);
    OSC_REG_OR (OSC_DIV_FAC_P0, (divider->fraction & OSC_DIV_FAC_FRAC_P0_MSK));
    OSC_Delay (3);
    OSC_REG_SET (OSC_DIV_FAC_UPD,OSC_DIV_FAC_UPD_EN);
    OSC_Delay (3);

    while (OSC_REG_GET (OSC_DIV_FAC_UPD) & OSC_DIV_FAC_UPD_EN)
    {
    }
}
/*****************************************************************************/
//  Description:    This function is Enable DCXO auto power down
//  Global resource dependence: analog global register
//
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableDcxoAutoPD (BOOLEAN isEnable)
{    
    OSC_REG_OR (ANA_MISC_CTL2,DCXO_MODE_SWT2);
    OSC_REG_AND (ANA_MISC_CTL2,~DCXO_MODE_SWT1);

    if (isEnable)
    {
        OSC_REG_OR (ANA_MISC_CTL9,DCXO_SLP_AUTO_PD_EN);
        OSC_Delay (3);
        OSC_REG_OR (ANA_SLP_LDO_CTL3,XTL_LDOABB_PD_EN);
    }
    else
    {
        OSC_REG_AND (ANA_MISC_CTL9,~DCXO_SLP_AUTO_PD_EN);
        OSC_Delay (3);
        OSC_REG_AND (ANA_SLP_LDO_CTL3,~XTL_LDOABB_PD_EN);
    }

    OSC_Delay (3);
}

/*****************************************************************************/
//  Description:    This function is Enable REF auto sleep
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableRefAutoSleep(DCXO_REF_E dcxo_ref,BOOLEAN isEnable)
{
	if(isEnable)
	{
		switch(dcxo_ref)
		{
			case DCXO_REF1:
				OSC_REG_OR(ANA_MISC_CTL8,DCXO_REF1_AUTO_SLP_EN);
				break;
			case DCXO_REF2:
				OSC_REG_OR(ANA_MISC_CTL8,DCXO_REF2_AUTO_SLP_EN);
				break;
			default:
				break;
		}
	}
	else
	{
		switch(dcxo_ref)
		{
			case DCXO_REF1:
				OSC_REG_AND(ANA_MISC_CTL8,~DCXO_REF1_AUTO_SLP_EN);
				break;
			case DCXO_REF2:
				OSC_REG_AND(ANA_MISC_CTL8,~DCXO_REF2_AUTO_SLP_EN);
				break;
			default:
				break;
		}		
	}
}

/*****************************************************************************/
//  Description:    This function is Enable low power of clock 32k
//  Global resource dependence: analog global register                
//                  
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void OSC_PHY_EnableLowPowerClock32k(BOOLEAN isEnable)
{
	//dummy
}