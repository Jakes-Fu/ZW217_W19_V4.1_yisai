MCFLAG_OPT   	= -Otime -zc
MCFLAG_OPT   	+=-DMP3_ASSERT_MODE
ifeq ($(strip $(PLATFORM)),SC8800H)
MCFLAG_OPT += -fx
endif
ifeq ($(strip $(PLATFORM)),SC8800G)
MCFLAG_OPT += -fx
endif
ifeq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT   	+= -DMP3_STATIC_TASK
ifeq ($(strip $(CHIP_VER)), 6600R2)
MCFLAG_OPT   	+= -DMP3_DISABLE_PROBE -DCHIP_VER_6600R2A
endif
ifeq ($(strip $(CHIP_VER)), 6600R3)
MCFLAG_OPT   	+= -DMP3_DISABLE_PROBE -DCHIP_VER_6600R2A
endif
#MCFLAG_OPT   	+= -DMP3_DISABLE_PROBE -DCHIP_VER_6600R1
endif

ifeq ($(strip $(PLATFORM)), 6800)
MCFLAG_OPT   	+= -D_SC6800_
endif

ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT   	+= -D_SC8800H_
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT   	+= -D_SC8800H_
endif

ifeq ($(strip $(PLATFORM)), HIMALAYA)
MCFLAG_OPT   	+= -D_SC6600H_ -DMP3_STATIC_TASK
endif

ifeq ($(strip $(PLATFORM)), NILE)
MCFLAG_OPT   	+= -D_SC6600I_ -DMP3_STATIC_TASK
endif

ifeq ($(strip $(PLATFORM)), 6600M)
MCFLAG_OPT   	+= -D_SC6600M_ -DMP3_STATIC_TASK
endif

MINCPATH =  MS_Ref/source/codec/mp3/inc MS_Ref/source/codec/mp3/src MS_Ref/export/inc \
 MS_Ref/source/aud_exp/eq/inc MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH +=$(L1_INCPATH) 	
MSRCPATH =  MS_Ref/source/codec/mp3/src

SOURCES			= mp3_decode.c mp3_adp.c mp3_getfileinfo.c mp3_format.c mp3_ffs.c

ifneq ($(strip $(PLATFORM)),SC8800H)
ifneq ($(strip $(PLATFORM)),SC8800G)
SOURCES	+= mp3_decode_rom.s
endif
endif

ifeq ($(strip $(PLATFORM)),SC8800H)
MINCPATH += MS_Ref\source\codec\mp3\src\mp3_decoder_8800h 
MSRCPATH += MS_Ref\source\codec\mp3\src\mp3_decoder_8800h

SOURCES	+= mp3_trans_samplerate.c mp3_huffdec_arm.s mp3_imdct_arm.s mp3_layer_arm.s mp3_parse_arm.s mp3_quantize_arm.s \
           mp3_stereo_arm.s mp3_stream_arm.s mp3_synth_arm.s mp3_decoder.c mp3_frame_arm.s mp3_decoder_main.c \
           layer12.c mp3_synth_arm_r2.s mp3_layer_arm_r2.s mp3_frame_arm_r2.s mp3_huffdec_arm_patch.s 
endif


ifeq ($(strip $(PLATFORM)),SC8800G)
MINCPATH += MS_Ref\source\codec\mp3\src\mp3_decoder_8800h 
MSRCPATH += MS_Ref\source\codec\mp3\src\mp3_decoder_8800h

SOURCES	+= mp3_trans_samplerate.c mp3_huffdec_arm.s mp3_imdct_arm.s mp3_layer_arm.s mp3_parse_arm.s mp3_quantize_arm.s \
           mp3_stereo_arm.s mp3_stream_arm.s mp3_synth_arm.s mp3_decoder.c mp3_frame_arm.s mp3_decoder_main.c \
           layer12.c mp3_synth_arm_r2.s mp3_layer_arm_r2.s mp3_frame_arm_r2.s mp3_huffdec_arm_patch.s 
endif