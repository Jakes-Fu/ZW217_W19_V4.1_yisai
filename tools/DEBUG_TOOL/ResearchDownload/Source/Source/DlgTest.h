#pragma once
#include "afxwin.h"


// CDlgTest 对话框

class CDlgTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest)

public:
	CDlgTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTest();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	CEdit m_ctlFile;
};
