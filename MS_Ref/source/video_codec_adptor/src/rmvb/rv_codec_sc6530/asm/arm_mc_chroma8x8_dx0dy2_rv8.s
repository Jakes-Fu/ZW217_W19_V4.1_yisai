;/*****************************************/
; arm_mc_chroma8x8_dx0dy2_rv8.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_CHROMA_SIZE	EQU		8

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2

pt0				RN		3
pt1				RN		4
pt2				RN		5
pt3				RN		6
pt4				RN		7
pt5				RN		8
pt6				RN		9
pt7				RN		10

pb				RN		11
pRef_b			RN		0

srcPitch_sub7	RN		2
i				RN		14

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		BILINEAR_FILTER_1PIX	$src_add, $pt
		ldrb	pb, [pRef_b], $src_add
		add		$pt, $pt, $pt, lsl #1
		add		$pt, $pt, pb
		add		$pt, $pt, pb, lsl #2
		add		$pt, $pt, #4
		mov		$pt, $pt, asr #3
		strb	$pt, [pPred], #1
		mov		$pt, pb 
				
		MEND   

;/***************************************************/
;arm_mc_chroma8x8_dx0dy2_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma8x8_dx0dy2_rv8 	
		
arm_mc_chroma8x8_dx0dy2_rv8

		stmfd	sp!, {r4 - r11, r14} 
		
		sub		srcPitch_sub7, srcPitch, #7
				
		ldrb	pt0, [pRef], #1
		ldrb	pt1, [pRef], #1
		ldrb	pt2, [pRef], #1
		ldrb	pt3, [pRef], #1
		ldrb	pt4, [pRef], #1
		ldrb	pt5, [pRef], #1
		ldrb	pt6, [pRef], #1
		ldrb	pt7, [pRef], srcPitch_sub7
		
		mov		i, #MB_CHROMA_SIZE
ROW_LOOP
		;//pix0
		ldrb	pb, [pRef_b], #1
		add		pt0, pt0, pt0, lsl #1
		add		pt0, pt0, pb
		add		pt0, pt0, pb, lsl #2
		add		pt0, pt0, #4
		mov		pt0, pt0, asr #3
		strb	pt0, [pPred], #1
		mov		pt0, pb 
		
	;//	BILINEAR_FILTER_1PIX	#1, pt0;//pix0	
		BILINEAR_FILTER_1PIX	#1, pt1;//pix1
		BILINEAR_FILTER_1PIX	#1, pt2;//pix2
		BILINEAR_FILTER_1PIX	#1, pt3;//pix3
		BILINEAR_FILTER_1PIX	#1, pt4;//pix4
		BILINEAR_FILTER_1PIX	#1, pt5;//pix5
		BILINEAR_FILTER_1PIX	#1, pt6;//pix6
		BILINEAR_FILTER_1PIX	srcPitch_sub7, pt7;//pix7

		subs	i, i, #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r11, pc}	
		
	ENDIF
				
		END