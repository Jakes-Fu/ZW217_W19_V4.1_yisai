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

	
		EXPORT		arm_mc_luma8x8_dx0dy0_rv9
		
arm_mc_luma8x8_dx0dy0_rv9
		stmfd	sp!, {r4 - r7, r14}
		
		tst		pRef, #3
		bne		NOT_WORD_ALIGN
				
		mov		i, #8
align_row_loop
		ldmia	pRef, {r5, r6}
		add		pRef, pRef, srcPitch;	//buble removed
		stmia	pPred, {r5, r6}		
		add		pPred, pPred, #MB_SIZE
		
		subs	i, i, #1
		bne		align_row_loop
		
		b		arm_mc_luma8x8_dx0dy0_rv9_END
		
NOT_WORD_ALIGN
		and		r5, pRef, #3
		sub		pRef, pRef, r5
		
		mov		ls_bits, r5, lsl #3
		rsb		rs_bits, ls_bits, #32
		
		mov		i, #8
ROW_LOOP
		ldmia	pRef, {r5, r6, r7}
		add		pRef, pRef, srcPitch	;//bubble removed
		
		mov		r5, r5, lsl ls_bits
		orr		r5, r5, r6, lsr rs_bits
		
		mov		r6, r6, lsl ls_bits
		orr		r6, r6, r7, lsr rs_bits
		
		stmia	pPred, {r5, r6}
		add		pPred, pPred, #MB_SIZE

		subs	i, i, #1
		bne		ROW_LOOP

arm_mc_luma8x8_dx0dy0_rv9_END		
		ldmfd	sp!, {r4 - r7, pc} 
		
		END
		
		
		