
#include "std_header.h"


#include "sci_types.h"
#include "mmi_modu_main.h"

const uint16 pedometer_nv_len[] =
{
	sizeof(BOOLEAN),

	sizeof(uint8),//date

	sizeof(int),//praise

	sizeof(int),//day1 step 
	sizeof(int),//day2 step 
	sizeof(int),//day3 step 
	sizeof(int),//day4 step 
	sizeof(int),//day5 step 
	sizeof(int),//day6 step 
	sizeof(int),//day7 step 
	
};


PUBLIC void MMI_RegPedometerNv(void)
{
	MMI_RegModuleNv(MMI_MODULE_PEDOMETER, pedometer_nv_len, sizeof(pedometer_nv_len)/sizeof(uint16));
}