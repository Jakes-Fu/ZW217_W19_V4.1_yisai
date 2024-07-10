#ifndef _ABUP_TIMER_H_
#define _ABUP_TIMER_H_


typedef enum _ABUP_TIMERID
{
	ABUP_TIMERID_00,
	ABUP_TIMERID_01,
	ABUP_TIMERID_02,
	ABUP_TIMERID_03,
	ABUP_TIMERID_04,
	ABUP_TIMERID_05,
	ABUP_TIMERID_06,
	ABUP_TIMERID_07,
	ABUP_TIMERID_08,
	ABUP_TIMERID_09,
	ABUP_TIMERID_MAXNUM
};


abup_extern void abup_start_timer(abup_int32 delay, abup_timer_callback func_ptr, ABUP_BOOL period);
abup_extern void abup_stop_timer(abup_timer_callback func_ptr);
abup_extern void abup_timer_handler(abup_uint32 time_id, abup_uint32 param);

extern void abup_timer1_cb(abup_uint32 timerid);
extern void abup_timer2_cb(abup_uint32 timerid);
extern void abup_timer3_cb(abup_uint32 timerid);

#endif /* _ABUP_TIMER_H_ */

