#if !defined(AFX_AUDIOFILEOPENDLG_H__F281C5CF_C6D3_4195_803F_0A616EB5B952__INCLUDED_)
#define AFX_AUDIOFILEOPENDLG_H__F281C5CF_C6D3_4195_803F_0A616EB5B952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AudioFileOpenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAudioFileOpenDlg dialog

class CAudioFileOpenDlg : public CFileDialog
{
// Construction
public:
    CAudioFileOpenDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
        LPCTSTR lpszDefExt = NULL,
        LPCTSTR lpszFileName = NULL,
        DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        LPCTSTR lpszFilter = NULL,
        CWnd* pParentWnd = NULL);
    
// Dialog Data
	//{{AFX_DATA(CAudioFileOpenDlg)
	enum { IDD = IDD_DIALOG_AUDIO_FILE_OPEN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioFileOpenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    virtual void OnFileNameChange();
    
	// Generated message map functions
	//{{AFX_MSG(CAudioFileOpenDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void ChangeFont();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOFILEOPENDLG_H__F281C5CF_C6D3_4195_803F_0A616EB5B952__INCLUDED_)
