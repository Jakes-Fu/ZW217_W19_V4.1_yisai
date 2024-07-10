;/*****************************************/
; arm_mc_chroma4x4_dx0dy2_rv8.s 
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

pb				RN		7

par3			RN		8
par4			RN		9
pred			RN		10

pRef_b			RN		0

srcPitch_sub3	RN		2
i				RN		14

	INCLUDE rv89_dec_asm_config.s

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
	IF RV89_DEC_ASM_RV8_EB = 1
		
		MACRO
		BILINEAR_FILTER_1PIX	$src_add, $pt, $dst_add
		ldrb	pb, [pRef_b], $src_add
		smlabb	$pt, $pt, par3, par4
		add		pred, pb, pb, lsl #2
		add		pred, pred, $pt
		mov		$pt, pred, asr #3
		strb	$pt, [pPred], #$dst_add
		mov		$pt, pb
		
		MEND   

;/***************************************************/
;arm_mc_chroma4x4_dx0dy2_rv8
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma4x4_dx0dy2_rv8 	
		
arm_mc_chroma4x4_dx0dy2_rv8

		stmfd	sp!, {r4 - r10, r14} 
		
		mov		par3, #3
		mov		par4, #4
		sub		srcPitch_sub3, srcPitch, #3
				
		ldrb	pt0, [pRef], #1
		ldrb	pt1, [pRef], #1
		ldrb	pt2, [pRef], #1
		ldrb	pt3, [pRef], srcPitch_sub3
		
		mov		i, #4
ROW_LOOP
		;//pix0
		ldrb	pb, [pRef_b], #1
		smlabb	pt0, pt0, par3, par4
		add		pred, pb, pb, lsl #2
		add		pred, pred, pt0
		mov		pt0, pred, asr #3
		strb	pt0, [pPred], #1
		mov		pt0, pb 
		
	;//	BILINEAR_FILTER_1PIX	#1, pt0, 1;//pix0	
		BILINEAR_FILTER_1PIX	#1, pt1, 1;//pix1
		BILINEAR_FILTER_1PIX	#1, pt2, 1;//pix2
		BILINEAR_FILTER_1PIX	srcPitch_sub3, pt3, 5;//pix3

		subs	i, i, #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r10, pc}	
		
	ENDIF
				
		END