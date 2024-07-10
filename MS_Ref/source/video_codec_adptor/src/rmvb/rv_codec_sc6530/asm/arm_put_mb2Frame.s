;/*************************************/
;arm_put_mb2Frame.s
;input:
;	r0: rv_decoder_ptr
;	r1: mb_cache_ptr
;	r2: ppxlcRecMBYUV[3]
;/*************************************/


MB_SIZE				EQU			16
MB_CHROMA_SIZE		EQU			8
WIDTH_OFFSET		EQU			16


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
pix2				RN			8
pix3				RN			14


			AREA	PROGRAM, CODE, READONLY
			
			CODE32
						
			EXPORT	arm_put_mb2Frame_rv
			
			
arm_put_mb2Frame_rv
			stmfd	sp!, {r4 - r8, r14}  
			
			ldrsh	width, [rv_decoder_ptr, #WIDTH_OFFSET]			
		
			;/*put Y */	
			ldr		dst, [ppxlcRecMBYUV, #0]
			mov		i, #MB_SIZE			
Y_ROW_LOOP
			ldmia	src!, {pix0, pix1, pix2, pix3}
			subs	i, i, #1
			stmia	dst, {pix0, pix1, pix2, pix3}		
			add		dst, dst, width
			
			bne		Y_ROW_LOOP
						
			;/***put U***/
			mov		width, width, lsr #1	
 			
			ldr		dst, [ppxlcRecMBYUV, #4]	
			mov		i, #MB_CHROMA_SIZE
U_ROW_LOOP
			ldmia	src!, {pix0, pix1}
			subs	i, i, #1
			stmia	dst, {pix0, pix1}
			add		dst, dst, width
			
			bne		U_ROW_LOOP
			
			;/***put V***/
			ldr		dst, [ppxlcRecMBYUV, #8]	
			mov		i, #MB_CHROMA_SIZE
V_ROW_LOOP
			ldmia	src!, {pix0, pix1}
			subs	i, i, #1
			stmia	dst, {pix0, pix1}
			add		dst, dst, width
			
			bne		V_ROW_LOOP

			ldmfd	sp!, {r4 - r8, pc}
			
			END
			





