#ifndef __RTMP_SYS_H__
#define __RTMP_SYS_H__

#ifdef _WIN32

	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#pragma warning(disable: 4018)

	#include <winsock2.h>
	#include <ws2tcpip.h>

	#ifdef _MSC_VER    /* MSVC */
		#if _MSC_VER < 1900
			#define snprintf _snprintf
			#define vsnprintf _vsnprintf
		#endif
		#define strcasecmp _stricmp
		#define strncasecmp _strnicmp
		#define	sys_strdup _strdup
	#endif

	#define GetSockError()  WSAGetLastError()
	#define SetSockError(e) WSASetLastError(e)
	#define setsockopt(a, b, c, d, e) (setsockopt)(a, b, c, (const char *)d, (int)e)
	#undef EWOULDBLOCK
	#define EWOULDBLOCK WSAETIMEDOUT    /* we don't use nonblocking, but we do use timeouts */
	#define sleep(n)    Sleep(n*1000)
	#define msleep(n)   Sleep(n)
	#define SET_RCVTIMEO(tv, s) int tv = s*1000

	#define rtmp_malloc(size)		malloc(size)
	#define rtmp_free(ptr)			free(ptr)
	#define rtmp_realloc(ptr, size)	realloc(ptr, size)
	#define rtmp_calloc(num, size)	calloc(num, size)

#elif _RTOS

	#include "sci_types.h"
	#include "os_api.h"
	#include "socket_types.h"
	#include "socket_api.h"

	#define REAL_WATCH_RTOS

	#define rtmp_malloc(size)		SCI_ALLOC_APP(size)
	#define rtmp_free(ptr)			SCI_Free(ptr)
	#define rtmp_realloc(ptr, size)	rtmp_realloc_impl(ptr, size)
	#define rtmp_calloc(num, size)	rtmp_calloc_impl(num, size)

	#define sleep(n)    SCI_Sleep(n*1000)
	#define msleep(n)   SCI_Sleep(n)

	#define	sys_strdup  strdup

	#define assert( exp ) SCI_ASSERT( exp )

#else /* !_WIN32 */

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/times.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#define GetSockError()  errno
	#define SetSockError(e) errno = e
	#undef closesocket
	#define closesocket(s)  close(s)
	#define msleep(n)       usleep(n*1000)
	#define SET_RCVTIMEO(tv, s) struct timeval tv = { s, 0 }

	#define rtmp_malloc(size)		malloc(size)
	#define rtmp_free(ptr)			free(ptr)
	#define rtmp_realloc(ptr, size)	realloc(ptr, size)
	#define rtmp_calloc(num, size)	calloc(num, size)

#endif

#include "rtmp.h"

#endif


#ifdef REAL_WATCH_RTOS

static void* rtmp_realloc_impl(void* ptr, size_t newSize)
{
	char* newPtr = NULL;
	size_t oldSize = 0;

	if (newSize == 0) {
		return NULL;
	}

	if (NULL == ptr) {
		newPtr = SCI_ALLOC_APP(newSize);
	}
	else {
		oldSize = SCI_MEMSIZE(ptr);

		SCI_ASSERT(oldSize > 0);
		SCI_ASSERT(newSize >= oldSize);
		newPtr = (char*)SCI_ALLOC_APP(newSize);
		SCI_ASSERT(NULL != newPtr);
		SCI_MEMSET(newPtr, 0, newSize);
		SCI_MEMCPY(newPtr, ptr, oldSize);
		SCI_FREE(ptr);
	}
	return (void *)newPtr;
}

static void* rtmp_calloc_impl(size_t num, size_t size)
{
	if(num == 0 || size == 0)
		return NULL;
	else {
		int total = num * size;
		void *ptr = rtmp_malloc(total);
	//	if (ptr)
	//		os_memset(ptr, 0, total);
		return ptr;		
	}
}
#endif

/*
 *      Copyright (C) 2010 Howard Chu
 *
 *  This file is part of librtmp.
 *
 *  librtmp is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1,
 *  or (at your option) any later version.
 *
 *  librtmp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with librtmp see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *  http://www.gnu.org/copyleft/lgpl.html
 */