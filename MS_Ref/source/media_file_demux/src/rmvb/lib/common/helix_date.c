

#include <time.h>
#include "helix_types.h"

HX_DATETIME HX_GET_DATETIME(void)
{
    struct tm* tm;
    time_t t = time(0);
    HX_DATETIME dt;

    tm = localtime(&t);
    dt.second = tm->tm_sec;
    dt.minute = tm->tm_min;
    dt.hour = tm->tm_hour;
    dt.dayofweek = tm->tm_wday;
    dt.dayofmonth = tm->tm_mday;
    dt.dayofyear = tm->tm_yday;
    dt.month = tm->tm_mon + 1; // 0 based (11 = December)
    dt.year = tm->tm_year;
    dt.gmtDelta = 0; // or something
    return dt;
}
