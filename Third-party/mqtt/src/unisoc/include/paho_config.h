#ifndef __PAHO_CONFIG_H__
#define __PAHO_CONFIG_H__

#if 1  //add by hongqun.cheng
//#include "socket_types.h"
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#include "errno.h"

#include "socket.h" ///for up layer
#include "time.h"
#endif

//typedef unsigned char	bool;

#define _UNISOC_

#ifndef NO_PERSISTENCE
#define NO_PERSISTENCE
#endif

#ifndef NOSTACKTRACE
#define NOSTACKTRACE
#endif

#ifndef HIGH_PERFORMANCE
#define HIGH_PERFORMANCE
#endif

#ifdef HEAP_H
#undef HEAP_H
#endif

#ifdef OPENSSL
#undef OPENSSL
#endif

//#include "net_config.h"
//#ifdef CONFIG_MBEDTLS_LIB_SUPPORT
//#include "mbedtls_config.h"
//#ifdef CONFIG_MBEDTLS_SSL_TLS
#ifndef MBEDTLS
#define MBEDTLS
#endif
//#endif
//#endif

#define BUILD_TIMESTAMP "Temp"
#define CLIENT_VERSION "V3.1.1"

#define htobe16(x) htons(x)
#define htobe32(x) htonl(x)
#define htobe64(x) htonl(x)
#define be16toh(x) ntohs(x)
#define be32toh(x) ntohl(x)
#define be64toh(x) ntohl(x)
#define getpeername(x, y, z) sci_sock_getpeername(x, y)

#if 0
//Porting Limits of integral types
#ifndef INT8_MIN
#define INT8_MIN (-128)
#endif
#ifndef INT16_MIN
#define INT16_MIN (-32767 - 1)
#endif
#ifndef INT32_MIN
#define INT32_MIN (-2147483647 - 1)
#endif
#ifndef INT8_MAX
#define INT8_MAX (127)
#endif
#ifndef INT16_MAX
#define INT16_MAX (32767)
#endif
#ifndef INT32_MAX
#define INT32_MAX (2147483647)
#endif
#ifndef UINT8_MAX
#define UINT8_MAX (255U)
#endif
#ifndef UINT16_MAX
#define UINT16_MAX (65535U)
#endif
#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif
#ifndef SCHAR_MIN
#define SCHAR_MIN (-128)
#endif
#ifndef SCHAR_MAX
#define SCHAR_MAX (127)
#endif
#ifndef UCHAR_MAX
#define UCHAR_MAX (255U)
#endif
#endif

#endif
