;/*****************************************/
; arm_ipred_CHROMA8x8_HORZ_PRED.s 
;input:
;	r0: pTopPix
;	r1: pLeftPix
;	r2: pPred
;	r3: pitch
;	r4: pMBCache
;/*****************************************/

MB_CHROMA_SIZE		EQU		8

pTopPix		RN		0 
pLeftPix	RN		1
pPred		RN		2
pitch		RN		3
pMBCache	RN		4

a			RN		0
b			RN		4


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
		
		MACRO
		IPRED_1ROW	$pix
		add		$pix, $pix, $pix, lsl #16
		add		$pix, $pix, $pix, lsl #8
		mov		r14, $pix			
		stmia 	pPred!, {$pix, r14}
		
		MEND   
	
		EXPORT	intra_pred_CHROMA8x8_HORZ_PRED
		
intra_pred_CHROMA8x8_HORZ_PRED
		stmfd	sp!, {r4, r14}
		 
		ldrb	a, [pLeftPix], pitch		
		ldrb	b, [pLeftPix], pitch
		
		IPRED_1ROW	a					;//0
		ldrb	a, [pLeftPix], pitch
		
		IPRED_1ROW	b					;//1
		ldrb	b, [pLeftPix], pitch
		
		IPRED_1ROW	a					;//2
		ldrb	a, [pLeftPix], pitch
		
		IPRED_1ROW	b					;//3
		ldrb	b, [pLeftPix], pitch
		
		IPRED_1ROW	a					;//4
		ldrb	a, [pLeftPix], pitch
		
		IPRED_1ROW	b					;//5
		ldrb	b, [pLeftPix], pitch
		
		IPRED_1ROW	a					;//6
		IPRED_1ROW	b					;//7
						
		ldmfd	sp!, {r4, pc}	
				
		END