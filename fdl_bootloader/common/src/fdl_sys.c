
#include "common.h"

#if defined(PLATFORM_UWS6121E)
#define SCI_32TICKTOMILLISECOND(_TICK)    (((uint64)_TICK *1000) / 32768)

static uint32 timer_Idle32KRefGet(void)
{
    return hwp_idleLps->lps_32k_ref;
}
#endif

uint32 fdl_GetTickCount(void)
{   
#if !defined(PLATFORM_UWS6121E)
	volatile uint32 clock_0;
	volatile uint32 clock_1;
	
	//read two times all the same to make sure
	clock_0 = REG32(SYST_VALUE);
	clock_1 = REG32(SYST_VALUE);
	
	while (clock_0 != clock_1)
	{
		clock_0 = REG32(SYST_VALUE);
		clock_1 = REG32(SYST_VALUE);	
	}
	
	return (clock_1);
#else
    return SCI_32TICKTOMILLISECOND(timer_Idle32KRefGet());
#endif
}

void fdl_delay(uint32 ms)
{
	uint32 time_0, time_1;
	
	time_0 = fdl_GetTickCount();	
	do
	{
		time_1 = fdl_GetTickCount();
	}while ((time_1 - time_0) < ms);     
}
