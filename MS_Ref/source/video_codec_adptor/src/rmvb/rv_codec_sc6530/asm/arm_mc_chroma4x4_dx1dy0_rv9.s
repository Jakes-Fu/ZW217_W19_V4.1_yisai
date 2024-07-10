;/*****************************************/
; arm_mc_chroma4x4_dx1dy0_rv9.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_CHROMA_SIZE	EQU		8

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2

pp0				RN		3
pp1				RN		4
pp2				RN		3
pp3				RN		4
pp4				RN		3

srcPitch_sub4	RN		2
i				RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		BILINEAR_FILTER_1PIX	$src_add, $p0, $p1, $dst_add
		ldrb	$p1, [pRef], $src_add
		add		$p0, $p0, $p0, lsl #1
		add		$p0, $p0, $p1
		add 	$p0, $p0, #1
		mov		$p0, $p0, asr #2
		strb	$p0, [pPred], #$dst_add
		
		MEND   

;/***************************************************/
;arm_mc_chroma4x4_dx1dy0_rv9
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma4x4_dx1dy0_rv9 	
		
arm_mc_chroma4x4_dx1dy0_rv9

		stmfd	sp!, {r4, r14} 

		sub		srcPitch_sub4, srcPitch, #4
		mov		i, #4
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pp0, [pRef], #1
		
		;//pix0
		ldrb	pp1, [pRef], #1
		add		pp0, pp0, pp0, lsl #1
		add		pp0, pp0, #1
		add		pp0, pp0, pp1
		mov		pp0, pp0, asr #2
		strb	pp0, [pPred], #1
			
		BILINEAR_FILTER_1PIX	#1, pp1, pp2, 1;//pix1
		BILINEAR_FILTER_1PIX	#1, pp2, pp3, 1;//pix2
		BILINEAR_FILTER_1PIX	srcPitch_sub4, pp3, pp4, 5;//pix3
		
		subs	i, i, #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4, pc}	
				
		END