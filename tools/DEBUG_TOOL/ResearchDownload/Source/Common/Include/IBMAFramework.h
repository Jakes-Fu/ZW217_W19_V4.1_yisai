#ifndef _IBMAFRAMEWORK_H__
#define _IBMAFRAMEWORK_H__

#include "BMAFExport.h"
#include "BootModeitf.h"

class IBMAFramework
{
public:
    virtual ~IBMAFramework()=0;
	virtual HRESULT BMAF_StartOneWork(LPCTSTR lpProductName, 
							LPCTSTR *ppFileList,
							DWORD  dwFileCount,
							LPVOID  pOpenArgument,
							BOOL bBigEndian, 
							DWORD dwOprCookie,
							BOOL bRcvThread, 
							LPCVOID pReceiver,
							LPCWSTR lpbstrProgID )=0;
	virtual HRESULT BMAF_StopAllWork()=0;
	virtual HRESULT BMAF_StopOneWork(DWORD dwOprCookie)=0;
	virtual HRESULT BMAF_GetBootModeObjInfo(DWORD dwOprCookie,LPVOID* ppBootModeObjT)=0;
	virtual HRESULT BMAF_RegisterBMPDll(LPCTSTR lpstrDllName)=0;
	virtual HRESULT BMAF_UnregisterBMPDll(LPCTSTR lpstrDllName)=0;
	virtual HRESULT BMAF_SubscribeObserver( DWORD dwOprCookie,
											IBMOprObserver* pSink, 
											ULONG uFlags )=0;
	virtual HRESULT BMAF_UnsubscribeObserver(DWORD dwOprCookie)=0;
	virtual HRESULT BMAF_SetProperty(DWORD dwPropertyID, LPCVOID pPropertyValue) = 0;
	virtual HRESULT BMAF_GetProperty(DWORD dwPropertyID, LPVOID*  lpPropertyValue) = 0;	
	virtual HRESULT BMAF_SetCommunicateChannelPtr(DWORD dwOprCookie,LPVOID pCommunicateChannel ) = 0;
	virtual HRESULT BMAF_GetCommunicateChannelPtr(DWORD dwOprCookie, /*[out]*/LPVOID* ppCommunicateChannel ) =0 ;
	virtual HRESULT BMAF_Release()=0;
	
};

BMAF_EXPORT_API BOOL CreateBMAFramework( IBMAFramework ** pBMAFramework );

#endif //_IBMAFRAMEWORK_H__