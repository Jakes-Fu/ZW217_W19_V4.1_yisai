
#include "sci_types.h"
#include "mem_cfg.h"

extern void MMU_EnableIDCM (void);
//extern void MMU_InvalideICACHEALL(void);
extern void MMU_PHY_InvalideICACHEALL (void);

#define MMU_InvalideICACHEALL        MMU_PHY_InvalideICACHEALL

/**********************************************************************
  Section Descriptor:
  31                   20 19        12 11 10  9  8   5  4  3  2  1  0
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  | section base address |   ZERO     |  AP | 0 | DAC | 1 | C/B | 1 0|
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AP: Access Permission Bits, it action together with R/S Bits in C1 register[9:8],
    (R represents ROM protection, S represents System protection.) AP can be b_11,
    thus Access Control will be done by DA bits.
    AP  S   R   SVC     USER
    00  0   0   No Access   No Access
    00  1   0   Read-Only   No Access
    00  0   1   Read-Only   Read-Only
    00  1   1   Unpredictable   Unpredictable
    01  X   X   Read/Write  No Access
    10  X   X   Read/Write  Read-Only
    11  X   X   Read/Write  Read/Write

DA: Domain Access Bits, it indicate the position of DAC in register C3.
C/B:    Cacheable/Bufferable,
    C:1 Enable, 0 Disable
    B:1 Enable, 0 Disable
***********************************************************************/
#define MMU_SD_CONST    0x00000012
#define MMU_AP_B00      0x00000000
#define MMU_AP_B01      0x00000400
#define MMU_AP_B10      0x00000800
#define MMU_AP_B11      0x00000C00
#define MMU_C_BIT       0x00000008
#define MMU_B_BIT       0x00000004

//MMU table start address can be configed by specific project! it is in mem_cfg_xxx.c.
/*
modify from 0x1600000 to 0x3f0000, to avoid memory corruption in RW region
when watchdog reset happen
in 8800x series, 0x3f0000--0x400000 is empty
in 6800 serirs, maybe has little problem, have risk to overlap the memory!!!!
Now, change to 0x008f0000, in reserved region.
*/

#if 1//for v7
/*
 * + Level 1 common descriptor
 */
#define PMD_TYPE_MASK		(3 << 0)
#define PMD_TYPE_FAULT		(0 << 0)
#define PMD_TYPE_TABLE		(1 << 0)
#define PMD_TYPE_SECT		(2 << 0)
#define PMD_BIT4		       (1 << 4)
#define PMD_DOMAIN(x)		((x) << 5)
#define PMD_PROTECTION		(1 << 9)	 
/*
 *   - section
 */
#define PMD_SECT_BUFFERABLE	(1 << 2)
#define PMD_SECT_CACHEABLE	(1 << 3)
#define PMD_SECT_XN		(1 << 4)	

#define PMD_PRIV_READ_ONLY_NOUSER (1<<15 | (1 << 10))
#define PMD_PRIV_READ_ONLY_ANDUSER (1<<15 | (1 << 10)|(1 << 11))
#define PMD_FULL_ACCESS ((1 << 10) | (1 << 11))



#define PMD_SECT_TEX(x)		((x) << 12)
#define PMD_SECT_APX		(1 << 15)	
#define PMD_SECT_S		       (1 << 16)	
#define PMD_SECT_nG		(1 << 17)
#define PMD_SECT_SUPER		(1 << 18)

#define PMD_SECT_UNCACHED	(0)
#define PMD_SECT_BUFFERED	(PMD_SECT_BUFFERABLE)
#define PMD_SECT_WT		(PMD_SECT_CACHEABLE)
#define PMD_SECT_WB		(PMD_SECT_CACHEABLE | PMD_SECT_BUFFERABLE)
#define PMD_SECT_WBWA		(PMD_SECT_TEX(1) | PMD_SECT_CACHEABLE | PMD_SECT_BUFFERABLE)
#endif

LOCAL const uint32 const_MMUTable = PHY_AON_IRAM_BASE_ADDR;


//LOCAL const uint32 const_MMUTableStartAddrRemap0 = 0x80000000;
//LOCAL const uint32 const_MMUTableStartAddrRemap1= 0x80000000;


//MMU_TABLE_ADDR must be aligned by 16K-Byte.
#define MMU_TABLE_ADDR          (const_MMUTable & 0xFFFFC000)
//#define MMU_TABLE_ADDR_REMAP    ((const_MMUTableStartAddrRemap1) & 0xFFFFC000 )
//#define MMU_TABLE_ADDR_REMAP0   ((const_MMUTableStartAddrRemap0) & 0xFFFFC000 )
//#define MMU_TABLE_ADDR_REMAP1   ((const_MMUTableStartAddrRemap1) & 0xFFFFC000 )

/*lint -e765 ,because g_mmu_page_table is referred in mmu_asm.s*/
// MMU page table starting address to be referred in mmu_asm.s
PUBLIC unsigned int *g_mmu_page_table; 

void MMU_Init (void)
{
    unsigned int *page_table;
    uint32 remap;
    int i;

    rprintf ("MMU_Init\n");

   

     g_mmu_page_table =  (unsigned int *)MMU_TABLE_ADDR;

    // 15Mb physical addr for page table
    page_table = g_mmu_page_table;

    // Create page table 1mb entries
    for (i = 0; i < 0x1000; i++)
    {
#if defined(PLATFORM_UWS6121E)

        //page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        //page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        if (( ( (i >= 0x600) && (i < 0x610))) || ( (i >= 0x700) && (i < 0x708)))
        {
         page_table[i] = (PMD_TYPE_SECT|PMD_FULL_ACCESS|PMD_SECT_TEX(0)|PMD_DOMAIN(0xF)) + (i << 20);
        }
        else if (( ( (i >= 0x800) && (i < 0x820))))
        {
         page_table[i] = (PMD_TYPE_SECT|PMD_SECT_WB|PMD_FULL_ACCESS|PMD_SECT_TEX(0)|PMD_DOMAIN(0xF)) + (i << 20);
        }
        else
        {
         page_table[i] = (PMD_TYPE_SECT|PMD_FULL_ACCESS|PMD_SECT_TEX(0)|PMD_DOMAIN(0xF)) + (i << 20);
        }
#elif NOR_FDL_SC6800H

        // romcode -> 0x0-0x0FFFFFFF
        if (i < 0xA)
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        }
        // Nor flash space: Write Buffer
        else if ( ( (i >= 0x300) && (i < 0x340)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_B_BIT) + (i << 20);
        }
        // Sram space: DCache & Write Buffer
        else if ( ( (i >= 0x340) && (i < 0x400)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        }
        else
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        }
#elif defined(PLATFORM_SC6531EFM)

		// romcode -> 0x0-0x0FFFFFFF
        if (i < 0xA)
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        }
        // Nor flash space: Write Buffer
        else if ( ( (i >= 0x100) && (i < 0x140)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_B_BIT) + (i << 20);
        }
        // Sram space: DCache & Write Buffer
        else if ( ( (i >= 0x140) && (i < 0x200)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        }
        else
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        }
#else //SDRAM
        // SDRAM -> CB (write back):0x0-0x0FFFFFFF
        if (i < 0x16)
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        }
        //else if (( i>=0x12 )&&(i <= 0x15)){
        //  page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        //}
        //else if ( (i>0x14) && (i < 0x100))
        //{
        //    page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        //}
        // Internal Shared Memeory: 0x1000_0000-0x1FFF_FFFF
        //else if ((i >= 0x100) && (i < 0x200))
        //  page_table[i] = 0x00000C1E + (i << 20);
        // Internal RAM Memeory: CB
        else if ( ( (i >= 0x300) && (i <= 0x400))) //  || (mustSetIramCached == TRUE && i == 0x400) )
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        }
        // FLASH memory:No CB
        //else if ((i >= 0x400) && (i < 0x500))
        //    page_table[i] = 0x00000C12 + (i << 20);
        // IO: NO CB
        //else if ((i >= 0x700) && (i < 0x900))
        //    page_table[i] = 0x00000C12 + (i << 20);
        // No Access
        else
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        }

#endif
    }

    //for MMU
    i = (uint32)(MMU_TABLE_ADDR) >> 20;
    rprintf ("TTB 0x%x,TTB in sector num%d\n",MMU_TABLE_ADDR,i);
	
    page_table[i] = (PMD_TYPE_SECT|PMD_SECT_UNCACHED|PMD_FULL_ACCESS|PMD_SECT_TEX(0)|PMD_DOMAIN(0xF)) + (i << 20);

    MMU_InvalideICACHEALL();//steve.zhan add.
    MMU_EnableIDCM();

    //Delay some time
    for (i=0; i<1000; i++) {}
}

void MMU_Init_ext (void)
{
    unsigned int *page_table;
    uint32 remap;
    int i; 

     g_mmu_page_table =  (unsigned int *)MMU_TABLE_ADDR;


    // 15Mb physical addr for page table
    page_table = g_mmu_page_table;

    // Create page table 1mb entries
    for (i = 0; i < 0x1000; i++)
    {
#ifdef defined(PLATFORM_UWS6121E)

        page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);

#elif defined(PLATFORM_SC6531EFM)
        // Nor flash space: Write Buffer
        if ( ( (i >= 0x000) && (i < 0x040)) || ( (i >= 0x100) && (i < 0x140)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_B_BIT) + (i << 20);
        }
        // Sram space: DCache & Write Buffer
        else if ( ( (i >= 0x040) && (i < 0x100)) || ( (i >= 0x140) && (i < 0x200)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        }
        else
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        }
#else
        // Nor flash space: Write Buffer
        if ( ( (i >= 0x000) && (i < 0x040)) || ( (i >= 0x300) && (i < 0x340)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_B_BIT) + (i << 20);
        }
        // Sram space: DCache & Write Buffer
        else if ( ( (i >= 0x040) && (i < 0x100)) || ( (i >= 0x340) && (i < 0x400)))
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11|MMU_C_BIT|MMU_B_BIT) + (i << 20);
        }
        else
        {
            page_table[i] = (MMU_SD_CONST|MMU_AP_B11) + (i << 20);
        }
#endif
    }

    MMU_InvalideICACHEALL();//steve.zhan add.
    MMU_EnableIDCM();

    //Delay some time
    for (i=0; i<0x100; i++) {}
}
//end of file mmu.c
