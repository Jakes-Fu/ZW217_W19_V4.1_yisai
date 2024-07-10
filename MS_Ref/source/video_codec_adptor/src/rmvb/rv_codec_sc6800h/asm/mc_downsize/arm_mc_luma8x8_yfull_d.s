;/**************************************************/
;arm_mc_luma8x8_yfull_d.s
;input
;r0: pRef
;r1: pPred
;r2: srcPitch
;r3: dx
;r4: line
;/**************************************************/

DST_WIDTH		EQU 	8

pRef			RN		0
pPred			RN		1

pClipTab		RN		2
dx 				RN 		3	;//only effective at first

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
		SIX_TAPS_FILTER_1PIX  $src_off, $p0, $p1, $p2, $p3, $p4, $p5
		ldrb	$p5, [pRef, $src_off]
		strb	r10, [pPred], #1
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

;/*********************************************************/
; 					main function begin here
;/**********************************************************/

		EXPORT	arm_mc_luma8x8_yfull_d 	
		IMPORT	g_rgiRvClipTab
		IMPORT 	g_intp_coeff
		
arm_mc_luma8x8_yfull_d

		stmfd	sp!, {r4 - r12, r14} 
		
		sub 	sp, sp, #4
		str 	r2, [sp]				;//src_pitch push to stack
		
		ldr		reg_tmp, =g_rgiRvClipTab
		ldr		pClipTab, [reg_tmp, #0]
		
		;//load filter coeff and rounding/shift_bits
		ldr		reg_tmp, =g_intp_coeff
		add 	reg_tmp, reg_tmp, dx, lsl #4
		ldmia 	reg_tmp, {coff2, coff3, rnd, shift_bits}		
		
		sub 	pRef, pRef, #2
		
ROW_LOOP	
		;/********load 6 pixels for filter***********/
		ldrb	pn2, [pRef, #0]
		ldrb	pn1, [pRef, #1]
		ldrb	pp0, [pRef, #2]
		ldrb	pp1, [pRef, #3]
		ldrb	pp2, [pRef, #4]
		
		;//pix0
		ldrb	pp3, [pRef, #5]
		add		r10, pn1, pp2
		smlabb	pn2, pp0, coff2, pn2
		add		r10, r10, r10, lsl #2
		rsb		r10, r10, pn2
		smlabb	pn2, pp1, coff3, pp3
		add		r10, r10, rnd
		add		pn2, pn2, r10
		mov 	pn2, pn2, asr shift_bits
		ldrb	r10, [pClipTab, pn2]	
		
		SIX_TAPS_FILTER_1PIX	#6, pn1, pp0, pp1, pp2, pp3, pp4	;//pix1
		SIX_TAPS_FILTER_1PIX	#7, pp0, pp1, pp2, pp3, pp4, pp5	;//pix2
		SIX_TAPS_FILTER_1PIX	#8, pp1, pp2, pp3, pp4, pp5, pp6	;//pix3		
		SIX_TAPS_FILTER_1PIX	#9, pp2, pp3, pp4, pp5, pp6, pp7	;//pix4
		SIX_TAPS_FILTER_1PIX	#10, pp3, pp4, pp5, pp6, pp7, pp8	;//pix5
		SIX_TAPS_FILTER_1PIX	#11, pp4, pp5, pp6, pp7, pp8, pp9	;//pix6
		SIX_TAPS_FILTER_1PIX	#12, pp5, pp6, pp7, pp8, pp9, pp10	;//pix7
		
		;/****************load src_ptch and loop_counter*************/
		ldr 	r4, [sp]
		ldr		row_cnt, [sp, #44]		
		
		add 	pRef, pRef, r4
		subs 	row_cnt, row_cnt, #1
		
		;/*****store the last interpolated pixel to pred buffer*****/
		strb	r10, [pPred], #1		
		
		str 	row_cnt, [sp, #44]
		bne		ROW_LOOP
		
		;//
		add		sp, sp, #4
		ldmfd	sp!, {r4 - r12, pc}	
		
		END
		
		