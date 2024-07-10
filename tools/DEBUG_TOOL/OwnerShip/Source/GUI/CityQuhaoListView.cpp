// CityQuhaoListView.cpp : implementation file
//

#include "stdafx.h"
#include "ownership.h"
#include "CityQuhaoListView.h"
#include <assert.h>
#include "DlgGenQuhao.h"
#include "DlgConvSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static bool cmp_quhao_info(const CITY_QUHAO_PTR &pa,const CITY_QUHAO_PTR &pb)
{ 
	return pa->uQuhao<pb->uQuhao;
}

/////////////////////////////////////////////////////////////////////////////
// CCityQuhaoListView

IMPLEMENT_DYNCREATE(CCityQuhaoListView, CListView)

CCityQuhaoListView::CCityQuhaoListView()
{
}

CCityQuhaoListView::~CCityQuhaoListView()
{
	ReleaseCityInfo(); //lint !e1551
}


BEGIN_MESSAGE_MAP(CCityQuhaoListView, CListView)
	//{{AFX_MSG_MAP(CCityQuhaoListView)
	ON_COMMAND(ID_LOAD_CITY_QUHAO, OnLoadCityQuhao)
	ON_UPDATE_COMMAND_UI(ID_LOAD_CITY_QUHAO, OnUpdateLoadCityQuhao)
	ON_COMMAND(ID_GEN_CITY_QUHAO, OnGenCityQuhao)
	ON_UPDATE_COMMAND_UI(ID_GEN_CITY_QUHAO, OnUpdateGenCityQuhao)
	ON_COMMAND(ID_CONV_CITY_QUHAO_MID_TO_TXT, OnConvCityQuhaoMidToTxt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCityQuhaoListView drawing

void CCityQuhaoListView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCityQuhaoListView diagnostics

#ifdef _DEBUG
void CCityQuhaoListView::AssertValid() const
{
	CListView::AssertValid();
}

void CCityQuhaoListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCityQuhaoListView message handlers

void CCityQuhaoListView::OnInitialUpdate() 
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
	
	lc.InsertColumn(0, _T("Province"),LVCFMT_LEFT,70);
	lc.InsertColumn(1, _T("City"),LVCFMT_LEFT,70);
	lc.InsertColumn(2, _T("Quhao"),LVCFMT_LEFT,65);
}

BOOL CCityQuhaoListView::LoadCityQuhaoText(LPCTSTR lpszFileName)
{
	CWaitCursor wait;
	FILE *pFile = NULL;
	CListCtrl &lc = GetListCtrl();
	lc.DeleteAllItems();
	ReleaseCityInfo();
	int nDirectIdx = 1;
	try
	{		
		pFile = _tfopen(lpszFileName,_T("rt"));
		if(pFile == NULL)
		{
			CString strError;
			strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszFileName,GetLastError());
			AfxMessageBox(strError);
			return FALSE;
		}
		char szLineA[_MAX_PATH] = {0};
		TCHAR szLine[_MAX_PATH] = {0};

		TCHAR *pProvince = NULL;
		TCHAR *pCity = NULL;
		TCHAR *pCur = NULL;
		TCHAR *pEnd = NULL;
		UINT    uQuhao=0;

		MSG msg;
		
		while(fgets(szLineA,_MAX_PATH,pFile))
		{
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			
			wait.Restore();
			CString strLine;
			memset(szLine,0,sizeof(szLine));
#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP,0,szLineA,strlen(szLineA),szLine,_MAX_PATH);
			strLine = szLine;
#else
			strLine = szLineA;
#endif
			strLine.TrimRight();
			strLine.TrimLeft();
			
			pCur = strLine.GetBuffer(_MAX_PATH);			
			if(!strLine.IsEmpty())
			{
				pEnd = _tcschr(pCur,_T(','));
				if(pEnd != NULL)
				{
					*pEnd = '\0';					
				}
				else
				{
					strLine.ReleaseBuffer();
					memset(szLineA,0,sizeof(szLineA));
					continue;
				}
				pProvince   = pCur;
				pCur = pEnd +1;

				pEnd = _tcschr(pCur,_T(','));
				if(pEnd != NULL)
				{
					*pEnd = '\0';					
				}
				else
				{
					strLine.ReleaseBuffer();
					memset(szLineA,0,sizeof(szLineA));
					continue;
				}
				pCity = pCur;

				pCur = pEnd +1;							
				uQuhao = _ttoi(pCur);
				
				CString strProvice = pProvince;	
				strProvice.TrimRight();				
				if(strProvice.IsEmpty())
				{
					strProvice.Format(_T("#%d"),nDirectIdx++);
				}			

				CITY_QUHAO_PTR pCP = new CITY_QUHAO_T(strProvice,pCity,uQuhao);
				m_CityInfo.push_back(pCP);

				strLine.ReleaseBuffer();
				int nCurItem = lc.GetItemCount();
				CString strQuhao;
				strQuhao.Format(_T("%d"),uQuhao);
				lc.InsertItem(nCurItem,pProvince);
				lc.SetItemText(nCurItem,1,pCity);
				lc.SetItemText(nCurItem,2,strQuhao);

				PROVINCE_PTR pProv = NULL;
				if(!m_mapPcq.Lookup(strProvice,pProv))
				{
					pProv = new PROVINCE_T;
					_tcscpy(pProv->szProvince, strProvice);
					pProv->agCityQuhao.Add(pCP);
					m_mapPcq.SetAt(strProvice,pProv);

					m_agProvinces.Add(strProvice);
				}
				else
				{
					pProv->agCityQuhao.Add(pCP);
				}
				/*lint -save -e429 */
			}/*lint -restore */
			
			memset(szLineA,0,sizeof(szLineA));
	
		}
		
	}
	catch (CException &e)
	{
		if(pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
		TCHAR szError[_MAX_PATH] = {0};
		e.GetErrorMessage(szError,_MAX_PATH);
		if(_tcslen(szError)!=NULL)
		{
			AfxMessageBox(szError);
			return FALSE;
		}
	}

	if(pFile != NULL)
	{
		fclose(pFile);
		pFile = NULL;
	}
    if(lc.GetItemCount()==0)
	{
		AfxMessageBox(_T("There is no validate data in file!"));
	}
	return TRUE;
}

void CCityQuhaoListView::OnLoadCityQuhao() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST/* | OFN_ALLOWMULTISELECT*/, 
		_T("City quhao file (*.txt)|*.txt||"));
	if(dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		LoadCityQuhaoText(strFilePath);
		AfxMessageBox(_T("Load complete!"));
	}
}

void CCityQuhaoListView::OnUpdateLoadCityQuhao(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}
void CCityQuhaoListView::ReleaseCityInfo()
{	
	if(m_CityInfo.size()>0)
	{
		std::vector<CITY_QUHAO_PTR>::iterator it;
		for(it = m_CityInfo.begin(); it!=m_CityInfo.end();it++)
		{
			delete (*it);
		}
		m_CityInfo.clear();
	}

	//m_agProvinces.RemoveAll();

	m_agProvinces.RemoveAll();
	
	CString  strKey = _T("");
    POSITION pos = NULL;    
    PROVINCE_PTR pProv= NULL;
    for( pos = m_mapPcq.GetStartPosition(); pos != NULL; )
    {
        m_mapPcq.GetNextAssoc( pos, strKey, pProv );	
		pProv->agCityQuhao.RemoveAll();
        delete pProv;
    }
    m_mapPcq.RemoveAll();
}


BOOL CCityQuhaoListView::GenerateCityQuhao(LPCTSTR lpszMidFileName)
{
	if(lpszMidFileName == NULL || _tcslen(lpszMidFileName) == 0)
		return FALSE;

	FILE *pFile = _tfopen(lpszMidFileName,_T("wb"));
	if(pFile == NULL)
	{
		CString strError;
		strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszMidFileName,GetLastError());
		AfxMessageBox(strError);
		return FALSE;
	}

	int nSize = m_mapPcq.GetCount();

	assert(nSize == m_agProvinces.GetSize());

	char szTmp[CITY_LEN] = {0};

	for(int i = 0; i< nSize ; i++)
	{
		CString strProvince = m_agProvinces[i];
		PROVINCE_PTR pProv = NULL;

		if(!m_mapPcq.Lookup(strProvince,pProv))
		{
			fclose(pFile);
			return FALSE;
		}

		//如果为直辖市
		if(strProvince[0] == '#')
		{
			//先写城市，长度 CITY_LEN
#ifdef _UNICODE
			memset(szTmp,0,sizeof(szTmp));
			WideCharToMultiByte(CP_ACP,0,pProv->agCityQuhao[0]->szCity,_tcslen(pProv->agCityQuhao[0]->szCity),
				                szTmp,CITY_LEN,NULL,NULL);
			fwrite(szTmp,CITY_LEN,1,pFile);
#else
			fwrite(pProv->agCityQuhao[0]->szCity,CITY_LEN,1,pFile);
#endif
			//再写类别，类别为1
			WORD wType = 1; 
			fwrite(&wType,sizeof(WORD),1,pFile);
			//最后写区号
			WORD wQuhao = (WORD)pProv->agCityQuhao[0]->uQuhao;
			fwrite(&wQuhao,sizeof(WORD),1,pFile);
		}
		else//一般的省市
		{
			//先写省份信息，省份名长度 CITY_LEN
#ifdef _UNICODE
			memset(szTmp,0,sizeof(szTmp));
			WideCharToMultiByte(CP_ACP,0,pProv->szProvince,_tcslen(pProv->szProvince),
				                szTmp,CITY_LEN,NULL,NULL);
			fwrite(szTmp,CITY_LEN,1,pFile);
#else
			fwrite(pProv->szProvince,CITY_LEN,1,pFile);
#endif
			//再写类别，类别为省份所包含的城市个数
			WORD wType = (WORD)pProv->agCityQuhao.GetSize(); 
			fwrite(&wType,sizeof(WORD),1,pFile);
			//最后写区号，区号为 0
			WORD wQuhao = 0;
			fwrite(&wQuhao,sizeof(WORD),1,pFile);

			//省份下的所有城市
			for(int j = 0; j< pProv->agCityQuhao.GetSize(); j++)
			{
				//先写城市，长度 CITY_LEN
#ifdef _UNICODE
				memset(szTmp,0,sizeof(szTmp));
				WideCharToMultiByte(CP_ACP,0,pProv->agCityQuhao[j]->szCity,_tcslen(pProv->agCityQuhao[j]->szCity),
				                szTmp,CITY_LEN,NULL,NULL);
				fwrite(szTmp,CITY_LEN,1,pFile);
#else
				fwrite(pProv->agCityQuhao[j]->szCity,CITY_LEN,1,pFile);
#endif
				//再写类别，类别为0
				wType = 0; 
				fwrite(&wType,sizeof(WORD),1,pFile);
				//最后写区号
				wQuhao = (WORD)pProv->agCityQuhao[j]->uQuhao;
				fwrite(&wQuhao,sizeof(WORD),1,pFile);
			}
		}
	}

	fclose(pFile);

	return TRUE;
}
BOOL CCityQuhaoListView::GenerateQuhaoCity(LPCTSTR lpszMidFileName)
{
	if(lpszMidFileName == NULL || _tcslen(lpszMidFileName) == 0)
		return FALSE;

	g_theApp.RemoveFileReadOnlyAttr(lpszMidFileName);

	FILE *pFile = _tfopen(lpszMidFileName,_T("wb"));
	if(pFile == NULL)
	{
		CString strError;
		strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszMidFileName,GetLastError());
		AfxMessageBox(strError);
		return FALSE;
	}

	std::vector<CITY_QUHAO_PTR> lstTmp(m_CityInfo);

	std::sort(lstTmp.begin(),lstTmp.end(),cmp_quhao_info);

	char szTmp[CITY_LEN] = {0};

	std::vector<CITY_QUHAO_PTR>::iterator it;
	for(it = lstTmp.begin(); it!=lstTmp.end();it++)
	{	
		//先写区号，为文本格式，长度为QUHAO_LEN
		memset(szTmp,0,sizeof(szTmp));
		sprintf(szTmp,"%d",(WORD)(*it)->uQuhao);
		fwrite(szTmp,QUHAO_LEN,1,pFile);

		memset(szTmp,0,sizeof(szTmp));
		//不是直辖市
		if(_tcslen((*it)->szProvince) != NULL && (*it)->szProvince[0] != '#')
		{
#ifdef _UNICODE
			WideCharToMultiByte(CP_ACP,0,(*it)->szProvince,_tcslen((*it)->szProvince),
				                szTmp,CITY_LEN,NULL,NULL);

#else
			_tcscpy(szTmp,(*it)->szProvince);
#endif
		}
		else//是直辖市
		{
			if(_tcslen((*it)->szCity) != NULL)
			{
#ifdef _UNICODE
				WideCharToMultiByte(CP_ACP,0,(*it)->szCity,_tcslen((*it)->szCity),
									szTmp,CITY_LEN,NULL,NULL);
#else
				_tcscpy(szTmp,(*it)->szCity);
#endif
			}	
			//先写城市，但是长度为 PROV_LEN
			fwrite(szTmp,PROV_LEN,1,pFile);
			//在写CITY_LEN的空内容
			memset(szTmp,0,sizeof(szTmp));
			fwrite(szTmp,CITY_LEN,1,pFile);
			//继续
			continue;
		}
		//不是直辖市，先写省份，长度为 PROV_LEN
		fwrite(szTmp,PROV_LEN,1,pFile);

		//再写城市，长度为 CITY_LEN
		memset(szTmp,0,sizeof(szTmp));
		if(_tcslen((*it)->szCity) != NULL)
		{
#ifdef _UNICODE
			WideCharToMultiByte(CP_ACP,0,(*it)->szCity,_tcslen((*it)->szCity),
				                szTmp,CITY_LEN,NULL,NULL);
#else
			_tcscpy(szTmp,(*it)->szCity);
#endif
		}						
		fwrite(szTmp,CITY_LEN,1,pFile);
	}	

	fclose(pFile);

	return TRUE;
}
BOOL CCityQuhaoListView::GenerateQuhao(LPCTSTR lpszMidFileName,LPCTSTR lpszCityMidFile)
{
	if(lpszMidFileName == NULL || _tcslen(lpszMidFileName) == 0)
		return FALSE;

	g_theApp.RemoveFileReadOnlyAttr(lpszMidFileName);

	if(lpszCityMidFile == NULL)
	{
		FILE *pFile = _tfopen(lpszMidFileName,_T("wb"));
		if(pFile == NULL)
		{
			CString strError;
			strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszMidFileName,GetLastError());
			AfxMessageBox(strError);
			return FALSE;
		}

		
		std::vector<CITY_QUHAO_PTR>::iterator it;
		for(it = m_CityInfo.begin(); it!=m_CityInfo.end();it++)
		{	
			WORD wQuhao = (WORD)(*it)->uQuhao;
			fwrite(&wQuhao,sizeof(WORD),1,pFile);
		}	

		fclose(pFile);	
		return TRUE;
	}

	return FALSE;
}

BOOL CCityQuhaoListView::GenerateCity(LPCTSTR lpszMidFileName)
{
	if(lpszMidFileName == NULL || _tcslen(lpszMidFileName) == 0)
		return FALSE;

	FILE *pFile = _tfopen(lpszMidFileName,_T("wb"));
	if(pFile == NULL)
	{
		CString strError;
		strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszMidFileName,GetLastError());
		AfxMessageBox(strError);
		return FALSE;
	}

	char szProvTmp[CITY_LEN] = {0};
	char szCityTmp[CITY_LEN] = {0};
	char szTmp[CITY_LEN] = {0};
	
	std::vector<CITY_QUHAO_PTR>::iterator it;
	for(it = m_CityInfo.begin(); it!=m_CityInfo.end();it++)
	{	
		    memset(szTmp,0,sizeof(szTmp));

			memset(szProvTmp,0,sizeof(szTmp));
			memset(szCityTmp,0,sizeof(szTmp));
			if(_tcslen((*it)->szProvince) != NULL  && (*it)->szProvince[0] != '#')
			{
#ifdef _UNICODE
				WideCharToMultiByte(CP_ACP,0,(*it)->szProvince,_tcslen((*it)->szProvince),
				                szProvTmp,CITY_LEN,NULL,NULL);
				strcat(szTmp,szProvTmp);
#else
				strcat(szTmp,(*it)->szProvince);
#endif
			}

			if(_tcslen((*it)->szCity) != NULL)
			{
#ifdef _UNICODE
				WideCharToMultiByte(CP_ACP,0,(*it)->szCity,_tcslen((*it)->szCity),
				                szCityTmp,CITY_LEN,NULL,NULL);
				strcat(szTmp,szCityTmp);
#else
				strcat(szTmp,(*it)->szCity);
#endif
			}
						
			fwrite(szTmp,CITY_LEN,1,pFile);
	}	

	fclose(pFile);			

	return TRUE;
}


void CCityQuhaoListView::OnGenCityQuhao() 
{
	// TODO: Add your command handler code here
	CDlgGenQuhao dlg;
	if(dlg.DoModal() == IDOK)
	{
		GenerateCityQuhao(dlg.m_strCityQuhaoFile);
		GenerateQuhaoCity(dlg.m_strQuhaoCityFile);
		GenerateCity(dlg.m_strCityOutFile);
		GenerateQuhao(dlg.m_strQuhaoFile,NULL);
		
		AfxMessageBox(_T("Complete!"));
	}
}

void CCityQuhaoListView::OnUpdateGenCityQuhao(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CListCtrl &lc = GetListCtrl();

	if(lc.GetItemCount() > 0)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

BOOL CCityQuhaoListView::ConvertCityQuhaoMid(LPCTSTR lpszMidFileName,LPCTSTR lpszTxtFileName)
{
	if(lpszMidFileName == NULL || _tcslen(lpszMidFileName) == 0)
		return FALSE;

	if(lpszTxtFileName == NULL || _tcslen(lpszTxtFileName) == 0)
		return FALSE;


	g_theApp.RemoveFileReadOnlyAttr(lpszTxtFileName);

	FILE *pFile = _tfopen(lpszMidFileName,_T("rb"));
	if(pFile ==  NULL)
	{
		CString strError;
		strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszMidFileName,GetLastError());
		AfxMessageBox(strError);
		return FALSE;
	}
	

	int nSize = 0;
	fseek(pFile,0,SEEK_END);
	nSize = ftell(pFile);
	fseek(pFile,0,SEEK_SET);

	if(nSize%34 != 0)
	{
		fclose(pFile);
		AfxMessageBox(_T("Format is not correct!"));
		return FALSE;
	}

	FILE *pOutFile = _tfopen(lpszTxtFileName,_T("wt"));
	if(pOutFile ==  NULL)
	{
		fclose(pFile);

		CString strError;
		strError.Format(_T("Open file [%s] fail. [ErrorCode:0x%08X]"),lpszTxtFileName,GetLastError());	
		AfxMessageBox(strError);
		return FALSE;
	}

	char szProv[PROV_LEN] = {0};

	BYTE szBuf[34] = {0};

	while(fread(szBuf,34,1,pFile))
	{
		WORD wType = *(WORD*)(szBuf+30);
		if(wType!= 0 && wType != 1)
		{
			szBuf[30] = '\0';
			strcpy(szProv,(char*)szBuf);
			memset(szBuf,0,sizeof(szBuf));
			continue;
		}
		else if(wType == 1)
		{
			szBuf[30] = '\0';
			fprintf(pOutFile,",%s,%d\n",szBuf,*(WORD*)(szBuf+32));
		}
		else
		{
			szBuf[30] = '\0';
			fprintf(pOutFile,"%s,%s,%d\n",szProv,szBuf,*(WORD*)(szBuf+32));
		}
		memset(szBuf,0,sizeof(szBuf));
	}

	fclose(pFile);
	fclose(pOutFile);

	return TRUE;

}

void CCityQuhaoListView::OnConvCityQuhaoMidToTxt() 
{
	// TODO: Add your command handler code here
	CDlgConvSetting dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(ConvertCityQuhaoMid(dlg.m_strQCMidFile,dlg.m_strQCTxtFile))
		{
			AfxMessageBox(_T("Convert Complete!"));
		}
		else
		{			
			AfxMessageBox(_T("Convert Failed!"));
		}
	}

}
