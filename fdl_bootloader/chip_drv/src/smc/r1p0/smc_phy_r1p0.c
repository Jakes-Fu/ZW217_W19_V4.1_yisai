/******************************************************************************
 ** File Name:      smc_phy_r1p0.c                                                                        *
 ** Author:         Yong.Li                                                   *
 ** DATE:           05/11/2012                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    define trace interface just for testing usage             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/11/2012     Yong.Li          Create                                    *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "arm_reg.h"
#include "../smc_phy.h"
#include "chip_drvapi.h"
#include "mmu_drvapi.h"
#include "..\..\mmu\mmu_phy.h"
#include "smc_reg_r1p0.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                           *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define SMC_PSRAM_START_ADDRESS   ((*(volatile uint32 *)0x205000E0 & 0x1) ? 0x34000000 : 0x04000000)

#pragma arm section code = "SMC_CFG_IN_IRAM"

PUBLIC void smc_default_config(void)
{
	*(volatile uint32 *)(0x20000000) = 0x11110000;
	*(volatile uint32 *)(0x20000020) = 0;
	*(volatile uint32 *)(0x20000024) = 0;
	*(volatile uint32 *)(0x20000004) = 0x00000010;
	
	*(volatile uint32 *)(0x20000050) = 0x00904FF0;
	*(volatile uint32 *)(0x20000054) = 0x0151FFFF;
	*(volatile uint32 *)(0x20000058) = 0x00A0744F;	
	*(volatile uint32 *)(0x2000005C) = 0;	
}

#define SMC_CLR_OR(v,c,o)\
	v &= ~c;\
	v |= o;

PUBLIC void SMC_Config(SMC_CFG_PTR_T smc_cfg_ptr)
{
	smc_cfg_ptr->clk_freq = CHIP_GetAhbClk();	
	if(smc_cfg_ptr->clk_freq < ARM_CLK_133M)
	{
		smc_cfg_ptr->latency_cnt = 3;
	}
	else if(smc_cfg_ptr->clk_freq < ARM_CLK_156M)
	{
		smc_cfg_ptr->latency_cnt = 4;
	}
	else
	{
		smc_cfg_ptr->latency_cnt = 5;
	}
	smc_cfg_ptr->bcr = smc_cfg_ptr->latency_cnt << APTIC_LATENCY_COUNTER_SHIFT;

	SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],(SMC_T_SAMPLE_DLY_W_MASK|SMC_T_SAMPLE_DLY_R_MASK),((0x1 << SMC_T_SAMPLE_DLY_W_SHIFT) | (0x1 << SMC_T_SAMPLE_DLY_R_SHIFT)));	
	smc_cfg_ptr->cfg0		    &= ~(SMC_CS_POSITION_MASK|SMC_CLK_MODE_MASK|SMC_CLK_OUT_EN_MASK|
									 SMC_CLK_OUT_MODE_MASK|SMC_SAMPLE_RST_MASK|SMC_DATA_SRC_MODE_MASK|
									 SMC_CLK_OUT_DLY_MASK|SMC_CLK_SAMPLE_SEL_MASK);
	if(smc_cfg_ptr->workmode == ASYNC_SINGLE)
	{
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_FIRST_W_MASK,(0xF << SMC_T_FIRST_W_SHIFT));
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_SAMPLE_STP_MASK,(0x1 << SMC_T_SAMPLE_STP_SHIFT));
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_WAIT_DLY_R_MASK,(0x1 << SMC_T_WAIT_DLY_R_SHIFT));
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_W_MASK,(0x1 << SMC_T_NEXT_W_SHIFT));
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_R_MASK,(0x1 << SMC_T_NEXT_R_SHIFT));
		
		smc_cfg_ptr->cfgx_cs1[2]  = 0xA0744F;
		smc_cfg_ptr->dl[0] 	      &= ~SMC_DLX_DLY_INV_MASK;
		smc_cfg_ptr->dl[1]		  &= ~SMC_DLX_DLY_INV_MASK;
		smc_cfg_ptr->cfgx_cs1[0]  &= ~SMC_WAIT_EN_R_MASK;
		smc_cfg_ptr->bcr		  |= (POL_HIGH << APTIC_WAIT_POLARITY_SHIFT);
		smc_cfg_ptr->cfgx_cs1[0]  |= (POL_HIGH << SMC_WAIT_POL_SHIFT);
	}
	else
	{
		smc_cfg_ptr->cfg0		  |= ((0x2 << SMC_CLK_OUT_DLY_SHIFT)|SMC_SAMPLE_RST_MASK|
											   SMC_CLK_OUT_MODE_MASK|SMC_CLK_OUT_EN_MASK);
		smc_cfg_ptr->dl[0]       |= SMC_DLX_DLY_INV_MASK;
		smc_cfg_ptr->dl[1]       |= SMC_DLX_DLY_INV_MASK;
		
		if(smc_cfg_ptr->clk_mode == HALF_HCLK)
		{
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_FIRST_W_MASK,((2*(smc_cfg_ptr->latency_cnt+1+1)) << SMC_T_FIRST_W_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_WAIT_DLY_R_MASK,(0x2 << SMC_T_WAIT_DLY_R_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_W_MASK,(0x2 << SMC_T_NEXT_W_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_R_MASK,(0x2 << SMC_T_NEXT_R_SHIFT));

			smc_cfg_ptr->cfgx_cs1[2] = 0xA0202A;
			smc_cfg_ptr->cfg0		 |= SMC_CLK_MODE_MASK;
		}
		else if(smc_cfg_ptr->clk_mode == HCLK_2X)
		{
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_FIRST_W_MASK,((smc_cfg_ptr->latency_cnt+1+1) << SMC_T_FIRST_W_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_WAIT_DLY_R_MASK,(0x1 << SMC_T_WAIT_DLY_R_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_W_MASK,(0x1 << SMC_T_NEXT_W_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_R_MASK,(0x1 << SMC_T_NEXT_R_SHIFT));
			
			smc_cfg_ptr->cfgx_cs1[2] = 0x501015;
			smc_cfg_ptr->cfg0		  &= ~SMC_CLK_OUT_EN_MASK;

			smc_cfg_ptr->cfgx_cs1[1] &= ~(0x1 << SMC_T_SAMPLE_DLY_R_SHIFT);
			smc_cfg_ptr->cfgx_cs1[1] &= ~(0x1 << SMC_T_SAMPLE_DLY_W_SHIFT);
			smc_cfg_ptr->cfg1		 &= ~(SMC_CLK_2X_EN_MASK|SMC_CLK_OUT_2X_EN_MASK|
										  SMC_CLK_SAMPLE_2X_EN_MASK|SMC_CLK_SAMPLE_2X_PHASE_MASK);
			smc_cfg_ptr->cfg1		 |= (SMC_CLK_2X_EN_MASK|SMC_CLK_OUT_2X_EN_MASK|SMC_CLK_SAMPLE_2X_EN_MASK);
			smc_cfg_ptr->dl[0]	     &= ~SMC_DLX_DLY_INV_MASK;
			smc_cfg_ptr->dl[1]		 &= ~SMC_DLX_DLY_INV_MASK;
		}
		else
		{
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_FIRST_W_MASK,((smc_cfg_ptr->latency_cnt+1+1) << SMC_T_FIRST_W_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_WAIT_DLY_R_MASK,(0x1 << SMC_T_WAIT_DLY_R_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_W_MASK,(0x1 << SMC_T_NEXT_W_SHIFT));
			SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_NEXT_R_MASK,(0x1 << SMC_T_NEXT_R_SHIFT));
			smc_cfg_ptr->cfgx_cs1[2] = 0x501015;
		}
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_SAMPLE_STP_MASK,(0x2 << SMC_T_SAMPLE_STP_SHIFT));
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[0],SMC_WAIT_EN_R_MASK,(0x1 << SMC_WAIT_EN_R_SHIFT));
		
		smc_cfg_ptr->bcr			 |= (POL_LOW << APTIC_WAIT_POLARITY_SHIFT);
		smc_cfg_ptr->cfgx_cs1[0]     |= (POL_LOW << SMC_WAIT_POL_SHIFT);
		
	}

	if(smc_cfg_ptr->mem_type == DDR_pSRAM)
	{
		smc_cfg_ptr->cfgx_cs1[0]     |= SMC_DDR_EN_MASK;
		smc_cfg_ptr->dl_dly_time     = 0x1;
		smc_cfg_ptr->cfg2            |= SMC_DLL_AUTO_CLR_EN_MASK;
		SMC_CLR_OR(smc_cfg_ptr->cfg2,SMC_DLL_CPST_THR_MASK,(0x3 << SMC_DLL_CPST_THR_SHIFT));
		
		smc_cfg_ptr->cfg2            |= (SMC_DLL_CPST_EN_MASK|SMC_DLL_CLR_MASK|SMC_DLL_EN_MASK);
		smc_cfg_ptr->cfg0            |= SMC_SAMPLE_AUTO_RST_EN_MASK;
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_WAIT_DLY_R_MASK,(0x2 << SMC_T_WAIT_DLY_R_SHIFT));
		SMC_CLR_OR(smc_cfg_ptr->cfgx_cs1[1],SMC_T_FIRST_W_MASK,((smc_cfg_ptr->latency_cnt + 1) << SMC_T_FIRST_W_SHIFT));
		
		smc_cfg_ptr->cfgx_cs1[3]     = 0xED000000;
		smc_cfg_ptr->dl[2]			 |= SMC_DLX_DLY_INV_MASK;
	}

#ifndef FPGA_VERIFICATION
	smc_cfg_ptr->dl_dly_sel= (smc_cfg_ptr->dl_dly_time== 0) ? 0 :(smc_cfg_ptr->clk_freq / (1000000000 / 128 / smc_cfg_ptr->dl_dly_time));
#endif
	smc_cfg_ptr->cfgx_cs1[1] |= SMC_T_FIRST_R_MASK;
	smc_cfg_ptr->cfg0        |= 0x22220000;
	smc_cfg_ptr->bcr |= (2 << APTIC_REGISTER_SELECT_SHIFT) | (((smc_cfg_ptr->workmode == ASYNC_SINGLE) ? 1:0) << APTIC_OPERATING_MODE_SHIFT) | 
						(smc_cfg_ptr->il_type << APTIC_INITIAL_LATENCY_SHIFT) | 
					 	(smc_cfg_ptr->mem_type << APTIC_MEM_TYPE_SHIFT) |
					 	(1 << APTIC_WAIT_CONFIGURATION_SHIFT) |
					 	(1 << APTIC_DRIVE_STRENGTH_SHIFT) |
					 	(smc_cfg_ptr->wrapmode << APTIC_BURST_WRAP_SHIFT) |
					 	(smc_cfg_ptr->bl << APTIC_BURST_LENGTH_SHIFT);
	
	smc_cfg_ptr->rcr = 1 << 4;
	smc_cfg_ptr->cfgx_cs1[0] &= ~(SMC_ROW_LENGTH_MASK|SMC_BURST_MODE_W_MASK|SMC_BURST_LENGTH_W_MASK|
										   SMC_BURST_MODE_R_MASK|SMC_BURST_LENGTH_R_MASK|SMC_MODE_W_MASK|SMC_MODE_R_MASK);
	smc_cfg_ptr->cfgx_cs1[0] |= SMC_ADMUX_EN_MASK |							
								(smc_cfg_ptr->rowlength << SMC_ROW_LENGTH_SHIFT) |
								SMC_ROW_DETECT_EN_W_MASK | SMC_ROW_DETECT_EN_R_MASK |
								(smc_cfg_ptr->wrapmode << SMC_BURST_MODE_W_SHIFT) |
								(smc_cfg_ptr->bl << SMC_BURST_LENGTH_W_SHIFT) |
								(smc_cfg_ptr->wrapmode << SMC_BURST_MODE_R_SHIFT) |
								(smc_cfg_ptr->bl << SMC_BURST_LENGTH_R_SHIFT) |
								(smc_cfg_ptr->workmode << SMC_MODE_W_SHIFT) |
								(smc_cfg_ptr->workmode << SMC_MODE_R_SHIFT);
	MMU_DisableIDCM();
							
	// default config
	smc_default_config();

	// external memory config CRE control register enable
	smc_cfg_ptr->smc_cfg->cfgx_cs1[0] |= SMC_CRE_EN_MASK;
	*(volatile uint16 *)(SMC_PSRAM_START_ADDRESS + (smc_cfg_ptr->bcr << 1)) = 0;
	for(smc_cfg_ptr->index= 0; smc_cfg_ptr->index < 10; smc_cfg_ptr->index++){}
	*(volatile uint16 *)(SMC_PSRAM_START_ADDRESS + (smc_cfg_ptr->rcr << 1)) = 0;
	for(smc_cfg_ptr->index = 0; smc_cfg_ptr->index < 10; smc_cfg_ptr->index++){}
	smc_cfg_ptr->smc_cfg->cfgx_cs1[0] &= ~SMC_CRE_EN_MASK;
	
	for(smc_cfg_ptr->index = 0; smc_cfg_ptr->index < 10; smc_cfg_ptr->index++){}

#ifdef FPGA_VERIFICATION	
	smc_cfg_ptr->smc_cfg->dl[2] &= ~SMC_DLX_DLY_CPST_EN_MASK;
#else
	/*Config DLL*/
	smc_cfg_ptr->smc_cfg->dl[1] |= (smc_cfg_ptr->mem_type << SMC_DLX_DLY_CPST_EN_SHIFT) | (smc_cfg_ptr->dl_dly_sel << SMC_DLX_DLY_SEL_SHIFT);
	smc_cfg_ptr->smc_cfg->dl[2] |= (smc_cfg_ptr->mem_type << SMC_DLX_DLY_CPST_EN_SHIFT) | (smc_cfg_ptr->dl_dly_sel << SMC_DLX_DLY_SEL_SHIFT);
	smc_cfg_ptr->smc_cfg->dl[3] |= (smc_cfg_ptr->mem_type << SMC_DLX_DLY_CPST_EN_SHIFT) | (smc_cfg_ptr->dl_dly_sel << SMC_DLX_DLY_SEL_SHIFT);
	smc_cfg_ptr->smc_cfg->dl[4] |= (smc_cfg_ptr->mem_type << SMC_DLX_DLY_CPST_EN_SHIFT) | (smc_cfg_ptr->dl_dly_sel << SMC_DLX_DLY_SEL_SHIFT);
	smc_cfg_ptr->smc_cfg->dl[5] |= (smc_cfg_ptr->mem_type << SMC_DLX_DLY_CPST_EN_SHIFT) | (smc_cfg_ptr->dl_dly_sel << SMC_DLX_DLY_SEL_SHIFT);
	smc_cfg_ptr->smc_cfg->dl[6] |= (smc_cfg_ptr->mem_type << SMC_DLX_DLY_CPST_EN_SHIFT) | (smc_cfg_ptr->dl_dly_sel << SMC_DLX_DLY_SEL_SHIFT);	

	smc_cfg_ptr->smc_cfg->cfg2 = smc_cfg_ptr->cfg2;
	smc_cfg_ptr->smc_cfg->cfg2 &= ~SMC_DLL_CLR_MASK;
#endif	
	/*Config SMC*/
	smc_cfg_ptr->smc_cfg->cfg0  = smc_cfg_ptr->cfg0;
	smc_cfg_ptr->smc_cfg->dl[0] |= smc_cfg_ptr->dl[0];
	smc_cfg_ptr->smc_cfg->dl[1] |= smc_cfg_ptr->dl[1];
	smc_cfg_ptr->smc_cfg->cfg1  = (smc_cfg_ptr->cfg1 | (0x1 << SMC_DDR_WDATA_DLY_SHIFT));
	
	smc_cfg_ptr->smc_cfg->cfgx_cs1[0] = smc_cfg_ptr->cfgx_cs1[0];
	smc_cfg_ptr->smc_cfg->cfgx_cs1[1] = smc_cfg_ptr->cfgx_cs1[1];
	smc_cfg_ptr->smc_cfg->cfgx_cs1[2] = smc_cfg_ptr->cfgx_cs1[2];
	smc_cfg_ptr->smc_cfg->cfgx_cs1[3] = smc_cfg_ptr->cfgx_cs1[3];

	smc_cfg_ptr->smc_cfg->cfg0 &= ~SMC_SAMPLE_RST_MASK;

	for(smc_cfg_ptr->index = 0; smc_cfg_ptr->index < 100; smc_cfg_ptr->index++)
	{
	}	
	
	MMU_EnableIDCM();
}

LOCAL SMC_CFG_T smc_cfg_info = {0};
PUBLIC void SMC_Init(void)
{
	smc_cfg_info.mem_type =  SDR_pSRAM;
	smc_cfg_info.workmode = BURST;
	smc_cfg_info.il_type = VARIABLE;
	smc_cfg_info.wrapmode = NOWRAP;
	smc_cfg_info.bl = CONTINUOUS;
	smc_cfg_info.rowlength = RL256;
	smc_cfg_info.clk_mode = HCLK_2X;
	smc_cfg_info.smc_cfg = (SMC_CTRL_REG_PTR_T)((uint32)0x20000000);
	
	SMC_Config(&smc_cfg_info);
}

#pragma arm section code

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

