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
    if ( ( PMIC_26M_USED_BY_ADC    != open_flag )
      && ( PMIC_26M_USED_BY_AUDIO  != open_flag )
      && ( PMIC_26M_USED_BY_WCN    != open_flag )      
      )
    {
        SCI_PASSERT(0, ("ERR OPEN FLAG =%d", open_flag));
    } 

    SCI_DisableIRQ();

    if ( PMIC_26M_USED_BY_ADC == open_flag )
    {
        /* enable 26M output to PMIC*/
        *((volatile unsigned long *)0x500304c8) = 0x1;  //ANA_ctrl_reg32=1
        
        /* for adc: enable_clk_26m_audio */
        hwp_sysreg->RF_ANA_26M_CTRL_SET = BIT_2;  //RF_ANA_26M_CTRL_SET bit2

        PMIC_26M_clock_status |= (PMIC_26M_USED_BY_ADC);
    }
    else if ( PMIC_26M_USED_BY_AUDIO == open_flag )
    {
        /* enable 26M output to PMIC*/
        *((volatile unsigned long *)0x500304c8) = 0x1;  //ANA_ctrl_reg32=1

        /* enable_clk_26m_audio */
        hwp_sysreg->RF_ANA_26M_CTRL_SET = BIT_2;

        /* enable_clk_26m_aud2ad */        
        hwp_sysreg->RF_ANA_26M_CTRL_SET = BIT_4;   

        PMIC_26M_clock_status |= (PMIC_26M_USED_BY_AUDIO);

    }
    else if ( PMIC_26M_USED_BY_WCN == open_flag )
    {
        /* enable 26M output to PMIC*/
        *((volatile unsigned long *)0x500304c8) = 0x1;  //ANA_ctrl_reg32=1

        hwp_sysreg->RF_ANA_26M_CTRL_SET = BIT_8;

        PMIC_26M_clock_status |= (PMIC_26M_USED_BY_WCN);
        
    }

//    SCI_TraceLow("PMIC_26M_open ana_ctrl_reg32=0x%x, rf_ana_26M_ctrl=0x%x, PMIC_26M_clock_status=0x%x, open_flag=0x%x", 
//                    (*(volatile unsigned long *)0x500304c8), hwp_sysreg->RF_ANA_26M_CTRL, PMIC_26M_clock_status, open_flag);

    SCI_RestoreIRQ();
    
}


PUBLIC void PMIC_26M_close (uint32 close_flag)
{
    if ( ( PMIC_26M_USED_BY_ADC    != close_flag )
      && ( PMIC_26M_USED_BY_AUDIO  != close_flag )
      && ( PMIC_26M_USED_BY_WCN    != close_flag )      
      )
    {
        SCI_PASSERT(0, ("ERR CLOSE FLAG =%d", close_flag));
    } 

    SCI_DisableIRQ();
    
    PMIC_26M_clock_status &= (~close_flag);

    if  ( 0 == PMIC_26M_clock_status )
    {
        /* disable 26M output to PMIC*/
        *((volatile unsigned long *)0x500304c8) = 0x0;  //ANA_ctrl_reg32=1
    }

    /* bit8 and bit4 单独被wcn 和 audio 使用，可以清除 */
    if ( PMIC_26M_USED_BY_WCN == close_flag )
    {
        hwp_sysreg->RF_ANA_26M_CTRL_CLR = BIT_8;
    }
    else if ( PMIC_26M_USED_BY_AUDIO == close_flag )
    {
        /* enable_clk_26m_aud2ad */        
        hwp_sysreg->RF_ANA_26M_CTRL_CLR = BIT_4;        
    }
    
    if ( ( PMIC_26M_USED_BY_ADC     == close_flag )
      || ( PMIC_26M_USED_BY_AUDIO   == close_flag )
      )
    {
        /* only charge and audio both are closed, clear the bit2 */
        if ( (PMIC_26M_USED_BY_ADC   != (PMIC_26M_clock_status & PMIC_26M_USED_BY_ADC))
          && (PMIC_26M_USED_BY_AUDIO != (PMIC_26M_clock_status & PMIC_26M_USED_BY_AUDIO)))
        {
            hwp_sysreg->RF_ANA_26M_CTRL_CLR = BIT_2;
        }
    }

//    SCI_TraceLow("PMIC_26M_close ana_ctrl_reg32=0x%x, rf_ana_26M_ctrl=0x%x, PMIC_26M_clock_status=0x%x, close_flag=0x%x", 
//                    (*(volatile unsigned long *)0x500304c8), hwp_sysreg->RF_ANA_26M_CTRL, PMIC_26M_clock_status, close_flag);

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


