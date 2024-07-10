;/*****************************************/
; arm_mc_luma8x8_dx0dy1_rv8.s 
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

par6			RN		7
par8			RN		8

pRefTemp		RN		9

i				RN		11
pClipTab		RN		14

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32
	IF RV89_DEC_ASM_RV8_EB = 1		
		
		MACRO
		FOUR_TAPS_FILTER_1PIX	$p0, $p1, $p2, $p3, $dst_off
		ldrb	$p3, [pRefTemp], srcPitch
		add		r10, $p2, $p1, lsl #1
		smlabb	r10, r10, par6, par8
		strb	r12, [pPred, #$dst_off]
		add		$p0, $p0, $p3
		rsb		$p0, $p0, r10	
		ldrb	r12, [pClipTab, $p0, asr #4]
				
		MEND   

;/***************************************************/
;arm_mc_luma8x8_dx0dy1_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma8x8_dx0dy1_rv8 	
		IMPORT	g_rgiRvClipTab
		
arm_mc_luma8x8_dx0dy1_rv8

		stmfd	sp!, {r4 - r12, r14} 
		
		ldr		r14, =g_rgiRvClipTab
		ldr		pClipTab, [r14, #0]
		
		mov		par6, #6
		mov		par8, #8

		sub		pRef, pRef, srcPitch
		add		pPred, pPred, #7
		
		mov		i, #7
ROW_LOOP
		add		pRefTemp, pRef, i
		;/***********load six pixeles***************/
		ldrb	pn1, [pRefTemp], srcPitch
		ldrb	pp0, [pRefTemp], srcPitch
		ldrb	pp1, [pRefTemp], srcPitch
		
		;//pix0
		ldrb	pp2, [pRefTemp], srcPitch
		add		r10, pp1, pp0, lsl #1
		smlabb	r10, r10, par6, par8
		add		pn1, pn1, pp2
		rsb		pn1, pn1, r10
		ldrb	r12, [pClipTab, pn1, asr #4]
		FOUR_TAPS_FILTER_1PIX	pp0, pp1, pp2, pp3, (0*MB_SIZE);//pix1
		FOUR_TAPS_FILTER_1PIX	pp1, pp2, pp3, pp4, (1*MB_SIZE);//pix2
		FOUR_TAPS_FILTER_1PIX	pp2, pp3, pp4, pp5, (2*MB_SIZE);//pix3		
		FOUR_TAPS_FILTER_1PIX	pp3, pp4, pp5, pp6, (3*MB_SIZE);//pix4
		FOUR_TAPS_FILTER_1PIX	pp4, pp5, pp6, pp7, (4*MB_SIZE);//pix5
		FOUR_TAPS_FILTER_1PIX	pp5, pp6, pp7, pp8, (5*MB_SIZE);//pix6
		FOUR_TAPS_FILTER_1PIX	pp6, pp7, pp8, pp9, (6*MB_SIZE);//pix7
			
		subs	i, i, #1
		strb	r12, [pPred, #(7*MB_SIZE)]
		sub		pPred, pPred, #1	
		bge		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r12, pc}	
		
	ENDIF
				
		END