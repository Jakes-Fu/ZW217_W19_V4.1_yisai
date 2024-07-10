/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#ifndef _LS_OSA_H_
#define _LS_OSA_H_

#ifdef WIN32
#include "alipay_common.h"
#else
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#endif
#if ( defined(__ARMCC_VERSION) || defined(_MSC_VER) ) && \
        !defined(inline) && !defined(__cplusplus)
#define inline __inline
#endif

#define LS_LOG_TAG          "<LS_LOG> "

#define LS_NET_TYPE_TCP     0
#define LS_NET_TYPE_UDP     1

struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* and microseconds */
};
/**
 * @brief format and print data.
 *
 * @param fmt[in]: formatted string.
 */
void ls_osa_print(const char *fmt, ...);

/**
 * @brief write formatted data to a string.
 *
 * @param str[in]:  string that holds written text.
 * @param size[in]: maximum length of character will be written.
 * @param fmt[in]:  string that contains the text to be written.
 *
 * @return: the written length.
 */
int ls_osa_snprintf(char *str, size_t size, const char *fmt, ...);

/**
 * @brief allocates a block of size bytes of memory.
 *
 * @param size[in]: the block size in bytes.
 *
 * @return a pointer to the allocated memory.
 */
void *ls_osa_malloc(size_t size);

/**
 * @brief allocates memory for an array of nmemb elements of size bytes each,
 *        the memory is set to zero.
 *
 * @param nmemb[in]: array elements item counts.
 * @param size[in]: size in bytes for every array elements.
 *
 * @return a pointer to the allocated memory.
 */
void *ls_osa_calloc(size_t nmemb, size_t size);

/**
 * @brief frees the memory space pointed to by ptr.
 */
void ls_osa_free(void *ptr);

/**
 * @brief sleep thread itself.
 *
 * @param msec[in]: the time interval to be suspended, in milliseconds.
 */
void ls_osa_msleep(unsigned int msec);

/* @brief get current timestamp in milliseconds.
 *
 * @return: timestamp.
 */
#ifdef WIN32
int64_t ls_osa_get_time_ms(void);
#else
long long ls_osa_get_time_ms(void);
#endif
#endif /* _LS_OSA_H_ */

