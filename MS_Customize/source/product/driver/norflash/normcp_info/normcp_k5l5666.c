/******************************************************************************
 ** File Name:      normcp_k5l5666.c				                                             *
 ** Author:         	Yong.Li										                                                   *
 ** DATE:            13/01/2009                                                								*
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.     							*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           		NAME             DESCRIPTION                               							*
 ** 17/03/2010     	Yong.Li         	 Create.                                   							*
 ******************************************************************************/
#if defined(CHIP_ENDIAN_LITTLE)
typedef union utram_bcr{
	struct utram_bcr_bits_map {
		volatile  uint   burst_len			:BIT_RANGE(2,0);		
		volatile  uint   burst_wrap			:BIT_RANGE(3,3);		
		volatile  uint   driver_strenght		:BIT_RANGE(5,4);			
		volatile  uint   reserved3			:BIT_RANGE(7,6);		
		volatile  uint   wait_configuration	:BIT_RANGE(8,8);				
		volatile  uint   reserved2			:BIT_RANGE(9,9);		
		volatile  uint   wait_polority		:BIT_RANGE(10,10);			
		volatile  uint   latency_counter		:BIT_RANGE(13,11);			
		volatile  uint   init_latency			:BIT_RANGE(14,14);		
		volatile  uint   operating_mode		:BIT_RANGE(15,15);			
		volatile  uint   reserved1			:BIT_RANGE(17,16);		
		volatile  uint   register_select		:BIT_RANGE(19,18);			
		volatile  uint   noused				:BIT_RANGE(31,20);	
	}bits_map;
	volatile  uint32   sram_bcr_value;
}UTRAM_BCR_U;

//no need ,reserved for future description
typedef union utram_rcr{
	struct utram_rcr_bits_map {
		volatile  uint   PAR					:BIT_RANGE(2,0);
		volatile  uint   reserved3			:BIT_RANGE(3,3);		
		volatile  uint   DPD					:BIT_RANGE(4,4);
		volatile  uint   reserved2			:BIT_RANGE(6,5);		
		volatile  uint   page_eb				:BIT_RANGE(7,7);	
		volatile  uint   reserved1			:BIT_RANGE(17,8);		
		volatile  uint   register_select		:BIT_RANGE(19,18);			
		volatile  uint   reserved0			:BIT_RANGE(31,20);		
	}bits_map;
	volatile  uint32   sram_rcr_value;		
}UTRAM_RCR_U;

#else  //big endian
typedef union utram_bcr{
	struct utram_bcr_bits_map {
		volatile  uint   noused				:BIT_RANGE(31,20);
		volatile  uint   register_select		:BIT_RANGE(19,18);
		volatile  uint   reserved1			:BIT_RANGE(17,16);
		volatile  uint   operating_mode		:BIT_RANGE(15,15);
		volatile  uint   init_latency			:BIT_RANGE(14,14);
		volatile  uint   latency_counter		:BIT_RANGE(13,11);
		volatile  uint   wait_polority		:BIT_RANGE(10,10);
		volatile  uint   reserved2			:BIT_RANGE(9,9);
		volatile  uint   wait_configuration	:BIT_RANGE(8,8);
		volatile  uint   reserved3			:BIT_RANGE(7,6);
		volatile  uint   driver_strenght		:BIT_RANGE(5,4);		
		volatile  uint   burst_wrap			:BIT_RANGE(3,3);
		volatile  uint   burst_len			:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint32   sram_bcr_value;
}UTRAM_BCR_U;

//no need ,reserved for future description
typedef union utram_rcr{
	struct utram_rcr_bits_map {
		volatile  uint   reserved0			:BIT_RANGE(31,20);
		volatile  uint   register_select		:BIT_RANGE(19,18);
		volatile  uint   reserved1			:BIT_RANGE(17,8);
		volatile  uint   page_eb				:BIT_RANGE(7,7);
		volatile  uint   reserved2			:BIT_RANGE(6,5);
		volatile  uint   DPD					:BIT_RANGE(4,4);
		volatile  uint   reserved3			:BIT_RANGE(3,3);
		volatile  uint   PAR					:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint32   sram_rcr_value;		
}UTRAM_RCR_U;
#endif

LOCAL BOOLEAN utram_cr_cfg_func(
	SRAM_CFG_FUNC_INPARAM_PTR		sram_cfg_input_ptr,
	SRAM_CFG_FUNC_OUTPARAM_PTR		sram_cfg_output_ptr
	)
{	
	UTRAM_BCR_U sram_bcr;
	UTRAM_RCR_U sram_rcr;	//just uint32 type for rcr is enough	

	//@default value
	sram_bcr.sram_bcr_value = 0x89D1F;
	sram_rcr.sram_rcr_value = 0x10;
	
	//cfg by input one by one
	sram_bcr.bits_map.operating_mode = (sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)?0x0:0x1;
	
	if(sram_cfg_input_ptr->access_mode == MEM_PAGE_MODE)
	{
		sram_rcr.bits_map.page_eb = 1;
	}

	else if(sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)
	{		
		sram_bcr.bits_map.wait_polority = ( sram_cfg_input_ptr->wait_polarity == WAIT_ACTIVE_LOW )?0x0:0x1;

		sram_bcr.bits_map.wait_configuration = (sram_cfg_input_ptr->is_wait_one_cycle_delay)?0x1:0x0;

		if(!sram_cfg_input_ptr->is_clk_rising_edge)
			return SCI_FALSE;	//no support		
		
		sram_bcr.bits_map.burst_wrap = (sram_cfg_input_ptr->is_burst_wrap)?0x0:0x1;
		
		sram_bcr.bits_map.driver_strenght = 0x01;		

		switch(sram_cfg_input_ptr->burst_len)
		{
			case BURST_LEN_4:
				sram_bcr.bits_map.burst_len = 1;
				break;
			case BURST_LEN_8:
				sram_bcr.bits_map.burst_len = 2;
				break;
			case BURST_LEN_16:
				sram_bcr.bits_map.burst_len = 3;
				break;
			case BURST_LEN_32:
				sram_bcr.bits_map.burst_len = 4;
				break;
			case BURST_LEN_CONTINUOUS:
				sram_bcr.bits_map.burst_len = 7;
				if(sram_cfg_input_ptr->is_burst_wrap)
				{
					return SCI_FALSE;	// wrap cant use with continuse burst
				}
				break;
			default:
				SCI_ASSERT(SCI_FALSE);/*assert verified*/
				break;
		}

		// Wait state

		sram_bcr.bits_map.init_latency = 0;

		// Wait state
		sram_bcr.bits_map.latency_counter = (sram_cfg_input_ptr->clk <= 66)? 3:3;
		
		sram_cfg_output_ptr->first_data_latency = sram_bcr.bits_map.latency_counter;
		
		sram_cfg_output_ptr->is_wait_used = SCI_TRUE;
	
	}

	sram_cfg_output_ptr->cs_cr_set_func = SRAM_CR_SET_BY_EMC;

	sram_cfg_output_ptr->crI = sram_bcr.sram_bcr_value;

	sram_cfg_output_ptr->crII = sram_rcr.sram_rcr_value;

	return SCI_TRUE;
}

const NORMCP_SPEC_T g_normcp_K5L5666CAM_spec  = 
{
	0xEC,		//Manufacturer ID
	0x2206,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	ROW_LEN_512,	

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	100,			NULL,		100,			20,			0	},{	100,			NULL,			0,		4,	},	{83},
	//sram
	{	70,			NULL,		70,			20,			0	},{	70,			NULL,			0,		4,	},	{104},
	
	utram_cr_cfg_func,
	
	PNULL
};