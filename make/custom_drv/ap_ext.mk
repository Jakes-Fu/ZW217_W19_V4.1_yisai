
PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

MCFLAG_OPT += -D_SP9100_P3_ -D_REF_SC8800D_

MSRCPATH +=  MS_Customize/source/product/driver/ap_ext/src \
 MS_Customize/source/product/config/$(strip $(PROD_CONFIG))

MINCPATH +=  MS_Customize/source/product/driver/ap_ext/inc \
 MS_Ref/export/inc \
 MS_Ref/source/sfs/inc \
 MS_Ref/source/ap/inc \
 MS_Customize/source/product/config/$(strip $(PROD_CONFIG))

ifeq ($(strip $(MME_ENGINE)),)
$(error You must choose a MME engine between MV8602 and MV8650!)
endif

ifeq ($(strip $(MME_ENGINE)), MV8602)
MCFLAG_OPT += -D__MV8602__ -D__T3_FEATURE_HOST__ -D__T3_FEATURE_BIG_ENDIAN__
MV8602_BASE = MS_Customize/source/product/driver/ap_ext/mv8602
MINCPATH +=  $(MV8602_BASE)/include \
 $(MV8602_BASE)/source/attach \
 $(MV8602_BASE)/source

MSRCPATH +=  $(MV8602_BASE)/source \
 $(MV8602_BASE)/source/attach
endif

ifeq ($(strip $(MME_ENGINE)), MV8650)
MCFLAG_OPT += -D__MV8650__ -D__MV_FEATURE_HOST__ -D__MV_FEATURE_BIG_ENDIAN__
MV8650_BASE = MS_Customize/source/product/driver/ap_ext/mv8650
MINCPATH +=  $(MV8650_BASE)/lib \
 $(MV8650_BASE)/mvattach \
 $(MV8650_BASE)/lib/dmc \
 $(MV8650_BASE)/libif/audioplayer \
 $(MV8650_BASE)/mvattach/lcd \
 $(MV8650_BASE)/mvattach/sensor \
 $(MV8650_BASE)/mvattach/audio \
 $(MV8650_BASE)/lib/inc \
 $(MV8650_BASE)/lib/camera \
 $(MV8650_BASE)/lib/gmaster \
 $(MV8650_BASE)/libif/camera \
 $(MV8650_BASE)/libif/fat \
 $(MV8650_BASE)/libif/jpeg \
 $(MV8650_BASE)/libif/mpeg \
 $(MV8650_BASE)/libif/mme \
 $(MV8650_BASE)/libif/audiotag \
 $(MV8650_BASE)/libif/audiorecorder \
 $(MV8650_BASE)/libif/audio \
 $(MV8650_BASE)/lib/clock \
 $(MV8650_BASE)/lib/audiotag/id3tag \
 $(MV8650_BASE)/lib/audiotag/m4atag \
 $(MV8650_BASE)/lib/audiotag/wmatag \
 $(MV8650_BASE)/lib/tvencoder \
 $(MV8650_BASE)/lib/primitive

MSRCPATH +=  $(MV8650_BASE)/mvattach \
 $(MV8650_BASE)/mvattach/sensor \
 $(MV8650_BASE)/mvattach/lcd \
 $(MV8650_BASE)/mvattach/audio \
 $(MV8650_BASE)/libif/audio \
 $(MV8650_BASE)/libif/audioeffect \
 $(MV8650_BASE)/libif/audioplayer \
 $(MV8650_BASE)/libif/audiorecorder \
 $(MV8650_BASE)/libif/audiotag \
 $(MV8650_BASE)/libif/camera \
 $(MV8650_BASE)/libif/DB \
 $(MV8650_BASE)/libif/debug \
 $(MV8650_BASE)/libif/fat \
 $(MV8650_BASE)/libif/ImageViewer \
 $(MV8650_BASE)/libif/jpeg \
 $(MV8650_BASE)/libif/mme \
 $(MV8650_BASE)/libif/mpeg \
 $(MV8650_BASE)/libif/onetimetone \
 $(MV8650_BASE)/libif/Sdk \
 $(MV8650_BASE)/libif/usb \
 $(MV8650_BASE)/lib \
 $(MV8650_BASE)/lib/audioplayer \
 $(MV8650_BASE)/lib/audiorecorder \
 $(MV8650_BASE)/lib/audiotag \
 $(MV8650_BASE)/lib/audiotag/id3tag \
 $(MV8650_BASE)/lib/audiotag/m4atag \
 $(MV8650_BASE)/lib/audiotag/wmatag \
 $(MV8650_BASE)/lib/camera \
 $(MV8650_BASE)/lib/clock \
 $(MV8650_BASE)/lib/csd \
 $(MV8650_BASE)/lib/dmc \
 $(MV8650_BASE)/lib/gmaster \
 $(MV8650_BASE)/lib/jpegdec \
 $(MV8650_BASE)/lib/mp4v \
 $(MV8650_BASE)/lib/periperals \
 $(MV8650_BASE)/lib/primitive \
 $(MV8650_BASE)/lib/sdcard \
 $(MV8650_BASE)/lib/tvencoder
endif

#Project SC8800D,sepcial files
ifeq ($(strip $(PLATFORM)), SC8800D)
SOURCES	+= mv8602recordvb_as.c \
			mp4_adp.c \
			mv8602playvb_as.c 			

#ap interface driver											
SOURCES += ap_task_ext.c \
			ap_dc.c \
			ap_device.c \
			ap_vt_imp.c \
			mp3_adp.c \
			mv_audio.c \
			mv_dv.c \
			mv_video.c\
			mv_device.c \
			mvmassstorageai.c \
			sd_task.c \
			sdcard_bsd.c \
			ap_sd.c \
			sd_timer.c

ifeq ($(strip $(MME_ENGINE)), MV8602)
SOURCES +=	ap_streaming.c
endif

ifeq ($(strip $(MME_ENGINE)), MV8602)
SOURCES +=	mvattach.c mvaudiopi.c mvprimitive.c mvperi.c \
			mvmp4vi.c mvmp4vdeci.c mvjpegdeci.c mvexifenc.c \
			mvexifdec.c mvcamera.c mvaudiori.c mvsdif.c \
			mvmmeai.c mvjpegai.c mvaudiopai.c mvcamai.c \
			mvfatai.c mvsensor_ov9650.c mvsensor.c \
			mvaudiodecai.c mvaudioencai.c mvmpegai.c \
			mvmpegdecai.c isrTask.c mvlcd.c mvcameraai.c \
			mvcontrol.c wm8976.c mvinit.c mvaudiopID3Tagi.c \
			mvonetimetoneai.c Audio_device.c
endif

ifeq ($(strip $(MME_ENGINE)), MV8650)
SOURCES += mvaudiopi.c 	mvaudiori.c mvaudiopID3Tagi.c drms.c mp4atom.c \
			mp4ff.c mp4meta.c mp4ports.c mp4sample.c mp4util.c mvcamera.c \
			mvexifdec.c mvexifenc.c mvdelay.c mvpll.c mvSdram.c \
			mvgmaster.c mvjpegdeci.c mvmp4vdeci.c mvmp4vi.c mvperi.c \
			mvprimitive.c mvtvout.c mvaudiodecai.c \
			mvaudioencai.c mvaudio3dai.c mvaudioeqai.c mvaudioveai.c \
			mvaudiovsai.c mvaudiopai.c mvaudiorai.c mvaudioptagi.c \
			mvM4aTag.c mvcamai.c mvcameraai.c mvCsdAi.c MvDBai.c \
			mvblockioai.c mvfatai.c ImageViewerai.c mvjpegai.c \
			mvmmeai.c mvmpegai.c mvmpegdecai.c mvonetimetoneai.c \
			mvusbai.c audioapi.c mvlcd.c sensor.c mvattach.c \
			mvsdif.c mvTVEncoder.c isrTask.c wm8976.c Audio_device.c \
			MVsensor_ov2640_wingtech.c
endif
endif
