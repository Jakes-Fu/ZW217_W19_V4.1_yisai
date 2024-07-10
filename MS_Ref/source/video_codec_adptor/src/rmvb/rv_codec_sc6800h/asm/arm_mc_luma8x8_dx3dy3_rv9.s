;/*****************************************/
; arm_mc_luma8x8_dx3dy3_rv9.s 
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/*****************************************/

MB_SIZE		EQU		16

pRef			RN		0
pPred			RN		1 
srcPitch		RN		2

t0				RN		3
t1				RN		4
b0				RN		5
b1				RN		6	
i				RN		7

pRef_b			RN		14
srcPitch_sub8	RN		2
		
		AREA	PROGRAM, CODE, READONLY
		CODE32	
		
		MACRO
		OUT_1PIX	$dst_add
		add		t0, t0, t1
		add		b0,	b0, b1
		add		t0, t0, b0
		add		t0, t0, #2
		mov		t0, t0, asr #2
		strb	t0, [pPred], $dst_add
		
		MEND   
		
		MACRO
		OUT		$src_add, $dst_add
		mov		t0, t1
		mov		b0, b1
		ldrb	t1, [pRef], $src_add
		ldrb	b1, [pRef_b], $src_add
		OUT_1PIX	$dst_add
		
		MEND   

;/***************************************************/
;arm_mc_luma8x8_dx3dy3_rv9
;input:
;	r0: pRef
;	r1: pPred
;	r2: srcPitch
;/****************************************************/		
		EXPORT	arm_mc_luma8x8_dx3dy3_rv9 	
	
arm_mc_luma8x8_dx3dy3_rv9

		stmfd	sp!, {r4 - r7, r14} 
		
		add		pRef_b, pRef, srcPitch
		sub		srcPitch_sub8, srcPitch, #8
		mov		i, #8
ROW_LOOP
		ldrb	t0, [pRef], #1
		ldrb	t1, [pRef], #1
		ldrb	b0, [pRef_b], #1
		ldrb	b1, [pRef_b], #1
	
		OUT_1PIX	#1	;//pix0
		
		OUT		#1, #1	;//pix1
		OUT		#1, #1	;//pix2
		OUT		#1, #1	;//pix3
		OUT		#1, #1	;//pix4
		OUT		#1, #1	;//pix5
		OUT		#1, #1	;//pix6
		OUT		srcPitch_sub8, #9	;//pix7
		
		subs	i, i, #1	
		bne		ROW_LOOP 
		
		ldmfd	sp!, {r4 - r7, pc}	
				
		END