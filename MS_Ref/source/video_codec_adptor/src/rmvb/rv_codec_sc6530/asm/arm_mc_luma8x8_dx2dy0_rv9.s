;/*****************************************/
; arm_mc_luma8x8_dx2dy0_rv9.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_SIZE		EQU		16

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2

pn2				RN		3
pn1				RN		4
pp0				RN		5
pp1				RN		6
pp2				RN		7
pp3				RN		8
pp4				RN		3
pp5				RN		4
pp6				RN		5
pp7				RN		6
pp8				RN		7
pp9				RN		8
pp10			RN		3

par20			RN		9

i				RN		11
tmp0			RN		14
pClipTab		RN		14
srcPitch_sub12	RN		2

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		SIX_TAPS_FILTER_1PIX	$src_off, $p0, $p1, $p2, $p3, $p4, $p5
		ldrb	$p5, [pRef], $src_off
		strb	r10, [pPred], #1
		add		r10, $p2, $p3
		smlabb	r10, par20, r10, $p0
		add		$p0, $p1, $p4
		add		$p0, $p0, $p0, lsl #2
		sub		$p0, $p5, $p0
		add		$p0, $p0, r10
		add		$p0, $p0, #16
		ldrb	r10, [pClipTab, $p0, asr #5]
		
		MEND   

;/***************************************************/
;arm_mc_luma8x8_dx2dy0_rv9
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma8x8_dx2dy0_rv9 	
		IMPORT	g_rgiRvClipTab
		
arm_mc_luma8x8_dx2dy0_rv9

		stmfd	sp!, {r4 - r11, r14} 
		
		ldr		tmp0, =g_rgiRvClipTab
		ldr		pClipTab, [tmp0, #0]

		mov		par20, #20
		sub		srcPitch_sub12, srcPitch, #12
		sub		pRef, pRef, #2
		mov		i, #8
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pn2, [pRef], #1
		ldrb	pn1, [pRef], #1
		ldrb	pp0, [pRef], #1
		ldrb	pp1, [pRef], #1
		ldrb	pp2, [pRef], #1
		
		;//pix0
		ldrb	pp3, [pRef], #1
		add		r10, pp0, pp1
		smlabb	r10, par20, r10, pn2
		add		pn2, pn1, pp2
		add		pn2, pn2, pn2, lsl #2
		sub		pn2, pp3, pn2
		add		pn2, pn2, r10
		add		pn2, pn2, #16
		ldrb	r10, [pClipTab, pn2, asr #5]
		
		SIX_TAPS_FILTER_1PIX	#1, pn1, pp0, pp1, pp2, pp3, pp4;//pix1
		SIX_TAPS_FILTER_1PIX	#1, pp0, pp1, pp2, pp3, pp4, pp5;//pix2
		SIX_TAPS_FILTER_1PIX	#1, pp1, pp2, pp3, pp4, pp5, pp6;//pix3		
		SIX_TAPS_FILTER_1PIX	#1, pp2, pp3, pp4, pp5, pp6, pp7;//pix4
		SIX_TAPS_FILTER_1PIX	#1, pp3, pp4, pp5, pp6, pp7, pp8;//pix5
		SIX_TAPS_FILTER_1PIX	#1, pp4, pp5, pp6, pp7, pp8, pp9;//pix6
		SIX_TAPS_FILTER_1PIX	srcPitch_sub12, pp5, pp6, pp7, pp8, pp9, pp10;//pix7
		
		subs	i, i, #1
		strb	r10, [pPred], #9
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r11, pc}	
				
		END