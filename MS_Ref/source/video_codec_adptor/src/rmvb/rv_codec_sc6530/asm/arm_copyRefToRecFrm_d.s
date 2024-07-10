;/*************************************/
;arm_copy_ref_MB_d.s
;input:
;	r0: rv_decoder_ptr
;	r1: pRecFrame[3]
;/*************************************/

MB_SIZE				EQU			16
MB_SIZE_HALF		EQU			8
MB_CHROMA_SIZE		EQU			8
MB_CHROMA_SIZE_HALF	EQU			4
WIDTH_OFFSET		EQU			0xd8
REC_OFFSET			EQU			(18*4)
REF_OFFSET			EQU			(19*4)	;//back ref


;/********register map**********/
rv_decoder_ptr		RN			0
ppxlcRecMBYUV		RN			1
pRecFrame			RN			2
pRefFrame			RN			3
width				RN			4
offset				RN			5

i					RN			0
src					RN			6
dst					RN			7

pix0				RN			8
pix1				RN			9

pred				RN			7
pred_d				RN			6
mb_cache_ptr		RN			10

t0					RN			8
t1					RN			9
t2					RN			11
t3					RN			14
t4					RN			8
t5					RN			9
t6					RN			11
t7					RN			14

tmp					RN			12	
uv					RN			0

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
						
			EXPORT	copyRefToRecFrm_d
						
			
copyRefToRecFrm_d
			stmfd	sp!, {r4 - r12, r14}  
			
			ldr		mb_cache_ptr, [rv_decoder_ptr, #40]
			
			ldrh	width, [rv_decoder_ptr, #WIDTH_OFFSET]
			ldr		r14, [rv_decoder_ptr, #REF_OFFSET]
			ldr		pRefFrame, [r14, #8]
			ldr		r14, [rv_decoder_ptr, #REC_OFFSET]
			ldr		pRecFrame, [r14, #8]
							
			;/*put Y */	
			ldr		dst, [ppxlcRecMBYUV, #0]
			ldr		r14, [pRecFrame, #0]
			ldr		src, [pRefFrame], #4
			sub		offset, dst, r14
			add		src, src, offset
			 
			mov		i, #MB_SIZE_HALF			
Y_ROW_LOOP
			;//row 0
			ldmia	src, {pix0, pix1}
			add		src, src, width
			stmia	dst, {pix0, pix1}		
			add		dst, dst, width

			;//row 1
			ldmia	src, {pix0, pix1}
			add		src, src, width
			stmia	dst, {pix0, pix1}		
			add		dst, dst, width	
			
			subs	i, i, #2
			bne		Y_ROW_LOOP
			
		;/////////////////////////////
			add		pred, mb_cache_ptr, #240	;//(15*16)
			sub		pred_d, src, width
			
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
			ldrb	t7, [pred_d], -width
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
			ldrb	t6, [pred_d], -width
			
			;//15, 14
			add		tmp, t7, t7, lsl #4
			sub		tmp, tmp, t6
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_SIZE	;//15	
			strb	t7,  [pred], #-MB_SIZE	;//14
			
			;//13, 12
			ldrb	t5, [pred_d], -width
			add		tmp, t6, t6, lsl #3
			add		tmp, tmp, t7, lsl #3
			sub		tmp, tmp, t5
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_SIZE	;//13
			strb	t6,  [pred], #-MB_SIZE	;//12
			
			;//11, 10
			ldrb	t4, [pred_d], -width
			FOUR_TAPS_FILTER_1PIX	t4, t5, t6, t7, #-MB_SIZE	;//11
			strb	t5, [pred], #-MB_SIZE		;//10
			
			;//9, 8
			ldrb	t3, [pred_d], -width
			FOUR_TAPS_FILTER_1PIX	t3, t4, t5, t6, #-MB_SIZE	;//9
			strb	t4, [pred], #-MB_SIZE		;//8
			
			;//7, 6
			ldrb	t2, [pred_d], -width
			FOUR_TAPS_FILTER_1PIX	t2, t3, t4, t5, #-MB_SIZE	;//7
			strb	t3, [pred], #-MB_SIZE		;//6
			
			;//5, 4
			ldrb	t1, [pred_d], -width
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
						
			;/***put U***/
			
			ldr		dst, [ppxlcRecMBYUV, #4]
			ldr		r14, [pRecFrame, #4]
			mov		width, width, lsr #1
			sub		offset, dst, r14
						
			mov		uv, #2
UV_LOOP				
			ldr		src, [pRefFrame], #4
			add		src, src, offset
			

			ldr		pix0, [src], width
			ldr		pix1, [src], width
			
			str		pix0, [dst], width
			str		pix1, [dst], width
			
			ldr		pix0, [src], width
			ldr		pix1, [src]	;//, width
			
			str		pix0, [dst], width
			str		pix1, [dst], width
						
			
			;///////////////////////////////////////
			add		pred, mb_cache_ptr, #0x100;//16X16+7*8
			
			cmp		uv,#2
			addeq	pred, pred, #56
			addne	pred, pred, #120
			
		;//	sub		pred_d, src, width
		
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
			ldrb	t3, [pred_d], -width
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
			ldrb	t2, [pred_d], -width
			
			;//7, 6
			add		tmp, t3, t3, lsl #4
			sub		tmp, tmp, t2
			add		tmp, tmp, #8
			mov		tmp, tmp, asr #4
			strb	tmp, [pred], #-MB_CHROMA_SIZE	;//7
			strb	t3,  [pred], #-MB_CHROMA_SIZE	;//6
			
			;//5, 4
			ldrb	t1, [pred_d], -width
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
			
			;/***put V***/
			ldr		dst, [ppxlcRecMBYUV, #8]
			
			subs	uv, uv, #1
			bne		UV_LOOP
			

			ldmfd	sp!, {r4 - r12, pc}
			
			END
			





