;/*****************************************/
; arm_mc_luma8x8_dx0dy1_rv9.s 
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

pRefTemp		RN		9
par52			RN		12

i				RN		11
tmp0			RN		14
pClipTab		RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		SIX_TAPS_FILTER_1PIX	$p0, $p1, $p2, $p3, $p4, $p5, $dst_off
		ldrb	$p5, [pRefTemp], srcPitch
		strb	r10, [pPred, #$dst_off]
		smlabb	$p0, $p2, par52, $p0
		add		r10, $p1, $p4
		add		r10, r10, r10, lsl #2
		rsb		r10, r10, $p5
		add		r10, r10, $p3, lsl #2
		add		r10, r10, $p3, lsl #4
		add		r10, r10, #32
		add		$p0, $p0, r10
		ldrb	r10, [pClipTab, $p0, asr #6]
		
		MEND   

;/***************************************************/
;arm_mc_luma8x8_dx0dy1_rv9
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma8x8_dx0dy1_rv9 	
		IMPORT	g_rgiRvClipTab
		
arm_mc_luma8x8_dx0dy1_rv9

		stmfd	sp!, {r4 - r12, r14} 
		
		ldr		tmp0, =g_rgiRvClipTab
		ldr		pClipTab, [tmp0, #0]
		
		mov		par52, #52

		sub		pRef, pRef, srcPitch, lsl #1
		add		pPred, pPred, #7
		
		mov		i, #7
ROW_LOOP
		add		pRefTemp, pRef, i
		;/***********load six pixeles***************/
		ldrb	pn2, [pRefTemp], srcPitch
		ldrb	pn1, [pRefTemp], srcPitch
		ldrb	pp0, [pRefTemp], srcPitch
		ldrb	pp1, [pRefTemp], srcPitch
		ldrb	pp2, [pRefTemp], srcPitch
		
		;//pix0
		ldrb	pp3, [pRefTemp], srcPitch
		smlabb	pn2, pp0, par52, pn2
		add		r10, pn1, pp2
		add		r10, r10, r10, lsl #2
		rsb		r10, r10, pp3
		add		r10, r10, pp1, lsl #2
		add		r10, r10, pp1, lsl #4
		add		r10, r10, #32
		add		pn2, pn2, r10
		ldrb	r10, [pClipTab, pn2, asr #6]
		
		SIX_TAPS_FILTER_1PIX	pn1, pp0, pp1, pp2, pp3, pp4, (0*MB_SIZE);//pix1
		SIX_TAPS_FILTER_1PIX	pp0, pp1, pp2, pp3, pp4, pp5, (1*MB_SIZE);//pix2
		SIX_TAPS_FILTER_1PIX	pp1, pp2, pp3, pp4, pp5, pp6, (2*MB_SIZE);//pix3		
		SIX_TAPS_FILTER_1PIX	pp2, pp3, pp4, pp5, pp6, pp7, (3*MB_SIZE);//pix4
		SIX_TAPS_FILTER_1PIX	pp3, pp4, pp5, pp6, pp7, pp8, (4*MB_SIZE);//pix5
		SIX_TAPS_FILTER_1PIX	pp4, pp5, pp6, pp7, pp8, pp9, (5*MB_SIZE);//pix6
		SIX_TAPS_FILTER_1PIX	pp5, pp6, pp7, pp8, pp9, pp10,(6*MB_SIZE);//pix7
			
		subs	i, i, #1
		strb	r10, [pPred, #(7*MB_SIZE)]
		sub		pPred, pPred, #1	
		bge		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r12, pc}	
				
		END