#ifndef __SPRD_FTS_LOG_H__
#define __SPRD_FTS_LOG_H__

#include "sci_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPRD_FTS_LOG_TAG "sprd_fts:"
#define SPRD_ENGPC_LOG_TAG "ENGPC:"

#define SPRD_FTS_TRACE

#ifdef SPRD_FTS_TRACE
#define SPRD_FTS_LOG(fmt, args...) SCI_TraceLow(SPRD_FTS_LOG_TAG fmt"\r\n", fmt, ##args)
#define ENG_LOG(fmt, args...) {\
                                SCI_TraceLow(SPRD_ENGPC_LOG_TAG fmt"\r\n", ##args); \
                               }
#else
#define SPRD_FTS_LOG(format, ...)
#define ENG_LOG(fmt, args...)
#endif

#ifdef __cplusplus
}
#endif

#endif
