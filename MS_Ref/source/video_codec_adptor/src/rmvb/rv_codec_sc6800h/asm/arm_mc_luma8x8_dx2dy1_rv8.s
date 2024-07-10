;/*****************************************/
; arm_mc_luma8x8_dx2dy1_rv8.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_SIZE			EQU		16
BLOCK_SIZE		EQU		8

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2

pn1				RN		3
pp0				RN		4
pp1				RN		5
pp2				RN		6
pp3				RN		3
pp4				RN		4
pp5				RN		5
pp6				RN		6
pp7				RN		3
pp8				RN		4
pp9				RN		5

par6			RN		7
par128			RN		8

i				RN		11
pClipTab		RN		14

pTmpBfr			RN		9
srcPitch_sub10	RN		2
pRefTemp		RN		2

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		FOUR_TAPS_FILTER_1PIX_H	$p0, $p1, $p2, $p3, $src_off
		ldrb	$p3, [pRef], $src_off
		add		r14, $p1, $p2, lsl #1
		rsb		$p0, $p0, r14, lsl #1
		rsb		r10, $p3, r14, lsl #2
		add		$p0, $p0, r10
		strh	$p0, [pTmpBfr], #2
		
		MEND   
		
		MACRO
		FOUR_TAPS_FILTER_1PIX_V	$p0, $p1, $p2, $p3, $dst_off
		ldrsh	$p3, [pRefTemp], #(2*BLOCK_SIZE)
		add		r10, $p2, $p1, lsl #1
		smlabb	r10, r10, par6, par128
		strb	r12, [pPred, #$dst_off]
		add		$p0, $p0, $p3
		rsb		$p0, $p0, r10
		ldrb	r12, [pClipTab, $p0, asr #8]
				
		MEND   

;/***************************************************/
;arm_mc_luma8x8_dx2dy1_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma8x8_dx2dy1_rv8 	
		IMPORT	g_rgiRvClipTab
		IMPORT	g_int_bfr
				
arm_mc_luma8x8_dx2dy1_rv8

		stmfd	sp!, {r4 - r12, r14} 
		
		ldr		pTmpBfr, =g_int_bfr
			
		sub		pRef, pRef, srcPitch
		sub		pRef, pRef, #1
		sub		srcPitch_sub10, srcPitch, #10
		mov		i, #11
ROW_LOOP_H
		;/***********load six pixeles***************/
		ldrb	pn1, [pRef], #1
		ldrb	pp0, [pRef], #1
		ldrb	pp1, [pRef], #1
		
		;//pix0
		ldrb	pp2, [pRef], #1
		add		r14, pp0, pp1, lsl #1
		rsb		pn1, pn1, r14, lsl #1
		rsb		r10, pp2, r14, lsl #2
		add		pn1, pn1, r10
		strh	pn1, [pTmpBfr], #2
	;//	FOUR_TAPS_FILTER_1PIX_H	pn1, pp0, pp1, pp2, #1;//pix0
		FOUR_TAPS_FILTER_1PIX_H	pp0, pp1, pp2, pp3, #1;//pix1
		FOUR_TAPS_FILTER_1PIX_H	pp1, pp2, pp3, pp4, #1;//pix2
		FOUR_TAPS_FILTER_1PIX_H	pp2, pp3, pp4, pp5, #1;//pix3		
		FOUR_TAPS_FILTER_1PIX_H	pp3, pp4, pp5, pp6, #1;//pix4
		FOUR_TAPS_FILTER_1PIX_H	pp4, pp5, pp6, pp7, #1;//pix5
		FOUR_TAPS_FILTER_1PIX_H	pp5, pp6, pp7, pp8, #1;//pix6
		FOUR_TAPS_FILTER_1PIX_H	pp6, pp7, pp8, pp9, srcPitch_sub10;//pix7
		
		subs	i, i, #1
		bne		ROW_LOOP_H  
		
		ldr		r14, =g_rgiRvClipTab
		ldr		pClipTab, [r14, #0]
		
		mov		par6, #6
		mov		par128, #128
		
		sub		pTmpBfr, pTmpBfr, #176	;//-11*16
		add		pPred, pPred, #7
		mov		i, #7
ROW_LOOP_V
		add		pRefTemp, pTmpBfr, i, lsl #1
		ldrsh	pn1, [pRefTemp], #(2*BLOCK_SIZE)
		ldrsh	pp0, [pRefTemp], #(2*BLOCK_SIZE)
		ldrsh	pp1, [pRefTemp], #(2*BLOCK_SIZE)
		
		;//pix0
		ldrsh	pp2, [pRefTemp], #(2*BLOCK_SIZE)
		add		r10, pp1, pp0, lsl #1
		smlabb	r10, r10, par6, par128
		add		pn1, pn1, pp2
		rsb		pn1, pn1, r10
		ldrb	r12, [pClipTab, pn1, asr #8]
		
		FOUR_TAPS_FILTER_1PIX_V	pp0, pp1, pp2, pp3, (0*MB_SIZE);//pix1
		FOUR_TAPS_FILTER_1PIX_V	pp1, pp2, pp3, pp4, (1*MB_SIZE);//pix2
		FOUR_TAPS_FILTER_1PIX_V	pp2, pp3, pp4, pp5, (2*MB_SIZE);//pix3		
		FOUR_TAPS_FILTER_1PIX_V	pp3, pp4, pp5, pp6, (3*MB_SIZE);//pix4
		FOUR_TAPS_FILTER_1PIX_V	pp4, pp5, pp6, pp7, (4*MB_SIZE);//pix5
		FOUR_TAPS_FILTER_1PIX_V	pp5, pp6, pp7, pp8, (5*MB_SIZE);//pix6
		FOUR_TAPS_FILTER_1PIX_V	pp6, pp7, pp8, pp9, (6*MB_SIZE);//pix7
				
		subs	i, i, #1
		strb	r12, [pPred, #(7*MB_SIZE)]
		sub		pPred, pPred, #1	
		bge		ROW_LOOP_V
	
		ldmfd	sp!, {r4 - r12, pc}	
		
	ENDIF
			
		END