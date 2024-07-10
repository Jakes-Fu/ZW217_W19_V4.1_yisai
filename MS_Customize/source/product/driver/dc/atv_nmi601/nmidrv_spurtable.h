
#ifndef _NMIDRV_SPURTABLE_H_
#define _NMIDRV_SPURTABLE_H_

#include "nmiatv.h"


//SPUR TABLE
//IF NO RIPPLE, DO NOT FILL THIS TABLE!!! 
//the spur table is different project by project, don't use other project's spur table.

//How to fill this table
//(1) measure the spur using spectrum analyzer
//(2) record the spur frequency and width into this table

//#define _COMPANY_PROJECT_HWVER1_0_   //example
//#define _SIMCOM_P338_HW2_
#define _SC6600L2_xxxxx_
//...

#if defined(_COMPANY_PROJECT_HWVER1_0_)
tNotchPara _PROJECT_SPUR_[] = 
{
	//end of table
    {0, 0, 0} 
};
#elif defined(_SIMCOM_P338_HW2_)
tNotchPara _PROJECT_SPUR_[] = 
{
	//example:
    {481000000, sw_4KHz,0},

	//end of table
    {0, 0, 0} 
};
#elif defined(_SC6600L2_xxxxx_)
tNotchPara _PROJECT_SPUR_[] = 
{
	//example:
	{480000000, sw_256KHz, 1},	//external
	{560000000, sw_256KHz, 1},
	{640000000, sw_256KHz, 1},

	//end of table
    {0, 0, 0} 
};

#endif






#endif  /*_NMIDRV_SPURTABLE_H_*/
