;/*****************************************/
; intra_pred_Luma16x16_PLANAR_PRED.s 
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

a			RN		4
b			RN		5
c			RN		6

tp			RN		0
lpb			RN		1
lpt			RN		7

iH			RN		8
iV			RN		9

bFactor		RN		0
cFactor		RN		1
sum			RN		3

i 			RN		10
j			RN		11
pClipTab	RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
		
		MACRO
		IPRED_1PIX
		ldrb	r12, [pClipTab, sum, asr #5]
		add		sum, sum, b
		strb	r12, [pPred], #1
		
		MEND   
			
		EXPORT	intra_pred_Luma16x16_PLANAR_PRED
		IMPORT	g_rgiRvClipTab
		
intra_pred_Luma16x16_PLANAR_PRED
		stmfd	sp!, {r4 - r12, r14}
		
		mov		iH, #0
		mov		iV, #0
		 
		ldrb	r11, [tp, #15]
		rsb		r14, pitch, pitch, lsl #4
		ldrb	r12, [pLeftPix,r14]
		mov		a,	r11, lsl #4
		add		a, 	a, r12, lsl #4
					
		add		lpb, pLeftPix, pitch, lsl #3
		sub		lpt, lpb, pitch, lsl #1
		
		mov		i, #1
ROW_LOOP0			
		add		r14, i, #7
		ldrb	r5, [tp, r14]
		rsb		r14, r14, #14
		ldrb	r6, [tp, r14]
		
		ldrb	r12, [lpb], pitch	;//remove interlock
		sub		r14, r5, r6
		mla		iH, i, r14, iH
		
		ldrb	r5, [lpt]
		sub		lpt, lpt, pitch
		sub		r14, r12, r5
		mla		iV, i, r14, iV
				
		add		i, i, #1
		cmp		i, #8
		blt		ROW_LOOP0

		ldrb	r5, [tp, #15]
		ldrb	r6, [tp, #-1]
		sub		r14, r5, r6
		add		iH, iH, r14, lsl #3

		ldrb	r5, [lpb]
		sub		r14, r5, r6
		add		iV, iV, r14, lsl #3
		
		add		r14, iH, iH, asr #2
		mov		b, r14, asr #4
		add		r14, iV, iV, asr #2
		mov		c, r14, asr #4
		
		sub		cFactor, c, c, lsl #3
		sub		bFactor, b, b, lsl #3
		
		ldr		r14, =g_rgiRvClipTab
		ldr		pClipTab, [r14, #0]	
		
		mov		j, #16
LOOP_J
		add		sum, a, cFactor
		add		sum, sum, #16
		add		sum, sum, bFactor
		
		mov		i, #16
LOOP_I		
		IPRED_1PIX
		IPRED_1PIX
		IPRED_1PIX
		IPRED_1PIX
		
		subs	i, i, #4
		bne		LOOP_I
		
		add		cFactor, cFactor, c		
		subs	j, j, #1
		bne		LOOP_J		
				
		ldmfd	sp!, {r4 - r12, pc}	
				
		END