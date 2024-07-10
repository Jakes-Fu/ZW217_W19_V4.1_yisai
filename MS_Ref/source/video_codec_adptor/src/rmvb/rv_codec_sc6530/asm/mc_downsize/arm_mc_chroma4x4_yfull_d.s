;/**************************************************/
;arm_mc_chroma4x4_yfull_d.s
;input
;r0: pRef
;r1: pPred
;r2: srcPitch
;r3: dx
;/**************************************************/

;register mapping
pRef			RN		0
pPred			RN		1
src_pitch		RN 		2
dx 				RN 		3

dx0				RN 		3
dx1				RN 		4

pp0				RN		5
pp1				RN		6
pp2				RN		7
pp3				RN		5
pp4				RN		6

row_cnt			RN		8

reg_tmp0 		RN 		9
reg_tmp1 		RN 		10
cnst_4			RN		14




		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		EXPORT	arm_mc_chroma4x4_yfull_d	
		
arm_mc_chroma4x4_yfull_d
		stmfd	sp!, {r4 - r10, r14} 
		
		rsb		dx1, dx0, #8
		mov		cnst_4, #4
		
		mov		row_cnt, #4
		
ROW_LOOP	
		ldrb 	pp0, [pRef]
		ldrb 	pp1, [pRef, #1]			
		ldrb 	pp2, [pRef, #2]	
				
		;/***********point 0, 1***********/
		smlabb	reg_tmp0, pp1, dx0, cnst_4
		smlabb	reg_tmp1, pp2, dx0, cnst_4
		smlabb 	reg_tmp0, pp0, dx1, reg_tmp0
		smlabb	reg_tmp1, pp1, dx1, reg_tmp1		
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred], #1
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred], #1	
		
		;/***********point 2, 3***********/
		ldrb 	pp3, [pRef, #3]			
		ldrb 	pp4, [pRef, #4]	
		
		smlabb	reg_tmp0, pp3, dx0, cnst_4
		smlabb	reg_tmp1, pp4, dx0, cnst_4
		smlabb 	reg_tmp0, pp2, dx1, reg_tmp0
		smlabb	reg_tmp1, pp3, dx1, reg_tmp1		
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred], #1
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred], #1	
		
		add 	pRef, pRef, src_pitch
		
		subs	row_cnt, row_cnt, #1
		
		bne		ROW_LOOP
		
		ldmfd	sp!, {r4 - r10, pc}			
		
		END
		
		
		