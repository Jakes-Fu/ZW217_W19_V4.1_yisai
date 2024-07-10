;/*****************************************/
; intra_pred_4x4_HOR_PRED.s 
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

pix			RN		1
dstPitch	RN		14
pitch		RN		3


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
	
		EXPORT	intra_pred_4x4_HOR_PRED
		
intra_pred_4x4_HOR_PRED
		stmfd	sp!, {r14}
		 
		;//0 
		cmp		beChroma, #0
		ldr		pitch, [sp, #4]
		ldrb	pix, [pLeftPix], pitch
		moveq	dstPitch, #16
		movne	dstPitch, #8
		
		add	pix, pix, pix, lsl #16
		add	pix, pix, pix, lsl #8
		str   pix, [pPred], dstPitch 

		;///1
		ldrb  pix, [pLeftPix], pitch
		add	pix, pix, pix, lsl #16
		add	pix, pix, pix, lsl #8
		str   pix, [pPred], dstPitch 

		;///2
		ldrb  pix, [pLeftPix], pitch
		add	pix, pix, pix, lsl #16
		add	pix, pix, pix, lsl #8
		str   pix, [pPred], dstPitch 

		;///3
		ldrb  pix, [pLeftPix]
		add	pix, pix, pix, lsl #16
		add	pix, pix, pix, lsl #8
		str   pix, [pPred]
				
		ldmfd	sp!, {pc}	
				
		END