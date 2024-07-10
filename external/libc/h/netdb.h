/**
 * Copyright (C) 2021 Alibaba.inc, All rights reserved.
 *
 * @file   httpdns.c
 * @brief  
 * @author gaopan.gpr@alibaba-inc.com
 * @date   2021/01/15
 */


#ifndef NETDB_H
#define NETDB_H

#include <sys/socket.h>


#ifdef __cplusplus
extern "C" {
#endif




// struct hostent *gethostbyname(const char *name);
// int gethostbyname_r(const char *name, struct hostent *ret, char *buf,
//                 size_t buflen, struct hostent **result, int *h_errnop);
void freeaddrinfo(struct addrinfo *ai);
int getaddrinfo(const char *nodename,
       const char *servname,
       const struct addrinfo *hints,
       struct addrinfo **res);



#ifdef __cplusplus
}
#endif

#endif
