ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH     += DAPS/export/inc/tcpip6
MCFLAG_OPT   += -DIPV6_SUPPORT
else
MINCPATH     += DAPS/export/inc/tcpip
endif

MSRCPATH += Third-party/mqtt/src/paho.mqtt.c-master/src \
Third-party/mqtt/src/unisoc/time \
Third-party/mqtt/src/unisoc/thread \
Third-party/mqtt/src/unisoc/tls \

MINCPATH +=  Third-party/mqtt/src/unisoc/include \
Third-party/mqtt/src/paho.mqtt.c-master/src \
Third-party/mqtt/src/unisoc/time \
Third-party/mqtt/src/unisoc/thread \
Third-party/mqtt/src/unisoc/tls \

MINCPATH +=RTOS/export/inc
  
MINCPATH  += external/libc/h
MINCPATH  += external/libc/h/sys
MINCPATH  += external/libc


ifeq ($(strip $(MBEDTLS_VERSION)), V206)
MINCPATH += Third-party/mbedtls/V206/include
MINCPATH += Third-party/mbedtls/V206/include/mbedtls
else
#else ifeq ($(strip $(MBEDTLS_VERSION)), V224)
MINCPATH += Third-party/mbedtls/V224/include
MINCPATH += Third-party/mbedtls/V224/include/mbedtls
endif

SOURCES = MQTTAsync.c Base64.c Clients.c FakerLog.c LinkedList.c MQTTPacket.c MQTTPacketOut.c \
MQTTProperties.c MQTTProtocolClient.c  MQTTProtocolOut.c MQTTReasonCodes.c SHA1.c \
Socket.c SocketBuffer.c utf-8.c WebSocket.c Thread.c UniTime.c TLSSocket.c 