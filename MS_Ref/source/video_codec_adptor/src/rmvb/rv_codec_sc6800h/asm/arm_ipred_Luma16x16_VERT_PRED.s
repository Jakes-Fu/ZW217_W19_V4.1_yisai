;/*****************************************/
; arm_ipred_Luma16x16_VERT_PRED.s 
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
c			RN		4
d			RN		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32
		
		MACRO
		IPRED_4ROW	
		stmia pPred!, {a, b, c, d}
		stmia pPred!, {a, b, c, d}
		stmia pPred!, {a, b, c, d}
		stmia pPred!, {a, b, c, d}
		
		MEND   
			
		EXPORT	intra_pred_Luma16x16_VERT_PRED
		
intra_pred_Luma16x16_VERT_PRED
		stmfd	sp!, {r4, r14} 
		
		ldmia	pTopPix, {a, b, c, d}

		IPRED_4ROW
		IPRED_4ROW
		IPRED_4ROW
		IPRED_4ROW
		
		ldmfd	sp!, {r4, pc}	
				
		END