ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH     += DAPS/export/inc/tcpip6
MCFLAG_OPT   += -DIPV6_SUPPORT
else
MINCPATH     += DAPS/export/inc/tcpip
endif

MSRCPATH += Third-party/liblwm2m/src \
MSRCPATH += Third-party/liblwm2m/src/dm_utils \
Third-party/liblwm2m/src/wakaama \
Third-party/liblwm2m/src/wakaama/core \
Third-party/liblwm2m/src/wakaama/core/er-coap-13 \
Third-party/liblwm2m/src/wakaama/examples/shared \
Third-party/liblwm2m/src/wakaama/examples/client \


MINCPATH +=  Third-party/liblwm2m/include \
Third-party/liblwm2m/src/wakaama/core \
Third-party/liblwm2m/src/wakaama/core/er-coap-13 \
Third-party/liblwm2m/src/wakaama/examples/shared \
Third-party/liblwm2m/src/wakaama/examples/client \


MINCPATH +=MS_Ref/export/inc \

MINCPATH +=BASE/l4/export/inc \
MINCPATH +=BASE/Layer1/export/inc  

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

SOURCES = dm_endpoint.c dm_object.c sha256.c lwm2m_api.c  ctwing_support_test.c onenet_support_test.c objtable.c object_skel.c lwm2m_fota.c epname_encode.c j_aes.c j_base64.c   \
objects.c packet.c  er-coap-13.c  data.c bootstrap.c discover.c lwm2m_uri.c block1.c lwm2m_list.c  tlv.c registration.c utils.c observe.c liblwm2m.c management.c json.c transaction.c \
commandline.c memtrace.c  lwm2m_platform.c mbedconnection.c \
object_location.c object_access_control.c object_server.c  lwm2mclient.c object_connectivity_stat.c object_security.c system_api.c test_object.c object_connectivity_moni.c object_device.c  object_firmware.c object_temperature.c object_illuminance.c 
