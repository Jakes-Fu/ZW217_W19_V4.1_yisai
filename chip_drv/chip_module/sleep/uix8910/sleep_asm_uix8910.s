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
USER_MODE			EQU 		0x10
LOCKOUT 			EQU 		&C0 		;Interrupt lockout value
LOCK_MSK			EQU 		&C0 		;Interrupt lockout mask value
MODE_MASK			EQU 		&1F 		;Processor Mode Mask
SUP_MODE			EQU 		&13 		;Supervisor Mode (SVC)
IRQ_MODE			EQU 		&12 		;Interrupt Mode (IRQ)
FIQ_MODE			EQU 		&11 		;Fast Interrupt Mode (FIQ)
ABT_MODE			EQU 		&17 		;abort mode
UND_MODE			EQU 		&1B 		;undefined mode
SYS_MODE			EQU 		&1F 		;system mode 2013_12_11
I_BIT				EQU 		0x80		;Interrupt bit of CPSR and SPSR
F_BIT				EQU 		0x40		;Interrupt bit of CPSR and SPSR 


REMAP           	    	EQU      0x205000E0
IDL_SYSCTRL2    	    	EQU      0x50090004
IDL_REF32K_ADDR         	EQU      0x50090088
IDLE_RES9					EQU      0x50094024
SPI_FLASH_CONFIG            EQU      0x00880014

     IF :DEF:CHIP_VER_UWS6121E  
SPI_FLASH2_CONFIG          EQU      0x00881014
     ENDIF

;    IF :DEF:ARM_CORE_POWER_DOMAIN 

;	IMPORT MMU_DisableIDCM_BeforeSleep
;	IMPORT MMU_AFTER_SLEEP
	IMPORT LpsDomainSt
	EXPORT RECOVERY_ARM_REG

    CODE32

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;void RECOVERY_ARM_REG(void);
; recocery ap cpu register.register
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
RECOVERY_ARM_REG
; resume spi_flash config	
    ldr     r2, =SPI_FLASH_CONFIG
    ldr     r3, =0x221                          
    str     r3,[r2]
    
   IF :DEF:CHIP_VER_UWS6121E  
; resume spi_flash2 config	
    ldr     r2, =SPI_FLASH2_CONFIG
    ldr     r3, =0x221                          
    str     r3,[r2]
   ENDIF

    ldr     r2, =IDLE_RES9
    mov     r3, #0x3                          
    str     r3,[r2]

    ldr     r2, =LpsDomainSt
    mov     r3, #0x8                          ;LPS_DEEPSLP_DOFF
    str     r3,[r2]

    ldr     r4,[r2]
    cmp     r3,r4
	beq		loop_go_run
loop_check_while
	b     	loop_check_while

loop_go_run
    ldr     r2, =IDLE_RES9
    mov     r3, #0x4                          
    str     r3,[r2]
    
    ldr     r0, =gLpsRegSaveEnd    
;add description when test
    ldr     r2, =RECOVERY_ARM_REG 
;add description when test    
    adr     r3, RECOVERY_ARM_REG 
    add     r0, r0, r3
    sub     r0, r0, r2	

;13. set to SVC mode  
    mrs     r1,cpsr                         ;read cpsr
    bic     r1,r1,#MODE_MASK                	
    orr     r1,r1,#SUP_MODE    
    msr     cpsr_cxsf,r1 
    ldmdb   r0!,{r12-r14}
    msr     spsr_cxsf,r12                   ;Restore SPSR

    ldr     r2, =IDLE_RES9
    mov     r3, #0x5                          
    str     r3,[r2]
    
;12. set to und mode  
    mrs     r1,cpsr                         ;read cpsr
    bic     r1,r1,#MODE_MASK                	
    orr     r1,r1,#UND_MODE    
    msr     cpsr_cxsf,r1 
    ldmdb   r0!,{r12-r14}
    msr     spsr_cxsf,r12                   ;Restore SPSR

    ldr     r2, =IDLE_RES9
    mov     r3, #0x6                          
    str     r3,[r2]
    
;11. set to abort mode
    mrs     r1,cpsr     
    bic     r1,r1,#MODE_MASK    
    orr     r1,r1,#ABT_MODE    
    msr     cpsr_cxsf,r1 
    ldmdb   r0!,{r12-r14}
    msr     spsr_cxsf,r12                   ;Restore SPSR

    ldr     r2, =IDLE_RES9
    mov     r3, #0x7                          
    str     r3,[r2]
    
;10.set to irq mode    
    mrs     r1,cpsr                         ;Pickup current CPSR
    bic     r1,r1,#MODE_MASK                ;Clear the mode bits	
    orr     r1,r1,#IRQ_MODE                 ;Set the IRQ mode bits
    msr     cpsr_cxsf,r1                    ;Move to the IRQ mode
    ldmdb   r0!,{r12-r14}
    msr     spsr_cxsf,r12                   ;Restore SPSR

    ldr     r2, =IDLE_RES9
    mov     r3, #0x8                          
    str     r3,[r2]
    
;9. set to sys mode    
    mrs 	r1,cpsr 						;Pickup current CPSR
    bic 	r1,r1,#MODE_MASK				;Clear the mode bits	
    orr 	r1,r1,#SYS_MODE 				;Set the IRQ mode bits
    msr 	cpsr_cxsf,r1					;Move to the IRQ mode
    ldmdb	r0!,{r12-r14}
    msr 	spsr_cxsf,r12					;Restore SPSR


    ldr     r2, =IDLE_RES9
    mov     r3, #0x9                          
    str     r3,[r2]
    
;8. restore cpsr
    ldmdb	r0!, {r1}
    msr 	cpsr_cxsf,r1

   
;7. restore vectors_base_secure_world    
    ldmdb   r0!, {r2}
    mcr	    p15, 0, r2, c12, c0, 1

    ldr     r2, =IDLE_RES9
    mov     r3, #0xA                          
    str     r3,[r2]
    
;6. restore vectors_base   
    ldmdb   r0!, {r2}
    mcr	    p15, 0, r2, c12, c0, 0        


;5. restore cacr    
    ldmdb   r0!, {r2}
    mcr	    p15, 0, r2, c1, c0, 2


    ldr     r2, =IDLE_RES9
    mov     r3, #0xB                          
    str     r3,[r2]
    
;4. restore 2->pmnc; 3->ccnt;  4->pmn0; 5->pmn1
    ldmdb   r0!, {r2-r5}
    ;mcr     p15, 0, r3, c15, c12, 1         ;ccnt 
    ;mcr     p15, 0, r4, c15, c12, 2         ;pmn0 
    ;mcr     p15, 0, r5, c15, c12, 3         ;pmn1 
    ;mcr     p15, 0, r2, c15, c12, 0         ;pmnc 

;3. restore ;;r2->mmu,  r3->ttb0,  r4->ttb1,r5->dacr,  r6->context id
    ldmdb   r0!,{r2-r6}   
    mcr     p15, 0, r5, c3,  c0, 0           ;dacr-domain access control Register 
    mcr     p15, 0, r3, c2,  c0, 1           ;ttb1
    mcr     p15, 0, r4, c2,  c0, 0           ;ttb0
    mcr     p15, 0, r6, c13, c0, 1           ;context ID
    ;mcr 	p15, 0, r2, c1,  c0, 0		     ;MMU control 
    dsb
    isb
	
;2. restore spsr
    ldmdb   r0!, {r1}
    msr     spsr_cxsf, r1


;;;;; __FPU_Enable
    ;;;Permit access to VFP/NEON, registers by modifying CPACR   
    MRC     p15,0,R1,c1,c0,2      
    ORR     R1,R1,#0x00F00000   
    MCR     p15,0,R1,c1,c0,2   
    ;;;Ensure that subsequent instructions occur in the context of VFP/NEON access permitted   
    ISB        					;Enable VFP/NEON    
    VMRS    R1,FPEXC     
    ORR     R1,R1,#0x40000000     
    VMSR    FPEXC,R1        	;Initialise VFP/NEON registers to 0    
    MOV     R2,#0        		;Initialise D16 registers to 0    
    VMOV    D0, R2,R2       
    VMOV    D1, R2,R2      
    VMOV    D2, R2,R2     
    VMOV    D3, R2,R2      
    VMOV    D4, R2,R2       
    VMOV    D5, R2,R2    
    VMOV    D6, R2,R2      
    VMOV    D7, R2,R2     
    VMOV    D8, R2,R2    
    VMOV    D9, R2,R2      
    VMOV    D10,R2,R2     
    VMOV    D11,R2,R2     
    VMOV    D12,R2,R2     
    VMOV    D13,R2,R2      
    VMOV    D14,R2,R2      
    VMOV    D15,R2,R2 
    ;#if 0
    ;VMOV    D16,R2,R2      
    ;VMOV    D17,R2,R2    
    ;VMOV    D18,R2,R2     
    ;VMOV    D19,R2,R2      
    ;VMOV    D20,R2,R2      
    ;VMOV    D21,R2,R2      
    ;VMOV    D22,R2,R2    
    ;VMOV    D23,R2,R2     
    ;VMOV    D24,R2,R2     
    ;VMOV    D25,R2,R2      
    ;VMOV    D26,R2,R2      
    ;VMOV    D27,R2,R2    
    ;VMOV    D28,R2,R2     
    ;VMOV    D29,R2,R2    
    ;VMOV    D30,R2,R2   
    ;VMOV    D31,R2,R2 
    ;#endif
    VMRS    R2,FPSCR       
    LDR     R3,=0x00086060  ;Mask off all bits that do not have to be preserved. Non-preserved bits can/should be zero.     
    AND     R2,R2,R3     
    VMSR    FPSCR,R2   
    ;BX      LR

    ldr     r2, =IDLE_RES9
    mov     r3, #0xC                          
    str     r3,[r2]
    
;1. restore sys mode registers
    ldmdb   r0!, {r4-r14} 
    isb
    dsb
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    mov     pc, lr
    nop
    nop
    nop  	

;    ENDIF    ;// IF :DEF:ARM_CORE_POWER_DOMAIN 
    

; IF SCI_DEEP_SLEEP
    CODE16
    EXPORT  tx_enter_deep_sleep
    
tx_enter_deep_sleep    
    ADR     a4, tx_enter_deep_sleep_32  ; Build 32-bit entry point 
    BX      a4                          ; Switch to 32-bit instructions
    CODE32
tx_enter_deep_sleep_32
;  // 1.set arm core stop
;   set MCU_is_sleep = 1;
    LDR     R1, [pc, #SLEEP_FLAG-.-8] 
    MOV     R2, #1
    STR     R2, [R1]    
    CMP     R0,#0x0
    BNE     ARM_STANDBY	

;   // 2.Set MCU system sleep enable
MCU_Stop
;    IF :DEF:ARM_CORE_POWER_DOMAIN 
    CODE32
    EXPORT   BACKUP_ARM_REG
BACKUP_ARM_REG    
	ldr 	r0, =gLpsRegSave
	
;1.save r4-r14								; space 4*11
	stmia	r0!, {r4-r14}

;2. Save spsr					   			; space 4*1	 
	mrs 	r12,spsr					  
	stmia	r0!,{r12}
	
;3. save mmu ttb ,dacr ,id				   	; space 4*5	 
	mrc 	p15, 0, r1, c1, c0, 0			; MMU control 
	mrc 	p15, 0, r2, c2, c0, 1			; ttb1
	mrc 	p15, 0, r3, c2, c0, 0		   	; ttb0
	mrc 	p15, 0, r4, c3, c0, 0		   	; save dacr(domain access control Register) 
	mrc 	p15, 0, ip, c13,c0, 1		   	; save context ID(application space identifier and process ID)
	stmia	r0!, {r1-r4, ip}

;4. save pmnc,ccnt,pmn0,pmn1			   	; space 4*4
	;mrc	 p15, 0, r1, c15, c12, 0	   	; save pmnc(used to control CCNT,CUNT0,CUNT1 ) 
	;mrc	 p15, 0, r2, c15, c12, 1	   	; save ccnt(includes the count value of the core clock) 
	;mrc	 p15, 0, r3, c15, c12, 2	   	; save CUNT0
	;mrc	 p15, 0, ip, c15, c12, 3	   	; save CUNT1
	stmia	 r0!, {r1-r3, ip}

;5. save cacr							   	; space 4*1
	mrc 	p15, 0, r1, c1, c0, 2		   	; read CACR 
	stmia	r0!, {r1}

;6. save vectors_base					   	; space 4*1
	mrc 	p15, 0, r1, c12, c0, 0		   	; save  Secure or Non-secure Vector Base Address
	stmia	r0!, {r1}

;7. save vectors_base_monitor		       	; space 4*1
	mrc 	p15, 0, r1, c12, c0, 1		   	; save Monitor  Vector Base Address 
	stmia	r0!, {r1}

;8. save cpsr								; space 4*1	
	mrs 	r1,cpsr 
	mrs 	r10,cpsr 					
	stmia	r0!,{r1}

;9. save the SYS mode						; space 4*3	 
	mrs 	r1,cpsr
	bic 	r1,r1,#MODE_MASK
	orr 	r1,r1,#SYS_MODE
	msr 	cpsr_cxsf,r1
	mrs 	r12,spsr					   	; Save SPSR into R12
	stmia	r0!, {r12-r14}
	  
;10.save irq mode						   	; space 4*3	  
	mrs 	r1,cpsr 					   	; Pickup current CPSR
	bic 	r1,r1,#MODE_MASK			   	; Clear the mode bits
	orr 	r1,r1,#IRQ_MODE 			   	; Set the IRQ mode bits
	msr 	cpsr_cxsf,r1				   	; Move to IRQ mode
	mrs 	r12,spsr					   	; Save SPSR into R12
	stmia	r0!,{r12-r14}				   	; Store R12-R14

;11.save abort 						   		; space 4*3
	mrs 	r1,cpsr
	bic 	r1,r1,#MODE_MASK
	orr 	r1,r1,#ABT_MODE
	msr 	cpsr_cxsf,r1
	mrs 	r12,spsr
	stmia	r0!,{r12-r14}

;12.save undefined 					   		; space 4*3
	mrs 	r1,cpsr
	bic 	r1,r1,#MODE_MASK
	orr 	r1,r1,#UND_MODE
	msr 	cpsr_cxsf,r1
	mrs 	r12,spsr
	stmia	r0!,{r12-r14}

;13.save svc mode 					       	; space 4*3
	mrs 	r1,cpsr
	bic 	r1,r1,#MODE_MASK
	orr 	r1,r1,#SUP_MODE
	msr 	cpsr_cxsf,r1
	mrs 	r12,spsr
	stmia	r0!,{r12-r14}

;return mode,
	msr 	cpsr_cxsf,r10

    
;restore A5 Register and exit sleep process 
	ldr 	r0, =gLpsRegSave  ; restore registers 
	ldmfd	r0!, {r4-r14}
	dsb
	isb 
		
; enable ap seep sleep	
	ldr 	r0,=IDL_SYSCTRL2
	ldr	    r2,=0x49444c45 			     ; set 0x49444c45
	str 	r2,[r0] 		             ; enable deep sleep
	
;	ENDIF     ;// IF :DEF:ARM_CORE_POWER_DOMAIN
    


; // enable arm core stop
ARM_STANDBY
	dsb
	wfi
	nop
	nop
	nop
	nop
	nop
	nop
	bx		lr
	;mov 	pc, lr
	nop

	
SLEEP_FLAG
    DCD      MCU_is_sleep
AHB_FLAG
    DCD      ahb_sleep_flag
SLEEP_STEP
    DCD      s_sleep_step




	CODE32
	EXPORT remap_and_reboot
;0:mapped to external memory;1:on-chip ROM
remap_and_reboot
    LDR  r1, =REMAP
    MOV  r2, #1
    STR  r2, [r1]
    MOV  pc, r0


;    IF :DEF:ARM_CORE_POWER_DOMAIN    
gLpsRegSave 	;all 40 
	space 4*11	;1.r4-14
	space 4*1	;2.spsr
	space 4*5	;3.cp15 - MMU control, ttb0,ttb1 dacr, context ID
	space 4*4	;4.more cp15 -pmnc, ccnt, pmn0, pmn1
	space 4*1	;5.cacr
	space 4*1	;6.vectors_base
	space 4*1	;7.vectors_base_monitor 
	space 4*1	;8.cpsr
	space 4*3	;9.sys<r13,r14,spsr>
	space 4*3	;10.irq<r13,r14,spsr>
	space 4*3	;11.abort<r13,r14,spsr>
	space 4*3	;12.und<r13,r14,spsr>
	space 4*3	;13.svc<r13,r14,spsr>
gLpsRegSaveEnd 

    
;    ENDIF   
  END
