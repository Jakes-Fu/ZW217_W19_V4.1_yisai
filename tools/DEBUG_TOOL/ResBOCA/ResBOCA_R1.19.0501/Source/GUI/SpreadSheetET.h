// SpreadSheetET.h: interface for the CSpreadSheetET class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPREADSHEETET_H__3F83A2C1_3B58_49EA_9C74_5F863A4EFD58__INCLUDED_)
#define AFX_SPREADSHEETET_H__3F83A2C1_3B58_49EA_9C74_5F863A4EFD58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SpreadSheet.h"

#pragma warning(push,3)
#include "comdef.h" 
#include "etapp.h"
#include "atlconv.h"
#pragma warning(pop)


class CSpreadSheetET : public CSpreadSheet  
{
public:
	CSpreadSheetET();
	virtual ~CSpreadSheetET();
	virtual BOOL Init(BOOL bReadOnce = FALSE);
	virtual BOOL OpenExcel(LPCTSTR lpszExcelName,BOOL bRead = TRUE);
	virtual BOOL OpenSheet(LPCTSTR lpszSheetName=NULL);
	virtual BOOL CreateSheet(LPCTSTR lpszSheetName=NULL,int nIndex = 0);
	virtual void GetBounds(int &nCols,int &nRows) const;

	//RowIndex zero-based
    virtual BOOL ReadRow(int nRowIndex, CStringArray &agCells);
	virtual void Close();
	virtual int GetSheetNames(CStringArray &agSheets);	
	virtual BOOL WriteRow(CStringArray &agCells,int nIndex=0);

protected:
	void ClearSA();

private:
	_Application	m_app; 
	Workbooks		m_wbsWorkbooks; 
	_Workbook		m_wbWorkbook; 
	Worksheets		m_wssWorksheets; 
	_Worksheet		m_wsWorksheet; 
	Range           m_range;

	_Worksheet		m_wsWorksheet2; 
	Range           m_range2;

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

#endif // !defined(AFX_SPREADSHEETET_H__3F83A2C1_3B58_49EA_9C74_5F863A4EFD58__INCLUDED_)
