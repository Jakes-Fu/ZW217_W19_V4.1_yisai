// Storage.h: interface for the CStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STORAGE_H__0B007901_6A59_4FE1_B649_6A28987A926F__INCLUDED_)
#define AFX_STORAGE_H__0B007901_6A59_4FE1_B649_6A28987A926F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInfoWrapper;
class CStorage  
{
public:
	
	BOOL ReadPresetSMSFromFile(LPCTSTR pszFileName);
	BOOL SavePresetSMSToFile(LPCTSTR pszFileName);

	BOOL ReadMSFromFile(LPCTSTR pszFileName);
	BOOL SaveMSToFile(LPCTSTR pszFileName);
	CStorage();
	~CStorage();

protected:
	BOOL WriteMemo(void);
	BOOL WritePB(void);
	BOOL WriteSMS(void);

	BOOL ReadMemo(void);
	BOOL ReadPB(void);
	BOOL ReadSMS(void);

private:
	//int m_nPresetSMSCount;
	int m_nSMSCount;
	int m_nPBCount;
	int m_nMemoCount;
	int m_nSchdlCount;

	CFile m_file;
	CInfoWrapper * m_pIW;

	static const _TCHAR STORAGE_FLAG[];
	static const _TCHAR STORAGE_PRESETSMS_FLAG[];
	
	static const _TCHAR STORAGE_FLAG_V1[];
};

#endif // !defined(AFX_STORAGE_H__0B007901_6A59_4FE1_B649_6A28987A926F__INCLUDED_)
