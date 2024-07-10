ifeq ($(strip $(LWM2M_SUPPORT)), TRUE)
#Preprocessor
#MCFLAG_OPT = --gnu
#MCFLAG_OPT += --c99 
MCFLAG_OPT += -DKAL_ON_RTT

ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MCFLAG_OPT   += -DLWM2M_LITTLE_ENDIAN
else
MCFLAG_OPT   += -DLWM2M_BIG_ENDIAN
endif
MCFLAG_OPT   += -DLWM2M_CLIENT_MODE -DLWM2M_SUPPORT_JSON -DLWM2M_WITH_LOGS

#Directory
MINCPATH     += Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/core \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/core/er-coap-13 \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/sdksrc/sdk \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/sdksrc/shared \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/testsdk \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/lib \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/lib/h \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/lib/h/sys \
				DAPS/export/inc \
				DAPS/source/common/inc\
				MSL/l4/inc \
				MSL/sim/inc \
				MSL/gas/inc \
				MSL/atc/inc \
				MS_MMI_Main/source/mmi_app/app/pdp/h \
				MS_MMI_Main/source/mmi_app/app/connection/h \
				MS_MMI_Main/source/mmi_kernel/include \
				CAF/Template/include \
				MS_MMI_Main/source/mmi_app/kernel/h \
				MS_MMI_Main/source/mmi_app/common/h \
				MS_MMI_Main/source/resource \
				BASE/l4/export/inc \
				BASE/sim/export/inc \
				BASE/PS/export/inc
	   
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH     += DAPS/export/inc/tcpip6
MCFLAG_OPT   += -DIPV6_SUPPORT
else
MINCPATH     += DAPS/export/inc/tcpip
endif

MSRCPATH     += Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/core \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/core/er-coap-13 \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/sdksrc/sdk \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/sdksrc/shared \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/lib/c \
				Third-party/lwm2m/richinfo-lwm2msdk-M-v2.0/sdksrc/testsdk
				
SOURCES      += block1.c \
				bootstrap.c \
				data.c \
				discover.c \
				json.c \
				liblwm2m.c \
				list.c \
				management.c \
				objects.c \
				observe.c \
				packet.c \
				registration.c \
				tlv.c \
				transaction.c \
				uri.c \
				utils.c \
				er-coap-13.c \
				j_aes.c \
				j_base64.c \
				lwm2mclient.c \
				lwm2msdk.c \
				object_device.c \
				object_dm.c \
				object_security.c \
				object_server.c \
				connection.c \
				memtrace.c \
				platform.c \
				socket.c \
				POSIX_thread.c \
#				main.c
#				commandline.c \

endif