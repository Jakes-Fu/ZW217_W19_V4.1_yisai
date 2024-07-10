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

    m_strToolTip = _T("◇1、\"我的相册\"仅支持*.jpg格式且像素要求上小于320*240的图片;\r\n\r\n")
                   _T("◇2、\"我的图片\"仅支持*.jpg、*.gif、*.wbmp和*.bmp格式,大小不超过20KB，像素要求上小于320*240的图片;\r\n\r\n")
                   _T("◇3、\"我的图片\"目录下的BMP文件因文件解压缩的缘故造成手机察看图片速度较慢。\r\n\r\n")
                   _T("◇4、本手机不支持将gif格式图片设定为桌面图片的功能。\r\n\r\n")
                   _T("◇5、不符合以上条件的图片有可能造成不可预知的问题.");

    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
