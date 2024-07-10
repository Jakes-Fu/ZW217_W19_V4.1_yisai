/******************************************************************************
 ** File Name:      busmonitor_phy_v5.c                                                *
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
#include "busmonitor_reg_v5.h"
//#include "../../int/v0/int_reg_v0.h"
#include "fiq_drvapi.h"
//#include "isr_drvapi.h"

#include "busmonitor_drvapi.h"
#include "../busmonitor_phy.h"

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
LOCAL BM_STATUS bm_status;
LOCAL unsigned long match_addr = 0;
LOCAL unsigned long match_data = 0;

LOCAL char s_bm_inf_buf[100];

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
extern unsigned long _tx_pc_val_before_fiq;

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
LOCAL void bm_open (int point)
{
    //BM_PRINT:"[BM DRV:]bm_open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BUSMONITOR_PHY_V5_82_112_2_17_23_3_22_384,(uint8*)"");

    /* Bus monitor enable and interrupt*/
    CHIP_REG_OR (CHN_INT (point), BIT_29);
    CHIP_REG_OR (CHN_INT (point), BIT_28);
    CHIP_REG_OR (CHN_INT (point), BIT_0);

    // Register bm FIQ
    FIQ_RegHandler (TB_BM_INT, (FIQ_HANDLE) bm_isr);
    /* irq_enable register:enable Bus monitor interrupt */
    CHIPDRV_EnableFIQINT(TB_BM_INT) ;
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BUSMONITOR_PHY_V5_109_112_2_17_23_3_22_385,(uint8*)"");

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
LOCAL void bm_rw_callback (unsigned long data, unsigned long pc_before_fiq)
{

    MMU_DISABLE
    bm_close();
    sprintf (s_bm_inf_buf, "BUS MONITOR: match_address=0x%x, match_data=0x%x", match_addr, match_data);//lint !e718 !e746
    SCI_Assert (s_bm_inf_buf, __FILE__, __LINE__);/*assert verified*/
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
                (bm_status.points[i].on_raise) (bm_status.points[i].data, _tx_pc_val_before_fiq);
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
    /* Disable bus monitor irq */
    CHIPDRV_DisableFIQINT(TB_BM_INT);
    // unregister bm FIQ
    FIQ_UnRegHandler (TB_BM_INT);
    /* Disable bus monitor */
    CHIP_REG_AND (BUS_MON_CTL_BASE, ~BIT_0);
#if 0 //!defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A) //sc6531efm only has one busmonitior
    CHIP_REG_AND (BUS_MON_CTL_BASE+0x1000, ~BIT_0);
    CHIP_REG_AND (BUS_MON_CTL_BASE+0x2000, ~BIT_0);
#if defined(CHIP_VER_6531)
    CHIP_REG_AND (BUS_MON_CTL_BASE+0x3000, ~BIT_0);
#endif
#endif
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BUSMONITOR_PHY_V5_219_112_2_17_23_3_22_386,(uint8*)"");

    SCI_ASSERT ( (MAX_BM_POINTS_NUM > point) && (0 <= point));/*assert verified*/
    SCI_ASSERT (setting);/*assert verified*/

    bm_clean_point (point);

    bm_point = &bm_status.points[point];
    bm_point_reg = (volatile BM_POINT_REG *) (CHN_INT (point));
    /* Set addr min and max */
    bm_point_reg->bm_addr_min  = setting->addr_min;
    bm_point_reg->bm_addr_max  = setting->addr_max;
    bm_point_reg->bm_addr_mask  = setting->addr_mask;
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

    if(0 == point)
    {
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		switch (setting->chn_sel)
		{
			case BM_ARM_DATA:
			case BM_ARM_INSTRUCTION:
			case BM_ARM7_ACCESS_EMC:
			case BM_DMA_W:
				CHIP_REG_OR (AHB_SYS_CTL0, 1 << BUSMON0_CHN_MUX_SHIFT);
				break;
				
			case BM_SDIO:
			case BM_DCAM:
			case BM_ARM7_ACCESS_NONE_EMC:
			case BM_DMA_R:
				CHIP_REG_AND (AHB_SYS_CTL0, ~(1 << BUSMON0_CHN_MUX_SHIFT));
				CHIP_REG_OR (AHB_SYS_CTL0, 1 << BUSMON0_CHN0_MUX_SHIFT);
				break;
				
			case BM_LZMA:
			case BM_LCM_ACCESS_EMC:
			case BM_LCM_ACCESS_LCM:
				CHIP_REG_AND (AHB_SYS_CTL0, ~(1 << BUSMON0_CHN_MUX_SHIFT));
				CHIP_REG_AND (AHB_SYS_CTL0, ~(1 << BUSMON0_CHN0_MUX_SHIFT));
				break;
				
			default:
				CHIP_REG_OR (AHB_SYS_CTL0, 1 << BUSMON0_CHN_MUX_SHIFT);
				break;

		}
		
        CHIP_REG_AND (AHB_SYS_CTL0, ~BUSMON0_CHN_SEL_MSK);
        CHIP_REG_OR (AHB_SYS_CTL0, ((setting->chn_sel & 0x3) << BUSMON0_CHN_SEL_SHIFT));
#else		
        CHIP_REG_AND (AHB_SYS_CTL0, ~BUSMON0_CHN_SEL_MSK);
        CHIP_REG_OR (AHB_SYS_CTL0, (setting->chn_sel << BUSMON0_CHN_SEL_SHIFT));
#endif
    }
#if 0 //!defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A) //sc6531efm only has one busmonitior
    else if(1 == point)
    {
        CHIP_REG_AND (AHB_SYS_CTL0, ~BUSMON1_CHN_SEL_MSK);
        CHIP_REG_OR (AHB_SYS_CTL0, (setting->chn_sel << BUSMON1_CHN_SEL_SHIFT));
    }
    else if(2 == point)
    {
        CHIP_REG_AND (AHB_SYS_CTL0, ~BUSMON2_CHN_SEL_MSK);
        CHIP_REG_OR (AHB_SYS_CTL0, (setting->chn_sel << BUSMON2_CHN_SEL_SHIFT));
    }
#if defined(CHIP_VER_6531)
    else
    {
        if(setting->chn_sel == 0)
        {
            CHIP_REG_AND (AHB_SYS_CTL0, ~BUSMON3_CHN_SEL);
	    }
        else
	    {
            CHIP_REG_OR (AHB_SYS_CTL0, BUSMON3_CHN_SEL);
        }        
    }
#endif
#endif

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
	volatile BM_POINT_REG *bm_point_reg;
	
	bm_point_reg = (volatile BM_POINT_REG *) (CHN_INT (0));
    match_addr = bm_point_reg->bm_addr_match;
    match_data = bm_point_reg->bm_data_match;

    *bm_match_addr = match_addr;
    *bm_match_data = match_data;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
