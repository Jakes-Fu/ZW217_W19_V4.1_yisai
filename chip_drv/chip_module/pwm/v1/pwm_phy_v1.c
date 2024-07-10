/******************************************************************************
 ** File Name:      pwm_phy_v1.c                                                 *
 ** Author:         Lin.liu                                                   *
 ** DATE:           03/01/2004                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 PWM device.                                               *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/01/2004     Lin.liu          Create.                                   *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"

LOCAL const uint32 pwm_prescale_table[5] = {1, 13, 130, 256, 0};

/*********************************************************************************/
//  Description: This function output PWM waveform
//  Author:      Lin.Liu
//  Input:
//               pwm_num :   the no. of the pwm, should be 0, 1, 2.
//               freq    :   the freq of tone. if is zero, don't set the freq( all set as 0xFFFF).
//               duty_cycle: the duty of pulse. It should be set from 0x0 to 0x64.
//                               If it is more than 0x64, duty should be set 0x64.
//  Return:
//               None
//  Notes:
//               Config the PWMA/PWMB. ( PWMA is the combination of two identical but seqpartely
//               programmable PWM circuits, PWM0 and PWM1. The outputs from the two circuits
//               are toggled at a rate of the PWM0 pre-scaled clock rate. By only enable the PWM0
//               or PWM1. the PWMA can also output the PWM0 or PWM1 output. PWMB is theoutput
//               directly from the pwm2.)
/*********************************************************************************/
PUBLIC void PWM_Config (int pwm_num,  uint32  freq,  uint16  duty_cycle)
{
    pwm_s  *pwm_ptr = 0;
    uint32  pwm_bit = 0;
    uint32  tmp, i, mod=0, prescale;
    uint32  pwm_clk = CHIP_GetAPBClk();
    uint32  sleep_mode;

    if (0 == freq)
    {
        sleep_mode = ENABLE_APB_SLEEP;
    }
    else
    {
        sleep_mode = DISABLE_APB_SLEEP;
    }

    pwm_ptr = (pwm_s *) (PWMx_BASE ( (uint32) pwm_num));
    pwm_bit = GEN_PWMx (pwm_num);

    DPSLP_PWM_EnableApbSleep (pwm_num,sleep_mode);

    /*@xingyun.he changed 2005-11-26 begin*/
    /*if freq is larger than PWM_MAX_FREQ, then we keep the old value, and directly return;
       otherwise if freq is less than PWM_MIN_FREQ, then we close pwm, and return.*/
    if (freq <= PWM_MIN_FREQ (pwm_clk))
    {
        /*disable pwm enable*/
        pwm_ptr->prescale &= ~0x100;
        /*disable general register pwm_en*/
        CHIP_REG_AND (GEN_PWM, ~pwm_bit);
        return;
    }

    if (freq >= PWM_MAX_FREQ (pwm_clk))
    {
        return;
    }

    /*@xingyun.he changed 2005-11-26 end*/

    /*@Nick.Zhao changed 2004-11-23 begin */
    /*check duty_cycle*/
    if (duty_cycle >100)
    {
        duty_cycle=100;
    }

    /*@Nick.Zhao changed 2004-11-23 end */

    /*first enable global control registe*/
    CHIP_REG_OR (GEN_PWM, pwm_bit);
    /*select the prescale*/
    tmp = pwm_clk / freq;  /*lint !e414 comfirmed by xuepeng*/
    i = 0;

    while (0 != (prescale = pwm_prescale_table[i]))
    {
        mod = tmp / (prescale);

        if ( (mod > 10) && (256 >= mod))
        {
            /*Ok, use this pair*/
            break;
        }

        i++;
    }

    if (0 == prescale)
    {
        /*Not found*/
        return;
    }

    /*@Nick.Zhao changed 2004-11-23 begin */
    // calculate the mod_duty field.
    duty_cycle = (duty_cycle * mod) / 100;
    mod--;
    /*@Nick.Zhao changed 2004-11-23 end */


    pwm_ptr->tone         = 0xFFFF;
    pwm_ptr->pattern_low  = 0xFFFF;
    pwm_ptr->pattern_high = 0xFFFF;

    pwm_ptr->cnt_duty     = mod | (duty_cycle << 8);
    prescale--;
    pwm_ptr->prescale = prescale | 0x100;
}

/*********************************************************************************/
//  Description: This function Enable/Disable PWM Function
//  Author:      Feng.Jiang
//  Input:
//               pwm_num :   the no. of the pwm, should be 0, 1, 2.
//               on_off: Enable or Disable the PWM Function
//  Return:
//               None
//  Notes:
//               Enable or Disable the PWMA/PWMB. ( PWMA is the combination of two identical but seqpartely
//               programmable PWM circuits, PWM0 and PWM1. The outputs from the two circuits
//               are toggled at a rate of the PWM0 pre-scaled clock rate. By only enable the PWM0
//               or PWM1. the PWMA can also output the PWM0 or PWM1 output. PWMB is theoutput
//               directly from the pwm2.)
/*********************************************************************************/
LOCAL void PWM_Control (int pwm_num,  BOOLEAN on_off)
{
    uint32  pwm_bit;

    pwm_bit = GEN_PWMx (pwm_num);

    if (on_off)
    {
        CHIP_REG_OR (GEN_PWM, pwm_bit);
    }
    else
    {
        CHIP_REG_AND (GEN_PWM, ~pwm_bit);
    }

    DPSLP_PWM_EnableApbSleep (pwm_num, ( (uint32) !on_off));
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
PUBLIC uint32 PWM_Ioctl (uint32 pwm_num, uint32 cmd, uint32 *arg)
{
    uint32 ret_val = 0;

    SCI_ASSERT ( ( (PWM_NUM > pwm_num) && (0 < pwm_num)));/*assert verified*/
    SCI_ASSERT (NULL != arg);/*assert verified*/

    switch (cmd)
    {
        case PWM_CTL_S_ONOFF:
            PWM_Control (pwm_num,  *arg);
            break;
        case PWM_CTL_S_CLKSRC:
            break;
        default:
            break;
    }

    return  ret_val;
}
//End
