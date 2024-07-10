;/*****************************************/
; intra_pred_4x4_VERT_PRED.s 
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

a			RN		2
dstPitch	RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
	
		EXPORT	intra_pred_4x4_VERT_PRED
		
intra_pred_4x4_VERT_PRED
		stmfd	sp!, {r14}
		 
		ldr		a, [pTopPix]
		
		cmp		beChroma, #0
		moveq	dstPitch, #16
		movne	dstPitch, #8
		
		str		a, [pPred], dstPitch
		str		a, [pPred], dstPitch
		str		a, [pPred], dstPitch
		str		a, [pPred], dstPitch
				
		ldmfd	sp!, {pc}	
				
		END