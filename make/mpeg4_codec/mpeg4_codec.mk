
MCFLAG_OPT   	= -D_ARM_


######################################################################################################
#                                MACRO DEFINITION
######################################################################################################


MCFLAG_OPT	+=  -D_MP4CODEC_DATA_PARTITION_


#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6800H)
MCFLAG_OPT	+= -D_VSP_  -DSC6800H_VIDEO_ROMCODE -DMPEG4_DEC -DMPEG4_ENC 
MCFLAG_OPT	+= -D_MEMSET_ASM_ -D_GETSAD16X16_ASM_ -D_DCT_ASM_ -D_ASM_HOR_EXTENSION_ -D_MC_ASM_ -D_IDCT_ASM_ -D_VLC_ASM_
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6530)
MCFLAG_OPT	+= -D_VSP_ -DSC6530_VIDEO_ROMCODE -DMPEG4_DEC -DMPEG4_ENC
MCFLAG_OPT	+= -D_MEMSET_ASM_ -D_GETSAD16X16_ASM_ -D_DCT_ASM_ -D_ASM_HOR_EXTENSION_ -D_MC_ASM_ -D_IDCT_ASM_ -D_VLC_ASM_
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MCFLAG_OPT	+= -D_VSP_ -DSC6530_VIDEO_ROMCODE -DMPEG4_DEC -DMPEG4_ENC
MCFLAG_OPT	+= -D_MEMSET_ASM_ -D_GETSAD16X16_ASM_ -D_DCT_ASM_ -D_ASM_HOR_EXTENSION_ -D_MC_ASM_ -D_IDCT_ASM_ -D_VLC_ASM_
endif

ifeq ($(strip $(PLATFORM)),	UIX8910_WITH_6530_VSP_CMODEL)
MCFLAG_OPT	+= -D_SIMULATION_ -DKAL_ON_RTT -DMPEG4_DEC -DMPEG4_ENC
#MCFLAG_OPT	+= -D_MEMSET_ASM_ -D_GETSAD16X16_ASM_ -D_DCT_ASM_ -D_ASM_HOR_EXTENSION_ -D_MC_ASM_ -D_IDCT_ASM_ -D_VLC_ASM_
MCFLAG_OPT	+= -D_MEMSET_ASM_ -D_GETSAD16X16_ASM_ -D_DCT_ASM_ -D_ASM_HOR_EXTENSION_ -D_MC_ASM_ -D_IDCT_ASM_
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
MCFLAG_OPT	+= -DKAL_ON_RTT -DMPEG4_DEC -DMPEG4_ENC -DCHIP_ENDIAN_LITTLE 

#for decoder neon. need makefile.rules "-cpu= cortex-a5.neon" for __TARGET_FEATURE_NEON  -D__ARM_NEON__ -D__TARGET_FEATURE_NEON -D__TARGET_FPU_VFP
MCFLAG_OPT += --cpu cortex-a5.neon
MCFLAG_OPT +=--fpu VFPv4
MCFLAG_OPT += -DIDCT_ASSEMBLY -DMC_ASSEMBLY

#for encoder
#MP4_ENC_ASM_ENABLE = TRUE
#MCFLAG_OPT	+= -DMP4_ENC_ASM_ENABLE -D_MEMSET_ASM_ -D_GETSAD16X16_ASM_ -D_DCT_ASM_ -D_ASM_HOR_EXTENSION_ -D_MC_ASM_ -D_IDCT_ASM_
endif

#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT	+= -D_VSP_ 
endif
#-----------------------------------------------------------------------------------------------------



######################################################################################################
#                                INCLUDE PATH and SOURCE FILE
######################################################################################################


            
#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6800H)
MINCPATH = 	MS_Ref/source/vsp/sc6800h/inc 														\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6800h/inc/common	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6800h/inc/decoder	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6800h/inc/encoder
			  
MSRCPATH		= MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6800h/src/decoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6800h/src/encoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6800h/src/encoder\asm

SOURCES			= mp4dec_bfrctrl.c mp4dec_bitstream.c mp4dec_datapartitioning.c mp4dec_global.c \
			  mp4dec_header.c mp4dec_interface.c  mp4dec_malloc.c mp4dec_mb.c mp4dec_mc.c mp4dec_mv.c\
			  mp4dec_session.c  mp4dec_table.c mp4dec_transpose.c mp4dec_vld.c mp4dec_vop.c
			  

SOURCES += 	mp4enc_bitstrm.c            					\
						mp4enc_block.c              					\
						mp4enc_dct.c                					\
						mp4enc_global.c             					\
						mp4enc_header.c             					\
						mp4enc_init.c               					\
						mp4enc_interface.c          					\
						mp4enc_malloc.c             					\
						mp4enc_mb.c                 					\
						mp4enc_mc.c                 					\
						mp4enc_me.c                 					\
						mp4enc_mv.c                 					\
						mp4enc_quant.c              					\
						mp4enc_table.c              					\
						mp4enc_vlc.c                					\
						mp4enc_vop.c                					\
						mp4enc_cmodel.c												\
						                            					\
						arm_AddErrAndRef.s										\
						arm_compterOneBlockErr.s    					\
						arm_coputeErrAndSad.s       					\
						arm_CopyRefBlk.s											\
						arm_copyRefToRecMB.s									\
						arm_fdct8x8_C_row.s         					\
						arm_fdct8x8_C_row_chroma.s						\
						arm_fdct8x8_int_row.s       					\
						arm_fdct8x8_int_col_inter_qp6.s       \
						arm_fdct8x8_int_col_intra_qp4.s       \
						arm_fdct8x8_int_col_intra_qp6.s       \
						arm_fdct8x8_int_row.s									\
						arm_fixIDCT_inter.s										\
						arm_fixIDCT_intra.s										\
						arm_GetMBA.s													\
						arm_GetSad16x16.s           					\
						arm_GetSad16x16_RefMB.s								\
						arm_InterCoeffNotLast.s								\
						arm_IntraCoeffNotLast.s								\
						arm_mc8x8_xfull_yfalf.s								\
						arm_mc8x8_xhalf_yfull.s								\
						arm_mc8x8_xyfull.s										\
						arm_mc8x8_xyhalf.s										\
						arm_mc16x16_xfull_yhalf.s							\
						arm_mc16x16_xhalf_yfull.s							\
						arm_mc16x16_xyfull.s									\
						arm_mc16x16_xyhalf.s									\
						arm_memset32WordToZero.s    					\
						arm_OutputBits.s            					\
						arm_PutIntraCoef_dc_only.s						\
						arm_ReadSrcMB.s												\
						asm_horExtendYUV.s
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6530)
MINCPATH = 	MS_Ref/source/vsp/sc6530/inc 														\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/common	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/decoder	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/encoder	\
			 		  MS_Ref/source/image_proc/inc 		
			  
MSRCPATH		= MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/decoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/encoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/common

SOURCES			= mp4_common_func.c mp4_common_table.c mp4dec_bfrctrl.c mp4dec_bitstream.c mp4dec_block.c mp4dec_command.c \
        mp4dec_datapartitioning.c mp4dec_global.c \
			  mp4dec_header.c mp4dec_interface.c  mp4dec_malloc.c mp4dec_mb.c mp4dec_mc.c mp4dec_mv.c\
			  mp4dec_session.c  mp4dec_table.c mp4dec_transpose.c mp4dec_vld.c mp4dec_vop.c mp4dec_isr.c
			  
SOURCES += 	mp4enc_bitstrm.c            					\
						mp4enc_block.c              					\
						mp4enc_cmodel.c                       \
						mp4enc_dct.c                					\
						mp4enc_global.c             					\
						mp4enc_header.c             					\
						mp4enc_init.c               					\
						mp4enc_interface.c          					\
						mp4enc_malloc.c             					\
						mp4enc_mb.c                 					\
						mp4enc_mc.c                 					\
						mp4enc_me.c                 					\
						mp4enc_mv.c                 					\
						mp4enc_quant.c              					\
						mp4enc_table.c              					\
						mp4enc_vlc.c                					\
						mp4enc_vop.c
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MINCPATH = 	MS_Ref/source/vsp/sc6530/inc 														\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/common	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/decoder	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/encoder	\
			 		  MS_Ref/source/image_proc/inc 		
			  
MSRCPATH		= MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/decoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/encoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/common

SOURCES			= mp4_common_func.c mp4_common_table.c mp4dec_bfrctrl.c mp4dec_bitstream.c mp4dec_block.c mp4dec_command.c \
        mp4dec_datapartitioning.c mp4dec_global.c \
			  mp4dec_header.c mp4dec_interface.c  mp4dec_malloc.c mp4dec_mb.c mp4dec_mc.c mp4dec_mv.c\
			  mp4dec_session.c  mp4dec_table.c mp4dec_transpose.c mp4dec_vld.c mp4dec_vop.c mp4dec_isr.c
			  
SOURCES += 	mp4enc_bitstrm.c            					\
						mp4enc_block.c              					\
						mp4enc_cmodel.c                       \
						mp4enc_dct.c                					\
						mp4enc_global.c             					\
						mp4enc_header.c             					\
						mp4enc_init.c               					\
						mp4enc_interface.c          					\
						mp4enc_malloc.c             					\
						mp4enc_mb.c                 					\
						mp4enc_mc.c                 					\
						mp4enc_me.c                 					\
						mp4enc_mv.c                 					\
						mp4enc_quant.c              					\
						mp4enc_table.c              					\
						mp4enc_vlc.c                					\
						mp4enc_vop.c
endif

#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	UWS6121E)
MINCPATH = 			MS_Ref/source/mpeg4_codec/mpeg4_codec_uws6121e/inc/decoder	\
			 		MS_Ref/source/image_proc/inc 		
			  
MSRCPATH		= MS_Ref/source/mpeg4_codec/mpeg4_codec_uws6121e/src/decoder	

SOURCES			=  mp4dec_bfrctrl.c mp4dec_bitstream.c mp4dec_block.c \
        				mp4dec_datapartitioning.c mp4dec_global.c \
			  		mp4dec_header.c mp4dec_interface.c  mp4dec_malloc.c mp4dec_mb.c mp4dec_mc.c mp4dec_mv.c\
			  		mp4dec_session.c  mp4dec_table.c mp4dec_vld.c mp4dec_vop.c\
			  		mp4dec_error_handle.c mp4dec_FixPointDCT.c mp4dec_gmc.c mp4dec_rvld.c
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
MINCPATH  += 	    		MS_Ref/source/mpeg4_codec/mpeg4_codec_uws6121e/inc/encoder	\
			       	  MS_Ref/source/image_proc/inc 		
			  
MSRCPATH		+= MS_Ref/source/mpeg4_codec/mpeg4_codec_uws6121e/src/common \
						MS_Ref/source/mpeg4_codec/mpeg4_codec_uws6121e/src/encoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_uws6121e/src/encoder/asm	
		  
SOURCES += 	mp4_common_table.c mp4enc_bitstrm.c            					\
						mp4enc_block.c              					\
						mp4enc_cmodel.c                       \
						mp4enc_dct.c                					\
						mp4enc_global.c             					\
						mp4enc_header.c             					\
						mp4enc_init.c               					\
						mp4enc_interface.c          					\
						mp4enc_malloc.c             					\
						mp4enc_mb.c                 					\
						mp4enc_mc.c                 					\
						mp4enc_me.c                 					\
						mp4enc_mv.c                 					\
						mp4enc_quant.c              					\
						mp4enc_table.c              					\
						mp4enc_vlc.c                					\
						mp4enc_vop.c			

ifeq ($(strip $(MP4_ENC_ASM)), TRUE)
SOURCES +=				arm_AddErrAndRef.s										\
						arm_compterOneBlockErr.s    					\
						arm_coputeErrAndSad.s       					\
						arm_CopyRefBlk.s											\
						arm_copyRefToRecMB.s									\
						arm_fdct8x8_C_row.s         					\
						arm_fdct8x8_C_row_chroma.s						\
						arm_fdct8x8_int_row.s       					\
						arm_fdct8x8_int_col_inter_qp6.s       \
						arm_fdct8x8_int_col_intra_qp4.s       \
						arm_fdct8x8_int_col_intra_qp6.s       \
						arm_fdct8x8_int_row.s									\
						arm_fixIDCT_inter.s										\
						arm_fixIDCT_intra.s										\
						arm_GetMBA.s													\
						arm_GetSad16x16.s           					\
						arm_GetSad16x16_RefMB.s								\
						arm_InterCoeffNotLast.s								\
						arm_IntraCoeffNotLast.s								\
						arm_mc8x8_xfull_yfalf.s								\
						arm_mc8x8_xhalf_yfull.s								\
						arm_mc8x8_xyfull.s										\
						arm_mc8x8_xyhalf.s										\
						arm_mc16x16_xfull_yhalf.s							\
						arm_mc16x16_xhalf_yfull.s							\
						arm_mc16x16_xyfull.s									\
						arm_mc16x16_xyhalf.s									\
						arm_memset32WordToZero.s    					\
						arm_OutputBits.s            					\
						arm_PutIntraCoef_dc_only.s						\
						arm_ReadSrcMB.s												\
						asm_horExtendYUV.s
endif

endif

ifeq ($(strip $(PLATFORM)),	UIX8910_WITH_6530_VSP_CMODEL)
MINCPATH = 	MS_Ref/source/vsp/sc6530/inc 														\
			MS_Ref/source/vsp/c_model/sc6530/inc	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/common	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/decoder	\
			  		MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/encoder	\
			 		  MS_Ref/source/image_proc/inc 		
			  
MSRCPATH		= MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/decoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/encoder	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/encoder/asm	\
							MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/src/common

SOURCES			= mp4_common_func.c mp4_common_table.c mp4dec_bfrctrl.c mp4dec_bitstream.c mp4dec_block.c mp4dec_command.c \
        mp4dec_datapartitioning.c mp4dec_global.c \
			  mp4dec_header.c mp4dec_interface.c  mp4dec_malloc.c mp4dec_mb.c mp4dec_mc.c mp4dec_mv.c\
			  mp4dec_session.c  mp4dec_table.c mp4dec_transpose.c mp4dec_vld.c mp4dec_vop.c mp4dec_isr.c
			  
SOURCES += 	mp4enc_bitstrm.c            					\
						mp4enc_block.c              					\
						mp4enc_cmodel.c                       \
						mp4enc_dct.c                					\
						mp4enc_global.c             					\
						mp4enc_header.c             					\
						mp4enc_init.c               					\
						mp4enc_interface.c          					\
						mp4enc_malloc.c             					\
						mp4enc_mb.c                 					\
						mp4enc_mc.c                 					\
						mp4enc_me.c                 					\
						mp4enc_mv.c                 					\
						mp4enc_quant.c              					\
						mp4enc_table.c              					\
						mp4enc_vlc.c                					\
						mp4enc_vop.c									\
																		\
						arm_AddErrAndRef.s										\
						arm_compterOneBlockErr.s    					\
						arm_coputeErrAndSad.s       					\
						arm_CopyRefBlk.s											\
						arm_copyRefToRecMB.s									\
						arm_fdct8x8_C_row.s         					\
						arm_fdct8x8_C_row_chroma.s						\
						arm_fdct8x8_int_row.s       					\
						arm_fdct8x8_int_col_inter_qp6.s       \
						arm_fdct8x8_int_col_intra_qp4.s       \
						arm_fdct8x8_int_col_intra_qp6.s       \
						arm_fdct8x8_int_row.s									\
						arm_fixIDCT_inter.s										\
						arm_fixIDCT_intra.s										\
						arm_GetMBA.s													\
						arm_GetSad16x16.s           					\
						arm_GetSad16x16_RefMB.s								\
						arm_InterCoeffNotLast.s								\
						arm_IntraCoeffNotLast.s								\
						arm_mc8x8_xfull_yfalf.s								\
						arm_mc8x8_xhalf_yfull.s								\
						arm_mc8x8_xyfull.s										\
						arm_mc8x8_xyhalf.s										\
						arm_mc16x16_xfull_yhalf.s							\
						arm_mc16x16_xhalf_yfull.s							\
						arm_mc16x16_xyfull.s									\
						arm_mc16x16_xyhalf.s									\
						arm_memset32WordToZero.s    					\
						arm_OutputBits.s            					\
						arm_PutIntraCoef_dc_only.s						\
						arm_ReadSrcMB.s												\
						asm_horExtendYUV.s
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC8800G)
MINCPATH =  MS_Ref/source/mpeg4_codec/mpeg4_codec_sc8800g/inc/common \
            MS_Ref/source/mpeg4_codec/mpeg4_codec_sc8800g/inc/decoder \
            MS_Ref/source/mpeg4_codec/mpeg4_codec_sc8800g/inc/encoder	\
            MS_Ref/source/vsp/sc8800g/inc															\
            MS_Ref/source/mpeg4_codec/mpeg4_codec_sc8800g/export_inc

MSRCPATH =  MS_Ref/source/mpeg4_codec/mpeg4_codec_sc8800g/src/common \
            MS_Ref/source/mpeg4_codec/mpeg4_codec_sc8800g/src/decoder \
            MS_Ref/source/mpeg4_codec/mpeg4_codec_sc8800g/src/encoder

SOURCES     =   mp4_common_func.c mp4_common_table.c \
                mp4dec_bfrctrl.c mp4dec_bitstream.c mp4dec_datapartitioning.c mp4dec_error_handle.c \
                mp4dec_global.c mp4dec_header.c mp4dec_interface.c mp4dec_malloc.c \
                mp4dec_mb.c mp4dec_mc.c mp4dec_mv.c mp4dec_session.c \
                mp4dec_table.c mp4dec_vld.c mp4dec_vop.c \
                mp4enc_bfrctrl.c mp4enc_bitstrm.c mp4enc_command.c \
                mp4enc_global.c mp4enc_header.c mp4enc_init.c mp4enc_interface.c \
                mp4enc_malloc.c mp4enc_mb.c mp4enc_me.c mp4enc_mv.c \
                mp4enc_ratecontrol.c mp4enc_table.c mp4enc_vlc.c \
                mp4enc_vop.c 
endif
#-----------------------------------------------------------------------------------------------------




MINCPATH += chip_drv/export/inc/outdated
