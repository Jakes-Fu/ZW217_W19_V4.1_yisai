// Filter.h: interface for the CFilter class.
//////////////////////////////////////////////////////////////////////
#ifndef _FILTER_H_ 
#define _FILTER_H_

#include "StdAfx.h"
#include "DspKernelExport.h"
#pragma warning(push,3)
#include "vector"
#pragma  warning(pop)
#pragma warning( disable : 4018 4663)
using namespace std;

typedef struct TP_FILTER_DATA
{
    TP_FILTER_DATA(){wAddr = 0; dwStartPos = 0;};   
    TP_FILTER_DATA(WORD _wAddr, DWORD _dwStartPos){wAddr= _wAddr; dwStartPos= _dwStartPos;}; 
    
    WORD  wAddr;
    DWORD dwStartPos;    

}TP_FILTER_DATA,*PTR_TP_FILTER_DATA;

typedef vector<DWORD> DWVECTOR;

class CFilter  
{
public:
	CFilter();
	virtual ~CFilter();
    
public:      
    //set filter condition
    BOOL	  SetFilterContext(ITPSet* pTPSet);

    BOOL      StartFilter(TP_FILTER_DATA* pFilterData);
    void      PauseFilter();
    void      ResumeFIlter();
    void      StopFilter();
      
    void      Reset(){ m_lCurIdx = 0;  m_vecIdx.clear();} 

    DWORD     GetUpdate(DWVECTOR& vecTP);

    ITPSet*   GetTPSet();

private:
        BOOL CreateMonitorThread();
        void ThreadProc();
        static DWORD WINAPI MonitorThread(LPVOID pParam );  

private:
    HANDLE    m_eventThread;  
    HANDLE    m_eventPause;
    BOOL      m_bThreadExit;	     //thread-exit flag    


    DWVECTOR  m_vecIdx;
    ULONG     m_lCurIdx; 
    
    ITPSet*   m_pTPSet;    
    
    HANDLE    m_hThreadProc;
    
    TP_FILTER_DATA  m_FilterData;         
};

#endif // _FILTER_H_