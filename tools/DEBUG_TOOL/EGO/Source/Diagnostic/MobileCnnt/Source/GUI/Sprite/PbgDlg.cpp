// PbgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "PbgDlg.h"
#include "InfoWrapper.h"
#include "IPBG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPbgDlg dialog


CPbgDlg::CPbgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPbgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPbgDlg)
	m_strName = _T("");
	m_nStore = -1;
	m_nMemNum = 0;
	//}}AFX_DATA_INIT
	m_pPBG      = NULL;
	m_bNew     = TRUE;

	m_pIW        = NULL;
	m_pPBGFactory = NULL;

	m_nOldStore     = -1;

	m_nUsedInPhone  = 0;
	m_nTotalInPhone = 0;
}

void CPbgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPbgDlg)
	DDX_Control(pDX, IDC_STC_PLACE, m_stcPlace);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDT_PBG_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 20);	
	DDX_Text(pDX, IDC_EDT_PBG_MEMNUM, m_nMemNum);
	DDX_CBIndex(pDX, IDC_PBG_CMB_STORE, m_nStore);
	DDX_Control(pDX, IDC_PBG_CMB_STORE, m_cmbStore);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPbgDlg, CDialog)
	//{{AFX_MSG_MAP(CPbgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPbgDlg message handlers
void CPbgDlg::OnOK() 
{
	UpdateData();

	WORD wStore = (WORD)GetStoreIDFromIdx(m_nStore);
	if( m_bNew && wStore == ID_STORE_PHONE && m_nUsedInPhone == m_nTotalInPhone )
	{
		AfxMessageBox(IDS_PLACE_FULL);
		return;
	}

	if( m_strName.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_PBG_NAME_EMPTY);
		m_edtName.SetFocus();
		return;
	}

	BOOL bAdd = FALSE;
	_ASSERTE( m_pIW != NULL );
	if( m_bNew || m_nStore != m_nOldStore )
	{
		m_pPBG = new PBGROUP;
		if( NULL == m_pPBG )
		{
			AfxMessageBox(IDS_ERR_RES_NOENOUGH);
			return;
		}
		bAdd = TRUE;
	}
	//修改前作一个备份
	PPBGROUP pPBG = NULL;
	if(!bAdd)
	{
		pPBG = new PBGROUP;
		memcpy(pPBG,m_pPBG,sizeof(PBGROUP));
	}
	
	CWaitCursor waitCursor;

	m_pPBG->wStore = wStore;	

	_tcscpy(m_pPBG->szName,        m_strName);

	m_pPBG->nMemberNum = m_nMemNum;

	BOOL bOK = FALSE;

	if( m_nStore == 1 )
	{
		bOK = theApp.TestIsConnected();

		if(!bOK)
		{
			if(bAdd)
			{
				delete m_pPBG;
				m_pPBG = NULL;
			}
			else
			{
				memcpy(m_pPBG,pPBG,sizeof(PBGROUP));
				delete pPBG;
				pPBG = NULL;
			}
			return;
		}
		if( bAdd )
			bOK = m_pPBGFactory->AddPBG( m_pPBG );
		else
			bOK = m_pPBGFactory->ModifyPBG(m_pPBG);

		if( !bOK )
		{
			if(bAdd)
			{
				delete m_pPBG;
				m_pPBG = NULL;
				AfxMessageBox(IDS_ERR_ADD_FAIL);
			}
			else
			{
				memcpy(m_pPBG,pPBG,sizeof(PBGROUP));
				delete pPBG;
				pPBG = NULL;
				AfxMessageBox(IDS_ERR_MODIFY_FAIL);
			}
			
			theApp.TestIsConnected();

			return;
		}
	}

	if(pPBG != NULL)
	{
		delete pPBG;
		pPBG = NULL;
	}

	if( bAdd && bOK )
	{
		m_pIW->AddPBG(m_pPBG);
	}


	CDialog::OnOK();
}

BOOL CPbgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CWaitCursor waitCursor;

	InitCtrls();

	m_pIW = theApp.GetInfoWrapper();
	_ASSERTE( m_pIW != NULL );

	m_pPBGFactory = theApp.m_pPBGFactory;
	_ASSERTE( m_pPBGFactory != NULL );

	if( !m_bNew && m_pPBG != NULL )
	{
		m_strName    = m_pPBG->szName;
		m_nMemNum    = m_pPBG->nMemberNum;
	    UpdateData(FALSE);
	}

	SetSMSPlace(ID_STORE_PHONE);

    //暂时不在PC中存储群组信息
    CWnd * pWnd = GetDlgItem(IDC_PBG_CMB_STORE);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(FALSE);

	if( !theApp.TestIsConnected(FALSE) )
	{
		m_nStore = 0;
	}
	else
	{
		//SIM 没有群组

		if( m_pPBG != NULL && m_pPBG->wStore == ID_STORE_PHONE &&
			m_pPBGFactory->SelectStorePlace(ID_STORE_PHONE) )
		{
			m_pPBGFactory->GetPBGNum(&m_nUsedInPhone, &m_nTotalInPhone);
			SetSMSPlace(ID_STORE_PHONE);

			m_nStore = 1;
		}		

		UpdateData(FALSE);
	}

	m_nOldStore = m_nStore;   

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPbgDlg::InitCtrls()
{
	VERIFY( m_edtName.SubclassDlgItem(IDC_EDT_PBG_NAME, this) );

	// set limit
	m_edtName.SetLimitText(MAX_GROUP_NAME_LEN);

	HINSTANCE hRes = AfxGetResourceHandle();
	static const int nMaxLen = 512;
	_TCHAR szValue[ nMaxLen ] = { 0 };
	VERIFY( LoadString(hRes, IDS_STORE_PLACE, szValue, nMaxLen) > 0 );
	if(_tcslen(szValue)>0)
	{
		if(szValue[_tcslen(szValue)-1] != _T(','))
		{
			_tcscat(szValue,_T(","));
		}
	}
	const _TCHAR cFlag = _T(',');
	LPTSTR pText = szValue;
	LPTSTR pFind = _tcschr(szValue, cFlag);
	m_cmbStore.ResetContent();
    for( int i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';      
		CString strSP = pText;	
		pText = pFind + 1;
		pFind = _tcschr(pText, cFlag);
		m_cmbStore.AddString(strSP);
	}

}

void CPbgDlg::SetSMSPlace(WORD wStore)
{
	CString strFormat;
	VERIFY( strFormat.LoadString(IDS_SMS_PLACE) );
	if(!m_bNew)
	{
		m_stcPlace.SetWindowText(_T(""));
		return;
	}

	CString strPrompt;
	switch( wStore )
	{
	case ID_STORE_PHONE:
		strPrompt.Format(strFormat, m_nUsedInPhone, m_nTotalInPhone - m_nUsedInPhone);
		break;
	default:
		strPrompt = _T("");
	}

	m_stcPlace.SetWindowText(strPrompt);
}

__inline int CPbgDlg::GetStoreIDFromIdx(int nIdx)
{
	switch( nIdx )
	{
	case 0:
		return ID_STORE_PC;
	case 1:
		return ID_STORE_PHONE;
	case 2:
		return ID_STORE_SIM;
	case 3:
		return ID_STORE_SIM2;
	case 4:
		return ID_STORE_SIM3;
	case 5:
		return ID_STORE_SIM4;
	default:
		_ASSERTE( 0 );
	}

	return -1;
}

__inline int CPbgDlg::GetStoreIdxFromID(int nID)
{
	switch( nID )
	{
	case ID_STORE_PC:
		return 0;
	case ID_STORE_PHONE:
		return 1;
	case ID_STORE_SIM:
		return 2;
	case ID_STORE_SIM2:
		return 3;
	case ID_STORE_SIM3:
		return 4;
	case ID_STORE_SIM4:
		return 5;
	default:
		_ASSERTE( 0 );
	}

	return -1;
}
//DEL BOOL CPbgDlg::PreTranslateMessage(MSG* pMsg) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	if( pMsg->message == WM_CHAR )
//DEL 	{
//DEL 		CWnd * pWnd = GetFocus();
//DEL 		_ASSERTE( pWnd != NULL );
//DEL 
//DEL 		// 同MMI保持一致
//DEL 		_TCHAR cValue = pMsg->wParam;
//DEL 		if( pWnd->m_hWnd == m_edtPersonPhone.m_hWnd || 
//DEL 			pWnd->m_hWnd == m_edtOfficePhone.m_hWnd ||
//DEL 			pWnd->m_hWnd == m_edtHomePhone.m_hWnd )
//DEL 		{
//DEL 			if( cValue < _T('0') || cValue > _T('9') )
//DEL 			{
//DEL 				if( cValue != _T('+') && cValue != _T('*') && 
//DEL 					cValue != _T('#') && cValue != VK_BACK )
//DEL 				{
//DEL 					return TRUE;
//DEL 				}	
//DEL 			}
//DEL 		}
//DEL 		else if( pWnd->m_hWnd == m_edtEmail.m_hWnd )
//DEL 		{
//DEL 			if( cValue == _T('~') || cValue == _T('`') || cValue == _T('\\'))
//DEL 			{
//DEL 				return TRUE;	
//DEL 			}
//DEL 		}
//DEL 	}	
//DEL 	return CDialog::PreTranslateMessage(pMsg);
//DEL }


