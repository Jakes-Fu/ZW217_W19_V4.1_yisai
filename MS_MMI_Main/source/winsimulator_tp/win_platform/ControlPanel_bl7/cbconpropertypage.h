#if !defined(AFX_CBPROPERTYPAGE_H__CB3DC030_ACD0_461F_87B2_D92DF79DEDF2__INCLUDED_)
#define AFX_CBPROPERTYPAGE_H__CB3DC030_ACD0_461F_87B2_D92DF79DEDF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CBPropertyPage.h : header file
//
#include "resource.h"
#include "afxtempl.h"
#include "mn_type.h"

/////////////////////////////////////////////////////////////////////////////
// CCBConPropertyPage dialog

class CCBConPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCBConPropertyPage)

// Construction
public:
	CCBConPropertyPage();
	~CCBConPropertyPage();

// Dialog Data
	//{{AFX_DATA(CCBConPropertyPage)
	enum { IDD = IDD_DIALOG_CBCON };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCBConPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	public:
		int  sys_id;
protected:
	// Generated message map functions
	//{{AFX_MSG(CCBConPropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnRadioMsgtype1();
	afx_msg void OnRadioMsgtype2();
	afx_msg void OnRadioMsgtype3();
	afx_msg void OnRadioMsgtype4();
	afx_msg void OnCheckDisall();
	afx_msg void OnCheckEnathis();
	afx_msg void OnChangeEditContent();
	afx_msg void OnMaxtextEditContent();
	afx_msg void OnChangeEditPeriod();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    
	
	typedef struct
    {
        int             nMessageID;
        int             nMessageCode;
        MN_SMSCB_DCS_T  dcs;
        int             nPeriod;
        int             nMinuteLeft;
        BOOL            bEnable;
        CString         csMessageType;
        
        // @Liu Jun 2004-02-26 #CR7514
        // [
        int             nMessageLen;
        MN_CB_MSG_ARR_T arrMessage;
        // ]
    } CB_MESSAGE_ELEMENT_T;

    int m_nCurMessageTypeIndex;
    CArray<CB_MESSAGE_ELEMENT_T, CB_MESSAGE_ELEMENT_T> m_aCBMsg;
	void ShowMessageType(int nIndex);
	void SendCBMsgToPS(CB_MESSAGE_ELEMENT_T &cmeElement);
	void LogMessageSent(CB_MESSAGE_ELEMENT_T &cmeElement);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBPROPERTYPAGE_H__CB3DC030_ACD0_461F_87B2_D92DF79DEDF2__INCLUDED_)
