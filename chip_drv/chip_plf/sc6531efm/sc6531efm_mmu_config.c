#include "sci_types.h"
#include "mmu_drvapi.h"
#include "os_api.h"

#ifdef  VM_SUPPORT
LOCAL struct map_desc map_array[] = 
{
    {0x10000000,   __phys_to_pfn(0x10000000),  (0x200000),       MEM_TYPE_DEVICE},   
    {0x20000000,   __phys_to_pfn(0x20000000),  (0x1000000),      MEM_TYPE_DEVICE},   
    {0x30000000,   __phys_to_pfn(0x30000000),  (0x20000000),     MEM_TYPE_DEVICE},
    {0x40000000,   __phys_to_pfn(0x40000000),  (0x100000),       MEM_TYPE_DEVICE},
    {0x50000000,   __phys_to_pfn(0x50000000),  (0x4000000),      MEM_TYPE_DEVICE},
    {0x60000000,   __phys_to_pfn(0x60000000),  (0x10000000),     MEM_TYPE_DEVICE},
    {0x80000000,   __phys_to_pfn(0x80000000),  (0x20000000),     MEM_TYPE_DEVICE},
    {0xFFF00000,   __phys_to_pfn(0xFFF00000),  (0x100000),       MEM_TYPE_DEVICE},

#if 0//test mmu driver api
    #define MEM_TYPE_ORDER	                                          0
    #define MEM_TYPE_DEVICE	                                   		  1
    #define MEM_TYPE_NOCACHED_DATA	                                  2
    #define MEM_TYPE_CODE			                                  3
    #define MEM_TYPE_MEMORY_WB_DATA		                              4
    #define MEM_TYPE_MEMORY_WA_DATA                                   5
    #define MEM_TYPE_DATA_RO                                          6
    #define MEM_TYPE_DATA_WT                                          7
    #define MEM_TYPE_NO_ACCESS                                        8

    {0xF0000000,   __phys_to_pfn(0xF0000000),  (5*1024),         MEM_TYPE_DEVICE},
    {0xF1000000,   __phys_to_pfn(0xF1000000),  (5*1024),         MEM_TYPE_NOCACHED_DATA},
    {0xF2000000,   __phys_to_pfn(0xF2000000),  (5*1024),         MEM_TYPE_CODE},
    {0xF3000000,   __phys_to_pfn(0xF3000000),  (5*1024),         MEM_TYPE_MEMORY_WB_DATA},
    {0xF4000000,   __phys_to_pfn(0xF4000000),  (5*1024),         MEM_TYPE_DATA_RO},
    {0xF5000000,   __phys_to_pfn(0xF5000000),  (5*1024),         MEM_TYPE_DATA_WT},
    {0xF6000000,   __phys_to_pfn(0xF6000000),  (5*1024),         MEM_TYPE_NO_ACCESS},

#endif
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


