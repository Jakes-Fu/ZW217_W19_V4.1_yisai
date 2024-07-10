;/**************************************/
; inverse transformation: arm_mc_itrans4x4_d.s
;input:
;	r0: pred
;	r1: coff
;   r2: width
;/**************************************/
;/************register map**************/
pred			RN			0
dst				RN			0
coff			RN			1
width			RN			2
src				RN			3 
i				RN			4

m0  			RN			5
m1				RN			6
m2				RN			7
m3				RN			8
							
n0				RN			9
n1				RN			m0
n2				RN			m2
n3				RN			m1

predPix			RN			m3

par13			RN			12

widthx3			RN			10
pClipTable		RN			11
tmp0			RN			14
tmp1			RN			1


				AREA	PROGRAM, CODE, READONLY
				
				CODE32
				
				IMPORT	g_rgiRvClipTab
				
rvdec_ITrans_MC_blk4x4_d	FUNCTION			
				EXPORT	rvdec_ITrans_MC_blk4x4_d
				

				stmfd	sp!, {r4 - r12, r14}
				
				mov		par13, #13
				
				;/******horizontal inverse transeform*******/
				mov		src, coff
				mov		i, #4
HOR_LOOP
				ldrsh	m0, [src, #0]
				ldrsh	m1, [src, #2]
				ldrsh	m2, [src, #4]
				ldrsh	m3, [src, #6]
				
				adds	n0, m0, m2
				mulne	n0, par13, n0
				subs	n1, m0, m2
				mulne	n1, par13, n1
				rsb		r10, m1, m1, lsl #3
				add		r14, m3, m3, lsl #4
				sub		n2, r10, r14
				add		r10, m1, m1, lsl #4
				rsb		r14, m3, m3, lsl #3
				add		n3, r10, r14
				
				add		tmp0, n0, n3
				strh	tmp0, [src], #2
				add		tmp0, n1, n2
				strh	tmp0, [src], #2
				sub		tmp0, n1, n2
				strh	tmp0, [src], #2
				sub		tmp0, n0, n3
				strh	tmp0, [src], #2
				
				subs	i, i, #1
				bne		HOR_LOOP
				
				;/*****vertical inverse transform
				; and add prediction and residual*****/ 				
				mov		src, coff
				ldr		tmp0, =g_rgiRvClipTab
				ldr		pClipTable, [tmp0, #0]
				add		widthx3, width, width, lsl #1
				
				mov		i, #2
				add		src, src, #2
VER_LOOP 	
				ldrsh	m0, [src, #(0*4*2)]
				ldrsh	m1, [src, #(1*4*2)]
				ldrsh 	m2, [src, #(2*4*2)]
				ldrsh 	m3, [src, #(3*4*2)]
				
				adds	n0, m0, m2
				mulne	n0, par13, n0
				subs	n1, m0, m2
				mulne	n1, par13, n1
				rsb		r1, m1, m1, lsl #3
				add		r14, m3, m3, lsl #4
				sub		n2, r1, r14
				add		r1, m1, m1, lsl #4
				rsb		r14, m3, m3, lsl #3
				add		n3, r1, r14
				
				ldrb	predPix, [pred, #0]
				add		tmp0, n1, n2
				add		tmp0, tmp0, #0x200
				add		predPix, predPix, tmp0, asr #10
				ldrb	tmp0, [pClipTable, predPix]
				
				ldrb	predPix, [pred, width]  ;/*for remove interlock time between ldrb and strb*/
				sub		tmp1, n0, n3
				strb	tmp0, [pred, #0] 
				add		tmp1, tmp1, #0x200
				add		predPix, predPix, tmp1, asr #10
				ldrb	tmp1, [pClipTable, predPix]
				 		
				subs	i, i, #1
				add		src, src,#4					
				strb	tmp1, [pred, width]							
				add		pred, pred, #1 			
				bne		VER_LOOP 				

				ldmfd	sp!, {r4 - r12, pc}
				ENDFUNC
				

;/**************************************/
; inverse transformation: arm_mc_itrans4x4_d.s
;input:
;	r0: pred
;	r1: DCCoeff
;/**************************************/
;/************register map**************/
pPred				RN			0
DCCoeff				RN			1
pClipTable0			RN			2
v					RN			1
rec1				RN			3
rec2				RN			4

				
rvdec_ITrans_MC_blk4x4_onlyDC_d 		FUNCTION
				EXPORT		rvdec_ITrans_MC_blk4x4_onlyDC_d
				
				stmfd	sp!, {r4, r14}
				
				mov		r14, #0xa9	;//13*13
				mov		r2, #0x200
				smlabb	v, DCCoeff, r14, r2
				
				ldr		r14, =g_rgiRvClipTab
				ldr		pClipTable0, [r14, #0]
				
				mov		v, v, asr #10
				
				
				ldrb	rec1, [pred, #0]
				ldrb	rec2, [pred, #1]
				
				add   	rec1, v, rec1
				ldrb  	rec1, [pClipTable0, rec1]
				add		rec2, v, rec2
				ldrb	rec2, [pClipTable0, rec2]
				
				strb	rec1, [pred, #0]
				ldrb	rec1, [pred, #8]
				strb	rec2, [pred, #1]
				ldrb	rec2, [pred, #9]
				
								
				add   	rec1, v, rec1
				ldrb  	rec1, [pClipTable0, rec1]
				add		rec2, v, rec2
				ldrb	rec2, [pClipTable0, rec2]
				
				
				strb	rec1, [pred, #8]
				strb	rec2, [pred, #9]		
				
	
				ldmfd	sp!, {r4, pc}
				ENDFUNC	

				END			
