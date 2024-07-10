#ifndef __CMS_TEST_LOG__
#define __CMS_TEST_LOG__
#include "os_util.h"
#include "os_api.h"

#if 1
#define LOG_TEST_E(tag, format, ...)        SCI_TRACE_LOW("[E][%s] "format, tag, ##__VA_ARGS__)
#define LOG_TEST_W(tag, format, ...)        SCI_TRACE_LOW("[W][%s] "format, tag, ##__VA_ARGS__)
#define LOG_TEST_I(tag, format, ...)        SCI_TRACE_LOW("[I][%s] "format, tag, ##__VA_ARGS__)
#define LOG_TEST_D(tag, format, ...)        SCI_TRACE_LOW("[D][%s] "format, tag, ##__VA_ARGS__)
#else
#define LOG_TEST_E(tag, fmt, ...)
#define LOG_TEST_W(tag, fmt, ...)
#define LOG_TEST_I(tag, fmt, ...)
#define LOG_TEST_D(tag, fmt, ...)
#endif

#endif