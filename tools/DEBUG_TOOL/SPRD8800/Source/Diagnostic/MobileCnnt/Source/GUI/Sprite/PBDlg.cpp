// PBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "PBDlg.h"
#include "InfoWrapper.h"
#include "IPB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPBDlg dialog


CPBDlg::CPBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPBDlg)
	m_strEmail = _T("");
	m_strHomePhone = _T("");
	m_strName = _T("");
	m_strOfficePhone = _T("");
	m_strPersonPhone = _T("");
	m_strFax = _T("");	
	//}}AFX_DATA_INIT

	m_pPB      = NULL;
	m_bNew     = TRUE;

	m_pIW        = NULL;
	m_pPBFactory = NULL;

	m_nStore = -1;
	m_nOldStore     = -1;

	m_nUsedInPhone  = 0;
	m_nTotalInPhone = 0;
	m_nUsedInSIM    = 0;
	m_nTotalInSIM   = 0;

	m_nUsedInSIM2    = 0;
	m_nTotalInSIM2   = 0;

	m_nUsedInSIM3    = 0;
	m_nTotalInSIM3   = 0;

	m_nUsedInSIM4    = 0;
	m_nTotalInSIM4   = 0;
}


void CPBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPBDlg)
	DDX_Control(pDX, IDC_STC_PLACE, m_stcPlace);
	DDX_Control(pDX, IDC_STC_GROUP, m_stcGroup);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDT_EMAIL, m_strEmail);
	DDX_Text(pDX, IDC_EDT_HOME_PHONE, m_strHomePhone);
	DDX_Text(pDX, IDC_EDT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDT_OFFICE_PHONE, m_strOfficePhone);
	DDX_Text(pDX, IDC_EDT_PERSON_PHONE, m_strPersonPhone);
	DDX_Text(pDX, IDC_EDT_FAX, m_strFax);
	DDX_Control(pDX, IDC_CMB_GROUP, m_cmbGroup);
	DDX_Control(pDX, IDC_PB_CMB_STORE, m_cmbStore);
//	DDX_CBIndex(pDX, IDC_PB_CMB_STORE, m_nStore);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPBDlg, CDialog)
	//{{AFX_MSG_MAP(CPBDlg)
	ON_CBN_SELCHANGE(IDC_PB_CMB_STORE, OnSelchangeCmbStore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPBDlg message handlers

BOOL CPBDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CWaitCursor waitCursor;

	InitCtrls();


	m_pIW = theApp.GetInfoWrapper();
	_ASSERTE( m_pIW != NULL );

	m_pPBFactory = theApp.m_pPBFactory;
	_ASSERTE( m_pPBFactory != NULL );

	if( !m_bNew && m_pPB != NULL )
	{
		m_strName        = m_pPB->szName;
		m_strPersonPhone = m_pPB->szPhone;
		m_strHomePhone   = m_pPB->szHomePhone;
		m_strOfficePhone = m_pPB->szOfficePhone;
		m_strEmail       = m_pPB->szEmail;
		m_strFax         = m_pPB->szFax;
	}
	if(m_nStore == -1)
	{
		m_nStore = STORE_IN_PC;
	}

	if( !theApp.TestIsConnected(FALSE) )
	{
		m_nStore = STORE_IN_PC;
		CWnd * pWnd = GetDlgItem(IDC_PB_CMB_STORE);
		_ASSERTE( pWnd != NULL );
		pWnd->EnableWindow(FALSE);
	}
	else
	{
	
		if(m_pPBFactory->SelectStorePlace(ID_STORE_PHONE))
		{
			m_pPBFactory->GetPBNum(&m_nUsedInPhone, &m_nTotalInPhone);
		}

		if(theApp.IsPluggedSIM(1) && m_pPBFactory->SelectStorePlace(ID_STORE_SIM))
		{
			m_pPBFactory->GetPBNum(&m_nUsedInSIM, &m_nTotalInSIM);
		}

		if(theApp.IsPluggedSIM(2) && m_pPBFactory->SelectStorePlace(ID_STORE_SIM2))
		{
			m_pPBFactory->GetPBNum(&m_nUsedInSIM2, &m_nTotalInSIM2);
		}

		if(theApp.IsPluggedSIM(3) && m_pPBFactory->SelectStorePlace(ID_STORE_SIM3))
		{
			m_pPBFactory->GetPBNum(&m_nUsedInSIM3, &m_nTotalInSIM3);
		}

		if(theApp.IsPluggedSIM(4) && m_pPBFactory->SelectStorePlace(ID_STORE_SIM4))
		{
			m_pPBFactory->GetPBNum(&m_nUsedInSIM4, &m_nTotalInSIM4);
		}

		if( m_pPB != NULL)
		{
			if( m_pPB->wStore == ID_STORE_PHONE &&
				m_pPBFactory->SelectStorePlace(ID_STORE_PHONE) )
			{		
				
				m_nStore = STORE_IN_BM;
				EnableNoneSimInfo(TRUE);
			}

			if(m_pPB->wStore == ID_STORE_SIM && 
				m_pPBFactory->SelectStorePlace(ID_STORE_SIM) )
			{			
						
				m_nStore = STORE_IN_SM;
				EnableNoneSimInfo(FALSE);
			}

			if( m_pPB->wStore == ID_STORE_SIM2 && 
				m_pPBFactory->SelectStorePlace(ID_STORE_SIM2) )
			{			
							
				m_nStore = STORE_IN_SM2;
				EnableNoneSimInfo(FALSE);
			}


			if( m_pPB->wStore == ID_STORE_SIM3 && 
				m_pPBFactory->SelectStorePlace(ID_STORE_SIM3) )
			{			
							
				m_nStore = STORE_IN_SM3;
				EnableNoneSimInfo(FALSE);
			}	
			
			if( m_pPB->wStore == ID_STORE_SIM4 && 
				m_pPBFactory->SelectStorePlace(ID_STORE_SIM4) )
			{			
							
				m_nStore = STORE_IN_SM4;
				EnableNoneSimInfo(FALSE);
			}
		}

	}

	m_nOldStore = m_nStore;
    
	int nGroupSize = m_arrGroupName.GetSize();
	if(nGroupSize > 0)
	{
		for(int i = 0;i<nGroupSize;i++)
		{
		    m_cmbGroup.AddString(m_arrGroupName.GetAt(i));
		}
		if(m_pPB!=NULL)
		{
			m_cmbGroup.SetCurSel(m_pIW->GetPBGIndex(m_pPB->nGroupIndex));
		}			
		else
		{
			m_cmbGroup.SetCurSel(0);
		}

	}
	else
	{
		m_stcGroup.EnableWindow(FALSE);
		m_cmbGroup.EnableWindow(FALSE);
	}
#ifdef SPRD8800
	if(m_nStore == STORE_IN_BM)
	{
		m_nStore = STORE_IN_PC;
	}
#endif 

	int nIndex = -1;
	if(m_mapSPIndex.Lookup(m_nStore,nIndex) && nIndex != -1)
	{
		m_cmbStore.SetCurSel(nIndex);
	}
	else
	{
		m_cmbStore.SetCurSel(0);
	}

	UpdateData(FALSE);

	OnSelchangeCmbStore();

	return TRUE;
}

void CPBDlg::InitCtrls()
{
	VERIFY( m_edtName.SubclassDlgItem(IDC_EDT_NAME, this) );
	VERIFY( m_edtPersonPhone.SubclassDlgItem(IDC_EDT_PERSON_PHONE, this) );
	VERIFY( m_edtHomePhone.SubclassDlgItem(IDC_EDT_HOME_PHONE, this) );
	VERIFY( m_edtOfficePhone.SubclassDlgItem(IDC_EDT_OFFICE_PHONE, this) );
	VERIFY( m_edtEmail.SubclassDlgItem(IDC_EDT_EMAIL, this) );
	VERIFY( m_edtFax.SubclassDlgItem(IDC_EDT_FAX, this) );

	m_edtPersonPhone.DisablePaste(FALSE);
	m_edtHomePhone.DisablePaste(FALSE);
	m_edtOfficePhone.DisablePaste(FALSE);
	m_edtFax.DisablePaste(FALSE);

	// set limit
	m_edtName.SetLimitText(/*MAX_PERSON_NAME_LEN*/theApp.m_nValidPBNameLen);
	m_edtPersonPhone.SetLimitText(MAX_PHONE_NUM_LEN);
	m_edtHomePhone.SetLimitText(MAX_PHONE_NUM_LEN);
	m_edtOfficePhone.SetLimitText(MAX_PHONE_NUM_LEN);
	m_edtFax.SetLimitText(MAX_PHONE_NUM_LEN);

	m_edtEmail.SetLimitText(MAX_EMAIL_LEN);

#ifdef SPRD8800
	GetDlgItem(IDC_RDO_PHONE)->EnableWindow(FALSE);
#endif

	if(theApp.m_bUSIM)
	{
		GetDlgItem(IDC_EDT_FAX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_FAX)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDT_FAX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_FAX)->ShowWindow(SW_HIDE);
	}

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
	m_mapSPIndex.RemoveAll();
	int nIdx = 0;
    for( int i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';      
		CString strSP = pText;	
		pText = pFind + 1;
		pFind = _tcschr(pText, cFlag);

		if(i<=1)
		{
			m_cmbStore.AddString(strSP);
			m_mapSPIndex.SetAt(STORE_IN_PC+i,nIdx);
			m_cmbStore.SetItemData(nIdx++,STORE_IN_PC+i);
			
		}
		else
		{
			if(theApp.IsPluggedSIM(i-1))
			{
				m_cmbStore.AddString(strSP);
				m_mapSPIndex.SetAt(STORE_IN_PC+i,nIdx);
				m_cmbStore.SetItemData(nIdx++,STORE_IN_PC+i);
			}
			else
			{
				m_mapSPIndex.SetAt(STORE_IN_PC+i,-1);
			}
		}
	}
}

void CPBDlg::SetSMSPlace(WORD wStore)
{
	CString strFormat;
	VERIFY( strFormat.LoadString(IDS_SMS_PLACE) );

	CString strPrompt;
	switch( wStore )
	{
	case ID_STORE_PHONE:
		strPrompt.Format(strFormat, m_nUsedInPhone, m_nTotalInPhone - m_nUsedInPhone);
		break;
	case ID_STORE_SIM:
		strPrompt.Format(strFormat, m_nUsedInSIM, m_nTotalInSIM - m_nUsedInSIM);
		break;
	case ID_STORE_SIM2:
		strPrompt.Format(strFormat, m_nUsedInSIM2, m_nTotalInSIM2- m_nUsedInSIM2);
		break;
	case ID_STORE_SIM3:
		strPrompt.Format(strFormat, m_nUsedInSIM3, m_nTotalInSIM3- m_nUsedInSIM3);
		break;
	case ID_STORE_SIM4:
		strPrompt.Format(strFormat, m_nUsedInSIM4, m_nTotalInSIM4- m_nUsedInSIM4);
		break;
	default:
		strPrompt = _T("");
	}

	m_stcPlace.SetWindowText(strPrompt);
}

__inline int CPBDlg::GetStoreIDFromIdx(int nIdx)
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

__inline int CPBDlg::GetStoreIdxFromID(int nID)
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

void CPBDlg::OnSelchangeCmbStore()
{
	UpdateData();
	CWaitCursor waitCursor;
	int nCurSel = m_cmbStore.GetCurSel();
	if(nCurSel == -1)
		return;

	m_nStore = m_cmbStore.GetItemData(nCurSel);

	switch( m_nStore )
	{
	case STORE_IN_PC:
		{
			EnableNoneSimInfo(TRUE);
			SetSMSPlace( ID_STORE_PC );
			m_cmbGroup.SetCurSel(-1);
			m_stcGroup.EnableWindow(FALSE);
			m_cmbGroup.EnableWindow(FALSE);
			break;
		}	
	case STORE_IN_BM:
		{
				EnableNoneSimInfo(TRUE);	
				SetSMSPlace(ID_STORE_PHONE);
				int nOldSel = m_cmbGroup.GetCurSel();
				m_cmbGroup.SetCurSel(-1);
				m_cmbGroup.ResetContent();
				int nCount = m_pIW->GetPBGCount(ID_STORE_ALL);
				if( nCount > 0 )
				{
					int i = 0;	
					nCount -= theApp.m_nSIMCount;
					for(i = 0;i < nCount;i++)
					{
						m_cmbGroup.AddString(m_arrGroupName.GetAt(i));
					}
					if(nOldSel < nCount && nOldSel != -1)
					{
						m_cmbGroup.SetCurSel(nOldSel);	
					}
					else
					{
						m_cmbGroup.SetCurSel(0);
					}
				}
				m_stcGroup.EnableWindow(TRUE);
				m_cmbGroup.EnableWindow(TRUE);
			break;
		}
	case STORE_IN_SM:
		{
				EnableNoneSimInfo(FALSE);
				SetSMSPlace( ID_STORE_SIM );
				m_cmbGroup.SetCurSel(-1);
				m_cmbGroup.ResetContent();
				int nCount = m_pIW->GetPBGCount(ID_STORE_ALL);
				if( nCount > 0 )
				{
					for(int i = 0;i < nCount;i++)
					{
						m_cmbGroup.AddString(m_arrGroupName.GetAt(i));
					}

					int nCurSel = m_pIW->GetPBGIndexByName(_T("SIM1"));	
					nCurSel = (nCurSel == -1)? (nCount-1) : nCurSel;
					
					m_cmbGroup.SetCurSel(nCurSel);
				}
				m_stcGroup.EnableWindow(FALSE);
				m_cmbGroup.EnableWindow(FALSE);
			break;
		}
	case STORE_IN_SM2:
		{
				EnableNoneSimInfo(FALSE);
				SetSMSPlace( ID_STORE_SIM2 );
				m_cmbGroup.SetCurSel(-1);
				m_cmbGroup.ResetContent();
				int nCount = m_pIW->GetPBGCount(ID_STORE_ALL);
				if( nCount > 0 )
				{
					for(int i = 0;i < nCount;i++)
					{
						m_cmbGroup.AddString(m_arrGroupName.GetAt(i));
					}
					
					int nCurSel = m_pIW->GetPBGIndexByName(_T("SIM2"));	
					nCurSel = (nCurSel == -1)? (nCount-1) : nCurSel;
					
					m_cmbGroup.SetCurSel(nCurSel);
				
				}
				m_stcGroup.EnableWindow(FALSE);
				m_cmbGroup.EnableWindow(FALSE);	
			break;
		}
	case STORE_IN_SM3:
		{
			EnableNoneSimInfo(FALSE);
			SetSMSPlace( ID_STORE_SIM3 );
			m_cmbGroup.SetCurSel(-1);
			m_cmbGroup.ResetContent();
			int nCount = m_pIW->GetPBGCount(ID_STORE_ALL);
			if( nCount > 0 )
			{
				for(int i = 0;i < nCount;i++)
				{
					m_cmbGroup.AddString(m_arrGroupName.GetAt(i));
				}

				int nCurSel = m_pIW->GetPBGIndexByName(_T("SIM3"));	
				nCurSel = (nCurSel == -1)? (nCount-1) : nCurSel;
					
				m_cmbGroup.SetCurSel(nCurSel);
			}
			m_stcGroup.EnableWindow(FALSE);
			m_cmbGroup.EnableWindow(FALSE);	
			break;
		}
	case STORE_IN_SM4:
		{
			EnableNoneSimInfo(FALSE);
			SetSMSPlace( ID_STORE_SIM4 );
			m_cmbGroup.SetCurSel(-1);
			m_cmbGroup.ResetContent();
			int nCount = m_pIW->GetPBGCount(ID_STORE_ALL);
			if( nCount > 0 )
			{
				for(int i = 0;i < nCount;i++)
				{
					m_cmbGroup.AddString(m_arrGroupName.GetAt(i));
				}

				int nCurSel = m_pIW->GetPBGIndexByName(_T("SIM4"));	
				nCurSel = (nCurSel == -1)? (nCount-1) : nCurSel;
					
				m_cmbGroup.SetCurSel(nCurSel);
			}
			m_stcGroup.EnableWindow(FALSE);
			m_cmbGroup.EnableWindow(FALSE);	
			break;
		}
	default:
		_ASSERTE( 0 );
	}


}

void CPBDlg::EnableNoneSimInfo(BOOL bEnable)
{
	CWnd * pWnd = GetDlgItem(IDC_EDT_HOME_PHONE);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM || bEnable);

	pWnd = GetDlgItem(IDC_STC_HOME_PHONE);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM || bEnable);

	pWnd = GetDlgItem(IDC_EDT_OFFICE_PHONE);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM || bEnable);

	pWnd = GetDlgItem(IDC_STC_OFFICE_PHONE);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM || bEnable);

	pWnd = GetDlgItem(IDC_EDT_FAX);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM || bEnable);

	pWnd = GetDlgItem(IDC_STC_FAX);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM || bEnable);


	pWnd = GetDlgItem(IDC_EDT_EMAIL);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM || bEnable);

	pWnd = GetDlgItem(IDC_STC_EMAIL);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(theApp.m_bUSIM|| bEnable);


	pWnd = GetDlgItem(IDC_CMB_GROUP);
	_ASSERTE( pWnd != NULL );
	pWnd->EnableWindow(bEnable);

	if( theApp.m_bUSIM || bEnable )
	{
		CString strText;
		m_edtHomePhone.GetWindowText(strText);
		if( !strText.IsEmpty() )
			m_strHomePhone = strText;

		m_edtOfficePhone.GetWindowText(strText);
		if( !strText.IsEmpty() )
			m_strOfficePhone = strText;

		m_edtEmail.GetWindowText(strText);
		if( !strText.IsEmpty() )
			m_strEmail = strText;

		m_edtFax.GetWindowText(strText);
		if( !strText.IsEmpty() )
			m_strFax = strText;

		m_edtHomePhone.SetWindowText(m_strHomePhone);
		m_edtOfficePhone.SetWindowText(m_strOfficePhone);
		m_edtEmail.SetWindowText(m_strEmail);
		m_edtFax.SetWindowText(m_strFax);
	}
	else
	{
		m_edtHomePhone.SetWindowText(_T(""));
		m_edtOfficePhone.SetWindowText(_T(""));
		m_edtEmail.SetWindowText(_T(""));
		m_edtFax.SetWindowText(_T(""));
	}
}

void CPBDlg::OnCancel()
{	
	if(m_bNew && m_pPB != NULL)
	{
		delete m_pPB;
		m_pPB = NULL;
	}

	CDialog::OnCancel();
}

void CPBDlg::OnClose()
{
	if(m_bNew && m_pPB != NULL)
	{
		delete m_pPB;
		m_pPB = NULL;
	}
	CDialog::OnClose();
}

void CPBDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strTitle;
	strTitle.LoadString(IDS_SMS_TITLE);
	CString strResult;
	

	UpdateData();
    if(!CheckPhoneNum())
	{
		//AfxMessageBox(IDS_ERR_PHONE_NUMBER);
		strResult.LoadString(IDS_ERR_PHONE_NUMBER);
		MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}

	WORD wStore = (WORD)GetStoreIDFromIdx(m_nStore);
	if( m_bNew && wStore == ID_STORE_PHONE && m_nUsedInPhone == m_nTotalInPhone )
	{
		//AfxMessageBox(IDS_PLACE_FULL);
		strResult.LoadString(IDS_ERR_PB_MS_FULL);
		MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}

	if( m_bNew && wStore == ID_STORE_SIM && m_nUsedInSIM == m_nTotalInSIM )
	{
		//AfxMessageBox(IDS_PLACE_FULL);
		strResult.LoadString(IDS_ERR_PB_SIM_FULL);
		MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}

	if( m_bNew && wStore == ID_STORE_SIM2 && m_nUsedInSIM2 == m_nTotalInSIM2 )
	{
		//AfxMessageBox(IDS_PLACE_FULL);
		strResult.LoadString(IDS_ERR_PB_SIM_FULL);
		MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}

	if( m_bNew && wStore == ID_STORE_SIM3 && m_nUsedInSIM3 == m_nTotalInSIM3 )
	{
		//AfxMessageBox(IDS_PLACE_FULL);
		strResult.LoadString(IDS_ERR_PB_SIM_FULL);
		MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}

	if( m_bNew && wStore == ID_STORE_SIM4 && m_nUsedInSIM4 == m_nTotalInSIM4 )
	{
		//AfxMessageBox(IDS_PLACE_FULL);
		strResult.LoadString(IDS_ERR_PB_SIM_FULL);
		MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}

	// MMI储存手机号码时，只要个人号码、住宅号码、办公号码、邮件中任一
	// 一个不为空就可以保存了。当全为空时保存，提示“请输入号码”
	// MMI允许名称为空

	if( m_strPersonPhone.IsEmpty() && m_strOfficePhone.IsEmpty() &&
		m_strHomePhone.IsEmpty() && m_strEmail.IsEmpty() && m_strFax.IsEmpty())
	{
		//AfxMessageBox(IDS_ERR_PB_NUM_EMPTY);
		strResult.LoadString(IDS_ERR_PB_NUM_EMPTY);
		MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
		m_edtName.SetFocus();
		return;
	}

	BOOL bAdd = FALSE;
	_ASSERTE( m_pIW != NULL );
	if( m_bNew || m_nStore != m_nOldStore )
	{
		m_pPB = new PB;
		if( NULL == m_pPB )
		{
			//AfxMessageBox(IDS_ERR_RES_NOENOUGH);
			strResult.LoadString(IDS_ERR_RES_NOENOUGH);
			MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
			return;
		}
		bAdd = TRUE;
	}
	//修改前作一个备份
	PPB pPB = NULL;
	if(!bAdd)
	{
		pPB = new PB;
		memcpy(pPB,m_pPB,sizeof(PB));
	}
	
	CWaitCursor waitCursor;

	m_pPB->wStore = wStore;
	
	int nGroupSize = m_arrGroupName.GetSize();
	int nGIdx = 0;
	if(nGroupSize!=0)
	{
		nGIdx = m_cmbGroup.GetCurSel();
		if(nGIdx < 0 && nGIdx >=nGroupSize)
			nGIdx = 0;
		m_pPB->nGroupIndex = m_pIW->GetPBG(nGIdx)->nID;
	}
	else
	{
		m_pPB->nGroupIndex = 1;
	}
	_tcscpy(m_pPB->szName,        m_strName);
	_tcscpy(m_pPB->szPhone,       m_strPersonPhone);
	_tcscpy(m_pPB->szHomePhone,   m_strHomePhone);
	_tcscpy(m_pPB->szOfficePhone, m_strOfficePhone);
	_tcscpy(m_pPB->szEmail,       m_strEmail);
	_tcscpy(m_pPB->szFax,       m_strFax);

	BOOL bOK = FALSE;

	if( m_nStore != STORE_IN_PC )
	{
		bOK = theApp.TestIsConnected();
		if(!bOK)
		{
			if(bAdd)
			{
				delete m_pPB;
				m_pPB = NULL;
			}
			else
			{
				memcpy(m_pPB,pPB,sizeof(PB));
				delete pPB;
				pPB = NULL;
			}
			return;
		}
		
		if( bAdd )
			bOK = m_pPBFactory->AddPB( m_pPB );
		else
			bOK = m_pPBFactory->ModifyPB(m_pPB);

		if( !bOK )
		{
			if(bAdd)
			{
				delete m_pPB;
				m_pPB = NULL;

				strResult.LoadString(IDS_ERR_ADD_FAIL);
				MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
			}
			else
			{
				memcpy(m_pPB,pPB,sizeof(PB));
				delete pPB;
				pPB = NULL;
				
				strResult.LoadString(IDS_ERR_MODIFY_FAIL);
				MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);
			}			
	
			return;
		}
	}
	else
	{
		bOK = TRUE;
	}

	if(pPB != NULL)
	{
		delete pPB;
		pPB = NULL;
	}

	if( bAdd )
	{
		m_pIW->AddPB(m_pPB);
	}

	CDialog::OnOK();
}

BOOL CPBDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR )
	{
		CWnd * pWnd = GetFocus();
		_ASSERTE( pWnd != NULL );

		// 同MMI保持一致
		_TCHAR cValue = (_TCHAR)(pMsg->wParam);
		if( pWnd->m_hWnd == m_edtPersonPhone.m_hWnd || 
			pWnd->m_hWnd == m_edtOfficePhone.m_hWnd ||
			pWnd->m_hWnd == m_edtHomePhone.m_hWnd )
		{
			if( cValue < _T('0') || cValue > _T('9') )
			{
				if( cValue != _T('+') && cValue != _T('*') && 
					cValue != _T('#') && cValue != VK_BACK && 
					cValue != _T('P') && cValue != _T('W') && cValue != _T('N')&&
					cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
				{
					return TRUE;
				}	
			}
		}
		else if( pWnd->m_hWnd == m_edtEmail.m_hWnd )
		{
			if( cValue == _T('~') || cValue == _T('`') || cValue == _T('\\'))
			{
				return TRUE;	
			}
		}
	}

 	return CDialog::PreTranslateMessage(pMsg);
}
BOOL CPBDlg::CheckPhoneNum()
{
	UpdateData();
    LPTSTR lpsz = NULL;
	BOOL bRetPerson = TRUE;
	BOOL bRetHome = TRUE;
	BOOL bRetOffice = TRUE;

	if(!m_strPersonPhone.IsEmpty())
	{
		lpsz = (LPTSTR)(LPCTSTR)m_strPersonPhone;		
		if(m_edtPersonPhone.HasUcs2Char(lpsz))
		{
			bRetPerson = FALSE;
		}
		else
		{
			while(*lpsz)
			{	
				if( (*lpsz >= _T('0') && *lpsz <= _T('9') ) ||
					*lpsz == _T('+') || *lpsz == _T('*') || *lpsz == _T('#') ||
					*lpsz == _T('P') || *lpsz == _T('W') || *lpsz == _T('N'))
				{
					lpsz++;					
				}
				else
				{
					bRetPerson = FALSE;
					break;
				}
				
			}
		}

	}
	if(!m_strHomePhone.IsEmpty())
	{
		lpsz = (LPTSTR)(LPCTSTR)m_strHomePhone;
		if(m_edtHomePhone.HasUcs2Char(lpsz))
		{
			bRetHome = FALSE;
		}
		else
		{
			while(*lpsz)
			{	
				if( (*lpsz >= _T('0') && *lpsz <= _T('9') ) ||
					*lpsz == _T('+') || *lpsz == _T('*') || *lpsz == _T('#')||
					*lpsz == _T('P') || *lpsz == _T('W') || *lpsz == _T('N'))
				{
					lpsz++;					
				}
				else
				{
					bRetHome = FALSE;
					break;
				}
				
			}
		}
	}
	if(!m_strOfficePhone.IsEmpty())
	{
		lpsz = (LPTSTR)(LPCTSTR)m_strOfficePhone;

		if(m_edtOfficePhone.HasUcs2Char(lpsz))
		{
			bRetOffice = FALSE;
		}
		else
		{
			while(*lpsz)
			{	
				if( (*lpsz >= _T('0') && *lpsz <= _T('9') ) ||
					*lpsz == _T('+') || *lpsz == _T('*') || *lpsz == _T('#') ||
					*lpsz == _T('P') || *lpsz == _T('W') || *lpsz == _T('N'))
				{
					lpsz++;					
				}
				else
				{
					bRetOffice = FALSE;
					break;
				}
				
			}
		}
	}


	if(!bRetPerson)
	{
		m_edtPersonPhone.SetFocus();
		m_edtPersonPhone.SetSel(0,-1);
		m_edtPersonPhone.Invalidate();
		return FALSE;
	}
	if(!bRetHome)
	{
		m_edtHomePhone.SetFocus();
		m_edtHomePhone.SetSel(0,-1);	
		m_edtHomePhone.Invalidate();
		return FALSE;
	}
	if(!bRetOffice)
	{
		m_edtOfficePhone.SetFocus();
		m_edtOfficePhone.SetSel(0,-1);
		m_edtOfficePhone.Invalidate();
		return FALSE;
	}

	return TRUE;

}


