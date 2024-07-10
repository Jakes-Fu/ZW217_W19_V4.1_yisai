;/*************************************/
;arm_copyLeftPredPixel.s
;input:
;	r0: pMBCache
;/*************************************/

MB_SIZE				EQU			0x10
MB_SIZEx2			EQU			0x20
MB_SIZEx3			EQU			0x30
MB_CHROMA_SIZE		EQU			8
MB_CHROMA_SIZEx2	EQU			0x10
MB_CHROMA_SIZEx3	EQU			0x18

;/********register map**********/
mb_cache_ptr		RN		0
dst					RN		1
src					RN		2
i					RN		3
val0				RN		4
val1				RN		5




			AREA	PROGRAM, CODE, READONLY
			
			CODE32
						
			EXPORT	copyLeftPredPixel
			
			
copyLeftPredPixel
			stmfd	sp!, {r4,r5,r14}  

 
			add      src,mb_cache_ptr,#0xf
  
			add      dst,mb_cache_ptr,#0x400
			add      dst,dst,#0x334
	
		
			mov		 i,#4
			
					
Y_ROW_LOOP_DS			
			
			ldrb     val0,[src],#MB_SIZE
			ldrb     val1,[src],#MB_SIZE
			strb     val0,[dst],#1
			ldrb     val0,[src],#MB_SIZE	
			strb     val1,[dst],#1
			ldrb     val1,[src],#MB_SIZE
			strb     val0,[dst],#1
			strb     val1,[dst],#1
		
			subs	i, i, #1
			bne		Y_ROW_LOOP_DS
			

			;//
			add      src,mb_cache_ptr,#0x100
			add		 src,src,#0x7
			
  			add      dst,mb_cache_ptr,#0x400
			add      dst,dst,#0x344	
				
			;//0
			ldrb     val0,[src],#MB_CHROMA_SIZE
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			ldrb     val0,[src],#MB_CHROMA_SIZE
			strb     val1,[dst],#1
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			strb     val1,[dst],#1
		
			;//1
			ldrb     val0,[src],#MB_CHROMA_SIZE
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			ldrb     val0,[src],#MB_CHROMA_SIZE
			strb     val1,[dst],#1
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			strb     val1,[dst],#1	
		
			
 			;//v
			add      src,mb_cache_ptr,#0x140
			add		 src,src,#0x7
			
  			add      dst,mb_cache_ptr,#0x400
			add      dst,dst,#0x34c	
					
			;//0
			ldrb     val0,[src],#MB_CHROMA_SIZE
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			ldrb     val0,[src],#MB_CHROMA_SIZE
			strb     val1,[dst],#1
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			strb     val1,[dst],#1
			
			;//1
			ldrb     val0,[src],#MB_CHROMA_SIZE
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			ldrb     val0,[src],#MB_CHROMA_SIZE
			strb     val1,[dst],#1
			ldrb     val1,[src],#MB_CHROMA_SIZE
			strb     val0,[dst],#1
			strb     val1,[dst],#1
	
			ldmfd	sp!, {r4,r5,pc}
			
			END
			



 

