
MCFLAG_OPT   	= -Otime -zc -DCHIP_SC6600C 

ifeq ($(strip $(PLATFORM)), SC8800D)
MCFLAG_OPT 		+= -D_REF_SC8800D_ -DDSP_AUDIO_II
endif

ifeq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT		+= -DDSP_AUDIO_II 
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif
ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT		+= -DDSP_AUDIO_III -DDSP_AUDIO_DUALMODE
endif
ifeq ($(strip $(PLATFORM)), SC6800H)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT		+= -DHW_VIBERATE
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MCFLAG_OPT		+= -DDSP_AUDIO_III
MCFLAG_OPT		+= -DHW_VIBERATE
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MCFLAG_OPT		+= -DDSP_AUDIO_III
MCFLAG_OPT		+= -DHW_VIBERATE
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT		+= -DDSP_AUDIO_III -DDSP_AUDIO_DUALMODE
endif

ifeq ($(strip $(FM_VBC)), TRUE)
MCFLAG_OPT		+=-DFM_VBC_EQ
endif

MINCPATH =  MS_Ref/source/audioservice/inc \
						MS_Ref/source/audioservice/inc_export \
						MS_Ref/source/aud_config/inc_export\
						MS_Ref/source/aud_dev/inc
MINCPATH += chip_drv/export/inc/outdated \
						common/export/inc BASE/ps/export/inc

MINCPATH +=$(L1_INCPATH) 			

ifeq ($(strip $(DUALMIC)), TRUE)
MINCPATH += MS_Ref/export/inc/dualmic
else
MINCPATH += MS_Ref/export/inc/singlemic
endif	

MSRCPATH =  MS_Ref/source/audioservice/src MS_Ref/source/audioservice/src/audio_service_manage \
 MS_Ref/source/audioservice/src/audio_service_manage/as_submodules MS_Ref/source/audioservice/src/audio_devicemode_manage

SOURCES				= acm_main.c adm_dp.c adm_mng.c apm_codec.c apm_express.c apm_gen.c  aspec_calc.c\
								asm_hc.c asm_main.c audio_api.c \
								admm.c audio_nv_arm.c \
								codec_param_nv.c cvs_ctrl_param_nv.c
