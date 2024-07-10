ADUPSFOTAPATH        =  Third-party/adups

MINCPATH +=	MS_Ref/source/base/inc \
            chip_drv/chip_plf/sc6531efm \
            chip_drv/chip_plf/common\inc \
            chip_drv/export/inc \
            BASE/l4/export/inc \
            common/export/inc \
            chip_drv/export/inc/outdated \
            DAPS/export/inc/tcpip6 \
            $(ADUPSFOTAPATH)/hal/include \
            $(ADUPSFOTAPATH)/hal/halcore/include \
            $(ADUPSFOTAPATH)/app/include \
            BASE/atc/modem/source/h \
            BASE/atc/feature_phone/source/h \
            BASE/sim/export/inc \
            BASE/ps/export/inc \
            BASE/atc/export/inc \

            

MSRCPATH +=	$(ADUPSFOTAPATH) \
						$(ADUPSFOTAPATH)/hal/src

SOURCES += adups_debug.c \
	 				 adups_device.c \
	 				 adups_display.c
