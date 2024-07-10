
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
GE2DRIVERPATH        =  MS_Customize/source/product/driver/gps/green_eye2

MINCPATH += build/$(PROJECT)_builddir/tmp

MINCPATH +=	MS_Ref/source/base/inc \
                        chip_drv/chip_plf/sc6531efm \
                        chip_drv/chip_plf/common\inc \
                        $(GE2DRIVERPATH)/ge2_download/inc \
                        $(GE2DRIVERPATH)/ge2_libgps_mp/gps_lib/inc \
                        chip_drv/export/inc \
                        Third-party/qx/inc

MSRCPATH +=	$(GE2DRIVERPATH) \
			$(GE2DRIVERPATH)/ge2_download/src \
			$(GE2DRIVERPATH)/ge2_libgps_mp/gps_lib/src

SOURCES +=	gps_greeneye2.c \
			greeneye2_crc16.c \
			greeneye2_packet.c \
			greeneye2_download.c \
			greeneye2.c \
			greeneye2_hardware.c \
			greeneye2_gnss_libgps_api.c \
			greeneye2_comm.c \
			greeneye2_nmeaenc.c

ifeq ($(strip $(GPS_MINI_LOG_SUPPORT)), TRUE)
SOURCES +=	greeneye2_gnss_log.c
endif

ifeq ($(strip $(GPS_TCARD_LOG_SUPPORT)), TRUE)
SOURCES +=	greeneye2_tcard_log.c
endif

ifeq ($(strip $(GPS_AGPS_SUPPORT)), QX)
SOURCES +=	greeneye2_eph.c \
			greeneye2_agps_interface.c
endif
else
MCFLAG_OPT += -DBINARY_IMAGE_IN_MEMORY

MSRCPATH +=	MS_Customize/source/product/driver/gps/sirf/prgflash/src \
			MS_Customize/source/product/driver/gps/sirf/prgflash/flash_images \
			MS_Customize/source/product/driver/gps/sirf/prgflash/sirfflashengineep/src
			
MINCPATH +=	MS_Customize/source/product/driver/gps/sirf/prgflash/common \
			MS_Customize/source/product/driver/gps/sirf/prgflash/src \
			MS_Customize/source/product/driver/gps/sirf/prgflash/sirfflashengineep/src \
			MS_Customize/source/product/driver/gps/sirf/prgflash/flash_images

#prgflash_ss3img.mcp
SOURCES +=	commio.c \
			errsup.c \
			imgacc.c \
			main.c \
			timsup.c \
			delay.c \
			imgdata_ss3.c

#SiRFflashEngineEP.mcp
SOURCES +=	starter.c \
			burner.c \
			chksum.c \
			command.c \
			loader.c \
			pckunpck.c \
			sndrcv.c \
			sirfflashengineep.c
endif


