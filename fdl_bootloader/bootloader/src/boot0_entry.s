;******************************************************************************
;   David.Jia   2007.10.26      share_version_union    
;
;   ARMASM -keep -bi -g -apcs /interwork -fpu None entry.s
;SC6600R    -pd "_BL_NF_SC6600R_ SETL {TRUE}"
;SC6600H    -pd "_BL_NF_SC6600H_ SETL {TRUE}"
;SC6600I    -pd "_BL_NF_SC6600I_ SETL {TRUE}"
;SC6800     -cpu ARM926EJ-S -pd "_BL_NF_SC6800_ SETL {TRUE}"
;******************************************************************************
			PRESERVE8
	    AREA reset, CODE, READONLY

Mode_SVC        EQU     0x13 ; 0x13 or ox80 or 0x40
I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

	IF (:DEF:PLATFORM_SC8800H) :LOR: (:DEF:PLATFORM_SC8800G)
REMAP          EQU        0x20900218
SVC_STACK_TEMP  EQU     0x40008000  
GEN2_ADDR	  	EQU     0x8b00002c
ON_CHIP_RAM_EN  EQU     0xF
	ENDIF   
	IF :DEF:PLATFORM_SC6800H
REMAP          EQU        0x20900014
SVC_STACK_TEMP  EQU     0x40006800                      ;// SVC stack
GEN2_ADDR	  	EQU     0x8b00004c
ON_CHIP_RAM_EN  EQU     0x18000000
    ENDIF
    
    IF :DEF:PLATFORM_SC8800G 
BIGEND_PROT_REG EQU     0x20900290
AHB_CTRL5_REG   EQU     0x20900230
    ENDIF
    
		ENTRY
		
    IMPORT  Chip_Init
;    IMPORT  MMU_Init
    IMPORT  __main

    EXPORT  __vectors
__vectors
    LDR     pc, =reset_handler                  ; Reset goes to startup function
    nop       ; Undefined handler
    nop       ; Software interrupt handler
    nop       ; Prefetch exeception handler
    nop       ; Abort exception handler
    nop       ; Reserved exception handler
    nop       ; IRQ interrupt handler
    nop       ; FIQ interrupt handler
    
    ;nand config of first bootloader page
    nop       ; checksum
    nop       ; nand page type
    nop       ; nand address cycle
    nop       ; nand data bus len
    nop	      ; advance ;not used in 6800H
    nop       ; magic data
    
    ;nand config for hash value
    nop       ;hash value 0 ; secure boot
    nop       ;hash value 1 ; secure boot
    nop       ;hash value 2 ; secure boot
    nop       ;hash value 3 ; secure boot

    IF (:DEF:PLATFORM_SC8800G)
        IF (:DEF:CHIP_ENDIAN_BIG) 
        dcd 0x87654321  ;big-endian flag
        ELSE
            IF (:DEF:NAND_ENDIAN_SWITCH)
            dcd 0x12345678  ;little-endian flag
            ELSE
            dcd 0  ;little-endian flag
            ENDIF
        ENDIF
    ENDIF

reset_handler

;  //Remap
    LDR    R0, =REMAP
    LDR    R1, =0
    STR    R1, [R0]  
    
    MRC p15,0,r0,c1,c0,0
    IF (:DEF:CHIP_ENDIAN_BIG) 
    ORR r0,r0,#0x80				            ; Set ARM926EJ to big endian mode ;	
    ELSE 
    BIC r0,r0,#0x80				            ; Set ARM926EJ to little endian mode ;			
    ENDIF
    
    IF (:DEF:_LITTLE_ENDIAN)
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
    
    IF (:DEF:PLATFORM_SC8800H) :LOR: (:DEF:PLATFORM_SC8800G)
    ELSE
   
;// Enable on chip ram for ARM
    LDR     R0, =GEN2_ADDR
    LDR     R1, =ON_CHIP_RAM_EN
    LDR     R2, [R0]
    ORR     R1, R1, R2
    STR     R1, [R0]
    ENDIF
;   set up temp stack
    MSR     CPSR_c, #Mode_SVC:OR:F_Bit:OR:I_Bit   ;// Close I and F IRQ
    LDR    sp, =SVC_STACK_TEMP
    
;// Initialize the SDRAM
    BL     Chip_Init
;    BL     MMU_Init  
;// Re-set up stack 
;// The sp(0x008f0000-)and mmu table(16k,0x008f0000+) here must be in the reserved region. (0x00900000)
    LDR     sp, =0x003f0000

    B __main

    EXPORT __user_initial_stackheap
__user_initial_stackheap
    MOV     pc, lr

    EXPORT  set_pc
    IMPORT  MMU_DisableIDCM
set_pc
    BL      MMU_DisableIDCM
    MOV     pc, r0
   
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
