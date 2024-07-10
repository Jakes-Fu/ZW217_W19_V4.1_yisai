	IMPORT     MCU_is_sleep
	IMPORT     ahb_sleep_flag
	IMPORT     s_sleep_step
	
	AREA  |C$$code|, CODE, READONLY
|x$codeseg| DATA
	
;/**************************************************************************/ 
;/*                                                                        */ 
;/*  FUNCTION                                               RELEASE        */ 
;/*                                                                        */ 
;/*    tx_enter_deep_sleep                                 THUMB/ARM       */ 
;/*                                                                        */ 
;/*  DESCRIPTION                                                           */ 
;/*                                                                        */ 
;/*    ARM enter sleep mode                               				   */ 
;/*    called by SCI_EnterDeepSleep                        				   */ 
;/*                                                                        */ 
;/**************************************************************************/ 

SLEEP_CTL0      EQU      0x205000D0
SLEEP_CTL1      EQU      0x205000D4	
REMAP          EQU      0x205000E0

    IF :DEF:ARM_CORE_POWER_DOMAIN 

    IMPORT MMU_DisableIDCM_BeforeSleep
    IMPORT MMU_AFTER_SLEEP

FIQ_MODE        EQU     0xD1                ; Disable irq,fiq FIQ mode
IRQ_MODE        EQU     0xD2                ; Disable irq,fiq IRQ mode
SVC_MODE        EQU     0xD3                ; Disable irq,fiq SVC mode
UNDEFINE_MODE   EQU     0xDB                ; Disable irq,fiq UNDEFINE mode
ABORT_MODE      EQU     0xD7                ; Disable irq,fiq ABORT mode
SYS_MODE        EQU     0xDF                ; Disable irq,fiq SYSTEM mode

AHB_ARCH_PORT       EQU     0x20500120
AHB_SYS_CTL2        EQU     0x205000B8
UNLOCK_SYS_CTL2     EQU     0xA1B2
ARM9_CORE_PD_FLAG   EQU     0x10000

    EXPORT saved_state_cur
    EXPORT saved_state_svc
    EXPORT saved_state_sys
    EXPORT saved_state_irq
    EXPORT saved_state_abt
    EXPORT saved_state_und
    EXPORT saved_state_fiq
    EXPORT saved_state_cp15

    CODE32
    EXPORT  RECOVERY_ARM_REG
    
RECOVERY_ARM_REG

;    REG32(AHB_SYS_CTL2) &= ~ARM9_CORE_PD_FLAG;
;    REG32(AHB_ARCH_PORT) &= ~0xA1B2; // unlock AHB_SYS_CTL2

    LDR     R5, =AHB_SYS_CTL2
    BIC     R4, R4, #ARM9_CORE_PD_FLAG  ;clean arm core power down flag
    STR     R4, [R5]
    
    LDR  R5, =AHB_ARCH_PORT
    LDR  R3, [R5]
    LDR  R4, =UNLOCK_SYS_CTL2
    BIC  R3, R3, R4 
    STR  R3, [R5]   


;recovery mmu
    MOV     r0, #SVC_MODE      
    MSR     cpsr_cxsf, r0           

    LDR     r0, =saved_state_cp15    ;save svc_mode
    LDMFD   r0!, {r4-r7}    

    MCR     p15, 0, r4, c13, c0, 0      ; PID
    MCR     p15, 0, r5, c3, c0, 0       ; Domain ID
    MCR     p15, 0, r6, c2, c0, 0       ; translation table base
    MCR     p15, 0, r7, c1, c0, 0       ; turn on MMU, etc,recovery
    NOP                 ; second-to-last before mmu
    NOP
    NOP
    NOP
    NOP 
    NOP
    
;recovery fiq
    MOV     r0, #FIQ_MODE                   
    MSR     cpsr_cxsf, r0                         
    LDR     r0, =saved_state_fiq    
    LDMFD   r0!, {r8-r14}
    LDMFD   r0!, {r1}       
    MSR     spsr_cxsf, r1

;recovery und
    MOV     r0, #UNDEFINE_MODE      
    MSR     cpsr_cxsf, r0               
    LDR     r0, =saved_state_und    
    LDMFD   r0!, {r13-r14}
    LDMFD   r0!, {r1}       
    MSR     spsr_cxsf, r1
;recovery abt
    MOV     r0, #ABORT_MODE      
    MSR     cpsr_cxsf, r0           ;    
    LDR     r0, =saved_state_abt    ;
    LDMFD   r0!, {r13-r14} 
    LDMFD   r0!, {r1}       ;
    MSR     spsr_cxsf, r1
;recovery irq
    MOV     r0, #IRQ_MODE      
    MSR     cpsr_cxsf, r0              
    LDR     r0, =saved_state_irq    
    LDMFD   r0!, {r13-r14}
    LDMFD   r0!, {r1}       
    MSR     spsr_cxsf, r1
;recovery sys
    MOV     r0, #SYS_MODE      
    MSR     cpsr_cxsf, r0               
    LDR     r0, =saved_state_sys    
    LDMFD   r0!, {r12-r13}
;recovery svc
    MOV     r0, #SVC_MODE      
    MSR     cpsr_cxsf, r0             
    LDR     r0, =saved_state_svc    
    LDMFD   r0!, {r13-r14}
    LDMFD   r0!, {r1}       
    MSR     spsr_cxsf, r1    
;recovery cur_mode
    LDR     r0, =saved_state_cur  
    LDR     r1, [r0,#60]
    MSR     cpsr_cxsf, r1
    ADD     r0, r0, #0x4
    LDMFD  r0, {r1-r13,LR}   

    MCR     p15, 0, r1, c8, c7, 0       ;   Invalidate the TLB
    
    STMFD   sp!, {lr}
    BL      MMU_AFTER_SLEEP
    LDMFD   sp!, {lr}             ; restore registers
    
    B      Call_Exit

    ENDIF    ;// IF :DEF:ARM_CORE_POWER_DOMAIN 
    

; IF SCI_DEEP_SLEEP
    CODE16
    EXPORT  tx_enter_deep_sleep
tx_enter_deep_sleep
    
    ADR     a4, tx_enter_deep_sleep_32  ; Build 32-bit entry point 
    BX      a4                          ; Switch to 32-bit instructions
    CODE32
tx_enter_deep_sleep_32
	STMFD   r13!, {r0-r4}               ; May not need 
	
;   // Save sleep level 
	MOV     R3, R0	
	
;   // Set Sleep flag
;   set MCU_is_sleep = 1;
    LDR     R1, [pc, #SLEEP_FLAG-.-8] 
    MOV     R2, #1
    STR     R2, [R1]
    
;	// Check sleep level and find sleep entry
    CMP     R3,#0x0
    BNE     ARM_STANDBY	


MCU_Stop

;   // Set MCU system sleep enable
    LDR  R1, =SLEEP_CTL1
    LDR  R2, [R1]
    ORR  R2, R2, #0x18
    STR  R2, [R1, #0]   ; MCU System sleep enable

;   // Wait some cycles       
    MOV  R1, #0x20
Wait0
    SUB  R1, R1, #0x1
    CMP  R1, #0x0
    BNE  Wait0
    
    IF :DEF:ARM_CORE_POWER_DOMAIN 
    CODE32
    EXPORT   BACKUP_ARM_REG
    
BACKUP_ARM_REG    
    STMFD   r13!, {r0}                     ; Push r0
    LDR     r0, =saved_state_cur    
    STMIA   r0!, {r0-r14}
    MRS     r1, cpsr    ; Get CPSR
    STMIA   r0!, {r1}   ;save cpsr
    LDR     r0, =saved_state_cur    
    LDMFD   r13!, {r1}				; Pop r0  
    STR		r1,[r0]					;save r0
    STR		r13,[r0, #52]			; Save R13
    
;save svc mode
    MOV     r0, #SVC_MODE                   
    MSR     cpsr_cxsf, r0                      
    LDR     r0, =saved_state_svc    
    STMIA   r0!, {r13-r14}
    MRS     r1, spsr    ; Get SPSR
    STMIA   r0!, {r1} 
;in svc mode save cp15 reg
    LDR     r0, =saved_state_cp15
    MRC	    p15, 0, r4, c13, c0, 0	; PID
    MRC	    p15, 0, r5, c3, c0, 0	; Domain ID
    MRC	    p15, 0, r6, c2, c0, 0	; translation table base address
    MRC	    p15, 0, r7, c1, c0, 0	; control register

	STMIA	r0!, { r4 - r7 }    
    
;save sys mode
    MOV     r0, #SYS_MODE                   
    MSR     cpsr_cxsf, r0                    
    LDR     r0, =saved_state_sys    
    STMIA   r0!, {r12-r13}

;save irq mode
    MOV     r0, #IRQ_MODE                  
    MSR     cpsr_cxsf, r0                     
    LDR     r0, =saved_state_irq   
    STMIA   r0!, {r13-r14}
    MRS     r1, spsr    ; Get SPSR
    STMIA   r0!, {r1}
    
;save abort mode
    MOV     r0, #ABORT_MODE                   
    MSR     cpsr_cxsf, r0                         
    LDR     r0, =saved_state_abt   
    STMIA   r0!, {r13-r14}
    MRS     r1, spsr    ; Get SPSR
    STMIA   r0!, {r1}   
    
;save undefine mode
    MOV     r0, #UNDEFINE_MODE                   
    MSR     cpsr_cxsf, r0                        
    LDR     r0, =saved_state_und   
    STMIA   r0!, {r13-r14}
    MRS     r1, spsr    ; Get SPSR
    STMIA   r0!, {r1}  
    
;save fiq mode
    MOV     r0, #FIQ_MODE                  
    MSR     cpsr_cxsf, r0                         
    LDR     r0, =saved_state_fiq      
    STMIA   r0!, {r8-r14}
    MRS     r1, spsr    ; Get SPSR
    STMIA   r0!, {r1} 
    
;recovery and go on
    MOV     r0, #SVC_MODE      
    MSR     cpsr_cxsf, r0      ; Enter SVC mode
    LDR     r0, =saved_state_cur  
    LDR     r1, [r0,#60]
    MSR     cpsr_cxsf, r1       ;enter cur mode,should same svc
    ADD     r0, r0, #0x4
    LDMFD  r0, {r1-r13,LR}

    
;    PRESERVE8

;    REG32(AHB_ARCH_PORT) |= 0xA1B2; // unlock AHB_SYS_CTL2
;    REG32(AHB_SYS_CTL2) |= ARM9_CORE_PD_FLAG;

    LDR  R1, =AHB_ARCH_PORT
    LDR  R2, [R1]
    LDR  R4, =UNLOCK_SYS_CTL2
    ORR  R2, R2, R4 
    STR  R2, [R1, #0]   
    
    LDR  R1, =AHB_SYS_CTL2
    LDR  R2, [R1]
    ORR  R2, R2, #ARM9_CORE_PD_FLAG  

    STR  R2, [R1, #0]   ; set arm core power down flag

ARM_STANDBY
    STMFD   sp!, {lr}
    BL      MMU_DisableIDCM_BeforeSleep
    LDMFD   sp!, {lr}             ; restore registers 

    MOV r0, #0
    MCR p15,0,r0,c7,c0,4    ;  arm core sleep, wait for int
    
    ELSE    ;// IF :DEF:ARM_CORE_POWER_DOMAIN 

;   set s_sleep_step |= BIT_5;
;    LDR     R1, [pc, #SLEEP_STEP-.-8] 
;    LDR     R2, [R1]
;    ORR     R2, R2, #0x20
;    STR     R2, [R1]

ARM_STANDBY
;   // Set ARM9 into low power state
    LDR  R1, =SLEEP_CTL0
    LDR  R2, [R1]
    ORR  R2, R2, #0x2
    STR  R2, [R1, #0]   ; arm core sleep

    ENDIF     ;// IF :DEF:ARM_CORE_POWER_DOMAIN 

    
    NOP
    NOP
    NOP 
    NOP
    NOP
    NOP 
    NOP
    NOP
    NOP 
    NOP
    NOP
    NOP 
    
;   // Wait some cycles       
    MOV  R1, #0x100
Wait1
    SUB  R1, R1, #0x1
    CMP  R1, #0x0
    BNE  Wait1
    

ARM_WAKEUP
;   set s_sleep_step |= BIT_6;
;    LDR     R1, [pc, #SLEEP_STEP-.-8] 
;    LDR     R2, [R1]
;    ORR     R2, R2, #0x40
;    STR     R2, [R1]
             
Call_Exit
    LDMFD  r13!, {r0-r4} ; May not need
    BX     LR     	  ; return
	
SLEEP_FLAG
    DCD      MCU_is_sleep
AHB_FLAG
    DCD      ahb_sleep_flag
SLEEP_STEP
    DCD      s_sleep_step


    IF :DEF:ARM_CORE_POWER_DOMAIN    

;    IMPORT  armcore_stack_space

saved_state_cur
;    DCD     armcore_stack_space
;    SPACE  4 * 16 ; r0-14,cpsr
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
    DCD		00000000	; cpsr
    
saved_state_svc
;    DCD     (armcore_stack_space + 4 * 16)
;    SPACE  4 * 3 ; r13-14  spsr_svc
    DCD     00000000    ; r13
    DCD     00000000    ; r14
    DCD     00000000    ; spsr_svc
    
saved_state_sys
;    DCD     (armcore_stack_space + 4 * 19)
;    SPACE  4 * 2 ; r12, r13       
    DCD     00000000    ; r12
    DCD     00000000    ; r13
    
saved_state_irq
;    DCD     (armcore_stack_space + 4 * 21)
;    SPACE  4 * 3 ; r13_irq, r14_irq, spsr_irq
    DCD     00000000    ; r13_irq
    DCD     00000000    ; r14_irq
    DCD     00000000    ; spsr_irq
    
saved_state_abt
;    DCD     (armcore_stack_space + 4 * 24)
;   SPACE  4 * 3 ; r13_abt, r14_abt, spsr_abt
    DCD     00000000    ; r13_abt
    DCD     00000000    ; r14_abt
    DCD     00000000    ; spsr_abt

saved_state_und
;    DCD     (armcore_stack_space + 4 * 27)
;    SPACE  4 * 3 ; r13_undef, r14_undef, spsr_undef
    DCD     00000000    ; r13_undef
    DCD     00000000    ; r14_undef
    DCD     00000000    ; spsr_undef

saved_state_fiq
;    DCD     (armcore_stack_space + 4 * 30)
;    SPACE  4 * 8 ; r8_fiq, r9_fiq, r10_fiq, r11_fiq, r12_fiq, r13_fiq, r14_fiq, spsr_fiq
    DCD		00000000	; r8_fiq
    DCD		00000000	; r9_fiq
    DCD		00000000	; r10_fiq
    DCD		00000000	; r11_fiq
    DCD		00000000	; r12_fiq
    DCD		00000000	; r13_fiq
    DCD		00000000	; r14_fiq
    DCD		00000000	; spsr_fiq

saved_state_cp15
;    DCD     (armcore_stack_space + 4 * 38)
;    SPACE  4 * 4  ; cp15  
    DCD		00000000	; cp15
    DCD		00000000	; cp15
    DCD		00000000	; cp15
    DCD		00000000	; cp15
    
    ENDIF 

	CODE32
	EXPORT remap_and_reboot
;0:mapped to external memory;1:on-chip ROM
remap_and_reboot
    LDR  r1, =REMAP
    MOV  r2, #1
    STR  r2, [r1]
    MOV  pc, r0
  
  END
