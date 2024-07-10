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
#include "timer_ap.h"
#include "rda2720_glb.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif



extern void serial_puts(const char *s);

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void)
{
    return UIX8910;
}

/*****************************************************************************/
// Description :    This function is used get the chip type ex
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_EX_E CHIP_GetChipTypeEx (void)
{
    return CHIP_TYPE_EX_MAX;
}

/*****************************************************************************/
// Description :    This function is used to init chip.
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void Chip_Init(void)
{
    clock_init();
    ddr_init();
}

PUBLIC void Time_DelayUS(unsigned int us)
{
    unsigned int start;
	unsigned int ticks;
	start = hwp_timer2->hwtimer_curval_l;
    ticks = us*2;  //timer freq = 2000000HZ  0.5us + 1
    while(!(((unsigned)((hwp_timer2->hwtimer_curval_l) - start)) > ticks))
    {
	}
}

uint32 CHIP_PHY_GetECOFlag(void)
{
    return hwp_rfReg->chip_id_reg2;
}

uint32 CHIP_PHY_Get7sRstFlag (void)
{
    if ((ANA_REG_GET(ANA_POR_SRC_FLAG)&BIT_POR_PBINT_7S_SW_FLAG)
        &&((ANA_REG_GET(ANA_POR_SRC_FLAG)&BIT_POR_PBINT_7S_HW_FLAG)==0))
        return 1;
    else
        return 0;
}

void CHIP_PHY_Clean7sRstFlag (void)
{
    ANA_REG_OR(ANA_POR_7S_CTRL,BIT_PBINT_7S_FLAG_CLR);
    ANA_REG_AND(ANA_POR_7S_CTRL,~BIT_PBINT_7S_FLAG_CLR);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
