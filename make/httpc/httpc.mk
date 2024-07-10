#----------------------------------------------------------------------------#
#                                  HTTPC                                      #
#----------------------------------------------------------------------------#
ifeq ($(strip $(MBEDTLS_VERSION)), V224)
MINCPATH += Third-party/mbedtls/V224/include
else
#else ifeq ($(strip $(MBEDTLS_VERSION)), V206)
MINCPATH += Third-party/mbedtls/V206/include
endif

MINCPATH     += DAPS/source/httpc/h 
MINCPATH     += DAPS/source/hsm/inc 
MINCPATH     += DAPS/source/netmanager/inc 
MINCPATH     +=	DAPS/source/common/inc

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif

MSRCPATH     += DAPS/source/httpc/c

SOURCES      += httpc_api.c httpc_task.c httpc.c httpc_mbedtls.c httpc_serialize.c\
				httpc_signal.c httpc_net.c httpc_common.c httpc_timer.c

	