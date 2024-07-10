;/*****************************************/
; arm_mc_chroma8x8_dx1dy0_rv8.s 
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
pp5				RN		4
pp6				RN		3
pp7				RN		4
pp8				RN		3

par5			RN		5
par4			RN		6
pred			RN		7

srcPitch_sub8	RN		2
i				RN		14

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		BILINEAR_FILTER_1PIX	$p0, $p1, $src_add
		ldrb	$p1, [pRef], $src_add
		smlabb	$p0, $p0, par5, par4
		add		pred, $p1, $p1, lsl #1
		add		pred, pred, $p0
		mov		$p0, pred, asr #3
		strb	$p0, [pPred], #1
		
		MEND   

;/***************************************************/
;arm_mc_chroma8x8_dx1dy0_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma8x8_dx1dy0_rv8 	
		
arm_mc_chroma8x8_dx1dy0_rv8

		stmfd	sp!, {r4 - r7, r14} 
		
		mov		par5, #5
		mov		par4, #4

		sub		srcPitch_sub8, srcPitch, #8
		mov		i, #8
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pp0, [pRef], #1
				
		;//pix0
		ldrb	pp1, [pRef], #1
		smlabb	pp0, pp0, par5, par4
		add		pred, pp1, pp1, lsl #1
		add		pred, pred, pp0
		mov		pp0, pred, asr #3
		strb	pp0, [pPred], #1
					
		BILINEAR_FILTER_1PIX	pp1, pp2, #1;//pix1
		BILINEAR_FILTER_1PIX	pp2, pp3, #1;//pix2
		BILINEAR_FILTER_1PIX	pp3, pp4, #1;//pix3
		BILINEAR_FILTER_1PIX	pp4, pp5, #1;//pix4
		BILINEAR_FILTER_1PIX	pp5, pp6, #1;//pix5
		BILINEAR_FILTER_1PIX	pp6, pp7, #1;//pix6
		BILINEAR_FILTER_1PIX	pp7, pp8, srcPitch_sub8;//pix3
		
		subs	i, i, #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r7, pc}	
		
	ENDIF
				
		END