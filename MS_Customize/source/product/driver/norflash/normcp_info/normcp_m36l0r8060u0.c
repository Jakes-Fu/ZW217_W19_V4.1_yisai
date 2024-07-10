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

const NORMCP_SPEC_T g_normcp_m36l0r8060u0_spec  = 
{
	0x0020,		//Manufacturer ID
	0x883C,
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
	{	70,			7,			NULL,		NULL,	},{		70,			7,			0,		0,	},	{66},
	//sram
	{	70,			5,			NULL,		NULL,	},{		70,			5,			0,		0,	},	{83},

	m69km048a_psram_cr_cfg_func,
	
	m58lr128k_flash_cr_cfg_func	
};



