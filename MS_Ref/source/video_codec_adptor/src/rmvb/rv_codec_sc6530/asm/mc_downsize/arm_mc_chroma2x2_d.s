;/**************************************************/
;arm_mc_chroma2x2_d.s
;input
;r0: pRef
;r1: pPred
;r2: srcPitch
;r3: dx
;/**************************************************/

;//register mapping
pRef			RN		0
pPred			RN		1
src_pitch		RN 		2
dx 				RN 		3
dy 				RN		4

dd00			RN		3
dd01			RN		4
dd10			RN		5
dd11			RN		6

pp00			RN		7
pp01			RN		8
pp02			RN		9
pp10 			RN		10
pp11			RN		11
pp12			RN		12

reg_tmp0		RN		2
reg_tmp1		RN		7

cnst_32			RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		EXPORT	arm_mc_chroma2x2_d	
		
arm_mc_chroma2x2_d

		stmfd	sp!, {r4 - r12, r14} 
		
		sub		sp, sp, #4		
		str		src_pitch, [sp]	
			
		ldr 	dy, [sp, #44]
		
		rsb		r7, dx, #8		;//(8-dx)
		rsb		r8, dy, #8		;//(8-dy)
		smulbb	dd11, dx, dy
		smulbb	dd10, r7, dy
		
		smulbb	dd01, dx, r8
		smulbb 	dd00, r7, r8
		
		mov 	cnst_32, #32
		
		
		;/************first row*************/
		ldrb	pp00, [pRef]
		ldrb	pp01, [pRef, #1]	
		ldrb	pp02, [pRef, #2]	
		
		add		pRef, pRef, src_pitch
		
		ldrb	pp10, [pRef]
		ldrb	pp11, [pRef, #1]
		ldrb	pp12, [pRef, #2]		
		
		smlabb	r2, pp00, dd00, cnst_32
		smlabb 	r7, pp01, dd00, cnst_32
		
		smlabb	r2, pp01, dd01, r2
		smlabb	r7, pp02, dd01, r7
		
		smlabb	r2, pp10, dd10, r2
		smlabb 	r7, pp11, dd10, r7
		
		smlabb	r2, pp11, dd11, r2
		smlabb	r7, pp12, dd11, r7
		
		mov		r2, r2, lsr #6
		mov		r7, r7, lsr #6		
		
		strb	r2, [pPred]
		strb	r7, [pPred, #1]
		
		;/************second row*************/	
		ldr		src_pitch, [sp], #4
		add		pRef, pRef, src_pitch	
		ldrb	pp00, [pRef]
		ldrb	pp01, [pRef, #1]	
		ldrb	pp02, [pRef, #2]	
		
		smlabb	r2, pp10, dd00, cnst_32
		smlabb 	r10, pp11, dd00, cnst_32
		
		smlabb	r2, pp11, dd01, r2
		smlabb	r10, pp12, dd01, r10
		
		smlabb	r2, pp00, dd10, r2
		smlabb 	r10, pp01, dd10, r10
		
		smlabb	r2, pp01, dd11, r2
		smlabb	r10, pp02, dd11, r10
		
		mov		r2, r2, lsr #6
		mov		r10, r10, lsr #6	
		
		strb	r2, [pPred, #4]
		strb	r10, [pPred, #5]
		
		ldmfd	sp!, {r4 - r12, pc}	
		
		END
		