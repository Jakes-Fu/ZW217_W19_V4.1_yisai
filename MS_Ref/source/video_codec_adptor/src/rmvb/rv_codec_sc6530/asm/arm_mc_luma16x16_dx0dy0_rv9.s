;/*******interpolation for mv_x and mv_y is full***********/
;/**************************************************/
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/**************************************************/

MB_SIZE			EQU		16

		AREA	PROGRAM, CODE, READONLY
		
pRef			RN		0
pPred			RN		1 
srcPitch		RN		2
ls_bits			RN		3
rs_bits			RN		4

i				RN		14

	
		EXPORT		arm_mc_luma16x16_dx0dy0_rv9
		
arm_mc_luma16x16_dx0dy0_rv9
		stmfd	sp!, {r4 - r9, r14}
		
		tst		pRef, #3
		bne		NOT_WORD_ALIGN
				
		mov		i, #16
align_row_loop
		ldmia	pRef, {r5, r6, r7, r8}
		add		pRef, pRef, srcPitch;	//buble removed
		stmia	pPred!, {r5, r6, r7, r8}
		
		ldmia	pRef, {r5, r6, r7, r8}
		add		pRef, pRef, srcPitch;	//buble removed
		stmia	pPred!, {r5, r6, r7, r8}				
		
		subs	i, i, #2
		bne		align_row_loop
		
		b		arm_mc_luma16x16_dx0dy0_rv9_END
		
NOT_WORD_ALIGN
		and		r5, pRef, #3
		sub		pRef, pRef, r5
		
		mov		rs_bits, r5, lsl #3
		rsb		ls_bits, rs_bits, #32
		
		mov		i, #16
ROW_LOOP
		ldmia	pRef, {r5, r6, r7, r8, r9}
		add		pRef, pRef, srcPitch	;//bubble removed
		
		mov		r5, r5, lsr rs_bits
		orr		r5, r5, r6, lsl ls_bits
		
		mov		r6, r6, lsr rs_bits
		orr		r6, r6, r7, lsl ls_bits
		
		mov		r7, r7, lsr rs_bits
		orr		r7, r7, r8, lsl ls_bits
		
		mov		r8, r8, lsr rs_bits
		orr		r8, r8, r9, lsl ls_bits
		
		stmia	pPred!, {r5, r6, r7, r8}

		subs	i, i, #1
		bne		ROW_LOOP

arm_mc_luma16x16_dx0dy0_rv9_END		
		ldmfd	sp!, {r4 - r9, pc} 
		
		END
		
		
		