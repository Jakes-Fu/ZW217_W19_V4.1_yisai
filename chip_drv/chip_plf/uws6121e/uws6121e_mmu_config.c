#include "sci_types.h"
#include "mmu_drvapi.h"
#include "os_api.h"

#if 1 //def  VM_SUPPORT
LOCAL struct map_desc map_array[] = 
{
    {0x00000000,   __phys_to_pfn(0x00000000),  (0x01000000),     MEM_TYPE_DEVICE},   
    {0x02000000,   __phys_to_pfn(0x02000000),  (0x06000000),     MEM_TYPE_DEVICE},   
    {0x10000000,   __phys_to_pfn(0x10000000),  (0x10000000),     MEM_TYPE_DEVICE},   
    {0x50000000,   __phys_to_pfn(0x50000000),  (0x08000000),     MEM_TYPE_DEVICE},   
    {0x60000000,   __phys_to_pfn(0x60000000),  (0x08000000),     MEM_TYPE_DEVICE},   
    {0x70000000,   __phys_to_pfn(0x70000000),  (0x08000000),     MEM_TYPE_DEVICE},   
    {0x80000000,   __phys_to_pfn(0x80000000),  (0x08000000),     MEM_TYPE_DEVICE},   
};

#else

CONST MMU_MMAP_CFG_T   map_array[] =
{
    {
        //0x10000000- 0x1010 0000
        0x10000000, 0x10000000, (0x100000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //0x20000000- 0x20700000
        0x20000000, 0x20000000, (0x600000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //rotation control, ahb
        0x20700000, 0x20700000, (0x200000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //rotation control, ahb
        0x20800000, 0x20800000, (0x300000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //rotation control, ahb
        0x20A00000, 0x20A00000, (0x300000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //rotation control, ahb
        0x20D00000, 0x20D00000, (0x300000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //0x30000000 - 0x40000000
        0x30000000, 0x30000000, (0x10000000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_SVC_R, MMU_AP_SVC_R, MMU_AP_SVC_R,MMU_AP_SVC_R},\
        PAGE_CB_WT,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //0x40000000 - 0x40100000
        0x40000000, 0x40000000, (0x100000>>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
   {
        //0x60000000-0x70000000
        0x60000000, 0x60000000, ( (0x10000000) >>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
   {
        //0x60000000-0x70000000
        0x70000000, 0x70000000, ( (0x00800000) >>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //0x80000000-0x90000000
        0x80000000, 0x80000000, ( 0x10000000 >>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    },
    {
        //0x80000000-0x90000000
        0xFFF00000, 0xFFF00000, ( 0x100000 >>10),   MMU_SECTION_MAPPING, \
        {MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS, MMU_AP_ALL_ACCESS,MMU_AP_ALL_ACCESS},\
        PAGE_CB_UNCACHED,   MMU_DOMAIN_ALL_ACCESS
    }
		
};
#endif

PUBLIC void *MMU_PHY_GetDeviceMap (uint32  *item_num)
{
    SCI_ASSERT (item_num);/*assert verified*/
    *item_num  = (sizeof (map_array) /sizeof (map_array[0]));
    return (void *) map_array;
}


