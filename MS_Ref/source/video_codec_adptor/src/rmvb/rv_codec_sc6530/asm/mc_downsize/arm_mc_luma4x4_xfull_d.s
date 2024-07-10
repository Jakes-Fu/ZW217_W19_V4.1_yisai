;/**************************************************/
;arm_mc_luma4x4_xfull_d.s
;input
;r0: pRef
;r1: pPred
;r2: srcPitch
;r3: dy
;/**************************************************/

DST_WIDTH		EQU 	8

pRef			RN		0
pPred			RN		1

pClipTab		RN		2
dy 				RN 		3	;//only effective at first

pn2				RN		3
pn1				RN		4
pp0				RN		5
pp1				RN		6
pp2				RN		7
pp3				RN		8
pp4				RN		3
pp5				RN		4
pp6				RN		5
pp7				RN		6
pp8				RN		7
pp9				RN		8
pp10			RN		3

coff2			RN		9
coff3			RN		11

reg_tmp 		RN 		10
;src_pitch 		RN 		10
row_cnt			RN 		3

rnd				RN 		12
shift_bits 		RN 		14


		AREA	PROGRAM, CODE, READONLY
		
		CODE32	
		
		MACRO
		SIX_TAPS_FILTER_1PIX  $dst_off, $p0, $p1, $p2, $p3, $p4, $p5
		ldr 	$p5, [sp, #0]
		strb	r10, [pPred, $dst_off]
		ldrb	$p5, [pRef], $p5
		add		r10, $p1, $p4
		smlabb	$p0, $p2, coff2, $p0
		add		r10, r10, r10, lsl #2
		rsb		r10, r10, $p0
		smlabb	$p0, $p3, coff3, $p5
		add		r10, r10, rnd
		add		$p0, $p0, r10
		mov		$p0, $p0, asr shift_bits
		ldrb	r10, [pClipTab, $p0]	
		
		MEND 
		
		
;/**********************************************************/
; 					main function begin here
;/**********************************************************/

		EXPORT	arm_mc_luma4x4_xfull_d 	
		IMPORT	g_rgiRvClipTab
		IMPORT 	g_intp_coeff
		
arm_mc_luma4x4_xfull_d

		stmfd	sp!, {r4 - r12, r14} 		
				
		;;//src_pitch and loop_counter push to stack		
		sub 	sp, sp, #8
		str 	r2, [sp, #0]			;//src_pitch
		mov		r10, #4
		str 	r10, [sp, #4] 			;//loop_cnt		
		
		ldr 	r10, [sp]
		
		ldr		r5, =g_rgiRvClipTab
		ldr		pClipTab, [r5, #0]
		
		;//load filter coeff and rounding/shift_bits
		ldr		r5, =g_intp_coeff
		add 	r5, r5, dy, lsl #4
		ldmia 	r5, {coff2, coff3, rnd, shift_bits}		
		
		sub 	pRef, pRef, r10, lsl #1 		;//ref point to first pixel used for filter		
		
ROW_LOOP	
		;/********load 6 pixels for filter***********/
		
		ldrb	pn2, [pRef], r10
		ldrb	pn1, [pRef], r10
		ldrb	pp0, [pRef], r10
		ldrb	pp1, [pRef], r10
		ldrb	pp2, [pRef], r10
		
		;//pix0
		ldrb	pp3, [pRef], r10
		add		r10, pn1, pp2
		smlabb	pn2, pp0, coff2, pn2
		add		r10, r10, r10, lsl #2
		rsb		r10, r10, pn2
		smlabb	pn2, pp1, coff3, pp3
		add		r10, r10, rnd
		add		pn2, pn2, r10
		mov		pn2, pn2, asr shift_bits
		ldrb	r10, [pClipTab, pn2]	
		
		SIX_TAPS_FILTER_1PIX	#0, pn1, pp0, pp1, pp2, pp3, pp4	;//pix1
		SIX_TAPS_FILTER_1PIX	#8, pp0, pp1, pp2, pp3, pp4, pp5	;//pix2
		SIX_TAPS_FILTER_1PIX	#16, pp1, pp2, pp3, pp4, pp5, pp6	;//pix3		
		
		;/***********load loop_counter*********/
		ldr 	row_cnt, [sp, #4]
		subs 	row_cnt, row_cnt, #1		
		
		;/*****store the last interpolated pixel to pred buffer*****/
		strb	r10, [pPred, #24]
		add 	pPred, pPred, #1		
		
		;/***********load src_pitch*********/
		ldr 	r10, [sp, #0]						
		add 	r4, r10, r10, lsl #3		
		sub 	pRef, pRef, r4
		add 	pRef, pRef, #1			
		
		str 	row_cnt, [sp, #4]
		bne		ROW_LOOP
		
		;//restore the context
		add		sp, sp, #8
		ldmfd	sp!, {r4 - r12, pc}	
		
		END
		
		
		