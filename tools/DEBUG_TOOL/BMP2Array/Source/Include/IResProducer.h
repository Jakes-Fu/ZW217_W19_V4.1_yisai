#if !defined(_IRESPRODUCER_H_)
#define _IRESPRODUCER_H_

#pragma once

#include "SciDllExport.h"
#include "mmi_resource_def.h"
#include "GlobalDef.h"

class IResProducer
{
public:
	/* 
	 * Init the parameters for resource producer
	 * @param lpszBinFileName: the name of BinFile
	 * @param nModuleCount:the resource module number
	 * @param bCover: when "lpszBinFileName" file is existed,if true, cover it,else return X_FALSE
	 * @param pLangInfo: point to X_LANG_INFO_T struct
	 * @param dwFlag: bit0, 1,string has no flag; 0,string has flag (2 bytes)
	 * @return 
	 */
    virtual HRESULT Init(LPCTSTR lpszBinFileName,int nModuleCount, MMI_MODULE_VER_INFO_T &mvt,BOOL bCover = TRUE, PX_LANG_INFO_T pLangInfo = NULL,DWORD dwFlag = 0, int nLangId = -1) =0;	
	/* 
	 * Add resource module information
	 * @param pagResInfo: a ptr point to an array of X_RESOURCE_INFO_T
	 * @param nResCount: number of  X_RESOURCE_INFO_T in the pagResInfo
	 * @param nLangCount: the number of languages 
	 * @param pMmiRes: a ptr point to MMI_RESOURCE_T
	 * @return 
	 */   
	virtual HRESULT AddResModuleInfo( LPVOID pagResInfo, int nResCount,int nLangCount,PMMI_RESOURCE_T pMmiRes) = 0;
    /* 
	 * Save resource module information to MSXML dom 
	 * @return 
	 */   
	virtual HRESULT SaveResModule(BOOL bCheck = TRUE)=0;
    /* 
	 * Save to *.xrm file
	 * @return 
	 */   
	virtual HRESULT Close()=0;
    /* 
	 * Release this interface
	 * @return 
	 */   
	virtual void    Release()=0;

	/* 
	 * Set the window handle to receive the progress message
	 * @param hWnd: the handle to receive the progress message
	 * @return 
	 */
    virtual HRESULT SetWndForProgress( HWND hWnd ) = 0;
	/* 
	 * Set the UIThread to receive the progress message
	 * @param pThread: the ptr UIThread of  to receive the progress message
	 * @return 
	 */
	virtual HRESULT SetUIThreadForProgress(CWinThread * pThread) = 0;

	virtual ~IResProducer()=0;
};

SCIDLL_EXPORT_API BOOL CreateResProducer ( IResProducer ** ppResProducer, UINT uPlatform = 0);

#endif //_IRESPRODUCER_H_