;/**************************************************/
;arm_mc_chroma4x4_xfull_d.s
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

dy0				RN 		3
dy1				RN 		4

pp0				RN		5
pp1				RN		6
pp2				RN		7
pp3				RN		5
pp4				RN		6

col_cnt			RN		8

reg_tmp0 		RN 		9
reg_tmp1 		RN 		10
cnst_4			RN		14




		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		EXPORT	arm_mc_chroma4x4_xfull_d	
		
arm_mc_chroma4x4_xfull_d
		stmfd	sp!, {r4 - r10, r14} 
		
		rsb		dy1, dy0, #8
		mov		cnst_4, #4
		
		mov		col_cnt, #4
		
COL_LOOP	
		ldrb 	pp0, [pRef], src_pitch
		ldrb 	pp1, [pRef], src_pitch			
		ldrb 	pp2, [pRef], src_pitch	
				
		;/***********point 0, 1***********/
		smlabb	reg_tmp0, pp1, dy0, cnst_4
		smlabb	reg_tmp1, pp2, dy0, cnst_4
		smlabb 	reg_tmp0, pp0, dy1, reg_tmp0
		smlabb	reg_tmp1, pp1, dy1, reg_tmp1		
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred, #0]
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred, #4]	
		
		;/***********point 2, 3***********/
		ldrb 	pp3, [pRef], src_pitch			
		ldrb 	pp4, [pRef], #1	
		
		smlabb	reg_tmp0, pp3, dy0, cnst_4
		smlabb	reg_tmp1, pp4, dy0, cnst_4
		smlabb 	reg_tmp0, pp2, dy1, reg_tmp0
		smlabb	reg_tmp1, pp3, dy1, reg_tmp1		
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred, #8]
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred, #12]	
		
		sub		pRef, pRef, src_pitch, lsl #2
		
		add		pPred, pPred, #1
		
		subs	col_cnt, col_cnt, #1
		
		bne		COL_LOOP
		
		ldmfd	sp!, {r4 - r10, pc}			
		
		END