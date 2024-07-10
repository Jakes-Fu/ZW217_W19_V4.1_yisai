#if !defined(AFX_TPADDRDESCDLG_H__5B8C0F42_6E53_4EBB_9946_7E98D61CBB75__INCLUDED_)
#define AFX_TPADDRDESCDLG_H__5B8C0F42_6E53_4EBB_9946_7E98D61CBB75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPAddrDescDlg.h : header file
//

#include <AfxTempl.h>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CTPAddrDescDlg dialog

class CTPAddrDescDlg : public CDialog
{
// Construction
public:
	CTPAddrDescDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTPAddrDescDlg)
	enum { IDD = IDD_DLG_TP_ADDR_DESC };
	CListCtrl	m_ctlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPAddrDescDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
//    CMap<DWORD,DWORD,int,int> m_AddrMap;
	CMap<CString, LPCTSTR, int, int> m_AddrMap;

//    CMap<DWORD, DWORD, DWORD, DWORD> m_mapColor;
	CMap<CString, LPCTSTR, DWORD, DWORD> m_mapColor;


    /*@ Liu Kai CR4895 2003-9-23 */
    static const char CSZ_HEADER[32];

// Implementation
protected:
	void RefreshAddrMap(void);
    BOOL Load();
    BOOL Save();

    BOOL GetFilePath(char* szPath);

    void RemoveItem();

    void Refresh();

    void AddItem(int nIndex,CString& strAddr, CString& strData,CString& strDesc,
        CString& strFilter, DWORD dwColor);

    void RemoveItem(int nIndex);

public:
    void LookupDesc(DWORD dwAddr,DWORD dwData, char* szDesc);

    DWORD LookupColor( DWORD dwAddr, DWORD dwData );

    /*@ Liu Kai CR4895 2003-9-23 */
    BOOL FindFilter(DWORD,DWORD,char*);

	// Generated message map functions
	//{{AFX_MSG(CTPAddrDescDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonAdd();
	afx_msg void OnDblclkListDesc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDel();
	afx_msg void OnClose();
	virtual void OnOK();
	//}}AFX_MSG
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPADDRDESCDLG_H__5B8C0F42_6E53_4EBB_9946_7E98D61CBB75__INCLUDED_)
