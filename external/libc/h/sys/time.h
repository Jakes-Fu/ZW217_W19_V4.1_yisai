#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__


#include <sys/types.h>
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif


#ifndef _TIME_T_DEFINED
#define _TIME_T_DEFINED
typedef unsigned int time_t;
#endif

#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED
/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
    long    tv_sec;     /* seconds */
    long    tv_usec;    /* and microseconds */
};
#endif /* _TIMEVAL_DEFINED */


#ifndef _TIMESPEC_DEFINED	 
#define _TIMESPEC_DEFINED	 
/*	 
* Structure defined by POSIX.1b to be like a timeval.	 
*/	 
struct timespec {	 
   time_t  tv_sec;     /* seconds */	 
   long    tv_nsec;    /* and nanoseconds */	 
};	 
#endif /* _TIMESPEC_DEFINED */ 



#define timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define timercmp(tvp,uvp,cmp)					\
		((tvp)->tv_sec cmp (uvp)->tv_sec ||		\
		 ((tvp)->tv_sec == (uvp)->tv_sec && (tvp)->tv_usec cmp (uvp)->tv_usec))
#define timerclear(tvp)		(tvp)->tv_sec = (tvp)->tv_usec = 0




// ----time.h


#ifndef _TIMEZONE_DEFINED /* also in sys/time.h */
#define _TIMEZONE_DEFINED
/* Provided for compatibility with code that assumes that
   the presence of gettimeofday function implies a definition
   of struct timezone. */
struct timezone
{
  int tz_minuteswest; /* of Greenwich */
  int tz_dsttime;     /* type of dst correction to apply */
};

extern int  mingw_gettimeofday (struct timeval *p, struct timezone *z);

#endif /* _TIMEZONE_DEFINED */

/*
   Implementation as per:
   The Open Group Base Specifications, Issue 6
   IEEE Std 1003.1, 2004 Edition

   The timezone pointer arg is ignored.  Errors are ignored.
*/
#ifndef _GETTIMEOFDAY_DEFINED
#define _GETTIMEOFDAY_DEFINED
int  gettimeofday(struct timeval *__restrict__,
                         void *__restrict__  /* tzp (unused) */);
#endif

void timeradd(struct timeval *a , struct timeval *b , struct timeval *res);

void timersub(struct timeval *a , struct timeval *b , struct timeval *res);

/* Get current value of clock CLOCK_ID and store it in TP.  */
// extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp);

/* Set clock CLOCK_ID to value TP.  */
// extern int clock_settime (clockid_t __clock_id, const struct timespec *__tp);


#ifdef __cplusplus
}
#endif



#endif