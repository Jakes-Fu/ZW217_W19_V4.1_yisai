;/*****************************************/
; arm_mc_luma16x16_dx1dy0_rv9.s 
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
pp11			RN		4
pp12			RN		5
pp13			RN		6
pp14			RN		7
pp15			RN		8
pp16			RN		3
pp17			RN		4
pp18			RN		5

par52			RN		9
par20			RN		12

i				RN		11
tmp0			RN		14
pClipTab		RN		14
srcPitch_sub20	RN		2

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		SIX_TAPS_FILTER_1PIX	$src_off, $p0, $p1, $p2, $p3, $p4, $p5
		ldrb	$p5, [pRef], $src_off
		strb	r10, [pPred], #1
		add		r10, $p1, $p4
		smlabb	$p0, $p2, par52, $p0
		add		r10, r10, r10, lsl #2
		rsb		r10, r10, $p0
		smlabb	$p0, $p3, par20, $p5
		add		r10, r10, #32
		add		$p0, $p0, r10
		ldrb	r10, [pClipTab, $p0, asr #6]	
	
		MEND   

;/***************************************************/
;arm_mc_luma16x16_dx1dy0_rv9
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma16x16_dx1dy0_rv9 	
		IMPORT	g_rgiRvClipTab
		
arm_mc_luma16x16_dx1dy0_rv9

		stmfd	sp!, {r4 - r12, r14} 
		
		ldr		tmp0, =g_rgiRvClipTab
		ldr		pClipTab, [tmp0, #0]
		
		mov		par52, #52
		mov		par20, #20
		
		sub		srcPitch_sub20, srcPitch, #20
		sub		pRef, pRef, #2
		mov		i, #16
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pn2, [pRef], #1
		ldrb	pn1, [pRef], #1
		ldrb	pp0, [pRef], #1
		ldrb	pp1, [pRef], #1
		ldrb	pp2, [pRef], #1
		
		;//pix0
		ldrb	pp3, [pRef], #1
		add		r10, pn1, pp2
		smlabb	pn2, pp0, par52, pn2
		add		r10, r10, r10, lsl #2
		rsb		r10, r10, pn2
		smlabb	pn2, pp1, par20, pp3
		add		r10, r10, #32
		add		pn2, pn2, r10
		ldrb	r10, [pClipTab, pn2, asr #6]		
		
		SIX_TAPS_FILTER_1PIX	#1, pn1, pp0, pp1, pp2, pp3, pp4;//pix1
		SIX_TAPS_FILTER_1PIX	#1, pp0, pp1, pp2, pp3, pp4, pp5;//pix2
		SIX_TAPS_FILTER_1PIX	#1, pp1, pp2, pp3, pp4, pp5, pp6;//pix3		
		SIX_TAPS_FILTER_1PIX	#1, pp2, pp3, pp4, pp5, pp6, pp7;//pix4
		SIX_TAPS_FILTER_1PIX	#1, pp3, pp4, pp5, pp6, pp7, pp8;//pix5
		SIX_TAPS_FILTER_1PIX	#1, pp4, pp5, pp6, pp7, pp8, pp9;//pix6
		SIX_TAPS_FILTER_1PIX	#1, pp5, pp6, pp7, pp8, pp9, pp10;//pix7
		SIX_TAPS_FILTER_1PIX	#1, pp6, pp7, pp8, pp9, pp10, pp11;//pix8
		SIX_TAPS_FILTER_1PIX	#1, pp7, pp8, pp9, pp10, pp11, pp12;//pix9
		SIX_TAPS_FILTER_1PIX	#1, pp8, pp9, pp10, pp11, pp12, pp13;//pix10
		SIX_TAPS_FILTER_1PIX	#1, pp9, pp10, pp11, pp12, pp13, pp14;//pix11
		SIX_TAPS_FILTER_1PIX	#1, pp10, pp11, pp12, pp13, pp14, pp15;//pix12
		SIX_TAPS_FILTER_1PIX	#1, pp11, pp12, pp13, pp14, pp15, pp16;//pix13
		SIX_TAPS_FILTER_1PIX	#1, pp12, pp13, pp14, pp15, pp16, pp17;//pix14
		SIX_TAPS_FILTER_1PIX	srcPitch_sub20, pp13, pp14, pp15, pp16, pp17, pp18;//pix15
		
		subs	i, i, #1
		strb	r10, [pPred], #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r12, pc}	
				
		END