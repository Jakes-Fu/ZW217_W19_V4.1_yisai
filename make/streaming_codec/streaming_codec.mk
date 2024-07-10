
ifeq ($(strip $(PLATFORM)),	SC8800H)
MCFLAG_OPT	+=  -D_DV_SC8800H 
endif

MINCPATH   =   MS_Ref/export/inc \
               MS_Ref/source/audioservice/inc_export \
               MS_Ref/source/audioservice/inc \
               MS_Ref/source/streaming_codec/inc

MSRCPATH   =   MS_Ref/source/streaming_codec/src

SOURCES	   =   sm_codec_task.c sm_codec_common.c sm_video_codec.c \
               sm_audio_codec.c sm_aac_codec.c sm_amr_codec.c dummy.c
 
