// DlgEditCity.cpp : implementation file
//

#include "stdafx.h"
#include "ownership.h"
#include "DlgEditCity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CITYNAME_LENGTH			     30

/////////////////////////////////////////////////////////////////////////////
// CDlgEditCity dialog


CDlgEditCity::CDlgEditCity(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditCity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEditCity)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEditCity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditCity)
	DDX_Control(pDX, IDC_ECF_LST_CITY, m_lstCity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditCity, CDialog)
	//{{AFX_MSG_MAP(CDlgEditCity)
	ON_BN_CLICKED(IDC_ECF_BTN_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_ECF_BTN_IMPORT_TEXT, OnImportText)
	ON_BN_CLICKED(IDC_ECF_BTN_SAVE_AS, OnSaveAs)
	ON_BN_CLICKED(IDC_ECF_BTN_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditCity message handlers
BOOL CDlgEditCity::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	DWORD dwStyle = m_lstCity.GetStyle();
    dwStyle |= LVS_REPORT/* | LVS_OWNERDRAWFIXED*/;
    m_lstCity.ModifyStyle(0,dwStyle);
	m_lstCity.SetBkColor(RGB(232, 232, 232));	
	m_lstCity.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstCity.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	m_lstCity.SetExtendedStyle(dwExStyle);
	
	m_lstCity.InsertColumn(0, _T("City Name"),LVCFMT_LEFT,140);	


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditCity::OnImport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".data"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("City files (*.data)|*.dat|All files (*.*)|*.*||"));
	
	if(dlg.DoModal() == IDOK)
	{
		m_strCityFile = dlg.GetPathName();
		m_lstCity.DeleteAllItems();
		LoadCity();
	}
	
}

BOOL CDlgEditCity::LoadCity()
{
	CWaitCursor wait;

	CWaitCursor waitCur;
	CFileFind finder;
	if(!finder.FindFile(m_strCityFile))
	{
		CString strError;
		strError.Format(_T("Can not find city file [%s]"),m_strCityFile.operator LPCTSTR());
		AfxMessageBox(strError);
		return FALSE;
	}
	
	waitCur.Restore();
	
    FILE *pFile = _tfopen(m_strCityFile,_T("rb"));
	
	if(pFile == NULL)
	{
		CString strError;
		strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),m_strCityFile.operator LPCTSTR(),GetLastError());
		AfxMessageBox(strError);
		return FALSE;
	}
	
	fseek(pFile, 0, SEEK_END);
	DWORD dwSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	if(dwSize % CITYNAME_LENGTH != 0)
	{
		fclose(pFile);
		CString strError;
		strError.Format(_T("Can not load city file [%s]"),m_strCityFile.operator LPCTSTR());
		AfxMessageBox(strError);
		return FALSE;
	}
	
	DWORD dwItemCount = dwSize / CITYNAME_LENGTH;
	
	char szCityName[CITYNAME_LENGTH] = {0};
	TCHAR szCityNameW[CITYNAME_LENGTH] = {0};
	
	for (int i=0;i < (int)dwItemCount;i++)
	{
		memset(szCityName,0,CITYNAME_LENGTH);		
		dwSize = fread(szCityName,1,CITYNAME_LENGTH,pFile);
		
#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,szCityName,CITYNAME_LENGTH,szCityNameW,CITYNAME_LENGTH);
		m_lstCity.InsertItem(i,szCityNameW);	
#else		
		m_lstCity.InsertItem(i,szCityName);
#endif

	}
	
	fclose(pFile);	

	
	return TRUE;
}

void CDlgEditCity::OnImportText() 
{
	// TODO: Add your control notification handler code here
	m_lstCity.DeleteAllItems();
	CFileDialog dlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("City files (*.txt)|*.txt||"));
	
	if(dlg.DoModal() == IDCANCEL)
	{
		 return;
	}

	CString strFile = dlg.GetPathName();

	CString strLine;

	CStringArray agString;
	int nIdx= 0;

	char szCityName[CITYNAME_LENGTH*2] = {0};
	TCHAR szCityNameW[CITYNAME_LENGTH] = {0};

// 	CStdioFile file;
// 	file.Open(strFile,CFile::modeRead | CFile::shareDenyWrite,NULL);
// 
// 	while(file.ReadString(szCityNameW,CITYNAME_LENGTH)!=NULL)
// 	{
// #ifdef _UNICODE
// 		memcpy(szCityName,szCityNameW,CITYNAME_LENGTH*2);
// 		MultiByteToWideChar(CP_ACP,0,szCityName,CITYNAME_LENGTH,szCityNameW,CITYNAME_LENGTH);
// 		m_lstCity.InsertItem(nIdx++,szCityNameW);	
// #else		
// 		m_lstCity.InsertItem(nIdx++,szCityNameW);
// #endif
// 
// 	}

// 	file.Close();

	FILE *pFile =  _tfopen(strFile,_T("rt"));

	while(fgets(szCityName,CITYNAME_LENGTH*2,pFile) != NULL)
	{
		memset(szCityNameW,0,sizeof(szCityNameW));
#ifdef _UNICODE	
		MultiByteToWideChar(CP_ACP,0,szCityName,strlen(szCityName),szCityNameW,CITYNAME_LENGTH);
#endif 
		CString str = szCityNameW;
		str.TrimRight(_T("\r\n"));
		str.TrimRight(_T("\n"));
		if(str.GetLength()== 0)
		{
			memset(szCityName,0,sizeof(szCityName));
			continue;
		}
		m_lstCity.InsertItem(nIdx++,str);

		memset(szCityName,0,sizeof(szCityName));

	}

	fclose(pFile);
	
	if(m_lstCity.GetItemCount() > 0)
	{
		AfxMessageBox(_T("Import complete!"));
	}
	else
	{
		AfxMessageBox(_T("No city is imported!"));
	}


}

void CDlgEditCity::OnSaveAs() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstCity.GetItemCount();

	if(nCount == 0)
	{
		AfxMessageBox(_T("There is no cities!"));
		return;
	}
	CFileDialog dlg(FALSE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("City files (*.txt)|*.txt||"));
	
	if(dlg.DoModal() == IDCANCEL)
	{
		 return;
	}

	CString strFile = dlg.GetPathName();

	DWORD dwAtt = ::GetFileAttributes(strFile);
	dwAtt &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strFile,dwAtt);

	char szCityName[CITYNAME_LENGTH] = {0};
	
	FILE *pFile = _tfopen(strFile,_T("wt"));
	for(int i=0; i<nCount; i++)
	{
		CString str = m_lstCity.GetItemText(i,0);
		memset(szCityName,0,sizeof(szCityName));

#ifdef _UNICODE		
		WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),szCityName,CITYNAME_LENGTH,NULL,NULL);
#else		
		strcpy(szCityName,str);
#endif	
		fprintf(pFile,"%s\r\n",szCityName);
	}

	fclose(pFile);

	AfxMessageBox(_T("Export complete!"));
	
}

void CDlgEditCity::OnSave() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstCity.GetItemCount();

	if(nCount == 0)
	{
		AfxMessageBox(_T("There is no cities!"));
		return;
	}
	CFileDialog dlg(FALSE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("City files (*.dat)|*.dat|All files (*.*)|*.*||"));
	
	if(dlg.DoModal() == IDCANCEL)
	{
		 return;
	}

	CString strFile = dlg.GetPathName();

	DWORD dwAtt = ::GetFileAttributes(strFile);
	dwAtt &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strFile,dwAtt);

	char szCityName[CITYNAME_LENGTH] = {0};
	
	FILE *pFile = _tfopen(strFile,_T("wb"));
	for(int i=0; i<nCount; i++)
	{
		memset(szCityName,0,CITYNAME_LENGTH);
		CString str = m_lstCity.GetItemText(i,0);
#ifdef _UNICODE		
		WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),szCityName,CITYNAME_LENGTH,NULL,NULL);
#else		
		strcpy(szCityName,str);
#endif	
		fwrite(szCityName,CITYNAME_LENGTH,1,pFile);
	}

	fclose(pFile);

	AfxMessageBox(_T("Save complete!"));

}
