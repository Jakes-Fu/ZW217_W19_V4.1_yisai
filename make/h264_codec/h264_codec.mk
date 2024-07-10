
MCFLAG_OPT   	= -D_ARM_


######################################################################################################
#                                MACRO DEFINITION
######################################################################################################



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT	+= -D_VSP_ 
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6800H)
MCFLAG_OPT += -DSC6800H_VIDEO_ROMCODE	-D_ARM926EJ_S_ 
MCFLAG_OPT += -DPUT_MB_ASSEMLY -DITRANS_ASSEMBLY -DDBK_ARM_OPT -D_ARM_MC_ASSEMBLY_ -D_ASM_HOR_EXTENSION_
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6530)
MCFLAG_OPT += -DSC6530_VIDEO_ROMCODE -D_ARM926EJ_S_ 
MCFLAG_OPT += -DPUT_MB_ASSEMLY -DITRANS_ASSEMBLY -DDBK_ARM_OPT -D_ARM_MC_ASSEMBLY_ -D_ASM_HOR_EXTENSION_

#must add this for error stream
MCFLAG_OPT += -DH264_STREAMING_SUPPORT

endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MCFLAG_OPT += -DSC6531EFM_VIDEO_ROMCODE -D_ARM926EJ_S_ 
MCFLAG_OPT += -DPUT_MB_ASSEMLY -DITRANS_ASSEMBLY -DDBK_ARM_OPT -D_ARM_MC_ASSEMBLY_ -D_ASM_HOR_EXTENSION_

#must add this for error stream
MCFLAG_OPT += -DH264_STREAMING_SUPPORT

endif

ifeq ($(strip $(PLATFORM)),	UIX8910_for6531efm)
MCFLAG_OPT += -DSC6531EFM_VIDEO_ROMCODE -D_ARM926EJ_S_ 
MCFLAG_OPT += -DPUT_MB_ASSEMLY -DITRANS_ASSEMBLY -DDBK_ARM_OPT -D_ARM_MC_ASSEMBLY_ -D_ASM_HOR_EXTENSION_

#must add this for error stream
MCFLAG_OPT += -DH264_STREAMING_SUPPORT

endif

ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
#UIX8910_H264 = TRUE
#UIX8910_H264_V2 = TRUE
UIX8910_H264_V3 = TRUE
endif

ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E)" ""
#UWS6121E_H264 = TRUE
#UWS6121E_H264_V2 = TRUE
UWS6121E_H264_V3 = TRUE
endif

ifeq ($(strip $(UIX8910_H264)),	TRUE)
#MCFLAG_OPT += -DSC6530_VIDEO_ROMCODE -D_ARM926EJ_S_ 
MCFLAG_OPT += --cpu cortex-a5.neon
MCFLAG_OPT +=--fpu VFPv4
#MCFLAG_OPT += -DPUT_MB_ASSEMLY -DITRANS_ASSEMBLY -DDBK_ARM_OPT -D_ARM_MC_ASSEMBLY_ -D_ASM_HOR_EXTENSION_
MCFLAG_OPT += -DPUT_MB_ASSEMLY -DDBK_ARM_OPT

#must add this for error stream
MCFLAG_OPT += -DH264_STREAMING_SUPPORT

endif


ifeq ($(strip $(UIX8910_H264_V2)),	TRUE)
#for v2
MCFLAG_OPT += --cpu cortex-a5.neon
MCFLAG_OPT +=--fpu VFPv4
MCFLAG_OPT += -DPUT_MB_ASSEMLY -DITRANS_ASSEMBLY -DDBK_ARM_OPT -D_ARM_MC_ASSEMBLY_ -D_ASM_HOR_EXTENSION_
#MCFLAG_OPT += -DPUT_MB_ASSEMLY -DITRANS_ASSEMBLY  -DDBK_ARM_OPT  -D_ASM_HOR_EXTENSION_
endif

ifeq ($(strip $(UIX8910_H264_V3)),	TRUE)
MCFLAG_OPT += --cpu cortex-a5.neon
MCFLAG_OPT +=--fpu VFPv4
#MCFLAG_OPT += -DPUT_MB_ASSEMLY -DDBK_ARM_OPT
USE_ITRANS_ASSEMBLY = FALSE
#MCFLAG_OPT += -DITRANS_ASSEMBLY
MCFLAG_OPT += -D_NEON_OPT_
#MCFLAG_OPT += -D_NEON_OPT2_

#must add this for error stream
MCFLAG_OPT += -DH264_STREAMING_SUPPORT

endif

ifeq ($(strip $(UWS6121E_H264_V3)),	TRUE)
MCFLAG_OPT += --cpu cortex-a5.neon
MCFLAG_OPT +=--fpu VFPv4
#MCFLAG_OPT += -DPUT_MB_ASSEMLY -DDBK_ARM_OPT
USE_ITRANS_ASSEMBLY = FALSE
#MCFLAG_OPT += -DITRANS_ASSEMBLY
MCFLAG_OPT += -D_NEON_OPT_
#MCFLAG_OPT += -D_NEON_OPT2_

#must add this for error stream
MCFLAG_OPT += -DH264_STREAMING_SUPPORT

endif

#-----------------------------------------------------------------------------------------------------
#--------------------------------------
ifeq ($(strip $(CHIP_VER)), 6530)
6530_H264 = TRUE
endif

ifeq ($(strip $(CHIP_VER)), 6531)
6531_H264 = TRUE
endif

ifeq ($(strip $(CHIP_VER)), 6531EFM)
6531EFM_H264 = TRUE
endif


#-----------------------------------------------------------------------------------------------------

######################################################################################################
#                                INCLUDE PATH and SOURCE FILE
######################################################################################################



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6800H)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6800h/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_sc6800h/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6800h/src					\
						MS_Ref/source/h264_codec/h264_codec_sc6800h/asm

SOURCES  	=   	h264dec_bitstrm.c											\
								h264dec_buffer.c                      \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         \
								                                      \
								arm_bilinear_filter.s                 \
								arm_bilinear_filter_4x4.s             \
								arm_ChromMC2x2.s                      \
								arm_ChromMC4x4.s                      \
								arm_edgeFilterUV_hor.s                \
								arm_edgeFilterUV_ver.s                \
								arm_edgeFilterY_hor.s                 \
								arm_edgeFilterY_ver.s                 \
								arm_edgeStrongFilterUV_hor.s          \
								arm_edgeStrongFilterUV_ver.s          \
								arm_edgeStrongFilterY_hor.s           \
								arm_edgeStrongFilterY_ver.s           \
								arm_itrans.s                          \
								arm_luma13x13_interpolation_hor.s	    \
								arm_luma13x13_interpolation_ver.s     \
								arm_luma13x8_interpolation_hor.s      \
								arm_luma4x4_interpolation_hor.s       \
								arm_luma4x4_interpolation_ver.s       \
								arm_luma4x9_interpolation_ver.s       \
								arm_luma8x13_interpolation_ver.s      \
								arm_luma8x8_interpolation_hor.s       \
								arm_luma8x8_interpolation_ver.s       \
								arm_luma9x4_interpolation_hor.s       \
								arm_luma9x9_interpolation_hor.s       \
								arm_luma9x9_interpolation_ver.s       \
								arm_MC_luma4x4_dx0dy0.s               \
								arm_MC_luma4x4_dx0dy1.s               \
								arm_MC_luma4x4_dx0dy3.s               \
								arm_MC_luma4x4_dx1dy0.s               \
								arm_MC_luma4x4_dx3dy0.s               \
								arm_MC_luma4x4_xyqpix.s               \
								arm_MC_luma8x8_dx0dy0.s               \
								arm_MC_luma8x8_dx0dy1.s               \
								arm_MC_luma8x8_dx0dy3.s               \
								arm_MC_luma8x8_dx1dy0.s               \
								arm_MC_luma8x8_dx3dy0.s               \
								arm_MC_luma8x8_xyqpix.s               \
								arm_put_mb2Frame.s                    \
								asm_horExtendYUV.s
endif
#-----------------------------------------------------------------------------------------------------




#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC8800G)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_sc8800g/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_sc8800g/inc/decoder	\
            MS_Ref/source/vsp/sc8800g/inc

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_sc8800g/src/decoder

SOURCES  	=   h264dec_bitstream.c		\
							h264dec_buffer.c			\
							h264dec_command.c			\
							h264dec_deblock.c	\
							h264dec_fmo.c					\
							h264dec_global.c			\
							h264dec_header.c			\
							h264dec_image.c				\
							h264dec_init.c				\
							h264dec_interface.c		\
							h264dec_malloc.c			\
							h264dec_mb.c					\
							h264dec_mv.c					\
							h264dec_parset.c			\
							h264dec_slice.c				\
							h264dec_table.c				
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(6530_H264)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6530/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_sc6530/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6530/src					\
						MS_Ref/source/h264_codec/h264_codec_sc6530/asm

SOURCES  	=   	h264dec_bitstrm.c											\
								h264dec_buffer.c                      \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         \
								                                      \
								arm_bilinear_filter.s                 \
								arm_bilinear_filter_4x4.s             \
								arm_ChromMC2x2.s                      \
								arm_ChromMC4x4.s                      \
								arm_edgeFilterUV_hor.s                \
								arm_edgeFilterUV_ver.s                \
								arm_edgeFilterY_hor.s                 \
								arm_edgeFilterY_ver.s                 \
								arm_MC_luma4x4_dx0dy0.s               \
								arm_MC_luma4x4_dx0dy1.s               \
								arm_MC_luma4x4_dx0dy3.s               \
								arm_MC_luma4x4_dx1dy0.s               \
								arm_MC_luma4x4_dx3dy0.s               \
								arm_MC_luma4x4_xyqpix.s               \
								arm_MC_luma8x8_dx0dy0.s               \
								arm_MC_luma8x8_dx0dy1.s               \
								arm_MC_luma8x8_dx0dy3.s               \
								arm_MC_luma8x8_dx1dy0.s               \
								arm_MC_luma8x8_dx3dy0.s               \
								arm_MC_luma8x8_xyqpix.s               \
								arm_put_mb2Frame.s
endif
#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(6531_H264)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6531/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_sc6531/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6531/src			\
						MS_Ref/source/h264_codec/h264_codec_sc6531/asm

SOURCES  	=   	h264dec_bitstream.c								  \
								h264dec_biaridecod.c				  \
								h264dec_buffer.c                      \
								h264dec_cabac.c						  \
								h264dec_context_init.c				  \
								h264dec_ctx_table.c					  \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         \
								h264dec_vld_table.c					  \
								h264_arm_stream_parse.s             \
								h264_Crom_code.c    

endif

ifeq ($(strip $(6531EFM_H264)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6531efm/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_sc6531efm/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_sc6531efm/src			\
						MS_Ref/source/h264_codec/h264_codec_sc6531efm/asm

SOURCES  	=   	h264dec_bitstream.c								  \
								h264dec_biaridecod.c				  \
								h264dec_buffer.c                      \
								h264dec_cabac.c						  \
								h264dec_context_init.c				  \
								h264dec_ctx_table.c					  \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         \
								h264dec_vld_table.c					  \
								h264_arm_stream_parse.s             \
								h264_Crom_code.c    

endif


ifeq ($(strip $(UIX8910_H264_for6531efm)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_uix8910/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910/src			\
						MS_Ref/source/h264_codec/h264_codec_uix8910/asm

SOURCES  	=   	h264dec_bitstream.c								  \
								h264dec_biaridecod.c				  \
								h264dec_buffer.c                      \
								h264dec_cabac.c						  \
								h264dec_context_init.c				  \
								h264dec_ctx_table.c					  \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         \
								h264dec_vld_table.c					  \
								h264_arm_stream_parse.s             \
								h264_Crom_code.c    

endif


ifeq ($(strip $(UIX8910_H264_V3)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910v3/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_uix8910v3/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910v3/src					\
						MS_Ref/source/h264_codec/h264_codec_uix8910v3/asm

SOURCES  	=   	h264dec_bitstrm.c											\
								h264dec_buffer.c                      \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         

ifeq ($(strip $(USE_ITRANS_ASSEMBLY)),	TRUE) 
SOURCES		+= h264dec_isqt_neon.s	
endif

endif


ifeq ($(strip $(UWS6121E_H264_V3)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_uws6121ev3/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_uws6121ev3/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_uws6121ev3/src					\
						MS_Ref/source/h264_codec/h264_codec_uws6121ev3/asm

SOURCES  	=   	h264dec_bitstrm.c											\
								h264dec_buffer.c                      \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         

ifeq ($(strip $(USE_ITRANS_ASSEMBLY)),	TRUE) 
SOURCES		+= h264dec_isqt_neon.s	
endif

endif



ifeq ($(strip $(UIX8910_H264_V2)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910v2/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_uix8910v2/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910v2/src					\
						MS_Ref/source/h264_codec/h264_codec_uix8910v2/asm

SOURCES  	=   	h264dec_bitstrm.c											\
								h264dec_buffer.c                      \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         \
								                                      \
								arm_bilinear_filter.s                 \
								arm_bilinear_filter_4x4.s             \
								arm_ChromMC2x2.s                      \
								arm_ChromMC4x4.s                      \
								arm_edgeFilterUV_hor.s                \
								arm_edgeFilterUV_ver.s                \
								arm_edgeFilterY_hor.s                 \
								arm_edgeFilterY_ver.s                 \
								arm_edgeStrongFilterUV_hor.s          \
								arm_edgeStrongFilterUV_ver.s          \
								arm_edgeStrongFilterY_hor.s           \
								arm_edgeStrongFilterY_ver.s           \
								arm_itrans.s                          \
								arm_luma13x13_interpolation_hor.s	    \
								arm_luma13x13_interpolation_ver.s     \
								arm_luma13x8_interpolation_hor.s      \
								arm_luma4x4_interpolation_hor.s       \
								arm_luma4x4_interpolation_ver.s       \
								arm_luma4x9_interpolation_ver.s       \
								arm_luma8x13_interpolation_ver.s      \
								arm_luma8x8_interpolation_hor.s       \
								arm_luma8x8_interpolation_ver.s       \
								arm_luma9x4_interpolation_hor.s       \
								arm_luma9x9_interpolation_hor.s       \
								arm_luma9x9_interpolation_ver.s       \
								arm_MC_luma4x4_dx0dy0.s               \
								arm_MC_luma4x4_dx0dy1.s               \
								arm_MC_luma4x4_dx0dy3.s               \
								arm_MC_luma4x4_dx1dy0.s               \
								arm_MC_luma4x4_dx3dy0.s               \
								arm_MC_luma4x4_xyqpix.s               \
								arm_MC_luma8x8_dx0dy0.s               \
								arm_MC_luma8x8_dx0dy1.s               \
								arm_MC_luma8x8_dx0dy3.s               \
								arm_MC_luma8x8_dx1dy0.s               \
								arm_MC_luma8x8_dx3dy0.s               \
								arm_MC_luma8x8_xyqpix.s               \
								arm_put_mb2Frame.s                    \
								asm_horExtendYUV.s
endif

ifeq ($(strip $(UIX8910_H264)),	TRUE)
MINCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910/inc/common	\
            MS_Ref/source/h264_codec/h264_codec_uix8910/inc/decoder

MSRCPATH =  MS_Ref/source/h264_codec/h264_codec_uix8910/src					\
						MS_Ref/source/h264_codec/h264_codec_uix8910/asm

SOURCES  	=   	h264dec_bitstrm.c											\
								h264dec_buffer.c                      \
								h264dec_deblock.c                     \
								h264dec_fmo.c                         \
								h264dec_global.c                      \
								h264dec_header.c                      \
								h264dec_image.c                       \
								h264dec_init.c                        \
								h264dec_interface.c                   \
								h264dec_ipred.c                       \
								h264dec_isqt.c                        \
								h264dec_malloc.c                      \
								h264dec_mb.c                          \
								h264dec_mc4x4.c                       \
								h264dec_mc8x8.c                       \
								h264dec_mv.c                          \
								h264dec_parset.c                      \
								h264dec_slice.c                       \
								h264dec_table.c                       \
								h264dec_vld.c                         \
								                                      \
								arm_bilinear_filter.s                 \
								arm_bilinear_filter_4x4.s             \
								arm_ChromMC2x2.s                      \
								arm_ChromMC4x4.s                      \
								arm_edgeFilterUV_hor.s                \
								arm_edgeFilterUV_ver.s                \
								arm_edgeFilterY_hor.s                 \
								arm_edgeFilterY_ver.s                 \
								arm_MC_luma4x4_dx0dy0.s               \
								arm_MC_luma4x4_dx0dy1.s               \
								arm_MC_luma4x4_dx0dy3.s               \
								arm_MC_luma4x4_dx1dy0.s               \
								arm_MC_luma4x4_dx3dy0.s               \
								arm_MC_luma4x4_xyqpix.s               \
								arm_MC_luma8x8_dx0dy0.s               \
								arm_MC_luma8x8_dx0dy1.s               \
								arm_MC_luma8x8_dx0dy3.s               \
								arm_MC_luma8x8_dx1dy0.s               \
								arm_MC_luma8x8_dx3dy0.s               \
								arm_MC_luma8x8_xyqpix.s               \
								arm_put_mb2Frame.s
endif

#-----------------------------------------------------------------------------------------------------




MINCPATH += chip_drv/export/inc/outdated

######## if VIDEO_SECURITY_SUPPORT
MINCPATH += MS_Ref/source/video_security/export
######## endif
