/******************************************************************************
 ** File Name:      normcp_k5l2833ata-af66.c							                                             *
 ** Author:         	Yiyue.He										                                                   *
 ** DATE:            24/12/2008                                                								*
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.     	  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           		NAME             DESCRIPTION                          *
 ** 07/05/2009     	Yiyue.He         	 Create.                                  *
 ******************************************************************************/
typedef union k5l2833ata_af66_utsram2_bcr{
	struct utsram2_bcr_bits_map {
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
}k5l2833ata_af66_UTSRAM2_BCR_U;

typedef union k5l2833ata_af66_utsram2_rcr{
	struct utsram2_rcr_bits_map {
		volatile  uint   reserved0			:BIT_RANGE(31,20);
		volatile  uint   register_select		:BIT_RANGE(19,18);
		volatile  uint   reserved1			:BIT_RANGE(17,8);
		volatile  uint   page				:BIT_RANGE(7,7);
		volatile  uint   reserved2			:BIT_RANGE(6,5);
		volatile  uint   DPD					:BIT_RANGE(4,4);
		volatile  uint   reserved3			:BIT_RANGE(3,3);
		volatile  uint   PAR					:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint32   sram_rcr_value;		
}k5l2833ata_af66_UTSRAM2_RCR_U;

typedef union k5l2833ata_af66_norflash_cr{
	struct flash1_cr_bits_map {
		volatile  uint	reserved0			:BIT_RANGE(31,21);
		volatile  uint	drv_ctrl				:BIT_RANGE(20,19);
		volatile  uint	rdy_active			:BIT_RANGE(18,18);
		volatile  uint	burst_read_mode	:BIT_RANGE(17,15);
		volatile  uint	wait_states			:BIT_RANGE(14,12);
		volatile  uint	reserved1   			:BIT_RANGE(11,0);
	}bits_map;
	volatile  uint32   flash_cr_value;
}k5l2833ata_af66_NORFLASH_CR_U;


#pragma arm section code = "NORMCP_SPEC_FUNC_IN_SRAM"
LOCAL void k5l2833ata_af66_flash_cr_set_func(uint32 flash_addr_offset, uint32 cr, uint32 crII)
{
	*(volatile uint16 *)(flash_addr_offset+0xAAA)	= 0xAAAA;    
	*(volatile uint16 *)(flash_addr_offset+0x554)	= 0x5555;
	*(volatile uint16 *)(flash_addr_offset+ (cr<<1) + 0xAAA) = 0xC0;

	{
			uint16 i = 0;
			for(i=0; i<20; i++);
	}
	
	*(volatile uint16 *)(flash_addr_offset+0xAAA)	= 0xAAAA;    
	*(volatile uint16 *)(flash_addr_offset+0x554)	= 0x5555;	
	*(volatile uint16 *)(flash_addr_offset + (crII<<1) + 0xAAA) = 0xC5;
	
	{
			uint16 i = 0;
			for(i=0; i<20; i++);
	}


}
#pragma arm section code

#if 0
LOCAL void k5l2833ata_af66_sram_cr_set_func(uint32 sram_addr_offset,uint32 sram_bcr_value,uint32 sram_rcr_value)
{
	*(volatile uint32 *)(0x20000014 + (sram_addr_offset>>22)) |= 0x20000;	
	
	 *(volatile uint16*)(sram_addr_offset + (sram_bcr_value<<1)) = 0x0;
	 *(volatile uint16 *)(sram_addr_offset + (sram_rcr_value<<1)) = 0x0;
	 
	 *(volatile uint32 *)(0x20000014 + (sram_addr_offset>>22)) &= ~(0x20000);	
	 for(sram_addr_offset =0; sram_addr_offset<10; ++sram_addr_offset);
}
#endif


LOCAL BOOLEAN k5l2833ata_af66_sram_cr_cfg_func(
	SRAM_CFG_FUNC_INPARAM_PTR		sram_cfg_input_ptr,
	SRAM_CFG_FUNC_OUTPARAM_PTR		sram_cfg_output_ptr
	)
{	
	k5l2833ata_af66_UTSRAM2_BCR_U sram_bcr;
	k5l2833ata_af66_UTSRAM2_RCR_U sram_rcr;		

	//@default value
	sram_bcr.sram_bcr_value = 0x1B9FDF;
	sram_rcr.sram_rcr_value = 0x13FFF8;
	
	//cfg by input one by one
	sram_bcr.bits_map.operating_mode = (sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)?0x0:0x1;
	sram_rcr.bits_map.page = (sram_cfg_input_ptr->access_mode == MEM_PAGE_MODE)?0x1:0x0;

	if(sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)
	{		
		sram_bcr.bits_map.wait_polority = ( sram_cfg_input_ptr->wait_polarity == WAIT_ACTIVE_LOW )?0x0:0x1;

		sram_bcr.bits_map.wait_configuration = (sram_cfg_input_ptr->is_wait_one_cycle_delay)?0x1:0x0;

		if(!sram_cfg_input_ptr->is_clk_rising_edge)
			return SCI_FALSE;	//no support		
		
		sram_bcr.bits_map.burst_wrap = (sram_cfg_input_ptr->is_burst_wrap)?0x0:0x1;
		
		sram_bcr.bits_map.driver_strenght = 0x0;

		

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
		sram_bcr.bits_map.latency_counter = (sram_cfg_input_ptr->clk <= 66)? 2:3;
		
		sram_cfg_output_ptr->first_data_latency = sram_bcr.bits_map.latency_counter;
		
		sram_cfg_output_ptr->is_wait_used = SCI_TRUE;
	
	}

	sram_cfg_output_ptr ->cs_cr_set_func = SRAM_CR_SET_BY_EMC;

       sram_cfg_output_ptr->crI = sram_bcr.sram_bcr_value;

       sram_cfg_output_ptr->crII = sram_rcr.sram_rcr_value;
	
	return SCI_TRUE;
}


LOCAL BOOLEAN k5l2833ata_af66_flash_cr_cfg_func(
	FLASH_CFG_FUNC_INPARAM_PTR		flash_cfg_input_ptr,
	FLASH_CFG_FUNC_OUTPARAM_PTR		flash_cfg_output_ptr
	)
{
	k5l2833ata_af66_NORFLASH_CR_U flash_cr;
	uint32	flash_crII = 0x0;

	// Flash config register default value
	flash_cr.flash_cr_value = 0x104000;

	//cfg by input one by one
	if(flash_cfg_input_ptr->access_mode == MEM_BURST_MODE)
	{			
		flash_cr.bits_map.rdy_active = (flash_cfg_input_ptr->is_wait_one_cycle_delay)?0x1:0x0;
		
		if(!flash_cfg_input_ptr->is_clk_rising_edge)
			return SCI_FALSE;	//no support		

		switch(flash_cfg_input_ptr->burst_len)
		{
			case BURST_LEN_4:
				return SCI_FALSE;//no support
				break;
			case BURST_LEN_8:
				flash_cr.bits_map.burst_read_mode = 0x1;
				break;
			case BURST_LEN_16:
				flash_cr.bits_map.burst_read_mode = 0x2;
				break;
			case BURST_LEN_32:
				return SCI_FALSE;//no support
				break;
			case BURST_LEN_CONTINUOUS:
				flash_cr.bits_map.burst_read_mode = 0x000;
				if(flash_cfg_input_ptr->is_burst_wrap)
				{
					return SCI_FALSE;	// wrap cant use with continuse burst
				}
				break;
			default:
				SCI_ASSERT(SCI_FALSE);/*assert verified*/
				break;
		}

		{
			uint8 latency_counter_bits[12] = {0xFF,0xFF,0xFF,0XFF,0,1,2,3,4,0XFF,0XFF,0XFF};
			uint8 first_data_latency = 0;
			uint8 clk = flash_cfg_input_ptr->clk;
			
			if(clk<= 54)
				first_data_latency = 4;
			else if(clk>54 && clk<=70)
				first_data_latency = 5;
			else if(clk>70 && clk<=83)
				first_data_latency = 6;					
			else if(clk>83 && clk<=100)
				first_data_latency = 7;
			else if(clk>100 && clk<=108)
				first_data_latency = 8;		
			else 
				return SCI_FALSE;

										
			flash_cr.bits_map.wait_states = latency_counter_bits[first_data_latency] & 0x7;	
			
			flash_crII = 0x1000;
			
			flash_cfg_output_ptr->first_data_latency = first_data_latency;
			
			flash_cfg_output_ptr->is_wait_used = SCI_TRUE;
		}
	}

	flash_cfg_output_ptr->cs_cr_set_func = k5l2833ata_af66_flash_cr_set_func;

       flash_cfg_output_ptr->crI = flash_cr.flash_cr_value;

       flash_cfg_output_ptr->crII = flash_crII;

	return SCI_TRUE;
}

const NORMCP_SPEC_T g_normcp_k5l2833ata_af66_spec  = //also for k5l2833ata_aq11 k5l2833atb-af66 k5l2833atb_aq11 k5l2866atb_af66  k5l2866atb_aq11
{
	0x00EC,		//Manufacturer ID
	0x2402,
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE ,

	FLASH_WRITE_BUFFER_NONE,

	ROW_LEN_128,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			5,			70,			20,		},{		70,			5,			0,		0,	},	{66},
	//sram
	{	70,			5,			70,			20,		},{		70,			5,			0,		0,	},	{66},

	k5l2833ata_af66_sram_cr_cfg_func,
	
	k5l2833ata_af66_flash_cr_cfg_func	

};

const NORMCP_SPEC_T g_normcp_k5l2833aba_af66_spec  = //also for k5l2833aba_aq11 k5l2833abb-af66 k5l2833abb_aq11 k5l2866abb_af66  k5l2866abb_aq11
{
	0x00EC,		//Manufacturer ID
	0x2403,
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE ,

	FLASH_WRITE_BUFFER_NONE,

	ROW_LEN_128,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			5,			70,			20,		},{		70,			5,			0,		0,	},	{66},
	//sram
	{	70,			5,			70,			20,		},{		70,			5,			0,		0,	},	{66},

	k5l2833ata_af66_sram_cr_cfg_func,
	
	k5l2833ata_af66_flash_cr_cfg_func	

};


