// ChartFindView.cpp : implementation file
//
#include "stdafx.h"
#include "dsplogger.h"
#include "FindFindView.h"
#include "MainFrm.h"
#include "DspKernelExport.h"
#include "MathFormula.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AUTO_TIME_ID 1
#define MAX_LIST_ITEMS  40
#define DEF_ITEM_COLOR  RGB(0,0,0)

#define SECTION_ADDR		"AddrFilter"
#define SECTION_DATA		"DataFilter"
#define SECTION_STATUS		"ChkStatus"
/////////////////////////////////////////////////////////////////////////////
// CFindFindView

IMPLEMENT_DYNCREATE(CFindFindView, CFormView)

CFindFindView::CFindFindView()
	: CFormView(CFindFindView::IDD)
{
	//{{AFX_DATA_INIT(CFindFindView)
	m_strData = _T("");
	m_strAddr = _T("");
	m_strSpace = _T("");
	//}}AFX_DATA_INIT
	m_pDoc    =NULL;
	
	CString strSection = "Find Address list";
	CString strEntry   = "Find Address%d";
	
    m_cmbAddrVal.SetMaxMruCount(10);	
    m_cmbAddrVal.SetMruSection(strSection);
    m_cmbAddrVal.SetMruEntryFormat(strEntry);

	//teana hu 2008-12-08
	m_strLeftAddr = _T("");
	m_strRightAddr = _T("");
	m_strLeftData = _T("");
	m_strRightData = _T("");
}

CFindFindView::~CFindFindView()
{

}//lint !e1540

void CFindFindView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindFindView)
	DDX_Control(pDX, IDC_LST_CUSTOMIZE_CHART, m_lstCustomCtrl);
	DDX_Control(pDX, IDC_SPIN_SPACE, m_spinCtrl);
    DDX_Control(pDX, IDC_LST_INFO, m_lstInfo);
	DDX_Control(pDX, IDC_CMB_ADDR, m_cmbAddr);
	DDX_Control(pDX, IDC_CMB_DATA, m_cmbData);
	DDX_Text(pDX, IDC_EDT_DATA, m_strData);
	DDX_CBString(pDX, IDC_COMBO_ADDR, m_strAddr);
	DDX_Text(pDX, IDC_EDIT_SPACE, m_strSpace);
	DDX_Control(pDX, IDC_CHECK_DUALITY, m_btnCheckBox);
	DDX_Text(pDX, IDC_EDT_ADDR_LEFT, m_strLeftAddr);
	DDX_Text(pDX, IDC_EDT_ADDR_RIGHT, m_strRightAddr);
	DDX_Text(pDX, IDC_EDT_DATA_LEFT, m_strLeftData);
	DDX_Text(pDX, IDC_EDT_DATA_RIGHT, m_strRightData);
	DDX_Control(pDX, IDC_CHECK_DESC, m_btnCheckDesc);
	DDX_Text(pDX, IDC_EDT_DESC, m_strEdtDesc);
	DDX_Control(pDX, IDC_COMBO_MATCH, m_cmbMatching);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindFindView, CFormView)
	//{{AFX_MSG_MAP(CFindFindView)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_INFO, OnItemchangedLstInfo)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBtnFind)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LST_INFO, OnCustomDrawLstInfo)
	ON_CBN_SELCHANGE(IDC_CMB_DATA, OnSelchangeCmbData)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_AUTO, OnButtonAuto)
	ON_WM_TIMER()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPACE, OnDeltaposSpinSpace)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_CHECK_DUALITY, OnCheckDuality)
	ON_BN_CLICKED(IDC_CHECK_DESC, OnCheckDesc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_CUSTOMIZE_CHART, OnItemchangedLstCustomizeChart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindFindView diagnostics

#ifdef _DEBUG
void CFindFindView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFindFindView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFindFindView message handlers
void CFindFindView::OnInitialUpdate() 
{	
	CFormView::OnInitialUpdate();

    CMainFrame * pFrame = (CMainFrame * )AfxGetMainWnd();
    ITPSet * pSet = pFrame->m_pDspKernel->GetTpSet();
    _ASSERTE(NULL != pSet);

	m_pDoc= (CFindDoc*)GetDocument();
    _ASSERTE( m_pDoc != NULL );
    
    m_pDoc->m_nFindDataNum = 0;
    
    //init list control
    DWORD dwStyle = m_lstInfo.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES ;
    m_lstInfo.SetExtendedStyle(dwStyle);
    m_lstInfo.InsertColumn(0, _T("Addr"), LVCFMT_LEFT, 70);
    m_lstInfo.InsertColumn(1, _T("Data"), LVCFMT_LEFT, 70);
    m_lstInfo.InsertColumn(2, _T("Num"), LVCFMT_LEFT,  50);

    
	CComboBox * pCmbAddr = (CComboBox*)GetDlgItem(IDC_COMBO_ADDR);
    pCmbAddr->LimitText(4);

    CEdit * pEditData = (CEdit *)GetDlgItem(IDC_EDT_DATA);
    pEditData->SetLimitText(4);

    int nIdx=0;
    for(int i = 0; i < TP_FLAG_MAX_NUM; ++i )
    {
	    nIdx = m_cmbAddr.AddString( pSet->GetTpFlagDesc(i));
		m_cmbAddr.SetItemData(nIdx, i);        
		
		nIdx = m_cmbData.AddString(pSet->GetTpFlagDesc(i));
        m_cmbData.SetItemData(nIdx, i);
    }
	
    m_cmbAddrVal.SubclassDlgItem(IDC_COMBO_ADDR,this);
    m_cmbAddrVal.LoadMruList();


    m_spinCtrl.SetRange(1,10);
    m_spinCtrl.SetPos(1);
    m_strSpace ="1Sec";

	LoadSettings();
	InitInput();

	//teana hu 2008-12-08
	m_btnCheckBox.SetCheck(0);
	((CEdit *)GetDlgItem(IDC_EDT_ADDR_LEFT))->EnableWindow(FALSE);
	((CEdit *)GetDlgItem(IDC_EDT_ADDR_RIGHT))->EnableWindow(FALSE);
	((CEdit *)GetDlgItem(IDC_EDT_DATA_LEFT))->EnableWindow(FALSE);
	((CEdit *)GetDlgItem(IDC_EDT_DATA_RIGHT))->EnableWindow(FALSE);

	//teana hu 2009.11.18
	m_btnCheckDesc.SetCheck(0);
	((CEdit *)GetDlgItem(IDC_EDT_DESC))->EnableWindow(FALSE);
	m_cmbMatching.SetCurSel(0);
	m_cmbMatching.EnableWindow(FALSE);

	InitCustomizeGraph();

	UpdateData(FALSE);
}

void CFindFindView::InitCustomizeGraph()
{
	CRect rect;
	m_lstCustomCtrl.GetClientRect(&rect);
    DWORD dwStyle = m_lstCustomCtrl.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES ;
    m_lstCustomCtrl.SetExtendedStyle(dwStyle);
    m_lstCustomCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, rect.Width());
	
    PGRAPH_INFO_PTR	pGraphInfo = g_theApp.m_pGraphInfo;
	if (pGraphInfo && pGraphInfo->pChartGraph)
	{
		for (int i=0; i<pGraphInfo->dwChartCount;++i)
		{
			CHART_INFO_T* pIT = pGraphInfo->pChartGraph + i;
			m_lstCustomCtrl.InsertItem( i, pIT->szAlias );   
		}
	}
}

void CFindFindView::InitInput()
{
	//addr
    m_cmbAddr.SetCurSel(0);
    m_cmbAddrVal.SetCurSel(0);
    m_cmbAddrVal.SetFocus();

	//value
    m_cmbData.SetCurSel(TP_FLAG_MAX_NUM-2);  //set focus on like 
	m_strData       = _T("****");

	UpdateData(FALSE);
}

void CFindFindView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{	
// 	UNREFERENCED_PARAMETER(pSender);
// 	UNREFERENCED_PARAMETER(lHint);
// 	UNREFERENCED_PARAMETER(pHint);
	//teana hu 2011.06.01
	if( pSender != NULL && lHint == WM_UPDATE_REFIND  )							
    {
		OnBtnFind();
    }
	else if (pSender != NULL && lHint ==WM_AUTO_LOAD_FILTER)
	{
		LoadSelectedFilter();
	}
	//
}

void CFindFindView::OnBtnAdd() 
{
	UpdateData();

	if(m_btnCheckBox.GetCheck() == 1)
	{
		if(m_strLeftAddr.IsEmpty())
		{
			AfxMessageBox(_T("Please input the left addr for duality!"));
			return;
		}
		if(m_strRightAddr.IsEmpty())
		{
			AfxMessageBox(_T("Please input the right addr for duality!"));
			return;
		}
		if(m_strLeftData.IsEmpty())
		{
			AfxMessageBox(_T("Please input the left data for duality!"));
			return;
		}
		if(m_strRightData.IsEmpty())
		{
			AfxMessageBox(_T("Please input the right data for duality!"));
			return;
		}

		if((m_strLeftAddr.GetAt(0) != 'D' && m_strLeftAddr.GetAt(0) != 'd') || (m_strRightAddr.GetAt(0) != 'D' && m_strRightAddr.GetAt(0) != 'd'))
		{
			AfxMessageBox(_T("Invalid addr condition for duality!"));
			return;
		}

		m_strLeftAddr.MakeLower();
		m_strLeftData.MakeLower();
		m_strRightAddr.MakeLower();
		m_strRightData.MakeLower();
		
		int nItem= AddListItem(m_strLeftAddr,m_strRightAddr,m_strLeftData,m_strRightData, FLAG_DUALITY);
		if (nItem == -1)
		{		
			return;
		}

		char * pEnd = NULL;

		DWORD dwLeftAddr = (WORD)strtol(m_strLeftAddr, &pEnd, 16);
		DWORD dwRightAddr = (WORD)strtol(m_strRightAddr, &pEnd, 16);
		
		if(m_pDoc->AddFindData(dwLeftAddr,m_strLeftData,dwRightAddr,m_strRightData,FLAG_DUALITY))//leftaddr,leftdata,rightaddr,rightdata
		{		
			m_lstInfo.SetCheck(nItem);	
			m_lstInfo.SetCheck(nItem);	//i do't know why, may be it a bug of windows
		}
	}
	else if(m_btnCheckDesc.GetCheck() == 1)
	{
		if(m_strEdtDesc.IsEmpty())
		{
			AfxMessageBox(_T("Please input the description!"));
			return;
		}

		int nMatchMode = 1;
		if(m_cmbMatching.GetCurSel() == 0)
		{
			nMatchMode = 1;
		}
		else
		{
			nMatchMode = 0;
		}
		int nItem= AddListItem("",m_strEdtDesc,"","", FLAG_DESCRIPTION);
		if (nItem == -1)
		{		
			return;
		}
		
		if(m_pDoc->AddFindData(TP_DESCRIPTION,m_strEdtDesc,nMatchMode,"", FLAG_DESCRIPTION))
		{		
			m_lstInfo.SetCheck(nItem);	
			m_lstInfo.SetCheck(nItem);	//i do't know why, may be it a bug of windows
		}
	}
	else
	{
		if( m_strAddr.IsEmpty() )
		{
			AfxMessageBox(_T("Please input the address!"));
			return;
		}
		
		if( m_strAddr.GetLength() != 4 )
		{
			AfxMessageBox(_T("The address's length must is 4 !"));
			return;
		}
		
		if( m_strData.IsEmpty() )
		{
			AfxMessageBox(_T("Please input the data!"));
			return;
		}
		
		if( m_strData.GetLength() != 4 )
		{
			AfxMessageBox(_T("The data's length must is 4 !"));
			return;
		}
		
		if( m_strAddr.Compare(_T("****")) == 0 && m_strData.Compare(_T("****")) == 0 )
		{
			AfxMessageBox(_T("Invalid condition!"));
			return;
		}

		if( (m_strAddr.Compare(_T("****")) == 0 && m_cmbAddr.GetCurSel() == TP_UNLIKE)
			|| (m_strData.Compare(_T("****")) == 0 && m_cmbData.GetCurSel() == TP_UNLIKE))
		{
			AfxMessageBox(_T("Invalid condition!"));
			return;
		}

		m_strAddr.MakeLower();
		m_strData.MakeLower();
		
		WORD wAddrFlag = (WORD)m_cmbAddr.GetItemData(m_cmbAddr.GetCurSel());
		WORD wDataFlag = (WORD)m_cmbData.GetItemData(m_cmbData.GetCurSel());
		
		CString strAddrFlag;
		CString strDataFlag;
		m_cmbAddr.GetWindowText(strAddrFlag);
		m_cmbData.GetWindowText(strDataFlag);
		
		int nItem= AddListItem(strAddrFlag,m_strAddr,strDataFlag,m_strData);
		if (nItem == -1)
		{		
			return;
		}
		
		if(m_pDoc->AddFindData(wAddrFlag,m_strAddr,wDataFlag,m_strData))
		{		
			m_lstInfo.SetCheck(nItem);	
			m_lstInfo.SetCheck(nItem);	//i do't know why, may be it a bug of windows
		}
	}

	SetListItemColor();

    if( m_lstInfo.GetItemCount() == MAX_LIST_ITEMS )
    {
        CWnd * pWnd = GetDlgItem(IDC_BTN_ADD);
        _ASSERTE( pWnd != NULL );
        pWnd->EnableWindow(FALSE);
    }
	
	m_cmbAddrVal.Add2MruList();
	OnBtnFind(); 
	InitInput();
}

void CFindFindView::OnBtnDel() 
{
	WORD wAddrFlag, wDataFlag;
	CString strAddr,   strData;
	
    POSITION pos = m_lstInfo.GetFirstSelectedItemPosition();
	if(pos == NULL) return;
    int nItem = m_lstInfo.GetNextSelectedItem(pos);
	
	GetItemInfo(nItem, wAddrFlag,strAddr,wDataFlag,strData);	
	
	m_pDoc->DelFindData(wAddrFlag,strAddr,wDataFlag,strData);	
    m_lstInfo.DeleteItem(nItem);
	
	SetListItemColor();

    CWnd * pWnd = GetDlgItem(IDC_BTN_ADD);
    _ASSERTE( pWnd != NULL );
    pWnd->EnableWindow(TRUE);
	OnBtnFind(); 
}



BOOL CFindFindView::PreTranslateMessage(MSG* pMsg) 
{
	//del item in list
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_DELETE)
        {
			POSITION pos = m_lstInfo.GetFirstSelectedItemPosition();
			if(pos == NULL)
			{
				// No item was selected
				return CFormView::PreTranslateMessage(pMsg);
			}			
			int nIndex = m_lstInfo.GetNextSelectedItem(pos);
			
			LockWindowUpdate();
			OnBtnDel();
			m_lstInfo.SetItemState(nIndex,LVIS_SELECTED, LVIS_SELECTED); 
			UnlockWindowUpdate();
        }
		
    }

	//check input 
	if( pMsg->message == WM_CHAR )
    {
        CWnd * pWndAddr =(CWnd*) GetDlgItem(IDC_COMBO_ADDR);
        _ASSERTE( pWndAddr != NULL );

        CWnd * pWndData = GetDlgItem(IDC_EDT_DATA);
        _ASSERTE( pWndData != NULL );

        if( ::IsChild(pWndAddr->m_hWnd, pMsg->hwnd) || pMsg->hwnd == pWndData->m_hWnd )
            
        {
            if( pMsg->wParam < '0' || pMsg->wParam > '9' )
            {
                if( pMsg->wParam < 'a' || pMsg->wParam > 'f' )
                {
                    if( (pMsg->wParam < 'A' || pMsg->wParam > 'F') &&
                        pMsg->wParam != VK_BACK )
                    {
                        if( pMsg->wParam == '*' )
                        {
                            if( pMsg->hwnd == pWndAddr->m_hWnd &&
                                m_cmbAddr.GetItemData(m_cmbAddr.GetCurSel()) != TP_LIKE
								&& m_cmbAddr.GetItemData(m_cmbAddr.GetCurSel()) != TP_UNLIKE)
                            {
                                return TRUE;
                            }

                            if( pMsg->hwnd == pWndData->m_hWnd &&
                                m_cmbData.GetItemData(m_cmbData.GetCurSel()) != TP_LIKE
								&& m_cmbData.GetItemData(m_cmbData.GetCurSel()) != TP_UNLIKE)
                            {
                                return TRUE;
                            }
                        }
                        else
                        {
                            return TRUE;
                        }
                    }
                }                
            }
        }
    }

	return CFormView::PreTranslateMessage(pMsg);
}

void CFindFindView::OnItemchangedLstInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{

	CWnd * pWnd = GetDlgItem(IDC_BTN_DEL);
    pWnd->EnableWindow( m_lstInfo.GetFirstSelectedItemPosition() != NULL );
	
	/************************************************************************/
	/* get check box status in listctrl
	/************************************************************************/
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
	{
		return;	// No change
	}

	//Old check box state
	BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1); 
	if (bPrevState < 0)
	{
		bPrevState = 0;
	}

	// New check box state
	BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
	if (bChecked < 0)   
	{	
		bChecked = 0; 
	}	
	// No change in check status
	if (bPrevState == bChecked)
	{
		return; 
	}

	// Now bChecked holds the new check box state	
	OnCheckStatus(pNMListView->iItem, bChecked);
	SetListItemColor();

	OnBtnFind() ;
	*pResult = 0;
}

void CFindFindView::OnBtnFind() 
{
	//updata all view in same docment
    m_pDoc->UpdateAllViews( this, WM_UPDATE_FIND);
	
	//updata item tp count
	UpdataTPCount();
	
}

void CFindFindView::AutoRefresh()
{    
	//updata chart view redraw graph
    m_pDoc->UpdateAllViews( this, WM_AUTO_REFRESH);
    
	//updata item tp count
	UpdataTPCount();
}

void CFindFindView::OnCustomDrawLstInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

    switch( pLVCD->nmcd.dwDrawStage )
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;
    case CDDS_ITEMPREPAINT:
        pLVCD->clrText = pLVCD->nmcd.lItemlParam;
        break;
    }
}

void CFindFindView::OnSelchangeCmbData() 
{
	if( m_cmbData.GetItemData(m_cmbData.GetCurSel()) == TP_LIKE || m_cmbData.GetItemData(m_cmbData.GetCurSel()) == TP_UNLIKE)
    {
        UpdateData();

        m_strData = _T("****");
        UpdateData(FALSE);
    }

}



void CFindFindView::GetItemInfo(int nItem, WORD& wAddrFlag, CString& strAddr, WORD& wDataFlag, CString& strData)
{
	if (nItem < 0 || nItem >= m_lstInfo.GetItemCount())  
		return;
	
	CString strAddrLine= m_lstInfo.GetItemText(nItem,0);
	CString strDataLine= m_lstInfo.GetItemText(nItem,1);

	if(strAddrLine.Find(_T("Addr")) != -1)
	{
		int nIndex = strAddrLine.Find(_T("<"));
		if(nIndex != -1)
		{
			wAddrFlag = (WORD)strtol(strAddrLine.Left(nIndex), NULL, 16);
			strAddrLine.Delete(0, nIndex + 6);
			wDataFlag = (WORD)strtol(strAddrLine, NULL, 16);
		}
	}
	else if(strAddrLine.Find(_T("Desc:")) != -1)
	{
		wAddrFlag = TP_DESCRIPTION;
		if(m_cmbMatching.GetCurSel() == 0)
		{
			wDataFlag = 1;
		}
		else
		{
			wDataFlag = 0;
		}
		strAddrLine.Delete(0, 5);
		strAddr = strAddrLine;	
		strData = "";
		return;
	}

	if(strDataLine.Find(_T("Data")) != -1)
	{
		int nIndex = strDataLine.Find(_T("<"));
		if(nIndex != -1)
		{
			strAddr = strDataLine.Left(nIndex);
			strDataLine.Delete(0, nIndex + 6);
			strData = strDataLine;
		}

		return;
	}
	
	int nPos= strAddrLine.ReverseFind(' ');	
	CString	strAddrFlag= strAddrLine.Left(nPos);
	strAddr    = strAddrLine.Right(strAddrLine.GetLength()-nPos-1);
	
	nPos= strDataLine.ReverseFind(' ');
	CString strDataFlag= strDataLine.Left(nPos);
	strData    = strDataLine.Right(strDataLine.GetLength()-nPos-1);


	wAddrFlag=0xFFFF;
	wDataFlag=0xFFFF;
	CMainFrame * pFrame = (CMainFrame * )AfxGetMainWnd();
    ITPSet * pSet = pFrame->m_pDspKernel->GetTpSet();
    for(int i = 0; i < TP_FLAG_MAX_NUM; ++i )
    {		
		CString strFlag= pSet->GetTpFlagDesc(i);
		if (!strAddrFlag.CompareNoCase(strFlag ))
		{
			wAddrFlag=i;
		} 
		
		if (!strDataFlag.CompareNoCase(strFlag))
		{
			wDataFlag=i;
		} 
    }
	
}
void  CFindFindView::CustomGrapItemChanged(int nItem , BOOL bCheck)
{
	WORD wAddrFlag, wDataFlag;
	CString strAddr,   strData;


	PGRAPH_INFO_PTR	pGraphInfo = g_theApp.m_pGraphInfo;
	if (pGraphInfo && pGraphInfo->pChartGraph && nItem < pGraphInfo->dwChartCount )
	{
		CHART_INFO_T* pIT = pGraphInfo->pChartGraph + nItem;
		wAddrFlag = pIT->wAddrCond;
		wDataFlag = pIT->wDataCond;
		strAddr.Format(_T("%x"),pIT->wAddr);
		if (pIT->wData == 0xFFFF)
		{
			strData=_T("****");
		}
		else
		{
			strData.Format(_T("%x"),pIT->wData);
		}
		
		strAddr.MakeLower();
		strData.MakeLower();
		
		if (bCheck)
		{
			BOOL bResult = FALSE;
			
			bResult= m_pDoc->AddFindData(wAddrFlag,strAddr,wDataFlag,strData,FLAG_GENERAL,pIT->szAlias);
			
			bResult ?  m_lstCustomCtrl.SetCheck(nItem,TRUE) : m_lstCustomCtrl.SetCheck(nItem,FALSE);
		}
		else
		{
			m_pDoc->DelFindData(wAddrFlag,strAddr,wDataFlag,strData,pIT->szAlias);
		}

	}

	
	
}
void CFindFindView::OnCheckStatus(int nItem , BOOL bCheck)
{
	WORD wAddrFlag, wDataFlag;
	CString strAddr,   strData;
	
	GetItemInfo(nItem, wAddrFlag, strAddr, wDataFlag, strData );
	
	if (bCheck)
	{
		BOOL bResult = FALSE;
		
		if(m_btnCheckBox.GetCheck())
		{
			char * pEnd = NULL;
			
			DWORD dwLeftAddr = (WORD)strtol(m_strLeftAddr, &pEnd, 16);
			DWORD dwRightAddr = (WORD)strtol(m_strRightAddr, &pEnd, 16);
			
			bResult = m_pDoc->AddFindData(dwLeftAddr,m_strLeftData,dwRightAddr,m_strRightData,FLAG_DUALITY);//leftaddr,leftdata,rightaddr,rightdata
		}
		else if(wAddrFlag > TP_DESCRIPTION)
		{
			bResult = m_pDoc->AddFindData(wAddrFlag,strAddr,wDataFlag,strData,FLAG_DUALITY);
		}
		else if(wAddrFlag == TP_DESCRIPTION)
		{
			bResult = m_pDoc->AddFindData(wAddrFlag,strAddr,wDataFlag,strData,FLAG_DESCRIPTION);
		}
		else
		{
			bResult= m_pDoc->AddFindData(wAddrFlag,strAddr,wDataFlag,strData);
		}
		bResult ?  m_lstInfo.SetCheck(nItem,TRUE) : m_lstInfo.SetCheck(nItem,FALSE);
	}
	else
	{
		m_pDoc->DelFindData(wAddrFlag,strAddr,wDataFlag,strData);
	}

}

void CFindFindView::SetListItemColor()
{
	//set items color
    int nItem = m_lstInfo.GetItemCount();
	for (int num=0; num < nItem ;num++)
	{
		if( m_lstInfo.GetCheck(num))
		{	
			int idx= Item2FindDataIdx(num);
			m_lstInfo.SetItemData(num, m_pDoc->m_arrFindColor[idx]);  
		}	
		else
		{
			m_lstInfo.SetItemData(num, DEF_ITEM_COLOR);  			
		}
		
	}
}

int CFindFindView::AddListItem(CString strAddrFlag, CString strAddr, CString strDataFlag, CString strData,  DWORD dwFlag)
{ 
	_TCHAR  szTxt0[128];
    _TCHAR  szTxt1[128];

	switch(dwFlag) {
	case FLAG_GENERAL:
		_stprintf(szTxt0, _T("%s %s"), strAddrFlag, strAddr);
		_stprintf(szTxt1, _T("%s %s"), strDataFlag, strData);
		break;
	case FLAG_DUALITY:
		_stprintf(szTxt0, _T("%s<Addr<%s"), strAddrFlag, strAddr);
		if(strDataFlag != _T("****") && strData != _T("****"))
		{
			_stprintf(szTxt1, _T("%s<Data<%s"), strDataFlag, strData);
		}
		else if(strDataFlag != _T("****") && strData == _T("****"))
		{
			_stprintf(szTxt1, _T("%s<Data<****"), strDataFlag);
		}
		else if(strDataFlag == _T("****") && strData != _T("****"))
		{
			_stprintf(szTxt1, _T("****<Data<%s"), strData);
		}
		else
		{
			_stprintf(szTxt1, _T("****<Data<****"));
		}
		break;
	case FLAG_DESCRIPTION:
		_stprintf(szTxt0, _T("Desc:%s"), strAddr);
		_stprintf(szTxt1, _T("%s %s"), strDataFlag, strData);
		break;
	default:
		break;
	}
		
	for (int i=0; i< m_lstInfo.GetItemCount(); i++)
	{
		CString strItem		= m_lstInfo.GetItemText(i,0);
		CString strSubItem	= m_lstInfo.GetItemText(i,1);
		if (strItem== szTxt0 && strSubItem == szTxt1)
		{
			//AfxMessageBox(_T("The condition already exist!"));
			m_lstInfo.SetCheck(i,TRUE);
			return -1;			
		}
	}
	
	int nItem = m_lstInfo.GetItemCount();
    m_lstInfo.InsertItem( nItem, szTxt0 );   
    m_lstInfo.SetItemText(nItem, 1, szTxt1);
	return nItem;
}

void CFindFindView::UpdataTPCount()
{
	CString strCount; 

	//set subchart points number
	for (int i=0;i< m_lstInfo.GetItemCount(); i++)
	{
		if (m_lstInfo.GetCheck(i))
		{			
			int nIdx= Item2FindDataIdx(i);
			DWORD dwCount= m_pDoc->m_arrFindData[nIdx].dwFindCount;
			
			strCount.Format("%d",dwCount);
			m_lstInfo.SetItemText(i, 2 , strCount);
		}		
		else
		{
			m_lstInfo.SetItemText(i, 2 , "");
		}
	}
}

int  CFindFindView::Item2FindDataIdx(int nIdx)
{
	WORD wAddrFlag, wDataFlag;
	CString strAddr,   strData;

	//get list item info
	GetItemInfo(nIdx, wAddrFlag, strAddr, wDataFlag, strData );
	
	//get find struct index 
	int idx= m_pDoc->GetFindDataIdx(wAddrFlag,strAddr,wDataFlag,strData);
	
	return idx;
}

void CFindFindView::OnDestroy() 
{
	CFormView::OnDestroy();
	m_cmbAddrVal.SaveMruList();
	SaveSettings();
}

// Save List item settings
void CFindFindView::SaveSettings()
{
	//polo.jiang change cfg from reg to ini
	CString strAddr;
	CString strData;
	CString strCheck;
	CString idx;
	int nCount	  = m_lstInfo.GetItemCount();

    GetModuleFileName(NULL,m_szCfgFile,_MAX_PATH);
	TCHAR* p=_tcsrchr(m_szCfgFile,_T('.'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	_tcscat(m_szCfgFile,_T(".ini"));
	
	for (int i=0; i< MAX_LIST_ITEMS; i++)
	{
		strAddr="";
		strData="";
		strCheck="";
		if ( i < nCount )
		{
			strAddr= m_lstInfo.GetItemText(i,0);
			strData= m_lstInfo.GetItemText(i,1);
			strCheck.Format("%d",m_lstInfo.GetCheck(i));
			
		}
		idx.Format("%d",i);

		WritePrivateProfileString(SECTION_ADDR,idx,strAddr.operator LPCTSTR(),m_szCfgFile);
		WritePrivateProfileString(SECTION_DATA,idx,strData.operator LPCTSTR(),m_szCfgFile);
		WritePrivateProfileString(SECTION_STATUS,idx,strCheck.operator LPCTSTR(),m_szCfgFile);
		
	}
	/*
	CString strAddr;
	CString strData;
	CString idx;
	
    CWinApp* pApp = AfxGetApp();
	int nCount	  = m_lstInfo.GetItemCount();

	for (int i=0; i< MAX_LIST_ITEMS; i++)
	{
		strAddr="";
		strData="";
		if ( i < nCount )
		{
			strAddr= m_lstInfo.GetItemText(i,0);
			strData= m_lstInfo.GetItemText(i,1);			
		}
		idx.Format("%d",i);
		pApp->WriteProfileString(SECTION_ADDR, idx, strAddr);
		pApp->WriteProfileString(SECTION_DATA, idx, strData);
		
	}
	*/
}

// Load list item settings
void CFindFindView::LoadSettings()
{

	//polo.jiang change cfg from reg to ini
	TCHAR szValue[_MAX_PATH] = {0};
	CString strAddr;
	CString strData;
	CString idx;
	
    GetModuleFileName(NULL,m_szCfgFile,_MAX_PATH);
	TCHAR* p=_tcsrchr(m_szCfgFile,_T('.'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	_tcscat(m_szCfgFile,_T(".ini"));
	
	for (int i=0; i< MAX_LIST_ITEMS; i++)
	{
		idx.Format("%d",i);
		GetPrivateProfileString(SECTION_ADDR,idx,_T(""),szValue, _MAX_PATH, m_szCfgFile);
		strAddr = szValue;
		GetPrivateProfileString(SECTION_DATA,idx,_T(""),szValue, _MAX_PATH, m_szCfgFile);
		strData = szValue;
		if ( strAddr != "" && strData != "")
		{
			m_lstInfo.InsertItem( i, strAddr );   
			m_lstInfo.SetItemText(i,1,strData);	
		}				
	}

}

void CFindFindView::OnButtonAuto() 
{
   // static BOOL bAutoRefresh= TRUE;
    if (!m_pDoc->m_bAutoRefresh)
    {
        GetDlgItem(IDC_BUTTON_AUTO)->SetWindowText("Stop");
        WORD nSpace= m_spinCtrl.GetPos();
        SetTimer(AUTO_TIME_ID,nSpace*1000 , NULL);    
        m_pDoc->m_bAutoRefresh = TRUE;
    }
    else
    {
        GetDlgItem(IDC_BUTTON_AUTO)->SetWindowText("Auto");
        KillTimer(AUTO_TIME_ID);
        m_pDoc->m_bAutoRefresh = FALSE;
    }
	
}

void CFindFindView::OnTimer(UINT nIDEvent) 
{
    if (AUTO_TIME_ID == nIDEvent)
    {
        AutoRefresh();
    }

	CFormView::OnTimer(nIDEvent);
}

void CFindFindView::OnDeltaposSpinSpace(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int nSpace= pNMUpDown->iPos;
    CString strSpace;
    strSpace.Format("%dSec",nSpace);
    GetDlgItem(IDC_EDIT_SPACE)->SetWindowText(strSpace);
  
	*pResult = 0;
}


void CFindFindView::OnButtonExport() 
{
	//save current tp 
    m_pDoc->UpdateAllViews( this, WM_SAVE_CURRENT_TP);	
}

void CFindFindView::OnCheckDuality() 
{
	// TODO: Add your control notification handler code here
	if(m_btnCheckBox.GetCheck() == 1)
	{
		m_btnCheckDesc.EnableWindow(FALSE);
		m_strLeftData = _T("****");
		m_strRightData = _T("****");
		
		((CEdit *)GetDlgItem(IDC_EDT_ADDR_LEFT))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDT_ADDR_RIGHT))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA_LEFT))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA_RIGHT))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CMB_ADDR))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CMB_DATA))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_COMBO_ADDR))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA))->EnableWindow(FALSE);
	}
	else
	{
		m_btnCheckDesc.EnableWindow(TRUE);
		m_strLeftAddr.Empty();
		m_strRightAddr.Empty();
		m_strLeftData.Empty();
		m_strRightData.Empty();	
		
		((CEdit *)GetDlgItem(IDC_EDT_ADDR_LEFT))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDT_ADDR_RIGHT))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA_LEFT))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA_RIGHT))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CMB_ADDR))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CMB_DATA))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_COMBO_ADDR))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA))->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CFindFindView::OnCheckDesc() 
{
	// TODO: Add your control notification handler code here
	if(m_btnCheckDesc.GetCheck() == 1)
	{
		m_btnCheckBox.EnableWindow(FALSE);
		m_cmbMatching.EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDT_DESC))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CMB_ADDR))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CMB_DATA))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_COMBO_ADDR))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA))->EnableWindow(FALSE);
	}
	else
	{
		m_btnCheckBox.EnableWindow(TRUE);
		m_strEdtDesc.Empty();
		m_cmbMatching.EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDT_DESC))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CMB_ADDR))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CMB_DATA))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_COMBO_ADDR))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDT_DATA))->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
	
}
void CFindFindView::LoadSelectedFilter()
{
	TCHAR szValue[_MAX_PATH] = {0};
	CString strAddr;
	CString strData;
	CString idx;
	int nCount = 0;
	BOOL bChecked = FALSE;
	BOOL bAutoLoad = FALSE;
	
    GetModuleFileName(NULL,m_szCfgFile,_MAX_PATH);
	TCHAR* p=_tcsrchr(m_szCfgFile,_T('.'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	_tcscat(m_szCfgFile,_T(".ini"));
	
	bAutoLoad = GetPrivateProfileInt("Settings", "AutoLoadChart", FALSE, m_szCfgFile );
	for (int i=0; i< MAX_LIST_ITEMS; i++)
	{
		idx.Format("%d",i);
		GetPrivateProfileString(SECTION_ADDR,idx,_T(""),szValue, _MAX_PATH, m_szCfgFile);
		strAddr = szValue;
		GetPrivateProfileString(SECTION_DATA,idx,_T(""),szValue, _MAX_PATH, m_szCfgFile);
		strData = szValue;
		bChecked = GetPrivateProfileInt(SECTION_STATUS, idx, FALSE, m_szCfgFile );
		if ( bAutoLoad && strAddr != "" && strData != "" && 
			bChecked && nCount<DEFAULT_FIND_DATA_NUM)
		{
			m_lstInfo.SetCheck(i);
			++nCount;			
		}				
	}
};

void CFindFindView::OnItemchangedLstCustomizeChart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWnd * pWnd = GetDlgItem(IDC_BTN_DEL);
    pWnd->EnableWindow( m_lstInfo.GetFirstSelectedItemPosition() != NULL );
	
	/************************************************************************/
	/* get check box status in listctrl
	/************************************************************************/
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
	{
		return;	// No change
	}

	//Old check box state
	BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1); 
	if (bPrevState < 0)
	{
		bPrevState = 0;
	}

	// New check box state
	BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
	if (bChecked < 0)   
	{	
		bChecked = 0; 
	}	
	// No change in check status
	if (bPrevState == bChecked)
	{
		return; 
	}

	// Now bChecked holds the new check box state	
	CustomGrapItemChanged(pNMListView->iItem, bChecked);

	OnBtnFind() ;
	
	*pResult = 0;
}
