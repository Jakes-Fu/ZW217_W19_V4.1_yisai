#pragma once
#include "afxwin.h"


// CDlgTest �Ի���

class CDlgTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest)

public:
	CDlgTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTest();

// �Ի�������
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	CEdit m_ctlFile;
};
