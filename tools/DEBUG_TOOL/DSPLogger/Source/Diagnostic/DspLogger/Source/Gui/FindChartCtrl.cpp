// FindChartCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "MainFrm.h"
#include "FindChartCtrl.h"
#include "FindDoc.h"
#include "FindChartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CFindChartCtrl::MAX_X_SCALE = 10000;
const int CFindChartCtrl::MAX_Y_SCALE = 500;
/////////////////////////////////////////////////////////////////////////////
// CFindChartCtrl

IMPLEMENT_DYNCREATE(CFindChartCtrl, CFormView)

CFindChartCtrl::CFindChartCtrl()
	: CFormView(CFindChartCtrl::IDD)
{
	//{{AFX_DATA_INIT(CFindChartCtrl)
	m_nUnsigned		= 0;
	m_nByTime		= 0;
	m_nCurveNum		= DEFAULT_FIND_DATA_NUM;
	m_bDrawTP		= FALSE;
	m_bDrawTogether = FALSE;
	m_bUseSameY		= FALSE;
	//}}AFX_DATA_INIT
    m_nSelect		= 0;
    m_pDoc			= NULL;
	m_nSelectBak    = -1;
    
}//lint !e1401

CFindChartCtrl::~CFindChartCtrl()
{
}//lint !e1540

void CFindChartCtrl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindChartCtrl)
	DDX_Control(pDX, IDC_LST_FIND, m_lstFindData);
	DDX_Radio(pDX, IDC_RDO_UNSIGNED, m_nUnsigned);
	DDX_Radio(pDX, IDC_RDO_BYTIME, m_nByTime);
	DDX_Check(pDX, IDC_CHECK_DRAWTP, m_bDrawTP);
	DDX_Check(pDX, IDC_CHECK_DRAWTOGETHER, m_bDrawTogether);
	DDX_Check(pDX, IDC_CHECK_USESAMEY, m_bUseSameY);
	DDX_Text(pDX, IDC_EDT_CURVE_NUM, m_nCurveNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindChartCtrl, CFormView)
	//{{AFX_MSG_MAP(CFindChartCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_FIND, OnItemchangedLstFind)
	ON_BN_CLICKED(IDC_RDO_UNSIGNED, OnRdoUnsigned)
	ON_BN_CLICKED(IDC_RDO_SIGNED, OnRdoSigned)
	ON_BN_CLICKED(IDC_RDO_BYTIME, OnRdoBytime)
	ON_BN_CLICKED(IDC_RDO_BYIDX, OnRdoByidx)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LST_FIND, OnKillfocusLstFind)
	ON_BN_CLICKED(IDC_CHECK_DRAWTP, OnCheckDrawtp)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_FIND, OnDblclkLstFind)
	ON_BN_CLICKED(IDC_CHECK_DRAWTOGETHER, OnCheckDrawtogether)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnButtonChange)
	ON_BN_CLICKED(IDC_CHECK_USESAMEY, OnCheckUseSameY)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LST_FIND, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindChartCtrl diagnostics

#ifdef _DEBUG
void CFindChartCtrl::AssertValid() const
{
	CFormView::AssertValid();
}

void CFindChartCtrl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFindChartCtrl message handlers

void CFindChartCtrl::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_pDoc=(CFindDoc*) GetDocument();	
	ASSERT(m_pDoc!=NULL);

	//init listctrl
    DWORD dwStyle = m_lstFindData.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    m_lstFindData.SetExtendedStyle(dwStyle);
    m_lstFindData.InsertColumn(0, _T(""), LVCFMT_LEFT, 105);
	
	//init Slider x and y
    CSliderCtrl * psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_H);
    _ASSERTE( psc != NULL );
    psc->SetRange(1, MAX_X_SCALE);
    psc->SetPos(1);

    psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_V);
    _ASSERTE( psc != NULL );
    psc->SetRange(1, MAX_Y_SCALE);
    psc->SetPos(1);

	SetStatus(TRUE);

    for( int i = 0; i < MAX_FIND_DATA_NUM; ++i )
    {
        m_arrSigned[i] = FALSE;
        m_arrByTime[i] = TRUE;
        m_arrYScale[i] = 1;

        m_arrMask[i] = 0xffff;
        m_arrMult[i] = 1.0;
    }

	m_pDoc->UpdateAllViews( this, WM_AUTO_LOAD_FILTER);
}

void CFindChartCtrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UpdateData(FALSE);
	if( pSender != NULL && lHint == WM_UPDATE_FIND  )
    {
        int nItem = 0;
        m_lstFindData.DeleteAllItems();
		SetStatus(TRUE);
        for(int i = 0; i < m_pDoc->m_nFindDataNum; ++i )
        {
			char szTmp[256] = {0};

			if(m_pDoc->m_arrFindData[i].dwReserve2 == TP_A_LESS_VALUE_LESS_B)
			{
				sprintf(szTmp, "%x<Value<%x", m_pDoc->m_arrFindData[i].wAddrFlag, m_pDoc->m_arrFindData[i].wDataFlag);
			}
			else if(m_pDoc->m_arrFindData[i].dwReserve2 == TP_DESCRIPTION)
			{
				_tcscpy(szTmp, m_pDoc->m_arrFindData[i].szDescription);
			}
			else
			{
				_tcscpy(szTmp, m_pDoc->m_arrFindData[i].szAddrValue);
			}
             nItem = m_lstFindData.InsertItem(i, szTmp);
             m_lstFindData.SetItemData(nItem, m_pDoc->m_arrFindColor[i]);
			 SetStatus(FALSE);
        }

		//teana hu 2012.04.09
		if(m_nSelectBak != -1)
		{
			m_nSelect = m_nSelectBak;
			m_nSelectBak = -1;
		}
		else
		{
			m_nSelect = 0;
		}
		//
        
    }	
	
	if( pSender != NULL &&  lHint == WM_SELECT_CHART  )
	{	
		int nSelectChart=   *((int*)pHint);

		if( nSelectChart < 0) return;

		m_lstFindData.SetItemState(nSelectChart, LVIS_SELECTED, LVIS_SELECTED);
		m_lstFindData.Invalidate();
	}
	
}

void CFindChartCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LPNMLVCUSTOMDRAW plvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

    switch(plvcd->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;
    case CDDS_ITEMPREPAINT:
        plvcd->clrText   = plvcd->nmcd.lItemlParam;
        if( m_nSelect == (int)plvcd->nmcd.dwItemSpec )
        {
            plvcd->clrTextBk = RGB(60,180,100);
        }
        *pResult = CDRF_DODEFAULT;
		break;
    default:
	    *pResult = 0;
    }
}

void CFindChartCtrl::OnItemchangedLstFind(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (m_bDrawTogether)
	{
		m_nSelect=0;
	}
	else
	{
       m_nSelect = pNMListView->iItem;
	}

    pNMListView->uNewState = pNMListView->uOldState = 0;
    pNMListView->uChanged  = 0;

    m_nUnsigned = m_arrSigned[m_nSelect] ? 1 : 0;
    m_nByTime   = m_arrByTime[m_nSelect] ? 0 : 1;

    CSliderCtrl * psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_V);
    _ASSERTE( psc != NULL );
    psc->SetPos(m_arrYScale[m_nSelect]);

    UpdateData(FALSE);
    m_lstFindData.Invalidate(FALSE);
    m_lstFindData.SetItemState(m_nSelect, (UINT)(~LVIS_SELECTED), (UINT)LVIS_SELECTED);

	*pResult = 1;
}

void CFindChartCtrl::OnRdoUnsigned() 
{
	m_arrSigned[m_nSelect] = FALSE;

    UpdateChartView(UCA_UNSIGNED, 0, m_nSelect);
	//teana hu 2011.06.01
	m_nSelectBak = m_lstFindData.GetSelectionMark();
	m_pDoc->UpdateAllViews( this, WM_UPDATE_REFIND);
	//
}

void CFindChartCtrl::OnRdoSigned() 
{
	m_arrSigned[m_nSelect] = TRUE;

    UpdateChartView(UCA_SIGNED, 0, m_nSelect);
	//teana hu 2011.06.01
	m_nSelectBak = m_lstFindData.GetSelectionMark();
	m_pDoc->UpdateAllViews( this, WM_UPDATE_REFIND);
	//

}

void CFindChartCtrl::OnRdoBytime() 
{
	m_arrByTime[m_nSelect] = TRUE;
	m_nByTime = 0;

    UpdateChartView(UCA_BYTIME, 0, m_nSelect);
}

void CFindChartCtrl::OnRdoByidx() 
{
	m_arrByTime[m_nSelect] = FALSE;
	m_nByTime = 1;

    UpdateChartView(UCA_BYINDEX, 0, m_nSelect);
}

void CFindChartCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( pScrollBar != NULL )//&& nSBCode == TB_ENDTRACK )
    {
        CSliderCtrl * psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_H);
        _ASSERTE( psc != NULL );
        _ASSERTE( psc->m_hWnd == pScrollBar->m_hWnd );

        UpdateChartView(UCA_X_SCALE, psc->GetPos(), m_nSelect);
    }

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFindChartCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( pScrollBar != NULL )
    {
        CSliderCtrl * psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_V);
        _ASSERTE( psc != NULL );
        _ASSERTE( psc->m_hWnd == pScrollBar->m_hWnd );

        m_arrYScale[m_nSelect] = psc->GetPos();
        UpdateChartView(UCA_Y_SCALE, m_arrYScale[m_nSelect], m_nSelect);
    }

	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFindChartCtrl::UpdateChartView( int nAct, LPARAM lParam, int nIdx )
{
    static UC_HINT ucHint;
    ucHint.eAct   = (UC_ACTION)nAct;
    ucHint.wParam = 0;
    ucHint.lParam = lParam;
    ucHint.nIdx   = nIdx;
    
    m_pDoc->UpdateAllViews(this, WM_UPDATE_CHART, (CObject *)&ucHint);

}

void CFindChartCtrl::OnKillfocusLstFind(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
    POSITION pos = m_lstFindData.GetFirstSelectedItemPosition();
    if( pos != NULL )
    {
	    int nSel = m_lstFindData.GetNextSelectedItem(pos);
        m_lstFindData.SetItemState(nSel, (UINT)(~LVIS_SELECTED), (UINT)LVIS_SELECTED);

    }
	
	*pResult = 0;
}

void CFindChartCtrl::OnCheckDrawtp() 
{
	UpdateData();
    UpdateChartView(UCA_DRAWTP, m_bDrawTP, m_nSelect);
}

void CFindChartCtrl::OnCheckDrawtogether() 
{
	UpdateData();
	m_nSelect=0;
	m_lstFindData.EnableWindow(!m_bDrawTogether);
    UpdateChartView(UCA_DRAWTOGETHER, m_bDrawTogether, m_nSelect);
}

void CFindChartCtrl::OnDblclkLstFind(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 0;
}


void CFindChartCtrl::OnButtonReset() 
{	
	CSliderCtrl * psc=NULL;
	//reset x	
	psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_H);
	psc->SetPos(1);
	UpdateChartView(UCA_X_SCALE, 1, m_nSelect);
	
	//reset y 
	psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_V);
	psc->SetPos(1);
	for(int i = 0; i < m_pDoc->m_nFindDataNum; ++i )
	{
		UpdateChartView(UCA_Y_SCALE, 1, i);
	}
	
	//Set Scroll Pos to (0,0)
	UpdateChartView(UCA_RESET, 0, 0);
	
	for (int idx=0; idx< MAX_FIND_DATA_NUM;idx++)
	{
		m_arrYScale[idx]=1;
	}	
}

void CFindChartCtrl::OnButtonChange()
{
	int nOldCurveNum = m_nCurveNum;
	UpdateData( TRUE);	
	
	if(m_nCurveNum > MAX_FIND_DATA_NUM)
	{
		AfxMessageBox("Error: Excess Max Curve Number (20)!");
		m_nCurveNum = nOldCurveNum;
		UpdateData( FALSE);
		return;
	}
	if (m_lstFindData.GetItemCount() > m_nCurveNum)
	{
		AfxMessageBox("Error: Selected items >Curve Number!  \nFirstly pls removed some selected items! ");
		m_nCurveNum = nOldCurveNum;
		UpdateData( FALSE);
		return;
	}
	UpdateChartView(UCA_CHANGE_CURVE_NUM, 1, m_nCurveNum);
}

void CFindChartCtrl::OnCheckUseSameY() 
{
	UpdateData();	
    UpdateChartView(UCA_DRAWONSAMEY,m_bUseSameY , m_nSelect);
	
}

void CFindChartCtrl::SetStatus(BOOL bEnable)
{
/*	if(bEnable)
	{
		((CEdit *)GetDlgItem(IDC_EDT_CURVE_NUM))->SetReadOnly(FALSE);
		((CButton *)GetDlgItem(IDC_BUTTON_CHANGE))->EnableWindow(TRUE);
	}
	else
	{
		((CEdit *)GetDlgItem(IDC_EDT_CURVE_NUM))->SetReadOnly(TRUE);
		((CButton *)GetDlgItem(IDC_BUTTON_CHANGE))->EnableWindow(FALSE);
	}*/
}
