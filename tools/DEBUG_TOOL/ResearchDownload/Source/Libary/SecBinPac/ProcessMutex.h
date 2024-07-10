#pragma once

class CProcessMutex
{
public:
	CProcessMutex(const char* pMutexName = NULL);
	virtual ~CProcessMutex(void);
	BOOL Lock();
	BOOL UnLock();
private:
	HANDLE m_hMutex;
	CString m_strMutexName;
};
