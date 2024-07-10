// SpreadSheet.cpp: implementation of the CSpreadSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpreadSheet.h"
#include <atlconv.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpreadSheet::CSpreadSheet()
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

CSpreadSheet::~CSpreadSheet()
{	
	if(m_bOpened)
	{
		Close();
	}

	CoUninitialize();
}

BOOL CSpreadSheet::Init(BOOL bReadOnce /*= FALSE*/)
{
	USES_CONVERSION;
	//创建 Excel 服务器(启动Excel) 	
	if (!m_app.CreateDispatch(_T("Excel.Application"),NULL)) 
	{ 
		DWORD dwErrorCode = ::GetLastError();
		LPVOID lpMsgBuf=NULL;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS|
			FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			dwErrorCode,
			0, // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);

		//_tprintf(_T("\nCreate Excel.Application failed,[ErrorCode:0x%X][Desc:%s].\n"),dwErrorCode,(LPCTSTR)lpMsgBuf);
		printf("\nCreate Excel.Application failed, [ErrorCode:0x%X], %s\n",dwErrorCode,W2A((LPCTSTR)lpMsgBuf));
		LocalFree( lpMsgBuf );	
		return FALSE; 
 	}
 	else
 	{
 		_tprintf(_T("\nCreate Excel.Application success.\n"));
 		m_app.SetVisible(FALSE);
 	}

	m_app.SetDisplayAlerts(FALSE);
	m_bReadOnce = bReadOnce;
	return TRUE;
}
BOOL CSpreadSheet::OpenExcel(LPCTSTR lpszExcelName,BOOL bRead /*= TRUE*/)
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
	m_bRead        = bRead;

    LPDISPATCH      lpdisp;
	
	COleVariant covOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
	COleVariant covFalse((short)FALSE);
	
	m_wbsWorkbooks.AttachDispatch(m_app.GetWorkbooks());

	if(m_bRead)
	{
		lpdisp=m_wbsWorkbooks.Open(lpszExcelName, covOpt,covOpt,covOpt,
			covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt); 
		if(lpdisp == NULL)
		{
			DWORD dwErrorCode = ::GetLastError();
			LPVOID lpMsgBuf=NULL;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS|
				FORMAT_MESSAGE_MAX_WIDTH_MASK,
				NULL,
				dwErrorCode,
				0, // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);
			
			//_tprintf(_T("\nOpen Excel [%s] failed, [ErrorCode:0x%X][Desc:%s].\n"),lpszExcelName,dwErrorCode,(LPCTSTR)lpMsgBuf);
			printf("\nOpen Excel [%s] failed, [ErrorCode:0x%X], %s\n",lpszExcelName,dwErrorCode,W2A((LPCTSTR)lpMsgBuf));
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

	if(m_bRead)
	{
		long lCount = m_wssWorksheets.GetCount();
		
		for(long i = 1; i<=lCount; i++)
		{
			lpdisp = m_wssWorksheets.GetItem(COleVariant(i));
			if(lpdisp==NULL)
			{
				continue;
			}
			_EXCEL::_Worksheet		sheet; 
			sheet.AttachDispatch(lpdisp);
			m_agSheetNames.Add(sheet.GetName());
			
			sheet.ReleaseDispatch();
		}
	}

	return  TRUE;

}

BOOL CSpreadSheet::OpenSheet(LPCTSTR lpszSheetName /*= NULL*/)
{
//	HRESULT hr = ::CoInitialize(NULL);
//	UNUSED_ALWAYS(hr);

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
		_EXCEL::Range rgUsedRange;
		_EXCEL::Range rgRange;
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

BOOL CSpreadSheet::CreateSheet(LPCTSTR lpszSheetName/*=NULL*/,int nIndex/*=0*/)
{
	if(nIndex == 0)
	{
		m_wsWorksheet.AttachDispatch(m_wssWorksheets.GetItem(_variant_t("Sheet1"))); 
		if(lpszSheetName)
		{
			m_wsWorksheet.SetName(lpszSheetName);
		}	
		//m_range.AttachDispatch(m_wsWorksheet.GetCells()); 
		m_lCurRow = 1;
	}
	else if(nIndex == 1)
	{
        if (m_wssWorksheets.GetCount() == 1)
        {
            COleVariant covOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
            m_wssWorksheets.Add(covOpt,_variant_t(m_wsWorksheet.m_lpDispatch),COleVariant((short)1),covOpt);
        }
		m_wsWorksheet2.AttachDispatch(m_wssWorksheets.GetItem(_variant_t("Sheet2"))); 
		if(lpszSheetName)
		{
			m_wsWorksheet2.SetName(lpszSheetName);
		}	
		//m_range2.AttachDispatch(m_wsWorksheet2.GetCells()); 
		m_lCurRow2 = 1;
	}


	return TRUE;
}

void CSpreadSheet::GetBounds(int &nCols,int &nRows) const
{
	nCols = m_nCols;
	nRows = m_nRows;
}

BOOL CSpreadSheet::ReadRow(int nRowIndex, CStringArray &agCells)
{
	agCells.RemoveAll();
	if(nRowIndex >= m_nRows)
	{
		return FALSE;
	}
	
	long index[2];
    VARIANT var;
	CString strCell;

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
BOOL CSpreadSheet::WriteRow(CStringArray &agCells,int nIndex/*=0*/)
{
	UINT i = 0; 
	UINT nNum = agCells.GetSize();
	if(nNum==0)
		return TRUE;

	CString strCell;
	long lCurRow = 0;
	if(nIndex == 0)
	{
		lCurRow = m_lCurRow;
		CString strCell1;
		CString strCell2;
		strCell1.Format(_T("A%d"),lCurRow);
		strCell2.Format(_T("%s%d"),GetColIndex(agCells.GetSize()-1),lCurRow);
		m_range.AttachDispatch(m_wsWorksheet.GetRange(COleVariant(strCell1),COleVariant(strCell2)));
	}
	else if(nIndex == 1)
	{
		lCurRow = m_lCurRow2;
		CString strCell1;
		CString strCell2;
		strCell1.Format(_T("A%d"),lCurRow);
		strCell2.Format(_T("%s%d"),GetColIndex(agCells.GetSize()-1),lCurRow);
		m_range.AttachDispatch(m_wsWorksheet2.GetRange(COleVariant(strCell1),COleVariant(strCell2)));
	}
	
	for(i=1;i<= nNum;i++)
	{
		strCell = agCells.GetAt(i-1);
		strCell.Insert(0,_T('\''));
		m_range.SetItem(COleVariant(long(1)),COleVariant(long(i)),COleVariant(strCell));
		//if(nIndex == 0)
		//	m_range.SetItem(COleVariant(long(m_lCurRow)),COleVariant(long(i)),COleVariant(strCell));
		//else if(nIndex == 1)
		//	m_range2.SetItem(COleVariant(long(m_lCurRow2)),COleVariant(long(i)),COleVariant(strCell));
	}
	m_range.ReleaseDispatch();
//	m_wbWorkbook.SaveCopyAs(COleVariant(m_strExcelFile)); 

	if(nIndex == 0)
		m_lCurRow++;
	else if(nIndex == 1)
		m_lCurRow2++;

	return TRUE;
}

void CSpreadSheet::Close()
{
	COleVariant covOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
	COleVariant covFalse((short)FALSE);

	if(!m_bRead)
	{
		m_wbWorkbook.SaveCopyAs(COleVariant(m_strExcelFile)); 
	}
	
	m_wbWorkbook.Close(covFalse,COleVariant(m_strExcelFile),covOpt); 
	m_wbsWorkbooks.Close(); 

	ClearSA();
	if(!m_bRead)
	{
		m_range.ReleaseDispatch();
		m_range2.ReleaseDispatch();
	}
	m_wsWorksheet2.ReleaseDispatch();
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

void CSpreadSheet::ClearSA()
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

int CSpreadSheet::GetSheetNames(CStringArray &agSheets)
{
	agSheets.RemoveAll();
	agSheets.Copy(m_agSheetNames);
	return agSheets.GetSize();
}

CString CSpreadSheet::GetColIndex(int nCol) //base 0
{
	// A->IV
	if(nCol > 230)
		return _T("");

	int n1 = nCol/26;
	int n2 = nCol%26;

	CString str=_T("");
	if(n1>0)
	{
		str.Format(_T("%c%c"),'A'+n1-1,'A'+n2);
	}
	else
	{
		str.Format(_T("%c"),'A'+n2);
	}

	return str;
	
}

CString CSpreadSheet::VariantToCString(VARIANT var)
{
	CString strValue = _T("");
	time_t cur_time;
	CTime time_value;
	COleCurrency var_currency;
	switch(var.vt)
	{
	case VT_EMPTY:
		strValue=_T("");
		break;
	case VT_UI1:
		strValue.Format(_T("%d"),var.bVal);
		break;
	case VT_I2:
		strValue.Format(_T("%d"),var.iVal);
		break;
	case VT_I4:
		strValue.Format(_T("%d"),var.lVal);
		break;
	case VT_R4:
		strValue.Format(_T("%f"),var.fltVal);
		break;
	case VT_R8:
		strValue.Format(_T("%f"),var.dblVal);
		break;
	case VT_CY:
		var_currency=var;
		strValue=var_currency.Format(0);
		break;
	case VT_BSTR:
		strValue.Format (_T("%s"),(CString)var.bstrVal);
		break;
	case VT_NULL:
		strValue=_T("");
		break;
	case VT_DATE:
		cur_time = (long)var.date;
		time_value=cur_time;
		strValue=time_value.Format(_T("%A,%B%d,%Y"));
		break;
	case VT_BOOL:
		strValue.Format(_T("%d"),var.boolVal );
		break;
	default: 
		strValue=_T("");
		break;
	}
	return strValue;
}


