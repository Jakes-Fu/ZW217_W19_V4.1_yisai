/*
 * Copyright (C) 2016-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <stdarg.h>
#include "zfb_common.h"
#include "ls_osa.h"
//#include "timing_alt.h"

#ifdef WIN32
void ls_osa_print_win(const char *fmt, ...)
{
    return;
}
#endif
void ls_osa_print(const char *fmt, ...)
{
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;

    va_start(va_args, fmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,fmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,fmt, va_args);
#endif
    if (tmp_len > 0) {
        ZFB_ID2TraceExt(buf,tmp_len);
      }
    va_end(va_args);
}

int ls_osa_snprintf(char *str, size_t size, const char *fmt, ...)
{
    va_list args;
    int     rc;

    va_start(args, fmt);
#ifdef WIN32
    rc = vsprintf(str,  fmt, args);
#else
    rc = vsnprintf(str, size, fmt, args);
#endif
    va_end(args);

    return rc;
}

void *ls_osa_malloc(size_t size)
{
    return SCI_ALLOC_APP((unsigned int)size);
}

void *ls_osa_calloc(size_t nmemb, size_t size)
{
    return SCI_ALLOC_APP(nmemb*size);
}

void ls_osa_free(void *ptr)
{
    if(ptr != NULL)
    {
        SCI_FREE(ptr);
    }
}

void ls_osa_msleep(unsigned int msec)
{
    SCI_SLEEP(msec);
}

#ifdef WIN32
int64_t ls_osa_get_time_ms(void)
{
    return ZFBGetSystemTimeMS();
}
#else
long long ls_osa_get_time_ms(void)
{
    return ZFBGetSystemTimeMS();
}
#endif

