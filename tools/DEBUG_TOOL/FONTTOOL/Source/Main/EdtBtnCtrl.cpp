// EdtBtnCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "fonttool.h"
#include "EdtBtnCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdtBtnCtrl dialog


CEdtBtnCtrl::CEdtBtnCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CEdtBtnCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdtBtnCtrl)
	m_strText = _T("");
	//}}AFX_DATA_INIT
	m_nItem = -1;
	m_nSubItem = -1;
	m_nType = EB_NORMAL;
}


void CEdtBtnCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdtBtnCtrl)
	DDX_Control(pDX, IDC_CMB, m_cmbCtrl);
	DDX_Control(pDX, IDC_EDT_TEXT, m_edtText);
	DDX_Control(pDX, IDC_BTN_SEL, m_btnSel);
	DDX_Text(pDX, IDC_EDT_TEXT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdtBtnCtrl, CDialog)
	//{{AFX_MSG_MAP(CEdtBtnCtrl)
	ON_BN_CLICKED(IDC_BTN_SEL, OnBtnSel)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDT_TEXT, OnChangeEdtText)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDT_TEXT, OnKillfocusEdtText)
	ON_CBN_KILLFOCUS(IDC_CMB, OnKillfocusCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdtBtnCtrl message handlers

void CEdtBtnCtrl::OnBtnSel() 
{
	// TODO: Add your control notification handler code here
	static _TCHAR BASED_CODE szFilter[] = _T("BDF Files (*.bdf)|*.bdf||");
    CString strFile=_T("");	
    CFileDialog dlg(TRUE, NULL, strFile, OFN_NOCHANGEDIR |OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
	
	if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strText = dlg.GetPathName();
		UpdateData(FALSE);		
    }
	m_edtText.SetFocus();
}

void CEdtBtnCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rcDlg;
	GetClientRect(&rcDlg);
	int width = rcDlg.right - rcDlg.left;
	
	CRect rcBtn;
	rcBtn.SetRect(width>=20?rcDlg.right-20:rcDlg.left, rcDlg.top, rcDlg.right, rcDlg.bottom);	
	if(m_nType== EB_NORMAL)
	{
		CWnd *pBtnFile = GetDlgItem(IDC_BTN_SEL);
		if (pBtnFile->GetSafeHwnd() != NULL) 
		{
			pBtnFile->MoveWindow(rcBtn);
		}
	}
	
	CRect rcEdt;
	CWnd *pEdtText = GetDlgItem(IDC_EDT_TEXT);	
	if(m_nType== EB_NORMAL)
	{
		rcEdt.SetRect(rcDlg.left,rcDlg.top,rcBtn.left,rcDlg.bottom);
		if (pEdtText->GetSafeHwnd() != NULL) 
		{
			pEdtText->MoveWindow(rcEdt);
		}
	}
	else if(m_nType== EB_EDIT)
	{
		rcEdt.SetRect(rcDlg.left,rcDlg.top,rcDlg.right,rcDlg.bottom);
		if (pEdtText->GetSafeHwnd() != NULL) 
		{
			pEdtText->MoveWindow(rcEdt);
		}
	}	
	else if(m_nType== EB_COMBOX)
	{
		CRect rcCmb;
		rcCmb.SetRect(rcDlg.left,rcDlg.top,rcDlg.right,rcDlg.bottom);
		CWnd *pCmb = GetDlgItem(IDC_CMB);	
		if (pCmb->GetSafeHwnd() != NULL)
		{
			pCmb->MoveWindow(rcCmb);
		}
	}
}

void CEdtBtnCtrl::OnChangeEdtText() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CEdtBtnCtrl::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete this;
}



void CEdtBtnCtrl::OnKillfocusEdtText() 
{
	// TODO: Add your control notification handler code here
	if(m_nType != EB_NORMAL && m_nType != EB_EDIT)
	{
		return;
	}

	if(m_nType == EB_NORMAL)
	{
		if(GetFocus() == (CWnd *)&m_btnSel)
			return;
	}

	UpdateData(TRUE);	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask = LVIF_TEXT;	
	lvDispInfo.item.iItem = m_nItem;
	lvDispInfo.item.iSubItem = m_nSubItem;
	lvDispInfo.item.pszText = LPTSTR((LPCTSTR)m_strText);
	lvDispInfo.item.cchTextMax = m_strText.GetLength();

	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	
//	DestroyWindow();

	CDialog::OnCancel();
	
}

BOOL CEdtBtnCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFont* font = GetParent()->GetFont();
	m_edtText.SetFont(font);
	m_edtText.SetWindowText(m_strText);
	
	m_edtText.SetFocus();
	m_edtText.SetSel(0,0);

	if(m_nType == EB_NORMAL)
	{
		m_cmbCtrl.ShowWindow(SW_HIDE);
	}
	else if(m_nType == EB_EDIT)
	{
		m_cmbCtrl.ShowWindow(SW_HIDE);
		m_btnSel.ShowWindow(SW_HIDE);
	}
	else if(m_nType == EB_COMBOX)
	{
		for(int i=0;i<m_agStrings.GetSize();i++)
		{
			m_cmbCtrl.AddString(m_agStrings[i]);
		}

		if(m_agStrings.GetSize()>0)
		{
			int nFind = -1;
			if(!m_strText.IsEmpty())
			{
				nFind = m_cmbCtrl.FindString(0,m_strText);
			}
			if(nFind != -1)
			{
				m_cmbCtrl.SetCurSel(nFind);
			}
			else
			{
				m_cmbCtrl.SetCurSel(0);
			}
		}

		m_edtText.ShowWindow(SW_HIDE);
		m_btnSel.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CEdtBtnCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)	
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);			
			return 1;
		}					
		
	}
	return CDialog::PreTranslateMessage(pMsg);
}



void CEdtBtnCtrl::OnKillfocusCmb() 
{
	if(m_nType != EB_COMBOX)
	{
		return;
	}

	m_cmbCtrl.GetWindowText(m_strText);

//	UpdateData(TRUE);	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask = LVIF_TEXT;	
	lvDispInfo.item.iItem = m_nItem;
	lvDispInfo.item.iSubItem = m_nSubItem;
	lvDispInfo.item.pszText = LPTSTR((LPCTSTR)m_strText);
	lvDispInfo.item.cchTextMax = m_strText.GetLength();

	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	
//	DestroyWindow();

	CDialog::OnCancel();
	
}
