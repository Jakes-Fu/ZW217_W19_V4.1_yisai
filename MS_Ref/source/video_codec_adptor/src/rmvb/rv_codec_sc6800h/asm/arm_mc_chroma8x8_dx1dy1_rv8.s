;/*****************************************/
; arm_mc_chroma8x8_dx1dy1_rv8.s 
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
pt8				RN		11

pb0				RN		2
pb1				RN		12
pb2				RN		2
pb3				RN		12
pb4				RN		2
pb5				RN		12
pb6				RN		2
pb7				RN		12
pb8				RN		2

tmp				RN		14
pRef_b			RN		0

srcPitch_sub8	RN		2
i				RN		14

	MAP		0
ii			FIELD	4
pitch		FIELD 	4
length		FIELD	0

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1	
		
		MACRO
		BILINEAR_FILTER_1PIX	$pt0, $pt1,$pb0,$pb1, $src_add
		ldrb	$pb1, [pRef_b], $src_add
		add		tmp, $pt1, $pb0
		rsb		tmp, tmp, tmp, lsl #4
		add		tmp, tmp, $pt0, lsl #4
		add		$pt0, $pt0, $pb1
		add		$pt0, $pt0, $pt0, lsl #3
		add		$pt0, $pt0, tmp
		add		$pt0, $pt0, #32
		mov		$pt0, $pt0, asr #6
		strb	$pt0, [pPred], #1
		mov		$pt0, $pb0
		
		MEND   

;/***************************************************/
;arm_mc_chroma8x8_dx1dy1_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma8x8_dx1dy1_rv8 	
		
arm_mc_chroma8x8_dx1dy1_rv8

		stmfd	sp!, {r4 - r12, r14} 
		sub		sp,	sp,	#length

		sub		srcPitch_sub8, srcPitch, #8
				
		ldrb	pt0, [pRef], #1
		ldrb	pt1, [pRef], #1
		ldrb	pt2, [pRef], #1
		ldrb	pt3, [pRef], #1
		ldrb	pt4, [pRef], #1	
		ldrb	pt5, [pRef], #1
		ldrb	pt6, [pRef], #1
		ldrb	pt7, [pRef], #1
		ldrb	pt8, [pRef], srcPitch_sub8
		
		mov		i, #MB_CHROMA_SIZE

ROW_LOOP
		str		i, [sp, #ii]
		str		srcPitch_sub8, [sp, #pitch]
		
		ldrb	pb0, [pRef_b], #1
		
		;//pix0
		ldrb	pb1, [pRef_b], #1
		add		tmp, pt1, pb0
		rsb		tmp, tmp, tmp, lsl #4
		add		tmp, tmp, pt0, lsl #4
		add		pt0, pt0, pb1
		add		pt0, pt0, pt0, lsl #3
		add		pt0, pt0, tmp
		add		pt0, pt0, #32
		mov		pt0, pt0, asr #6
		strb	pt0, [pPred], #1
		mov		pt0, pb0
 			
		BILINEAR_FILTER_1PIX pt1, pt2, pb1, pb2 ,#1 ;//pix1
		BILINEAR_FILTER_1PIX pt2, pt3, pb2, pb3	,#1 ;//pix2
		BILINEAR_FILTER_1PIX pt3, pt4, pb3, pb4	,#1 ;//pix3
		BILINEAR_FILTER_1PIX pt4, pt5, pb4, pb5	,#1 ;//pix4
		BILINEAR_FILTER_1PIX pt5, pt6, pb5, pb6	,#1 ;//pix5
		BILINEAR_FILTER_1PIX pt6, pt7, pb6, pb7	,#1 ;//pix6
		BILINEAR_FILTER_1PIX pt7, pt8, pb7, pb8	,#0 ;//pix7
		mov		pt8, pb8
		
		ldr		i, [sp, #ii]
		ldr		srcPitch_sub8, [sp, #pitch]
		
		subs	i, i, #1
		add		pRef_b, pRef_b, srcPitch_sub8	
		bne		ROW_LOOP  	
		
		add		sp,	sp,	#length
		ldmfd	sp!, {r4 - r12, pc}	
		
	ENDIF
				
		END
