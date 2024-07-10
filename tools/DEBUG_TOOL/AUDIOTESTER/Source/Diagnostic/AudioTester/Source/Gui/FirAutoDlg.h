#if !defined(AFX_FIRAUTODLG_H__54A34E19_17D2_4DFF_9BB1_2515A1C3F99E__INCLUDED_)
#define AFX_FIRAUTODLG_H__54A34E19_17D2_4DFF_9BB1_2515A1C3F99E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FirAutoDlg.h : header file
//

#include "FrequencyGraphyStatic.h"

typedef enum
{
	NO_LOAD_ORG,
	LOAD_ORG,
	MODIFY_ORG
};

#define ROW_COUNT 26   
/////////////////////////////////////////////////////////////////////////////
// CFirAutoDlg dialog

class CFirAutoDlg : public CDialog
{
// Construction
public:
	CFirAutoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFirAutoDlg)
	enum { IDD = IDD_DIALOG_FIR_AUTO_ADJUST };
	CFrequencyGraphyStaticEx	m_Graphy;
	CFrequencyGraphyStaticEx2	m_Drag;
	BOOL	m_bShowOri;
	BOOL	m_bShowTar;
	BOOL	m_bShowReal;
	int		m_nGain;
	BOOL	m_bShowModified;
	BOOL	m_bShowSpec;
	double	m_dOffset;
	BOOL	m_bShowFinal;
	CListCtrl	m_listFir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirAutoDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    int m_nSampleRate;
	BOOL m_bAuto;
	int m_nLoadOriginalFlag;
	BOOL m_bVisibleFirButton;
	BOOL m_bDefaultFir;

public:
#define MAX_EQ_NUMBER 400
    int m_arrFrequency[MAX_EQ_NUMBER];
    double m_arrOriEq[MAX_EQ_NUMBER];
    double m_arrTarEq[MAX_EQ_NUMBER];
    int m_arrEq[MAX_EQ_NUMBER];
    int m_nEqNum;

    double m_arrHighEq[MAX_EQ_NUMBER];
    int m_arrHighFreq[MAX_EQ_NUMBER];
    int m_nHighEqNum;

    double m_arrLowEq[MAX_EQ_NUMBER];
    int m_arrLowFreq[MAX_EQ_NUMBER];
    int m_nLowEqNum;
    
    static CString m_strTarEqFile;
    SpecialRectOfCoefficient* m_pArea;
	SpecialRectOfCoefficient* m_pTempArea;

public:
    BOOL m_bOriReady,m_bTarReady,m_bReady,m_bSpecReady;

protected:
    BOOL CheckEqData( int nEqNum,int* pFrequency,BOOL bTarget );
    void UpdateCtrl();
    
    // Load default spec and target file if they exist
    void LoadDefault();

	//teana hu 2011.03.02
	void GetConfig();
	//BOOL LoadOriFile(CString strFile,int* pFrequency,double* pEq,int* pEqNum );
	void SetListCtrl();

public:
    void GetOriEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems );
    void GetTarEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems );
    void GetRealEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems );
    void GetModifiedEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems );
    void GetHighEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems );
    void GetLowEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems );
    void GetFinalEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems );
	void ResetTarEq(int nIndex, double dTarEq);
	void GetOriEqXValue(double dX[65]);
	
	//teana hu 2011.02.17
	void ModifySpecFreqValue(int nDiff);
	int	GetHighEQYValue(int x);
	void SetFirListHighLight(int nItem);
	//
	void SetListData();

private:
	static int m_vPowerFrequence[ROW_COUNT];
    

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFirAutoDlg)
	afx_msg void OnButtonFirSpec();
	afx_msg void OnButtonFirOriginal();
	afx_msg void OnButtonFirTarget();
	afx_msg void OnButtonFirAdjust();
	afx_msg void OnCheckOriginal();
	afx_msg void OnCheckResult();
	afx_msg void OnCheckTarget();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckModified();
	afx_msg void OnCheckSpec();
	virtual void OnOK();
	afx_msg void OnCheckFinal();
	afx_msg void OnButtonOffsetApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDefaultFir();
	afx_msg void OnButtonRefreshFir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRAUTODLG_H__54A34E19_17D2_4DFF_9BB1_2515A1C3F99E__INCLUDED_)
