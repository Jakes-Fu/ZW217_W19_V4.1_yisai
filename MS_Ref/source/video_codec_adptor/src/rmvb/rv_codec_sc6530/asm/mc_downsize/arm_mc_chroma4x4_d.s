;/**************************************************/
;arm_mc_chroma4x4_xfull_d.s
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

pp0				RN		7
pp1				RN		8
pp2				RN		9
pp3				RN		10
pp4				RN		11

row_cnt			RN		12

reg_tmp0 		RN 		12
reg_tmp1 		RN 		2

cnst_32			RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		EXPORT	arm_mc_chroma4x4_d	
		
arm_mc_chroma4x4_d
		stmfd	sp!, {r4 - r12, r14} 
		
		sub		sp, sp, #8
		str		src_pitch, [sp, #0]
		
		mov		row_cnt, #4
		str		row_cnt, [sp, #4]
		
		ldr		dy, [sp, #48]
		
		rsb		r7, dx, #8		;//(8-dx)
		rsb		r8, dy, #8		;//(8-dy)
		smulbb	dd11, dx, dy
		smulbb	dd10, r7, dy
		
		smulbb	dd01, dx, r8
		smulbb 	dd00, r7, r8
		
		mov 	cnst_32, #32
		
		ldr		src_pitch, [sp, #0]
		ldrb	pp0, [pRef, #0]
		ldrb	pp1, [pRef, #1]
		ldrb	pp2, [pRef, #2]
		ldrb	pp3, [pRef, #3]
		ldrb	pp4, [pRef, #4]
		
		add		pRef, pRef, src_pitch
		
ROW_LOOP
		smlabb	reg_tmp0, pp0, dd00, cnst_32			;//reg_tmp0 = pp00*dd00 + 32
		smlabb	reg_tmp1, pp1, dd00, cnst_32			;//reg_tmp1 = pp01*dd00 + 32
		
		smlabb	reg_tmp0, pp1, dd01, reg_tmp0			;//reg_tmp0 = pp00*dd00 + 32 + pp01*dd01
		smlabb	reg_tmp1, pp2, dd01, reg_tmp1			;//reg_tmp1 = pp01*dd00 + 32 + pp02*dd01
		
		ldrb	pp0, [pRef, #0]							;//pp0 = bottom row pp10
		ldrb	pp1, [pRef, #1]							;//pp1 = bottom row pp11
		
		smlabb	reg_tmp0, pp0, dd10, reg_tmp0			;//reg_tmp0 = pp00*dd00 + 32 + pp01*dd01 + pp10*dd10
		smlabb 	reg_tmp1, pp1, dd10, reg_tmp1			;//reg_tmp1 = pp01*dd00 + 32 + pp02*dd01 + pp11*dd10
		
		smlabb	reg_tmp0, pp1, dd11, reg_tmp0			;//reg_tmp0 = pp00*dd00 + 32 + pp01*dd01 + pp10*dd10 + pp11*dd11
		mov		reg_tmp0, reg_tmp0, lsr #6
		strb	reg_tmp0, [pPred], #1
		
		smlabb	reg_tmp0, pp2, dd00, cnst_32			;//reg_tmp0 = pp02*dd00 + 32
		ldrb	pp2, [pRef, #2]							;//pp2 = bottom row pp12
		
		smlabb	reg_tmp0, pp3, dd01, reg_tmp0			;//reg_tmp0 = pp02*dd00 + 32 + pp03*dd01
		
		smlabb	reg_tmp1, pp2, dd11, reg_tmp1			;//reg_tmp1 = pp01*dd00 + 32 + pp02*dd01 + pp11*dd10 + pp12*dd11		
		
		smlabb	reg_tmp0, pp2, dd10, reg_tmp0			;//reg_tmp0 = pp02*dd00 + 32 + pp03*dd01 + pp12*dd10
		
		mov		reg_tmp1, reg_tmp1, lsr #6
		strb	reg_tmp1, [pPred], #1	
				;
		smlabb	reg_tmp1, pp3, dd00, cnst_32			;//reg_tmp1 = pp03*dd00 + 32		
		
		ldrb	pp3, [pRef, #3]							;//pp3: pp13
		
		smlabb	reg_tmp1, pp4, dd01, reg_tmp1			;//reg_tmp1 = pp03*dd00 + 32 + pp04*dd01		
		
		smlabb	reg_tmp0, pp3, dd11, reg_tmp0			;//reg_tmp0 = pp02*dd00 + 32 + pp03*dd01 + pp12*dd10 + pp13*d11
		
		smlabb	reg_tmp1, pp3, dd10, reg_tmp1			;//reg_tmp1 = pp03*dd00 + 32 + pp04*dd01 + pp13*dd10
		
		mov		reg_tmp0, reg_tmp0, lsr #6
		
		strb	reg_tmp0, [pPred], #1
		
		ldrb	pp4, [pRef, #4]		
		
		ldr		row_cnt, [sp, #4]						;//for remove interlock
		
		smlabb	reg_tmp1, pp4, dd11, reg_tmp1			;//reg_tmp1 = pp03*dd00 + 32 + pp04*dd01 + pp13*dd10 + pp14*dd11
		
		subs	row_cnt, row_cnt, #1					;//for remove interlock
		
		mov		reg_tmp1, reg_tmp1, lsr #6
		
		strb	reg_tmp1, [pPred], #1
		
		ldr		src_pitch, [sp, #0]
		;//ldr		row_cnt, [sp, #4]
		
		str		row_cnt, [sp, #4]
		
		add		pRef, pRef, src_pitch
		
		;//subs	row_cnt, row_cnt, #1		
		
		bne		ROW_LOOP		
		
		add		sp, sp, #8
		
		ldmfd	sp!, {r4 - r12, pc}	
		
		END
		
		
		
		
		
		 
		
		







		
		