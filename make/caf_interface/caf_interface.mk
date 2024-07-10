
#CAMERA
MCFLAG_OPT += -DCAMERA_SUPPORT

MCFLAG_OPT   	= 

MINCPATH =  CAF\Template\include \
 $(MMI_DIR)/source/mmi_kernel/source/h $(MMI_DIR)/source/mmi_kernel/include $(MMI_DIR)/source/mmi_gui/include \
 $(MMI_DIR)/source/mmi_ctrl/include  \
 Third-party/ImageDecoder/Include \
 MS_Ref/source/aud_config/inc_export $(MMI_DIR)/source/mmi_utility/h \
 $(MMI_DIR)/source/mmi_app/app/audio/h \
 $(MMI_DIR)/source/mmi_app/kernel/h $(MMI_DIR)/source/mmi_app/common/h  \
 $(MMI_DIR)/source/mmi_app/app/fmm/h \
 $(MMI_DIR)/source/mmi_app/app/setting/h \
 $(MMI_DIR)/source/mmi_app/app/theme/h \
 $(MMI_DIR)/source/mmi_app/app/audio/h \
 base/l4/export/inc \
 $(MMI_DIR)/source/mmi_app/kernel/h \
 $(MMI_DIR)/source/mmi_app/common/h \
 $(MMI_DIR)/source/mmi_app/custom/h \
 $(MMI_DIR)/source/resource \
base/sim/export/inc DAPS/export/inc char_lib/export/inc  base/l4/source/arm\
 version \
$(MMI_DIR)/source/mmi_app/app/camera/h \
$(MMI_DIR)/source/mmi_app/app/mpeg4/h \
$(MMI_DIR)/source/mmi_app/app/pb/h \
$(MMI_DIR)/source/mmi_app/app/pdp/h \
CAF/Interface/include \

MINCPATH += $(PS_INCPATH)
MINCPATH += chip_drv/export/inc/outdated

MSRCPATH =  CAF/Interface/source/filesys \
			CAF/Interface/source/sound \
			CAF/Interface/source/media \
			CAF/Interface/source/tapi \
			CAF/Interface/source/image \
			CAF/Interface/source/heap \
			CAF/Interface/source/socket \
			CAF/Interface/source/thread 

#ifeq ($(strip $(MMI_SERVICE_SUPPORT)), TRUE)
    MCFLAG_OPT += -DMMISRV_SUPPORT
    MINCPATH  +=  $(MMI_DIR)/source/mmi_service/export
    MINCPATH  +=  $(MMI_DIR)/source/mmi_service/source/mmisrvmgr/h
    MINCPATH  +=  $(MMI_DIR)/source/mmi_service/source/mmisrvapi/h

#    ifeq ($(strip $(MMI_SERVICE_AUDIO_SUPPORT)), TRUE)
        MCFLAG_OPT  += -DMMISRV_AUDIO_SUPPORT
        MINCPATH  +=  $(MMI_DIR)/source/mmi_service/source/mmisrvaud/h
        MINCPATH  +=  $(MMI_DIR)/source/mmi_service/source/mmisrvaud_test/h
#    endif

##endif


MINCPATH +=  build/$(PROJECT)_builddir/tmp  
CREATEFILE := $(shell $(PERL) ./make/perl_script/gettime_tofile.pl build/$(PROJECT)_builddir/tmp/buildtime.h)           	



SOURCES			= cafmm.c caffilesys.c cafsocket.c cafsound.c caftapi.c cafimage.c cafheap.c cafthread.c cafmedia.c
