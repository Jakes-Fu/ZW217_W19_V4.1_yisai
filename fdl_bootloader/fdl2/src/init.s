;;; Copyright ARM Ltd 2000. All rights reserved.
;
; This module performs ROM/RAM remapping (if required), initializes stack pointers and 
; interrupts for each mode, and finally branches to __main in the C library (which 
; eventually calls main()).
;
; On reset, the ARM core starts up in Supervisor (SVC) mode, in ARM state, with IRQ and FIQ disabled.

		PRESERVE8
        AREA    Init, CODE, READONLY


; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_SVC_NO_IRQ EQU     0xD3 ; 0x13 or ox80 or 0x40
I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

    IF :DEF:PLATFORM_SC8800G 
BIGEND_PROT_REG EQU     0x20900290
AHB_CTRL5_REG   EQU     0x20900230
    ENDIF


; --- System memory locations
        ENTRY
        
        EXPORT Reset_Handler
        
	CODE32
Reset_Handler
; Disable MMU,cache and transfer to little_endian mode.     
    MRC p15,0,r0,c1,c0,0
    
    IF (:DEF:CHIP_ENDIAN_BIG)
    ORR r0,r0,#0x80				            ; Set ARM926EJ to big endian mode ;	
    ELSE 
    BIC r0,r0,#0x80				            ; Set ARM926EJ to little endian mode ;			
    ENDIF
    
    IF (:DEF:SC8800S_LITTLE_ENDIAN)
    BIC r0,r0,#1                            ; disable MMU
    LDR r1,=0x1004
    BIC r0,r0,r1                            ; disable cache
    ENDIF

    MCR p15,0,r0,c1,c0,0
    
    ;Set Endian Regs of SC8800G
    IF (:DEF:CHIP_ENDIAN_BIG) :LAND: (:DEF:PLATFORM_SC8800G)
    LDR     R0, =BIGEND_PROT_REG
    MOV     R2, #0xD4
    ORR     R2, R2, #0xC300
    STR     R2,[R0]

    LDR     R2, =AHB_CTRL5_REG
    MOV     R1, #0xff
    ORR     R1, R1, #0x300

    STR     R1, [R2]
    MOV     R1, #0
    STR     R1, [R0]
    ENDIF

    
;   set stack limit to 0
    MOV    R10, #0
        
; --- Initialise stack pointer registers

; Enter SVC mode and set up the SVC stack pointer
    MOV     a1, #Mode_SVC_NO_IRQ            ; Build SVC mode CPSR
    MSR     CPSR_c, a1                      ; Enter SVC mode    
    LDR     a2, =0x31000000			        ; Pickup SVC mode stack address                                                                   
                                            ; max RAM size of out chip is 8M Bits
    MOV     sp, a2                          ; Setup SVC MODE stack pointer
    
; Set up other stack pointers if necessary
        ; ...

; --- Initialise memory system
        ; ...

; --- Initialise critical IO devices
        ; ...

; --- Initialise interrupt system variables here
        ; ...
	
;	LDR   r0, =0x8b00002c
;	LDR	  r1, =0x1ffC000
;	STR	  r1, [r0]
	
; --- change mem R/W period
;	LDR r0, = 0x70000000
;	LDR r1, = 0x0802111F
;	STR r1, [r0]
   
;      IMPORT  main_entry

       IMPORT  __main
; --- Now enter the C code
;      B      main_entry   ; note use B not BL, because an application will never return this way
       B       __main

; --- Now enter the FDL code
; jump to target address
; on entry
; r0 = int startaddr
; no exit
              
        EXPORT  JumpToTarget
JumpToTarget    
        MOV  pc, r0


	EXPORT _just_for_call
_just_for_call
	MOV     a1, #Mode_SVC_NO_IRQ            ; Build SVC mode CPSR
    MSR     CPSR_c, a1                      ; Enter SVC mode    
    MOV		a2, #100
                                            ; max RAM size of out chip is 8M Bits
    MOV     sp, a2                          ; Setup SVC MODE stack pointer
    

    END
