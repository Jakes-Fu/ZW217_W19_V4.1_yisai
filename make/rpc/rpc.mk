ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH = MS_Ref/source/rpc/inc \
           MS_Ref/export/inc \
           MS_Include/chip_drv

ifeq ($(strip $(CONFIG_ANTIREG_LICENSE_SUPPORT)), TRUE)
MINCPATH += chip_drv/chip_module/efuse/uws6121e/anti
endif

MSRCPATH = MS_Ref/source/rpc/src
endif

SOURCES = rpc_daemon.c \
          rpc_dispatch_ap.c \
          rpc_hello_ap.c \
          cfw_event_names.c \
          cfw_rpc_ap.c \
          rpc_audio_ap.c \
          rpc_verify_interface_ap.c \
          rpc_stub.c \
