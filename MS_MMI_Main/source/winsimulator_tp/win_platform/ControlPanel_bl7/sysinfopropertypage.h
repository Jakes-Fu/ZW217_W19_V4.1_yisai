#if !defined(AFX_SYSINFOPROPERTYPAGE_H__A41A3663_2065_48F6_9A49_1BA384DE5BE7__INCLUDED_)
#define AFX_SYSINFOPROPERTYPAGE_H__A41A3663_2065_48F6_9A49_1BA384DE5BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysInfoPropertyPage.h : header file
//
#include "stdafx.h"
#include "resource.h"


class CConPanSysInfoData
{
public:
	int m_nSigLev;
	int m_nBattery;
	int m_nPLMNMcc;
	int m_nPLMNMnc;
    int m_nPLMNLac;
    int m_nPLMNCell;
	int m_nPLMNSvrState;
    int m_nCHARGEStatus;/*@jassmine MS00004749 2003-9-15*/
	int m_nPowerStatus;	
	int m_bModified; 
    int m_bModifiedCharge;/*@jassmine MS00004749 2003-9-15*/
    
    // This members for the second sim card
    int m_nSigLev2;
    int m_nBattery2;
    int m_nPLMNMcc2;
    int m_nPLMNMnc2;
    int m_nPLMNLac2;
    int m_nPLMNCell2;
    int m_nPLMNSvrState2;
    int m_nPowerStatus2;	
    
    int m_bModified2;

	//apple add for third sim card
	int m_nSigLev3;
    int m_nBattery3;
    int m_nPLMNMcc3;
    int m_nPLMNMnc3;
    int m_nPLMNLac3;
    int m_nPLMNCell3;
    int m_nPLMNSvrState3;
    int m_nPowerStatus3;	
    
    int m_bModified3;

	//honliang add for fourth sim card
	int m_nSigLev4;
    int m_nBattery4;
    int m_nPLMNMcc4;
    int m_nPLMNMnc4;
    int m_nPLMNLac4;
    int m_nPLMNCell4;
    int m_nPLMNSvrState4;
    int m_nPowerStatus4;	
    
    int m_bModified4;
	


	CConPanSysInfoData()
	{
		m_bModified = FALSE;
        m_bModifiedCharge = FALSE;
        m_bModified2 = FALSE;
		m_bModified3 = FALSE;
		m_bModified4 = FALSE;
	}
};
/////////////////////////////////////////////////////////////////////////////
// CSysInfoPropertyPage dialog

class CSysInfoPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSysInfoPropertyPage)

// Construction
public:
	CSysInfoPropertyPage();
	~CSysInfoPropertyPage();

public:
	void UpdatePLMNInfoData(BOOL bUpdate);
    void UpdatePLMNInfoData2(BOOL bUpdate);
	//apple
	void UpdatePLMNInfoData3(BOOL bUpdate);
	//@hongliang 2010-11-1
	void UpdatePLMNInfoData4(BOOL bUpdate);

    void UpdateChargeInfoData(BOOL bUpdate);
	void DealCPMsg(void *pmsg);
	CConPanSysInfoData m_SysInfoData;

    BOOL m_bAutoPage;
public:
	void SetModified(BOOL bModified);
    void SetModified2( BOOL bModified );
	//apple
	void SetModified3( BOOL bModified );
	//@hongliang 2010-11-1
	void SetModified4( BOOL bModified );

    /*@jassmine MS00004749 2003-9-15*/
    void SetModifiedCharge(BOOL bModified);
// Dialog Data
	//{{AFX_DATA(CSysInfoPropertyPage)
	enum { IDD = IDD_DIALOG_SYSINFO };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSysInfoPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSysInfoPropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditPlmnMcc();
	afx_msg void OnChangeEditPlmnMnc();

    /*@jassmine MS00004749 2003-9-15*/
    afx_msg void OnChangeEditPlmnLac();
    afx_msg void OnChangeEditPlmnCell();
    afx_msg void OnSelchangeComboChargeStatus();

	afx_msg void OnSelchangeComboPlmnSvrstate();
	afx_msg void OnReleasedcaptureSliderBattery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonApply();
	afx_msg void OnButtonCancel();

    /*@jassmine MS00004749 2003-9-15*/
    afx_msg void OnButtonApply1();
	afx_msg void OnButtonCancel1();

	afx_msg void OnButtonSimcard();
	afx_msg void OnButtonSetsimcard();
	afx_msg void OnCheckAutopage();
    afx_msg void OnButtonApply2();
    afx_msg void OnButtonCancel2();
    afx_msg void OnChangeEditPlmnCell2();
    afx_msg void OnChangeEditPlmnLac2();
    afx_msg void OnChangeEditPlmnMcc2();
    afx_msg void OnChangeEditPlmnMnc2();
    afx_msg void OnButtonSetsimcard2();
    afx_msg void OnButtonSimcard2();
	afx_msg void OnSelchangeComboPlmnSvrstate2(); 
	//apple add for third sim card
	afx_msg void OnButtonApply3();
    afx_msg void OnButtonCancel3();
    afx_msg void OnChangeEditPlmnCell3();
    afx_msg void OnChangeEditPlmnLac3();
    afx_msg void OnChangeEditPlmnMcc3();
    afx_msg void OnChangeEditPlmnMnc3();
    afx_msg void OnButtonSetsimcard3();
    afx_msg void OnButtonSimcard3();
	afx_msg void OnSelchangeComboPlmnSvrstate3();

	//@hongliang 2010-11-1 for fourth sim card
	afx_msg void OnButtonApply4();
    afx_msg void OnButtonCancel4();
    afx_msg void OnChangeEditPlmnCell4();
    afx_msg void OnChangeEditPlmnLac4();
    afx_msg void OnChangeEditPlmnMcc4();
    afx_msg void OnChangeEditPlmnMnc4();
    afx_msg void OnButtonSetsimcard4();
    afx_msg void OnButtonSimcard4();
	afx_msg void OnSelchangeComboPlmnSvrstate4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSINFOPROPERTYPAGE_H__A41A3663_2065_48F6_9A49_1BA384DE5BE7__INCLUDED_)
