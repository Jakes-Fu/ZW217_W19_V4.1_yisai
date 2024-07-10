// MemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "MemoDlg.h"
#include "InfoWrapper.h"
#include "IMemo.h"
#include "mmiatc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int CMemoDlg::m_nMaxContentLen = MAX_MEMO_LEN;

const int CMemoDlg::MIN_YEAR  = 1980;
const int CMemoDlg::MAX_YEAR  = 2099;
/////////////////////////////////////////////////////////////////////////////
// CMemoDlg dialog


CMemoDlg::CMemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemoDlg)
	m_strContent = _T("");
	m_nStore = 0;
	//}}AFX_DATA_INIT
	m_nOldStore  = -1;
	m_bNew       = TRUE;
	m_nTotalInPhone = 0;
	m_nUsedInPhone  = 0;
	
	m_pMemo      = NULL;
	m_pIW        = NULL;
	m_pMemoFactory = NULL;
}


void CMemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoDlg)
	DDX_Control(pDX, IDC_CMB_MODE, m_cmbMode);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STC_PLACE, m_stcPlace);
	DDX_Control(pDX, IDC_DTP_TIME, m_dtpTime);
	DDX_Control(pDX, IDC_DTP_DATE, m_dtpDate);
	DDX_Control(pDX, IDC_DTP_DATE_END, m_dtpDateEnd);
	DDX_Control(pDX, IDC_CHK_SUN, m_btnSun);
	DDX_Control(pDX, IDC_CHK_MON, m_btnMon);
	DDX_Control(pDX, IDC_CHK_TUE, m_btnTue);
	DDX_Control(pDX, IDC_CHK_WED, m_btnWed);
	DDX_Control(pDX, IDC_CHK_THURS, m_btnThu);
	DDX_Control(pDX, IDC_CHK_FRI, m_btnFri);
	DDX_Control(pDX, IDC_CHK_SAT, m_btnSat);
	DDX_Text(pDX, IDC_EDT_CONTENT, m_strContent);
	DDX_Radio(pDX, IDC_RDO_PC, m_nStore);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemoDlg, CDialog)
	//{{AFX_MSG_MAP(CMemoDlg)
	ON_EN_VSCROLL(IDC_EDT_CONTENT, OnVscrollEdtContent)
	ON_EN_UPDATE(IDC_EDT_CONTENT, OnUpdateEdtContent)
	ON_BN_CLICKED(IDC_RDO_PC, OnRdoPc)
	ON_BN_CLICKED(IDC_RDO_PHONE, OnRdoPhone)	
	ON_CBN_SELCHANGE(IDC_CMB_MODE, OnSelchangeCmbMode)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoDlg message handlers

BOOL CMemoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWaitCursor waitCursor;
	InitCtrls();
	// TODO: Add extra initialization here


	m_pIW = theApp.GetInfoWrapper();
	_ASSERTE( m_pIW != NULL );

	m_pMemoFactory = theApp.m_pMemoFactory;
	_ASSERTE( m_pMemoFactory != NULL );

	BOOL bEnablePhone = theApp.TestIsConnected(FALSE);
    COleDateTime odt;	
	if( !m_bNew && NULL != m_pMemo)// Edit Memo
	{		
		m_nStore     = GetStoreIdxFromID(m_pMemo->wStore);
		m_strContent = m_pMemo->szContent;		
	
		odt.SetDate( m_pMemo->memoStartDate.wYear, m_pMemo->memoStartDate.btMonth, m_pMemo->memoStartDate.btDay );
		m_dtpDate.SetTime( odt );

		odt.SetDate( m_pMemo->memoEndDate.wYear, m_pMemo->memoEndDate.btMonth, m_pMemo->memoEndDate.btDay );
		m_dtpDateEnd.SetTime( odt );

		odt.SetTime( m_pMemo->memoTime.btHour, m_pMemo->memoTime.btMinut, 0 );
		m_dtpTime.SetTime( odt );	
		
		m_cmbMode.SetCurSel(m_pMemo->nMemoMode - 1);

		SetModeFre(m_pMemo->nMemoMode,m_pMemo->fre);
	}
	else // New Memo
	{
		m_cmbMode.SetCurSel(0);
		SetModeFre(0,0);
	}
	
// 	SetSMSPlace(ID_STORE_PC);
	if(m_nStore == -1)
		m_nStore = 0; //pc
	
	if(!bEnablePhone)
	{
		m_nStore = 0; //PC
		CWnd * pWnd = GetDlgItem(IDC_RDO_PHONE);
		_ASSERTE( pWnd != NULL );
		pWnd->EnableWindow(FALSE);
	
	}
	else
	{
		if(m_pMemoFactory->SelectStorePlace(ID_STORE_PHONE) )
		{
			m_pMemoFactory->GetMemoNum(&m_nUsedInPhone, &m_nTotalInPhone);
		}
		if( m_pMemo != NULL && m_pMemo->wStore == ID_STORE_PHONE)
		{			
// 			SetSMSPlace(ID_STORE_PHONE);
			m_nStore = 1; //Phone
		}
		else
		{
			if( m_nUsedInPhone >= m_nTotalInPhone && m_nTotalInPhone != 0)				
				bEnablePhone = FALSE;
		}
	}

	if( !bEnablePhone )
	{
		m_nStore = 0;//PC
		CWnd * pWnd = GetDlgItem(IDC_RDO_PHONE);
		_ASSERTE( pWnd != NULL );
		pWnd->EnableWindow(FALSE);
// 		SetSMSPlace(ID_STORE_PC);
	}

	m_nOldStore = m_nStore;

    if(m_nStore == 1) //Phone
	{
		SetSMSPlace(ID_STORE_PHONE);
	}
	else
	{
		SetSMSPlace(ID_STORE_PC);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMemoDlg::InitCtrls()
{

	COleDateTime odtMin, odtMax;
	odtMin.SetDate( MIN_YEAR, 1, 1 );
	odtMax.SetDate( MAX_YEAR, 12, 31 );
	m_dtpDate.SetRange( &odtMin, &odtMax );
	m_dtpDate.SetFormat(_T("yyy-MM-dd"));

	m_dtpDate.SetMonthCalColor(MCSC_TITLEBK, RGB( 20, 120, 20 ));
	m_dtpDate.SetMonthCalColor(MCSC_MONTHBK , CLR_DLGBCKGRND);

	m_dtpDateEnd.SetRange( &odtMin, &odtMax );
	m_dtpDateEnd.SetFormat(_T("yyy-MM-dd"));

	m_dtpDateEnd.SetMonthCalColor(MCSC_TITLEBK, RGB( 20, 120, 20 ));
	m_dtpDateEnd.SetMonthCalColor(MCSC_MONTHBK , CLR_DLGBCKGRND);

	m_edtContent.SubclassDlgItem(IDC_EDT_CONTENT, this);
	m_edtContent.SetLimitText(m_nMaxContentLen);
	m_edtContent.EnableHover(FALSE);

    HINSTANCE hRes = AfxGetResourceHandle();
	static const int nMaxLen = 512;
	_TCHAR szValue[ nMaxLen ] = { 0 };

	// load memo mode string
	VERIFY( LoadString(hRes, IDS_MEMO_MODE, szValue, nMaxLen) > 0 );	
	const _TCHAR cFlag = _T(',');
	LPTSTR pText = szValue;
	LPTSTR pFind = _tcschr(szValue, cFlag);
	int i;
    for( i= 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
		m_cmbMode.AddString(pText);
		pText = pFind + 1;
		pFind = _tcschr(pText, cFlag);
	}
	m_cmbMode.AddString(pText);
}

void CMemoDlg::OnVscrollEdtContent() 
{
	// TODO: Add your control notification handler code here
	CWnd * pWnd = GetDlgItem(IDC_EDT_CONTENT);
	_ASSERTE( pWnd != NULL );

	pWnd->Invalidate();
}

void CMemoDlg::OnUpdateEdtContent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CWnd * pWnd = GetDlgItem(IDC_EDT_CONTENT);
	_ASSERTE( pWnd != NULL );

	pWnd->Invalidate();
}

void CMemoDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);


	WORD wStore = (WORD)GetStoreIDFromIdx(m_nStore);
	if( m_bNew && wStore == ID_STORE_PHONE && m_nUsedInPhone == m_nTotalInPhone )
	{
		AfxMessageBox(IDS_ERR_MEMO_MS_FULL);
		return;
	}
	
	
	COleDateTime tPhoneCur = COleDateTime::GetCurrentTime();
	tPhoneCur -= theApp.m_tsPcPhone;
	//AfxMessageBox(tPhoneCur.Format("%y/%m/%d,%H:%M:%S"));	
	
	
	COleDateTime tTime;
	m_dtpTime.GetTime(tTime);

	COleDateTime tDate;
	m_dtpDate.GetTime(tDate);	
	COleDateTime tUserSet(tDate.GetYear(),tDate.GetMonth(),tDate.GetDay(),
			       tTime.GetHour(),tTime.GetMinute(),tTime.GetSecond());
    //差值120秒
	COleDateTimeSpan tsDel(120);

	if( m_cmbMode.GetCurSel() == 0 && (tUserSet - tPhoneCur)<tsDel)
	{
		AfxMessageBox(IDS_ERR_OUT_OF_DATE);
		return;
	}
	m_dtpDateEnd.GetTime(tDate);
	COleDateTime tUserSet2(tDate.GetYear(),tDate.GetMonth(),tDate.GetDay(),
			        tTime.GetHour(),tTime.GetMinute(),tTime.GetSecond());

	COleDateTimeSpan tsDel2(0);
	if((tUserSet2 - tUserSet)<tsDel2)
	{
		AfxMessageBox(IDS_ERR_MEMO_END_DATE);
		return;
	}


	BOOL bAdd = FALSE;
	_ASSERTE( m_pIW != NULL );
	if( m_bNew || m_nStore != m_nOldStore )
	{
		m_pMemo = new MEMO;
		if( NULL == m_pMemo )
		{
			AfxMessageBox(IDS_ERR_RES_NOENOUGH);
			return;
		}
		bAdd = TRUE;
	}

    _ASSERTE( m_pMemo != NULL);

	//修改前作一个备份
	PMEMO pMemo = NULL;
	if(!bAdd)
	{
		pMemo = new MEMO;
		memcpy(pMemo,m_pMemo,sizeof(MEMO));
	}

	CWaitCursor waitCursor;

	COleDateTime odt;

	m_dtpDate.GetTime( odt );

	m_pMemo->memoStartDate.wYear   = (WORD)odt.GetYear();
	m_pMemo->memoStartDate.btMonth = (BYTE)odt.GetMonth();
	m_pMemo->memoStartDate.btDay   = (BYTE)odt.GetDay();

	m_pMemo->memoTime.wYear   = (WORD)odt.GetYear();
	m_pMemo->memoTime.btMonth = (BYTE)odt.GetMonth();
	m_pMemo->memoTime.btDay   = (BYTE)odt.GetDay();

	m_dtpTime.GetTime( odt );
	m_pMemo->memoTime.btHour  = (BYTE)odt.GetHour();
	m_pMemo->memoTime.btMinut = (BYTE)odt.GetMinute();

	m_dtpDateEnd.GetTime( odt );

	m_pMemo->memoEndDate.wYear   = (WORD)odt.GetYear();
	m_pMemo->memoEndDate.btMonth = (BYTE)odt.GetMonth();
	m_pMemo->memoEndDate.btDay   = (BYTE)odt.GetDay();

	m_pMemo->wStore = (WORD)GetStoreIDFromIdx(m_nStore);
	_tcscpy(m_pMemo->szContent, m_strContent );
	m_pMemo->bUsed  = TRUE;
	m_pMemo->bUcs2  = m_edtContent.HasUcs2Char();

	m_pMemo->nMemoMode = (WORD)(m_cmbMode.GetCurSel()+1);
	m_pMemo->fre = GetMemoFre(m_pMemo->nMemoMode);
	m_pMemo->wStore = wStore;


	BOOL bOK = FALSE;
	if( m_nStore == 1 )
	{

		bOK=theApp.TestIsConnected();

		if(!bOK)
		{
			if(bAdd)
			{
				delete m_pMemo;
				m_pMemo = NULL;
			}
			else
			{
				memcpy(m_pMemo,pMemo,sizeof(MEMO));
				delete pMemo;
				pMemo = NULL;
			}
			return;
		}

		if( bAdd )
			bOK = m_pMemoFactory->AddMemo( m_pMemo );
		else
			bOK = m_pMemoFactory->ModifyMemo( m_pMemo );

		if( !bOK )
		{
			if(bAdd)
			{
				delete m_pMemo;
				m_pMemo = NULL;
				AfxMessageBox(IDS_ERR_ADD_FAIL);
			}
			else
			{
				memcpy(m_pMemo,pMemo,sizeof(MEMO));
				delete pMemo;
				AfxMessageBox(IDS_ERR_MODIFY_FAIL);
			}
			
			theApp.TestIsConnected();
			return;
		}
	}
	else
	{
		bOK = TRUE;
	}

	if( bAdd && bOK)
		m_pIW->AddMEMO(m_pMemo);

	if(pMemo != NULL)
	{
		delete pMemo;
		pMemo = NULL;
	}
	
	CDialog::OnOK();
}

__inline int CMemoDlg::GetStoreIDFromIdx(int nIdx)
{
	switch( nIdx )
	{
	case 0:
		return ID_STORE_PC;
	case 1:
		return ID_STORE_PHONE;
	default:
		_ASSERTE( 0 );
	}

	return -1;
}

__inline int CMemoDlg::GetStoreIdxFromID(int nID)
{
	switch( nID )
	{
	case ID_STORE_PC:
		return 0;
	case ID_STORE_PHONE:
		return 1;
	default:
		_ASSERTE( 0 );
	}

	return -1;
}

void CMemoDlg::SetSMSPlace(WORD wStore)
{
	CString strFormat;
	VERIFY( strFormat.LoadString(IDS_SMS_PLACE) );

	CString strPrompt;
	if( wStore == ID_STORE_PHONE)
	{
		strPrompt.Format(strFormat, m_nUsedInPhone, m_nTotalInPhone - m_nUsedInPhone);
		m_stcPlace.SetWindowText(strPrompt);
	}
	else
	{
		m_stcPlace.SetWindowText(_T(""));
	}
}

void CMemoDlg::OnRdoPc() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SetSMSPlace(ID_STORE_PC);
	if(m_bNew)
	{
		CTime _time = CTime::GetCurrentTime();	
		m_dtpDate.SetTime( &_time);
		m_dtpTime.SetTime( &_time );	
	}
	UpdateData(FALSE);
}

void CMemoDlg::OnRdoPhone() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CWaitCursor waitCursor;
	BOOL bRet = TRUE;
	bRet = m_pMemoFactory->SelectStorePlace(ID_STORE_PHONE);
	_ASSERT(bRet);
	bRet = m_pMemoFactory->GetMemoNum(&m_nUsedInPhone, &m_nTotalInPhone);
	_ASSERT(bRet);
	SetSMSPlace(ID_STORE_PHONE);
    if(m_bNew)
	{
		COleDateTime tPhone = COleDateTime::GetCurrentTime();
		tPhone -= theApp.m_tsPcPhone;
		
		m_dtpDate.SetTime( tPhone );
		m_dtpTime.SetTime( tPhone );
	}
	UpdateData(FALSE);
}

void CMemoDlg::OnSelchangeCmbMode() 
{
	// TODO: Add your control notification handler code here

	if((m_cmbMode.GetCurSel()+1) == MEMO_MODE_EVERYWEEK)
	{
		m_btnSun.EnableWindow(TRUE);
		m_btnMon.EnableWindow(TRUE);
		m_btnTue.EnableWindow(TRUE);
		m_btnWed.EnableWindow(TRUE);
		m_btnThu.EnableWindow(TRUE);
		m_btnFri.EnableWindow(TRUE);
		m_btnSat.EnableWindow(TRUE);
	}
	else
	{
		m_btnSun.EnableWindow(FALSE);
		m_btnMon.EnableWindow(FALSE);
		m_btnTue.EnableWindow(FALSE);
		m_btnWed.EnableWindow(FALSE);
		m_btnThu.EnableWindow(FALSE);
		m_btnFri.EnableWindow(FALSE);
		m_btnSat.EnableWindow(FALSE);

		m_btnSun.SetCheck(FALSE);
		m_btnMon.SetCheck(FALSE);
		m_btnTue.SetCheck(FALSE);
		m_btnWed.SetCheck(FALSE);
		m_btnThu.SetCheck(FALSE);
		m_btnFri.SetCheck(FALSE);
		m_btnSat.SetCheck(FALSE);
	}	
}


void CMemoDlg::SetModeFre(int Mode,BYTE fre)
{
	if(Mode == MEMO_MODE_EVERYWEEK)
	{
		m_btnSun.EnableWindow(TRUE);
		m_btnMon.EnableWindow(TRUE);
		m_btnTue.EnableWindow(TRUE);
		m_btnWed.EnableWindow(TRUE);
		m_btnThu.EnableWindow(TRUE);
		m_btnFri.EnableWindow(TRUE);
		m_btnSat.EnableWindow(TRUE);

		BYTE mask[7]= {MASK_B0,MASK_B1,MASK_B2,MASK_B3,MASK_B4,MASK_B5,MASK_B6};
		CButton * arrBtnPtr[7] = {&m_btnSun,&m_btnMon,&m_btnTue,&m_btnWed,&m_btnThu,&m_btnFri,&m_btnSat};
		for(int i=0;i<7; i++)
		{
			if(fre&mask[i])
			{
				arrBtnPtr[i]->SetCheck(TRUE);
			}
			else
			{
				arrBtnPtr[i]->SetCheck(FALSE);
			}
		}
	}
	else
	{
		m_btnSun.EnableWindow(FALSE);
		m_btnMon.EnableWindow(FALSE);
		m_btnTue.EnableWindow(FALSE);
		m_btnWed.EnableWindow(FALSE);
		m_btnThu.EnableWindow(FALSE);
		m_btnFri.EnableWindow(FALSE);
		m_btnSat.EnableWindow(FALSE);

		m_btnSun.SetCheck(FALSE);
		m_btnMon.SetCheck(FALSE);
		m_btnTue.SetCheck(FALSE);
		m_btnWed.SetCheck(FALSE);
		m_btnThu.SetCheck(FALSE);
		m_btnFri.SetCheck(FALSE);
		m_btnSat.SetCheck(FALSE);
	}
}

BYTE CMemoDlg::GetMemoFre(int Mode)
{
	BYTE fre = 0;
	if(Mode == MEMO_MODE_EVERYWEEK)
	{
		BYTE mask[7]= {MASK_B0,MASK_B1,MASK_B2,MASK_B3,MASK_B4,MASK_B5,MASK_B6};
		CButton * arrBtnPtr[7] = {&m_btnSun,&m_btnMon,&m_btnTue,&m_btnWed,&m_btnThu,&m_btnFri,&m_btnSat};
		for(int i=0;i<7; i++)
		{
			if(arrBtnPtr[i]->GetCheck())
			{
				fre = (BYTE)(fre | mask[i]);
			}	
		}
	}
	return fre;
}
