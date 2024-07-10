/******************************************************************************
 ** File Name:      Normcp_burst_not_support.c				                                             *
 ** Author:         	Yiyue.he										                                                   *
 ** DATE:            13/01/2009                                                								*
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.     							*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           		NAME             DESCRIPTION                               							*
 ** 13/01/2009     	Yiyue.He         	 Create.                                   							*
 ******************************************************************************/


//@checked
const NORMCP_SPEC_T g_normcp_K5L2731CAM_spec  = 
{
	0xEC,		//Manufacturer ID
	0x257E,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		70,			20,			0	},{	70,			NULL,			0,		4,	},	{NULL},
	//sram
	{	70,			NULL,		70,			20,			0	},{	70,			NULL,			0,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};

//@checked
const NORMCP_SPEC_T g_normcp_S71PLJ_spec  = 
{
	0X1,		//Manufacturer ID
	0x7E2000,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		70,			20,				0	},{	70,			NULL,		0,		4,	},	{NULL},
	//sram
	{	70,			NULL,		70,			20,				0	},{	70,			NULL,		0,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};

//@checked
const NORMCP_SPEC_T g_normcp_TSB_TV00570002_spec  = 
{
	0x98,		//Manufacturer ID
	0x03,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE ,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle || data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		70,			25,			0},{		70,			NULL,		NULL,		4,	},	{NULL},
	//sram
	{	70,			NULL,		70,			30,			0},{		70,			NULL,		NULL,		4,	},	{NULL},

	NULL,
	
	NULL		

};


 //@no checked
const NORMCP_SPEC_T g_normcp_KAD060700_spec  = 
{
	0xEC,		//Manufacturer ID
	0x22E0,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		NULL,		NULL,			0	},{	50,			NULL,	NULL,		4,	},	{NULL},
	//sram
	{	70,			NULL,		NULL,		NULL,			0	},{	50,			NULL,	NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};


//@no checked
const NORMCP_SPEC_T g_normcp_S71PLN_spec  = 
{
	0x1,		//Manufacturer ID
	0x7E2000,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE |
	FLASH_WRITE_BUFFER_MODE,

	FLASH_WRITE_BUFFER_LEN_64,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		70,			35,				0	},{	50,			NULL,	NULL,		4,	},	{NULL},
	//sram
	{	70,			NULL,		70,			35,				0	},{	50,			NULL,	NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};
const NORMCP_SPEC_T g_normcp_S71PL127N_spec  = 
{
	0x1,		//Manufacturer ID
	0x7E2000,	
	0x1,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE |
	FLASH_WRITE_BUFFER_MODE,

	FLASH_WRITE_BUFFER_LEN_64,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		70,			30,				0	},{	70,			NULL,	NULL,		4,	},	{NULL},
	//sram
	{	70,			NULL,		70,			30,				0	},{	70,			NULL,	NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};

//@no checked
const NORMCP_SPEC_T g_normcp_L18_spec  = 
{
	0x89,		//Manufacturer ID
	0x880C,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_INTEL_MODE	,

	FLASH_WRITE_SINGLE_MODE |
	FLASH_WRITE_BUFFER_MODE,

	FLASH_WRITE_BUFFER_LEN_64,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	115,			NULL,		115,			30,				0	},{	70,			NULL,	NULL,		4,	},	{NULL},
	//sram
	{	100,			NULL,		NULL,		NULL,			0	},{	70,			NULL,	NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};

//@no checked
const NORMCP_SPEC_T g_normcp_L30_INTEL_spec  = 
{
	0x89,		//Manufacturer ID
	0x8812,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_INTEL_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	85,			NULL,		85,			25,				0	},{	50,			NULL,	NULL,		4,	},	{NULL},
	//sram
	{	85,			NULL,		NULL,		NULL,			0	},{	50,			NULL,	NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};
 //@no checked
const NORMCP_SPEC_T g_normcp_W18_INTEL_spec  = 
{
	0x89,		//Manufacturer ID
	0x8864,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_INTEL_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	100,			NULL,		100,			30,				0	},{	70,			NULL,	NULL,		4,	},	{NULL},
	//sram
	{	88,			NULL,		NULL,		NULL,			0	},{	70,			NULL,	NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};
//@no checked
const NORMCP_SPEC_T g_normcp_W18_ST_spec  = 
{
	0x20,		//Manufacturer ID
	0x8810,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_INTEL_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	100,			NULL,		100,			30,				0	},{	70,			NULL,	NULL,		4,	},	{NULL},
	//sram
	{	88,			NULL,		NULL,		NULL,			0	},{	70,			NULL,	NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};

const NORMCP_SPEC_T g_normcp_Memocom_2832B_spec  = 
{
	0x89,		//Manufacturer ID
	0x8900,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_INTEL_M18_MODE,

	FLASH_WRITE_SINGLE_MODE,
	FLASH_WRITE_BUFFER_NONE,
	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	96,			5,		96,			15,			7	},{	40,			5,			0,		0,	},	{80},
	//sram
	{	70,		    5,		70,			20,			10	},{	70,			5,			10,		10,	},	{80},

	PNULL,
	
	PNULL	

};
const NORMCP_SPEC_T g_normcp_CFEON_EN71GL128B0_spec  = 
{
	0x7F,		//Manufacturer ID
	0x227E,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE,

	FLASH_WRITE_SINGLE_MODE |
	FLASH_WRITE_BUFFER_MODE,

	FLASH_WRITE_BUFFER_LEN_64,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle || data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		70,			25,			0},{		70,			NULL,		NULL,		4,	},	{NULL},
	//sram
	{	70,			NULL,		70,			25,			0},{		70,			NULL,		NULL,		4,	},	{NULL},

	PNULL,
	
	PNULL	

};

const NORMCP_SPEC_T g_normcp_M36L0T70x0T3_spec  = 
{
    0x20,       //Manufacturer ID
    0x88C4, 
    0x1,
        
    NORMCP_ADP_MODE,
    
    FLASH_INTEL_MODE    ,
    
    FLASH_WRITE_SINGLE_MODE |
    FLASH_WRITE_BUFFER_MODE,
    
    FLASH_WRITE_BUFFER_LEN_64,

    
    NULL,

    /*timing    if time :ns , if freq:Mhz*/
    //flash
    /*read time |   (r)adv width | page read time | page cycle | data-ce || write time | (w)adv width | ce#-we# | we-ce   || max sync freq */
    {   85,         5,       85,         25,             0   },{ 85,         5,   4,       4,  },  {NULL},
    //sram
    {   85,         5,       85,       25,           0   },{ 85,         5,   4,       4,  },  {NULL},

    PNULL,
    
    PNULL   

};
const NORMCP_SPEC_T g_normcp_K5L6331CAA_spec  = 
{
	0xEC,		//Manufacturer ID
	0x257E,	
	0x1,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time |	(r)adv width | page read time | page cycle | data-ce || write time | (w)adv width |	ce#-we# | we-ce	  || max sync freq */
	{	70,			NULL,		70,			20,			0	},{	70,			NULL,			0,		4,	},	{NULL},
	//sram
	{	70,			NULL,		70,			25,			0	},{	70,			NULL,			0,		4,	},	{NULL},

	PNULL,
	
	PNULL		
};
const NORMCP_SPEC_T g_normcp_TSB_TV00560002_spec  = 
{
	0x98,		//Manufacturer ID
	0x96,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE ,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time | (r)adv width | page read time | page cycle || data-ce || write time | (w)adv width | ce#-we# | we-ce || max sync freq */
	{	70,				0,			70,				30,				0},		{	70,			0,			0,			0,	},	{0},
	//sram
	{	70,				0,			70,				30,				0},		{	70,			0,			0,			0,	},	{0},

	NULL,
	
	NULL		

};

const NORMCP_SPEC_T g_normcp_TSB_TV00560003_spec  = 
{
	0x98,		//Manufacturer ID
	0x97,	
	0x0,
		
	NORMCP_ADP_MODE,

	FLASH_AMD_MODE	,

	FLASH_WRITE_SINGLE_MODE ,

	FLASH_WRITE_BUFFER_NONE,

	NULL,

	/*timing	if time :ns , if freq:Mhz*/
	//flash
	/*read time | (r)adv width | page read time | page cycle || data-ce || write time | (w)adv width | ce#-we# | we-ce || max sync freq */
	{	70,				0,			70,				30,				0},		{	70,			0,			0,			0,	},	{0},
	//sram
	{	70,				0,			70,				30,				0},		{	70,			0,			0,			0,	},	{0},

	NULL,
	
	NULL		

};
