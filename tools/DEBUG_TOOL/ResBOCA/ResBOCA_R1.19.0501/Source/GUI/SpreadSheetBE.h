// SpreadSheetBE.h: interface for the CSpreadSheetBE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPREADSHEETBE_H__3A0F7B85_5AD8_4450_8E20_0ECEB4E6E3CE__INCLUDED_)
#define AFX_SPREADSHEETBE_H__3A0F7B85_5AD8_4450_8E20_0ECEB4E6E3CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SpreadSheet.h"
#include "BasicExcelVC6.h"
using namespace YExcel;

class CSpreadSheetBE : public CSpreadSheet  
{
public:
	CSpreadSheetBE();
	virtual ~CSpreadSheetBE();
	
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
	CString GetSheetName(int iIndex);

private:
	BasicExcel          	m_app; 
	BasicExcelWorksheet    *m_pCurSheet;
	BasicExcelWorksheet    *m_pCurSheet2;
	
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

#endif // !defined(AFX_SPREADSHEETBE_H__3A0F7B85_5AD8_4450_8E20_0ECEB4E6E3CE__INCLUDED_)
