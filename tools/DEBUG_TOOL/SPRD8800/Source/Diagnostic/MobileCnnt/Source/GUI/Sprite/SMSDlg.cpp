// SMSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "DlgSelNum.h"
#include "DlgSelGroup.h"
#include "SMSDlg.h"
#include "SMSPresetDlg.h"
#include "InfoWrapper.h"
#include "ISMS.h"
#include "Converter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const DWORD CSMSDlg::SEND_FLAG      = 0x0001;
const DWORD CSMSDlg::SAVE_FLAG      = 0x0010;
const DWORD CSMSDlg::SEND_SAVE_FLAG = 0x0011;
/////////////////////////////////////////////////////////////////////////////
// CSMSDlg dialog


CSMSDlg::CSMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSDlg)
	m_strContent = _T("");
	m_strPhoneNum = _T("");
	m_nStore = 0;
	m_strInputNum = _T("");
	m_strLeftNum = _T("");
	m_bAutoSplish = FALSE;
	m_bStateReport = FALSE;
	m_strSmsNum = _T("");
	m_nCurSim = 0;
	//}}AFX_DATA_INIT
	
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

	m_EditMode    = SEM_UNKNOWN;
	m_pSMS        = NULL;
	m_pSMSFactory = NULL;
	m_pIW  = NULL;
	pDlgSendPrg   = NULL;
	m_bModified   = FALSE; 
	ZeroMemory(m_szBuf,sizeof(m_szBuf));
	
	m_bStore = FALSE;
	m_bTrunked = FALSE;

}

void CSMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSDlg)
	DDX_Control(pDX, IDC_EDT_CONTENT, m_edtContent);
	DDX_Control(pDX, IDC_BTN_GROUP, m_btnGroup);
	DDX_Control(pDX, IDC_STC_PLACE, m_stcPlace);
	DDX_Control(pDX, IDC_CMB_SENDSAVE, m_cmbSendSave);
	DDX_Control(pDX, IDC_SMS_CMB_STORE, m_cmbStore);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_PRESETSMS, m_btnPresetSMS);
	DDX_Control(pDX, IDC_BTN_PHONENUM, m_btnPhoneNum);
	DDX_Text(pDX, IDC_EDT_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_EDT_PHONENUM, m_strPhoneNum);	
	DDX_Text(pDX, IDC_STC_INPUT_NUM, m_strInputNum);
	DDX_Text(pDX, IDC_STC_LEAVE_NUM, m_strLeftNum);
	DDX_Check(pDX, IDC_CHK_AUTOSPLISH, m_bAutoSplish);
	DDX_Check(pDX, IDC_CHK_STATEREPORT, m_bStateReport);
	DDX_Text(pDX, IDC_STC_SMSNUM, m_strSmsNum);
	DDX_CBIndex(pDX, IDC_CMB_SMS_SIM_SEL, m_nCurSim);
	DDX_CBIndex(pDX, IDC_SMS_CMB_STORE, m_nStore);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSMSDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSDlg)
	ON_EN_UPDATE(IDC_EDT_PHONENUM, OnUpdateEdtPhonenum)
	ON_EN_VSCROLL(IDC_EDT_PHONENUM, OnVscrollEdtPhonenum)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_PRESETSMS, OnBtnPresetsms)
	ON_BN_CLICKED(IDC_BTN_PHONENUM, OnBtnPhoneNum)
	ON_CBN_SELCHANGE(IDC_CMB_SENDSAVE, OnSelchangeCmbSendsave)
	ON_BN_CLICKED(IDC_RDO_STORE, OnRdoStore)
	ON_BN_CLICKED(IDC_RDO_MS, OnRdoMs)
	ON_BN_CLICKED(IDC_RDO_SIM, OnRdoSim)
	ON_EN_CHANGE(IDC_EDT_CONTENT, OnChangeEdtContent)
	ON_BN_CLICKED(IDC_CHK_AUTOSPLISH, OnChkAutosplish)
	ON_BN_CLICKED(IDC_BTN_GROUP, OnBtnGroup)
	ON_BN_CLICKED(IDC_CHK_STATEREPORT, OnChkStatereport)
	ON_EN_CHANGE(IDC_EDT_PHONENUM, OnChangeEdtPhonenum)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSDlg message handlers

BOOL CSMSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWaitCursor waitCursor;

	InitCtrls();
	// TODO: Add extra initialization here
	
	m_pIW = theApp.GetInfoWrapper();
	_ASSERTE( m_pIW != NULL );

	m_pSMSFactory = theApp.m_pSMSFactory;
	_ASSERTE( m_pSMSFactory != NULL );
    int nLen=0;
	CString str,strContent;
	PSMS ps=NULL;
	switch( m_EditMode )
	{
	case SEM_EDIT:
		{
			m_strPhoneNum = m_pSMS->szPhone;
			if(m_pSMS->pNext==NULL)
				m_strContent  = m_pSMS->szContent;
			else
			{
				str = m_pSMS->szContent;
				strContent = str;
				ps = m_pSMS->pNext;
				while(ps!=NULL)
				{
					str = ps->szContent;
					strContent += str;
					ps=ps->pNext;
				}
				m_strContent  = strContent;
			}
			if(m_pSMS->bUcs2)
			{
				
				nLen = GetFullCharCount(m_strContent);
#ifdef _EGO
				if(!theApp.m_bSupportLongSMS)
				{
					if(nLen > MAX_SMS_UCS2_LEN)
					{
						nLen = MAX_SMS_UCS2_LEN;
						m_strContent = m_strContent.Left(MAX_SMS_UCS2_LEN);
					}
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strSmsNum.Format(_T("%2d"),1);
					m_strLeftNum.Format(_T("%3d"),MAX_SMS_UCS2_LEN-nLen);
					
				}
				else
				{
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_UCS2_LEN-nLen);
					if(nLen%67==0)
						m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) );
					else
						m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) + 1);
				}
#else
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_UCS2_LEN-nLen);
				if(nLen%67==0)
					m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) );
				else
					m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) + 1);
#endif 
			}
			else
			{
				nLen = GetFullCharCount(m_strContent);
#ifdef _EGO
				if(!theApp.m_bSupportLongSMS)
				{
					if(nLen > MAX_SMS_LEN)
					{
						nLen = MAX_SMS_LEN;
						m_strContent = m_strContent.Left(MAX_SMS_LEN);
					}
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strSmsNum.Format(_T("%2d"),1);
					m_strLeftNum.Format(_T("%3d"),MAX_SMS_LEN-nLen);
					
				}
				else
				{
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_LEN-nLen);
					if(nLen%SMS_CONTENT_MAX_LEN==0)
						m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) );
					else
						m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) + 1);
				}
#else
	
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_LEN-nLen);
				if(nLen%SMS_CONTENT_MAX_LEN==0)
					m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) );
				else
					m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) + 1);
#endif 
	
			}
			break;
		}
	case SEM_FORWARD:
		{
			if(m_pSMS->pNext==NULL)
				m_strContent  = m_pSMS->szContent;
			else
			{
				str = m_pSMS->szContent;
				strContent = str;
				ps = m_pSMS->pNext;
				while(ps!=NULL)
				{
					str = ps->szContent;
					strContent += str;
					ps=ps->pNext;
				}
				m_strContent  = strContent;
			}
			// 转发允许编辑
			//m_edtContent.EnableWindow(FALSE);
			//m_btnPresetSMS.EnableWindow(FALSE);


			if(m_pSMS->bUcs2)
			{
				
				nLen = GetFullCharCount(m_strContent);
#ifdef _EGO
				if(!theApp.m_bSupportLongSMS)
				{
					if(nLen > MAX_SMS_UCS2_LEN)
					{
						nLen = MAX_SMS_UCS2_LEN;
						m_strContent = m_strContent.Left(MAX_SMS_UCS2_LEN);
					}
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strSmsNum.Format(_T("%2d"),1);
					m_strLeftNum.Format(_T("%3d"),MAX_SMS_UCS2_LEN-nLen);
					
				}
				else
				{
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_UCS2_LEN-nLen);
					if(nLen%67==0)
						m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) );
					else
						m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) + 1);
				}
#else
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_UCS2_LEN-nLen);
				if(nLen%67==0)
					m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) );
				else
					m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN-3) + 1);
#endif 
			}
			else
			{
				nLen = GetFullCharCount(m_strContent);
#ifdef _EGO
				if(!theApp.m_bSupportLongSMS)
				{
					if(nLen > MAX_SMS_LEN)
					{
						nLen = MAX_SMS_LEN;
						m_strContent = m_strContent.Left(MAX_SMS_LEN);
					}
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strSmsNum.Format(_T("%2d"),1);
					m_strLeftNum.Format(_T("%3d"),MAX_SMS_LEN-nLen);
					
				}
				else
				{
					m_strInputNum.Format(_T("%3d"),nLen);
					m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_LEN-nLen);
					if(nLen%SMS_CONTENT_MAX_LEN==0)
						m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) );
					else
						m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) + 1);
				}
#else
	
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_LEN-nLen);
				if(nLen%SMS_CONTENT_MAX_LEN==0)
					m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) );
				else
					m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) + 1);
#endif 
	
			}
			break;
		}
	case SEM_ANSWER:
		m_strPhoneNum = m_pSMS->szPhone;

		//CWnd * pWnd = GetDlgItem(IDC_EDT_PHONENUM);
		//_ASSERTE( NULL != pWnd );
		//pWnd->EnableWindow(FALSE);

		//m_btnPhoneNum.EnableWindow(FALSE);

		break;
	
	}

	SetSMSPlace(ID_STORE_PC);
	m_nStore = 0;

	if( !theApp.TestIsConnected(FALSE) )
	{
		CComboBox * pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SENDSAVE);
		_ASSERTE( pCmb != NULL );
		// 离线模式下只能保存
		pCmb->SetCurSel(1);
		pCmb->EnableWindow(FALSE);

		// 离线模式下只能保存在pc中
		m_cmbStore.EnableWindow(FALSE);
		m_nStore = 0;

		//离线不能使用群发
		m_btnGroup.EnableWindow(FALSE);
		
		pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SMS_SIM_SEL);
		_ASSERTE( pCmb != NULL );
		pCmb->EnableWindow(FALSE);
	}
	else
	{
		if( m_pSMS != NULL)
		{
			if(	m_pSMS->wStore == ID_STORE_PHONE)
			{
				if(m_pSMSFactory->SelectStorePlace(SMS_SP_MS) )
				{
					m_pSMSFactory->QueryCapacity(m_nTotalInPhone, m_nUsedInPhone);
					SetSMSPlace(ID_STORE_PHONE);
				}

				m_nStore = 1;
			}		
			else if(m_pSMS->wStore == ID_STORE_SIM)
			{
				if(theApp.m_nMMIVer == MMI_VERSION_2 &&  theApp.m_nSIMCount > 1)
				{
					m_pSMSFactory->SetActiveSim(SMS_SP_SIM);
				}

				if(m_pSMSFactory->SelectStorePlace(SMS_SP_SIM) )
				{
					m_pSMSFactory->QueryCapacity(m_nTotalInSIM, m_nUsedInSIM);
					SetSMSPlace(ID_STORE_SIM);
				}

				m_nStore = 2;
			}
			else if(m_pSMS->wStore == ID_STORE_SIM2 && m_pSMSFactory->SetActiveSim(SMS_SP_SIM2))
			{
				if(m_pSMSFactory->SelectStorePlace(SMS_SP_SIM2) )
				{
					m_pSMSFactory->QueryCapacity(m_nTotalInSIM2, m_nUsedInSIM2);
					SetSMSPlace(ID_STORE_SIM2);
				}

				m_nStore = 3;
			}
			else if(m_pSMS->wStore == ID_STORE_SIM3 && m_pSMSFactory->SetActiveSim(SMS_SP_SIM3))
			{
				if(m_pSMSFactory->SelectStorePlace(SMS_SP_SIM3) )
				{
					m_pSMSFactory->QueryCapacity(m_nTotalInSIM3, m_nUsedInSIM3);
					SetSMSPlace(ID_STORE_SIM3);
				}

				m_nStore = 4;
			}
			else if(m_pSMS->wStore == ID_STORE_SIM4 && m_pSMSFactory->SetActiveSim(SMS_SP_SIM4))
			{
				if(m_pSMSFactory->SelectStorePlace(SMS_SP_SIM4) )
				{
					m_pSMSFactory->QueryCapacity(m_nTotalInSIM4, m_nUsedInSIM4);
					SetSMSPlace(ID_STORE_SIM4);
				}

				m_nStore = 4;
			}
		}		
	}
#ifdef _EGO
	if(!theApp.m_bSupportLongSMS)
	{

		GetDlgItem(IDC_CHK_AUTOSPLISH)->ShowWindow(SW_HIDE);
		m_bAutoSplish = FALSE;
		m_edtContent.SetLimitText(MAX_SMS_LEN);
		m_edtContent.SetSMSInput(FALSE);
	}
	else
	{
		m_bAutoSplish = TRUE;
	}
#else
	m_bAutoSplish = TRUE;
#endif 
	m_bStateReport = FALSE;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSMSDlg::InitCtrls()
{
	//VERIFY( m_edtContent.SubclassDlgItem(IDC_EDT_CONTENT, this) );
	m_edtContent.EnableHover(FALSE);
	m_edtContent.SetLimitText(MAX_LONG_SMS_LEN);
	m_edtContent.SetSMSInput();

	VERIFY( m_edtPhoneNum.SubclassDlgItem(IDC_EDT_PHONENUM, this) );

	InitCmbBox();
}

void CSMSDlg::SetSMSPlace(int nStore)
{
	CString strFormat;
	VERIFY( strFormat.LoadString(IDS_SMS_PLACE) );

	CString strPrompt;
	switch( nStore )
	{
	case ID_STORE_SIM4:
		strPrompt.Format(strFormat, m_nUsedInSIM4, m_nTotalInSIM4 - m_nUsedInSIM4);
		break;
	case ID_STORE_SIM3:
		strPrompt.Format(strFormat, m_nUsedInSIM3, m_nTotalInSIM3 - m_nUsedInSIM3);
		break;
	case ID_STORE_SIM2:
		strPrompt.Format(strFormat, m_nUsedInSIM2, m_nTotalInSIM2 - m_nUsedInSIM2);
		break;
	case ID_STORE_SIM:
		strPrompt.Format(strFormat, m_nUsedInSIM, m_nTotalInSIM - m_nUsedInSIM);
		break;
	case ID_STORE_PHONE:
		strPrompt.Format(strFormat, m_nUsedInPhone, m_nTotalInPhone - m_nUsedInPhone);
		break;
	}

	m_stcPlace.SetWindowText(strPrompt);
}

void CSMSDlg::OnUpdateEdtPhonenum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CWnd * pWnd = GetDlgItem(IDC_EDT_PHONENUM);
	_ASSERTE( pWnd != NULL );

	pWnd->Invalidate();
}

void CSMSDlg::OnVscrollEdtPhonenum() 
{
	// TODO: Add your control notification handler code here
	CWnd * pWnd = GetDlgItem(IDC_EDT_PHONENUM);
	_ASSERTE( pWnd != NULL );

	pWnd->Invalidate();
}

void CSMSDlg::InitCmbBox()
{
	CString strValue;
	VERIFY( strValue.LoadString(IDS_SMS_OPTION) );

	_TCHAR cSep = _T(',');
	CStringArray arrText;

	int    nLen  = strValue.GetLength();
	LPTSTR pBuf  = strValue.GetBuffer(nLen);
	LPTSTR pFind = _tcschr(pBuf, cSep);
	for( ; pFind != NULL; pFind = _tcschr(pBuf, cSep) )
	{
		*pFind = _T('\0');
		arrText.Add( pBuf );
		pBuf = pFind + 1;
	}
	arrText.Add( pBuf );
	strValue.ReleaseBuffer(nLen);

	int nCount = arrText.GetSize();
	DWORD arrSendSave[] = { SEND_FLAG, SAVE_FLAG, SEND_SAVE_FLAG };
	_ASSERTE( nCount == sizeof(arrSendSave)/sizeof(arrSendSave[0]) );

	CComboBox * pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SENDSAVE);
	_ASSERTE( pCmb != NULL );
	for( int i = 0; i < nCount; ++i )
	{
		pCmb->AddString(arrText[i]);
		pCmb->SetItemData(i, arrSendSave[i]);
	}
	pCmb->SetCurSel(0); // send

	// init send valid date
	VERIFY( strValue.LoadString(IDS_SMS_VALID_DATE) );
	
	arrText.RemoveAll();

	nLen = strValue.GetLength();
	pBuf = strValue.GetBuffer(nLen);
	pFind = _tcschr(pBuf, _T(','));
	for(; pFind != NULL; pFind = _tcschr(pBuf, _T(',')) )
	{
		*pFind = _T('\0');
		arrText.Add(pBuf);
		pBuf = pFind + 1;
	}
	arrText.Add(pBuf);
	strValue.ReleaseBuffer(nLen);

	nCount = arrText.GetSize();

	DWORD arrData[] = { 1, 6, 24, 72, 168 };
	_ASSERTE( sizeof(arrData)/sizeof(arrData[0]) == nCount );

	BOOL bSame = FALSE;
	pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SEND_VALID_DATE);
	_ASSERTE( pCmb != NULL );
	for( i = 0; i < nCount; ++i )
	{
		pCmb->AddString(arrText[i]);
		pCmb->SetItemData(i, arrData[i]);
		if( m_pSMS != NULL && m_pSMS->nValidDate == (int)arrData[i] )
		{
			bSame = TRUE;
			pCmb->SetCurSel(i);
		}
	}
    // 
	if( !bSame )
	{
		pCmb->SetCurSel( 2 ); // 24 hours
	}
	

	// init send sms select sim 
	m_mapSimIndex.RemoveAll();
	VERIFY( strValue.LoadString(IDS_SMS_SEND_SIM_SEL) );
	
	arrText.RemoveAll();
	
	nLen = strValue.GetLength();
	pBuf = strValue.GetBuffer(nLen);
	pFind = _tcschr(pBuf, _T(','));
	for(; pFind != NULL; pFind = _tcschr(pBuf, _T(',')) )
	{
		*pFind = _T('\0');
		arrText.Add(pBuf);
		pBuf = pFind + 1;
	}
	arrText.Add(pBuf);
	strValue.ReleaseBuffer(nLen);
	nCount = arrText.GetSize();
	DWORD arrSendSimSel[MAX_SIM_NUM] = { 1, 2, 3, 4};
	pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SMS_SIM_SEL);
	_ASSERTE( pCmb != NULL );
	int nIdex = 0;
	for( i = 0; i < nCount; ++i )
	{
		if(theApp.IsPluggedSIM(i+1))
		{
			pCmb->AddString(arrText[i]);
			pCmb->SetItemData(nIdex, arrSendSimSel[i]);	
			m_mapSimIndex.SetAt(i+1,nIdex);
			nIdex++;
		}
	}

	m_nCurSim=0;

	int nCurSim = 0;

	if(m_pSMS != NULL)
	{
		if(m_pSMS->wStore == ID_STORE_SIM4 )
		{
			nCurSim = 0;			
			if(m_mapSimIndex.Lookup(4,nCurSim))
			{
				m_nCurSim = nCurSim;
			}

		}
		else if(m_pSMS->wStore == ID_STORE_SIM3 )
		{
			nCurSim = 0;			
			if(m_mapSimIndex.Lookup(3,nCurSim))
			{
				m_nCurSim = nCurSim;
			}

		}
		else if(m_pSMS->wStore == ID_STORE_SIM2 )
		{
			nCurSim = 0;			
			if(m_mapSimIndex.Lookup(2,nCurSim))
			{
				m_nCurSim = nCurSim;
			}
		}
	}

	if( theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM4 )
	{
		nCurSim = 0;			
		if(m_mapSimIndex.Lookup(4,nCurSim))
		{
			m_nCurSim = nCurSim;
		}
		pCmb->EnableWindow(FALSE);
	}
	else if( theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM3 )
	{
		nCurSim = 0;			
		if(m_mapSimIndex.Lookup(3,nCurSim))
		{
			m_nCurSim = nCurSim;
		}
		pCmb->EnableWindow(FALSE);
	}	
	else if( theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM2 )
	{
		nCurSim = 0;			
		if(m_mapSimIndex.Lookup(2,nCurSim))
		{
			m_nCurSim = nCurSim;
		}
		pCmb->EnableWindow(FALSE);
	}
	else if( theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM1 || theApp.m_nSIMState == SIM_PLUGED_SINGLE )
	{
		nCurSim = 0;	
		pCmb->EnableWindow(FALSE);
	}

	//Init Store Place
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
	pFind = _tcschr(szValue, cFlag);
	m_cmbStore.ResetContent();
    for( int k = 0; pFind != NULL; ++k)
	{
		*pFind = '\0';      
		CString strSP = pText;	
		pText = pFind + 1;
		pFind = _tcschr(pText, cFlag);
		m_cmbStore.AddString(strSP);	
	//	m_cmbStore.SetItemData(k,STORE_IN_PC+k);

	}
	m_cmbStore.SetCurSel(0);
	m_nStore = 0;
}

void CSMSDlg::OnBtnPhoneNum()
{
    UpdateData(TRUE);

	CDlgSelNum dlg(theApp.m_pSMSFrame);
	if( dlg.DoModal() == IDOK )
	{
		PPB pPb    = NULL;
		int nCount = dlg.m_arrSel.GetSize();
		for( int i = 0; i < nCount; ++i )
		{
			if( m_mapNameToPhone.GetCount() > 0 && !m_strPhoneNum.IsEmpty())
				m_strPhoneNum += _T(";");

			pPb = m_pIW->GetPB(dlg.m_arrSel[i]);
			/*去掉电话输入框显示名字，保留此代码以待将来又要此功能:( */
		    //if( _tcslen(pPb->szName) > 0 )
			//{
			//	m_mapNameToPhone.SetAt( pPb->szName, pPb->szPhone );
			//	m_strPhoneNum += pPb->szName;
			//}
			//else
			//{
				m_mapNameToPhone.SetAt( pPb->szPhone, pPb->szPhone );
				m_strPhoneNum += pPb->szPhone;
			//}
		}
		UpdateData(FALSE);
	}
}

void CSMSDlg::OnBtnPresetsms() 
{
	// TODO: Add your control notification handler code here
	//
	UpdateData();

	CSMSPresetDlg dlg(theApp.m_pSMSFrame);

	dlg.EnableSelect();

	if( dlg.DoModal() == IDOK )
	{
		_ASSERTE( m_pIW != NULL );

		PPRESETSMS pPSMS = m_pIW->GetCurSelPresetSMS();

        int nMaxNum = 0xffff;
        int nOldNum = GetFullCharCount(m_strContent);
        int nNewNum = GetFullCharCount(pPSMS->szContent);
		BOOL bUcs = FALSE;
        if( HasUcs2Char((LPCTSTR)m_strContent) || m_edtContent.HasUcs2Char(pPSMS->szContent))
        {
			if(m_bAutoSplish)
				nMaxNum = MAX_LONG_SMS_UCS2_LEN;
			else
				nMaxNum = MAX_SMS_UCS2_LEN;

			if(m_edtContent.HasUcs2Char(pPSMS->szContent))
				bUcs = TRUE;

        }
        else
        {
			if(m_bAutoSplish)
				nMaxNum = MAX_LONG_SMS_LEN;
			else
				nMaxNum = MAX_SMS_LEN;
        }

        if( nOldNum + nNewNum > nMaxNum )
        {
            if(AfxMessageBox(IDS_SMS_LEN_INVALID,MB_OKCANCEL)==IDCANCEL)
				return;

			if(nMaxNum - nOldNum == 0)
				return;
			
			/*
			if(bUcs)
			{
				
				LPTSTR pNext = NULL;
				CString str(pPSMS->szContent);
				LPTSTR pStr = (LPTSTR)(LPCTSTR)str;
				int nNum=0;

				while( *pStr )
				{
					pNext = CharNext(pStr);					
					pStr = pNext;
					nNum++;
					if(nNum ==  (nMaxNum - nOldNum) )
					{		
						*pStr = '\0';
						break;
					}
					
				}

				m_strContent += str;			

			
			}
			else
			{
				CString strtemp(pPSMS->szContent);
				LPSTR pStr = (LPSTR)(LPCTSTR)strtemp;
				*(pStr+(nMaxNum - nOldNum)) = '\0';
				
				m_strContent += strtemp;
			}
			*/

			m_strContent += pPSMS->szContent;
			m_strContent = m_strContent.Left(nMaxNum);

        }
        else
		{
			m_strContent += pPSMS->szContent;
		}
		

        
		UpdateData(FALSE);

		OnChangeEdtContent();
	}

}

BOOL CSMSDlg::IsValidateData()
{
	//  这里不对号码的长度作判断，因为号码的长度有长有短，并不统一
	m_strPhoneNum.TrimLeft();
	m_strPhoneNum.TrimRight();

	if( m_strPhoneNum.IsEmpty() )
	{
		return EmptyPrompt(IDC_STC_PHONENUM);
	}

	m_arrName.RemoveAll();
	
	CString strName;
	LPCTSTR pBuf = m_strPhoneNum.GetBuffer(0);
	LPTSTR pFind = _tcschr(pBuf, _T(';'));
	while( pFind != NULL )
	{
		*pFind = '\0';
		strName = pBuf;
		strName.TrimLeft();
		strName.TrimRight();
		
		if( !StrIsValidate(strName) )
		{
			m_strPhoneNum.ReleaseBuffer();
			return FALSE;
		}
		if(!strName.IsEmpty())
		{
			m_arrName.Add(strName);
		}		
		*pFind = _T(';');
		pBuf = pFind + 1;
		pFind = _tcschr(pBuf, _T(';'));
	}
	strName = pBuf;
	strName.TrimLeft();
	strName.TrimRight();
	if( !StrIsValidate(strName) )
	{
		m_strPhoneNum.ReleaseBuffer();
		return FALSE;
	}

	if(!strName.IsEmpty())
	{
		m_arrName.Add(strName);
	}	
	m_strPhoneNum.ReleaseBuffer();

	// 允许发送空短信
	//if( m_strContent.IsEmpty() )
	//{
	//	return EmptyPrompt(IDC_STC_CONTENT);
	//}
    
	//对于Edit一个SMS，查看是否改变原有的
	//若改变则置m_bModified

	if((m_EditMode == SEM_EDIT||m_EditMode== SEM_FORWARD) && m_pSMS != NULL)
	{
		 if(_tcscmp(m_pSMS->szPhone,m_strPhoneNum)!= 0 ||
			_tcscmp(m_pSMS->szContent,m_strContent) != 0 )
			m_bModified = TRUE;		
	}
	if(m_pSMS == NULL)
	{
		if(!m_strPhoneNum.IsEmpty() || !m_strContent.IsEmpty())
			m_bModified = TRUE;
	}

	return TRUE;
}

void CSMSDlg::OnBtnOk()
{
	UpdateData(TRUE);
	
	if( !IsValidateData() )
		return;

	m_bSimSelCmdEnable = GetDlgItem(IDC_CMB_SMS_SIM_SEL)->IsWindowEnabled();
	SetTimer(1,200,NULL);
	ModifyCtrls(FALSE);

	DWORD dwThreadID = 0;
	/*HANDLE hThread = */CreateThread(NULL,
        0,
        (LPTHREAD_START_ROUTINE)GetThreadFunc,
        (LPVOID)this,
        NULL,
        &dwThreadID);

/*

	CWaitCursor waitCursor;
	CString strTitle;
	strTitle.LoadString(IDS_SMS_TITLE);
	CString strMsg;

	//拆分内容
	int	nLen = GetFullCharCount(m_strContent);
	//test
	m_arrName.GetSize();
	//test

	if(!m_bAutoSplish)
	{
		if((HasUcs2Char((LPCTSTR)m_strContent) && nLen>MAX_SMS_UCS2_LEN) ||			
			(!HasUcs2Char((LPCTSTR)m_strContent) && nLen>MAX_SMS_LEN))
		{	
			if(AfxMessageBox(IDS_WARN_SPLISH,MB_OKCANCEL)==IDCANCEL)
				return;
		}
	}
	int nItem = SplishContent();
	UNREFERENCED_PARAMETER(nItem);

	CComboBox * pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SENDSAVE);
	_ASSERTE( pCmb != NULL );

	DWORD dwData = pCmb->GetItemData(pCmb->GetCurSel());
	CString strName;
	CString strPhoneNum;
	int nCount = m_arrName.GetSize();
	if(nCount == 0)
	{
		EmptyPrompt(IDC_STC_PHONENUM);
		return;
	}
	BOOL bSave = FALSE;
	BOOL bSend = FALSE;

	BOOL bActiveSIM = TRUE;

	if( dwData & SEND_FLAG )
	{
		CComboBox * pCmbSim = (CComboBox *)GetDlgItem(IDC_CMB_SMS_SIM_SEL);
		DWORD dwSimNum = pCmbSim->GetItemData(pCmbSim->GetCurSel());
		if(theApp.m_nMMIVer == MMI_VERSION_2)
		{	
			bActiveSIM = m_pSMSFactory->SetActiveSim((_SMS_STORE_PLACE_E)dwSimNum);		
		}
	}

	if( (dwData & SEND_FLAG) && bActiveSIM )
	{
		for( int i = 0; i < nCount; ++i )
		{
			bSave   = FALSE;
			strName = m_arrName[i];
			if( StrIsNumeric(strName) )
			{
				strPhoneNum = strName;
				strName.Empty();
			}
			else
			{
				VERIFY( m_mapNameToPhone.Lookup(strName, strPhoneNum) );
			}
			
			BOOL bMore = FALSE;

#ifdef SPRD8800
			if(i < nCount - 1)
			{
				bMore = TRUE;
			}
			else
			{
				bMore = FALSE;
			}
#endif

			waitCursor.Restore();

			SetTimer(1,200,NULL);			
			// send sms
			bSend = SendSMS(strPhoneNum, bMore);
			KillTimer(1);
			GetDlgItem(IDC_STC_SENDING)->SetWindowText(_T(" "));
			if( !bSend )
			{
				theApp.TestIsConnected();
				strMsg.LoadString(IDS_ERR_SEND_FAIL);
				if( !(dwData & SAVE_FLAG) && m_bModified &&
					::MessageBox(theApp.m_pSMSFrame->m_hWnd, strMsg,strTitle, MB_YESNO | MB_ICONWARNING) == IDYES )
				{
					bSave = TRUE;
					break;
				}
			}	
		}
	}
	else if(!bActiveSIM)
	{
		AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
	}
*/
// 	if( dwData & SAVE_FLAG || bSave/* || (bSend && m_EditMode == SEM_EDIT)*/)
// 	{
// 		strName = m_arrName[0];
// 		if( StrIsNumeric(strName) )
// 		{
// 			strPhoneNum = strName;
// 			strName.Empty();
// 		}
// 		else
// 		{
// 			VERIFY( m_mapNameToPhone.Lookup(strName, strPhoneNum) );
// 		}
// 		// save sms
// 		if( /*m_bModified && */!IsExistInPc(strPhoneNum, m_strContent))
// 			SaveSMS(strName, strPhoneNum, bSend);
// 	    m_bModified = FALSE;
// 		CDialog::OnOK();
// 	}


}

void CSMSDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 这里不需添加任何代码，也不能删除。否则焦点按回车键可能会导致调用此函数
}

void CSMSDlg::OnSelchangeCmbSendsave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nCurSel = m_cmbSendSave.GetCurSel();
	DWORD dwData = m_cmbSendSave.GetItemData(nCurSel);

	if( dwData & SAVE_FLAG )
	{
		m_nStore = 0;
		CWnd * pWnd = GetDlgItem(IDC_RDO_MS);
		_ASSERTE( pWnd != NULL );

		pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_RDO_SIM);
		_ASSERTE( pWnd != NULL );

		pWnd->EnableWindow(FALSE);

		UpdateData(FALSE);
	}
}

void CSMSDlg::OnRdoStore() 
{
	// TODO: Add your control notification handler code here
	SetSMSPlace(ID_STORE_PC);
}

void CSMSDlg::OnRdoMs() 
{
	// TODO: Add your control notification handler code here
	CWaitCursor waitCursor;

	if( m_nUsedInPhone == 0 && m_nTotalInPhone == 0 &&
		m_pSMSFactory->SelectStorePlace(SMS_SP_MS) )
	{
		m_pSMSFactory->QueryCapacity(m_nTotalInPhone, m_nUsedInPhone);
	}		
	SetSMSPlace(ID_STORE_PHONE);
}

void CSMSDlg::OnRdoSim() 
{
	// TODO: Add your control notification handler code here
	CWaitCursor waitCursor;

	if( m_nUsedInSIM ==0 && m_nTotalInSIM == 0 && 
		m_pSMSFactory->SelectStorePlace(SMS_SP_SIM) )
	{
		m_pSMSFactory->QueryCapacity(m_nTotalInSIM, m_nUsedInSIM);
	}
	SetSMSPlace(ID_STORE_SIM);
}

BOOL CSMSDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR )
	{
		CWnd * pWnd = GetFocus();
		if( pWnd != NULL && pWnd->m_hWnd == m_edtPhoneNum.m_hWnd )
		{
			_TCHAR cValue = (_TCHAR)(pMsg->wParam);
			if( cValue < _T('0') || cValue > _T('9') )
			{
				if( cValue != _T('+') && cValue != _T(';') && cValue != VK_BACK  && 
					cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
				{
					return TRUE;
				}	
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//DEL void CSMSDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	CWnd * pWnd = GetDlgItem(IDC_EDT_CONTENT);
//DEL 	_ASSERTE( pWnd != NULL );
//DEL 
//DEL 	pWnd->Invalidate();
//DEL 
//DEL 	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
//DEL }

BOOL CSMSDlg::IsExistInPc(LPCTSTR pszPhoneNum, LPCTSTR pszContent)
{
	_ASSERTE( pszPhoneNum != NULL && pszContent != NULL );

//	if( m_pSMS != NULL && m_pSMS->wStore != ID_STORE_PC )
//		return FALSE;

	PSMS psms = NULL;
	int nCount = m_pIW->GetSMSCount(ID_STORE_ALL,FALSE);
	for( int i = 0; i < nCount; i++ )
	{
		psms = m_pIW->GetSMS( i );

		if( psms->wStore == ID_STORE_PC &&
			_tcscmp(psms->szPhone, pszPhoneNum) == 0 &&
			_tcscmp(psms->szContent, pszContent) == 0 )
		{
			CString strTitle;
			strTitle.LoadString(IDS_SMS_TITLE);
			CString strMsg;
			strMsg.LoadString(IDS_SMS_EXIST_IN_PC);
			if(::MessageBox(theApp.m_pSMSFrame->m_hWnd,strMsg,strTitle,MB_YESNO|MB_ICONWARNING) == IDYES )
				return  FALSE;
			else
				return TRUE;
		}
	}

	return FALSE;
}

#ifndef  _UNICODE

BOOL CSMSDlg::HasUcs2Char(LPCSTR pStr) const
{
	_ASSERTE( pStr != NULL );

	LPCSTR pNext = NULL;
	while( *pStr )
	{
		pNext = CharNextExA(CP_ACP,pStr,0);
		if( pNext - pStr > 1 )
			return TRUE;

		pStr = pNext;
	}

	return FALSE;
}
#else

BOOL CSMSDlg::HasUcs2Char(LPCTSTR pStr) const
{
	_ASSERTE( pStr != NULL );
    char *pChar = (char *)pStr;
	int nLen = _tcslen(pStr);
	int i=0;
	for(i=0; i<nLen*2; i=i+2)
	{
		if(!(pChar[i] < (UCHAR)0x80 && pChar[i+1] == 0)) 
			break;
	}
	
	if(i>=nLen*2)
	{
		return FALSE;
	}
	
	return TRUE;
}
#endif

void CSMSDlg::OnChangeEdtContent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

    int nLen =0;
	if(m_bAutoSplish)
	{
		if(HasUcs2Char(m_strContent))
		{
			nLen = GetFullCharCount(m_strContent);			
			m_strInputNum.Format(_T("%3d"),nLen);
			m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_UCS2_LEN-nLen);
			if(nLen%(MAX_SMS_UCS2_LEN -3)==0)
				m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN -3));
			else
				m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN -3) + 1);
		}
		else
		{			
			nLen = GetFullCharCount(m_strContent);	
			m_strInputNum.Format(_T("%3d"),nLen);
			m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_LEN-nLen);
			if(nLen%SMS_CONTENT_MAX_LEN==0)
				m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) );
			else
				m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) + 1);
		}
	}
	else
	{
		if(HasUcs2Char(m_strContent))
		{
			nLen = GetFullCharCount(m_strContent);
			m_strInputNum.Format(_T("%3d"),nLen);
			m_strLeftNum.Format(_T("%3d"),MAX_SMS_UCS2_LEN-nLen);
			m_strSmsNum.Format(_T("%2d"),1 );
		}
		else
		{
			nLen = GetFullCharCount(m_strContent);
			m_strInputNum.Format(_T("%3d"),nLen);
			m_strLeftNum.Format(_T("%3d"),MAX_SMS_LEN-nLen);
			m_strSmsNum.Format(_T("%2d"),1 );
		}
	}
	CWnd *pWnd = GetDlgItem(IDC_STC_SMSNUM);
	pWnd->SetWindowText(m_strSmsNum);
	pWnd = GetDlgItem(IDC_STC_INPUT_NUM);
	pWnd->SetWindowText(m_strInputNum);
	pWnd = GetDlgItem(IDC_STC_LEAVE_NUM);
	pWnd->SetWindowText(m_strLeftNum);
	//UpdateData(FALSE);

}

void CSMSDlg::OnChkAutosplish() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	int nLen =0;
	if(m_bAutoSplish)
	{
		if(m_strContent.GetLength()>0)
		{
			if(HasUcs2Char(m_strContent))
			{
				nLen = GetFullCharCount(m_strContent);			
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_UCS2_LEN-nLen);
				if(nLen%(MAX_SMS_UCS2_LEN -3)==0)
					m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN -3));
				else
					m_strSmsNum.Format(_T("%2d"),nLen/(MAX_SMS_UCS2_LEN -3) + 1);
			}
			else
			{
				nLen = GetFullCharCount(m_strContent);	
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_LONG_SMS_LEN-nLen);
				if(nLen%SMS_CONTENT_MAX_LEN==0)
					m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) );
				else
					m_strSmsNum.Format(_T("%2d"),nLen/(SMS_CONTENT_MAX_LEN) + 1);
			}
			
		}
		UpdateData(FALSE);
		m_edtContent.SetLimitText(MAX_LONG_SMS_LEN);
		m_edtContent.SetSMSInput(TRUE);
	}
	else
	{
		if(m_strContent.GetLength()>0)
		{
			BOOL bHasUcs2Char = HasUcs2Char(m_strContent);
			if(m_strContent.GetLength()> (bHasUcs2Char?MAX_SMS_UCS2_LEN:MAX_SMS_LEN))
			{
				CString strTitle;
				CString strWarn;
				strWarn.LoadString(IDS_WARN_SPLISH);
				this->GetWindowText(strTitle);
				if(MessageBox(strWarn,strTitle,MB_OKCANCEL| MB_ICONWARNING)==IDCANCEL)
				{
					m_bAutoSplish = TRUE;
					UpdateData(FALSE);
					return;
				}
			}


			m_strContent = GetStrLeft(m_strContent,
				bHasUcs2Char?MAX_SMS_UCS2_LEN:MAX_SMS_LEN);

			if(HasUcs2Char(m_strContent))
			{
				nLen = GetFullCharCount(m_strContent);
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_SMS_UCS2_LEN-nLen);
				m_strSmsNum.Format(_T("%2d"),1 );
			}
			else
			{
				nLen = GetFullCharCount(m_strContent);
				m_strInputNum.Format(_T("%3d"),nLen);
				m_strLeftNum.Format(_T("%3d"),MAX_SMS_LEN-nLen);
				m_strSmsNum.Format(_T("%2d"),1 );
			}
			
		}
		UpdateData(FALSE);
		m_edtContent.SetLimitText(MAX_SMS_LEN);
		m_edtContent.SetSMSInput(FALSE);
	}
}

void CSMSDlg::OnBtnGroup() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

	CDlgSelGroup dlg(theApp.m_pSMSFrame);
	if( dlg.DoModal() == IDOK )
	{
		PPB pPb    = NULL;
		int nCount = dlg.m_arrSel.GetSize();
		for( int i = 0; i < nCount; ++i )
		{
			if( m_mapNameToPhone.GetCount() > 0 && !m_strPhoneNum.IsEmpty())
				m_strPhoneNum += _T(";");

			pPb = m_pIW->GetPB(dlg.m_arrSel[i]);
//		    if( _tcslen(pPb->szName) > 0 )
//			{
//				m_mapNameToPhone.SetAt( pPb->szName, pPb->szPhone );
//				m_strPhoneNum += pPb->szName;
//			}
//			else
// 			{
				m_mapNameToPhone.SetAt( pPb->szPhone, pPb->szPhone );
				m_strPhoneNum += pPb->szPhone;
// 			}
		}
		UpdateData(FALSE);
	}	
}

int  CSMSDlg::SplishContent()
{
	m_arrContent.RemoveAll();

	int nLen=0;
	int nLeftLen =0;
	int nItem =0;
	int i=0;
	nLen = GetFullCharCount(m_strContent);
	CString strItem;
	if(!m_bAutoSplish)
	{
		if(HasUcs2Char(m_strContent) && nLen>MAX_SMS_UCS2_LEN)
			m_arrContent.Add(GetStrLeft(m_strContent,MAX_SMS_UCS2_LEN));
		else if(!HasUcs2Char(m_strContent) && nLen>MAX_SMS_LEN)
			m_arrContent.Add(GetStrLeft(m_strContent,SMS_CONTENT_MAX_LEN));
		else
			m_arrContent.Add(m_strContent);
	}
	else
	{
		if(HasUcs2Char(m_strContent))
		{
			if(nLen%(MAX_SMS_UCS2_LEN-3)==0)
				nItem = nLen/(MAX_SMS_UCS2_LEN-3);
			else
				nItem = nLen/(MAX_SMS_UCS2_LEN-3) +1;
			nLeftLen = nLen;
			strItem = m_strContent;
			for(i=0;i < nItem; i++)
			{
				nLeftLen -=(MAX_SMS_UCS2_LEN-3);
				if(nLeftLen>=0)
				{
					m_arrContent.Add(GetStrLeft(strItem,(MAX_SMS_UCS2_LEN-3)));
					strItem = GetStrRight(strItem,nLeftLen);
				}
				else
					m_arrContent.Add(strItem);
			}
		}
		else
		{
			if(nLen%SMS_CONTENT_MAX_LEN==0)
				nItem = nLen/SMS_CONTENT_MAX_LEN;
			else
				nItem = nLen/SMS_CONTENT_MAX_LEN +1;
			nLeftLen = nLen;
			strItem = m_strContent;
			//teana hu 2009.06.04
			if(nItem == 1)
			{
				m_arrContent.Add(strItem);
			}
			else
			{
				for(i=0;i < nItem; i++)
				{
					nLeftLen -=(SMS_CONTENT_MAX_LEN-7);
					if(nLeftLen>=0)
					{
						m_arrContent.Add(GetStrLeft(strItem,(SMS_CONTENT_MAX_LEN-7)));
						strItem = GetStrRight(strItem,nLeftLen);
					}
					else
						m_arrContent.Add(strItem);
				}
			}
		}
	}

	return nItem;
}

LPTSTR CSMSDlg::GetStrLeft(LPCTSTR pStr,int nCount)
{
	_ASSERT(pStr != NULL);
	_ASSERT(nCount <= GetFullCharCount(pStr));
	LPTSTR pNext = NULL;
	ZeroMemory(m_szBuf,sizeof(m_szBuf));
	LPTSTR pDst = m_szBuf;
	if(pDst == NULL)
		return NULL;
	memset(pDst,'\0',nCount*2 +4);
	_TCHAR *p = pDst;
	for(int i =0; i<nCount;i++)
	{
		pNext = CharNext(pStr);
		while(pStr!=pNext)
		{
			*p = *pStr;
			p++;
			pStr++;
		}
		pStr = pNext;
	}
	return pDst;
}

LPTSTR CSMSDlg::GetStrRight(LPCTSTR pStr,int nCount)
{
	_ASSERT(pStr != NULL);
	_ASSERT(nCount <= GetFullCharCount(pStr));

	LPTSTR pNext = NULL;
	ZeroMemory(m_szBuf,sizeof(m_szBuf));
	LPTSTR pDst = m_szBuf;
	if(pDst == NULL)
		return NULL;
	memset(pDst,'\0',nCount*2 +4);
	_TCHAR *p = pDst;
	int nLeft = GetFullCharCount(pStr) - nCount;
	for(int i = 0;i< nLeft;i++)
		pStr = CharNext(pStr);

	for(int j =0; j<nCount;j++)
	{
		pNext = CharNext(pStr);
		while(pStr!=pNext)
		{
			*p = *pStr;
			p++;
			pStr++;
		}
		pStr = pNext;
	}
	return pDst;

}

void CSMSDlg::OnChkStatereport() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CSMSDlg::OnChangeEdtPhonenum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	if(m_strPhoneNum.GetLength()<=20)
		return;
	int nSelStart, nSelEnd;
	((CEdit*)GetDlgItem(IDC_EDT_PHONENUM))->GetSel(nSelStart, nSelEnd);
    BOOL bBack = FALSE;
	_TCHAR *pCur = NULL;
	_TCHAR *pNext = NULL;
	_TCHAR *pCursor = NULL;
	pCur = m_strPhoneNum.GetBuffer(10);
	pNext = _tcschr(pCur,_T(';'));
	pCursor = pCur + nSelStart;
	while(pNext)
	{
		if(pNext - pCur > MAX_PHONE_NUM_LEN)
		{			
            if(pCursor<pNext && pCursor > pCur)
			{
				_tcscpy(pCursor-1,pCursor);
				pCur = pNext + 1;
				bBack = TRUE;
			}
			else
			{
				pCur += MAX_PHONE_NUM_LEN;
				_tcscpy(pCur,pNext);
				pCur += 1;
			}
            pNext = _tcschr(pCur,_T(';'));
		}
		else
		{
			pCur = pNext+1;
            pNext = _tcschr(pCur,_T(';'));
		}
	}
	if(pNext == NULL && pCur != NULL)
	{
		if(_tcslen(pCur)>MAX_PHONE_NUM_LEN)
		{
			*(pCur+MAX_PHONE_NUM_LEN) = _T(';');
			*(pCur+MAX_PHONE_NUM_LEN +1 ) = _T('\0');
		}
	}
	m_strPhoneNum.ReleaseBuffer();
	UpdateData(FALSE);
	
    if(bBack)
	    ((CEdit*)GetDlgItem(IDC_EDT_PHONENUM))->SetSel(nSelStart-1, nSelEnd-1);
	else 
		((CEdit*)GetDlgItem(IDC_EDT_PHONENUM))->SetSel(nSelStart, nSelEnd);

}

void CSMSDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int count = 0;
	if(nIDEvent==1)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		}
		if(count == 0)
		{
			GetDlgItem(IDC_STC_SENDING)->SetWindowText(_T(" "));
		}
		else if(count == 1)
		{
			GetDlgItem(IDC_STC_SENDING)->SetWindowText(_T("."));
		}
		else if(count == 2)
		{
			GetDlgItem(IDC_STC_SENDING)->SetWindowText(_T(".."));
		}
		else if(count == 4)
		{
			GetDlgItem(IDC_STC_SENDING)->SetWindowText(_T("..."));
			count = -1;
		}

		count++;
	}
	
	CDialog::OnTimer(nIDEvent);
}

DWORD WINAPI CSMSDlg::GetThreadFunc(LPVOID lpParam)
{
	CSMSDlg * This = (CSMSDlg * )lpParam;
	return This->OnSend();
}

DWORD CSMSDlg::OnSend()
{
	CWaitCursor waitCursor;
	CString strTitle;
	strTitle.LoadString(IDS_SMS_TITLE);
	CString strMsg;

	//拆分内容
	int	nLen = GetFullCharCount(m_strContent);
	//test
	m_arrName.GetSize();
	//test

	if(!m_bAutoSplish)
	{
		if((HasUcs2Char((LPCTSTR)m_strContent) && nLen>MAX_SMS_UCS2_LEN) ||			
			(!HasUcs2Char((LPCTSTR)m_strContent) && nLen>MAX_SMS_LEN))
		{	
			if(AfxMessageBox(IDS_WARN_SPLISH,MB_OKCANCEL)==IDCANCEL)
			{
				KillTimer(1);
				ModifyCtrls(TRUE);
				return 0;
			}
		}
	}
	int nItem = SplishContent();
	UNREFERENCED_PARAMETER(nItem);

	CComboBox * pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SENDSAVE);
	_ASSERTE( pCmb != NULL );

	DWORD dwData = pCmb->GetItemData(pCmb->GetCurSel());
	CString strName;
	CString strPhoneNum;
	int nCount = m_arrName.GetSize();
	if(nCount == 0)
	{
		EmptyPrompt(IDC_STC_PHONENUM);
		return 0;
	}
	BOOL bSave = FALSE;
	BOOL bSend = FALSE;

	BOOL bActiveSIM = TRUE;

	if( dwData & SEND_FLAG )
	{
		CComboBox * pCmbSim = (CComboBox *)GetDlgItem(IDC_CMB_SMS_SIM_SEL);
		DWORD dwSimNum = pCmbSim->GetItemData(pCmbSim->GetCurSel());
		if(theApp.m_nMMIVer == MMI_VERSION_2 &&  theApp.m_nSIMCount > 1)
		{	
			bActiveSIM = m_pSMSFactory->SetActiveSim((_SMS_STORE_PLACE_E)dwSimNum);		
		}
	}

	if( (dwData & SEND_FLAG) && bActiveSIM )
	{
		for( int i = 0; i < nCount; ++i )
		{
			bSave   = FALSE;
			strName = m_arrName[i];
			if( StrIsNumeric(strName) )
			{
				strPhoneNum = strName;
				strName.Empty();
			}
			else
			{
				VERIFY( m_mapNameToPhone.Lookup(strName, strPhoneNum) );
			}
			
			BOOL bMore = FALSE;

#ifdef SPRD8800
			if(i < nCount - 1)
			{
				bMore = TRUE;
			}
			else
			{
				bMore = FALSE;
			}
#endif

			waitCursor.Restore();
					
			// send sms
			bSend = SendSMS(strPhoneNum, bMore);

			GetDlgItem(IDC_STC_SENDING)->SetWindowText(_T(" "));
			if( !bSend )
			{
				theApp.TestIsConnected();
				strMsg.LoadString(IDS_ERR_SEND_FAIL);
				if( !(dwData & SAVE_FLAG) && m_bModified &&
					::MessageBox(theApp.m_pSMSFrame->m_hWnd, strMsg,strTitle, MB_YESNO | MB_ICONWARNING) == IDYES )
				{
					bSave = TRUE;
					break;
				}
			}	
		}
	}
	else if(!bActiveSIM)
	{
		AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
	}

	if( dwData & SAVE_FLAG || bSave/* || (bSend && m_EditMode == SEM_EDIT)*/)
	{
		strName = m_arrName[0];
		if( StrIsNumeric(strName) )
		{
			strPhoneNum = strName;
			strName.Empty();
		}
		else
		{
			VERIFY( m_mapNameToPhone.Lookup(strName, strPhoneNum) );
		}
		// save sms
		if( /*m_bModified && */!IsExistInPc(strPhoneNum, m_strContent))
			SaveSMS(strName, strPhoneNum, bSend);
	    m_bModified = FALSE;
		//CDialog::OnOK();
		PostMessage(WM_CLOSE);
	}
	KillTimer(1);
	ModifyCtrls(TRUE);
	return 0;	
}

void  CSMSDlg::ModifyCtrls(BOOL bActive)
{
	GetDlgItem(IDC_EDT_PHONENUM)->EnableWindow(bActive);
	GetDlgItem(IDC_BTN_PHONENUM)->EnableWindow(bActive);
	GetDlgItem(IDC_BTN_GROUP)->EnableWindow(bActive);	
	GetDlgItem(IDC_CMB_SENDSAVE)->EnableWindow(bActive);
	GetDlgItem(IDC_CMB_SEND_VALID_DATE)->EnableWindow(bActive);
	GetDlgItem(IDC_CHK_STATEREPORT)->EnableWindow(bActive);
	GetDlgItem(IDC_EDT_CONTENT)->EnableWindow(bActive);
	GetDlgItem(IDC_BTN_PRESETSMS)->EnableWindow(bActive);
	GetDlgItem(IDC_BTN_OK)->EnableWindow(bActive);
	GetDlgItem(IDCANCEL)->EnableWindow(bActive);
	if(bActive&&m_bSimSelCmdEnable)
	{
		GetDlgItem(IDC_CMB_SMS_SIM_SEL)->EnableWindow(bActive);
	}
}
