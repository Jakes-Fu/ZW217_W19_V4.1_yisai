#if !defined(AFX_MRUCOMBOBOX_H__AF181EB1_8DC2_4163_A8BA_D74AF1D446F9__INCLUDED_)
#define AFX_MRUCOMBOBOX_H__AF181EB1_8DC2_4163_A8BA_D74AF1D446F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MruComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMruComboBox window

class CMruComboBox : public CComboBox
{
// Construction
public:
	CMruComboBox();

// Attributes
protected:
    // Max mru list item count
    unsigned int m_uMaxMruCount;

    // The registry section name 
    CString m_strMruSection;

    // The registry entry format string
    CString m_strMruEntryFormat;

// Operations
public:
    // Add current string to mru list
    BOOL Add2MruList();

    // Add a string to mru list
    BOOL Add2MruList(CString& strText);

    // Save MRU list
    BOOL SaveMruList();

    // Load MRU list
    BOOL LoadMruList();

    // Set maximum mru list item count
    void SetMaxMruCount(unsigned int uMax)
    {
        m_uMaxMruCount = uMax;
    }

    // Set registry section name
    void SetMruSection(CString& strSection)
    {
        m_strMruSection = strSection;
    }

    // Set registry entry format string
    void SetMruEntryFormat(CString& strFormat)
    {
        m_strMruEntryFormat = strFormat;
    }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMruComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMruComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMruComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MRUCOMBOBOX_H__AF181EB1_8DC2_4163_A8BA_D74AF1D446F9__INCLUDED_)
