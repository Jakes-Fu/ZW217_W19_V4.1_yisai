// SysInfoPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "SysInfoPropertyPage.h"
#include "ConPanPropertySheet.h"
#include "ConPanLayer.h"
#include "ConPanFunc.h"
#include "ConPan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysInfoPropertyPage property page

IMPLEMENT_DYNCREATE(CSysInfoPropertyPage, CPropertyPage)

CSysInfoPropertyPage::CSysInfoPropertyPage() : CPropertyPage(CSysInfoPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CSysInfoPropertyPage)
	//}}AFX_DATA_INIT
}

CSysInfoPropertyPage::~CSysInfoPropertyPage()
{
    
}

void CSysInfoPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysInfoPropertyPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysInfoPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSysInfoPropertyPage)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_PLMN_MCC, OnChangeEditPlmnMcc)
	ON_EN_CHANGE(IDC_EDIT_PLMN_MNC, OnChangeEditPlmnMnc)
    /*@jassmine MS00004749 2003-9-15*/
    ON_EN_CHANGE(IDC_EDIT_PLMN_LAC,OnChangeEditPlmnLac)
    ON_EN_CHANGE(IDC_EDIT_PLMN_CELL,OnChangeEditPlmnCell)
    ON_CBN_SELCHANGE(IDC_COMBO_CHARGE_STATUS, OnSelchangeComboChargeStatus)
    /*@End MS00004749 2003-9-15*/
	ON_CBN_SELCHANGE(IDC_COMBO_PLMN_SVRSTATE, OnSelchangeComboPlmnSvrstate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BATTERY, OnReleasedcaptureSliderBattery)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
    /*@jassmine MS00004749 2003-9-15*/
    ON_BN_CLICKED(IDC_BUTTON_APPLY1, OnButtonApply1)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL1, OnButtonCancel1)
    /*@End MS00004749 2003-9-15*/
	ON_BN_CLICKED(IDC_BUTTON_SIMCARD, OnButtonSimcard)
	ON_BN_CLICKED(IDC_BUTTON_SETSIMCARD, OnButtonSetsimcard)
    ON_BN_CLICKED(IDC_CHECK_AUTOPAGE, OnCheckAutopage)
	ON_BN_CLICKED(IDC_BUTTON_APPLY2, OnButtonApply2)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL2, OnButtonCancel2)
    ON_EN_CHANGE(IDC_EDIT_PLMN_CELL2, OnChangeEditPlmnCell2)
    ON_EN_CHANGE(IDC_EDIT_PLMN_LAC2, OnChangeEditPlmnLac2)
    ON_EN_CHANGE(IDC_EDIT_PLMN_MCC2, OnChangeEditPlmnMcc2)
    ON_EN_CHANGE(IDC_EDIT_PLMN_MNC2, OnChangeEditPlmnMnc2)
	ON_CBN_SELCHANGE(IDC_COMBO_PLMN_SVRSTATE2, OnSelchangeComboPlmnSvrstate2)
    ON_BN_CLICKED(IDC_BUTTON_SETSIMCARD2, OnButtonSetsimcard2)
    ON_BN_CLICKED(IDC_BUTTON_SIMCARD2, OnButtonSimcard2)
	//apple add for third sim card
	ON_BN_CLICKED(IDC_BUTTON_APPLY3, OnButtonApply3)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL3, OnButtonCancel3)
    ON_EN_CHANGE(IDC_EDIT_PLMN_CELL3, OnChangeEditPlmnCell3)
    ON_EN_CHANGE(IDC_EDIT_PLMN_LAC3, OnChangeEditPlmnLac3)
    ON_EN_CHANGE(IDC_EDIT_PLMN_MCC3, OnChangeEditPlmnMcc3)
    ON_EN_CHANGE(IDC_EDIT_PLMN_MNC3, OnChangeEditPlmnMnc3)
	ON_CBN_SELCHANGE(IDC_COMBO_PLMN_SVRSTATE3, OnSelchangeComboPlmnSvrstate3)
    ON_BN_CLICKED(IDC_BUTTON_SETSIMCARD3, OnButtonSetsimcard3)
    ON_BN_CLICKED(IDC_BUTTON_SIMCARD3, OnButtonSimcard3)
	//@hongliang 2010-11-1 for fourth sim card
	ON_BN_CLICKED(IDC_BUTTON_APPLY4, OnButtonApply4)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL4, OnButtonCancel4)
    ON_EN_CHANGE(IDC_EDIT_PLMN_CELL4, OnChangeEditPlmnCell4)
    ON_EN_CHANGE(IDC_EDIT_PLMN_LAC4, OnChangeEditPlmnLac4)
    ON_EN_CHANGE(IDC_EDIT_PLMN_MCC4, OnChangeEditPlmnMcc4)
    ON_EN_CHANGE(IDC_EDIT_PLMN_MNC4, OnChangeEditPlmnMnc4)
	ON_CBN_SELCHANGE(IDC_COMBO_PLMN_SVRSTATE4, OnSelchangeComboPlmnSvrstate4)
    ON_BN_CLICKED(IDC_BUTTON_SETSIMCARD4, OnButtonSetsimcard4)
    ON_BN_CLICKED(IDC_BUTTON_SIMCARD4, OnButtonSimcard4)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysInfoPropertyPage message handlers

void CSysInfoPropertyPage::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::PostNcDestroy();
	//delete this;
}

BOOL CSysInfoPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSliderCtrl * psdSigLev=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV);
	CSliderCtrl * psdBattery=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_BATTERY);
	CEdit *pedPLMNMcc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC);
	CEdit *pedPLMNMnc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC);

    /*@jassmine MS00004749 2003-9-15*/
    CEdit *pedPLMNLac=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC);
    CEdit *pedPLMNCell=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL);

	CComboBox *pcbPLMNss=(CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE);

    /*@jassmine MS00004749 2003-9-15*/
    CComboBox *pcbCHARGEss=(CComboBox *) GetDlgItem(IDC_COMBO_CHARGE_STATUS);

	CStatic *pstPower=(CStatic *) GetDlgItem(IDC_STATIC_POWER);
	CButton *pbtApply=(CButton *) GetDlgItem(IDC_BUTTON_APPLY);
	CButton *pbtCancel=(CButton *) GetDlgItem(IDC_BUTTON_CANCEL);
    /*@jassmine MS00004749 2003-9-15*/
    CButton *pbtApply1=(CButton *) GetDlgItem(IDC_BUTTON_APPLY1);
	CButton *pbtCancel1=(CButton *) GetDlgItem(IDC_BUTTON_CANCEL1);

	CButton *pbtSimCard=(CButton *) GetDlgItem(IDC_BUTTON_SIMCARD);
	CButton *pbtAutoPage=(CButton *) GetDlgItem(IDC_CHECK_AUTOPAGE);

	psdSigLev->SetRange(0, 63);
	psdBattery->SetRange(0, 5);
	psdSigLev->SetPos(63);
	psdBattery->SetPos(5);	
	SetDlgItemInt(IDC_STATIC_BATTERY, psdBattery->GetPos());
	SetDlgItemInt(IDC_STATIC_SIGLEV, psdSigLev->GetPos());

	pedPLMNMcc->SetWindowText("460");
	pedPLMNMnc->SetWindowText("00");
	pcbPLMNss->InsertString(-1, "No Cell");
	pcbPLMNss->InsertString(-1, "Normal");
	pcbPLMNss->InsertString(-1, "Limited");
	pcbPLMNss->SetCurSel(1);
	pstPower->SetWindowText("OFF");
    /*@jassmine MS00004749 2003-9-15*/
    pedPLMNLac->SetWindowText("1000");
	pedPLMNCell->SetWindowText("1000");
    pcbCHARGEss->InsertString(-1, "ON");
	pcbCHARGEss->InsertString(-1, "OFF");
	pcbCHARGEss->SetCurSel(1);
    pbtApply1->EnableWindow(FALSE);
	pbtCancel1->EnableWindow(FALSE);
    /*End MS00004749 2003-9-15*/
	pbtApply->EnableWindow(FALSE);
	pbtCancel->EnableWindow(FALSE);
    UpdatePLMNInfoData(TRUE);
	UpdateChargeInfoData(TRUE);

    m_bAutoPage = TRUE;
    pbtAutoPage->SetCheck(1);

    // for the second simcard controls
    psdSigLev=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV2);
    pedPLMNMcc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC2);
    pedPLMNMnc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC2);
    
    pedPLMNLac=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC2);
    pedPLMNCell=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL2);
    
    pcbPLMNss=(CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE2);
    
    psdSigLev->SetRange(0, 63);
    psdSigLev->SetPos(63);
    SetDlgItemInt(IDC_STATIC_SIGLEV2, psdSigLev->GetPos());
    
    pedPLMNMcc->SetWindowText("460");
    pedPLMNMnc->SetWindowText("00");
    pcbPLMNss->InsertString(-1, "No Cell");
    pcbPLMNss->InsertString(-1, "Normal");
    pcbPLMNss->InsertString(-1, "Limited");
    pcbPLMNss->SetCurSel(1);
    pstPower->SetWindowText("OFF");
    pedPLMNLac->SetWindowText("1000");
    pedPLMNCell->SetWindowText("1000");
    
    pbtApply=(CButton *) GetDlgItem(IDC_BUTTON_APPLY2);
    pbtCancel=(CButton *) GetDlgItem(IDC_BUTTON_CANCEL2);
    
    pbtApply->EnableWindow(FALSE);
    pbtCancel->EnableWindow(FALSE);
    
    UpdatePLMNInfoData2(TRUE);

	//apple add for third sim card
    psdSigLev=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV3);
    pedPLMNMcc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC3);
    pedPLMNMnc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC3);
    
    pedPLMNLac=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC3);
    pedPLMNCell=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL3);
    
    pcbPLMNss=(CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE3);
    
    psdSigLev->SetRange(0, 63);
    psdSigLev->SetPos(63);
    SetDlgItemInt(IDC_STATIC_SIGLEV3, psdSigLev->GetPos());
    
    pedPLMNMcc->SetWindowText("460");
    pedPLMNMnc->SetWindowText("00");
    pcbPLMNss->InsertString(-1, "No Cell");
    pcbPLMNss->InsertString(-1, "Normal");
    pcbPLMNss->InsertString(-1, "Limited");
    pcbPLMNss->SetCurSel(1);
    pstPower->SetWindowText("OFF");
    pedPLMNLac->SetWindowText("1000");
    pedPLMNCell->SetWindowText("1000");
    
    pbtApply=(CButton *) GetDlgItem(IDC_BUTTON_APPLY3);
    pbtCancel=(CButton *) GetDlgItem(IDC_BUTTON_CANCEL3);
    
    pbtApply->EnableWindow(FALSE);
    pbtCancel->EnableWindow(FALSE);
    
    UpdatePLMNInfoData3(TRUE);

	//honliang add for fourth sim card
    psdSigLev=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV4);
    pedPLMNMcc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC4);
    pedPLMNMnc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC4);
    
    pedPLMNLac=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC4);
    pedPLMNCell=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL4);
    
    pcbPLMNss=(CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE4);
    
    psdSigLev->SetRange(0, 63);
    psdSigLev->SetPos(63);
    SetDlgItemInt(IDC_STATIC_SIGLEV4, psdSigLev->GetPos());
    
    pedPLMNMcc->SetWindowText("460");
    pedPLMNMnc->SetWindowText("00");
    pcbPLMNss->InsertString(-1, "No Cell");
    pcbPLMNss->InsertString(-1, "Normal");
    pcbPLMNss->InsertString(-1, "Limited");
    pcbPLMNss->SetCurSel(1);
    pstPower->SetWindowText("OFF");
    pedPLMNLac->SetWindowText("1000");
    pedPLMNCell->SetWindowText("1000");
    
    pbtApply=(CButton *) GetDlgItem(IDC_BUTTON_APPLY4);
    pbtCancel=(CButton *) GetDlgItem(IDC_BUTTON_CANCEL4);
    
    pbtApply->EnableWindow(FALSE);
    pbtCancel->EnableWindow(FALSE);
    
    UpdatePLMNInfoData4(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSysInfoPropertyPage::UpdatePLMNInfoData(BOOL bUpdate  )
{
	CSliderCtrl * psdSigLev = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV);
	CStatic * pstSigLev = (CStatic *) GetDlgItem(IDC_STATIC_SIGLEV);
	CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER); 

	CEdit *pedPLMNMcc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC);
	CEdit *pedPLMNMnc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC);
    /*@jassmine MS00004749 2003-9-15*/
    CEdit *pedPLMNLac = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC);
	CEdit *pedPLMNCell = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL);

	CComboBox *pcbPLMNss = (CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE);
	
	if (bUpdate)
	{
		m_SysInfoData.m_nSigLev = psdSigLev->GetPos();
		m_SysInfoData.m_nPLMNMcc = GetDlgItemInt(IDC_EDIT_PLMN_MCC);
		m_SysInfoData.m_nPLMNMnc = GetDlgItemInt(IDC_EDIT_PLMN_MNC);
        /*@jassmine MS00004749 2003-9-15*/
        m_SysInfoData.m_nPLMNLac = GetDlgItemInt(IDC_EDIT_PLMN_LAC);
		m_SysInfoData.m_nPLMNCell = GetDlgItemInt(IDC_EDIT_PLMN_CELL);

		m_SysInfoData.m_nPLMNSvrState = pcbPLMNss->GetCurSel();
        CString csPower;
        GetDlgItemText(IDC_STATIC_POWER, csPower);
        m_SysInfoData.m_nPowerStatus = csPower.CompareNoCase("ON") == 0 ? 1 : 0;
	}
	else
	{
		psdSigLev->SetPos(m_SysInfoData.m_nSigLev);
		SetDlgItemInt(IDC_STATIC_SIGLEV, m_SysInfoData.m_nSigLev);
		SetDlgItemInt(IDC_EDIT_PLMN_MCC, m_SysInfoData.m_nPLMNMcc);
		SetDlgItemInt(IDC_EDIT_PLMN_MNC, m_SysInfoData.m_nPLMNMnc);
        /*@jassmine MS00004749 2003-9-15*/
        SetDlgItemInt(IDC_EDIT_PLMN_LAC, m_SysInfoData.m_nPLMNLac);
		SetDlgItemInt(IDC_EDIT_PLMN_CELL, m_SysInfoData.m_nPLMNCell);

		pcbPLMNss->SetCurSel(m_SysInfoData.m_nPLMNSvrState);
        CString csPower;
        csPower = m_SysInfoData.m_nPowerStatus ? "ON" : "OFF";
        pstPower->SetWindowText(csPower);

	}	
}

void CSysInfoPropertyPage::UpdatePLMNInfoData2(BOOL bUpdate)
{
    CSliderCtrl * psdSigLev = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV2);
    CStatic * pstSigLev = (CStatic *) GetDlgItem(IDC_STATIC_SIGLEV2);
    CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER2); 
    
    CEdit *pedPLMNMcc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC2);
    CEdit *pedPLMNMnc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC2);
    
    CEdit *pedPLMNLac = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC2);
    CEdit *pedPLMNCell = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL2);
    
    CComboBox *pcbPLMNss = (CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE2);
    
    if (bUpdate)
    {
        m_SysInfoData.m_nSigLev2 = psdSigLev->GetPos();
        m_SysInfoData.m_nPLMNMcc2 = GetDlgItemInt(IDC_EDIT_PLMN_MCC2);
        m_SysInfoData.m_nPLMNMnc2 = GetDlgItemInt(IDC_EDIT_PLMN_MNC2);
        
        m_SysInfoData.m_nPLMNLac2 = GetDlgItemInt(IDC_EDIT_PLMN_LAC2);
        m_SysInfoData.m_nPLMNCell2 = GetDlgItemInt(IDC_EDIT_PLMN_CELL2);
        
        m_SysInfoData.m_nPLMNSvrState2 = pcbPLMNss->GetCurSel();
        CString csPower;
        GetDlgItemText(IDC_STATIC_POWER2, csPower);
        m_SysInfoData.m_nPowerStatus2 = csPower.CompareNoCase("ON") == 0 ? 1 : 0;
    }
    else
    {
        psdSigLev->SetPos(m_SysInfoData.m_nSigLev2);
        SetDlgItemInt(IDC_STATIC_SIGLEV2, m_SysInfoData.m_nSigLev2);
        SetDlgItemInt(IDC_EDIT_PLMN_MCC2, m_SysInfoData.m_nPLMNMcc2);
        SetDlgItemInt(IDC_EDIT_PLMN_MNC2, m_SysInfoData.m_nPLMNMnc2);
        /*@jassmine MS00004749 2003-9-15*/
        SetDlgItemInt(IDC_EDIT_PLMN_LAC2, m_SysInfoData.m_nPLMNLac2);
        SetDlgItemInt(IDC_EDIT_PLMN_CELL2, m_SysInfoData.m_nPLMNCell2);
        
        pcbPLMNss->SetCurSel(m_SysInfoData.m_nPLMNSvrState2);
        CString csPower;
        csPower = m_SysInfoData.m_nPowerStatus2 ? "ON" : "OFF";
        pstPower->SetWindowText(csPower);
        
    }	
}


void CSysInfoPropertyPage::UpdatePLMNInfoData3(BOOL bUpdate)
{
    CSliderCtrl * psdSigLev = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV3);
    CStatic * pstSigLev = (CStatic *) GetDlgItem(IDC_STATIC_SIGLEV3);
    CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER3); 
    
    CEdit *pedPLMNMcc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC3);
    CEdit *pedPLMNMnc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC3);
    
    CEdit *pedPLMNLac = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC3);
    CEdit *pedPLMNCell = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL3);
    
    CComboBox *pcbPLMNss = (CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE3);
    
    if (bUpdate)
    {
        m_SysInfoData.m_nSigLev3 = psdSigLev->GetPos();
        m_SysInfoData.m_nPLMNMcc3 = GetDlgItemInt(IDC_EDIT_PLMN_MCC3);
        m_SysInfoData.m_nPLMNMnc3 = GetDlgItemInt(IDC_EDIT_PLMN_MNC3);
        
        m_SysInfoData.m_nPLMNLac3 = GetDlgItemInt(IDC_EDIT_PLMN_LAC3);
        m_SysInfoData.m_nPLMNCell3 = GetDlgItemInt(IDC_EDIT_PLMN_CELL3);
        
        m_SysInfoData.m_nPLMNSvrState3 = pcbPLMNss->GetCurSel();
        CString csPower;
        GetDlgItemText(IDC_STATIC_POWER3, csPower);
        m_SysInfoData.m_nPowerStatus3 = csPower.CompareNoCase("ON") == 0 ? 1 : 0;
    }
    else
    {
        psdSigLev->SetPos(m_SysInfoData.m_nSigLev3);
        SetDlgItemInt(IDC_STATIC_SIGLEV3, m_SysInfoData.m_nSigLev3);
        SetDlgItemInt(IDC_EDIT_PLMN_MCC3, m_SysInfoData.m_nPLMNMcc3);
        SetDlgItemInt(IDC_EDIT_PLMN_MNC3, m_SysInfoData.m_nPLMNMnc3);
        /*@jassmine MS00004749 2003-9-15*/
        SetDlgItemInt(IDC_EDIT_PLMN_LAC3, m_SysInfoData.m_nPLMNLac3);
        SetDlgItemInt(IDC_EDIT_PLMN_CELL3, m_SysInfoData.m_nPLMNCell3);
        
        pcbPLMNss->SetCurSel(m_SysInfoData.m_nPLMNSvrState3);
        CString csPower;
        csPower = m_SysInfoData.m_nPowerStatus3 ? "ON" : "OFF";
        pstPower->SetWindowText(csPower);
        
    }	
}
void CSysInfoPropertyPage::UpdatePLMNInfoData4(BOOL bUpdate)
{
    CSliderCtrl * psdSigLev = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV4);
    CStatic * pstSigLev = (CStatic *) GetDlgItem(IDC_STATIC_SIGLEV4);
    CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER4); 
    
    CEdit *pedPLMNMcc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MCC4);
    CEdit *pedPLMNMnc = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC4);
    
    CEdit *pedPLMNLac = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_LAC4);
    CEdit *pedPLMNCell = (CEdit *) GetDlgItem(IDC_EDIT_PLMN_CELL4);
    
    CComboBox *pcbPLMNss = (CComboBox *) GetDlgItem(IDC_COMBO_PLMN_SVRSTATE4);
    
    if (bUpdate)
    {
        m_SysInfoData.m_nSigLev4 = psdSigLev->GetPos();
        m_SysInfoData.m_nPLMNMcc4 = GetDlgItemInt(IDC_EDIT_PLMN_MCC4);
        m_SysInfoData.m_nPLMNMnc4 = GetDlgItemInt(IDC_EDIT_PLMN_MNC4);
        
        m_SysInfoData.m_nPLMNLac4 = GetDlgItemInt(IDC_EDIT_PLMN_LAC4);
        m_SysInfoData.m_nPLMNCell4 = GetDlgItemInt(IDC_EDIT_PLMN_CELL4);
        
        m_SysInfoData.m_nPLMNSvrState4 = pcbPLMNss->GetCurSel();
        CString csPower;
        GetDlgItemText(IDC_STATIC_POWER4, csPower);
        m_SysInfoData.m_nPowerStatus4 = csPower.CompareNoCase("ON") == 0 ? 1 : 0;
    }
    else
    {
        psdSigLev->SetPos(m_SysInfoData.m_nSigLev4);
        SetDlgItemInt(IDC_STATIC_SIGLEV4, m_SysInfoData.m_nSigLev4);
        SetDlgItemInt(IDC_EDIT_PLMN_MCC4, m_SysInfoData.m_nPLMNMcc4);
        SetDlgItemInt(IDC_EDIT_PLMN_MNC4, m_SysInfoData.m_nPLMNMnc4);
        /*@jassmine MS00004749 2003-9-15*/
        SetDlgItemInt(IDC_EDIT_PLMN_LAC4, m_SysInfoData.m_nPLMNLac4);
        SetDlgItemInt(IDC_EDIT_PLMN_CELL4, m_SysInfoData.m_nPLMNCell4);
        
        pcbPLMNss->SetCurSel(m_SysInfoData.m_nPLMNSvrState4);
        CString csPower;
        csPower = m_SysInfoData.m_nPowerStatus4 ? "ON" : "OFF";
        pstPower->SetWindowText(csPower);
        
    }	
}
/*@jassmine MS00004749 2003-9-16*/
void CSysInfoPropertyPage::UpdateChargeInfoData(BOOL bUpdate)
{
	CSliderCtrl * psdBattery = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_BATTERY);
	CStatic * pstBattery = (CStatic *) GetDlgItem(IDC_STATIC_BATTERY);
    CComboBox *pcbCHARGEss = (CComboBox *) GetDlgItem(IDC_COMBO_CHARGE_STATUS);	

	if (bUpdate)
	{
		m_SysInfoData.m_nBattery = psdBattery->GetPos();
        m_SysInfoData.m_nCHARGEStatus = pcbCHARGEss->GetCurSel();
	}
	else
	{
		psdBattery->SetPos(m_SysInfoData.m_nBattery);
		SetDlgItemInt(IDC_STATIC_BATTERY, m_SysInfoData.m_nBattery);
        pcbCHARGEss->SetCurSel(m_SysInfoData.m_nCHARGEStatus);
	}	
}
/*@End MS00004749 2003-9-16*/

void CSysInfoPropertyPage::DealCPMsg(void *pmsg)
{
	ASSERT(pmsg);
	xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;
	
	switch(psig->SignalCode)
	{
	case MN_CAMP_ON_REQ:
		{
			MN_CAMP_ON_REQ_T * req_ptr = (MN_CAMP_ON_REQ_T*)psig;
			xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			MN_SERVICE_STATE_E svrstate;

			InnerSCICreateSignal(&sig_ptr,
				MN_CAMP_ON_CNF,
				sizeof(MN_CAMP_ON_CNF_T),
				P_CPANEL);
			
			if (MN_DUAL_SYS_1 == req_ptr->dual_sys)
			{
				if (m_SysInfoData.m_nPLMNSvrState == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState == 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}

				((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_req = req_ptr->plmn;  //the request plmn
				
				if (svrstate == MN_PLMN_NO_CELL)            //no cell available
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = FALSE;
				}else
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = TRUE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mcc = m_SysInfoData.m_nPLMNMcc;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc = m_SysInfoData.m_nPLMNMnc;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc_digit_num = 2;
					
					if (!req_ptr->sel_any_plmn)
					{
						//the request plmn equals the available plmn
						if (req_ptr->plmn.mcc == m_SysInfoData.m_nPLMNMcc &&
							req_ptr->plmn.mnc == m_SysInfoData.m_nPLMNMnc &&
							req_ptr->plmn.mnc_digit_num == 2)
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
						}else
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
						}
					}else
					{
						((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
					}

					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac;			  
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.cell_id = m_SysInfoData.m_nPLMNCell;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.rxlev = m_SysInfoData.m_nSigLev;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.arfcn = 0;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.bsic = 0;
				}

				m_SysInfoData.m_nPowerStatus = 1;
				CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER); 
				CString csPower;
				csPower = m_SysInfoData.m_nPowerStatus ? "ON" : "OFF";
				pstPower->SetWindowText(csPower);
			}
			else if (MN_DUAL_SYS_2 == req_ptr->dual_sys)
			{
				if (m_SysInfoData.m_nPLMNSvrState2 == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState2 == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState2 == 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}

				((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_req = req_ptr->plmn;  //the request plmn
				
				if (svrstate == MN_PLMN_NO_CELL)            //no cell available
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = FALSE;
				}else
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = TRUE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mcc = m_SysInfoData.m_nPLMNMcc2;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc = m_SysInfoData.m_nPLMNMnc2;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc_digit_num = 2;
					
					if (!req_ptr->sel_any_plmn)
					{
						//the request plmn equals the available plmn
						if (req_ptr->plmn.mcc == m_SysInfoData.m_nPLMNMcc2 &&
							req_ptr->plmn.mnc == m_SysInfoData.m_nPLMNMnc2 &&
							req_ptr->plmn.mnc_digit_num == 2)
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
						}else
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
						}
					}else
					{
						((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
					}

					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac2;			  
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.cell_id = m_SysInfoData.m_nPLMNCell2;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.rxlev = m_SysInfoData.m_nSigLev2;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.arfcn = 0;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.bsic = 0;
				}

				m_SysInfoData.m_nPowerStatus2 = 1;
				CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER2); 
				CString csPower;
				csPower = m_SysInfoData.m_nPowerStatus2 ? "ON" : "OFF";
				pstPower->SetWindowText(csPower);
			}
			else if ((MN_DUAL_SYS_1+2) == req_ptr->dual_sys)
			{
				if (m_SysInfoData.m_nPLMNSvrState3 == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState3 == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState3 == 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}
				
				((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_req = req_ptr->plmn;  //the request plmn
				
				if (svrstate == MN_PLMN_NO_CELL)            //no cell available
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = FALSE;
				}else
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = TRUE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mcc = m_SysInfoData.m_nPLMNMcc3;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc = m_SysInfoData.m_nPLMNMnc3;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc_digit_num = 2;
					
					if (!req_ptr->sel_any_plmn)
					{
						//the request plmn equals the available plmn
						if (req_ptr->plmn.mcc == m_SysInfoData.m_nPLMNMcc3 &&
							req_ptr->plmn.mnc == m_SysInfoData.m_nPLMNMnc3 &&
							req_ptr->plmn.mnc_digit_num == 2)
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
						}else
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
						}
					}else
					{
						((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
					}
					
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac3;			  
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.cell_id = m_SysInfoData.m_nPLMNCell3;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.rxlev = m_SysInfoData.m_nSigLev3;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.arfcn = 0;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.bsic = 0;
				}
				
				m_SysInfoData.m_nPowerStatus3 = 1;
				CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER3); 
				CString csPower;
				csPower = m_SysInfoData.m_nPowerStatus3 ? "ON" : "OFF";
				pstPower->SetWindowText(csPower);
			}
			else
			{
				if (m_SysInfoData.m_nPLMNSvrState4 == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState4 == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState4 == 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}
				
				((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_req = req_ptr->plmn;  //the request plmn
				
				if (svrstate == MN_PLMN_NO_CELL)            //no cell available
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = FALSE;
				}else
				{
					((MN_CAMP_ON_CNF_T *)sig_ptr)->is_plmn_available = TRUE;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mcc = m_SysInfoData.m_nPLMNMcc4;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc = m_SysInfoData.m_nPLMNMnc4;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->plmn_available.mnc_digit_num = 2;
					
					if (!req_ptr->sel_any_plmn)
					{
						//the request plmn equals the available plmn
						if (req_ptr->plmn.mcc == m_SysInfoData.m_nPLMNMcc4 &&
							req_ptr->plmn.mnc == m_SysInfoData.m_nPLMNMnc4 &&
							req_ptr->plmn.mnc_digit_num == 2)
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
						}else
						{
							((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = FALSE;
						}
					}else
					{
						((MN_CAMP_ON_CNF_T *)sig_ptr)->is_success = TRUE;
					}
					
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac4;			  
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.cell_id = m_SysInfoData.m_nPLMNCell4;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.rxlev = m_SysInfoData.m_nSigLev4;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.arfcn = 0;
					((MN_CAMP_ON_CNF_T *)sig_ptr)->cell_info.bsic = 0;
				}
				
				m_SysInfoData.m_nPowerStatus4 = 1;
				CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER4); 
				CString csPower;
				csPower = m_SysInfoData.m_nPowerStatus4 ? "ON" : "OFF";
				pstPower->SetWindowText(csPower);
			}

			((MN_CAMP_ON_CNF_T *)sig_ptr)->dual_sys = req_ptr->dual_sys;
			SendCPMsgToPSDummy(sig_ptr);
		}
		break;
	case MN_NREG_REQ:
		{
			if (MN_DUAL_SYS_1 == ((MN_NREG_REQ_T*)psig)->dual_sys)
			{
				if (((MN_NREG_REQ_T*)psig)->cause = MS_POWER_OFF)
				{
					m_SysInfoData.m_nPowerStatus = 0;
					CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER); 
					CString csPower;
					csPower = m_SysInfoData.m_nPowerStatus ? "ON" : "OFF";
					pstPower->SetWindowText(csPower);
				}
			}
			else if (MN_DUAL_SYS_2 == ((MN_NREG_REQ_T*)psig)->dual_sys)
			{
				if (((MN_NREG_REQ_T*)psig)->cause = MS_POWER_OFF)
				{
					m_SysInfoData.m_nPowerStatus2 = 0;
					CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER2); 
					CString csPower;
					csPower = m_SysInfoData.m_nPowerStatus2 ? "ON" : "OFF";
					pstPower->SetWindowText(csPower);
				}
			}
			else if ((MN_DUAL_SYS_1+2) == ((MN_NREG_REQ_T*)psig)->dual_sys)
			{
				if (((MN_NREG_REQ_T*)psig)->cause = MS_POWER_OFF)
				{
					m_SysInfoData.m_nPowerStatus3 = 0;
					CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER3); 
					CString csPower;
					csPower = m_SysInfoData.m_nPowerStatus3 ? "ON" : "OFF";
					pstPower->SetWindowText(csPower);
				}
			}
			else
			{
				if (((MN_NREG_REQ_T*)psig)->cause = MS_POWER_OFF)
				{
					m_SysInfoData.m_nPowerStatus4 = 0;
					CStatic *pstPower = (CStatic *) GetDlgItem(IDC_STATIC_POWER4); 
					CString csPower;
					csPower = m_SysInfoData.m_nPowerStatus4 ? "ON" : "OFF";
					pstPower->SetWindowText(csPower);
				}
			}
		}
        break;
	case MN_REG_REQ:
		{
//			ASSERT(((MN_REG_REQ_T*)psig)->plmn.mcc == m_SysInfoData.m_nPLMNMcc &&
//				((MN_REG_REQ_T*)psig)->plmn.mnc == m_SysInfoData.m_nPLMNMnc);
//			ASSERT(m_SysInfoData.m_nPLMNSvrState == MN_PLMN_NO_CELL);
			
			xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			InnerSCICreateSignal(&sig_ptr,
				MN_REG_CNF,
				sizeof(MN_REG_CNF_T),
				P_CPANEL);
			MN_SERVICE_STATE_E svrstate;
			
			if (MN_DUAL_SYS_1 == ((MN_REG_REQ_T*)psig)->dual_sys)
			{
				if (m_SysInfoData.m_nPLMNSvrState == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState == 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}
				((MN_REG_CNF_T*)sig_ptr)->state = svrstate;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc = m_SysInfoData.m_nPLMNMnc;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mcc = m_SysInfoData.m_nPLMNMcc;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc_digit_num = 2;
			}
			else if (MN_DUAL_SYS_2 == ((MN_REG_REQ_T*)psig)->dual_sys)
			{
				if (m_SysInfoData.m_nPLMNSvrState2 == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState2 == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState2== 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}
				((MN_REG_CNF_T*)sig_ptr)->state = svrstate;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc = m_SysInfoData.m_nPLMNMnc2;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mcc = m_SysInfoData.m_nPLMNMcc2;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc_digit_num = 2;
			}
			else if ((MN_DUAL_SYS_1+2) == ((MN_REG_REQ_T*)psig)->dual_sys)
			{
				if (m_SysInfoData.m_nPLMNSvrState3 == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState3 == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState3 == 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}
				((MN_REG_CNF_T*)sig_ptr)->state = svrstate;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc = m_SysInfoData.m_nPLMNMnc3;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mcc = m_SysInfoData.m_nPLMNMcc3;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc_digit_num = 2;
			}
			else
			{
				if (m_SysInfoData.m_nPLMNSvrState4 == 0)
				{
					svrstate = MN_PLMN_NO_CELL;					
				}
				else if (m_SysInfoData.m_nPLMNSvrState4 == 1)
				{
					svrstate = MN_PLMN_NORMAL;
				}
				else if (m_SysInfoData.m_nPLMNSvrState4 == 2)
				{
					svrstate = MN_PLMN_LIMITED;
				}
				((MN_REG_CNF_T*)sig_ptr)->state = svrstate;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc = m_SysInfoData.m_nPLMNMnc4;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mcc = m_SysInfoData.m_nPLMNMcc4;
				((MN_REG_CNF_T*)sig_ptr)->plmn.mnc_digit_num = 2;
			}
			((MN_REG_CNF_T*)sig_ptr)->dual_sys = ((MN_REG_REQ_T*)psig)->dual_sys;
			SendCPMsgToPSDummy(sig_ptr);
		}
		break;
 /*   case MN_CELL_INFO_RSP:
        {
            CEdit *pedPLMNMnc=(CEdit *) GetDlgItem(IDC_EDIT_PLMN_MNC);
            m_SysInfoData.m_nPLMNMnc = ((MN_CELL_INFO_RSP_T *) psig)->plmn.mnc; 
            if (0 == m_SysInfoData.m_nPLMNMnc)//CMCC
            {
                pedPLMNMnc->SetWindowText("00");
            }
            else //CUCC
            {
                pedPLMNMnc->SetWindowText("01");
            }
        }
        break;*/
	default:
		break;	
	}

}

void CSysInfoPropertyPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
   
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
	CSliderCtrl * psdSigLev = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV);
	//apple add for siglevel2
	CSliderCtrl * psdSigLev2 = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV2);
	CSliderCtrl * psdSigLev3 = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV3);
	CSliderCtrl * psdSigLev4 = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV4);

	CSliderCtrl * psdBattery = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_BATTERY);
	
	SetDlgItemInt(IDC_STATIC_BATTERY,psdBattery->GetPos());
	SetDlgItemInt(IDC_STATIC_SIGLEV,psdSigLev->GetPos());
	SetDlgItemInt(IDC_STATIC_SIGLEV2,psdSigLev2->GetPos());
	SetDlgItemInt(IDC_STATIC_SIGLEV3,psdSigLev3->GetPos());
	SetDlgItemInt(IDC_STATIC_SIGLEV4,psdSigLev4->GetPos());
    /*@jassmine MS00004749 2003-9-16*/
    if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SIGLEV )
    {
	    SetModified(TRUE);	
    }
	else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SIGLEV2 )
	{
		SetModified2( TRUE );
	}
	else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SIGLEV3 )
	{
		SetModified3( TRUE );
	}
	else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SIGLEV4 )
	{
		SetModified4( TRUE );
	}
    else 
    {
        SetModifiedCharge(TRUE);	
    }
    /*@End MS00004749 2003-9-16*/
}


void CSysInfoPropertyPage::OnChangeEditPlmnMcc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	SetModified(TRUE);
	// TODO: Add your control notification handler code here
	
}

void CSysInfoPropertyPage::OnChangeEditPlmnMnc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	SetModified(TRUE);
	// TODO: Add your control notification handler code here
	
}

/*@jassmine MS00004749 2003-9-15*/
void CSysInfoPropertyPage::OnChangeEditPlmnLac() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	SetModified(TRUE);
	// TODO: Add your control notification handler code here
	
}

void CSysInfoPropertyPage::OnChangeEditPlmnCell() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	SetModified(TRUE);
	// TODO: Add your control notification handler code here
	
}

void CSysInfoPropertyPage::OnSelchangeComboChargeStatus() 
{
	// TODO: Add your control notification handler code here
	SetModifiedCharge(TRUE);

}
/*@End MS00004749 2003-9-15*/

void CSysInfoPropertyPage::OnSelchangeComboPlmnSvrstate() 
{
	// TODO: Add your control notification handler code here
	SetModified(TRUE);

}

void CSysInfoPropertyPage::OnReleasedcaptureSliderBattery(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (pNMHDR->idFrom == IDC_SLIDER_SIGLEV)
	{
		CSliderCtrl * psdSigLev=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV);		
		if ((m_SysInfoData.m_nSigLev ) != psdSigLev->GetPos())
		{
			SetModified(TRUE);
		}	
	}
	else if (pNMHDR->idFrom == IDC_SLIDER_SIGLEV2 )
	{
		CSliderCtrl * psdSigLev2=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV2);		
		if ((m_SysInfoData.m_nSigLev2) != psdSigLev2->GetPos())
		{
			SetModified2(TRUE);
		}	
	}
	else if (pNMHDR->idFrom == IDC_SLIDER_SIGLEV3 )
	{
		CSliderCtrl * psdSigLev3=(CSliderCtrl *) GetDlgItem(IDC_SLIDER_SIGLEV3);		
		if ((m_SysInfoData.m_nSigLev3) != psdSigLev3->GetPos())
		{
			SetModified3(TRUE);
		}	
	}
	else if(pNMHDR->idFrom == IDC_SLIDER_BATTERY)
	{
		CSliderCtrl * psdBattery = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_BATTERY);
		if ((m_SysInfoData.m_nBattery) != psdBattery->GetPos())
		{
			SetModifiedCharge(TRUE);/*@jassmine MS00004749 2003-9-15*/
		}
	}
	*pResult = 0;
}

void CSysInfoPropertyPage::SetModified(BOOL bModified)
{
	m_SysInfoData.m_bModified = bModified;	
	CButton *pbtApply = (CButton *) GetDlgItem(IDC_BUTTON_APPLY);
	CButton *pbtCancel = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL);	
	if (bModified)
	{
		pbtApply->EnableWindow(TRUE);
		pbtCancel->EnableWindow(TRUE);
	}
	else
	{
		pbtApply->EnableWindow(FALSE);
		pbtCancel->EnableWindow(FALSE);
	}
	UpdateWindow();
}

void CSysInfoPropertyPage::SetModified2( BOOL bModified )
{
    m_SysInfoData.m_bModified2 = bModified;	
    CButton *pbtApply = (CButton *) GetDlgItem(IDC_BUTTON_APPLY2);
    CButton *pbtCancel = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL2);	
    if (bModified)
    {
        pbtApply->EnableWindow(TRUE);
        pbtCancel->EnableWindow(TRUE);
    }
    else
    {
        pbtApply->EnableWindow(FALSE);
        pbtCancel->EnableWindow(FALSE);
    }
    UpdateWindow();
}

void CSysInfoPropertyPage::SetModified3( BOOL bModified )
{
    m_SysInfoData.m_bModified3 = bModified;	
    CButton *pbtApply = (CButton *) GetDlgItem(IDC_BUTTON_APPLY3);
    CButton *pbtCancel = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL3);	
    if (bModified)
    {
        pbtApply->EnableWindow(TRUE);
        pbtCancel->EnableWindow(TRUE);
    }
    else
    {
        pbtApply->EnableWindow(FALSE);
        pbtCancel->EnableWindow(FALSE);
    }
    UpdateWindow();
}

void CSysInfoPropertyPage::SetModified4( BOOL bModified )
{
    m_SysInfoData.m_bModified4 = bModified;	
    CButton *pbtApply = (CButton *) GetDlgItem(IDC_BUTTON_APPLY4);
    CButton *pbtCancel = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL4);	
    if (bModified)
    {
        pbtApply->EnableWindow(TRUE);
        pbtCancel->EnableWindow(TRUE);
    }
    else
    {
        pbtApply->EnableWindow(FALSE);
        pbtCancel->EnableWindow(FALSE);
    }
    UpdateWindow();
}

/*@jassmine MS00004749 2003-9-15*/
void CSysInfoPropertyPage::SetModifiedCharge(BOOL bModified)
{
	m_SysInfoData.m_bModifiedCharge = bModified;
	CButton *pbtApply1 = (CButton *) GetDlgItem(IDC_BUTTON_APPLY1);
	CButton *pbtCancel1 = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL1);
    
    if (bModified)
	{
		pbtApply1->EnableWindow(TRUE);
		pbtCancel1->EnableWindow(TRUE);
	}
	else
	{
		pbtApply1->EnableWindow(FALSE);
		pbtCancel1->EnableWindow(FALSE);
	}
    UpdateWindow();
}
/*@End MS00004749 2003-9-15*/

void CSysInfoPropertyPage::OnButtonApply() 
{

	// TODO: Add your control notification handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(1))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
	CConPanSysInfoData old_SysInfoData;
	PLMN_T	old_plmn;
	
	old_SysInfoData = m_SysInfoData;
	old_plmn.mnc = m_SysInfoData.m_nPLMNMnc;
	old_plmn.mcc = m_SysInfoData.m_nPLMNMcc;
	old_plmn.mnc_digit_num = 2;
	UpdatePLMNInfoData(TRUE);
	SetModified(FALSE);

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *) PNULL;
	MN_CELL_INO_CHG_IND_T *info_chg_ptr = PNULL;
	
	InnerSCICreateSignal(&sig_ptr,
 					  MN_CELL_INO_CHG_IND,
					  sizeof(MN_CELL_INO_CHG_IND_T),
					  P_CPANEL);
	ASSERT(sig_ptr != PNULL);

	info_chg_ptr = (MN_CELL_INO_CHG_IND_T *)sig_ptr;
	info_chg_ptr->is_plmn_chg = FALSE;
	info_chg_ptr->is_sevice_state_chg = FALSE;
	info_chg_ptr->is_siglev_chg = FALSE;

	if (old_SysInfoData.m_nPLMNMcc != m_SysInfoData.m_nPLMNMcc ||
		old_SysInfoData.m_nPLMNMnc != m_SysInfoData.m_nPLMNMnc)
	{
		info_chg_ptr->is_plmn_chg = TRUE;
		info_chg_ptr->old_plmn = old_plmn;
	}
	if (old_SysInfoData.m_nPLMNSvrState != m_SysInfoData.m_nPLMNSvrState)
	{
		info_chg_ptr->is_sevice_state_chg = TRUE;

		if (old_SysInfoData.m_nPLMNSvrState == 0)
		{
			info_chg_ptr->old_state = MN_PLMN_NO_CELL;					
		}
		else if (old_SysInfoData.m_nPLMNSvrState == 1)
		{
			info_chg_ptr->old_state = MN_PLMN_NORMAL;
		}
		else if (old_SysInfoData.m_nPLMNSvrState == 2)
		{
			info_chg_ptr->old_state = MN_PLMN_LIMITED;
		}
	}
	if (old_SysInfoData.m_nSigLev != m_SysInfoData.m_nSigLev)
	{
		info_chg_ptr->is_siglev_chg = TRUE;
	}

	MN_SERVICE_STATE_E svrstate;
	if (m_SysInfoData.m_nPLMNSvrState == 0)
	{
		svrstate = MN_PLMN_NO_CELL;					
	}
	else if (m_SysInfoData.m_nPLMNSvrState == 1)
	{
		svrstate = MN_PLMN_NORMAL;
	}
	else if (m_SysInfoData.m_nPLMNSvrState == 2)
	{
		svrstate = MN_PLMN_LIMITED;
	}
	
	info_chg_ptr->plmn.mcc = m_SysInfoData.m_nPLMNMcc;			  
	info_chg_ptr->plmn.mnc = m_SysInfoData.m_nPLMNMnc;	
	info_chg_ptr->plmn.mnc_digit_num = 2;																																																																																							

	info_chg_ptr->state = svrstate;
    /*@jassmine MS00004749 2003-9-15*/
    info_chg_ptr->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac;
    info_chg_ptr->cell_info.cell_id = m_SysInfoData.m_nPLMNCell;
	info_chg_ptr->cell_info.rxlev = m_SysInfoData.m_nSigLev;
	info_chg_ptr->cell_info.arfcn = 0;
	info_chg_ptr->cell_info.bsic = 0;
	info_chg_ptr->dual_sys = MN_DUAL_SYS_1;
	SendCPMsgToPSDummy(sig_ptr);
}

void CSysInfoPropertyPage::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	UpdatePLMNInfoData(FALSE);
	SetModified(FALSE);
}

void CSysInfoPropertyPage::OnButtonApply1() 
{

	// TODO: Add your control notification handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(1))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}

	UpdateChargeInfoData(TRUE);
	SetModifiedCharge(FALSE);	


	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *) PNULL;
	
	InnerSCICreateSignal(&sig_ptr,
 					  CHR_MSG_CAP_IND,
					  sizeof(xSignalHeaderRec) + 2,
					  P_CPANEL);

	bool svrstate = false;	
	
	if (m_SysInfoData.m_nCHARGEStatus == 0)
	{
		svrstate = true;			
	}
	else if (m_SysInfoData.m_nCHARGEStatus == 1)
	{
		svrstate = false;		
	}			  
	((BYTE *)sig_ptr)[ sizeof(xSignalHeaderRec) ] = svrstate;
    ((BYTE *)sig_ptr)[ sizeof(xSignalHeaderRec) + 1 ] = m_SysInfoData.m_nBattery;	
	
	SendCPMsgToPSDummy(sig_ptr); 

}

void CSysInfoPropertyPage::OnButtonCancel1() 
{
	// TODO: Add your control notification handler code here
    UpdateChargeInfoData(FALSE);
	SetModifiedCharge(FALSE);

}

void CSysInfoPropertyPage::OnButtonSimcard() 
{
	// TODO: Add your control notification handler code here
	CButton *pbtSimCard=(CButton *) GetDlgItem(IDC_BUTTON_SIMCARD);
	if (InnerQuerySIM(0))
	{
        InnerRejectSIM(0);
		pbtSimCard->SetWindowText("Insert");

	}
	else
	{
        InnerInsertSIM(0);
		pbtSimCard->SetWindowText("Reject");
	}
}

void CSysInfoPropertyPage::OnButtonSetsimcard() 
{
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(1))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
    InnerShowPadSIM(0);
}

void CSysInfoPropertyPage::OnCheckAutopage() 
{
	// TODO: Add your control notification handler code here
    m_bAutoPage = !m_bAutoPage;
	
}

void CSysInfoPropertyPage::OnButtonApply2() 
{
   	// TODO: Add your control notification handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(2))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
	CConPanSysInfoData old_SysInfoData;
	PLMN_T	old_plmn;
	
	old_SysInfoData = m_SysInfoData;
	old_plmn.mnc = m_SysInfoData.m_nPLMNMnc2;
	old_plmn.mcc = m_SysInfoData.m_nPLMNMcc2;
	old_plmn.mnc_digit_num = 2;
	UpdatePLMNInfoData2(TRUE);
	SetModified2(FALSE);

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *) PNULL;
	MN_CELL_INO_CHG_IND_T *info_chg_ptr = PNULL;
	
	InnerSCICreateSignal(&sig_ptr,
 					  MN_CELL_INO_CHG_IND,
					  sizeof(MN_CELL_INO_CHG_IND_T),
					  P_CPANEL);
	ASSERT(sig_ptr != PNULL);

	info_chg_ptr = (MN_CELL_INO_CHG_IND_T *)sig_ptr;
	info_chg_ptr->is_plmn_chg = FALSE;
	info_chg_ptr->is_sevice_state_chg = FALSE;
	info_chg_ptr->is_siglev_chg = FALSE;

	if (old_SysInfoData.m_nPLMNMcc2 != m_SysInfoData.m_nPLMNMcc2 ||
		old_SysInfoData.m_nPLMNMnc2 != m_SysInfoData.m_nPLMNMnc2)
	{
		info_chg_ptr->is_plmn_chg = TRUE;
		info_chg_ptr->old_plmn = old_plmn;
	}
	if (old_SysInfoData.m_nPLMNSvrState2 != m_SysInfoData.m_nPLMNSvrState2)
	{
		info_chg_ptr->is_sevice_state_chg = TRUE;

		if (old_SysInfoData.m_nPLMNSvrState2 == 0)
		{
			info_chg_ptr->old_state = MN_PLMN_NO_CELL;					
		}
		else if (old_SysInfoData.m_nPLMNSvrState2 == 1)
		{
			info_chg_ptr->old_state = MN_PLMN_NORMAL;
		}
		else if (old_SysInfoData.m_nPLMNSvrState2 == 2)
		{
			info_chg_ptr->old_state = MN_PLMN_LIMITED;
		}
	}
	if (old_SysInfoData.m_nSigLev2 != m_SysInfoData.m_nSigLev2)
	{
		info_chg_ptr->is_siglev_chg = TRUE;
	}

	MN_SERVICE_STATE_E svrstate;
	if (m_SysInfoData.m_nPLMNSvrState2 == 0)
	{
		svrstate = MN_PLMN_NO_CELL;					
	}
	else if (m_SysInfoData.m_nPLMNSvrState2 == 1)
	{
		svrstate = MN_PLMN_NORMAL;
	}
	else if (m_SysInfoData.m_nPLMNSvrState2 == 2)
	{
		svrstate = MN_PLMN_LIMITED;
	}
	
	info_chg_ptr->plmn.mcc = m_SysInfoData.m_nPLMNMcc2;			  
	info_chg_ptr->plmn.mnc = m_SysInfoData.m_nPLMNMnc2;	
	info_chg_ptr->plmn.mnc_digit_num = 2;																																																																																							

	info_chg_ptr->state = svrstate;
    /*@jassmine MS00004749 2003-9-15*/
    info_chg_ptr->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac2;
    info_chg_ptr->cell_info.cell_id = m_SysInfoData.m_nPLMNCell2;
	info_chg_ptr->cell_info.rxlev = m_SysInfoData.m_nSigLev2;
	info_chg_ptr->cell_info.arfcn = 0;
	info_chg_ptr->cell_info.bsic = 0;
	info_chg_ptr->dual_sys = MN_DUAL_SYS_2;
	SendCPMsgToPSDummy(sig_ptr);
}

void CSysInfoPropertyPage::OnButtonCancel2() 
{
    UpdatePLMNInfoData2(FALSE);
    SetModified2(FALSE);
}

void CSysInfoPropertyPage::OnChangeEditPlmnCell2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    SetModified2( TRUE );
	
}

void CSysInfoPropertyPage::OnChangeEditPlmnLac2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified2( TRUE );
}

void CSysInfoPropertyPage::OnChangeEditPlmnMcc2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified2( TRUE );
}

void CSysInfoPropertyPage::OnChangeEditPlmnMnc2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified2( TRUE );
}
void CSysInfoPropertyPage::OnSelchangeComboPlmnSvrstate2() 
{
	// TODO: Add your control notification handler code here
	SetModified2(TRUE);

}

void CSysInfoPropertyPage::OnButtonSetsimcard2() 
{
    //?? the second sim card functions
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(2))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
    InnerShowPadSIM(1);

}

void CSysInfoPropertyPage::OnButtonSimcard2() 
{
    //?? the second sim card functions
	CButton *pbtSimCard=(CButton *) GetDlgItem(IDC_BUTTON_SIMCARD2);
	
	if (InnerQuerySIM(1))
	{
        InnerRejectSIM(1);
		pbtSimCard->SetWindowText("Insert");
	}
	else
	{
        InnerInsertSIM(1);
		pbtSimCard->SetWindowText("Reject");
	}
}


void CSysInfoPropertyPage::OnButtonApply3() 
{
   	// TODO: Add your control notification handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(3))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
	CConPanSysInfoData old_SysInfoData;
	PLMN_T	old_plmn;
	
	old_SysInfoData = m_SysInfoData;
	old_plmn.mnc = m_SysInfoData.m_nPLMNMnc3;
	old_plmn.mcc = m_SysInfoData.m_nPLMNMcc3;
	old_plmn.mnc_digit_num = 2;
	UpdatePLMNInfoData3(TRUE);
	SetModified3(FALSE);

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *) PNULL;
	MN_CELL_INO_CHG_IND_T *info_chg_ptr = PNULL;
	
	InnerSCICreateSignal(&sig_ptr,
 					  MN_CELL_INO_CHG_IND,
					  sizeof(MN_CELL_INO_CHG_IND_T),
					  P_CPANEL);
	ASSERT(sig_ptr != PNULL);

	info_chg_ptr = (MN_CELL_INO_CHG_IND_T *)sig_ptr;
	info_chg_ptr->is_plmn_chg = FALSE;
	info_chg_ptr->is_sevice_state_chg = FALSE;
	info_chg_ptr->is_siglev_chg = FALSE;

	if (old_SysInfoData.m_nPLMNMcc3 != m_SysInfoData.m_nPLMNMcc3 ||
		old_SysInfoData.m_nPLMNMnc3 != m_SysInfoData.m_nPLMNMnc3)
	{
		info_chg_ptr->is_plmn_chg = TRUE;
		info_chg_ptr->old_plmn = old_plmn;
	}
	if (old_SysInfoData.m_nPLMNSvrState3 != m_SysInfoData.m_nPLMNSvrState3)
	{
		info_chg_ptr->is_sevice_state_chg = TRUE;

		if (old_SysInfoData.m_nPLMNSvrState3 == 0)
		{
			info_chg_ptr->old_state = MN_PLMN_NO_CELL;					
		}
		else if (old_SysInfoData.m_nPLMNSvrState3 == 1)
		{
			info_chg_ptr->old_state = MN_PLMN_NORMAL;
		}
		else if (old_SysInfoData.m_nPLMNSvrState3 == 2)
		{
			info_chg_ptr->old_state = MN_PLMN_LIMITED;
		}
	}
	if (old_SysInfoData.m_nSigLev3 != m_SysInfoData.m_nSigLev3)
	{
		info_chg_ptr->is_siglev_chg = TRUE;
	}

	MN_SERVICE_STATE_E svrstate;
	if (m_SysInfoData.m_nPLMNSvrState3 == 0)
	{
		svrstate = MN_PLMN_NO_CELL;					
	}
	else if (m_SysInfoData.m_nPLMNSvrState3 == 1)
	{
		svrstate = MN_PLMN_NORMAL;
	}
	else if (m_SysInfoData.m_nPLMNSvrState3 == 2)
	{
		svrstate = MN_PLMN_LIMITED;
	}
	
	info_chg_ptr->plmn.mcc = m_SysInfoData.m_nPLMNMcc3;			  
	info_chg_ptr->plmn.mnc = m_SysInfoData.m_nPLMNMnc3;	
	info_chg_ptr->plmn.mnc_digit_num = 2;																																																																																							

	info_chg_ptr->state = svrstate;
    /*@jassmine MS00004749 2003-9-15*/
    info_chg_ptr->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac3;
    info_chg_ptr->cell_info.cell_id = m_SysInfoData.m_nPLMNCell3;
	info_chg_ptr->cell_info.rxlev = m_SysInfoData.m_nSigLev3;
	info_chg_ptr->cell_info.arfcn = 0;
	info_chg_ptr->cell_info.bsic = 0;
	info_chg_ptr->dual_sys = MN_DUAL_SYS_2;
	SendCPMsgToPSDummy(sig_ptr);
}

void CSysInfoPropertyPage::OnButtonCancel3() 
{
    UpdatePLMNInfoData3(FALSE);
    SetModified3(FALSE);
}

void CSysInfoPropertyPage::OnChangeEditPlmnCell3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    SetModified3( TRUE );
	
}

void CSysInfoPropertyPage::OnChangeEditPlmnLac3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified3( TRUE );
}

void CSysInfoPropertyPage::OnChangeEditPlmnMcc3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified3( TRUE );
}

void CSysInfoPropertyPage::OnChangeEditPlmnMnc3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified3( TRUE );
}
void CSysInfoPropertyPage::OnSelchangeComboPlmnSvrstate3() 
{
	// TODO: Add your control notification handler code here
	SetModified3(TRUE);

}

void CSysInfoPropertyPage::OnButtonSetsimcard3() 
{
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(3))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
	InnerShowPadSIM(2);

}

void CSysInfoPropertyPage::OnButtonSimcard3() 
{
	CButton *pbtSimCard=(CButton *) GetDlgItem(IDC_BUTTON_SIMCARD3);
	
	if (InnerQuerySIM(2))
	{
        InnerRejectSIM(2);
		pbtSimCard->SetWindowText("Insert");
	}
	else
	{
        InnerInsertSIM(2);
		pbtSimCard->SetWindowText("Reject");
	}
}

void CSysInfoPropertyPage::OnButtonApply4() 
{
   	// TODO: Add your control notification handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(4))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
	CConPanSysInfoData old_SysInfoData;
	PLMN_T	old_plmn;
	
	old_SysInfoData = m_SysInfoData;
	old_plmn.mnc = m_SysInfoData.m_nPLMNMnc4;
	old_plmn.mcc = m_SysInfoData.m_nPLMNMcc4;
	old_plmn.mnc_digit_num = 2;
	UpdatePLMNInfoData4(TRUE);
	SetModified4(FALSE);

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *) PNULL;
	MN_CELL_INO_CHG_IND_T *info_chg_ptr = PNULL;
	
	InnerSCICreateSignal(&sig_ptr,
 					  MN_CELL_INO_CHG_IND,
					  sizeof(MN_CELL_INO_CHG_IND_T),
					  P_CPANEL);
	ASSERT(sig_ptr != PNULL);

	info_chg_ptr = (MN_CELL_INO_CHG_IND_T *)sig_ptr;
	info_chg_ptr->is_plmn_chg = FALSE;
	info_chg_ptr->is_sevice_state_chg = FALSE;
	info_chg_ptr->is_siglev_chg = FALSE;

	if (old_SysInfoData.m_nPLMNMcc4 != m_SysInfoData.m_nPLMNMcc4 ||
		old_SysInfoData.m_nPLMNMnc4 != m_SysInfoData.m_nPLMNMnc4)
	{
		info_chg_ptr->is_plmn_chg = TRUE;
		info_chg_ptr->old_plmn = old_plmn;
	}
	if (old_SysInfoData.m_nPLMNSvrState4 != m_SysInfoData.m_nPLMNSvrState4)
	{
		info_chg_ptr->is_sevice_state_chg = TRUE;

		if (old_SysInfoData.m_nPLMNSvrState4 == 0)
		{
			info_chg_ptr->old_state = MN_PLMN_NO_CELL;					
		}
		else if (old_SysInfoData.m_nPLMNSvrState4 == 1)
		{
			info_chg_ptr->old_state = MN_PLMN_NORMAL;
		}
		else if (old_SysInfoData.m_nPLMNSvrState4 == 2)
		{
			info_chg_ptr->old_state = MN_PLMN_LIMITED;
		}
	}
	if (old_SysInfoData.m_nSigLev4 != m_SysInfoData.m_nSigLev4)
	{
		info_chg_ptr->is_siglev_chg = TRUE;
	}

	MN_SERVICE_STATE_E svrstate;
	if (m_SysInfoData.m_nPLMNSvrState4 == 0)
	{
		svrstate = MN_PLMN_NO_CELL;					
	}
	else if (m_SysInfoData.m_nPLMNSvrState4 == 1)
	{
		svrstate = MN_PLMN_NORMAL;
	}
	else if (m_SysInfoData.m_nPLMNSvrState4 == 2)
	{
		svrstate = MN_PLMN_LIMITED;
	}
	
	info_chg_ptr->plmn.mcc = m_SysInfoData.m_nPLMNMcc4;			  
	info_chg_ptr->plmn.mnc = m_SysInfoData.m_nPLMNMnc4;	
	info_chg_ptr->plmn.mnc_digit_num = 2;																																																																																							

	info_chg_ptr->state = svrstate;
    /*@jassmine MS00004749 2003-9-15*/
    info_chg_ptr->cell_info.lai.lac = m_SysInfoData.m_nPLMNLac4;
    info_chg_ptr->cell_info.cell_id = m_SysInfoData.m_nPLMNCell4;
	info_chg_ptr->cell_info.rxlev = m_SysInfoData.m_nSigLev4;
	info_chg_ptr->cell_info.arfcn = 0;
	info_chg_ptr->cell_info.bsic = 0;
	info_chg_ptr->dual_sys = (MN_DUAL_SYS_E)(MN_DUAL_SYS_1+3);
	SendCPMsgToPSDummy(sig_ptr);
}

void CSysInfoPropertyPage::OnButtonCancel4() 
{
    UpdatePLMNInfoData4(FALSE);
    SetModified4(FALSE);
}

void CSysInfoPropertyPage::OnChangeEditPlmnCell4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    SetModified4( TRUE );
	
}

void CSysInfoPropertyPage::OnChangeEditPlmnLac4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified4( TRUE );
}

void CSysInfoPropertyPage::OnChangeEditPlmnMcc4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified4( TRUE );
}

void CSysInfoPropertyPage::OnChangeEditPlmnMnc4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified4( TRUE );
}
void CSysInfoPropertyPage::OnSelchangeComboPlmnSvrstate4() 
{
	// TODO: Add your control notification handler code here
	SetModified4(TRUE);

}

void CSysInfoPropertyPage::OnButtonSetsimcard4() 
{
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(4))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}
	InnerShowPadSIM(3);

}

void CSysInfoPropertyPage::OnButtonSimcard4() 
{
	CButton *pbtSimCard=(CButton *) GetDlgItem(IDC_BUTTON_SIMCARD4);
	
	if (InnerQuerySIM(3))
	{
        InnerRejectSIM(3);
		pbtSimCard->SetWindowText("Insert");
	}
	else
	{
        InnerInsertSIM(3);
		pbtSimCard->SetWindowText("Reject");
	}
}

