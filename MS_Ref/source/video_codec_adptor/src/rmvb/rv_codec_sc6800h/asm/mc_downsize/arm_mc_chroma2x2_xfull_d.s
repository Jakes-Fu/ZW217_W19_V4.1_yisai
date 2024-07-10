;/**************************************************/
;arm_mc_chroma2x2_xfull_d.s
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
dy 				RN 		3

dy0				RN 		3
dy1				RN 		4

intp_out		RN		5

pp0				RN		6
pp1				RN		7
pp2				RN		8

reg_tmp0 		RN 		9
reg_tmp1 		RN 		10
cnst_4			RN		14




		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		EXPORT	arm_mc_chroma2x2_xfull_d	
		
arm_mc_chroma2x2_xfull_d

		stmfd	sp!, {r4 - r10, r14} 
		
		rsb		dy1, dy0, #8
		mov		cnst_4, #4
		
		;/***************first column***********************/
		ldrb 	pp0, [pRef]
		ldrb 	pp1, [pRef, src_pitch]			
		ldrb 	pp2, [pRef, src_pitch, lsl #1]			
		
		smlabb	reg_tmp0, pp1, dy0, cnst_4
		smlabb	reg_tmp1, pp2, dy0, cnst_4
		smlabb 	reg_tmp0, pp0, dy1, reg_tmp0
		smlabb	reg_tmp1, pp1, dy1, reg_tmp1		
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred]
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred, #4]
		
		;/***************second column***********************/
		add		pRef, pRef, #1
		ldrb 	pp0, [pRef]
		ldrb 	pp1, [pRef, src_pitch]			
		ldrb 	pp2, [pRef, src_pitch, lsl #1]	
		
		smlabb	reg_tmp0, pp1, dy0, cnst_4
		smlabb	reg_tmp1, pp2, dy0, cnst_4
		smlabb 	reg_tmp0, pp0, dy1, reg_tmp0
		smlabb	reg_tmp1, pp1, dy1, reg_tmp1
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred, #1]
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred, #5]
		
		ldmfd	sp!, {r4 - r10, pc}	
		
		END
		
		
		