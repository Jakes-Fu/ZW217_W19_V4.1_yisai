// ToolTipDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "ToolTipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipDlg dialog


CToolTipDlg::CToolTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolTipDlg)
	m_strToolTip = _T("");
	//}}AFX_DATA_INIT
}


void CToolTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolTipDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDT_TOOLTIP, m_strToolTip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolTipDlg, CDialog)
	//{{AFX_MSG_MAP(CToolTipDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipDlg message handlers

BOOL CToolTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    m_strToolTip = _T("��1��\"�ҵ����\"��֧��*.jpg��ʽ������Ҫ����С��320*240��ͼƬ;\r\n\r\n")
                   _T("��2��\"�ҵ�ͼƬ\"��֧��*.jpg��*.gif��*.wbmp��*.bmp��ʽ,��С������20KB������Ҫ����С��320*240��ͼƬ;\r\n\r\n")
                   _T("��3��\"�ҵ�ͼƬ\"Ŀ¼�µ�BMP�ļ����ļ���ѹ����Ե������ֻ��쿴ͼƬ�ٶȽ�����\r\n\r\n")
                   _T("��4�����ֻ���֧�ֽ�gif��ʽͼƬ�趨Ϊ����ͼƬ�Ĺ��ܡ�\r\n\r\n")
                   _T("��5������������������ͼƬ�п�����ɲ���Ԥ֪������.");

    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
