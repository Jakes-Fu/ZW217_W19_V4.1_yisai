ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH     += DAPS/export/inc/tcpip6
MCFLAG_OPT   += -DIPV6_SUPPORT
else
MINCPATH     += DAPS/export/inc/tcpip
endif

MSRCPATH += Third-party/libcoap/src \
Third-party/libcoap/src/obgm_libcoap \
Third-party/libcoap/src/obgm_libcoap/examples \
Third-party/libcoap/src/obgm_libcoap/examples/contiki \
Third-party/libcoap/src/obgm_libcoap/examples/lwip \
Third-party/libcoap/src/obgm_libcoap/include/coap \
Third-party/libcoap/src/obgm_libcoap/src \
Third-party/libcoap/src/obgm_libcoap/src/platform/posix \


MINCPATH +=  Third-party/libcoap/include \
Third-party/libcoap/src/obgm_libcoap \
Third-party/libcoap/src/obgm_libcoap/examples \
Third-party/libcoap/src/obgm_libcoap/examples/contiki \
Third-party/libcoap/src/obgm_libcoap/examples/lwip \
Third-party/libcoap/src/obgm_libcoap/include/coap \
Third-party/libcoap/src/obgm_libcoap/src \
Third-party/libcoap/src/obgm_libcoap/src/platform/posix \


MINCPATH +=RTOS/export/inc
  
MINCPATH  += external/libc/h
MINCPATH  += external/libc/h/sys
MINCPATH  += external/libc/h/arpa
MINCPATH  += external/libc


ifeq ($(strip $(MBEDTLS_VERSION)), V206)
MINCPATH += Third-party/mbedtls/V206/include
MINCPATH += Third-party/mbedtls/V206/include/mbedtls
else
#else ifeq ($(strip $(MBEDTLS_VERSION)), V224)
MINCPATH += Third-party/mbedtls/V224/include
MINCPATH += Third-party/mbedtls/V224/include/mbedtls
endif

SOURCES = coap_async_api.c client.c coap_list.c  Thread.c   \
address.c async.c block.c coap_debug.c coap_dtls.c coap_event.c  coap_keystore.c coap_mbeddtls.c coap_mem.c coap_time.c encode.c hashkey.c net.c option.c pdu.c resource.c str.c subscribe.c  uri.c \
coap_io.c  prng.c run.c
