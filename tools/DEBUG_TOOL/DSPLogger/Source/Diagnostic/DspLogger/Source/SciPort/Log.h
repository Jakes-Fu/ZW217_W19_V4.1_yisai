// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__EF7C3385_4C4F_41BF_8268_0B4C60C30FBA__INCLUDED_)
#define AFX_LOG_H__EF7C3385_4C4F_41BF_8268_0B4C60C30FBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define LOG_MAX_BUF_LEN 512
class CLog  
{
public:
    BOOL Open( LPCTSTR pszFileName );

    BOOL Log( LPVOID pBuffer, int nBufLen );
    BOOL LogEx( LPCTSTR pszFormat, ... );

    void Close( void );

public:
	CLog();
	 ~CLog();
private:

    HANDLE m_hFile;

    static const int MAX_BUF_LEN;
	char m_szBufA[LOG_MAX_BUF_LEN];
	_TCHAR m_szBufW[LOG_MAX_BUF_LEN];
};

#endif // !defined(AFX_LOG_H__EF7C3385_4C4F_41BF_8268_0B4C60C30FBA__INCLUDED_)
