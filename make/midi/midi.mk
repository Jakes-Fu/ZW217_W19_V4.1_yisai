#MCFLAG_OPT   	+= -DMIDI_ASSERT_MODE

MIDI_VER = V2

ifeq ($(strip $(SOUNDBANK_VER)),NEWMIDI_75KB)
MIDI_VER = V3
endif
ifeq ($(strip $(SOUNDBANK_VER)),NEWMIDI_170KB)
MIDI_VER = V3
endif
ifeq ($(strip $(SOUNDBANK_VER)),NEWMIDI_400KB)
MIDI_VER = V3
endif

ifeq ($(strip $(PLATFORM)),SC6800H)
ifeq ($(strip $(PRODUCT_CONFIG)),sc6800h_sp6804h)
MCFLAG_OPT   	+= -DMIDI_PROFILE_SIMPLE
endif
endif

ifeq ($(strip $(PLATFORM)),SC6530)
MCFLAG_OPT   	+= -DMIDI_PROFILE_SIMPLE
endif

ifeq ($(strip $(PLATFORM)),SC6531EFM)
MCFLAG_OPT   	+= -DMIDI_PROFILE_SIMPLE
endif

ifeq ($(strip $(PLATFORM)),UWS6121E)
MCFLAG_OPT   	+= -DMIDI_PROFILE_SIMPLE
endif

MCFLAG_OPT		+= -D_DYNAMIC_THREAD_ -DMIDI_DISABLE_IRAM

ifeq ($(strip $(MIDI_VER)),V1)
MINCPATH = MS_Ref/source/codec/midi/inc MS_Ref/source/codec/midi/src MS_Ref/export/inc BASE/ps/export/inc
MINCPATH +=$(L1_INCPATH) 	
MSRCPATH =  MS_Ref/source/codec/midi/src
SOURCES			= midi_sc_api.c midi_adp.c
endif

ifeq ($(strip $(MIDI_VER)),V2)
MINCPATH = MS_Ref/source/codec/midi/src_new MS_Ref/export/inc MS_Ref/source/codec/midi/midi_new BASE/ps/export/inc
MINCPATH +=$(L1_INCPATH)
MINCPATH += chip_drv/export/inc/outdated
MSRCPATH =  MS_Ref/source/codec/midi/midi_new MS_Ref/source/codec/midi/src_new
SOURCES = midi_constant.c midi_decode.c midi_keyon.c midi_calculation.c midi_api.c midi_modenv.c midi_volenv.c midi_vibrato.c midi_modlfo.c midi_filter.c
SOURCES += midi_adp.c
endif

ifeq ($(strip $(MIDI_VER)),V3)
MINCPATH = MS_Ref/export/inc MS_Ref/source/codec/midiv3/midicore/host_src MS_Ref/source/codec/midiv3/midicore/lib_scisynth MS_Ref/source/codec/midiv3/midicore/lib_src
MINCPATH +=$(L1_INCPATH)
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MSRCPATH =  MS_Ref/source/codec/midiv3 MS_Ref/source/codec/midiv3/midicore/lib_scisynth MS_Ref/source/codec/midiv3/midicore/host_src MS_Ref/source/codec/midiv3/midicore/lib_src
SOURCES = midi_adp.c
SOURCES += sci_synth.c   sci_synth_irq.c  		   sci_synth_regop.c \
		eas_data.c           eas_enhancer.c  			eas_enhdata.c eas_flog.c \
		eas_math.c           eas_midi.c 					eas_mididata.c  \
		eas_mixbuf.c         eas_mixer.c          eas_pan.c 			\
    eas_pcm.c            eas_pcmdata.c        eas_public.c    \
    eas_smf.c            eas_smfdata.c 				eas_tcdata.c eas_tonecontrol.c \
    eas_voicemgt.c 			 eas_wtengine.c       eas_wtsynth.c

REVERB_ENABLED = FALSE

ifeq ($(strip $(PLATFORM)),SC6530)
SOURCES += sci_synth_hardware.c
endif

ifeq ($(strip $(PLATFORM)),SC6531EFM)
SOURCES += sci_synth_hardware.c
endif

ifneq ($(strip $(PLATFORM)),SC6530)
ifneq ($(strip $(PLATFORM)),SC6531EFM)
SOURCES += sci_synth_simulator.c
endif
endif

ifeq ($(strip $(SOUNDBANK_VER)),NEWMIDI_75KB)
SOURCES += 75k_8b_22k_128.c
MCFLAG_OPT += -D_SAMPLE_RATE_22050
endif

ifeq ($(strip $(SOUNDBANK_VER)),NEWMIDI_170KB)
SOURCES += 170kb_8b_22khz.c
MCFLAG_OPT += -D_SAMPLE_RATE_22050
endif

ifeq ($(strip $(SOUNDBANK_VER)),NEWMIDI_400KB)
SOURCES += 400kb_8b_32khz.c
MCFLAG_OPT += -D_SAMPLE_RATE_32000
endif

SOURCES += eas_config.c   eas_hostmm.c     eas_report.c

MCFLAG_OPT += -DMMAPI_SUPPORT -DEAS_WT_SYNTH -D_8_BIT_SAMPLES
MCFLAG_OPT += -DEXTERNAL_AUDIO

MCFLAG_OPT += -DNUM_OUTPUT_CHANNELS=2 -DMAX_SYNTH_VOICES=64 -DMMAPI_SUPPORT -DEAS_WT_SYNTH -D_8_BIT_SAMPLES -D_FILTER_ENABLED

#MCFLAG_OPT += -D_ENHANCER_ENABLED

ifeq ($(strip $(REVERB_ENABLED)),TRUE)
SOURCES += eas_reverb.c
MCFLAG_OPT += -D_REVERB_ENABLED
endif

#MCFLAG_OPT += -D_CHORUS_ENABLED
#MCFLAG_OPT += -D_IMELODY_PARSER -D_RTTTL_PARSER -D_OTA_PARSER -D_WAVE_PARSER -D_IMA_DECODER
#MCFLAG_OPT += -DNATIVE_EAS_KERNEL
MCFLAG_OPT += -DUNIFIED_DEBUG_MESSAGES
endif

ALAW_SOUNDBANK = FALSE

ifeq ($(strip $(SOUNDBANK_VER)),ARM_700KB)
ALAW_SOUNDBANK = TRUE
endif

ifeq ($(strip $(SOUNDBANK_VER)),ARM_256KB)
ALAW_SOUNDBANK = TRUE
endif

ifeq ($(strip $(SOUNDBANK_VER)),ARM_300KB)
ALAW_SOUNDBANK = TRUE
endif

ifeq ($(strip $(ALAW_SOUNDBANK)),TRUE)
MCFLAG_OPT += -DALAW_SOUNDBANK
endif

ifeq ($(strip $(PLATFORM)),SC8800H)
MCFLAG_OPT += -Otime $(CPU) ARM926EJ-S
endif

ifeq ($(strip $(PLATFORM)),SC8800G)
MCFLAG_OPT += -Otime $(CPU) ARM926EJ-S
endif

ifeq ($(strip $(PLATFORM)),SC6800H)
MCFLAG_OPT += -Otime $(CPU) ARM926EJ-S
endif

ifeq ($(strip $(PLATFORM)),SC6530)
MCFLAG_OPT += -Otime $(CPU) ARM926EJ-S
endif

ifeq ($(strip $(PLATFORM)),SC6531EFM)
MCFLAG_OPT += -Otime $(CPU) ARM926EJ-S
endif

ifeq ($(strip $(PLATFORM)),UWS6121E)
MCFLAG_OPT += -Otime $(CPU) ARM926EJ-S
endif