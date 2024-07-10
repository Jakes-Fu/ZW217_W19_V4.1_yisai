#if !defined(AFX_VOLUMEDLG_H__9958A510_4222_4597_B9AE_0E0A30740024__INCLUDED_)
#define AFX_VOLUMEDLG_H__9958A510_4222_4597_B9AE_0E0A30740024__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeDlg.h : header file
//

#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg dialog
class SpecialRectOfVolume;

class CVolumeDlg : public CDialog
{
// Construction
public:
	CVolumeDlg(CWnd* pParent = NULL);   // standard constructor
	SpecialRectOfVolume* m_pArea;

	//teana hu 2012.06.11
	BOOL m_bSmartPhone;
	//

// Dialog Data
	//{{AFX_DATA(CVolumeDlg)
	enum { IDD = IDD_DIALOG_VOLUME };
	CListCtrlEx	m_lstVolume;
	int m_nStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitListCtrl();

	int CaclStep(int nStep);
	int ReCaclStep(int nStep);

	//teana hu 2012.06.12
	int CalcPA( double& pa, LONG volume );
	//

public:
	//teana hu 2012.07.03
	void SetDspCodecFlag(BOOL bDspCodecInfo){m_bDspCodecInfo = bDspCodecInfo;}
	//

	// Generated message map functions
	//{{AFX_MSG(CVolumeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonRefreshVolumeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	typedef CListCtrlEx::COLUMN_DATA LIST_COLUME;
    typedef std::vector<LIST_COLUME>ROW_DATA;   
    typedef std::vector<ROW_DATA> LINE_DATA;
    LINE_DATA m_vAllLinesData;
	
	//teana hu 2012.02.21
	BOOL m_bDspCodecInfo;
	//

	static const short pa_value_handsfree_array[16];
	static const float pa_dB_handsfree_array[16];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEDLG_H__9958A510_4222_4597_B9AE_0E0A30740024__INCLUDED_)
