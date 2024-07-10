// DlgAdvSetting.cpp : implementation file
//

#include "stdafx.h"
#include "fonttool.h"
#include "DlgAdvSetting.h"
#include "EdtBtnCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAdvSetting dialog


CDlgAdvSetting::CDlgAdvSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdvSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdvSetting)
	m_bGlobalOption = FALSE;
	m_nFontType = -1;
	//}}AFX_DATA_INIT
	m_agFontType.RemoveAll();
	m_agFontType.Add(_T("MONO"));
	m_agFontType.Add(_T("GRAY"));

	m_pPtSizeProp	= NULL;
	m_pExPtSizeProp	= NULL;
	m_nPtSizeNum	= 0;
	m_nExPtSizeNum	= 0;
}


void CDlgAdvSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdvSetting)
	DDX_Control(pDX, IDC_AS_LST_PTSIZE_EX, m_lstExPtSize);
	DDX_Control(pDX, IDC_AS_LST_PTSIZE, m_lstPtSize);
	DDX_Check(pDX, IDC_AS_CHK_G_FT, m_bGlobalOption);
	DDX_Radio(pDX, IDC_AS_RDO_MONO, m_nFontType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAdvSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgAdvSetting)
	ON_BN_CLICKED(IDC_AS_RDO_MONO, OnRdoMono)
	ON_BN_CLICKED(IDC_AS_RDO_GRAY, OnRdoGray)
	ON_BN_CLICKED(IDC_AS_CHK_G_FT, OnChkGFt)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_AS_LST_PTSIZE, OnEndlabeleditAsLstPtsize)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_AS_LST_PTSIZE_EX, OnEndlabeleditAsLstPtsizeEx)
	ON_NOTIFY(NM_DBLCLK, IDC_AS_LST_PTSIZE, OnDblclkAsLstPtsize)
	ON_NOTIFY(NM_DBLCLK, IDC_AS_LST_PTSIZE_EX, OnDblclkAsLstPtsizeEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAdvSetting message handlers

BOOL CDlgAdvSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	InitPtSizeList(&m_lstPtSize,m_pPtSizeProp,m_nPtSizeNum);
	InitPtSizeList(&m_lstExPtSize,m_pExPtSizeProp,m_nExPtSizeNum);

	m_bGlobalOption = FALSE;
	GetDlgItem(IDC_AS_RDO_GRAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_AS_RDO_MONO)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAdvSetting::SetPtSizeProp(FONT_PROPERTY_PTR pProp,int nNum)
{
	m_pPtSizeProp = pProp;
	m_nPtSizeNum  = nNum;
}
void CDlgAdvSetting::SetExPtSizeProp(FONT_PROPERTY_PTR pProp,int nNum)
{
	m_pExPtSizeProp = pProp;
	m_nExPtSizeNum  = nNum;
}

void CDlgAdvSetting::InitPtSizeList(CListCtrl * pCtrl,FONT_PROPERTY_PTR pPtSizeProp,int nNum)
{
	DWORD dwExtStyle = pCtrl->GetExtendedStyle();
	dwExtStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	pCtrl->SetExtendedStyle(dwExtStyle);
	
	pCtrl->InsertColumn(0,_T("Size"),LVCFMT_LEFT,62);
	pCtrl->InsertColumn(1,_T("Type"),LVCFMT_LEFT,62);
	pCtrl->InsertColumn(2,_T("Shrink"),LVCFMT_LEFT,62);

	for(int i = 0;i< nNum;i++)
	{
		CString strTmp;
		strTmp.Format(_T("%d"),pPtSizeProp[i].nFontSize);
		pCtrl->InsertItem(i,strTmp);

		//pCtrl->SetComboBox(i,1,TRUE,&m_agFontType,80,pPtSizeProp[i].nFontType);
		strTmp = m_agFontType[pPtSizeProp[i].nFontType];
		pCtrl->SetItemText(i,1,strTmp);
		strTmp.Format(_T("%d"),pPtSizeProp[i].nShrinkSize);
		pCtrl->SetItemText(i,2,strTmp);
		//pCtrl->SetEdit(i,2,XET_DECIMAL_NUMBER);
	}
}

void CDlgAdvSetting::SetFontType(int nType,CListCtrl * pCtrl)
{
	int nCount = pCtrl->GetItemCount();
	for(int i = 0;i< nCount;i++)
	{
		//pCtrl->SetComboBox(i,1,TRUE,&m_agFontType,80,nType);
		pCtrl->SetItemText(i,1,m_agFontType[nType]);
	}
}

void CDlgAdvSetting::OnRdoMono() 
{
	// TODO: Add your control notification handler code here
	SetFontType(0,&m_lstPtSize);
	SetFontType(0,&m_lstExPtSize);
	m_lstExPtSize.EnableWindow(TRUE);
	m_lstPtSize.EnableWindow(TRUE);
	
}

void CDlgAdvSetting::OnRdoGray() 
{
	// TODO: Add your control notification handler code here
	SetFontType(1,&m_lstPtSize);
	SetFontType(1,&m_lstExPtSize);
	m_lstExPtSize.EnableWindow(TRUE);
	m_lstPtSize.EnableWindow(TRUE);
}

void CDlgAdvSetting::OnChkGFt() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_AS_RDO_GRAY)->EnableWindow(m_bGlobalOption);
	GetDlgItem(IDC_AS_RDO_MONO)->EnableWindow(m_bGlobalOption);
}

void CDlgAdvSetting::OnOK() 
{
	// TODO: Add extra validation here
	int i=0;
	BOOL bError = FALSE;
	CString strError = _T("");
	for(i=0;i<m_nPtSizeNum;i++)
	{
		CString strType = m_lstPtSize.GetItemText(i,1);
		if(strType.CompareNoCase(_T("MONO"))==0)
		{
			m_pPtSizeProp[i].nFontType = FONT_TYPE_MONO;
		}
		else
		{
			m_pPtSizeProp[i].nFontType = FONT_TYPE_GRAY;
		}

		CString strShrinkSize = m_lstPtSize.GetItemText(i,2);
		int nShrinkSize = _ttoi(strShrinkSize);
		if(nShrinkSize <= 0 || nShrinkSize >  m_pPtSizeProp[i].nFontSize)
		{
			bError = TRUE;
			CString strTmp;
			strTmp.Format(_T("Shrink size \"%s\" is not valid for font size \"%d\", valid range (0,%d]\n"),
				strShrinkSize,m_pPtSizeProp[i].nFontSize,m_pPtSizeProp[i].nFontSize);
			strError += strTmp;
		}
		else
		{
			m_pPtSizeProp[i].nShrinkSize = nShrinkSize;
		}
	}

	for(i=0;i<m_nExPtSizeNum;i++)
	{
		CString strType = m_lstExPtSize.GetItemText(i,1);
		if(strType.CompareNoCase(_T("MONO"))==0)
		{
			m_pExPtSizeProp[i].nFontType = FONT_TYPE_MONO;
		}
		else
		{
			m_pExPtSizeProp[i].nFontType = FONT_TYPE_GRAY;
		}

		CString strShrinkSize = m_lstExPtSize.GetItemText(i,2);
		int nShrinkSize = _ttoi(strShrinkSize);
		if(nShrinkSize <= 0 || nShrinkSize >  m_pExPtSizeProp[i].nFontSize)
		{
			bError = TRUE;
			CString strTmp;
			strTmp.Format(_T("Shrink size \"%s\" is not valid for font size \"%d\", valid range (0,%d]\n"),
				strShrinkSize,m_pExPtSizeProp[i].nFontSize,m_pExPtSizeProp[i].nFontSize);
			strError += strTmp;
		}
		else
		{
			m_pExPtSizeProp[i].nShrinkSize = nShrinkSize;
		}
	}

	if(bError)
	{
		AfxMessageBox(strError);
		return;
	}
	
	CDialog::OnOK();
}



void CDlgAdvSetting::OnEndlabeleditAsLstPtsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_lstPtSize.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	*pResult = 0;
}

void CDlgAdvSetting::OnEndlabeleditAsLstPtsizeEx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_lstExPtSize.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	*pResult = 0;
}

void CDlgAdvSetting::OnDblclkAsLstPtsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;

	CEdtBtnCtrl *pCtrl = NULL;
	int nItem = pnia->iItem;
	int nSubItem = pnia->iSubItem;
	if(nItem == -1 || nSubItem == 0)
		return;

	pCtrl = new CEdtBtnCtrl();

	if(NULL != pCtrl)
	{
		pCtrl->m_nItem = nItem;
		pCtrl->m_nSubItem = nSubItem;
		pCtrl->m_strText = m_lstPtSize.GetItemText(nItem,nSubItem);
		if(nSubItem==1)
		{
			pCtrl->m_nType = EB_COMBOX;
			pCtrl->m_agStrings.RemoveAll();
			pCtrl->m_agStrings.Copy(m_agFontType);
		}
		else if(nSubItem==2)
		{
			pCtrl->m_nType = EB_EDIT;
		}
		if(!pCtrl->Create(IDD_EDT_BTN_DLG,FromHandle(m_lstPtSize.m_hWnd)))
		{
			AfxMessageBox(_T("Error to create EdtBtnCtrl"));
			return;
		}
	}
	else
	{
		return;
	}

	CRect rect;
	m_lstPtSize.GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	CRect rtList;
	m_lstPtSize.GetClientRect(&rtList);
	if(rect.right>rtList.right)
		rect.right = rtList.right;
	if(rect.left < rtList.left)
		rect.left = rtList.left;
	if(rect.bottom>rtList.bottom)
		rect.bottom = rtList.bottom;
	if(rect.top < rtList.top)
		rect.top = rtList.top;
	
	if(pCtrl->m_nType == EB_COMBOX)
	{
		pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top+7,NULL);
	}
	else
	{
		pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top,NULL);
	}
	
    pCtrl->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CDlgAdvSetting::OnDblclkAsLstPtsizeEx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;

	CEdtBtnCtrl *pCtrl = NULL;
	int nItem = pnia->iItem;
	int nSubItem = pnia->iSubItem;
	if(nItem == -1 || nSubItem == 0)
		return;

	pCtrl = new CEdtBtnCtrl();

	if(NULL != pCtrl)
	{
		pCtrl->m_nItem = nItem;
		pCtrl->m_nSubItem = nSubItem;
		pCtrl->m_strText = m_lstExPtSize.GetItemText(nItem,nSubItem);
		if(nSubItem==1)
		{
			pCtrl->m_nType = EB_COMBOX;
			pCtrl->m_agStrings.RemoveAll();
			pCtrl->m_agStrings.Copy(m_agFontType);
		}
		else if(nSubItem==2)
		{
			pCtrl->m_nType = EB_EDIT;
		}
		if(!pCtrl->Create(IDD_EDT_BTN_DLG,FromHandle(m_lstExPtSize.m_hWnd)))
		{
			AfxMessageBox(_T("Error to create EdtBtnCtrl"));
			return;
		}
	}
	else
	{
		return;
	}

	CRect rect;
	m_lstExPtSize.GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	CRect rtList;
	m_lstExPtSize.GetClientRect(&rtList);
	if(rect.right>rtList.right)
		rect.right = rtList.right;
	if(rect.left < rtList.left)
		rect.left = rtList.left;
	if(rect.bottom>rtList.bottom)
		rect.bottom = rtList.bottom;
	if(rect.top < rtList.top)
		rect.top = rtList.top;
	
	if(pCtrl->m_nType == EB_COMBOX)
	{
		pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top+7,NULL);
	}
	else
	{
		pCtrl->SetWindowPos(&wndTop,rect.left,rect.top-1,rect.right-rect.left,rect.bottom-rect.top,NULL);
	}
	
    pCtrl->ShowWindow(SW_SHOW);
	*pResult = 0;
}
