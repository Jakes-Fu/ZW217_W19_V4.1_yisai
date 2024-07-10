// OwnershipListView.cpp : implementation file
//

#include "stdafx.h"
#include "Ownership.h"
#include "OwnershipListView.h"
#include "DlgLoadText.h"
#include "MainFrm.h"
#include "DlgUpdateOwnership.h"
#include "DlgGenSetting.h"
#include "DlgLoadMids.h"

static bool cmp_number_info(const NUMBER_INFO_PTR &pa,const NUMBER_INFO_PTR &pb)
{ 
	return pa->wNumber<pb->wNumber;
}


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CITYNAME_LENGTH			     30
/////////////////////////////////////////////////////////////////////////////
// COwnershipListView
//typedef bool (comp_data*)(NUMBER_INFO_PTR &pa,NUMBER_INFO_PTR &pb);

IMPLEMENT_DYNCREATE(COwnershipListView, CListView)

COwnershipListView::COwnershipListView()
{
	m_nUnknownCityID = 389;
	m_pListNumberInfo = NULL;
}

COwnershipListView::~COwnershipListView()
{
	ReleaseAllList();  //lint !e1551

	if(m_pListNumberInfo != NULL)
	{
		delete [] m_pListNumberInfo;
		m_pListNumberInfo = NULL;
	}
}


BEGIN_MESSAGE_MAP(COwnershipListView, CListView)
	//{{AFX_MSG_MAP(COwnershipListView)
	ON_COMMAND(ID_LOAD_TEXT_FILES, OnLoadTextFiles)
	ON_UPDATE_COMMAND_UI(ID_LOAD_TEXT_FILES, OnUpdateLoadTextFiles)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_GEN_CITY_MAP, OnGenOwnership)
	ON_UPDATE_COMMAND_UI(ID_GEN_CITY_MAP, OnUpdateGenOwnership)
	ON_COMMAND(ID_LOAD_MID_FILES, OnLoadMidFiles)
	ON_UPDATE_COMMAND_UI(ID_LOAD_MID_FILES, OnUpdateLoadMidFiles)
	ON_COMMAND(ID_CLEAR, OnClear)
	ON_UPDATE_COMMAND_UI(ID_CLEAR, OnUpdateClear)
	ON_COMMAND(ID_EXPORT_TEXT, OnExportText)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_TEXT, OnUpdateExportText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnershipListView drawing

void COwnershipListView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	UNUSED_ALWAYS(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// COwnershipListView diagnostics

#ifdef _DEBUG
void COwnershipListView::AssertValid() const
{
	CListView::AssertValid();
}

void COwnershipListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

COwnershipDoc* COwnershipListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COwnershipDoc)));
	return (COwnershipDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COwnershipListView message handlers

void COwnershipListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl &lc = GetListCtrl();

	DWORD dwStyle = lc.GetStyle();
    dwStyle |= LVS_REPORT/* | LVS_OWNERDRAWFIXED*/;
    lc.ModifyStyle(0,dwStyle);
	lc.SetBkColor(RGB(232, 232, 232));	
	lc.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = lc.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	lc.SetExtendedStyle(dwExStyle);
	
	lc.InsertColumn(0, _T("Number"),LVCFMT_LEFT,70);
	lc.InsertColumn(1, _T("City ID"),LVCFMT_LEFT,50);
	lc.InsertColumn(2, _T("City Name"),LVCFMT_LEFT,104);

	int nNumSector = g_theApp.m_agNumberSector.GetSize()/2;
	_ASSERTE(nNumSector>0);
	m_pListNumberInfo = new std::vector<NUMBER_INFO_PTR>[(UINT)nNumSector];
}

void COwnershipListView::OnLoadTextFiles() 
{
	// TODO: Add your command handler code here
	CListCtrl &lc = GetListCtrl();
	CDlgLoadText dlg;
	if(dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
		pMf->m_wndStatusBar.SetPaneText(0,_T("Loading..."));
		ReleaseAllList();
		lc.DeleteAllItems();		
		if(!LoadCityFile(dlg.m_strCityFile))
		{
			pMf->m_wndStatusBar.SetPaneText(0,_T("Load city file fail"));
			return;
		}
		if(!LoadNumberFiles(dlg.m_agNumberFiles))
		{
			pMf->m_wndStatusBar.SetPaneText(0,_T("Load text file fail"));
		}
		SortAndDisplay();
		pMf->m_wndStatusBar.SetPaneText(0,_T("Load complete"));
	}
}

void COwnershipListView::OnUpdateLoadTextFiles(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	UNREFERENCED_PARAMETER(pCmdUI);
}

BOOL COwnershipListView::LoadCityFile(LPCTSTR lpszFile)
{
	CWaitCursor wait;
	m_agCities.RemoveAll();
	CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
	CWaitCursor waitCur;
	CFileFind finder;
	if(!finder.FindFile(lpszFile))
	{
		CString strError;
		strError.Format(_T("Can not find city file [%s]"),lpszFile);
		AfxMessageBox(strError);
		return FALSE;
	}
	
	waitCur.Restore();
	
    FILE *pFile = _tfopen(lpszFile,_T("rb"));
	
	if(pFile == NULL)
	{
		CString strError;
		strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszFile,GetLastError());
		AfxMessageBox(strError);
		return FALSE;
	}
	
	fseek(pFile, 0, SEEK_END);
	DWORD dwSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	if(dwSize % CITYNAME_LENGTH != 0)
	{
		fclose(pFile);
		CString strError;
		strError.Format(_T("Can not load city file [%s]"),lpszFile);
		AfxMessageBox(strError);
		return FALSE;
	}
	
	DWORD dwItemCount = dwSize / CITYNAME_LENGTH;
	
	char szCityName[CITYNAME_LENGTH] = {0};
	TCHAR szCityNameW[CITYNAME_LENGTH] = {0};
	
	CProgressCtrl & prgCtrl = pMf->m_wndStatusBar.m_prgrsCtrl;
	prgCtrl.ShowWindow(SW_SHOW);
	prgCtrl.SetRange32(0, dwItemCount);
	prgCtrl.SetPos(0);
	
	BOOL bFindUnknownCity = FALSE;
	
	for (int i=0;i < (int)dwItemCount;i++)
	{
		memset(szCityName,0,CITYNAME_LENGTH);		
		dwSize = fread(szCityName,1,CITYNAME_LENGTH,pFile);
		
#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,szCityName,CITYNAME_LENGTH,szCityNameW,CITYNAME_LENGTH);
		m_agCities.Add(szCityNameW);
#else		
		m_agCities.Add(szCityName);
#endif
		if(!bFindUnknownCity)
		{
			if(m_agCities.GetAt(m_agCities.GetSize()-1).Compare(_T("未知"))==0)
			{
				m_nUnknownCityID = m_agCities.GetSize();
				bFindUnknownCity = TRUE;
			}
		}
		
		prgCtrl.SetPos(i);
		
	}
	
	fclose(pFile);
	
	prgCtrl.ShowWindow(SW_HIDE);
	
	if(!bFindUnknownCity)
	{
		m_agCities.Add(_T("未知"));
		m_nUnknownCityID = m_agCities.GetSize();
	}
	
	return TRUE;

}
BOOL COwnershipListView::LoadNumberFiles(const CStringArray &agFiles)
{
	_ASSERTE(m_pListNumberInfo != NULL);
	int nSectorNum =g_theApp.m_agNumberSector.GetSize()/2;	
	if(m_pListNumberInfo == NULL || nSectorNum == 0 )
	{
		return FALSE;
	}

	CWaitCursor wait;

	MSG msg;
	
//	CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
	
	int nFileNum = agFiles.GetSize();
	
	
	TCHAR szCityFullName[CITYNAME_LENGTH+1] = {0};
	char  szLine[300] = {0};
	TCHAR szLineW[300] = {0};
	TCHAR *pNumber = NULL;
	TCHAR *pProvince = NULL;
	TCHAR *pCity = NULL;
	
//	TCHAR szCityID[10] = {0};

	BOOL bEmpty = TRUE;
	
	int nCityNum = m_agCities.GetSize();
	int nCityID = -1;

	
	WORD   wUnknowCityID = (WORD)m_nUnknownCityID;
	int i=0;
	for(i= 0; i<nFileNum; i++)
	{
		FILE * pFile = _tfopen(agFiles.GetAt(i),_T("rt"));	
		if(pFile == NULL)
		{
			CString strError;
		    strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),agFiles.GetAt(i).operator LPCTSTR(),GetLastError());	
			AfxMessageBox(strError);
			continue;
		}

		wait.Restore();
		
		while(fgets(szLine,sizeof(szLine),pFile))
		{
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			wait.Restore();
			
			memset(szLineW,0,sizeof(szLineW));
			memset(szCityFullName,0,sizeof(szCityFullName));

#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP,0,szLine,strlen(szLine),szLineW,300);
#else
			strcpy(szLineW,szLine);
#endif

			CString strBuf = szLineW;

			//strBuf.Remove(_T(' '));
			//strBuf.Remove(_T('\r'));
			//strBuf.Remove(_T('\n'));
			//在MBCS下，使用remove会出现问题，需要用Replace
			//在Unicode，下没有问题。
			strBuf.Replace(_T(" "),_T(""));
			strBuf.Replace(_T("\r"),_T(""));
			strBuf.Replace(_T("\n"),_T(""));			

			LPTSTR pBuf =  strBuf.GetBuffer(1);
			pNumber = pBuf;
			pBuf = _tcschr(pBuf,_T(','));
			if(pBuf == NULL)
			{
				strBuf.ReleaseBuffer();
				memset(szLine,0,sizeof(szLine));
				continue;
			}
			*pBuf = _T('\0');
			pBuf = pBuf + 1;

			pProvince = pBuf;
			pBuf = _tcschr(pBuf,_T(','));
			if(pBuf == NULL)
			{
				strBuf.ReleaseBuffer();
				memset(szLine,0,sizeof(szLine));
				continue;
			}

			*pBuf = _T('\0');
			pBuf = pBuf + 1;

			pCity = pBuf;
			pBuf = _tcschr(pBuf,_T(','));
			if(pBuf == NULL)
			{
				strBuf.ReleaseBuffer();
				memset(szLine,0,sizeof(szLine));
				continue;
			}

			*pBuf = _T('\0');

			
			UINT nNumber = _ttoi(pNumber);	
			if(_tcscmp(pCity,_T("北京")) &&
				_tcscmp(pCity,_T("上海")) &&
				_tcscmp(pCity,_T("重庆")) &&
				_tcscmp(pCity,_T("天津")))
			{
				_tcscpy(szCityFullName,pProvince);
				_tcscat(szCityFullName,pCity);				
			}
			else
			{
				_tcscat(szCityFullName,pCity);
			}

			strBuf.ReleaseBuffer();
			
			nCityID = -1;
			for(int k = 0; k<nCityNum;k++)
			{
				CString strCity = m_agCities.GetAt(k);
				if(strCity.Compare(szCityFullName) == 0)
				{
					nCityID = k+1;
					break;
				}
			}
			if(nCityID==-1)
			{
				nCityID = wUnknowCityID;
			}
			
			for(int j=0;j<nSectorNum;j++)
			{
				if(nNumber>=g_theApp.m_agNumberSector[j*2] && nNumber <= g_theApp.m_agNumberSector[2*j+1])
				{
					NUMBER_INFO_PTR pNumberInfo = new NUMBER_INFO_T;
					pNumberInfo->wNumber = (WORD)(nNumber - g_theApp.m_agNumberSector[j*2]);
					pNumberInfo->wCityID = (WORD)nCityID;			
					m_pListNumberInfo[j].push_back(pNumberInfo);			
					bEmpty = FALSE;
					/*lint -save -e429 */
				}/*lint -restore */
			}

			memset(szLine,0,sizeof(szLine));
		}
		
		fclose(pFile);
	}
	if(bEmpty)
	{
		AfxMessageBox(_T("There is no valid data in text file.\r\nMaybe its format is not correct!"));
	}
	return TRUE;
}

BOOL COwnershipListView::SortAndDisplay()
{
	int nSectorNum =g_theApp.m_agNumberSector.GetSize()/2;
	if(m_pListNumberInfo == NULL || nSectorNum == 0)
	{
		return TRUE;
	}

	CWaitCursor wait;
	CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
	CProgressCtrl & prgCtrl = pMf->m_wndStatusBar.m_prgrsCtrl;
	prgCtrl.ShowWindow(SW_SHOW);

	TCHAR szBuf[100]={0};

	
	CListCtrl &lc = GetListCtrl();
	int nIndex = 0;
	for(int i=0;i<nSectorNum;i++)
	{
		if(m_pListNumberInfo[i].size()!=0)
		{
			std::sort(m_pListNumberInfo[i].begin(),m_pListNumberInfo[i].end(),cmp_number_info);
			
			int nSectorNumMin = g_theApp.m_agNumberSector.GetAt(i*2);
			int nSectorNumMax = g_theApp.m_agNumberSector.GetAt(i*2+1);

			int nCurBaseNumber = 0;
			int nNumCount = nSectorNumMax-nSectorNumMin+1;
			int nCurCityID = -1;
//			int nOldCityID = -1;
			int nCurNumber = 0;
			int nPos=0;

			

			prgCtrl.SetRange32(0, nNumCount);
			prgCtrl.SetPos(nPos);
			
			MSG msg;
			//std::vector<NUMBER_INFO_PTR>::iterator it;
			//for(it = m_pListNumberInfo[i].begin(); it!=m_pListNumberInfo[i].end();it++)
			int nCount = m_pListNumberInfo[i].size();
			for(int k = 0; k<nCount;k++)
			{
				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				
				wait.Restore();

				//nCurCityID = (*it)->wCityID;
				//nCurNumber = (*it)->wNumber;
				nCurCityID = m_pListNumberInfo[i].at(k)->wCityID;
			    nCurNumber = m_pListNumberInfo[i].at(k)->wNumber;
				if(nCurNumber > nCurBaseNumber)
				{					
					for(int j=0;j<(nCurNumber-nCurBaseNumber);j++)
					{
						NUMBER_INFO_PTR pNI = new NUMBER_INFO_T( (WORD)(nCurBaseNumber+j),(WORD)m_nUnknownCityID);
						_ASSERTE(pNI != NULL);		

						//m_pListNumberInfo[i].insert(it,pNI);
						m_pListNumberInfo[i].push_back(pNI);

						memset(szBuf,0,sizeof(szBuf));
						_stprintf(szBuf,_T("%d"),nCurBaseNumber+j+nSectorNumMin);			
						lc.InsertItem(nIndex,szBuf);

						memset(szBuf,0,sizeof(szBuf));
						_stprintf(szBuf,_T("%d"),m_nUnknownCityID);				
						lc.SetItemText(nIndex,1,szBuf);
						lc.SetItemText(nIndex,2,m_agCities.GetAt(m_nUnknownCityID-1));

						lc.SetItemData(nIndex,(DWORD)pNI);	
						
//						CString strText;
//						strText.Format(_T("%d"),nCurBaseNumber+j+nSectorNumMin);
//						lc.InsertItem(nIndex,strText);
// 						strText.Format(_T("%d"),m_nUnknownCityID);
// 						lc.SetItemText(nIndex,1,strText);
// 						lc.SetItemText(nIndex,2,m_agCities.GetAt(m_nUnknownCityID-1));
//						lc.SetItemData(nIndex,(DWORD)pNI);		
						nIndex++;
						prgCtrl.SetPos(++nPos);
						TRACE(_T("%d\r\n"),nIndex);
					}

					nCurBaseNumber = nCurNumber;
				}									

				CString strText;
				strText.Format(_T("%d"),nCurNumber+nSectorNumMin);
				lc.InsertItem(nIndex,strText);
				strText.Format(_T("%d"),nCurCityID);
				lc.SetItemText(nIndex,1,strText);
				lc.SetItemText(nIndex,2,m_agCities.GetAt(nCurCityID-1));
				//lc.SetItemData(nIndex,(DWORD)(*it));
				lc.SetItemData(nIndex,(DWORD)m_pListNumberInfo[i].at(k));
				nIndex++;
				nCurBaseNumber++;
				prgCtrl.SetPos(++nPos);

				if( k == ( nCount - 1 ) && nCurBaseNumber < nNumCount)
				{   
					for(int j=0;j<(nNumCount-nCurBaseNumber);j++)
					{
						NUMBER_INFO_PTR pNI = new NUMBER_INFO_T( (WORD)(nCurBaseNumber+j),(WORD)m_nUnknownCityID);
						_ASSERTE(pNI != NULL);		

						//m_pListNumberInfo[i].insert(it,pNI);
						m_pListNumberInfo[i].push_back(pNI);

						memset(szBuf,0,sizeof(szBuf));
						_stprintf(szBuf,_T("%d"),nCurBaseNumber+j+nSectorNumMin);			
						lc.InsertItem(nIndex,szBuf);

						memset(szBuf,0,sizeof(szBuf));
						_stprintf(szBuf,_T("%d"),m_nUnknownCityID);				
						lc.SetItemText(nIndex,1,szBuf);
						lc.SetItemText(nIndex,2,m_agCities.GetAt(m_nUnknownCityID-1));

						lc.SetItemData(nIndex,(DWORD)pNI);							
	
						nIndex++;
						prgCtrl.SetPos(++nPos);
						TRACE(_T("%d\r\n"),nIndex);
					}
				}
				
			}
			
			std::sort(m_pListNumberInfo[i].begin(),m_pListNumberInfo[i].end(),cmp_number_info);
		}
	}
	prgCtrl.ShowWindow(SW_HIDE);

	

	return TRUE;
}

BOOL COwnershipListView::GenerateOwnership(const CStringArray &agFiles)
{
	int nSectorNum = agFiles.GetSize();
	if(m_pListNumberInfo == NULL || nSectorNum == 0)
	{
		return FALSE;
	}
	CWaitCursor wait;
	CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
	CProgressCtrl & prgCtrl = pMf->m_wndStatusBar.m_prgrsCtrl;
	prgCtrl.ShowWindow(SW_SHOW);

	for(int i=0;i<nSectorNum;i++)
	{
		CString strDstFile = agFiles.GetAt(i);
		int nSize = m_pListNumberInfo[i].size();

		if(!strDstFile.IsEmpty() && nSize !=0)
		{
			int nPos=0;			
			prgCtrl.SetRange32(0, nSize);
			prgCtrl.SetPos(nPos);
			g_theApp.RemoveFileReadOnlyAttr(strDstFile);
			FILE * pFile = _tfopen(strDstFile,_T("wb"));
			if(pFile == NULL)
			{
				CString strError;
				strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),strDstFile.operator LPCTSTR(),GetLastError());
				AfxMessageBox(strError);
				continue;
			}
			MSG msg;
			int nCurCityID = -1;
			int nOldCityID = -1;
			std::vector<NUMBER_INFO_PTR>::iterator it;
			for(it = m_pListNumberInfo[i].begin(); it!=m_pListNumberInfo[i].end();it++)
			{	
				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				nCurCityID = (*it)->wCityID;
				if(nCurCityID != nOldCityID)
				{
					fwrite(*it,sizeof(NUMBER_INFO_T),1,pFile);
					nOldCityID = nCurCityID;
				}
				prgCtrl.SetPos(++nPos);
			}	

			fclose(pFile);
		}
	}

	prgCtrl.ShowWindow(SW_HIDE);
	return TRUE;
}

void COwnershipListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;
	int nItem = pnia->iItem;
	if(nItem == -1)
		return;
	CListCtrl &lc = GetListCtrl();
	
	CDlgUpdateOwnership dlg;
	dlg.m_nCityID = _ttoi(lc.GetItemText(nItem,1));
	dlg.m_strStartNumber = lc.GetItemText(nItem,0);
	dlg.m_strEndNumber = lc.GetItemText(nItem,0);
	dlg.m_pagCities = &m_agCities;
	CString strMaxNumber = lc.GetItemText(lc.GetItemCount()-1,0);
	dlg.m_nMaxEndNum = (UINT)_ttol(strMaxNumber.operator LPCTSTR());
	if(dlg.DoModal() == IDOK)
	{
		int nStartNumber = _ttoi(dlg.m_strStartNumber);
		int nEndNumber   = _ttoi(dlg.m_strEndNumber);
		int nNumberCount = nEndNumber - nStartNumber + 1;
		CString strCity = m_agCities.GetAt(dlg.m_nCityID-1);
		
		for(int i= 0; i< nNumberCount; i++)
		{
			CString strCurNumber= lc.GetItemText(nItem + i,0);
			UINT nNumber = (UINT)_ttol(strCurNumber.operator LPCTSTR());
			if(nNumber != (UINT)(nStartNumber + i))
			{
				nNumberCount -= (nNumber-nStartNumber - i);
				nStartNumber = nNumber - i;
			}

			CString strCityID;
			strCityID.Format(_T("%d"),dlg.m_nCityID);
			lc.SetItemText(nItem + i,1,strCityID);
			lc.SetItemText(nItem + i,2,strCity);
			
			NUMBER_INFO_PTR pNI = (NUMBER_INFO_PTR)lc.GetItemData(nItem + i);
			pNI->wCityID = (WORD)dlg.m_nCityID;
			
		}
		
	}
	*pResult = 0;
}

void COwnershipListView::OnGenOwnership() 
{
	// TODO: Add your command handler code here
	int nSectorNum = g_theApp.m_agNumberSector.GetSize()/2;
	if(m_pListNumberInfo == NULL || nSectorNum == 0)
	{
		return;
	}

	CDlgGenSetting dlg;	
	int i= 0;
	for(i=0;i<nSectorNum;i++)
	{		
		dlg.m_agFlag.Add(m_pListNumberInfo[i].size()>0?1:0);
	}

	if(dlg.DoModal() == IDOK)
	{
		int k = 0;
		CStringArray agFiles;
		for(i=0;i<nSectorNum;i++)
		{		
			if(dlg.m_agFlag.GetAt(i)==0)
			{
				agFiles.Add(_T(""));
			}
			else
			{
				agFiles.Add(dlg.m_agDstFiles.GetAt(k++));
			}
		}
		CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
		if(!GenerateOwnership(agFiles))
		{			
			pMf->m_wndStatusBar.SetPaneText(0,_T("Generate fail"));
		}
		else
		{
				pMf->m_wndStatusBar.SetPaneText(0,_T("Generate success!"));
		}
	}
}

void COwnershipListView::OnUpdateGenOwnership(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = FALSE;
	if(m_pListNumberInfo != NULL)
	{
		int nSectorNum = g_theApp.m_agNumberSector.GetSize()/2;
		for(int i=0;i<nSectorNum;i++)
		{		
			if(m_pListNumberInfo[i].size()>0)
			{
				bEnable=TRUE;
				break;
			}
		}
	}
	pCmdUI->Enable(bEnable);
}

void COwnershipListView::ReleaseAllList()
{
	int nSectorNum = g_theApp.m_agNumberSector.GetSize()/2;
	if(nSectorNum == 0 || m_pListNumberInfo == NULL)
	{
		return;
	}
	for(int i=0;i<nSectorNum;i++)
	{		
		if(m_pListNumberInfo[i].size()>0)
		{
			std::vector<NUMBER_INFO_PTR>::iterator it;
			for(it = m_pListNumberInfo[i].begin(); it!=m_pListNumberInfo[i].end();it++)
			{
				delete (*it);
			}
			m_pListNumberInfo[i].clear();
		}
	}
}

void COwnershipListView::OnLoadMidFiles() 
{
	// TODO: Add your command handler code here	
	CDlgLoadMids dlg;
	if(m_agCities.GetSize() > 0)
	{
		dlg.m_bSelCityFile = FALSE;
	}
	if(dlg.DoModal()== IDOK)
	{
		CWaitCursor wait;		
		CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
		pMf->m_wndStatusBar.SetPaneText(0,_T("Loading..."));
		_ASSERT(pMf != NULL);
		CListCtrl &lc = GetListCtrl();
		lc.DeleteAllItems();

		if(dlg.m_bSelCityFile)
		{
			if(!LoadCityFile(dlg.m_strCityFile))
			{
				if(pMf != NULL) 
					pMf->m_wndStatusBar.SetPaneText(0,_T("Load city file fail"));
				return;
			}
		}

		if(!LoadOwnershipFiles(dlg.m_agOwnershipFiles))
		{
			if(pMf != NULL)
				pMf->m_wndStatusBar.SetPaneText(0,_T("Load binary file fail"));
			return;
		}
		SortAndDisplay();
		if(pMf != NULL)
			pMf->m_wndStatusBar.SetPaneText(0,_T("Load complete"));
	}
}

void COwnershipListView::OnUpdateLoadMidFiles(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	UNUSED_ALWAYS(pCmdUI);
}

BOOL COwnershipListView::LoadOwnershipFiles(const CStringArray &agFiles)
{
	int nSectorNum = agFiles.GetSize();
	if(m_pListNumberInfo == NULL || nSectorNum == 0)
	{
		return FALSE;
	}

	CWaitCursor wait;

	CMainFrame * pMf = (CMainFrame *)AfxGetMainWnd();
	CProgressCtrl & prgCtrl = pMf->m_wndStatusBar.m_prgrsCtrl;
	prgCtrl.ShowWindow(SW_SHOW);

	int nCityNum = m_agCities.GetSize();
	for(int i=0;i<nSectorNum;i++)
	{	
		if(agFiles.GetAt(i).IsEmpty())
		{
			continue;
		}

		BOOL bListEmpty = TRUE;

		std::vector<NUMBER_INFO_PTR>::iterator it = NULL;
		
		if(m_pListNumberInfo[i].size() != 0)
		{
			bListEmpty = FALSE;
			it = m_pListNumberInfo[i].begin();
		}

		FILE * pFile = _tfopen(agFiles.GetAt(i),_T("rb"));	
		if(pFile == NULL)
		{
			CString strError;
			strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),agFiles.GetAt(i).operator LPCTSTR(),GetLastError());
			AfxMessageBox(strError);
			wait.Restore();
			continue;
		}
		fseek(pFile, 0, SEEK_END);
		DWORD dwSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		if(dwSize % sizeof(NUMBER_INFO_T) != 0)
		{
			fclose(pFile);

			CString strError;
			strError.Format(_T("Load [%s] failed!"),agFiles.GetAt(i).operator LPCTSTR());
			AfxMessageBox(strError);
			
			wait.Restore();
			continue;
		}

		DWORD dwItemCount = dwSize / sizeof(NUMBER_INFO_T);
		int nCurNumber = 0;
		int nCurCityID = -1;
		int nOldNumber = 0;
		int nOldCityID = -1;	


		int nNumberCount = g_theApp.m_agNumberSector[2*i+1] - g_theApp.m_agNumberSector[2*i] +1;
		int nPos=0;
		prgCtrl.SetRange32(0, nNumberCount);
		prgCtrl.SetPos(nPos);

		MSG msg;

		for(int j=0; j< (int)dwItemCount;j++)
		{

			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			wait.Restore();

			NUMBER_INFO_T ni;
			fread(&ni,sizeof(NUMBER_INFO_T),1,pFile);
			nCurCityID = ni.wCityID;
			nCurNumber = ni.wNumber;
			if(nCurCityID > nCityNum || nCurNumber >= nNumberCount || (j==0 && nCurNumber !=0))
			{
				ReleaseAllList();
				CString strError;
				strError.Format(_T("City map file (%s) is not correct"),agFiles.GetAt(i).operator LPCTSTR());
				AfxMessageBox(strError);
				return FALSE;
			}

			if(j>0)
			{
				for(int k = nOldNumber; k< nCurNumber;k++)
				{
					NUMBER_INFO_PTR pNI = new NUMBER_INFO_T;
					pNI->wCityID = (WORD)nOldCityID;
					pNI->wNumber = (WORD)k;
					if(!bListEmpty)
					{
						if(it != NULL)
						{
							_ASSERT((*it)->wNumber == pNI->wNumber);

							if((*it)->wCityID == (WORD)m_nUnknownCityID )
							{
								(*it)->wCityID = pNI->wCityID;
							}
						}

						delete pNI;
					}
					else
					{
						m_pListNumberInfo[i].push_back(pNI);
					}
					prgCtrl.SetPos(++nPos);
				}
			}
			nOldCityID = nCurCityID;
			nOldNumber = nCurNumber;

			if(j== (int)(dwItemCount-1))
			{
				for(int k = nOldNumber; k< nNumberCount;k++)
				{
					NUMBER_INFO_PTR pNI = new NUMBER_INFO_T;
					pNI->wCityID = (WORD)nOldCityID;
					pNI->wNumber = (WORD)k;

					if(!bListEmpty)
					{
						if(it != NULL)
						{
							_ASSERT((*it)->wNumber == pNI->wNumber);
							
							if((*it)->wCityID == (WORD)m_nUnknownCityID )
							{
								(*it)->wCityID = pNI->wCityID;
							}
						}
						
						delete pNI;
					}
					else
					{
						m_pListNumberInfo[i].push_back(pNI);
					}					
	
					prgCtrl.SetPos(++nPos);
				}
			}
		}
		fclose(pFile);
	}
	prgCtrl.ShowWindow(SW_HIDE);
	return TRUE;
}

void COwnershipListView::OnClear() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;		
	ReleaseAllList();
	CListCtrl &lc = GetListCtrl();
	lc.DeleteAllItems();
	m_agCities.RemoveAll();

}

void COwnershipListView::OnUpdateClear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = m_agCities.GetSize()>0 ? TRUE : FALSE;
	pCmdUI->Enable(bEnable);
}

void COwnershipListView::OnExportText() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
	_T("City file (*.txt)|*.txt||"));
	
	if(dlg.DoModal() == IDCANCEL)
	{
		 return;
	}

	CWaitCursor wait;

	CString strFile = dlg.GetPathName();

	DWORD dwAtt = ::GetFileAttributes(strFile);
	dwAtt &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strFile,dwAtt);

	CListCtrl &lc = GetListCtrl();
	int nCount = lc.GetItemCount();

	char szCityName[CITYNAME_LENGTH] = {0};
	char szNumber[CITYNAME_LENGTH] = {0};
	
	FILE *pFile = _tfopen(strFile,_T("wt"));

	MSG msg;
	for(int i=0; i<nCount; i++)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		wait.Restore();

		memset(szNumber,0,sizeof(szNumber));
		memset(szCityName,0,sizeof(szCityName));
		CString strNumber = lc.GetItemText(i,0);
		CString strCityName = lc.GetItemText(i,2);
#ifdef _UNICODE		
		WideCharToMultiByte(CP_ACP,0,strNumber,strNumber.GetLength(),szNumber,CITYNAME_LENGTH,NULL,NULL);
		WideCharToMultiByte(CP_ACP,0,strCityName,strCityName.GetLength(),szCityName,CITYNAME_LENGTH,NULL,NULL);
#else		
		strcpy(szNumber,strNumber);
		strcpy(szCityName,strCityName);
#endif	
		
		fprintf(pFile,"%s, ,%s,\r\n",szNumber,szCityName);
	}

	fclose(pFile);

	AfxMessageBox(_T("Export complete!"));
}

void COwnershipListView::OnUpdateExportText(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = m_agCities.GetSize()>0 ? TRUE : FALSE;
	pCmdUI->Enable(bEnable);
}
