;/*****************************************/
; arm_mc_chroma8x8_dx0dy1_rv9.s 
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

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		BILINEAR_FILTER_1PIX	$pt,$src_add
		ldrb	pb, [pRef_b], $src_add
		add		$pt, $pt, $pt, lsl #1
		add 	$pt, $pt, #2
		add		$pt, $pt, pb
		mov		$pt, $pt, asr #2
		strb	$pt, [pPred], #1
		mov		$pt, pb
		
		MEND   

		EXPORT	arm_mc_chroma8x8_dx0dy1_rv9 	
		
arm_mc_chroma8x8_dx0dy1_rv9

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
		add		pt0, pt0, #2
		add		pt0, pt0, pb
		mov		pt0, pt0, asr #2
		strb	pt0, [pPred], #1
		mov		pt0, pb
		BILINEAR_FILTER_1PIX  pt1,  #1;//pix1	
		BILINEAR_FILTER_1PIX  pt2,  #1;//pix2	
		BILINEAR_FILTER_1PIX  pt3,	#1;//pix3
		BILINEAR_FILTER_1PIX  pt4,	#1;//pix4	
		BILINEAR_FILTER_1PIX  pt5,	#1;//pix5	
		BILINEAR_FILTER_1PIX  pt6,	#1;//pix6		
		BILINEAR_FILTER_1PIX  pt7,	srcPitch_sub7;//pix7
				
		subs	i, i, #1
		bne		ROW_LOOP 
		 	
		ldmfd	sp!, {r4 - r11, pc}	
				
		END