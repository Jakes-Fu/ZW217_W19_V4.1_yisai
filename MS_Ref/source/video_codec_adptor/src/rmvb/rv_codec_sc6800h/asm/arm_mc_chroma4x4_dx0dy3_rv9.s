;/*****************************************/
; arm_mc_chroma4x4_dx0dy3_rv9.s 
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

pb0				RN		7
pb1				RN		8
pb2				RN		7
pb3				RN		8

pRef_b			RN		0

srcPitch_sub3	RN		2
i				RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		BILINEAR_FILTER_1PIX	$src_add, $pt, $pb, $pb_next, $dst_add
		ldrb	$pb_next, [pRef_b], $src_add
		add		$pt, $pt, $pb
		add		$pt, $pt, $pb, lsl #1
		add		$pt, $pt, #2
		mov		$pt, $pt, asr #2
		strb	$pt, [pPred], #$dst_add
		mov		$pt, $pb
		
		MEND   

		EXPORT	arm_mc_chroma4x4_dx0dy3_rv9 	
		
arm_mc_chroma4x4_dx0dy3_rv9

		stmfd	sp!, {r4 - r8, r14} 
				
		sub		srcPitch_sub3, srcPitch, #3
		
		ldrb	pt0, [pRef], #1
		ldrb	pt1, [pRef], #1
		ldrb	pt2, [pRef], #1
		ldrb	pt3, [pRef], srcPitch_sub3

		mov		i, #4
ROW_LOOP
		;/***********load six pixeles***************/
		ldrb	pb0, [pRef_b], #1
				
		;//pix0
		ldrb	pb1, [pRef_b], #1
		add		pt0, pt0, pb0
		add		pt0, pt0, pb0, lsl #1
		add		pt0, pt0, #2
		mov		pt0, pt0, asr #2
		strb	pt0, [pPred], #1
		mov		pt0, pb0
	;//	BILINEAR_FILTER_1PIX	#1, pt0, pb0, pb1, 1;//pix0	
		BILINEAR_FILTER_1PIX	#1, pt1, pb1, pb2, 1;//pix1
		BILINEAR_FILTER_1PIX	srcPitch_sub3, pt2, pb2, pb3, 1;//pix2
		add		pt3, pt3, pb3
		add		pt3, pt3, pb3, lsl #1
		add		pt3, pt3, #2
		mov		pt3, pt3, asr #2
		strb	pt3, [pPred], #5
		mov		pt3, pb3
		
		subs	i, i, #1	
		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r8, pc}	
				
		END
