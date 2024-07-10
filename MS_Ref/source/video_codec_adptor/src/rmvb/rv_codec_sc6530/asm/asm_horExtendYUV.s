;/horizontal Y and UV extension, Y extend 16 pixel each side, UV extend 8 pixel
;r0: *YUVFrame [3]
;r1: iStartY
;r2: iStartUV
;r3: extendWidth of Y
;r4: heigh of frame Y 

YUVFrame	RN		0
iStartY		RN		1
iStartUV	RN		2
extWidth	RN		3
height		RN		4

pSrc0		RN		5
pSrc1		RN		6
pDst0		RN		7
pDst1		RN		8

src0		RN		9
src1		RN		10
src2		RN		11
src3		RN		12
		
i			RN		14
tmp			RN		1

YEXTENT_SIZE		EQU		16
UVEXTEN_SIZE		EQU		8


			AREA	PROGRAM, CODE, READONLY
			
			export 	asm_horExtendYUV
			
asm_horExtendYUV
			stmfd	sp!, {r4 - r12, r14}
			ldr		height, [sp, #40]
			
			;/*Y horizontal extention*/
			ldr		r14, [YUVFrame, #0]
			add		pSrc0, r14, iStartY
			sub		pDst0, pSrc0, #YEXTENT_SIZE
			
			add		pSrc1, pSrc0, extWidth
			sub		pSrc1, pSrc1, #(YEXTENT_SIZE*2+1)
			add		pDst1, pSrc1, #1
			
			mov		i, height
Y_ROW_LOOP
			;//left side extension
			ldrb	tmp, [pSrc0], extWidth
			orr		tmp, tmp, tmp, lsl #8
			orr		src0, tmp, tmp, lsl #16
			mov		src1, src0
			mov		src2, src0
			mov		src3, src0
			stmia	pDst0, {src0, src1, src2, src3} 
			
			;//right side extension
			ldrb	tmp, [pSrc1], extWidth
			orr		tmp, tmp, tmp, lsl #8
			orr		src0, tmp, tmp, lsl #16
			mov		src1, src0
			mov		src2, src0
			mov		src3, src0
			stmia	pDst1, {src0, src1, src2, src3} 
			                
			add		pDst0, pDst0, extWidth
			add		pDst1, pDst1, extWidth

			subs	i, i, #1
			bne		Y_ROW_LOOP	
			
			
			
			;/*U horizontal extension*/
			mov	  	extWidth, extWidth, lsr #1
			ldr		r14, [YUVFrame, #4]
			add		pSrc0, r14, iStartUV
			sub		pDst0, pSrc0, #UVEXTEN_SIZE
			
			add		pSrc1, pSrc0, extWidth
			sub		pSrc1, pSrc1, #(UVEXTEN_SIZE*2+1)
			add		pDst1, pSrc1, #1			
			
			mov		i, height, lsr #1
U_ROW_LOOP
			ldrb	tmp, [pSrc0], extWidth
			orr		tmp, tmp, tmp, lsl #8
			orr		src0, tmp, tmp, lsl #16
			mov		src1, src0
			stmia	pDst0, {src0, src1} 
			
			
			ldrb	tmp, [pSrc1], extWidth
			orr		tmp, tmp, tmp, lsl #8
			orr		src0, tmp, tmp, lsl #16
			mov		src1, src0
			stmia	pDst1, {src0, src1}
			
			add		pDst0, pDst0, extWidth
			add		pDst1, pDst1, extWidth

			subs	i, i, #1
			bne		U_ROW_LOOP
			
			
			;//V horizontal extension
			ldr		r14, [YUVFrame, #8]
			add		pSrc0, r14, iStartUV
			sub		pDst0, pSrc0, #UVEXTEN_SIZE
			
			add		pSrc1, pSrc0, extWidth
			sub		pSrc1, pSrc1, #(UVEXTEN_SIZE*2+1)
			add		pDst1, pSrc1, #1			
			
			mov		i, height, lsr #1
V_ROW_LOOP
			ldrb	tmp, [pSrc0], extWidth
			orr		tmp, tmp, tmp, lsl #8
			orr		src0, tmp, tmp, lsl #16
			mov		src1, src0
			stmia	pDst0, {src0, src1} 
			
			
			ldrb	tmp, [pSrc1], extWidth
			orr		tmp, tmp, tmp, lsl #8
			orr		src0, tmp, tmp, lsl #16
			mov		src1, src0
			stmia	pDst1, {src0, src1}
			
			add		pDst0, pDst0, extWidth
			add		pDst1, pDst1, extWidth

			subs	i, i, #1
			bne		V_ROW_LOOP
			
			
			ldmfd	sp!, {r4 - r12, pc}
			end

						




		