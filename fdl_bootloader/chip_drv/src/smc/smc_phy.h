/******************************************************************************
 ** File Name:      EMC_test.h                                                *
 ** Author:         Johnny.Wang                                               *
 ** DATE:           2010-07-28                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
#ifndef _SDRAM_DRV_H_
#define _SDRAM_DRV_H_
#include "sci_types.h"

typedef unsigned long ul;
typedef unsigned long long ull;
typedef unsigned long volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;
#define UL_ONEBITS 0xffffffff
#define UL_LEN 32
#define ONE 0x00000001


/******************************************************************************
                            Enum define
******************************************************************************/
typedef enum
{
	SDR_pSRAM = 0,
	DDR_pSRAM,
	MAX_MEM_TYPE
}MEM_TYPE_E;

typedef enum{
    MEM_1K_BYTE  = 1024,
    MEM_1M_BYTE  = 1024*1024,
    MEM_1G_BYTE  = 1024*1024*1024
}MEM_SIZE_E;




typedef enum
{
    ZERO_TO_F = 0,
	F_TO_ZERO,
	FIVE_TO_A,
	A_TO_FIVE,
	FZERO_TO_ZEROF,
	ZEROF_TO_FZERO,
	BIGGER,
	LITTER
}BURST_DATA_TYPE_E;




typedef enum{
	ASYNC_SINGLE = 0,
	PAGE,
	SYNC_SINGLE,
	BURST,
	MAX_WORKMODE
}SMC_WORKMODE_E;

typedef enum{
	VARIABLE = 0,
	FIXED,	
	MAX_IL_TYPE
}SMC_INIT_LANTENCY_TYPE_E;

typedef enum{
	WRAP = 0,
	NOWRAP,
	
	MAX_WRAPMODE
}SMC_WRAPMODE_E;

typedef enum{
	BL4 = 0,
	BL8,
	BL16,
	BL32,	
	CONTINUOUS = 7,
	MAX_BL
}SMC_BURST_LENGTH_E;

typedef enum{
	RL64 = 0,
	RL128,
	RL256,
	RL512,
	RL1024,
	MAX_RL
}CRAM_ROW_LENGTH_E;

typedef enum{
	HCLK = 0,
	HALF_HCLK,
	HCLK_2X,
	MAX_CLK_MODE
}CLK_MODE_E;

typedef enum{
	POL_LOW = 0,
	POL_HIGH,
	MAX_POL
}WAIT_POL_E;

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
typedef struct
{
	volatile uint32 cfg0;
	volatile uint32 cfg1;
	volatile uint32 cfg2;
	volatile uint32 reserve0[4];
	volatile uint32 sts0;
	volatile uint32 dl[7];
	volatile uint32 reserve1;
	volatile uint32 cfgx_cs0[4];
	volatile uint32 cfgx_cs1[4];
	volatile uint32 cfgx_cs2[4];
	volatile uint32 cfgx_cs3[4];
}SMC_CTRL_REG_T,*SMC_CTRL_REG_PTR_T;

typedef struct{
	SMC_CTRL_REG_PTR_T smc_cfg;
	MEM_TYPE_E mem_type;
	SMC_WORKMODE_E workmode;
	SMC_INIT_LANTENCY_TYPE_E il_type;
	SMC_WRAPMODE_E wrapmode;
	SMC_BURST_LENGTH_E bl;
	CRAM_ROW_LENGTH_E rowlength;
	CLK_MODE_E clk_mode;
	uint32 index;
	uint32 latency_cnt;
	uint32 dl_dly_time;
	uint32 dl_dly_sel;
	uint32 clk_freq;
	uint32 bcr;
	uint32 rcr;
	uint32 cfg0;
	uint32 cfg1;
	uint32 cfg2;
	uint32 reserve0[4];
	uint32 sts0;
	uint32 dl[7];
	uint32 reserve1;
	uint32 cfgx_cs0[4];
	uint32 cfgx_cs1[4];
	uint32 cfgx_cs2[4];
	uint32 cfgx_cs3[4];
}SMC_CFG_T, *SMC_CFG_PTR_T;

typedef struct{
	uint32 smc_cfg0;
	uint32 smc_cfg2;
	uint32 smc_cfg0_cs1;
	uint32 smc_cfg1_cs1;
	uint32 smc_cfg2_cs1;
	uint32 smc_cfg3_cs1;
}SMC_REG_T;
#endif

/*******************************************************************************
                          Function declare
*******************************************************************************/
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
PUBLIC void SMC_Config(SMC_CFG_PTR_T smc_cfg_ptr);
#endif
PUBLIC void SMC_Init(void);

#endif // _SDRAM_DRV_H_

