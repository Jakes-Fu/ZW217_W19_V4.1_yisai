// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Filter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFilter::CFilter()
{
    m_pTPSet       = NULL;
    m_bThreadExit  = FALSE;
    m_hThreadProc  = NULL;
    m_lCurIdx      = 0;

    m_eventThread  = ::CreateEvent(NULL,TRUE,TRUE,NULL);
    m_eventPause   = ::CreateEvent(NULL,TRUE,TRUE,NULL);

    m_vecIdx.reserve(10000);
}


CFilter::~CFilter()
{
    StopFilter();
    
    Sleep(200);    
    if (NULL != m_hThreadProc)
    {
        TerminateThread(m_hThreadProc, (ULONG)-1);           
    }
    
    CloseHandle(m_hThreadProc);
    m_hThreadProc = NULL;  

    CloseHandle(m_eventThread);   
    CloseHandle(m_eventPause);
}

BOOL CFilter::SetFilterContext(ITPSet* pTPSet)
{
    ASSERT(NULL != pTPSet);
    m_pTPSet= pTPSet;    
    return TRUE;
}

BOOL CFilter::StartFilter(TP_FILTER_DATA* pFilterData )
{
    ASSERT(NULL != pFilterData);

    if (NULL == pFilterData) return FALSE;
          
    m_FilterData  = *pFilterData;
    m_lCurIdx     = 0;

    CreateMonitorThread();
    
    return TRUE;
}

void CFilter::PauseFilter()
{
    ::ResetEvent(m_eventPause);
}

void CFilter::ResumeFIlter()
{
    ::SetEvent(m_eventPause);
}

void CFilter::StopFilter()
{
    m_bThreadExit = TRUE; 
}


DWORD CFilter::GetUpdate(DWVECTOR& vecTP)
{        
   DWORD dwSize=  m_vecIdx.size();
   if(0 == dwSize)   return 0;
   
   ::ResetEvent(m_eventThread);

   //copy data 
   DWORD i=0;
   for(i=0; i< m_vecIdx.size(); i++)
   {
        DWORD dwIdx= m_vecIdx.at(i);
        vecTP.push_back(dwIdx);
   }

   m_vecIdx.clear();
   
   ::SetEvent(m_eventThread);

   return i;
}

ITPSet* CFilter::GetTPSet()
{
    return m_pTPSet;
}

BOOL CFilter::CreateMonitorThread()
{
    m_hThreadProc = CreateThread( 
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)MonitorThread,
        this,
        0,
        NULL);
    
    if( m_hThreadProc == NULL ) 
        return FALSE;

    return TRUE;
}

DWORD CFilter:: MonitorThread(LPVOID pParam )
{
    CFilter* pFilter= (CFilter*)pParam;
    ASSERT(NULL != pFilter);

    pFilter->ThreadProc();
    
    return 0L;
}

void CFilter::ThreadProc()
{
    while (!m_bThreadExit)
    {
        //pause thread
        WaitForSingleObject(m_eventPause, INFINITE);
        
        while (m_lCurIdx <  m_pTPSet->GetTpCount())
        {
            if(m_lCurIdx >= m_FilterData.dwStartPos)
            {                
                WORD addr= m_pTPSet->GetTPAddr(m_lCurIdx);
                if (m_FilterData.wAddr == addr)
                {
                    //wait until copy data finished and resume thread
                    WaitForSingleObject(m_eventThread,INFINITE);
                    m_vecIdx.push_back(m_lCurIdx);               
                }
            }
            m_lCurIdx++;            
        }  

        //important !!! 
        Sleep(100);
    }
}