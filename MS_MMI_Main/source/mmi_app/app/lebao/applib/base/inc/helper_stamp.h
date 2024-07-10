#ifndef __HELPER_TIMESTAMP_H
#define __HELPER_TIMESTAMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"

sds timestamp_nowtime(void);
sds timestamp_nowday(void);
sds timestamp_ms(void);

unsigned int timestamp_seconds_int(void);
unsigned int timestamp_tz_str_to_time(const char* str);

#ifdef __cplusplus
}
#endif
#endif
