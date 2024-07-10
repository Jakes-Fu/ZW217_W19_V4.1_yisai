;/*************************************/
;arm_down_sample_mb.s
;input:
;	r0: mb_cache_ptr
;/*************************************/

MB_SIZE				EQU			16
MB_CHROMA_SIZE		EQU			8
MB_SIZE_D			EQU			8
MB_CHROMA_SIZE_D	EQU			4

;/********register map**********/
mb_cache_ptr		RN		0
src					RN		1
dst					RN		2
i					RN		3
val0				RN		4
val1				RN		5

;//for v
src1				RN		6
dst1				RN		0

tmp0				RN		7
tmp1				RN		14


			AREA	PROGRAM, CODE, READONLY
			
			CODE32
			
			MACRO
			COMBINE_FOUR_PIXEL	$s, $d 
			ldmia	$s!, {val0, val1}
			and		tmp0, val1, #0xff000000
			and		tmp1, val1, #0x0000ff00
			add		tmp0, tmp0, tmp1, lsl #8
			and		tmp1, val0, #0xff000000
			add		tmp0, tmp0, tmp1, lsr #16
			and		tmp1, val0, #0x0000ff00
			add		tmp0, tmp0, tmp1, lsr #8			
			str		tmp0, [$d], #4
			MEND  
						
			EXPORT	Down_Sample_Mb
			
			
Down_Sample_Mb
			stmfd	sp!, {r4 - r7,r14}  
			
			add      src,mb_cache_ptr,#0x10
			
			;//mbpix_d offset 0x6d4
			add      dst,mb_cache_ptr,#0x400
			add      dst,dst,#0x2d4

			mov      i,#MB_SIZE_D
			
Y_ROW_LOOP_DS

			COMBINE_FOUR_PIXEL	src, dst ;//0
			COMBINE_FOUR_PIXEL	src, dst ;//1
			add		src, src, #MB_SIZE
			
			subs	i, i, #1
			bne		Y_ROW_LOOP_DS		
			
		;//uv downsize
   
   			;//u
			add      src,mb_cache_ptr,#0x100
			add      src,src,#MB_CHROMA_SIZE

			add      dst,mb_cache_ptr,#0x400
			add      dst,dst,#0x314
			
			;//v
			add      src1,mb_cache_ptr,#0x140
			add      src1,src1,#MB_CHROMA_SIZE

			add      dst1,mb_cache_ptr,#0x400
			add      dst1,dst1,#0x324

			mov      i,#MB_CHROMA_SIZE_D				
UV_ROW_LOOP_DS			
			
			COMBINE_FOUR_PIXEL	src, dst ;//u
			COMBINE_FOUR_PIXEL	src1, dst1 ;//v
			
			add		src, src, #MB_CHROMA_SIZE
			add		src1, src1, #MB_CHROMA_SIZE
							
			subs	i, i, #1
			bne		UV_ROW_LOOP_DS			

			ldmfd	sp!, {r4 - r7,pc}
			
			END
			



 

