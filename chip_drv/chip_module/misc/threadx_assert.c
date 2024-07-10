/******************************************************************************
 ** File Name:      threadx_assert.c                                          *
 ** Author:         Richard Yang                                              *
 ** DATE:           05/03/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file including SCI_Assert function to identify logic *
 **                 errors! When ASSERT in WIN32, it calls _assert in WIN32   *
 **                 library; else if ASSERT in target board, it will flush    *
 **                 ASSERT strings by serial port, and waiting debugging      *
 **                 command from client tools!                                *
 ******************************************************************************
 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 05/03/2002      Richard.Yang    Create.                                   *
 ** 12/31/2002      Xueliang.Wang   Modified.                                 *
 **                 Rename the file name from sci_assert.c to threadx_assert.c*
 ** 02/13/2004      Xueliang.Wang   Modify CR7292                             *
 **                 Delete trace information when exception                   *
 **                 Add to read memory 0x40000000 - 0x4fffffff                *
 ** 11/03/2010  fei.zhang        Dump DSP data when assert  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#ifdef _DEBUG

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// RTOS ThreadX API head file.
#include "threadx_os.h"
#include "os_api.h"
#include "os_import.h"
#include "diag.h"
#include "os_apiext.h"
#include "os_param.h"
#include "sci_log.h"
#include "com_drvapi.h"
#include "chip_drvapi.h"
#include "power.h"
#include "dal_time.h"
#include "chip_plf_export.h"
#include "nvitem.h"
#include "nv_type.h"
#include "threadx_assert.h"

#include "sio.h"
#include "init.h"
#include "dma_drv_internal.h"
#include "dal_lcd.h"
#include "task_monitor.h"
#include "ref_outport.h"
#include "version.h"
#include "vcom_usb.h"
#include "wdg_drvapi.h"
#include "gpio_drvapi.h"
#include "tb_drv.h"
#include "iram_mgr.h"
#include "emc_drvapi.h"
#include "normcp_spec.h"
#include "flash_drvapi.h"
#include "../emc/emc_reg_if.h"
#include "../usb/usb_timer.h"
#include "gadget.h"
#include "../usb/usb_phy.h"
#include "gpio_prod_cfg.h"
#include "mem_prod.h"

#ifdef CARD_LOG
#include "logsave_output.h"
#endif

#include "mmu_drvapi.h"
#ifdef DSP_USB_LOG
#include "dsp_log.h"
#include "ref_param.h"
#endif

#include "flash.h"
#include "sci_ftl_api.h"
#include "NAND_PartitionId.h"
#include   "spload_cfg.h"
#include "xsr_partition.h"


#ifndef WIN32 // In THUMB Target Mode
/**---------------------------------------------------------------------------*
 **                             Macro Definition                              *
 **---------------------------------------------------------------------------*/
// We can set the program address to the bottom of the Interrupt Vector!
#define     TOP_OF_PROGRAM              0x10
#define     THUMB_SUB_SP_CODE_MASK      0xff80
#define     THUMB_SUB_SP_CODE           0xb080

#define     THUMB_ADD_SP_CODE_MASK      0xff80
#define     THUMB_ADD_SP_CODE           0xb000

#define     THUMB_PUSH_CODE_MASK        0xfe00
#define     THUMB_PUSH_CODE             0xb400

#define     ARM_LTMDB_CODE_MASK         0xFFFFBF00
#define     ARM_LTMDB_CODE              0xE92D0000
#define     SIZE_OF_REG_LIST_1          8        // In ARM instruction

#define     THUMB_BL_MASK               0xf000
#define     THUMB_BL_CODE               0xf000
#ifdef ARM_ARCH_V7
#define     SIZE_OF_REG_LIST            16        // In Thumb instruction, there
#else
#define     SIZE_OF_REG_LIST            9        // In Thumb instruction, there
#endif
#define     STACK_INIT_VAL              0x5A5A5A5A    //MEM_INIT_VALUE = 0x5A

#define     COMMAND_MAX_SIZE            20

#define     MAX_ASSERT_BUF              1024
#define     ASSERT_START_FLAG           0xA5A5
#define     WATCHDOG_RESET_FLAG         0x5A5A
#define     ASSERT_START_MASK           0xFFFF
#define     NV_ASSERT_NUM               5
#define     NV_ASSERT_BASE              (NV_ITEM_ID_E)(NV_ASSERT_INFO0)
#define     NV_ASSERT_END               (NV_ASSERT_BASE + NV_ASSERT_NUM)

#define     MCU_IRQ_MODE                0x2
#define     MCU_FIQ_MODE                0x1
#define     MCU_SVC_MODE                0x3

#define     PRINT_STR_LEN_MAX           800
#define     MAX_PASSERT_INFO_STR        100    // Max size of passsert string
#define     ASSERT_MAX_COUNT            10

#define     PRINT_NAME_LEN_MAX          16

#define     ASSERT_PRINT                SCI_PrintAssertString

#define SCI_CARD_LOG_SAVE_IDLE          0x00
#define SCI_CARD_LOG_SAVE_BUSY          0x01

/**---------------------------------------------------------------------------*
 **                         Data Structure Definition                        *
 **---------------------------------------------------------------------------*/
#ifdef FLASH_TYPE_SPI
extern  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config;
#endif


typedef struct
{
    uint32 r0;
    uint32 r1;
    uint32 r2;
    uint32 r3;
    uint32 r4;
    uint32 r5;
    uint32 r6;
    uint32 r7;
    uint32 r8;
    uint32 r9;
    uint32 r10;
    uint32 r11;
    uint32 r12;
    uint32 r13;
    uint32 r14;
    uint32 PC;
    uint32 SPSR;
    uint32 CPSR;
} CUR_REG_T;

typedef struct
{
    uint32 r13;
    uint32 r14;
    uint32 SPSR;
} OTHER_REG_T;

typedef void *ASSERT_CMD_FUNC_PTR (void *exp, void *file, uint32 line);

typedef struct assert_cmd_list_tag
{
    uint8 cmd_name[64];
    ASSERT_CMD_FUNC_PTR *cmd_func;
    BOOLEAN cmd_auto_exec;
} ASSERT_CMD_LIST;

typedef struct IRAM_tag
{
    unsigned short assert_start_flag;
    unsigned short cur_nv_id;
    unsigned short len;
    unsigned short buf[MAX_ASSERT_BUF];
} IRAM_buf;


// Full context save area structure!
// It is used when an interrupt occurs!
typedef struct CONTEXT_FULL_SAVE_REG_tag
{
    uint32   int_stack;
    uint32   cpsr;
    uint32   r0;
    uint32   r1;
    uint32   r2;
    uint32   r3;
    uint32   r4;
    uint32   r5;
    uint32   r6;
    uint32   r7;
    uint32   r8;
    uint32   r9;
    uint32   r10;
    uint32   r11;
    uint32   r12;
    uint32   lr;
    uint32   pc;
} CONTEXT_FULL_SAVE_REG_T;

// Minimal context save area structure!
// It is used when a task is suspended normally!
typedef struct context_minimal_save_reg_tag
{
    uint32   int_stack;
    uint32   cpsr;
    uint32   r4;
    uint32   r5;
    uint32   r6;
    uint32   r7;
    uint32   r8;
    uint32   r9;
    uint32   r10;
    uint32   r11;
    uint32   r12;
    uint32   pc;
} CONTEXT_MINIMAL_SAVE_REG_T;

typedef enum
{
    NORMAL_E,
    EXCEPTION_E,
    IRQ_E,
    FIQ_E
} ASSERT_TYPE_E;

typedef enum
{
    NONE_EXCEPTION,
    UNDEFINED_EXCEPTION,
    SWI_EXCEPTION,
    PREFETCH_EXCEPTION,
    ABORT_EXCEPTION
} EXCEPTION_TYPE_E;


typedef enum
{
    PRINT_PATH_NORMAL = 0x00,
    PRINT_PATH_DIAG,

    PRINT_PATH_MAX
} PRINT_PATH_E;

typedef void (*FAT_ASSERTDUMP) (char *, uint32);

#ifdef WRE_SUPPORT
extern  BOOLEAN  WBOOT_IsWRERunning (void);

#define WRE_MAX_DLL_NUM             20
#define WRE_APP_NAME_MAX_NUM        40
#define WRE_CHECK_NODE_ID_NONE      0xFFFFFFFF

typedef struct wre_app_info_tag
{
    uint32 ap_id;
    uint32 ro_start;
    uint32 ro_end;
    uint32 rw_start;
    uint32 rw_end;
    char   ap_name[WRE_APP_NAME_MAX_NUM];
} WRE_APP_INFO_T;
typedef struct wre_all_info_tag
{
    uint32          dll_count;
    WRE_APP_INFO_T  wre_app[WRE_MAX_DLL_NUM];

} WRE_ALL_INFO_T;

void WreAddDll (char *ap_name_ptr,uint32 uID,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end);
void WreDelDll (char *ap_name_ptr,uint32 uID,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end);

#if defined(PLATFORM_UWS6121E)
extern void   ipc_nofity_cp_assert(void);
extern int LOAD_modem_isFinish(void);
#endif

WRE_ALL_INFO_T wre_dll_address_table = {0};
/**---------------------------------------------------------------------------*
 **                  after wre boot call it
 **---------------------------------------------------------------------------*/
PUBLIC void WreInitForAssert (void)
{
    memset (&wre_dll_address_table,0,sizeof (WRE_ALL_INFO_T));
}
/**---------------------------------------------------------------------------*
 **                  put wre app|wrd  rw addr to wre assert struct
 **---------------------------------------------------------------------------*/
void trace_wre_app_stack_info (void)
{
    uint16 i;
    //char buf[200];

    if (wre_dll_address_table.dll_count == 0)
    {
        SCI_TRACE_LOW (" ==== app node empty! ====\n");
    }
    else
    {
        for (i=0; i<wre_dll_address_table.dll_count; i++)
        {
            SCI_TRACE_LOW (" ======Node%2d.%-40s, ID:%08x, ROs: 0x%08x, ROe: 0x%08x, RWs: 0x%08x, RWe: 0x%08x  ======\n",
                           i+1,
                           wre_dll_address_table.wre_app[i].ap_name,
                           wre_dll_address_table.wre_app[i].ap_id,
                           wre_dll_address_table.wre_app[i].ro_start,
                           wre_dll_address_table.wre_app[i].ro_end,
                           wre_dll_address_table.wre_app[i].rw_start,
                           wre_dll_address_table.wre_app[i].rw_end);
        }
    }
}

PUBLIC void WreAddDll (char *ap_name_ptr,uint32 ap_id,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end)
{
#if 1   //david li, changed for easy testing
    uint16 i;

    for (i=0; i<wre_dll_address_table.dll_count; i++)
    {
        if (0 == strcmp (ap_name_ptr,wre_dll_address_table.wre_app[i].ap_name))
        {
            break;
        }
    }

    if (i < wre_dll_address_table.dll_count)
    {
        //allow upload for many times.
        wre_dll_address_table.wre_app[i].ap_id = ap_id;
        wre_dll_address_table.wre_app[i].ro_start = ro_start;
        wre_dll_address_table.wre_app[i].ro_end = ro_end;
        wre_dll_address_table.wre_app[i].rw_start = rw_start;
        wre_dll_address_table.wre_app[i].rw_end = rw_end;
    }
    else     if (i < WRE_MAX_DLL_NUM)
    {
        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].ap_id= ap_id;
        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].ro_start = ro_start;
        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].ro_end = ro_end;

        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].rw_start = rw_start;
        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].rw_end = rw_end;

        strncpy (wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].ap_name,ap_name_ptr,WRE_APP_NAME_MAX_NUM);
        wre_dll_address_table.dll_count++;
    }

    trace_wre_app_stack_info();
#else

    if (wre_dll_address_table.dll_count < WRE_MAX_DLL_NUM)
    {
        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].ro_start = ro_start;
        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].ro_end = ro_end;

        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].rw_start = rw_start;
        wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].rw_end = rw_end;

        strncpy (wre_dll_address_table.wre_app[wre_dll_address_table.dll_count].ap_name,ap_name_ptr,WRE_APP_NAME_MAX_NUM);
        wre_dll_address_table.dll_count++;
    }

#endif
}

/**---------------------------------------------------------------------------*
 **                  del a  wre app|wrd  from wre assert struct
 **---------------------------------------------------------------------------*/
PUBLIC void WreDelDll (char *ap_name_ptr,uint32 ap_id,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end)
{
    uint32    i=0;
    BOOLEAN   find_dll = FALSE;

    for (i = 0; i < wre_dll_address_table.dll_count; i++)
    {
        if ( (ap_id != WRE_CHECK_NODE_ID_NONE) && (ap_id == wre_dll_address_table.wre_app[i].ap_id))
        {
            find_dll = TRUE;
        }
        else if (0 == strcmp (ap_name_ptr,wre_dll_address_table.wre_app[i].ap_name))
        {
            find_dll = TRUE;
        }

        if (find_dll == TRUE)
        {
            break;
        }
    }

    if (TRUE == find_dll)
    {
        SCI_TRACE_LOW (" ==== remove node: %-40s, ID:%08x, ROs: 0x%08x, ROe: 0x%08x, RWs: 0x%08x, RWe: 0x%08x  =====\n",
                       wre_dll_address_table.wre_app[i].ap_name,
                       wre_dll_address_table.wre_app[i].ap_id,
                       wre_dll_address_table.wre_app[i].ro_start,
                       wre_dll_address_table.wre_app[i].ro_end,
                       wre_dll_address_table.wre_app[i].rw_start,
                       wre_dll_address_table.wre_app[i].rw_end);

        memset (& (wre_dll_address_table.wre_app[i]),0,sizeof (WRE_APP_INFO_T));

        if (i<wre_dll_address_table.dll_count-1)
        {
            memcpy (& (wre_dll_address_table.wre_app[i]),
                    & (wre_dll_address_table.wre_app[i+1]),
                    (wre_dll_address_table.dll_count-i-1) *sizeof (WRE_APP_INFO_T));
        }

        wre_dll_address_table.dll_count--;
    }

    trace_wre_app_stack_info();
}

LOCAL BOOLEAN WRE_IsRORegion (uint32 address)
{
    uint32    i=0;
    BOOLEAN   in_wre = FALSE;

    for (i=0; i<wre_dll_address_table.dll_count; i++)
    {
        if ( (address >=wre_dll_address_table.wre_app[i].ro_start)
                && (address <= wre_dll_address_table.wre_app[i].ro_end)
           )
        {
            in_wre = TRUE;
            break;
        }
    }

    return in_wre;
}


LOCAL uint32 WRE_whichRORegion (uint32 address)
{
    uint32    i=0;
    BOOLEAN   in_wre = FALSE;

    for (i=0; i<wre_dll_address_table.dll_count; i++)
    {
        if ( (address >=wre_dll_address_table.wre_app[i].ro_start)
                && (address <= wre_dll_address_table.wre_app[i].ro_end)
           )
        {
            in_wre = TRUE;
            break;
        }
    }

    if (in_wre)
    {
        return i;
    }
    else
    {
        return WRE_CHECK_NODE_ID_NONE;
    }
}

LOCAL BOOLEAN WRE_IsRWRegion (uint32 address)
{
    uint32    i=0;
    BOOLEAN   in_wre = FALSE;

    for (i=0; i<wre_dll_address_table.dll_count; i++)
    {
        if ( (address >=wre_dll_address_table.wre_app[i].rw_start)
                && (address <= wre_dll_address_table.wre_app[i].rw_end)
           )
        {
            in_wre = TRUE;
            break;
        }
    }

    return in_wre;
}

LOCAL BOOLEAN OS_WreCheckStackRegion (uint32 addr)
{
    if (WBOOT_IsWRERunning())
    {
        return (OS_CheckStackRegion (addr) || WRE_IsRWRegion (addr));
    }
    else
    {
        return OS_CheckStackRegion (addr);
    }
}

LOCAL BOOLEAN OS_WreCheckCodeRegion (uint32 addr)
{
    if (WBOOT_IsWRERunning())
    {
        return (OS_CheckCodeRegion (addr) || WRE_IsRORegion (addr));
    }
    else
    {
        return OS_CheckCodeRegion (addr);
    }
}

#endif

/**---------------------------------------------------------------------------*
 **               External  Function Prototypes                               *
 **---------------------------------------------------------------------------*/
extern void FAT_SystemAssert (uint8 *const buf,FAT_ASSERTDUMP dumpChars);
extern uint32  __tx_save_cur_reg (void);
#ifdef PRODUCT_DM
extern void sme_OS_DumpSmeBuff (void);
/*+cr241956*/
extern void sme_OS_DumpCtrlBuffPoolInfo (void);
/*-cr241956*/
extern void sme_flush_buffer (uint32 val);
extern void sme_print_func_entry_log (void);
extern void kal_debug_print_queue_info (void);
#endif
//extern void L1_Assert_Dsp (void);
//extern void dsp_soft_reset (uint16  flag);
//extern void DL_reset_tddsp (void);

/**---------------------------------------------------------------------------*
 **               External  Variables Prototypes                               *
 **---------------------------------------------------------------------------*/
// RTOS important parameter
extern TX_EVENT_FLAGS_GROUP_PTR _tx_event_flags_created_ptr;
extern TX_MUTEX_PTR             _tx_mutex_created_ptr;
extern TX_SEMAPHORE_PTR         _tx_semaphore_created_ptr;
extern TX_INTERNAL_TIMER        *_tx_timer_list_head;
extern TX_TIMER_PTR             _tx_timer_created_ptr;
extern  TX_THREAD _tx_timer_thread;

//Define in Tx_illdb.s
extern uint32                   _tx_pc_val_before_fiq;
extern uint32                   _tx_pc_val_before_irq;
extern uint32                   abort_DFSR_reg;
extern uint32                   abort_FAR_reg;
extern CUR_REG_T                cur_mode_reg;
extern OTHER_REG_T              svc_mode_reg;
extern OTHER_REG_T              irq_mode_reg;
extern OTHER_REG_T              undefine_mode_reg;
extern OTHER_REG_T              abort_mode_reg;
extern OTHER_REG_T              fiq_mode_reg;

/**---------------------------------------------------------------------------*
 **                  Local  Function Prototypes                               *
 **---------------------------------------------------------------------------*/
LOCAL void _TXAS_Cmd_PrintHelpInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintAssertInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintImportRegisters (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_DumpMemory2File (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_DumpAllocatedMemInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintMemoryInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintThreadInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintStackInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintCallbackFuncList (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_ReadMemory (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintTaskTimeList (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_ReadMemoryTofile (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_DumpSwitchMemory (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintEventInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintMutexInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintSemaphoreInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintFatCtlBlkInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_AllAssertInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintVersionInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_ResetMCU (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintTimerInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintBytePool (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintPSFuncCallStack (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintPSQueueInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_PrintiramInfo (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_DumpRunningNV (void *exp, void *file, uint32 line);
LOCAL void _TXAS_Cmd_DumpFixedNV (void *exp, void *file, uint32 line);
#ifdef DSP_USB_LOG
LOCAL void _TXAS_Cmd_PrintDSPAssertInfo (void *exp, void *file, uint32 line);
#endif
LOCAL void _TXAS_Cmd_WriteMemory (void *exp, void *file, uint32 line);

/**---------------------------------------------------------------------------*
 **                      Local Variables  Definition                          *
 **---------------------------------------------------------------------------*/
// #define  DS_LOADING_TRACK
#ifdef DS_LOADING_TRACK
extern uint64 uart_time;
extern uint64 idle_time;
uint64 excute_time = 0;
uint64 excute_time_div = 0;
#endif

BOOLEAN is_dsp_assert = FALSE;

#ifndef MODEM_PLATFORM
extern void MMITHEME_PrintInfo (void *exp, void *file, uint32 line);
#endif

PUBLIC const ASSERT_CMD_LIST threadx_assert_list[] =
{
    /*lint -e64*/
    {"\nAssert Debug Menu:",                    NULL                              ,SCI_TRUE},
    {"0. Print help info.",                     _TXAS_Cmd_PrintHelpInfo           ,SCI_TRUE},
    {"1. Print assert info.",                   _TXAS_Cmd_PrintAssertInfo         ,SCI_TRUE},
    {"2. Print important registers.",           _TXAS_Cmd_PrintImportRegisters    ,SCI_TRUE},
    {"3. Dump all memory to a file.",           _TXAS_Cmd_DumpMemory2File         ,SCI_TRUE},
    {"4. Print memory allocated info.",         _TXAS_Cmd_DumpAllocatedMemInfo    ,SCI_TRUE},
    {"5. Print memory pools info.",             _TXAS_Cmd_PrintMemoryInfo         ,SCI_TRUE},
    {"6. Print tasks info.",                    _TXAS_Cmd_PrintThreadInfo         ,SCI_TRUE},
    {"7. Print stack info.",                    _TXAS_Cmd_PrintStackInfo          ,SCI_TRUE},
    {"8. Print callback function list.",        _TXAS_Cmd_PrintCallbackFuncList   ,SCI_TRUE},
    {"9. Read memory.",                         _TXAS_Cmd_ReadMemory              ,SCI_FALSE},
    {"a. Dump task usage.",                     _TXAS_Cmd_PrintTaskTimeList       ,SCI_TRUE},
    {"b. Dump definite memory to a file.",      _TXAS_Cmd_ReadMemoryTofile        ,SCI_FALSE},
    {"c. Dump TaskSwitch memory to a file.",    _TXAS_Cmd_DumpSwitchMemory        ,SCI_TRUE},
    {"d. Print Event list information.",        _TXAS_Cmd_PrintEventInfo          ,SCI_TRUE},
    {"e. Print Mutex list information.",        _TXAS_Cmd_PrintMutexInfo          ,SCI_TRUE},
    // {"f. Print timer list infomation.",      _TXAS_Cmd_PrintTimerInfo          ,SCI_TRUE}, can not use "f",becase "f" has been used for other CMD.
    {"g. Print Semaphore list information.",    _TXAS_Cmd_PrintSemaphoreInfo      ,SCI_TRUE},
    {"h. Print Fat system control block info.", _TXAS_Cmd_PrintFatCtlBlkInfo      ,SCI_TRUE},
    {"i. Print Byte Pool infomation",           _TXAS_Cmd_PrintBytePool           ,SCI_TRUE},
    {"j. Print PS tasks queue info",            _TXAS_Cmd_PrintPSQueueInfo        ,SCI_TRUE},
    {"k. Print PS function call stack",         _TXAS_Cmd_PrintPSFuncCallStack    ,SCI_TRUE},
    {"l. Print timer list infomation.",         _TXAS_Cmd_PrintTimerInfo          ,SCI_TRUE},
#ifdef DSP_USB_LOG
    {"m. Print DSP assert info.",         _TXAS_Cmd_PrintDSPAssertInfo          ,SCI_TRUE},
#endif
    {"n. Write memory.",                         _TXAS_Cmd_WriteMemory              ,SCI_FALSE},
#ifndef MODEM_PLATFORM
    {"r. Print MMI info.",                    MMITHEME_PrintInfo    ,SCI_TRUE},
#endif
    {"s. Dump Fixed NV  to file.",           _TXAS_Cmd_DumpFixedNV        ,SCI_TRUE},
    {"t. Print the all assert information.",    _TXAS_Cmd_AllAssertInfo           ,SCI_FALSE},
    {"u. Dump Running NV to file.",           _TXAS_Cmd_DumpRunningNV        ,SCI_TRUE},
    {"v. Print Version Information.",           _TXAS_Cmd_PrintVersionInfo        ,SCI_TRUE},
    {"w. Print iram Information.",           _TXAS_Cmd_PrintiramInfo        ,SCI_TRUE},
    {"z. Reset MCU.",                           _TXAS_Cmd_ResetMCU                ,SCI_FALSE},
    {"\0",                                      NULL                              ,SCI_FALSE}
    /*lint +e64*/
};

const char abort_fault_status_debug_list[][50] =
{
    {"Reserved"},                                       //0000
    {"Alignment fault, Domain invalid"},                //0001
    {"Reserved"},                                       //0010
    {"Alignment fault, Domain invalid"},                //0011
    {"Reserved"},                                       //0100
    {"Translation fault, Domain invalid"},              //0101
    {"Reserved"},                                       //0110
    {"Translation fault, Domain valid"},                //0111
    {"External abort, Domain valid"},                   //1000
    {"Domain fault"},                                   //1001
    {"External abort, Domain valid"},                   //1010
    {"Domain fault"},                                   //1011
    {"External abort on translation, Domain Invalid"},  //1100
    {"Permission fault, Domain valid"},                 //1101
    {"External abort on translation, Domain valid"},    //1110
    {"Permission fault, Domain valid"},                 //1111
};

PUBLIC const char task_supend_type[][20] =
{
    {"TX_READY"},
    {"TX_COMPLETED"},
    {"TX_TERMINATED"},
    {"TX_SUSPENDED"},
    {"TX_SLEEP"},
    {"TX_QUEUE_SUSP"},
    {"TX_SEMAPHORE_SUSP"},
    {"TX_EVENT_FLAG"},
    {"TX_BLOCK_MEMORY"},
    {"TX_BYTE_MEMORY"},
    {"TX_IO_DRIVER"},
    {"TX_FILE"},
    {"TX_TCP_IP"},
    {"TX_MUTEX_SUSP"},
};


LOCAL char              str_buf[PRINT_STR_LEN_MAX];
LOCAL char              cmd_buf[COMMAND_MAX_SIZE + 1];
LOCAL char              passert_info_str[MAX_PASSERT_INFO_STR];
LOCAL uint32            s_assert_flag       = SCI_FALSE;
LOCAL uint16            curNVID             = 0;
LOCAL ASSERT_TYPE_E     assert_type         = NORMAL_E;
LOCAL EXCEPTION_TYPE_E  s_exception_type    = NONE_EXCEPTION;
LOCAL BOOLEAN           b_is_watchdog       = SCI_FALSE;
LOCAL BOOLEAN           b_is_passert        = SCI_FALSE;
LOCAL uint32            last_sp             = 0;
LOCAL uint32            last_lr             = 0;
LOCAL uint32            func_entry          = 0;
LOCAL uint32            s_is_current_thread_saved = SCI_FALSE;
LOCAL BOOLEAN           s_is_dsp_run        = SCI_FALSE;
LOCAL BOOLEAN           s_is_assert         = SCI_FALSE ;
LOCAL uint32            s_assert_cnt        = 0;
LOCAL ASSERT_CMD_LIST   *s_cmd_list_ptr     = NULL;
LOCAL uint8             *s_assert_info_ptr  = NULL;
uint32 g_card_log_save_status = SCI_CARD_LOG_SAVE_IDLE;
#endif // End of WIN32


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*****************************************************************************/
//  Description:    The function is used to set assert mode
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_SetAssert (BOOLEAN is_assert)
{
    s_is_assert = is_assert;
    s_assert_cnt++;
}

/*****************************************************************************/
//  Description:    The function is used to set a global flag. The flag
//                  indicates whether SCI_Assert
//                  be called.
//  Global resource dependence:
//  Author:         rick.zhang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_SetAssertFlag (uint32 flag)
{
    s_assert_flag = flag;
}

/*****************************************************************************/
//  Description:    The function is used to read assert Info from NV and send to the tool.
//  Global resource dependence:
//  Author:         Johnson.sun
//  Note:
/*****************************************************************************/
LOCAL uint32 _TXAS_HandleReadAssertInfo (
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Length of input message.
)
{
    MSG_HEAD_T  *msg_head;
    uint16      len;
    uint16      id;
    uint8       status;
    uint16      sum_len;
    char        *buf;
    char        *temp_buf;
    char        *str="Recently 10 Assert Informations:";
    char        id_msg_buf[] = "\r\n 0-> ";
    uint8       id_msg_size  = strlen (id_msg_buf);


    buf = (char *) SCI_ALLOC_APP (MAX_ASSERT_BUF * 11);
    SCI_ASSERT (SCI_NULL != buf);/*assert verified*/

    sum_len = strlen (str);
    strcpy ( (char *) buf, (const char *) str);
    strcat ( (char *) (buf+sum_len), "\r\n");
    sum_len = sum_len + 2;

    for (id = NV_ASSERT_BASE; id < NV_ASSERT_END; id++)
    {
        len = NVITEM_GetLength (id);

        id_msg_buf[3] = '0' + id%10;

        if (len > 0 + 2)
        {
            temp_buf = (char *) SCI_ALLOC_APP (MAX_ASSERT_BUF);
            SCI_ASSERT (SCI_NULL != temp_buf);/*assert verified*/
            status = EFS_NvitemRead (id, len, (uint8 *) temp_buf);

            if (NVERR_NONE == status)
            {
                strcat ( (char *) (buf+sum_len), id_msg_buf);
                strcat ( (char *) (buf+sum_len+ id_msg_size), (const char *) (temp_buf+2));
                sum_len = sum_len + id_msg_size + len  - 2;
            }

            SCI_FREE (temp_buf);
        }
    }

    msg_head = SCI_ALLOC_APP (sum_len + sizeof (MSG_HEAD_T));
    SCI_ASSERT (SCI_NULL != msg_head);/*assert verified*/
    *dest_ptr       = (uint8 *) msg_head;
    *dest_len_ptr   = sum_len + sizeof (MSG_HEAD_T);

    // Copy head structure.
    SCI_MEMCPY ( (void *) msg_head, src_ptr, sizeof (MSG_HEAD_T));
    msg_head->len = sum_len + sizeof (MSG_HEAD_T);

    // Copy Assert Info string.
    SCI_MEMCPY ( (void *) (msg_head + 1), buf, sum_len);
    // Set response packet length.
    msg_head->len = *dest_len_ptr;

    SCI_FREE (buf);
    return 1;
}

/*****************************************************************************/
//  Description:    The function is used to reinit the low level when assert
//                  before product init
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_InitSystemBeforeBootUp (void)
{
    SIO_CONTROL_S   m_dcb;

    //do some important module init
    DMA_HAL_Init();
#if !defined(PLATFORM_UWS6121E)  //temp_stub_sgq
    GPIO_Init();
    ADC_Init();
    LCD_Init();
#endif
    //connet the debug port(default Uart0, if usb wire plug in, can use the usb log)
    //creat COM_DEBUG
#if !defined(PLATFORM_UWS6121E)  //temp_stub_sgq
    m_dcb.baud_rate      = BAUD_115200;
    SIO_Create (COM_DEBUG, COM0, &m_dcb);
#else
     m_dcb.baud_rate      = BAUD_921600;
     m_dcb.flow_control   = HW_FLOW_CONTROL;
    SIO_Create (COM_DEBUG, COM1, &m_dcb);
#endif
    //init sio log buffer first
    SIO_ResetBuf (COM_DEBUG);
}

#ifndef WIN32

/*****************************************************************************/
//  Description:    The function is used to get a command line from pc
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_GetALine (char *str)
{
    char ch;
    uint32 i;
    /* Get command string. */
    for (i = 0; i < COMMAND_MAX_SIZE; i++) {
        ch = OS_DebugGetChar();
        if ( (ch != '\n') && (ch != 0x7e)) {
            * (str + i) = ch;
        } else {
            break;
        }
    }
    * (str + i) = '\0';
}


/*****************************************************************************/
//  Description:    The function is used to convert string to number
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 _TXAS_GetNumFromStr (uint32 *value_ptr, char *src_ptr)
{
    char    base[3];
    char    *end_ptr;

    // Check input parameter.
    if (SCI_NULL == src_ptr)
    {
        return SCI_ERROR;
    }

    // Get base of the value.
    base[0] = *src_ptr;
    base[1] = * (src_ptr + 1);
    base[2] = '\0';

    // Check the base of value, then convert from string to uint32.
    if ( (0 == strcmp ( (char *) base, "0B")) || (0 == strcmp ( (char *) base, "0b")))
    {
        // Value base is 2.
        *value_ptr = strtoul ( (char *) (src_ptr + 2), &end_ptr, 2);
    }
    else if ( (0 == strcmp ( (char *) base, "0O")) || (0 == strcmp ( (char *) base, "0o")))
    {
        // Value base is 8.
        *value_ptr = strtoul ( (char *) (src_ptr + 2), &end_ptr, 8);
    }
    else if ( (0 == strcmp ( (char *) base, "0X")) || (0 == strcmp ( (char *) base, "0x")))
    {
        // Value base is 16.
        *value_ptr = strtoul ( (char *) (src_ptr + 2), &end_ptr, 16);
    }
    else if ( (0 == strcmp ( (char *) base, "0D")) || (0 == strcmp ( (char *) base, "0d")))
    {
        // Value base is 10.
        *value_ptr = strtoul ( (char *) (src_ptr + 2), &end_ptr, 10);
    }
    else if ( (base[0] >= '0') && (base[0] <= '9'))
    {
        // Value base is 10.
        *value_ptr = strtoul ( (char *) src_ptr, &end_ptr, 10);
    }
    else
    {
        // Error src_ptr.
        end_ptr = src_ptr;
        *value_ptr = 0;

        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    Check parameters of access MCU command.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:           If the parameters is valid, return MCU_ACCESS_SUCCESS;
//                  else return error code.
//
//                  ADDRESS                      ACCESS_MODE     Read/Write
//                  0x00000000 - 0x01FFFFFFF     B/W/D           R
//                  0x02000000 - 0x0FFFFFFFF     B/W/D           R/W
//                  0x10000000 - 0x1FFFFFFFF     B               R
//                                               W               R/W
//                  0x20000000 - 0x2FFFFFFFF     D               R/W
//                  0x40000000 - 0x4FFFFFFFF     B/W/D           R/W
//                  0x80000000 - 0xBFFFFFFFF     B/W/D           R/W
//                  0xFFFFFFFC                   D               R
/*****************************************************************************/
LOCAL uint32 _TXAS_CheckMemAccessParam (
    uint32  start_addr,     // Access start address.
    uint32  size            // Size to be access.
)
{
    uint32  status          = 0;
    uint32  end_addr        = 0;
    uint32  size_in_byte    = size;

    end_addr = start_addr + size_in_byte;

    // Check access mode(BYTE/WORD/DWORD) and Read/Write.

#if defined(PLATFORM_UWS6121E)

    if (start_addr <= 0x000FFFFF)
    {
        if (end_addr <= 0x000FFFFF)
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x00800000) && (start_addr <= 0x00881FFF))
    {
        if ( (end_addr >= 0x00800000) && (end_addr <= 0x00881FFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x08200000) && (start_addr <= 0x09FFFFFF))
    {
        if ( (end_addr >= 0x08200000) && (end_addr <= 0x09FFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x10000000) && (start_addr <= 0x15FFFFFF))
    {
        if ( (end_addr >= 0x10000000) && (end_addr <= 0x15FFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }

    }
    else if ( (start_addr >= 0x20000000) && (start_addr <= 0x27FFFFFF))
    {
        if ( (end_addr >= 0x20000000) && (end_addr <= 0x27FFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x40000000) && (start_addr <= 0x4001EFFF))
    {
        if ( (end_addr >= 0x40000000) && (end_addr <= 0x4001EFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x50000000) && (start_addr <= 0x503FFFFF))
    {
        if ( (end_addr >= 0x50000000) && (end_addr <= 0x503FFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x60000000) && (start_addr <= 0x67FFFFFF))
    {
        if ( (end_addr >= 0x60000000) && (end_addr <= 0x67FFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x70000000) && (start_addr <= 0x7FFFFFFF))
    {
        if ( (end_addr >= 0x70000000) && (end_addr <= 0x7FFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x80000000) && (start_addr <= 0xBFFFFFFF))
    {
        if ( (end_addr >= 0x80000000) && (end_addr <= 0xBFFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else
    {
        status = SCI_SUCCESS;
    }



#else
    if (start_addr <= 0x01FFFFFF)
    {
        if (end_addr <= 0x01FFFFFF)
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x02000000) && (start_addr <= 0x0FFFFFFF))
    {
        if ( (end_addr >= 0x02000000) && (end_addr <= 0x0FFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x10000000) && (start_addr <= 0x1FFFFFFF))
    {
        if ( (end_addr >= 0x10000000) && (end_addr <= 0x1FFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }

    }
    else if ( (start_addr >= 0x20000000) && (start_addr <= 0x2FFFFFFF))
    {
        if ( (end_addr >= 0x20000000) && (end_addr <= 0x2FFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x40000000) && (start_addr <= 0x4FFFFFFF))
    {
        if ( (end_addr >= 0x40000000) && (end_addr <= 0x4FFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x70000000) && (start_addr <= 0x7FFFFFFF))
    {
        if ( (end_addr >= 0x70000000) && (end_addr <= 0x7FFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if ( (start_addr >= 0x80000000) && (start_addr <= 0xBFFFFFFF))
    {
        if ( (end_addr >= 0x80000000) && (end_addr <= 0xBFFFFFFF))
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else if (start_addr == 0xFFFFFFFC)
    {
        if (end_addr == 0x0)
        {
            status = SCI_SUCCESS;
        }
        else
        {
            status = SCI_ERROR;
        }
    }
    else
    {
        status = SCI_SUCCESS;
    }

#endif
    return status;
}


/*****************************************************************************/
//  Description:    The _TXAS_GetCallbackFunAddr function is used to get the
//                  function address called in the stack.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#ifdef ARM_ARCH_V7

#define THUMB_32BITS_SUB  (0x1a0)//(0b1101<<5)
#define THUMB_32BITS_SUB_2  (0x0a0)//(0b0101<<5)
#define THUMB_32BITS_ADD  0x100 //(0b1000<<5)

#define THUMB_32BITS_SP_IMMEDIATE_CODE  0xd
#define THUMB_32BITS_SP_IMMEDIATE_MASK  0xf

#define THUMB_32BITS_A   (0xe8<<8) //(0b11101<<11)
#define THUMB_32BITS_B   (0xf0<<8)//(0b11110<<11)
#define THUMB_32BITS_C   (0xf8<<8)//(0b11111<<11)

#define THUMB_32BITS_MARK (0xf8<<8)//(0b11111<<11)

#define THUMB_32BITS_DATA_PRO_OPE  (BIT_5|BIT_6|BIT_7|BIT_8)

#define THUMB_32BITS_OP1 (BIT_11|BIT_12)
#define THUMB_32BITS_OP2 ( BIT_4|BIT_5|BIT_6|BIT_7|BIT_8|BIT_9|BIT_10)
#define THUMB_32BITS_OP   BIT_15

#define ARM_SUB_MARK 0xfef
#define ARM_SUB_CODE 0x24d
#define ARM_SUB_RD_MARK 0xF000

extern void _TXAS_SaveMainReg (uint32 *cur_sp, uint32 *cur_lr, uint32 *cur_pc);

LOCAL uint32 ROR_shift(uint32 data_in, uint8 bits_count,uint8 shift_count)
{
    uint32 n=0;
    uint32 data_out=0;
    if(!shift_count||!bits_count)
    {
        return data_in;
    }
    n=shift_count%bits_count;
    data_out=(data_in>>n)|(data_in<<(bits_count-n));
    return data_out;
}


LOCAL uint32 ARM_Expand_Imm(uint32 imm)
{
    uint32 data_out=0;
    uint32 unrotate_value=imm&0xff;
    uint8 shift_count =2*(imm>>8);

    data_out=ROR_shift(unrotate_value,32,shift_count);
    return data_out;
}


LOCAL uint32 THUMB_Expand_Imm(uint32 imm)
{
    uint8 temp=0;
    uint32 imm_out=0;
    uint8 imm_base=imm&0xff;

    temp=(((imm&BIT_8) | (imm&BIT_9))>>8);

    if((!(imm&BIT_10))&&(!(imm&BIT_11)))
    {
        switch( temp )
        {
            case 0:
                imm_out=imm_base;
                break;
            case 1:
                imm_out=(imm_base<<16) | imm_base;
                break;
            case 2:
                if(imm_base)
                {
                    imm_out=(imm_base <<24)|(imm_base<<8);
                }
                else
                {
                    SCI_ASSERT(0);
                }
                break;
            case 3:
                if(imm_base)
                {
                    imm_out=(imm_base<<24)|(imm_base<<16)|(imm_base<<8)|imm_base;
                }
                break;

            default:
                imm_out=0;
                break;

        }


    }
    else
    {
        uint32 unrotate_value = imm_base | 0x80;
        uint8 shift_count=(imm&0xF80)>>7;

        imm_out=ROR_shift(unrotate_value,32,shift_count);

    }

    return imm_out;
}


LOCAL uint8 bit_1_count(uint16 reg_list)
{
    int32 i=0;
    uint8 bits_1_count=0;
    for (i = 0; i < SIZE_OF_REG_LIST; i++)
    {
        if (reg_list & (1 << i))
        {
            bits_1_count++;
        }
    }

    return bits_1_count;
}




LOCAL uint32 _TXAS_GetCallbackFunAddr (uint32 *sp, uint32 *lr, uint32 *fun)
{
    uint16  loccal_var_size = 0;    // stack size allocated for local variable

    uint16  *pc_ptr         = (uint16 *) *lr;

    // Warning: The code below here can only work for THUMB code!
    for(;;)
    {
        uint16  saved_reg_size  = 0;    // stack size for saving registers, last one is lr(r14)
        // PC pointer reached the top of the program!
        if ( (uint32) pc_ptr <= TOP_OF_PROGRAM)
        {
            return SCI_PARAM_ERROR;
        }

        if (!OS_CheckCodeRegion ( (uint32) pc_ptr))
        {
            return SCI_ERROR;
        }

        /*pc ptr is invalid*/
        if ( (uint32) pc_ptr & 0x01)
        {
            pc_ptr=(uint32) pc_ptr - 0x01;
//            ASSERT_PRINT ("\talign fault!, add:0x%0x\n", pc_ptr);
     //       return SCI_ERROR;
        }

#ifdef VM_SUPPORT
       //fix bug1009100.avoid to exceed the start virture address boundary and pc_ptr++
        if((uint32)pc_ptr == MEM_GetVMStartAddr())
        pc_ptr++;
#endif

        // check 32bits thumb instructions
        if((((*(pc_ptr-1)) &THUMB_32BITS_MARK) == THUMB_32BITS_A)
            ||(((*(pc_ptr-1))&THUMB_32BITS_MARK) == THUMB_32BITS_B)
            ||(((*(pc_ptr-1))&THUMB_32BITS_MARK) ==THUMB_32BITS_C))
        {

            if((((*(pc_ptr-1))&THUMB_32BITS_OP1)== (0x1<<12)) && (!((*(pc_ptr))&THUMB_32BITS_OP)))
            {
                 // process SUB  SP   1
                if((((*(pc_ptr-1))&THUMB_32BITS_DATA_PRO_OPE)==THUMB_32BITS_SUB)
                &&(((*(pc_ptr-1))&THUMB_32BITS_SP_IMMEDIATE_MASK)==THUMB_32BITS_SP_IMMEDIATE_CODE))
                {
                    int32 i= ((*(pc_ptr-1))&BIT_10)>>10;
                    int32 im3=((*(pc_ptr))&(BIT_14|BIT_13|BIT_12))>>12;
                    int32 im8=((*(pc_ptr))&0xff);
                    uint8 Rd=((*(pc_ptr))&0xf00)>>8;

                    if(Rd ==13)
                    {
                        loccal_var_size +=  THUMB_Expand_Imm((i<<11)|(im3<<8)|im8)/4;
                    }

                }

                // process SUB  SP    2
                else if((((*(pc_ptr-1))&THUMB_32BITS_DATA_PRO_OPE)==THUMB_32BITS_SUB_2)
                &&(((*(pc_ptr-1))&THUMB_32BITS_SP_IMMEDIATE_MASK)==THUMB_32BITS_SP_IMMEDIATE_CODE))
                {
                    int32 i= ((*(pc_ptr-1))&BIT_10)>>10;
                    int32 im3=((*(pc_ptr))&(BIT_14|BIT_13|BIT_12))>>12;
                    int32 im8=((*(pc_ptr))&0xff);
                    uint8 Rd=((*(pc_ptr))&0xf00)>>8;


                    if(Rd ==13)
                    {
                        loccal_var_size += ((i<<11)|(im3<<8)|im8)/4;
                    }

                }

                //process  ADD SP

                else if((((*(pc_ptr-1))&THUMB_32BITS_DATA_PRO_OPE) == THUMB_32BITS_ADD)
                &&(((*(pc_ptr-1))&THUMB_32BITS_SP_IMMEDIATE_MASK)==THUMB_32BITS_SP_IMMEDIATE_CODE))
                {
                    int32 i= ((*(pc_ptr-1))&BIT_10)>>10;
                    int32 im3=((*(pc_ptr))&(BIT_14|BIT_13|BIT_12))>>12;
                    int32 im8=((*(pc_ptr))&0xff);
                    uint8 Rd=((*(pc_ptr))&0xf00)>>8;

                    if(Rd ==13)
                    {
                        loccal_var_size -=  THUMB_Expand_Imm((i<<11)|(im3<<8)|im8)/4;
                    }
                }


            }


            // Process PUSH  more than one registers OPE
            else if((((*(pc_ptr-1))&THUMB_32BITS_OP1) == (0x1<<11)) &&(((*(pc_ptr-1))&THUMB_32BITS_OP2) == (0x12<<4))
            &&(!((*(pc_ptr))&THUMB_32BITS_OP)))
            {
                uint16 reg_list= *(pc_ptr)&0x5fff;
                uint32 M_bit=*(pc_ptr)&0x4000;
                saved_reg_size=bit_1_count(reg_list);
                loccal_var_size += saved_reg_size;

                if(M_bit)
                {
                    *fun = (uint32) (pc_ptr-1);
                    break;
                }

            }

            // Process PUSH  only one register OPE
            //coverity 18422. fix the code defect
            else if(((*(pc_ptr-1))&THUMB_32BITS_OP1 == (0x3<<11))&&(((*(pc_ptr-1))&THUMB_32BITS_OP2) == (0x04<<4))
            &&((*pc_ptr)&0x0FFF==0xD04))
            {
                uint8 Rt= ((*pc_ptr)&(BIT_12|BIT_13|BIT_14|BIT_15))>>12;
                loccal_var_size +=1;
                if(Rt==14)
                {
                    *fun = (uint32) (pc_ptr-1);
                    break;
                }
            }

            pc_ptr --;

        }

        // Operator SUB SP!
        else if ( (*pc_ptr & THUMB_SUB_SP_CODE_MASK) == THUMB_SUB_SP_CODE)
        {
            loccal_var_size += *pc_ptr & (~THUMB_SUB_SP_CODE_MASK);
        }

        // Operator ADD SP!
        else if ( (*pc_ptr & THUMB_ADD_SP_CODE_MASK) == THUMB_ADD_SP_CODE)
        {
            loccal_var_size -= *pc_ptr & (~THUMB_ADD_SP_CODE_MASK);

            /* Richard add here for pop operation!*/
            /* We may met some cases, in which we may call
               function implemented in .s files.
               It will be SUB to negative value.*/
            if (loccal_var_size >= 0xf000)
                /* We just simply recover the value!
                   Warning: It will failed some cases! */
            {
                loccal_var_size += (*pc_ptr) & (~THUMB_ADD_SP_CODE_MASK);
            }

            /* Richard add here for pop operation!*/

            pc_ptr--;
        }

        /* PUSH operator! */
        else if ( (*pc_ptr & THUMB_PUSH_CODE_MASK) == THUMB_PUSH_CODE)
        {
            uint16 reg_list;
            uint16 i;

            reg_list = *pc_ptr & (~THUMB_PUSH_CODE_MASK);
            // Calculate number of registers are pushed in stack
            saved_reg_size = bit_1_count(reg_list);;
            loccal_var_size += saved_reg_size;
            /* Well, Here is the entry for a function!*/
            if (reg_list & 0x0100)
            {
                // This push instruction pushs r14 also, so it must be the entry of the function
                *fun = (uint32) pc_ptr;
                break;
            }
        }

        pc_ptr--;
    }

    // lr is the last one in stack after push(r14..) has been executed
    *sp += loccal_var_size*4;

    if (! (OS_CheckStackRegion (*sp) && OS_CheckCodeRegion (* (uint32 *) (*sp - 4))))
    {
        *lr = 0;

        return SCI_ERROR;
    }
    else
    {
        *lr = * (uint32 *) (*sp - 4);
    }

    // Return address should be even
    // However lr saved in stack is alway odd
    if (*lr & 0x01)
    {
        //Skip the BL instruction and the one next to it(maybe a add sp instruction)
        *lr -= (1 + 6);
    }

    return SCI_SUCCESS;
}





#else



/*****************************************************************************/
//  Description:    The _TXAS_GetCallbackFunAddr function is used to get the
//                  function address called in the stack.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL uint32 _TXAS_GetCallbackFunAddr (uint32 *sp, uint32 *lr, uint32 *fun)
{
    uint16  loccal_var_size = 0;    // stack size allocated for local variable
    uint16  saved_reg_size  = 0;    // stack size for saving registers, last one is lr(r14)
    uint16  *pc_ptr         = (uint16 *) *lr;
    uint16  reg_list        = 0;
    uint16  i               = 0;
    uint32  dwCode          = 0;

    // Warning: The code below here can only work for THUMB code!
    for (;;)
    {
        // PC pointer reached the top of the program!
        if ( (uint32) pc_ptr <= TOP_OF_PROGRAM)
        {
            return SCI_PARAM_ERROR;
        }

#ifdef WRE_SUPPORT

        if (!OS_WreCheckCodeRegion ( (uint32) pc_ptr))
#else
            if (!OS_CheckCodeRegion ( (uint32) pc_ptr))
#endif
            {
                return SCI_ERROR;
            }

        /*pc ptr is invalid*/
        if ( (uint32) pc_ptr & 0x01)
        {
            ASSERT_PRINT ("\talign fault!, add:0x%0x\n", pc_ptr);
            return SCI_ERROR;
        }

        // Since BL Code is 4 byte always.We should sub 2 command here!
        if ( (* (pc_ptr - 1) & THUMB_BL_MASK) == THUMB_BL_CODE)
        {
            if ( (* (pc_ptr - 2) & THUMB_BL_MASK) == THUMB_BL_MASK)
            {
                pc_ptr--;
            }
        }

        // Operator SUB SP!
        if ( (*pc_ptr & THUMB_SUB_SP_CODE_MASK) == THUMB_SUB_SP_CODE)
        {
            loccal_var_size += *pc_ptr & (~THUMB_SUB_SP_CODE_MASK);
            pc_ptr--;
        }

        // Operator ADD SP!
        if ( (*pc_ptr & THUMB_ADD_SP_CODE_MASK) == THUMB_ADD_SP_CODE)
        {
            loccal_var_size -= *pc_ptr & (~THUMB_ADD_SP_CODE_MASK);

            /* Richard add here for pop operation!*/
            /* We may met some cases, in which we may call
               function implemented in .s files.
               It will be SUB to negative value.*/
            if (loccal_var_size >= 0xf000)
                /* We just simply recover the value!
                   Warning: It will failed some cases! */
            {
                loccal_var_size += (*pc_ptr) & (~THUMB_ADD_SP_CODE_MASK);
            }

            /* Richard add here for pop operation!*/

            pc_ptr--;
        }

        /* PUSH operator! */
        if ( (*pc_ptr & THUMB_PUSH_CODE_MASK) == THUMB_PUSH_CODE)
        {
            reg_list = *pc_ptr & (~THUMB_PUSH_CODE_MASK);
            // Calculate number of registers are pushed in stack
            saved_reg_size = 0;

            for (i = 0; i < SIZE_OF_REG_LIST; i++)
            {
                if (reg_list & (1 << i))
                {
                    saved_reg_size++;
                }
            }

            /* Well, Here is the entry for a function!*/
            if (reg_list & 0x0100)
            {
                // This push instruction pushs r14 also, so it must be the entry of the function
                *fun = (uint32) pc_ptr;
                break;
            }
            /* Simply push operator! */
            else
            {
                loccal_var_size += saved_reg_size;
            }
        }

        /*LTMDB SP!, {Rn,R14} operatior!*/
        dwCode = * (uint16 *) pc_ptr;

        if (ARM_LTMDB_CODE == (dwCode & ARM_LTMDB_CODE_MASK))
        {
            // Calculate number of registers are pushed in stack
            saved_reg_size = 0;

            for (i = 0; i < SIZE_OF_REG_LIST_1; i++)
            {
                if (dwCode & (1 << i))
                {
                    saved_reg_size += 1;
                }
            }

            if (BIT_14 & dwCode)
            {
                // This instruction pushs r14 also, so it must be the entry of the function
                saved_reg_size += 1;
                *fun = (uint32) pc_ptr;
                break;
            }
            else
            {
                loccal_var_size += saved_reg_size;
            }
        }

        pc_ptr--;
    }

    // lr is the last one in stack after push(r14..) has been executed
    *sp += (loccal_var_size + saved_reg_size) * 4;


#ifdef WRE_SUPPORT

    if (! (OS_WreCheckStackRegion (*sp) && OS_WreCheckCodeRegion (* (uint32 *) (*sp - 4))))
#else
        if (! (OS_CheckStackRegion (*sp) && OS_CheckCodeRegion (* (uint32 *) (*sp - 4))))
#endif
        {
            *lr = 0;

            return SCI_ERROR;
        }
        else
        {
            *lr = * (uint32 *) (*sp - 4);
        }

    // Return address should be even
    // However lr saved in stack is alway odd
    if (*lr & 0x01)
    {
        //Skip the BL instruction and the one next to it(maybe a add sp instruction)
        *lr -= (1 + 6);
    }

    return SCI_SUCCESS;
}



#endif


/*****************************************************************************/
//  Description:    The _SCI_SaveMainReg function is used to save sp/lr/pc register
//                  _DEBUG.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL __inline void _TXAS_SaveMainReg (uint32 *cur_sp, uint32 *cur_lr, uint32 *cur_pc)
{
#ifndef WIN32
#if __ARMCC_VERSION > 220000  /*lint !e553*/

#pragma arm
    __asm
    {
        MOV *cur_sp,    __current_sp()
        MOV *cur_lr,    __return_address()
        MOV *cur_pc,    __current_pc()
    }
#if  (1) //defined(RVCT_VERSION_V504)
#pragma arm
#else
#pragma thumb
#endif
#else
    __asm
    {
        MOV *cur_sp,    r13
        MOV *cur_lr,    r14
        MOV *cur_pc,    r15
    }

#endif // __ARMCC_VERSION
#endif // WIN32
}


/*****************************************************************************/
//  Description:    The _TXAS_GetStackBacktrace function is used to search and
//                  print a thread's stack callback function information.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL uint32 _TXAS_GetStackBacktrace (
    SCI_THREAD_T *thread_block            // Thread Block
)
{
    uint32          status;
    void            *m_reg_ptr;
    SCI_THREAD_T    *cur_thread_block   = SCI_NULL;


    cur_thread_block = (SCI_THREAD_T *) SCI_GetThreadBlockFromId (SCI_IdentifyThread());

    if (thread_block == cur_thread_block && !s_is_current_thread_saved)
    {
        return SCI_SUCCESS;
    }

    if (SCI_NULL == thread_block)
    {
        return SCI_ERROR;
    }

    m_reg_ptr = (void *) ( (TX_THREAD *) &thread_block->thread)->tx_stack_ptr;
    status = * ( (uint32 *) m_reg_ptr);

    if (status == 0)
    {
        last_sp = (uint32) m_reg_ptr + sizeof (CONTEXT_MINIMAL_SAVE_REG_T);
        last_lr = ( (CONTEXT_MINIMAL_SAVE_REG_T *) m_reg_ptr)->pc;

        // Well context switch occured normally.
        // We should sub last_lr by 7 to the last call PC address.
        last_lr -= 7;
    }
    else    // status == 1
    {
        last_sp = (uint32) m_reg_ptr + sizeof (CONTEXT_FULL_SAVE_REG_T);
        last_lr = ( (CONTEXT_FULL_SAVE_REG_T *) m_reg_ptr)->pc;
    }

    ASSERT_PRINT ("\nTask %ld (%s):\n", thread_block->thread_id, ( (TX_THREAD *) &thread_block->thread)->tx_thread_name);

    status = _TXAS_GetCallbackFunAddr (&last_sp, &last_lr, &func_entry);

    while (status == SCI_SUCCESS)
    {
        //david li, 2011-12-08, here to outprint WRE app entry info
#ifdef  WRE_SUPPORT
        uint32  which;

        which = WRE_whichRORegion (func_entry);

        if (which == WRE_CHECK_NODE_ID_NONE)
        {
            ASSERT_PRINT ("\tEntry at [0x%08lx] in mocor\n", func_entry);
        }
        else
        {
            //ASSERT_PRINT ("=== Absolute address: 0x%08lx ===", func_entry);
            //ASSERT_PRINT ("=== Relative address : 0x%08lx, in app: %s ===", func_entry-wre_dll_address_table.wre_app[which].ro_start, wre_dll_address_table.wre_app[which].ap_name);
            //ASSERT_PRINT ("\tEntry at [0x%08lx]\n", (func_entry-wre_dll_address_table.wre_app[which].ro_start));
            ASSERT_PRINT ("\tEntry at [0x%08lx] in '%s'\n", (func_entry-wre_dll_address_table.wre_app[which].ro_start), wre_dll_address_table.wre_app[which].ap_name);
        }

#else
        ASSERT_PRINT ("\tEntry at [0x%-8lx]\n", (uint32) func_entry);
#endif

        // Reach top of the stack!
        if (last_sp >= (uint32) ( (TX_THREAD *) &thread_block->thread)->tx_stack_end - 8)
        {
            break;
        }

        status = _TXAS_GetCallbackFunAddr (&last_sp, &last_lr, &func_entry);
    }

    if (SCI_ERROR == status)
    {
        ASSERT_PRINT ("\t......\n");
    }

    ASSERT_PRINT ("***Reach top of stack ***\n");

    return 0;
}

/*****************************************************************************/
//  Description:    The function is used to trace the callback list of a threadx
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_TraceCallbackList (void)
{
    uint32 status;

    for (;;)
    {
        status = _TXAS_GetCallbackFunAddr (&last_sp, &last_lr, &func_entry);

        if (SCI_SUCCESS != status)
        {
            break;
        }

        //david li, 2011-12-08, here to outprint WRE app entry info
#ifdef  WRE_SUPPORT
        {
            uint32  which;
            which = WRE_whichRORegion (func_entry);

            if (which == WRE_CHECK_NODE_ID_NONE)
            {
                ASSERT_PRINT ("\tEntry at [0x%08lx] in macor\n", func_entry);
            }
            else
            {
                //ASSERT_PRINT ("=== Absolute address: 0x%08lx ===", func_entry);
                //ASSERT_PRINT ("=== Relative address : 0x%08lx, in app: %s ===", func_entry-wre_dll_address_table.wre_app[which].ro_start, wre_dll_address_table.wre_app[which].ap_name);
                //ASSERT_PRINT ("\tEntry at [0x%08lx]\n", (func_entry-wre_dll_address_table.wre_app[which].ro_start));
                ASSERT_PRINT ("\tEntry at [0x%08lx] in '%s'\n", (func_entry-wre_dll_address_table.wre_app[which].ro_start), wre_dll_address_table.wre_app[which].ap_name);
            }
        }
#else
        ASSERT_PRINT ("\tEntry at [0x%-8lx]\n", (uint32) func_entry);
#endif

#ifdef WRE_SUPPORT

        if (!OS_WreCheckStackRegion (last_sp))
#else
            if (!OS_CheckStackRegion (last_sp))
#endif
            {
                break;
            }
    }

    if (SCI_ERROR == status)
    {
        ASSERT_PRINT ("\t......\n");
    }

    ASSERT_PRINT ("***Reach top of stack.***\r\n");

}

/*****************************************************************************/
//  Description:    The function is used to get the value of current mode SPSR
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 _TXAS_GetSPSR (void)
{
    return (uint32) * ( ( (uint32 *) &cur_mode_reg) + 16);
}

/*****************************************************************************/
//  Description:    The function is used to get the value of current mode CPSR
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 _TXAS_GetCPSR (void)
{
    return (uint32) * ( ( (uint32 *) &cur_mode_reg) + 17);
}

/*****************************************************************************/
//  Description:    The function is used to parse file name
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL const char *_TXAS_ParseFileName (const char *file)
{
    const char *ptr = file;
    static const char null_string[] = "\0";

    if (NULL == file)
    {
        return null_string;
    }

    // to tail.
    while (*ptr)
    {
        ptr++;
    }

    ptr--;

    while ( (ptr >= file) && ('\\' != *ptr) && ('/' != *ptr))
    {
        ptr--;
    }

    return (ptr >= file) ? ptr + 1 : file;
}

/*****************************************************************************/
//  Description:    Store a new assert info to flash..
//  Global resource dependence:
//  Author:         Johnson.sun
//  Note:
/*****************************************************************************/
LOCAL int _TXAS_WriteAssertInfoToNV (const uint8 *ptr, int len)
{
    int       ret = 0;
    uint16    id = 0;

    if (len > 0)
    {
        id = NV_ASSERT_BASE + (curNVID % NV_ASSERT_NUM);
        ret = (0 == EFS_NvitemWrite (id, (uint16) len, (uint8 *) ptr, 1))
              ? 1 : 0;
    }

    return ret;
}

/*****************************************************************************/
//  Description:    Search current assert Id number from 10 nv Id.
//  Global resource dependence:
//  Author:         Johnson.sun
//  Note:
/*****************************************************************************/
LOCAL uint16 _TXAS_SearchCurAssertID (void)
{
    int16  max, min, curId;
    uint16 temp;
    uint16 len;
    uint8  status;
    uint8   *buf;
    int    id;

    max = min = curId = -1;
    buf = (uint8 *) SCI_ALLOC_APP (MAX_ASSERT_BUF+2);
    SCI_ASSERT (SCI_NULL != buf);/*assert verified*/

    for (id = NV_ASSERT_BASE; id < NV_ASSERT_END; id++)
    {
        len = NVITEM_GetLength (id);

        if (len > 0)
        {
            status = EFS_NvitemRead (id, len, (uint8 *) buf);

            if (NVERR_NONE == status)
            {
                temp = (uint16) (*buf) + ( (uint16) (* (buf+1)) << 8);

                if (temp > max)
                {
                    max = temp;
                }

                if (temp < min)
                {
                    min = temp;
                }
            }
        }
    }

    if (max < 0)
    {
        curId = 0;
    }

    if ( (max - min) >= NV_ASSERT_NUM) //overflow and back again.
    {
        curId = min + 1;
    }
    else
    {
        curId = max + 1;
    }

    SCI_FREE (buf);
    return (uint16) curId;
}

/*****************************************************************************/
//  Description:    The function is used to copy print-name
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintNameCopy (char *src_name, char *dest_name)
{
    if (strlen (src_name) >= (PRINT_NAME_LEN_MAX-1))
    {
        SCI_MEMCPY (dest_name, src_name, PRINT_NAME_LEN_MAX-1);
    }
    else
    {
        uint16  i;
        strcpy (dest_name, src_name);

        for (i = strlen (dest_name); i < (PRINT_NAME_LEN_MAX-1); i++)
        {
            dest_name[i] = ' ';
        }
    }

    dest_name[PRINT_NAME_LEN_MAX-1] = 0;
}

/*****************************************************************************/
//  Description:    The function is used to print string by the special print path
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
/*lint -e628 for function "va_start (args, x_format);" */
LOCAL void _TXAS_PrintString (PRINT_PATH_E print_path, char *x_format, ...)
{
    char      *format_str = str_buf;
    va_list    args;//lint !e522 -e522
    int        nBuf;
    uint32 print_len = 0;
    va_start (args, x_format);
    nBuf = vsprintf (format_str, x_format, args);
    /* Was the expanded string too long? */
    va_end (args);

    if (nBuf >0)
    {
        print_len = (nBuf  <  PRINT_STR_LEN_MAX) ? nBuf : PRINT_STR_LEN_MAX ;
    }
    else
    {
        return;
    }

    if (PRINT_PATH_NORMAL == print_path)
    {
        OS_DumpPrint (format_str, (uint32) print_len);
    }
    else if (PRINT_PATH_DIAG == print_path)
    {
#ifdef SHELL_DEBUG_SUPPORT
        DIAG_SendInfo (SHELL_DATA_TEXT, (uint8 *) format_str, (uint32) print_len);
#endif
    }
    else
    {
        // do nothing
    }
}
/*****************************************************************************/
//  Description:    The function is used to print the version information
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintVersionInfo (void)
{
    const char *info_ptr = NULL;
    uint32 t = 0;
    uint32 num = VERSION_GetInfoNum();

    for (t = 0; t < num; t++)
    {
        info_ptr = VERSION_GetInfo (t);

        if (info_ptr)
        {
            OS_DumpPrint (info_ptr, strlen (info_ptr));/*lint !e605*/
        }
    }

    OS_DumpFlush();
}

/*****************************************************************************/
//  Description:    The function is used to print fat control block information
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintFatCtlBlkInfo (void)
{
    ASSERT_PRINT ("\nFat system control block infomation:");

    FAT_SystemAssert ( (uint8 *) str_buf,OS_DumpPrint);
}

/*****************************************************************************/
//  Description:    The _TXAS_DumpCurThreadInfo function is used print current
//                  thread information, such as thread ID, name, stack, queue.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_DumpCurThreadInfo (void)
{
    TX_THREAD       *thread_ptr     = SCI_NULL;
    TX_QUEUE        *queue_ptr      = SCI_NULL;
    SCI_THREAD_T    *my_thread_ptr  = SCI_NULL;


    if ( (IRQ_E == assert_type) && (SCI_FALSE == b_is_watchdog))
    {
        ASSERT_PRINT ("\n!!! It is in IRQ !!!");
        ASSERT_PRINT ("\nBefore enter IRQ, PC = 0x%08lx:\n", _tx_pc_val_before_irq);
    }
    else if ( (EXCEPTION_E == assert_type) && ( (_TXAS_GetSPSR() & 0xf) == MCU_IRQ_MODE))
    {
        ASSERT_PRINT ("\n!!! It is in IRQ before exception!!! PC = 0x%08lx", cur_mode_reg.r14);
        ASSERT_PRINT ("\nBefore enter IRQ, PC = 0x%08lx:\n", _tx_pc_val_before_irq);
    }
    else if ( (FIQ_E == assert_type) && ( (_TXAS_GetSPSR() & 0xf) == MCU_IRQ_MODE))
    {
        ASSERT_PRINT ("\n!!! It is in IRQ before FIQ!!! PC = 0x%08lx", _tx_pc_val_before_fiq);
        ASSERT_PRINT ("\nBefore enter IRQ, PC = 0x%08lx:\n", _tx_pc_val_before_irq);

    }
    else
    {
        ASSERT_PRINT ("\nCurrent thread info:");

        thread_ptr = tx_thread_identify();

        if (SCI_NULL == thread_ptr)
        {
            ASSERT_PRINT ("\n!!! None thread !!! \n");
        }
        else
        {
            // User thread.
            my_thread_ptr = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

            if ( (my_thread_ptr->block_type == SCI_STATIC_THREAD)
                    || (my_thread_ptr->block_type == SCI_DYNAMIC_THREAD)
                    || (my_thread_ptr->block_type == KAL_DYNAMIC_THREAD)
                    || (my_thread_ptr->block_type == KAL_STATIC_THREAD))
            {
                // Static or Dynamic Thread.
                queue_ptr = & (my_thread_ptr->queue);
                ASSERT_PRINT (
                    "\r\n\
                    ID:               0x%lx\r\n\
                    Name:             %s\r\n\
                    Tcb_Addr:         0x%08lx\r\n\
                    Last_Err:         0x%lx\r\n\
                    Stack_Start:      0x%08lx\r\n\
                    Stack_End:        0x%08lx\r\n\
                    Queue_Name:       %s\r\n\
                    Queue_Total:      %ld\r\n\
                    Queue_Used:       %ld\r\n\
                    Queue_Available:  %ld\r\n\
                    Queue_Start:      0x%08lx\r\n\
                    Queue_End:        0x%08lx\r\n",
                    my_thread_ptr->thread_id,
                    thread_ptr->tx_thread_name,
                    (uint32) my_thread_ptr,
                    (uint32) my_thread_ptr->last_error,
                    (uint32) thread_ptr->tx_stack_start,
                    (uint32) thread_ptr->tx_stack_end,
                    queue_ptr->tx_queue_name,
                    (uint32) queue_ptr->tx_queue_capacity,
                    (uint32) queue_ptr->tx_queue_enqueued,
                    (uint32) queue_ptr->tx_queue_available_storage,
                    (uint32) queue_ptr->tx_queue_start,
                    (uint32) queue_ptr->tx_queue_end
                );
            }
            else
            {
                // Other thread.
                ASSERT_PRINT (
                    "\
                    Name:             %s\r\n\
                    Stack_Ptr:        0x%08lx\r\n\
                    Stack_Start:      0x%08lx\r\n\
                    Stack_End:        0x%08lx\r\n",
                    thread_ptr->tx_thread_name,
                    (uint32) thread_ptr->tx_stack_ptr,
                    (uint32) thread_ptr->tx_stack_start,
                    (uint32) thread_ptr->tx_stack_end
                );
            }
        }
    }
}

/*****************************************************************************/
//  Description:    The ReadMemory function is used read memory.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_ReadMemoryTofile (void)
{

    uint32  addr;
    uint32  size;
    uint32  status;

    ASSERT_PRINT ("\nDump memory to a file:\n");

    for (;;)
    {
        ASSERT_PRINT ("Input address:\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&addr, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            addr &= (~0x3);
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted address is invalid !!!\n");
            continue;
        }
    }

    for (;;)
    {
        ASSERT_PRINT ("Input size:\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&size, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            size = (size & (~0x3)) + 0x4;
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted size is invalid !!!\n");
            continue;
        }
    }

    // Check if address and size are valid.
    status = _TXAS_CheckMemAccessParam (addr, size);

    if (SCI_SUCCESS == status)
    {
        OS_DumpDefiniteMemory ( (uint32) addr, size);
    }
    else
    {
        // Address and size are invalid.
        ASSERT_PRINT ("\n\t !!! Address or size is invalid !!!!\n");
        OS_DumpFlush();
    }
}

/*****************************************************************************/
//  Description:    The PrintUserRegInfo function is used to print values of
//                  user registers.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintUserRegInfo()
{
    uint32  *reg_ptr;

    if (NORMAL_E == assert_type)
    {
        ASSERT_PRINT ("\nCurrent status is SVC, below is the registers before assert:\r\n");
    }
    else if (IRQ_E == assert_type)
    {
        ASSERT_PRINT ("\nCurrent status is IRQ, below is the registers before assert:\r\n");
    }
    else if (FIQ_E == assert_type)
    {
        ASSERT_PRINT ("\nCurrent status is FIQ, below is the registers before assert:\r\n");
    }
    else
    {
        ASSERT_PRINT ("\nCurrent status is exception, below is the registers before Exception:");
    }

    // First write current mode registers
    reg_ptr = (uint32 *) (&cur_mode_reg);

    ASSERT_PRINT ("Current mode:\r\n");

    ASSERT_PRINT (
        "\n\
        R0  = 0x%08lx    R1   = 0x%08lx\r\n\
        R2  = 0x%08lx    R3   = 0x%08lx\r\n\
        R4  = 0x%08lx    R5   = 0x%08lx\r\n\
        R6  = 0x%08lx    R7   = 0x%08lx\r\n\
        R8  = 0x%08lx    R9   = 0x%08lx\r\n\
        R10 = 0x%08lx    R11  = 0x%08lx\r\n\
        R12 = 0x%08lx    R13  = 0x%08lx\r\n\
        R14 = 0x%08lx    PC   = 0x%08lx\r\n\
        SPSR= 0x%08lx    CPSR = 0x%08lx\r\n",
        *reg_ptr,         * (reg_ptr + 1),
        * (reg_ptr + 2),   * (reg_ptr + 3),
        * (reg_ptr + 4),   * (reg_ptr + 5),
        * (reg_ptr + 6),   * (reg_ptr + 7),
        * (reg_ptr + 8),   * (reg_ptr + 9),
        * (reg_ptr + 10),  * (reg_ptr + 11),
        * (reg_ptr + 12),  * (reg_ptr + 13),
        * (reg_ptr + 14),  * (reg_ptr + 15),
        * (reg_ptr + 16),   * (reg_ptr + 17));

    // Second write SVC mode registers
    reg_ptr = (uint32 *) (&svc_mode_reg);
    ASSERT_PRINT ("SVC mode:\r\n");

    ASSERT_PRINT (
        "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
        *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));

    // Third write IRQ mode registers
    reg_ptr = (uint32 *) (&irq_mode_reg);
    ASSERT_PRINT ("IRQ mode:\r\n");

    ASSERT_PRINT (
        "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
        *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));

    // Fourth write Abort mode registers
    reg_ptr = (uint32 *) (&abort_mode_reg);
    ASSERT_PRINT ("Abort mode:\r\n");

    ASSERT_PRINT (
        "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
        *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));

    // Fifth write Undefined mode registers
    reg_ptr = (uint32 *) (&undefine_mode_reg);
    ASSERT_PRINT ("Undefined mode:\r\n");

    ASSERT_PRINT (
        "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
        *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));

    // Sixth write FIQ mode registers
    reg_ptr = (uint32 *) (&fiq_mode_reg);
    ASSERT_PRINT ("FIQ mode:\r\n");

    ASSERT_PRINT (
        "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
        *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));
}

/*****************************************************************************/
//  Description:    The PrintPAssertInfo function is used print assert
//                  information to SIO and LCD.
//                  File, Line, Assert expression.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintPAssertInfo (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
    void    *assert_info  // the pointer to assert info
)
{
    uint32  i;
    uint32  name_len;
    char    file_name[SCI_MAX_NAME_SIZE + 1];

    ASSERT_PRINT ("\nCurrent Version: \n        %s\n", COMMON_GetProjectVersionInfo());

    // Print assert information to SIO.
    if (b_is_watchdog)
    {
        ASSERT_PRINT ("\nPossible error: Loop forever at 0x%08lx\n%s\n", _tx_pc_val_before_irq, exp);
    }
    else if (EXCEPTION_E == assert_type)
    {
        if (ABORT_EXCEPTION == s_exception_type)
        {
            ASSERT_PRINT ("\nException at 0x%08lx\nASSERT(%s)\n\nAbort fault(DFSR:0x%08lx): %s !\nFault address :0x%08lx\n", \
                          cur_mode_reg.r14,
                          exp,
                          abort_DFSR_reg,
                          abort_fault_status_debug_list[ (abort_DFSR_reg&0xf) ],
                          abort_FAR_reg);
        }
        else
        {
            ASSERT_PRINT ("\nException at 0x%08lx\nASSERT(%s)\n", cur_mode_reg.r14, exp);
        }
    }
    else
    {
        if (PNULL != assert_info)
        {
            ASSERT_PRINT ("\nFile:  %s\nLine:  %ld\nPASSERT(%s)\n", file, line, exp);
        }
        else
        {
            ASSERT_PRINT ("\nFile:  %s\nLine:  %ld\nASSERT(%s)\n", file, line, exp);
        }

    }

    // Delete directory name string of the file.
    sprintf (str_buf, "%s", (char *) file);

    for (i = 0, name_len = 0; str_buf[i] != 0; i++)
    {
        if ( (str_buf[i] == '\\') || (str_buf[i] == '/'))
        {
            name_len = 0;
        }
        else
        {
            name_len++;
        }
    }

    strcpy (file_name, str_buf + strlen (str_buf) - name_len);

    // Print assert information to LCD.
    if (b_is_watchdog)
    {
        sprintf (str_buf, "Loop for ever at 0x%08lx%s", _tx_pc_val_before_irq, (char *) exp);
    }
    else if (EXCEPTION_E == assert_type)
    {
        // Here R14 should be the return address!
        if (ABORT_EXCEPTION == s_exception_type)
        {
            sprintf (str_buf, "\nException at 0x%08lx\nASSERT(%s)\n\nAbort fault(DFSR:0x%08lx): %s !\nFault address :0x%08lx\n", \
                     cur_mode_reg.r14,
                     (char *) exp,
                     abort_DFSR_reg,
                     (char *) abort_fault_status_debug_list[ (abort_DFSR_reg&0xf) ],
                     abort_FAR_reg);
        }
        else
        {
            sprintf (str_buf, "\nException at 0x%08lx\nASSERT(%s)\n", cur_mode_reg.r14, (char *) exp);
        }
    }
    else
    {
        if (PNULL != assert_info)
        {
            sprintf (str_buf, "File:%s Line:%ld PASSERT(%s)", file_name, line, (char *) exp);
        }
        else
        {
            sprintf (str_buf, "File:%s Line:%ld ASSERT(%s)", file_name, line, (char *) exp);
        }
    }

    if (PNULL != assert_info)
    {
        strcpy (str_buf + strlen (str_buf), assert_info);
    }

#if defined(PLATFORM_UWS6121E)
    //wake up lcd
    LCD_EnterSleep(LCD_ID_0, SCI_FALSE);
    //set lcd back light
    GPIO_SetLcdBackLight(SCI_TRUE);
       //clear lcd
    LCD_Clear(LCD_ID_0, 0xFFE0);  //set to yellow
#endif

  LCD_DisplayText (0, 0, str_buf, strlen (str_buf));
}
/*****************************************************************************/
//  Description:    The PrintThreadInfo function is used print all threads
//                  information, such as thread ID, name, queue.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintThreadInfo (PRINT_PATH_E print_path)
{
    uint32          pc_val;
    uint32          status;
    void            *m_reg_ptr;
    char            task_name[PRINT_NAME_LEN_MAX];
    TX_THREAD       *thread_ptr     = SCI_NULL;
    TX_QUEUE        *queue_ptr      = SCI_NULL;
    SCI_THREAD_T    *my_thread_ptr  = SCI_NULL;
    uint32 error_cnt = 0;

    // Print static thread info.
    _TXAS_PrintString (print_path, "\nTasks info:");
    _TXAS_PrintString (print_path, "\n\tTask_ID    Name            Tcb_Addr\
        Current_PC  Queue_All   Queue_Avail");

    my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_TRUE, PNULL);
    thread_ptr      = & (my_thread_ptr->thread);
    queue_ptr       = & (my_thread_ptr->queue);

    while (SCI_NULL != my_thread_ptr)
    {
        if ( (&_tx_timer_thread != &my_thread_ptr->thread) &&
                (SCI_STATIC_THREAD != my_thread_ptr->block_type) &&
                (SCI_DYNAMIC_THREAD != my_thread_ptr->block_type) &&
                (KAL_STATIC_THREAD != my_thread_ptr->block_type) &&
                (KAL_DYNAMIC_THREAD != my_thread_ptr->block_type))
        {
            _TXAS_PrintString (print_path, "\n-----Error Thread Block Type:\
                0x%-08lx 0x%-8lx  0x%08lx %d", my_thread_ptr->block_type,
                               my_thread_ptr->thread_id, (uint32) my_thread_ptr,
                               thread_ptr->tx_priority);

            if (error_cnt++ > 3)
            {
                break;
            }
            else
            {
                my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_FALSE, (SCI_THREAD_PTR) my_thread_ptr);
                continue;
            }
        }

        thread_ptr      = & (my_thread_ptr->thread);
        queue_ptr       = & (my_thread_ptr->queue);

        // Get thread name.
        _TXAS_PrintNameCopy (thread_ptr->tx_thread_name, task_name);


        // Get PC value.
        if (thread_ptr == tx_thread_identify())
        {
            _TXAS_PrintString (print_path, "\n********0x%-8lx %s 0x%08lx %d", my_thread_ptr->thread_id, task_name, (uint32) my_thread_ptr, thread_ptr->tx_priority);
        }
        else
        {
            // Get PC value of other thread.
            m_reg_ptr = (void *) thread_ptr->tx_stack_ptr;
            status = * ( (uint32 *) m_reg_ptr);

            if (status == 0)
            {
                pc_val = ( (CONTEXT_MINIMAL_SAVE_REG_T *) m_reg_ptr)->pc - 7;
            }
            else    // status == 1
            {
                pc_val = ( (CONTEXT_FULL_SAVE_REG_T *) m_reg_ptr)->pc;
            }

            _TXAS_PrintString (print_path, "\n\t0x%-8lx %s 0x%08lx  0x%08lx  %-10ld  %-10ld   %s   %d",
                               my_thread_ptr->thread_id,
                               task_name,
                               (uint32) my_thread_ptr,
                               pc_val,
                               (uint32) queue_ptr->tx_queue_capacity,
                               (uint32) queue_ptr->tx_queue_available_storage,
                               &task_supend_type[thread_ptr->tx_state][0],
                               thread_ptr->tx_priority
                              );
        }

        //OS_DumpPrint (str_buf, strlen (str_buf));

        my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_FALSE, (SCI_THREAD_PTR) my_thread_ptr);

    }

}

/*****************************************************************************/
//  Description:    The PrintStackInfo function is used print all threads
//                  stack information.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintStackInfo (PRINT_PATH_E print_path)
{
    uint32          max_used_size;
    uint32          cur_stack;
    uint32          *temp_ptr;
    char            task_name[PRINT_NAME_LEN_MAX];
    TX_THREAD       *thread_ptr     = SCI_NULL;
    SCI_THREAD_T    *my_thread_ptr  = SCI_NULL;
    uint32 error_cnt = 0;

    // Print static thread info.
    _TXAS_PrintString (print_path, "\nStack info:");
    _TXAS_PrintString (print_path, "\n\tTask_ID    Name            TotalSize  Max_Used   Available  Cur_Ptr    Start      End        ");


    my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_TRUE, PNULL);

    while (SCI_NULL != my_thread_ptr)
    {
        if ( (&_tx_timer_thread != &my_thread_ptr->thread) &&
                (SCI_STATIC_THREAD != my_thread_ptr->block_type) &&
                (SCI_DYNAMIC_THREAD != my_thread_ptr->block_type) &&
                (KAL_STATIC_THREAD != my_thread_ptr->block_type) &&
                (KAL_DYNAMIC_THREAD != my_thread_ptr->block_type))
        {
            _TXAS_PrintString (print_path, "\n-----Error Thread Block Type:0x%-08lx 0x%-8lx ", my_thread_ptr->block_type, (uint32) my_thread_ptr);

            if (error_cnt++ > 3)
            {
                break;
            }
            else
            {
                my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_FALSE, (SCI_THREAD_PTR) my_thread_ptr);
                continue;
            }
        }

        thread_ptr      = & (my_thread_ptr->thread);

        // Get thread name.
        _TXAS_PrintNameCopy (thread_ptr->tx_thread_name, task_name);

        // Get current stack pointer.
        if (thread_ptr != tx_thread_identify())
        {
            cur_stack = (uint32) thread_ptr->tx_stack_ptr;
            // Get max used size.
            max_used_size = (uint32) thread_ptr->tx_stack_size;

            for (temp_ptr = (uint32 *) thread_ptr->tx_stack_start;
                    temp_ptr <= (uint32 *) cur_stack;
                    temp_ptr++)
            {
                if (*temp_ptr == STACK_INIT_VAL)    //MEM_INIT_VALUE = 0x5A
                {
                    max_used_size -= 4;
                }
                else
                {
                    break;
                }
            }

            _TXAS_PrintString (print_path, "\n\t0x%-8lx %s %-10ld %-10ld %-10ld 0x%08lx 0x%08lx 0x%08lx",
                               my_thread_ptr->thread_id,
                               task_name,
                               (uint32) thread_ptr->tx_stack_size,
                               max_used_size,
                               (uint32) cur_stack - (uint32) thread_ptr->tx_stack_start,
                               (uint32) cur_stack,
                               (uint32) thread_ptr->tx_stack_start,
                               (uint32) thread_ptr->tx_stack_end);
        }

        my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_FALSE, (SCI_THREAD_PTR) my_thread_ptr);
    }

}

/*****************************************************************************/
//  Description:    The PrintCurThreadCallbackFuncList function is used print
//                  Callback information of the system.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintCurThreadCallbackFuncList (void)
{
    TX_THREAD   *thread_ptr;
    SCI_THREAD_T    *thread_block   = SCI_NULL;

    if (IRQ_E == assert_type)
    {
        // Cuerrent case is an IRQ
        if (b_is_watchdog)
        {
            ASSERT_PRINT ("\nWatch Dog Timer Expired.\nPossible error: Loop for ever at 0x%08lx", _tx_pc_val_before_irq);
        }
        else
        {
            // Print IRQ handle callback list.
            ASSERT_PRINT ("\nIRQ handle callback list:\n");

            last_sp = cur_mode_reg.r13;
            last_lr = cur_mode_reg.PC;
            _TXAS_TraceCallbackList();

            ASSERT_PRINT ("\nCallback of thread interrupted by IRQ:");
        }

        s_is_current_thread_saved = SCI_TRUE;
    }
    else if (EXCEPTION_E == assert_type)
    {
        // check if is an IRQ
        if ( (_TXAS_GetSPSR() & 0xf) == MCU_IRQ_MODE)
        {
            // Print IRQ handle callback list.
            ASSERT_PRINT ("\nIRQ handle callback list:\n");

            last_sp = irq_mode_reg.r13;
            last_lr = cur_mode_reg.r14;
            _TXAS_TraceCallbackList();

            ASSERT_PRINT ("\nCallback of thread interrupted by IRQ:");

            s_is_current_thread_saved = SCI_TRUE;
        }
        else // Normal thread
        {
            // Print IRQ handle callback list.
            ASSERT_PRINT ("\nCurrent thread callback list:\n");

            last_sp = svc_mode_reg.r13;
            last_lr = cur_mode_reg.r14;
            _TXAS_TraceCallbackList();
        }
    }
    else if (NORMAL_E == assert_type)
    {
        // Get current thread info.
        thread_ptr = tx_thread_identify();

        if (SCI_NULL != thread_ptr)
        {
            last_sp = cur_mode_reg.r13;
#ifdef ARM_ARCH_V7
            last_lr = cur_mode_reg.r14;
#else
#if __ARMCC_VERSION > 220000  /*lint !e553*/
            last_lr = cur_mode_reg.r14 - 1; //rvds compiler r14 will be odd, must be change to even.
#else
            last_lr = cur_mode_reg.PC;
#endif
#endif

            // Print current thread name.
            // And print the ID if it is user thread.
            thread_block = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

            if ( (thread_block->block_type == SCI_STATIC_THREAD)
                    || (thread_block->block_type == SCI_DYNAMIC_THREAD)
                    || (thread_block->block_type == KAL_DYNAMIC_THREAD)
                    || (thread_block->block_type == KAL_DYNAMIC_THREAD))
            {
                ASSERT_PRINT ("\nCurrent Task %ld (%s):\n", thread_block->thread_id, thread_ptr->tx_thread_name);
            }
            else
            {
                ASSERT_PRINT ("\nCurrent Task %s:\n", thread_ptr->tx_thread_name);
            }

            _TXAS_TraceCallbackList();
        }
    }
    else if (FIQ_E == assert_type)
    {
        if ( (_TXAS_GetSPSR() & 0xf) == MCU_IRQ_MODE)
        {
            // Print IRQ handle callback list.
            ASSERT_PRINT ("\nIRQ handle callback list:\n");

            last_sp = irq_mode_reg.r13;
            last_lr = _tx_pc_val_before_fiq;
            _TXAS_TraceCallbackList();

            ASSERT_PRINT ("\nCallback of thread interrupted by IRQ:");

            s_is_current_thread_saved = SCI_TRUE;
        }
        else // Normal thread
        {
            // Print IRQ handle callback list.
            ASSERT_PRINT ("\nCurrent thread callback list:\n");

            last_sp = svc_mode_reg.r13;
            last_lr = _tx_pc_val_before_fiq;
            _TXAS_TraceCallbackList();
        }
    }
}

/*****************************************************************************/
//  Description:    The PrintCallbackFuncList function is used print Callback
//                  information of the system.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintCallbackFuncList (void)
{
    SCI_THREAD_T    *my_thread_ptr  = SCI_NULL;

    ASSERT_PRINT ("\nAll Task Callback Function List:\n");

    // Print current thread callback.
    _TXAS_PrintCurThreadCallbackFuncList();

    /*
     0   SCI_MAX_STATIC_THREAD_ID         kal_max_num_tasks_g
    |---------------------|----------------------------|
    |    Static thread    |  Dynamic thread            |
    |---------------------|----------------------------|
    */

    my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_TRUE, PNULL);

    while (SCI_NULL != my_thread_ptr)
    {
        _TXAS_GetStackBacktrace (my_thread_ptr);

        my_thread_ptr = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_FALSE, (SCI_THREAD_PTR) my_thread_ptr);
    }

}

/*****************************************************************************/
//  Description:    The function is used to print the time list of all task
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintTaskTimeList (void)
{
#ifdef DS_LOADING_TRACK
    int32       id;
    TX_THREAD   *thread_ptr;
    SCI_THREAD_T    *thread_block   = SCI_NULL;

    ASSERT_PRINT ("\nAll Task time consumption List:\n");

    thread_block = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_TRUE, PNULL);

    while (SCI_NULL != thread_block)
    {
        excute_time += thread_block->thread.time;
        thread_block = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_FALSE, (SCI_THREAD_PTR) thread_block);
    }

    excute_time += uart_time;
    excute_time += idle_time;
    excute_time_div = excute_time/1000;


    thread_block = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_TRUE, PNULL);

    while (SCI_NULL != thread_block)
    {
        ASSERT_PRINT ("\nTask %s:\n", thread_block->thread.tx_thread_name);
        ASSERT_PRINT ("\ntime : %d percent: %d\n", thread_block->thread.time, ( (thread_block->thread.time) /excute_time_div));

        thread_block = (SCI_THREAD_T *) SCI_GetNextThreadBlock (SCI_FALSE, (SCI_THREAD_PTR) thread_block);
    }

    ASSERT_PRINT ("\nTask uart: %d percent: %d\n", uart_time, (uart_time) /excute_time_div);
    ASSERT_PRINT ("\nTask idle: %d percent: %d\n", idle_time, (idle_time) /excute_time_div);
    ASSERT_PRINT ("\nTask total time: %ud\n", excute_time);
#endif
}

/*****************************************************************************/
//  Description:    The PrintEventInfo function is used print all events
//                  information, such as name, current flags and suspended
//                  thread list.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintEventInfo (void)
{
    TX_EVENT_FLAGS_GROUP        *event_ptr      = SCI_NULL;
    TX_EVENT_FLAGS_GROUP        *event_ptr_previous     = SCI_NULL;
    TX_THREAD_PTR               suspend_list    = SCI_NULL;
    BOOLEAN is_queue_exact = SCI_TRUE;

    ASSERT_PRINT ("Event Information:\n");

    ASSERT_PRINT ("%-20s%-12s%-15s%-20s", "Name", "CurrentFlag", "TotalSuspended", "SuspendList");

    if (SCI_NULL != _tx_event_flags_created_ptr)
    {
        event_ptr = _tx_event_flags_created_ptr;
        event_ptr_previous = _tx_event_flags_created_ptr;

        do
        {
            ASSERT_PRINT ("%-20s%-12x%-5ld", event_ptr->tx_event_flags_name,
                          event_ptr->tx_event_flags_current,
                          event_ptr->tx_event_flags_suspended_count);

            if ( (event_ptr->tx_event_flags_suspended_count > 0) &&
                    (SCI_NULL != event_ptr->tx_event_flags_suspension_list))
            {
                suspend_list = event_ptr->tx_event_flags_suspension_list;

                do
                {
                    ASSERT_PRINT ("%-20s%-12s%-15s%-20s\n", "", "", "", suspend_list->tx_thread_name);

                    suspend_list = suspend_list->tx_suspended_next;
              if((SCI_NULL != suspend_list) && (suspend_list == suspend_list->tx_suspended_next) && (suspend_list != event_ptr->tx_event_flags_suspension_list))
              {
                      ASSERT_PRINT ("%-20s%-15s%-15s%-20s\n", "", "", "", suspend_list->tx_thread_name);
                break;
              }

                }
                while ( (SCI_NULL != suspend_list) &&
                        (suspend_list != event_ptr->tx_event_flags_suspension_list));
            }

            event_ptr = event_ptr->tx_event_flags_created_next;

            if (SCI_NULL == event_ptr)
            {
                SCI_ASSERT (0);/*assert to do*/
            }
            else
            {

                //judge the queue is right begin.
                if (event_ptr_previous == event_ptr->tx_event_flags_created_previous)
                {
                    event_ptr_previous = event_ptr;
                    is_queue_exact = SCI_TRUE;
                }
                else
                {
                    is_queue_exact = SCI_FALSE;
                }
            }

            //judge the queue is right end.
        }
        while ( (SCI_NULL != event_ptr) &&
                (event_ptr != _tx_event_flags_created_ptr) && (is_queue_exact));
    }
    else
    {
        ASSERT_PRINT ("%s", "***No Event***");
    }
}

/*****************************************************************************/
//  Description:    The PrintMutexInfo function is used print all mutexs
//                  information, such as name, owner, count and suspended
//                  thread list.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintMutexInfo (void)
{
    TX_MUTEX_PTR  mutex_ptr       = SCI_NULL;
    TX_MUTEX_PTR  mutex_ptr_previous = SCI_NULL;
    TX_THREAD_PTR suspend_list    = SCI_NULL;
    char          owner_name[20];
    BOOLEAN is_queue_exact = SCI_TRUE;

    ASSERT_PRINT ("Mutex Information:\n");

    ASSERT_PRINT ("%-30s%-20s%-15s%-15s%-20s", "Name", "Owner", "TotalSuspended", "OwnershipCount", "SuspendList");

    if (SCI_NULL != _tx_mutex_created_ptr)
    {
        mutex_ptr = _tx_mutex_created_ptr;
        mutex_ptr_previous = _tx_mutex_created_ptr;

        do
        {
            if (NULL == mutex_ptr->tx_mutex_owner)
            {
                sprintf (owner_name, "%-16s", "[No Owner]");
            }
            else
            {
                sprintf (owner_name, "%-16s", mutex_ptr->tx_mutex_owner->tx_thread_name);
            }

            ASSERT_PRINT ("%-30s%-20s%-15ld%-14ld", mutex_ptr->tx_mutex_name,
                          owner_name,
                          mutex_ptr->tx_mutex_suspended_count,
                          mutex_ptr->tx_mutex_ownership_count);

            if (mutex_ptr->tx_mutex_suspended_count > 0)
            {
                suspend_list = mutex_ptr->tx_mutex_suspension_list;

                while (NULL != suspend_list)
                {
                    ASSERT_PRINT ("%-30s%-20s%-15s%-15s%-20s\n", "", "", "", "", suspend_list->tx_thread_name);

                    suspend_list = suspend_list->tx_suspended_next;

                    if (suspend_list == mutex_ptr->tx_mutex_suspension_list)
                    {
                        break;
                    }
                }
            }


            mutex_ptr = mutex_ptr->tx_mutex_created_next;

            if (NULL == mutex_ptr)
            {
                SCI_ASSERT (0); /*assert to do*/
            }
            else
            {
                //judge the queue is right begin.
                if (mutex_ptr_previous == mutex_ptr->tx_mutex_created_previous)
                {
                    mutex_ptr_previous = mutex_ptr;
                    is_queue_exact = SCI_TRUE;
                }
                else
                {
                    is_queue_exact = SCI_FALSE;
                }
            }

            //judge the queue is right end.
        }
        while ( (SCI_NULL != mutex_ptr) && (mutex_ptr != _tx_mutex_created_ptr) && (is_queue_exact));
    }
    else
    {
        ASSERT_PRINT ("***No Mutex***");
    }
}

/*****************************************************************************/
//  Description:    The PrintTimerInfo function is used print all threads
//                  information, such as thread ID, name, queue.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintTimerInfo (void)
{
    uint32 current_ticks = SCI_GetTickCount();
    uint8 timer_name[32];
    TX_TIMER_PTR timer_list_ptr = SCI_NULL;
    TX_TIMER_PTR timer_list_ptr_previous = SCI_NULL;
    TX_INTERNAL_TIMER  *timer_internal_ptr = SCI_NULL;
    TX_INTERNAL_TIMER  *timer_internal_ptr_previous = SCI_NULL;
    BOOLEAN is_queue_exact = SCI_TRUE;

    if ( (SCI_NULL != _tx_timer_created_ptr) || (SCI_NULL != _tx_timer_list_head))
    {

        if (SCI_NULL != _tx_timer_created_ptr)
        {
            ASSERT_PRINT ("\nALL created Threads' timer infomation: \n");
            ASSERT_PRINT (" cur_ticks\t name\t\t\t\t rem_ticks\t re_init_ticks\t Cur_Ptr\t Next_Ptr\t Pre_Ptr\t ");

            timer_list_ptr = _tx_timer_created_ptr;
            timer_list_ptr_previous = _tx_timer_created_ptr;

            do
            {
                timer_list_ptr = timer_list_ptr->tx_timer_created_next;

                if (timer_list_ptr-> tx_timer_id !=  0x4154494DUL)
                {
                    ASSERT_PRINT ("\nTimer list is corrupted.Timer pointer: 0x%x\n",timer_list_ptr);
                    return;
                }

                // Get timer name.
                if (strlen (timer_list_ptr->tx_timer_name) >= 31)
                {
                    SCI_MEMCPY (timer_name,timer_list_ptr->tx_timer_name, 31);
                }
                else
                {
                    uint16  i;
                    strcpy ( (char *) timer_name, timer_list_ptr->tx_timer_name);

                    for (i = strlen ( (char *) timer_name); i < 31; i++)
                    {
                        timer_name[i] = ' ';
                    }
                }

                timer_name[31] = '\0';

                ASSERT_PRINT ("%ld\t %s %ld\t\t %ld\t\t 0x%x\t 0x%x\t 0x%x\t",
                              current_ticks,
                              timer_name,
                              timer_list_ptr->tx_timer_internal.tx_remaining_ticks,
                              timer_list_ptr->tx_timer_internal.tx_re_initialize_ticks,
                              timer_list_ptr,
                              timer_list_ptr-> tx_timer_created_next,
                              timer_list_ptr-> tx_timer_created_previous);

                //judge the queue is right begin.
                if (timer_list_ptr_previous == timer_list_ptr-> tx_timer_created_previous)
                {
                    timer_list_ptr_previous = timer_list_ptr;
                    is_queue_exact = SCI_TRUE;
                }
                else
                {
                    is_queue_exact = SCI_FALSE;
                }

                //judge the queue is right end.
            }
            while ( (_tx_timer_created_ptr != timer_list_ptr->tx_timer_created_next) && (timer_list_ptr->tx_timer_created_next) && (is_queue_exact));
        }

        if (SCI_NULL != _tx_timer_list_head)
        {
            ASSERT_PRINT ("\nActive Threads' timer infomation:\n");
            ASSERT_PRINT (" cur_ticks\t name\t\t\t\t rem_ticks\t re_init_ticks\tCur_Int_Ptr\t Next_Int_Ptr\t Pre_Int_Ptr\t");

            timer_internal_ptr = _tx_timer_list_head;
            timer_internal_ptr_previous = _tx_timer_list_head;

            do
            {

                timer_internal_ptr = timer_internal_ptr->tx_active_next;
                timer_list_ptr = (TX_TIMER_PTR) ( (uint32) timer_internal_ptr - 8);

                if (timer_list_ptr-> tx_timer_id !=  0x4154494DUL)
                {
                    //ASSERT_PRINT ("\nTimer list is corrupted.Timer pointer: 0x%x\n",timer_list_ptr);
                    //return;
                    ASSERT_PRINT ("%ld\t  thread_timer %ld\t\t %ld\t\t 0x%x\t 0x%x\t 0x%x\t",
                                  current_ticks,
                                  timer_internal_ptr->tx_remaining_ticks,
                                  timer_internal_ptr->tx_re_initialize_ticks,
                                  timer_internal_ptr,
                                  timer_internal_ptr->tx_active_next,
                                  timer_internal_ptr->tx_active_previous
                                 );
                }
                else
                {

                    // Get timer name.
                    if (strlen (timer_list_ptr->tx_timer_name) >= 31)
                    {
                        SCI_MEMCPY (timer_name,timer_list_ptr->tx_timer_name, 31);
                    }
                    else
                    {
                        uint16  i;
                        strcpy ( (char *) timer_name, timer_list_ptr->tx_timer_name);

                        for (i = strlen ( (char *) timer_name); i < 31; i++)
                        {
                            timer_name[i] = ' ';
                        }
                    }

                    timer_name[31] = '\0';

                    ASSERT_PRINT ("%ld\t %s %ld\t\t %ld\t\t 0x%x\t 0x%x\t 0x%x\t",
                                  current_ticks,
                                  timer_name,
                                  timer_list_ptr->tx_timer_internal.tx_remaining_ticks,
                                  timer_list_ptr->tx_timer_internal.tx_re_initialize_ticks,
                                  timer_internal_ptr,
                                  timer_internal_ptr->tx_active_next,
                                  timer_internal_ptr->tx_active_previous
                                 );
                }

                //judge the queue is right begin.
                if (timer_internal_ptr_previous == timer_internal_ptr->tx_active_previous)
                {
                    timer_internal_ptr_previous = timer_internal_ptr;
                    is_queue_exact = SCI_TRUE;
                }
                else
                {
                    is_queue_exact = SCI_FALSE;
                }

                //judge the queue is right end.
            }
            while ( (_tx_timer_list_head != timer_internal_ptr->tx_active_next) && (timer_internal_ptr->tx_active_next) && (is_queue_exact));

        }

    }
    else
    {
        ASSERT_PRINT ("\n No Timer list in current system!");
    }


}


/*****************************************************************************/
//  Description:    The PrintSemaphoreInfo function is used print all Semaphore
//                  information, such as thread name, count.
//  Global resource dependence:
//  Author:         Dong.yang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintSemaphoreInfo (void)
{
    TX_SEMAPHORE_PTR            semaphore_ptr   = SCI_NULL;
    TX_SEMAPHORE_PTR            semaphore_ptr_previous   = SCI_NULL;
    TX_THREAD_PTR               suspend_list    = SCI_NULL;
    BOOLEAN is_queue_exact = SCI_TRUE;

    ASSERT_PRINT ("\nSemaphore infomation:");
    ASSERT_PRINT ("Name                                Counter ");

    if (_tx_semaphore_created_ptr)
    {
        semaphore_ptr = _tx_semaphore_created_ptr;
        semaphore_ptr_previous = _tx_semaphore_created_ptr;

        do
        {
            if (semaphore_ptr->tx_semaphore_id != 0x53454D41UL) //TX_SEMAPHORE_ID
            {
                /* The event list has been destroied */
                ASSERT_PRINT ("\nCurrent semaphore list has been destroied !!!");
                break;
            }

            ASSERT_PRINT ("%-30s         %-10ld ",
                          semaphore_ptr->tx_semaphore_name,
                          semaphore_ptr->tx_semaphore_count
                         );

            if (semaphore_ptr->tx_semaphore_suspension_list)
            {
                /* Print all the tasks info suspended the list */

                suspend_list = semaphore_ptr->tx_semaphore_suspension_list;

                do
                {
                    //4
                    ASSERT_PRINT ("Suspend Task_Name :  %s ",
                                  suspend_list->tx_thread_name
                                 );
                    suspend_list = suspend_list->tx_suspended_next;

                }
                while (suspend_list
                        && (suspend_list != semaphore_ptr->tx_semaphore_suspension_list));

            }

            semaphore_ptr = semaphore_ptr->tx_semaphore_created_next;

            if (!semaphore_ptr)
            {
                /* check if the semaphore list has been destroied */
                ASSERT_PRINT ("\nCurrent semaphore list has been destroied !!!");
                break;
            }

            //judge the queue is right begin.
            if (semaphore_ptr_previous == semaphore_ptr->tx_semaphore_created_previous)
            {
                semaphore_ptr_previous = semaphore_ptr;
                is_queue_exact = SCI_TRUE;
            }
            else
            {
                is_queue_exact = SCI_FALSE;
            }

            //judge the queue is right end.

        }
        while ( (_tx_semaphore_created_ptr != semaphore_ptr) && (is_queue_exact));

    }    //3
    else
    {
        ASSERT_PRINT ("\nNo any semaphore in current system !");
    }

    ASSERT_PRINT ("\n========================================================\n");
}

/*****************************************************************************/
//  Description:    The ReadMemory function is used read memory.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_ReadMemory (void)
{
    uint32  i;
    uint32  addr;
    uint32  size;
    uint32  status;
    uint32  addr_format = 1;

    ASSERT_PRINT ("\nRead memory:\n");

    __readmem_label:

    for (;;)
    {
        ASSERT_PRINT ("Input address format(1:32      2/others:16):\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&addr_format, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted address format is invalid !!!\n");
            continue;
        }
    }

    for (;;)
    {
        ASSERT_PRINT ("Input address:\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&addr, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            addr &= (~0x3);
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted address is invalid !!!\n");
            continue;
        }
    }

    for (;;)
    {
        ASSERT_PRINT ("Input size(0< size < 1024):\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&size, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            size = (size & (~0x3)) + 0x4;
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted size is invalid !!!\n");
            continue;
        }
    }

    // Check if address and size are valid.
    status = _TXAS_CheckMemAccessParam (addr, size);

    if (SCI_SUCCESS == status)
    {
        // Print memory.
        ASSERT_PRINT ("Read Memory Start Address: 0x%08lx    Size: 0x%08lx\n", (uint32) addr, size);

        if (1== addr_format)
        {
            // Print dword by dword.

            uint32 *val_ptr = (uint32 *) addr;

            for (i = 0; i < size; i += 0x10)
            {
                ASSERT_PRINT ("\t0x%p    0x%08lx 0x%08lx 0x%08lx 0x%08lx\n",
                              val_ptr, *val_ptr, * (val_ptr + 1), * (val_ptr + 2), * (val_ptr + 3));
                val_ptr += 4;
            }
        }
        else
        {
            // Print word by word.

            uint16 *val_ptr = (uint16 *) addr;

            for (i = 0; i < size; i += 0x10)
            {
                ASSERT_PRINT ("\t0x%p    0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
                              val_ptr, *val_ptr, * (val_ptr + 1), * (val_ptr + 2), * (val_ptr + 3),
                              * (val_ptr + 4), * (val_ptr + 5), * (val_ptr + 6), * (val_ptr + 7));
                val_ptr += 8;
            }
        }

        OS_DumpFlush();
    }
    else
    {
        // Address and size are invalid.
        ASSERT_PRINT ("\n\t !!! Address or size is invalid !!!!\n");
        OS_DumpFlush();

        goto __readmem_label;
    }
}

/*****************************************************************************/
//  Description:    The WriteMemory function is used write memory.
//  Global resource dependence:
//  Author:         Fei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_WriteMemory (void)
{
    uint32  i;
    uint32  addr;
    uint32  size;
    uint32  value;
    uint32  status;
    uint32  addr_format = 1;

    ASSERT_PRINT ("\nWrite memory:\n");

    __writemem_label:

    for (;;)
    {
        ASSERT_PRINT ("Input address format(1:32      2/others:16):\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&addr_format, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted address format is invalid !!!\n");
            continue;
        }
    }

    for (;;)
    {
        ASSERT_PRINT ("Input address:\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&addr, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            addr &= (~0x3);
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted address is invalid !!!\n");
            continue;
        }
    }

    for (;;)
    {
        ASSERT_PRINT ("Input size(0< size < 1024):\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&size, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            size = (size & (~0x3)) + 0x4;
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted size is invalid !!!\n");
            continue;
        }
    }

    for (;;)
    {
        ASSERT_PRINT ("Input value:\n");
        OS_DumpFlush();

        _TXAS_GetALine (cmd_buf);
        status = _TXAS_GetNumFromStr (&value, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            //      value = (value & (~0x3)) + 0x4;
            break;
        }
        else
        {
            ASSERT_PRINT ("\t !!! Inputted value is invalid !!!\n");
            continue;
        }
    }

    // Check if address and size are valid.
    status = _TXAS_CheckMemAccessParam (addr, size);

    if (SCI_SUCCESS == status)
    {
        // Print memory.
        ASSERT_PRINT ("Write Memory Start Address: 0x%08lx    Size: 0x%08lx    Value: 0x%08lx\n", (uint32) addr, size, value);

        if (1== addr_format)
        {
            // Write dword by dword.

            uint32 *val_ptr = (uint32 *) addr;

            for (i = 0; i < (size/4); i++)
            {
                * (val_ptr + i) = value;
            }
        }
        else
        {
            // Write word by word.

            uint16 *val_ptr = (uint16 *) addr;

            for (i = 0; i < (size/2); i++)
            {
                * (val_ptr + i) = (uint16) value;
            }
        }

        ASSERT_PRINT ("Write Memory finished!\n");
        OS_DumpFlush();
    }
    else
    {
        // Address and size are invalid.
        ASSERT_PRINT ("\n\t !!! Address or size is invalid !!!!\n");
        OS_DumpFlush();

        goto __writemem_label;
    }
}



/*****************************************************************************/
//  Description:    The _TXAS_PrintHeapBlockInfo function is used print block memory
//                  information of the byte pool, such as block address,block
//                  size etc
//  Global resource dependence:
//  Author:         Dong.Yang
//  Note:
/*****************************************************************************/
LOCAL uint32 _TXAS_PrintHeapBlockInfo (TX_BYTE_POOL_PTR pool_ptr)
{
    char                       *current_ptr;    /* Current block pointer      */
    char                       *next_ptr;       /* Next block pointer         */
    uint32                      available_bytes;/* Calculate bytes available  */
    uint32                      block_index=0;

    /* print free segment in the pool */
    ASSERT_PRINT ("\nFree segment information in the pool:\n");
    ASSERT_PRINT ("%-10s %-20s %-20s ","index","Start_address","Size(Unit: byte)");

    /* Walk through the memory pool in search for free blocks.  */
    current_ptr =      pool_ptr->tx_byte_pool_search;

    do
    {

        /* Check to see if this block is free.  */
        if (* ( (uint32 *) (current_ptr + sizeof (char *))) == 0xFFFFEEEEUL) //TX_BYTE_BLOCK_FREE

        {

            /* Block is free, print the blocks info.  */

            /* Pickup the next block's pointer.  */
            next_ptr =  * ( (char **) current_ptr);

            /* Calculate the number of byte available in this block.  */
            available_bytes =  next_ptr - current_ptr;
            ASSERT_PRINT ("%-10ld 0x%-18x %-20ld ",
                          ++block_index,
                          current_ptr+sizeof (char *) +sizeof (uint32),
                          available_bytes);
        }
        else if ( (* ( (uint32 *) (current_ptr + sizeof (char *))) != 0xAAAAAAAAUL) //TX_BYTE_BLOCK_ALLOC
                  && (* ( (uint32 *) (current_ptr + sizeof (char *))) != (uint32) pool_ptr))
        {
            /* This block has been destroied. */

            ASSERT_PRINT ("Current memory list has been destroied! ");
            return SCI_ERROR;

        }

        /* Move to next block.  */
        current_ptr =      * ( (char **) current_ptr);

    }
    while (current_ptr != pool_ptr->tx_byte_pool_search);

    if (block_index == 0)
    {
        ASSERT_PRINT ("No free block in the pool!");
    }

    return SCI_SUCCESS;

}

/*****************************************************************************/
//  Description:    The _TXAS_HeapClean function is used print system heap
//                  memory information, such as mem address,mem size etc
//  Global resource dependence:
//  Author:         Dong.Yang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_HeapClean (TX_BYTE_POOL_PTR pool_ptr)
{
    char                       *current_ptr;    /* Current block pointer      */
    char                       *next_ptr;       /* Next block pointer         */

    /* Walk through the memory pool collect garbage */
    current_ptr =      pool_ptr->tx_byte_pool_search;

    do
    {

        /* Check to see if this block is free.  */
        if (* ( (uint32 *) (current_ptr + sizeof (char *))) == 0xFFFFEEEEUL) //TX_BYTE_BLOCK_FREE
        {
            /* Pickup the next block's pointer.  */
            next_ptr =  * ( (char **) current_ptr);

            while (* ( (uint32 *) (next_ptr + sizeof (char *))) == 0xFFFFEEEEUL) //TX_BYTE_BLOCK_FREE
            {
                * ( (char **) current_ptr) = * ( (char **) next_ptr);

                /* See if the search pointer is affected.  */
                if (pool_ptr->tx_byte_pool_search == next_ptr)
                {
                    pool_ptr->tx_byte_pool_search = current_ptr;
                }

                next_ptr =  * ( (char **) current_ptr);
            }
        }

        /* Move to next block.  */
        current_ptr =      * ( (char **) current_ptr);

    }
    while (current_ptr != pool_ptr->tx_byte_pool_search);

}
/*****************************************************************************/
//  Description:    The PrintHeapInfo function is used print system heap
//                  memory information, such as mem address,mem size etc
//  Global resource dependence:
//  Author:         Dong.Yang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintHeapInfo (void)
{
#ifdef PRODUCT_DM
    extern SCI_BYTE_POOL_T      byte_static_heap;
    TX_THREAD_PTR               suspend_list    = SCI_NULL;
    TX_BYTE_POOL_PTR            pool_ptr   =    &byte_static_heap;
    //uint32                      available_bytes;/* Calculate bytes available  */
    //uint32                      index = 0;
    //char                       *current_ptr;    /* Current block pointer      */
    //char                       *next_ptr;       /* Next block pointer         */
    uint32                      rlt;

    /* print the byte pool general info */
    ASSERT_PRINT ("\n byte_static_heap infomation:");

    ASSERT_PRINT ("\nPool_name:     %s\nStart_address: 0x%p \nPool_size:     %dk \nAvailable:     %d \nFragment:      %d"
                  ,pool_ptr->tx_byte_pool_name       /*Pool_name*/
                  ,pool_ptr->tx_byte_pool_start      /*Start_address*/
                  ,pool_ptr->tx_byte_pool_size/1024   /*Pool_size*/
                  ,pool_ptr->tx_byte_pool_available  /*Available*/
                  ,pool_ptr->tx_byte_pool_fragments);/*Fragment*/

    /* print the suspended task */
    if (pool_ptr->tx_byte_pool_suspension_list)
    {
        /* some task suspending on the pool */
        suspend_list = pool_ptr->tx_byte_pool_suspension_list;

        do
        {
            //4
            ASSERT_PRINT ("Suspend Task_Name :  %s ",
                          suspend_list->tx_thread_name);
            suspend_list = suspend_list->tx_suspended_next;

        }
        while (suspend_list != pool_ptr->tx_byte_pool_suspension_list);
    }
    else
    {
        /* no task suspending on the pool */
        ASSERT_PRINT ("\nNo task suspending on the pool !");
    }

    ASSERT_PRINT ("\nOriginal free block info:\n");

    rlt = _TXAS_PrintHeapBlockInfo (pool_ptr);

    if (rlt != SCI_SUCCESS)
    {
        /* This memory list has been destroied.  */
        return;
    }

    ASSERT_PRINT ("\nAfter garbage collect free block info:\n");

    _TXAS_HeapClean (pool_ptr);
    _TXAS_PrintHeapBlockInfo (pool_ptr);

    ASSERT_PRINT ("\n========================================================\n");
#endif
}


/*****************************************************************************/
//  Description:    The PrintPsQueueInfo function is used print protocal stack
//                  queue information, such as mem address,mem size etc
//  Global resource dependence:
//  Author:         Dong.Yang
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintPsQueueInfo (void)
{
#ifdef PRODUCT_DM
    kal_debug_print_queue_info();
#endif
}

LOCAL void _TXAS_PrintSmeBuff2File (void)
{
#ifdef PRODUCT_DM
    ASSERT_PRINT ("\n=============== Dump Sme Memory To One File start==============\n");

    sme_OS_DumpSmeBuff();

    ASSERT_PRINT ("\n=============== Dump Sme Memory To One File finish=============\n");

#endif
}

LOCAL void _TXAS_PrintSmeCtrlBuffPoolInfo (void)
{
#ifdef PRODUCT_DM
    sme_OS_DumpCtrlBuffPoolInfo();
#endif
}

/*****************************************************************************/
//  Description:    The function is used to print sme function entry log
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_PrintSmeFuncEntryLog (void)
{
#ifdef PRODUCT_DM
    sme_print_func_entry_log();
#endif
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintHelpInfo (void *exp, void *file, uint32 line)
{
    if (s_cmd_list_ptr)
    {
        uint32 i = 0;

        while (s_cmd_list_ptr[i].cmd_name[0] != '\0')
        {
            OS_DumpPrint ( (char *) s_cmd_list_ptr[i].cmd_name, sizeof (s_cmd_list_ptr[i].cmd_name));
            i++;
        }
    }
    else
    {
        const char  *help_ptr = " ***Could Not Find Assert Command Table***";
        OS_DumpPrint ( (char *) help_ptr, strlen (help_ptr));
    }
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintAssertInfo (void *exp, void *file, uint32 line)
{

    _TXAS_PrintPAssertInfo (exp, file, line, s_assert_info_ptr);

    if (PNULL != s_assert_info_ptr)
    {
        OS_DumpPrint ( (char *) s_assert_info_ptr, strlen ( (char *) s_assert_info_ptr));
    }

    _TXAS_DumpCurThreadInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintImportRegisters (void *exp, void *file, uint32 line)
{
    _TXAS_PrintUserRegInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_DumpMemory2File (void *exp, void *file, uint32 line)
{
    _TXAS_Cmd_PrintAssertInfo (exp,file,line);
    _TXAS_PrintUserRegInfo();
    OS_DumpMemory();
    _TXAS_PrintSmeBuff2File();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_DumpAllocatedMemInfo (void *exp, void *file, uint32 line)
{
    SCI_DumpAllocatedMemInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintMemoryInfo (void *exp, void *file, uint32 line)
{
    SCI_DumpPoolInfo();
    SCI_DumpHeapInfo();
    SCI_DumpAppMemInfo();
    /*+cr241956*/
    _TXAS_PrintSmeCtrlBuffPoolInfo();
    /*-cr241956*/
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintThreadInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintThreadInfo (PRINT_PATH_NORMAL);
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintStackInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintStackInfo (PRINT_PATH_NORMAL);
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintCallbackFuncList (void *exp, void *file, uint32 line)
{
    _TXAS_PrintCallbackFuncList();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_ReadMemory (void *exp, void *file, uint32 line)
{
    _TXAS_ReadMemory();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_WriteMemory (void *exp, void *file, uint32 line)
{
    _TXAS_WriteMemory();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintTaskTimeList (void *exp, void *file, uint32 line)
{
    _TXAS_PrintTaskTimeList();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_ReadMemoryTofile (void *exp, void *file, uint32 line)
{
    _TXAS_ReadMemoryTofile();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_DumpSwitchMemory (void *exp, void *file, uint32 line)
{
    DumpSwitchMemory();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintEventInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintEventInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintMutexInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintMutexInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintSemaphoreInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintSemaphoreInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintFatCtlBlkInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintFatCtlBlkInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintVersionInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintVersionInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_ResetMCU (void *exp, void *file, uint32 line)
{
   OS_Reset();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintTimerInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintTimerInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintBytePool (void *exp, void *file, uint32 line)
{
    _TXAS_PrintHeapInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintPSFuncCallStack (void *exp, void *file, uint32 line)
{
    _TXAS_PrintSmeFuncEntryLog();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintPSQueueInfo (void *exp, void *file, uint32 line)
{
    _TXAS_PrintPsQueueInfo();
}

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_AllAssertInfo (void *exp, void *file, uint32 line)
{

    _TXAS_Cmd_PrintAssertInfo (exp,file,line);
    _TXAS_PrintUserRegInfo();
    OS_DumpMemory();
    _TXAS_PrintSmeBuff2File();
    SCI_DumpAllocatedMemInfo();
    SCI_DumpPoolInfo();
    SCI_DumpHeapInfo();
    SCI_DumpAppMemInfo();
    /*+cr241956*/
    _TXAS_PrintSmeCtrlBuffPoolInfo();
    /*-cr241956*/
    _TXAS_PrintThreadInfo (PRINT_PATH_NORMAL);
    _TXAS_PrintStackInfo (PRINT_PATH_NORMAL);
    _TXAS_PrintCallbackFuncList();
    _TXAS_PrintFatCtlBlkInfo();
    _TXAS_PrintTaskTimeList();
    _TXAS_PrintEventInfo();
    _TXAS_PrintMutexInfo();
    _TXAS_PrintTimerInfo();
    _TXAS_PrintSemaphoreInfo();
    _TXAS_PrintHeapInfo();
    _TXAS_PrintSmeFuncEntryLog();
    _TXAS_PrintPsQueueInfo();
#ifndef MODEM_PLATFORM
    MMITHEME_PrintInfo (exp, file, line);
#endif
    OS_SendPacket (SEND_PACKET_END);
    SCI_PrintAssertString("Dump Finished");

}

#ifdef DSP_USB_LOG
/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _TXAS_Cmd_PrintDSPAssertInfo (void *exp, void *file, uint32 line)
{
    const char  *dump_ok_ptr = " Dump DSP Assert Infomation finished!";
    const char  *dump_fail_ptr = " Dump DSP Assert Infomation NOT Support!";

    if (DSP_LOG_ARM_COM_DEBUG == REFPARAM_GetDspDebugPortPhyNo())
    {
        if (REFPARAM_GetDspCardLogFlag())
        {
            SIO_DumpDSPData();
        }

        SIO_DumpDSPMemory();
        OS_DumpPrint ( (char *) dump_ok_ptr, strlen (dump_ok_ptr));
    }
    else
    {
        OS_DumpPrint ( (char *) dump_fail_ptr, strlen (dump_fail_ptr));
    }
}
#endif


/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/

LOCAL void _TXAS_Cmd_PrintiramInfo (void *exp, void *file, uint32 line)
{
    IRAM_PrintiramInfo();
}

LOCAL void DUMP_RunningNV_Nor (void)
{
    uint32  addr;
    uint32  size;
#ifndef FLASH_TYPE_SPI
    NOR_FLASH_CONFIG_PTR  nor_flash_cfg_ptr = NULL;
    EMC_GetNorFlash_Partition (&nor_flash_cfg_ptr);
    addr = (uint32) (nor_flash_cfg_ptr->efs_start_addr)- (uint32) (nor_flash_cfg_ptr->file_sect_num + 1) * (nor_flash_cfg_ptr->sect_size);
    size = (uint32) (nor_flash_cfg_ptr->sect_num - nor_flash_cfg_ptr->file_sect_num-1) * (nor_flash_cfg_ptr->sect_size);
#else
    addr = s_platform_patitiion_config.efs_start_addr;
    size  = s_platform_patitiion_config.flash_size - (s_platform_patitiion_config.efs_start_addr - s_platform_patitiion_config.start_addr);
#endif
    ASSERT_PRINT ("\nRunningNV addr = %8x, RunningNV size = %8x:\n", addr,size);
    OS_DumpDefiniteMemory (addr, size);
}

LOCAL void DUMP_FixedNV_Nor (void)
{
    uint32  addr;
    uint32  size;
#ifndef FLASH_TYPE_SPI
    NOR_FLASH_CONFIG_PTR  nor_flash_cfg_ptr = NULL;
    EMC_GetNorFlash_Partition (&nor_flash_cfg_ptr);
    addr = (uint32) nor_flash_cfg_ptr->fixnv_addr;
    size = (uint32) nor_flash_cfg_ptr->efs_start_addr - (uint32) nor_flash_cfg_ptr->fixnv_addr;
#else
    addr = s_platform_patitiion_config.fixnv_addr;
    size  = s_platform_patitiion_config.prodinfo_addr - s_platform_patitiion_config.fixnv_addr;
#endif
    ASSERT_PRINT ("\nFixedNV addr = %8x, FixedNV size = %8x:\n", addr,size);
    OS_DumpDefiniteMemory (addr, size);
}

LOCAL void _TXAS_Cmd_DumpFixedNV (void *exp, void *file, uint32 line)
{
    ASSERT_PRINT ("Dump Fixed NV Start:\n");
#ifndef  NANDBOOT_SUPPORT
    DUMP_FixedNV_Nor();
#else
    DUMP_NV_Nand (BML_DOWNLOADED_NV_PART_NAME);
#endif
    ASSERT_PRINT ("Dump Fixed NV End:\n");
}


LOCAL void _TXAS_Cmd_DumpRunningNV (void *exp, void *file, uint32 line)
{
    ASSERT_PRINT ("Dump Running NV Start:\n");
#ifndef  NANDBOOT_SUPPORT
    DUMP_RunningNV_Nor();
#else
    DUMP_NV_Nand (STL_NV_FS_PART);
#endif
    ASSERT_PRINT ("Dump Running NV End:\n");
}


/*****************************************************************************/
//  Description:    The _SCI_SystemAssert function is typically used to identify logic
//                  errors during program development, by implementing the
//                  expression argumnet to evaluate to SCI_FALSE only when the
//                  program is operating incorrectly. After debugging is
//                  complete, assertion checking can be turned off without
//                  modifying the source file by deleting the predifinition
//                  _DEBUG.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
extern void Debug_Uart4_puts(char *Str);
int systemcount = 0;

LOCAL void _TXAS_PrintPAssertInfo_Uart4 (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
    void    *assert_info  // the pointer to assert info
)
{
    uint32  i;
    uint32  name_len;
    char    file_name[SCI_MAX_NAME_SIZE + 1];

    ASSERT_PRINT ("\nCurrent Version: \n        %s\n", COMMON_GetProjectVersionInfo());

    // Print assert information to SIO.
    if (b_is_watchdog)
    {
        ASSERT_PRINT ("\nPossible error: Loop forever at 0x%08lx\n%s\n", _tx_pc_val_before_irq, exp);
    }
    else if (EXCEPTION_E == assert_type)
    {
        if (ABORT_EXCEPTION == s_exception_type)
        {
            ASSERT_PRINT ("\nException at 0x%08lx\nASSERT(%s)\n\nAbort fault(DFSR:0x%08lx): %s !\nFault address :0x%08lx\n", \
                          cur_mode_reg.r14,
                          exp,
                          abort_DFSR_reg,
                          abort_fault_status_debug_list[ (abort_DFSR_reg&0xf) ],
                          abort_FAR_reg);
        }
        else
        {
            ASSERT_PRINT ("\nException at 0x%08lx\nASSERT(%s)\n", cur_mode_reg.r14, exp);
        }
    }
    else
    {
        if (PNULL != assert_info)
        {
            ASSERT_PRINT ("\nFile:  %s\nLine:  %ld\nPASSERT(%s)\n", file, line, exp);
        }
        else
        {
            ASSERT_PRINT ("\nFile:  %s\nLine:  %ld\nASSERT(%s)\n", file, line, exp);
        }

    }

    // Delete directory name string of the file.
    sprintf (str_buf, "%s", (char *) file);

    for (i = 0, name_len = 0; str_buf[i] != 0; i++)
    {
        if ( (str_buf[i] == '\\') || (str_buf[i] == '/'))
        {
            name_len = 0;
        }
        else
        {
            name_len++;
        }
    }

    strcpy (file_name, str_buf + strlen (str_buf) - name_len);

    // Print assert information to LCD.
    if (b_is_watchdog)
    {
        sprintf (str_buf, "Loop for ever at 0x%08lx%s", _tx_pc_val_before_irq, (char *) exp);
    }
    else if (EXCEPTION_E == assert_type)
    {
        // Here R14 should be the return address!
        if (ABORT_EXCEPTION == s_exception_type)
        {
            sprintf (str_buf, "\nException at 0x%08lx\nASSERT(%s)\n\nAbort fault(DFSR:0x%08lx): %s !\nFault address :0x%08lx\n", \
                     cur_mode_reg.r14,
                     (char *) exp,
                     abort_DFSR_reg,
                     (char *) abort_fault_status_debug_list[ (abort_DFSR_reg&0xf) ],
                     abort_FAR_reg);
        }
        else
        {
            sprintf (str_buf, "\nException at 0x%08lx\nASSERT(%s)\n", cur_mode_reg.r14, (char *) exp);
        }
    }
    else
    {
        if (PNULL != assert_info)
        {
            sprintf (str_buf, "File:%s Line:%ld PASSERT(%s)", file_name, line, (char *) exp);
        }
        else
        {
            sprintf (str_buf, "File:%s Line:%ld ASSERT(%s)", file_name, line, (char *) exp);
        }
    }

    if (PNULL != assert_info)
    {
        strcpy (str_buf + strlen (str_buf), assert_info);
    }

     Debug_Uart4_puts(str_buf);

}


LOCAL void _TXAS_SystemAssert (
    void    *exp,     // Expression (including pointers) that evaluates
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
    char    *assert_info_ptr
)
{

#ifndef WIN32
    char            cmd_char    = 0;
    uint8           assert_mode = 0;
    INIT_STATUS_E   init_status = STATUS_START_INIT;
    uint32          cur_sp      = 0;
    uint32          cur_lr      = 0;
    uint32          cur_pc      = 0;
    uint32          cur_cpsr    = 0;
    uint32          i           = 0;

    TX_INTERRUPT_SAVE_AREA

    dump_usb_registers();

#if defined(BLUETOOTH_SUPPORT) && defined(PLATFORM_SC6531EFM)
    //send dump share memory log(RX) Interrup to BT
    if(0x5a5a != *((uint16 *)(0x301003F0)))
    {
         (*(volatile uint32*)(0x8B000160)) |= (1 << 16);
    }
#endif

    OS_SendTestPointRequest (0xff, 0xf0);
    OS_SendTestPointRequest ((uint32)exp, (uint32)file);
    OS_SendTestPointRequest ((uint32)line, (uint32)assert_info_ptr);

    _TXAS_SaveMainReg (&cur_sp, &cur_lr, &cur_pc);

    s_assert_info_ptr = (uint8 *) assert_info_ptr;


    OSPARAM_GetAssertMode (&assert_mode);

    if ( (ASSERT_MAX_COUNT < s_assert_cnt) && (SCI_ASSERT_DEBUG_MODE == assert_mode) && (EXCEPTION_E != assert_type) )
    {
        /*to prevent assert again*/
        return;
    }

    if((EXCEPTION_E == assert_type) && (s_assert_cnt > 1))
        {while(1);}


    SCI_InitLogSwitch (FALSE);

    WDG_TimerStop ();

    //_GPIO_PROD_SetVal (GPIO_PROD_LCD_BL_EN_ID, SCI_TRUE);

    CHIP_ActiveSMCLK (SCI_TRUE);

    TX_DISABLE_FIQ_IRQ
    INT_HAL_DisableINT();

    MMU_AlignFaultDisable();
    MMU_UseNorDOM();    //Set MMU to Manager Access Permission

#if defined(PLATFORM_UWS6121E)
    if(1 == LOAD_modem_isFinish())
    {
      ipc_nofity_cp_assert();
     }
#endif

    OS_SendTestPointRequest (0xff, 0xf2);

    SCI_EnterAssertMode();

    CHIP_BeforeAssert();


    // Set mode.
    if (EXCEPTION_E != assert_type)
    {
        // Save all mode registers.
        __tx_save_cur_reg();

        cur_mode_reg.r13 = cur_sp;
        cur_mode_reg.r14 = cur_lr;
        cur_mode_reg.PC  = cur_pc;

        cur_cpsr = _TXAS_GetCPSR() & 0xf;

        if (cur_cpsr == MCU_IRQ_MODE)
        {
            assert_type = IRQ_E;

            irq_mode_reg.r14 = cur_lr;
        }
        else if (cur_cpsr == MCU_SVC_MODE)
        {
            assert_type = NORMAL_E;

            svc_mode_reg.r14 = cur_lr;
        }
        else if (cur_cpsr == MCU_FIQ_MODE)
        {
            assert_type = FIQ_E;
        }
    }

    OS_SendTestPointRequest (0xff, 0xff);

    _TXAS_SetAssert (SCI_TRUE);
//bringup_tmp
#if 0 //defined(PLATFORM_UWS6121E)     //need to set some addr to MEM_TYPE_DEVICE before dump
    hwp_pwrctrl->PWR_HWEN |= CP_PWRCTRL_ZSP_PON_EN;   //enable PON mode as this bit=0 when watchdog reset
    mmu_MapOnPage_Set(0x20000000, 0x20000000,MEM_TYPE_DEVICE, 0x10000000);
    mmu_MapOnPage_Set(0x40000000, 0x40000000,MEM_TYPE_DEVICE, 0x10000000);
#endif

#ifdef CARD_LOG
    if (SCI_CARD_LOG_SAVE_IDLE == g_card_log_save_status)
    {
        g_card_log_save_status = SCI_CARD_LOG_SAVE_BUSY;
        OS_SendTestPointRequest (0xff, 0xf1);
        //SCI_TRACE_LOW:"LOGSAVE: CHIPDRV SCI_PAssert"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT,THREADX_ASSERT_3534_112_2_17_23_5_41_1104, (uint8 *) "");
        LogSaveOutput_Send_Assert_Message (exp, file, line);
        g_card_log_save_status = SCI_CARD_LOG_SAVE_IDLE;
    }
#endif


#if !defined(PLATFORM_UWS6121E)
    s_is_dsp_run = CHIP_ResetDspInAssertMode();
#endif

    //get the init status
    init_status = INIT_GetInitStatus();

    // Delay for a while to print out information.
    if (SCI_ASSERT_DEBUG_MODE == assert_mode)
    {
#if !defined(PLATFORM_UWS6121E) //temp_stub_sgq
        CHIP_DspOutPutAssertInfo();
#endif
        if (init_status == STATUS_START_INIT)
        {
            //connect the debug port to export assert info
            _TXAS_InitSystemBeforeBootUp();
        }

        /* It is necessary to set assert flag and sme_flush_buffer will dump
         * data through com directly. */
        _TXAS_SetAssertFlag (SCI_TRUE);
        OS_SendRemainLogMessage();
#ifdef PRODUCT_DM
        sme_flush_buffer (0UL);
#endif
    }
    else
    {
    //temporarily support to dump data for user version,and No reset
    #ifndef DEBUG_FOR_USER_SUPPORT
        if (init_status == STATUS_FINISH_PROD_INIT)
        {
            if ( (RESTART_BY_ALARM == POWER_GetRestartCondition())
                    || (RESTART_BY_CHARGE == POWER_GetRestartCondition())
               )

            {
                POWER_SetResetMode (NORMAL_MODE);
            }
            else
            {
                POWER_SetResetMode (ASSERT_BACK_MODE);
            }
        }

        OS_Reset();

        for (;;) {}
    #endif
    }

#ifdef DSP_USB_LOG

    if (FALSE == REFPARAM_GetDspCardLogFlag())
    {
        SIO_DumpDSPData();

        if (TRUE == SCI_IsDSPAssert())
        {
            SIO_DumpDSPMemory();
            SCI_SetDSPAssertFlag (FALSE);   //recover dsp assert flag to FALSE
        }
    }

#endif

    // Code below is for assertion showing a menu for more debugging information
    // retrieving functions. Legel tool needs to support it.
    // Write dump data here to clear data before.
    // Ensure the next frame data can be received properly.
    // We have to send two frames to ensure channel Server can get the last one!

    OS_DumpPrint (SCI_NULL, 0);
    OS_DumpPrint (SCI_NULL, 0);
    s_cmd_list_ptr = (ASSERT_CMD_LIST *) threadx_assert_list;

    if (
        (DSK_COM0 != SIO_GetPhyPortNo (COM_DEBUG))
        && (DSK_COM1 != SIO_GetPhyPortNo (COM_DEBUG))
    )


#if 1
    {
        // Print assert information to SIO and LCD.

        _TXAS_PrintPAssertInfo (exp, file, line, assert_info_ptr);

        if (PNULL != assert_info_ptr)
        {
            OS_DumpPrint (assert_info_ptr, strlen (assert_info_ptr));
        }

        _TXAS_DumpCurThreadInfo();
        _TXAS_PrintUserRegInfo();

        // Print menu info.
        i = 0;

        while (s_cmd_list_ptr[i].cmd_name[0] != '\0')
        {
            OS_DumpPrint ( (char *) s_cmd_list_ptr[i].cmd_name, sizeof (s_cmd_list_ptr[i].cmd_name));
            i++;
        }

        OS_DumpFlush();

        for (;;)
        {
            _TXAS_GetALine (cmd_buf);
            SCI_TraceLowly("getAline:%s,len:%d\n",cmd_buf,strlen(cmd_buf));

            if (strlen(cmd_buf) == 1)
            {
                cmd_char = cmd_buf[0];
            }
            else
            {
                i = 0;

                while (s_cmd_list_ptr[i].cmd_name[0] != '\0')
                {
                    OS_DumpPrint ( (char *) s_cmd_list_ptr[i].cmd_name, sizeof (s_cmd_list_ptr[i].cmd_name));
                    i++;
                }

                continue;
            }

            i = 0;

            while (cmd_char != s_cmd_list_ptr[i].cmd_name[0] && s_cmd_list_ptr[i].cmd_name[0] != '\0')
            {
                i++;
            }

            if (s_cmd_list_ptr[i].cmd_name[0] == '\0')
            {
                i = 0;
            }

            if (s_cmd_list_ptr[i].cmd_func != NULL)
            {
                s_cmd_list_ptr[i].cmd_func (exp, file, line);
            }

            OS_DumpFlush();

        }
    }
#endif

    #if 0
    {

        /* Print assert information to SIO and LCD. */
        _TXAS_PrintPAssertInfo(exp, file, line, assert_info_ptr);

        if (PNULL != assert_info_ptr)
            OS_DumpPrint(assert_info_ptr, strlen (assert_info_ptr));

        _TXAS_DumpCurThreadInfo();
        _TXAS_PrintUserRegInfo();

        /* Print menu info. */
        i = 0;

        while (s_cmd_list_ptr[i].cmd_name[0] != '\0') {
            OS_DumpPrint((char *) s_cmd_list_ptr[i].cmd_name, sizeof (s_cmd_list_ptr[i].cmd_name));
            i++;
        }

        OS_DumpFlush();



        for (; ;) {
            _TXAS_GetALine(cmd_buf);

            if (strlen (cmd_buf) == 1) {
                cmd_char = cmd_buf[0];

                i = 0;
                while (cmd_char != s_cmd_list_ptr[i].cmd_name[0] && s_cmd_list_ptr[i].cmd_name[0] != '\0') {
                    i++;
                }

                if (s_cmd_list_ptr[i].cmd_name[0] == '\0')
                    i = 0;

                if (s_cmd_list_ptr[i].cmd_func != NULL)
                    s_cmd_list_ptr[i].cmd_func (exp, file, line);

                OS_DumpFlush();
            }
        }
    }

#endif

    else
    {
        i = 0;

        while (s_cmd_list_ptr[i].cmd_name[0] != '\0')
        {
            sprintf ( (char *) str_buf, (char *) s_cmd_list_ptr[i].cmd_name);
            LCD_DisplayText (0,0,str_buf, strlen (str_buf));

            if (s_cmd_list_ptr[i].cmd_func != NULL && s_cmd_list_ptr[i].cmd_auto_exec)
            {
                s_cmd_list_ptr[i].cmd_func (exp, file, line);
            }

            i++;
        }

        for (;;) {}

        //OS_Reset();

    }

#else // WIN32 Mode

    // We disable interrupt here simply to avoid two ASSERT occurs,
    // cause in Visual C++ platform, different ASSERT may occurs in different threads!
    // We just care the first ASSERT more.
    SCI_TRACE_LOW (assert_info_ptr);


    SCI_DisableIRQ();

    _assert (exp, file, line);

    SCI_RestoreIRQ();

#endif // End of WIN32

}
/**---------------------------------------------------------------------------*
 **                      PUBLIC FUNCTIONS DECLARATION                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to check if the system is in the assert mode
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsAssert (void)
{
    return s_is_assert ;
}


/*****************************************************************************/
//  Description:    The function is used to check if dsp can run in the assert mode
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsDspRunInAssertMode (void)
{
    return  s_is_dsp_run ;
}

/*****************************************************************************/
//  Description:    The function is used to get a global flag. The flag
//                  indicates whether SCI_Assert be called.
//  Global resource dependence:
//  Author:         rick.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_GetAssertFlag (void)
{
    return s_assert_flag;
}

/*****************************************************************************/
//  Description:    The function is used to covert the assert information to char string
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC char *SCI_SaveAssertInfo (
    const char *x_format, ...)
{
    va_list    args;
    int        nBuf;
    char        *temp_str = passert_info_str;

    va_start (args, x_format);
    nBuf = vsprintf (passert_info_str, x_format, args);

    /* was there an error? was the expanded string too long? */
    if (nBuf < 0)
    {
        temp_str = NULL;
    }
    else if (nBuf > MAX_PASSERT_INFO_STR - 1)
    {
        passert_info_str[MAX_PASSERT_INFO_STR - 1] = '\0';
    }

    va_end (args);

    return temp_str;
}

/*****************************************************************************/
//  Description:    The function is used to print string about assert information
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PrintAssertString (char *x_format, ...)
{
    char      *format_str = str_buf;
    va_list    args;
    int        nBuf;

    va_start (args, x_format);
    nBuf = vsnprintf (format_str, PRINT_STR_LEN_MAX-1, x_format, args);

    /* Was the expanded string too long? */

    va_end (args);

    if (nBuf > 0)
    {
        OS_DumpPrint (format_str, (uint32) nBuf);
    }
}

/*****************************************************************************/
//  Description:    The function is used to register reading asseret Info Function.
//  Global resource dependence:
//  Author:         Johnson.sun
//  Note:
/*****************************************************************************/
PUBLIC void SCI_RegisterReadAssertInfoCmdRoutine (void)
{
    DIAG_RegisterCmdRoutine (DIAG_READ_ASSERT_F,  _TXAS_HandleReadAssertInfo);
}


/*****************************************************************************/
//  Description:    The SCI_HandleUndefined function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleUndefined (void)
{
    assert_type = EXCEPTION_E;
    s_exception_type = UNDEFINED_EXCEPTION;
    SCI_Assert ("Undefined exception handler !", __FILE__, __LINE__);/*assert to do*/
}

/*****************************************************************************/
//  Description:    The SCI_HandleSwi function is used handle SWI.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleSwi (void)
{
    assert_type = EXCEPTION_E;
    s_exception_type = SWI_EXCEPTION;
    SCI_Assert ("SWI exception handler !", __FILE__, __LINE__);/*assert to do*/
}

/*****************************************************************************/
//  Description:    The SCI_HandlePrefetch function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandlePrefetch (void)
{
    assert_type = EXCEPTION_E;
    s_exception_type = PREFETCH_EXCEPTION;
    SCI_Assert ("Prefetch exception handler !", __FILE__, __LINE__);/*assert to do*/
}

/*****************************************************************************/
//  Description:    The SCI_HandleAbort function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleAbort (void)
{
    assert_type = EXCEPTION_E;
    s_exception_type = ABORT_EXCEPTION;
    SCI_Assert ("Abort exception handler !", __FILE__, __LINE__);/*assert to do*/
}

/*****************************************************************************/
//  Description:    The SCI_HandleReserved function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleReserved (void)
{
    assert_type = EXCEPTION_E;
    SCI_Assert ("Reserved exception handler !", __FILE__, __LINE__);/*assert to do*/
}

/*****************************************************************************/
//  Description:    The SCI_HandleWatchDog function is used handle watch dog.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleWatchDog (void)
{
    b_is_watchdog = SCI_TRUE;
    SCI_Assert ("Watch Dog Timer Expired. ", __FILE__, __LINE__);/*assert to do*/
}

/*****************************************************************************/
//  Description:    The SCI_SetAssertMode function used to set assert mode.
//                  If b_debug_mode is SCI_TURE, it prints assert information
//                  when assert; else reset.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_SetAssertMode (
    BOOLEAN b_debug_mode
)
{
    uint8   mode;

    if (b_debug_mode)
    {
        mode = SCI_ASSERT_DEBUG_MODE;
    }
    else
    {
        mode = SCI_ASSERT_RELEASE_MODE;
    }

    OSPARAM_SetAssertMode (mode);
}

/*****************************************************************************/
//  Description:    The SCI_SetDSPAssertFlag function used to set dsp assert flag.
//  Global resource dependence: is_dsp_assert
//  Author:         Fei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_SetDSPAssertFlag (BOOLEAN flag)
{
    if (flag)
    {
        is_dsp_assert = TRUE;
    }
    else
    {
        is_dsp_assert = FALSE;
    }
}

/*****************************************************************************/
//  Description:    The SCI_IsDSPAssert function used to get dsp assert flag.
//  Global resource dependence: is_dsp_assert
//  Author:         Fei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsDSPAssert (void)
{
    return is_dsp_assert;
}

/*****************************************************************************/
//  Description:    The SCI_SetAssertMode function used to set assert mode.
//                  If b_debug_mode is SCI_TURE, it prints assert information
//                  when assert; else reset.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_SetRecordAssertFlag (BOOLEAN b_record_assert_flag)
{
    if (b_record_assert_flag)
    {
        // saveAssertInfo = TRUE;
    }
    else
    {
        // saveAssertInfo = FALSE;
    }
}

/*****************************************************************************/
//  Description:    The function is used to print thread information to shell
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PrintThreadInfoToShell (void)
{
    _TXAS_PrintThreadInfo (PRINT_PATH_DIAG);
}

/*****************************************************************************/
//  Description:    The function is used to print stack information to shell
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PrintStackInfoToShell (void)
{
    _TXAS_PrintStackInfo (PRINT_PATH_DIAG);
}

/*****************************************************************************/
//  Description:    The SCI_Assert function is typically used to identify logic
//                  errors during program development, by implementing the
//                  expression argumnet to evaluate to SCI_FALSE only when the
//                  program is operating incorrectly. After debugging is
//                  complete, assertion checking can be turned off without
//                  modifying the source file by deleting the predifinition
//                  _DEBUG.
//  Global resource dependence:
//  Author: Richard.Yang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_Assert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
)
{
    _TXAS_SystemAssert (exp, file, line, PNULL);
}

PUBLIC void SCI_Assert_NoDebug (void)
{
    _TXAS_SystemAssert (0, NULL, 0, NULL);
}

/*****************************************************************************/
//  Description:    The SCI_PAssert function supports printing message and variables
//                  when asserting.
//  Author:     Bin.Lu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PAssert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
    char    *assert_info_ptr
)
{
    _TXAS_SystemAssert (exp, file, line, assert_info_ptr);
}

/*****************************************************************************/
//  Description:    The SCI_DAssert function will provide debug MMI regardless of release mode
//                  Called by diag task receive assert cmd and other uplayer artificial assert
//  Global resource dependence:
//  Author: Richard.Yang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_DAssert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
)
{
    _TXAS_SystemAssert (exp, file, line, PNULL);
}
#endif // End of WIN32
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // End of _DEBUG


