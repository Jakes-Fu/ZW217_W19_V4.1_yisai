;/*****************************************/
; arm_mc_chroma8x8_dx0dy3_rv9.s 
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

pb0				RN		11
pb1				RN		12
pb2				RN		11
pb3				RN		12
pb4				RN		11
pb5				RN		12
pb6				RN		11
pb7				RN		12

pRef_b			RN		0

srcPitch_sub7	RN		2
i				RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		BILINEAR_FILTER_1PIX	$pt, $pb, $pb_next, $src_add 
		ldrb	$pb_next, [pRef_b], $src_add
		add		$pt, $pt, $pb
		add		$pt, $pt, $pb, lsl #1
		add		$pt, $pt, #2
		mov		$pt, $pt, asr #2
		strb	$pt, [pPred], #1
		mov		$pt, $pb
		
		MEND   

		EXPORT	arm_mc_chroma8x8_dx0dy3_rv9 	
		
arm_mc_chroma8x8_dx0dy3_rv9

		stmfd	sp!, {r4 - r12, r14} 
		
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

		ldrb	pb0, [pRef_b], #1
		
		;//pix0
		ldrb	pb1, [pRef_b], #1
		add		pt0, pt0, pb0
		add		pt0, pt0, pb0, lsl #1
		add		pt0, pt0, #2
		mov		pt0, pt0, asr #2
		strb	pt0, [pPred], #1
		mov		pt0, pb0
		BILINEAR_FILTER_1PIX pt1, pb1, pb2,	#1;//pix1	
		BILINEAR_FILTER_1PIX pt2, pb2, pb3,	#1;//pix2	
		BILINEAR_FILTER_1PIX pt3, pb3, pb4,	#1;//pix3
		BILINEAR_FILTER_1PIX pt4, pb4, pb5,	#1;//pix4	
		BILINEAR_FILTER_1PIX pt5, pb5, pb6,	#1;//pix5	
		BILINEAR_FILTER_1PIX pt6, pb6, pb7,	srcPitch_sub7;//pix6		
		;//pix7
		add		pt7, pt7, pb7
		add		pt7, pt7, pb7, lsl #1
		add		pt7, pt7, #2
		mov		pt7, pt7, asr #2
		strb	pt7, [pPred], #1
		mov		pt7, pb7
			
		subs	i, i, #1
		bne		ROW_LOOP 
		
		ldmfd	sp!, {r4 - r12, pc}	
				
		END
