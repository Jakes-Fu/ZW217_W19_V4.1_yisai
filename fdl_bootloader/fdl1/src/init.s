;;; Copyright ARM Ltd 2000. All rights reserved.
;
; This module performs ROM/RAM remapping (if required), initializes stack pointers and 
; interrupts for each mode, and finally branches to __main in the C library (which 
; eventually calls main()).
;
; On reset, the ARM core starts up in Supervisor (SVC) mode, in ARM state, with IRQ and FIQ disabled.

        PRESERVE8
        AREA    Init, CODE, READONLY
	IF (:DEF:PLATFORM_SC8800G)
SVC_STACK_TEMP  EQU     0x40008000                      ;// SVC stack
	ELSE
SVC_STACK_TEMP  EQU     0x40006800                      ;// SVC stack
	ENDIF
	IMPORT  Chip_Init

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_SVC_NO_IRQ EQU     0xD3 ; 0x13 or ox80 or 0x40
I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled
BIT_ENDIAN      EQU     0x80 
; --- System memory locations

        ENTRY

        IMPORT  stack_bottom
        IMPORT  __main
        EXPORT  Reset_Handler
	CODE32
Reset_Handler
; Disable MMU,cache and transfer to big_endian mode.    
    MRC p15,0,r0,c1,c0,0
    
    
    BIC r0,r0,#1                            ; disable MMU
    LDR r1,=0x1004
    BIC r0,r0,r1                            ; disable cache
     MCR p15,0,r0,c1,c0,0        

;    set stack limit to 0             
     MOV    R10, #0
     
; --- Initialise stack pointer registers

	;//REMAP
	IF (:DEF:PLATFORM_SC8800H) :LOR: (:DEF:PLATFORM_SC8800G)
	LDR    r0, =0x20900218
	LDR    r1, [r0, #0]
	ORR    r1, r1, #1 
	STR    r1, [r0, #0]
	ENDIF
	IF :DEF:PLATFORM_SC6800H 
	LDR    r0, =0x20900014
	LDR    r1, [r0, #0]
	ORR    r1, r1, #1 
	STR    r1, [r0, #0]
	ENDIF
	
; Enter SVC mode and set up the SVC stack pointer
    MOV     a1, #Mode_SVC_NO_IRQ            ; Build SVC mode CPSR
    MSR     CPSR_c, a1                      ; Enter SVC mode    
    
    LDR    sp, =SVC_STACK_TEMP
    
	;// Initialize the SDRAM
    BL     Chip_Init  
    
    LDR     a2, [pc, #SVC_STACK-.-8]        ; Pickup SVC mode stack address                                            
    LDR     a2, [a2,#0]                         
                                            ; max RAM size of out chip is 8M Bits
    MOV     sp, a2                          ; Setup SVC MODE stack pointer
    
; Set up other stack pointers if necessary
        ; ...

; --- Initialise memory system
        ; ...

; --- Initialise critical IO devices
        ; ...
; Pull up USB LDO only under usb boot mode       
;if sc6800d
;	*(volatile unsigned long *) 0x8b00003C &= ~(1<<12);
;	*(volatile unsigned long *) 0x8b00003C |= 1<<13;
;else
;	*(volatile unsigned long *) 0x8b00003C &= ~(1<<13);
;	*(volatile unsigned long *) 0x8b00003C |= 1<<12;
    IF :DEF:NAND_FDL_SC8800H
    MOV   a1,#0x8b000000
    LDR   a2,[a1,#0x18]
    MOV   a2,a2,lsl #5
    MOV   a2,a2,lsr #30
    CMP   a2,#1
    BNE   do_next
    
    LDR   a1,=0x8b00003c
    LDR   a2,[a1]

    BIC   a2,a2,#0x1000
    ORR   a2,a2,#0x2000
    ELSE
    BIC   a2,a2,#0x2000
    ORR   a2,a2,#0x1000

    STR   a2,[a1]
	ENDIF            
do_next
; --- Initialise interrupt system variables here
        ; ...
	
;	LDR   r0, =0x8b00002c
;	LDR	  r1, =0x1ffC000
;	STR	  r1, [r0]
	
; --- change mem R/W period
;	LDR r0, = 0x70000000
;	LDR r1, = 0x0802111F
;	STR r1, [r0]
; --- Now enter the C code
      B       __main   ; note use B not BL, because an application will never return this way

        

; --- Now enter the FDL code
; jump to target address
; on entry
; r0 = int startaddr
; no exit

	EXPORT __user_initial_stackheap
__user_initial_stackheap
    MOV     pc, lr
              
        EXPORT  JumpToTarget
JumpToTarget    
        MOV  pc, r0
        
SVC_STACK                               
    DCD     stack_bottom

    EXPORT g_ahb_clk
    EXPORT g_emc_clk
    EXPORT s_colum
    EXPORT sdram_parameters
    EXPORT s_sdram_raw_cfg
g_ahb_clk
    DCD     0
g_emc_clk
		DCD     0
s_colum
		DCD     0   
sdram_parameters
		DCD     0
s_sdram_raw_cfg
		DCD     0
		DCD     0
		DCD     0
		DCD     0
		DCD     0
		DCD     0
		DCD     0
		DCD     0

    END
