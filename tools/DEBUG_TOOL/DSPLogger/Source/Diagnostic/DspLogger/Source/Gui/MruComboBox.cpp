// MruComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MruComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const unsigned int cMaxMruCount = 4;
const char szSection[] = "MRU ComboBox Section";
const char szEntry[] = "MRU ComboBox Entry %d";

/////////////////////////////////////////////////////////////////////////////
// CMruComboBox

CMruComboBox::CMruComboBox()
{
    m_uMaxMruCount = cMaxMruCount;
    m_strMruSection = szSection;
    m_strMruEntryFormat = szEntry;
}

CMruComboBox::~CMruComboBox()
{
}


BEGIN_MESSAGE_MAP(CMruComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMruComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMruComboBox message handlers

// Save MRU list
BOOL CMruComboBox::SaveMruList()
{
	//polo.jiang change cfg from reg to ini

	CString strEntry;
	CString strText;
	BOOL bRet = TRUE;
	TCHAR szCfgFile[_MAX_PATH] = {0};

    GetModuleFileName(NULL,szCfgFile,_MAX_PATH);
	TCHAR* p=_tcsrchr(szCfgFile,_T('.'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	_tcscat(szCfgFile,_T(".ini"));

	for(int i=0;i<GetCount();i++)
    {
		strEntry.Format(m_strMruEntryFormat,i);
		GetLBText(i,strText);

		bRet = WritePrivateProfileString(m_strMruSection.operator LPCTSTR(),strEntry.operator LPCTSTR(),
			strText.operator LPCTSTR(),szCfgFile);
		if(!bRet)
		{
			break;
		}
    }
	
	/*
	CWinApp* pApp = AfxGetApp();
	
	CString strEntry;
	CString strText;
	BOOL bRet = TRUE;
	
    for(int i=0;i<GetCount();i++)
    {
		strEntry.Format(m_strMruEntryFormat,i);
		GetLBText(i,strText);
		bRet = pApp->WriteProfileString(m_strMruSection,strEntry,strText);
		if(!bRet)
		{
			break;
		}
    }
	
	*/
    return bRet;
}

// Load MRU list
BOOL CMruComboBox::LoadMruList()
{

	//polo.jiang change cfg from reg to ini
	CString strEntry;
    CString strText;
    BOOL bRet = TRUE;
	TCHAR szCfgFile[_MAX_PATH] = {0};
	TCHAR szValue[_MAX_PATH] = {0};
    GetModuleFileName(NULL,szCfgFile,_MAX_PATH);
	TCHAR* p=_tcsrchr(szCfgFile,_T('.'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	_tcscat(szCfgFile,_T(".ini"));


	
    for(unsigned int i=0;i<m_uMaxMruCount;i++)
    {
        strEntry.Format(m_strMruEntryFormat,i);

		GetPrivateProfileString(m_strMruSection,strEntry,_T(""),szValue, _MAX_PATH, szCfgFile);
		strText = szValue;
		
        if(strText.GetLength() == 0)
        {
            continue;
        }
		
        AddString(strText);
    }

    /*CWinApp* pApp = AfxGetApp();

    CString strEntry;
    CString strText;
    BOOL bRet = TRUE;

    for(unsigned int i=0;i<m_uMaxMruCount;i++)
    {
        strEntry.Format(m_strMruEntryFormat,i);
        strText = pApp->GetProfileString(m_strMruSection,strEntry);

        if(strText.GetLength() == 0)
        {
            continue;
        }

        AddString(strText);
    }*/
    
    return bRet;
}

// Add current string to mru list
BOOL CMruComboBox::Add2MruList()
{
    // Get current string
    CString strText;
    GetWindowText(strText);

    return Add2MruList(strText);
}

// Add a string to mru list
BOOL CMruComboBox::Add2MruList(CString& strText)
{
    // If the string be found in the list
    // move to the first place,or insert 
    // to the head.
    int nIndex = FindString(0,strText);
    if(nIndex != -1)
    {
        DeleteString(nIndex);
    }
    InsertString(0,strText);
    SetCurSel(0);
    
    unsigned int nCount = GetCount();
    if(nCount > m_uMaxMruCount)
    {
        DeleteString(nCount-1);
    }

    return TRUE;
}
