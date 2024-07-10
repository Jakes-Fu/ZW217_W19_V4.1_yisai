;;; Copyright SPRD. 2011 All rights reserved.
;;steve.zhan
; ============================================================================
; Enable the MMU function
; Spreadtrum Communications Inc.
; For arm v7 arch.
; ============================================================================
    INCLUDE mmu_cfg.inc

    PRESERVE8
    AREA mmu_functions, CODE, READONLY
    CODE32

    IMPORT   outer_inv_range
    IMPORT   outer_clean_range
    IMPORT   outer_flush_range
    IMPORT   outer_flush_all
    IMPORT   outer_inv_all
    IMPORT   outer_disable
    IMPORT   outer_cache_poweroff
    EXPORT  MMU_SetTTB
    EXPORT  MMU_EnableIDCM
    EXPORT  MMU_DisableIDCM
    EXPORT  MMU_IsEnable

    EXPORT  MMU_DcacheSyncAll
    EXPORT  MMU_IcacheSyncAll
    EXPORT  MMU_SetDomAccess;  !No checked.

    EXPORT  MMU_DrainWriteBuffer

    EXPORT  MMU_EnableRBit;    !No checked.
    EXPORT  MMU_AlignFaultDisable
    EXPORT  MMU_AlignFaultEnable

    EXPORT  MMU_EnableICache
    EXPORT  MMU_PHY_EnableICache
    EXPORT  MMU_PHY_InvalideICACHEALL
    EXPORT  MMU_PHY_DisableICache
    EXPORT  MMU_PHY_ICacheIsEnable

    EXPORT  MMU_PHY_InvalideDCACHEALL
    EXPORT  MMU_PHY_InvalideDCACHEALL_DisableDcache
    EXPORT  MMU_PHY_DisableDCache    
    EXPORT  MMU_PHY_EnableDCache
    EXPORT  MMU_PHY_DCacheIsEnable
    EXPORT  MMU_PHY_DCacheSync
    EXPORT  MMU_PHY_CacheInvaliateSingle
    EXPORT  mmu_invalidate_tlb_all
    EXPORT  MMU_InvalidateTlbSingle

    EXPORT  MMU_PHY_DmaMapRegion


    EXPORT  MMU_UseDefDOM  ;!no checked
    EXPORT  MMU_UseNorDOM  ;!no checked

    EXPORT  MMU_MapHighVector
    EXPORT  MMU_MapLowVector

    EXPORT  core_setup
    EXPORT  mmu_proc_get_cr
    EXPORT  mmu_asm_set_prrr
    EXPORT  mmu_asm_set_nmrr
    EXPORT  mmu_dcache_clean_area    
	
_outer_inv_range
    STMFD   sp!, {a1-a4, lr}
    bl outer_inv_range    
    LDMFD   sp!, {a1-a4, pc}

_outer_clean_range
    STMFD   sp!, {a1-a4, lr}
    bl outer_clean_range    
    LDMFD   sp!, {a1-a4, pc}

_outer_flush_range
    STMFD   sp!, {a1-a4, lr}
    bl outer_flush_range    
    LDMFD   sp!, {a1-a4, pc}

_outer_flush_all
    STMFD   sp!, {a1-a4, lr}
    bl outer_flush_all    
    LDMFD   sp!, {a1-a4, pc}

_outer_inv_all
    STMFD   sp!, {a1-a4, lr}
    bl outer_inv_all    
    LDMFD   sp!, {a1-a4, pc}

_outer_disable
    STMFD   sp!, {a1-a4, lr}
    bl outer_disable    
    LDMFD   sp!, {a1-a4, pc}    

    MACRO 
    MMU_DisableInt
    MRS     r8, CPSR                    ; Pickup current CPSR
    MOV     r7, #0xC0                   ; Build interrupt mask
    ORR     r6, r8, r7                  ; Or-in new interrupt lockout bits
    MSR     CPSR_c, r6                  ; Close all irq and fiq;
    MEND
    
    MACRO 
    MMU_RestoreInt
    MSR     CPSR_c, r8                  ; Restore all irq and fiq; 
    MEND

    MACRO  ;use r2,r3 to read
    READ_CACHE_LINE
    mrc	p15, 0, r3, c0, c0, 1	; read ctr
    lsr	r3, r3, #16
    and	r3, r3, #0xf		; cache line size encoding
    mov	r2, #4			; bytes per word
    mov	r2, r2, lsl r3		; actual cache line size
    MEND

; Invalidate Unified TLB
mmu_invalidate_tlb_all
    STMFD   sp!, {a1,r6-r8, lr}
    MMU_DisableInt
    MOV   r0, #0
    MCR   p15, 0, r0, c8, c7, 0
    isb
    NOP
    NOP
    NOP  
    NOP  
    NOP  
    MMU_RestoreInt
    LDMFD   sp!, {a1,r6-r8, pc}

MMU_IcacheSyncAll
    STMFD   sp!, {r0,r6-r8, lr}
    MMU_DisableInt
    mov	r0, #0
    mcr	p15, 0, r0, c7, c5, 0	      ; I+BTB cache invalidate
    bl     _outer_flush_all
    MMU_RestoreInt
    LDMFD   sp!, {r0, r6-r8,PC}             ; restore registers 	

mmu_proc_get_cr
    STMFD   sp!, {r0, lr}
    mrc p15, 0, r0, c1, c0, 0	
    LDMFD   sp!, {r0, pc}

mmu_dcache_clean_area
    STMFD   sp!, {r0,r1,r6-r8, lr}
    MMU_DisableInt

CLEAN_ENTRY
    mcr	 p15, 0, r0, c7, c10, 1		; clean D entry
    add	 r0, r0, #CACHE_DLINESIZE
    subs r1, r1, #CACHE_DLINESIZE
    bhi	 CLEAN_ENTRY
    dsb
    MMU_RestoreInt
    LDMFD   sp!, {r0,r1,r6-r8, PC}             ; restore registers 	

;Invalidate D-cache by set/way,and disable Dcache
MMU_PHY_InvalideDCACHEALL_DisableDcache
    STMFD   sp!, {a1-a4, lr}    
    dmb				        ; ensure ordering with previous memory accesses
    dsb;NOTICE, NEED?
    isb;NOTICE, NEED?
    ;bl MMU_PHY_DCacheSync
    
    MRC p15, 1, r3, c0, c0, 0           ; Read current Cache Size Identification Register
    MOV r1, #0x1FF
    AND r3, r1, r3, LSR #13             ; r3 = (number of sets - 1)
    MOV  r0, #0                         ; r0 -> way counter
way_loop4
    MOV  r1, #0                         ; r1 -> set counter
set_loop4
    MOV  r2, r0, LSL #30                ;
    ORR  r2, r1, LSL #5                 ; r2 -> set/way cache-op format
    MCR  p15, 0, r2, c7, c14, 2          ; clean an Invalidate line described by r2
    ADD  r1, r1, #1                     ; Increment set counter
    CMP  r1, r3                         ; Check if the last set is reached...
    BLE  set_loop4                      ; ...if not, continue the set_loop...
    ADD  r0, r0, #1                     ; ...else, Increment way counter
    CMP  r0, #4                         ; Check if the last way is reached...
    BLT  way_loop4                      ; ...if not, continue the way_loop

     dsb
     isb
     NOP
    NOP
    NOP  
    
    MRC   p15, 0, r0, c1, c0, 0 ; Read Control Register configuration data
    BIC   r0, r0, #(0x1 << 2)   ; Disable D Cache
    MCR   p15, 0, r0, c1, c0, 0 ; Write Control Register configuration data
    
    dsb
    isb
    NOP
    NOP
    NOP
 
    
    LDMFD   sp!, {a1-a4, PC}            ; restore registers 


;Invalidate D-cache by set/way
MMU_PHY_InvalideDCACHEALL
    STMFD   sp!, {a1-a4, lr}    
    dmb				        ; ensure ordering with previous memory accesses
    dsb;NOTICE, NEED?
    isb;NOTICE, NEED?
    bl MMU_PHY_DCacheSync
    
    MRC p15, 1, r3, c0, c0, 0           ; Read current Cache Size Identification Register
    MOV r1, #0x1FF
    AND r3, r1, r3, LSR #13             ; r3 = (number of sets - 1)
    MOV  r0, #0                         ; r0 -> way counter
way_loop1
    MOV  r1, #0                         ; r1 -> set counter
set_loop1
    MOV  r2, r0, LSL #30                ;
    ORR  r2, r1, LSL #5                 ; r2 -> set/way cache-op format
    MCR  p15, 0, r2, c7, c6, 2          ; Invalidate line described by r2
    ADD  r1, r1, #1                     ; Increment set counter
    CMP  r1, r3                         ; Check if the last set is reached...
    BLE  set_loop1                      ; ...if not, continue the set_loop...
    ADD  r0, r0, #1                     ; ...else, Increment way counter
    CMP  r0, #4                         ; Check if the last way is reached...
    BLT  way_loop1                      ; ...if not, continue the way_loop
    
    dsb
    isb
    
    NOP
    NOP
    NOP
    
    LDMFD   sp!, {a1-a4, PC}            ; restore registers 

;Invalidate D-cache by set/way
MMU_PHY_NoSync_InvalideDCACHEALL
    STMFD   sp!, {a1-a4, lr}    
    dmb				        ; ensure ordering with previous memory accesses
    dsb;NOTICE, NEED?
    isb;NOTICE, NEED?
    
    MRC p15, 1, r3, c0, c0, 0           ; Read current Cache Size Identification Register
    MOV r1, #0x1FF
    AND r3, r1, r3, LSR #13             ; r3 = (number of sets - 1)
    MOV  r0, #0                         ; r0 -> way counter
nosync_way_loop
    MOV  r1, #0                         ; r1 -> set counter
nosync_set_loop
    MOV  r2, r0, LSL #30                ;
    ORR  r2, r1, LSL #5                 ; r2 -> set/way cache-op format
    MCR  p15, 0, r2, c7, c6, 2          ; Invalidate line described by r2
    ADD  r1, r1, #1                     ; Increment set counter
    CMP  r1, r3                         ; Check if the last set is reached...
    BLE  nosync_set_loop                ; ...if not, continue the set_loop...
    ADD  r0, r0, #1                     ; ...else, Increment way counter
    CMP  r0, #4                         ; Check if the last way is reached...
    BLT  nosync_way_loop                ; ...if not, continue the way_loop
    
    dsb
    isb
    
    NOP
    NOP
    NOP
    LDMFD   sp!, {a1-a4, PC}            ; restore registers

core_setup
    STMFD   sp!, {a1-r10, lr}
    
    MOV     r0, #0
    mcr     p15, 0, r0, c7, c5, 0           ;ICIALLU-invalidate entire icache,and flushes btc
    
    ;bl	MMU_PHY_InvalideDCACHEALL
    MRC p15, 1, r3, c0, c0, 0           ; Read current Cache Size Identification Register
    MOV r1, #0x1FF
    AND r3, r1, r3, LSR #13             ; r3 = (number of sets - 1)
    MOV  r0, #0                         ; r0 -> way counter
core_way_loop1
    MOV  r1, #0                         ; r1 -> set counter
core_set_loop1
    MOV  r2, r0, LSL #30                ;
    ORR  r2, r1, LSL #5                 ; r2 -> set/way cache-op format
    MCR  p15, 0, r2, c7, c6, 2          ; Invalidate line described by r2
    ADD  r1, r1, #1                     ; Increment set counter
    CMP  r1, r3                         ; Check if the last set is reached...
    BLE  core_set_loop1                 ; ...if not, continue the set_loop...
    ADD  r0, r0, #1                     ; ...else, Increment way counter
    CMP  r0, #4                         ; Check if the last way is reached...
    BLT  core_way_loop1                 ; ...if not, continue the way_loop
    
    dsb
    isb
    
    MOV   r0, #0                            ; SBZ
    MCR   p15, 0, r0, c8, c7, 0             ; TLBIALL - Invalidate entire Unified TLB
    
    mrc	p15, 0, r0, c0, c0, 0		; read main ID register
    and	r10, r0, #0xff000000		; ARM?
    teq	r10, #0x41000000
    beq	ARM_CORE
DEAAD_LOOP
    b DEAAD_LOOP	

ARM_CORE
    and	r5, r0, #0x00f00000		; variant
    and	r6, r0, #0x0000000f	        ; revision
    orr	r6, r6, r5, lsr #20-4		; combine variant and revision
    ubfx	r0, r0, #4, #12			; primary part number
    
    ldr	r10, =0x00000c05		; Cortex-A5 primary part number
    teq	r0, r10
    bne DEAAD_LOOP

    mov	r10, #0
    mcr	p15, 0, r10, c7, c5, 0		; I+BTB cache invalidate
    dsb
    
    mrc	p15, 0, r0, c1, c0, 0
    orr	r0, r0, #0x00002000	        ; using h v;//clear bits 13 (-normal-V-)
    bic	r0, r0, #0x00000007	        ; clear bits 2:0 (-CAM)
    orr	r0, r0, #0x00000002	        ; set bit 1 (--A-) Align
    orr	r0, r0, #0x00000800	        ; set bit 11 (Z---) BTB
    orr	r0, r0, #0x00000400	        ; set bit 10 (Z---) swp instruction
    orr r0, r0, #0x00001000             ; set bit 12 (I--)
    orr	r0, r0, #0x10000000	        ; set bit t  remapping.
    mcr	p15, 0, r0, c1, c0, 0 
    
    mov r10, #0
    mcr	p15, 0, r10, c2, c0, 2		; TTB control register
    mov r4, #0x41                   ; steve.zhan ...PLS NOTICE.....
    mcr	p15, 0, r4, c2, c0,  1		; load TTB1
    
    
    ldr	r5, =0x0
    ldr	r6, =0x0
    mcr	p15, 0, r5, c10, c2, 0
    mcr	p15, 0, r6, c10, c2, 1
    
    mrc	p15, 0, r0, c1, c0, 0		; read control register
    bic	r0, r0, r5			; clear bits them
    orr	r0, r0, r6			; set them
    
    LDMFD   sp!, {a1-r10, PC}                ; restore registers 


MMU_EnableIDCM
        STMFD   sp!, {a1-a4,r6-r8, lr}
        MMU_DisableInt
        
        mrc	p15, 0, r1, c1, c0, 0		;read control reg   
        orr	r1, r1, #(1 << 1)               ;CR_A
        orr	r1, r1, #(1 << 2)               ;CR_C
        bic	r1, r1, #(1 << 11)              ;CR_Z
        orr	r1, r1, #(1 << 12)              ;CR_I
        orr     r1, r1, #(1<<0)                 ;cr_m
        
        ;mov	r2, #0x1f
        ldr	r2, =0x55555555
        mcr	p15, 0, r2, c3, c0, 0		; load domain access register
        
        
;       ;MOV     r2, #0x08                 ; RGN=b01  (outer cacheable write-back cached, write allocate)                                           ; S=0      (translation table walk to non-shared memory)
;        mov     r2, #0x0
;        ORR     r2,r2,#0x1                 ; IRGN=b01 (inner cacheability for the translation table walk is Write-back Write-allocate)
        ; IRGN=b00 (inner cacheability for the translation table walk is non-cacheable)  
;        ORR     r0,r0,r2                    	
;        mcr	p15, 0, r0, c2, c0, 0		; load page table pointer
        
        
        MOV r3, #0                          ; SBZ
        MCR p15, 0, r3, c7, c5, 0           ; ICIALLU - Invalidate entire I Cache, and flushes branch target cache
        
        bl MMU_PHY_NoSync_InvalideDCACHEALL ;during mmu_enable, need not sync d-cache
        
        MOV   r3, #0                         ; SBZ
        MCR   p15, 0, r3, c8, c7, 0          ; TLBIALL - Invalidate entire Unified TLB
        
        ;turn_mmu_on
        mov	r1, r1
        mcr	p15, 0, r1, c1, c0, 0		; write control reg
        mrc	p15, 0, r3, c0, c0, 0		; read id reg
        isb
        mov	r3, r3
        mov	r3, r3
        
        MMU_RestoreInt
        LDMFD   sp!, {a1-a4, r6-r8,PC}             ; restore registers 

mmu_asm_set_prrr
        STMFD   sp!, {r0,r1,r6-r8, lr}       
        MMU_DisableInt
        mrc	p15, 0, r1, c10, c2, 0
        orr     r1, r1, r0	
        mcr	p15, 0, r1, c10, c2, 0	
        MMU_RestoreInt
        LDMFD   sp!, {r0,r1,r6-r8, PC}             ; restore registers 

mmu_asm_set_nmrr
        STMFD   sp!, {r0,r1,r6-r8,  lr}
        MMU_DisableInt
        mrc	p15, 0, r1, c10, c2, 1
        orr     r1, r1, r0
        mcr	p15, 0, r1, c10, c2, 1
        MMU_RestoreInt
        LDMFD   sp!, {r0,r1,r6-r8,  PC}             ; restore registers 


;========================================================================================================

MMU_PHY_ICacheIsEnable
    STMFD   sp!, {lr}
    MRC p15, 0, a1, c1, c0, 0           ;Query the state of the instruction cache   
    AND a1,  a1,  #0x1000
    LDMFD   sp!, {PC}               

MMU_PHY_DCacheIsEnable
    STMFD   sp!, {lr}
    MRC p15, 0, a1, c1, c0, 0           ;Query the state of the DATA cache   
    AND a1,  a1,  #0x4
    LDMFD   sp!, {PC}               

MMU_SetTTB
    STMFD   sp!, {r0,r2,r6-r8,  lr}
    MMU_DisableInt
    ;        MOV     r2, #0x08                 ; RGN=b01  (outer cacheable write-back cached, write allocate)                                           ; S=0      (translation table walk to non-shared memory)
    mov     r2, #0x0
    ORR     r2,r2,#0x1                 ; IRGN=b01 (inner cacheability for the translation table walk is Write-back Write-allocate)
    ; IRGN=b00 (inner cacheability for the translation table walk is non-cacheable)  
    ORR     r0,r0,r2                    	
    mcr	p15, 0, r0, c2, c0, 0		; load page table pointer
    MMU_RestoreInt
    LDMFD   sp!, {r0,r2,r6-r8, PC}                ; restore registers 


MMU_IsEnable
    STMFD   sp!, {r0,r1, lr}            ; save lr_USR and non-callee                  
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x1			
    AND     r0, r0, r1
    LDMFD   sp!, {r0,r1, PC}             ; restore registers 


MMU_DisableIDCM
    STMFD   sp!, {a1-a4,r6-r8, lr}               
    MMU_DisableInt
    bl      v7_flush_dcache_all
    bl      outer_cache_poweroff
    bl      MMU_PHY_InvalideICACHEALL

    mrc	p15, 0, r1, c1, c0, 0		;read control reg   
    bic	r1, r1, #(1 << 1)               ;CR_A
    bic	r1, r1, #(1 << 2)               ;CR_C
    bic	r1, r1, #(1 << 11)              ;CR_Z
    bic	r1, r1, #(1 << 12)              ;CR_I
    bic r1, r1, #(1<<0)                 ;cr_m
        
    mov	r1, r1
    mcr	p15, 0, r1, c1, c0, 0		; write control reg
    mrc	p15, 0, r3, c0, c0, 0		; read id reg
    dmb
    dsb
    isb
    mov	r3, r3
    mov	r3, r3

    MMU_RestoreInt
    LDMFD   sp!, {a1-a4,r6-r8, PC}             ; restore registers 



	
MMU_PHY_DisableICache
    STMFD   sp!, {r0, lr}    
    MRC   p15, 0, r0, c1, c0, 0 ; Read Control Register configuration data
    BIC   r0, r0, #(0x1 << 12)  ; Disable I Cache
    MCR   p15, 0, r0, c1, c0, 0 ; Write Control Register configuration data

    dsb
    isb
    NOP
    NOP
    NOP
    
    LDMFD sp!, {r0, PC}              ; restore registers 

MMU_PHY_DisableDCache
    STMFD   sp!, {r0, lr}
    
    MRC   p15, 0, r0, c1, c0, 0 ; Read Control Register configuration data
    BIC   r0, r0, #(0x1 << 2)   ; Disable D Cache
    MCR   p15, 0, r0, c1, c0, 0 ; Write Control Register configuration data
    
    dsb
    isb
    NOP
    NOP
    NOP

    LDMFD   sp!, {r0, PC}             ; restore registers 


MMU_PHY_EnableICache
    STMFD sp!, {r0, lr}              ;save lr_USR and non-callee
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #0x1000          ;enable ICache
    MCR     p15, 0, r0, c1, c0, 0
    dsb
    isb
    NOP
    NOP
    NOP
    LDMFD sp!, {r0, PC}              ;restore registers 

MMU_EnableICache
    STMFD   sp!, {r0,r6-r8, lr}
    MMU_DisableInt
    MRC   p15, 0, r0, c1, c0, 0 ; Read Control Register configuration data
    ORR   r0, r0, #(0x1 << 12)  ; Enable I Cache
    MCR   p15, 0, r0, c1, c0, 0 ; Write Control Register configuration data
    dsb
    isb
    NOP
    NOP
    NOP
    MMU_RestoreInt
    LDMFD   sp!, {r0,r6-r8, PC}             ; restore registers 

MMU_PHY_EnableDCache
    STMFD   sp!, {r0, r6-r8,lr}
    MRC   p15, 0, r0, c1, c0, 0 ; Read Control Register configuration data
    ORR   r0, r0, #(0x1 << 2)   ; Enable D Cache
    MCR   p15, 0, r0, c1, c0, 0 ; Write Control Register configuration data
    
    dsb
    isb
    NOP
    NOP
    NOP
    LDMFD   sp!, {r0, r6-r8,PC}             ; restore registers 

	
MMU_SetDomAccess
    STMFD   sp!, {a1-a4, r6-r8,lr}               
    MMU_DisableInt
    MVN r2, r1                 ; invert mask
    BIC r3, r0, r2             ; clear values that are not masked
    MRC p15, 0, r3, c3, c0, 0  ; get domain access register values
    BIC r3, r3, r1             ; mask off bits that change
    ORR r3, r3, r0             ; set bits that change
    MCR p15, 0, r3, c3, c0, 0  ; set domain access register values
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4,r6-r8, PC}             ; restore registers 


MMU_InvalidateTlbSingle
    MCR     p15, 0, r0, c8, c7, 1   
    dsb
    isb
    BX      lr
         
MMU_DrainWriteBuffer
    dsb
    isb
    BX      lr

;============================================================================
; MMU_AlignFaultEnable
; ARGS: NONE
; DESCRIPTION
; Enables the address alignment fault checking
;============================================================================
MMU_AlignFaultEnable
    STMFD   sp!, {a1-a4,r6-r8, lr}
    MMU_DisableInt
    mov     r0, #0
    mrc     p15, 0 , r0, c1, c0, 0
    orr     r0, r0, # 0x2
    mcr     p15, 0 , r0, c1, c0, 0

    MMU_RestoreInt
    LDMFD   sp!, {a1-a4,r6-r8, PC}             ; restore registers 

;============================================================================
; MMU_AlignFaultDisable
; ARGS: NONE
; DESCRIPTION
; Enables the address alignment fault checking
;============================================================================
MMU_AlignFaultDisable
    STMFD   sp!, {a1-a4,r6-r8, lr}
    MMU_DisableInt
    mov     r0, #0
    mrc     p15, 0 , r0, c1, c0, 0
    bic     r0, r0, # 0x2
    mcr     p15, 0 , r0, c1, c0, 0
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, r6-r8,PC}             ; restore registers 

;============================================================================
; MMU_EnableRBit
; ARGS :NONE
; DESCRIPTION
;   Enables R bit in MMU control register
;============================================================================
MMU_EnableRBit
    STMFD   sp!, {a1-a4, lr}   
    LDMFD   sp!, {a1-a4, PC}     ; restore registers 


v7_dma_flush_range
    STMFD   sp!, {a1-a4, lr}

    READ_CACHE_LINE
    sub	r3, r2, #1
    bic	r0, r0, r3
    
retry_c
    mcr	p15, 0, r0, c7, c14, 1		; clean & invalidate D / U line
    add	r0, r0, r2
    cmp	r0, r1
    blo	retry_c                               ;;!NOTICE, blo -> blt
    dsb
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 


v7_dma_inv_range
    STMFD   sp!, {a1-a4, lr}

    READ_CACHE_LINE
    sub	r3, r2, #1
    tst	r0, r3
    bic	r0, r0, r3
    mcrne	p15, 0, r0, c7, c14, 1		; clean & invalidate D / U line
    
    tst	r1, r3
    bic	r1, r1, r3
    mcrne	p15, 0, r1, c7, c14, 1		; clean & invalidate D / U line
retry_r
    mcr	p15, 0, r0, c7, c6, 1		        ; invalidate D / U line
    add	r0, r0, r2
    cmp	r0, r1
    blo	retry_r                                   ;;!NOTICE, blo -> blt
    dsb
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 

v7_dma_clean_range
    STMFD   sp!, {a1-a4, lr}
    READ_CACHE_LINE
    
    sub	r3, r2, #1
    bic	r0, r0, r3
retry_b
    mcr	p15, 0, r0, c7, c10, 1		;clean D / U line
    add	r0, r0, r2
    cmp	r0, r1
    blo	retry_b                              ;!NOTICE, blo -> blt
    dsb
    LDMFD   sp!, {a1-a4, PC}            ; restore registers 


_dma_clean_range
    STMFD   sp!, {lr}
    ;bl _outer_clean_range
    bl v7_dma_clean_range
    bl _outer_clean_range
    LDMFD   sp!, {pc}

_dma_inv_range
    STMFD   sp!, {lr}
    ;bl _outer_inv_range
    bl v7_dma_inv_range
    bl _outer_inv_range
    LDMFD   sp!, {pc}

_dma_flush_range
    STMFD   sp!, {lr}
    ;bl	       _outer_flush_range
    bl	       v7_dma_flush_range
    bl	       _outer_flush_range
    LDMFD   sp!, {pc}

__DmaMapArea 
    add	r1, r1, r0
    cmp	r2, #1                                          ;DMA_TO_DEVICE
    beq	_dma_clean_range
    bcs	_dma_inv_range
    b	_dma_flush_range
	
;============================================================================
;
; 	MMU_PHY_DmaMapRegion(start, size, dir)
; 	- start	- kernel virtual start address
; 	- size	- size of region
; 	- dir	- DMA direction
; 
;============================================================================
MMU_PHY_DmaMapRegion
    STMFD   sp!, {r0-r4, lr}
    bl __DmaMapArea
    LDMFD   sp!, {r0-r4, PC}             ; restore registers 


;============================================================================
;
; 	MMU_DcacheSyncAll()
;       This is public interface function
;       Don't depend any RES. because FDL1,FDL2,BOOTLOADER modules can use this.
;============================================================================
MMU_DcacheSyncAll
    STMFD   sp!, {a1-a4,r6-r8, lr}
    MMU_DisableInt

    bl MMU_IcacheSyncAll
    bl MMU_PHY_DCacheSync
    bl _outer_flush_all
;    bl MMU_PHY_InvalideDCACHEALL
    
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4,r6-r8, PC}             ; restore registers 


MMU_UseDefDOM
    STMFD   sp!, {a1, lr}
    LDR     r0, =MMU_DEF_DOM_VALUE    
    MCR     p15, 0, r0, c3, c0, 0

    LDMFD   sp!, {a1, PC}             ; restore registers 


MMU_UseNorDOM
    STMFD   sp!, {a1, lr}
    LDR     r0, =MMU_NOR_DOM_VALUE    
    MCR     p15, 0, r0, c3, c0, 0

    LDMFD   sp!, {a1, PC}             ; restore registers 



MMU_MapHighVector
    STMFD sp!, {a1, lr}              ;save lr_USR and non-callee
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #0x2000          ;map to high vector addr.
    MCR     p15, 0, r0, c1, c0, 0
    LDMFD sp!, {a1, PC}              ;restore registers 
    
MMU_MapLowVector
    STMFD sp!, {a1, lr}              ;save lr_USR and non-callee
    MRC     p15, 0, r0, c1, c0, 0
    BIC     r0, r0, #0x2000          ;map to low vector addr.
    MCR     p15, 0, r0, c1, c0, 0
    LDMFD sp!, {a1, PC}              ;restore registers 




;	Flush the whole D-cache.
;	- mm    - mm_struct describing address space
v7_flush_dcache_all
        STMFD   sp!, {a1-a4, lr}
	dmb					; ensure ordering with previous memory accesses
;===================================================================        
; Invalidate D-cache by set/way
; Note: for Cortex-A5, there is no instruction for invalidating
; the whole D-cache. Need to invalidate line by line. 
;===================================================================
; Read cache size from the Cache Size Identification Register
        MRC p15, 1, r3, c0, c0, 0           ; Read current Cache Size Identification Register
        MOV r1, #0x1FF
        AND r3, r1, r3, LSR #13             ; r3 = (number of sets - 1)
        MOV  r0, #0                         ; r0 -> way counter
way_loop2
        MOV  r1, #0                         ; r1 -> set counter
set_loop2
        MOV  r2, r0, LSL #30                ;
        ORR  r2, r1, LSL #5                 ; r2 -> set/way cache-op format
        MCR  p15, 0, r2, c7, c14, 2         ; clean and Invalidate line described by r2
        ADD  r1, r1, #1                     ; Increment set counter
        CMP  r1, r3                         ; Check if the last set is reached...
        BLE  set_loop2                       ; ...if not, continue the set_loop...
        ADD  r0, r0, #1                     ; ...else, Increment way counter
        CMP  r0, #4                         ; Check if the last way is reached...
        BLT  way_loop2                       ; ...if not, continue the way_loop


        dsb
        isb
        
        NOP
        NOP
        NOP
	
	LDMFD   sp!, {a1-a4, PC}             ; restore registers 	


;	v7_flush_cache_all()
; *	Flush the entire cache system.
; *  The data cache flush is now achieved using atomic clean / invalidates
; *  working outwards from L1 cache. This is done using Set/Way based cache
; *  maintainance instructions.
; *  The instruction cache can still be invalidated back to the point of
; *  unification in a single instruction.
MMU_PHY_DCacheSync
    STMFD   sp!, {r0, lr}

	bl	v7_flush_dcache_all
	bl    _outer_flush_all
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0 ; I+BTB cache invalidate
	
	dsb
	isb

       NOP
       NOP
       NOP	
       
    LDMFD   sp!, {r0, PC}             ; restore registers 	


;	MMU_PHY_CacheInvaliateSingle()
MMU_PHY_CacheInvaliateSingle
    STMFD   sp!, {a1-a4, lr}
    MOV r2, #0x20
    add	r3, r0, r1
loop_i   
	mcr	p15, 0, r0, c7, c5, 1 ; I cache invalidate
	mcr	p15, 0, r0, c7, c6, 1 ; D cache invalidate
	add r0, r0, r2
	CMP  r0, r3
    BLT  loop_i 
    
	dsb
	isb
	
    NOP
    NOP
    NOP	
       
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 	


    ;;; +++ bug 1350964 /1341705/1349442
    PRESERVE8
    AREA mmu_functions_in_ddr, CODE, READONLY
    CODE32

MMU_PHY_InvalideICACHEALL
    STMFD   sp!, {r0, lr}            ; save lr_USR and non-callee
    MOV     r0, #0    
    MCR     p15, 0, r0, c7, c5, 0       ;Invalidate(flush)the ICache
    MCR     p15, 0, r0, c8, c7, 0       ;TLBIALL - Invalidate entire Unified TLB
    
    dsb
    isb
    NOP
    NOP
    NOP

    LDMFD   sp!, {r0, PC}             ; restore registers
    ;;; -- bug 1350964 /1341705/1349442

   END
	
