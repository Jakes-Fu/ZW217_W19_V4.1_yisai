// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__EF7C3385_4C4F_41BF_8268_0B4C60C30FBA__INCLUDED_)
#define AFX_LOG_H__EF7C3385_4C4F_41BF_8268_0B4C60C30FBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLog  
{
public:
    BOOL Open( LPCTSTR pszFileName );
    BOOL Log( LPVOID pBuffer, int nBufLen );
    BOOL LogEx( LPCTSTR pszFormat, ... );
    void Close( void );
/////////////////////////////////////////////
    BOOL OpenLog();
	void XLog(LPCTSTR lpszLog);
    void CloseLog( void );
	BOOL IsOpenLog();

public:
	CLog();
	 ~CLog();
private:

    HANDLE m_hFile;
	FILE   *m_pLogFile;

    static const int MAX_BUF_LEN;
};

#endif // !defined(AFX_LOG_H__EF7C3385_4C4F_41BF_8268_0B4C60C30FBA__INCLUDED_)
