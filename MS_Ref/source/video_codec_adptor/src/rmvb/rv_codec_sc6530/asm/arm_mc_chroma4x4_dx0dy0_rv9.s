;/*****************************************/
; arm_mc_chroma4x4_dx0dy0_rv9.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_CHROMA_SIZE	EQU		8

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2
ls_bits			RN		3
rs_bits			RN		4

i				RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
			
		EXPORT	arm_mc_chroma4x4_dx0dy0_rv9 	
		
arm_mc_chroma4x4_dx0dy0_rv9

		stmfd	sp!, {r4 - r6, r14} 

		tst		pRef, #3
		bne		NOT_WORD_ALIGN
				
		;//word align case				
		;//row 0
		ldr		r5, [pRef], srcPitch
		ldr		r6, [pRef], srcPitch	;//bubble removed
		str		r5, [pPred], #MB_CHROMA_SIZE

		;//row 1	
		str		r6, [pPred], #MB_CHROMA_SIZE

		;//row 2
		ldr		r5, [pRef], srcPitch
		ldr		r6, [pRef], srcPitch	;//bubble removed
		str		r5, [pPred], #MB_CHROMA_SIZE

		;//row 3
		str		r6, [pPred], #MB_CHROMA_SIZE
		
		b		arm_mc_chroma4x4_dx0dy0_rv9_END
		
NOT_WORD_ALIGN
		and		r5, pRef, #3
		sub		pRef, pRef, r5
		
		mov		rs_bits, r5, lsl #3
		rsb		ls_bits, rs_bits, #32
		
		mov		i, #4
ROW_LOOP
		ldmia	pRef, {r5, r6}
		add		pRef, pRef, srcPitch	;//bubble removed
		
		mov		r5, r5, lsr rs_bits
		orr		r5, r5, r6, lsl ls_bits
		str		r5, [pPred], #MB_CHROMA_SIZE

		subs	i, i, #1
		bne		ROW_LOOP

arm_mc_chroma4x4_dx0dy0_rv9_END		
		ldmfd	sp!, {r4 - r6, pc} 
		
		END
		