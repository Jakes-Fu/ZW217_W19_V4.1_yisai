// TimerHelper.h: interface for the CTimerHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMERHELPER_H__04B87717_C4A6_4934_A496_A84A46488A60__INCLUDED_)
#define AFX_TIMERHELPER_H__04B87717_C4A6_4934_A496_A84A46488A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning( disable :4201 4245 )
//***********************************************//
// CTimerHelper declarions
//***********************************************//


class CTimerHelper
{
public:
    long  m_lTickCount;
    UINT  m_uEventId;
	
public:
    CTimerHelper()
    {
        m_lTickCount = 0;
        m_uEventId   = 0;
    }
	
    long GetTickCount();
	
    // Start timer event
    void BeginTimer();
	
    // Stop timer event
    void EndTimer();

protected:
    void ClearTickCount()
    {
        m_lTickCount = 0;
    }	
};

#endif // !defined(AFX_TIMERHELPER_H__04B87717_C4A6_4934_A496_A84A46488A60__INCLUDED_)
