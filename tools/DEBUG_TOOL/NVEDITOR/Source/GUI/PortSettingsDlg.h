#if !defined(AFX_LFPSETTINGSDLG_H__CA607DEF_2AF8_4106_8613_6EEEFA9688ED__INCLUDED_)
#define AFX_LFPSETTINGSDLG_H__CA607DEF_2AF8_4106_8613_6EEEFA9688ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortSettingsDlg.h : header file
//
#include <afxtempl.h>
#include "NumericEdit.h"
#define MAX_PRODUCT_NAME 32

typedef struct
{
        DWORD dwAddr;
        TCHAR szAddr[MAX_PRODUCT_NAME];
}NvBaseAddr;

/////////////////////////////////////////////////////////////////////////////
// CPortSettingsDlg dialog

class CPortSettingsDlg : public CDialog
{
// Construction
public:
	CPortSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLFPSettingsDlg)
	enum { IDD = IDD_PORT_SETTINGS };
	CComboBox	m_ctlPort;
	CComboBox	m_ctlBaud;
	CString     m_strFdl;
    CComboBox	m_cmbProduct;
    CEdit       m_edtFdl2;
    CButton     m_btnFdl2;
    CString     m_strFdl2;
	CNumericEdit m_edtFDLBase;
	CNumericEdit m_edtFDL2Base;
	//}}AFX_DATA
	int m_nPort;
    int m_nBaudrate;
	BOOL m_bCanNoFdl;
	NvBaseAddr m_dwNvBaseAddr;
    CString m_strIniFilePath;
    CString m_strProduct;
    BOOL    m_bNewFDLFile;
    /*CR212808 xmzhang add begin*/
    BOOL      m_bNvBaseStr;
    /*CR212808 xmzhang add end*/

	DWORD   m_dwFDLBase;
	DWORD   m_dwFDL2Base;
    
private:

    struct Product_Type
    {
        TCHAR szProductType[MAX_PRODUCT_NAME];
        long lNVBase;
        TCHAR szBase[MAX_PRODUCT_NAME];
        BOOL bReadOnly;
    };

    CEdit m_editNvBase;    
    CArray<Product_Type,Product_Type> m_ProductList;
    
    int m_nProduct;

private:    
    int EnumKeys(TCHAR* pSection,CStringArray* pKeys);
        
public:
    


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLFPSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLFPSettingsDlg)
	virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeProductType();    
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnFdlBrowse();
    afx_msg void OnFdl2Browse();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LFPSETTINGSDLG_H__CA607DEF_2AF8_4106_8613_6EEEFA9688ED__INCLUDED_)
