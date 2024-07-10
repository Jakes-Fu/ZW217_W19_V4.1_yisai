// DlgBkmk.cpp : implementation file
//

#include "stdafx.h"
#include "ResOver.h"
#include "DlgBkmk.h"
#include <AFXTEMPL.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBkmk dialog


CDlgBkmk::CDlgBkmk(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBkmk::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBkmk)
	m_strPos = _T("");
	m_strUsed = _T("");
	m_strName1 = _T("");
	m_strName10 = _T("");
	m_strName2 = _T("");
	m_strName3 = _T("");
	m_strName4 = _T("");
	m_strName5 = _T("");
	m_strName6 = _T("");
	m_strName7 = _T("");
	m_strName8 = _T("");
	m_strName9 = _T("");
	m_strURL1 = _T("");
	m_strURL10 = _T("");
	m_strURL2 = _T("");
	m_strURL3 = _T("");
	m_strURL4 = _T("");
	m_strURL5 = _T("");
	m_strURL6 = _T("");
	m_strURL7 = _T("");
	m_strURL8 = _T("");
	m_strURL9 = _T("");
	for(int i = 0; i < 10; i++)
	{
		m_nMnc[i] = 0;
		m_nMcc[i] = 0;
	}


	//}}AFX_DATA_INIT

	m_nCurIndex = 0;
	m_strBKMK = _T("");
	m_strTmpPath = _T("");
	m_bViewOnly = FALSE;

}


void CDlgBkmk::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBkmk)
	DDX_Text(pDX, IDC_DBS_STC_POS, m_strPos);
	DDX_Text(pDX, IDC_DBS_STC_USED, m_strUsed);
		DDX_Text(pDX, IDC_BMK_EDT_MCC1, m_nMcc[0]);
	DDV_MinMaxUInt(pDX, m_nMcc[0], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC1, m_nMnc[0]);
	DDV_MinMaxUInt(pDX, m_nMnc[0], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC2, m_nMcc[1]);
	DDV_MinMaxUInt(pDX, m_nMcc[1], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC2, m_nMnc[1]);
	DDV_MinMaxUInt(pDX, m_nMnc[1], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC3, m_nMcc[2]);
	DDV_MinMaxUInt(pDX, m_nMcc[2], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC3, m_nMnc[2]);
	DDV_MinMaxUInt(pDX, m_nMnc[2], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC4, m_nMcc[3]);
	DDV_MinMaxUInt(pDX, m_nMcc[3], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC4, m_nMnc[3]);
	DDV_MinMaxUInt(pDX, m_nMnc[3], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC5, m_nMcc[4]);
	DDV_MinMaxUInt(pDX, m_nMcc[4], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC5, m_nMnc[4]);
	DDV_MinMaxUInt(pDX, m_nMnc[4], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC6, m_nMcc[5]);
	DDV_MinMaxUInt(pDX, m_nMcc[5], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC6, m_nMnc[5]);
	DDV_MinMaxUInt(pDX, m_nMnc[5], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC7, m_nMcc[6]);
	DDV_MinMaxUInt(pDX, m_nMcc[6], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC7, m_nMnc[6]);
	DDV_MinMaxUInt(pDX, m_nMnc[6], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC8, m_nMcc[7]);
	DDV_MinMaxUInt(pDX, m_nMcc[7], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC8, m_nMnc[7]);
	DDV_MinMaxUInt(pDX, m_nMnc[7], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC9, m_nMcc[8]);
	DDV_MinMaxUInt(pDX, m_nMcc[8], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC9, m_nMnc[8]);
	DDV_MinMaxUInt(pDX, m_nMnc[8], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MCC10, m_nMcc[9]);
	DDV_MinMaxUInt(pDX, m_nMcc[9], 0, 65535);
		DDX_Text(pDX, IDC_BMK_EDT_MNC10, m_nMnc[9]);
	DDV_MinMaxUInt(pDX, m_nMnc[9], 0, 65535);

	DDX_Text(pDX, IDC_DBS_EDT_NAME1, m_strName1);
	DDV_MaxChars(pDX, m_strName1, 16);
	DDX_Text(pDX, IDC_DBS_EDT_NAME10, m_strName10);
	DDV_MaxChars(pDX, m_strName10, 16);
	DDX_Text(pDX, IDC_DBS_EDT_NAME2, m_strName2);
	DDV_MaxChars(pDX, m_strName2, 16);
	DDX_Text(pDX, IDC_DBS_EDT_NAME3, m_strName3);
	DDX_Text(pDX, IDC_DBS_EDT_NAME4, m_strName4);
	DDX_Text(pDX, IDC_DBS_EDT_NAME5, m_strName5);
	DDX_Text(pDX, IDC_DBS_EDT_NAME6, m_strName6);
	DDX_Text(pDX, IDC_DBS_EDT_NAME7, m_strName7);
	DDX_Text(pDX, IDC_DBS_EDT_NAME8, m_strName8);
	DDV_MaxChars(pDX, m_strName8, 16);
	DDX_Text(pDX, IDC_DBS_EDT_NAME9, m_strName9);
	DDV_MaxChars(pDX, m_strName9, 16);
	DDX_Text(pDX, IDC_DBS_EDT_URL1, m_strURL1);
	DDV_MaxChars(pDX, m_strURL1, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL10, m_strURL10);
	DDV_MaxChars(pDX, m_strURL10, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL2, m_strURL2);
	DDV_MaxChars(pDX, m_strURL2, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL3, m_strURL3);
	DDV_MaxChars(pDX, m_strURL3, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL4, m_strURL4);
	DDV_MaxChars(pDX, m_strURL4, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL5, m_strURL5);
	DDV_MaxChars(pDX, m_strURL5, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL6, m_strURL6);
	DDV_MaxChars(pDX, m_strURL6, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL7, m_strURL7);
	DDV_MaxChars(pDX, m_strURL7, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL8, m_strURL8);
	DDV_MaxChars(pDX, m_strURL8, 512);
	DDX_Text(pDX, IDC_DBS_EDT_URL9, m_strURL9);
	DDV_MaxChars(pDX, m_strURL9, 512);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBkmk, CDialog)
	//{{AFX_MSG_MAP(CDlgBkmk)
	ON_BN_CLICKED(IDC_DBS_BTN_PREV, OnPrev)
	ON_BN_CLICKED(IDC_DBS_BTN_NEXT, OnNext)
	ON_BN_CLICKED(IDC_DBS_BTN_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_DBS_BTN_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE1, OnChkState1)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE2, OnChkState2)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE3, OnChkState3)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE4, OnChkState4)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE5, OnChkState5)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE6, OnChkState6)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE7, OnChkState7)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE8, OnChkState8)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE9, OnChkState9)
	ON_BN_CLICKED(IDC_DBS_CHK_STATE10, OnChkState10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBkmk message handlers

BOOL CDlgBkmk::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Parser.m_tHead.nCount > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;
	}

	ShowCurBkmk();

	UpdateCtrlState();

	GetDlgItem(IDC_DBS_BTN_PREV)->EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDlgBkmk::SetBuf(LPBYTE pBuf,DWORD dwSize, LPCTSTR lpszTmpPath)
{
	BOOL bOK = m_Parser.DecodeBKMK(pBuf,dwSize);
	m_strTmpPath = lpszTmpPath;

	if(m_Parser.m_tHead.nCount > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;
	}
	
	if(m_bViewOnly)
	{
		ShowCurBkmk();
		UpdateCtrlState();
		GetDlgItem(IDC_DBS_BTN_PREV)->EnableWindow(FALSE);
		if(m_Parser.m_tHead.nCount <= 10)
		{
			GetDlgItem(IDC_DBS_BTN_NEXT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_DBS_BTN_NEXT)->EnableWindow(TRUE);
		}
		UpdateData(FALSE);
	}
	return bOK;
}

void CDlgBkmk::ShowCurBkmk()
{
	if(m_nCurIndex == -1)
	{
		if(m_bViewOnly)
		{
			m_strPos.Format(_T("%d/%d"),m_Parser.m_tHead.nCount==0 ? 0 : (m_nCurIndex+1), (m_Parser.m_tHead.nCount + 9) /10);
		}
		else
		{
			m_strPos.Format(_T("%d/%d"),m_nCurIndex+1 ,(m_Parser.m_vBKMK.size()+9)/10);	
		}
		m_strUsed.Format(_T("USED:%d"),m_Parser.m_tHead.nCount);
		UpdateData(FALSE);
		UpdateCtrlState();
		return;
	}

	UINT uID =  IDC_DBS_CHK_STATE1;
	UINT uIDMnc = IDC_BMK_EDT_MNC1;
	UINT uIDMcc = IDC_BMK_EDT_MCC1;

// 	int nCount = 10;
// 	if((m_nCurIndex*10 + 9) > (int)m_Parser.m_vBKMK.size())
// 	{
// 		nCount = (int)m_Parser.m_vBKMK.size() - m_nCurIndex*10;
// 	}
	
	for(int i = 0; i< 10; i++)
	{
		((CButton*)GetDlgItem(uID+3*i))->SetCheck(m_Parser.m_vBKMK[m_nCurIndex*10 + i].active_flag);
		GetDlgItem(uID+3*i+1)->SetWindowText(m_Parser.m_vBKMK[m_nCurIndex*10 + i].name);
		GetDlgItem(uID+3*i+2)->SetWindowText(m_Parser.m_vBKMK[m_nCurIndex*10 + i].url);

		SetDlgItemInt(uIDMcc+2*i,m_Parser.m_vBKMK[m_nCurIndex*10 + i].nMcc);
		SetDlgItemInt(uIDMnc+2*i,m_Parser.m_vBKMK[m_nCurIndex*10 + i].nMnc);
		//m_nMcc[i] = m_Parser.m_vBKMK[m_nCurIndex*10 + i].nMcc;
		//m_nMnc[i] = m_Parser.m_vBKMK[m_nCurIndex*10 + i].nMnc;

	}

	UpdateData();

	if(m_bViewOnly)
	{
		m_strPos.Format(_T("%d/%d"),m_Parser.m_tHead.nCount==0 ? 0 : (m_nCurIndex+1), (m_Parser.m_tHead.nCount + 9) /10);
	}
	else
	{
		m_strPos.Format(_T("%d/%d"),m_nCurIndex+1 ,(m_Parser.m_vBKMK.size()+9)/10);	
	}
	m_strUsed.Format(_T("USED:%d"),m_Parser.m_tHead.nCount);

	UpdateData(FALSE);	

	UpdateCtrlState();
}

void CDlgBkmk::SaveCurBkmk()
{
	UpdateData();

	if(m_nCurIndex == -1)
		return;

	UINT uID =  IDC_DBS_CHK_STATE1;
	//UINT uIDMnc = IDC_BMK_EDT_MNC1;
	//UINT uIDMcc = IDC_BMK_EDT_MCC1;

	for(int i = 0; i< 10; i++)
	{	
		if((m_nCurIndex*10 + i) >= (int)m_Parser.m_vBKMK.size())
		{
			BOOKMARK_T bm;
			bm.Init();
			m_Parser.m_vBKMK.push_back(bm);
		}
		//UINT mcc = 0,mnc = 0;
		//GetDlgItem(uIDMcc+2*i)->GetDlgItemInt(mcc);
		//GetDlgItem(uIDMnc+2*i)->GetDlgItemInt(mnc);
		m_Parser.m_vBKMK[m_nCurIndex*10 + i].nMcc = m_nMcc[i];
		m_Parser.m_vBKMK[m_nCurIndex*10 + i].nMnc = m_nMnc[i];;

		CString strName;
		GetDlgItem(uID+3*i+1)->GetWindowText(strName);
		_tcscpy(m_Parser.m_vBKMK[m_nCurIndex*10 + i].name,strName);

		CString strUrl;
		GetDlgItem(uID+3*i+2)->GetWindowText(strUrl);
		_tcscpy(m_Parser.m_vBKMK[m_nCurIndex*10 + i].url,strUrl);

		m_Parser.m_vBKMK[m_nCurIndex*10 + i].active_flag = ((CButton*)GetDlgItem(uID+3*i))->GetCheck();
	}
}

void CDlgBkmk::OnPrev() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurIndex == -1)
	{
		return;
	}
	UpdateData();
	if(!CheckValid())
	{
		return;
	}

	SaveCurBkmk();
	m_nCurIndex --;
	if(m_nCurIndex<=0)
	{
		m_nCurIndex = 0;		
	}	
	ShowCurBkmk();
	if(m_nCurIndex == 0)
	{
		GetDlgItem(IDC_DBS_BTN_PREV)->EnableWindow(FALSE);
	}

	int nUpbound = m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vBKMK.size();

	nUpbound = (nUpbound + 9) / 10;

	if(m_nCurIndex < (nUpbound-1))
	{
		GetDlgItem(IDC_DBS_BTN_NEXT)->EnableWindow(TRUE);
	}
}

void CDlgBkmk::OnNext() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurIndex == -1)
	{
		if(!m_bViewOnly)
		{
			if(AfxMessageBox(_T("Do you want to insert one pageat the tail?"),MB_YESNO) == IDYES)
			{
				for(int i = 0; i< 10; i++)
				{
					BOOKMARK_T bm;
					bm.Init();
					m_Parser.m_vBKMK.push_back(bm);					
				}
				m_nCurIndex = 0;
				ShowCurBkmk();	
			}
			return;
		}
	}

	UpdateData();
	if(!CheckValid())
	{
		return;
	}

	SaveCurBkmk();	
	m_nCurIndex++;

	int nUpbound = m_bViewOnly? m_Parser.m_tHead.nCount : m_Parser.m_vBKMK.size();
	nUpbound = (nUpbound + 9) / 10;

	if(m_nCurIndex >= nUpbound && !m_bViewOnly)
	{
		if(m_Parser.m_vBKMK.size() < (int)(BKMK_MAX_NUM/10)*10)
		{
			if(AfxMessageBox(_T("Do you want to insert one page at the tail?"),MB_YESNO) == IDYES)
			{
				for(int i = 0; i< 10; i++)
				{
					BOOKMARK_T bm;
					bm.Init();
					m_Parser.m_vBKMK.push_back(bm);
					
				}

				nUpbound ++;

				if(nUpbound == (int)(BKMK_MAX_NUM/10))
				{
					GetDlgItem(IDC_DBS_BTN_NEXT)->EnableWindow(FALSE);
				}
			}
			else
			{
				m_nCurIndex--;
				return;
			}
		}
	}	

	if(m_nCurIndex > nUpbound)
	{
		m_nCurIndex = nUpbound-1;	
		if(m_nCurIndex == -1)
		{
			m_nCurIndex = 0;
		}
	}
	ShowCurBkmk();
	if(m_nCurIndex == (nUpbound-1) && m_bViewOnly)
	{
		GetDlgItem(IDC_DBS_BTN_NEXT)->EnableWindow(FALSE);
	}

	if(m_nCurIndex > 0)
	{
		GetDlgItem(IDC_DBS_BTN_PREV)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DBS_BTN_PREV)->EnableWindow(FALSE);
	}
}


void CDlgBkmk::UpdateCtrlState()
{
	UINT uID =  IDC_DBS_CHK_STATE1;
	UINT uIDMcc =  IDC_BMK_EDT_MCC1;
	UINT uIDMnc =  IDC_BMK_EDT_MNC1;
	for(int i = 0; i< 10; i++)
	{
		BOOL bActive = ((CButton*)GetDlgItem(uID+3*i))->GetCheck();
		((CButton*)GetDlgItem(uID+3*i))->EnableWindow(!m_bViewOnly&&m_nCurIndex!=-1);
		GetDlgItem(uID+3*i+1)->EnableWindow(bActive && !m_bViewOnly);
		GetDlgItem(uID+3*i+2)->EnableWindow(bActive && !m_bViewOnly);	

		GetDlgItem(uIDMcc+2*i)->EnableWindow(bActive && !m_bViewOnly);
		GetDlgItem(uIDMnc+2*i)->EnableWindow(bActive && !m_bViewOnly);
	}

	if(m_bViewOnly)
	{
		GetDlgItem(IDC_DBS_BTN_IMPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DBS_BTN_EXPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DBS_STC_USED)->ShowWindow(SW_HIDE);
	}
}


void CDlgBkmk::OnImport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,_T("All file(*.*)|*.*||"));
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	FILE *pFile = _tfopen(dlg.GetPathName(),_T("rb"));
	if(pFile == NULL)
	{
		CString strErr;
		strErr.Format(_T("Open file \"%s\" failed!"),dlg.GetPathName());
		AfxMessageBox(strErr);
		return;
	}
	fseek(pFile,0,SEEK_END);
	DWORD dwSize = ftell(pFile);
	fseek(pFile,0,SEEK_SET);
	if(dwSize == 0)
	{
		CString strErr;
		strErr.Format(_T("File size is zero!"));
		AfxMessageBox(strErr);
		return;
	}
	LPBYTE pBuf = new BYTE[dwSize];
	DWORD dwRead = fread(pBuf,1,dwSize,pFile);

	fclose(pFile);

	if(dwRead != dwSize)
	{
		CString strErr;
		strErr.Format(_T("Read file failed!"));
		AfxMessageBox(strErr);
		return;
	}

	BOOL bOK = m_Parser.DecodeBKMK(pBuf,dwSize);

	if(!bOK)
	{
		CString strErr;
		strErr.Format(_T("Import failed!\nPlease check the file format!"));
		AfxMessageBox(strErr);
	}

	delete [] pBuf;
	if(bOK && m_Parser.m_vBKMK.size() > 0)
	{
		m_nCurIndex = 0;
	}
	else
	{
		m_nCurIndex = -1;
	}
	ShowCurBkmk();
	UpdateCtrlState();
	GetDlgItem(IDC_DBS_BTN_PREV)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CDlgBkmk::OnExport() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!CheckValid())
	{
		return;
	}

	SaveCurBkmk();

	if(m_Parser.m_vBKMK.size() == 0)
	{
		AfxMessageBox(_T("No item to save!"));
		return;
	}

	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("All file(*.*)|*.*||"));
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	LPBYTE pBuf = NULL;
	DWORD  dwSize = 0;
	m_Parser.EncodeBKMK(pBuf,dwSize);

	FILE *pFile = _tfopen(dlg.GetPathName(),_T("wb"));
	if(pFile == NULL)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}
	
	DWORD dwWirte = fwrite(pBuf,1,dwSize,pFile);
	fclose(pFile);
	pFile = NULL;

	if(dwWirte != dwSize)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}

	AfxMessageBox(_T("Export complete!"));
}

void CDlgBkmk::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nCurIndex == -1)
	{
		CDialog::OnCancel();
		return;
	}
	UpdateData();
	if(!CheckValid())
	{
		return;
	}

	SaveCurBkmk();

	LPBYTE pBuf = NULL;
	DWORD  dwSize = 0;
	m_Parser.EncodeBKMK(pBuf,dwSize);

	m_strBKMK.Empty();

	CString strFile;
	strFile.Format(_T("%s\\_bkmk.dat"),m_strTmpPath);
	FILE *pFile = _tfopen(strFile.operator LPCTSTR(),_T("wb"));
	if(pFile == NULL)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}
	
	DWORD dwWirte = fwrite(pBuf,1,dwSize,pFile);
	fclose(pFile);
	pFile = NULL;

	if(dwWirte != dwSize)
	{
		AfxMessageBox(_T("Save data failed!"));
		return;
	}

	m_strBKMK = strFile;

	CDialog::OnOK();
}

BOOL CDlgBkmk::CheckValid()
{
	UINT uID =  IDC_DBS_CHK_STATE1;

	CMap<CString,LPCTSTR,CString,LPCTSTR> mapName;

	int i= 0;
	int nMaxCount = m_Parser.m_vBKMK.size();
	for(i = 0; i< nMaxCount; i++)
	{
		//remove current display ones
		if(i>= m_nCurIndex*10  && i< (m_nCurIndex+1)*10)
		{
			continue;
		}
		if(_tcslen(m_Parser.m_vBKMK[i].name) != 0)
		{
			mapName.SetAt(m_Parser.m_vBKMK[i].name,m_Parser.m_vBKMK[i].name);
		}
	}

	for(i = 0; i< 10; i++)
	{	
		if(((CButton*)GetDlgItem(uID+3*i))->GetCheck())
		{
			CString strName;
			GetDlgItem(uID+3*i+1)->GetWindowText(strName);
			if(strName.IsEmpty())
			{			
				GetDlgItem(uID+3*i+1)->SetFocus();
				AfxMessageBox(_T("Name can not be empty if it is marked!"));
				return FALSE;
			}

			CString strUrl;
			GetDlgItem(uID+3*i+2)->GetWindowText(strUrl);
			if(strUrl.IsEmpty())
			{
				GetDlgItem(uID+3*i+2)->SetFocus();
				AfxMessageBox(_T("Url can not be empty if it is marked!"));
				return FALSE;
			}

			CString strTmp;
			//if(mapName.Lookup(strName,strTmp))
			//{
			//	GetDlgItem(uID+3*i+1)->SetFocus();
			//	AfxMessageBox(_T("The name exist already,please change it to another name!"));
			//	return FALSE;
			//}
			//else
			{
				mapName.SetAt(strName,strName);
			}



		}
	}

	return TRUE;
}

void CDlgBkmk::OnCheck(UINT nID)
{
	if(m_nCurIndex == -1)
	{
		return;
	}
	UpdateData();
	BOOL bActive = ((CButton*)GetDlgItem(nID))->GetCheck();

	int nIndex = m_nCurIndex*10 + (nID - IDC_DBS_CHK_STATE1)/3;

	if(m_Parser.m_vBKMK[nIndex].active_flag && !bActive)
	{
		m_Parser.m_tHead.nCount --;
	}
	else if(!m_Parser.m_vBKMK[nIndex].active_flag && bActive)
	{
		m_Parser.m_tHead.nCount ++;
	}
	m_Parser.m_vBKMK[nIndex].active_flag =  bActive;
	m_strUsed.Format(_T("USED:%d"),m_Parser.m_tHead.nCount);
	UpdateData(FALSE);
	UpdateCtrlState();
}
void CDlgBkmk::OnChkState1() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE1);
}

void CDlgBkmk::OnChkState2() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE2);
}

void CDlgBkmk::OnChkState3() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE3);
}

void CDlgBkmk::OnChkState4() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE4);
}

void CDlgBkmk::OnChkState5() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE5);
}

void CDlgBkmk::OnChkState6() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE6);
}

void CDlgBkmk::OnChkState7() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE7);
}

void CDlgBkmk::OnChkState8() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE8);
}

void CDlgBkmk::OnChkState9() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE9);
}

void CDlgBkmk::OnChkState10() 
{
	// TODO: Add your control notification handler code here
	OnCheck(IDC_DBS_CHK_STATE10);
}

BOOL CDlgBkmk::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR )
	{
		CWnd * pWnd = GetFocus();
		_ASSERTE( pWnd != NULL );
	
		_TCHAR cValue = (_TCHAR)(pMsg->wParam);
		if( pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME1)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME2)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME3)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME4)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME5)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME6)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME7)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME8)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME9)->m_hWnd || 
			pWnd->m_hWnd == GetDlgItem(IDC_DBS_EDT_NAME10)->m_hWnd) 

		{
			//名称里面不能含有'\\', '/', ':', '*', '?', '"', '<', '>', '|',0x0D, 0x0A，'.'	
			if( cValue == _T('\\') || cValue == _T('/') || 
				cValue == _T(':') || cValue == _T('*') ||
				cValue == _T('?') || cValue == _T('"') ||
				cValue == _T('<') || cValue == _T('>') ||
				cValue == _T('|') || cValue == _T('.') )
				//cValue != VK_BACK && cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
			{
				return TRUE;
			}	
		
		}
	
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}