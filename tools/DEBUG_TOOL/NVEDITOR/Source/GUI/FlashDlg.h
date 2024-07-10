#if !defined(AFX_FLASHDLG_H__1416C23E_7052_477B_98AF_3F265A7FF2C3__INCLUDED_)
#define AFX_FLASHDLG_H__1416C23E_7052_477B_98AF_3F265A7FF2C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlashDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlashDlg dialog

class CFlashDlg : public CDialog
{
// Construction
public:
	CFlashDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashDlg)
	enum { IDD = IDD_DIALOG_FLASH };
//	int		m_nBaudRate;
//	int		m_nPort;
	CString	m_strAddress;
	CString	m_strSize;
//	CString	m_strFileName;
	//}}AFX_DATA
//    UINT m_uBaudRate;
//    UINT m_uPort;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
/*
    UINT GetBaudRate()
    {
        return m_uBaudRate;
    }
    UINT GetPort()
    {
        return m_uPort;
    }
*/

    DWORD GetAddress()
    {
        DWORD dwAddr;
        if(m_strAddress.Left(2).CompareNoCase(_T("0x")))
        {
            // Decimal
            _stscanf(m_strAddress,_T("%d"),&dwAddr);
        }
        else
        {
            // Heximal
            _stscanf(m_strAddress,_T("%x"),&dwAddr);
        }
        return dwAddr;
    }

    /*CR212808 xmzhang add begin*/
    CString GetAddressEx()
    {
        return m_strAddress;
    }
    /*CR212808 xmzhang add end*/

    DWORD GetSize()
    {
        DWORD dwSize;
        if(m_strSize.Left(2).CompareNoCase(_T("0x")))
        {
            // Decimal
            _stscanf(m_strSize,_T("%d"),&dwSize);
        }
        else
        {
            // Heximal
            _stscanf(m_strSize,_T("%x"),&dwSize);
        }
        return dwSize;
    }
/*
    CString& GetFDLFile()
    {
        return m_strFileName;
    }
*/

protected:
	// Generated message map functions
	//{{AFX_MSG(CFlashDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
//	afx_msg void OnButtonFlashBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHDLG_H__1416C23E_7052_477B_98AF_3F265A7FF2C3__INCLUDED_)
