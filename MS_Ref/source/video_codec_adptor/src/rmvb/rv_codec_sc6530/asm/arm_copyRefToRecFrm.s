;/*************************************/
;arm_copy_ref_MB.s
;input:
;	r0: rv_decoder_ptr
;	r1: pRecFrame[3]
;/*************************************/

MB_SIZE				EQU			16
MB_CHROMA_SIZE		EQU			8
WIDTH_OFFSET		EQU			16
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
pix2				RN			10
pix3				RN			14


			AREA	PROGRAM, CODE, READONLY
			
			CODE32
						
			EXPORT	copyRefToRecFrm
			
			
copyRefToRecFrm
			stmfd	sp!, {r4 - r11, r14}  
			
			ldrh	width, [rv_decoder_ptr, #WIDTH_OFFSET]
			ldr		r14, [rv_decoder_ptr, #REF_OFFSET]
			ldr		pRefFrame, [r14, #8]
			ldr		r14, [rv_decoder_ptr, #REC_OFFSET]
			ldr		pRecFrame, [r14, #8]
							
			;/*put Y */	
			ldr		dst, [ppxlcRecMBYUV, #0]
			ldr		r14, [pRecFrame, #0]
			ldr		src, [pRefFrame, #0]
			sub		offset, dst, r14
			add		src, src, offset
			 
			mov		i, #MB_SIZE			
Y_ROW_LOOP
			;//row 0
			ldmia	src, {pix0, pix1, pix2, pix3}
			add		src, src, width
			stmia	dst, {pix0, pix1, pix2, pix3}		
			add		dst, dst, width

			;//row 1
			ldmia	src, {pix0, pix1, pix2, pix3}
			add		src, src, width
			stmia	dst, {pix0, pix1, pix2, pix3}		
			add		dst, dst, width	
			
			subs	i, i, #2
			bne		Y_ROW_LOOP
						
			;/***put U***/
			mov		width, width, lsr #1
			ldr		dst, [ppxlcRecMBYUV, #4]
			ldr		r14, [pRecFrame, #4]
			ldr		src, [pRefFrame, #4]
			sub		offset, dst, r14
			add		src, src, offset

			mov		i, #MB_CHROMA_SIZE
U_ROW_LOOP
			ldmia	src, {pix0, pix1}
			add		src, src, width
			stmia	dst, {pix0, pix1}
			add		dst, dst, width

			ldmia	src, {pix0, pix1}
			add		src, src, width
			stmia	dst, {pix0, pix1}
			add		dst, dst, width
			
			subs	i, i, #2
			bne		U_ROW_LOOP
			
			;/***put V***/
			ldr		src, [pRefFrame, #8]
			ldr		dst, [ppxlcRecMBYUV, #8]
			add		src, src, offset	
			mov		i, #MB_CHROMA_SIZE
V_ROW_LOOP
			ldmia	src, {pix0, pix1}
			add		src, src, width
			stmia	dst, {pix0, pix1}
			add		dst, dst, width

			ldmia	src, {pix0, pix1}
			add		src, src, width
			stmia	dst, {pix0, pix1}
			add		dst, dst, width
			
			subs	i, i, #2	
			bne		V_ROW_LOOP

			ldmfd	sp!, {r4 - r11, pc}
			
			END
			





