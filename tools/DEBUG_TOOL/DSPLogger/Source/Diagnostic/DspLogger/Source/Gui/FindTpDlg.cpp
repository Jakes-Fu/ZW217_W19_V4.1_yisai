// FindTpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindTpDlg.h"
#include "Mainfrm.h"

#include "TestPointsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindTpDlg dialog


CFindTpDlg::CFindTpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindTpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindTpDlg)
	m_strAddr = _T("");
	m_strData = _T("");
	//}}AFX_DATA_INIT

    m_pFindData = NULL;
    m_nFindNum  = 0;
    m_nFindSize = 0;
}


void CFindTpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindTpDlg)
	DDX_Control(pDX, IDC_LST_INFO, m_lstInfo);
	DDX_Control(pDX, IDC_CMB_ADDR, m_cmbAddr);
	DDX_Control(pDX, IDC_CMB_DATA, m_cmbData);
	DDX_Text(pDX, IDC_EDT_ADDR, m_strAddr);
	DDX_Text(pDX, IDC_EDT_DATA, m_strData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindTpDlg, CDialog)
	//{{AFX_MSG_MAP(CFindTpDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_INFO, OnItemchangedLstInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindTpDlg message handlers

BOOL CFindTpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CEdit * pWndAddr = (CEdit *)GetDlgItem(IDC_EDT_ADDR);
    _ASSERTE( pWndAddr != NULL );
    CEdit * pWndData = (CEdit *)GetDlgItem(IDC_EDT_DATA);
    _ASSERTE( pWndData != NULL );

    pWndAddr->SetLimitText(4);
    pWndData->SetLimitText(4);

    m_lstInfo.InsertColumn(0, _T("Addr"), LVCFMT_LEFT, 110);
    m_lstInfo.InsertColumn(1, _T("Data"), LVCFMT_LEFT, 110);

    DWORD dwStyle = m_lstInfo.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstInfo.SetExtendedStyle(dwStyle);
    
    CMainFrame * pFrame = (CMainFrame * )AfxGetMainWnd();
    ITPSet * pSet = pFrame->m_pDspKernel->GetTpSet();

    int nIdx = 0;
    int i    = 0;
    for( i = 0; i < TP_FLAG_MAX_NUM; ++i )
    {
        nIdx = m_cmbAddr.AddString( pSet->GetTpFlagDesc(i));
        m_cmbAddr.SetItemData(nIdx, i);

        nIdx = m_cmbData.AddString(pSet->GetTpFlagDesc(i));
        m_cmbData.SetItemData(nIdx, i);
    }

    i--;
    m_cmbAddr.SetCurSel(0);
    m_cmbData.SetCurSel(i);

    m_strData = _T("****");

    _TCHAR szTxt[128];

    for( i = 0; i < m_nFindNum; ++i )
    {
        _stprintf(szTxt, _T("%s %s"), pSet->GetTpFlagDesc(m_pFindData[i].wAddrFlag), 
            m_pFindData[i].szAddrValue);
        m_lstInfo.InsertItem(i, szTxt);
        
        _stprintf(szTxt, _T("%s %s"), pSet->GetTpFlagDesc(m_pFindData[i].wDataFlag), 
            m_pFindData[i].szDataValue);
        m_lstInfo.SetItemText(i, 1, szTxt);
    }

    UpdateData(FALSE);

    return FALSE;
                  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindTpDlg::OnOK() 
{	
    if( m_nFindNum <= 0 )
    {
        AfxMessageBox(_T("Please add some items!"));
        return;        
    }

    CDialog::OnOK();
}    

void CFindTpDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

BOOL CFindTpDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR )
    {
        CWnd * pWndAddr = GetDlgItem(IDC_EDT_ADDR);
        _ASSERTE( pWndAddr != NULL );
        CWnd * pWndData = GetDlgItem(IDC_EDT_DATA);
        _ASSERTE( pWndData != NULL );

        if( pMsg->hwnd == pWndAddr->m_hWnd ||
            pMsg->hwnd == pWndData->m_hWnd )
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
                                m_cmbAddr.GetItemData(m_cmbAddr.GetCurSel()) != TP_LIKE )
                            {
                                return TRUE;
                            }

                            if( pMsg->hwnd == pWndData->m_hWnd &&
                                m_cmbData.GetItemData(m_cmbData.GetCurSel()) != TP_LIKE )
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

	return CDialog::PreTranslateMessage(pMsg);
}

void CFindTpDlg::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

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

    m_strAddr.MakeLower();
    m_strData.MakeLower();

    WORD wAddrFlag = (WORD)m_cmbAddr.GetItemData(m_cmbAddr.GetCurSel());
    WORD wDataFlag = (WORD)m_cmbData.GetItemData(m_cmbData.GetCurSel());
    for( int i = 0; i < m_nFindNum; ++i )
    {
        if( m_pFindData[i].wAddrFlag == wAddrFlag && 
            m_strAddr.CompareNoCase(m_pFindData[i].szAddrValue) == 0 )
        {
            if( m_pFindData[i].wDataFlag == wDataFlag &&
                m_strData.CompareNoCase(m_pFindData[i].szDataValue) == 0 )
            {
                AfxMessageBox(_T("The condition already exist!"));
                return;
            }
        }
    }
    int nItem = m_lstInfo.GetItemCount();
    _ASSERTE( nItem == m_nFindNum );

    _TCHAR  szTxt[128];
    _TCHAR  szTmp[128];

    m_cmbAddr.GetWindowText(szTmp, 128);
    _stprintf(szTxt, _T("%s %s"), szTmp, m_strAddr);
    m_lstInfo.InsertItem( nItem, szTxt );

    m_cmbData.GetWindowText(szTmp, 128);
    _stprintf(szTxt, _T("%s %s"), szTmp, m_strData);
    m_lstInfo.SetItemText(nItem, 1, szTxt);

    m_pFindData[m_nFindNum].wAddrFlag = wAddrFlag;
    strcpy(m_pFindData[m_nFindNum].szAddrValue, m_strAddr);
    m_pFindData[m_nFindNum].wDataFlag = wDataFlag;
    strcpy(m_pFindData[m_nFindNum].szDataValue, m_strData);

    m_nFindNum++;

    if( m_nFindNum == MAX_FIND_DATA_NUM )
    {
        CWnd * pWnd = GetDlgItem(IDC_BTN_ADD);
        _ASSERTE( pWnd != NULL );

        pWnd->EnableWindow(FALSE);
    }
}

void CFindTpDlg::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
/*	if( AfxMessageBox(_T("Are sure delete the item?"), MB_YESNO) == IDNO )
    {
        return;
    }
*/
    POSITION pos = m_lstInfo.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		return;
	}
	
    int nSel = m_lstInfo.GetNextSelectedItem(pos);
    m_lstInfo.DeleteItem(nSel);
    m_nFindNum--;

    for( int i = nSel; i < m_nFindNum; ++i )
    {
        memcpy(m_pFindData + i, m_pFindData + i + 1, sizeof(TP_FIND_DATA));
    }
    
    CWnd * pWnd = GetDlgItem(IDC_BTN_ADD);
    _ASSERTE( pWnd != NULL );

    pWnd->EnableWindow(TRUE);
}

void CFindTpDlg::OnItemchangedLstInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
//	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	CWnd * pWnd = GetDlgItem(IDC_BTN_DEL);
    _ASSERTE( pWnd != NULL );

    pWnd->EnableWindow( m_lstInfo.GetFirstSelectedItemPosition() != NULL );

	*pResult = 0;
}
