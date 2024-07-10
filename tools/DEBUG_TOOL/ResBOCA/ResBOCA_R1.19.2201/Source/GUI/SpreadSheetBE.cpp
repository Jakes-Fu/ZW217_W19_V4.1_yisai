// SpreadSheetBE.cpp: implementation of the CSpreadSheetBE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResOver.h"
#include "SpreadSheetBE.h"
#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpreadSheetBE::CSpreadSheetBE()
{
	m_pCurSheet = NULL;
	m_pCurSheet2= NULL;
	
	m_nCols = 0;
	m_nRows = 0;
	
	m_strExcelFile = _T("");
	
	m_bOpened = FALSE;
	
	m_agSheetNames.RemoveAll();
	
	m_bReadOnce = FALSE;
	
	m_bRead = TRUE;
	m_lCurRow = 0;
	m_lCurRow2= 0;
}

CSpreadSheetBE::~CSpreadSheetBE()
{
	m_app.Close();
}


BOOL CSpreadSheetBE::Init(BOOL bReadOnce /*= FALSE*/)
{
	m_bReadOnce = bReadOnce;
	return TRUE;
}
BOOL CSpreadSheetBE::OpenExcel(LPCTSTR lpszExcelName,BOOL bRead /*= TRUE*/)
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

	if(m_bRead)
	{
		if(!m_app.Load(T2A(lpszExcelName)))
		{
			printf("\nOpen Excel [%s] failed.\n",lpszExcelName);
			return FALSE;
		}		
	}
	else
	{
		m_app.New();
	}

	m_bOpened = TRUE;

	m_agSheetNames.RemoveAll();

	m_lCurRow = 1;

	if(m_bRead)
	{
		long lCount = m_app.GetTotalWorkSheets();		
		for(long i = 0; i<lCount; i++)
		{			
			m_agSheetNames.Add(GetSheetName(i));		
		}
	}

	return  TRUE;

}

BOOL CSpreadSheetBE::OpenSheet(LPCTSTR lpszSheetName /*= NULL*/)
{
	if(m_bOpened )
	{
		m_nCols = 0;
		m_nRows = 0;

		int nSheetIndex = 0;

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
				nSheetIndex=j;
				bMatch = TRUE;
				break;
			}
		}
		if(!bMatch)
		{
			return FALSE;
		}

		m_pCurSheet = m_app.GetWorksheet(nSheetIndex);

		if(m_pCurSheet == NULL)
		{
			return FALSE;
		}

		m_nRows = m_pCurSheet->GetTotalRows();
		m_nCols = m_pCurSheet->GetTotalCols();

		return TRUE;
	}

	return FALSE;
}

BOOL CSpreadSheetBE::CreateSheet(LPCTSTR lpszSheetName/*=NULL*/,int nIndex/*=0*/)
{
	USES_CONVERSION;

	if(nIndex > m_app.GetTotalWorkSheets())
		return FALSE;

	if(!m_app.RenameWorksheet(nIndex,lpszSheetName))
	{
		m_app.RenameWorksheet(nIndex,T2A(lpszSheetName));
	}		

	if(nIndex == 0)
	{
		m_pCurSheet = m_app.GetWorksheet(nIndex);
		m_lCurRow = 0;
	}
	else if(nIndex == 1)
	{
		m_pCurSheet2 = m_app.GetWorksheet(nIndex);
		m_lCurRow2 = 0;
	}

	return TRUE;
}

void CSpreadSheetBE::GetBounds(int &nCols,int &nRows) const
{
	nCols = m_nCols;
	nRows = m_nRows;
}

BOOL CSpreadSheetBE::ReadRow(int nRowIndex, CStringArray &agCells)
{
	USES_CONVERSION;
	agCells.RemoveAll();
	if(nRowIndex > m_nRows)
	{
		return FALSE;
	}
	
	CString strCell;

	for(int i=0; i<m_nCols; ++i)
	{
		BasicExcelCell* cell = m_pCurSheet->Cell(nRowIndex,i);
		strCell = _T("");
		switch (cell->Type())
		{
		case BasicExcelCell::UNDEFINED:		
			break;
			
		case BasicExcelCell::INT:
			strCell.Format(_T("%d"), cell->GetInteger());
			break;
			
		case BasicExcelCell::DOUBLE:	
			strCell.Format(_T("%.6lf"), cell->GetDouble());
			break;
			
		case BasicExcelCell::STRING:
			strCell = A2T((char *)cell->GetString());		
			break;
			
		case BasicExcelCell::WSTRING:
			strCell =  W2T((LPWSTR)cell->GetWString());
			break;
		}
		agCells.Add(strCell);
	}


	return TRUE;
}
BOOL CSpreadSheetBE::WriteRow(CStringArray &agCells,int nIndex/*=0*/)
{
	UINT i = 0; 
	UINT nNum = agCells.GetSize();
	if(nNum==0)
		return TRUE;

	CString strCell;
	

	BasicExcelWorksheet *pCurSheet = (nIndex == 0) ? m_pCurSheet : m_pCurSheet2;
	long lCurRow = (nIndex == 0) ? m_lCurRow : m_lCurRow2;
	
	for(i=0;i< nNum;i++)
	{
		strCell = agCells.GetAt(i);
		//strCell.Insert(0,_T('\''));
		pCurSheet->Cell(lCurRow,i)->SetWString(strCell.operator LPCTSTR());
	}

	if(nIndex == 0)
		m_lCurRow++;
	else if(nIndex == 1)
		m_lCurRow2++;

	return TRUE;
}

void CSpreadSheetBE::Close()
{
	USES_CONVERSION;
	if(!m_bRead)
	{
		m_app.SaveAs(T2A(m_strExcelFile.operator LPCTSTR()));
	}

	m_app.Close();
	
	m_bOpened = FALSE;
	m_nCols = 0;
	m_nRows = 0;	
	m_lCurRow = 0;
	m_lCurRow2 = 0;
}

CString CSpreadSheetBE::GetSheetName(int iIndex)
{
	USES_CONVERSION;
	wchar_t * pStr =  m_app.GetUnicodeSheetName(iIndex);

	if(pStr != NULL)
	{
		return W2T(pStr);
	}
	else
	{
		char *pStr2 = m_app.GetAnsiSheetName(iIndex);
		if(pStr2 == NULL)
			return _T("");

		return A2T(pStr2);
	}
}

int CSpreadSheetBE::GetSheetNames(CStringArray &agSheets)
{
	agSheets.RemoveAll();
	agSheets.Copy(m_agSheetNames);
	return agSheets.GetSize();
}