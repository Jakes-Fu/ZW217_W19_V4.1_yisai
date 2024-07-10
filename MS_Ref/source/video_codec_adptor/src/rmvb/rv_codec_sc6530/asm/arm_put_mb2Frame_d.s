;/*************************************/
;arm_put_mb2Frame_d.s
;input:
;	r0: rv_decoder_ptr
;	r1: mb_cache_ptr
;	r2: ppxlcRecMBYUV[3]
;/*************************************/


MB_SIZE_HALF		EQU			8
MB_CHROMA_SIZE_HALF	EQU			4
WIDTH_OFFSET		EQU			216
BUF_OFFSET0			EQU			0x400
BUF_OFFSET1			EQU			0x2d4


;/********register map**********/

rv_decoder_ptr		RN			0
mb_cache_ptr		RN			1
ppxlcRecMBYUV		RN			2
width				RN			3
i					RN			4
src					RN			1
dst					RN			5
pix0				RN			6
pix1				RN			7



			AREA	PROGRAM, CODE, READONLY
			
			CODE32
						
			EXPORT	RvDec_WriteRecMB2Frame_d
			
			
RvDec_WriteRecMB2Frame_d
			stmfd	sp!, {r4 - r8, r14}  
			
			ldrsh	width, [rv_decoder_ptr, #WIDTH_OFFSET]			
		
			;/*put Y */	
			ldr		dst, [ppxlcRecMBYUV, #0]
			mov		i, #MB_SIZE_HALF			
			add		src, src, #BUF_OFFSET0
			add		src, src, #BUF_OFFSET1
Y_ROW_LOOP
			ldmia	src!, {pix0, pix1}
			subs	i, i, #1
			stmia	dst, {pix0, pix1}		
			add		dst, dst, width
			
			bne		Y_ROW_LOOP
						
			;/***put U***/
			mov		width, width, lsr #1	
 			
			ldr		dst, [ppxlcRecMBYUV, #4]	
			mov		i, #MB_CHROMA_SIZE_HALF
U_ROW_LOOP
			ldmia	src!, {pix0}
			subs	i, i, #1
			stmia	dst, {pix0}
			add		dst, dst, width
			
			bne		U_ROW_LOOP
			
			;/***put V***/
			ldr		dst, [ppxlcRecMBYUV, #8]	
			mov		i, #MB_CHROMA_SIZE_HALF
V_ROW_LOOP
			ldmia	src!, {pix0}
			subs	i, i, #1
			stmia	dst, {pix0}
			add		dst, dst, width
			
			bne		V_ROW_LOOP

			ldmfd	sp!, {r4 - r8, pc}
			
			END
			





