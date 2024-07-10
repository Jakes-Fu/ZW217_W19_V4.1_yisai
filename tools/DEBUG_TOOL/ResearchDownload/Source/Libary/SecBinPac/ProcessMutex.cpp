#include "stdafx.h"
#include "ProcessMutex.h"
#include <atlconv.h>

CProcessMutex::CProcessMutex(const char* pMutexName/* = NULL*/)
{
	USES_CONVERSION;
	if (NULL == pMutexName)
	{
		m_strMutexName = _T("SecBinPac");
	}
	else
	{
		m_strMutexName = A2T(pMutexName);
	}
	
	m_hMutex = CreateMutex(NULL, FALSE, m_strMutexName.operator LPCTSTR());
}

CProcessMutex::~CProcessMutex(void)
{
	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
}

BOOL CProcessMutex::Lock()
{
	BOOL bOK = FALSE;

	if (m_hMutex && WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex, INFINITE))
	{
		bOK = TRUE;
	}

	return bOK;
}

BOOL CProcessMutex::UnLock()
{
	BOOL bOK = FALSE;
	if (m_hMutex)
	{
		bOK = ReleaseMutex(m_hMutex);
	}
	return bOK;
}
