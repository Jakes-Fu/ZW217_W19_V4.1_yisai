;;; Copyright ARM Ltd 2000. All rights reserved.
;
; This module performs ROM/RAM remapping (if required), initializes stack pointers and 
; interrupts for each mode, and finally branches to __main in the C library (which 
; eventually calls main()).
;
; On reset, the ARM core starts up in Supervisor (SVC) mode, in ARM state, with IRQ and FIQ disabled.


        AREA    Init, CODE, READONLY

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_SVC_NO_IRQ EQU     0xD3 ; 0x13 or ox80 or 0x40
I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled
; --- System memory locations
        ENTRY
        IMPORT  lcm_heap_addr
	        EXPORT  Reset_Handler
	CODE32
Reset_Handler
; --- Initialise Hardware
; --- Initialise ARM common registers
	MOV		r0, #0
	MOV		r1, #0
	MOV		r2, #0
	MOV		r3, #0
	MOV		r4, #0
	MOV		r5, #0
	MOV		r6, #0
	MOV		r7, #0
	MOV		r8, #0
	MOV		r9, #0
	MOV		r10,#0
	MOV		r11,#0
	MOV		r12,#0
	MOV		sp, #0
	MOV		lr, #0
	
; --- Initialise stack pointer registers

; Enter SVC mode and set up the SVC stack pointer
    MOV     a1, #Mode_SVC_NO_IRQ            ; Build SVC mode CPSR
    MSR     CPSR_c, a1                      ; Enter SVC mode    
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
;	*(volatile unsigned long *) 0x8b00003C &= ~(1<<13);
;	*(volatile unsigned long *) 0x8b00003C |= 1<<12;

    MOV   a1,#0x8b000000
    LDR   a2,[a1,#0x18]
    MOV   a2,a2,lsl #5
    MOV   a2,a2,lsr #30
    CMP   a2,#1
    BNE   do_next
    
    LDR   a1,=0x8b00003c
    LDR   a2,[a1]
    BIC   a2,a2,#0x2000
    ORR   a2,a2,#0x1000
    STR   a2,[a1]
do_next
; --- Initialise interrupt system variables here
        ; ...

        ;IMPORT  main_entry
        IMPORT __main

; --- Now enter the C code
      B     __main   ; note use B not BL, because an application will never return this way
        
          

     EXPORT __user_initial_stackheap
        
__user_initial_stackheap  
      LDR  r0, =SVC_STACK
      MOV  pc, lr
      
        
SVC_STACK                               
    DCD     lcm_heap_addr

    END