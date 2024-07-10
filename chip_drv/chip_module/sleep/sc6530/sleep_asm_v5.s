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

;   while ((*(volatile uint32 *)0x20000008 & 0x04));
;  //Wait the BIT2(rf_sts_smem_busy) of EMC sts0 equal to 0x0 for EMC idle
    LDR    R1, =0x20000008
Get_val
    LDR    R2, [R1]  
    TST    R2, #0x4
    BNE    Get_val 
    
MCU_Stop
;   // Set MCU system sleep enable
    LDR  R1, =SLEEP_CTL1
    LDR  R2, [R1]
    ORR  R2, R2, #0x18
    STR  R2, [R1, #0]   ; MCU System sleep enable

ARM_STANDBY
;   // Wait some cycles       
    MOV  R1, #0x20
Wait0
    SUB  R1, R1, #0x1
    CMP  R1, #0x0
    BNE  Wait0
    
;   set s_sleep_step |= BIT_5;
    LDR     R1, [pc, #SLEEP_STEP-.-8] 
    LDR     R2, [R1]
    ORR     R2, R2, #0x20
    STR     R2, [R1]
    
;   // Set ARM9 into low power state
    LDR  R1, =SLEEP_CTL0
    LDR  R2, [R1]
    ORR  R2, R2, #0x2
    STR  R2, [R1, #0]   ; arm core sleep
    NOP
    NOP
    NOP 
    
;   // Wait some cycles       
    MOV  R1, #0x20
Wait1
    SUB  R1, R1, #0x1
    CMP  R1, #0x0
    BNE  Wait1

;   set s_sleep_step |= BIT_6;
    LDR     R1, [pc, #SLEEP_STEP-.-8] 
    LDR     R2, [R1]
    ORR     R2, R2, #0x40
    STR     R2, [R1]
             
Call_Exit
    LDMFD  r13!, {r0-r4} ; May not need
    BX     LR     	  ; return
	
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
  
  END
