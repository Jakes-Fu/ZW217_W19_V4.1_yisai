;/**************************************************/
;arm_mc_chroma2x2_yfull_d.s
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

intp_out		RN		5

pp0				RN		6
pp1				RN		7
pp2				RN		8

reg_tmp0 		RN 		9
reg_tmp1 		RN 		10
cnst_4			RN		14




		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		EXPORT	arm_mc_chroma2x2_yfull_d	
		
arm_mc_chroma2x2_yfull_d

		stmfd	sp!, {r4 - r10, r14} 
		
		rsb		dx1, dx0, #8
		mov		cnst_4, #4
		
		;/***************first row***********************/
		ldrb 	pp0, [pRef]
		ldrb 	pp1, [pRef, #1]			
		ldrb 	pp2, [pRef, #2]			
		
		smlabb	reg_tmp0, pp1, dx0, cnst_4
		smlabb	reg_tmp1, pp2, dx0, cnst_4
		smlabb 	reg_tmp0, pp0, dx1, reg_tmp0
		smlabb	reg_tmp1, pp1, dx1, reg_tmp1		
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred]
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred, #1]
		
		;/***************second row***********************/
		add		pRef, pRef, src_pitch
		ldrb 	pp0, [pRef]
		ldrb 	pp1, [pRef, #1]			
		ldrb 	pp2, [pRef, #2]	
		
		smlabb	reg_tmp0, pp1, dx0, cnst_4
		smlabb	reg_tmp1, pp2, dx0, cnst_4
		smlabb 	reg_tmp0, pp0, dx1, reg_tmp0
		smlabb	reg_tmp1, pp1, dx1, reg_tmp1
		
		mov		reg_tmp0, reg_tmp0, lsr #3
		strb 	reg_tmp0, [pPred, #4]
		mov		reg_tmp1, reg_tmp1, lsr #3
		strb 	reg_tmp1, [pPred, #5]
		
		ldmfd	sp!, {r4 - r10, pc}	
		
		END
		
		
		