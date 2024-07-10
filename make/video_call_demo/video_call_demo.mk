# video
MINCPATH += \
	Third-party/agora_rtc_sdk/include \
    DAPS/export/inc/tcpip6 \
	BASE/PS/export/inc \
	BASE/l4/export/inc \
	BASE/sim/export/inc \
	BASE/atc/export/inc \
	CAF/Template/include \
	MS_MMI_Main/source/mmi_kernel/include \
	MS_MMI_Main/source/mmi_app/kernel/h \
	MS_MMI_Main/source/mmi_app/app/phone/h \
	MS_MMI_Main/source/mmi_app/app/video_call_demo/h \
	MS_MMI_Main/source/mmi_utility/h \
	MS_MMI_Main/source/mmi_app/common/h \
	MS_MMI_Main/source/resource \
	MS_MMI_Main/source/mmi_app/app/pdp/h \
	MS_MMI_Main/source/mmi_app/app/connection/h \
	MS_Ref/source/display/inc \
	MS_Ref/source/display/inc \
	MS_Ref/export/inc \
    MS_Ref/source/image_proc/inc \
	MS_MMI_Main/source/mmi_app/app/audio/h


MSRCPATH += $(MMI_DIR)/source/mmi_app/app/video_call_demo/c

MCFLAG_OPT = \
    -D__KERNEL_ARM_ARCH__=8 \
    -DCONFIG_SPREADTRUM_OS \
    -DBITS_PER_LONG=32 \
    -DCONFIG_32BIT

SOURCES += video_call_demo.c