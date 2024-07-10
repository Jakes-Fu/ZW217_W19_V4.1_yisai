#include "abup_typedef.h"
#include "abup_debug.h"
#include "abup_timer.h"

#include "os_api.h"
#include "mmk_timer.h"


static ABUP_TIMER_STRUCT g_abup_timer[ABUP_TIMERID_MAXNUM]={0};
void abup_start_timer(abup_int32 delayms,abup_timer_callback func_ptr, ABUP_BOOL period)
{
	abup_uint8 idx = 0;

	for (idx = 0; idx < ABUP_TIMERID_MAXNUM; idx++)
	{
		if (g_abup_timer[idx].func_ptr == func_ptr)
		{
			MMK_StopTimer(g_abup_timer[idx].timerid);
			g_abup_timer[idx].timerid = 0;
			g_abup_timer[idx].func_ptr = NULL;
			g_abup_timer[idx].delay = 0;
		}
	}

	for (idx = 0; idx < ABUP_TIMERID_MAXNUM; idx++)
	{
		if(g_abup_timer[idx].func_ptr == NULL)
		{
			g_abup_timer[idx].func_ptr = func_ptr;
			break;
		}
	}
	abup_DebugPrint("%s: idx=%d", __func__, idx);

	if (idx >= ABUP_TIMERID_MAXNUM) 
		return;

	g_abup_timer[idx].delay = delayms;

	g_abup_timer[idx].timerid = MMK_CreateTimerCallback(delayms, abup_timer_handler, (uint32)idx, period);
}


void abup_stop_timer(abup_timer_callback func_ptr)
{
	abup_uint8 idx = 0;

	abup_DebugPrint("%s: timer func: 0x%08x", __func__, (abup_uint32)func_ptr);

	for (idx = 0; idx < ABUP_TIMERID_MAXNUM; idx++)
	{
		if (g_abup_timer[idx].func_ptr == func_ptr)
		{
			break;
		}
	}

	if (idx >= ABUP_TIMERID_MAXNUM) 
		return;

	abup_DebugPrint("%s: timer idx:%d", __func__, idx);

	if (g_abup_timer[idx].timerid)
	{
		MMK_StopTimer(g_abup_timer[idx].timerid);
		g_abup_timer[idx].timerid = 0;
		g_abup_timer[idx].func_ptr = NULL;
		g_abup_timer[idx].delay = 0;
	}	
}


void abup_timer_handler(abup_uint32 time_id, abup_uint32 param)
{
	uint8 idx = (uint8)param;
	abup_timer_callback func_ptr = NULL;

	if (idx >= ABUP_TIMERID_MAXNUM) return;

	if (g_abup_timer[idx].timerid == time_id)
	{
		func_ptr = g_abup_timer[idx].func_ptr;
		MMK_StopTimer(time_id);
		g_abup_timer[idx].timerid = 0;
		g_abup_timer[idx].func_ptr = NULL;
		g_abup_timer[idx].delay = 0;
		if (func_ptr) func_ptr();
	}

	abup_DebugPrint("%s: timer idx:%d, send timer msg to abup", __func__, idx);
}

void abup_timer_cb(abup_uint32 timerid)
{		
	abup_DebugPrint("%s: timer idx:%d", __func__, timerid);
	
	if(timerid >= ABUP_TIMERID_MAXNUM) //abup_TIMERID_MAXNUM
		return; //timer is not found
	
	if(g_abup_timer[timerid].func_ptr)
		g_abup_timer[timerid].func_ptr();
}


void abup_timer1_cb(abup_uint32 timerid)
{
	static abup_uint32 timer_idx=0;
	abup_time_struct time;

	timer_idx++;

	abup_get_device_time(&time);
	
	abup_DebugPrint("abup curr date time1: %04d-%02d-%02d %02d:%02d:%02d", \
		time.nYear,time.nMonth,time.nDay, time.nHour, time.nMin,  time.nSec);

	abup_stop_timer(abup_timer1_cb);

	abup_DebugPrint("abup re start timer1: idx: %d", timer_idx);
	abup_start_timer(1000, abup_timer1_cb, ABUP_FALSE); 
}

void abup_timer2_cb(abup_uint32 timerid)
{
	static abup_uint32 timer_idx=0;
	abup_time_struct time;

	timer_idx++;

	abup_get_device_time(&time);
	
	abup_DebugPrint("abup curr date time2: %04d-%02d-%02d %02d:%02d:%02d", \
		time.nYear,time.nMonth,time.nDay, time.nHour, time.nMin,  time.nSec);

	abup_stop_timer(abup_timer2_cb);

	abup_DebugPrint("abup re start timer2: idx: %d", timer_idx);
	abup_start_timer(1000, abup_timer2_cb, ABUP_FALSE); 
}

void abup_timer3_cb(abup_uint32 timerid)
{
	static abup_uint32 timer_idx=0;
	abup_time_struct time;

	timer_idx++;

	abup_get_device_time(&time);
	
	abup_DebugPrint("abup curr date time3: %04d-%02d-%02d %02d:%02d:%02d", \
		time.nYear,time.nMonth,time.nDay, time.nHour, time.nMin,  time.nSec);

	abup_stop_timer(abup_timer3_cb);

	abup_DebugPrint("abup re start timer3: idx: %d", timer_idx);
	abup_start_timer(1000, abup_timer3_cb, ABUP_FALSE); 
}


