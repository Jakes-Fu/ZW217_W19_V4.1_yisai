
ifeq ($(strip $(PLATFORM)),	SC6600L)
WMA_VER = ARM7
endif

ifeq ($(strip $(PLATFORM)),SC6800H)
WMA_VER = ARM9
endif

ifeq ($(strip $(PLATFORM)),SC6530)
WMA_VER = ARM9
endif

ifeq ($(strip $(PLATFORM)),SC6531EFM)
WMA_VER = ARM9
endif

ifeq ($(strip $(PLATFORM)),SC8800H)
WMA_VER = ARM9
endif

ifeq ($(strip $(PLATFORM)),SC8800G)
WMA_VER = ARM9
endif

ifeq ($(strip $(WMA_VER)),ARM7)
MCFLAG_OPT   	= -DDISABLE_OPT -D_RECONSTRUCTION_ -DWMFAPI_NO_DRM \
			-DWMA_OPT_STRMDEC_ARM -DWMA_OPT_INVERSQUAN_LOWRATE_ARM -DWMA_OPT_LPCSTD_ARM -DENABLE_LPC \
			-D_Embedded_x86 -DBITSTREAM_CORRUPTION_ASSERTS -DWMA_OPT_SUBFRAMERECON_ARM -DWMA_DEBUG -DENABLE_ALL_ENCOPT \
			-DWMA_OPT_AURECON_ARM -DBUILD_INTEGER -D_RTOS -DWMA_OPT_HUFFDEC_ARM -D_ARM_ -DWMA_OPT_FFT_ARM -D_DEBUG \
			-DBUILD_WMASTD -D_WMF_ -DWMAAPI_NO_DRM_STREAM -DWMA_V9 -D_RAM_ALLOC_ -DWMAAPI_NO_DRM 
ifeq ($(strip $(CHIP_ENDIAN)),LITTLE)
MCFLAG_OPT   	+= -DLITTLE_ENDIAN
else
MCFLAG_OPT   	+= -DBIG_ENDIAN
endif

MASMFLAG_OPT   = 
endif


ifeq ($(strip $(WMA_VER)),	ARM9)
MCFLAG_OPT   	= -DDISABLE_OPT -D_RECONSTRUCTION_ \
					-DWMFAPI_NO_DRM -DWMA_OPT_STRMDEC_ARM -DWMA_OPT_INVERSQUAN_LOWRATE_ARM -DWMA_OPT_LPCSTD_ARM -DENABLE_LPC \
					-D_Embedded_x86  -DWMA_OPT_SUBFRAMERECON_ARM -DWMA_DEBUG -DENABLE_ALL_ENCOPT \
					-DWMA_OPT_AURECON_ARM -DBUILD_INTEGER -D_RTOS -DWMA_OPT_HUFFDEC_ARM -D_ARM_ -DWMA_OPT_FFT_ARM -D_DEBUG \
					-DBUILD_WMASTD -D_WMF_ -D_REF_SC6800_ -DWMAAPI_NO_DRM_STREAM -DWMA_V9 -D_RAM_ALLOC_ -DWMAAPI_NO_DRM
ifeq ($(strip $(CHIP_ENDIAN)),LITTLE)
MCFLAG_OPT   	+= -DLITTLE_ENDIAN
else
MCFLAG_OPT   	+= -DBIG_ENDIAN
endif
MASMFLAG_OPT   = 
endif

MCFLAG_OPT       +=-DWMA_ASSERT_MODE

MINCPATH =  MS_Ref/source/codec/wma/src/asfwma/asfdemuxsrc \
 MS_Ref/source/codec/wma/src/asfwma/include \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/armx \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/common \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/decoder \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/types \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/include \
 MS_Ref/source/audioservice/inc_export \
 MS_Ref/source/audioservice/inc \
 MS_Ref/export/inc\
 chip_drv/export/inc/outdated \
 BASE/ps/export/inc 
												
MSRCPATH =  MS_Ref/source/codec/wma/src \
 MS_Ref/source/codec/wma/src/asfwma/asfdemuxsrc \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/armx \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/common \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/decoder \
 MS_Ref/source/codec/wma/src/asfwma/wmadecodersrc/api
				

SOURCES			= wma_adp.c \
				msaudio_arm.s        huffdec_arm.s      lpcstd_arm.s        msaudiostdpro_arm.s\
				fft_arm.s            strmdec_arm.s      lowrate_arm.s       arm_stub.c\
				asf_demux.c          wmfparse.c \
				dectablesstd_ram.c   dectablesstd_rom.c    dectablesstdpro.c \
				msaudiotemplate.c    msaudiostd.c          aumalloc.c \
				configcommon.c       strmdec_wma.c         entropydec.c \
				msaudiostdpro.c      entropydecstd.c       lpcstd.c \
				msaudio.c            lowrate_commonstd.c   msaudiodec_v0_1.c\
				wma_decoder_api.c				 
				 
				
