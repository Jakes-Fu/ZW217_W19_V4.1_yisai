/******************************************************************************
 ** File Name:    pagespy_dmc_phy.h                                             *
 ** Author:       jiajiang.lu                                                     *
 ** DATE:         3/05/2019                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/05/2019     jiajiang.lu     Create.                                     *
 ******************************************************************************/

#ifndef _HAL_PAGESPY_DMC_H_
#define _HAL_PAGESPY_DMC_H_

#define   PAGESPY_EVENT_TYPE_NUM     4
#define   PAGESPY_AID_MASTER_NUM     16


// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
// HAL_PAGE_NUM_T
// -----------------------------------------------------------------------------
/// Those are the 4 pages which can be protected
// =============================================================================
typedef enum
{
    HAL_PAGESPY_0=0,
    HAL_PAGESPY_1,
    HAL_PAGESPY_2,
    HAL_PAGESPY_3,
    HAL_PAGESPY_4,
    HAL_PAGESPY_5,
    HAL_PAGESPY_6,
    HAL_PAGESPY_7,
    HAL_PAGESPY_8,
    HAL_PAGESPY_9,
    HAL_PAGESPY_10,
    HAL_PAGESPY_11,
    HAL_PAGESPY_12,
    HAL_PAGESPY_13,
    HAL_PAGESPY_14,
    HAL_PAGESPY_15,

    HAL_PAGESPY_QTY
}HAL_PAGE_NUM_T;

// =============================================================================
// HAL_PAGESPY_ADDR_MODE_T
// -----------------------------------------------------------------------------
/// Type defining the possible Page Spy Mode
/// triggering an interrupt
// =============================================================================
typedef enum
{
    /// Triggered on reading or writing of the protected page
    HAL_PAGE_WRITE_HIT_TRIGGER=1,
    HAL_PAGE_READ_HIT_TRIGGER=2,
    HAL_PAGE_WR_HIT_TRIGGER=3,
}HAL_PAGESPY_ADDR_MODE_T;

typedef enum
{
    HAL_PAGE_TIME_TRIGGER=0,
    HAL_PAGE_LONGTIME_TRIGGER=1,
}HAL_PAGESPY_TIME_MODE_T;

typedef enum
{
    HAL_PAGE_NUMW_TRIGGER=0,
    HAL_PAGE_NUMR_TRIGGER=1,
}HAL_PAGESPY_NUM_MODE_T;

typedef enum
{
	HAL_PAGE_MASTER_A5     = 0x0,   /*m_a5*/
	HAL_PAGE_MASTER_F8     = 0x041,	/*m_f8*/
	HAL_PAGE_MASTER_LZMA   = 0x040,	/*m_lzma*/
	HAL_PAGE_MASTER_ZSP_DRAM = 0x080,  /*m_zsp_dram*/
	HAL_PAGE_MASTER_GOUDA = 0x0C1,   /*m_gouda*/
	HAL_PAGE_MASTER_USB = 0x0C0,   /*m_usb*/
	HAL_PAGE_MASTER_CPA5 = 0x140,	/*m_cpa5*/
	HAL_PAGE_MASTER_WCN_MEM = 0x184,	/*m_wcn_mem*/
	HAL_PAGE_MASTER_LCD = 0x042,	/*m_lcd*/
	HAL_PAGE_MASTER_MERGE_MEM = 0x182,   /*merge_mem*/
	HAL_PAGE_MASTER_USB11 = 0x0C2,   /*m_usb11*/
	HAL_PAGE_MASTER_M_CE = 0x186,	/*m_ce*/
}HAL_PAGESPY_AID1_T;

typedef enum
{
	HAL_PAGE_MERGE_AXIDMA = 0x1A2,	/*m_axidma*/
	HAL_PAGE_MERGE_LTE = 0x18A,	/*m_lte*/
	HAL_PAGE_MERGE_AON = 0x192,   /*m_aon*/
	HAL_PAGE_MERGE_GGE = 0x19A,	/*m_gge*/
	HAL_PAGE_MERGE_SYSIFC = 0x182,	/*m_sysifc*/
}HAL_PAGESPY_AID2_T;

typedef struct
{
	HAL_PAGESPY_AID1_T  pagespy_aid;
	char               aid_name[8];
}HAL_PAGESPY_AID_CHECK_T;


typedef struct
{
    uint8                         time_int:1;
    uint8                         longtime_int:1;
    uint8                         writenum_int:1;
    uint8                         readnum_int:1;
    uint8                         writehit_int:1;
    uint8                         readhit_int:1;
} HAL_PAGE_IRQ_STATUS_T;
typedef struct
{
    void (*lisr_givenTime)(uint32 *wBytes_cnt, uint32 *rBytes_cnt);
    void (*lisr_visitNum)(uint32 *time_cnt);
}HAL_PAGESPY_LISR_REGISTER_T;

typedef void (*lisr_givenTime)(uint32 *, uint32 *);
typedef void (*lisr_visitNum)(uint32 *);

typedef enum
{
    PAGESPY_ADDRHIT=0,
    PAGESPY_TIME=1,
    PAGESPY_LONGTIME=2,
	PAGESPY_NUM=3,
	PAGESPY_NONE=0xFF
}HAL_PAGESPY_EVENT_TYPE_T;

typedef enum
{
    PAGESPY_MEM_CPBIN=0,
    PAGESPY_MEM_APBIN,
    PAGESPY_MEM_AP_CP_SM,
    PAGESPY_MEM_AP_ZSP_SM,
    PAGESPY_MEM_CP_ZSP_SM,
    PAGESPY_MEM_CPLOG,
    PAGESPY_MEM_END,
}HAL_PAGESPY_MEM_TYPE_T;

#define  PAGESPY_TIME_REC_NUM    16
typedef struct
{
	uint32  time_type_wcnt[PAGESPY_TIME_REC_NUM];
	uint32  time_type_rcnt[PAGESPY_TIME_REC_NUM];
}HAL_PAGESPY_TIME_CNT_T;

PUBLIC void hal_PagespyInit(void);
PUBLIC void hal_PageProtectAddrSetup(uint8 ch, HAL_PAGESPY_ADDR_MODE_T mode, uint32 startAddr, uint32 endAddr);
PUBLIC uint8 halb_PagePhyNumGet(uint8 id);
PUBLIC void hal_PageStop(uint8 ch);

/// @}
#endif // _HAL_DBG_H_

