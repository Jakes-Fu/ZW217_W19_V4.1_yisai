;/*****************************************/
; intra_pred_Luma16x16_HORZ_PRED.s 
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

a			RN		0
b			RN		4

i 			RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
	
		EXPORT	intra_pred_Luma16x16_HORZ_PRED
		
intra_pred_Luma16x16_HORZ_PRED
		stmfd	sp!, {r4, r14}
		 
		ldrb	a, [pLeftPix], pitch
		
		mov		i, #MB_SIZE
ROW_LOOP
		add		a, a, a, lsl #16
		add		a, a, a, lsl #8
		mov		b, a			
		stmia 	pPred!, {a, b}
		stmia 	pPred!, {a, b}
				
		subs	i, i, #1
		ldrb	a, [pLeftPix], pitch
		bne		ROW_LOOP	
		
		ldmfd	sp!, {r4, pc}	
				
		END