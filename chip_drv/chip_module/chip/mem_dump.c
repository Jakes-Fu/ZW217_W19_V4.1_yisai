/******************************************************************************
 ** File Name:      mem_dump.c                                                 *
 ** Author:                                                            *
 ** DATE:                                                           *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:       *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                               *
 ** 24/11/2004             Create.                                   *
 ******************************************************************************/
#include "os_api.h"
#include "mem_dump.h"
#include "chip_plf_export.h"

/*****************************************************************************/
// Description :    This function is used to get the len of shared mem.
// Global resource dependence :
// Author :         kevin.wang
// Note :length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetSharedMemLen (void)
{
    return (SHARE_MEM_END - SHARE_MEM_BEGIN + 1);
}
/*****************************************************************************/
// Description :    This function is used to get the base address of shared mem.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetSharedMemBase (void)
{
    return SHARE_MEM_BEGIN;
}

/*****************************************************************************/
// Description :    This function is used to get the len of Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetInterRamLen (void)
{
    return (INTER_RAM_END - INTER_RAM_BEGIN + 1);
}

/*****************************************************************************/
// Description :    This function is used to get the base address of
//                  Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetInterRamBase (void)
{
    return INTER_RAM_BEGIN;
}

#if defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description :    This function is used to get the len of BCPU Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetBcpuSramLen (void)
{
    return (BCPU_SRAM_END - BCPU_SRAM_BEGIN + 1);
}

/*****************************************************************************/
// Description :    This function is used to get the base address of
//                  BCPU Internal On Chip RAM space.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetBcpuSramBase (void)
{
    return BCPU_SRAM_BEGIN;
}
#endif

#if defined(BLUETOOTH_SUPPORT) && defined(PLATFORM_SC6531EFM)
/*****************************************************************************/
// Description :    This function is used to get the len of Internal On Chip BT share memory.
// Global resource dependence :
// Author :         
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetBTShareRamLen (void)
{
    return SHARE_MEM1_SIZE;
}

/*****************************************************************************/
// Description :    This function is used to get the base address of
//                  Internal On Chip BT share memory.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
uint32 CHIP_GetBTShareRamBase (void)
{
    return SHARE_MEM1_BEGIN;
}

/*****************************************************************************/
// Description :    This function is used to get the len of Internal On Chip BT RAM space.
// Global resource dependence :
// Author :         
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetBTIRamLen (void)
{
    return (INTER_BT_MEM_END - INTER_BT_MEM_BEGIN + 1);
}

/*****************************************************************************/
// Description :    This function is used to get the base address of
//                  Internal On Chip BT RAM space.
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
uint32 CHIP_GetBTIRamBase (void)
{
    return INTER_BT_MEM_BEGIN;
}

extern uint8 *bt_code_addr;
extern uint32 bt_code_size;
uint32 CHIP_GetBTCodeAddr (void)
{
	return (uint32)bt_code_addr;
}

uint32 CHIP_GetBTCodeSize (void)
{
	return bt_code_size;
}
#endif

/*****************************************************************************/
// Description :    This function is used to get the value of DSP memory base address
// Global resource dependence :
// Author :         kevin.wang
// Note : the return value is not cared in SC6600M
/*****************************************************************************/
uint32 CHIP_GetDspMemBaseAddr (void)
{
    return DSP_MEM_BEGIN;
}

/*****************************************************************************/
// Description :    This function is used to get the value of DSP memory length.
// Global resource dependence :
// Author :         kevin.wang
// Note : the return value is not cared in SC6600M
/*****************************************************************************/
uint32 CHIP_GetDspMemLen (void)
{
    return DSP_MEM_LEN;
}


/*****************************************************************************/
// Description :    This function is used to get the value of the first control
//                  register base address
// Global resource dependence :
// Author :         kevin.wang
// Note : the return value is not cared in SC6600M
/*****************************************************************************/
uint32 CHIP_GetFirstRegBaseAddr (void)
{
    return EXTERNAL_MEM_CTL_BEGIN;
}

/*****************************************************************************/
// Description :    This function is used to get the len of all control registers.
// Global resource dependence :
// Author :         kevin.wang
// Note : length unit is byte.
/*****************************************************************************/
uint32 CHIP_GetAllCtlRegLen (void)
{
    uint32 AllCtlRegLen = 0;

    CTL_REG_INFO_T *CtlRegInfoTab = (CTL_REG_INFO_T *) CHIP_GetInfoOfCtlReg();

    while (CtlRegInfoTab->reg_address_begin != 0xFFFFFFFF)
    {
        AllCtlRegLen += (CtlRegInfoTab->reg_address_end - CtlRegInfoTab->reg_address_begin + 4);
        CtlRegInfoTab++;
    }

    return AllCtlRegLen;
}

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
)
{
    uint32 i;
    uint32 num = 0;
    uint32 retval = DRVEXP_MEMTBL_START_NOT_IN;
    MEMORY_ACCESS_INFO_T  *memory_acc_table = (MEMORY_ACCESS_INFO_T *) Mem_getAccTable (&num);

    for (i = 0; i < num; i++)
    {
        if ( (start_addr >= memory_acc_table[i].mem_start) && (start_addr <= memory_acc_table[i].mem_end))
        {
            retval = DRVEXP_MEMTBL_START_IN;

            if ( (acc_condition & memory_acc_table[i].acc_condition) == acc_condition)
            {
                retval |= DRVEXP_MEMTBL_COND_MATCH;
                //break;
            }
            else
            {
                retval |= DRVEXP_MEMTBL_COND_NOT_MATCH;
                //break;
            }

            if (end_addr <= memory_acc_table[i].mem_end)
            {
                retval |= DRVEXP_MEMTBL_END_MATCH;
                break;
            }
            else
            {
                retval |= DRVEXP_MEMTBL_END_NOT_MATCH;
                break;
            }
        }
    }

    if (i == num)
    {
        retval = DRVEXP_MEMTBL_START_NOT_IN;
    }

    return retval;
}


