;/*****************************************/
; arm_mc_luma16x16_dx2dy0_rv8.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_SIZE		EQU		16

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
pp10			RN		6
pp11			RN		3
pp12			RN		4
pp13			RN		5
pp14			RN		6
pp15			RN		3
pp16			RN		4
pp17			RN		5

par6			RN		7
par8			RN		8

i				RN		9
pClipTab		RN		14

srcPitch_sub18	RN		2

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		FOUR_TAPS_FILTER_1PIX	$p0, $p1, $p2, $p3, $src_off
		ldrb	$p3, [pRef], $src_off
		add		r10, $p1, $p2, lsl #1
		smlabb	r10, r10, par6, par8
		strb	r12, [pPred], #1
		add		$p0, $p0, $p3
		rsb		$p0, $p0, r10
		ldrb	r12, [pClipTab, $p0, asr #4]
		
		MEND   

;/***************************************************/
;arm_mc_luma16x16_dx2dy0_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma16x16_dx2dy0_rv8 	
		IMPORT	g_rgiRvClipTab
		
arm_mc_luma16x16_dx2dy0_rv8

		stmfd	sp!, {r4 - r10, r12, r14} 
		
		ldr		r14, =g_rgiRvClipTab
		ldr		pClipTab, [r14, #0]
		
		mov		par6, #6
		mov		par8, #8
		sub		srcPitch_sub18, srcPitch, #18

		sub		pRef, pRef, #1
		mov		i, #16
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pn1, [pRef], #1
		ldrb	pp0, [pRef], #1
		ldrb	pp1, [pRef], #1
		
		;//pix0
		ldrb	pp2, [pRef], #1
		add		r10, pp0, pp1, lsl #1
		smlabb	r10, r10, par6, par8
		add		pn1, pn1, pp2
		rsb		pn1, pn1, r10
		ldrb	r12, [pClipTab, pn1, asr #4]
				
		FOUR_TAPS_FILTER_1PIX	pp0, pp1, pp2, pp3, #1;//pix1
		FOUR_TAPS_FILTER_1PIX	pp1, pp2, pp3, pp4, #1;//pix2
		FOUR_TAPS_FILTER_1PIX	pp2, pp3, pp4, pp5, #1;//pix3		
		FOUR_TAPS_FILTER_1PIX	pp3, pp4, pp5, pp6, #1;//pix4
		FOUR_TAPS_FILTER_1PIX	pp4, pp5, pp6, pp7, #1;//pix5
		FOUR_TAPS_FILTER_1PIX	pp5, pp6, pp7, pp8, #1;//pix6
		FOUR_TAPS_FILTER_1PIX	pp6, pp7, pp8, pp9, #1;//pix7
		FOUR_TAPS_FILTER_1PIX	pp7, pp8, pp9, pp10, #1;//pix8
		FOUR_TAPS_FILTER_1PIX	pp8, pp9, pp10, pp11, #1;//pix9
		FOUR_TAPS_FILTER_1PIX	pp9, pp10, pp11, pp12, #1;//pix10		
		FOUR_TAPS_FILTER_1PIX	pp10, pp11, pp12, pp13, #1;//pix11
		FOUR_TAPS_FILTER_1PIX	pp11, pp12, pp13, pp14, #1;//pix12
		FOUR_TAPS_FILTER_1PIX	pp12, pp13, pp14, pp15, #1;//pix13
		FOUR_TAPS_FILTER_1PIX	pp13, pp14, pp15, pp16, #1;//pix14
		FOUR_TAPS_FILTER_1PIX	pp14, pp15, pp16, pp17, srcPitch_sub18;//pix15
	
		subs	i, i, #1
		strb	r12, [pPred], #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r10, r12, pc}	
		
	ENDIF
			
		END