; ============================================================================
; Enable the MMU function
; Spreadtrum Communications Inc.
;
; NOTE:
; 1. The ARM9 data cache must be used together with MMU, while the instruction
;    cache can be used independently.
; 2. This file contain some APIs and  PHY layer fuctions.
; 
; Related Files: mmu.c mmu_cfg.inc mmu_drvapi.h mmu_phy.h
;
; ============================================================================
    INCLUDE mmu_cfg.inc

    AREA mmu_functions, CODE, READONLY
    CODE32

    EXPORT  MMU_FlushTlbEntry
    EXPORT  mmu_proc_get_cr
    EXPORT  MMU_SetTTB
    EXPORT  MMU_EnableIDCM
    EXPORT  MMU_EnableIDCM_AfterSleep
    EXPORT  MMU_DisableIDCM
    EXPORT  MMU_DisableIDCM_BeforeSleep
    EXPORT  MMU_IsEnable

    EXPORT  MMU_DcacheSyncAll
    EXPORT  MMU_IcacheSyncAll
    EXPORT  MMU_SetDomAccess

    EXPORT  MMU_InvalidateTlbSingle
    EXPORT  MMU_DrainWriteBuffer

    EXPORT  MMU_EnableRBit
    EXPORT  MMU_AlignFaultDisable
    EXPORT  MMU_AlignFaultEnable

    EXPORT  MMU_EnableICache


    EXPORT  MMU_PHY_InvalideICACHEALL
    EXPORT  MMU_PHY_DisableICache
    EXPORT  MMU_PHY_EnableICache
    EXPORT  MMU_PHY_ICacheIsEnable

    EXPORT  MMU_PHY_InvalideDCACHEALL
    EXPORT  MMU_PHY_DisableDCache    
    EXPORT  MMU_PHY_EnableDCache
    EXPORT  MMU_PHY_DCacheIsEnable
    EXPORT  MMU_PHY_DCacheSync
  
    EXPORT  MMU_PHY_DmaMapRegion

    EXPORT  MMU_UseDefDOM
    EXPORT  MMU_UseNorDOM

    EXPORT  MMU_MapHighVector
    EXPORT  MMU_MapLowVector

    EXPORT  MMU_AFTER_SLEEP
    
    MACRO 
    MMU_DisableInt
    MRS     a4, CPSR                    ; Pickup current CPSR
    MOV     a3, #0xC0                   ; Build interrupt mask
    ORR     a2, a4, a3                  ; Or-in new interrupt lockout bits
    MSR     CPSR_c, a2                  ; Close all irq and fiq;
    MEND
    
    MACRO 
    MMU_RestoreInt
    MSR     CPSR_c, a4                  ; Restore all irq and fiq; 
    MEND

mmu_proc_get_cr
    mrc p15, 0, r0, c1, c0, 0	
    bx lr
    
MMU_PHY_DCacheSync
    STMFD sp!, {a1, lr}                 ; save lr_USR and non-callee 
tcv_loop 
    MRC p15, 0, r15, c7, c14, 3         ; test and clean 
    BNE tcv_loop
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c10, 4      ; drain the write buffer
    LDMFD sp!, {a1, PC}                 ; restore registers    


MMU_PHY_ICacheIsEnable
    STMFD   sp!, {a1,lr}
    MRC p15, 0, a1, c1, c0, 0           ;Query the state of the instruction cache   
    AND a1,  a1,  #0x1000
    LDMFD   sp!, {a1,PC}               

MMU_PHY_DCacheIsEnable
    STMFD   sp!, {a1,lr}
    MRC p15, 0, a1, c1, c0, 0           ;Query the state of the DATA cache   
    AND a1,  a1,  #0x4
    LDMFD   sp!, {a1,PC}               

MMU_SetTTB
    MCR p15, 0, r0, c2, c0, 0           ; Write TTB register with base address,read Only.
    BX  lr

MMU_IsEnable
    STMFD   sp!, {a2, lr}            ; save lr_USR and non-callee                  
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x1			
    AND     r0, r0, r1
    LDMFD   sp!, {a2, PC}             ; restore registers 

MMU_AFTER_SLEEP
    STMFD   sp!, {r0, lr}            ; save lr_USR and non-callee                  
    MOV     r0, #0                      ;   Invalidate the ICache and DCache
    MCR     p15, 0, r0, c7, c7, 0    
;    MCR     p15, 0, r0, c7, c10, 4      ;   Drain the write buffer
    MCR     p15, 0, r0, c8, c7, 0       ;   Invalidate the TLB    
    LDMFD   sp!, {r0, PC}             ; restore registers 

MMU_EnableIDCM    
    STMFD   sp!, {a1-a4, lr}            ; save lr_USR and non-callee                  
    MMU_DisableInt

    MOV     r1, #0                      ;   Invalidate the ICache and DCache
    MCR     p15, 0, r1, c7, c7, 0    
    MCR     p15, 0, r1, c7, c10, 4      ;   Drain the write buffer
    MCR     p15, 0, r1, c8, c7, 0       ;   Invalidate the TLB
    ;   Domain Access Control: set all domains to Client
    ;   We must set domain access before enble MMU, otherwise Bus-Error will occur!
    LDR     r0, =MMU_DEF_DOM_VALUE    
    MCR     p15, 0, r0, c3, c0, 0
    
    ;   Enable the ICache, DCache, write buffer, MMU and alignment fault checking, align faule
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x127F			
    ORR     r0, r0, r1
    MCR     p15, 0, r0, c1, c0, 0    
    NOP
    NOP
    NOP
    NOP
    NOP

    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 


MMU_EnableIDCM_AfterSleep
    STMFD   sp!, {a1-a4, lr}
    MMU_DisableInt
;   Enable the ICache, DCache, write buffer, MMU and alignment fault checking
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x127F
    ORR     r0, r0, r1
    MCR     p15, 0, r0, c1, c0, 0

    NOP
    NOP
    NOP
    NOP
    NOP
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 


MMU_DisableIDCM
    STMFD   sp!, {a1-a4, lr}               
    MMU_DisableInt
    MRC     p15, 0, r0, c1, c0, 0     ;Disable the ICache, DCache, write buffer, 
                                      ;MMU and alignment fault checking
    LDR     r1, =0x1007               ;for Norflash, need No alignment access.
    BIC     r0, r0, r1
    ORR     r0, r0, #0x78
    MCR     p15, 0, r0, c1, c0, 0

tci_loop0
    MRC p15, 0, r15, c7, c14, 3       ;test clean and invalidate
    BNE tci_loop0

    MOV     r0, #0
    MCR     p15, 0, r0, c7, c7, 0     ;Invalidate the ICache and DCache
    MCR     p15, 0, r0, c7, c10, 4    ;Drain the write buffer
    MCR     p15, 0, r0, c8, c7, 0     ;Invalidate the TLB
    
    NOP
    NOP
    NOP
    NOP
    NOP

    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 


;// note, this function is only called when enter deepsleep(arm/ahb/sdram or arm/sdram)
MMU_DisableIDCM_BeforeSleep
    STMFD   sp!, {a1-a4, lr}               
    MMU_DisableInt

tci_loop1
    MRC p15, 0, r15, c7, c14, 3          ; test clean and invalidate DCACHE
    BNE tci_loop1

    MOV     r0, #0
    MCR     p15, 0, r0, c7, c10, 4        ;   Drain the write buffer

;    MOV  R1, #0x150;   //Wait some cycles       
;Wait1
;    SUB  R1, R1, #0x1
;    CMP  R1, #0x0
;    BNE  Wait1
    
;   Disable the DCache, write buffer, MMU, NOTE, not invalidate and disable ICACHE...
;    MRC     p15, 0, r0, c1, c0, 0
;    LDR     r1, =0x0005
;    BIC     r0, r0, r1
;    ORR     r0, r0, #0x78
;    MCR     p15, 0, r0, c1, c0, 0
    
    NOP
    NOP
    NOP
    NOP
    NOP

    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 
	

MMU_PHY_InvalideDCACHEALL    
    STMFD sp!, {a1, lr}               ; save lr_USR and non-callee
tcv_a 
    MRC p15, 0, r15, c7, c14, 3          ; test clean and invalidate
    BNE tcv_a
    
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c6, 0        ;Invalidate DCache
    MCR     p15, 0, r0, c8, c7, 0        ;flush i+d-TLBs    
 
    LDMFD sp!, {a1, PC}               ; restore registers    


MMU_PHY_InvalideICACHEALL
    STMFD   sp!, {a1, lr}            ; save lr_USR and non-callee
    MOV     r0, #0    
    MCR     p15, 0, r0, c7, c5, 0       ;Invalidate(flush)the ICache
    MCR     p15, 0, r0, c8, c5, 0       ;flush ITLB only
    NOP                                 ;next few instructions may be via cache.
    NOP
    NOP
    NOP
    NOP

    LDMFD   sp!, {a1, PC}             ; restore registers


	
MMU_PHY_DisableICache
    STMFD sp!, {a1, lr}              ; save lr_USR and non-callee
    
    MRC     p15, 0, r0, c1, c0, 0
    BIC     r0, r0, #0x1000             ;disable icache(but not mmu,etc)
    MCR     p15, 0, r0, c1, c0, 0
    MOV     r0, #0
    MCR     p15, 0,r0, c7, c5, 0         ;flush icache

    NOP
    NOP
    NOP
    NOP
    NOP
    
    LDMFD sp!, {a1, PC}              ; restore registers 

MMU_PHY_DisableDCache
    STMFD sp!, {a1, lr}              ; save lr_USR and non-callee

    MRC     p15, 0, r0, c1, c0, 0
    BIC     r0, r0, #0x000C             ;disable DCache And write buffer ,But not
    MCR     p15, 0, r0, c1, c0, 0       ;mmu and alignment faults.
    MOV     r0, #0
    MCR     p15, 0,r0, c7, c6, 0        ;clear data cache   

    LDMFD sp!, {a1, PC}              ;restore registers 


MMU_PHY_EnableICache
    STMFD sp!, {a1, lr}              ;save lr_USR and non-callee
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #0x1000          ;enable ICache
    MCR     p15, 0, r0, c1, c0, 0
    LDMFD sp!, {a1, PC}              ;restore registers 

MMU_EnableICache
    STMFD   sp!, {a1-a4, lr}               
    MMU_DisableInt
    bl MMU_PHY_EnableICache
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 

MMU_PHY_EnableDCache
    STMFD sp!, {a1, lr}              ; save lr_USR and non-callee
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #0x000F             ;enable DCache(also ensures the mmu,aligment fault,
    MCR     p15, 0, r0, c1, c0, 0       ;and write buffer are enabled.
    LDMFD sp!, {a1, PC}              ;restore registers 

	
MMU_SetDomAccess
    STMFD   sp!, {a1-a4, lr}               
    MMU_DisableInt
    MVN r2, r1                 ; invert mask
    BIC r3, r0, r2             ; clear values that are not masked
    MRC p15, 0, r3, c3, c0, 0  ; get domain access register values
    BIC r3, r3, r1             ; mask off bits that change
    ORR r3, r3, r0             ; set bits that change
    MCR p15, 0, r3, c3, c0, 0  ; set domain access register values
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 

MMU_FlushTlbEntry
    mcr	p15, 0, r0, c7, c10, 1;
    bx      lr

MMU_InvalidateTlbSingle
    MCR     p15, 0, r0, c8, c7, 1
    BX      lr
         
MMU_DrainWriteBuffer
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c10, 4
    BX      lr

;============================================================================
; MMU_AlignFaultEnable
; ARGS: NONE
; DESCRIPTION
; Enables the address alignment fault checking
;============================================================================
MMU_AlignFaultEnable
    STMFD   sp!, {a1-a4, lr}
    MMU_DisableInt
    mov     r0, #0
    mrc     p15, 0 , r0, c1, c0, 0
    orr     r0, r0, # 0x2
    mcr     p15, 0 , r0, c1, c0, 0

    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 

;============================================================================
; MMU_AlignFaultDisable
; ARGS: NONE
; DESCRIPTION
; Enables the address alignment fault checking
;============================================================================
MMU_AlignFaultDisable
    STMFD   sp!, {a1-a4, lr}
    MMU_DisableInt
    mov     r0, #0
    mrc     p15, 0 , r0, c1, c0, 0
    bic     r0, r0, # 0x2
    mcr     p15, 0 , r0, c1, c0, 0
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 

;============================================================================
; MMU_EnableRBit
; ARGS :NONE
; DESCRIPTION
;   Enables R bit in MMU control register
;============================================================================
MMU_EnableRBit
    STMFD   sp!, {a1-a4, lr}
    MMU_DisableInt
    mrc    p15, 0, r0, c1, c0, 0 ; read control register
    orr    r0, r0, # (1 << 9)
    mcr    p15, 0, r0, c1, c0, 0 ; write control register
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}     ; restore registers 
;============================================================================
; *	dma_flush_range(start, end)
; *
; *	Clean and invalidate the specified virtual address range.
; *
; *	- start	- virtual start address
; *	- end	- virtual end address
;============================================================================
arm926_dma_flush_range
	bic	r0, r0, #CACHE_DLINESIZE-1

;#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
retry1
        mcr	p15, 0, r0, c7, c14, 1		; clean+invalidate D entry
;#else
;retry1
;	mcr	p15, 0, r0, c7, c6, 1		; invalidate D entry
;#endif
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	retry1
	mcr	p15, 0, r0, c7, c10, 4		; drain WB
	mov	pc, lr

;============================================================================
; *	dma_inv_range(start, end)
; *
; *	Invalidate (discard) the specified virtual address range.
; *	May not write back any entries.  If 'start' or 'end'
; *	are not cache line aligned, those lines must be written
; *	back.
; *
; *	- start	- virtual start address
; *	- end	- virtual end address
; *
; * (same as v4wb)
;============================================================================
arm926_dma_inv_range
;#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
	tst	r0, #CACHE_DLINESIZE-1
	mcrne	p15, 0, r0, c7, c10, 1		; clean D entry
	tst	r1, #CACHE_DLINESIZE-1
	mcrne	p15, 0, r1, c7, c10, 1		; clean D entry
;#endif
	bic	r0, r0, #CACHE_DLINESIZE-1
	
retry2	
        mcr	p15, 0, r0, c7, c6, 1		; invalidate D entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	retry2
	mcr	p15, 0, r0, c7, c10, 4		; drain WB
	mov	pc, lr

;============================================================================
; *	dma_clean_range(start, end)
; *
; *	Clean the specified virtual address range.
; *
; *	- start	- virtual start address
; *	- end	- virtual end address
; *
; *     (same as v4wb)
;============================================================================
arm926_dma_clean_range
;#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
	bic	r0, r0, #CACHE_DLINESIZE-1
	
retry3	
        mcr     p15, 0, r0, c7, c10, 1		; clean D entry
	add     r0, r0, #CACHE_DLINESIZE
	cmp     r0, r1
	blo	retry3
;#endif
	mcr	p15, 0, r0, c7, c10, 4		; drain WB
	mov	pc, lr
 
__DmaMapArea 
	add	r1, r1, r0
	cmp	r2, #1                                          ;DMA_TO_DEVICE
	beq	arm926_dma_clean_range
	bcs	arm926_dma_inv_range
	b	arm926_dma_flush_range

;============================================================================
;
; 	MMU_PHY_DmaMapRegion(start, size, dir)
; 	- start	- kernel virtual start address
; 	- size	- size of region
; 	- dir	- DMA direction
; 
;============================================================================
MMU_PHY_DmaMapRegion
        STMFD   sp!, {r0-r2, lr}
        bl __DmaMapArea
        LDMFD   sp!, {r0-r2, PC}             ; restore registers 


;============================================================================
;
; 	MMU_IcacheSyncAll()
;       This is public interface function
;       Don't depend any RES. because FDL1,FDL2,BOOTLOADER modules can use this.
;============================================================================
MMU_IcacheSyncAll
    STMFD   sp!, {a1-a4, lr}
    MMU_DisableInt

    bl MMU_PHY_DCacheSync
    bl MMU_PHY_InvalideICACHEALL
    
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 

;============================================================================
;
; 	MMU_DcacheSyncAll()
;       This is public interface function
;       Don't depend any RES. because FDL1,FDL2,BOOTLOADER modules can use this.
;============================================================================
MMU_DcacheSyncAll
    STMFD   sp!, {a1-a4, lr}
    MMU_DisableInt
    
    bl MMU_PHY_DCacheSync
    bl MMU_PHY_InvalideDCACHEALL
    
    MMU_RestoreInt
    LDMFD   sp!, {a1-a4, PC}             ; restore registers 





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

    END


