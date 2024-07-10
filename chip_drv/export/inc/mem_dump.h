#ifndef _MEM_DUMP_H_
#define _MEM_DUMP_H_

#include "sci_types.h"
#include "iram_mgr.h" 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(PLATFORM_UWS6121E)
#define NUM_FIQ_IRQ    2
#else
#define NUM_FIQ_IRQ    32
#endif

typedef struct ctl_reg_info_tag
{
    uint32  reg_address_begin;      //address of the first word
    uint32  reg_address_end;       //address of the last word
} CTL_REG_INFO_T;

typedef struct ip_reg_info_tag
{
    uint8 magic[16];                            //default "dumpreg"
    CTL_REG_INFO_T  *ctl_reg_info_tab_ptr;      //ip reg tab
} IP_REG_INFO_T;


typedef struct ctl_mem_info_tag
{
    uint32  mem_address_begin;      //address of the first word
    uint32  mem_address_end;       //address of the last word
    uint32  mem_size;       //size of region
} CTL_MEM_INFO_T;


typedef enum
{
    DRVEXP_MEMTBL_START_IN          = 0x01,     //start address found in table
    DRVEXP_MEMTBL_START_NOT_IN      = 0x02,     //start address not found in table
    DRVEXP_MEMTBL_END_MATCH         = 0x04,     //end address match start address
    DRVEXP_MEMTBL_END_NOT_MATCH     = 0x08,     //end address not match start address
    DRVEXP_MEMTBL_COND_MATCH        = 0x10,     //condition match start address
    DRVEXP_MEMTBL_COND_NOT_MATCH    = 0x20      //condition not match start address
} DRVEXP_MEMTBL_PARAMSTATUS_E;

/*****************************************************************************/
//  Description:    The function is used to get base address of dump memory.
//  Global resource dependence:
//  Author:         kevin.wang
//  Note:
/*****************************************************************************/
uint32 MEM_GetDumpBaseAddr (void);

/*****************************************************************************/
//  Description:    The function is used to get length of dump memory.
//  Global resource dependence:
//  Author:         kevin.wang
//  Note:
/*****************************************************************************/
uint32 MEM_GetDumpLen (void);

/*****************************************************************************/
// Description :    This function is used to get the len of shared mem.
// Global resource dependence :
// Author :         kevin.wang
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetSharedMemLen (void);

/*****************************************************************************/
// Description :    This function is used to get the base address of shared mem.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetSharedMemBase (void);

/*****************************************************************************/
// Description :    This function is used to get the len of Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetInterRamLen (void);

/*****************************************************************************/
// Description :    This function is used to get the base address of
//                  Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetInterRamBase (void);

#if defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    This function is used to get the len of BCPU Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetBcpuSramLen (void);


/*****************************************************************************/
// Description :    This function is used to get the base address of
//                  BCPU Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetBcpuSramBase (void);

#endif

#if defined(BLUETOOTH_SUPPORT) && defined(PLATFORM_SC6531EFM) && defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    This function is used to get the len of Internal On Chip BT RAM space.
// Global resource dependence :
// Author :         
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetBTIRamLen (void);

/*****************************************************************************/
// Description :    This function is used to get the base address of
//                  Internal On Chip BT RAM space.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
uint32 CHIP_GetBTIRamBase (void);
#endif


/*****************************************************************************/
// Description :    This function is used to get the name of FIQ/IRQ.
// Global resource dependence :fiq_irq_name
// Author :         kevin.wang
// Note :
/*****************************************************************************/
const char **CHIP_GetNameOfFiqIrq (void);

/*****************************************************************************/
// Description :    This function is used to get the infomation of control
//                  registers,including the base address,the length and the
//                  name of External Memory Control Registers and AHB General
//                  Control Registers,DMA Control Register,Digital Camera Space,
//                  USB Device Space,NAND Flash Control and LCM Control Space,
//                  Peripheral Address Space.
// Global resource dependence :g_CtlRegInfoTab
// Author :         kevin.wang
// Note : the unit of length is a word(32bit).
/*****************************************************************************/
void const *CHIP_GetInfoOfCtlReg (void);

void const *CHIP_GetInfoOfDumpReg (void);

/*****************************************************************************/
// Description :    This function is used to get the value of DSP memory base address
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetDspMemBaseAddr (void);

/*****************************************************************************/
// Description :    This function is used to get the value of DSP memory length.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetDspMemLen (void);

/*****************************************************************************/
// Description :    This function is used to get the value of the first control
//                  register base address
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetFirstRegBaseAddr (void);

/*****************************************************************************/
// Description :    This function is used to get the len of all control registers.
// Global resource dependence :
// Author :         kevin.wang
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetAllCtlRegLen (void);

/*****************************************************************************/
// Description :    This function is used to get the number of control registers group.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetNumCtlRegGrp (void);

/*****************************************************************************/
//  Description:    Get IRAM information used by DSP
//  Global resource dependence:
//  Author:
//  Note:           
/*****************************************************************************/
PUBLIC IRAM_AREA* MEM_GetIRAMDsp (uint32 *count);

/*****************************************************************************/
//  Description:    Check parameters of the memory table.
//  Global resource dependence:
//  Author:         Andy.Chen
//  Note: return value
//
//        DRVEXP_MEMTBL_START_IN,         //start address found in table
//        DRVEXP_MEMTBL_START_NOT_IN,     //start address not found in table
//        DRVEXP_MEMTBL_END_MATCH,        //end address match start address
//        DRVEXP_MEMTBL_END_NOT_MATCH,    //end address not match start address
//        DRVEXP_MEMTBL_COND_MATCH,       //condition match start address
//        DRVEXP_MEMTBL_COND_NOT_MATCH    //condition not match start address
//
//
/*****************************************************************************/
PUBLIC DRVEXP_MEMTBL_PARAMSTATUS_E SCI_GetParamStatus (
    uint32 start_addr,      // Access start address.
    uint32 end_addr,        // Access start address.
    uint32 acc_condition    // Access condition
);
/*****************************************************************************/
// Description :    This function is used to get the number of control memory group.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 CHIP_GetNumCtlMemGrp (void);

/*****************************************************************************/
// Description :    This function is used to get the infomation of control
//                  memory group
// Global resource dependence :g_CtlMemInfoTab
// Author :         kevin.wang
// Note : the unit of length is a word(32bit).
/*****************************************************************************/
PUBLIC void const *CHIP_GetInfoOfCtlMem (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* XSR_PORT_H */
