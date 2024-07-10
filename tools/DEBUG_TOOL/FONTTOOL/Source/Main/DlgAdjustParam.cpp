// DlgAdjustParam.cpp : implementation file
//

#include "stdafx.h"
#include "fonttool.h"
#include "DlgAdjustParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustParam dialog


CDlgAdjustParam::CDlgAdjustParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdjustParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdjustParam)
	m_nBXDelta = 0;
	m_nBYDelta = 0;
	m_nBXYRange = 0;
	m_nFBBX = 0;
	//}}AFX_DATA_INIT
}


void CDlgAdjustParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdjustParam)
	DDX_Text(pDX, IDC_EDT_BX_DELTA, m_nBXDelta);
	DDX_Text(pDX, IDC_EDT_BY_DELTA, m_nBYDelta);
	DDX_Radio(pDX, IDC_RDO_BXY_GLYPH, m_nBXYRange);
	DDX_Text(pDX, IDC_EDT_MODIFY_FBBX, m_nFBBX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAdjustParam, CDialog)
	//{{AFX_MSG_MAP(CDlgAdjustParam)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustParam message handlers
