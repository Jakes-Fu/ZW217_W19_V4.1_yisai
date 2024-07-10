/******************************************************************************
 ** File Name:      busmonitor_phy_v3.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           07/30/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/30/2010     liuhao     Create.                                   *
 ******************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "chip_plf_export.h"
#include "busmonitor_reg_v3.h"
//#include "../../int/v3/int_reg_v3.h"

#include "busmonitor_drvapi.h"
#include "../busmonitor_phy.h"
//#include "isr_drvapi.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_BM
#ifdef DEBUG_BM
#define BM_PRINT   SCI_TRACE_LOW
#else
#define BM_PRINT
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define MMU_DISABLE

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/
LOCAL BM_STATUS bm_status =
{
    FALSE,
    {
        {FALSE, NULL, 0},
        {FALSE, NULL, 0}
    }
};
LOCAL unsigned long match_addr = 0;
LOCAL unsigned long match_data = 0;

LOCAL char s_bm_inf_buf[100];

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
extern unsigned long _tx_pc_val_before_irq;

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
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
LOCAL void bm_clk_eb (void)
{
    //BM_PRINT:"[BM DRV:]bm_clk_eb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BUSMONITOR_PHY_V3_79_112_2_17_23_3_21_380,(uint8*)"");

    if (!bm_status.clk_eb)
    {
        /* AHB_CTL0 register,Mon_eb,enable the monitor clock. */
        CHIP_REG_OR (AHB_CTL0, (AHB_CTL0_BM0_EN | AHB_CTL0_BM1_EN));
        bm_status.clk_eb = TRUE;
    }
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
LOCAL void bm_open (int point)
{
    //BM_PRINT:"[BM DRV:]bm_open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BUSMONITOR_PHY_V3_100_112_2_17_23_3_21_381,(uint8*)"");

    /* Bus monitor enable and interrupt*/
    CHIP_REG_OR (CHN_INT (point), BIT_28);
    CHIP_REG_OR (CHN_INT (point), BIT_0);

    // Register bm IRQ
    ISR_RegHandler (TB_BM_INT, bm_isr);
    /* irq_enable register:enable Bus monitor interrupt */
    
    CHIPDRV_EnableIRQINT(TB_BM_INT);
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
LOCAL void bm_clean_point (int point)
{
    BM_POINT *bm_point;
    volatile BM_POINT_REG *bm_point_reg;

    //BM_PRINT:"[BM DRV:]bm_clean_point"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BUSMONITOR_PHY_V3_127_112_2_17_23_3_21_382,(uint8*)"");

    bm_point = &bm_status.points[point];
    bm_point_reg = (volatile BM_POINT_REG *) (CHN_INT (point));

    if (bm_point->enabled)
    {
        /* because the int_clr_bit cannot clear itself, we have to do it */
        bm_point_reg->bm_int |=BIT_29;
        /* disable int */
        bm_point_reg->bm_int&= (~ (BIT_28));
        /* disable the chn */
        bm_point_reg->bm_int&= (~ (BIT_0));
        bm_point->enabled       = FALSE;
        bm_point->on_raise      = NULL;
        bm_point->data          = 0;
    }
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
LOCAL void bm_rw_callback (unsigned long data, unsigned long pc_before_irq)
{

    MMU_DISABLE
    bm_close();
    sprintf (s_bm_inf_buf, "BUS MONITOR num%d: invalid memory read or write : pc = 0x%08lx", data, pc_before_irq);
    SCI_Assert (s_bm_inf_buf, __FILE__, __LINE__);/*assert to do*/
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
PUBLIC ISR_EXE_T bm_isr (uint32 num)
{
    int i;
    volatile unsigned long int_status;
    volatile BM_POINT_REG *bm_point_reg;

    for (i = 0; i < MAX_BM_POINTS_NUM; ++i)
    {
        int_status = REG32 (CHN_INT (i)) >> 31;

        if (int_status)
        {
            bm_point_reg = (volatile BM_POINT_REG *) (CHN_INT (i));
            match_addr = bm_point_reg->bm_addr_match;
            match_data = bm_point_reg->bm_data_match;

            if (bm_status.points[i].on_raise)
            {
                (bm_status.points[i].on_raise) (bm_status.points[i].data, _tx_pc_val_before_irq);
            }

            CHIP_REG_OR (CHN_INT (i), BIT_29);
        }
    }

    return ISR_DONE;
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
PUBLIC void bm_close (void)
{

    if (bm_status.clk_eb)
    {
        /* Disable bus monitor irq */
	CHIPDRV_DisableIRQINT(TB_BM_INT);
		
        // unregister bm IRQ
        ISR_UnRegHandler (TB_BM_INT);
        /* Disable bus monitor */
        CHIP_REG_AND (BUS_MON_CTL_BASE, ~BIT_0);
        CHIP_REG_AND (BUS_MON_CTL_BASE+0x1000, ~BIT_0);
        CHIP_REG_AND (BUS_MON_CTL_BASE+0x2000, ~BIT_0);
        /* AHB_CTL0 register,Mon_eb,disable the monitor clock. */
        CHIP_REG_AND (AHB_CTL0, ~ (AHB_CTL0_BM0_EN | AHB_CTL0_BM1_EN));
        bm_status.clk_eb = FALSE;
    }
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
PUBLIC void bm_set_point (int point, const BM_POINT_SETTING *setting)
{
    BM_POINT *bm_point;
    volatile BM_POINT_REG *bm_point_reg;

    //BM_PRINT:"[BM DRV:]bm_set_point"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BUSMONITOR_PHY_V3_245_112_2_17_23_3_22_383,(uint8*)"");

    SCI_ASSERT ( (MAX_BM_POINTS_NUM > point) && (0 <= point));/*assert verified*/
    SCI_ASSERT (setting);/*assert verified*/

    if (!bm_status.clk_eb)
    {
        bm_clk_eb();
        bm_status.clk_eb = TRUE;
    }

    bm_clean_point (point);

    bm_point = &bm_status.points[point];
    bm_point_reg = (volatile BM_POINT_REG *) (CHN_INT (point));
    /* Set addr min and max */
    bm_point_reg->bm_addr_min  = setting->addr_min;
    bm_point_reg->bm_addr_max  = setting->addr_max;
    /* Set data target and mask */
    bm_point_reg->bm_data_min=setting->data_min;
    bm_point_reg->bm_data_max=setting->data_max;
    bm_point_reg->bm_data_mask=setting->data_mask;
    bm_point_reg->bm_cfg= BIT_0 | /* bit 0--Busmon_Write_en*/
                          (setting->access_mode << 1) | /* bit 1--Busmon_write */
                          (setting->size_en << 4) | /* bit 4--Busmon_size_en */
                          (setting->access_size << 5) | /* bit 5--Busmon_size */
                          (setting->burst_en << 16) | /* bit 16--Busmon_burst_en */
                          (setting->burst_type << 17); /* bit 17--Busmon_burst */
    CHIP_REG_OR (AHB_CTL3, ( (~setting->chn_sel) << (point + 4)));

    /* enable the point */
    bm_point->enabled = TRUE;

    bm_point->on_raise = setting->on_raise;

    if (NULL == setting->on_raise)
    {
        bm_point->on_raise = bm_rw_callback;
    }

    bm_point->data = point;

    bm_open (point);
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
PUBLIC void bm_get_match (uint32 *bm_match_addr, uint32 *bm_match_data)
{
    *bm_match_addr = match_addr;
    *bm_match_data = match_data;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
