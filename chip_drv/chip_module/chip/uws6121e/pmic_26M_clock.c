/******************************************************************************
 ** File Name:      pmic_phy_v7.c                                              *
 ** Author:         haibing.chen                                               *
 ** DATE:           2/25/2010                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                 *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/03/2018     haibing.chen      Create.                                   *
 **                                                                      *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "pmic_26M_clock.h"
#include "sys_ctrl.h"

/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/


/* 
 * NOTE1: CHARGE, AUDIO, WCN all need use 26m clock, for save power, need close 26M clock when power on, 
 *        so must add common control of 26m clock
 * NOTE2: PMIC_26M_clock_status is not same with RF_ANA_26M_CTRL_SET, because the CHARGE AND AUDIO both use the bit2 
 */
 
LOCAL uint32    PMIC_26M_clock_status = 0;



PUBLIC void PMIC_26M_status_init (void)
{
    PMIC_26M_clock_status = 0;
}


PUBLIC void PMIC_26M_open (uint32 open_flag)
{
    REG_SYS_CTRL_SYSCTRL_REG0_T clk_26m;
    SCI_DisableIRQ();
    /* enable 26M output to PMIC*/
    clk_26m.v = hwp_sysCtrl->sysctrl_reg0;
    clk_26m.b.pmic_26m_en = 1;
    hwp_sysCtrl->sysctrl_reg0 = clk_26m.v;
    PMIC_26M_clock_status |= (PMIC_26M_USED_BY_ADC);
    SCI_RestoreIRQ(); 
}


PUBLIC void PMIC_26M_close (uint32 close_flag)
{
    REG_SYS_CTRL_SYSCTRL_REG0_T clk_26m;
    SCI_DisableIRQ();
    
    PMIC_26M_clock_status &= (~close_flag);

    if  ( 0 == PMIC_26M_clock_status )
    {
        /* disable 26M output to PMIC*/
	    clk_26m.v = hwp_sysCtrl->sysctrl_reg0;
	    clk_26m.b.pmic_26m_en = 1;
	    hwp_sysCtrl->sysctrl_reg0 = clk_26m.v;
    }

    if ( ( PMIC_26M_USED_BY_ADC     == close_flag ))
    {
        /* only charge and audio both are closed, clear the bit2 */
        if ( (PMIC_26M_USED_BY_ADC   != (PMIC_26M_clock_status & PMIC_26M_USED_BY_ADC)))
        {
            //hwp_sysreg->RF_ANA_26M_CTRL_CLR = BIT_2;
	    clk_26m.v = hwp_sysCtrl->sysctrl_reg0;
	    clk_26m.b.pmic_26m_en = 1;
	    hwp_sysCtrl->sysctrl_reg0 = clk_26m.v;
        }
    }

    SCI_RestoreIRQ();    
         
}

PUBLIC void PMIC_26M_Print_status(void)
{
//    SCI_TraceLow("PMIC_26M_Print_status ana_ctrl_reg32=0x%x, rf_ana_26M_ctrl=0x%x, status=%d", 
//                    (*(volatile unsigned long *)0x500304c8), hwp_sysreg->RF_ANA_26M_CTRL, PMIC_26M_clock_status);
}

PUBLIC void PMIC_26M_get_status(
    uint32         *ana_ctrl_reg32, 
    uint32         *rf_ana_26M_ctrl,
    uint32         *status)
{
    *ana_ctrl_reg32     = *((volatile unsigned long *)0x500304c8);
    *rf_ana_26M_ctrl    =  hwp_sysreg->RF_ANA_26M_CTRL;
    *status             = PMIC_26M_clock_status;
}


