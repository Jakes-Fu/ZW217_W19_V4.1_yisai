/**
 * Copyright (C) 2021 Alibaba.inc, All rights reserved.
 *
 * @file   httpdns.c
 * @brief  
 * @author gaopan.gpr@alibaba-inc.com
 * @date   2021/01/15
 */

#ifndef INET_H
#define INET_H

#ifdef __cplusplus
extern "C" {
#endif
typedef	unsigned int uint32_t;
typedef uint32_t  u32_t;

/* If your port already typedef's in_addr_t, define IN_ADDR_T_DEFINED
   to prevent this code from redefining it. */
// #if !defined(in_addr_t) && !defined(IN_ADDR_T_DEFINED)
// typedef u32_t in_addr_t;
// #endif

// struct in_addr {
//   in_addr_t s_addr;
// };

// struct in6_addr {
//   union {
//     u32_t u32_addr[4];
//     u8_t  u8_addr[16];
//   } un;
// #define s6_addr  un.u8_addr
// };

/** 255.255.255.255 */
#define IPADDR_NONE         ((u32_t)0xffffffffUL)
/** 127.0.0.1 */
#define IPADDR_LOOPBACK     ((u32_t)0x7f000001UL)
/** 0.0.0.0 */
#define IPADDR_ANY          ((u32_t)0x00000000UL)
/** 255.255.255.255 */
#define IPADDR_BROADCAST    ((u32_t)0xffffffffUL)

/** 255.255.255.255 */
#define INADDR_NONE         IPADDR_NONE
/** 127.0.0.1 */
#define INADDR_LOOPBACK     IPADDR_LOOPBACK
/** 0.0.0.0 */
#define INADDR_ANY          IPADDR_ANY
/** 255.255.255.255 */
#define INADDR_BROADCAST    IPADDR_BROADCAST

/** This macro can be used to initialize a variable of type struct in6_addr
    to the IPv6 wildcard address. */
// #define IN6ADDR_ANY_INIT {{{0,0,0,0}}}
/** This macro can be used to initialize a variable of type struct in6_addr
    to the IPv6 loopback address. */
// #define IN6ADDR_LOOPBACK_INIT {{{0,0,0,PP_HTONL(1)}}}
/** This variable is initialized by the system to contain the wildcard IPv6 address. */
// extern const struct in6_addr in6addr_any;

/* Definitions of the bits in an (IPv4) Internet address integer.

   On subnets, host and network parts are found according to
   the subnet mask, not these masks.  */
// #define IN_CLASSA(a)        IP_CLASSA(a)
// #define IN_CLASSA_NET       IP_CLASSA_NET
// #define IN_CLASSA_NSHIFT    IP_CLASSA_NSHIFT
// #define IN_CLASSA_HOST      IP_CLASSA_HOST
// #define IN_CLASSA_MAX       IP_CLASSA_MAX

// #define IN_CLASSB(b)        IP_CLASSB(b)
// #define IN_CLASSB_NET       IP_CLASSB_NET
// #define IN_CLASSB_NSHIFT    IP_CLASSB_NSHIFT
// #define IN_CLASSB_HOST      IP_CLASSB_HOST
// #define IN_CLASSB_MAX       IP_CLASSB_MAX

// #define IN_CLASSC(c)        IP_CLASSC(c)
// #define IN_CLASSC_NET       IP_CLASSC_NET
// #define IN_CLASSC_NSHIFT    IP_CLASSC_NSHIFT
// #define IN_CLASSC_HOST      IP_CLASSC_HOST
// #define IN_CLASSC_MAX       IP_CLASSC_MAX

// #define IN_CLASSD(d)        IP_CLASSD(d)
// #define IN_CLASSD_NET       IP_CLASSD_NET     /* These ones aren't really */
// #define IN_CLASSD_NSHIFT    IP_CLASSD_NSHIFT  /*   net and host fields, but */
// #define IN_CLASSD_HOST      IP_CLASSD_HOST    /*   routing needn't know. */
// #define IN_CLASSD_MAX       IP_CLASSD_MAX

// #define IN_MULTICAST(a)     IP_MULTICAST(a)

// #define IN_EXPERIMENTAL(a)  IP_EXPERIMENTAL(a)
// #define IN_BADCLASS(a)      IP_BADCLASS(a)

// #define IN_LOOPBACKNET      IP_LOOPBACKNET


// #ifndef INET_ADDRSTRLEN
// #define INET_ADDRSTRLEN     IP4ADDR_STRLEN_MAX
// #endif
// #if LWIP_IPV6
// #ifndef INET6_ADDRSTRLEN
// #define INET6_ADDRSTRLEN    IP6ADDR_STRLEN_MAX
// #endif
// #endif

// #if LWIP_IPV4

// #define inet_addr_from_ip4addr(target_inaddr, source_ipaddr) ((target_inaddr)->s_addr = ip4_addr_get_u32(source_ipaddr))
// #define inet_addr_to_ip4addr(target_ipaddr, source_inaddr)   (ip4_addr_set_u32(target_ipaddr, (source_inaddr)->s_addr))

// /* directly map this to the lwip internal functions */
// #define inet_addr(cp)                   ipaddr_addr(cp)
// #define inet_aton(cp, addr)             ip4addr_aton(cp, (ip4_addr_t*)addr)
// #define inet_ntoa(addr)                 ip4addr_ntoa((const ip4_addr_t*)&(addr))
// #define inet_ntoa_r(addr, buf, buflen)  ip4addr_ntoa_r((const ip4_addr_t*)&(addr), buf, buflen)

// #endif /* LWIP_IPV4 */

// #if LWIP_IPV6
// #define inet6_addr_from_ip6addr(target_in6addr, source_ip6addr) {(target_in6addr)->un.u32_addr[0] = (source_ip6addr)->addr[0]; \
//                                                                  (target_in6addr)->un.u32_addr[1] = (source_ip6addr)->addr[1]; \
//                                                                  (target_in6addr)->un.u32_addr[2] = (source_ip6addr)->addr[2]; \
//                                                                  (target_in6addr)->un.u32_addr[3] = (source_ip6addr)->addr[3];}
// #define inet6_addr_to_ip6addr(target_ip6addr, source_in6addr)   {(target_ip6addr)->addr[0] = (source_in6addr)->un.u32_addr[0]; \
//                                                                  (target_ip6addr)->addr[1] = (source_in6addr)->un.u32_addr[1]; \
//                                                                  (target_ip6addr)->addr[2] = (source_in6addr)->un.u32_addr[2]; \
//                                                                  (target_ip6addr)->addr[3] = (source_in6addr)->un.u32_addr[3]; \
//                                                                  ip6_addr_clear_zone(target_ip6addr);}

// /* directly map this to the lwip internal functions */
// #define inet6_aton(cp, addr)            ip6addr_aton(cp, (ip6_addr_t*)addr)
// #define inet6_ntoa(addr)                ip6addr_ntoa((const ip6_addr_t*)&(addr))
// #define inet6_ntoa_r(addr, buf, buflen) ip6addr_ntoa_r((const ip6_addr_t*)&(addr), buf, buflen)

// #endif /* LWIP_IPV6 */


#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_INET_H */
