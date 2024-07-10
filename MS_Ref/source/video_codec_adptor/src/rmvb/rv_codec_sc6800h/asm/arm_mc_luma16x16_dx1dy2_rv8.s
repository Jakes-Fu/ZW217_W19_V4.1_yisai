;/*****************************************/
; arm_mc_luma16x16_dx1dy2_rv8.s 
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
pp10			RN		6
pp11			RN		3
pp12			RN		4
pp13			RN		5
pp14			RN		6
pp15			RN		3
pp16			RN		4
pp17			RN		5

par6			RN		7
par128			RN		8

i				RN		11
pClipTab		RN		14

pTmpBfr			RN		9
srcPitch_sub18	RN		2
pRefTemp		RN		2

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		FOUR_TAPS_FILTER_1PIX_H	$p0, $p1, $p2, $p3, $src_off
		ldrb	$p3, [pRef], $src_off
		add		r14, $p2, $p1, lsl #1
		rsb		$p0, $p0, r14, lsl #1
		rsb		r10, $p3, r14, lsl #2
		add		$p0, $p0, r10
		strh	$p0, [pTmpBfr], #2
		
		MEND   
		
		MACRO
		FOUR_TAPS_FILTER_1PIX_V	$p0, $p1, $p2, $p3, $dst_off
		ldrsh	$p3, [pRefTemp], #(MB_SIZE*2)
		add		r10, $p1, $p2, lsl #1
		smlabb	r10, r10, par6, par128
		strb	r12, [pPred, #$dst_off]
		add		$p0, $p0, $p3
		rsb		$p0, $p0, r10
		ldrb	r12, [pClipTab, $p0, asr #8]
				
		MEND   

;/***************************************************/
;arm_mc_luma16x16_dx1dy2_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma16x16_dx1dy2_rv8 	
		IMPORT	g_rgiRvClipTab
		IMPORT	g_int_bfr
				
arm_mc_luma16x16_dx1dy2_rv8

		stmfd	sp!, {r4 - r12, r14} 
		
		ldr		pTmpBfr, =g_int_bfr
			
		sub		pRef, pRef, srcPitch
		sub		pRef, pRef, #1
		sub		srcPitch_sub18, srcPitch, #18
		mov		i, #19
ROW_LOOP_H
		;/***********load six pixeles***************/
		ldrb	pn1, [pRef], #1
		ldrb	pp0, [pRef], #1
		ldrb	pp1, [pRef], #1
		
		;//pix0
		ldrb	pp2, [pRef], #1
		add		r14, pp1, pp0, lsl #1
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
		FOUR_TAPS_FILTER_1PIX_H	pp6, pp7, pp8, pp9, #1;//pix7
		FOUR_TAPS_FILTER_1PIX_H	pp7, pp8, pp9, pp10, #1;//pix8
		FOUR_TAPS_FILTER_1PIX_H	pp8, pp9, pp10, pp11, #1;//pix9		
		FOUR_TAPS_FILTER_1PIX_H	pp9, pp10, pp11, pp12, #1;//pix10
		FOUR_TAPS_FILTER_1PIX_H	pp10, pp11, pp12, pp13, #1;//pix11
		FOUR_TAPS_FILTER_1PIX_H	pp11, pp12, pp13, pp14, #1;//pix12
		FOUR_TAPS_FILTER_1PIX_H	pp12, pp13, pp14, pp15, #1;//pix13
		FOUR_TAPS_FILTER_1PIX_H	pp13, pp14, pp15, pp16, #1;//pix14
		FOUR_TAPS_FILTER_1PIX_H	pp14, pp15, pp16, pp17, srcPitch_sub18;//pix15
	
		subs	i, i, #1
		bne		ROW_LOOP_H  
		
		ldr		r14, =g_rgiRvClipTab
		ldr		pClipTab, [r14, #0]
		
		mov		par6, #6
		mov		par128, #128
		
		sub		pTmpBfr, pTmpBfr, #608	;//-19*32
		add		pPred, pPred, #15
		mov		i, #15
ROW_LOOP_V
		add		pRefTemp, pTmpBfr, i, lsl #1
		ldrsh	pn1, [pRefTemp], #(MB_SIZE*2)
		ldrsh	pp0, [pRefTemp], #(MB_SIZE*2)
		ldrsh	pp1, [pRefTemp], #(MB_SIZE*2)
		
		;//pix0
		ldrsh	pp2, [pRefTemp], #(MB_SIZE*2)
		add		r10, pp0, pp1, lsl #1
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
		FOUR_TAPS_FILTER_1PIX_V	pp7, pp8, pp9, pp10, (7*MB_SIZE);//pix8
		FOUR_TAPS_FILTER_1PIX_V	pp8, pp9, pp10, pp11, (8*MB_SIZE);//pix9
		FOUR_TAPS_FILTER_1PIX_V	pp9, pp10, pp11, pp12, (9*MB_SIZE);//pix10		
		FOUR_TAPS_FILTER_1PIX_V	pp10, pp11, pp12, pp13, (10*MB_SIZE);//pix11
		FOUR_TAPS_FILTER_1PIX_V	pp11, pp12, pp13, pp14, (11*MB_SIZE);//pix12
		FOUR_TAPS_FILTER_1PIX_V	pp12, pp13, pp14, pp15, (12*MB_SIZE);//pix13
		FOUR_TAPS_FILTER_1PIX_V	pp13, pp14, pp15, pp16, (13*MB_SIZE);//pix14
		FOUR_TAPS_FILTER_1PIX_V	pp14, pp15, pp16, pp17, (14*MB_SIZE);//pix15
				
		subs	i, i, #1
		strb	r12, [pPred, #(15*MB_SIZE)]
		sub		pPred, pPred, #1	
		bge		ROW_LOOP_V
	
		ldmfd	sp!, {r4 - r12, pc}	
		
	ENDIF
				
		END