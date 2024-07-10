    IF (:DEF:PLATFORM_SC6531EFM)
    IMPORT CHIP_InitAdi
    ENDIF

    PRESERVE8
    AREA    Boot, CODE, READONLY

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs
Mode_SVC_NO_IRQ EQU     0xD3 ; 0x13 or ox80 or 0x40
I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

BIT18           EQU     1<<18
BIT19           EQU     1<<19
BIT20           EQU     1<<20
BIT21           EQU     1<<21

; --- System memory locations
    ENTRY
    IMPORT  stack_bottom        
    EXPORT  Reset_Handler
    CODE32
Reset_Handler
    

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

; Enter SVC mode and set up the SVC stack pointer
    MOV     a1, #Mode_SVC_NO_IRQ           ; Build SVC mode CPSR
    MSR     CPSR_c, a1                            ; Enter SVC mode    
    LDR     a2, [pc, #SVC_STACK-.-8]        ; Pickup SVC mode stack address                                            
    LDR     a2, [a2,#0]                         
                                            ; max RAM size of out chip is 8M Bits
    MOV     sp, a2                    ; Setup SVC MODE stack pointer
    IMPORT  vector_set
    BL vector_set  

    IF (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UWS6121E)
    ;Initialize the pSRAM
    IMPORT  Chip_Init
    BL     Chip_Init
    ENDIF

    IF (:DEF:PLATFORM_SC6530)    
    IMPORT __vfp_enable
    BL __vfp_enable
    ENDIF

do_next
    IMPORT __main
    ; --- Now enter the C code
    B      __main   ; note use B not BL, because an application will never return this way
              
    EXPORT __user_initial_stackheap        
__user_initial_stackheap  
    LDR  r0, =SVC_STACK
    BX		lr      
        
SVC_STACK                               
    DCD     stack_bottom
        
    PRESERVE8
    AREA    JUMP, CODE, READONLY
    EXPORT  JumpToTarget
JumpToTarget    
    BX		r0


    
    END
