MCFLAG_OPT   	= -Otime

ifeq ($(strip $(FM_VBC)), TRUE)
MCFLAG_OPT		+=-DFM_VBC_EQ
endif

MINCPATH =  MS_Ref/source/aud_exp/audioprocess/inc  MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc MS_Ref/export/inc
MINCPATH += chip_drv/export/inc/outdated
     
MSRCPATH =  MS_Ref/source/aud_exp/audioprocess/src

SOURCES	 =  filter_calc.c aud_proc_enha.s aud_proc_config.c aud_proc_exp.c \
					up_samplerate.c aud_enha_exp.c aud_enha_config.c aud_comm.c \

#ifeq ($(strip $(ARMVB_RECORD_SUPPORT)), TRUE) 
#					audio_iir_asm.s aud_arm_record_exp.c
#
#MCFLAG_OPT 		+= -DAUD_ARM_RECORD_EXP
#endif



