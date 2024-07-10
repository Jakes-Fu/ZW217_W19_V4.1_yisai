#define _MV_DATE_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include "mv_debug.h"
#include "sci_api.h"
#include "mv_date_api.h"
#include "Dal_time.h"
#include "mmicom_time.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/                                     

    
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : CM_GetCurrentTime
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_TIME CM_GetCurrentTime(void)
{
	_TIME time_sec = 0;
    SCI_DATE_T  sys_date = {0};
    SCI_TIME_T  time_t = {0};
	
	if(ERR_TM_NONE == TM_GetSysDate(&sys_date))
	{
#ifdef DATE_DEBUG
	   //MV_TRACE_LOW:"[MV]--WD[CM_GetCurrentTime]mday=%d,mon=%d,year=%d,wday=%d"
	   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_DATE_API_50_112_2_18_2_39_11_22,(uint8*)"dddd",sys_date.mday,sys_date.mon,sys_date.year,sys_date.wday);
#endif
    }
	if(ERR_TM_NONE == TM_GetSysTime( &time_t))
	{
#ifdef DATE_DEBUG
	    //MV_TRACE_LOW:"[MV]--WD[CM_GetCurrentTime]sec=%d,min=%d,hour=%d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_DATE_API_56_112_2_18_2_39_11_23,(uint8*)"ddd",time_t.sec,time_t.min,time_t.hour);
#endif
    }    

	time_sec = MMIAPICOM_Tm2Second(time_t.sec,
        time_t.min,
        time_t.hour,
        sys_date.mday,
        sys_date.mon,
        sys_date.year);
#ifdef DATE_DEBUG    
    //MV_TRACE_LOW:"[MV]--WD[CM_GetCurrentTime] TM_GetSysTime time_sec = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_DATE_API_67_112_2_18_2_39_11_24,(uint8*)"d", time_sec);
#endif    
	return time_sec;
}

/*****************************************************************************/
//  Description : CM_GetSystemTime
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_GetSystemTime(_TIME nTime,CM_SYSTIME* pSysTime)
{
     MMI_TM_T  time_t = {0};
     /**/
     //nTime = nTime + (365 * 5 + 366 * 15) * (24 * 3600);
     time_t = MMIAPICOM_Second2Tm(nTime);
     pSysTime->wYear = (_WORD)time_t.tm_year;
     pSysTime->wMonth = time_t.tm_mon;
     pSysTime->wDay = time_t.tm_mday;

     pSysTime->wHour = time_t.tm_hour;
     pSysTime->wMinute = time_t.tm_min;
     pSysTime->wSecond = time_t.tm_sec;

     pSysTime->wDayOfWeek = time_t.tm_wday;
     pSysTime->wDayOfYear = (_WORD)time_t.tm_yday;
#ifdef DATE_DEBUG     
     //MV_TRACE_LOW:"[MV]--WD[CM_GetSystemTime]wYear=%d,wMonth=%d,wDay=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_DATE_API_93_112_2_18_2_39_11_25,(uint8*)"ddd",time_t.tm_year,time_t.tm_mon,time_t.tm_mday);
     //MV_TRACE_LOW:"[MV]--WD[CM_GetSystemTime]wHour=%d,wMinute=%d,wDay=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_DATE_API_94_112_2_18_2_39_11_26,(uint8*)"ddd",time_t.tm_hour,time_t.tm_min,time_t.tm_sec);
     //MV_TRACE_LOW:"[MV]--WD[CM_GetSystemTime]wDayOfWeek=%d,wDayOfYear=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_DATE_API_95_112_2_18_2_39_11_27,(uint8*)"dd",time_t.tm_wday,time_t.tm_yday);
#endif
     return _TRUE;
}

/*****************************************************************************/
//  Description : CM_GetTickTime
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_UINT32 CM_GetTickTime()
{
     return SCI_GetTickCount();
	
}


