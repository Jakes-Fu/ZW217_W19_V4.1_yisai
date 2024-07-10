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
#include "smc_reg_sc6531efm.h"
#include "chip_plf_export.h"
#include "chip_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                           *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define SMC_PSRAM_START_ADDRESS   ((*(volatile uint32 *)0x205000E0 & 0x1) ? 0x14000000 : 0x04000000)
#define PSRAM_FLAG_REG 0x20500168

const PSRAM_SPEC psramSpec[] =
{
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	 ID   wtw  wtr  rtw  rtr  clk_out_dly
      |   |    |    |    |    |    clk_out_mode
      |   |    |    |    |    |    |    dll_en
      |   |    |    |    |    |    |          dll_cpst_thr
      |   |    |    |    |    |    |              dll_init_val
      |   |    |    |    |    |    |                 dl[0]-------------------------dl[6]
      |   |    |    |    |    |    |                 |                             |    psram_if          row_length
      |   |    |    |    |    |    |                 |                             |                              burst_length
      |   |    |    |    |    |    |                 |                             |                                          wrap_mode
      |   |    |    |    |    |    |                 |                             |                                                  work_mode
      |   |    |    |    |    |    |                 |                             |                                                         r_wait_dly
      |   |    |    |    |    |    |                 |                             |                                                              buffer_read_sel
      |   |    |    |    |    |    |                 |                             |                                                                   sample_dly
      |   |    |    |    |    |    |                 |                             |                                                                        sample_setup_time
      |   |    |    |    |    |    |                 |                             |                                                                             next_write_dly
      |   |    |    |    |    |    |                 |                             |                                                                                  first_write_dly
      |   |    |    |    |    |    |                 |                             |                                                                                       next_read_dly
      |   |    |    |    |    |    |                 |                             |                                                                                            first_read_dly
      |   |    |    |    |    |    |                 |                             |                                                                                                  drive_strength
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	{0x6, 0x5, 0x5, 0x5, 0x5, 0x5, 0x2, TRUE,  0, 0, 0x0, 0x0, 0x50,0x50,0x50,0x0, 0x0, INTERFACE_AADMUX, RL1024, CONTINUOUS, NOWRAP, BURST, 0x0, 0x5, 0x2, 0x6, 0x1, 0x3, 0x1, 0x10, 0x34},  //winbond
	{0xD, 0x3, 0x3, 0x3, 0x5, 0x4, 0x3, TRUE , 0, 0, 0x0, 0x0, 0x50,0x50,0x50,0x0, 0x0, INTERFACE_AADMUX, RL1024, CONTINUOUS, NOWRAP, BURST, 0x0, 0x7, 0x1, 0x5, 0x1, 0x3, 0x1, 0xa,  0x12},  //aptic
	{0x0, 0,   0,   0,   0,   0,    0,  0,     0, 0, 0,   0,   0,   0,   0,   0,   0,   0,                0,      0,          0,      0,     0,   0,   0,   0,   0,   0,   0,   0,    0}
};

PSRAM_SPEC* GetPsramParam(uint32 ID)
{
	uint32 i;

    i = 0;
    while(0 != psramSpec[i].vendorID)
    {
        if(ID == psramSpec[i].vendorID)
        {
            return (PSRAM_SPEC*)&psramSpec[i];
        }
        i++;
    }
    return (PSRAM_SPEC*)0;
}

static uint32 smc_read_id(SMC_CTRL_REG_PTR_T ctrl_ptr)
{
	uint32 id, i;

	//chose method2 to initialize psram
	ctrl_ptr->ce_len_cfg = 0x3 << SMC_CE_LEN_CFG_SHIFT;    //CE# low length
	
	ctrl_ptr->init2_start &= ~(SMC_INIT2_START);   //initialization method 2
	ctrl_ptr->init2_start |= SMC_INIT2_START;      //initialization method 2
	for(i = 0; i < 1000; i++){}
	
	ctrl_ptr->opt_sel &= ~(SMC_OPT_SEL);       //register read or write
	
	id = (*(volatile uint8 *)(SMC_PSRAM_START_ADDRESS + 0x1) & 0x1F); //read id
	for(i = 0; i < 100; i++){}

	ctrl_ptr->opt_sel |= SMC_OPT_SEL;          //memory read or write

	return id;
}

static void smc_set_id(uint32 id, SMC_CTRL_REG_PTR_T ctrl_ptr)
{
	if(0x6 == id)
	{
		ctrl_ptr->mem_sel_preserve = 0x5a5a;
		ctrl_ptr->mem_sel |= SMC_MEM_SEL;  	  //winbond
	}
	else if(0xD == id)
	{
		ctrl_ptr->mem_sel_preserve = 0x5a5a;
		ctrl_ptr->mem_sel &= ~(SMC_MEM_SEL);  //aptic
	}
}

static void smc_read_density(SMC_CTRL_REG_PTR_T ctrl_ptr, uint32 id)
{
	uint32 density1, density2, density, i;

	*(volatile uint32 *)(PSRAM_FLAG_REG) = 0;
	
	ctrl_ptr->opt_sel &= ~(SMC_OPT_SEL);       //register read or write
	
	density1 = (*(volatile uint8 *)(SMC_PSRAM_START_ADDRESS + 0x1) & 0x20);
	for(i = 0; i < 100; i++){}
	density2 = (*(volatile uint8 *)(SMC_PSRAM_START_ADDRESS + 0x2) & 0x7);
	for(i = 0; i < 100; i++){}

	ctrl_ptr->opt_sel |= SMC_OPT_SEL;          //memory read or write

	density = (density1 >> 2) | density2;

	*(volatile uint32 *)(PSRAM_FLAG_REG) = (id << 8) | density;
}

static void smc_default_config(SMC_CTRL_REG_PTR_T ctrl_ptr)
{
	uint32 i = 0;
	
	ctrl_ptr->cfg0 = 0x3333a0c0;     //cfg0
	ctrl_ptr->cfg1 = 0x00000007;     //cfg1

	ctrl_ptr->dl[0] = 0xF;            //delay 0
	ctrl_ptr->dl[1] = 0xF;            //delay 1
	ctrl_ptr->dl[2] = 0xF;            //delay 2
	ctrl_ptr->dl[3] = 0xA;            //delay 3
	ctrl_ptr->dl[4] = 0xA;            //delay 4
	ctrl_ptr->dl[5] = 0xA;            //delay 5
	ctrl_ptr->dl[6] = 0xA;            //delay 6
	
	ctrl_ptr->cfg0_cs1 = 0x0acc177f;     //cs1_cfg0
	ctrl_ptr->cfg3_cs1 = 0x1d000000;	 //cs1_cfg3

	ctrl_ptr->time_cfg_cs1 = 0x0005434a;   //time_cs1_cfg

	ctrl_ptr->cfg0 |= (SMC_SAMPLE_RST);    //cfg0 sample fifo reset
	ctrl_ptr->cfg0 &= ~(SMC_SAMPLE_RST);   //cfg0 sample fifo reset

	for(i = 0; i < 100; i++){}
	
}

PUBLIC void SMC_Init(void)
{	
	uint32 id,i;
	PSRAM_SPEC * s_spec = NULL;
	SMC_CTRL_REG_PTR_T smc_ctrl_ptr = NULL;

	smc_ctrl_ptr = (SMC_CTRL_REG_PTR_T)((uint32)0x20000000);
	
	smc_default_config(smc_ctrl_ptr);

	id = smc_read_id(smc_ctrl_ptr);

	smc_set_id(id, smc_ctrl_ptr);

	smc_read_density(smc_ctrl_ptr, id);

	s_spec = GetPsramParam(id);

	smc_ctrl_ptr->cfg0 &=	~(SMC_WRITE_TO_WRITE_MASK | SMC_WRITE_TO_READ_MASK | SMC_READ_TO_WRITE_MASK | SMC_READ_TO_READ_MASK | 
							SMC_CLK_SAMPLE_SEL_MASK | SMC_CLK_OUT_DLY_MASK | SMC_CLK_OUT_MODE_MASK);
	smc_ctrl_ptr->cfg0 |= 	s_spec->wtw << SMC_WRITE_TO_WRITE_SHIFT |
							s_spec->wtr << SMC_WRITE_TO_READ_SHIFT | 
							s_spec->rtw << SMC_READ_TO_WRITE_SHIFT | 
							s_spec->rtr << SMC_READ_TO_READ_SHIFT | 
							s_spec->clk_out_dly << SMC_CLK_OUT_DLY_SHIFT | 
							s_spec->clk_out_mode << SMC_CLK_OUT_MODE_SHIFT;

	smc_ctrl_ptr->dl[0] &= ~(SMC_DLX_DLY_SEL_MASK);
	smc_ctrl_ptr->dl[1] &= ~(SMC_DLX_DLY_SEL_MASK);
	smc_ctrl_ptr->dl[2] &= ~(SMC_DLX_DLY_SEL_MASK);
	smc_ctrl_ptr->dl[3] &= ~(SMC_DLX_DLY_SEL_MASK);
	smc_ctrl_ptr->dl[4] &= ~(SMC_DLX_DLY_SEL_MASK);
	smc_ctrl_ptr->dl[5] &= ~(SMC_DLX_DLY_SEL_MASK);
	smc_ctrl_ptr->dl[6] &= ~(SMC_DLX_DLY_SEL_MASK);
	smc_ctrl_ptr->dl[0] |= s_spec->dly_dl_sel[0];
	smc_ctrl_ptr->dl[1] |= s_spec->dly_dl_sel[1];
	smc_ctrl_ptr->dl[2] |= s_spec->dly_dl_sel[2];
	smc_ctrl_ptr->dl[3] |= s_spec->dly_dl_sel[3];
	smc_ctrl_ptr->dl[4] |= s_spec->dly_dl_sel[4];
	smc_ctrl_ptr->dl[5] |= s_spec->dly_dl_sel[5];
	smc_ctrl_ptr->dl[6] |= s_spec->dly_dl_sel[6];

	if(s_spec->dll_en)
	{
		smc_ctrl_ptr->cfg2 &=	~(SMC_DLL_CPST_THR_MASK | SMC_DLL_INIT_MASK | SMC_DLL_CPST_EN | SMC_DLL_EN);
		smc_ctrl_ptr->cfg2 |= 	s_spec->dll_cpst_thr << SMC_DLL_CPST_THR_SHIFT |
								s_spec->dll_init_val;

		//enable DLL
		//smc_ctrl_ptr->dl[0] |= SMC_DLX_DLY_CPST_EN;
		//smc_ctrl_ptr->dl[1] |= SMC_DLX_DLY_CPST_EN;
		smc_ctrl_ptr->dl[2] |= SMC_DLX_DLY_CPST_EN;
		smc_ctrl_ptr->dl[3] |= SMC_DLX_DLY_CPST_EN;
		smc_ctrl_ptr->dl[4] |= SMC_DLX_DLY_CPST_EN;
		//smc_ctrl_ptr->dl[5] |= SMC_DLX_DLY_CPST_EN;
		//smc_ctrl_ptr->dl[6] |= SMC_DLX_DLY_CPST_EN;

		smc_ctrl_ptr->cfg2 |= 	SMC_DLL_CPST_EN | 
								SMC_DLL_EN;

		while(!(smc_ctrl_ptr->cfg2 | BIT_31));
	}

	if(INTERFACE_AADMUX == s_spec->psram_if)
	{
		smc_ctrl_ptr->cfg0_cs1 |= SMC_ADMUX_EN | SMC_AADMUX_EN;
	}

	smc_ctrl_ptr->cfg0_cs1 &= 	~(SMC_ROW_LENGTH_MASK | 
								SMC_BURST_LENGTH_W_MASK | SMC_BURST_LENGTH_R_MASK | 
								SMC_BURST_MODE_W_MASK | SMC_BURST_MODE_R_MASK | 
								SMC_MODE_W_MASK | SMC_MODE_R_MASK);
	smc_ctrl_ptr->cfg0_cs1 |= 	(s_spec->row_length << SMC_ROW_LENGTH_SHIFT) |
								(s_spec->burst_length << SMC_BURST_LENGTH_W_SHIFT) |
								(s_spec->burst_length << SMC_BURST_LENGTH_R_SHIFT) |
								(s_spec->wrap_mode << SMC_BURST_MODE_W_SHIFT) |
								(s_spec->wrap_mode << SMC_BURST_MODE_R_SHIFT) |
								(s_spec->work_mode << SMC_MODE_W_SHIFT) |
								(s_spec->work_mode << SMC_MODE_R_SHIFT);

	smc_ctrl_ptr->cfg0_cs1 |=	SMC_DDR_EN |
								SMC_ROW_DETECT_EN_W | 
								SMC_ROW_DETECT_EN_R;

	smc_ctrl_ptr->cfg1_cs1 &= 	~(SMC_T_WAIT_DLY_R_MASK);
	smc_ctrl_ptr->cfg1_cs1 |= 	s_spec->r_wait_dly << SMC_T_WAIT_DLY_R_SHIFT;

	smc_ctrl_ptr->cfg3_cs1 &= 	~(SMC_T_BUFFER_READ_SEL_MASK | SMC_T_SAMPLE_DLY_DDR_MASK);
	smc_ctrl_ptr->cfg3_cs1 |= 	s_spec->buffer_read_sel << SMC_T_BUFFER_READ_SEL_SHIFT | 
								s_spec->sample_dly << SMC_T_SAMPLE_DLY_DDR_SHIFT;

	smc_ctrl_ptr->time_cfg_cs1 &=	~(SMC_SAMPLE_STP_MASK | SMC_2ND_DLY_W_MASK | 
										SMC_1ST_DLY_W_MASK | SMC_2ND_DLY_R_MASK |
										SMC_1ST_DLY_R_MASK);
	smc_ctrl_ptr->time_cfg_cs1 |=	(s_spec->sample_setup_time << SMC_SAMPLE_STP_SHIFT ) | 
									(s_spec->next_write_dly << SMC_2ND_DLY_W_SHIFT) |
									(s_spec->first_write_dly << SMC_1ST_DLY_W_SHIFT) |
									(s_spec->next_read_dly << SMC_2ND_DLY_R_SHIFT) |
									s_spec->first_read_dly;

	//psram drive strength
	smc_ctrl_ptr->opt_sel &= ~(SMC_OPT_SEL);       //register read or write
	*(volatile uint8 *)(SMC_PSRAM_START_ADDRESS) = s_spec->drive_strength;
	for(i = 0; i < 100; i++){}
	
	smc_ctrl_ptr->opt_sel |= SMC_OPT_SEL;          //memory read or write

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

