
#include "alloc-testing.h"
#include "framework.h"
#include"rsa_sprd.h"



extern uint8 init_timer_monitor(uint period, void (*timeoout)(uint32 param));

extern uint8 start_timer_monitor(HALO_TMID id );
extern uint8 change_timer_monitor(HALO_TMID id,uint period, void (*timeoout)(uint32 param) );	

extern uint8 stop_timer_monitor(HALO_TMID id );

extern uint8 delete_timer_monitor(HALO_TMID id );
void  test_Processpoweroff(void);

void  test_Processtimeout(uint32 lparam)
{
   
    PRTF("halo: enter the test timer expirer");
    stop_timer_monitor(HALO_NULL);
    PRTF("this case,  timeout need to power off wait for 5s");   
	HALO_Sleep(5);
    test_Processpoweroff();	
	HALO_Sleep(5);
    POWER_PowerOff();
    	//_HALO_Sendtimeout();
	
}

void  test_Processpoweroff(void)
{
	HALO_TICK_TIME_T s_tick_time = {0};
	HALO_GetTickTime(s_tick_time);
    PRTF("test_Processpoweroff enter with time %d.%d", s_tick_time.second, s_tick_time.milliseconds);
    PRTF("test_Processpoweroff");
    POWER_PowerOff();
    	//_HALO_Sendtimeout();
	
}
static unsigned char test_apcp_timeout(void)
{ 
	PRTF("**test start a valid timeout control\n");
	init_timer_monitor(HALO_NULL, test_Processtimeout);
        start_timer_monitor(HALO_NULL);
	HALO_Sleep(10*1000);
	stop_timer_monitor(HALO_NULL);
	PRTF("this case platform should not power off\n");
	PRTF("**test timeout process\n");
	change_timer_monitor(HALO_NULL,5000,test_Processtimeout);
	start_timer_monitor(HALO_NULL);
	PRTF("**HALO_Sleep 5 s\n");
	HALO_Sleep(10*1000);
	stop_timer_monitor(HALO_NULL);
	delete_timer_monitor(HALO_NULL);
	PRTF("**test poweroff");
	test_Processpoweroff();

}


static Unit_Test_T tests[] = {
	TEST(test_apcp_timeout),
	{NULL,NULL}
};



int test_actout(void)
{
	run_tests(tests);

	return 0;
}

