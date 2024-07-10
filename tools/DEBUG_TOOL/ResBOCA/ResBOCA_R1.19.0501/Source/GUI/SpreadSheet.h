// SpreadSheet.h: interface for the CSpreadSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPREADSHEET_H__376EA0AC_B05F_4CA7_BD22_FAFB72A38832__INCLUDED_)
#define AFX_SPREADSHEET_H__376EA0AC_B05F_4CA7_BD22_FAFB72A38832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push,3)
#include "comdef.h" 
#include "excel9.h"
#include "atlconv.h"
#pragma warning(pop)

#define EXCEL_SA_MAX_SIZE 2000
#define EXCEL_SA_COUNT    33    //(65536+1999)/2000

class CSpreadSheet  
{
public:
	CSpreadSheet();
	virtual ~CSpreadSheet();

	virtual BOOL Init(BOOL bReadOnce = FALSE);
	virtual BOOL OpenExcel(LPCTSTR lpszExcelName,BOOL bRead = TRUE);
	virtual BOOL OpenSheet(LPCTSTR lpszSheetName=NULL);
	virtual BOOL CreateSheet(LPCTSTR lpszSheetName=NULL,int nIndex=0);

	virtual void GetBounds(int &nCols,int &nRows) const;
	//RowIndex zero-based
	virtual BOOL ReadRow(int nRowIndex, CStringArray &agCells);
	virtual void Close();
	virtual int  GetSheetNames(CStringArray &agSheets);
	virtual BOOL WriteRow(CStringArray &agCells,int nIndex=0);

protected:
	CString VariantToCString(VARIANT var);
	void ClearSA();
	CString GetColIndex(int nCol);
private:
	_EXCEL::_Application	m_app; 
	_EXCEL::Workbooks		m_wbsWorkbooks; 
	_EXCEL::_Workbook		m_wbWorkbook; 
	_EXCEL::Worksheets		m_wssWorksheets; 
	_EXCEL::_Worksheet		m_wsWorksheet; 
	_EXCEL::Range           m_range;

	_EXCEL::_Worksheet		m_wsWorksheet2; 
	_EXCEL::Range           m_range2;

	COleSafeArray           m_sa[EXCEL_SA_COUNT];
	COleSafeArray           m_saAll;

	int                     m_nCols;
	int                     m_nRows;

	CString                 m_strExcelFile;

	BOOL                    m_bOpened;

	CStringArray            m_agSheetNames;
	
	BOOL                    m_bReadOnce;

	BOOL                    m_bRead;
	long                    m_lCurRow;
	long                    m_lCurRow2;
};

#endif // !defined(AFX_SPREADSHEET_H__376EA0AC_B05F_4CA7_BD22_FAFB72A38832__INCLUDED_)
