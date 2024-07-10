;/*****************************************/
; arm_mc_luma16x16_dx2dy2_rv8.s 
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

pp0				RN		3
pp1				RN		4
pp2				RN		5
pp3				RN		3
pp4				RN		4
pp5				RN		5
pp6				RN		3
pp7				RN		4
pp8				RN		5
pp9				RN		3
pp10			RN		4
pp11			RN		5
pp12			RN		3
pp13			RN		4
pp14			RN		5
pp15			RN		3
pp16			RN		4
pp17			RN		5

par6			RN		7

i				RN		11
pClipTab		RN		14

pTmpBfr			RN		9
srcPitch_sub17	RN		2
pRefTemp		RN		2

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		THREE_TAPS_FILTER_1PIX_H	$p0, $p1, $p2, $src_off
		ldrb	$p2, [pRef], $src_off
		smlabb	$p0, $p0, par6, $p1
		add		r14, $p2, $p1, lsl #3
		add		$p0, $p0, r14
		strh	$p0, [pTmpBfr], #2
		
		MEND   
		
		MACRO
		THREE_TAPS_FILTER_1PIX_V	$p0, $p1, $p2, $dst_off
		ldrsh	$p2, [pRefTemp], #(MB_SIZE*2)
		add		r10, $p1, $p1, lsl #3
		smlabb	r10, $p0, par6, r10
		strb	r12, [pPred, #$dst_off]
		add		$p0, $p2, #128
		add		$p0, $p0, r10 	
		ldrb	r12, [pClipTab, $p0, asr #8]
						
		MEND   

;/***************************************************/
;arm_mc_luma16x16_dx2dy2_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma16x16_dx2dy2_rv8 	
		IMPORT	g_rgiRvClipTab
		IMPORT	g_int_bfr
				
arm_mc_luma16x16_dx2dy2_rv8

		stmfd	sp!, {r4 - r7, r9 - r12, r14} 
		
		ldr		pTmpBfr, =g_int_bfr
		mov		par6, #6
			
		sub		srcPitch_sub17, srcPitch, #17
		mov		i, #18
ROW_LOOP_H
		;/***********load six pixeles***************/
		ldrb	pp0, [pRef], #1
		ldrb	pp1, [pRef], #1
		
		;//pix0
		ldrb	pp2, [pRef], #1
		smlabb	pp0, pp0, par6, pp1
		add		r14, pp2, pp1, lsl #3
		add		pp0, pp0, r14
		strh	pp0, [pTmpBfr], #2
	;//	THREE_TAPS_FILTER_1PIX_H	pp0, pp1, pp2, #1;//pix0
		THREE_TAPS_FILTER_1PIX_H	pp1, pp2, pp3, #1;//pix1
		THREE_TAPS_FILTER_1PIX_H	pp2, pp3, pp4, #1;//pix2
		THREE_TAPS_FILTER_1PIX_H	pp3, pp4, pp5, #1;//pix3
		THREE_TAPS_FILTER_1PIX_H	pp4, pp5, pp6, #1;//pix4
		THREE_TAPS_FILTER_1PIX_H	pp5, pp6, pp7, #1;//pix5
		THREE_TAPS_FILTER_1PIX_H	pp6, pp7, pp8, #1;//pix6
		THREE_TAPS_FILTER_1PIX_H	pp7, pp8, pp9, #1;//pix7
		THREE_TAPS_FILTER_1PIX_H	pp8, pp9, pp10, #1;//pix8
		THREE_TAPS_FILTER_1PIX_H	pp9, pp10, pp11, #1;//pix9		
		THREE_TAPS_FILTER_1PIX_H	pp10, pp11, pp12, #1;//pix10
		THREE_TAPS_FILTER_1PIX_H	pp11, pp12, pp13, #1;//pix11
		THREE_TAPS_FILTER_1PIX_H	pp12, pp13, pp14, #1;//pix12
		THREE_TAPS_FILTER_1PIX_H	pp13, pp14, pp15, #1;//pix13		
		THREE_TAPS_FILTER_1PIX_H	pp14, pp15, pp16, #1;//pix14
		THREE_TAPS_FILTER_1PIX_H	pp15, pp16, pp17, srcPitch_sub17;//pix15
		
		subs	i, i, #1
		bne		ROW_LOOP_H  
		
		ldr		r14, =g_rgiRvClipTab
		ldr		pClipTab, [r14, #0]
					
		sub		pTmpBfr, pTmpBfr, #576	;//-18*32
		add		pPred, pPred, #15
		mov		i, #15
ROW_LOOP_V
		add		pRefTemp, pTmpBfr, i, lsl #1
		ldrsh	pp0, [pRefTemp], #(MB_SIZE*2)
		ldrsh	pp1, [pRefTemp], #(MB_SIZE*2)
		
		;//pix0
		ldrsh	pp2, [pRefTemp], #(MB_SIZE*2)
		add		r10, pp1, pp1, lsl #3
		smlabb	r10, pp0, par6, r10
		add		pp0, pp2, #128
		add		pp0, pp0, r10 	
		ldrb	r12, [pClipTab, pp0, asr #8]
				
		THREE_TAPS_FILTER_1PIX_V	pp1, pp2, pp3, (0*MB_SIZE);//pix1
		THREE_TAPS_FILTER_1PIX_V	pp2, pp3, pp4, (1*MB_SIZE);//pix2
		THREE_TAPS_FILTER_1PIX_V	pp3, pp4, pp5, (2*MB_SIZE);//pix3		
		THREE_TAPS_FILTER_1PIX_V	pp4, pp5, pp6, (3*MB_SIZE);//pix4
		THREE_TAPS_FILTER_1PIX_V	pp5, pp6, pp7, (4*MB_SIZE);//pix5
		THREE_TAPS_FILTER_1PIX_V	pp6, pp7, pp8, (5*MB_SIZE);//pix6
		THREE_TAPS_FILTER_1PIX_V	pp7, pp8, pp9, (6*MB_SIZE);//pix7
		THREE_TAPS_FILTER_1PIX_V	pp8, pp9, pp10, (7*MB_SIZE);//pix8
		THREE_TAPS_FILTER_1PIX_V	pp9, pp10, pp11, (8*MB_SIZE);//pix9
		THREE_TAPS_FILTER_1PIX_V	pp10, pp11, pp12, (9*MB_SIZE);//pix10		
		THREE_TAPS_FILTER_1PIX_V	pp11, pp12, pp13, (10*MB_SIZE);//pix11
		THREE_TAPS_FILTER_1PIX_V	pp12, pp13, pp14, (11*MB_SIZE);//pix12
		THREE_TAPS_FILTER_1PIX_V	pp13, pp14, pp15, (12*MB_SIZE);//pix13
		THREE_TAPS_FILTER_1PIX_V	pp14, pp15, pp16, (13*MB_SIZE);//pix14
		THREE_TAPS_FILTER_1PIX_V	pp15, pp16, pp17, (14*MB_SIZE);//pix15
				
		subs	i, i, #1
		strb	r12, [pPred, #(15*MB_SIZE)]
		sub		pPred, pPred, #1	
		bge		ROW_LOOP_V
	
		ldmfd	sp!, {r4 - r7, r9 - r12, pc}	
		
	ENDIF
				
		END