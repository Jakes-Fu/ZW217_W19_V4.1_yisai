#ifndef _CONFIG_H_
#define _CONFIG_H_

#if 0  //modify by xaimeng.yang
#include <lwip/opt.h>
#include <lwip/debug.h>
#include <lwip/def.h> /* provide ntohs, htons */

#endif




#if 1  //modify by xaimeng.yang
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#include "errno.h"
#include "arpa/inet.h"
#include "socket.h" ///for up layer
#include "time.h"
#include "os_api.h"
#endif


#define inline        __inline  

#define NI_MAXSERV 32
#define NI_MAXHOST 1024


#define __ICCARM__     //for utlist.h   LDECLTYPE(x)

#define BSD 199103

#define WITH_POSIX 1

#define PACKAGE_NAME "libcoap"
#define PACKAGE_VERSION "master-b425b15-dtls-25863042"
#define PACKAGE_STRING PACKAGE_NAME PACKAGE_VERSION

/* it's just provided by libc. i hope we don't get too many of those, as
 * actually we'd need autotools again to find out what environment we're
 * building in */
//#define HAVE_STRNLEN 1

#define HAVE_MALLOC 1

#define HAVE_LIMITS_H
#define HAVE_NETINET_IN_H
#define HAVE_SYS_SOCKET_H
#define WITH_MBEDDTLS
#define HAVE_ASSERT_H
#define HAVE_UNISTD_H

#define COAP_RESOURCES_NOHASH
#define _POSIX_TIMERS 0
#define HAVE_SYS_UNISTD_H 0

#define COAP_NO_TRACE

#define  u64_t unsigned long  long int /**< 64 bit unsigned  */
#define  u32_t unsigned            int /**< 32 bit unsigned  */
#define  u16_t unsigned short      int /**< 16 bit unsigned  */
#define   u8_t unsigned char           /**<  8 bit unsigned  */

#define  i64_t          long  long int /**< 64 bit signed  */
#define  i32_t                     int /**< 32 bit signed  */
#define  i16_t          short      int /**< 16 bit signed  */
#define   i8_t          char           /**<  8 bit signed  */

#endif /* _CONFIG_H_ */
