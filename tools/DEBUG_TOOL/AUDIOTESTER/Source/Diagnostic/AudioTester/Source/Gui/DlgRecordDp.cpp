// DlgRecordDp.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgRecordDp.h"
#include <math.h>
#include "DlgRecordDpInputGain.h"
#include "DlgRecorDpAlc.h"
#include "DlgRcordDpLcf.h"
#include "DlgRecordDpEq.h"
#include "DlgAnalogGain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDp dialog


CDlgRecordDp::CDlgRecordDp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecordDp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecordDp)
		// NOTE: the ClassWizard will add member initialization here	
	//}}AFX_DATA_INIT
	ZeroMemory(&m_VolumeData, sizeof(VOLUME_STRUCT));
	m_bDspCodecInfo = FALSE;
	m_bSmartPhone = FALSE;
}


void CDlgRecordDp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecordDp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_STATIC_ARROW1, m_stcArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2, m_stcArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3, m_stcArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4, m_stcArrow4);
	DDX_Control(pDX, IDC_STATIC_ARROW5, m_stcArrow5);
	DDX_Control(pDX, IDC_STATIC_ARROW6, m_stcArrow6);
	DDX_Control(pDX, IDC_STATIC_ARROW7, m_stcArrow7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRecordDp, CDialog)
	//{{AFX_MSG_MAP(CDlgRecordDp)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_DP_INPUT_GAIN, OnBtnDpInputGain)
	ON_BN_CLICKED(IDC_BTN_DP_ALC, OnBtnDpAlc)
	ON_BN_CLICKED(IDC_BTN_DP_LCF, OnBtnDpLcf)
	ON_BN_CLICKED(IDC_BTN_DP_EQ, OnBtnDpEq)
	ON_BN_CLICKED(IDC_BUTTON_ANALOG_GAIN, OnBtnAnalogGain)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDp message handlers

BOOL CDlgRecordDp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_bDspCodecInfo || m_bSmartPhone)
	{
		GetDlgItem(IDC_BUTTON_ANALOG_GAIN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ANALOG_GAIN)->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRecordDp::SetValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecordDp::FetchValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecordDp::DrawTransparentBitmap(CDC *pDC, UINT IDImage, CRect &rect, COLORREF rgbMask)  
{  
	CDC ImageDC;
	CDC MaskDC;  
	CBitmap Image;
	CBitmap *pOldImage;  
	CBitmap maskBitmap;
	CBitmap *pOldMaskDCBitmap   ;  
	
	Image.LoadBitmap(IDImage);  
	ImageDC.CreateCompatibleDC(pDC);  
	
	pOldImage   =   ImageDC.SelectObject(&Image);  
	
	MaskDC.CreateCompatibleDC(pDC);  
	
	maskBitmap.CreateBitmap(rect.Width(),   rect.Height(),   1,   1,   NULL);  
	
	pOldMaskDCBitmap   =   MaskDC.SelectObject(   &maskBitmap   );  
	
	ImageDC.SetBkColor(rgbMask);  
	
	MaskDC.BitBlt(0,   0,   rect.Width(),   rect.Height(),   &ImageDC,   0,   0,   SRCCOPY);  
	
	ImageDC.SetBkColor(RGB(0,   0,   0));  
	ImageDC.SetTextColor(RGB(255,   255,   255));  
	ImageDC.BitBlt(0,   0,   rect.Width(),   rect.Height(),   &MaskDC,   0,   0,   SRCAND);  
	
	pDC-> BitBlt(rect.left,rect.top,rect.Width(),   rect.Height(),   &MaskDC,   0,   0,   SRCAND);  
	pDC-> BitBlt(rect.left,rect.top,rect.Width(),   rect.Height(),   &ImageDC,   0,   0,SRCPAINT);  
	
	MaskDC.SelectObject(pOldMaskDCBitmap);  
	ImageDC.SelectObject(pOldImage);  
}

void CDlgRecordDp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	
	m_stcArrow1.GetWindowRect(rect);
	ScreenToClient(rect);
	DrawTransparentBitmap(&dc, IDB_BITMAP_ARROW, rect, RGB(255,255,255));

	m_stcArrow2.GetWindowRect(rect);
	ScreenToClient(rect);
	DrawTransparentBitmap(&dc, IDB_BITMAP_ARROW, rect, RGB(255,255,255));

	m_stcArrow3.GetWindowRect(rect);
	ScreenToClient(rect);
	DrawTransparentBitmap(&dc, IDB_BITMAP_ARROW, rect, RGB(255,255,255));

	m_stcArrow4.GetWindowRect(rect);
	ScreenToClient(rect);
	DrawTransparentBitmap(&dc, IDB_BITMAP_ARROW, rect, RGB(255,255,255));

	m_stcArrow5.GetWindowRect(rect);
	ScreenToClient(rect);
	DrawTransparentBitmap(&dc, IDB_BITMAP_CURVE_ARROW, rect, RGB(255,255,255));

	m_stcArrow6.GetWindowRect(rect);
	ScreenToClient(rect);
	DrawTransparentBitmap(&dc, IDB_BITMAP_ARROW, rect, RGB(255,255,255));

	m_stcArrow7.GetWindowRect(rect);
	ScreenToClient(rect);
	DrawTransparentBitmap(&dc, IDB_BITMAP_ARROW, rect, RGB(255,255,255));
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgRecordDp::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDlgRecordDp::OnBtnDpInputGain() 
{
	// TODO: Add your control notification handler code here
	CDlgRecordDpInputGain dlg;
	dlg.SetValue(m_eq_data);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_eq_data);
	}
	
}

void CDlgRecordDp::OnBtnDpAlc() 
{
	// TODO: Add your control notification handler code here
	CDlgRecorDpAlc dlg;
	dlg.SetValue(m_eq_data);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_eq_data);
	}
}

void CDlgRecordDp::OnBtnDpLcf() 
{
	// TODO: Add your control notification handler code here
	CDlgRcordDpLcf dlg;
	dlg.SetValue(m_eq_data);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_eq_data);
	}
}

void CDlgRecordDp::OnBtnDpEq() 
{
	// TODO: Add your control notification handler code here
	CDlgRecordDpEq dlg;
	dlg.SetValue(m_eq_data);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_eq_data);
	}
	
}

void CDlgRecordDp::OnBtnAnalogGain()
{
	CDlgAnalogGain dlg;
	dlg.SetValue(m_VolumeData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.FetchValue(m_VolumeData);
	}
}

int CDlgRecordDp::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDlgRecordDp::SetArmValue( VOLUME_STRUCT& volume_data )
{
	memcpy(&m_VolumeData, &volume_data, sizeof(VOLUME_STRUCT));
}

void CDlgRecordDp::FetchArmValue( VOLUME_STRUCT& volume_data )
{
	memcpy(&volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
}

void CDlgRecordDp::SetControlFlag(BOOL bDspCodecInfo, BOOL bSmartPhone)
{
	m_bDspCodecInfo = bDspCodecInfo;
	m_bSmartPhone = bSmartPhone;
}