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
#include "osc_reg_uws6121e.h"
#include "adi_hal_internal.h"

#define MIN_CAL_COUNT               0x00000400
#define MAX_CAL_COUNT               0x00400000
#define OSC_REG_GET(addr)           ANA_REG_GET(addr)
#define OSC_REG_SET(addr,value)     ANA_REG_SET(addr,value)
#define OSC_REG_AND(addr,value)     ANA_REG_AND(addr,value)
#define OSC_REG_OR(addr,value)      ANA_REG_OR(addr,value)

#define OSC_CAL_DEBUG
#ifdef OSC_CAL_DEBUG
PUBLIC uint32 s_osc_cal_int_flag = 0;
PUBLIC uint32 s_osc_cal_start_flag = 0;
PUBLIC uint32 s_osc_cal_start_step1_tick = 0;
PUBLIC uint32 s_osc_cal_start_step2_tick = 0;
PUBLIC uint32 s_osc_cal_end_step1_tick = 0;
PUBLIC uint32 s_osc_cal_end_step2_tick = 0;
#endif
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

    while ( (SCI_GetTickCount() - cur_time) < ms);
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
				OSC_REG_OR(ANA_CLK32KLESS_CTRL0,RC_32K_EN);//enable SELF 32K clock
				OSC_Delay(3);			
				break;
			case CLOCK_DCXO_32K:
				OSC_REG_AND(ANA_POWER_PD_SW0,~LDO_DCXO_PD);	//open DCXO 32K LDO
				OSC_Delay(3);
				break;						
			case CLOCK_EXTE_32K:
				SCI_ASSERT(0);//do not support external 32k crystal
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
				OSC_REG_AND(ANA_CLK32KLESS_CTRL0,~RC_32K_EN);//disable SELF 32K clock
				OSC_Delay(3);
				break;
			case CLOCK_DCXO_32K:
				OSC_REG_OR(ANA_POWER_PD_SW0,LDO_DCXO_PD);	//disable DCXO 32K LDO
				OSC_Delay(3);
				break;						
			case CLOCK_EXTE_32K:
				SCI_ASSERT(0);//do not support external 32k crystal
			default:
				break;	
		}		
	}
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
    uint16 src_sel;
    src_sel = (uint16)(OSC_REG_GET(ANA_CLK32KLESS_CTRL0) & RC_32K_SEL);

    if (src_sel !=0 )
    {
          return CLOCK_SELF_32K;
    }
    else
    {
          return CLOCK_DCXO_32K;
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
			OSC_REG_OR(ANA_CLK32KLESS_CTRL0,RC_32K_SEL);//32K clock From RC
			OSC_Delay(3);				
			break;
		case CLOCK_DCXO_32K:
			OSC_REG_AND(ANA_CLK32KLESS_CTRL0,~RC_32K_SEL);//32K clock From XO
			OSC_Delay(3);
			break;						
		case CLOCK_EXTE_32K:
			SCI_ASSERT(0);//do not support external 32k crystal
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
		OSC_REG_AND(ANA_CLK32KLESS_CTRL0,~XO_LOW_CUR_EN);
            break;
        case LOWCURRENT_MODE:
            OSC_REG_OR(ANA_CLK32KLESS_CTRL0,XO_LOW_CUR_EN);
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
        OSC_REG_OR (ANA_MODULE_EN0,ANA_CAL_EN);
        OSC_Delay (3);
        OSC_REG_OR (ANA_DIG_CLK_EN0,ANA_CLK_CAL_EN);
        OSC_Delay (3);
//        OSC_REG_OR (ANA_XTL_WAIT_CTRL,XTL_EN);
//        OSC_Delay (3);
    }
    else
    {
        OSC_REG_AND (ANA_MODULE_EN0,~ANA_CAL_EN);
        OSC_Delay (3);
        OSC_REG_AND (ANA_DIG_CLK_EN0,~ANA_CLK_CAL_EN);
        OSC_Delay (3);
 //       OSC_REG_AND (ANA_XTL_WAIT_CTRL,~XTL_EN);
 //       OSC_Delay (3);
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
#ifdef OSC_CAL_DEBUG
    s_osc_cal_start_step1_tick = SCI_GetTickCount();
#endif
    OSC_REG_SET (OSC_CAL_CYCLE_P1, ( (cycleCount & CAL_CYCLE_P1_MSK) >> CAL_CYCLE_P1_SHIFT));
    OSC_Delay (3);
    OSC_REG_SET (OSC_CAL_CYCLE_P0, (cycleCount & CAL_CYCLE_P0_MSK));
    OSC_Delay (3);
    OSC_REG_SET (OSC_DIV_INT_CLR, (OSC_CAL_DONE_INT_CLR_MASK|OSC_FAC_UPD_DONE_INT_CLR_MASK));
    OSC_Delay (3);
    OSC_REG_SET (OSC_CAL_START,CAL_START_BUSY);
    OSC_Delay (20);
#ifdef OSC_CAL_DEBUG
    s_osc_cal_start_step2_tick = SCI_GetTickCount();
#endif
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

    while ( !(OSC_REG_GET (OSC_DIV_INT_RAW) & OSC_CAL_DONE_INT_RAW_MASK) )
    {
#ifdef OSC_CAL_DEBUG
        s_osc_cal_int_flag = OSC_REG_GET (OSC_DIV_INT_RAW);
        s_osc_cal_start_flag = OSC_REG_GET (OSC_CAL_START);
#endif   
    }

#ifdef OSC_CAL_DEBUG
    s_osc_cal_end_step1_tick = SCI_GetTickCount();
#endif

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
    uint32 num,dec,i,mod,temp_value = n+1,cnt = 0x0;

    //SCI_PASSERT ( (n >= MIN_CAL_COUNT && n <= MAX_CAL_COUNT), ("cali value must be in range"));/*assert verified*/

    for (i=0;; i++)
    {
        temp_value >>= 1;

        if (!temp_value)
        {
            break;
        }

        cnt++;
    }

    num = 13*125*125* (1 << (cnt + 7 - 18));
    dec = m;

    divider->integer = num/dec;

    mod = num - (divider->integer) * dec;

    divider->fraction = 0x0;

    for (i=0; i<16; i++)
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
    divider->integer -=1;
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
    while (OSC_REG_GET (OSC_DIV_FAC_UPD) & OSC_DIV_FAC_UPD_EN);

    OSC_REG_AND (OSC_DIV_FAC_P1,~OSC_DIV_FAC_INT_MSK);
    OSC_Delay (3);
    OSC_REG_AND (OSC_DIV_FAC_P0,~OSC_DIV_FAC_FRAC_MSK);
    OSC_Delay (3);
    OSC_REG_OR (OSC_DIV_FAC_P1, (divider->integer & OSC_DIV_FAC_INT_MSK));
    OSC_Delay (3);
    OSC_REG_OR (OSC_DIV_FAC_P0, (divider->fraction & OSC_DIV_FAC_FRAC_MSK));
    OSC_Delay (3);
    OSC_REG_SET (OSC_DIV_FAC_UPD,OSC_DIV_FAC_UPD_EN);
    OSC_Delay (3);

    while (OSC_REG_GET (OSC_DIV_FAC_UPD) & OSC_DIV_FAC_UPD_EN);

#ifdef OSC_CAL_DEBUG
    s_osc_cal_end_step2_tick = SCI_GetTickCount();
#endif
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
    if (isEnable)
    {
        OSC_REG_OR (ANA_SLP_LDO_PD_CTRL0,SLP_LDODCXO_PD_EN);
        OSC_Delay (3);
    }
    else
    {
        OSC_REG_AND (ANA_SLP_LDO_PD_CTRL0,~SLP_LDODCXO_PD_EN);
        OSC_Delay (3);
   }
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
/*
    if (isEnable)
    {
		OSC_REG_AND (ANA_LOW_PWR_CLK32K_CTRL,~ (TIME_BETWEEN_CALIBRATION_MSK | TIME_FOR_CALIBRATION_MSK | TIME_FOR_DCXO_STABLE_MSK));
		OSC_Delay (3);
		OSC_REG_OR (ANA_LOW_PWR_CLK32K_CTRL, ( ( (0x5) << TIME_BETWEEN_CALIBRATION_SHIFT) & TIME_BETWEEN_CALIBRATION_MSK));//25S
		OSC_Delay (3);
		OSC_REG_OR (ANA_LOW_PWR_CLK32K_CTRL, ( ( (0x4) << TIME_FOR_CALIBRATION_SHIFT) & TIME_FOR_CALIBRATION_MSK));//4//480ms
		OSC_Delay (3);
		OSC_REG_OR (ANA_LOW_PWR_CLK32K_CTRL, ( ( (0x3) << TIME_FOR_DCXO_STABLE_SHIFT) & TIME_FOR_DCXO_STABLE_MSK));//8ms
		OSC_Delay (3);
		OSC_REG_OR (ANA_LOW_PWR_CLK32K_CTRL,(DCXO_EXTRA_LOW_CUR_MODE_EN|DCXO_LOW_CUR_MODE_EN|LDO_DCXO_LP_AUTO_EN|LOW_PWR_CLK32K_EN));
		OSC_Delay (3);
    }
    else
    {
		OSC_REG_AND (ANA_LOW_PWR_CLK32K_CTRL,~(DCXO_EXTRA_LOW_CUR_MODE_EN|DCXO_LOW_CUR_MODE_EN|LDO_DCXO_LP_AUTO_EN|LOW_PWR_CLK32K_EN));
		OSC_Delay (3);
   }
	*/
}

