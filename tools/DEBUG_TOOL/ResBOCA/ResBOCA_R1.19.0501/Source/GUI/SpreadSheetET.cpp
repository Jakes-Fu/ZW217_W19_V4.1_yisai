// SpreadSheetET.cpp: implementation of the CSpreadSheetET class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpreadSheetET.h"
#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpreadSheetET::CSpreadSheetET()
{
	m_nCols = 0;
	m_nRows = 0;
	m_bOpened = FALSE;
	m_bReadOnce = FALSE;
	m_bRead = TRUE;
	m_lCurRow = 1;
	m_lCurRow2 = 1;
	CoInitialize(NULL);
}

CSpreadSheetET::~CSpreadSheetET()
{
	if(m_bOpened)
	{
		Close();
	}
	CoUninitialize();
}
BOOL CSpreadSheetET::Init(BOOL bReadOnce /*= FALSE*/)
{
	USES_CONVERSION;
	if (!m_app.CreateDispatch(_T("ET.Application"),NULL)) 
	{ 
		DWORD dwErrorCode = ::GetLastError();
		LPVOID lpMsgBuf=NULL;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErrorCode,
			0, // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);

		//_tprintf(_T("\nCreate ET.Application failed,[ErrorCode:0x%X][Desc:%s].\n"),dwErrorCode,(LPCTSTR)lpMsgBuf);
		printf("\nCreate ET.Application failed,[ErrorCode:0x%X],%s\n",dwErrorCode,W2A((LPCTSTR)lpMsgBuf));
		LocalFree( lpMsgBuf );	
		return FALSE; 
	}
	else
	{
		_tprintf(_T("\nCreate ET.Application success.\n"));
		m_app.SetVisible(FALSE);
	}

	m_app.SetDisplayAlerts(FALSE);
	m_bReadOnce = bReadOnce;
	return TRUE;
}
BOOL CSpreadSheetET::OpenExcel(LPCTSTR lpszExcelName,BOOL bRead /*= TRUE*/)
{
	if(m_bOpened)
	{
		Close();
	}

	if(lpszExcelName == NULL)
	{
		return FALSE;
	}

	USES_CONVERSION;

	m_strExcelFile = lpszExcelName;
	m_bRead = bRead;

    LPDISPATCH      lpdisp;	

	VARIANT varFileName;
	varFileName.vt = VT_BSTRT;
	varFileName.bstrVal = _bstr_t(lpszExcelName);
	
	COleVariant covOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
//	COleVariant covFalse((short)FALSE);
//	COleVariant covTrue((short)TRUE);

//	VARIANT varFalse;
//	varFalse.vt = VT_BOOL;
//	varFalse.boolVal = false;
	VARIANT varTrue;
	varTrue.vt = VT_BOOL;
	varTrue.boolVal = true;
	
	m_wbsWorkbooks.AttachDispatch(m_app.GetWorkbooks());	

	if(m_bRead)
	{
		lpdisp=m_wbsWorkbooks.Open(varFileName, //≥ı ºªØ. 
			covOpt,varTrue,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt); 
		
		if(lpdisp == NULL)
		{
			DWORD dwErrorCode = ::GetLastError();
			LPVOID lpMsgBuf=NULL;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dwErrorCode,
				0, // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);
			
			//_tprintf(_T("\nOpen Excel [%s] failed, [ErrorCode:0x%X][Desc:%s].\n"),lpszExcelName,dwErrorCode,(LPCTSTR)lpMsgBuf);
			printf("\nOpen Excel [%s] failed, [ErrorCode:0x%X][Desc:%s].\n",lpszExcelName,dwErrorCode,W2A((LPCTSTR)lpMsgBuf));
			LocalFree( lpMsgBuf );
			return FALSE;
		}
		
		m_wbWorkbook.AttachDispatch(lpdisp);
	}
	else
	{
		m_wbWorkbook.AttachDispatch(m_wbsWorkbooks.Add(covOpt));
	}

	m_wssWorksheets.AttachDispatch(m_wbWorkbook.GetWorksheets(),true); 	

	m_bOpened = TRUE;

	m_agSheetNames.RemoveAll();
	
	m_lCurRow = 1;
	m_lCurRow2 = 1;
	
	if(m_bRead)
	{
		long lCount = m_wssWorksheets.GetCount();
		for(long i = 1; i<= lCount; i++)
		{
			lpdisp = m_wssWorksheets.GetItem(COleVariant(i));
			if(lpdisp==NULL)
			{
				continue;
			}
			_Worksheet		sheet; 
			sheet.AttachDispatch(lpdisp);
			m_agSheetNames.Add(sheet.GetName());
			sheet.ReleaseDispatch();
		}
	}

	return  TRUE;

}

BOOL CSpreadSheetET::OpenSheet(LPCTSTR lpszSheetName /*= NULL*/ )
{
	if(m_bOpened )
	{
		m_nCols = 0;
		m_nRows = 0;

		CString strSheetName;
		if(lpszSheetName == NULL)
		{
			strSheetName = _T("Sheet1");
		}
		else
		{
			strSheetName = lpszSheetName;
		}

		BOOL bMatch = FALSE;
		for(int j=0; j< m_agSheetNames.GetSize(); j++)
		{
			if(m_agSheetNames[j].CompareNoCase(strSheetName) == 0)
			{
				bMatch = TRUE;
				break;
			}
		}
		if(!bMatch)
		{
			return FALSE;
		}

		LPDISPATCH  lpdisp = NULL;
		Range rgUsedRange;
		Range rgRange;
		VARIANT ret;
		try{

			ClearSA();
			
			if(m_wsWorksheet.m_lpDispatch != NULL)
			{
				m_wsWorksheet.ReleaseDispatch();
			}

			if(lpszSheetName == NULL)
			{
				lpdisp = m_wssWorksheets.GetItem(_variant_t("Sheet1")); 
			}
			else
			{
				lpdisp = m_wssWorksheets.GetItem(_variant_t(lpszSheetName));
			}

			if(lpdisp == NULL)
			{	
				return FALSE;
			}

			m_wsWorksheet.AttachDispatch(lpdisp);			
			rgUsedRange.AttachDispatch(m_wsWorksheet.GetUsedRange()); 			


			if(m_bReadOnce)
			{
				ret = rgUsedRange.GetValue2();
				if(ret.vt == VT_EMPTY)
				{
					m_wsWorksheet.ReleaseDispatch();
					return TRUE;
				}

				m_saAll.Detach();
				m_saAll.Attach(ret);

				long lNumRows;
				long lNumCols;

				m_saAll.GetUBound(1,&lNumRows);
				m_saAll.GetUBound(2,&lNumCols);

				m_nCols = lNumCols;
				m_nRows = lNumRows;

				rgUsedRange.ReleaseDispatch();
			}
			else
			{
				rgRange.AttachDispatch(rgUsedRange.GetColumns());
				m_nCols = rgRange.GetCount();
				rgRange.ReleaseDispatch();
				
				rgRange.AttachDispatch(rgUsedRange.GetRows());
				m_nRows = rgRange.GetCount();
				rgRange.ReleaseDispatch();
				
				rgUsedRange.ReleaseDispatch();
				
				int nSize = (m_nRows + EXCEL_SA_MAX_SIZE -1) / EXCEL_SA_MAX_SIZE;
				
				if(nSize > EXCEL_SA_COUNT)
				{
					m_wsWorksheet.ReleaseDispatch();
					return FALSE;
				}
				
				CString cell1;
				CString cell2;
				int nDeltSize = 0;
				
				for(int i= 0; i< nSize; i++)
				{
					cell1.Format(_T("A%d"),i*EXCEL_SA_MAX_SIZE+1);
					if(i== (nSize-1))
					{
						cell2.Format(_T("IV%d"),i*EXCEL_SA_MAX_SIZE + (m_nRows % EXCEL_SA_MAX_SIZE) );
					}
					else
					{
						cell2.Format(_T("IV%d"),(i+1)*EXCEL_SA_MAX_SIZE);
					}
					
					rgRange.AttachDispatch(m_wsWorksheet.GetRange(COleVariant(cell1),COleVariant(cell2)));
					ret = rgRange.GetValue2();
					if(ret.vt == VT_EMPTY)
					{
						nDeltSize += (i== (nSize-1))?(m_nRows % EXCEL_SA_MAX_SIZE):EXCEL_SA_MAX_SIZE;
						rgRange.Clear();
						rgRange.ReleaseDispatch();
						continue;
					}
					m_sa[i].Attach(ret);
					
					rgRange.Clear();
					rgRange.ReleaseDispatch();			
				}	

				m_nRows -= nDeltSize;
			}

			m_wsWorksheet.ReleaseDispatch();
			
		}catch (COleException &e) {
			
			ClearSA(); 			
			rgUsedRange.ReleaseDispatch();
			rgRange.ReleaseDispatch();	
			m_wsWorksheet.ReleaseDispatch();
			
			_TCHAR szErr[_MAX_PATH+1] = {0};
			e.GetErrorMessage(szErr,_MAX_PATH);
			_tprintf(_T("\nOpenSheet (%s) failed,[ERROR: %s].\n"),m_strExcelFile.operator LPCTSTR(),szErr);	
			return FALSE;
		}

		return TRUE;

	}

	return FALSE;
}


BOOL CSpreadSheetET::CreateSheet(LPCTSTR lpszSheetName/*=NULL*/,int nIndex /*= 0*/)
{
	if(nIndex == 0)
	{
		m_wsWorksheet.AttachDispatch(m_wssWorksheets.GetItem(_variant_t("Sheet1"))); 
		if(lpszSheetName)
		{
			m_wsWorksheet.SetName(lpszSheetName);
		}	
		m_range.AttachDispatch(m_wsWorksheet.GetCells()); 
		m_lCurRow = 1;
	}
	else if(nIndex == 1)
	{
		m_wsWorksheet2.AttachDispatch(m_wssWorksheets.GetItem(_variant_t("Sheet2"))); 
		if(lpszSheetName)
		{
			m_wsWorksheet2.SetName(lpszSheetName);
		}	
		m_range2.AttachDispatch(m_wsWorksheet2.GetCells()); 
		m_lCurRow2 = 1;
	}
	
	return TRUE;
}

void CSpreadSheetET::GetBounds(int &nCols,int &nRows) const
{
	nCols = m_nCols;
	nRows = m_nRows;
}

BOOL CSpreadSheetET::ReadRow(int nRowIndex, CStringArray &agCells)
{
	agCells.RemoveAll();
	if(nRowIndex >= m_nRows)
	{
		return FALSE;
	}
	
	long index[2];
    VARIANT var;
	CString strCell = _T("");
	if(m_bReadOnce)
	{
		for(int i = 0; i < m_nCols; i++)
		{
			index[0]=nRowIndex+1;
			index[1]=i+1;
			m_saAll.GetElement(index,&var);
			strCell= VariantToCString(var);
			agCells.Add(strCell);
		}
	}
	else
	{
		int nSaIndex = nRowIndex/EXCEL_SA_MAX_SIZE;
		int nRow =  nRowIndex%EXCEL_SA_MAX_SIZE;
		if(m_sa[nSaIndex].GetElemSize() == 0)
		{
			return FALSE;
		}
		
		long lNumRows;
		long lNumCols;
		m_sa[nSaIndex].GetUBound(1,&lNumRows);
		m_sa[nSaIndex].GetUBound(2,&lNumCols);
		
		if(nRow >= lNumRows)
		{
			return FALSE;
		}

		for(int i = 0; i < m_nCols; i++)
		{
			index[0]=nRow+1;
			index[1]=i+1;
			m_sa[nSaIndex].GetElement(index,&var);
			strCell= VariantToCString(var);
			agCells.Add(strCell);
		}
	}


	return TRUE;
}

BOOL CSpreadSheetET::WriteRow(CStringArray &agCells,int nIndex/*=0*/)
{
	UINT i = 0; 
	UINT nNum = agCells.GetSize();
	CString strCell;
	for(i=1;i<= nNum;i++)
	{
		strCell = agCells.GetAt(i-1);
		strCell.Insert(0,_T('\''));
		if(nIndex == 0)
			m_range.SetItem(COleVariant(long(m_lCurRow)),COleVariant(long(i)),COleVariant(strCell));
		else if(nIndex == 1)
			m_range2.SetItem(COleVariant(long(m_lCurRow2)),COleVariant(long(i)),COleVariant(strCell));
	}
	if(nIndex == 0)
		m_lCurRow++;
	else if(nIndex == 1)
		m_lCurRow2++;
	
	return TRUE;
}


void CSpreadSheetET::Close()
{
	COleVariant covOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
//	COleVariant covFalse((short)FALSE);
//	COleVariant covTrue((short)TRUE);

	VARIANT varFalse;
	varFalse.vt = VT_BOOL;
	varFalse.boolVal = false;
//	VARIANT varTrue;
//	varTrue.vt = VT_BOOL;
//	varTrue.boolVal = true;

	if(!m_bRead)
	{
		VARIANT varFileName;
		varFileName.vt = VT_BSTRT;
		varFileName.bstrVal = _bstr_t(m_strExcelFile);
		m_wbWorkbook.SaveAs(varFileName,covOpt,covOpt,covOpt,covOpt,covOpt,0,covOpt,covOpt,covOpt,covOpt);
	}

	m_wbWorkbook.Close(varFalse,covOpt,varFalse); 
	m_wbsWorkbooks.Close(varFalse); 
	
	ClearSA();
	if(!m_bRead)
	{
		m_range.ReleaseDispatch();
		m_range2.ReleaseDispatch();
	}
	m_wsWorksheet.ReleaseDispatch();
	m_wssWorksheets.ReleaseDispatch();
	m_wbWorkbook.ReleaseDispatch();
	m_wbsWorkbooks.ReleaseDispatch();

	m_app.Quit(); 
	m_app.ReleaseDispatch();

	m_bOpened = FALSE;
	m_nCols = 0;
	m_nRows = 0;
	m_lCurRow = 1;
	m_lCurRow2 = 1;
}

void CSpreadSheetET::ClearSA()
{
	if(m_bReadOnce)
	{
		m_saAll.Clear();
		m_saAll.Detach();
	}
	else
	{
		for(int i = 0; i< EXCEL_SA_COUNT; i++)
		{
			m_sa[i].Clear();
			m_sa[i].Detach();
		}
	}
}

int CSpreadSheetET::GetSheetNames(CStringArray &agSheets)
{
	agSheets.RemoveAll();
	agSheets.Copy(m_agSheetNames);
	return agSheets.GetSize();
}
