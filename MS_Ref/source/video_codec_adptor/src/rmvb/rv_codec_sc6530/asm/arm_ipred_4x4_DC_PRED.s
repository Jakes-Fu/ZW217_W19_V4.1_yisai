;/*****************************************/
; intra_pred_4x4_DC_PRED.s 
;input:
;	r0: pPred
;	r1: pTopPix
;	r2: pLeftPix
;	r3: beChroma
;	r4: pitch
;	r5: blkIdx
;	r6: pMBCache
;/*****************************************/

pPred		RN		0 
pTopPix		RN		1
pLeftPix	RN		2
beChroma	RN		3
pitch		RN		1
blkIdx		RN		4
pMBCache	RN		5

tmp1		RN		6
tmp2		RN		7
availNum	RN		8
dc			RN		9
sum			RN		9
dstPitch	RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
	
		EXPORT	intra_pred_4x4_DC_PRED
		
intra_pred_4x4_DC_PRED
		stmfd	sp!, {r4 -  r9, r14}
		
		ldr		pMBCache, [sp, #36]	;//
		mov		availNum, #0
		mov		sum, #0
		
		cmp 	beChroma, #0
		moveq	r14, #4
		movne	r14, #2
		
		ldr		blkIdx, [sp, #32]	;//
		ldrb	r7, [pMBCache, #0x309]
		
		cmp		blkIdx, r14
		bge		TOP_AVAIL

		cmp		r7, #0
		beq		CHECK_LEFT_AVAIL
		
TOP_AVAIL
		add		availNum, availNum, #1
		ldrb	tmp1, [pTopPix, #0]
		ldrb	tmp2, [pTopPix, #1]
		add		sum, sum, tmp1
		ldrb	tmp1, [pTopPix, #2]
		add		sum, sum, tmp2
		ldrb	tmp2, [pTopPix, #3]		
		add		sum, sum, tmp1
		add		sum, sum, tmp2
		
		
CHECK_LEFT_AVAIL
		cmp 	beChroma, #0
		moveq	r3, #3
		movne	r3, #1
		
		ldrb	r5, [pMBCache, #0x308]
		
		ands	r3, blkIdx, r3
		bne		LEFT_AVAIL
		
		cmp		r5, #0
		beq		DC_PRED
		
LEFT_AVAIL
		ldr		pitch, [sp, #28]
		
		add		availNum, availNum, #1
		ldrb	tmp1, [pLeftPix], pitch
		ldrb	tmp2, [pLeftPix], pitch
		add		sum, sum, tmp1
		ldrb	tmp1, [pLeftPix], pitch
		add		sum, sum, tmp2
		ldrb	tmp2, [pLeftPix]
		add		sum, sum, tmp1
		add		sum, sum, tmp2

DC_PRED			

		cmp		availNum, #2
		addeq	sum, sum, #4
		moveq	dc, sum, asr #3
		beq		DC_STR
		cmpne	availNum, #0
		moveq	dc, #0x80	
		addne	sum, sum, #2
		movne	dc, sum, asr #2
		
DC_STR
		add		dc, dc, dc, lsl #16
		add		dc, dc, dc, lsl #8	
		
		str 	dc, [pPred], dstPitch, lsl #2
		str	 	dc, [pPred], dstPitch, lsl #2
		str 	dc, [pPred], dstPitch, lsl #2
		str 	dc, [pPred]	
				
		ldmfd	sp!, {r4 -  r9, pc}	
				
		END