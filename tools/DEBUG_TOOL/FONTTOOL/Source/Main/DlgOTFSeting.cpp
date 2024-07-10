// DlgOTFSeting.cpp : implementation file
//

#include "stdafx.h"
#include "fonttool.h"
#include "DlgOTFSeting.h"
#include "EdtBtnCtrl.h"
#include "DlgAdvSetting.h"

#define MAX_ROWS (20)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOTFSeting dialog


CDlgOTFSeting::CDlgOTFSeting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOTFSeting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOTFSeting)
	m_strPointSize = _T("");
	m_strFontFile = _T("");
	m_strExPtSize = _T("");
	m_bVector = FALSE;
	m_nVectorShrinkPercent = 100;
	m_bCmpMore = FALSE;
	m_bCmp = FALSE;
	m_nMaxBearingYDelta = 0;
	//}}AFX_DATA_INIT
	
	m_pCSI = NULL;
	m_bOtf = TRUE;

	m_bGray = FALSE;

	_TCHAR szPath[_MAX_PATH] = {0};
	::GetModuleFileName(g_theApp.m_hInstance,szPath,_MAX_PATH);
	m_strConfigFile = szPath;
	if(m_strConfigFile.GetLength()>5)
	{
		m_strConfigFile.Delete(m_strConfigFile.GetLength()-4,4);
		m_strConfigFile += _T(".ini");
	}	
}

CDlgOTFSeting::~CDlgOTFSeting()
{
	ClearFtProperty();
}


void CDlgOTFSeting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOTFSeting)
	DDX_Control(pDX, IDC_FFS_LST_EXFILES, m_lstExFiles);
	DDX_Control(pDX, IDC_FFS_LST_FILES, m_lstFiles);
	DDX_Control(pDX, IDC_OFS_CMB_CHAR_SET, m_cmbCharSet);
	DDX_Text(pDX, IDC_OFS_EDT_POINT_SIZE, m_strPointSize);
	DDX_Text(pDX, IDC_OFS_FONT_FILE, m_strFontFile);
	DDX_Text(pDX, IDC_OFS_EDT_EX_PTSIZE, m_strExPtSize);
	DDX_Check(pDX, IDC_FFS_CHK_VEC_FLAG, m_bVector);
	DDX_Text(pDX, IDC_FFS_EDT_SHK_PERCENT, m_nVectorShrinkPercent);
	DDV_MinMaxUInt(pDX, m_nVectorShrinkPercent, 50, 100);
	DDX_Check(pDX, IDC_FFS_CHK_VEC_CMP_MORE, m_bCmpMore);
	DDX_Check(pDX, IDC_FFS_CHK_VEC_CMP, m_bCmp);
	DDX_Text(pDX, IDC_FFS_EDT_MAX_BEARING_Y_DELTA, m_nMaxBearingYDelta);
	DDV_MinMaxInt(pDX, m_nMaxBearingYDelta, -32, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOTFSeting, CDialog)
	//{{AFX_MSG_MAP(CDlgOTFSeting)
	ON_BN_CLICKED(IDC_OFS_BROWER, OnOfsBrower)
	ON_CBN_SELCHANGE(IDC_OFS_CMB_CHAR_SET, OnSelchangeOfsCmbCharSet)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_FFS_LST_EXFILES, OnEndlabeleditFfsLstExfiles)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_FFS_LST_FILES, OnEndlabeleditFfsLstFiles)
	ON_NOTIFY(NM_DBLCLK, IDC_FFS_LST_EXFILES, OnDblclkFfsLstExfiles)
	ON_NOTIFY(NM_DBLCLK, IDC_FFS_LST_FILES, OnDblclkFfsLstFiles)
	ON_BN_CLICKED(IDC_FFS_ADV_SETTING, OnAdvSetting)
	ON_BN_CLICKED(IDC_FFS_CHK_VEC_FLAG, OnChkVecFlag)
	ON_BN_CLICKED(IDC_FFS_CHK_VEC_CMP, OnChkVecCmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOTFSeting message handlers

void CDlgOTFSeting::OnOfsBrower() 
{
	// TODO: Add your control notification handler code here
	static _TCHAR BASED_CODE szFilterOtf[] = _T("OTF Files (*.ttf; *.otf; *.ttc)|*.ttf; *.otf; *.ttc||");
	static _TCHAR BASED_CODE szFilterBdf[] = _T("BDF Files (*.bdf)|*.bdf||");
    CString strFile=_T("");	
    CFileDialog dlg(TRUE, NULL, strFile, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, m_bOtf?szFilterOtf:szFilterBdf, NULL);
	
	if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strFontFile= dlg.GetPathName();
		GetDlgItem(IDC_OFS_FONT_FILE)->SetWindowText(m_strFontFile);
    }
}

BOOL CDlgOTFSeting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	_TCHAR szCharSet[_MAX_PATH] = {0};
	GetPrivateProfileString(_T("Settings"),_T("CharSet"),_T(""),szCharSet,_MAX_PATH,m_strConfigFile);
	CString strCharSet = szCharSet;

	m_cmbCharSet.Clear();
	int nIdx = 0;
	for(int i= 0; i< g_theApp.m_aCharSetName.GetSize(); i++)
	{
		if(g_theApp.m_aCharSetName[i].CompareNoCase(_T("_CUSTOMER")) != 0 &&
			!g_theApp.m_aCharSetName[i].IsEmpty())
		{
			m_cmbCharSet.AddString(g_theApp.m_aCharSetName[i]);
			if(!strCharSet.IsEmpty() && 
				strCharSet.CompareNoCase(g_theApp.m_aCharSetName[i]) == 0)
			{
				nIdx = m_cmbCharSet.GetCount()-1;
			}


		}
	}
	if(m_cmbCharSet.GetCount() > 0)
	{
		m_cmbCharSet.SetCurSel(nIdx);
		m_cmbCharSet.GetLBText(nIdx,strCharSet);
		g_theApp.m_mapCSI.Lookup(strCharSet,m_pCSI);

		CString strTmp;
		m_strPointSize = _T("");
		int j = 0;
		for(j = 0; j<m_pCSI->nFtTblNum; j++)
		{
			strTmp.Format(_T("%d,"),m_pCSI->pFtTblSize[j]);
			m_strPointSize += strTmp;
		}

		m_strExPtSize = _T("");
		for(j = 0; j<m_pCSI->nExFtTblNum; j++)
		{
			strTmp.Format(_T("%d,"),m_pCSI->pExFtTblSize[j]);
			m_strExPtSize += strTmp;
		}
		if(m_pCSI->nExFtTblNum == 0 || !m_bOtf )
		{
			GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(TRUE);
		}

		if(m_pCSI->nExFtTblNum == 0 )
		{
			GetDlgItem(IDC_FFS_LST_EXFILES)->EnableWindow(FALSE);
		}

		UpdateData(FALSE);
	}

	if(!m_bOtf)
	{
		GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_FFS_ADV_SETTING)->ShowWindow(SW_HIDE);
	}

	m_lstFiles.SetBkColor(RGB(232, 232, 232));	
	m_lstFiles.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstFiles.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;

	m_lstFiles.SetExtendedStyle(dwExStyle);

	m_lstExFiles.SetBkColor(RGB(232, 232, 232));	
	m_lstExFiles.ModifyStyle(0, LVS_SHOWSELALWAYS);	
	dwExStyle = m_lstExFiles.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	m_lstExFiles.SetExtendedStyle(dwExStyle);


	m_lstFiles.InsertColumn(0,_T("BDF Files For Normal FontSize"),LVCFMT_LEFT,365);
	m_lstExFiles.InsertColumn(0,_T("BDF Files For Special FontSize"),LVCFMT_LEFT,365);
	for(int j= 0; j< MAX_ROWS;j++)
	{
		m_lstFiles.InsertItem(j,_T(""));
		m_lstExFiles.InsertItem(j,_T(""));
	}

	m_bVector = m_pCSI->bVector;
	m_nVectorShrinkPercent = m_pCSI->nVectorShrinkPercent;
	m_nMaxBearingYDelta  = m_pCSI->nVectorMaxBearingYDelta;
	m_bCmp = m_pCSI->bVecCmp;
	m_bCmpMore = m_pCSI->bVecCmpMore;

	ModifyBorder();	

	if(m_bOtf)
	{
		_TCHAR szPath[_MAX_PATH] = {0};
		GetPrivateProfileString(_T("Settings"),_T("otfFile"),_T(""),szPath,_MAX_PATH,m_strConfigFile);
		m_strFontFile = szPath;
		UpdateData(FALSE);
	}

	UpdateData(FALSE);
	OnChkVecFlag();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOTFSeting::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_agBdfs.RemoveAll();
	m_agExBdfs.RemoveAll();

	CFileFind finder;

	if(m_bOtf)
	{
		if(m_strFontFile.IsEmpty())
		{
			AfxMessageBox(_T("Please input the font file path!"));
			GetDlgItem(IDC_OFS_FONT_FILE)->SetFocus();
			return;
		}

		if(!finder.FindFile(m_strFontFile))
		{
			AfxMessageBox(_T("Font file is not existed!\n Please input the font file path!"));
			GetDlgItem(IDC_OFS_FONT_FILE)->SetFocus();
			return;
		}
	}
	else
	{
		for(int i=0; i<MAX_ROWS; i++)
		{
			CString strText = m_lstFiles.GetItemText(i,0);
			strText.TrimLeft();
			strText.TrimRight();
			if(!strText.IsEmpty())
			{
				m_agBdfs.Add(strText);
			}

			strText = m_lstExFiles.GetItemText(i,0);
			strText.TrimLeft();
			strText.TrimRight();
			if(!strText.IsEmpty())
			{
				m_agExBdfs.Add(strText);
			}
		}

		if(m_agBdfs.GetSize()==0)
		{
			AfxMessageBox(_T("Please input the font file path!\n(Double click the list cell)"));
			return;
		}
	}

	CString str = _T("Following files are not exited:\n");

	int i= 0;
	BOOL bError = FALSE;
	for(i=0;i< m_agBdfs.GetSize(); i++)
	{
		if(!finder.FindFile(m_agBdfs[i]))
		{
			str +=  m_agBdfs[i];
			str += _T("\n");
			bError = TRUE;
		}
	}

	for(i=0;i< m_agExBdfs.GetSize(); i++)
	{
		if(!finder.FindFile(m_agExBdfs[i]))
		{
			str +=  m_agExBdfs[i];
			str += _T("\n");
			bError = TRUE;
		}
	}

	if(bError)
	{
		AfxMessageBox(str);
		return;
	}

	if(m_strPointSize.IsEmpty())
	{
		AfxMessageBox(_T("Please input point size!"));
		GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->SetFocus();
		return;
	}

	CUIntArray agPtSize;
	CUIntArray agExPtSize;

	int nCount = GetPtSizeArray(m_strPointSize,agPtSize);
	int nExCount = GetPtSizeArray(m_strExPtSize,agExPtSize);
	if(nCount!=0)
	{
		m_pCSI->nFtTblNum = nCount;
		SAFE_FREE_ARRAY(m_pCSI->pFtTblSize);
		SetPtInfo(&m_pCSI->pFtTblSize,agPtSize);
	}
	else
	{
		AfxMessageBox(_T("Please input point size!"));
		GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->SetFocus();
		return;
	}

	if(m_bVector)
	{
		if(nCount>1 || agPtSize[0]>128)
		{
			AfxMessageBox(_T("Please input only one point size and it's not larger than 128 for display!"));
			GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->SetFocus();
			return;
		}
	}
	else
	{
		for(i=0;i<nCount;i++)
		{
			if(agPtSize[i] > 128)
			{
				AfxMessageBox(_T("Please input the right point size which is not larger than 128!"));
				GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->SetFocus();
				return;
			}
		}
		for(i=0;i<nExCount;i++)
		{
			if(agExPtSize[i] > 64)
			{
				AfxMessageBox(_T("Please input the right point size which is not larger than 64!"));
				GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->SetFocus();
				return;
			}
		}
	}

	SAFE_FREE_ARRAY(m_pCSI->pExFtTblSize);
	m_pCSI->nExFtTblNum = nExCount;
	if(nExCount!=0)
	{
		SetPtInfo(&m_pCSI->pExFtTblSize,agExPtSize);
	}

	m_pCSI->bVector = m_bVector;
	m_pCSI->nVectorShrinkPercent = m_nVectorShrinkPercent;
	m_pCSI->nVectorMaxBearingYDelta = m_nMaxBearingYDelta;
	m_pCSI->bVecCmp = m_bCmp;
	m_pCSI->bVecCmpMore = m_bCmpMore;

	if(finder.FindFile(m_strConfigFile))
	{
		DWORD dwAtt = GetFileAttributes(m_strConfigFile);
		dwAtt &=~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(m_strConfigFile,dwAtt);
	}

	if(m_bOtf)
	{
		WritePrivateProfileString(_T("Settings"),_T("otfFile"),m_strFontFile,m_strConfigFile);		
	}

	CString strCharSet;
	m_cmbCharSet.GetWindowText(strCharSet);
	WritePrivateProfileString(_T("Settings"),_T("CharSet"),strCharSet,m_strConfigFile);

	CDialog::OnOK();
}

BOOL CDlgOTFSeting::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message == WM_CHAR)
	{
		CWnd * pWnd = GetFocus();
		_ASSERTE( pWnd != NULL );

		_TCHAR cValue = (_TCHAR)(pMsg->wParam);

		if(pWnd->m_hWnd == GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->m_hWnd || 
		   pWnd->m_hWnd == GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->m_hWnd)
		{
			
			if( cValue < _T('0') || cValue > _T('9') )
			{
				if(!m_bVector)
				{
					if( cValue != _T(',') && cValue != VK_BACK && 
						cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
					{
						return TRUE;
					}
				}
				else
				{
					if( cValue != VK_BACK && 
						cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
					{
						return TRUE;
					}
				}
			}

		}		
	}

	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgOTFSeting::OnSelchangeOfsCmbCharSet() 
{
	// TODO: Add your control notification handler code here
	CString strFontName;
	m_cmbCharSet.GetWindowText(strFontName);
	
	g_theApp.m_mapCSI.Lookup(strFontName,m_pCSI);
	CString strTmp;
	m_strPointSize = _T("");
	for(int j = 0; j<m_pCSI->nFtTblNum; j++)
	{
		strTmp.Format(_T("%d,"),m_pCSI->pFtTblSize[j]);
		m_strPointSize += strTmp;
	}
	m_strExPtSize = _T("");
	for(j = 0; j<m_pCSI->nExFtTblNum; j++)
	{
		strTmp.Format(_T("%d,"),m_pCSI->pExFtTblSize[j]);
		m_strExPtSize += strTmp;
	}
	if(m_pCSI->nExFtTblNum == 0 || !m_bOtf)
	{
		GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(TRUE);
	}

	if(m_pCSI->nExFtTblNum == 0 )
	{
		GetDlgItem(IDC_FFS_LST_EXFILES)->EnableWindow(FALSE);
		//m_lstExFiles.DeleteAllItems();
		for(j=0;j<m_lstExFiles.GetItemCount();j++)
		{
			m_lstExFiles.SetItemText(j,0,_T(""));
		}
	}
	else
	{
		GetDlgItem(IDC_FFS_LST_EXFILES)->EnableWindow(TRUE);
	}

	m_bVector = m_pCSI->bVector;
	m_nVectorShrinkPercent = m_pCSI->nVectorShrinkPercent;
	m_nMaxBearingYDelta = m_pCSI->nVectorMaxBearingYDelta;
	m_bCmpMore = m_pCSI->bVecCmpMore;
	m_bCmp = m_pCSI->bVecCmp;


	UpdateData(FALSE);
	OnChkVecFlag();
}
int CDlgOTFSeting::GetPtSizeArray(LPCTSTR lpszBuf,CUIntArray &agPtSize)
{
	agPtSize.RemoveAll();

	if(_tcslen(lpszBuf)== 0)
		return 0;

	LPTSTR  lpBuf  = (LPTSTR)lpszBuf;
	LPTSTR  lpFind = _tcschr(lpBuf, _T(','));
	int nPtSize = 0;
	while(lpFind != NULL)
	{
		*lpFind = _T('\0');
		nPtSize = _ttoi(lpBuf);
		if(nPtSize != 0)
		{
			agPtSize.Add(_ttoi(lpBuf));
		}
		lpBuf = lpFind + 1;
		lpFind = _tcschr(lpBuf, _T(','));
	}
	nPtSize = _ttoi(lpBuf);
	if(nPtSize != 0)
	{
		agPtSize.Add(_ttoi(lpBuf));
	}	

	int nCount = agPtSize.GetSize();

	return nCount;
}

void CDlgOTFSeting::OnEndlabeleditFfsLstExfiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_lstExFiles.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	*pResult = 0;
}

void CDlgOTFSeting::OnEndlabeleditFfsLstFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_lstFiles.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	*pResult = 0;
}

void CDlgOTFSeting::OnDblclkFfsLstExfiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;

	CEdtBtnCtrl *pCtrl = NULL;
	int nItem = pnia->iItem;
	int nSubItem = pnia->iSubItem;
	if(nItem == -1 || nSubItem != 0)
		return;

	for(int i = 0; i< nItem; i++)
	{
		CString strTxt = m_lstExFiles.GetItemText(i,0);
		strTxt.TrimLeft();
		strTxt.TrimRight();
		if(strTxt.IsEmpty())
			return;
	}
	
	pCtrl = new CEdtBtnCtrl();

	if(NULL != pCtrl)
	{
		pCtrl->m_nItem = nItem;
		pCtrl->m_nSubItem = nSubItem;
		pCtrl->m_strText = m_lstExFiles.GetItemText(nItem,nSubItem);
		if(!pCtrl->Create(IDD_EDT_BTN_DLG,FromHandle(m_lstExFiles.m_hWnd)))
		{
			AfxMessageBox(_T("Error to create EdtBtnCtrl"));
			return;
		}
	}
	else
	{
		return;
	}

	CRect rect;
	m_lstExFiles.GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	CRect rtList;
	m_lstExFiles.GetClientRect(&rtList);
	if(rect.right>rtList.right)
		rect.right = rtList.right;
	if(rect.left < rtList.left)
		rect.left = rtList.left;
	if(rect.bottom>rtList.bottom)
		rect.bottom = rtList.bottom;
	if(rect.top < rtList.top)
		rect.top = rtList.top;
	
	pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top,NULL);
	
    pCtrl->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CDlgOTFSeting::OnDblclkFfsLstFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;

	CEdtBtnCtrl *pCtrl = NULL;
	int nItem = pnia->iItem;
	int nSubItem = pnia->iSubItem;
	if(nItem == -1 || nSubItem != 0)
		return;
	for(int i = 0; i< nItem; i++)
	{
		CString strTxt = m_lstFiles.GetItemText(i,0);
		strTxt.TrimLeft();
		strTxt.TrimRight();
		if(strTxt.IsEmpty())
			return;
	}
	
	pCtrl = new CEdtBtnCtrl();

	if(NULL != pCtrl)
	{
		pCtrl->m_nItem = nItem;
		pCtrl->m_nSubItem = nSubItem;
		pCtrl->m_strText = m_lstFiles.GetItemText(nItem,nSubItem);
		if(!pCtrl->Create(IDD_EDT_BTN_DLG,FromHandle(m_lstFiles.m_hWnd)))
		{
			AfxMessageBox(_T("Error to create EdtBtnCtrl"));
			return;
		}
	}
	else
	{
		return;
	}

	CRect rect;
	m_lstFiles.GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	CRect rtList;
	m_lstFiles.GetClientRect(&rtList);
	if(rect.right>rtList.right)
		rect.right = rtList.right;
	if(rect.left < rtList.left)
		rect.left = rtList.left;
	if(rect.bottom>rtList.bottom)
		rect.bottom = rtList.bottom;
	if(rect.top < rtList.top)
		rect.top = rtList.top;
	
	pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top,NULL);
	
    pCtrl->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CDlgOTFSeting::ModifyBorder()
{
	CRect rcDlg;
	CRect rcOtfGB;
	CRect rcBdfGB;
	GetWindowRect(&rcDlg);
	GetDlgItem(IDC_FFS_OTF_GB)->GetWindowRect(&rcOtfGB);
	GetDlgItem(IDC_FFS_BDF_GB)->GetWindowRect(&rcBdfGB);



	int nDeltH = 0;
	CRect rc;
	CWnd * pWnd =NULL;

	// for otf
	UINT uIDMove[] = {
		IDC_FFS_CS_GB,
		IDC_FFS_STC_CS,
		IDC_FFS_STC_PS,
		IDC_FFS_STC_EXPS,
		IDC_OFS_CMB_CHAR_SET,
		IDC_OFS_EDT_POINT_SIZE,
		IDC_OFS_EDT_EX_PTSIZE,
		IDC_FFS_ADV_SETTING,
		IDC_FFS_CHK_VEC_FLAG,
        IDC_FFS_CHK_VEC_CMP,
		IDC_FFS_CHK_VEC_CMP_MORE,
		IDC_FFS_STC_SHRINK_PER,
		IDC_FFS_STC_PER,
		IDC_FFS_EDT_SHK_PERCENT,
		IDC_FFS_EDT_MAX_BEARING_Y_DELTA,
		IDC_FFS_STC_MAX_BEARING_Y_DELTA,
		IDC_STC_DIV,
		IDC_STC_DIV2,
		IDOK,
		IDCANCEL};
	UINT uIDHide[] = {
 		IDC_FFS_LST_FILES,
 		IDC_FFS_LST_EXFILES,
 		IDC_FFS_BDF_GB};

	// for BDF
	UINT uIDMove2[] = {
		IDC_FFS_LST_FILES,
		IDC_FFS_LST_EXFILES,
		IDC_FFS_BDF_GB,
		IDC_FFS_CS_GB,
		IDC_FFS_STC_CS,
		IDC_FFS_STC_PS,
		IDC_FFS_STC_EXPS,
		IDC_OFS_CMB_CHAR_SET,
		IDC_OFS_EDT_POINT_SIZE,	
		IDC_OFS_EDT_EX_PTSIZE,		
		IDOK,
		IDCANCEL};
	UINT uIDHide2[] = {
 		IDC_OFS_BROWER,
 		IDC_OFS_FONT_FILE,
 		IDC_FFS_OTF_GB,
		IDC_FFS_ADV_SETTING,
		IDC_FFS_CHK_VEC_FLAG,
		IDC_FFS_CHK_VEC_CMP,
		IDC_FFS_CHK_VEC_CMP_MORE,
		IDC_FFS_STC_SHRINK_PER,
		IDC_FFS_STC_PER,
		IDC_FFS_EDT_SHK_PERCENT,
		IDC_FFS_EDT_MAX_BEARING_Y_DELTA,
		IDC_FFS_STC_MAX_BEARING_Y_DELTA,
	    IDC_STC_DIV,
		IDC_STC_DIV2};


	if(m_bOtf)
	{
		nDeltH = rcBdfGB.Height();

		for(int i =0 ; i< sizeof(uIDMove)/sizeof(uIDMove[0]); i++)
		{	
			pWnd = GetDlgItem(uIDMove[i]);
			pWnd->GetWindowRect(&rc);
			rc.top -= nDeltH;
			rc.bottom -= nDeltH;
			ScreenToClient(&rc);
			pWnd->MoveWindow(&rc);
		}

		for(i =0 ; i< sizeof(uIDHide)/sizeof(uIDHide[0]); i++)
		{	
			GetDlgItem(uIDHide[i])->ShowWindow(SW_HIDE);
		}

	}
	else
	{
		nDeltH = rcOtfGB.Height();
		CRect rcTmp;
		GetDlgItem(IDC_FFS_CHK_VEC_FLAG)->GetWindowRect(&rcTmp);
		int nDeltH2 = rcTmp.Height()*3 + nDeltH + 20 ;

		for(int i =0 ; i< sizeof(uIDMove2)/sizeof(uIDMove2[0]); i++)
		{	
			pWnd = GetDlgItem(uIDMove2[i]);
			pWnd->GetWindowRect(&rc);
			if(uIDMove2[i] == IDOK || uIDMove2[i]== IDCANCEL)
			{
				nDeltH = nDeltH2;
			}
			rc.top -= nDeltH;
			rc.bottom -= nDeltH;
			ScreenToClient(&rc);
			pWnd->MoveWindow(&rc);
		}

		for(i =0 ; i< sizeof(uIDHide2)/sizeof(uIDHide2[0]); i++)
		{	
			GetDlgItem(uIDHide2[i])->ShowWindow(SW_HIDE);
		}

		GetDlgItem(	IDC_OFS_EDT_POINT_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(FALSE);
	}


	rcDlg.bottom -= nDeltH;
	MoveWindow(rcDlg);

	Invalidate();

}

void CDlgOTFSeting::OnAdvSetting() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CDlgAdvSetting dlg;

	CUIntArray agPtSize;
	int nCount = GetPtSizeArray(m_strPointSize,agPtSize);

	if(nCount == 0 )
		return;

	int i;
	FONT_PROPERTY_PTR pPtProp = new FONT_PROPERTY_T[nCount];
	for(i=0;i< nCount; i++)
	{
		FONT_PROPERTY_PTR pPtDefault = NULL;
		int nPtSize = agPtSize[i];
		pPtProp[i].nFontSize =  nPtSize;
		_ASSERTE(m_pCSI->pmapFTProp != NULL);
		if(m_pCSI->pmapFTProp->Lookup(nPtSize,pPtDefault))
		{
			pPtProp[i].nShrinkSize = pPtDefault->nShrinkSize;
			pPtProp[i].nFontType  =   pPtDefault->nFontType;
		}
		else
		{			
			pPtProp[i].nShrinkSize = nPtSize;
			pPtProp[i].nFontType  =  0;
		}
	}

	dlg.SetPtSizeProp(pPtProp,nCount);

	CUIntArray agExPtSize;
	FONT_PROPERTY_PTR pExPtProp = NULL;
	int nExCount = GetPtSizeArray(m_strExPtSize,agExPtSize);
	if(nExCount != 0)
	{
		pExPtProp = new FONT_PROPERTY_T[nExCount];
		for(i=0;i< nExCount; i++)
		{
			FONT_PROPERTY_PTR pPtDefault = NULL;
			int nExPtSize = agExPtSize[i];
			pExPtProp[i].nFontSize =  nExPtSize;
			_ASSERTE(m_pCSI->pmapFTProp != NULL);
			if(m_pCSI->pmapFTProp->Lookup(nExPtSize,pPtDefault))
			{
				pExPtProp[i].nShrinkSize = pPtDefault->nShrinkSize;
				pExPtProp[i].nFontType  =   pPtDefault->nFontType;
			}
			else
			{
				pExPtProp[i].nShrinkSize = nExPtSize;
				pExPtProp[i].nFontType  =  0;
			}
		}

		dlg.SetExPtSizeProp(pExPtProp,nExCount);
	}

	if( IDCANCEL == dlg.DoModal())
	{
		if(pPtProp != NULL)
		{
			delete []pPtProp;
		}
		if(pExPtProp != NULL)
		{
			delete [] pExPtProp;
		}
		return;
	}

	ClearFtProperty();

	for(i=0;i<nCount;i++)
	{
		FONT_PROPERTY_PTR pfp = new FONT_PROPERTY_T;
		memcpy(pfp,pPtProp+i,sizeof(FONT_PROPERTY_T));
		m_mapFontProperty.SetAt(pfp->nFontSize,pfp);
	}
	for(i=0;i<nExCount;i++)
	{
		FONT_PROPERTY_PTR pfp = new FONT_PROPERTY_T;
		memcpy(pfp,pExPtProp+i,sizeof(FONT_PROPERTY_T));
		m_mapFontProperty.SetAt(pfp->nFontSize,pfp);
	}

	if(pPtProp != NULL)
	{
		delete []pPtProp;
	}
	if(pExPtProp != NULL)
	{
		delete [] pExPtProp;
	}
}

void CDlgOTFSeting::ClearFtProperty()
{
	POSITION pos = m_mapFontProperty.GetStartPosition();
	int nPtSize;
	FONT_PROPERTY_PTR pProp = NULL;

	while(pos)
	{
		m_mapFontProperty.GetNextAssoc(pos,nPtSize,pProp);
		if(pProp!=NULL)
		{
			delete pProp;
			pProp = NULL;
		}
	}

	m_mapFontProperty.RemoveAll();
}

void CDlgOTFSeting::SetPtInfo(int ** pPtSize, CUIntArray &agPtSize)
{
	int nCount = agPtSize.GetSize();
	if(nCount == 0)
		return;

	int *pPt = new int[nCount];

	int nPtSize = 0;
	FONT_PROPERTY_PTR pProp=NULL;
	FONT_PROPERTY_PTR pPropCust=NULL;

	for(int i=0;i<nCount;i++)
	{
		nPtSize = agPtSize[i];
		pPt[i] = nPtSize;
		m_mapFontProperty.Lookup(nPtSize,pPropCust);
		if(m_pCSI->pmapFTProp->Lookup(nPtSize,pProp))
		{	
			if(pPropCust != NULL)
			{
				memcpy(pProp,pPropCust,sizeof(FONT_PROPERTY_T));
			}	
		}
		else
		{
			pProp = new FONT_PROPERTY_T;
			if(pPropCust != NULL)
			{
				memcpy(pProp,pPropCust,sizeof(FONT_PROPERTY_T));
			}
			else
			{
				pProp->nFontSize = nPtSize;
				pProp->nShrinkSize = nPtSize;
				pProp->nFontType = FONT_TYPE_MONO;
			}
			m_pCSI->pmapFTProp->SetAt(nPtSize,pProp);
		}

		if(pProp->nFontType != FONT_TYPE_MONO)
		{
			m_bGray = TRUE;
		}
	}

	*pPtSize = pPt;
}

void CDlgOTFSeting::OnChkVecFlag() 
{
	// TODO: Add your control notification handler code here
//	GetDlgItem(IDC_FFS_CHK_VEC_FLAG)->UpdateData();
	UpdateData();
	if(m_bVector)
	{
//		GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_FFS_ADV_SETTING)->EnableWindow(FALSE);
		CString str;
		str.Format(_T("%d"),VECTOR_DISP_POINT_SIZE);
		GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->SetWindowText(str);
		GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->SetWindowText(_T(""));

		GetDlgItem(IDC_FFS_STC_SHRINK_PER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FFS_EDT_SHK_PERCENT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FFS_STC_PER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FFS_EDT_MAX_BEARING_Y_DELTA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FFS_STC_MAX_BEARING_Y_DELTA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FFS_CHK_VEC_CMP)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_FFS_CHK_VEC_CMP_MORE)->ShowWindow(m_bCmp?SW_SHOW : SW_HIDE);
		GetDlgItem(IDC_FFS_CHK_VEC_CMP_MORE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_DIV2)->ShowWindow(SW_SHOW);

	}
	else
	{
		if(m_bOtf)
		{
			GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->EnableWindow(TRUE);
			if(m_pCSI!=NULL && m_pCSI->nExFtTblNum != 0)
			{
				GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(FALSE);
			}
			GetDlgItem(IDC_FFS_ADV_SETTING)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_OFS_EDT_POINT_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_OFS_EDT_EX_PTSIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_FFS_ADV_SETTING)->EnableWindow(FALSE);
		}

		GetDlgItem(IDC_FFS_STC_SHRINK_PER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FFS_EDT_SHK_PERCENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FFS_STC_PER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FFS_EDT_MAX_BEARING_Y_DELTA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FFS_STC_MAX_BEARING_Y_DELTA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FFS_CHK_VEC_CMP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FFS_CHK_VEC_CMP_MORE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_DIV2)->ShowWindow(SW_HIDE);
	}
}

void CDlgOTFSeting::OnChkVecCmp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//GetDlgItem(IDC_FFS_CHK_VEC_CMP_MORE)->ShowWindow(m_bCmp?SW_SHOW : SW_HIDE);
}
