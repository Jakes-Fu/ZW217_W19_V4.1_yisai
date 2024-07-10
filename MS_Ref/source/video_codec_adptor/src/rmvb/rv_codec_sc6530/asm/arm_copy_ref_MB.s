;/*************************************/
;arm_copy_ref_MB.s
;input:
;	r0: pRefFrame[3]
;	r1: pMBPix
;	r2: pitch
;	r3: iMbPosX
;	r4: iMbPosY
;/*************************************/

MB_SIZE				EQU			16
MB_CHROMA_SIZE		EQU			8
WIDTH_OFFSET		EQU			16


;/********register map**********/

pRefFrame			RN			0
pMBPix				RN			1
pitch				RN			2
iMbPosX				RN			3
iMbPosY				RN			4

offset				RN			10

i					RN			5
src					RN			6
dst					RN			1

pix0				RN			7
pix1				RN			8
pix2				RN			9
pix3				RN			14


			AREA	PROGRAM, CODE, READONLY
			
			CODE32
						
			EXPORT	copy_ref_MB
			
			
copy_ref_MB
			stmfd	sp!, {r4 - r11, r14}  
			
			ldr		iMbPosY, [sp, #36]
				
			;/*put Y */	
			ldr		src, [pRefFrame, #0]
			mla		offset, iMbPosY, pitch, iMbPosX
			mov		i, #MB_SIZE			
			add		src, src, offset
Y_ROW_LOOP
			;//row 0
			ldmia	src, {pix0, pix1, pix2, pix3}
			add		src, src, pitch
			stmia	dst!, {pix0, pix1, pix2, pix3}		

			;//row 1
			ldmia	src, {pix0, pix1, pix2, pix3}
			add		src, src, pitch
			stmia	dst!, {pix0, pix1, pix2, pix3}		
			
			subs	i, i, #2
			bne		Y_ROW_LOOP
						
			;/***put U***/
			mov		pitch, pitch, asr #1
			mov		iMbPosY, iMbPosY, asr #1
			mov		iMbPosX, iMbPosX, asr #1
			mla		offset, iMbPosY, pitch, iMbPosX
			ldr		src, [pRefFrame, #4]	
			mov		i, #MB_CHROMA_SIZE
			add		src, src, offset
U_ROW_LOOP
			ldmia	src, {pix0, pix1}
			add		src, src, pitch
			stmia	dst!, {pix0, pix1}

			ldmia	src, {pix0, pix1}
			add		src, src, pitch
			stmia	dst!, {pix0, pix1}
			
			subs	i, i, #2
			bne		U_ROW_LOOP
			
			;/***put V***/
			ldr		src, [pRefFrame, #8]	
			mov		i, #MB_CHROMA_SIZE
			add		src, src, offset
V_ROW_LOOP
			ldmia	src, {pix0, pix1}
			add		src, src, pitch
			stmia	dst!, {pix0, pix1}

			ldmia	src, {pix0, pix1}
			add		src, src, pitch
			stmia	dst!, {pix0, pix1}
			
			subs	i, i, #2	
			bne		V_ROW_LOOP

			ldmfd	sp!, {r4 - r11, pc}
			
			END
			





