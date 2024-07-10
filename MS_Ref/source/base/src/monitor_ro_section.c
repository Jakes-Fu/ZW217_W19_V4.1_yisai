#include "monitor_ro_section.h"
#include "bus_monitor.h"
#include "os_api.h"

/******************************************************************************
 * Implementation of monitor_ro_section
 ******************************************************************************/
static char s_buf[50]; /* The buffer is defined as global to avoid overflowing
                        * the fiq stack */
static void _monitor_callback(unsigned long data, unsigned long pc_before_fiq)
{
	bm_disable(); 
	sprintf(s_buf, "memory overlap: pc = 0x%08lx", pc_before_fiq);
	SCI_Assert(s_buf, __FILE__, __LINE__);/*assert verified*/
}

PUBLIC void monitor_ro_section(unsigned long section_size)
{
	bm_monitor_set(
		0, 
		section_size, 
		0xffffffff, 
		1, 
		PNULL);	
}

