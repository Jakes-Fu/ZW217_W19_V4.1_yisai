/******************************************************************************
 ** File Name:    mmu_phy.h                                                       *
 ** Author:                                                                   *
 ** DATE:         8/16/2010                                                   *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 08/16/2010    Steve.Zhan      Create.                                     *
 *****************************************************************************/
#ifndef _MMU_PHY_H_
#define _MMU_PHY_H_

#include "mmu_vm_config.h"

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         MACOR Definitions                                 *
 **---------------------------------------------------------------------------*/
#define MMU_DEBUG_ON            0

#if MMU_DEBUG_ON
#define MMU_TRACE(_x_)          SCI_TraceLow _x_
#define MMU_ASSERT(_x_)         SCI_ASSERT(_x_)
#else
#define MMU_TRACE(_x_)          ((void)0)
#define MMU_ASSERT(_x_)          SCI_ASSERT(_x_)
#endif


/* page dir table */
#define PTRS_PER_PGDIR              (4096)

#define PMD_SHIFT		            (20)
#define PGDIR_SHIFT                 (20)
//#define PGDIR_SIZE                  (1UL << PGDIR_SHIFT)
//#define PGDIR_MASK                  (~(PGDIR_SIZE - 1))

/*
 * section address mask and size definitions.
 */
#define SECTION_SHIFT		(20)
#define SECTION_SIZE		(1UL << SECTION_SHIFT)
#define SECTION_MASK		(~(SECTION_SIZE-1))


#define PTE_HWTABLE_PTRS	(PTRS_PER_PTE)
#define PTE_HWTABLE_OFF		(PTE_HWTABLE_PTRS * sizeof(uint32))
#define PTE_HWTABLE_SIZE	(PTRS_PER_PTE * sizeof(uint32))


#define PGDIR_COARSE_BASE_MASK      (0xFFFFFC00)
#define PGDIR_SECT_BASE_MASK        (0xFFF00000)
#define PGDIR_FINE_BASE_MASK        (0xFFFFF000)

#define PTRS_PER_COARSE_PTE         (256)
#define PTRS_PER_FINE_PTE           (1024)
//#define PTRS_PER_PTE		        (512)
#define PTRS_PER_PTE		        (256)


#define PTE_LARGE_BASE_MASK         (0xFFFF0000)
#define PTE_SMALL_BASE_MASK         (0xFFFFF000)
#define PTE_TINY_BASE_MASK          (0xFFFFFC00)

#define PTE_LARGE_SHIFT             (16)
#define PTE_LARGE_SIZE              (1UL << PTE_LARGE_SHIFT)
#define PTE_LARGE_MASK              (~(PTE_LARGE_SIZE - 1))

#define PTE_SMALL_SHIFT             (12)
#define PTE_SMALL_SIZE              (1UL << PTE_SMALL_SHIFT)
#define PTE_SMALL_MASK              (~(PTE_SMALL_SIZE - 1))

#define PTE_TINY_SHIFT              (10)
#define PTE_TINY_SIZE               (1UL << PTE_TINY_SHIFT)
#define PTE_TINY_MASK               (~(PTE_TINY_SIZE - 1))

#define PAGE_SIZE		(1 << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

#define PMD_SIZE		(1UL << PMD_SHIFT)
#define PMD_MASK		(~(PMD_SIZE-1))
#define PGDIR_SIZE		(1UL << PGDIR_SHIFT)
#define PGDIR_MASK		(~(PGDIR_SIZE-1))

#define pte_pfn(pte)		((pte) >> PAGE_SHIFT)
#define pfn_pte(pfn,prot)	(((pfn) << PAGE_SHIFT) | (prot))


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

#define L_PTE_MT_UNCACHED	(0x00 << 2)	
#define L_PTE_MT_BUFFERABLE	(0x01 << 2)	
#define L_PTE_MT_WRITETHROUGH	(0x02 << 2)	
#define L_PTE_MT_WRITEBACK	(0x03 << 2)	
#define L_PTE_MT_MINICACHE	 (0x06 << 2)	
#define L_PTE_MT_WRITEALLOC	(0x07 << 2)	
#define L_PTE_MT_DEV_SHARED	(0x04 << 2)	
#define L_PTE_MT_DEV_NONSHARED	(0x0c << 2)	
#define L_PTE_MT_DEV_WC		(0x09 << 2)
#define L_PTE_MT_DEV_CACHED	(0x0b << 2)
#define L_PTE_MT_MASK		(0x0f << 2)

/*
 * Level 2 common descriptor  
 */
#define PTE_TYPE_FAULT		(0 << 0)
#define PTE_TYPE_LARGE		(1 << 0)
#define PTE_TYPE_SMALL		(2 << 0)
#define PTE_TYPE_EXT		(3 << 0)
#define PTE_BUFFERABLE		(1 << 2)
#define PTE_CACHEABLE		(1 << 3)

#define PTE_TYPE_MASK(x)            (x & 0x00000003)

/*
 *   small page/tiny page
 */
#define PTE_EXT_XN		(1 << 0)

#define PTE_PRIV_READ_ONLY_NOUSER (1<<9 | (1 << 4))
#define PTE_PRIV_READ_ONLY_ANDUSER (1<<9 | (1 << 5))

#define PTE_FULL_ACCESS ( (3 << 4) )

#define PTE_EXT_TEX(x)		((x) << 6)
#define PTE_EXT_APX		(1 << 9)	
#define PTE_EXT_COHERENT	(1 << 9)	
#define PTE_EXT_SHARED		(1 << 10)	
#define PTE_EXT_NG		(1 << 11)	

PUBLIC void mmu_asm_set_prrr(uint32);
PUBLIC void mmu_asm_set_nmrr(uint32);

#endif

/*
 * CR1 bits (CP#15 CR1)
 */
#define CR_M	       (1 << 0)	
#define CR_A	       (1 << 1)	
#define CR_C	       (1 << 2)	
#define CR_W	       (1 << 3)	
#define CR_P	       (1 << 4)	
#define CR_D	       (1 << 5)	/* 32-bit data address range		*/
#define CR_L	       (1 << 6)	/* Implementation defined		*/
#define CR_B	       (1 << 7)	
#define CR_S	       (1 << 8)	
#define CR_R	       (1 << 9)	
#define CR_F	       (1 << 10)	
#define CR_Z	       (1 << 11)	
#define CR_I	       (1 << 12)	
#define CR_V	       (1 << 13)/* Vectors relocated to 0xffff0000	*/
#define CR_RR	       (1 << 14)
#define CR_L4	       (1 << 15)
#define CR_DT	       (1 << 16)
#define CR_IT	       (1 << 18)
#define CR_ST	       (1 << 19)
#define CR_FI	(1 << 21)	/* Fast interrupt (lower latency mode)	*/
#define CR_U 	(1 << 22)	/* Unaligned access operation		*/
#define CR_XP	(1 << 23)	/* Extended page tables			*/
#define CR_VE	(1 << 24)	/* Vectored interrupts			*/
#define CR_EE	(1 << 25)	/* Exception (Big) Endian		*/
#define CR_TRE	(1 << 28)	/* TEX remap enable			*/
#define CR_AFE	(1 << 29)	/* Access flag enable			*/
#define CR_TE	(1 << 30)	/* Thumb exception enable		*/


typedef enum
{
    PGDIR_TYPE_FAULT  = 0x0,
    PGDIR_TYPE_COARSE = 0x1,
    PGDIR_TYPE_SECT   = 0x2,
    PGDIR_TYPE_FINE   = 0x3
} PGDIR_TYPE_E;

#define PGDIR_TYPE_MASK(x)             (x & 0x00000003)
#define   PTE_TYPE_TINY  0x3

typedef uint32 pte_t;
typedef uint32 pgdir_t;
typedef uint32 pgd_t[1];
typedef uint32 pgprot_t;
typedef uint32  phys_addr_t;



/*-----------------------------------------------------------------------------
 * demand paging Macro Definitions.
 */
#if defined(DP_USE_SMALL_PAGE)//begin 

#define DP_PAGE_TYPE                    PTE_TYPE_SMALL
#define DP_PAGE_SIZE                    PTE_SMALL_SIZE
#define DP_PAGE_MASK                    PTE_SMALL_MASK
#define DP_PAGE_SHIFT                   PTE_SMALL_SHIFT

#if defined(DP_USE_COARSE_PTE)
#define DP_PTE_TYPE                     PGDIR_TYPE_COARSE
#elif defined(DP_USE_FINE_PTE)
#define DP_PTE_TYPE                     PGDIR_TYPE_FINE
#else
#error unknown pte type
#endif
//else

#elif defined(DP_USE_TINY_PAGE)

#define DP_PAGE_TYPE                    PTE_TYPE_TINY
#define DP_PAGE_SIZE                    PTE_TINY_SIZE
#define DP_PAGE_MASK                    PTE_TINY_MASK
#define DP_PAGE_SHIFT                   PTE_TINY_SHIFT
#if !defined(DP_USE_FINE_PTE)
#error only fine pte supports tiny page /* pte type, must be fine pte */
#endif /* !defined(DP_USE_FINE_PTE) */

#define DP_PTE_TYPE                     PGDIR_TYPE_FINE

#else
#error unknown page type
#endif
//end demand paging

PUBLIC void *MMU_PHY_GetDeviceMap (uint32  *item_num);

PUBLIC void MMU_Init_pgdir (void);
PUBLIC pte_t *MMU_AllocPte (uint32 addr, uint32 pt_type);
PUBLIC uint32 MMU_SetPteValue (pte_t *ptep, pte_t pte, uint32 pt_type);

PUBLIC void MMU_initVirPages (uint32 vir_base, uint32 vir_size, uint32 page_size);
PUBLIC void MMU_MapOnePage (uint32 vir_page, uint32 phy_page, const uint32 *ap, uint32 cb, uint32 page_size);
PUBLIC void MMU_UnMapOnePage (uint32 vir_page);
PUBLIC uint32 MMU_DpOnPageIfMap (uint32 vir_addr);

PUBLIC void MMU_DisableIDCM (void);
PUBLIC BOOLEAN MMU_IsEnable(void);
PUBLIC void MMU_EnableIDCM_AfterSleep (void);
PUBLIC void MMU_DisableIDCM_BeforeSleep (void);

PUBLIC void MMU_PHY_InvalideICACHEALL (void);
PUBLIC void MMU_PHY_DisableICache (void);
PUBLIC void MMU_PHY_EnableICache (void);
PUBLIC BOOLEAN MMU_PHY_ICacheIsEnable (void);

PUBLIC void MMU_PHY_DmaMapRegion(uint32 bufferAddr, uint32 bufferSize, uint32 dir);
PUBLIC void MMU_PHY_InvalideDCACHEALL (void);
PUBLIC void MMU_PHY_InvalideDCACHEALL_DisableDcache(void);
PUBLIC void MMU_PHY_DisableDCache (void);
PUBLIC void MMU_PHY_EnableDCache (void);
PUBLIC BOOLEAN MMU_PHY_DCacheIsEnable (void);
PUBLIC void MMU_PHY_DCacheSync (void);
PUBLIC void MMU_PHY_CacheInvaliateSingle(uint32 addr, uint32 size);

PUBLIC void MMU_InvalidateTlbSingle (uint32 addr);
PUBLIC void MMU_DrainWriteBuffer (void);

PUBLIC void MMU_EnableRBit (void);
PUBLIC BOOLEAN dp_handle_fault (uint32 vir_addr, void *voidFunc);
PUBLIC void VM_DP_SetVirAddr (uint32 addr, uint32 size);
#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E) 
PUBLIC void dp_init_pfn (uint32 pfn_base, uint32 pfn_size);
#endif
PUBLIC void dp_init_image (void);
PUBLIC void mmu_MapOnPage_Set(uint32 vir_addr, uint32 phy_addr,  uint32 mem_type, uint32 size);

//
#ifdef  VM_SUPPORT

PUBLIC int mmu_proc_get_cr(void);

typedef struct _LIST_NODE
{
    struct _LIST_NODE *prev, *next;
} LIST_NODE;

#if DP_USE_FIFO

typedef struct _DP_PFN_DESCRIPTOR
{
    uint32 vir_page;
} DP_PFN_DESCRIPTOR;


struct mmu_dm_uinit
{
    uint32 vir_base;
    uint32 vir_size;
	uint32 phy_base;
	uint32 phy_size;
	uint32 page_size;// 1k , 4k

//vm
#ifdef  NANDBOOT_SUPPORT
    uint32 s_unzip_cache_addr;
#endif
    uint32 dp_pfn_base;
    uint32 dp_pfn_tail;
    uint32 dp_pfn_count;
    DP_PFN_DESCRIPTOR * dp_pfn_desc;
	
//
};

#else

typedef struct _DP_PFN_DESCRIPTOR
{
    uint32 vir_page;
    LIST_NODE mapped;
    LIST_NODE cached;

    /* hash list */
    struct _DP_PFN_DESCRIPTOR **hash_pprev;
    struct _DP_PFN_DESCRIPTOR *hash_next;
} DP_PFN_DESCRIPTOR;
#define HASH_TABLE_SHIFT        (9)

#define HASH_TABLE_SIZE         (1UL << HASH_TABLE_SHIFT)
#define HASH_TABLE_MASK         (HASH_TABLE_SIZE - 1)

struct HASH_LIST_T
{
   LIST_NODE *dp_mapped_pfn;
   LIST_NODE *dp_cached_pfn;
   struct _DP_PFN_DESCRIPTOR *hash_tab[HASH_TABLE_SIZE];

};

struct mmu_dm_uinit
{
    uint32 vir_base;
    uint32 vir_size;
	uint32 phy_base;
	uint32 phy_size;
	uint32 page_size;
    struct HASH_LIST_T hash_list;
//vm
#ifdef  NANDBOOT_SUPPORT
    uint32 s_unzip_cache_addr;
#endif
    uint32 dp_pfn_base;
    uint32 dp_pfn_count;
    DP_PFN_DESCRIPTOR * dp_pfn_desc;
//
};

#endif



//
PUBLIC void MMU_dp_add_unit(struct mmu_dm_uinit *p_dm_uinit);
PUBLIC void mmu_dp_init_struct(void);
void MMU_FlushTlbEntry(uint32 *pmd);

#endif

#endif

