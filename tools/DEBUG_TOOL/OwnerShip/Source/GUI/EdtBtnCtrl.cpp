// EdtBtnCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Ownership.h"
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
	m_bOpen = TRUE;
}


void CEdtBtnCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdtBtnCtrl)
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdtBtnCtrl message handlers

void CEdtBtnCtrl::OnBtnSel() 
{
	// TODO: Add your control notification handler code here
	static _TCHAR BASED_CODE szFilter[] = _T("Number data files (*.dat)|*.dat|All files(*.*)|*.*||");
    CString strFile=_T("");	

    CFileDialog dlg(m_bOpen, _T(".dat"), strFile, 
		m_bOpen? (OFN_NOCHANGEDIR |OFN_HIDEREADONLY | OFN_FILEMUSTEXIST) : (OFN_NOCHANGEDIR |OFN_HIDEREADONLY | OFN_CREATEPROMPT |OFN_OVERWRITEPROMPT), 
		szFilter, NULL);
	
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
	CStatic *pBtnFile = (CStatic *)GetDlgItem(IDC_BTN_SEL);	
	if (pBtnFile->GetSafeHwnd() != NULL) {
		pBtnFile->MoveWindow(rcBtn);
	}
	
	CRect rcEdt;
	rcEdt.SetRect(rcDlg.left,rcDlg.top,rcBtn.left,rcDlg.bottom);
	CStatic *pEdtText = (CStatic *)GetDlgItem(IDC_EDT_TEXT);	
	if (pEdtText->GetSafeHwnd() != NULL) {
		pEdtText->MoveWindow(rcEdt);
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
	if(GetFocus() == (CWnd *)&m_btnSel)
		return;
	UpdateData(TRUE);	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT; //lint !e648
	lvDispInfo.item.mask = LVIF_TEXT;	
	lvDispInfo.item.iItem = m_nItem;
	lvDispInfo.item.iSubItem = m_nSubItem;
	lvDispInfo.item.pszText = const_cast<LPTSTR>(m_strText.operator LPCTSTR());
	lvDispInfo.item.cchTextMax = m_strText.GetLength();

	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	
	DestroyWindow();
	
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


