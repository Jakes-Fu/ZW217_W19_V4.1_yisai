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
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef _DEBUG

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// RTOS ThreadX API head file.
#include "threadx_os.h"
#include "os_api.h"
#include "os_apiext.h"
#include "os_param.h"
#include "os_import.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "cmddef.h"
#include "power.h"

#ifndef WIN32 // In THUMB Target Mode
// We can set the program address to the bottom of the Interrupt Vector!
#define     TOP_OF_PROGRAM          0x10
#define     THUMB_SUB_SP_CODE_MASK  0xff80
#define     THUMB_SUB_SP_CODE       0xb080

#define     THUMB_ADD_SP_CODE_MASK  0xff80
#define     THUMB_ADD_SP_CODE       0xb000

#define     THUMB_PUSH_CODE_MASK    0xfe00
#define     THUMB_PUSH_CODE         0xb400

#define     THUMB_BL_MASK           0xf000
#define     THUMB_BL_CODE           0xf000

#define     SIZE_OF_REG_LIST        9        // In Thumb instruction, there
// is only 9 register.

#define     STACK_INIT_VAL          0x5A5A5A5A    //MEM_INIT_VALUE = 0x5A

#define     COMMAND_MAX_SIZE        20

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
    uint32   pc;
} CONTEXT_MINIMAL_SAVE_REG_T;

typedef enum
{
    NORMAL_E,
    EXCEPTION_E,
    IRQ_E,
    FIQ_E
} ASSERT_TYPE_E;
#define MCU_IRQ_MODE 0x2
#define MCU_FIQ_MODE 0x1
#define MCU_SVC_MODE 0x3

// #define  DS_LOADING_TRACK
#ifdef DS_LOADING_TRACK
extern uint64 uart_time;
extern uint64 idle_time;
uint64 excute_time = 0;
uint64 excute_time_div = 0;
#endif


LOCAL g_is_assert =  SCI_FALSE ;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
extern void SCI_BeforeAssertReset (POWER_RESTART_CONDITION_E restart_condition);
extern void LCD_PointerSaveInAssertResetMode (void);
extern POWER_RESTART_CONDITION_E POWER_GetRestartCondition (void);
extern uint32 _tx_pc_val_before_fiq;

LOCAL void PrintAssertInfo (void *exp, void *file, uint32 line);

LOCAL void PrintCallbackFuncList (void);

LOCAL void PrintUserRegInfo (void);

LOCAL void PrintThreadInfo (void);

LOCAL void PrintStackInfo (void);

LOCAL void ReadMemory (void);

LOCAL uint32 GetSPSR (void);
LOCAL uint32 GetCPSR (void);
LOCAL void   PrintCurrentThreadInfo (void);

LOCAL void   PrintCurThreadCallbackFuncList (void);
LOCAL uint32 GetStackBacktrace (BLOCK_ID thread_id);
LOCAL uint32 GetCallbackFunAddr (uint32 *sp, uint32 *lr, uint32 *fun);
LOCAL void   GetALine (char *str);
LOCAL uint32 GetNumFromStr (uint32 *value_ptr, char *src_ptr);
LOCAL uint32 CheckMemAccessParam (uint32  start_addr, uint32  size);
//#ifdef DS_LOADING_TRACK
LOCAL void PrintTaskTimeList (void);
//#endif



LOCAL void threadx_dump_print_dispatch (char *string, uint32 size)
{
    OS_DumpPrint (string, size);
}

#define OS_DumpPrint   threadx_dump_print_dispatch


/**---------------------------------------------------------------------------*
 **                         Variables                                         *
 **---------------------------------------------------------------------------*/
// #include "tx_illdb.s"
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

extern CUR_REG_T     cur_mode_reg;
extern OTHER_REG_T   svc_mode_reg;
extern OTHER_REG_T   irq_mode_reg;
extern OTHER_REG_T   undefine_mode_reg;
extern OTHER_REG_T   abort_mode_reg;
extern uint32   __tx_save_cur_reg (void);

// defined tx_tcs.s
extern uint32   _tx_pc_val_before_irq;

// defined threadx_os.c
extern SCI_THREAD_T *s_static_thread_ptr_table[];

// defined in threadx_mem.c
extern SCI_BYTE_POOL_T  byte_heap;
extern SCI_BLOCK_POOL_T pool_52;
extern SCI_BLOCK_POOL_T pool_100;
extern SCI_BLOCK_POOL_T pool_300;
extern SCI_BLOCK_POOL_T pool_600;
extern SCI_BLOCK_POOL_T pool_1600;

//@lin.liu(2003-05-20)
//  Store assert information to nvitem.  see efs_item.c to get more information
extern int EFS_Store_Assert_Info (const char *file, int line, const char *exp);
extern void Record_Diagnostic_Info (const char *file, int line,
                                    const char *exp, const char *assert_info);

extern void LCD_DisplayText (uint16 , uint16 , char *, uint16);

const char debug_cmd_list[] =
{
    "\nAssert Debug Menu:\r\n\
        0. Print help info.\r\n\
        1. Print assert info.\r\n\
        2. Print important registers.\r\n\
        3. Dump all memory to a file.\r\n\
        4. Print memory allocated info.\r\n\
        5. Print memory pools info.\r\n\
        6. Print tasks info.\r\n\
        7. Print stack info.\r\n\
        8. Print callback function list.\r\n\
        9. Read memory\r\n\
        a. Dump task usage\r\n\
        f. Reset MCU\r\n"
};
char        str_buf[700];
LOCAL char      cmd_buf[COMMAND_MAX_SIZE + 1];

LOCAL ASSERT_TYPE_E assert_type     = NORMAL_E;
LOCAL BOOLEAN       b_is_watchdog   = SCI_FALSE;
LOCAL BOOLEAN       b_is_passert    = SCI_FALSE;

LOCAL uint32    last_sp;        // stack pointer when assertion happens
LOCAL uint32    last_lr;        // Return address when assertion happens
LOCAL uint32    func_entry;     // Current layer function entry address

//    LOCAL uint32  s_assert_sp;
//    LOCAL uint32  s_assert_lr;
//    LOCAL uint32  s_assert_pc;

LOCAL uint32    s_is_current_thread_saved = SCI_FALSE;
//@Steven
LOCAL uint32  dsp_is_run = 0;


extern void dump_DSP_Download (void);
#endif // End of WIN32

#define     MAX_PASSERT_INFO_STR    100    // Max size of passsert string

LOCAL char  passert_info_str[MAX_PASSERT_INFO_STR];

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

PUBLIC BOOLEAN SCI_IsAssert (void)
{
    //  return g_is_assert ;
    return FALSE;
}

LOCAL __inline void SCI_SetAssert (BOOLEAN mode)
{
    //  g_is_assert = mode;
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
#ifndef WIN32
    char    cmd_char;
    uint32  cur_sp;
    uint32  cur_lr;
    uint32  cur_pc;
    uint32  cur_cpsr;

    uint8   assert_mode;

    unsigned int interrupt_save;

    // Disable FIQ and IRQ bits in svc CPSR ;
    _tx_thread_interrupt_control (0xC0);
    // Disable chip interrupt ;
    CHIP_DisableFIQ ();

    SCI_EnterAssertMode();

    // Set mode.
    if (EXCEPTION_E != assert_type)
    {
        // Save register before assert.
        __asm
        {
            MOV cur_sp, r13
            MOV cur_lr, r14
            MOV cur_pc, r15
        }

        // Save all mode registers.
        __tx_save_cur_reg();

        cur_mode_reg.r13 = cur_sp;
        cur_mode_reg.r14 = cur_lr;
        cur_mode_reg.PC  = cur_pc;

        cur_cpsr = GetCPSR() & 0xf;

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

    SCI_SetAssert (SCI_TRUE);

    //@Steven
    //SEND_REQ_TO_DSP1(REF_CMD_ASSERT);
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;
    * (volatile uint32 *) (0x8b000010) |= 0x8;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }

    if (1 == cmd_ptr->RefCmd_block.cmd_done)
    {
        // dsp can reponse to FIQ
        dsp_is_run = 1;
    }
    else
    {
        // dsp can not reponse to FIQ and run away
        dsp_is_run = 0;
    }

    //@Steven

    b_is_passert    = SCI_FALSE;

    OSPARAM_GetAssertMode (&assert_mode);

    if (SCI_ASSERT_DEBUG_MODE == assert_mode)
    {
        OS_SendRemainLogMessage();
    }

    //@lin.liu(2004-09-22). CR13717
    // In release mode, don't record information to flash.
#ifndef _D_PH_R_
    // Debug mode

    // @lin.liu(2003-06-03)
    // Not support the EFS_Store_Assert_Info now.
    // store assert infor to flash.
    Record_Diagnostic_Info ( (const char *) file, line, (const char *) exp, NULL);


    if (SCI_ASSERT_RELEASE_MODE == assert_mode)
#endif //#ifdef _D_PH_R_    
    {
        /* @Xueliang.Wang change; CR:5041; (2003-10-07) */
        // Because it will set to RESET_MODE in function OS_Reset().

        /* @Xueliang.Wang added; CR5014; (2003-09-28) */
        /* @Jim.zhang CR:MS11459 2004-07-28 */

        LCD_PointerSaveInAssertResetMode();
        SCI_BeforeAssertReset (POWER_GetRestartCondition());

        /* @Jim.zhang CR:MS13387 2004-09-11 */
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

        /* end CR:MS13387 */

        //OS_Reset();

        /* end CR:MS11459 */
        /* End; @Xueliang.Wang change; CR:5041; (2003-10-07) */

        while (1);
    }

    //#endif

    // Code below is for noraml assertion without showing the function call histroy.
    // This code is good for debugging using lauterbach.


    // Code below is for assertion showing a menu for more debugging information
    // retrieving functions. Legel tool needs to support it.
    // Write dump data here to clear data before.
    // Ensure the next frame data can be received properly.
    // We have to send two frames to ensure channel Server can get the last one!
    OS_DumpPrint (SCI_NULL, 0);
    OS_DumpPrint (SCI_NULL, 0);

    // Print assert information to SIO and LCD.
    PrintAssertInfo (exp, file, line);
    PrintUserRegInfo();
    SCI_DumpCurThreadInfo();

    // Print menu info.
    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));

    while (1)
    {
        GetALine (cmd_buf);

        if (strlen (cmd_buf) == 1)
        {
            cmd_char = cmd_buf[0];
        }
        else
        {
            OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
            continue;
        }

        switch (cmd_char)
        {
            case '0':   // Help
                    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
                break;
            case '1':
                    PrintAssertInfo (exp, file, line);
                SCI_DumpCurThreadInfo();
                break;
            case '2':
                    PrintUserRegInfo();
                break;
            case '3':
                    // Memory dump.
                    OS_DumpMemory();
                break;
            case '4':
                    SCI_DumpAllocatedMemInfo();
                break;
            case '5':
                    SCI_DumpPoolInfo();
                break;
            case '6':
                    PrintThreadInfo();
                break;
            case '7':
                    PrintStackInfo();
                break;
            case '8':
                    PrintCallbackFuncList();
                break;
            case '9':
                    ReadMemory();
                break;
            case 'a':
                    PrintTaskTimeList();
                break;
            case 'f':
                    OS_Reset();
            default:
                    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
                break;
        }

        OS_DumpFlush();
    }

    //SCI_RestoreIRQ();

#else // WIN32 Mode

    // We disable interrupt here simply to avoid two ASSERT occurs,
    // cause in Visual C++ platform, different ASSERT may occurs in different threads!
    // We just care the first ASSERT more.
    SCI_DisableIRQ();

    _assert (exp, file, line);

    SCI_RestoreIRQ();

#endif // End of WIN32

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
#ifndef WIN32
    char    cmd_char;
    //uint32    i;
    uint32  cur_sp  = 0;
    uint32  cur_lr  = 0;
    uint32  cur_pc  = 0;
    uint32  cur_cpsr;

    uint8   assert_mode;

    // Disable FIQ and IRQ bits in svc CPSR ;
    _tx_thread_interrupt_control (0xC0);
    // Disable chip interrupt ;
    CHIP_DisableFIQ ();

    // Add disable IRQ here. Timer irq, ...
    {
        TX_INTERRUPT_SAVE_AREA
        TX_DISABLE
    }

    SCI_EnterAssertMode();

    // Set mode.
    if (EXCEPTION_E != assert_type)
    {
        // Save register before assert.
        __asm
        {
            MOV cur_sp, r13
            MOV cur_lr, r14
            MOV cur_pc, r15
        }

        // Save all mode registers.
        __tx_save_cur_reg();

        cur_mode_reg.r13 = cur_sp;
        cur_mode_reg.r14 = cur_lr;
        cur_mode_reg.PC  = cur_pc;

        cur_cpsr = GetCPSR() & 0xf;

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

    SCI_SetAssert (SCI_TRUE);
    //@Steven
    //SEND_REQ_TO_DSP1(REF_CMD_ASSERT);
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;
    * (volatile uint32 *) (0x8b000010) |= 0x8;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }

    if (1 == cmd_ptr->RefCmd_block.cmd_done)
    {
        // dsp can reponse to FIQ
        dsp_is_run = 1;
    }
    else
    {
        // dsp can not reponse to FIQ and run away
        dsp_is_run = 0;
    }

    //@Steven

    b_is_passert    = SCI_TRUE;
    // Delay for a while to print out information.
    OSPARAM_GetAssertMode (&assert_mode);

    if (SCI_ASSERT_DEBUG_MODE == assert_mode)
    {
        OS_SendRemainLogMessage();
    }

    //#endif

    //@lin.liu(2004-09-22). CR13717
    // In release mode, don't record information to flash.
#ifndef _D_PH_R_
    // Debug mode

    // @lin.liu(2003-05-20)
    //EFS_Store_Assert_Info( file, line, exp );

    // @lin.liu(2003-06-03)
    // Not support the EFS_Store_Assert_Info now.
    // store assert infor to flash.
    Record_Diagnostic_Info ( (const char *) file, line, (const char *) exp,
                             (const char *) assert_info_ptr);
    // Code below is for noraml assertion without showing the function call histroy.
    // This code is good for debugging using lauterbach.

    if (SCI_ASSERT_RELEASE_MODE == assert_mode)
#endif // _D_PH_R_    
    {
        /* @Xueliang.Wang change; CR:5041; (2003-10-07) */
        // Because it will set to RESET_MODE in function OS_Reset().

        /* @Xueliang.Wang added; CR5014; (2003-09-28) */
        /* @Jim.zhang CR:MS11459 2004-07-28 */

        LCD_PointerSaveInAssertResetMode();
        SCI_BeforeAssertReset (POWER_GetRestartCondition());

        /* @Jim.zhang CR:MS13387 2004-09-11 */
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

        /* end CR:MS13387 */

        //OS_Reset();

        /* end CR:MS11459 */
        /* End; @Xueliang.Wang change; CR:5041; (2003-10-07) */

        while (1);
    }

    //#endif

    // Code below is for assertion showing a menu for more debugging information
    // retrieving functions. Legel tool needs to support it.
    // Write dump data here to clear data before.
    // Ensure the next frame data can be received properly.
    // We have to send two frames to ensure channel Server can get the last one!
    OS_DumpPrint (SCI_NULL, 0);
    OS_DumpPrint (SCI_NULL, 0);

    // Print assert information to SIO and LCD.
    PrintAssertInfo (exp, file, line);
    OS_DumpPrint (assert_info_ptr, strlen (assert_info_ptr));
    PrintUserRegInfo();
    SCI_DumpCurThreadInfo();

    // Print menu info.
    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));

    while (1)
    {
        GetALine (cmd_buf);

        if (strlen (cmd_buf) == 1)
        {
            cmd_char = cmd_buf[0];
        }
        else
        {
            OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
            continue;
        }

        switch (cmd_char)
        {
            case '0':   // Help
                    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
                break;
            case '1':
                    PrintAssertInfo (exp, file, line);
                SCI_DumpCurThreadInfo();
                break;
            case '2':
                    PrintUserRegInfo();
                break;
            case '3':
                    // Memory dump.
                    OS_DumpMemory();
                break;
            case '4':
                    SCI_DumpAllocatedMemInfo();
                break;
            case '5':
                    SCI_DumpPoolInfo();
                break;
            case '6':
                    PrintThreadInfo();
                break;
            case '7':
                    PrintStackInfo();
                break;
            case '8':
                    PrintCallbackFuncList();
                break;
            case '9':
                    ReadMemory();
                break;
            case 'a':
                    PrintTaskTimeList();
                break;
            case 'f':
                    OS_Reset();
            default:
                    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
                break;
        }

        OS_DumpFlush();

    }

    //SCI_RestoreIRQ();

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
#ifndef WIN32
    char    cmd_char;
    uint32  cur_sp;
    uint32  cur_lr;
    uint32  cur_pc;
    uint32  cur_cpsr;

    // Disable FIQ and IRQ bits in svc CPSR ;
    _tx_thread_interrupt_control (0xC0);
    // Disable chip interrupt ;
    CHIP_DisableFIQ ();

    // Add disable IRQ here. Timer irq, ...
    {
        TX_INTERRUPT_SAVE_AREA
        TX_DISABLE
    }

    SCI_EnterAssertMode();

    // Set mode.
    if (EXCEPTION_E != assert_type)
    {
        // Save register before assert.
        __asm
        {
            MOV cur_sp, r13
            MOV cur_lr, r14
            MOV cur_pc, r15
        }

        // Save all mode registers.
        __tx_save_cur_reg();

        cur_mode_reg.r13 = cur_sp;
        cur_mode_reg.r14 = cur_lr;
        cur_mode_reg.PC  = cur_pc;

        cur_cpsr = GetCPSR() & 0xf;

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

    //@Steven
    //SEND_REQ_TO_DSP1(REF_CMD_ASSERT);
    cmd_ptr->RefCmd_block.cmd_done = 0;
    cmd_ptr->RefCmd_block.Cmd_ID = REF_CMD_ASSERT;
    * (volatile uint32 *) (0x8b000010) |= 0x8;

    //Wait for DSP and scheduler wake up
    {
        volatile uint32 tick1 = SCI_GetTickCount();
        volatile uint32 tick2 = SCI_GetTickCount();

        while ( (tick2 - tick1) < 30)
        {
            tick2 = SCI_GetTickCount();
        }
    }

    if (1 == cmd_ptr->RefCmd_block.cmd_done)
    {
        // dsp can reponse to FIQ
        dsp_is_run = 1;
    }
    else
    {
        // dsp can not reponse to FIQ and run away
        dsp_is_run = 0;
    }

    //@Steven

    b_is_passert    = SCI_FALSE;

    OS_SendRemainLogMessage();

    // Code below is for noraml assertion without showing the function call histroy.
    // This code is good for debugging using lauterbach.


    // Code below is for assertion showing a menu for more debugging information
    // retrieving functions. Legel tool needs to support it.
    // Write dump data here to clear data before.
    // Ensure the next frame data can be received properly.
    // We have to send two frames to ensure channel Server can get the last one!
    OS_DumpPrint (SCI_NULL, 0);
    OS_DumpPrint (SCI_NULL, 0);

    // Print assert information to SIO and LCD.
    PrintAssertInfo (exp, file, line);
    PrintUserRegInfo();
    SCI_DumpCurThreadInfo();

    // Print menu info.
    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));

    while (1)
    {
        GetALine (cmd_buf);

        if (strlen (cmd_buf) == 1)
        {
            cmd_char = cmd_buf[0];
        }
        else
        {
            OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
            continue;
        }

        switch (cmd_char)
        {
            case '0':   // Help
                    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
                break;
            case '1':
                    PrintAssertInfo (exp, file, line);
                SCI_DumpCurThreadInfo();
                break;
            case '2':
                    PrintUserRegInfo();
                break;
            case '3':
                    // Memory dump.
                    OS_DumpMemory();
                break;
            case '4':
                    SCI_DumpAllocatedMemInfo();
                break;
            case '5':
                    SCI_DumpPoolInfo();
                break;
            case '6':
                    PrintThreadInfo();
                break;
            case '7':
                    PrintStackInfo();
                break;
            case '8':
                    PrintCallbackFuncList();
                break;
            case '9':
                    ReadMemory();
                break;
            case 'a':
                    PrintTaskTimeList();
                break;
            case 'f':
                    OS_Reset();
            default:
                    OS_DumpPrint ( (char *) debug_cmd_list, sizeof (debug_cmd_list));
                break;
        }

        OS_DumpFlush();

    }

    //SCI_RestoreIRQ();

#else // WIN32 Mode

    // We disable interrupt here simply to avoid two ASSERT occurs,
    // cause in Visual C++ platform, different ASSERT may occurs in different threads!
    // We just care the first ASSERT more.
    SCI_DisableIRQ();

    _assert (exp, file, line);

    SCI_RestoreIRQ();

#endif // End of WIN32

}

#ifndef WIN32
/*****************************************************************************/
//  Description:    The PrintAssertInfo function is used print assert
//                  information to SIO and LCD.
//                  File, Line, Assert expression.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL void PrintAssertInfo (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
)
{
    uint32  i;
    uint32  name_len;
    char    file_name[SCI_MAX_NAME_SIZE + 1];

    sprintf (str_buf, "\nCurrent Version: \n        %s\n", COMMON_GetProjectVersionInfo());
    OS_DumpPrint (str_buf, strlen (str_buf));

    // Print assert information to SIO.
    if (b_is_watchdog)
    {
        sprintf (str_buf, "\nPossible error: Loop forever at 0x%08lx\n%s\n", _tx_pc_val_before_irq, exp);
    }
    else if (EXCEPTION_E == assert_type)
    {
        sprintf (str_buf, "\nException at 0x%08lx\nASSERT(%s)\n", cur_mode_reg.r14, exp);
    }
    else
    {
        if (b_is_passert)
        {
            sprintf (str_buf, "\nFile:  %s\nLine:  %ld\nPASSERT(%s)\n", file, line, exp);
        }
        else
        {
            sprintf (str_buf, "\nFile:  %s\nLine:  %ld\nASSERT(%s)\n", file, line, exp);
        }
    }

    OS_DumpPrint (str_buf, strlen (str_buf));

    // Delete directory name string of the file.
    sprintf (str_buf, "%s", file);

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
        sprintf (str_buf, "Loop for ever at 0x%08lx%s", _tx_pc_val_before_irq, exp);
    }
    else if (EXCEPTION_E == assert_type)
    {
        // Here R14 should be the return address!
        sprintf (str_buf, "Exception at 0x%08lxASSERT(%s)", cur_mode_reg.r14, exp);
    }
    else
    {
        if (b_is_passert)
        {
            sprintf (str_buf, "File:%sLine:%ldPASSERT(%s)", file_name, line, exp);
        }
        else
        {
            sprintf (str_buf, "File:%sLine:%ldASSERT(%s)", file_name, line, exp);
        }

    }

    LCD_DisplayText (0, 0, str_buf, strlen (str_buf));
}

/*****************************************************************************/
//  Description:    The PrintUserRegInfo function is used to print values of
//                  user registers.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void PrintUserRegInfo()
{
    uint32  *reg_ptr;

    if (NORMAL_E == assert_type)
    {
        sprintf (str_buf, "\nCurrent status is SVC, below is the registers before assert:\r\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
    }
    else if (IRQ_E == assert_type)
    {
        sprintf (str_buf, "\nCurrent status is IRQ, below is the registers before assert:\r\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
    }
    else if (FIQ_E == assert_type)
    {
        sprintf (str_buf, "\nCurrent status is FIQ, below is the registers before assert:\r\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
    }
    else
    {
        sprintf (str_buf, "\nCurrent status is exception, below is the registers before Exception:");
        OS_DumpPrint (str_buf, strlen (str_buf));

    }

    // First write current mode registers
    reg_ptr = (uint32 *) (&cur_mode_reg);

    sprintf (str_buf, "Current mode:\r\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    sprintf (str_buf,
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
    OS_DumpPrint (str_buf, strlen (str_buf));

    // Second write SVC mode registers
    reg_ptr = (uint32 *) (&svc_mode_reg);
    sprintf (str_buf, "SVC mode:\r\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    sprintf (str_buf,
             "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
             *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));
    OS_DumpPrint (str_buf, strlen (str_buf));

    // Third write IRQ mode registers
    reg_ptr = (uint32 *) (&irq_mode_reg);
    sprintf (str_buf, "IRQ mode:\r\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    sprintf (str_buf,
             "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
             *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));
    OS_DumpPrint (str_buf, strlen (str_buf));

    // Fourth write Abort mode registers
    reg_ptr = (uint32 *) (&abort_mode_reg);
    sprintf (str_buf, "Abort mode:\r\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    sprintf (str_buf,
             "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
             *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));
    OS_DumpPrint (str_buf, strlen (str_buf));

    // Fifth write Undefined mode registers
    reg_ptr = (uint32 *) (&undefine_mode_reg);
    sprintf (str_buf, "Undefined mode:\r\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    sprintf (str_buf,
             "\n\
        R13 = 0x%08lx    R14  = 0x%08lx\r\n\
        SPSR = 0x%08lx\r\n",
             *reg_ptr, * (reg_ptr + 1), * (reg_ptr + 2));
    OS_DumpPrint (str_buf, strlen (str_buf));
}

/*****************************************************************************/
//  Description:    The PrintThreadInfo function is used print all threads
//                  information, such as thread ID, name, queue.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void PrintThreadInfo (void)
{
    uint32          id;
    uint32          pc_val;
    uint32          status;
    void            *m_reg_ptr;
    char            task_name[16];
    TX_THREAD       *thread_ptr     = SCI_NULL;
    TX_THREAD       *start_ptr      = SCI_NULL;
    TX_QUEUE        *queue_ptr      = SCI_NULL;
    SCI_THREAD_T    *my_thread_ptr  = SCI_NULL;

    // Print static thread info.
    sprintf (str_buf, "\nTasks info:");
    OS_DumpPrint (str_buf, strlen (str_buf));
    sprintf (str_buf, "\n\tTask_ID    Name            Tcb_Addr    Current_PC  Queue_All   Queue_Avail");
    OS_DumpPrint (str_buf, strlen (str_buf));

    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID + 1; id++)
    {
        if (s_static_thread_ptr_table[id] != SCI_NULL)
        {
            my_thread_ptr   = s_static_thread_ptr_table[id];
            thread_ptr      = & (my_thread_ptr->thread);
            queue_ptr       = & (my_thread_ptr->queue);

            // Get thread name.
            if (strlen (thread_ptr->tx_thread_name) >= 15)
            {
                SCI_MEMCPY (task_name, thread_ptr->tx_thread_name, 15);
            }
            else
            {
                uint16  i;
                strcpy (task_name, thread_ptr->tx_thread_name);

                for (i = strlen (task_name); i < 15; i++)
                {
                    task_name[i] = ' ';
                }
            }

            task_name[15] = 0;


            // Get PC value.
            if (thread_ptr == tx_thread_identify())
            {
                sprintf (str_buf, "\n********0x%-8lx %s 0x%08lx", id, task_name, (uint32) my_thread_ptr);
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

                sprintf (str_buf, "\n\t0x%-8lx %s 0x%08lx  0x%08lx  %-10ld  %-10ld",
                         id, task_name, (uint32) my_thread_ptr, pc_val,
                         (uint32) queue_ptr->tx_queue_capacity,
                         (uint32) queue_ptr->tx_queue_available_storage);
            }

            OS_DumpPrint (str_buf, strlen (str_buf));
        }
    }

    // Print dynamic thread info.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID + 1; id++)
    {
        if (s_static_thread_ptr_table[id] != SCI_NULL)
        {
            break;
        }
    }

    start_ptr   = & (s_static_thread_ptr_table[id]->thread);
    thread_ptr  = start_ptr->tx_created_next;

    while ( (thread_ptr != start_ptr) && (thread_ptr != SCI_NULL))
    {
        my_thread_ptr = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);
        queue_ptr       = & (my_thread_ptr->queue);

        if (my_thread_ptr->block_type == SCI_DYNAMIC_THREAD)
        {
            if (strlen (thread_ptr->tx_thread_name) >= 15)
            {
                SCI_MEMCPY (task_name, thread_ptr->tx_thread_name, 15);
            }
            else
            {
                uint16  i;
                strcpy (task_name, thread_ptr->tx_thread_name);

                for (i = strlen (task_name); i < 15; i++)
                {
                    task_name[i] = ' ';
                }
            }

            task_name[15] = 0;

            // Get PC value.
            if (thread_ptr == tx_thread_identify())
            {
                sprintf (str_buf, "\n********0x%-8lx %s 0x%08lx", id, task_name, (uint32) my_thread_ptr);
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

                sprintf (str_buf, "\n\t0x%-8lx %s 0x%08lx  0x%08lx  %-10ld  %-10ld",
                         my_thread_ptr->thread_id, task_name, (uint32) my_thread_ptr, pc_val,
                         (uint32) queue_ptr->tx_queue_capacity,
                         (uint32) queue_ptr->tx_queue_available_storage);
            }

            OS_DumpPrint (str_buf, strlen (str_buf));
        }

        thread_ptr  = thread_ptr->tx_created_next;
    }
}

/*****************************************************************************/
//  Description:    The PrintStackInfo function is used print all threads
//                  stack information.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void PrintStackInfo (void)
{
    uint32          id;
    uint32          max_used_size;
    uint32          cur_stack;
    uint32          *temp_ptr;
    char            task_name[16];
    TX_THREAD       *thread_ptr     = SCI_NULL;
    TX_THREAD       *start_ptr      = SCI_NULL;
    SCI_THREAD_T    *my_thread_ptr  = SCI_NULL;

    // Print static thread info.
    sprintf (str_buf, "\nStack info:");
    OS_DumpPrint (str_buf, strlen (str_buf));
    sprintf (str_buf, "\n\tTask_ID    Name            TotalSize  Max_Used   Available  Cur_Ptr    Start      End        ");
    OS_DumpPrint (str_buf, strlen (str_buf));

    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID + 1; id++)
    {
        if (s_static_thread_ptr_table[id] != SCI_NULL)
        {
            my_thread_ptr   = s_static_thread_ptr_table[id];
            thread_ptr      = & (my_thread_ptr->thread);

            // Get thread name.
            if (strlen (thread_ptr->tx_thread_name) >= 15)
            {
                SCI_MEMCPY (task_name, thread_ptr->tx_thread_name, 15);
            }
            else
            {
                uint16  i;
                strcpy (task_name, thread_ptr->tx_thread_name);

                for (i = strlen (task_name); i < 15; i++)
                {
                    task_name[i] = ' ';
                }
            }

            task_name[15] = 0;

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

                sprintf (str_buf, "\n\t0x%-8lx %s %-10ld %-10ld %-10ld 0x%08lx 0x%08lx 0x%08lx",
                         my_thread_ptr->thread_id,
                         task_name,
                         (uint32) thread_ptr->tx_stack_size,
                         max_used_size,
                         (uint32) cur_stack - (uint32) thread_ptr->tx_stack_start,
                         (uint32) cur_stack,
                         (uint32) thread_ptr->tx_stack_start,
                         (uint32) thread_ptr->tx_stack_end);
                OS_DumpPrint (str_buf, strlen (str_buf));
            }

        }
    }

    // Print dynamic thread info.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID + 1; id++)
    {
        if (s_static_thread_ptr_table[id] != SCI_NULL)
        {
            break;
        }
    }

    start_ptr   = & (s_static_thread_ptr_table[id]->thread);
    thread_ptr  = start_ptr->tx_created_next;

    while ( (thread_ptr != start_ptr) && (thread_ptr != SCI_NULL))
    {
        my_thread_ptr = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

        if (my_thread_ptr->block_type == SCI_DYNAMIC_THREAD)
        {
            // Get thread name.
            if (strlen (thread_ptr->tx_thread_name) >= 15)
            {
                SCI_MEMCPY (task_name, thread_ptr->tx_thread_name, 15);
            }
            else
            {
                uint16  i;
                strcpy (task_name, thread_ptr->tx_thread_name);

                for (i = strlen (task_name); i < 15; i++)
                {
                    task_name[i] = ' ';
                }
            }

            task_name[15] = 0;

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

                sprintf (str_buf, "\n\t0x%-8lx %s %-10ld %-10ld %-10ld 0x%08lx 0x%08lx 0x%08lx",
                         id, task_name,
                         (uint32) thread_ptr->tx_stack_size,
                         max_used_size,
                         (uint32) cur_stack - (uint32) thread_ptr->tx_stack_start,
                         (uint32) cur_stack,
                         (uint32) thread_ptr->tx_stack_start,
                         (uint32) thread_ptr->tx_stack_end);
                OS_DumpPrint (str_buf, strlen (str_buf));
            }
        }

        thread_ptr  = thread_ptr->tx_created_next;
    }
}


/*****************************************************************************/
//  Description:    The SCI_DumpCurThreadInfo function is used print current
//                  thread information, such as thread ID, name, stack, queue.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_DumpCurThreadInfo (void)
{
    TX_THREAD       *thread_ptr     = SCI_NULL;
    TX_QUEUE        *queue_ptr      = SCI_NULL;
    SCI_THREAD_T    *my_thread_ptr  = SCI_NULL;
    uint32          tmp_sp          = cur_mode_reg.r13;

    if ( (IRQ_E == assert_type) && (SCI_FALSE == b_is_watchdog))
    {
        sprintf (str_buf, "\n!!! It is in IRQ !!!");
        OS_DumpPrint (str_buf, strlen (str_buf));
        sprintf (str_buf, "\nBefore enter IRQ, PC = 0x%08lx:\n", _tx_pc_val_before_irq);
        OS_DumpPrint (str_buf, strlen (str_buf));
    }
    //else if((EXCEPTION_E == assert_type) && ((tmp_sp >= irq_stack_start) && (tmp_sp <= irq_stack_end)))
    else if ( (EXCEPTION_E == assert_type) && ( (GetSPSR() & 0xf) == MCU_IRQ_MODE))
    {
        sprintf (str_buf, "\n!!! It is in IRQ before exception!!! PC = 0x%08lx", cur_mode_reg.r14);
        OS_DumpPrint (str_buf, strlen (str_buf));

        sprintf (str_buf, "\nBefore enter IRQ, PC = 0x%08lx:\n", _tx_pc_val_before_irq);
        OS_DumpPrint (str_buf, strlen (str_buf));
    }
    else if ( (FIQ_E == assert_type) && ( (GetSPSR() & 0xf) == MCU_IRQ_MODE))
    {
        sprintf (str_buf, "\n!!! It is in IRQ before FIQ!!! PC = 0x%08lx", _tx_pc_val_before_fiq);
        OS_DumpPrint (str_buf, strlen (str_buf));

        sprintf (str_buf, "\nBefore enter IRQ, PC = 0x%08lx:\n", _tx_pc_val_before_irq);
        OS_DumpPrint (str_buf, strlen (str_buf));
    }
    else
    {
        /*
                if (b_is_watchdog)
                {
                    sprintf(str_buf, "\nWatch Dog Timer Expired.\nPossible error: Loop for ever at 0x%08x", _tx_pc_val_before_irq);
                    OS_DumpPrint(str_buf, strlen(str_buf));
                }
        */
        sprintf (str_buf, "\nCurrent thread info:");
        OS_DumpPrint (str_buf, strlen (str_buf));

        thread_ptr = tx_thread_identify();

        if (SCI_NULL == thread_ptr)
        {
            sprintf (str_buf, "\n!!! None thread !!! \n");
            OS_DumpPrint (str_buf, strlen (str_buf));
        }
        else
        {
            // User thread.
            my_thread_ptr = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

            if ( (my_thread_ptr->block_type == SCI_STATIC_THREAD)
                    || (my_thread_ptr->block_type == SCI_DYNAMIC_THREAD))
            {
                // Static or Dynamic Thread.
                queue_ptr = & (my_thread_ptr->queue);
                sprintf (str_buf,
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
                OS_DumpPrint (str_buf, strlen (str_buf));
            }
            else
            {
                // Other thread.
                sprintf (str_buf,
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
                OS_DumpPrint (str_buf, strlen (str_buf));
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
LOCAL void ReadMemory (void)
{
    uint32  i;
    uint32  addr;
    uint32  size;
    uint32  status;

    sprintf (str_buf, "\nRead memory:\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    __readmem_label:

    while (1)
    {
        sprintf (str_buf, "Input address:\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
        OS_DumpFlush();

        GetALine (cmd_buf);
        status = GetNumFromStr (&addr, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            addr &= (~0x3);
            break;
        }
        else
        {
            sprintf (str_buf, "\t !!! Inputted address is invalid !!!\n");
            OS_DumpPrint (str_buf, strlen (str_buf));
            continue;
        }
    }

    while (1)
    {
        sprintf (str_buf, "Input size(0< size < 1024):\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
        OS_DumpFlush();

        GetALine (cmd_buf);
        status = GetNumFromStr (&size, cmd_buf);

        if (SCI_SUCCESS == status)
        {
            size = (size & (~0x3)) + 0x4;
            break;
        }
        else
        {
            sprintf (str_buf, "\t !!! Inputted size is invalid !!!\n");
            OS_DumpPrint (str_buf, strlen (str_buf));
            continue;
        }
    }

    // Check if address and size are valid.
    status = CheckMemAccessParam (addr, size);

    if (SCI_SUCCESS == status)
    {
        // Print memory.
        sprintf (str_buf, "Read Memory Start Address: 0x%08lx    Size: 0x%08lx\n", (uint32) addr, size);
        OS_DumpPrint (str_buf, strlen (str_buf));

        if ( (addr < 0x10000000) || (addr >= 0x20000000))
        {
            // Print dword by dword.

            uint32 *val_ptr = (uint32 *) addr;

            for (i = 0; i < size; i += 0x10)
            {
                sprintf (str_buf, "\t0x%p    0x%08lx 0x%08lx 0x%08lx 0x%08lx\n",
                         val_ptr, *val_ptr, * (val_ptr + 1), * (val_ptr + 2), * (val_ptr + 3));
                OS_DumpPrint (str_buf, strlen (str_buf));
                val_ptr += 4;
            }
        }
        else
        {
            // Print word by word.

            uint16 *val_ptr = (uint16 *) addr;

            for (i = 0; i < size; i += 0x10)
            {
                sprintf (str_buf, "\t0x%p    0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
                         val_ptr, *val_ptr, * (val_ptr + 1), * (val_ptr + 2), * (val_ptr + 3),
                         * (val_ptr + 4), * (val_ptr + 5), * (val_ptr + 6), * (val_ptr + 7));
                OS_DumpPrint (str_buf, strlen (str_buf));
                val_ptr += 8;
            }
        }

        OS_DumpFlush();
    }
    else
    {
        // Address and size are invalid.
        sprintf (str_buf, "\n\t !!! Address or size is invalid !!!!\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
        OS_DumpFlush();

        goto __readmem_label;
    }
}

LOCAL void GetALine (char *str)
{
    char    ch;
    uint32  i;

    // Get command string.
    for (i = 0; i < COMMAND_MAX_SIZE; i++)
    {
        ch = OS_DebugGetChar();

        if (ch != '\n')
        {
            * (str + i) = ch;
        }
        else
        {
            break;
        }
    }

    * (str + i) = '\0';
}

LOCAL uint32 GetNumFromStr (uint32 *value_ptr, char *src_ptr)
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
LOCAL uint32 CheckMemAccessParam (
    uint32  start_addr,     // Access start address.
    uint32  size            // Size to be access.
)
{
    uint32  status          = 0;
    uint32  end_addr        = 0;
    uint32  size_in_byte    = size;

    end_addr = start_addr + size_in_byte;

    // Check access mode(BYTE/WORD/DWORD) and Read/Write.
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
        status = SCI_ERROR;
    }

    return status;
}

/*****************************************************************************/
//  Description:    The PrintCallbackFuncList function is used print Callback
//                  information of the system.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL void PrintCallbackFuncList (void)
{
    int32       id;
    TX_THREAD   *start_ptr;
    TX_THREAD   *thread_ptr;
    SCI_THREAD_T    *thread_block   = SCI_NULL;

    sprintf (str_buf, "\nAll Task Callback Function List:\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    // Print current thread callback.
    PrintCurThreadCallbackFuncList();

    // Print other static thread callback.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID; id++)
    {
        if (SCI_NULL != s_static_thread_ptr_table[id])
        {
            GetStackBacktrace (id);
        }
    }

    // Print dynamic thread callback.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID + 1; id++)
    {
        if (s_static_thread_ptr_table[id] != SCI_NULL)
        {
            break;
        }
    }

    start_ptr   = & (s_static_thread_ptr_table[id]->thread);
    thread_ptr  = start_ptr->tx_created_next;

    while ( (thread_ptr != start_ptr) && (thread_ptr != SCI_NULL))
    {
        thread_block = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

        if (thread_block->block_type == SCI_DYNAMIC_THREAD)
        {
            GetStackBacktrace (thread_block->thread_id);
        }

        thread_ptr  = thread_ptr->tx_created_next;
    }
}

LOCAL void _trace_callback_list (void)
{
    uint32 status;

    do
    {
        status = GetCallbackFunAddr (&last_sp, &last_lr, &func_entry);

        if (SCI_SUCCESS != status)
        {
            break;
        }

        sprintf (str_buf, "\tEntry at [0x%-8lx]\n", (uint32) func_entry);
        OS_DumpPrint (str_buf, strlen (str_buf));

        if (last_sp >= OS_GetSramEndAddr())
        {
            break;
        }
    }
    while (1);

    if (SCI_ERROR == status)
    {
        sprintf (str_buf, "\t......\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
    }

    sprintf (str_buf, "***Reach top of stack.***\r\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

}

/*****************************************************************************/
//  Description:    The PrintCurThreadCallbackFuncList function is used print
//                  Callback information of the system.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL void PrintCurThreadCallbackFuncList (void)
{
    uint32      status;
    TX_THREAD   *thread_ptr;
    SCI_THREAD_T    *thread_block   = SCI_NULL;
    uint32          tmp_sp          = cur_mode_reg.r13;

    if (IRQ_E == assert_type)
    {
        // Cuerrent case is an IRQ
        if (b_is_watchdog)
        {
            sprintf (str_buf, "\nWatch Dog Timer Expired.\nPossible error: Loop for ever at 0x%08lx", _tx_pc_val_before_irq);
            OS_DumpPrint (str_buf, strlen (str_buf));
        }
        else
        {
            // Print IRQ handle callback list.
            sprintf (str_buf, "\nIRQ handle callback list:\n");
            OS_DumpPrint (str_buf, strlen (str_buf));

            last_sp = cur_mode_reg.r13;
            last_lr = cur_mode_reg.PC;
            _trace_callback_list();

            sprintf (str_buf, "\nCallback of thread interrupted by IRQ:");
            OS_DumpPrint (str_buf, strlen (str_buf));
        }

        s_is_current_thread_saved = SCI_TRUE;
    }
    else if (EXCEPTION_E == assert_type)
    {
        // check if is an IRQ
        if ( (GetSPSR() & 0xf) == MCU_IRQ_MODE)
        {
            // Print IRQ handle callback list.
            sprintf (str_buf, "\nIRQ handle callback list:\n");
            OS_DumpPrint (str_buf, strlen (str_buf));

            last_sp = irq_mode_reg.r13;
            last_lr = cur_mode_reg.r14;
            _trace_callback_list();

            sprintf (str_buf, "\nCallback of thread interrupted by IRQ:");
            OS_DumpPrint (str_buf, strlen (str_buf));

            s_is_current_thread_saved = SCI_TRUE;
        }
        else // Normal thread
        {
            // Print IRQ handle callback list.
            sprintf (str_buf, "\nCurrent thread callback list:\n");
            OS_DumpPrint (str_buf, strlen (str_buf));

            last_sp = svc_mode_reg.r13;
            last_lr = cur_mode_reg.r14;
            _trace_callback_list();
        }
    }
    else if (NORMAL_E == assert_type)
    {
        // Get current thread info.
        thread_ptr = tx_thread_identify();

        if (SCI_NULL != thread_ptr)
        {
            last_sp = cur_mode_reg.r13;
            last_lr = cur_mode_reg.PC;

            // Print current thread name.
            // And print the ID if it is user thread.
            thread_block = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

            if ( (thread_block->block_type == SCI_STATIC_THREAD)
                    || (thread_block->block_type == SCI_DYNAMIC_THREAD))
            {
                sprintf (str_buf, "\nCurrent Task %ld (%s):\n", thread_block->thread_id, thread_ptr->tx_thread_name);
            }
            else
            {
                sprintf (str_buf, "\nCurrent Task %s:\n", thread_ptr->tx_thread_name);
            }

            OS_DumpPrint (str_buf, strlen (str_buf));

            _trace_callback_list();
        }
    }
    else if (FIQ_E == assert_type)
    {
        if ( (GetSPSR() & 0xf) == MCU_IRQ_MODE)
        {
            // Print IRQ handle callback list.
            sprintf (str_buf, "\nIRQ handle callback list:\n");
            OS_DumpPrint (str_buf, strlen (str_buf));

            last_sp = irq_mode_reg.r13;
            last_lr = _tx_pc_val_before_fiq;
            _trace_callback_list();

            sprintf (str_buf, "\nCallback of thread interrupted by IRQ:");
            OS_DumpPrint (str_buf, strlen (str_buf));

            s_is_current_thread_saved = SCI_TRUE;
        }
        else // Normal thread
        {
            // Print IRQ handle callback list.
            sprintf (str_buf, "\nCurrent thread callback list:\n");
            OS_DumpPrint (str_buf, strlen (str_buf));

            last_sp = svc_mode_reg.r13;
            last_lr = _tx_pc_val_before_fiq;
            _trace_callback_list();
        }
    }
}


/*****************************************************************************/
//  Description:    The GetStackBackTrace function is used to search and
//                  print a thread's satck callback function information.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetStackBacktrace (
    BLOCK_ID thread_id            // Thread ID
)
{
    uint32      status;
    BLOCK_ID    current_id;
    void        *m_reg_ptr;
    SCI_THREAD_T    *thread_block   = SCI_NULL;

    current_id = SCI_IdentifyThread();

    if (current_id == thread_id && !s_is_current_thread_saved)
    {
        return SCI_SUCCESS;
    }

    // Check if it is a valid task!
    thread_block = (SCI_THREAD_T *) SCI_GetThreadBlockFromId (thread_id);

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

    sprintf (str_buf, "\nTask %ld (%s):\n", thread_id, ( (TX_THREAD *) &thread_block->thread)->tx_thread_name);
    OS_DumpPrint (str_buf, strlen (str_buf));

    status = GetCallbackFunAddr (&last_sp, &last_lr, &func_entry);

    while (status == SCI_SUCCESS)
    {
        sprintf (str_buf, "\tEntry at [0x%08lx]\n", func_entry);
        OS_DumpPrint (str_buf, strlen (str_buf));

        // Reach top of the stack!
        if (last_sp >= (uint32) ( (TX_THREAD *) &thread_block->thread)->tx_stack_end - 8)
        {
            break;
        }

        status = GetCallbackFunAddr (&last_sp, &last_lr, &func_entry);
    }

    if (SCI_ERROR == status)
    {
        sprintf (str_buf, "\t......\n");
        OS_DumpPrint (str_buf, strlen (str_buf));
    }

    //sprintf(str_buf, "***Reach top of stack at 0x%08lx***\n",
    //    (uint32)((TX_THREAD *)&thread_block->thread)->tx_stack_end);
    sprintf (str_buf, "***Reach top of stack ***\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    return 0;
}

/*****************************************************************************/
//  Description:    The GetCallbackFunAddr function is used to get the
//                  function address called in the stack.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCallbackFunAddr (uint32 *sp, uint32 *lr, uint32 *fun)
{
    uint16  loccal_var_size = 0;    // stack size allocated for local variable
    uint16  saved_reg_size  = 0;    // stack size for saving registers, last one is lr(r14)
    uint16  *pc_ptr         = (uint16 *) *lr;

    // Warning: The code below here can only work for THUMB code!
    while (1)
    {
        // PC pointer reached the top of the program!
        if ( (uint32) pc_ptr <= TOP_OF_PROGRAM)
        {
            return SCI_PARAM_ERROR;
        }

        if ( (uint32) pc_ptr >= OS_GetSramEndAddr())
        {
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

        /* Richard add here for pop operation!*/
        //      if(( *pc_ptr & POP_SP_CODE_MASK ) == POP_SP_CODE)
        //      {
        //          uint16 reg_list;

        //          reg_list = *pc_ptr & (~POP_SP_CODE_MASK);
        //          // Calculate number of registers are push in stack
        //          for(i = 0; i < SIZE_OF_REG_LIST; i++)
        //          {
        //              if( reg_list &( 1 << i))
        //                  loccal_var_size++;
        //          }
        //          pc_ptr--;
        //      }
        /* Richard add here for pop operation!*/

        /* PUSH operator! */
        if ( (*pc_ptr & THUMB_PUSH_CODE_MASK) == THUMB_PUSH_CODE)
        {
            uint16 reg_list;
            uint16 i;

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

        pc_ptr--;
    }

    // lr is the last one in stack after push(r14..) has been executed
    *sp += (loccal_var_size + saved_reg_size) * 4;

    if ( (*sp >= OS_GetSramEndAddr()) || (* (uint32 *) (*sp - 4) >= OS_GetFlashEndAddr()))
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


/*****************************************************************************/
//  Description:    The SCI_HandleUndefined function is used handle Prefetch.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_HandleUndefined (void)
{
    assert_type = EXCEPTION_E;
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


LOCAL uint32 GetSPSR (void)
{
    return (uint32) * ( ( (uint32 *) &cur_mode_reg) + 16);
}

LOCAL uint32 GetCPSR (void)
{
    return (uint32) * ( ( (uint32 *) &cur_mode_reg) + 17);
}

#endif // End of WIN32

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

LOCAL void CalulateTotalTime (void)
{
#ifdef DS_LOADING_TRACK
    int32       id;
    TX_THREAD   *start_ptr;
    TX_THREAD   *thread_ptr;
    SCI_THREAD_T    *thread_block   = SCI_NULL;


    // Print other static thread callback.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID; id++)
    {
        if (SCI_NULL != s_static_thread_ptr_table[id])
        {
            //sprintf(str_buf, "\nTask %s:\n", s_static_thread_ptr_table[id]->thread.tx_thread_name);
            //OS_DumpPrint(str_buf, strlen(str_buf));
            //sprintf(str_buf, "\ntime : %d\n", s_static_thread_ptr_table[id]->thread.time);
            //OS_DumpPrint(str_buf, strlen(str_buf));

            excute_time += s_static_thread_ptr_table[id]->thread.time;
            //GetStackBacktrace(id);
        }
    }

    // Print dynamic thread callback.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID + 1; id++)
    {
        if (s_static_thread_ptr_table[id] != SCI_NULL)
        {
            break;
        }
    }

    start_ptr   = & (s_static_thread_ptr_table[id]->thread);
    thread_ptr  = start_ptr->tx_created_next;

    while ( (thread_ptr != start_ptr) && (thread_ptr != SCI_NULL))
    {
        thread_block = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

        if (thread_block->block_type == SCI_DYNAMIC_THREAD)
        {
            //sprintf(str_buf, "\nTask %s:\n", thread_ptr->tx_thread_name);
            //OS_DumpPrint(str_buf, strlen(str_buf));
            //sprintf(str_buf, "\ntime : %d\n", thread_ptr->time);
            //OS_DumpPrint(str_buf, strlen(str_buf));

            excute_time += thread_ptr->time;
        }

        thread_ptr  = thread_ptr->tx_created_next;
    }


    //sprintf(str_buf, "\nTask uart: %d\n", uart_time);
    //OS_DumpPrint(str_buf, strlen(str_buf));
    excute_time += uart_time;

    //sprintf(str_buf, "\nTask idle: %d\n", idle_time);
    //OS_DumpPrint(str_buf, strlen(str_buf));
    excute_time += idle_time;
    excute_time_div = excute_time/1000;

    //sprintf(str_buf, "\nTask total time: %d\n", excute_time);
    //OS_DumpPrint(str_buf, strlen(str_buf));
#endif
}

LOCAL void PrintTaskTimeList (void)
{
#ifdef DS_LOADING_TRACK
    int32       id;
    TX_THREAD   *start_ptr;
    TX_THREAD   *thread_ptr;
    SCI_THREAD_T    *thread_block   = SCI_NULL;

    sprintf (str_buf, "\nAll Task Function List:\n");
    OS_DumpPrint (str_buf, strlen (str_buf));

    // Print current thread callback.
    //   PrintCurThreadCallbackFuncList();
    CalulateTotalTime();

    // Print other static thread callback.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID; id++)
    {
        if (SCI_NULL != s_static_thread_ptr_table[id])
        {
            sprintf (str_buf, "\nTask %s:\n", s_static_thread_ptr_table[id]->thread.tx_thread_name);
            OS_DumpPrint (str_buf, strlen (str_buf));
            sprintf (str_buf, "\ntime : %d percent: %d\n", s_static_thread_ptr_table[id]->thread.time, ( (s_static_thread_ptr_table[id]->thread.time) /excute_time_div));
            OS_DumpPrint (str_buf, strlen (str_buf));

            //excute_time += s_static_thread_ptr_table[id]->thread.time;
            //GetStackBacktrace(id);
        }
    }

    // Print dynamic thread callback.
    for (id = 0; id < SCI_MAX_STATIC_THREAD_ID + 1; id++)
    {
        if (s_static_thread_ptr_table[id] != SCI_NULL)
        {
            break;
        }
    }

    start_ptr   = & (s_static_thread_ptr_table[id]->thread);
    thread_ptr  = start_ptr->tx_created_next;

    while ( (thread_ptr != start_ptr) && (thread_ptr != SCI_NULL))
    {
        thread_block = (SCI_THREAD_T *) ( (uint32) thread_ptr - SCI_THREAD_GAP);

        if (thread_block->block_type == SCI_DYNAMIC_THREAD)
        {
            sprintf (str_buf, "\nTask %s:\n", thread_ptr->tx_thread_name);
            OS_DumpPrint (str_buf, strlen (str_buf));
            sprintf (str_buf, "\ntime : %d percent: %d\n", thread_ptr->time, (thread_ptr->time) /excute_time_div);
            OS_DumpPrint (str_buf, strlen (str_buf));

            //excute_time += thread_ptr->time;
        }

        thread_ptr  = thread_ptr->tx_created_next;
    }


    sprintf (str_buf, "\nTask uart: %d percent: %d\n", uart_time, (uart_time) /excute_time_div);
    OS_DumpPrint (str_buf, strlen (str_buf));
    //excute_time += uart_time;

    sprintf (str_buf, "\nTask idle: %d percent: %d\n", idle_time, (idle_time) /excute_time_div);
    OS_DumpPrint (str_buf, strlen (str_buf));
    //excute_time += idle_time;

    sprintf (str_buf, "\nTask total time: %ud\n", excute_time);
    OS_DumpPrint (str_buf, strlen (str_buf));
#endif
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // End of _DEBUG

