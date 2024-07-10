/******************************************************************************
 ** File Name:      normcp_m36l0r7050u3.c							                                             *
 ** Author:           Yiyue.he										                                                   *
 ** DATE:            24/07/2009                                                								*
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.     							*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           		NAME             DESCRIPTION                               							*
 ** 24/07/2009     	Yiyue.He         	 Create.                                   							*
 ******************************************************************************/
typedef union m69km048a_psram_bcr{
	struct m69km048a_psram_bcr_bits_map {
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
}M69KM048A_PSRAM_BCR_U;

//no need ,reserved for future description
typedef union m69km048a_psram_rcr{
	struct m69km048a_psram_rcr_bits_map {
		volatile  uint   reserved0			:BIT_RANGE(31,20);
		volatile  uint   register_select		:BIT_RANGE(19,18);
		volatile  uint   reserved1			:BIT_RANGE(17,5);
		volatile  uint   DPD					:BIT_RANGE(4,4);
		volatile  uint   reserved2			:BIT_RANGE(3,3);
		volatile  uint   PAR					:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint32   sram_rcr_value;		
}M69KM048A_PSRAM_RCR_U;

typedef union m58lr128kcd_norflash_cr{
	struct m58lr128kcd_norflash_cr_bits_map {
		volatile  uint	operating_mode		:BIT_RANGE(15,15);
		volatile  uint	reserved0			:BIT_RANGE(14,14);
		volatile  uint	init_latency			:BIT_RANGE(13,11);
		volatile  uint	wait_polority		:BIT_RANGE(10,10);
		volatile  uint	data_configuration	:BIT_RANGE(9,9);
		volatile  uint	wait_configuration	:BIT_RANGE(8,8);
		volatile  uint	burst_type			:BIT_RANGE(7,7);
		volatile  uint	clock_edge			:BIT_RANGE(6,6);
		volatile  uint	reserved1			:BIT_RANGE(5,4);
		volatile  uint	burst_wrap   		:BIT_RANGE(3,3);
		volatile  uint	burst_len   			:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint16   flash_cr_value;
}M58LR128KCD_NORFLASH_CR_U;


#pragma arm section code = "NORMCP_SPEC_FUNC_IN_SRAM"
LOCAL void m58lr128k_flash_cr_set_func(uint32 flash_addr_offset, uint32 cr, uint32 no_use)
{

	*(volatile uint16 *)(flash_addr_offset+ (cr<<1)) = 0x60;

	*(volatile uint16 *)(flash_addr_offset + (cr<<1)) = 0x3;
	
	*(volatile uint16 *)(flash_addr_offset) = 0xff;

}
#pragma arm section code

#if 0
LOCAL void m69km048a_psram_cr_set_func(uint32 sram_addr_offset,uint32 sram_bcr_value,uint32 sram_rcr_value)
{
	*(volatile uint32 *)(0x20000014 + (sram_addr_offset>>22)) |= 0x20000;	
	
	 *(volatile uint16*)(sram_addr_offset + (sram_bcr_value<<1)) = 0x0;
	 *(volatile uint16 *)(sram_addr_offset + (sram_rcr_value<<1)) = 0x0;
	 
	 *(volatile uint32 *)(0x20000014 + (sram_addr_offset>>22)) &= ~(0x20000);	

	 for(sram_addr_offset =0; sram_addr_offset<10; ++sram_addr_offset);
}
#endif


LOCAL BOOLEAN m69km048a_psram_cr_cfg_func(
	SRAM_CFG_FUNC_INPARAM_PTR		sram_cfg_input_ptr,
	SRAM_CFG_FUNC_OUTPARAM_PTR		sram_cfg_output_ptr
	)
{	
	M69KM048A_PSRAM_BCR_U sram_bcr;
	M69KM048A_PSRAM_RCR_U sram_rcr;	//just uint32 type for rcr is enough	

	//@default value
	sram_bcr.sram_bcr_value = 0x89D1F;
	sram_rcr.sram_rcr_value = 0x10;
	
	//cfg by input one by one
	sram_bcr.bits_map.operating_mode = (sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)?0x0:0x1;
	
	if(sram_cfg_input_ptr->access_mode == MEM_PAGE_MODE)
		return SCI_FALSE;  //no support		

	if(sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)
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
		sram_bcr.bits_map.latency_counter = (sram_cfg_input_ptr->clk <= 66)? 2:3;
		
		sram_cfg_output_ptr->first_data_latency = sram_bcr.bits_map.latency_counter;
		
		sram_cfg_output_ptr->is_wait_used = SCI_TRUE;
	
	}

	sram_cfg_output_ptr->cs_cr_set_func = SRAM_CR_SET_BY_EMC;

	sram_cfg_output_ptr->crI = sram_bcr.sram_bcr_value;

	sram_cfg_output_ptr->crII = sram_rcr.sram_rcr_value;

	return SCI_TRUE;
}


LOCAL BOOLEAN m58lr128k_flash_cr_cfg_func(
	FLASH_CFG_FUNC_INPARAM_PTR		flash_cfg_input_ptr,
	FLASH_CFG_FUNC_OUTPARAM_PTR		flash_cfg_output_ptr
	)
{
	M58LR128KCD_NORFLASH_CR_U flash_cr;

	// Flash config register default value
	flash_cr.flash_cr_value = 0xbacf;

	//cfg by input one by one
	flash_cr.bits_map.operating_mode = (flash_cfg_input_ptr->access_mode== MEM_BURST_MODE)?0x0:0x1;
	
	if(flash_cfg_input_ptr->access_mode == MEM_PAGE_MODE)
		return SCI_FALSE;

	
	
	if(flash_cfg_input_ptr->access_mode == MEM_BURST_MODE)
	{			

		flash_cr.bits_map.wait_polority = ( flash_cfg_input_ptr->wait_polarity == WAIT_ACTIVE_LOW )?0x0:0x1;

		flash_cr.bits_map.wait_configuration = (flash_cfg_input_ptr->is_wait_one_cycle_delay)?0x1:0x0;

		flash_cr.bits_map.data_configuration = 0;

		flash_cr.bits_map.burst_wrap = (flash_cfg_input_ptr->is_burst_wrap)?0x0:0x1;
		
		flash_cr.bits_map.clock_edge = (flash_cfg_input_ptr->is_clk_rising_edge)?0x1:0x0;

		switch(flash_cfg_input_ptr->burst_len)
		{
			case BURST_LEN_4:
				flash_cr.bits_map.burst_len = 0x1;
				break;
			case BURST_LEN_8:
				flash_cr.bits_map.burst_len = 0x2;
				break;
			case BURST_LEN_16:
				flash_cr.bits_map.burst_len = 0x3;
				break;
			case BURST_LEN_32:
				return SCI_FALSE;//no support
				break;
			case BURST_LEN_CONTINUOUS:
				flash_cr.bits_map.burst_len = 0x7;
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
			uint8 latency_counter_bits[8] = {0xFF,0xFF,2,3,4,5,6,7};
			uint8 first_data_latency = 0;
			uint8 clk = flash_cfg_input_ptr->clk;
			
			if(clk<= 30)
				first_data_latency = 2;
			else if(clk>30 && clk<=40)
				first_data_latency = 3;
			else if(clk>40 && clk<=54)
				first_data_latency = 4;					
			else if(clk>54 && clk<=66)
				first_data_latency = 4;
			else if(clk>66 && clk<=86)
				first_data_latency = 4;		
			else 
				return SCI_FALSE;

										
			flash_cr.bits_map.init_latency = latency_counter_bits[first_data_latency] & 0x7;	
				
			flash_cfg_output_ptr->first_data_latency = first_data_latency;
			
			flash_cfg_output_ptr->is_wait_used = SCI_TRUE;
		}
	}

	flash_cfg_output_ptr->cs_cr_set_func = m58lr128k_flash_cr_set_func;

	flash_cfg_output_ptr->crI = flash_cr.flash_cr_value;

	flash_cfg_output_ptr->crII = 0;

	return SCI_TRUE;
}

const NORMCP_SPEC_T g_normcp_m36l0r7050u3_spec  = //also for m36l0r7060u3_spec
{
	0x0020,		//Manufacturer ID
	0x882E,
	0x0,
		
	NORMCP_ADM_MODE,

	FLASH_INTEL_MODE,

	FLASH_WRITE_SINGLE_MODE |
	FLASH_WRITE_BUFFER_MODE,

	FLASH_WRITE_BUFFER_LEN_64,

	ROW_LEN_128,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			7,			NULL,		NULL,	},{		70,			7,			0,		0,	},	{86},
	//sram
	{	70,			5,			NULL,		NULL,	},{		70,			5,			0,		0,	},	{83},

	m69km048a_psram_cr_cfg_func,
	
	m58lr128k_flash_cr_cfg_func	
};

const NORMCP_SPEC_T g_normcp_m36l0r7050l3_spec  = //also for m36l0r7060l3_spec
{
	0x0020,		//Manufacturer ID
	0x882F,
	0x0,
		
	NORMCP_ADM_MODE,

	FLASH_INTEL_MODE,

	FLASH_WRITE_SINGLE_MODE |
	FLASH_WRITE_BUFFER_MODE,

	FLASH_WRITE_BUFFER_LEN_64,

	ROW_LEN_128,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			7,			NULL,		NULL,	},{		70,			7,			0,		0,	},	{86},
	//sram
	{	70,			5,			NULL,		NULL,	},{		70,			5,			0,		0,	},	{83},

	m69km048a_psram_cr_cfg_func,
	
	m58lr128k_flash_cr_cfg_func	

};



