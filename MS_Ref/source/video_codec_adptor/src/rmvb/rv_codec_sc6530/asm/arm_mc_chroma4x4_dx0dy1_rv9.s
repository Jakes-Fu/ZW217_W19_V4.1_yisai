;/*****************************************/
; arm_mc_chroma4x4_dx0dy1_rv9.s 
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

pRef_b			RN		0

srcPitch_sub3	RN		2
i				RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		BILINEAR_FILTER_1PIX	$src_add, $pt, $dst_add
		ldrb	pb, [pRef_b], $src_add
		add		$pt, $pt, $pt, lsl #1
		add 	$pt, $pt, #2
		add		$pt, $pt, pb
		mov		$pt, $pt, asr #2
		strb	$pt, [pPred], #$dst_add
		mov		$pt, pb
		
		MEND   

;/***************************************************/
;arm_mc_chroma4x4_dx0dy1_rv9
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_chroma4x4_dx0dy1_rv9 	
		
arm_mc_chroma4x4_dx0dy1_rv9

		stmfd	sp!, {r4 - r7, r14} 
		
		sub		srcPitch_sub3, srcPitch, #3
				
		ldrb	pt0, [pRef], #1
		ldrb	pt1, [pRef], #1
		ldrb	pt2, [pRef], #1
		ldrb	pt3, [pRef], srcPitch_sub3
		
		mov		i, #4
ROW_LOOP
		;//pix0
		ldrb	pb, [pRef_b], #1
		add		pt0, pt0, pt0, lsl #1
		add		pt0, pt0, #2
		add		pt0, pt0, pb
		mov		pt0, pt0, asr #2
		strb	pt0, [pPred], #1
		mov		pt0, pb
	;//	BILINEAR_FILTER_1PIX	#1, pt0, 1;//pix0	
		BILINEAR_FILTER_1PIX	#1, pt1, 1;//pix1
		BILINEAR_FILTER_1PIX	#1, pt2, 1;//pix2
		BILINEAR_FILTER_1PIX	srcPitch_sub3, pt3, 5;//pix3

		subs	i, i, #1
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r7, pc}	
				
		END