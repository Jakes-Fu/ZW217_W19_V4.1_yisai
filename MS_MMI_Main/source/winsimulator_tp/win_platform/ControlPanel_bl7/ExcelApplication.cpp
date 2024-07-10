/***************************************************************************************
** FileName    :  ExcelApplication.cpp                                                **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  08/02/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  implementation of the CExcelApplication class.                      **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  08/02/2010     Mingxuan.Zhu         Created                                       **
**  16/09/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "stdafx.h"
#include "ExcelApplication.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
CExcelApplication::CExcelApplication()
{
	m_sSheetName = "";
	m_sSheetRange = "";
}

//
CExcelApplication::~CExcelApplication()
{
}

/**************************************************************************************/
// Description      : Create Excel2003 Server(start Excel2003).
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::CreateExcelObject()
{
	// Create Excel2003 Server
	if (!m_pExcelApp.CreateDispatch("Excel.Application",NULL)) 
	{ 
        AfxMessageBox("Fail in Creating Excel Server!"); 
		exit(1); 
	}
}

/**************************************************************************************/
// Description      : release all excel objects and close excel progress.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::ReleaseExcelObjects()
{
	//release objects in excel
	m_cellBgColor.ReleaseDispatch();
    m_pSheetRange.ReleaseDispatch(); 
	m_pWorksheet.ReleaseDispatch(); 
	m_pWorksheets.ReleaseDispatch(); 
	m_pWorkbook.ReleaseDispatch(); 
	m_pWorkbooks.ReleaseDispatch();
	
	//close excel progress
    m_pExcelApp.Quit();
	m_pExcelApp.ReleaseDispatch();
}

/**************************************************************************************/
// Description      : create the object of the excel file.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::CreateNewWorkbook()
{
    COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
    
    m_pWorkbooks.AttachDispatch(m_pExcelApp.Workbooks().pdispVal,true);
    //create the object of the excel file
    m_pWorkbook.AttachDispatch(m_pWorkbooks.Add(covOptional).pdispVal);
}

/**************************************************************************************/
// Description      : get the object of the selected excel file.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString filePath  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::GetSelectedWorkbook(CString filePath)
{
    m_pWorkbooks.AttachDispatch(m_pExcelApp.Workbooks().pdispVal,true);
    //get the object of the selected excel file
    m_pWorkbook.AttachDispatch(m_pWorkbooks.Add(_variant_t(filePath)).pdispVal);
}

/**************************************************************************************/
// Description      : get the object of all the worksheets in the selected excel file.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::GetWorksheets()
{
    //get the selected excel file's worksheets
	m_pWorksheets.AttachDispatch(m_pWorkbook.Worksheets().pdispVal,true);
}

/**************************************************************************************/
// Description      : get the object of the selected sheet.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString sheetName  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
short CExcelApplication::GetSelectedSheet(CString sheetName)
{
    short sheetExistSwitch = 0;
       
    for(short iSheet=1;iSheet<=GetSheetsNumber();iSheet++)
    {
        m_pWorksheet.AttachDispatch(m_pWorksheets.Item(_variant_t((short)iSheet)).pdispVal,true);
        m_sSheetName = GetSheetName();
        if(m_sSheetName.CompareNoCase(sheetName)==0)
        {
            sheetExistSwitch = 1;
            break;
        }
    }

    if(sheetExistSwitch==1)  // the switch of the sheet named sheetName
    {
        m_pWorksheet.AttachDispatch(m_pWorksheets.Item(_variant_t((CString)sheetName)).pdispVal,true);
    }
    else
    {   // the switch of the default sheet
        m_pWorksheet.AttachDispatch(m_pWorksheets.Item(_variant_t((short)1)).pdispVal,true);
    }

    return sheetExistSwitch;
}

/**************************************************************************************/
// Description      : set the name of the sheet by sheet index.
//--------------------------------------------------------------------------------------
// Input Parameter  : int nSheetIndex  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::SetSheetName( short nSheetIndex, 
                                      CString sSheetName 
                                    )
{
    if(nSheetIndex < GetSheetsNumber())
    {
        m_pWorksheet.AttachDispatch(m_pWorksheets.Item(_variant_t(nSheetIndex)).pdispVal,true);
        m_pWorksheet.SetName(_variant_t(sSheetName));
    }
    else
    {
        AfxMessageBox("The sheet index exceeds the number in the current workbook!"); 
        return;
    }
}

/**************************************************************************************/
// Description      : get the object of next sheet from the selected sheet.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::GetNextSheet()
{
	m_pWorksheet = m_pWorksheet.GetNext().pdispVal;
}

/**************************************************************************************/
// Description      : get the total number of the worksheet in the selected excel file.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
int CExcelApplication::GetSheetsNumber()
{
	m_nSheetNumber = m_pWorksheets.GetCount().iVal;
	return m_nSheetNumber;
}

/**************************************************************************************/
// Description      : get the name of the selected worksheet.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CExcelApplication::GetSheetName()
{
	m_sSheetName = m_pWorksheet.GetName().bstrVal;
	return m_sSheetName;
}

/**************************************************************************************/
// Description      : get the object of the selected range by row and col number.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1.long row
//                    2.long col  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::GetRange(long row,long col)   //IN Param
{
    m_pSheetRange.AttachDispatch(m_pWorksheet.GetCells().pdispVal,true);
	m_pSheetRange.AttachDispatch(m_pSheetRange.Item(_variant_t((long)row),_variant_t((long)col)).pdispVal);
}

/**************************************************************************************/
// Description      : set the format of the range.
//--------------------------------------------------------------------------------------
// Input Parameter  : RANGE_FORMAT_E formatType  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::SetRangeFormat(RANGE_FORMAT_E eFormatType,long lNumParam)
{
    switch (eFormatType)
    {
    case WRAP_TEXT:
        {
            m_pSheetRange.SetWrapText(_variant_t((long)lNumParam));
        }
        break;
    default:
        break;
    }
}

/**************************************************************************************/
// Description      : get the value of the selected range by row and col number.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1.long row
//                    2.long col  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CExcelApplication::GetRangeValue(long row,long col)  //IN Param
{
	m_pSheetRange.AttachDispatch(m_pWorksheet.GetCells().pdispVal,true);
	m_pSheetRange.AttachDispatch(m_pSheetRange.Item(_variant_t((long)row),_variant_t((long)col)).pdispVal);
    m_sSheetRange = m_pSheetRange.GetText().bstrVal;
	return m_sSheetRange;
}

/**************************************************************************************/
// Description      : set the value in the selected range.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString rgValue  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::SetRangeValue(CString rgValue)  //IN Param
{
	m_pSheetRange.SetValue(_variant_t((CString)rgValue));
}

/**************************************************************************************/
// Description      : set the background color of the selected range.
//--------------------------------------------------------------------------------------
// Input Parameter  : short bgColor  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::SetRangeBgColor(short bgColor)  //IN Param
{
    m_cellBgColor.AttachDispatch(m_pSheetRange.GetInterior().pdispVal);
    m_cellBgColor.SetColorIndex(_variant_t((short)bgColor));
}

/**************************************************************************************/
// Description      : Save excel file by the inputted file path.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString filePath  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::SaveExcelFile(CString filePath)  //IN Param
{
	m_pWorkbook.SaveAs(_variant_t((CString)filePath),vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing);
}

/**************************************************************************************/
// Description      : Save excel file by original name.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CExcelApplication::SaveExcelFile()
{
    m_pWorkbook.Save();
}