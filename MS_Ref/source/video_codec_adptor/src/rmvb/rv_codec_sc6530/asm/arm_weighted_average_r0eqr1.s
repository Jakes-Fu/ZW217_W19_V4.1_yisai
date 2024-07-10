;/*************************************/
;arm_weighted_average_r0eqr1.s
;input:
;	r0: rv_decoder_ptr
;	r1: pmbpix
;	r2: pmbpix_b
;/*************************************/


MB_SIZE				EQU			16
MB_CHROMA_SIZE		EQU			8


;/********register map**********/

rv_decoder_ptr		RN			0
pmbpix				RN			1
pmbpix_b			RN			2

i					RN			14

int_f				RN			0
int_b				RN			3

int0				RN			4
int1				RN			5

msk1				RN			6
msk2				RN			7


			AREA	PROGRAM, CODE, READONLY
			
			CODE32
			
			MACRO
			AVERAGE_4PIX
			ldr		int_f, [pmbpix]
			ldr		int_b, [pmbpix_b], #4
			
			;//four pix
			and		int0, int_f, msk1
			mov		int0, int0, lsr #1	;//int0 = (int_f&0xfefefefe)/2
			
			and		int1, int_b, msk1
			add		int0, int0, int1, lsr #1	;//int0 += (int_b&0xfefefefe)/2
			
			and		int_f, int_f, msk2	;//int_f &= 0x01010101
			and		int_b, int_b, msk2	;//int_b &= 0x01010101
			add		int_f, int_f, int_b	;//int_f0 += int_b0
			add		int_f, int_f, msk2	;//int_f0 += 0x01010101
			and		int_f, int_f, msk1	;//int_f0 &= 0xfefefefe
			add		int0, int0, int_f, lsr #1	;//int0 += int_f/2
			str		int0, [pmbpix], #4
			
			MEND   
			
RvDec_Weighted_Average_r0EQr1 	FUNCTION					
			EXPORT	RvDec_Weighted_Average_r0EQr1
				
			stmfd	sp!, {r4 - r7, r14}  
						
			;//msk1, msk2
			ldr		r14, =table
			ldr		msk2, [r14, #0]
			ldr		msk1, [r14, #4]			
		
			mov		i, #24		
AVERG_LOOP
			AVERAGE_4PIX
			AVERAGE_4PIX
			AVERAGE_4PIX
			AVERAGE_4PIX
				
			subs	i, i, #1
			bne		AVERG_LOOP
			
			ldmfd	sp!, {r4 - r7, pc}
			
			
			ENDFUNC
			
;/*************************************/
;arm_weighted_average_r0eqr1.s
;input:
;	r0: rv_decoder_ptr
;	r1: pmbpix
;	r2: pmbpix_b
;/*************************************/	

ratio0				RN			3
ratio1				RN			4

pix_f0				RN			0
pix_b0				RN			5

pix_f1				RN			6
pix_b1				RN			7


			MACRO
			AVERAGE_1PIX_NE	$f0, $b0, $f1, $b1
			mul		$f0, ratio1, $f0
			ldrb	$f1, [pmbpix, #1]			
			mla		$b0, ratio0, $b0, $f0
			ldrb	$b1, [pmbpix_b], #1
			mov		$b0, $b0, asr #14
			strb	$b0, [pmbpix], #1
			
			MEND   	

			MACRO
			AVERAGE_4PIX_NE
			AVERAGE_1PIX_NE pix_f0, pix_b0, pix_f1, pix_b1;//0
			AVERAGE_1PIX_NE pix_f1, pix_b1, pix_f0, pix_b0;//1	
			AVERAGE_1PIX_NE pix_f0, pix_b0, pix_f1, pix_b1;//2	
			AVERAGE_1PIX_NE pix_f1, pix_b1, pix_f0, pix_b0;//3
				
			MEND   		
			
RvDec_Weighted_Average_r0NEr1 	FUNCTION					
			EXPORT	RvDec_Weighted_Average_r0NEr1
				
			stmfd	sp!, {r4 - r7, r14}  
						
			;//ratio0, ratio1
			ldr		ratio0, [rv_decoder_ptr, #48]
			ldr		ratio1, [rv_decoder_ptr, #52]
			
			ldrb	pix_f0, [pmbpix]			;//0
			ldrb	pix_b0, [pmbpix_b], #1	
		
			mov		i, #24	
AVERG_LOOP_NE
				
			AVERAGE_4PIX_NE
			AVERAGE_4PIX_NE
			AVERAGE_4PIX_NE
			AVERAGE_4PIX_NE
				
			subs	i, i, #1
			bne		AVERG_LOOP_NE
			
			ldmfd	sp!, {r4 - r7, pc}
			ENDFUNC
			
			AREA	DATA1, DATA	
table		dcd		0x01010101, 0xfefefefe	
		
			END
			





