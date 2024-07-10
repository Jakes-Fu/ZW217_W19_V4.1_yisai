;/*************************************/
;arm_upscaler_ppred.s
;input:
;	r0: mb_cache_ptr
;/*************************************/

MB_SIZE				EQU			16
MB_SIZE_D			EQU			8
MB_CHROMA_SIZE		EQU			8
MB_CHROMA_SIZE_D	EQU			4

;/********register map**********/

mb_cache_ptr		RN			0

pred				RN			1
pred_d				RN			2

t0					RN			3
t1					RN			4
t2					RN			5
t3					RN			6
t4					RN			3
t5					RN			4
t6					RN			5
t7					RN			6

i 					RN			7
uv					RN			7

tmp					RN			14


			AREA	PROGRAM, CODE, READONLY
			
			CODE32
			
			MACRO	
			FOUR_TAPS_FILTER_1PIX	$tt0, $tt1, $tt2, $tt3, $pred_offset  
			add		tmp, $tt1, $tt2
			add		tmp, tmp, tmp, lsl #3
			sub		tmp, tmp, $tt0
			sub		tmp, tmp, $tt3
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], $pred_offset	;//3
			MEND   
						
			EXPORT	Up_Scaler_ppred
						
Up_Scaler_ppred
			stmfd	sp!, {r4 - r7, r14}  
			
			add		pred, mb_cache_ptr, #0xf0
			add		pred_d, mb_cache_ptr, #0x400
			add		pred_d, pred_d, #0x30c
			
			;//0, 1
			ldrb	t0, [pred_d], #1
			ldrb	t1, [pred_d], #1
			ldrb	t2, [pred_d], #1
			strb	t0, [pred], #1		;//0
			add		tmp, t1, t1, lsl #3
			sub		tmp, tmp, t2
			add		tmp, tmp, t0, lsl #3
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #1		;//1
			
			;//2, 3
			ldrb	t3, [pred_d], #1
			strb	t1, [pred], #1	;//2
			add		tmp, t1, t2
			add		t0, t0, t3
			add		tmp, tmp, tmp, lsl #3
			sub		tmp, tmp, t0
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #1	;//3
			
			;//4, 5
			ldrb	t4, [pred_d], #1
			strb	t2, [pred], #1			;//4
			FOUR_TAPS_FILTER_1PIX	t1, t2, t3, t4, #1	;//5
			
			;//6, 7
			ldrb	t5, [pred_d], #1
			strb	t3, [pred], #1	;//6
			FOUR_TAPS_FILTER_1PIX	t2, t3, t4, t5, #1	;//7	
			
			;//8, 9
			ldrb	t6, [pred_d], #1
			strb	t4, [pred], #1	;//8
			FOUR_TAPS_FILTER_1PIX	t3, t4, t5, t6, #1	;//9
			
			;//10, 11
			ldrb	t7, [pred_d], #-MB_SIZE_D
			strb	t5, [pred], #1	;//10
			FOUR_TAPS_FILTER_1PIX	t4, t5, t6, t7, #1	;//11
							  
			;//12, 13
			strb	t6, [pred], #1	;//12
			add		tmp, t6, t6, lsl #3
			sub		tmp, tmp, t5
			add		tmp, tmp, t7, lsl #3
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #1	;//13
			
			;//14
			strb	t7, [pred], #1
			
		;// up scale left line
			ldrb	t6, [pred_d], #-MB_SIZE_D
			
			;//15, 14
			add		tmp, t7, t7, lsl #4
			sub		tmp, tmp, t6
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_SIZE	;//15	
			strb	t7,  [pred], #-MB_SIZE	;//14
			
			;//13, 12
			ldrb	t5, [pred_d], #-MB_SIZE_D
			add		tmp, t6, t6, lsl #3
			add		tmp, tmp, t7, lsl #3
			sub		tmp, tmp, t5
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_SIZE	;//13
			strb	t6,  [pred], #-MB_SIZE	;//12
			
			;//11, 10
			ldrb	t4, [pred_d], #-MB_SIZE_D
			FOUR_TAPS_FILTER_1PIX	t4, t5, t6, t7, #-MB_SIZE	;//11
			strb	t5, [pred], #-MB_SIZE		;//10
			
			;//9, 8
			ldrb	t3, [pred_d], #-MB_SIZE_D
			FOUR_TAPS_FILTER_1PIX	t3, t4, t5, t6, #-MB_SIZE	;//9
			strb	t4, [pred], #-MB_SIZE		;//8
			
			;//7, 6
			ldrb	t2, [pred_d], #-MB_SIZE_D
			FOUR_TAPS_FILTER_1PIX	t2, t3, t4, t5, #-MB_SIZE	;//7
			strb	t3, [pred], #-MB_SIZE		;//6
			
			;//5, 4
			ldrb	t1, [pred_d], #-MB_SIZE_D
			FOUR_TAPS_FILTER_1PIX	t1, t2, t3, t4, #-MB_SIZE	;//5
			strb	t2, [pred], #-MB_SIZE		;//4
			
			;//3, 2
			ldrb	t0, [pred_d]
			FOUR_TAPS_FILTER_1PIX	t0, t1, t2, t3, #-MB_SIZE	;//3
			strb	t1, [pred], #-MB_SIZE		;//2
			
			add		tmp, t1, t1, lsl #3
			add		tmp, tmp, t0, lsl #3
			sub		tmp, tmp, t2
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_SIZE	;//1
			strb	t0,  [pred]				;//0
			
		;/////////////////////////////	
		
	;//CHROMA
			mov		uv, #0
UV_LOOP				
			
			add		pred, mb_cache_ptr, uv, lsl #6
			add		pred, pred, #0x138
			add		pred_d, mb_cache_ptr, uv, lsl #4
			add		pred_d, pred_d, #0x720
		
			;//0, 1
			ldrb	t0, [pred_d], #1
			ldrb	t1, [pred_d], #1
			ldrb	t2, [pred_d], #1
			strb	t0, [pred], #1		;//0
			add		tmp, t1, t1, lsl #3
			sub		tmp, tmp, t2
			add		tmp, tmp, t0, lsl #3
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #1		;//1
			
			;//2, 3
			ldrb	t3, [pred_d], #-MB_CHROMA_SIZE_D
			strb	t1, [pred], #1	;//2
			FOUR_TAPS_FILTER_1PIX	t4, t5, t6, t7, #1	;//3
			
			;//4, 5
			strb	t2, [pred], #1	;//4
			add		tmp, t2, t2, lsl #3
			sub		tmp, tmp, t1
			add		tmp, tmp, t3, lsl #3
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #1	;//13
			
			;//6
			strb	t3, [pred], #1
			
			;// up scale left line
			ldrb	t2, [pred_d], #-MB_CHROMA_SIZE_D
			
			;//7, 6
			add		tmp, t3, t3, lsl #4
			sub		tmp, tmp, t2
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_CHROMA_SIZE	;//7
			strb	t3,  [pred], #-MB_CHROMA_SIZE	;//6
			
			;//5, 4
			ldrb	t1, [pred_d], #-MB_CHROMA_SIZE_D
			add		tmp, t2, t2, lsl #3
			add		tmp, tmp, t3, lsl #3
			sub		tmp, tmp, t1
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_CHROMA_SIZE	;//5
			strb	t6,  [pred], #-MB_CHROMA_SIZE	;//4
			
			;//3, 2
			ldrb	t0, [pred_d]
			FOUR_TAPS_FILTER_1PIX	t0, t1, t2, t3, #-MB_CHROMA_SIZE	;//3
			strb	t1, [pred], #-MB_CHROMA_SIZE		;//2
			
			add		tmp, t1, t1, lsl #3
			add		tmp, tmp, t0, lsl #3
			sub		tmp, tmp, t2
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_CHROMA_SIZE	;//1
			strb	t0,  [pred]						;//0
			
			;///////////////////////////////////////
				
			add		uv, uv, #1
			cmp		uv, #2
			blt		UV_LOOP
			
	
			ldmfd	sp!, {r4 - r7, pc}
			
			END
			





