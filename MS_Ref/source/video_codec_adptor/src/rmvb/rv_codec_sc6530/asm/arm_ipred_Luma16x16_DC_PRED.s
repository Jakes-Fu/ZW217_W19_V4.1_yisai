;/*****************************************/
; arm_ipred_Luma16x16_DC_PRED.s 
;input:
;	r0: pTopPix
;	r1: pLeftPix
;	r2: pPred
;	r3: pitch
;	r4: pMBCache
;/*****************************************/

MB_SIZE		EQU		16

pTopPix		RN		0 
pLeftPix	RN		1
pPred		RN		2
pitch		RN		3
pMBCache	RN		4

availNum	RN		7
dc			RN		8
sum			RN		8

tmp1		RN		5
tmp2		RN		6

i			RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
			
			
		EXPORT	intra_pred_Luma16x16_DC_PRED
		
intra_pred_Luma16x16_DC_PRED
		stmfd	sp!, {r4 - r8, r14} 
		
		ldr		pMBCache, [sp, #24]	;//
		mov		availNum, #0
		mov		sum, #0
		ldrb	r5, [pMBCache, #0x308]
		ldrb	r4, [pMBCache, #0x309]
		
		cmp		r5, #0
		beq		CHECK_TOP_AVAIL	

;//LEFT_AVAIL	
		add		availNum, availNum, #1	
		mov		i, #4
LEFT_LOOP
		ldrb 	tmp1, [pLeftPix], pitch
		ldrb 	tmp2, [pLeftPix], pitch
		add	 	sum, sum, tmp1
		ldrb 	tmp1, [pLeftPix], pitch
		add  	sum, sum, tmp2
		ldrb 	tmp2, [pLeftPix], pitch
		add	 	sum, sum, tmp1
		add	 	sum, sum, tmp2
		
		subs	i, i, #1
		bne		LEFT_LOOP	
		
CHECK_TOP_AVAIL
		cmp		r4, #0
		beq		DC_PRED
		
;//TOP_AVAIL		
		add		availNum, availNum, #1	
		mov		i, #4
TOP_LOOP			
		ldrb	tmp1, [pTopPix], #1
		ldrb	tmp2, [pTopPix], #1
		add		sum, sum, tmp1
		ldrb	tmp1, [pTopPix], #1
		add		sum, sum, tmp2
		ldrb	tmp2, [pTopPix], #1
		add		sum, sum, tmp1
		add		sum, sum, tmp2	
		
		subs	i, i, #1
		bne		TOP_LOOP		

DC_PRED			
		cmp		availNum, #2
		addeq	sum, sum, #16
		moveq	dc, sum, asr #5
		beq		DC_STR
		cmpne	availNum, #0
		moveq	dc, #0x80	
		addne	sum, sum, #8
		movne	dc, sum, asr #4
		
DC_STR
		add		dc, dc, dc, lsl #16
		add		dc, dc, dc, lsl #8	
		
		mov		r0, dc
		mov		r1, dc
		mov		r3, dc
		
		mov		i, #4
ROW_LOOP
		stmia	pPred!, {r0, r1, r3, dc} 		;//row0
		stmia	pPred!, {r0, r1, r3, dc}		;//row1
		stmia	pPred!, {r0, r1, r3, dc}		;//row2
		stmia	pPred!, {r0, r1, r3, dc}		;//row3
		
		subs	i, i, #1
		bne		ROW_LOOP	
		
		ldmfd	sp!, {r4 - r8, pc}	
				
		END