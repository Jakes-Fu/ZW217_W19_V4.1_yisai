;/*****************************************/
; arm_mc_chroma8x8_dx2dy0_rv8.s 
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

par3			RN		5
par4			RN		6
pred			RN		7

srcPitch_sub8	RN		2
i				RN		14

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		BILINEAR_FILTER_1PIX	$src_add, $p0, $p1
		ldrb	$p1, [pRef], $src_add
		smlabb	$p0, $p0, par3, par4
		add		pred, $p1, $p1, lsl #2
		add		pred, pred, $p0
		mov		$p0, pred, asr #3
		strb	$p0, [pPred], #1
		
		MEND   

;/***************************************************/
;arm_mc_chroma4x4_dx2dy0_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma8x8_dx2dy0_rv8 	
		
arm_mc_chroma8x8_dx2dy0_rv8

		stmfd	sp!, {r4 - r7, r14} 
		
		mov		par3, #3
		mov		par4, #4

		sub		srcPitch_sub8, srcPitch, #8
		mov		i, #8
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pp0, [pRef], #1
				
		;//pix0
		ldrb	pp1, [pRef], #1
		smlabb	pp0, pp0, par3, par4
		add		pred, pp1, pp1, lsl #2
		add		pred, pred, pp0
		mov		pp0, pred, asr #3
		strb	pp0, [pPred], #1
					
		BILINEAR_FILTER_1PIX	#1, pp1, pp2;//pix1
		BILINEAR_FILTER_1PIX	#1, pp2, pp3;//pix2
		BILINEAR_FILTER_1PIX	#1, pp3, pp4;//pix3
		BILINEAR_FILTER_1PIX	#1, pp4, pp5;//pix4
		BILINEAR_FILTER_1PIX	#1, pp5, pp6;//pix5
		BILINEAR_FILTER_1PIX	#1, pp6, pp7;//pix6
		BILINEAR_FILTER_1PIX	srcPitch_sub8, pp7, pp8;//pix7
		
		subs	i, i, #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r7, pc}
		
	ENDIF	
				
		END