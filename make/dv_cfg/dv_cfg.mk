
######################################################################################################
#                                FEATURE OPTIONS
######################################################################################################
ifeq ($(strip $(TMVB_DEC_SUPPORT)), TRUE)
	DV_CFG_RMVB_DEMUX_SUPPORT		= TRUE
	DV_CFG_RMVB_DEC_SUPPORT		= TRUE
endif


######################################################################################################
#                                MACRO DEFINITION
######################################################################################################
ifeq ($(strip $(DV_CFG_RMVB_DEMUX_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_CFG_RMVB_DEMUX_SUPPORT
endif

ifeq ($(strip $(DV_CFG_RMVB_DEC_SUPPORT)), TRUE)
	MCFLAG_OPT += -DDV_CFG_RMVB_DEC_SUPPORT
endif


######################################################################################################
#                                INCLUDE PATH and SOURCE FILE
######################################################################################################

MINCPATH 	=  MS_Ref/export/inc
MSRCPATH	=  MS_Ref/source/dv_cfg/src

SOURCES		=  dv_cfg.c
