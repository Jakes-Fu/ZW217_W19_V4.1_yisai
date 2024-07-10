/* fdl_conf.h
 * Any platform relevent parameters should go here.
 */
#ifndef FDL_CONF_H
#define FDL_CONF_H

#ifdef SECURE_BOOT_SUPPORT
#define STACK_SIZE      0x2000
#else
#define STACK_SIZE      0x800
#endif

/* Settings of packet */
#define MAX_PKT_SIZE    0x1000 /* Just data field of a packet excluding header and 
* checksum
*/
#define PACKET_MAX_NUM    2

#ifdef PLATFORM_UWS6121E
#define MEMORY_START        0x80000000
#define MEMORY_SIZE          0x02000000 /* 32M */
#else
#define MEMORY_START        0x14000000
#define MEMORY_SIZE          0x00400000 /* 4M */
#endif

#endif /* FDL_CONF_H */