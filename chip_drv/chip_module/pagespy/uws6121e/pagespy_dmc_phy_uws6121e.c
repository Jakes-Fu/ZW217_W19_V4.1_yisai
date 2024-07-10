/******************************************************************************
 ** File Name:     pagespy_dmc_phy_uix8910.c                                              *
 ** Author:         jiajiang.lu                                                   *
 ** DATE:           3/5/2019                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                 *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 3/5/2019     jiajiang.lu      Create.                                   *
 **                                                                      *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "../pagespy_dmc_phy.h"
#include "pagespy_dmc_reg_uws6121e.h"

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define    PAGESPY_DMC_NUM_AP       (3)
#define    PAGESPY_DMC_AP_ID0        (5)
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
HAL_PAGESPY_LISR_REGISTER_T g_pagespy_lisrEntry[PAGESPY_DMC_NB_PAGE];
HAL_PAGESPY_AID_CHECK_T  g_pagespy_aid[PAGESPY_AID_MASTER_NUM] =
{
	{HAL_PAGE_MASTER_A5, "AP_A5"},
	{HAL_PAGE_MASTER_F8, "F8"},
	{HAL_PAGE_MASTER_LZMA, "LZMA"},
	{HAL_PAGE_MASTER_ZSP_DRAM, "ZSPDRAM"},
	{HAL_PAGE_MASTER_GOUDA, "GOUDA"},
	{HAL_PAGE_MASTER_USB, "USB"},
	{HAL_PAGE_MASTER_CPA5, "CP_A5"},
	{HAL_PAGE_MASTER_WCN_MEM, "WCNMEM"},
	{HAL_PAGE_MASTER_LCD, "LCD"},
	{HAL_PAGE_MERGE_AXIDMA, "AXIDMA"},
	{HAL_PAGE_MERGE_LTE, "LTE"},
	{HAL_PAGE_MERGE_AON, "AON"},
	{HAL_PAGE_MERGE_GGE, "GGE"},
	{HAL_PAGE_MERGE_SYSIFC, "SYSIFC"},
	{HAL_PAGE_MASTER_USB11, "USB11"},
	{HAL_PAGE_MASTER_M_CE, "CE"},
};

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
uint8 halb_PagespyAddrHitAIDCheck(uint32 aid_info)
{
	uint8 i;
	for (i = 0; i < PAGESPY_AID_MASTER_NUM; i++)
	{
		if ((aid_info & 0x1C7) == (g_pagespy_aid[i].pagespy_aid & 0x1C7))    /*适配上AID,bit3-bit5固定为0*/
		{
                    if ((aid_info & 0x1C7) == HAL_PAGE_MASTER_MERGE_MEM)
                    {
                        switch(aid_info & 0x1ff)
                        {
                            case HAL_PAGE_MERGE_AXIDMA:
                            {
                                break;
                            }
                            case HAL_PAGE_MERGE_LTE:
                            {
                                i += 1;
                                break;
                            }
                            case HAL_PAGE_MERGE_AON:
                            {
                                i += 2;
                                break;
                            }
                            case HAL_PAGE_MERGE_GGE:
                            {
                                i += 3;
                                break;
                            }
                            case HAL_PAGE_MERGE_SYSIFC:
                            {
                                i += 4;
                                break;
                            }
                        }
                    }
                    return i;
		}
	}

    SCI_PASSERT(0, ("pagespy:hit AID error"));/*assert verified*/
    return 0xFF;     //clean warning
}

void pagespy_addrhit_lisr(uint8 page_num, HAL_PAGE_IRQ_STATUS_T pagespy_irq_status)
{
	uint32 hit_info;
	uint8 addrhit_index;

	hit_info = hwp_pagespyDmc->page[page_num].HIT_INFO;
       SCI_TRACE_LOW("pagespy addr hitinfo=0x%x\n",hit_info);
	addrhit_index = halb_PagespyAddrHitAIDCheck(((hit_info & PAGESPY_DMC_HIT_AID_MASK) >> PAGESPY_DMC_HIT_AID_SHIFT) & 0x1FF);
	if (pagespy_irq_status.readhit_int)
	{
              SCI_PASSERT(0, ("pagespy:addr_hit aid:%s, readaddr=0x%x",g_pagespy_aid[addrhit_index].aid_name, hwp_pagespyDmc->page[page_num].HIT_ADDR));/*assert verified*/
       }
	else if (pagespy_irq_status.writehit_int)
	{
              SCI_PASSERT(0, ("pagespy:addr_hit aid:%s, writeaddr=0x%x",g_pagespy_aid[addrhit_index].aid_name, hwp_pagespyDmc->page[page_num].HIT_ADDR));/*assert verified*/
	}
}

void pagespy_lisr(void)
{
    uint32 int_vec;
    unsigned short i = 0;
    union {
        uint32 reg; // used to write the full reg
        HAL_PAGE_IRQ_STATUS_T bitfield;
    } cause;

    int_vec = hwp_pagespyDmc->page[0].INT_VEC;

    for(i = PAGESPY_DMC_AP_ID0; i < PAGESPY_DMC_AP_ID0 + PAGESPY_DMC_NUM_AP; i++)
    {
        if(int_vec & (0x01 << i))
        {
            cause.reg = hwp_pagespyDmc->page[i].MON_INT_STATUS & 0x3F;
            hwp_pagespyDmc->page[i].MON_INT_STATUS = cause.reg;      /*清除中断状态，将清除INT_STATUS/INT_CAUSE/INT_VEC寄存器*/

			if ((cause.bitfield.writehit_int || cause.bitfield.readhit_int) && (hwp_pagespyDmc->page[i].MON_INT_EN & PAGESPY_DMC_HIT_INT_EN))
			{     /*是hit中断*/
				pagespy_addrhit_lisr(i, cause.bitfield);      /*hit发生，直接就蓝屏，所以中断由驱动定义*/
			}
			else if ((cause.bitfield.time_int || cause.bitfield.longtime_int) && (hwp_pagespyDmc->page[i].MON_INT_EN & PAGESPY_DMC_TIMER_INT_EN))
			{    /*是time中断*/
				if (g_pagespy_lisrEntry[i].lisr_givenTime != NULL)
				{
					g_pagespy_lisrEntry[i].lisr_givenTime((uint32 *)&(hwp_pagespyDmc->page[i].WRITE_BYTES_SR), (uint32 *)&(hwp_pagespyDmc->page[i].READ_BYTES_SR));
				}
			}
			else if ((cause.bitfield.writenum_int || cause.bitfield.readnum_int) && (hwp_pagespyDmc->page[i].MON_INT_EN & PAGESPY_DMC_NUM_INT_EN))
			{    /*是num中断*/
				if (g_pagespy_lisrEntry[i].lisr_visitNum != NULL)
				{
					g_pagespy_lisrEntry[i].lisr_visitNum((uint32 *)&(hwp_pagespyDmc->page[i].TIMER_COUNT));
				}
			}
        }
    }

}

/*ap only have 3 ch form 5 to 7*/
uint8 halb_PagePhyNumGet(uint8 id)
{
    if (id >= PAGESPY_DMC_NUM_AP)
    {
        SCI_PASSERT(0, ("pagespy:there is no ch"));/*assert verified*/
        return 0xFF;     //clean warning
    }
    else
    {
        return (PAGESPY_DMC_AP_ID0 + id);
    }
}

// =============================================================================
// hal_PageProtectSetup
// -----------------------------------------------------------------------------
/// This function setups a protection page
///
/// @param pageNum Name of the page we want to configure (#HAL_DBG_PAGE_UD0)
/// @param mode Protection mode
/// @param startAddr Address of the beginning of the page
/// @param endAddr  End address of the page. This address is not included in
/// the page
// =============================================================================
PUBLIC void hal_PageProtectAddrSetup(uint8 ch, HAL_PAGESPY_ADDR_MODE_T mode, uint32 startAddr, uint32 endAddr)
{
    if ((mode != HAL_PAGE_WRITE_HIT_TRIGGER) && (mode != HAL_PAGE_READ_HIT_TRIGGER) && (mode != HAL_PAGE_WR_HIT_TRIGGER))
    {
        SCI_PASSERT(0, ("pagespy:hitaddr para error"));/*assert verified*/
    }
    hwp_pagespyDmc->page[ch].MON_INT_STATUS = (PAGESPY_DMC_WRITE_HIT_INT_STATUS | PAGESPY_DMC_READ_HIT_INT_STATUS);     /*清状态标志*/
    hwp_pagespyDmc->page[ch].MON_START_ADDR = startAddr >> 4;
    hwp_pagespyDmc->page[ch].MON_END_ADDR = endAddr >> 4;
    hwp_pagespyDmc->page[ch].MON_CONF |= (PAGESPY_DMC_MON_HIT_MODE(mode) | PAGESPY_DMC_MON_HIT_EN);
    hwp_pagespyDmc->page[ch].MON_INT_EN |= PAGESPY_DMC_HIT_INT_EN;
    hwp_pagespyDmc->page[ch].MON_CTRL = PAGESPY_DMC_MON_CTRL_SR;
}

PUBLIC void hal_PageTimeThSetup(uint8 ch, HAL_PAGESPY_TIME_MODE_T mode, uint32 time_th, void *callback_func)
{
    uint32 mon_conf = 0;

    if (mode == HAL_PAGE_TIME_TRIGGER)
    {
    	mon_conf = PAGESPY_DMC_MON_TIME_EN;
    }
    else if (mode == HAL_PAGE_LONGTIME_TRIGGER)
    {
    	mon_conf = (PAGESPY_DMC_MON_TIME_EN | PAGESPY_DMC_MON_LONG_EN);
    }
    g_pagespy_lisrEntry[ch].lisr_givenTime = (lisr_givenTime)callback_func;
    hwp_pagespyDmc->page[ch].MON_INT_STATUS = (PAGESPY_DMC_TIME_INT_STATUS | PAGESPY_DMC_LONG_INT_STATUS);     /*清状态标志*/
    hwp_pagespyDmc->page[ch].MON_TIMER_TH = time_th;
    hwp_pagespyDmc->page[ch].MON_CONF |= mon_conf;
    if (callback_func == NULL)
    {
        hwp_pagespyDmc->page[ch].MON_INT_EN &= ~PAGESPY_DMC_TIMER_INT_EN;
    }
    else
    {
        hwp_pagespyDmc->page[ch].MON_INT_EN |= PAGESPY_DMC_TIMER_INT_EN;
    }
    hwp_pagespyDmc->page[ch].MON_CTRL = PAGESPY_DMC_MON_CTRL_SR;
}

PUBLIC void hal_PageStop(uint8 ch)
{
    hwp_pagespyDmc->page[ch].MON_CONF &= ~(PAGESPY_DMC_MON_TIME_EN | PAGESPY_DMC_MON_LONG_EN | PAGESPY_DMC_MON_HIT_EN);
    hwp_pagespyDmc->page[ch].MON_CTRL &= ~PAGESPY_DMC_MON_CTRL_SR;
}

PUBLIC void hal_PageTimeCtrl(uint8 ch, uint8 ctrl)
{
    if (ch != 0)
    {
        hwp_pagespyDmc->page[ch].MON_CONF |= PAGESPY_DMC_MON_TIME_EN;
        hwp_pagespyDmc->page[ch].MON_CTRL = PAGESPY_DMC_MON_CTRL_SR;
    }
    else
    {
        hwp_pagespyDmc->page[ch].MON_CONF &= ~(PAGESPY_DMC_MON_TIME_EN);
        hwp_pagespyDmc->page[ch].MON_CTRL &= ~(PAGESPY_DMC_MON_CTRL_SR);
    }
}

PUBLIC uint32 hal_PageTimeWBGet(uint8 ch)
{
    return hwp_pagespyDmc->page[ch].WRITE_BYTES_SR;
}

PUBLIC uint32 hal_PageTimeRBGet(uint8 ch)
{
    return hwp_pagespyDmc->page[ch].READ_BYTES_SR;
}

PUBLIC void hal_PageNumThSetup(uint8 ch, HAL_PAGESPY_NUM_MODE_T mode, uint32 num_th, void *callback_func)
{
    g_pagespy_lisrEntry[ch].lisr_visitNum = (lisr_visitNum)callback_func;
    hwp_pagespyDmc->page[ch].MON_INT_STATUS = (PAGESPY_DMC_WRITE_NUM_INT_STATUS | PAGESPY_DMC_READ_NUM_INT_STATUS);     /*清状态标志*/
    hwp_pagespyDmc->page[ch].MON_NUM_TH = num_th;
    hwp_pagespyDmc->page[ch].MON_CONF |= PAGESPY_DMC_MON_NUM_EN;
    hwp_pagespyDmc->page[ch].MON_INT_EN |= PAGESPY_DMC_NUM_INT_EN;
    hwp_pagespyDmc->page[ch].MON_CTRL = PAGESPY_DMC_MON_CTRL_SR;
}

PUBLIC void hal_PagespyInit(void)
{
    uint32  status    = 0;

    status = ISR_RegHandler(TB_PAGE_SPY_INT, pagespy_lisr);

    if (TB_SUCCESS == status)
    {
        /*enable dma int*/
        CHIPDRV_EnableIRQINT (TB_PAGE_SPY_INT);
    }
#if 0//bringup_tmp
    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_PAGESPY;
    hwp_sysCtrl->CLK_OTHERS_ENABLE = SYS_CTRL_ENABLE_OC_ID_PSRAM_PAGESPY;
#endif
}

