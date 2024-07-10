

######################################################################################################
#                                FEATURE OPTIONS
######################################################################################################
ifeq ($(strip $(TMVB_DEC_SUPPORT)), TRUE)
    VCODEC_ADP_RMVB_DEC_SUPPORT		= TRUE
endif


######################################################################################################
#                                MACRO DEFINITION
######################################################################################################
MCFLAG_OPT   	= -D_ARM_ -zc -fy 

ifeq ($(strip $(CHIP_ENDIAN)),	BIG)
    MCFLAG_OPT += -DRV_BIG_ENDIAN
    MCFLAG_OPT += -D_VCODEC_ADP_BIG_ENDIAN_
endif

ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
#{

    MCFLAG_OPT  +=  -DPUT_MB_ASSEMLY \
                    -DITRANS_ASSEMBLY \
                    -D_ARM_MC_ASSEMBLY_ \
                    -D_ARM_DOWNSIZE_

ifeq ($(strip $(PLATFORM)),	SC6800H)
#{
    MCFLAG_OPT  +=  -DSC6800H_VIDEO_ROMCODE
#}
endif

#}
endif



######################################################################################################
#                                INCLUDE PATH and SOURCE FILE
######################################################################################################

MINCPATH    =   MS_Ref/export/inc

ifeq ($(strip $(PLATFORM)),	SC6800H)
ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    MINCPATH += MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6800h/inc/common	\
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6800h/inc/decoder
endif


ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    MSRCPATH =  MS_Ref/source/video_codec_adptor/src/rmvb                   \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6800h/src/decoder   \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6800h/asm           \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6800h/asm/mc_downsize
endif


ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    SOURCES =   rmvb_dec_adp.c					\
                \
                rvdec_bfrctrl.c      		\
                rvdec_bitstrm.c  				\
                rvdec_deblock_rv8.c 		\
                rvdec_deblock_rv9.c 		\
                rvdec_frame.c						\
                rvdec_global.c 					\
                rvdec_header.c					\
                rvdec_init.c            \
                rvdec_interface.c       \
                rvdec_intp16x16_rv8.c   \
                rvdec_intp16x16_rv9.c   \
                rvdec_intp_rv8.c        \
                rvdec_intp_rv9.c        \
                rvdec_ipred.c           \
                rvdec_isqt.c						\
                rvdec_malloc.c					\
                rvdec_mb.c							\
                rvdec_mc.c							\
                rvdec_mv.c							\
                rvdec_table.c						\
                rvdec_vld.c							\
                rvdec_vld_table.c				\
                rvdec_intp_r9_d.c				\
                \
                arm_copyRefToRecFrm.s           \
                arm_copy_ref_MB.s               \
                arm_ipred_4x4_DC_PRED.s         \
                arm_ipred_4x4_HOR_PRED.s                    \
                arm_ipred_4x4_VERT_PRED.s                   \
                arm_ipred_CHROMA8x8_HORZ_PRED.s               \
                arm_ipred_CHROMA8x8_VERT_PRED.s               \
                arm_ipred_Luma16x16_DC_PRED.s                 \
                arm_ipred_Luma16x16_HORZ_PRED.s               \
                arm_ipred_Luma16x16_PLANAR_PRED.s             \
                arm_ipred_Luma16x16_VERT_PRED.s               \
                arm_mc_chroma4x4_dx0dy0_rv9.s                  \
                arm_mc_chroma4x4_dx0dy1_rv8.s                  \
                arm_mc_chroma4x4_dx0dy1_rv9.s                  \
                arm_mc_chroma4x4_dx0dy2_rv8.s                  \
                arm_mc_chroma4x4_dx0dy2_rv9.s                  \
                arm_mc_chroma4x4_dx0dy3_rv9.s                  \
                arm_mc_chroma4x4_dx1dy0_rv8.s                  \
                arm_mc_chroma4x4_dx1dy0_rv9.s                  \
                arm_mc_chroma4x4_dx1dy1_rv8.s                  \
                arm_mc_chroma4x4_dx1dy1_rv9.s                  \
                arm_mc_chroma4x4_dx1dy2_rv8.s                  \
                arm_mc_chroma4x4_dx1dy2_rv9.s                  \
                arm_mc_chroma4x4_dx1dy3_rv9.s                  \
                arm_mc_chroma4x4_dx2dy0_rv8.s                  \
                arm_mc_chroma4x4_dx2dy0_rv9.s                  \
                arm_mc_chroma4x4_dx2dy1_rv8.s                  \
                arm_mc_chroma4x4_dx2dy1_rv9.s                  \
                arm_mc_chroma4x4_dx2dy2_rv8.s                  \
                arm_mc_chroma4x4_dx2dy2_rv9.s                  \
                arm_mc_chroma4x4_dx2dy3_rv9.s                  \
                arm_mc_chroma4x4_dx3dy0_rv9.s                  \
                arm_mc_chroma4x4_dx3dy1_rv9.s                  \
                arm_mc_chroma4x4_dx3dy2_rv9.s                  \
                arm_mc_chroma4x4_dx3dy3_rv9.s                  \
                arm_mc_chroma8x8_dx0dy0_rv9.s                  \
                arm_mc_chroma8x8_dx0dy1_rv8.s                  \
                arm_mc_chroma8x8_dx0dy1_rv9.s                  \
                arm_mc_chroma8x8_dx0dy2_rv8.s                  \
                arm_mc_chroma8x8_dx0dy2_rv9.s                  \
                arm_mc_chroma8x8_dx0dy3_rv9.s                  \
                arm_mc_chroma8x8_dx1dy0_rv8.s                  \
                arm_mc_chroma8x8_dx1dy0_rv9.s                  \
                arm_mc_chroma8x8_dx1dy1_rv8.s                  \
                arm_mc_chroma8x8_dx1dy1_rv9.s                  \
                arm_mc_chroma8x8_dx1dy2_rv8.s                  \
                arm_mc_chroma8x8_dx1dy2_rv9.s                  \
                arm_mc_chroma8x8_dx1dy3_rv9.s                  \
                arm_mc_chroma8x8_dx2dy0_rv8.s                  \
                arm_mc_chroma8x8_dx2dy0_rv9.s                  \
                arm_mc_chroma8x8_dx2dy1_rv8.s                  \
                arm_mc_chroma8x8_dx2dy1_rv9.s                  \
                arm_mc_chroma8x8_dx2dy2_rv8.s                  \
                arm_mc_chroma8x8_dx2dy2_rv9.s                  \
                arm_mc_chroma8x8_dx2dy3_rv9.s                  \
                arm_mc_chroma8x8_dx3dy0_rv9.s                  \
                arm_mc_chroma8x8_dx3dy1_rv9.s                  \
                arm_mc_chroma8x8_dx3dy2_rv9.s                  \
                arm_mc_chroma8x8_dx3dy3_rv9.s                  \
                arm_mc_itans4x4.s                              \
                arm_mc_luma16x16_dx0dy0_rv9.s                  \
                arm_mc_luma16x16_dx0dy1_rv8.s                  \
                arm_mc_luma16x16_dx0dy1_rv9.s                  \
                arm_mc_luma16x16_dx0dy2_rv8.s                  \
                arm_mc_luma16x16_dx0dy2_rv9.s                  \
                arm_mc_luma16x16_dx0dy3_rv9.s                  \
                arm_mc_luma16x16_dx1dy0_rv8.s                  \
                arm_mc_luma16x16_dx1dy0_rv9.s                  \
                arm_mc_luma16x16_dx1dy1_rv8.s                  \
                arm_mc_luma16x16_dx1dy1_rv9.s                  \
                arm_mc_luma16x16_dx1dy2_rv8.s                  \
                arm_mc_luma16x16_dx1dy2_rv9.s                  \
                arm_mc_luma16x16_dx1dy3_rv9.s                  \
                arm_mc_luma16x16_dx2dy0_rv8.s                  \
                arm_mc_luma16x16_dx2dy0_rv9.s                  \
                arm_mc_luma16x16_dx2dy1_rv8.s                  \
                arm_mc_luma16x16_dx2dy1_rv9.s                  \
                arm_mc_luma16x16_dx2dy2_rv8.s                  \
                arm_mc_luma16x16_dx2dy2_rv9.s                  \
                arm_mc_luma16x16_dx2dy3_rv9.s                  \
                arm_mc_luma16x16_dx3dy0_rv9.s                  \
                arm_mc_luma16x16_dx3dy1_rv9.s                  \
                arm_mc_luma16x16_dx3dy2_rv9.s                  \
                arm_mc_luma16x16_dx3dy3_rv9.s                  \
                arm_mc_luma8x8_dx0dy0_rv9.s                  \
                arm_mc_luma8x8_dx0dy1_rv8.s                  \
                arm_mc_luma8x8_dx0dy1_rv9.s                  \
                arm_mc_luma8x8_dx0dy2_rv8.s                  \
                arm_mc_luma8x8_dx0dy2_rv9.s                  \
                arm_mc_luma8x8_dx0dy3_rv9.s                  \
                arm_mc_luma8x8_dx1dy0_rv8.s                  \
                arm_mc_luma8x8_dx1dy0_rv9.s                  \
                arm_mc_luma8x8_dx1dy1_rv8.s                  \
                arm_mc_luma8x8_dx1dy1_rv9.s                  \
                arm_mc_luma8x8_dx1dy2_rv8.s                  \
                arm_mc_luma8x8_dx1dy2_rv9.s                  \
                arm_mc_luma8x8_dx1dy3_rv9.s                  \
                arm_mc_luma8x8_dx2dy0_rv8.s                  \
                arm_mc_luma8x8_dx2dy0_rv9.s                  \
                arm_mc_luma8x8_dx2dy1_rv8.s                  \
                arm_mc_luma8x8_dx2dy1_rv9.s                  \
                arm_mc_luma8x8_dx2dy2_rv8.s                  \
                arm_mc_luma8x8_dx2dy2_rv9.s                  \
                arm_mc_luma8x8_dx2dy3_rv9.s                  \
                arm_mc_luma8x8_dx3dy0_rv9.s                  \
                arm_mc_luma8x8_dx3dy1_rv9.s                  \
                arm_mc_luma8x8_dx3dy2_rv9.s                  \
                arm_mc_luma8x8_dx3dy3_rv9.s                  \
                \
                arm_put_mb2Frame.s                           \
                arm_weighted_average_r0eqr1.s                \
                asm_horExtendYUV.s                           \
                rv89_dec_asm_config.s                        \
                arm_mc_chroma2x2_d.s                         \
                arm_mc_chroma2x2_xfull_d.s                  \
                arm_mc_chroma2x2_yfull_d.s                  \
                arm_mc_chroma4x4_d.s                        \
                arm_mc_chroma4x4_dx0dy0_d.s                 \
                arm_mc_chroma4x4_xfull_d.s                  \
                arm_mc_chroma4x4_yfull_d.s                  \
                arm_mc_luma4x4_xfull_d.s                    \
                arm_mc_luma4x4_yfull_d.s                    \
                arm_mc_luma8x8_xfull_d.s                    \
                arm_mc_luma8x8_yfull_d.s                    \
                arm_copyLeftPredPixel.s                     \
                arm_copyRefToRecFrm_d.s                     \
                arm_down_sample_mb.s                        \
                arm_mc_itans4x4_d.s                         \
                arm_put_mb2Frame_d.s                        \
                arm_upscaler_ppred.s                        \
                asm_horExtendYUV_d.s                        \
                RvDec_downsize_frame_i.s                    
endif

endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6530)
ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    MINCPATH += MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/inc/common	\
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/inc/decoder
endif


ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    MSRCPATH =  MS_Ref/source/video_codec_adptor/src/rmvb                   \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/src/decoder   \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/asm           \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/asm/mc_downsize
endif


ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    SOURCES =   rmvb_dec_adp.c					\
                \
                rvdec_bfrctrl.c      		\
                rvdec_bitstrm.c  				\
                rvdec_deblock_rv8.c 		\
                rvdec_deblock_rv9.c 		\
                rvdec_frame.c						\
                rvdec_global.c 					\
                rvdec_header.c					\
                rvdec_init.c            \
                rvdec_interface.c       \
                rvdec_intp16x16_rv8.c   \
                rvdec_intp16x16_rv9.c   \
                rvdec_intp_rv8.c        \
                rvdec_intp_rv9.c        \
                rvdec_ipred.c           \
                rvdec_isqt.c						\
                rvdec_malloc.c					\
                rvdec_mb.c							\
                rvdec_mc.c							\
                rvdec_mv.c							\
                rvdec_table.c						\
                rvdec_vld.c							\
                rvdec_vld_table.c				\
                rvdec_intp_r9_d.c				\
                \
                arm_copyRefToRecFrm.s           \
                arm_copy_ref_MB.s               \
                arm_ipred_4x4_DC_PRED.s         \
                arm_ipred_4x4_HOR_PRED.s                    \
                arm_ipred_4x4_VERT_PRED.s                   \
                arm_ipred_CHROMA8x8_HORZ_PRED.s               \
                arm_ipred_CHROMA8x8_VERT_PRED.s               \
                arm_ipred_Luma16x16_DC_PRED.s                 \
                arm_ipred_Luma16x16_HORZ_PRED.s               \
                arm_ipred_Luma16x16_PLANAR_PRED.s             \
                arm_ipred_Luma16x16_VERT_PRED.s               \
                arm_mc_chroma4x4_dx0dy0_rv9.s                  \
                arm_mc_chroma4x4_dx0dy1_rv8.s                  \
                arm_mc_chroma4x4_dx0dy1_rv9.s                  \
                arm_mc_chroma4x4_dx0dy2_rv8.s                  \
                arm_mc_chroma4x4_dx0dy2_rv9.s                  \
                arm_mc_chroma4x4_dx0dy3_rv9.s                  \
                arm_mc_chroma4x4_dx1dy0_rv8.s                  \
                arm_mc_chroma4x4_dx1dy0_rv9.s                  \
                arm_mc_chroma4x4_dx1dy1_rv8.s                  \
                arm_mc_chroma4x4_dx1dy1_rv9.s                  \
                arm_mc_chroma4x4_dx1dy2_rv8.s                  \
                arm_mc_chroma4x4_dx1dy2_rv9.s                  \
                arm_mc_chroma4x4_dx1dy3_rv9.s                  \
                arm_mc_chroma4x4_dx2dy0_rv8.s                  \
                arm_mc_chroma4x4_dx2dy0_rv9.s                  \
                arm_mc_chroma4x4_dx2dy1_rv8.s                  \
                arm_mc_chroma4x4_dx2dy1_rv9.s                  \
                arm_mc_chroma4x4_dx2dy2_rv8.s                  \
                arm_mc_chroma4x4_dx2dy2_rv9.s                  \
                arm_mc_chroma4x4_dx2dy3_rv9.s                  \
                arm_mc_chroma4x4_dx3dy0_rv9.s                  \
                arm_mc_chroma4x4_dx3dy1_rv9.s                  \
                arm_mc_chroma4x4_dx3dy2_rv9.s                  \
                arm_mc_chroma4x4_dx3dy3_rv9.s                  \
                arm_mc_chroma8x8_dx0dy0_rv9.s                  \
                arm_mc_chroma8x8_dx0dy1_rv8.s                  \
                arm_mc_chroma8x8_dx0dy1_rv9.s                  \
                arm_mc_chroma8x8_dx0dy2_rv8.s                  \
                arm_mc_chroma8x8_dx0dy2_rv9.s                  \
                arm_mc_chroma8x8_dx0dy3_rv9.s                  \
                arm_mc_chroma8x8_dx1dy0_rv8.s                  \
                arm_mc_chroma8x8_dx1dy0_rv9.s                  \
                arm_mc_chroma8x8_dx1dy1_rv8.s                  \
                arm_mc_chroma8x8_dx1dy1_rv9.s                  \
                arm_mc_chroma8x8_dx1dy2_rv8.s                  \
                arm_mc_chroma8x8_dx1dy2_rv9.s                  \
                arm_mc_chroma8x8_dx1dy3_rv9.s                  \
                arm_mc_chroma8x8_dx2dy0_rv8.s                  \
                arm_mc_chroma8x8_dx2dy0_rv9.s                  \
                arm_mc_chroma8x8_dx2dy1_rv8.s                  \
                arm_mc_chroma8x8_dx2dy1_rv9.s                  \
                arm_mc_chroma8x8_dx2dy2_rv8.s                  \
                arm_mc_chroma8x8_dx2dy2_rv9.s                  \
                arm_mc_chroma8x8_dx2dy3_rv9.s                  \
                arm_mc_chroma8x8_dx3dy0_rv9.s                  \
                arm_mc_chroma8x8_dx3dy1_rv9.s                  \
                arm_mc_chroma8x8_dx3dy2_rv9.s                  \
                arm_mc_chroma8x8_dx3dy3_rv9.s                  \
                arm_mc_itans4x4.s                              \
                arm_mc_luma16x16_dx0dy0_rv9.s                  \
                arm_mc_luma16x16_dx0dy1_rv8.s                  \
                arm_mc_luma16x16_dx0dy1_rv9.s                  \
                arm_mc_luma16x16_dx0dy2_rv8.s                  \
                arm_mc_luma16x16_dx0dy2_rv9.s                  \
                arm_mc_luma16x16_dx0dy3_rv9.s                  \
                arm_mc_luma16x16_dx1dy0_rv8.s                  \
                arm_mc_luma16x16_dx1dy0_rv9.s                  \
                arm_mc_luma16x16_dx1dy1_rv8.s                  \
                arm_mc_luma16x16_dx1dy1_rv9.s                  \
                arm_mc_luma16x16_dx1dy2_rv8.s                  \
                arm_mc_luma16x16_dx1dy2_rv9.s                  \
                arm_mc_luma16x16_dx1dy3_rv9.s                  \
                arm_mc_luma16x16_dx2dy0_rv8.s                  \
                arm_mc_luma16x16_dx2dy0_rv9.s                  \
                arm_mc_luma16x16_dx2dy1_rv8.s                  \
                arm_mc_luma16x16_dx2dy1_rv9.s                  \
                arm_mc_luma16x16_dx2dy2_rv8.s                  \
                arm_mc_luma16x16_dx2dy2_rv9.s                  \
                arm_mc_luma16x16_dx2dy3_rv9.s                  \
                arm_mc_luma16x16_dx3dy0_rv9.s                  \
                arm_mc_luma16x16_dx3dy1_rv9.s                  \
                arm_mc_luma16x16_dx3dy2_rv9.s                  \
                arm_mc_luma16x16_dx3dy3_rv9.s                  \
                arm_mc_luma8x8_dx0dy0_rv9.s                  \
                arm_mc_luma8x8_dx0dy1_rv8.s                  \
                arm_mc_luma8x8_dx0dy1_rv9.s                  \
                arm_mc_luma8x8_dx0dy2_rv8.s                  \
                arm_mc_luma8x8_dx0dy2_rv9.s                  \
                arm_mc_luma8x8_dx0dy3_rv9.s                  \
                arm_mc_luma8x8_dx1dy0_rv8.s                  \
                arm_mc_luma8x8_dx1dy0_rv9.s                  \
                arm_mc_luma8x8_dx1dy1_rv8.s                  \
                arm_mc_luma8x8_dx1dy1_rv9.s                  \
                arm_mc_luma8x8_dx1dy2_rv8.s                  \
                arm_mc_luma8x8_dx1dy2_rv9.s                  \
                arm_mc_luma8x8_dx1dy3_rv9.s                  \
                arm_mc_luma8x8_dx2dy0_rv8.s                  \
                arm_mc_luma8x8_dx2dy0_rv9.s                  \
                arm_mc_luma8x8_dx2dy1_rv8.s                  \
                arm_mc_luma8x8_dx2dy1_rv9.s                  \
                arm_mc_luma8x8_dx2dy2_rv8.s                  \
                arm_mc_luma8x8_dx2dy2_rv9.s                  \
                arm_mc_luma8x8_dx2dy3_rv9.s                  \
                arm_mc_luma8x8_dx3dy0_rv9.s                  \
                arm_mc_luma8x8_dx3dy1_rv9.s                  \
                arm_mc_luma8x8_dx3dy2_rv9.s                  \
                arm_mc_luma8x8_dx3dy3_rv9.s                  \
                \
                arm_put_mb2Frame.s                           \
                arm_weighted_average_r0eqr1.s                \
                asm_horExtendYUV.s                           \
                rv89_dec_asm_config.s                        \
                arm_mc_chroma2x2_d.s                         \
                arm_mc_chroma2x2_xfull_d.s                  \
                arm_mc_chroma2x2_yfull_d.s                  \
                arm_mc_chroma4x4_d.s                        \
                arm_mc_chroma4x4_dx0dy0_d.s                 \
                arm_mc_chroma4x4_xfull_d.s                  \
                arm_mc_chroma4x4_yfull_d.s                  \
                arm_mc_luma4x4_xfull_d.s                    \
                arm_mc_luma4x4_yfull_d.s                    \
                arm_mc_luma8x8_xfull_d.s                    \
                arm_mc_luma8x8_yfull_d.s                    \
                arm_copyLeftPredPixel.s                     \
                arm_copyRefToRecFrm_d.s                     \
                arm_down_sample_mb.s                        \
                arm_mc_itans4x4_d.s                         \
                arm_put_mb2Frame_d.s                        \
                arm_upscaler_ppred.s                        \
                asm_horExtendYUV_d.s                        \
                RvDec_downsize_frame_i.s                    
endif

endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    MINCPATH += MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/inc/common	\
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/inc/decoder
endif


ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    MSRCPATH =  MS_Ref/source/video_codec_adptor/src/rmvb                   \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/src/decoder   \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/asm           \
                MS_Ref/source/video_codec_adptor/src/rmvb/rv_codec_sc6530/asm/mc_downsize
endif


ifeq ($(strip $(VCODEC_ADP_RMVB_DEC_SUPPORT)), TRUE)
    SOURCES =   rmvb_dec_adp.c					\
                \
                rvdec_bfrctrl.c      		\
                rvdec_bitstrm.c  				\
                rvdec_deblock_rv8.c 		\
                rvdec_deblock_rv9.c 		\
                rvdec_frame.c						\
                rvdec_global.c 					\
                rvdec_header.c					\
                rvdec_init.c            \
                rvdec_interface.c       \
                rvdec_intp16x16_rv8.c   \
                rvdec_intp16x16_rv9.c   \
                rvdec_intp_rv8.c        \
                rvdec_intp_rv9.c        \
                rvdec_ipred.c           \
                rvdec_isqt.c						\
                rvdec_malloc.c					\
                rvdec_mb.c							\
                rvdec_mc.c							\
                rvdec_mv.c							\
                rvdec_table.c						\
                rvdec_vld.c							\
                rvdec_vld_table.c				\
                rvdec_intp_r9_d.c				\
                \
                arm_copyRefToRecFrm.s           \
                arm_copy_ref_MB.s               \
                arm_ipred_4x4_DC_PRED.s         \
                arm_ipred_4x4_HOR_PRED.s                    \
                arm_ipred_4x4_VERT_PRED.s                   \
                arm_ipred_CHROMA8x8_HORZ_PRED.s               \
                arm_ipred_CHROMA8x8_VERT_PRED.s               \
                arm_ipred_Luma16x16_DC_PRED.s                 \
                arm_ipred_Luma16x16_HORZ_PRED.s               \
                arm_ipred_Luma16x16_PLANAR_PRED.s             \
                arm_ipred_Luma16x16_VERT_PRED.s               \
                arm_mc_chroma4x4_dx0dy0_rv9.s                  \
                arm_mc_chroma4x4_dx0dy1_rv8.s                  \
                arm_mc_chroma4x4_dx0dy1_rv9.s                  \
                arm_mc_chroma4x4_dx0dy2_rv8.s                  \
                arm_mc_chroma4x4_dx0dy2_rv9.s                  \
                arm_mc_chroma4x4_dx0dy3_rv9.s                  \
                arm_mc_chroma4x4_dx1dy0_rv8.s                  \
                arm_mc_chroma4x4_dx1dy0_rv9.s                  \
                arm_mc_chroma4x4_dx1dy1_rv8.s                  \
                arm_mc_chroma4x4_dx1dy1_rv9.s                  \
                arm_mc_chroma4x4_dx1dy2_rv8.s                  \
                arm_mc_chroma4x4_dx1dy2_rv9.s                  \
                arm_mc_chroma4x4_dx1dy3_rv9.s                  \
                arm_mc_chroma4x4_dx2dy0_rv8.s                  \
                arm_mc_chroma4x4_dx2dy0_rv9.s                  \
                arm_mc_chroma4x4_dx2dy1_rv8.s                  \
                arm_mc_chroma4x4_dx2dy1_rv9.s                  \
                arm_mc_chroma4x4_dx2dy2_rv8.s                  \
                arm_mc_chroma4x4_dx2dy2_rv9.s                  \
                arm_mc_chroma4x4_dx2dy3_rv9.s                  \
                arm_mc_chroma4x4_dx3dy0_rv9.s                  \
                arm_mc_chroma4x4_dx3dy1_rv9.s                  \
                arm_mc_chroma4x4_dx3dy2_rv9.s                  \
                arm_mc_chroma4x4_dx3dy3_rv9.s                  \
                arm_mc_chroma8x8_dx0dy0_rv9.s                  \
                arm_mc_chroma8x8_dx0dy1_rv8.s                  \
                arm_mc_chroma8x8_dx0dy1_rv9.s                  \
                arm_mc_chroma8x8_dx0dy2_rv8.s                  \
                arm_mc_chroma8x8_dx0dy2_rv9.s                  \
                arm_mc_chroma8x8_dx0dy3_rv9.s                  \
                arm_mc_chroma8x8_dx1dy0_rv8.s                  \
                arm_mc_chroma8x8_dx1dy0_rv9.s                  \
                arm_mc_chroma8x8_dx1dy1_rv8.s                  \
                arm_mc_chroma8x8_dx1dy1_rv9.s                  \
                arm_mc_chroma8x8_dx1dy2_rv8.s                  \
                arm_mc_chroma8x8_dx1dy2_rv9.s                  \
                arm_mc_chroma8x8_dx1dy3_rv9.s                  \
                arm_mc_chroma8x8_dx2dy0_rv8.s                  \
                arm_mc_chroma8x8_dx2dy0_rv9.s                  \
                arm_mc_chroma8x8_dx2dy1_rv8.s                  \
                arm_mc_chroma8x8_dx2dy1_rv9.s                  \
                arm_mc_chroma8x8_dx2dy2_rv8.s                  \
                arm_mc_chroma8x8_dx2dy2_rv9.s                  \
                arm_mc_chroma8x8_dx2dy3_rv9.s                  \
                arm_mc_chroma8x8_dx3dy0_rv9.s                  \
                arm_mc_chroma8x8_dx3dy1_rv9.s                  \
                arm_mc_chroma8x8_dx3dy2_rv9.s                  \
                arm_mc_chroma8x8_dx3dy3_rv9.s                  \
                arm_mc_itans4x4.s                              \
                arm_mc_luma16x16_dx0dy0_rv9.s                  \
                arm_mc_luma16x16_dx0dy1_rv8.s                  \
                arm_mc_luma16x16_dx0dy1_rv9.s                  \
                arm_mc_luma16x16_dx0dy2_rv8.s                  \
                arm_mc_luma16x16_dx0dy2_rv9.s                  \
                arm_mc_luma16x16_dx0dy3_rv9.s                  \
                arm_mc_luma16x16_dx1dy0_rv8.s                  \
                arm_mc_luma16x16_dx1dy0_rv9.s                  \
                arm_mc_luma16x16_dx1dy1_rv8.s                  \
                arm_mc_luma16x16_dx1dy1_rv9.s                  \
                arm_mc_luma16x16_dx1dy2_rv8.s                  \
                arm_mc_luma16x16_dx1dy2_rv9.s                  \
                arm_mc_luma16x16_dx1dy3_rv9.s                  \
                arm_mc_luma16x16_dx2dy0_rv8.s                  \
                arm_mc_luma16x16_dx2dy0_rv9.s                  \
                arm_mc_luma16x16_dx2dy1_rv8.s                  \
                arm_mc_luma16x16_dx2dy1_rv9.s                  \
                arm_mc_luma16x16_dx2dy2_rv8.s                  \
                arm_mc_luma16x16_dx2dy2_rv9.s                  \
                arm_mc_luma16x16_dx2dy3_rv9.s                  \
                arm_mc_luma16x16_dx3dy0_rv9.s                  \
                arm_mc_luma16x16_dx3dy1_rv9.s                  \
                arm_mc_luma16x16_dx3dy2_rv9.s                  \
                arm_mc_luma16x16_dx3dy3_rv9.s                  \
                arm_mc_luma8x8_dx0dy0_rv9.s                  \
                arm_mc_luma8x8_dx0dy1_rv8.s                  \
                arm_mc_luma8x8_dx0dy1_rv9.s                  \
                arm_mc_luma8x8_dx0dy2_rv8.s                  \
                arm_mc_luma8x8_dx0dy2_rv9.s                  \
                arm_mc_luma8x8_dx0dy3_rv9.s                  \
                arm_mc_luma8x8_dx1dy0_rv8.s                  \
                arm_mc_luma8x8_dx1dy0_rv9.s                  \
                arm_mc_luma8x8_dx1dy1_rv8.s                  \
                arm_mc_luma8x8_dx1dy1_rv9.s                  \
                arm_mc_luma8x8_dx1dy2_rv8.s                  \
                arm_mc_luma8x8_dx1dy2_rv9.s                  \
                arm_mc_luma8x8_dx1dy3_rv9.s                  \
                arm_mc_luma8x8_dx2dy0_rv8.s                  \
                arm_mc_luma8x8_dx2dy0_rv9.s                  \
                arm_mc_luma8x8_dx2dy1_rv8.s                  \
                arm_mc_luma8x8_dx2dy1_rv9.s                  \
                arm_mc_luma8x8_dx2dy2_rv8.s                  \
                arm_mc_luma8x8_dx2dy2_rv9.s                  \
                arm_mc_luma8x8_dx2dy3_rv9.s                  \
                arm_mc_luma8x8_dx3dy0_rv9.s                  \
                arm_mc_luma8x8_dx3dy1_rv9.s                  \
                arm_mc_luma8x8_dx3dy2_rv9.s                  \
                arm_mc_luma8x8_dx3dy3_rv9.s                  \
                \
                arm_put_mb2Frame.s                           \
                arm_weighted_average_r0eqr1.s                \
                asm_horExtendYUV.s                           \
                rv89_dec_asm_config.s                        \
                arm_mc_chroma2x2_d.s                         \
                arm_mc_chroma2x2_xfull_d.s                  \
                arm_mc_chroma2x2_yfull_d.s                  \
                arm_mc_chroma4x4_d.s                        \
                arm_mc_chroma4x4_dx0dy0_d.s                 \
                arm_mc_chroma4x4_xfull_d.s                  \
                arm_mc_chroma4x4_yfull_d.s                  \
                arm_mc_luma4x4_xfull_d.s                    \
                arm_mc_luma4x4_yfull_d.s                    \
                arm_mc_luma8x8_xfull_d.s                    \
                arm_mc_luma8x8_yfull_d.s                    \
                arm_copyLeftPredPixel.s                     \
                arm_copyRefToRecFrm_d.s                     \
                arm_down_sample_mb.s                        \
                arm_mc_itans4x4_d.s                         \
                arm_put_mb2Frame_d.s                        \
                arm_upscaler_ppred.s                        \
                asm_horExtendYUV_d.s                        \
                RvDec_downsize_frame_i.s                    
endif

endif
#-----------------------------------------------------------------------------------------------------
