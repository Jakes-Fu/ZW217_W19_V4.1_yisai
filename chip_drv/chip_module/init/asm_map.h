;/******************************************************************************/
;/** File Name:      asm_map.h                                                 */
;/** Author:         raistlin.kong                                             */
;/** DATE:           11/17/2003                                                */
;/** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         */
;/** Description:                                                              */
;/**                 the asm map included by all the arm assemble file and this*/
;/**                 will decide to include the detailed map file for assemble */
;/**                 files                                                     */
;/******************************************************************************/

;/******************************************************************************/
;/**                        Edit History                                       */
;/** ------------------------------------------------------------------------- */
;/** DATE           NAME             DESCRIPTION                               */
;/** 11-17-2003     Raistlin.Kong    Create.                                   */
;/******************************************************************************/  
;import the const variable defined in the asm_cvt.c to used these value in the
;ARM asm code instead of the const VALUE define in the asm code.
    IMPORT      g_fiq_stack_size
    IMPORT      g_irq_stack_size
    IMPORT      g_undefine_stack_size
    IMPORT      g_abort_stack_size
    IMPORT      g_sys_stack_size
    IMPORT      g_timer_stack_size
    IMPORT      g_svc_stack_limit_add
    IMPORT      g_sys_stack_base
    IMPORT      g_fiq_stack_base
    IMPORT      g_exception_mode_flag
    IMPORT      g_hw_reset_addr
    IMPORT      g_sys_timer_pri

    IF (:DEF:PLATFORM_SC8800G)
    IMPORT      g_adi_arm_rd_cmd_add
    IMPORT      g_adi_rd_data_add
    ENDIF

    IF (:DEF:PLATFORM_SC6530)
    IMPORT      g_adi_arm_rd_cmd_add
    IMPORT      g_adi_rd_data_add
    ENDIF

    IF (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UWS6121E) 
    IMPORT      g_adi_arm_rd_cmd_add
    IMPORT      g_adi_rd_data_add
    ENDIF

; @Xueliang.Wang modified.(2002/05/22)
; Export the flowing Variable
    EXPORT TXILLDB_FIQ_STACK_SIZE
TXILLDB_FIQ_STACK_SIZE
    DCD     g_fiq_stack_size                            ; Keep 4-byte alignment

    EXPORT TXILLDB_FIQ_STACK
TXILLDB_FIQ_STACK
    DCD g_fiq_stack_base

    EXPORT TXILLDB_IRQ_STACK_SIZE
TXILLDB_IRQ_STACK_SIZE
    DCD     g_irq_stack_size                            ; Keep 4-byte alignment
    
    EXPORT TXILLDB_UNDEFINE_STACK_SIZE
TXILLDB_UNDEFINE_STACK_SIZE
    DCD     g_undefine_stack_size                       ; Keep 4-byte alignment
    
    EXPORT TXILLDB_ABORT_STACK_SIZE
TXILLDB_ABORT_STACK_SIZE
    DCD     g_abort_stack_size                          ; Keep 4-byte alignment
    
    EXPORT TXILLDB_SYS_STACK_SIZE
TXILLDB_SYS_STACK_SIZE
    DCD     g_sys_stack_size                            ; Keep 4-byte alignment
    
    EXPORT TXILLDB_TIM_STACK_SIZE
TXILLDB_TIM_STACK_SIZE
    DCD     g_timer_stack_size                          ; Timer stack size
    
    EXPORT TXILLDB_SVC_STACK
TXILLDB_SVC_STACK                               
    DCD     g_svc_stack_limit_add                       ; SVC stack Base
  
EXCEPTION_MODE_FLAG
    DCD     g_exception_mode_flag                       ; when system happen a invalid reset ,this flag will been set;
    
HWRST_ADD
    DCD     g_hw_reset_addr 

    IF (:DEF:PLATFORM_SC6530)
ADI_ARM_RD_CMD_ADDR
    DCD    g_adi_arm_rd_cmd_add

ADI_RD_DATA_ADDR
    DCD    g_adi_rd_data_add
    ENDIF

    IF (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UWS6121E)
ADI_ARM_RD_CMD_ADDR
    DCD    g_adi_arm_rd_cmd_add

ADI_RD_DATA_ADDR
    DCD    g_adi_rd_data_add
    ENDIF

SYS_TIMER_PRI
    DCD     g_sys_timer_pri


; @Xueliang.Wang modified end.(2002/05/22)

    IF (:DEF:PLATFORM_SC8800G)
;//tim.luo modified for sc8800g;
ADI_ARM_RD_CMD
    DCD     g_adi_arm_rd_cmd_add
ADI_RD_DATA
    DCD     g_adi_rd_data_add
    ENDIF
    
RAM_LIMIT                                               ; the ZW and ZI data should not excced the RAM LIMIT,and RAM after RAM_LIMIT
    DCD     g_sys_stack_base                            ; is used as stack of exceptions as FIQ,IRQ,ABORT and so on

    END
