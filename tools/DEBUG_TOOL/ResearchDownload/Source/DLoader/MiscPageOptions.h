#pragma once
#include "afxwin.h"


// CMiscPageOptions dialog

class CMiscPageOptions : public CPropertyPage
{
	DECLARE_DYNAMIC(CMiscPageOptions)

public:
	CMiscPageOptions();
	virtual ~CMiscPageOptions();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_MISC_OPTIONS };

	BOOL LoadSettings(LPCTSTR pFileName,CString &strErrorMsg);
	BOOL SaveSettings(LPCTSTR pFileName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	
public:
	CComboBox m_comboSelinux;
	CString m_strTmpSelinux;
	CString m_strSelinux;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
DECLARE_MESSAGE_MAP()
private:
	CString m_strIniFile;
public:
	afx_msg void OnCbnSelchangeComboSelinux();
};
