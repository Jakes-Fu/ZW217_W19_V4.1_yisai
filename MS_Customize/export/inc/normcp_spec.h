/******************************************************************************
 ** File Name:      normcp_spec.h							                                                   *
 ** Author:         	Yiyue.He										                                                   *
 ** DATE:           16/12/2008                                                									*
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.     							*
 ** Description:    NORMCP--|_ Norflash   Info  Descriptions 			                  					*					
 **                                   |_ Psram         													*
 **     			The info in this file should be indepedent with all the platform.						*
 **				They are only abstracted from the attribute borned in the flash and psram, 			*
 **                 	which can get from the spec released by the manufactory.						*
 **                                                                           										*
 **Function:		The content in the file are used by the platform memory interface 's Driver			*
 **			      [called EMC OR EMI OR MC etc.], and the flash/Sram config to platform code 		*
 **				[called custom mem cfg etc.]. The accessing way  can be variable as the designer's 	*
 **				reasonable plan.        														*
 **																						*
 **Use:			This file is expected as developing trend, which is builded up from different normcps 	*
 **				one by one, when the mcp item has been described, just use it,if not ,you can refer 	*
 **				to mcp spec,then write a new mcp item like a existed one.						*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           		NAME             DESCRIPTION                               							*
 ** 16/12/2008     	Yiyue.He        	 Create.                                   							*
 ******************************************************************************/
 
#ifndef _NORMCP_SPEC_H_
#define _NORMCP_SPEC_H_

/**---------------------------------------------------------------------------	*
 **                         				Dependencies                                      				*
 **---------------------------------------------------------------------------	*/
#include "os_api.h"   /* basic type: platform and language Dependency*/ 
/*Anything else Dependencies offend against design original intention */


/**---------------------------------------------------------------------------	*
 **                         				Compiler Flag                                    				*
 **---------------------------------------------------------------------------	*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------	*
 **                         				Mocro Define										*
 **---------------------------------------------------------------------------	*/
#define BIT_RANGE(_bit_h,_bit_l)		((_bit_h)-(_bit_l)+1)

/**---------------------------------------------------------------------------	*
 **                         				Data Structures                                   				*
 **---------------------------------------------------------------------------	*/
typedef enum
{
	NORMCP_ADP_MODE = 0x0,	
	NORMCP_ADM_MODE
}NORMCP_AD_MODE;

typedef enum
{
	FLASH_INTEL_MODE = 0x0,
	FLASH_AMD_MODE,
	FLASH_INTEL_M18_MODE /*start added by victorxu*/
}FLASH_DRV_MODE;

typedef enum
{
	MEM_NORMAL_MODE = 0x0,		
	MEM_PAGE_MODE,			
	MEM_BURST_MODE			
}NORMCP_ACCESS_MODE_E;

typedef enum
{
	BURST_LEN_1 = 0X0,
	BURST_LEN_4 ,
	BURST_LEN_8 ,
	BURST_LEN_16 ,
	BURST_LEN_32 ,
	BURST_LEN_CONTINUOUS
}NORMCP_BURST_LEN_E;

typedef enum
{
	FLASH_WRITE_SINGLE_MODE = 0x0,
	FLASH_WRITE_BUFFER_MODE = 0x1,
	IGNORE_OTHER_PARAM = 0x2
}FLASH_WRITE_MODE_E;

typedef enum
{
	FLASH_WRITE_BUFFER_NONE   = 0x00,
	FLASH_WRITE_BUFFER_LEN_16 = 16,		/*byte*/
	FLASH_WRITE_BUFFER_LEN_32 = 32,
	FLASH_WRITE_BUFFER_LEN_64 = 64,
	FLASH_WRITE_BUFFER_LEN_1024 = 1024 /*start added by victorxu*/
}FLASH_BUFFER_LEN_E;

typedef enum
{
	WAIT_ACTIVE_LOW 	= 0,
	WAIT_ACTIVE_HIGH 	= 1,
	RDY_ACTIVE_LOW 		= WAIT_ACTIVE_HIGH,
	RDY_ACTIVE_HIGH 	= WAIT_ACTIVE_LOW
}NORMCP_WAIT_POLARITY_E;

typedef enum
{
	ROW_LEN_64 = 0,// 16-bit
	ROW_LEN_128, 
	ROW_LEN_256, 
	ROW_LEN_512, 
	ROW_LEN_1024
}SRAM_ROW_BOUNDARY_LEN_E;

typedef void (*SRAM_NORFLASH_CR_SET_FUNC)(
	uint32 start_offset,
	uint32 crI,
	uint32 crII
	);

typedef struct CONFIG_FUNC_INPARAM
{
	NORMCP_ACCESS_MODE_E		access_mode;
	NORMCP_WAIT_POLARITY_E		wait_polarity;
	uint8 						clk;	
	BOOLEAN						is_wait_one_cycle_delay;	
	BOOLEAN						is_clk_rising_edge;
	BOOLEAN						is_burst_wrap;
	NORMCP_BURST_LEN_E			burst_len;
	uint32 						base_addr_offset;
}SRAM_CFG_FUNC_INPARAM_T, FLASH_CFG_FUNC_INPARAM_T,* SRAM_CFG_FUNC_INPARAM_PTR, * FLASH_CFG_FUNC_INPARAM_PTR;

typedef struct CONFIG_FUNC_OUTPARAM
{
	uint8 						first_data_latency;	//also is the initial wait states count
	BOOLEAN						is_wait_used;
	SRAM_NORFLASH_CR_SET_FUNC	cs_cr_set_func;
	uint32						crI;
	uint32						crII;
}SRAM_CFG_FUNC_OUTPARAM_T, FLASH_CFG_FUNC_OUTPARAM_T,* SRAM_CFG_FUNC_OUTPARAM_PTR, * FLASH_CFG_FUNC_OUTPARAM_PTR;


typedef BOOLEAN (*SRAM_CR_CFG_FUNC)(
	SRAM_CFG_FUNC_INPARAM_PTR		sram_cfg_input_ptr,
	SRAM_CFG_FUNC_OUTPARAM_PTR	sram_cfg_output_ptr
	);

typedef BOOLEAN (*FLASH_CR_CFG_FUNC)(
	FLASH_CFG_FUNC_INPARAM_PTR		flash_cfg_input_ptr,
	FLASH_CFG_FUNC_OUTPARAM_PTR	flash_cfg_output_ptr
	);


typedef struct NORMCP_SPEC_s
{
	
	//@flash
	/* Flash id */
	uint32				manu_id;
	uint32				devc_id;
	uint32				extend_id;
	
	/* ADDR&DATA MODE*/ /*SET ADP OR ADM*/
	NORMCP_AD_MODE		ad_mode;
	
	/* Flash access Mode*/ /*SET INTEL OR AMD*/
	FLASH_DRV_MODE		flash_drv_mode;

	/* Flash write buffer or single Mode*/
	uint32				flash_write_mode;

	uint32				flash_write_buf_len;

	/*Sram row boundary len*/
	SRAM_ROW_BOUNDARY_LEN_E	sram_row_boundary_len;


	/*timing*/
	struct{		
		uint8			rd_cycle_time;			//tRC:	read cycle time
		uint8			rd_adv_pulse_time;		//tavp:	address lantch width(rd) 	also is ADV# low time
		uint8			page_rd_access_time;	//tAA:	
		uint8			page_intra_cycle_time;	//tpc:	Page cycle time
		uint8			rd_data_hold_time;		//tOH:	data output hold time from CE
	}tFlashAsynReadTiming;

	struct{		
		uint8			wr_cycle_time;			//tWC:	write cycle time	
		uint8			wr_adv_pulse_time;		//tavp:	address lantch width(wr) 	also is ADV# low time
		uint8			wr_ce_stp_time;			//tCS:	CE# Setup Time to WE# low
		uint8			wr_ce_hold_time;		//tCH:	CE# Hold Time from WE# high
	}tFlashAsynWriteTiming;	

	struct{
		uint8			max_sync_freq;			//max clk in Mhz =  1/tCLK(us)
	}tFlashSynTiming;

	struct{		
		uint8			rd_cycle_time;			//tRC:	read cycle time
		uint8			rd_adv_pulse_time;		//tavp:	address lantch width(rd) 	also is ADV# low time
		uint8			page_rd_access_time;	//tAA:	
		uint8			page_intra_cycle_time;	//tpc:	Page cycle time
		uint8			rd_data_hold_time;		//tOH:	data output hold time from CE		
	}tSramAsynReadTiming;

	struct{		
		uint8			wr_cycle_time;			//tWC:	write cycle time	
		uint8			wr_adv_pulse_time;		//tavp:	address lantch width(wr) 	also is ADV# low time
		uint8			wr_ce_stp_time;			//tCS:	CE# Setup Time to WE# low
		uint8			wr_ce_hold_time;		//tCH:	CE# Hold Time from WE# high
	}tSramAsynWriteTiming;	

	struct{
		uint8			max_sync_freq;			//max clk in Mhz =  1/tCLK(us)
	}tSramSynTiming;

	
	SRAM_CR_CFG_FUNC	sram_cr_cfg_func;
	
	FLASH_CR_CFG_FUNC	flash_cr_cfg_func;
	
}NORMCP_SPEC_T, *NORMCP_SPEC_PTR;

#define	SRAM_CR_SET_BY_EMC	((SRAM_NORFLASH_CR_SET_FUNC)(1))
/**---------------------------------------------------------------------------	*
 **                         				Constant Variables                                				*
 **---------------------------------------------------------------------------	*/

//@old never check in new emc-mem code mode
extern const NORMCP_SPEC_T g_normcp_KAD060700_spec;
extern const NORMCP_SPEC_T g_normcp_S71PLJ_spec;
extern const NORMCP_SPEC_T g_normcp_S71PLN_spec;
extern const NORMCP_SPEC_T g_normcp_L18_spec;
extern const NORMCP_SPEC_T g_normcp_L30_INTEL_spec;
extern const NORMCP_SPEC_T g_normcp_W18_INTEL_spec;
extern const NORMCP_SPEC_T g_normcp_W18_ST_spec;
//@checked
extern const NORMCP_SPEC_T g_normcp_k5l2833ata_af66_spec;
extern const NORMCP_SPEC_T g_normcp_k5l2833aba_af66_spec;
extern const NORMCP_SPEC_T g_normcp_k5n2833atm_aq11_spec;
extern const NORMCP_SPEC_T g_normcp_k5n2833abm_aq11_spec;
extern const NORMCP_SPEC_T g_normcp_m36l0r7050u3_spec;
extern const NORMCP_SPEC_T g_normcp_m36l0r7050l3_spec;
extern const NORMCP_SPEC_T g_normcp_m36l0r7050t4zaq_spec;
extern const NORMCP_SPEC_T g_normcp_m36l0r7050t4zaq_spec_1;
extern const NORMCP_SPEC_T g_normcp_m36l0r7050b4zaq_spec;
extern const NORMCP_SPEC_T g_normcp_m36l0r7050b4zaq_spec_1;
extern const NORMCP_SPEC_T g_normcp_s71ws128pc0hf3sr_spec;
extern const NORMCP_SPEC_T g_normcp_K5L2731CAM_spec;
extern const NORMCP_SPEC_T g_normcp_TSB_TV00570002_spec;
extern const NORMCP_SPEC_T g_normcp_PF38F4050L0YUQ3_spec;
extern const NORMCP_SPEC_T g_normcp_PF38F4050L0YUQ3_B_spec;
extern const NORMCP_SPEC_T g_normcp_PF38F3040L0YUQ3_B_spec;

extern const NORMCP_SPEC_T g_normcp_TSB_TV00560002_spec;
extern const NORMCP_SPEC_T g_normcp_TSB_TV00560003_spec;
extern const NORMCP_SPEC_T g_normcp_PF38F3050M0Y3Q_spec;
extern const NORMCP_SPEC_T g_normcp_PF38F3050M0Y0YE_spec;
extern const NORMCP_SPEC_T g_normcp_Memocom_2832B_spec;
extern const NORMCP_SPEC_T g_normcp_S71PL127N_spec;
extern const NORMCP_SPEC_T g_normcp_m36l0r8060u0_spec;
extern const NORMCP_SPEC_T g_normcp_CFEON_EN71GL128B0_spec;
extern const NORMCP_SPEC_T g_normcp_M36L0T70x0T3_spec;
extern const NORMCP_SPEC_T g_normcp_PF38F3040L0YUQ3_spec;
extern const NORMCP_SPEC_T g_normcp_K5L6331CAA_spec;
extern const NORMCP_SPEC_T g_normcp_TSB_TV00570002ADGB_spec;
extern const NORMCP_SPEC_T g_normcp_K5L5666CAM_spec;
extern const NORMCP_SPEC_T g_normcp_k5n5629atb_spec;
extern const NORMCP_SPEC_T g_normcp_k5n2866ate_bq12_spec;
extern const NORMCP_SPEC_T g_normcp_CFEON_EN71NS128C0_spec;
/**---------------------------------------------------------------------------	*
 **                         				Function Interface		                               			*
 **---------------------------------------------------------------------------	*/
 
/**---------------------------------------------------------------------------	*
 **                         				Compiler Flag                                     				*
 **---------------------------------------------------------------------------	*/
#ifdef __cplusplus
    }
#endif

#endif // _NORMCP_SPEC_H_

 
