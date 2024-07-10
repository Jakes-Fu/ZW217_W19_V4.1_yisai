// TPAddrDescDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "TPAddrDescDlg.h"

#include "TPAddrDescItemDlg.h"
#include "eval.h"

#include "LUA_FUNC.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTPAddrDescDlg dialog

const char CTPAddrDescDlg::CSZ_HEADER[32] = "DSPLogger data file";

CTPAddrDescDlg::CTPAddrDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTPAddrDescDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTPAddrDescDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTPAddrDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTPAddrDescDlg)
	DDX_Control(pDX, IDC_LIST_DESC, m_ctlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTPAddrDescDlg, CDialog)
	//{{AFX_MSG_MAP(CTPAddrDescDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DESC, OnDblclkListDesc)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_WM_CLOSE()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DESC, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTPAddrDescDlg message handlers

BOOL CTPAddrDescDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    DWORD dwStyle = m_ctlList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_ctlList.SetExtendedStyle(dwStyle);

    m_ctlList.InsertColumn(0,"Address",LVCFMT_LEFT,80);
	m_ctlList.InsertColumn(1,"Data",LVCFMT_LEFT,80);
    m_ctlList.InsertColumn(2,"Description",LVCFMT_LEFT,0);
    m_ctlList.InsertColumn(3,"Filter",LVCFMT_LEFT,0);
    m_ctlList.InsertColumn(4, "Color", LVCFMT_LEFT, 100);

    // Load from file
    Load();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTPAddrDescDlg::Load()
{
    // Get File path first
    char szPath[_MAX_PATH];
    CWinApp* pApp = AfxGetApp();

    ::GetModuleFileName(pApp->m_hInstance,szPath,_MAX_PATH);
	
    char* p = strrchr(szPath,'.');
    p++;
    strcpy(p,"dat");

    // Open the file and load data
    CString strText;
    CStdioFile file(szPath, CFile::modeRead);

    int nCount = 0;
    m_ctlList.DeleteAllItems();
    m_AddrMap.RemoveAll();
    m_mapColor.RemoveAll();

    file.ReadString(strText);

    char szAddr[100] = {0}, szData[100] = {0}, szDesc[100] = {0},szFilter[100] = {0};
    char szColor[32] = { 0 };

    DWORD dwAddr(0);
	//teana hu 2009.08.21
	DWORD dwData = 0;

    if(strText.Compare(CSZ_HEADER))
    {
        // Previous version
        do
        {
            sscanf(strText,"%s",szAddr);
            int index = strText.Find(' ',0);
            strText.Delete(0,index);
            strText.TrimLeft();
            strcpy(szDesc,strText);
            sscanf(szAddr,"%lx",&dwAddr);

            m_ctlList.InsertItem(nCount,szAddr);
            m_ctlList.SetItemText(nCount,2,szDesc);

            // add by anli
            m_ctlList.SetItemData( nCount, RGB(0,0,0) );
            
			CString strKey;
			strKey.Format("%lx", dwAddr);
            m_AddrMap.SetAt(strKey,nCount);
			m_mapColor.SetAt(strKey, RGB(0,0,0));
            nCount++;
        }while(file.ReadString(strText));
    }
    else
    {
        while(file.ReadString(strText))
        {
            if(0 == strText.Find("Addr = "))
            {
                strText.Delete(0,7);
                strcpy(szAddr,strText);
                sscanf(szAddr,"%lx",&dwAddr);
                szDesc[0] = 0;
                szFilter[0] = 0;
				strcpy(szData, "****");
            }
			else if(0 == strText.Find("Data = "))
			{
				strText.Delete(0,7);
                strcpy(szData,strText);
				//teana hu 2009.08.21
				sscanf(szData, "%lx", &dwData);
			}
            else if(0 == strText.Find("Desc = "))
            {
                strText.Delete(0,7);
                strcpy(szDesc,strText);
            }
            else if(0 == strText.Find("Filt = "))
            {
                strText.Delete(0,7);
                strcpy(szFilter,strText);
            }
            else if( 0 == strText.Find("Color= "))
            {
                strText.Delete(0,7);
                strcpy(szColor,strText);

                m_ctlList.InsertItem(nCount,szAddr);
				m_ctlList.SetItemText(nCount,1,szData);
                m_ctlList.SetItemText(nCount,2,szDesc);
                m_ctlList.SetItemText(nCount,3,szFilter);

                DWORD dwClr = atol(szColor);
                m_ctlList.SetItemData(nCount, dwClr);

				//teana hu 2009.08.14
				CString strKey;
				if(strcmp(szData, "****") != 0)
				{
					strKey.Format("%lx(%lx)", dwAddr, dwData);
				}
				else
				{
					strKey.Format("%lx", dwAddr);
				}
				
				m_AddrMap.SetAt(strKey, nCount);
				m_mapColor.SetAt(strKey, dwClr);
				//
                nCount++;
            }
        }
    }

    file.Close();

    return TRUE;
}

BOOL CTPAddrDescDlg::Save()
{
    // Get file path first
    char szPath[_MAX_PATH];
    CWinApp* pApp = AfxGetApp();

    ::GetModuleFileName(pApp->m_hInstance,szPath,_MAX_PATH);

    char* p = strrchr(szPath,'.');
    p++;
    strcpy(p,"dat");
	
	//open file and write
	CStdioFile file;
	if( !file.Open( szPath, CFile::modeWrite | CFile::modeCreate) )
	{
		AfxMessageBox("Unable to Open and Write file !");
		return FALSE;
	}
	
    //CStdioFile file(szPath,  CFile::modeWrite | CFile::modeCreate);

    // Save file header
    file.WriteString(CSZ_HEADER);
    file.WriteString("\n\n");

    int nCount = m_ctlList.GetItemCount();
    for(int i=0;i<nCount;i++)
    {
        CString strText;

        // Save address
        strText = "Addr = ";
        strText += m_ctlList.GetItemText(i,0);
        strText += "\n";
        file.WriteString(strText);

		// Save data
		strText = "Data = ";
        strText += m_ctlList.GetItemText(i,1);
        strText += "\n";
        file.WriteString(strText);

        // Save description
        strText = "Desc = ";
        strText += m_ctlList.GetItemText(i,2);
        strText += "\n";
        file.WriteString(strText);

        // Save filter infomation
        strText = "Filt = ";
        strText += m_ctlList.GetItemText(i,3);
        strText += "\n";
        file.WriteString(strText);

        // save color
        strText.Format("Color= %d\n\n", m_ctlList.GetItemData(i));
        file.WriteString(strText);
    }

    return FALSE;
}

void CTPAddrDescDlg::LookupDesc(DWORD dwAddr,DWORD dwData, char* szDesc)
{
// 	char* str_tmp=(char *)malloc(256);
// 	lua_description((int)dwAddr, (int)dwData, &str_tmp);
// 	sprintf(szDesc,"%s",str_tmp);
// 	free(str_tmp);
	char sztmpDesc[MAX_LUA_DESCTRIPTION_LEN] = {0};
	lua_description_ex((int)dwAddr, (int)dwData,sztmpDesc);
	if(szDesc)
	{
		strncpy(szDesc,sztmpDesc,MAX_LUA_DESCTRIPTION_LEN);
	}
/*    ASSERT(szDesc != NULL);
	int index = -1;
	DWORD dwLikeAddr = 0;

	//teana hu 2009.08.21
	CString strKey;
	strKey.Format("%lx(%lx)", dwAddr, dwData);
	BOOL bRet = m_AddrMap.Lookup(strKey,index);
	if(!bRet)
	{
		strKey.Format("%lx", dwAddr);
		bRet = m_AddrMap.Lookup(strKey,index);
		if(!bRet)
		{
			dwLikeAddr = dwAddr >> 4;
			strKey.Format("%lx", dwLikeAddr);
			bRet = m_AddrMap.Lookup(strKey,index);
			if(!bRet)
			{
				dwLikeAddr = dwAddr >> 8;
				strKey.Format("%lx", dwLikeAddr);
				bRet = m_AddrMap.Lookup(strKey,index);
			}
		}
		
	}

	if(bRet)
    {
        m_ctlList.GetItemText(index,2,szDesc,100);

		//teana hu 2009.08.24
		CString strDesc = szDesc;
		
		if(strDesc.Find("%") != -1)
		{
			CString strTemp = strDesc;
			int nLeft = strTemp.Replace("(", "");
			int nRight = strTemp.Replace(")", "");
			if(nLeft != nRight)
			{
				strDesc.Replace("(", "");
				strDesc.Replace(")", "");
			}


			CString strFormat = _T("");
			CString strVar = _T("");
			int nIndex = strDesc.Find(_T("\","));
			if(nIndex != -1)
			{
				strFormat = strDesc.Left(nIndex + 1);
				strDesc.Delete(0, nIndex + 2);
				strVar = strDesc;
				strFormat.Replace("\"", "");
				strVar.Replace("data", "%d");
				strVar.Replace("Data", "%d");
				strVar.Replace("DATA", "%d");
			}

			char szValue[50] = {0};
			sprintf(szValue, strVar, dwData);
			Ceval x1(szValue);
			int nValue = x1.eval();

			sprintf(szDesc, strFormat, nValue);

		}
    }
    else
    {
        szDesc[0] = 0;
    }
*/
}

DWORD CTPAddrDescDlg::LookupColor( DWORD dwAddr, DWORD dwData  )
{
    DWORD dwClr = RGB(0, 0, 0);
	DWORD dwLikeAddr = 0;

	//teana hu 2009.08.24
	CString strKey;
	strKey.Format("%lx(%lx)", dwAddr, dwData);
	BOOL bRet = m_mapColor.Lookup(strKey,dwClr);
	if(!bRet)
	{
		strKey.Format("%lx", dwAddr);
		bRet = m_mapColor.Lookup(strKey,dwClr);
		if(!bRet)
		{
			dwLikeAddr = dwAddr >> 4;
			strKey.Format("%lx", dwLikeAddr);
			bRet = m_mapColor.Lookup(strKey,dwClr);
			if(!bRet)
			{
				dwLikeAddr = dwAddr >> 8;
				strKey.Format("%lx", dwLikeAddr);
				bRet = m_mapColor.Lookup(strKey,dwClr);
			}
		}
		
	}
    
    return dwClr;
}

void CTPAddrDescDlg::OnButtonLoad() 
{
    Load();	
}

void CTPAddrDescDlg::Refresh() 
{
    GetParent()->Invalidate();	
}


BOOL CTPAddrDescDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_DELETE)
        {
            RemoveItem();
        }
    }

	return CDialog::PreTranslateMessage(pMsg);
}

void CTPAddrDescDlg::RemoveItem()
{
    POSITION pos = m_ctlList.GetFirstSelectedItemPosition();
    if(pos == NULL)
    {
        // No item was selected
        return;
    }

    int nIndex = m_ctlList.GetNextSelectedItem(pos);

#if 0
    if(IDOK != AfxMessageBox(IDS_REMOVE_ITEM,MB_OKCANCEL,0))
    {
        // User cancelled
        return;
    }
#endif
	
    RemoveItem(nIndex);
}

void CTPAddrDescDlg::RemoveItem(int nIndex)
{
    if( nIndex < 0 || nIndex > m_ctlList.GetItemCount() )
    {
        return;
    }

    _TCHAR szTxt[64] = { 0 };
    LPTSTR pEnd;

    m_ctlList.GetItemText(nIndex, 0, szTxt, 64);
        
    DWORD dwAddr = _tcstoul(szTxt, &pEnd, 16);

	//teana hu 2009.08.21
	m_ctlList.GetItemText(nIndex, 1, szTxt, 64);
	DWORD dwData = _tcstoul(szTxt, &pEnd, 16);
	//

	CString strKey;
	if(strcmp(szTxt, "****") != 0)
	{
		strKey.Format("%lx(%lx)",dwAddr, dwData);
	}
	else
	{
		strKey.Format("%lx",dwAddr);
	}
    m_AddrMap.RemoveKey(strKey);
	m_mapColor.RemoveKey(strKey);

    LockWindowUpdate();

    m_ctlList.DeleteItem(nIndex);
    RefreshAddrMap();

    UnlockWindowUpdate();
}

void CTPAddrDescDlg::OnButtonAdd() 
{
	CTPAddrDescItemDlg dlg;

    if(IDOK != dlg.DoModal())
    {
        return;
    }

    int nCount = m_ctlList.GetItemCount();

    AddItem(nCount,dlg.m_strAddr, dlg.m_strData,dlg.m_strDesc,dlg.m_strFilter, dlg.m_dwColor);
}

void CTPAddrDescDlg::AddItem(int nIndex,CString& strAddr, CString& strData, CString& strDesc,
                             CString& strFilter, DWORD dwColor)
{
    // Check parameter
    if(nIndex < 0)
    {
        return;
    }

    int nCount = m_ctlList.GetItemCount();
    if(nIndex > nCount)
    {
        return;
    }

	DWORD dwAddr;
	sscanf(strAddr,"%lx",&dwAddr);
	DWORD dwData;
	sscanf(strData,"%lx",&dwData);

	//teana hu 2009.08.21
	BOOL bRet = FALSE;
	CString strKey;
	int index = -1;
	if(strcmp(strData, "****") != 0)
	{
		strKey.Format("%lx(%lx)", dwAddr, dwData);
	}
	else
	{
		strKey.Format("%lx", dwAddr);
	}
	m_mapColor.SetAt(strKey, dwColor);
	//
    bRet = m_AddrMap.Lookup(strKey,index);

    if(bRet)
    {
        // Already have this item
        CString strText;
        strText.LoadString(IDS_TP_ADDR_DESC_DUPLICATE);
        if(IDOK == AfxMessageBox(strText,MB_OKCANCEL))
        {
			m_ctlList.SetItemText(index,1,strData);
            m_ctlList.SetItemText(index,2,strDesc);
            m_ctlList.SetItemText(index,3,strFilter);
            m_ctlList.SetItemData(index, dwColor);            
        }
    }
    else
    {
        m_ctlList.InsertItem(nIndex,strAddr);
		m_ctlList.SetItemText(nIndex,1,strData);
        m_ctlList.SetItemText(nIndex,2,strDesc);
        m_ctlList.SetItemText(nIndex,3,strFilter);
        
        RefreshAddrMap();
    }
    
    m_ctlList.SetItemState(nIndex,LVIS_SELECTED, LVIS_SELECTED);

    Refresh();
}

void CTPAddrDescDlg::OnDblclkListDesc(NMHDR* , LRESULT* ) 
{
    POSITION pos = m_ctlList.GetFirstSelectedItemPosition();
    if(pos == NULL)
    {
        // No item was selected
        return;
    }

    int nIndex = m_ctlList.GetNextSelectedItem(pos);

    CTPAddrDescItemDlg dlg;
    dlg.m_strAddr = m_ctlList.GetItemText(nIndex,0);
	dlg.m_strData = m_ctlList.GetItemText(nIndex,1);
    dlg.m_strDesc = m_ctlList.GetItemText(nIndex,2);
    dlg.m_strFilter = m_ctlList.GetItemText(nIndex,3);
    dlg.m_dwColor   = m_ctlList.GetItemData(nIndex);

    DWORD dwAddr;
    sscanf(dlg.m_strAddr,"%lx",&dwAddr);

    if(IDOK != dlg.DoModal())
    {
        return;
    }

    RemoveItem(nIndex);    
    AddItem(nIndex,dlg.m_strAddr,dlg.m_strData, dlg.m_strDesc,dlg.m_strFilter, dlg.m_dwColor);
}

/*@ Liu Kai CR4895 2003-9-23 */
BOOL CTPAddrDescDlg::FindFilter(DWORD dwAddr,DWORD dwData, char* pszFilter)
{
    ASSERT(pszFilter != NULL);

    int index = 0;
	BOOL bRet = FALSE;
	//teana hu 2009.08.21
	CString strKey;
	strKey.Format("%lx(%lx)", dwAddr, dwData);
	//

	bRet = m_AddrMap.Lookup(strKey,index);
	if(!bRet)
	{
		strKey.Format("%lx", dwAddr);
		bRet = m_AddrMap.Lookup(strKey,index);
	}
    
    if(bRet)
    {
        m_ctlList.GetItemText(index,2,pszFilter,100);
        if(0 == strlen(pszFilter))
        {
            bRet = FALSE;
        }
    }
    else
    {
        pszFilter[0] = 0;
    }

    return bRet;
}

void CTPAddrDescDlg::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LPNMLVCUSTOMDRAW plvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

    switch(plvcd->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;
    case CDDS_ITEMPREPAINT:
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
    case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
        if( plvcd->iSubItem == 3 )
        {
		    plvcd->clrTextBk = plvcd->nmcd.lItemlParam;//RGB(0,200,10);
        }
		*pResult = CDRF_DODEFAULT;
		break;
    default:
	    *pResult = 0;
    }
}

void CTPAddrDescDlg::RefreshAddrMap()
{
    int   nCount = m_ctlList.GetItemCount();
    DWORD dwAddr = 0;
    DWORD dwClr  = 0;
	DWORD dwData = 0;
    _TCHAR szTxt[64] = { 0 };
    LPTSTR pEnd;

    for( int i = 0; i < nCount; ++i )
    {
        m_ctlList.GetItemText(i, 0, szTxt, 64);
        
        dwAddr = _tcstoul(szTxt, &pEnd, 16);

		//teana hu 2009.08.21
		m_ctlList.GetItemText(i, 1, szTxt, 64);
		dwData = _tcstoul(szTxt, &pEnd, 16);
		CString strKey;
		if(strcmp(szTxt, "****") != 0)
		{
			strKey.Format("%lx(%lx)", dwAddr, dwData);
		}
		else
		{
			strKey.Format("%lx", dwAddr);
		}
		//

        m_AddrMap.SetAt( strKey, i );

        dwClr = 0;
        m_mapColor.Lookup(strKey, dwClr);
        m_ctlList.SetItemData(i, dwClr);
    }
}

void CTPAddrDescDlg::OnButtonDel() 
{	
	RemoveItem();
}


void CTPAddrDescDlg::OnClose() 
{
    Save();
	CDialog::OnClose();
}

void CTPAddrDescDlg::OnOK() 
{
    Save();	
	CDialog::OnOK();
}
