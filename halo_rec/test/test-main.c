#include "system_hal.h"

LOCAL uint32 s_Halo_test_thread_id = HALO_INVALID_BLOCK_ID;
HALO_TICK_TIME_T s_test_time = {0};
typedef int (*subsystemFunction)(void);

extern int test_diagcmd(void);
extern int test_memory(void);
extern int test_algo(void);
extern int test_actout(void);

static subsystemFunction tests[] = {
	test_actout,
	NULL
};

LOCAL void _HALO_test_Task_Routine(uint32 argc, void *argv)
{
	int i;
	PRTF("_HALO_test_Task_Routine enter %d",__STDC_VERSION__);
	for (i=0; tests[i]!= NULL; ++i)
	{
		tests[i]();
	}
	PRTF("_HALO_test_Task_Routine leave");
}

void halo_init_test(void)
{
uint8 val = 0;
uint8 result = 0;


HALO_GetTickTime(s_test_time);
PRTF("halo_init_test enter with time %d.%d",s_test_time.second, s_test_time.milliseconds);
           

	s_Halo_test_thread_id = HALO_CreateThread(
	                    "halo_task", 
	                    "halo_queue",
	                    _HALO_test_Task_Routine,
	                    0,
	                    0,
	                    10240,
	                    HALO_QUEUESIZE,
	                    HALO_PRIORITY,
	                    HALO_PREEMPT,
	                    HALO_AUTO_START);


PRTF("halo_init leave");

return;
}