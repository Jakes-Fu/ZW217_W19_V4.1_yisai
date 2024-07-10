// FilterChartCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "MainFrm.h"
#include "FilterChartCtrl.h"
#include "FilterDoc.h"
#include "FmlDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum
{
    FIT_PAGE =0,
    FIT_HEIGHT,
    FIT_WIDTH,
};

/////////////////////////////////////////////////////////////////////////////
// CFilterChartCtrl

IMPLEMENT_DYNCREATE(CFilterChartCtrl, CFormView)

CFilterChartCtrl::CFilterChartCtrl()
	: CFormView(CFilterChartCtrl::IDD)
{
	//{{AFX_DATA_INIT(CFilterChartCtrl)
	m_screen_x = 0;
	m_screen_y = 0;
	m_nUnsiged = 0;
	m_nBytime  = 0;
	//}}AFX_DATA_INIT
    m_pDoc     = NULL;
}

CFilterChartCtrl::~CFilterChartCtrl()
{
    
}//lint !e1540

void CFilterChartCtrl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterChartCtrl)
	DDX_Control(pDX, IDC_LIST_MATHFML, m_lstFml);
	DDX_Slider(pDX, IDC_SLIDER_H, m_screen_x);
	DDX_Slider(pDX, IDC_SLIDER_V, m_screen_y);
	DDX_Radio(pDX, IDC_RDO_UNSIGNED, m_nUnsiged);
	DDX_Radio(pDX, IDC_RDO_BYTIME, m_nBytime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterChartCtrl, CFormView)
	//{{AFX_MSG_MAP(CFilterChartCtrl)
	ON_BN_CLICKED(IDC_BUTTON_CLEARGRAPH, OnButtonClear)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, OnButtonHide)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MATHFML, OnItemchangedListMathfml)
	ON_BN_CLICKED(IDC_BUTTON_FITPAGE, OnButtonFitpage)
	ON_BN_CLICKED(IDC_BUTTON_FITHEIGHT, OnButtonFitheight)
	ON_BN_CLICKED(IDC_BUTTON_WIDTH, OnButtonWidth)
	ON_BN_CLICKED(IDC_RADIO_POINT, OnRadioPoint)
	ON_BN_CLICKED(IDC_RADIO_DRAG, OnRadioDrag)
	ON_BN_CLICKED(IDC_RADIO_ZOOM, OnRadioZoom)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_CHECK_AUTOSCROLL, OnCheckAutoscroll)
	ON_BN_CLICKED(IDC_RDO_UNSIGNED, OnRdoUnsigned)
	ON_BN_CLICKED(IDC_RDO_SIGNED, OnRdoSigned)
	ON_BN_CLICKED(IDC_RDO_BYTIME, OnRdoBytime)
	ON_BN_CLICKED(IDC_RDO_BYIDX, OnRdoByidx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterChartCtrl diagnostics

#ifdef _DEBUG
void CFilterChartCtrl::AssertValid() const
{
	CFormView::AssertValid();
}

void CFilterChartCtrl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilterChartCtrl message handlers


void CFilterChartCtrl::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

    m_pDoc = (CFilterDoc*)GetDocument();
    m_pDoc->m_pChartCtrl = this;

     //init list control
    DWORD dwStyle = m_lstFml.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES ;
    m_lstFml.SetExtendedStyle(dwStyle);
    m_lstFml.InsertColumn(1, _T("Math Formula:"), LVCFMT_LEFT,200);
    
    //set slide range
    CSliderCtrl * psch = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_H);
    psch->SetRange(1,100);
    psch->SetPos(100);

    CSliderCtrl * pscv = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_V);
    pscv->SetRange(1,100); 
    pscv->SetPos(100);

    OnRadioPoint(); 
}


void CFilterChartCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( pScrollBar != NULL )
    {
        CSliderCtrl * psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_H);
        _ASSERTE( psc != NULL );
        _ASSERTE( psc->m_hWnd == pScrollBar->m_hWnd );

        int slidePos= psc->GetPos();
        int x_range = slidePos*1000;
 
        CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;        
        
        POSITION pos = m_lstFml.GetFirstSelectedItemPosition();
        if (pos == NULL) return;
        
        int nItem = m_lstFml.GetNextSelectedItem(pos);
        int index = m_vecGphIdx[nItem];
        CGraphProps*   grprop = graph_wnd.GetGraph(index); 


        SSinglePoint*  cross_point= grprop->GetCrossPoint();

        double X1, X2, Y1, Y2;   
        graph_wnd.GetGraphWorldCoords(&X1, &X2, &Y1, &Y2);
        graph_wnd.SetGraphWorldCoords(cross_point->x-x_range, cross_point->x+x_range , Y1, Y2);        
    }
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFilterChartCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( pScrollBar != NULL )
    {
        CSliderCtrl * psc = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_V);
        _ASSERTE( psc != NULL );
        _ASSERTE( psc->m_hWnd == pScrollBar->m_hWnd );

        int slidePos= psc->GetPos();

        int y_range = slidePos*10;
        CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;        
        
        POSITION pos = m_lstFml.GetFirstSelectedItemPosition();
        if (pos == NULL) return;
        
        int nItem = m_lstFml.GetNextSelectedItem(pos);
        int index = m_vecGphIdx[nItem];
        CGraphProps*   grprop = graph_wnd.GetGraph(index); 
        SSinglePoint*  cross_point= grprop->GetCrossPoint();

        double X1, X2, Y1, Y2;     
        graph_wnd.GetGraphWorldCoords(&X1, &X2, &Y1, &Y2);

        graph_wnd.SetGraphWorldCoords(X1, X2 , cross_point->y-y_range , cross_point->y+y_range);        
        TRACE("%0x - %0x\n",cross_point->y-y_range, cross_point->y+y_range);
    }

	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CFilterChartCtrl::OnButtonAdd() 
{
    CFmlDlg fmldlg;
    if(IDOK != fmldlg.DoModal())
        return;

    CMathFormula* pMathFml= fmldlg.m_pMathFml;
    ASSERT(pMathFml != NULL);
    
    TP_FILTER_DATA* pFilter_Data= &fmldlg.m_Filter_Data;
    ASSERT(pFilter_Data != NULL);

    CString strFml= fmldlg.m_strFml;
    

    int graph_index=  m_pDoc->m_pChartView->CreateGraph(pFilter_Data, pMathFml);
    if (-1 == graph_index)
        return ;

    m_vecGphIdx.push_back(graph_index);
    m_vecFmlStr.push_back(strFml.GetBuffer(strFml.GetLength()));
    strFml.ReleaseBuffer();
    
    m_lstFml.InsertItem(m_lstFml.GetItemCount(), strFml);
    m_lstFml.SetItemState(m_lstFml.GetItemCount()-1,LVIS_SELECTED, LVIS_SELECTED);
    
    GetDlgItem(IDC_BUTTON_ADD)->SetFocus();
}

void CFilterChartCtrl::OnButtonDel() 
{
    POSITION pos = m_lstFml.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nListItem = m_lstFml.GetNextSelectedItem(pos);
        
        int nGraphIdx = m_vecGphIdx[nListItem];

        m_pDoc->m_pChartView->RemoveGraph(nGraphIdx); 

        m_vecGphIdx.erase(m_vecGphIdx.begin()+nListItem);
        
        m_lstFml.DeleteItem(nListItem);           
    }
	
}


void CFilterChartCtrl::OnButtonHide() 
{
    POSITION pos = m_lstFml.GetFirstSelectedItemPosition();
    if (pos == NULL) return;

    int nItem = m_lstFml.GetNextSelectedItem(pos);
    int index = m_vecGphIdx[nItem];

    CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
    CGraphProps*   grprop= graph_wnd.GetGraph(index); 

    CString strTxt;
    CButton*pButHide= (CButton*)GetDlgItem(IDC_BUTTON_HIDE);
    pButHide->GetWindowText(strTxt);
    if (strTxt == "Hide")
    {
        pButHide->SetWindowText("Show");
        grprop->SetVisible(FALSE);
    }
    else
    {
        pButHide->SetWindowText("Hide");
        grprop->SetVisible(TRUE);
    }

    graph_wnd.UpdateWindows((unsigned long)GRAPH_WUV_ALL);

}


void CFilterChartCtrl::OnButtonExport() 
{
    POSITION pos = m_lstFml.GetFirstSelectedItemPosition();
    if (pos == NULL) return;

    int nItem = m_lstFml.GetNextSelectedItem(pos);
    int index = m_vecGphIdx[nItem];

    CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
    CGraphProps*   grprop= graph_wnd.GetGraph(index); 
	    
    
    if(grprop->GetSize() > 0 )
    {			
		CFileDialog filedlg( FALSE, _T("dat"), 
			NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("dsplogger files (*.dat)|*.dat|"), NULL );
		
		if( filedlg.DoModal() != IDOK )
		{
			return;
		}		
		CString strLogFile = filedlg.GetPathName();
		
		FILE* pf = fopen( strLogFile,"w" );
		if( NULL == pf )
		{
			// Could not open file
			return ;
		}

		CWaitCursor waitCursor;	
        
        ULONG count = grprop->GetSize(); 
		for(ULONG i = 0;i < count; i++)
		{
            double x,y;
            grprop->GetPoint(i,&x,&y);
			fprintf(pf,"%08f    %d\n",y, (DWORD)x);
		}
		
		fclose( pf );		
	}
}

void CFilterChartCtrl::OnItemchangedListMathfml(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    POSITION pos = m_lstFml.GetFirstSelectedItemPosition();
    if (pos == NULL)
        return;    
    int nItem = m_lstFml.GetNextSelectedItem(pos);
    int index = m_vecGphIdx[nItem];
    
    //set current select graph
    CGraphWnd& graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
    graph_wnd.SetGraphSelect(index);

    //set IDC_BUTTON_HIDE status    
    CGraphProps* grprop= graph_wnd.GetGraph(index);
    CWnd* wnd = GetDlgItem(IDC_BUTTON_HIDE);
    if (wnd == NULL) return;
    if (grprop->IsVisible()) 
    {
        wnd->SetWindowText(_T("Hide"));
    } 
    else
    {
        wnd->SetWindowText(_T("Show"));
    };
	
	*pResult = 0;
}

void CFilterChartCtrl::OnButtonFitpage() 
{
    DoFit(FIT_PAGE);
}


void CFilterChartCtrl::OnButtonFitheight() 
{
    DoFit(FIT_HEIGHT);
}

void CFilterChartCtrl::OnButtonWidth() 
{
    DoFit(FIT_WIDTH);	
}

void CFilterChartCtrl::DoFit(UINT fitType)
{
    CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;

    double minx, maxx, miny, maxy;
    if (!graph_wnd.GetBoundRect(&minx, &maxx, &miny, &maxy)) 
        return;

    double X1=0, X2=0, Y1=0, Y2=0;   
    graph_wnd.GetGraphWorldCoords(&X1, &X2, &X1, &X2);

 
    switch (fitType)
    {
    case FIT_HEIGHT:
        {
            Y1 = miny; Y2 = maxy*1.2;
        }; break;
    case FIT_WIDTH:
        {
            X1 = minx; X2 = maxx;
        }; break;
    case FIT_PAGE:
        {
            Y1 = miny; Y2 = maxy*1.2;
            X1 = minx; X2 = maxx;
        }; break;
    };

    graph_wnd.SetGraphWorldCoords(X1, X2, Y1, Y2, TRUE);     

}


void CFilterChartCtrl::OnRadioPoint() 
{
   CButton* pButPoint= (CButton*)GetDlgItem(IDC_RADIO_POINT);
   CButton* pButDrag = (CButton*)GetDlgItem(IDC_RADIO_DRAG);
   CButton* pButZoom = (CButton*)GetDlgItem(IDC_RADIO_ZOOM);
   pButPoint->SetCheck(TRUE);
   pButDrag->SetCheck(FALSE);
   pButZoom->SetCheck(FALSE);
   CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
   graph_wnd.m_opt = OPT_POINT;

}

void CFilterChartCtrl::OnRadioDrag() 
{
   CButton* pButPoint= (CButton*)GetDlgItem(IDC_RADIO_POINT);
   CButton* pButDrag = (CButton*)GetDlgItem(IDC_RADIO_DRAG);
   CButton* pButZoom = (CButton*)GetDlgItem(IDC_RADIO_ZOOM);
   pButPoint->SetCheck(FALSE);
   pButDrag->SetCheck(TRUE);
   pButZoom->SetCheck(FALSE);
   CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
   graph_wnd.m_opt = OPT_DRAG;
	
}

void CFilterChartCtrl::OnRadioZoom() 
{
   CButton* pButPoint= (CButton*)GetDlgItem(IDC_RADIO_POINT);
   CButton* pButDrag = (CButton*)GetDlgItem(IDC_RADIO_DRAG);
   CButton* pButZoom = (CButton*)GetDlgItem(IDC_RADIO_ZOOM);
   pButPoint->SetCheck(FALSE);
   pButDrag->SetCheck(FALSE);
   pButZoom->SetCheck(TRUE);
   CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
   graph_wnd.m_opt = OPT_ZOOM;
}

void CFilterChartCtrl::OnButtonStart() 
{
    //EnableAxisCtrl(FALSE);

//    CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
    CString strTitle;
    GetDlgItem(IDC_BUTTON_START)->GetWindowText(strTitle);
    if (strTitle == "Start")
    {
       GetDlgItem(IDC_BUTTON_START)->SetWindowText("Stop");
       m_pDoc->m_pChartView->StartFilter();
    }
    else
    {
       GetDlgItem(IDC_BUTTON_START)->SetWindowText("Start");
       m_pDoc->m_pChartView->StopFilter();
    }	
}

void CFilterChartCtrl::OnButtonReset() 
{
    for (int i=0; i< m_vecGphIdx.size(); i++)
    {
        int graph_index= m_vecGphIdx[i];
        m_pDoc->m_pChartView->ResetGraph(graph_index);
    } 

}

void CFilterChartCtrl::OnButtonClear() 
{
    CGraphWnd&  graph_wnd= m_pDoc->m_pChartView->m_graph_wnd;
    
    for (int i=0; i< m_vecGphIdx.size(); i++)
    {
        graph_wnd.ClearGraph(m_vecGphIdx[i]);
    }      

}


void CFilterChartCtrl::OnCheckAutoscroll() 
{
    BOOL &bAutoScroll= m_pDoc->m_pChartView->m_bAutoScroll;
    if (bAutoScroll)
    {
        bAutoScroll= FALSE;
        ((CButton*)GetDlgItem(IDC_CHECK_AUTOSCROLL))->SetCheck(FALSE);
    }
	else
    {
        bAutoScroll= TRUE;
        ((CButton*)GetDlgItem(IDC_CHECK_AUTOSCROLL))->SetCheck(TRUE);
    }
}

void CFilterChartCtrl::EnableAxisCtrl(BOOL bEnable)
{
    GetDlgItem(IDC_RDO_UNSIGNED)->EnableWindow(bEnable);
    GetDlgItem(IDC_RDO_SIGNED)->EnableWindow(bEnable);
    GetDlgItem(IDC_RDO_BYTIME)->EnableWindow(bEnable);
    GetDlgItem(IDC_RDO_BYIDX)->EnableWindow(bEnable);
}

void CFilterChartCtrl::OnRdoUnsigned() 
{
    m_nUnsiged = 0;
    m_pDoc->m_pChartView->m_bUnsiged = TRUE;
    OnButtonReset();
    UpdateData();
}

void CFilterChartCtrl::OnRdoSigned() 
{
    m_nUnsiged = -1;
    m_pDoc->m_pChartView->m_bUnsiged = FALSE;
    OnButtonReset();
    UpdateData();    
}

void CFilterChartCtrl::OnRdoBytime() 
{
	m_nBytime = 0;
    m_pDoc->m_pChartView->m_bByTime = TRUE;    
    OnButtonReset();
    UpdateData();
}

void CFilterChartCtrl::OnRdoByidx() 
{
	m_nBytime = -1;    
    m_pDoc->m_pChartView->m_bByTime = FALSE;   
    OnButtonReset();
    UpdateData();
}
