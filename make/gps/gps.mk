
#ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
GPSDRIVERPATH  =  MS_Ref/source/gps

#MINCPATH += build/$(PROJECT)_builddir/tmp

MINCPATH +=	MS_Ref/export/inc \
			MS_Ref/source/base/inc \
			MS_Ref/source/ulog/inc \
			RTOS/export/inc \
			chip_drv/export/inc \
			chip_drv/export/inc/outdated \
			chip_drv/chip_plf/sc6531efm \
			chip_drv/chip_plf/common\inc \
			chip_drv/chip_plf/ANTISW3 \
			chip_drv/chip_plf/uws6121e \
			DAPS/export/inc/tcpip6 \
			MS_Ref/source/engpc/sprd_fts_inc \
			MS_Ref/source/engpc/modules/libftmode/inc \
			MS_Ref/source/nvm/inc \
			Third-party/qx/inc \
			common/nv_parameters/wcn/h \
			$(GPSDRIVERPATH)/green_eye2/gnsspc \
			$(GPSDRIVERPATH)/green_eye2/ge2_libgps_mp/gps_lib/inc \
			BASE/l4/export/inc \
			BASE/PS/export/inc \
			BASE/Layer1/export/inc \


MSRCPATH +=	$(GPSDRIVERPATH) \
			$(GPSDRIVERPATH)/green_eye2 \
			$(GPSDRIVERPATH)/green_eye2/ge2_libgps_mp/gps_lib/src \
			$(GPSDRIVERPATH)/green_eye2/gnsspc \
			Third-party/qx/src

SOURCES	+=	gps_greeneye2.c \
			greeneye2.c \
			greeneye2_comm.c \
			greeneye2_gnss_libgps_api.c \
			greeneye2_hardware.c \
			greeneye2_nmeaenc.c \
            greeneye2_sipc.c \
			gnss_pc_cmd.c \
			gps_pc_mode.c \
			fft.c \
			gps_drv.c \
			gps_cfg.c \
			gps_diag.c \
			gps_adpt.c \
			gps_ual_signal.c \

ifeq ($(strip $(GPS_MINI_LOG_SUPPORT)), TRUE)
SOURCES +=	greeneye2_gnss_log.c
endif

ifeq ($(strip $(GPS_TCARD_LOG_SUPPORT)), TRUE)
#SOURCES +=	greeneye2_tcard_log.c
endif

ifeq ($(strip $(GPS_AGPS_SUPPORT)), QX)
SOURCES +=	greeneye2_eph.c \
			greeneye2_agps_interface.c \
			qxwz_sdk_os.c \
			qxwz_sdk_socket.c
endif

ifeq ($(strip $(SPREAD_ORBIT_SUPPORT)), TRUE)
MCFLAG_OPT += -DGPS_SPREAD_ORBIT_SUPPORT

SOURCES +=	greeneye2_spreadorbit.c
endif

ifeq ($(strip $(SPRD_SIPC_HOST_ENABLE)), TRUE)
MCFLAG_OPT      += -DSPRD_LIBGPS_SIPC
endif

#on W317 project, save eph flash to file, so disable it
#MCFLAG_OPT      += -DSAVE_EPH_FLASH_TO_NV

ifeq ($(strip $(RELEASE_INFO)), TRUE)
MCFLAG_OPT      += -DUSER_VERSION
endif

#endif