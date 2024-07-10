#Preprocessor
MCFLAG_OPT = --c99

#Directory
MINCPATH = DAPS/export/inc
# MINCPATH += MS_Ref/export/inc

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif

#Local Dir inc
MINCPATH += external/oneos/inc \
            external/oneos/inc/kernel \
            external/oneos/inc/socket \
            external/oneos/inc/fal    \
            chip_drv/export/inc \
            chip_drv/export/inc/outdated

#Local Dir src
MSRCPATH = external/oneos/src/socket \
           external/oneos/src/kernel \
           external/oneos/src/fal


#SOURCES = oneos_socket.c   oneos_fs_posix.c

# SOURCES = oneos_socket.c   oneos_fs_posix.c \
#           oneos_clock.c oneos_event.c oneos_msg.c \
# 		  oneos_mutex.c oneos_sem.c oneos_task.c oneos_timer.c
SOURCES = os_assert.c os_clock.c os_memory.c os_mutex.c os_sem.c os_task.c os_util.c  ring_buff.c socket.c fal_part.c

#audio
# MINCPATH += external/oneos/inc/audio
# MSRCPATH += external/oneos/src/audio
# SOURCES  += oneos_audio.c

#camera
# MINCPATH += external/oneos/inc/camera external/oneos/inc/lcd
# MINCPATH += MS_Ref/source/dc/dc_common/inc MS_Ref/source/image_proc/src/jpeg_codec/inc MS_Ref/source/isp_service/inc \
#             chip_drv/export/inc/outdated BASE/ps/export/inc \
#             MS_Ref/source/lcd/inc \
#             MS_Ref/source/image_proc/inc \
#             MS_Ref/source/image_proc/src/jpeg/inc \
#             MS_Ref/source/dc/include \
#             MS_Ref/source/scp/inc \
#             MS_Customize/export/inc

# MSRCPATH += external/oneos/src/camera
# SOURCES  += oneos_camera.c oneos_jpeg.c

#lcd
# MINCPATH += external/oneos/inc/lcd
# MSRCPATH += external/oneos/src/lcd
# SOURCES  += oneos_lcd.c

#mmi api
# MINCPATH += MS_MMI_Main/source/mmi_app/common/h  MS_MMI_Main/source/resource \
#             MS_MMI_Main/source/mmi_kernel/include CAF/Template/include \
#             MS_MMI_Main/source/mmi_app/kernel/h \
#             MS_MMI_Main/source/mmi_app/app/phone/h \
#             MS_MMI_Main/source/mmi_app/app/wlan/h \
# 			MS_MMI_Main/source/mmi_utility/h \
# 			BASE/l4/export/inc \
# 			BASE/sim/export/inc \
# 			BASE/atc/export/inc

# MINCPATH += external/oneos/inc/mmi_api
# MSRCPATH += external/oneos/src/mmi_api
# SOURCES  += oneos_mmi_api.c

#Local Dir inc
# MINCPATH += Third-party/oneos/audio/inc/

#Local Dir src
# MSRCPATH += Third-party/oneos/audio/src/
# MSRCPATH += Third-party/oneos/audio/demo/

# SOURCES += audio_framework.c audio_framework_demo.c