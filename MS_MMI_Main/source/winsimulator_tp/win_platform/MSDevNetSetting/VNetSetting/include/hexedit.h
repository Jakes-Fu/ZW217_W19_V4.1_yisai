#if !defined(AFX_NUMEDIT_H__4F03CBF0_8B78_495F_9CD4_13241E314C3A__INCLUDED_)
#define AFX_NUMEDIT_H__4F03CBF0_8B78_495F_9CD4_13241E314C3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEdit.h : header file
//

//******************************************************************//
//	�ļ����ͣ�	Header File
//	���ܣ�		ʮ��������ֵ����༭����
//	���ߣ�		Wang Yifeng
//	������ڣ�	2005_08_08
//	��ע��		��MFC��CEdit��������
//******************************************************************//

/////////////////////////////////////////////////////////////////////////////
// CNumEdit window

class CHexEdit : public CEdit
{
// Construction
public:
	CHexEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHexEdit();

	// Generated message map functions
protected:
    BOOL    ishexc( UINT nChar );

protected:
	//{{AFX_MSG(CHexEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMEDIT_H__4F03CBF0_8B78_495F_9CD4_13241E314C3A__INCLUDED_)
