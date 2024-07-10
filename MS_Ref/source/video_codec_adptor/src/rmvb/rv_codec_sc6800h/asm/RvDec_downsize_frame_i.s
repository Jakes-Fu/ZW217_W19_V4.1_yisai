;/*************************************/
;RvDec_downsize_frame_i.s
;input:
;	r0: pCurrentPic
;	r1: rv_decoder_ptr
;/*************************************/

RV_Y_EXTEND_SIZE	EQU			16
RV_UV_EXTEND_SIZE	EQU			8

;/********register map**********/
pCurrentPic			RN		0
rv_decoder_ptr		RN		1
src					RN		2
dst					RN		3

;//for v
src1				RN		1
dst1				RN		12

pDecFrame			RN		0
src_offset			RN		2
dst_offset			RN		3

dst_height			RN		4
dst_width_mb		RN		5
src_ext_width		RN 		6

val0				RN		7
val1				RN		8

i					RN 		9
j					RN		10

tmp0				RN		11
tmp1				RN		14


			AREA	PROGRAM, CODE, READONLY
			
			CODE32
			
			MACRO
			COMBINE_FOUR_PIXEL	$s, $d 
			ldmia	$s!, {val0, val1}
			and		tmp0, val0, #0xff000000
			and		tmp1, val0, #0x0000ff00
			add		tmp0, tmp0, tmp1, lsl #8
			add		tmp0, tmp0, val1, lsr #16
			and		tmp0, tmp0, #0xffffff00
			and		tmp1, val1, #0x0000ff00
			add		tmp0, tmp0, tmp1, lsr #8			
			str		tmp0, [$d], #4
			MEND  
					
			IMPORT	g_RvFrmYUVBfr			
			EXPORT	RvDec_downsize_frame_i
			
			
RvDec_downsize_frame_i
			stmfd	sp!, {r4 - r12, r14}  

 
			ldr		 r7, =g_RvFrmYUVBfr
 			ldr      pDecFrame,[pCurrentPic,#8]
			ldrb     dst_width_mb,[rv_decoder_ptr, #2]
   			ldrh     dst_height,[rv_decoder_ptr,#0xd6]
			ldrh     src_offset,[rv_decoder_ptr,#0x14]
			ldrh     dst_offset,[rv_decoder_ptr,#0xdc]
			
			ldr		 r8, [r7, #(32*3)]
			ldr      r14,[pDecFrame,#0]
			ldrh     src_ext_width,[rv_decoder_ptr,#0x10]
				
			add      src,r8,src_offset
			add      dst,r14,dst_offset	
			
			mov		 i,dst_height					
Y_ROW_LOOP_DS			
			mov		 j,dst_width_mb
			
Y_COL_LOOP_DS		
			COMBINE_FOUR_PIXEL src, dst;//0	
			COMBINE_FOUR_PIXEL src, dst;//1
			
			subs	j, j, #1
			bne		Y_COL_LOOP_DS		
						    
			add      dst,dst,#RV_Y_EXTEND_SIZE
			add      src,src,#0x20	;//RV_Y_EXTEND_SIZE*2
		   	add      src,src,src_ext_width
			
			subs	i, i, #1
			bne		Y_ROW_LOOP_DS
				
			;//uv offset
   
			ldrh     r9,[rv_decoder_ptr,#0x16]
 
 			ldr		 r7, =g_RvFrmYUVBfr
 			ldrh     r10,[rv_decoder_ptr,#0xde]
 			
 			ldr		 r8, [r7, #(32*3+4)]
			
			;//u downsize
 
			ldr      r14,[pDecFrame,#4]
			add      src,r8,r9
 			add      dst,r14,r10	
 			
 			ldr		 r8, [r7, #(32*3+8)]
			ldr      r14,[pDecFrame,#8]
			add      src1,r8,r9
 			add      dst1,r14,r10	

			mov		 i,dst_height, asr #1							
UV_ROW_LOOP_DS	
		
			mov		 j,dst_width_mb			
UV_COL_LOOP_DS
					
			COMBINE_FOUR_PIXEL src, dst	;//u					
			COMBINE_FOUR_PIXEL src1, dst1;//v	
			
			subs	j, j, #1
			bne		UV_COL_LOOP_DS		
						    
			add      dst,dst,#RV_UV_EXTEND_SIZE
			add      src,src,#0x10	;//(RV_UV_EXTEND_SIZE*2)
		   	add      src,src,src_ext_width,asr #1
		   	
		   	add      dst1,dst1,#RV_UV_EXTEND_SIZE
			add      src1,src1,#0x10	;//(RV_UV_EXTEND_SIZE*2)
		   	add      src1,src1,src_ext_width,asr #1
					
			subs	i, i, #1
			bne		UV_ROW_LOOP_DS	
					

			ldmfd	sp!, {r4 - r12, pc}
			
			END
			



 

