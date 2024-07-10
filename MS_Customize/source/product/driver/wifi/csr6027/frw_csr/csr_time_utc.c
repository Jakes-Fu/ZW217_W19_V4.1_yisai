/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_time.h"

#include "os_api.h"
#include "rtc_drvapi.h"

#define CSR_SCHED_SECONDS_PER_DAY   (24*60*60)
#define CSR_SCHED_SECONDS_PER_HOUR  (60*60)
#define CSR_SCHED_SECONDS_PER_MIN   (60)

#if 0
#define CSR_GET_TIME_STRINGIFY_REAL(a) #a
#define CSR_GET_TIME_STRINGIFY(a) CSR_GET_TIME_STRINGIFY_REAL(a)

#define CSR_GET_TIME_ASSERT(cond, str)              \
do {                                                \
    if (!(cond))                                    \
    {                                               \
        char *panic_arg = "[" __FILE__ ":" CSR_GET_TIME_STRINGIFY(__LINE__) "] - " CSR_GET_TIME_STRINGIFY(cond); \
        char *panic_str = "[" __FILE__ ":" CSR_GET_TIME_STRINGIFY(__LINE__) "] - " str; \
        char *panic = CsrPmemZalloc(CsrStrLen(panic_arg) + CsrStrLen(panic_str) + 3); \
        CsrStrCat(panic, panic_arg);                                                \
        CsrStrCat(panic, "\n");                                                     \
        CsrStrCat(panic, panic_str);                                                \
        CsrStrCat(panic, "\n");                                                     \
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_ASSERTION_FAIL, panic);                  \
        CsrPmemFree(panic);                                                            \
    }                                                                               \
} while(0)

static const CsrUint32 secondsUntilYear[] =
{
    946684800,  /* 2000 */
    978307200,  /* 2001 */
    1009843200, /* 2002 */
    1041379200, /* 2003 */
    1072915200, /* 2004 */
    1104537600, /* 2005 */
    1136073600, /* 2006 */
    1167609600, /* 2007 */
    1199145600, /* 2008 */
    1230768000, /* 2009 */
    1262304000, /* 2010 */
    1293840000, /* 2011 */
    1325376000, /* 2012 */
    1356998400, /* 2013 */
    1388534400, /* 2014 */
    1420070400, /* 2015 */
    1451606400, /* 2016 */
    1483228800, /* 2017 */
    1514764800, /* 2018 */
    1546300800, /* 2019 */
    1577836800, /* 2020 */
    1609459200, /* 2021 */
    1640995200, /* 2022 */
    1672531200, /* 2023 */
    1704067200, /* 2024 */
    1735689600, /* 2025 */
    1767225600, /* 2026 */
    1798761600, /* 2027 */
    1830297600, /* 2028 */
    1861920000, /* 2029 */
    1893456000, /* 2030 */
    1924992000, /* 2031 */
    1956528000, /* 2032 */
    1988150400, /* 2033 */
    2019686400, /* 2034 */
    2051222400, /* 2035 */
    2082758400, /* 2036 */
    2114380800, /* 2037 */
    2145916800  /* 2038 */
};


static const CsrUint32 secondsUntilMonth[2][12] =
{
    /* Non leap year */
    {
        (CSR_SCHED_SECONDS_PER_DAY) * (0),                                  /* January */
        (CSR_SCHED_SECONDS_PER_DAY) * (31),                                 /* February */
        (CSR_SCHED_SECONDS_PER_DAY) * (28+31),                              /* March */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+28+31),                           /* April */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+28+31),                        /* May */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+30+31+28+31),                     /* June */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+30+31+28+31),                  /* July */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+30+31+30+31+28+31),               /* August */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+31+30+31+30+31+28+31),            /* September */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+31+30+31+30+31+28+31),         /* October */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+30+31+31+30+31+30+31+28+31),      /* November */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+30+31+31+30+31+30+31+28+31)    /* December */
    },
    /* Leap year */
    {
        (CSR_SCHED_SECONDS_PER_DAY) * (0),                                  /* January */
        (CSR_SCHED_SECONDS_PER_DAY) * (31),                                 /* February */
        (CSR_SCHED_SECONDS_PER_DAY) * (29+31),                              /* March */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+29+31),                           /* April */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+29+31),                        /* May */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+30+31+29+31),                     /* June */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+30+31+29+31),                  /* July */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+30+31+30+31+29+31),               /* August */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+31+30+31+30+31+29+31),            /* September */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+31+30+31+30+31+29+31),         /* October */
        (CSR_SCHED_SECONDS_PER_DAY) * (31+30+31+31+30+31+30+31+29+31),      /* November */
        (CSR_SCHED_SECONDS_PER_DAY) * (30+31+30+31+31+30+31+30+31+29+31)    /* December */
    }
};

static CsrBool csrSchedIsLeapYear(CsrUint8 tm_year)
{
    return ((tm_year % 4) == 0 && ((tm_year % 100) != 0 || (tm_year % 400) == 0));
}

static void csrSchedRtcVerify(TimeStructure *now)
{
    CSR_GET_TIME_ASSERT(now->tm_year <= 38, "Year out of range");
    CSR_GET_TIME_ASSERT(now->tm_mon < 12, "Month out of range");
    CSR_GET_TIME_ASSERT(now->tm_mday > 0 && now->tm_mday <= 31, "Month day out of range");
    CSR_GET_TIME_ASSERT(now->tm_hour < 24, "Hour out of range");
    CSR_GET_TIME_ASSERT(now->tm_min < 60, "Minutes out of range");
    CSR_GET_TIME_ASSERT(now->tm_sec < 60, "Seconds out of range");
}
#endif

static CsrUint32 csrSchedConvertRtcToUtc(TB_RTC_TIME_T *now)
{
     CsrUint32 seconds = now->day * CSR_SCHED_SECONDS_PER_DAY + 
                                    now->hour* CSR_SCHED_SECONDS_PER_HOUR +
                                    now->min* CSR_SCHED_SECONDS_PER_MIN +
                                    now->sec;

    return seconds;
}
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrTimeUtcGet
 *
 *  DESCRIPTION
 *      Get the current system wallclock timestamp in UTC.
 *      Specifically, if tod is non-NULL, the contents will be set to the
 *      number of seconds (plus any fraction of a second in milliseconds)
 *      since January 1st 1970.  If low is non-NULL, the contents will be
 *      set to the low 32 bit part of the current system time in microseconds, 
 *      as would have been returned by CsrTimeGet(). If high is non-NULL, the 
 *      contents will be set to the high 32 bit part of the current system
 *      time, as would be returned in the high output parameter of CsrTimeGet().
 *
 *  NOTE
 *      NULL pointers may be provided for both low and high parameters.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrTimeUtcGet(CsrTimeUtc *tod, CsrTime *low, CsrTime *high)
{
    TB_RTC_TIME_T now;

    RTC_Get(&now);

//    SCI_TraceLow("CsrTimeUtcGet: days; %d, hours; %d, min: %d, secs: %d", now.day, now.hour, now.min, now.sec);

    if (low != NULL)
    {
        *low = CsrTimeGet(high);
    }

    if (tod != NULL)
    {
        tod->sec  = csrSchedConvertRtcToUtc(&now);
//        SCI_TraceLow("CsrTimeUtcGet: secs: %u", tod->sec);        
        tod->msec = 0;
    }
}

