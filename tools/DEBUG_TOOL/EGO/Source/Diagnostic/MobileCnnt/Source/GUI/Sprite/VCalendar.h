// VCalendar.h: interface for the CVCalendar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VCALENDAR_H__3DD4EB50_A43A_43D5_98B8_31D7F320AFD3__INCLUDED_)
#define AFX_VCALENDAR_H__3DD4EB50_A43A_43D5_98B8_31D7F320AFD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VCS_MAX_BUF_LEN 1024
#include "globaldef.h"

class CVCalendar  
{
public:
	CVCalendar();
	virtual ~CVCalendar();

	BOOL Save(LPCTSTR pszFileName);
	BOOL Parse(LPCTSTR pszFileName);
	void SetValues(CStringArray &arrValues);
    int DecodeQP(const char* pSrc, unsigned char* pDst, int nSrcLen); 
	int EncodeQP(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
    void DateToString(CString &str,MEMO_DATE &memoDate);
    void StringToDate(MEMO_DATE &memoDate,CString &str);



	CString		m_strDescript;
	CString		m_strSummary;

	MEMO_DATE   m_memoStartDate;
	MEMO_DATE   m_memoEndDate;

private:
	CString		m_strStartDT;
	CString		m_strEndDT;
	FILE	   *m_pFile;

	char        m_szTempA[VCS_MAX_BUF_LEN];
	_TCHAR      m_szTempW[VCS_MAX_BUF_LEN];
	unsigned char m_szTempUA[VCS_MAX_BUF_LEN];

	static const _TCHAR VCALENDAR_BEGIN[];
	static const _TCHAR VCALENDAR_END[];

	static const _TCHAR EVENT_BEGIN[];
	static const _TCHAR EVENT_END[];

	static const _TCHAR VCALENDAR_VERSION[];

	static const _TCHAR START_DATE[];	
	static const _TCHAR END_DATE[];
	static const _TCHAR END_DATE2[];

	static const _TCHAR DESCRIPTION[];
	static const _TCHAR SUMMARY[];

	static const _TCHAR START_DATE_OPT[];
	static const _TCHAR END_DATE_OPT[];
	static const _TCHAR END_DATE2_OPT[];

	static const _TCHAR DESCRIPTION_OPT[];
	static const _TCHAR SUMMARY_OPT[];

};

#endif // !defined(AFX_VCALENDAR_H__3DD4EB50_A43A_43D5_98B8_31D7F320AFD3__INCLUDED_)
