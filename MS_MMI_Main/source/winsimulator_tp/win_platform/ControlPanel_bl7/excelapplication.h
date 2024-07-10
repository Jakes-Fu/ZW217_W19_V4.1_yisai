/***************************************************************************************
** FileName    :  ExcelApplication.h                                                  **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  24/02/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  interface for the CExcelApplication class.                          **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  08/02/2010     Mingxuan.Zhu         Created                                       **
**  16/09/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/

//excel2003 read/write header file
#include "xl5chs32.h"
#include "comdef.h"// _variant_t require
#include "windows.h"

#if !defined(AFX_EXCELAPPLICATION_H__A6B683F8_E6A1_4B01_9FD4_E6AE53DD9F63__INCLUDED_)
#define AFX_EXCELAPPLICATION_H__A6B683F8_E6A1_4B01_9FD4_E6AE53DD9F63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum
{
    WRAP_TEXT,
    ROW_HEIGHT,
    COL_WIDTH,
    VERTICAL_ALIGNMENT
}RANGE_FORMAT_E;

class CExcelApplication  
{
private:
    //excel2003 objects' definition
    Application m_pExcelApp;
    Workbooks m_pWorkbooks;
    Workbook m_pWorkbook;
    Worksheets m_pWorksheets;
    Worksheet m_pWorksheet;
    Range m_pSheetRange;
    Interior m_cellBgColor;
    
    //sheet and range's variant
    CString m_sSheetName;
    CString m_sSheetRange;
    int m_nSheetNumber;
    
public:
    CExcelApplication();
    virtual ~CExcelApplication();
    void CreateExcelObject();              // Create Excel2003 Server(start Excel2003).
    void ReleaseExcelObjects();            // release all excel objects and close excel progress.
    
    void CreateNewWorkbook();                    // create the object of the excel file.
    void GetSelectedWorkbook(CString filePath);  // get the object of the selected excel file.
    void GetWorksheets();                        // get the object of all the worksheets in the selected excel file.
    short GetSelectedSheet(CString sheetName);   // get the object of the selected sheet.
    void GetNextSheet();                         // get the object of next sheet from the selected sheet.
    int GetSheetsNumber();                       // get the total number of the worksheet in the selected excel file.
    CString GetSheetName();                      // get the name of the selected worksheet.
    void SetSheetName( short nSheetIndex, 
                       CString sSheetName);      // set the name of the sheet by sheet index.
    void GetRange(long row,long col);            // get the object of the selected range by row and col number.
    void SetRangeFormat( RANGE_FORMAT_E eFormatType,
                         long lNumParam);        // set the format of the range.
    CString GetRangeValue( long row,
                           long col );           // get the value of the selected range by row and col number.
    void SetRangeValue(CString rgValue);         // set the value in the selected range.
    void SaveExcelFile(CString filePath);        // Save excel file by the inputted file path.
    void SaveExcelFile();                        // Save excel file by original name.
    void SetRangeBgColor(short bgColor);         // set the background color of the selected range.
};

#endif // !defined(AFX_EXCELAPPLICATION_H__A6B683F8_E6A1_4B01_9FD4_E6AE53DD9F63__INCLUDED_)