/******************************************************************************
 ** File Name:      ldo_drv.c                                             *
 ** Author:         Yi.Qiu                                                 *
 ** DATE:           01/09/2009                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/09/2009     Yi.Qiu        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "adc_drvapi.h"
#include "ldo_drvapi.h"
#include "ldo_cfg.h"
#include "adi_hal_internal.h"
#include "efuse_drvapi.h"

#include "uws6121e_reg_analog.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Local variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint8 s_vcam_count;/*lint -esym(551,s_vcam_count)*/
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

LDO_CTL_PTR g_ldo_ctl_tab = NULL;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define LDO_VOLT_MIN    1200
#define LDO_VOLT_MAX    3300
#define DELAY_10US 2000  // 204MHz 2000cycles
#define LDO_CAL_DEBUG 0

LOCAL CAL_LDO_PTR Cal_GetLdoCtl (LDO_ID_E ldo_id);

#if defined(LDO_CALIBRATION_SUPPORT)
LOCAL void __calibrate_ldo(CAL_LDO_PTR cal_ldo_tab);
LOCAL BOOLEAN adc_cali_build = SCI_FALSE;
#endif
#endif

/**---------------------------------------------------------------------------*
 **                         Function Declaration                              *
 **---------------------------------------------------------------------------*/

void LCM_Backlight_turnon()
{
    ANA_REG_OR(0xC08, 0x200);    /*enable RTLC clk*/
    ANA_REG_SET(0x1D8, 0x0);    /*exit sleep mode*/
    ANA_REG_SET(0x1B8, 0x20);
    ANA_REG_SET(0x1BC, 0x20);
    ANA_REG_SET(0x1C0, 0x20);
    ANA_REG_SET(0x1C4, 0x20);
    ANA_REG_SET(0x180, 0xCCCC);
}

#if defined(PLATFORM_UWS6121E)
#if 0
void LDO_DcdcWpaSet(uint8 on_off, uint8 apc_sel, uint8 apc_ramp_sel, uint16 vol_mv)
{
    uint32 wpa_reg1_apc, wpa_reg2_on;
    uint32 wpa_vol;
    wpa_reg1_apc = ANA_REG_GET(ANA_DCDC_WPA_REG1);
    wpa_reg2_on = ANA_REG_GET(ANA_DCDC_WPA_REG2);
    if (0 == apc_sel)
    {
        wpa_reg1_apc &= ~BIT_7;
        if (1 == on_off)
        {
            wpa_vol = (vol_mv - 400) / 25;
            ANA_REG_SET(ANA_DCDC_WPA_VOL, wpa_vol);
        }
    }
    else
    {
        wpa_reg1_apc |= BIT_7;
        wpa_reg1_apc = (apc_ramp_sel == 0) ? (wpa_reg1_apc & (~BIT_6)) : (wpa_reg1_apc | BIT_6);
    }
    if (0 == on_off)
    {
        wpa_reg2_on |= BIT_0;
    }
    else
    {
        wpa_reg2_on &= ~BIT_0;
    }
    ANA_REG_SET(ANA_DCDC_WPA_REG1, wpa_reg1_apc);
    ANA_REG_SET(ANA_DCDC_WPA_REG2, wpa_reg2_on);
}

#endif

void LDO_TestInit(void)
{


    LDO_TurnOnLDO(LDO_LDO_LCD);
    LDO_TurnOffLDO(LDO_LDO_SD);   /*need to close,openned when powerup*/
#if !defined(HW_6121_pre_w217)
    LDO_TurnOffLDO(LDO_LDO_IO33);
#endif

	//LDO_TurnOnLDO(LDO_LDO_VDD18);
	LDO_TurnOnLDO(LDO_LDO_IO18);

#if !defined(MEM_TYPE_DDR)
//  LDO_TurnOffLDO(LDO_LDO_DDR12);
#endif


}

void LDO_DcdcCoreDeepSleepSet(void)
{
    ANA_REG_OR(ANA_SLP_DCDC_PD_CTRL, 0x8);
    ANA_REG_SET(ANA_DCDC_CORE_SLP_CTRL0, (LDO_CORE_SLP_IN_STEP_EN | (0x10 << LDO_CORE_SLP_STEP_VOL_SHIFT) | (3 << LDO_CORE_SLP_STEP_NUM_SHIFT) | (0x3 << LDO_CORE_SLP_STEP_DELAY_SHIFT)));


    ANA_REG_SET(ANA_DCDC_CORE_SLP_CTRL1, (0xE0 << LDO_CORE_V_DS_SHIFT));
    ANA_REG_OR(ANA_DCDC_VLG_SEL, 0x3);
}
#endif

void halPmuSwitchPower(uint32 id, uint32 enabled, uint32 lp_enabled)
{
    switch (id)
    {
        case LDO_LDO_DCXO:
            prvUnlockPowerReg();
            if(enabled)
            {
                ANA_REG_AND(ANA_POWER_PD_SW0, (~(0x0001<<7)));
            }
            else
            {
                ANA_REG_OR(ANA_POWER_PD_SW0, (0x0001<<7)); //1:POWERDOWN
            }
            if(lp_enabled)
            {
                ANA_REG_AND(ANA_SLP_LDO_PD_CTRL0, (~(0x0001<<11)));
            }
            else
            {
                ANA_REG_OR(ANA_SLP_LDO_PD_CTRL0, (0x0001<<11)); //1:POWERDOWN
            }
        break;

        default:
            // ignore silently
            break;
    }

}


void halPmuSwitchPowerPm2(uint32 id, uint32 pm2)
{
    prvUnlockPowerReg();

    switch (id)
    {
    case LDO_LDO_DCXO:
       if(pm2)
        {
            ANA_REG_AND(ANA_PM2_PD_EN, ~BIT_2);
        }
        else
        {
            ANA_REG_OR(ANA_PM2_PD_EN, BIT_2); //1// Enable Power Down
        }
    break;

    case HAL_POWER_MEM:
       if(pm2)
        {
            ANA_REG_AND(ANA_RESERVED_REG5, ~BIT_2);
        }
        else
        {
            ANA_REG_OR(ANA_RESERVED_REG5, BIT_2); //1   // Enable Power Down
        }
    break;

    case HAL_POWER_DCDC_GEN:
       if(pm2)
        {
            ANA_REG_AND(ANA_PM2_PD_EN, ~BIT_4);
        }
        else
        {
            ANA_REG_OR(ANA_PM2_PD_EN, BIT_4); //1   // Enable Power Down
        }
    break;

    case HAL_POWER_VIO33:
       if(pm2)
        {
            ANA_REG_AND(ANA_PM2_PD_EN, ~BIT_1);
        }
        else
        {
            ANA_REG_OR(ANA_PM2_PD_EN, BIT_1);   // Enable Power Down
        }
    break;

    default:
        // ignore silently
    break;
    }

}


/*****************************************************************************/
//  Description:    this function is used to initialize LDO voltage level.
//  Global resource dependence:
//  Author: Tao.Feng && Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_Init (void)
{
    uint8 i;

    s_vcam_count = 0;
    g_ldo_ctl_tab = Ldo_Get_Cfg();

    SCI_ASSERT (NULL != g_ldo_ctl_tab);/*assert verified*/


    //LDO_DcdcWpaSet(1, 0, 0, 3500);   /*WPA*/
    LDO_TestInit();       /*test:first open some device vol,such as sd/camara*/

    //lcm backlight stub
    //LCM_Backlight_turnon();
    //deepsleep init set for ldo
    LDO_DeepSleepInit();

    return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description:  Turn on the LDO specified by input parameter ldo_id
//  Global resource dependence: NONE
//  Author:  Tao.Feng && Yi.Qiu
//  Note:    return value = LDO_ERR_OK if operation is executed successfully
/*****************************************************************************/
LOCAL  LDO_CTL_PTR LDO_GetLdoCtl (LDO_ID_E ldo_id)
{
    uint8 i = 0;
    LDO_CTL_PTR ctl = NULL;

    SCI_ASSERT (NULL != g_ldo_ctl_tab);/*assert verified*/

    for (i=0; g_ldo_ctl_tab[i].id != LDO_LDO_MAX; i++)
    {
        if (g_ldo_ctl_tab[i].id == ldo_id)
        {
            ctl = &g_ldo_ctl_tab[i];
            break;
        }
    }

    SCI_PASSERT (ctl != NULL, ("ldo_id = %d", ldo_id));/*assert verified*/

    return ctl;
}

/*****************************************************************************/
//  Description:  Turn on the LDO specified by input parameter ldo_id
//  Global resource dependence: NONE
//  Author:  Tao.Feng && Yi.Qiu
//  Note:    return value = LDO_ERR_OK if operation is executed successfully
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_TurnOnLDO (LDO_ID_E ldo_id)
{
	LDO_CTL_PTR ctl = NULL;
#if defined(PLATFORM_UWS6121E)
     if(((ldo_id>=LDO_LDO_SIM0) &&(ldo_id<=LDO_LDO_SIM1)))
	return LDO_ERR_OK;		// Do nothing
#endif



	ctl = LDO_GetLdoCtl (ldo_id);
	SCI_PASSERT (ctl != NULL, ("ldo_id = %d", ldo_id));/*assert verified*/

	if (ctl->bp_reg == NULL)
	{
	return LDO_ERR_OK;
	}

	SCI_DisableIRQ();
	SCI_PASSERT (ctl->ref >= 0, ("ctl->ref = %d", ctl->ref));/*assert verified*/

	if (ctl->ref == 0)
	{
              prvUnlockPowerReg();
		LDO_REG_AND(ctl->bp_reg, ~ctl->bp);
	}

	ctl->ref++;

	SCI_RestoreIRQ();

	return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description:  Turo off the LDO specified by parameter ldo_id
//  Global resource dependence: NONE
//  Author: Tao.Feng && Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_TurnOffLDO (LDO_ID_E ldo_id)
{
	LDO_CTL_PTR ctl = NULL;
#if defined(PLATFORM_UWS6121E)
       if(((ldo_id>=LDO_LDO_SIM0) &&(ldo_id<=LDO_LDO_SIM1)))
	return LDO_ERR_OK;		// Do nothing
#endif


    ctl = LDO_GetLdoCtl (ldo_id);
    SCI_PASSERT (ctl != NULL, ("ldo_id = %d", ldo_id));/*assert verified*/

    if (ctl->bp_reg == NULL)
    {
        return LDO_ERR_OK;
    }

    SCI_DisableIRQ();

    if (ctl->ref > 0)
    {
        ctl->ref--;
    }

    if (ctl->ref == 0)
    {
              prvUnlockPowerReg();
		LDO_REG_OR(ctl->bp_reg,ctl->bp);

    }

    SCI_RestoreIRQ();
    return LDO_ERR_OK;

}

/*****************************************************************************/
//  Description: Find the LDO status -- ON or OFF
//  Global resource dependence:
//  Author: Tao.Feng && Yi.Qiu
//  Note: return SCI_TRUE means LDO is ON, SCI_FALSE is OFF
/*****************************************************************************/
PUBLIC BOOLEAN LDO_IsLDOOn (LDO_ID_E ldo_id)
{
    uint32  masked_val = 0;
    LDO_CTL_PTR ctl = NULL;

#if defined(PLATFORM_UWS6121E)
       if((ldo_id>=LDO_LDO_SIM0) &&(ldo_id<=LDO_LDO_SIM1))
	return TRUE;		// Do nothing
#endif

    ctl = LDO_GetLdoCtl (ldo_id);
    SCI_PASSERT (ctl != NULL, ("ldo_id = %d", ldo_id));/*assert verified*/

    masked_val = (LDO_REG_GET (ctl->bp_reg) & ctl->bp);

    return (masked_val?SCI_FALSE:SCI_TRUE);
}

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
/*****************************************************************************/
//  Description:  change the LDO voltage level specified by parameter ldo_id
//  Global resource dependence:
//  Author: Tao.Feng && Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_SetVoltLevel (LDO_ID_E ldo_id, LDO_VOLT_LEVEL_E volt_level)
{
#ifdef PLATFORM_SC6800H
    LOCAL_VAR_DEF
#endif

    uint32 b0_mask,b1_mask,b2_mask;
    LDO_CTL_PTR  ctl = PNULL;

    b0_mask = (volt_level & BIT_0) ?~0:0;
    b1_mask = (volt_level & BIT_1) ?~0:0;
    b2_mask = (volt_level & BIT_2) ?~0:0;

    ctl = LDO_GetLdoCtl (ldo_id);
    SCI_PASSERT (ctl != NULL, ("ldo_id = %d", ldo_id));/*assert verified*/

    if (ctl->level_reg_b0 == NULL)
    {
        return LDO_ERR_ERR;
    }

    if (ctl->level_reg_b2 != NULL)
    {
        if ((ctl->level_reg_b0 == ctl->level_reg_b1) && (ctl->level_reg_b0 == ctl->level_reg_b2))
        {
            SET_LEVEL (ctl->level_reg_b0, b0_mask, b1_mask, b2_mask, ctl->b0, ctl->b0_rst, ctl->b1, ctl->b1_rst, ctl->b2, ctl->b2_rst);
        }
        else
        {
            SET_LEVELBIT (ctl->level_reg_b0, b0_mask, ctl->b0, ctl->b0_rst);
            SET_LEVELBIT (ctl->level_reg_b1, b1_mask, ctl->b1, ctl->b1_rst);
            SET_LEVELBIT (ctl->level_reg_b2, b2_mask, ctl->b2, ctl->b2_rst);
        }
    }
    else
    {
        if (ctl->level_reg_b0 == ctl->level_reg_b1)
        {
            SET_LEVEL (ctl->level_reg_b0, b0_mask, b1_mask, NULL, ctl->b0, ctl->b0_rst, ctl->b1, ctl->b1_rst, NULL, NULL);
        }
        else
        {
            SET_LEVELBIT (ctl->level_reg_b0, b0_mask, ctl->b0, ctl->b0_rst);
            SET_LEVELBIT (ctl->level_reg_b1, b1_mask, ctl->b1, ctl->b1_rst);
        }
    }
    return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description: Get LDO voltage level
//  Global resource dependence:
//  Author: Tao.Feng && Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC LDO_VOLT_LEVEL_E LDO_GetVoltLevel (LDO_ID_E ldo_id)
{
    uint32 level_ret = 0;
    LDO_CTL_PTR ctl = NULL;

    ctl = LDO_GetLdoCtl (ldo_id);
    SCI_PASSERT (ctl != NULL, ("ldo_id = %d", ldo_id));/*assert verified*/

    if (ctl->level_reg_b2 != NULL)
    {
        if ((ctl->level_reg_b0 == ctl->level_reg_b1) && (ctl->level_reg_b0 == ctl->level_reg_b2))
        {
            GET_LEVEL (ctl->level_reg_b0, ctl->b0, ctl->b1, ctl->b2, level_ret);
        }
        else
        {
            GET_LEVELBIT (ctl->level_reg_b0, ctl->b0, BIT_0, level_ret);
            GET_LEVELBIT (ctl->level_reg_b1, ctl->b1, BIT_1, level_ret);
            GET_LEVELBIT (ctl->level_reg_b2, ctl->b2, BIT_2, level_ret);
        }
    }
    else
    {
        if (ctl->level_reg_b0 == ctl->level_reg_b1)
        {
            GET_LEVEL (ctl->level_reg_b0, ctl->b0, ctl->b1, NULL, level_ret);
        }
        else
        {
            GET_LEVELBIT (ctl->level_reg_b0, ctl->b0, BIT_0, level_ret);
            GET_LEVELBIT (ctl->level_reg_b1, ctl->b1, BIT_1, level_ret);
        }
    }

    return level_ret;
}
#else
/*****************************************************************************/
//  Description: Set LDO voltage
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_SetVoltValue (LDO_ID_E ldo_id, uint32 volt_val)
{
    uint32 cur_gear = 0;
	uint32 target_gear = 0;
	uint32 gear_diff = 0;
    uint32 i = 1;
	uint32 j = 0;
    CAL_LDO_PTR ldo_info = Cal_GetLdoCtl(ldo_id);

	if(NULL == ldo_info)
	{
		return LDO_ERR_ERR;
	}
#if defined(PLATFORM_UWS6121E)
     if(((ldo_id>=LDO_LDO_SIM0) &&(ldo_id<=LDO_LDO_SIM1)))
	return LDO_ERR_OK;		// Do nothing
#endif

    if((volt_val < ldo_info->vol_min) || (volt_val > ldo_info->vol_max))
    {
    	return LDO_ERR_ERR;
    }

	if(ldo_info->current_vol == volt_val)
	{
		// if volt_val equal to current voltage, do not need to set ldo voltage
		return LDO_ERR_OK;
	}


    if(volt_val > ldo_info->default_vol)
    {
        gear_diff = ((volt_val - ldo_info->default_vol) * 2)/ldo_info->step;
        target_gear = ldo_info->default_gear + gear_diff;
    }
    else
    {
        gear_diff = ((ldo_info->default_vol - volt_val) * 2)/ldo_info->step;
        target_gear = ldo_info->default_gear - gear_diff;
    }

	ldo_info->current_vol = volt_val;

    cur_gear = ANA_REG_GET(ldo_info->reg);
	cur_gear = (cur_gear & ldo_info->mask_bit) >> ldo_info->shift_bit;

	// if ldo is on && (volt_val != current vol), first set voltage to volt_val step by step, then calibrate this ldo
	// else calibrate this ldo after turn on it
	if(LDO_IsLDOOn(ldo_id))
	{
		if(cur_gear > target_gear)
		{
			gear_diff = cur_gear - target_gear;
			while(i <= gear_diff)
			{
				ANA_REG_MSK_OR(ldo_info->reg, ((cur_gear - i) << (ldo_info->shift_bit)), ldo_info->mask_bit);
				for(j = 0; j < DELAY_10US; j++){}

				i++;
			}
		}
		else
		{
			gear_diff = target_gear - cur_gear;
			while(i <= gear_diff)
			{
				ANA_REG_MSK_OR(ldo_info->reg, ((cur_gear + i) << (ldo_info->shift_bit)), ldo_info->mask_bit);
				for(j = 0; j < DELAY_10US; j++){}

				i++;
			}
		}

		#if defined(LDO_CALIBRATION_SUPPORT)
		if(adc_cali_build && ldo_info->if_cali_en)
		{
			__calibrate_ldo(ldo_info);
		}
		#endif
	}
	else
	{
		ANA_REG_MSK_OR (ldo_info->reg, (target_gear << ldo_info->shift_bit), ldo_info->mask_bit);
		ldo_info->cali_done = SCI_FALSE;
	}

    return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description: Set LDO voltage
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 LDO_GetVoltValue (LDO_ID_E ldo_id)
{
    uint32 cur_val=0;
    CAL_LDO_PTR ldo_info =NULL;

#if defined(PLATFORM_UWS6121E)
    if(((ldo_id>=LDO_LDO_SIM0) &&(ldo_id<=LDO_LDO_SIM1)))
	return LDO_ERR_ERR;		// Do nothing
#endif

    ldo_info = Cal_GetLdoCtl(ldo_id);

	if(NULL == ldo_info)
	{
		return LDO_ERR_ERR;
	}

	cur_val = ldo_info->current_vol;

    return cur_val;

}
#endif

/*****************************************************************************/
//  Description:  Control The VCAM Domain PAD to avoid VCAM Domain PAD input not floating
//  Global resource dependence: NONE
//  Author:  Tao.Feng && Yi.Qiu
//  Note:    return value = LDO_ERR_OK if operation is executed successfully
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_ControVCAMPad (BOOLEAN pad_on)
{
#ifdef PLATFORM_SC6600L

    if (pad_on)
    {
        if (s_vcam_count ==0)
        {
            //enable VCAM Domain PAD control when VCAM Domain LDO (LDO_AUX2) are power up
            CHIP_REG_AND (GR_LDO_CTL0, ~ (BIT_18|BIT_19));
            CHIP_REG_OR (GR_LDO_CTL0, BIT_19);
        }

        s_vcam_count++;
    }
    else
    {
        if (s_vcam_count > 0)
        {
            s_vcam_count--;

            if (s_vcam_count ==0)
            {
                //disable VCAM Domain PAD control when VCAM Domain LDO (LDO_AUX2) are power down, to avoid VCAM Domain PAD input not floating
                CHIP_REG_AND (GR_LDO_CTL0, ~ (BIT_18|BIT_19));
                CHIP_REG_OR (GR_LDO_CTL0, BIT_18);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"VCAMPad is not opened!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LDO_HAL_309_112_2_17_23_5_15_1058,(uint8*)"");
        }
    }

#endif
    return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description:  Shut down any LDO that do not used when system enters deepsleep
//  Global resource dependence: s_ldo_reopen[]
//  Author: Tao.Feng && Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC void LDO_DeepSleepInit (void)
{
    uint8 i;
    SLP_LDO_CTL_PTR  slp_ldo_ctl_tab;

    LDO_REG_OR (ANA_SLP_LDO_PD_CTRL1, SLP_LDO_PD_EN);    //all ldo/dcdc powerdown enable in deep sleep mode
    slp_ldo_ctl_tab = Slp_Ldo_Get_Cfg();

    SCI_ASSERT (NULL != slp_ldo_ctl_tab);/*assert verified*/

    for (i=0; slp_ldo_ctl_tab[i].id != SLP_LDO_MAX; i++)
    {
        if (slp_ldo_ctl_tab[i].value == SLP_BIT_SET)
        {
            LDO_REG_OR (slp_ldo_ctl_tab[i].ldo_reg, slp_ldo_ctl_tab[i].mask);
        }
        else
        {
            LDO_REG_AND (slp_ldo_ctl_tab[i].ldo_reg, ~slp_ldo_ctl_tab[i].mask);
        }
    }
#if defined(PLATFORM_UWS6121E)
     LDO_DcdcCoreDeepSleepSet();
	 //ANA_REG_SET(ANA_SLP_LDO_LP_CTRL0, 0x7bfc);
	 //ANA_REG_SET(ANA_SLP_LDO_LP_CTRL1, 0x9);
	 //ANA_REG_SET(ANA_SLP_DCDC_LP_CTRL, 0x2);

#if defined(POP_MDY_SUPPORT)
       ANA_REG_OR (ANA_BLTC_CTRL, (0xC000));    /*output by soft*/
       ANA_REG_SET (ANA_KPLED_CTRL0, (0xb0));    /*kbled_ctrl0*/
       ANA_REG_SET (ANA_KPLED_CTRL1, (0x30));    /*kbled_ctrl1*/
       SCI_Sleep(600);
       ANA_REG_AND(ANA_KPLED_CTRL0, (0x7f));   /*ldo on*/
#endif
#endif
}

/*****************************************************************************/
//  Description:  Control the status of the LDO in deepsleep specified by input parameter ldo_id in
//  Global resource dependence: NONE
//  Author:  yuhua
// input: ldo_id, id of the ldo
//          auto_close, TRUE: auto close the ldo in deepsleep
//          auto_close, FALSE, do nothing in deepsleep
//  Note:    return value = LDO_ERR_OK if operation is executed successfully
/*****************************************************************************/
PUBLIC LDO_ERR_E LDO_AutoCloseInDeepSleep (SLP_LDO_E ldo_id, BOOLEAN auto_close)
{
    uint8 i;

    SLP_LDO_CTL_PTR slp_ldo_ctl_tab = NULL;

    slp_ldo_ctl_tab = Slp_Ldo_Get_Cfg();
    SCI_ASSERT (NULL != slp_ldo_ctl_tab);/*assert verified*/

    SCI_PASSERT (ldo_id <= SLP_LDO_MAX, ("ldo_id = %d", ldo_id));/*assert verified*/

    SCI_DisableIRQ();

    for (i=0; slp_ldo_ctl_tab[i].id != SLP_LDO_MAX; i++)
    {
        if (slp_ldo_ctl_tab[i].id == ldo_id)
        {
            if (auto_close)
            {
                LDO_REG_OR (slp_ldo_ctl_tab[i].ldo_reg, slp_ldo_ctl_tab[i].mask);
            }
            else
            {
                LDO_REG_AND (slp_ldo_ctl_tab[i].ldo_reg, ~slp_ldo_ctl_tab[i].mask);
            }

            break;
        }
    }

    SCI_RestoreIRQ();
    return LDO_ERR_OK;
}

#ifdef PLATFORM_SC8800G
/*****************************************************************************/
//  Description: turn off system core ldo
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void LDO_TurnOffCoreLDO (void)
{
    ANA_REG_SET (ANA_LDO_PD_SET, ANA_LDO_PD_SET_MSK);   /// turn off system core ldo
}

/*****************************************************************************/
//  Description: turn off all module ldo befor system core ldo
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/

LOCAL void LDO_TurnOffAllModuleLDO (void)
{
    ANA_REG_SET (ANA_LDO_PD_CTL, ANA_LDO_PD_CTL_MSK);               ///turn off all module ldo
    ANA_REG_MSK_OR (ANA_PA_CTL, LDO_PA_SET, (LDO_PA_SET|LDO_PA_RST)); ///PA poweroff
}
/*****************************************************************************/
//  Description:  Shut down all LDO when system poweroff
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LDO_TurnOffAllLDO (void)
{
    LDO_TurnOffAllModuleLDO();
    LDO_TurnOffCoreLDO();
}
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
LOCAL CAL_LDO_PTR Cal_GetLdoCtl (LDO_ID_E ldo_id)
{
    uint8 i = 0;
    CAL_LDO_PTR cal_ldo_tab = NULL;
	CAL_LDO_PTR ldo_cali_info = NULL;
	uint32 cal_ldo_tab_size = Cal_Ldo_Get_Tab_Size();

	if(LDO_LDO_MAX == ldo_id)
	{
		return NULL;
	}

    cal_ldo_tab = Cal_Ldo_Get_Cfg();

	for(i = 0; i < cal_ldo_tab_size; i++)
    {
        if(ldo_id == cal_ldo_tab[i].id)
        {
            ldo_cali_info = &cal_ldo_tab[i];
            break;
        }
    }

    return ldo_cali_info;
}

LOCAL uint32 Cal_GetLdoTabIndex (LDO_ID_E ldo_id)
{
    uint32 i = 0xFF;
    CAL_LDO_PTR cal_ldo_tab = NULL;
	CAL_LDO_PTR ldo_cali_info = NULL;
	uint32 cal_ldo_tab_size = Cal_Ldo_Get_Tab_Size();

	if(LDO_LDO_MAX == ldo_id)
	{
		return NULL;
	}

    cal_ldo_tab = Cal_Ldo_Get_Cfg();

	for(i = 0; i < cal_ldo_tab_size; i++)
    {
        if(ldo_id == cal_ldo_tab[i].id)
        {
            break;
        }
    }

    return i;
}

#if defined(LDO_CALIBRATION_SUPPORT)
typedef struct
{
    int32   x_adc;
    int32   y_vol;
    int32   slope;
    int32   zoom_in;
}ADC_CAL_T;

#if defined(LDO_CAL_DEBUG)
typedef struct
{
	LDO_ID_E ldo_id;
	uint32 current_gear;
	uint32 current_voltage;
    uint32 adc_tmp_val[16];
	uint32 adc_average_val;
	uint32 adc_1v_vol;
	uint32 adc_convert_vol;
	BOOLEAN trim_dir;
	uint32 gear_diff;
	uint32 reg_gear;
}LDO_CAL_DEBUG_T;

LDO_CAL_DEBUG_T adc_cal_debug[15] = {0};
#endif

LOCAL ADC_CAL_T adc_small_scale_cal;

LOCAL void __ldoa_group_adc_chn_mux (CAL_LDOA_CHANNEL_E mux)
{
	uint16 value = 0;

	value = ANA_REG_GET(ANA_LDO_CH_CTRL);
	value &= ~(LDOA_CAL_SEL_CH_MSK);
	value |= (mux << LDOA_CAL_SEL_CH_SHIFT);
	ANA_REG_SET(ANA_LDO_CH_CTRL, value);
}

LOCAL void __ldob_group_adc_chn_mux (CAL_LDOB_CHANNEL_E mux)
{
	uint16 value = 0;

	value = ANA_REG_GET(ANA_LDO_CH_CTRL);
	value &= ~(LDOB_CAL_SEL_CH_MSK);
	value |= (mux << LDOB_CAL_SEL_CH_SHIFT);
	ANA_REG_SET(ANA_LDO_CH_CTRL, value);
}

LOCAL uint32 __convert_adc_to_voltage(uint32 adc_value)
{
	int32 result;
    int32 adcvalue = (int32)adc_value;

    result = adc_small_scale_cal.y_vol - ((adc_small_scale_cal.x_adc - adcvalue) * adc_small_scale_cal.slope) / adc_small_scale_cal.zoom_in;

    if(result < 0)
    {
        result = 0;
    }

    return result;
}

LOCAL BOOLEAN __adc_small_scale_calibrate(void)
{
	uint32 adc_small_cali[2];
	int32 vol0 = 1000;  // 1v
	int32 vol1 = 100;   // 0.1v
	const int32 Zoom_in = 1000; // used to enlarge slope value to store in int32
	BOOLEAN ret = FALSE;

	//adc small scale calibration
	ret = EFUSE_HAL_GetADCSmallScaleCali(adc_small_cali);

	if(!ret)
	{
		adc_cali_build = SCI_FALSE;
		return FALSE;
	}

	adc_small_scale_cal.slope = Zoom_in * (vol0 - vol1) / (adc_small_cali[0] - adc_small_cali[1]); //calculate the adc slope
    adc_small_scale_cal.zoom_in = Zoom_in;
    adc_small_scale_cal.x_adc = adc_small_cali[0];
    adc_small_scale_cal.y_vol = vol0;

	adc_cali_build = SCI_TRUE;

	return TRUE;
}

LOCAL BOOLEAN __ldo_get_adjust_vol_gear(CAL_LDO_T * cal_data, uint32 * ldo_gear)
{
	uint32 ldo_level_vol = 0;
	uint32 ret = 0;
	uint32 adc_tmp = 0;
	uint32 i = 0;
	uint32 j = 0;
	uint32 adc_value = 0;
	uint32 default_vol = cal_data->default_vol;
	uint32 gear_tmp = 0;
	uint32 current_gear = 0;
	uint32 current_voltage = cal_data->current_vol;
	uint32 temp = 0;
	#if defined(LDO_CAL_DEBUG)
	uint32 index = 0;
	index = Cal_GetLdoTabIndex(cal_data->id);
	#endif

	// dummy read
	for(i = 0; i < 16; i++)
	{
		ADC_GetResultDirectly (cal_data->adc_ch, SCI_TRUE);
		for(j = 0; j < 1000; j++){};
	}

	for(i = 0; i < 16; i++)
    {
    	//adc scale 1v2
    	adc_tmp = ADC_GetResultDirectly (cal_data->adc_ch, SCI_TRUE);
    	adc_value += adc_tmp;

		for(j = 0; j < 1000; j++){};

		#if defined(LDO_CAL_DEBUG)
		adc_cal_debug[index].adc_tmp_val[i] = adc_tmp;
		#endif
    }

    adc_value = adc_value >> 4;

    ldo_level_vol = __convert_adc_to_voltage(adc_value);

	#if defined(LDO_CAL_DEBUG)
	adc_cal_debug[index].adc_1v_vol= ldo_level_vol;
	adc_cal_debug[index].adc_average_val = adc_value;
	#endif

	ldo_level_vol = ldo_level_vol *  current_voltage / 1000;

	#if defined(LDO_CAL_DEBUG)
	adc_cal_debug[index].current_voltage = current_voltage;
	adc_cal_debug[index].adc_convert_vol= ldo_level_vol;
	#endif

	if(ldo_level_vol > current_voltage)
	{
		gear_tmp = ((ldo_level_vol - current_voltage) * 2) / (cal_data->step);

		// if voltage difference does not exceed 1% of the default voltage, there is no need to adjust the voltage profile
		if((ldo_level_vol - current_voltage) < (current_voltage / 100))
		{
			*(ldo_gear) = 0;
			return ret;
		}

		ret = 0;
	}
	else
	{
		gear_tmp = ((current_voltage - ldo_level_vol) * 2) /(cal_data->step);

		// if voltage difference does not exceed 1% of the default voltage, there is no need to adjust the voltage profile
		if((current_voltage - ldo_level_vol) < (current_voltage / 100))
		{
			*(ldo_gear) = 0;
			return ret;
		}

		ret = 1;
	}

	*(ldo_gear) = gear_tmp;

	#if defined(LDO_CAL_DEBUG)
	adc_cal_debug[index].trim_dir= ret;
	adc_cal_debug[index].gear_diff = gear_tmp;
	#endif

	return ret;
}

LOCAL void __calibrate_ldo(CAL_LDO_PTR cal_ldo_tab)
{
	uint32 i = 1;
	uint32 j = 0;
	uint32 gear_diff = 0;
	uint32 current_gear = 0;
	BOOLEAN trim_dir = FALSE;
	CAL_LDO_PTR ldo_cali_info = cal_ldo_tab;
	uint32 index = 0;

	if(NULL == cal_ldo_tab)
	{
		return ;
	}

	if(!LDO_IsLDOOn(ldo_cali_info->id))
	{
		return ;
	}

	#if defined(LDO_CAL_DEBUG)
	index = Cal_GetLdoTabIndex(ldo_cali_info->id);
	adc_cal_debug[index].ldo_id = ldo_cali_info->id;
	#endif

	//get ldo current gear
	current_gear = (LDO_REG_GET(ldo_cali_info->reg) & (ldo_cali_info->mask_bit)) >> (ldo_cali_info->shift_bit);
	#if defined(LDO_CAL_DEBUG)
	adc_cal_debug[index].current_gear = current_gear;
	#endif

	//enable calibration
	if(ldo_cali_info->ldo_group == CAL_LDO_GROUP_A)
	{
		__ldoa_group_adc_chn_mux (ldo_cali_info->cali_chn);
	}
	else
	{
		__ldob_group_adc_chn_mux (ldo_cali_info->cali_chn);
	}

	trim_dir = __ldo_get_adjust_vol_gear(ldo_cali_info, &gear_diff);

	//disable calibration
	if(ldo_cali_info->ldo_group == CAL_LDO_GROUP_A)
	{
		__ldoa_group_adc_chn_mux (0);
	}
	else
	{
		__ldob_group_adc_chn_mux (0);
	}

	if(0 == gear_diff)
	{
		// do not need calibrate
		#if defined(LDO_CAL_DEBUG)
		adc_cal_debug[index].reg_gear = (LDO_REG_GET(ldo_cali_info->reg) & (ldo_cali_info->mask_bit)) >> (ldo_cali_info->shift_bit);
		#endif
		ldo_cali_info->cali_done = SCI_TRUE;
		return ;
	}

	if(LDO_LDO_SF == ldo_cali_info->id)
	{
		// if ldo sf needs calibration, switch voltage select to software register
		ANA_REG_OR(ANA_LDO_SF_REG0, LDO_SF_V_SEL);
	}

	//adjust ldo voltage
	if(trim_dir)
	{
		if((ldo_cali_info->gear_max - current_gear) < gear_diff)
		{
			gear_diff = (ldo_cali_info->gear_max - current_gear);
		}

		while(i <= gear_diff)
		{
			ANA_REG_MSK_OR(ldo_cali_info->reg, ((current_gear + i) << (ldo_cali_info->shift_bit)), ldo_cali_info->mask_bit);
			for(j = 0; j < DELAY_10US; j++){}

			i++;
		}
	}
	else
	{
		if(current_gear < gear_diff)
		{
			gear_diff = (current_gear - ldo_cali_info->gear_min);
		}

		while(i <= gear_diff)
		{
			ANA_REG_MSK_OR(ldo_cali_info->reg, ((current_gear - i) << (ldo_cali_info->shift_bit)), ldo_cali_info->mask_bit);
			for(j = 0; j < DELAY_10US; j++){}

			i++;
		}
	}

	ldo_cali_info->cali_done = SCI_TRUE;

	#if defined(LDO_CAL_DEBUG)
	adc_cal_debug[index].reg_gear = (LDO_REG_GET(ldo_cali_info->reg) & (ldo_cali_info->mask_bit)) >> (ldo_cali_info->shift_bit);
	#endif
}

LOCAL void __calibrate_ldo_by_id(LDO_ID_E ldo_id)
{
	CAL_LDO_PTR ldo_cali_info = NULL;

	ldo_cali_info = Cal_GetLdoCtl(ldo_id);

	if(NULL == ldo_cali_info)
	{
		return ;
	}

	__calibrate_ldo(ldo_cali_info);
}

LOCAL void __calibrate_ldo_group(void)
{
	uint32 i = 0;
	CAL_LDO_PTR cal_ldo_tab = NULL;

	cal_ldo_tab = Cal_Ldo_Get_Cfg();

	for(i = 0; cal_ldo_tab[i].id != LDO_LDO_MAX; i++)
	{
		if(!(cal_ldo_tab[i].if_cali_en))
		{
			continue;
		}

		__calibrate_ldo(&cal_ldo_tab[i]);
	}
}
#endif

LOCAL void __early_calibrate_ldo_ref( void )
{
#if 0
	uint32 i = 0;
	uint16 value = 0;
	uint16 defalut_val = 0;
	BOOLEAN ret = SCI_FALSE;
	uint32 cali_value = 0;

	//LDOB_REFTRIM
	ret = EFUSE_HAL_GetLDOBRefCaliData(&cali_value);
	if(ret)
	{
		uint32 j = 1;
		uint32 temp_diff = 0;

		defalut_val = ANA_REG_GET(ANA_LDO_REFTRIM_REG);
		defalut_val &= (LDOB_CAL_SEL_REFTRIM_MSK);
		defalut_val = defalut_val >> LDOB_CAL_SEL_REFTRIM_SHIFT;

		if(defalut_val < cali_value)
		{
			temp_diff = (cali_value - defalut_val);
			while(j <= temp_diff)
			{
				value = ANA_REG_GET(ANA_LDO_REFTRIM_REG);
				value &= ~(LDOB_CAL_SEL_REFTRIM_MSK);
				value |= ((defalut_val + j) << LDOB_CAL_SEL_REFTRIM_SHIFT);
				ANA_REG_SET(ANA_LDO_REFTRIM_REG, value);
				for(i = 0; i < DELAY_10US; i++){}

				j++;
			}
		}
		else
		{
			temp_diff = (defalut_val - cali_value);
			while(j <= temp_diff)
			{
				value = ANA_REG_GET(ANA_LDO_REFTRIM_REG);
				value &= ~(LDOB_CAL_SEL_REFTRIM_MSK);
				value |= ((defalut_val - j) << LDOB_CAL_SEL_REFTRIM_SHIFT);
				ANA_REG_SET(ANA_LDO_REFTRIM_REG, value);
				for(i = 0; i < DELAY_10US; i++){}

				j++;
			}
		}

	}

	//LDO Core Sleep
	ret = EFUSE_HAL_GetLDOCoreSLPCaliData(&cali_value);
	if(ret)
	{
		value = ANA_REG_GET(ANA_LDO_CORE_SLP_CTRL1);
		value &= ~(LDO_CORE_V_DS_MSK);
		value |= (cali_value << LDO_CORE_V_DS_SHIFT);
		ANA_REG_SET(ANA_LDO_CORE_SLP_CTRL1, value); //DS CORE:0.85V
	}
#endif
}

PUBLIC void LDO_GeneralLDOCalibration(void)
{
	__early_calibrate_ldo_ref();

#if defined(LDO_CALIBRATION_SUPPORT)
	if(!__adc_small_scale_calibrate())
	{
		return ;
	}

	__calibrate_ldo_group();
#endif
}

/*****************************************************************************/
//  Description:    The function for cpll ldo voltage calibration
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LDO_CPLLLDOCalibration(void)
{
    uint32 cali_value = 0;
	uint32 default_level = 0;
	BOOLEAN ret = SCI_FALSE;
	uint32 temp_diff;
	uint32 i = 1;
	uint32 j = 0;
	uint32 temp_value = 0;

    ret = EFUSE_HAL_GetCpllLdoCaliData(&cali_value);

	if(!ret)
	{
		return;
	}

	default_level = *((volatile uint32 *)CPLL_APLL_CONFIG);
	default_level &= (0x1F << CPLL_LDO_RESERVED_SHIFT);
	default_level = default_level >> CPLL_LDO_RESERVED_SHIFT;

	if(default_level < cali_value)
	{
		temp_diff = cali_value - default_level;

		while(i <= temp_diff)
		{
			temp_value = *((volatile uint32 *)CPLL_APLL_CONFIG);
			temp_value &= ~(0x1F << CPLL_LDO_RESERVED_SHIFT);
			temp_value |= (default_level + i) << CPLL_LDO_RESERVED_SHIFT;
			*((volatile uint32 *)CPLL_APLL_CONFIG) = temp_value;
			for(j = 0; j < DELAY_10US; j++);

			i++;
		}
	}
	else
	{
		temp_diff = default_level - cali_value;

		while(i <= temp_diff)
		{
			temp_value = *((volatile uint32 *)CPLL_APLL_CONFIG);
			temp_value &= ~(0x1F << CPLL_LDO_RESERVED_SHIFT);
			temp_value |= (default_level - i) << CPLL_LDO_RESERVED_SHIFT;
			*((volatile uint32 *)CPLL_APLL_CONFIG) = temp_value;
			for(j = 0; j < DELAY_10US; j++);

			i++;
		}
	}
}

/*****************************************************************************/
//  Description:    The function for avddvb calibration
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LDO_AVDDVBCalibration(void)
{
	uint32 temp_value = 0;
	uint32 value = 0;
	BOOLEAN ret = FALSE;

	ret = EFUSE_HAL_GetAVDDVBCaliData(&temp_value);

	if(!ret)
	{
		return;
	}

	//enable aud module
	//ANA_REG_OR (ANA_MODULE_EN0, ANA_AUD_EN);

	value = ANA_REG_GET((ANA_AUD_BASE+4));
	value &= ~(0x1F << 11);
	value |= temp_value << 11;
	ANA_REG_SET((ANA_AUD_BASE+4), value);

	//disable aud module
	//ANA_REG_AND (ANA_MODULE_EN0, ~ANA_AUD_EN);
}

PUBLIC void LDO_Calibration(void)
{
	LDO_CPLLLDOCalibration();

	LDO_AVDDVBCalibration();

	LDO_GeneralLDOCalibration();
}
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

