	IMPORT     MCU_is_sleep
	IMPORT     ahb_sleep_flag
		
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
SLEEP_CTL      EQU      0x20900010
REMAP          EQU      0x20900014

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
    LDR  R1, =SLEEP_CTL
    LDR  R2, [R1]
    ORR  R2, R2, #0x30
    STR  R2, [R1, #0]   ; MCU System sleep enable

ARM_STANDBY
;   // Wait some cycles       
    MOV  R1, #0x20
Wait0
    SUB  R1, R1, #0x1
    CMP  R1, #0x0
    BNE  Wait0
    
;   // Set ARM9 into low power state
    LDR  R1, =SLEEP_CTL
    LDR  R2, [R1]
    ORR  R2, R2, #0x1
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
             
Call_Exit
    LDMFD  r13!, {r0-r4} ; May not need
    BX     LR     	  ; return
	
SLEEP_FLAG
    DCD      MCU_is_sleep
AHB_FLAG
    DCD      ahb_sleep_flag


	CODE32
	EXPORT remap_and_reboot
;0:mapped to external memory;1:on-chip ROM
remap_and_reboot
    LDR  r1, =REMAP
    MOV  r2, #1
    STR  r2, [r1]
    MOV  pc, r0
  
  END
