;/*****************************************/
; arm_ipred_CHROMA8x8_VERT_PRED.s 
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

a			RN		1
b			RN		3

		AREA	PROGRAM, CODE, READONLY
		
		CODE32
		
		MACRO
		IPRED_4ROW	
		stmia pPred!, {a, b}
		stmia pPred!, {a, b}
		stmia pPred!, {a, b}
		stmia pPred!, {a, b}
		
		MEND   
			
		EXPORT	intra_pred_CHROMA8x8_VERT_PRED
		
intra_pred_CHROMA8x8_VERT_PRED
		stmfd	sp!, {r14} 
		
		ldmia	pTopPix, {a, b}

		IPRED_4ROW
		IPRED_4ROW
		
		ldmfd	sp!, {pc}	
				
		END