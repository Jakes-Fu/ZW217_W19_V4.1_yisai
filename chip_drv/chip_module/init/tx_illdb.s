;/**************************************************************************/ 
;/*                                                                        */ 
;/*            Copyright (c) 1996-2001 by Express Logic Inc.               */ 
;/*                                                                        */ 
;/*  This software is copyrighted by and is the sole property of Express   */ 
;/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
;/*  in the software remain the property of Express Logic, Inc.  This      */ 
;/*  software may only be used in accordance with the corresponding        */ 
;/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
;/*  distribution, or disclosure of this software is expressly forbidden.  */ 
;/*                                                                        */
;/*  This Copyright notice may not be removed or modified without prior    */ 
;/*  written consent of Express Logic, Inc.                                */ 
;/*                                                                        */ 
;/*  Express Logic, Inc. reserves the right to modify this software        */ 
;/*  without notice.                                                       */ 
;/*                                                                        */ 
;/*  Express Logic, Inc.                                                   */
;/*  11440 West Bernardo Court               info@expresslogic.com         */
;/*  Suite 366                               http://www.expresslogic.com   */
;/*  San Diego, CA  92127                                                  */
;/*                                                                        */
;/**************************************************************************/
;
;
;/**************************************************************************/
;/**************************************************************************/
;/**                                                                       */ 
;/** ThreadX Component                                                     */ 
;/**                                                                       */
;/**   Initialize (INI)                                                    */
;/**                                                                       */
;/**************************************************************************/
;/**************************************************************************/
;
;
;#define    TX_SOURCE_CODE
;
;
;/* Include necessary system files.  */
;
;#include   "tx_api.h"
;#include   "tx_ini.h"
;#include   "tx_thr.h"
;#include   "tx_tim.h"
;


FIQ_MODE        EQU     0xD1                ; Disable irq,fiq FIQ mode
IRQ_MODE        EQU     0xD2                ; Disable irq,fiq IRQ mode
SVC_MODE        EQU     0xD3                ; Disable irq,fiq SVC mode
UNDEFINE_MODE	EQU     0xDB                ; Disable irq,fiq UNDEFINE mode
ABORT_MODE      EQU     0xD7                ; Disable irq,fiq ABORT mode
SYS_MODE        EQU     0xDF                ; Disable irq,fiq SYSTEM mode

ABORT_HANDLE_MODE  EQU     0x93                  ;SVC Mode, IRQ Disable, FIQ Enable
    IF (:DEF:PLATFORM_UWS6121E)
SVC_Stack_Temp  EQU     0x80FFFFF0          ; SVC temporary stack
    ELSE
SVC_Stack_Temp  EQU     0x40002000          ; SVC temporary stack
    ENDIF
    
JTAG_REENABLE_CTL        EQU     0x8B000380
    
    IMPORT      ISR_Handler
    IMPORT      SCI_HandleUndefined
    IMPORT      SCI_HandleSwi
    IMPORT      SCI_HandlePrefetch
    IMPORT      SCI_HandleAbort
    IMPORT      SCI_HandleReserved
    IMPORT      __PureAssert
    IMPORT      __BOOT_ResetHandler
    IMPORT      __BOOT_IRAM_EN
    IMPORT      __BOOT_InitModule
    
    IF (:DEF:VM_SUPPORT)
    IMPORT      MMU_HandlePrefetch
    IMPORT      MMU_HandleAbort
    IMPORT  	_tx_pc_val_before_irq
    ENDIF
    
    IMPORT      _tx_thread_system_stack_ptr
    IMPORT      _tx_initialize_unused_memory
    IMPORT      _tx_thread_context_save
    IMPORT      _tx_thread_context_restore
    IMPORT      _tx_timer_interrupt
    IMPORT      _tx_timer_stack_start
    IMPORT      _tx_timer_stack_size
    IMPORT      _tx_timer_priority
    IMPORT      __main

    IF (:DEF:TF_LOAD_PROTECT)
    IMPORT TF_EnterInit
    ENDIF    

    IF (:DEF:FLASH_TYPE_SPI)
    IF (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UWS6121E)
    IMPORT SPIFLASH_SetCS0SpaceSize
    ELSE
    IMPORT SPIFLASH_SetCS1StartAddr
    ENDIF
    ENDIF

	IF (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UWS6121E)
	IMPORT CHIP_InitAdi
	ENDIF
;
;
;
	AREA  |C$$data|, DATA
|x$dataseg|

    EXPORT g_reg_data
g_reg_data
    EXPORT cur_mode_reg
cur_mode_reg
    DCD		00000000	; r0
    DCD		00000000	; r1
    DCD		00000000	; r2
    DCD		00000000	; r3
    DCD		00000000	; r4
    DCD		00000000	; r5
    DCD		00000000	; r6
    DCD		00000000	; r7
    DCD		00000000	; r8
    DCD		00000000	; r9
    DCD		00000000	; r10
    DCD		00000000	; r11
    DCD		00000000	; r12
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; pc
    DCD		00000000	; spsr
    DCD		00000000	; cpsr

	EXPORT irq_mode_reg
irq_mode_reg
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; spsr

    EXPORT svc_mode_reg
svc_mode_reg
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; spsr

    EXPORT undefine_mode_reg
undefine_mode_reg
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; spsr

    EXPORT abort_mode_reg
abort_mode_reg
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; spsr

    EXPORT fiq_mode_reg
fiq_mode_reg
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; spsr
    
cur_undefine_reg
    DCD		00000000	; r0
    DCD		00000000	; r1
    DCD		00000000	; r2
    DCD		00000000	; r3
    DCD		00000000	; r4
    DCD		00000000	; r5
    DCD		00000000	; r6
    DCD		00000000	; r7
    DCD		00000000	; r8
    DCD		00000000	; r9
    DCD		00000000	; r10
    DCD		00000000	; r11
    DCD		00000000	; r12
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; pc
    DCD		00000000	; spsr
    DCD		00000000	; cpsr
    DCD     0x5a5a5a5a  ; end
    
cur_Dabout_reg
    DCD		00000000	; r0
    DCD		00000000	; r1
    DCD		00000000	; r2
    DCD		00000000	; r3
    DCD		00000000	; r4
    DCD		00000000	; r5
    DCD		00000000	; r6
    DCD		00000000	; r7
    DCD		00000000	; r8
    DCD		00000000	; r9
    DCD		00000000	; r10
    DCD		00000000	; r11
    DCD		00000000	; r12
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; pc
    DCD		00000000	; spsr
    DCD		00000000	; cpsr
    DCD     0x5a5a5a5a    ; end
    
cur_Pabout_reg
    DCD		00000000	; r0
    DCD		00000000	; r1
    DCD		00000000	; r2
    DCD		00000000	; r3
    DCD		00000000	; r4
    DCD		00000000	; r5
    DCD		00000000	; r6
    DCD		00000000	; r7
    DCD		00000000	; r8
    DCD		00000000	; r9
    DCD		00000000	; r10
    DCD		00000000	; r11
    DCD		00000000	; r12
    DCD		00000000	; r13
    DCD		00000000	; r14
    DCD		00000000	; pc
    DCD		00000000	; spsr
    DCD		00000000	; cpsr
    DCD     0x5a5a5a5a    ; end
	
	EXPORT abort_DFSR_reg
abort_DFSR_reg
	DCD	    0	; Data fault status register

	EXPORT abort_IFSR_reg
abort_IFSR_reg
	DCD	    0	; Instruction fault status register	

	EXPORT abort_FAR_reg
abort_FAR_reg
	DCD		0	; Fault address register

	EXPORT	_tx_pc_val_before_fiq
_tx_pc_val_before_fiq
	DCD		0    
    EXPORT  _tx_r4_val_before_fiq
_tx_r4_val_before_fiq    
    DCD     0    

    IF (:DEF:XFILE_SUPPORT)
    EXPORT  xf_assert_write_end_flag
xf_assert_write_end_flag    
    DCD     0

    EXPORT  xf_start_sct
xf_start_sct
    DCD     0
	
	EXPORT xf_current_offset
xf_current_offset
	DCD     0
	
    EXPORT  xf_assert_write_flag
xf_assert_write_flag
    DCD     0

;    EXPORT  xf_trace_log_start_flag
;xf_trace_log_start_flag
;    DCD     0

;    EXPORT  xf_get_reset_reason
;xf_get_reset_reason
;    DCD     0
    ENDIF	
	
    IF (:DEF:PLATFORM_SC6530)
_tx_abort_stack_start         ; Start Address of Exception Handle Stack 
     DCD   0x40008000         
_tx_abort_stack_end           ; End Address of Exception Handle Stack 
     DCD   0x4000A000         
    ENDIF

	 IF (:DEF:PLATFORM_SC6531EFM) 
_tx_abort_stack_start         ; Start Address of Exception Handle Stack 
     DCD   0x40002000         
_tx_abort_stack_end           ; End Address of Exception Handle Stack 
     DCD   0x40002800         
    ENDIF

	 IF  (:DEF:PLATFORM_UWS6121E)
_tx_abort_stack_start         ; Start Address of Exception Handle Stack 
     DCD   0x00802000         
_tx_abort_stack_end           ; End Address of Exception Handle Stack 
     DCD   0x00802800         
    ENDIF
    
    IF (:DEF:VM_SUPPORT)
    EXPORT	_tx_r13_svc_mode
_tx_r13_svc_mode
    DCD		0           
	EXPORT	_tx_r14_before_exception
_tx_r14_before_exception
    DCD		0       
    EXPORT	_tx_abort_exception_magic
_tx_abort_exception_magic
    DCD		0xDEADDEAD 
    ENDIF
    
        PRESERVE8
    AREA  Init, CODE, READONLY
    CODE32
    ENTRY
;
;/* Define the ARM7 vector area.  This should be located or copied to 0.  */
;
    EXPORT  __vectors
__vectors
    LDR     pc,=__tx_reset                  ; Reset goes to startup function
    LDR     pc,=__tx_undefined              ; Undefined handler
    LDR     pc,=__tx_swi_interrupt          ; Software interrupt handler
    LDR     pc,=__tx_prefetch_handler       ; Prefetch exeception handler
    LDR     pc,=__tx_abort_handler          ; Abort exception handler
    LDR     pc,=__tx_reserved_handler       ; Reserved exception handler
    LDR     pc,=__tx_irq_handler            ; IRQ interrupt handler
    LDR     pc,=__tx_fiq_handler            ; FIQ interrupt handler

	IF (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UWS6121E)
	IF (:DEF:NOR_BOOTLOADER_SUPPORT)
    	ELSE
	;magic number
    DCD     0x36353632
	ENDIF    
	ENDIF 
	
        PRESERVE8
        AREA  |C$$code|, CODE, READONLY
        GET asm_map.h
|x$codeseg| DATA
;
;/**************************************************************************/ 
;/*                                                                        */ 
;/*  FUNCTION                                               RELEASE        */ 
;/*                                                                        */ 
;/*    _tx_initialize_low_level                            THUMB/ARM       */ 
;/*                                                           4.0          */ 
;/*  AUTHOR                                                                */ 
;/*                                                                        */ 
;/*    William E. Lamie, Express Logic, Inc.                               */ 
;/*                                                                        */ 
;/*  DESCRIPTION                                                           */ 
;/*                                                                        */ 
;/*    This function is responsible for any low-level processor            */ 
;/*    initialization, including setting up interrupt vectors, saving the  */ 
;/*    system stack pointer, finding the first available memory address,   */ 
;/*    and setting up parameters for the system's timer thread.            */ 
;/*                                                                        */ 
;/*  INPUT                                                                 */ 
;/*                                                                        */ 
;/*    None                                                                */ 
;/*                                                                        */ 
;/*  OUTPUT                                                                */ 
;/*                                                                        */ 
;/*    None                                                                */ 
;/*                                                                        */ 
;/*  CALLS                                                                 */ 
;/*                                                                        */ 
;/*    None                                                                */ 
;/*                                                                        */ 
;/*  CALLED BY                                                             */ 
;/*                                                                        */ 
;/*    _tx_initialize_kernel_enter           ThreadX entry function        */ 
;/*                                                                        */ 
;/*  RELEASE HISTORY                                                       */ 
;/*                                                                        */ 
;/*    DATE              NAME                      DESCRIPTION             */ 
;/*                                                                        */ 
;/*  06-20-1997     William E. Lamie         Initial Version 3.0           */ 
;/*  11-11-1997     William E. Lamie         Modified comment(s),          */ 
;/*                                            resulting in version 3.0b.  */ 
;/*  03-01-1998     William E. Lamie         Modified comment(s),          */ 
;/*                                            resulting in version 3.0d.  */ 
;/*  01-01-1999     William E. Lamie         Modified comment(s),          */ 
;/*                                            resulting in version 3.0e.  */ 
;/*  03-01-2000     William E. Lamie         Modified comments(s),         */
;/*                                            modified interrupt vector,  */ 
;/*                                            added code section,         */ 
;/*                                            removed code to bypass ARM  */ 
;/*                                            C library, and changed MSR  */ 
;/*                                            instructions to use the     */ 
;/*                                            flags modifier, resulting   */ 
;/*                                            in version 3.0f.            */ 
;/*  01-28-2001     William E. Lamie         Modified comment(s),          */ 
;/*                                            resulting in version 4.0.   */ 
;/*  03-08-2002     Richard.Yang             Modified Registers for        */ 
;/*                                            Spreadtrum ARM chip,        */ 
;/*                                            Initial Version 1.0.        */ 
;/*                                                                        */ 
;/**************************************************************************/ 
;VOID   _tx_initialize_low_level(VOID)
;{
    CODE16
    EXPORT  _tx_initialize_low_level
_tx_initialize_low_level
    ADR     a1, _tx_initialize_low_level_32 ; Build 32-bit entry point 
    BX      a1                              ; Switch to 32-bit instructions
    CODE32
_tx_initialize_low_level_32
;
;   /* We must be in SVC mode at this point!  */
;
    LDR     a4, [pc, #TXILLDB_FIQ_STACK-.-8];=|Image$$ZI$$Limit|         ; Get end of non-initialized RAM area
    LDR     a2, [a4, #0]                    ;Pick the value from memory pointer
;
;   /* Initialize FIQ mode */    22
    LDR     a4, [pc, #TXILLDB_FIQ_STACK_SIZE-.-8]   ; Pickup stack size
    LDR     a3, [a4, #0]                    ;Pick the value from memory pointer
    MOV     a1, #FIQ_MODE                   ; Build FIQ mode CPSR
    MSR     CPSR_fsxc, a1                   ; Enter FIQ mode
	MOV		lr, #0
	MOV		a1, #16
	MSR		SPSR_fsxc, a1		
    ADD     a2, a2, a3                      ; Calculate start of FIQ stack
    BIC     a2, a2, #7                      ; Ensure 8 byte alignment
    SUB     sp, a2, #8                      ; Backup 8 byte,  Setup FIQ stack pointer
    MOV     sl, #0                          ; Clear sl
    MOV     fp, #0                          ; Clear fp
;

    LDR     a4, [pc, #RAM_LIMIT-.-8];=|Image$$ZI$$Limit|         ; Get end of non-initialized RAM area
    LDR     a2, [a4, #0]      
    
;   /* Initialize IRQ mode */    
    LDR     a4, [pc, #TXILLDB_IRQ_STACK_SIZE-.-8]   ; Pickup IRQ stack
    LDR     a3, [a4, #0]                    ;Pick the value from memory pointer
    MOV     a1, #IRQ_MODE                   ; Build IRQ mode CPSR
    MSR     CPSR_fsxc, a1                      ; Enter IRQ mode
	MOV		lr, #0
	MOV		a1, #16
	MSR		SPSR_fsxc, a1		
    ADD     a2, a2, a3                      ; Calculate start of IRQ stack
    BIC     a2, a2, #7                      ; Ensure 8 byte alignment
    SUB     sp, a2, #8                      ; Backup 8 byte, Setup IRQ stack pointer

;
;   /* Initialize UNDEFINE mode */    
    LDR     a4, [pc, #TXILLDB_UNDEFINE_STACK_SIZE-.-8]   ; Pickup UNDEFINE stack
    LDR     a3, [a4, #0]                     ;Pick the value from memory pointer
    MOV     a1, #UNDEFINE_MODE              ; Build UNDEFINE mode CPSR
    MSR     CPSR_fsxc, a1                   ; Enter UNDEFINE mode
	MOV		lr, #0
	MOV		a1, #16
	MSR		SPSR_fsxc, a1		
    ADD     a2, a2, a3                      ; Calculate start of UNDEFINE stack
    BIC     a2, a2, #7                      ; Ensure 8 byte alignment
    SUB     sp, a2, #8                       ; Backup 8 byte, Setup UNDEFINE stack pointer
;
;   /* Initialize ABORT mode */    
    LDR     a4, [pc, #TXILLDB_ABORT_STACK_SIZE-.-8]   ; Pickup ABORT stack
    LDR     a3, [a4, #0]                    ;Pick the value from memory pointer
    MOV     a1, #ABORT_MODE              ; Build ABORT mode CPSR
    MSR     CPSR_fsxc, a1                   ; Enter ABORT mode
	MOV		lr, #0
	MOV		a1, #16
	MSR		SPSR_fsxc, a1		
    ADD     a2, a2, a3                      ; Calculate start of ABORT stack
    BIC     a2, a2, #7                      ; Ensure 8 byte alignment
    SUB     sp, a2, #8                      ; Backup 8 byte, Setup ABORT stack pointer
;
;   /* Initialize SYS mode */    
    LDR     a4, [pc, #TXILLDB_SYS_STACK_SIZE-.-8]   ; Pickup SYS stack
    LDR     a3, [a4, #0]                    ;Pick the value from memory pointer
    MOV     a1, #SYS_MODE                   ; Build SYS mode CPSR
    MSR     CPSR_fsxc, a1                   ; Enter SYS mode
	MOV		lr, #0
	MOV		a1, #16
	MSR		SPSR_fsxc, a1		
    ADD     a2, a2, a3                      ; Calculate start of SYS stack
    BIC     a2, a2, #7                      ; Ensure 8 byte alignment
    SUB     sp, a2, #8                      ; Backup 8 byte, Setup SYS stack pointer
;
;
    MOV     a1, #SVC_MODE                   ; Build SVC mode CPSR
    MSR     CPSR_c, a1                      ; Enter SVC mode
    LDR     a4, [pc, #SYS_STACK_PTR-.-8]    ; Pickup stack pointer
    STR     a2, [a4, #0]                    ; Save the system stack
;
;    /* Save the system stack pointer.  */
;    _tx_thread_system_stack_ptr = (VOID_PTR) (sp);
;
    LDR     a2, [pc, #SYS_STACK_PTR-.-8]    ; Pickup address of system stack ptr
    LDR     a1, [a2, #0]                    ; Pickup system stack 
    ADD     a1, a1, #4                      ; Increment to next free word
;
;    /* Pickup the first available memory address.  */
;
;    /* Allocate space for the timer thread's stack.  */
;    _tx_timer_stack_start =        first_available_memory;
;    _tx_timer_stack_size =         stack_size;
;    _tx_timer_priority =           0;
;
	STMFD	r13!, {r4}						; Push r4  Save R4 since the outside may use it
    LDR     a2, [pc, #TIMER_STACK-.-8]      ; Pickup timer stack ptr address
    LDR     a4, [pc, #TIMER_STACK_SIZE-.-8] ; Pickup timer stack size address
    LDR     r4, [pc, #TXILLDB_TIM_STACK_SIZE-.-8]   ; Pickup actual stack size
    LDR     a3, [r4, #0]
    STR     a1, [a2, #0]                    ; Store timer stack base
    STR     a3, [a4, #0]                    ; Store timer stack size
    ADD     a1, a1, a3                      ; New free memory address
    LDR     a2, [pc, #TIMER_PRIORITY-.-8]   ; Pickup timer priority address
    LDR     a3, [pc, #SYS_TIMER_PRI-.-8]    ; Build timer thread priority
    LDR     a3, [a3, #0]    
    STR     a3, [a2, #0]                    ; Store timer thread priority
;
;    /* Save the first available memory address.  */
;    _tx_initialize_unused_memory =  (VOID_PTR) System Stack + Timer Stack;
;
    LDR     a3, [pc, #UNUSED_MEMORY-.-8]    ; Pickup unused memory ptr address
    STR     a1, [a3, #0]                    ; Save first free memory address
	LDMFD   r13!, {r4}						; Pop r4
    
;
;    /* Done, return to caller.  */
;
    BX      lr                              ; Return to caller
;}
;
;
;/* Define shells for each of the interrupt vectors.  */
;
    EXPORT  __tx_reset
__tx_reset
;// Check whether unexpectly pc reset
  
;// Enter SVC mode and setup temp SVC stack pointer
    MSR     CPSR_c, #SVC_MODE                ; Close I and F IRQ
    LDR     SP, =SVC_Stack_Temp

;// Clear all registers
    MOV     r0, #0
    MOV     r1, #0
    MOV     r2, #0
    MOV     r3, #0
    MOV     r4, #0
    MOV     r5, #0
    MOV     r6, #0
    MOV     r7, #0
    MOV     r8, #0
    MOV     r9, #0
    MOV     r10,#0
    MOV     r11,#0
    MOV     r12,#0
    MOV     lr, #0
    MOV     a1, #16
    MSR     SPSR_fsxc, a1
    
       
    ;//init abort mode stack before watchdog reset 
    BL _tx_initialize_low_level_32
        
   BL      __BOOT_InitModule               ; Initialize several necessary chip
                                            ; module such as MMU/Cache/EMC etc.	
   
    BL CHIP_InitAdi

	
    IF (:DEF:FLASH_TYPE_SPI)
	;;BL  SPIFLASH_SetCS0SpaceSize    ;for SC6531EFM set cs0 space size
    ENDIF
    
   	IF (:DEF:TF_LOAD_PROTECT)
 	;;BL  TF_EnterInit
   	ENDIF
   	

;// Init SVC mode stack at the first time
    MOV     r11, sp
    LDR     r12, [pc, #TXILLDB_SVC_STACK-.-8]; Pickup SVC mode stack address
    LDR     r12, [r12, #0]                   ; Pick the const value from memory
    BIC     r12, r12, #7                     ; Ensure long word alignment
    SUB     sp,  r12, #8
    
    STMFD   r13!, {r0-r7,r11,r14}            ; Save Assert rigster

    BL      __BOOT_ResetHandler              ; Check whether the watch dog reset

    BL      __BOOT_IRAM_EN                   ; Enable IRAM controlled by ARM

;// Jump ARM library to init RW and ZI region
;// after that it will call main function
    B       __main                           ; Jump to system main function
;
;
    EXPORT  __tx_irq_handler
    EXPORT  __tx_irq_processing_return
__tx_irq_handler
;    /* Jump to context save to save system context.  */
    B       _tx_thread_context_save
    CODE16
__tx_irq_processing_return
;
;    /* At this point execution is still in the Thumb/IRQ mode.  The CPSR, point of
;       interrupt, and all C scratch registers are available for use.  In 
;       addition, IRQ interrupts may be re-enabled - with certain restrictions -
;       if nested IRQ interrupts are desired.  Interrupts may be re-enabled over
;       small code sequences where lr is saved before enabling interrupts and 
;       restored after interrupts are again disabled.  */
;
;    /* For debug purpose, execute the timer interrupt processing here.  In
;       a real system, some kind of status indication would have to be checked
;       before the timer interrupt handler could be called.  */
;
	BL	ISR_Handler
;
;    /* Jump to context restore to restore system context.  */
;
;;_tx_return
    LDR     a1, =_tx_thread_context_restore ; Pickup context restore address
    MOV     pc, a1                          ; Jump to context restore
    LTORG
    
    ALIGN
    CODE32
;
;
	EXPORT __tx_save_cur_reg
	CODE32
__tx_save_cur_reg	
	STMFD	r13!, {r12}						; Push r12
    LDR     r12, =cur_mode_reg              ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR

    EXPORT  __tx_save_all_reg
    CODE32
__tx_save_all_reg
    MRS     r1, cpsr						; Save current mode.
;   Save SVC mode registers.
    MOV     r0, #SVC_MODE                   ; Build SVC mode CPSR
    MSR     CPSR_c, r0                      ; Enter SVC mode
    LDR     r12, =svc_mode_reg              ; Get save register address
    STR		r13, [r12, #0]					; Save r13
    STR		r14, [r12, #4]					; Save r14
    MRS     r0,  spsr
    STR		r0,  [r12, #8]					; Save SPSR

;   Save IRQ mode registers.
    MOV     r0, #IRQ_MODE                   ; Build IRQ mode CPSR
    MSR     CPSR_c, r0                      ; Enter IRQ mode
    LDR     r12, =irq_mode_reg              ; Get save register address
    STR		r13, [r12, #0]					; Save r13
    STR		r14, [r12, #4]					; Save r14
    MRS     r0,  spsr
    STR		r0,  [r12, #8]					; Save SPSR
;   Save Abort mode registers.
    MOV     r0, #ABORT_MODE                 ; Build IRQ mode CPSR
    MSR     CPSR_c, r0                      ; Enter IRQ mode
    LDR     r12, =abort_mode_reg            ; Get save register address
    STR		r13, [r12, #0]					; Save r13
    STR		r14, [r12, #4]					; Save r14
    MRS     r0,  spsr
    STR		r0,  [r12, #8]					; Save SPSR
;   Save Undefined mode registers.
    MOV     r0, #UNDEFINE_MODE              ; Build IRQ mode CPSR
    MSR     CPSR_c, r0                      ; Enter IRQ mode
    LDR     r12, =undefine_mode_reg         ; Get save register address
    STR		r13, [r12, #0]					; Save r13
    STR		r14, [r12, #4]					; Save r14
    MRS     r0,  spsr
    STR		r0,  [r12, #8]					; Save SPSR
;   Save FIQ mode registers. 
    MOV     r0, #FIQ_MODE                   ; Build FIQ mode CPSR
    MSR     CPSR_c, r0                      ; Enter FIQ mode
    LDR     r12, =fiq_mode_reg              ; Get save register address
    STR		r13, [r12, #0]					; Save r13
    STR		r14, [r12, #4]					; Save r14
    MRS     r0,  spsr
    STR		r0,  [r12, #8]					; Save SPSR

    mov     r10,  #0    
	MSR     CPSR_c, r1						; Restore mode.
	BX		lr								; Return
;
;	
;
    IF (:DEF:PLATFORM_SC6530)
    IMPORT __vfp_enable
    ENDIF
    EXPORT  __tx_undefined
    CODE32
__tx_undefined
;   Save all current registers.
    IF (:DEF:PLATFORM_SC6530)
    STMDB   r13!,   {r0-r12, r14}    
;********************************
;Check thumb bit, normal undef if in thumb mode
    MOV R0, R14
    MRS R1, SPSR
    TST R1, #32
    BNE normal_undef
;Check coprocessor instructions 
; Is it a coprocessor instruction? To avoid the need to change this
; code in the future, this is checked carefully. (A check of instr[27]
; is sufficient in all hardware-generated cases, but some people like
; to "fake" undefined instruction trap entries when another exception
; handler cannot deal with an illegal instruction.)
;   So we test for instruction bits 27:24 = 1100, 1101 or 1110, done
; as an optimised range check on (instruction << 4).
    LDR R1, [R0, #-4]
    MOV R0, R1, LSL #4
    SUB R0, R0, #0xC0000000
    CMP R0, #0x30000000
    BHS normal_undef
;Check VFP instructions
;now R1 = undef_instruction
    AND R0, R1, #0x00000F00
    LSR R2, R0, #8
    CMP R2, #12
    BHS normal_undef
    CMP R2, #9
    BLO normal_undef

;********************************
    BL      __vfp_enable
    LDMIA r13!, {r0-r12, r14}
    SUBS PC, r14, #4
normal_undef
    LDMIA r13!, {r0-r12, r14}
    ENDIF
	STMFD	r13!, {r12}						; Push r12
	STMFD	r13!, {r12}						; Push r12 again ;
	
    LDR     r12, =cur_undefine_reg          ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
    LDR     r0, [r12, #0]                   ; Restore r0 
    LDR     r12, =cur_mode_reg              ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12  again
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
    
	BL      __tx_save_all_reg	
	
    LDR     r0, =undefine_mode_reg         ; Get save register address
    LDR     r1, =cur_mode_reg
    LDR     r1, [r1, #56]
    STR     r1, [r0, #4] 

    MOV     r10,    #0
	LDR		R0, =SCI_HandleUndefined
	BX		R0
;
;
    EXPORT  __tx_swi_interrupt    
__tx_swi_interrupt
;   Save all current registers.
	STMFD	r13!, {r12}						; Push r12
    LDR     r12, =cur_mode_reg              ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
    LDR     r0, [r12, #0]                   ; Restore r0 
	BL      __tx_save_all_reg	
    LDR     r0, =svc_mode_reg         ; Get save register address
    LDR     r1, =cur_mode_reg
    LDR     r1, [r1, #56]
    STR     r1, [r0, #4] 
	
	LDR		R0, =SCI_HandleSwi
	BX		R0
;
;
    EXPORT  __tx_prefetch_handler
    CODE32
__tx_prefetch_handler
;   Save all current registers.

    IF (:DEF:VM_SUPPORT)
    STMDB   r13!,   {r0-r12, r14}
    MOV     r10,    #0
    MOV     r0,     r14
    BL      MMU_HandlePrefetch
    TEQ     r0,     #0
    LDMNEIA r13!,   {r0-r12, r14}
    SUBNES  PC, r14, #4
    LDMIA r13!,   {r0-r12, r14}
    ENDIF
	STMFD	r13!, {r12}						; Push r12
	STMFD	r13!, {r12}						; Push r12 again ;
	
    IF (:DEF:VM_SUPPORT)	
    LDR     r12, =_tx_r14_before_exception
    STR     r14, [r12, #0]
    ENDIF
	
    LDR     r12, =cur_Pabout_reg          ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
    LDR     r0, [r12, #0]                   
    LDR     r12, =cur_mode_reg              ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12  again
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
	
	BL      __tx_save_all_reg	

    LDR     r0, =abort_mode_reg         ; Get save register address
    LDR     r1, =cur_mode_reg
    LDR     r1, [r1, #56]
    STR     r1, [r0, #4] 
	
    IF (:DEF:VM_SUPPORT)
    MOV     r12, #ABORT_HANDLE_MODE          ; Switch to SVC Mode, IRQ Disable, FIQ Enable
    MSR     CPSR_c, r12                      ; Enter SVC mode 
    LDR     r12, =_tx_r13_svc_mode
    STR     sp, [r12, #0]                    ; Save SVC Mode stack Pointer to Gloable value    

        
__do_prefetch_exception_handle
    LDR     r12, =_tx_abort_exception_magic
    LDR     r12, [r12, #0]
    STMFD   r13!, {r12}						 ; Push Abort magic Value
    LDR     r12, =_tx_r14_before_exception
    LDR     r12, [r12, #0]
    STMFD   r13!, {r12}						 ; Push Abort Address
    LDR     r12, =_tx_r13_svc_mode
    LDR     r12, [r12,#0]    
    MOV     r12, lr
    STMFD   r13!, {r12}						 ; Push SVC Mode lr Address
    ENDIF
	    
    LDR	   R12, =SCI_HandlePrefetch
    BX	   R12
;
;
    EXPORT  __tx_abort_handler
    CODE32
__tx_abort_handler

    IF (:DEF:VM_SUPPORT)
    STMDB   r13!,   {r0-r12, r14}
    MOV     r10,    #0
    MRC     p15, 0, r0, c5, c0, 0
    MRC     p15, 0, r1, c6, c0, 0
    BL      MMU_HandleAbort
    TEQ     r0,     #0
    LDMNEIA r13!,   {r0-r12, r14}
    SUBNES  PC,  r14, #8
    LDMIA r13!,   {r0-r12, r14}
    ENDIF
     
;   Save all current registers.
	STMFD	r13!, {r12}						; Push r12
	STMFD	r13!, {r12}						; Push r12 again ;

    IF (:DEF:VM_SUPPORT)	
    LDR     r12, =_tx_r14_before_exception
    STR     r14, [r12, #0]
    ENDIF
    
    LDR     r12, =cur_Dabout_reg          ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
    LDR     r0, [r12, #0]                   
    LDR     r12, =cur_mode_reg              ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12  again
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
    
	BL      __tx_save_all_reg	

    LDR     r0, =abort_mode_reg         ; Get save register address
    LDR     r1, =cur_mode_reg
    LDR     r1, [r1, #56]
    STR     r1, [r0, #4] 

	IF (:DEF:PLATFORM_SC8800H) :LOR: (:DEF:PLATFORM_SC6800H) :LOR: (:DEF:PLATFORM_SC8800G) :LOR: (:DEF:PLATFORM_SC6530) :LOR: (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UWS6121E)
	MRC		p15,0,r0,c5,c0,0				; Read DFSR
	LDR		r1, =abort_DFSR_reg				; Get abort_DFSR_reg memory address
	STR		r0,[r1]							; Save DRSR value to memory
	
	MRC		p15,0,r0,c5,c0,1				; Read IFSR
	LDR		r1, =abort_IFSR_reg				; Get abort_IFSR_reg memory address
	STR		r0,[r1]							; Save IRSR value to memory
	
	MRC		p15,0,r0,c6,c0,0				; Read FAR
	LDR		r1, =abort_FAR_reg				; Get abort_FAR_reg memory address
	STR		r0,[r1]							; Save FAR value to memory
	ENDIF

    IF (:DEF:VM_SUPPORT)
    MOV     r12, #ABORT_HANDLE_MODE          ; Switch To SVC Mode, IRQ Disable, FIQ Enable
    MSR     CPSR_c, r12                      ; Enter SVC mode 
    LDR     r12, =_tx_r13_svc_mode
    STR     sp, [r12, #0]                    ; Save SVC Mode stack Pointer to Gloable value
    
__do_abort_exception_handle      
    LDR     r12, =_tx_abort_exception_magic
    LDR     r12, [r12, #0]
    STMFD   r13!, {r12}						 ; Push Abort magic Value
    LDR     r12, =_tx_r14_before_exception
    LDR     r12, [r12, #0]
    STMFD   r13!, {r12}						 ; Push Abort Address
    LDR     r12, =_tx_r13_svc_mode
    LDR     r12, [r12,#0]
    STMFD   r13!, {r12}						 ; Push SVC Mode sp Address before abort
    MOV     r12, lr
    STMFD   r13!, {r12}						 ; Push SVC Mode lr Address before abort
    ENDIF
	
    LDR	   R12, =SCI_HandleAbort
    BX	   R12
;
;
    EXPORT  __tx_reserved_handler
    CODE32
__tx_reserved_handler
;   Save all current registers.
	STMFD	r13!, {r12}						; Push r12
    LDR     r12, =cur_mode_reg              ; Get save register address
    STMIA   r12, {r0-r15}       	        ; Save all user registers in save area
	LDMFD   r13!, {r0}						; Pop r12
	STR     r0, [r12, #48]                  ; Save r12
    STR		r13,[r12, #52]					; Save R13
    MRS     r0, spsr                        ; Get SPSR 
    STR		r0, [r12, #64]					; Save SPSR
    MRS     r0, cpsr                        ; Get CPSR
    STR		r0, [r12, #68]					; Save CPSR
    LDR     r0, [r12, #0]                   	
	BL      __tx_save_all_reg	
	
	LDR		R0, =SCI_HandleReserved
	BX		R0
;
;

SYS_STACK_PTR
    DCD     _tx_thread_system_stack_ptr
UNUSED_MEMORY
    DCD     _tx_initialize_unused_memory
TIMER_STACK
    DCD     _tx_timer_stack_start
TIMER_STACK_SIZE
    DCD     _tx_timer_stack_size
TIMER_PRIORITY
    DCD     _tx_timer_priority
;
;	
;	IMPORT  DSP_FiqHandler
	IMPORT  ARM_FIQ_Handler
	
    EXPORT  __tx_fiq_handler
    CODE32
__tx_fiq_handler
    STMDB   r13!, {r0-r12, r14}              ; Save all user registers in save area
    LDR     a1, =_tx_r4_val_before_fiq    
    STR		r4, [a1, #0]   
    LDR     a1, =_tx_pc_val_before_fiq      ; Pickup address of pc value before fiq
    STR		lr, [a1, #0]                    ; Pickup current thread pointer
;	BL DSP_FiqHandler
	BL ARM_FIQ_Handler
	LDMIA   r13!, {r0-r12, r14}
	SUBS PC, r14, #4
;

;Get current cpu contex. @steve.zhan
    AREA    Init, CODE, READONLY
    EXPORT GetCxt
GetCxt
    stmdb   sp!, {r1-r3,lr}
    stmdb   r0!, {r0-r12,lr}
    mrs     r1, cpsr
    mov     r2, lr
    add     r3, sp, #0x38
    stmdb   r0!, {r1-r3}

    ldmia   sp!, {r1-r3,pc}

    EXPORT  TF_JumpToTarget
TF_JumpToTarget    
    MOV  pc, r0 

    END


