/******************************************************************************
 ** File Name:      normcp_s71ws128pc0hf3sr.c							                                             *
 ** Author:         	Yiyue.He										                                                   *
 ** DATE:            24/12/2008                                                								*
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.     							*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           		NAME             DESCRIPTION                               							*
 ** 24/12/2008     	Yiyue.he         	 Create.                                   							*
 ******************************************************************************/
typedef union micron_cellular_sram_bcr{
	struct sram_bcr_bits_map {
		volatile  uint   noused				:BIT_RANGE(31,21);
		volatile  uint   reserved6			:BIT_RANGE(20,20);
	    volatile  uint   register_select	:BIT_RANGE(19,19);
	    volatile  uint   reserved5			:BIT_RANGE(18,16);
		volatile  uint   operating_mode		:BIT_RANGE(15,15);
		volatile  uint   reserved4			:BIT_RANGE(14,14);
		volatile  uint   latency_counter	:BIT_RANGE(13,11);
		volatile  uint   wait_polority		:BIT_RANGE(10,10);
		volatile  uint   reserved3   		:BIT_RANGE(9,9);
		volatile  uint   wait_configuration :BIT_RANGE(8,8);
		volatile  uint   reserved2			:BIT_RANGE(7,7);
		volatile  uint   clock_configuration:BIT_RANGE(6,6);
		volatile  uint   output_impedance	:BIT_RANGE(5,5);
		volatile  uint   reserved1			:BIT_RANGE(4,4);
		volatile  uint   burst_wrap			:BIT_RANGE(3,3);
		volatile  uint   burst_len			:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint32   sram_bcr_value;
}MICRON_CELLULAR_SRAM_BCR;

typedef union micron_cellular_sram_rcr{
	struct sram_rcr_bits_map {
		volatile  uint   noused				:BIT_RANGE(31,21);
		volatile  uint   reserved3   		:BIT_RANGE(20,20);
		volatile  uint   register_select	:BIT_RANGE(19,19);
		volatile  uint   reserved2			:BIT_RANGE(18,8);
		volatile  uint   page			    :BIT_RANGE(7,7);
		volatile  uint   ignored			:BIT_RANGE(6,5);
		volatile  uint   DPD				:BIT_RANGE(4,4);
		volatile  uint   reserved1			:BIT_RANGE(3,3);
		volatile  uint   PAR				:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint32   sram_rcr_value;		
}MICRON_CELLULAR_SRAM_RCR;

typedef union spansion_29ws128p_flash_cr{
	struct flash_cr_bits_map {
		volatile  uint   read_mode			:BIT_RANGE(15,15);
		volatile  uint   reserved3			:BIT_RANGE(14,14);
		volatile  uint   wait_states		:BIT_RANGE(13,11);
		volatile  uint   RDY_polarity		:BIT_RANGE(10,10);
		volatile  uint   reserved2   		:BIT_RANGE(9,9);
		volatile  uint   RDY_configuration 	:BIT_RANGE(8,8);	//same func as wait configue , RDY= !WAIT
		volatile  uint   reserved1			:BIT_RANGE(7,5);
		volatile  uint   RDY_function		:BIT_RANGE(4,4);
		volatile  uint   burst_wrap			:BIT_RANGE(3,3);
		volatile  uint   burst_len			:BIT_RANGE(2,0);
	}bits_map;
	volatile  uint16   flash_cr_value;
}SPANSION_29WS128P_FLASH_CR;


#pragma arm section code = "NORMCP_SPEC_FUNC_IN_SRAM"
LOCAL void spansion_29ws128p_flash_cr_set_func(uint32 flash_addr_offset, uint32 cr, uint32 crII)
{
	*(volatile uint16 *)(flash_addr_offset+0xAAA) = 0xAAAA;    
	*(volatile uint16 *)(flash_addr_offset+0x554) = 0x5555;
	*(volatile uint16 *)(flash_addr_offset+0xAAA) = 0xD0D0;
	*(volatile uint16 *)(flash_addr_offset+0x000) = cr;
	*(volatile uint16 *)(flash_addr_offset+0x002) = crII;
	*(volatile uint16 *)(flash_addr_offset+0x000) = 0xF0;

	for(flash_addr_offset =0; flash_addr_offset<100; ++flash_addr_offset);

}
#pragma arm section code

#if 0
LOCAL void micron_cellular_sram_cr_set_func(uint32 sram_addr_offset,uint32 sram_bcr_value,uint32 sram_rcr_value)
{
	*(volatile uint32 *)(0x20000014 + (sram_addr_offset>>22)) |= 0x20000;												
						
	 *(volatile uint16*)(sram_addr_offset + (sram_bcr_value<<1)) = 0x0;
	 *(volatile uint16 *)(sram_addr_offset + (sram_rcr_value<<1)) = 0x0;
	 
	 *(volatile uint32 *)(0x20000014 + (sram_addr_offset>>22)) &= ~(0x20000);	
	 for(sram_addr_offset =0; sram_addr_offset<10; ++sram_addr_offset);
}
#endif


LOCAL BOOLEAN micron_cellular_sram_cr_cfg_func(
	SRAM_CFG_FUNC_INPARAM_PTR		sram_cfg_input_ptr,
	SRAM_CFG_FUNC_OUTPARAM_PTR	sram_cfg_output_ptr
	)
{

	{
		MICRON_CELLULAR_SRAM_BCR sram_bcr;
		MICRON_CELLULAR_SRAM_RCR sram_rcr;
		

		//@default value
		sram_bcr.sram_bcr_value = 0x89D4F;
		sram_rcr.sram_rcr_value = 0x00010;
		
		//cfg by input one by one
		sram_bcr.bits_map.operating_mode = (sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)?0x0:0x1;
		sram_rcr.	bits_map.page = (sram_cfg_input_ptr->access_mode == MEM_PAGE_MODE)?0x1:0x0;

		if(sram_cfg_input_ptr->access_mode == MEM_BURST_MODE)
		{		
			sram_bcr.bits_map.wait_polority = ( sram_cfg_input_ptr->wait_polarity == WAIT_ACTIVE_LOW )?0x0:0x1;

			sram_bcr.bits_map.wait_configuration = (sram_cfg_input_ptr->is_wait_one_cycle_delay)?0x1:0x0;

			if(!sram_cfg_input_ptr->is_clk_rising_edge)
				return SCI_FALSE;	//no support
			
			sram_bcr.bits_map.clock_configuration = (sram_cfg_input_ptr->is_clk_rising_edge)?0x1:0x0;
			
			sram_bcr.bits_map.burst_wrap = (sram_cfg_input_ptr->is_burst_wrap)?0x0:0x1;

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
					return SCI_FALSE;//no support
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
			
			{
				uint8 latency_counter_bits[8] = {2,2,2,3,4,5,6,7};
				uint8 burst_initial_latency_time = 39; //ns
				
				sram_cfg_output_ptr->first_data_latency = (burst_initial_latency_time * sram_cfg_input_ptr->clk)/1000 + 1;

				sram_bcr.bits_map.latency_counter = latency_counter_bits[sram_cfg_output_ptr->first_data_latency-1];	

				sram_cfg_output_ptr->is_wait_used = SCI_TRUE;
			}
		}

              sram_cfg_output_ptr->cs_cr_set_func = SRAM_CR_SET_BY_EMC;
              sram_cfg_output_ptr->crI = sram_bcr.sram_bcr_value;
              sram_cfg_output_ptr->crII = sram_rcr.sram_rcr_value;
    }	
	return SCI_TRUE;
}




LOCAL BOOLEAN spansion_29ws128p_flash_cr_cfg_func(
	FLASH_CFG_FUNC_INPARAM_PTR		flash_cfg_input_ptr,
	FLASH_CFG_FUNC_OUTPARAM_PTR	flash_cfg_output_ptr
	)
{


	{
		SPANSION_29WS128P_FLASH_CR flash_cr;
		uint16	flash_crII = 0xfffF;
		
		flash_cr.flash_cr_value = 0xEFC8;
		
		//cfg by input one by one
		flash_cr.bits_map.read_mode = (flash_cfg_input_ptr->access_mode == MEM_BURST_MODE)?0x0:0x1;

		if(flash_cfg_input_ptr->access_mode == MEM_BURST_MODE)
		{
		
			flash_cr.bits_map.RDY_polarity = ( flash_cfg_input_ptr->wait_polarity == WAIT_ACTIVE_LOW )?0x1:0x0;
			flash_cr.bits_map.RDY_configuration = (flash_cfg_input_ptr->is_wait_one_cycle_delay)?0x0:0x1;
			
			if(!flash_cfg_input_ptr->is_clk_rising_edge)
				return SCI_FALSE;	//no support		
				
			flash_cr.bits_map.burst_wrap = (flash_cfg_input_ptr->is_burst_wrap)?0x1:0x0;

			switch(flash_cfg_input_ptr->burst_len)
			{
				case BURST_LEN_4:
					return SCI_FALSE;//no support
					break;
				case BURST_LEN_8:
					flash_cr.bits_map.burst_len = 0x2;
					break;
				case BURST_LEN_16:
					flash_cr.bits_map.burst_len = 0x3;
					break;
				case BURST_LEN_32:
					flash_cr.bits_map.burst_len = 0x4;
					break;
				case BURST_LEN_CONTINUOUS:
					flash_cr.bits_map.burst_len = 0x0;
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
				uint8 latency_counter_bits[12] = {0xFF,0xFF,0,1,2,3,4,5,8,9,10,11};
				uint8 first_data_latency = 0;
				uint8 clk = flash_cfg_input_ptr->clk;

				if(flash_cfg_input_ptr->is_burst_wrap)
				{
					if(clk<= 27)
						first_data_latency = 3;
					else if(clk>27 && clk<=40)
						first_data_latency = 4;
					else if(clk>40 && clk<=54)
						first_data_latency = 5;					
					else if(clk>54 && clk<=66)
						first_data_latency = 6;
					else if(clk>66 && clk<=80)
						first_data_latency = 7;
					else if(clk>80 && clk<=95)
						first_data_latency = 8;
					else if(clk>95 && clk<=104)
						first_data_latency = 11;
					else 
						return SCI_FALSE;
				}
				else
				{
					if(clk<= 27)
						first_data_latency = 3;
					else if(clk>27 && clk<=40)
						first_data_latency = 4;
					else if(clk>40 && clk<=54)
						first_data_latency = 6;					
					else if(clk>54 && clk<=66)
						first_data_latency = 7;
					else if(clk>66 && clk<=80)
						first_data_latency = 8;
					else if(clk>80 && clk<=95)
						first_data_latency = 9;
					else if(clk>95 && clk<=104)
						first_data_latency = 11;
					else 
						return SCI_FALSE;
				}
								
				flash_cr.bits_map.wait_states = latency_counter_bits[first_data_latency] & 0x7;	
				
				flash_crII = (flash_crII & 0xFFFE) | (latency_counter_bits[first_data_latency]>>3);
				
				flash_cfg_output_ptr->first_data_latency = first_data_latency;
				
				flash_cfg_output_ptr->is_wait_used = SCI_TRUE;
			}
		}

              flash_cfg_output_ptr->cs_cr_set_func = spansion_29ws128p_flash_cr_set_func;
              flash_cfg_output_ptr->crI = flash_cr.flash_cr_value;
              flash_cfg_output_ptr->crII = flash_crII;	
              
    }

	return SCI_TRUE;
	
}


const NORMCP_SPEC_T g_normcp_s71ws128pc0hf3sr_spec  = 
{
	0x0001,		//Manufacturer ID
	0x7E4400,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE |
	FLASH_WRITE_BUFFER_MODE,

	FLASH_WRITE_BUFFER_LEN_64, //bytes

	ROW_LEN_128,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	83,			8,			80,			20,			0},{		60,			6,			4,		0,	},	{104},
	//sram
	{	70,			10,			70,			20,			5},{		70,			10,			10,		0,	},	{104},

	micron_cellular_sram_cr_cfg_func,
	
	spansion_29ws128p_flash_cr_cfg_func	

};


