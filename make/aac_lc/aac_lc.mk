MCFLAG_OPT   	= -Otime -zc -D_AACARM_ 

#MCFLAG_OPT        +=-DAAC_ASSERT_MODE

ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MCFLAG_OPT   += -DAAC_DEMUX_LITTLENDIAN
endif

MINCPATH = BASE/Layer1/source/h MS_Ref/source/codec/aac_lc/inc MS_Ref/export/inc MS_Ref/source/audioservice/inc
MINCPATH += $(L1_INCPATH)
MINCPATH += chip_drv/export/inc/outdated
MINCPATH +=MS_Ref/source/codec/aud_dsp_mng/inc
MSRCPATH		= MS_Ref/source/codec/aac_lc/src

SOURCES			= aac_lc_adp.c \
                          aac_demux_bits.c aac_interface.c id3_parse.c \
                          mp4.c mp4atom.c mp4ff.c mp4sample.c mp4tagupdate.c mp4util.c 
