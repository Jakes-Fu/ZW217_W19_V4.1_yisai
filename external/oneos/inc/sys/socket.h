/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        socket.h
 *
 * @brief       This file is a posix wrapper for at_sock.h.
 *
 * @revision
 * Date         Author          Notes
 * 2020-06-16   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "netdb.h"
#include "os_types.h"

int os_socket(int domain, int type, int protocol);
int os_closesocket(int fd);
int os_connect(int fd, const struct sockaddr *name, socklen_t namelen);
int os_sendto(int fd, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
int os_send(int fd, const void *data, size_t size, int flags);
int os_recvfrom(int fd, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
int os_recv(int fd, void *mem, size_t len, int flags);
int os_select(int fd, os_bool_t isreadset, os_bool_t iswriteset, os_bool_t isexceptset, int timeout_ms);
int os_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);
struct hostent *os_gethostbyname(const char *name);
int os_getaddrinfo( const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
void os_freeaddrinfo(struct addrinfo *ai);
#endif