#include "sci_types.h"
//#include "chip_drv_common_io.h"

/*ÒÔÏÂ´ò×®*/
void outer_cache_poweroff(void)
{}
void outer_clean_range(unsigned long start, unsigned long end)
{}
void outer_disable(void)
{}
void outer_flush_all(void)
{}
void outer_inv_range(unsigned long start, unsigned long end)
{}
void outer_flush_range(unsigned long start, unsigned long end)
{}
void outer_inv_all(void)
{}
void MMU_FlushTlbEntry(uint32 *pmd)
{}
void MMU_DisableIDCM_BeforeSleep(void)
{
    uint32 temp;
}

