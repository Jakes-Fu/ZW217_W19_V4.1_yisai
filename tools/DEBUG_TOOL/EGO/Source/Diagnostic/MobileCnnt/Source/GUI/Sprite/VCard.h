// VCard.h: interface for the CVCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VCARD_H__4F1D5788_681A_4F4D_A388_932DA905CD1F__INCLUDED_)
#define AFX_VCARD_H__4F1D5788_681A_4F4D_A388_932DA905CD1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct _PB;

#define  VC_MAX_BUF_LEN 1024

class CVCard  
{
public:
	CVCard();
	virtual ~CVCard();

	BOOL Save(LPCTSTR pszFileName);
	BOOL Parse(LPCTSTR pszFileName);
	void SetValues(CStringArray &arrValues);
	void ConvertToPB(_PB * pPB);
	int  DecodeQP(const char* pSrc, unsigned char* pDst, int nSrcLen);


	CString		m_strFullName;

	CString		m_strTelWorkVoice;
	CString		m_strTelHomeVoice;
	CString		m_strTelCellVoice;	
	CString     m_strTelWorkFax;

	CString		m_strEmailPrefInternet;

	CString     m_strRev;

private:
	FILE  *		m_pFile;
    char        m_szTempA[VC_MAX_BUF_LEN];
	_TCHAR      m_szTempW[VC_MAX_BUF_LEN];
	
	static const _TCHAR VCARD_BEGIN[];
	static const _TCHAR VCARD_END[];

	static const _TCHAR VCARD_VERSION[];
	static const _TCHAR VCARD_REV[];

	static const _TCHAR NAME1[];
	static const _TCHAR NAME2[];
	static const _TCHAR NAME3[];
	static const _TCHAR FULL_NAME[];
	static const _TCHAR FULL_NAME2[];

	static const _TCHAR TEL_WORK_VOICE[];
	static const _TCHAR TEL_HOME_VOICE[];
	static const _TCHAR TEL_CELL_VOICE[];
	static const _TCHAR TEL_WORK_FAX[];

	static const _TCHAR TEL_WORK_VOICE2[];
	static const _TCHAR TEL_HOME_VOICE2[];
	static const _TCHAR TEL_CELL_VOICE2[];


	static const _TCHAR EMAIL_PREF_INTERNET[];


};

#endif // !defined(AFX_VCARD_H__4F1D5788_681A_4F4D_A388_932DA905CD1F__INCLUDED_)
