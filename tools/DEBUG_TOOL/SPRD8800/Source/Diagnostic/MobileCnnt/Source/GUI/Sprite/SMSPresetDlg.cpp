// SMSPresetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "SMSPresetDlg.h"
#include "InfoWrapper.h"

#pragma warning(push,3)
#include <vector>
#include <algorithm>
#pragma warning(pop)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSPresetDlg dialog

CSMSPresetDlg::CSMSPresetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSPresetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSPresetDlg)
	m_strCaption = _T("");
	m_strContent = _T("");
	m_strContentView = _T("");
	m_strInputNumE = _T("");
	m_strLeftNumE = _T("");
	m_strContentNum = _T("");
	//}}AFX_DATA_INIT
	m_rcDlg.SetRectEmpty();
	m_rcEditPos.SetRectEmpty();

	m_bEnableSelect = FALSE;
	m_bEdit         = FALSE;
	m_pIW = NULL;
	m_bShowEdit = FALSE;
}


void CSMSPresetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSPresetDlg)
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_BTN_SELECT, m_btnSelect);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_LST_CAPTION, m_lstCaption);
	DDX_Text(pDX, IDC_EDT_CAPTION, m_strCaption);
	DDX_Text(pDX, IDC_EDT_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_EDT_CONTENT_VIEW, m_strContentView);
	DDX_Text(pDX, IDC_STC_INPUT_NUM_E, m_strInputNumE);
	DDX_Text(pDX, IDC_STC_LEFT_NUM_E, m_strLeftNumE);
	DDX_Text(pDX, IDC_STC_CONTENT_NUM, m_strContentNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSPresetDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSPresetDlg)
	ON_BN_CLICKED(IDC_BTN_SELECT, OnBtnSelect)
	ON_BN_CLICKED(IDC_BTN_NEW, OnBtnNew)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEidt)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDOK, OnBtnOk)
	ON_BN_CLICKED(IDCANCEL, OnBtnCancel)
	ON_LBN_SELCHANGE(IDC_LST_CAPTION, OnSelchangeLstCaption)
	ON_EN_VSCROLL(IDC_EDT_CONTENT, OnVscrollEdtContent)
	ON_EN_CHANGE(IDC_EDT_CONTENT, OnChangeEdtContent)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSPresetDlg message handlers

BOOL CSMSPresetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitCtrls();

	// TODO: Add extra initialization here
	CWnd * pWnd = GetDlgItem(IDC_EDITWND_POS);
	_ASSERTE( pWnd != NULL );

	GetWindowRect(m_rcDlg);
	pWnd->GetWindowRect(m_rcEditPos);

	m_bShowEdit = FALSE;
	ShowEditSection();

	
	m_pIW = theApp.GetInfoWrapper();
	_ASSERTE( m_pIW != NULL );

	pWnd = GetDlgItem(IDC_BTN_SELECT);
	_ASSERTE( pWnd != NULL );

	pWnd->EnableWindow( FALSE );

	pWnd = GetDlgItem(IDC_BTN_EDIT);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BTN_DELETE);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(FALSE);

	FillCaptionList();
	if(m_lstCaption.GetCount()>0)
	{
		m_lstCaption.SetCurSel(0);
		OnSelchangeLstCaption();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSMSPresetDlg::InitCtrls()
{

	VERIFY( m_edtCaption.SubclassDlgItem(IDC_EDT_CAPTION, this) );
	m_edtCaption.SetLimitText(30);

	VERIFY( m_edtContent.SubclassDlgItem(IDC_EDT_CONTENT, this) );
	m_edtContent.EnableHover(FALSE);
	m_edtContent.SetLimitText(MAX_SMS_LEN);
	//m_edtContent.SetSMSInput();
}

void CSMSPresetDlg::FillCaptionList()
{
	_ASSERTE( m_pIW != NULL );

	m_lstCaption.ResetContent();

	PPRESETSMS pSMS = NULL;
	int nCount = m_pIW->GetPresetSMSCount();
	for( int i = 0; i < nCount; ++i )
	{
		pSMS = m_pIW->GetPresetSMS(i);
		
		m_lstCaption.AddString(pSMS->szCaption);
	}
}

void CSMSPresetDlg::OnBtnSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_lstCaption.GetCurSel();
	if( nCurSel == LB_ERR )
		return;

	_ASSERTE( m_pIW != NULL );
	m_pIW->SetCurSelPresetSMS( nCurSel );

	CDialog::OnOK();
}

void CSMSPresetDlg::OnBtnNew() 
{
	// TODO: Add your control notification handler code here
	m_strCaption.Empty();
	m_strContent.Empty();

	if( !m_bShowEdit )
	{
		m_bShowEdit = TRUE;
		ShowEditSection();
	}

	CWnd * pWnd = GetDlgItem(IDC_EDT_CAPTION);
	_ASSERTE( pWnd != NULL );

	pWnd->EnableWindow( TRUE );
	m_strInputNumE.Empty();
	m_strLeftNumE.Empty();

	UpdateData(FALSE);
   
	//*@Hongliang Xin 2006-6-1
    pWnd->SetFocus();	
	m_btnEdit.EnableWindow(FALSE);
	m_btnNew.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(FALSE);
	m_btnExit.EnableWindow(FALSE);
	m_lstCaption.EnableWindow(FALSE);
}

void CSMSPresetDlg::OnBtnEidt() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_lstCaption.GetCurSel();
	if( nCurSel == LB_ERR )
		return;

	_ASSERTE( m_pIW != NULL );
	PPRESETSMS pSMS = m_pIW->GetPresetSMS(nCurSel);

	m_strCaption = pSMS->szCaption;
	m_strContent = pSMS->szContent;

	UpdateData(FALSE);

	if( !m_bShowEdit )
	{
		m_bShowEdit = TRUE;
		ShowEditSection();
	}

	m_bEdit = TRUE;
	CWnd * pWnd = GetDlgItem(IDC_EDT_CAPTION);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow( m_bEdit );

	//*@Hongliang Xin 2006-6-1
	CWnd * pWndContent = GetDlgItem(IDC_EDT_CONTENT);
	_ASSERTE( pWndContent != NULL );
	pWndContent->SetFocus();	
	m_btnEdit.EnableWindow(FALSE);
	m_btnNew.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(FALSE);
	m_btnExit.EnableWindow(FALSE);
	m_lstCaption.EnableWindow(FALSE);

	OnChangeEdtContent();
}

void CSMSPresetDlg::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	//int nCurSel = m_lstCaption.GetCurSel();
	int nSelCount = m_lstCaption.GetSelCount();
	int nItemCount = m_lstCaption.GetCount();//*@Hongliang Xin
	_ASSERTE( nSelCount <= nItemCount );

	if( AfxMessageBox(IDS_DEL_CONFIRM, MB_YESNO) != IDYES )
	{
		return;
	}

	CUIntArray agIdx;
	
	agIdx.SetSize(nSelCount);
	m_lstCaption.GetSelItems(nSelCount,(int *)agIdx.GetData());

	std::vector<int> vIdx;
	for(int i = 0; i< nSelCount; i++)
	{
		vIdx.push_back(agIdx[i]);
	}

	std::sort(vIdx.begin(),vIdx.end());

	_ASSERTE( m_pIW != NULL );

	for(i=nSelCount-1; i>=0; i--)
	{		
		m_pIW->DelPresetSMS( vIdx[i] );
		m_lstCaption.DeleteString(vIdx[i]);
	}	
    
	m_strContentView.Empty();


	if(0 == m_lstCaption.GetCount())
	{
		m_btnDelete.EnableWindow(FALSE);
		m_btnEdit.EnableWindow(FALSE);
		m_btnSelect.EnableWindow(FALSE);
		m_strContentNum.Empty();
		UpdateData(FALSE);
		return;
	}
	
	UpdateData(FALSE);
	
	m_lstCaption.SetCurSel(0);

	OnSelchangeLstCaption();

}

void CSMSPresetDlg::ShowEditSection()
{
	CWnd * pWnd = GetDlgItem(IDC_EDITWND_POS);
	_ASSERTE( pWnd != NULL );

	pWnd->GetWindowRect(m_rcEditPos);

	int nDlgW = m_rcDlg.Width();
	int nDlgH = m_rcDlg.Height();
	GetWindowRect(m_rcDlg);

	m_rcDlg.right  = m_rcDlg.left + nDlgW;
	m_rcDlg.bottom = m_rcDlg.top  + nDlgH;

	if( m_bShowEdit )
	{
		MoveWindow(m_rcDlg);
	}
	else
	{
		CRect rc  = m_rcDlg;
		rc.bottom = m_rcEditPos.top;
		MoveWindow(rc);
	}

	Invalidate();
}

void CSMSPresetDlg::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CStringArray arrStr;
	arrStr.Add( m_strCaption );
	arrStr.Add( m_strContent );

	CUIntArray arrUint;
	arrUint.Add(IDC_STC_NAME);
	arrUint.Add(IDC_STC_CONTENT);

	int nSize = arrStr.GetSize();
	VERIFY( nSize == arrUint.GetSize() );

	for( int i = 0; i < nSize; ++i )
	{
		if( arrStr[i].IsEmpty() )
		{
			CString strFormat;	
			VERIFY( strFormat.LoadString(IDS_PROMPT_EMPTY) );
			
			CWnd * pWnd = GetDlgItem(arrUint[i]);
			_ASSERTE(pWnd != NULL);
			
			CString strText;
			pWnd->GetWindowText(strText);
			strText.TrimRight(_T(':'));

			CString strPrompt;
			strPrompt.Format( strFormat, strText );
			AfxMessageBox(strPrompt);
			
			return;
		}
	}

	_ASSERTE( m_pIW != NULL );

	if( !m_bEdit && m_pIW->IsPresetSMSExist( m_strCaption ) )
	{
		CString strFormat;	
		VERIFY( strFormat.LoadString(IDS_EXIST) );

		CString strPrompt;
		strPrompt.Format( strFormat, m_strCaption );
		AfxMessageBox(strPrompt);

		m_strCaption.Empty();
		return ;
	}

	PPRESETSMS pSMS = NULL;
	if( m_bEdit)
	{
		int nCurSel = m_lstCaption.GetCurSel();
		_ASSERTE( nCurSel >= 0 && nCurSel < m_lstCaption.GetCount() );

		pSMS = m_pIW->GetPresetSMS(nCurSel);
		_tcscpy(pSMS->szContent, m_strContent);
		_tcscpy(pSMS->szCaption, m_strCaption);

		m_strContentView = m_strContent;

        m_lstCaption.DeleteString(nCurSel);
		m_lstCaption.InsertString(nCurSel,m_strCaption);
		
		m_lstCaption.SetCurSel(nCurSel);//*@Hongliang Xin

		OnSelchangeLstCaption();
	}
	else
	{
		pSMS = new PRESETSMS;
		if( pSMS == NULL )
		{
			AfxMessageBox(IDS_ERR_RES_NOENOUGH);
			return;
		}
		
		_tcscpy(pSMS->szCaption, m_strCaption);
		_tcscpy(pSMS->szContent, m_strContent);
		m_pIW->AddPresetSMS( pSMS );

		m_lstCaption.AddString( m_strCaption );

		m_lstCaption.SetCurSel(m_lstCaption.GetCount()-1);//*@Hongliang Xin
		OnSelchangeLstCaption();
	}
	
	m_strCaption.Empty();
	m_strContent.Empty();
    m_strInputNumE.Empty();
	m_strLeftNumE.Empty();
	UpdateData(FALSE);

	m_bShowEdit = FALSE;
	ShowEditSection();

	m_bEdit = FALSE;

	//*@Hongliang Xin 2006-6-1
	//Begin
	m_btnNew.EnableWindow(TRUE);
	m_btnEdit.EnableWindow(TRUE);
	m_btnDelete.EnableWindow(TRUE);
	m_btnExit.EnableWindow(TRUE);
	m_lstCaption.EnableWindow(TRUE);
	//End//xe*/

	m_btnNew.SetFocus();
}

void CSMSPresetDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	m_bShowEdit = FALSE;
	ShowEditSection();
	//*@Hongliang Xin 2006-6-1
	//Begin
	if(m_lstCaption.GetCount()>0 && m_lstCaption.GetCurSel() != -1)
	{
		m_btnEdit.EnableWindow(TRUE);
		m_btnDelete.EnableWindow(TRUE);
	}
	m_btnNew.EnableWindow(TRUE);
	m_btnExit.EnableWindow(TRUE);
	m_lstCaption.EnableWindow(TRUE);
	//End//xe*/
}

void CSMSPresetDlg::EnableSelect(BOOL bEnable /* = TRUE */)
{
	m_bEnableSelect = bEnable;
}

void CSMSPresetDlg::OnSelchangeLstCaption() 
{
	// TODO: Add your control notification handler code here
	CWnd * pWnd = GetDlgItem(IDC_BTN_SELECT);
	_ASSERTE( pWnd != NULL );

	pWnd->EnableWindow( m_bEnableSelect );

	pWnd = GetDlgItem(IDC_BTN_EDIT);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_BTN_DELETE);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(TRUE);

	_ASSERTE( m_pIW != NULL );
	int nCurSel = m_lstCaption.GetCurSel();

	PPRESETSMS pSMS = m_pIW->GetPresetSMS(nCurSel);

	m_strContentView = pSMS->szContent;

	m_strCaption = pSMS->szCaption;
	m_strContent = pSMS->szContent;
    
	CString str;
	str.Format(_T("%d"),((CCoolEdit *)GetDlgItem(IDC_EDT_CONTENT))->GetFullCharNum(m_strContentView,0));
	m_strContentNum = str;

	UpdateData(FALSE);
}

void CSMSPresetDlg::OnVscrollEdtContent() 
{
	// TODO: Add your control notification handler code here
	CWnd * pWnd = GetDlgItem(IDC_EDT_CONTENT);
	_ASSERTE( pWnd != NULL );

	pWnd->Invalidate();
}

void CSMSPresetDlg::OnChangeEdtContent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	CWnd * pWnd = GetDlgItem(IDC_EDT_CONTENT);
	_ASSERTE( pWnd != NULL );
    
	pWnd->Invalidate();

	CCoolEdit * pEdit = (CCoolEdit *)pWnd;
	int nNum = pEdit->GetFullCharNum(m_strContent,0);
	CString str;
	str.Format(_T("%d"),nNum);
	if(nNum>0)
	{
		m_strInputNumE = str;
		if(pEdit->HasUcs2Char(m_strContent))
		{
			str.Format(_T("%d"),MAX_SMS_LEN /2 - nNum);
			m_strLeftNumE = str;
		}
		else
		{
			str.Format(_T("%d"),MAX_SMS_LEN  - nNum);
			m_strLeftNumE = str;
		}
	}
	else
	{
		m_strInputNumE = str;
		str.Format(_T("%d"),MAX_SMS_LEN  - nNum);
		m_strLeftNumE = str;
	}

	UpdateData(FALSE);

}

void CSMSPresetDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here

	CDialog::OnCancel();
}


void CSMSPresetDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnCancel();
//	CDialog::OnClose();
}
