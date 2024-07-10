// TimerHelper.cpp: implementation of the CTimerHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimerHelper.h"
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  HIGH_PRECIOUS_TIME

#ifdef HIGH_PRECIOUS_TIME
#include <Windows.h>

inline double __fastcall usTimeCounter(bool bStart)
{
	double dResult = 0.0f;
	static LARGE_INTEGER tFrep;
	static LARGE_INTEGER tBegin;
	static bool bInit = false;
	LARGE_INTEGER tEnd;
	if(!bInit)
	{
		QueryPerformanceFrequency(&tFrep);
		bInit = true;
	}
	
	if(bStart)
	{
		QueryPerformanceCounter(&tBegin);
		return dResult;
	}
	else
	{
		QueryPerformanceCounter(&tEnd);
	}
	
	dResult = ((double)(tEnd.QuadPart - tBegin.QuadPart))/(double)tFrep.QuadPart;
	
	return dResult;
}
#endif

//*******************************************************//
// CTimerHelper implement
//*******************************************************//
void CALLBACK TimerHelperProc(UINT wTimerID, UINT msg, 
    DWORD dwUser, DWORD dw1, DWORD dw2) 
{
    CTimerHelper* pTimer = (CTimerHelper*)dwUser;
    ++(pTimer->m_lTickCount);
}

// Start timer event
void CTimerHelper::BeginTimer()
{
#ifdef HIGH_PRECIOUS_TIME
	usTimeCounter(true);
#else
    if( m_uEventId == 0 )
    {
        m_uEventId = ::timeSetEvent(1,1,TimerHelperProc,(DWORD)this,TIME_PERIODIC);
    }
#endif

}

// Stop timer event
void CTimerHelper::EndTimer()
{	
#ifdef HIGH_PRECIOUS_TIME
	usTimeCounter(true);
#else
	if( 0 != m_uEventId )
	{
		::timeKillEvent(m_uEventId);
		m_uEventId = 0;
	}
#endif
}

long  CTimerHelper::GetTickCount()
{
#ifdef HIGH_PRECIOUS_TIME
	m_lTickCount = (long)(usTimeCounter(false)*1000);
	
#endif
	return m_lTickCount;
}