MCFLAG_OPT = -Otime


######################################################################################################
#                                MACRO DEFINITION
######################################################################################################

MCFLAG_OPT += -D_SIMULATION_ -D_ARM_ -DMPEG4_DEC -DMPEG4_ENC -DKAL_ON_RTT



######################################################################################################
#                                INCLUDE PATH and SOURCE FILE
######################################################################################################



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)), SC6800H)
MINCPATH = MS_Ref/source/vsp/sc6800h/inc
MSRCPATH = MS_Ref/source/vsp/sc6800h/src
SOURCES	= vsp_drv.c
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)), SC6530)
MINCPATH += chip_drv/export/inc/outdated MS_Ref/source/vsp/sc6530/inc
MSRCPATH = MS_Ref/source/vsp/sc6530/src
SOURCES	= vsp_drv.c
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH += chip_drv/export/inc/outdated MS_Ref/source/vsp/sc6531efm/inc
MSRCPATH = MS_Ref/source/vsp/sc6531efm/src
SOURCES	= vsp_drv.c
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH += chip_drv/export/inc/outdated MS_Ref/source/vsp/uws6121e/inc
MINCPATH +=	MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/common
MINCPATH += MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/decoder
MINCPATH += MS_Ref/source/mpeg4_codec/mpeg4_codec_sc6530/inc/encoder
MINCPATH +=	MS_Ref/source/vsp/sc6530/inc
MINCPATH +=	MS_Ref/source/vsp/c_model/sc6530/inc
			
					
MSRCPATH = MS_Ref/source/vsp/uws6121e/src
MSRCPATH += MS_Ref/source/vsp/c_model/sc6530/src

SOURCES	+= vsp_drv.c \
                        ahbm_top.c \
                        bsm_core.c \
                        bsm_top.c \
                        bsm_trace.c \
                        buffer_top.c \
                        common_top.c \
                        common_trace.c \
                        dcam_top.c \
                        dct_asic.c \
                        dct_core.c \
                        dct_top.c \
                        dct_trace.c \
                        global_top.c \
                        mbc_core.c \
                        mbc_top.c \
                        mbc_trace.c \
                        mca_core.c \
                        mca_top.c \
                        mca_trace.c \
                        vlc_core.c \
                        vlc_jpeg_rlc.c \
                        vlc_top.c \
                        vlc_trace.c \
                        vld_jpeg_rld.c \
                        vld_mpeg4_dcac_pred.c \
                        vld_mpeg4_rld.c \
                        vld_top.c \
                        vld_trace.c 
                        
endif
#-----------------------------------------------------------------------------------------------------



#-----------------------------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)), SC8800G)
MINCPATH = MS_Ref/source/vsp/sc8800g/inc
MSRCPATH = MS_Ref/source/vsp/sc8800g/src
SOURCES	= vsp_drv_sc8800g.c
endif
#-----------------------------------------------------------------------------------------------------
