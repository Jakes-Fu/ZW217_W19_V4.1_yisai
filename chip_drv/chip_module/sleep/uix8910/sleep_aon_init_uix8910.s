

	AREA  |C$$code|, CODE, READONLY
|x$codeseg| DATA
	


LPS_AP_RESUME_MAGIC     	EQU      0xD8E1A000
PRIMCPU_RESUME_MAGIC_ADDR   EQU      0x50094028
DDR_DMC_PHY_SAVE_END 		EQU	     0x70753159


REG_LPDDR_PHY_BASE          EQU      0x08801000
REG_PSRAM_PHY_BASE          EQU      0x08801800
REG_DMC_CTRL_BASE           EQU 	 0x08803000
IDLE_RES8					EQU      0x50094020
IDLE_RES9					EQU      0x50094024

;    IF :DEF:ARM_CORE_POWER_DOMAIN 

    EXPORT UNI_WAKEUP_ENTRY
	IMPORT DDR_PARAM_SAVE_ADDR
	IMPORT phy_off_val
	IMPORT dmc_off_val
	IMPORT pad_off_val
	IMPORT cmd_off_val


;    PRESERVE8      ;Added for RVDSv4.0 Specially                                              
;    AREA  lps_sect,CODE,READONLY
    CODE32

UNI_WAKEUP_ENTRY
; // check WAKEUP jump flag and addr (right now in SRAM)
; // off = 0x00, MAGIC, off = 0x04, ADDR
;wakeup_while
;	b   wakeup_while
; check spiflash config
; enable icache.

    mov     r0, #0    
    MCR     p15, 0, r0, c7, c5, 0       ;Invalidate(flush)the ICache
    MCR     p15, 0, r0, c8, c7, 0       ;TLBIALL - Invalidate entire Unified TLB
    dsb
    isb
    
    mrc	p15, 0, r1, c1, c0, 0		;read control reg   
    orr	r1, r1, #(1 << 12)          ;CR_I
	mcr	p15, 0, r1, c1, c0, 0		;write control reg
        
	ldr	r2, =PRIMCPU_RESUME_MAGIC_ADDR
	ldr	r5, =LPS_AP_RESUME_MAGIC
	ldr	r3, [r2]	  	;//get flag 
	cmp	r3, r5		  	;//compare flag
	bne	not_wakeup
	ldr	r0, =0xAAAAAAAA
	str	r0, [r2]		;//@ clear flag
	ldr	r6, [r2, #4]	;//@get addr

; // add delay to wait ddr stable, this should be done before dmc and
; //	 phy recovery.The time should be larger than 1024/ddr_freq.Now rom
; //	 has consume 10us, so it's not necessary to delay.
; // mov	r0, #0x100000
; // loop1:	subs	r0, #1
; // bne	loop1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; 1.config phy register
;	@Here, we should restore dmc registers and pll setup.
	ldr	r3, =DDR_DMC_PHY_SAVE_END	;ldr	r3, aon_save_end
	ldr r4, =REG_PSRAM_PHY_BASE
	ldr r0, =phy_off_val  	;ldr	r0, aon_save_addr
loop_copy_to_phyreg	
	ldr r1, [r0]
	cmp	r1, r3
	beq	loop_copy_to_phyreg_end
	ldr	r2, [r0, #4]
	str	r2, [r4, r1]
	add	r0, r0, #8	
	
; only check run step
	add r1, r1,r4
	ldr r2,=IDLE_RES9
	str r1,[r2]
	
	b	loop_copy_to_phyreg
loop_copy_to_phyreg_end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 2.set ddr/psram pad config
	ldr r0, =pad_off_val  	
loop_copy_to_padreg
	ldr r1, [r0]
	cmp	r1, r3
	beq	loop_copy_to_padreg_end
	ldr	r2, [r0, #4]
	str	r2, [r1]
	add	r0, r0, #8

; only check run step
	add r1, r1,r4
	ldr r2,=IDLE_RES9
	str r1,[r2]
	
	b loop_copy_to_padreg
loop_copy_to_padreg_end	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 3.copy dmc to regster
	ldr r4, =REG_DMC_CTRL_BASE
	ldr r0, =dmc_off_val
	
loop_copy_to_dmcreg
	ldr r1, [r0]
	cmp	r1, r3
	beq	loop_copy_to_dmcreg_end
	ldr	r2, [r0, #4]
	str	r2, [r4, r1]
	add	r0, r0, #8

; only check run step
	add r1, r1,r4
	ldr r2,=IDLE_RES9
	str r1,[r2]
	
	b	loop_copy_to_dmcreg
loop_copy_to_dmcreg_end




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 4.
	ldr r0, =cmd_off_val  	
loop_copy_to_cmmreg
	ldr r1, [r0]
	cmp	r1, r3
	beq	ddr_copy_end
	ldr	r2, [r0, #4]
	str	r2, [r1]
	add	r0, r0, #8
	
	mov  r4,#100
loop_while	
	subs r4,#1
	bne  loop_while
	
	
; only check run step
	ldr r2,=IDLE_RES9	
	str r1,[r2]	
	b loop_copy_to_cmmreg	
ddr_copy_end


;; apa5 run trace point
    ldr     r0, =IDLE_RES9
    mov     r1, #2                          
    str     r1,[r0]
    
	dsb
	bx r6


not_wakeup
	b     not_wakeup
	mov     r0, #0
	bx r0

 
  END
