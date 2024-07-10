
MCFLAG_OPT   	= -Otime

ifeq ($(strip $(CHIP_ENDIAN)),	BIG)
MCFLAG_OPT	+= -D_DV_BIG_ENDIAN
endif

MCFLAG_OPT += --cpu cortex-a5.neon
MCFLAG_OPT +=--fpu VFPv4
#MCFLAG_OPT += -D_NEON_OPT2_

ifeq ($(strip $(PLATFORM)),	SC6800H)
	DV_ISP_VER = v0
endif
ifeq ($(strip $(PLATFORM)),	SC8800G)
	DV_ISP_VER = v1
endif
ifeq ($(strip $(PLATFORM)),	SC6530)
	DV_ISP_VER = v1
endif
ifneq ($(findstring $(strip $(PLATFORM)),UWS6121E SC6531EFM),)
	DV_ISP_VER = v1
endif




######################################################################################################
#                                FEATURE OPTIONS
######################################################################################################
DV_PLAYER_SUPPORT 	= TRUE
DV_PLAYFROMFILE_SUPPORT = TRUE
DV_PLAYFROMMEM_SUPPORT 	= TRUE
ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
DV_PLAYFROMSTREAM_SUPPORT		= TRUE
endif
ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
DV_PLAYFROMSTREAM_SUPPORT		= TRUE
endif

ifeq ($(strip $(VIDEO_RECORD_SUPPORT)),	TRUE)
	DV_RECORDER_SUPPORT 		= TRUE
	DV_RECORDFROMCAM_SUPPORT = TRUE
#ifneq ($(strip $(ATV_SUPPORT)), NONE)  ##default TRUE
	DV_RECORDFROMATV_SUPPORT = TRUE
#endif
	DV_RECORDTOFILE_SUPPORT = TRUE
endif	

ifeq ($(strip $(VIDEO_RECORD_SUPPORT)),	FALSE)
	DV_RECORDER_SUPPORT 		= FALSE
	DV_RECORDFROMCAM_SUPPORT = FALSE
ifneq ($(strip $(ATV_SUPPORT)), NONE)  
	DV_RECORDFROMATV_SUPPORT = TRUE
endif
	DV_RECORDTOFILE_SUPPORT = FALSE
endif	

ifeq ($(strip $(VT_SUPPORT)), TRUE)
DV_VT_SUPPORT 			= TRUE
endif



DV_MP4_DEMUX_SUPPORT 		= TRUE
DV_MP4_MUX_SUPPORT   		= TRUE
ifeq ($(strip $(AVI_DEC_SUPPORT)), TRUE)
DV_AVI_DEMUX_SUPPORT		= TRUE
endif
ifeq ($(strip $(AVI_ENC_SUPPORT)), TRUE)
DV_AVI_MUX_SUPPORT		= TRUE
endif
ifeq ($(strip $(FLV_DEC_SUPPORT)), TRUE)
DV_FLV_DEMUX_SUPPORT		= TRUE
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 16X16)
    DV_USER_DEMUX_SUPPORT		= FALSE
else ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
    DV_USER_DEMUX_SUPPORT		= FALSE
else ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 128X128)
    DV_USER_DEMUX_SUPPORT		= FALSE
else
    DV_USER_DEMUX_SUPPORT		= TRUE
endif

ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
DV_STREAM_DEMUX_SUPPORT		= TRUE
endif



DV_MPEG4_DEC_SUPPORT		= TRUE
DV_MPEG4_ENC_SUPPORT		= TRUE
ifneq ($(strip $(JPG_DEC_SUPPORT)), NONE)
DV_MJPG_DEC_SUPPORT		= TRUE
endif
ifneq ($(strip $(JPG_ENC_SUPPORT)), NONE)
ifeq ($(strip $(AVI_ENC_SUPPORT)), TRUE)
DV_MJPG_ENC_SUPPORT		= TRUE
endif
ifeq ($(strip $(TYPE_3GP_SUPPORT)), MJPEG)
DV_MJPG_ENC_SUPPORT		= TRUE
endif
endif
ifeq ($(strip $(H264_DEC_SUPPORT)), TRUE)
DV_H264_DEC_SUPPORT		= TRUE
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 16X16)
    DV_USER_VDEC_SUPPORT		= FALSE
else ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
    DV_USER_VDEC_SUPPORT		= FALSE
else ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 128X128)
    DV_USER_VDEC_SUPPORT		= FALSE
else
    DV_USER_VDEC_SUPPORT		= TRUE
endif



DV_AMR_ENC_SUPPORT			= TRUE
DV_AMR_DEC_SUPPORT			= TRUE
ifeq ($(strip $(AAC_SUPPORT)), TRUE)
DV_AAC_DEC_SUPPORT			= TRUE
endif
ifeq ($(strip $(MP3_SUPPORT)), TRUE)
DV_MP3_DEC_SUPPORT			= TRUE
endif
ifeq ($(strip $(WAV_SUPPORT)), TRUE)
DV_PCM_ENC_SUPPORT			= TRUE
DV_PCM_DEC_SUPPORT			= TRUE
endif
ifeq ($(strip $(DRA_SUPPORT)), TRUE)
DV_DRA_DEC_SUPPORT			= TRUE
endif
ifeq ($(strip $(TA8LBR_SUPPORT)), TRUE)
DV_RA_DEC_SUPPORT		= TRUE
endif

ifeq ($(strip $(AMR_A2DP_SUPPORT)), FALSE)
DV_AMR_DSP_DEC_SUPPORT		= TRUE
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 16X16)
DV_LOW_MEMORY_SUPPORT		= TINY
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
DV_LOW_MEMORY_SUPPORT		= SMALL
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 128X128)
DV_LOW_MEMORY_SUPPORT		= SMALL
endif



######################################################################################################
#                                MACRO DEFINITION
######################################################################################################


ifeq ($(strip $(DV_PLAYER_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_PLAYER_SUPPORT

	ifeq ($(strip $(DV_PLAYFROMFILE_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_PLAYFROMFILE_SUPPORT
	endif
	
	ifeq ($(strip $(DV_PLAYFROMMEM_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_PLAYFROMMEM_SUPPORT
	endif
	
	ifeq ($(strip $(DV_PLAYFROMSTREAM_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_PLAYFROMSTREAM_SUPPORT
	endif

	
endif



ifeq ($(strip $(DV_RECORDER_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_RECORDER_SUPPORT

	ifeq ($(strip $(DV_RECORDFROMCAM_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_RECORDFROMCAM_SUPPORT
	endif
	
	ifeq ($(strip $(DV_RECORDFROMATV_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_RECORDFROMATV_SUPPORT
	endif

	ifeq ($(strip $(DV_RECORDTOFILE_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_RECORDTOFILE_SUPPORT
	endif
endif


ifeq ($(strip $(DV_RECORDER_SUPPORT)), FALSE)	
	ifeq ($(strip $(DV_RECORDFROMATV_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_RECORDER_SUPPORT
	MCFLAG_OPT += -DDV_RECORDFROMATV_SUPPORT
	MCFLAG_OPT += -DDV_RECORDTOFILE_SUPPORT
	endif
endif


ifeq ($(strip $(DV_VT_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_VT_SUPPORT
endif



ifeq ($(strip $(DV_MP4_DEMUX_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_MP4_DEMUX_SUPPORT
endif

ifeq ($(strip $(DV_MP4_MUX_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_MP4_MUX_SUPPORT
endif

ifeq ($(strip $(DV_AVI_DEMUX_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_AVI_DEMUX_SUPPORT
endif

ifeq ($(strip $(DV_AVI_MUX_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_AVI_MUX_SUPPORT
endif

ifeq ($(strip $(DV_FLV_DEMUX_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_FLV_DEMUX_SUPPORT
endif

ifeq ($(strip $(DV_USER_DEMUX_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_USER_DEMUX_SUPPORT
endif

ifeq ($(strip $(DV_STREAM_DEMUX_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_STREAM_DEMUX_SUPPORT
endif


ifeq ($(strip $(DV_MPEG4_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_MPEG4_DEC_SUPPORT
endif

ifeq ($(strip $(DV_MPEG4_ENC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_MPEG4_ENC_SUPPORT
endif

ifeq ($(strip $(DV_MJPG_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_MJPG_DEC_SUPPORT
endif

ifeq ($(strip $(DV_MJPG_ENC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_MJPG_ENC_SUPPORT
endif

ifeq ($(strip $(DV_H264_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_H264_DEC_SUPPORT
	ifeq ($(strip $(PLATFORM)),	SC6800H)
	MCFLAG_OPT += -DDV_H264_DEC_SUPPORT_SW
	endif
	ifeq ($(strip $(PLATFORM)),	SC8800G)
	MCFLAG_OPT += -DDV_H264_DEC_SUPPORT_HW
	endif
	ifeq ($(strip $(PLATFORM)),	SC6530)
	MCFLAG_OPT += -DDV_H264_DEC_SUPPORT_SW
	endif
	ifneq ($(findstring $(strip $(PLATFORM)),UWS6121E SC6531EFM),)
	MCFLAG_OPT += -DDV_H264_DEC_SUPPORT_SW
	endif
endif

ifeq ($(strip $(DV_USER_VDEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_USER_VDEC_SUPPORT
endif




ifeq ($(strip $(DV_AMR_ENC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_AMR_ENC_SUPPORT
endif

ifeq ($(strip $(DV_AMR_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_AMR_DEC_SUPPORT
ifeq ($(strip $(DV_AMR_DSP_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_AMR_DSP_DEC
endif
endif

ifeq ($(strip $(DV_AAC_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_AAC_DEC_SUPPORT
endif

ifeq ($(strip $(DV_MP3_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_MP3_DEC_SUPPORT
endif

ifeq ($(strip $(DV_PCM_ENC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_PCM_ENC_SUPPORT
endif

ifeq ($(strip $(DV_PCM_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_PCM_DEC_SUPPORT
endif

ifeq ($(strip $(DV_DRA_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_DRA_DEC_SUPPORT
endif

ifeq ($(strip $(DV_RA_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DDV_RA_DEC_SUPPORT
endif

ifeq ($(strip $(DV_LOW_MEMORY_SUPPORT)), TINY)
MCFLAG_OPT += -DDV_LOW_MEMORY_SUPPORT
endif

ifeq ($(strip $(DV_LOW_MEMORY_SUPPORT)), SMALL)
MCFLAG_OPT += -DDV_LOW_MEMORY_SUPPORT
endif


#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT	+= -D_DV_SC8800G

# It depends on the vsp capacity , memory size and user requirement

MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_VGA
MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_VGA
MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_HQVGA
#MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_720P

MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_QVGA
MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_QVGA

MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L4
MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L4
MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L4
#MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L5

MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L1
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6800H)
MCFLAG_OPT	+= -D_DV_SC6800H

# It depends on the vsp capacity , memory size and user requirement

MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_VGA
MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_VGA
MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_QVGA
MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_720P

MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_QVGA
MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_QVGA

MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L1
MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L1
MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L1
MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L5

MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L0
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),	SC6530)
MCFLAG_OPT	+= -D_DV_SC6530

# It depends on the vsp capacity , memory size and user requirement

ifeq ($(strip $(DV_LOW_MEMORY_SUPPORT)), TINY)
    MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_VGA
    MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_QVGA
    
    MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_CIF
    MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_CIF
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L0
    MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L0
    MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L0
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L0
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_AUDIO_SUPPORT_L0
    MCFLAG_OPT  += -DDV_AVI_DEMUX_AUDIO_SUPPORT_L0
    MCFLAG_OPT  += -DDV_FLV_DEMUX_AUDIO_SUPPORT_L0
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_AUDIO_SUPPORT_L0
    
    MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_FILE_CACHE_CONFIG_L0

else
 ifeq ($(strip $(DV_LOW_MEMORY_SUPPORT)), SMALL)
    MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_VGA
    MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_CIF
    MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_CIF
    
    MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_QVGA
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L6
    MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L2
    MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L6
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L6
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_AUDIO_SUPPORT_L1
    MCFLAG_OPT  += -DDV_AVI_DEMUX_AUDIO_SUPPORT_L1
    MCFLAG_OPT  += -DDV_FLV_DEMUX_AUDIO_SUPPORT_L1
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_AUDIO_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_FILE_CACHE_CONFIG_L1
 else
    MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_VGA
    MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_SVGA
    ifeq ($(strip $(CHIP_VER)),	6531)
    MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_HVGA
    else
    MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_QVGA
    endif
    MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_720P
    
    MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_CIF
    MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_CIF
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L3
    MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L3
    MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L3
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L3
    
    MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_FILE_CACHE_CONFIG_L1
 endif
endif

endif

ifneq ($(findstring $(strip $(PLATFORM)),UWS6121E SC6531EFM),)
MCFLAG_OPT	+= -D_DV_SC6530

# It depends on the vsp capacity , memory size and user requirement

ifeq ($(strip $(DV_LOW_MEMORY_SUPPORT)), TINY)
    MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_QVGA
    
    MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_QVGA
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L0
    MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L0
    MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L0
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L0
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_AUDIO_SUPPORT_L0
    MCFLAG_OPT  += -DDV_AVI_DEMUX_AUDIO_SUPPORT_L0
    MCFLAG_OPT  += -DDV_FLV_DEMUX_AUDIO_SUPPORT_L0
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_AUDIO_SUPPORT_L0
    
    MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_FILE_CACHE_CONFIG_L0

else
 ifeq ($(strip $(DV_LOW_MEMORY_SUPPORT)), SMALL)
    MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_QVGA
    
    MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_QVGA
    MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_QVGA
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L6
    MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L2
    MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L6
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L6
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_AUDIO_SUPPORT_L1
    MCFLAG_OPT  += -DDV_AVI_DEMUX_AUDIO_SUPPORT_L1
    MCFLAG_OPT  += -DDV_FLV_DEMUX_AUDIO_SUPPORT_L1
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_AUDIO_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_FILE_CACHE_CONFIG_L1
 else
    MCFLAG_OPT  += -DDV_MJPG_DEC_SUPPORT_VGA
    MCFLAG_OPT  += -DDV_MPEG4_DEC_SUPPORT_SVGA
    MCFLAG_OPT  += -DDV_H264_DEC_SUPPORT_QVGA

    MCFLAG_OPT  += -DDV_RV_DEC_SUPPORT_720P
    
    MCFLAG_OPT  += -DDV_MJPG_ENC_SUPPORT_CIF
    MCFLAG_OPT  += -DDV_MPEG4_ENC_SUPPORT_CIF
    
    MCFLAG_OPT  += -DDV_MP4_DEMUX_SUPPORT_L3
    MCFLAG_OPT  += -DDV_AVI_DEMUX_SUPPORT_L3
    MCFLAG_OPT  += -DDV_FLV_DEMUX_SUPPORT_L3
    MCFLAG_OPT  += -DDV_RMVB_DEMUX_SUPPORT_L3
    
    MCFLAG_OPT  += -DDV_AVI_MUX_SUPPORT_L1
    
    MCFLAG_OPT  += -DDV_FILE_CACHE_CONFIG_L1
 endif
endif

endif
#-----------------------------------------------------------------------------------------------------


										
								

######################################################################################################
#                                INCLUDE PATH and SOURCE FILE
######################################################################################################


MINCPATH =  MS_Ref/source/dv/inc

MINCPATH += MS_Ref/source/aud_config/inc_export 			\
            MS_Ref/source/audioservice/inc_export 		\
            MS_Ref/source/image_proc/inc 							\
            MS_Ref/source/isp_service/inc							\
            MS_Ref/source/lcd/inc
            
MINCPATH += MS_Ref/source/codec/aud_common/aud_encode/inc \
            MS_Ref/source/codec/aud_common/frame_buffer/inc \
            MS_Ref/source/codec/aud_common/aud_mux/inc \
            MS_Ref/source/codec/aud_common/audio_input/inc \
            MS_Ref/source/codec/aud_common/stream/inc \
            MS_Ref/source/codec/aud_common/inc \
            MS_Ref/export/inc
            
MINCPATH += BASE/ps/export/inc						
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += MS_Ref/source/mpeg4_codec/mpeg4_codec_uws6121e/inc/encoder
MINCPATH += MS_Ref/rtos/export/inc




MSRCPATH += MS_Ref/source/dv/src/player								\
            MS_Ref/source/dv/src/recorder							\
            MS_Ref/source/dv/src/vt										\
            \
            MS_Ref/source/dv/src/module								\
            MS_Ref/source/dv/src/access								\
            \
            MS_Ref/source/dv/src/demux/avi						\
            MS_Ref/source/dv/src/demux/mp4						\
            MS_Ref/source/dv/src/demux/flv						\
            MS_Ref/source/dv/src/demux/stream					\
            MS_Ref/source/dv/src/demux/user     			\
            \
            MS_Ref/source/dv/src/mux/avi							\
            MS_Ref/source/dv/src/mux/mp4 							\
            \
            MS_Ref/source/dv/src/audio_decoder 				\
            MS_Ref/source/dv/src/audio_encoder				\
            \
            MS_Ref/source/dv/src/video_decoder 				\
            MS_Ref/source/dv/src/video_encoder				\
            \
            MS_Ref/source/dv/src/video_display				\
            MS_Ref/source/dv/src/video_capture				\
            MS_Ref/source/dv/src/utils


SOURCES	+=  access.c						\
            videocodec_task.c				\
            block.c							\
            misc.c							\
            module.c						\
            color_convert.c




ifeq ($(strip $(DV_PLAYER_SUPPORT)), TRUE)
SOURCES 		+=  player_display_$(strip $(DV_ISP_VER)).c
SOURCES     +=  player_new.c  player_main.c player_state.c player_inner.c
SOURCES     +=  audio_dec.c

	ifeq ($(strip $(DV_PLAYFROMFILE_SUPPORT)), TRUE)
	SOURCES 	+=  file_cache_rd.c
	endif

	ifeq ($(strip $(DV_PLAYFROMMEM_SUPPORT)), TRUE)
	SOURCES 	+=  access_mem.c
	endif

	ifeq ($(strip $(DV_PLAYFROMSTREAM_SUPPORT)), TRUE)
	SOURCES 	+=  access_stream.c
	endif
endif





ifeq ($(strip $(DV_RECORDER_SUPPORT)), TRUE)
SOURCES +=  recorder_capture_$(strip $(DV_ISP_VER)).c
SOURCES +=  recorder_new.c recorder_app.c recorder_inner.c
	ifeq ($(strip $(DV_RECORDTOFILE_SUPPORT)), TRUE)
	SOURCES 	+=  file_cache_wr.c tmpfile.c
	endif
endif

ifeq ($(strip $(DV_RECORDER_SUPPORT)), FALSE)
	ifeq ($(strip $(DV_RECORDFROMATV_SUPPORT)), TRUE)
		SOURCES +=  recorder_capture_$(strip $(DV_ISP_VER)).c
		SOURCES +=  recorder_new.c recorder_app.c recorder_inner.c 
		SOURCES +=  file_cache_wr.c tmpfile.c
	endif
	ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
		SOURCES +=  recorder_new.c recorder_app.c recorder_inner.c
		SOURCES +=  file_cache_wr.c tmpfile.c
	endif
endif

ifeq ($(strip $(DV_VT_SUPPORT)), TRUE)
SOURCES     +=  vt_intg.c
endif





ifeq ($(strip $(DV_MP4_DEMUX_SUPPORT)), TRUE)
SOURCES +=  mp4_opt2.c libmp4_opt2.c
endif

ifeq ($(strip $(DV_MP4_MUX_SUPPORT)), TRUE)
SOURCES +=  mp4_mux_opt.c
endif

ifeq ($(strip $(DV_AVI_DEMUX_SUPPORT)), TRUE)
SOURCES +=  avi.c libavi.c 
endif

ifeq ($(strip $(DV_AVI_MUX_SUPPORT)), TRUE)
SOURCES +=  avi_mux.c  
endif

ifeq ($(strip $(DV_FLV_DEMUX_SUPPORT)), TRUE)
SOURCES +=  demux_flv.c 
endif

ifeq ($(strip $(DV_USER_DEMUX_SUPPORT)), TRUE)
SOURCES +=  demux_user.c
endif


ifeq ($(strip $(DV_STREAM_DEMUX_SUPPORT)), TRUE)
SOURCES += demux_stream.c
endif





ifeq ($(strip $(DV_AMR_ENC_SUPPORT)), TRUE)
SOURCES +=   amr_enc.c
endif

ifeq ($(strip $(DV_AMR_DEC_SUPPORT)), TRUE)
SOURCES +=   amr_dec.c
endif

ifeq ($(strip $(DV_PCM_ENC_SUPPORT)), TRUE)
SOURCES +=  pcm_enc.c
endif

ifeq ($(strip $(DV_PCM_DEC_SUPPORT)), TRUE)
SOURCES +=  pcm_dec.c
endif

ifeq ($(strip $(DV_AAC_DEC_SUPPORT)), TRUE)
SOURCES +=  aac_dec.c
endif

ifeq ($(strip $(DV_MP3_DEC_SUPPORT)), TRUE)
SOURCES +=  mp3_dec.c 
endif

ifeq ($(strip $(DV_DRA_DEC_SUPPORT)), TRUE)
SOURCES +=  dra_dec.c 
endif

ifeq ($(strip $(DV_RA_DEC_SUPPORT)), TRUE)
SOURCES +=  ra_dec.c
endif





ifeq ($(strip $(DV_MPEG4_ENC_SUPPORT)), TRUE)
SOURCES +=  mpeg4_enc.c
endif

ifeq ($(strip $(DV_MPEG4_DEC_SUPPORT)), TRUE)
SOURCES +=  mpeg4_dec.c
endif

ifeq ($(strip $(DV_MJPG_DEC_SUPPORT)), TRUE)
SOURCES +=  mjpg_dec.c
endif

ifeq ($(strip $(DV_MJPG_ENC_SUPPORT)), TRUE)
SOURCES +=  mjpg_enc_$(strip $(DV_ISP_VER)).c
endif

ifeq ($(strip $(DV_H264_DEC_SUPPORT)), TRUE)
SOURCES +=  h264_dec.c
endif

ifeq ($(strip $(DV_USER_VDEC_SUPPORT)), TRUE)
SOURCES +=  user_vdec.c
endif


