
#include "std_header.h"


#include "sci_types.h"
#include "alarmClock_nv.h"
#include "alarmClock_export.h"
#include "mmi_modu_main.h"

const uint16 alarmClock_nv_len[] =
{
	sizeof(BOOLEAN),//flag

	sizeof(int),

	sizeof(BOOLEAN),//is_delay

};


PUBLIC void MMI_RegAlarmClockNv(void)
{
	MMI_RegModuleNv(MMI_MODULE_ALARMCLOCK, alarmClock_nv_len, sizeof(alarmClock_nv_len)/sizeof(uint16));
}