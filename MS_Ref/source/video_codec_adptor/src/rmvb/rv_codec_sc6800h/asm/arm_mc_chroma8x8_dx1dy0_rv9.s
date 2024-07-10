;/*****************************************/
; arm_mc_chroma8x8_dx1dy0_rv9.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_CHROMA_SIZE	EQU		8

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2

pp0				RN		3
pp1				RN		4
pp2				RN		5
pp3				RN		6
pp4				RN		3
pp5				RN		4
pp6				RN		5
pp7				RN		6
pp8				RN		3

par3			RN 		7

srcPitch_sub8	RN		2
i				RN		14

		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		BILINEAR_FILTER_1PIX	$p0, $p1, $pnn, $src_add
		smlabb	$p0, par3, $p0, $p1
		ldrb	$pnn, [pRef], $src_add
		add		$p0, $p0, #1
		mov		$p0, $p0, asr #2
		strb	$p0, [pPred], #1	
		
		MEND   

		EXPORT	arm_mc_chroma8x8_dx1dy0_rv9 	
		
arm_mc_chroma8x8_dx1dy0_rv9

		stmfd	sp!, {r4 - r7, r14} 
		
		sub		srcPitch_sub8, srcPitch, #MB_CHROMA_SIZE
		mov		par3, #3

		mov		i, #MB_CHROMA_SIZE
ROW_LOOP
		ldrb	pp0, [pRef], #1
		ldrb	pp1, [pRef], #1
		ldrb	pp2, [pRef], #1
				
		smlabb	pp0, par3, pp0, pp1
		ldrb	pp3, [pRef], #1
		add		pp0, pp0, #1
		mov		pp0, pp0, asr #2
		strb	pp0, [pPred], #1	;//pix0
	;//	BILINEAR_FILTER_1PIX	pp0, pp1, pp3, #1;//pix0		
		BILINEAR_FILTER_1PIX	pp1, pp2, pp4, #1;//pix1
		BILINEAR_FILTER_1PIX	pp2, pp3, pp5, #1;//pix2
		BILINEAR_FILTER_1PIX	pp3, pp4, pp6, #1;//pix3
		BILINEAR_FILTER_1PIX	pp4, pp5, pp7, #1;//pix4
		BILINEAR_FILTER_1PIX	pp5, pp6, pp8, srcPitch_sub8;//pix5								
		
		smlabb	pp6, par3, pp6, pp7
		add		pp6, pp6, #1
		mov		pp6, pp6, asr #2
		strb	pp6, [pPred], #1	;//pix6

		smlabb	pp7, par3, pp7, pp8
		subs	i, i, #1	
		add		pp7, pp7, #1
		mov		pp7, pp7, asr #2
		strb	pp7, [pPred], #1	;//pix7

		bne		ROW_LOOP  	
		
		ldmfd	sp!, {r4 - r7, pc}	
				
		END
