# video
MINCPATH += \
	$(MMI_DIR)/source/mmi_app/app/video_call_mw/h \
    Third-party/agora_rtc_sdk/include \
	Third-party/oneos/inc/audio \
	Third-party/oneos/inc/camera \
	Third-party/oneos/inc/fs \
	Third-party/oneos/inc/kernel \
	Third-party/oneos/inc/lcd \
	Third-party/oneos/inc/socket \
	BASE/PS/export/inc \
	BASE/l4/export/inc \
	BASE/sim/export/inc \
	BASE/atc/export/inc \
	CAF/Template/include \
	MS_MMI_Main/source/mmi_kernel/include \
	MS_MMI_Main/source/mmi_app/kernel/h \
	MS_MMI_Main/source/mmi_app/app/phone/h \
	MS_MMI_Main/source/mmi_utility/h \
	MS_MMI_Main/source/mmi_app/common/h \
	MS_MMI_Main/source/resource \
	MS_MMI_Main/source/mmi_app/app/pdp/h \
	MS_MMI_Main/source/mmi_app/app/connection/h \
	MS_Ref/source/display/inc \
	MS_Ref/source/display/inc \
	MS_Ref/export/inc \
    MS_Ref/source/image_proc/inc \
	MS_MMI_Main/source/mmi_app/app/audio/h \
	MS_MMI_Main/source/mmi_gui/include \
	MS_Ref/source/lcd/inc \
	DAPS/export/inc/tcpip6

MSRCPATH += $(MMI_DIR)/source/mmi_app/app/video_call_mw/c

MCFLAG_OPT = \
    -D__KERNEL_ARM_ARCH__=8 \
    -DCONFIG_SPREADTRUM_OS \
    -DBITS_PER_LONG=32 \
    -DCONFIG_32BIT

SOURCES += agora_video_call.c \
	g722_decode.c \
	g722_encode.c \
	avc_capture.c \
	avc_decode.c \
	avc_encode.c \
	avc_rtc.c \
	avc_render.c \
	avc_platform_w217.c \
	avc_pipeline.c \
	avc_ringbuf.c \
	avc_license.c \
	tjpgd.c \
	libyuv.c \
	fps_cc.c
