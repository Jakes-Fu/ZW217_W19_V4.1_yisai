// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "etapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _Application properties

/////////////////////////////////////////////////////////////////////////////
// _Application operations

LPDISPATCH _Application::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetDescription()
{
	CString result;
	InvokeHelper(0xfffff06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetWorkbooks()
{
	LPDISPATCH result;
	InvokeHelper(0x111000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x111001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetActiveWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x111002, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetActiveWorkbook()
{
	LPDISPATCH result;
	InvokeHelper(0x111005, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetActiveSheet()
{
	LPDISPATCH result;
	InvokeHelper(0x111006, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Application::GetVisible()
{
	BOOL result;
	InvokeHelper(0x111003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetUserControl()
{
	BOOL result;
	InvokeHelper(0x111004, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetUserControl(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Application::GetNames()
{
	LPDISPATCH result;
	InvokeHelper(0x111007, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetRange(const VARIANT& Cell1, const VARIANT& Cell2)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x111008, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Cell1, &Cell2);
	return result;
}

LPDISPATCH _Application::GetSelection()
{
	LPDISPATCH result;
	InvokeHelper(0x111009, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetActiveCell()
{
	LPDISPATCH result;
	InvokeHelper(0x11100a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application::GetCustomListCount()
{
	long result;
	InvokeHelper(0x11100b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x11100c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetActivePrinter()
{
	CString result;
	InvokeHelper(0x11100d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetActivePrinter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11100d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Application::GetDefaultFilePath()
{
	CString result;
	InvokeHelper(0x11100f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetDefaultFilePath(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11100f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Application::GetShowStartupDialog()
{
	BOOL result;
	InvokeHelper(0x111010, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetShowStartupDialog(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetDisplayFormulaBar()
{
	BOOL result;
	InvokeHelper(0x111011, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetDisplayFormulaBar(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111011, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetDisplayStatusBar()
{
	BOOL result;
	InvokeHelper(0x111012, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetDisplayStatusBar(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetIteration()
{
	BOOL result;
	InvokeHelper(0x111014, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetIteration(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Application::GetMaxIterations()
{
	long result;
	InvokeHelper(0x111015, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetMaxIterations(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x111015, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

double _Application::GetMaxChange()
{
	double result;
	InvokeHelper(0x111016, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _Application::SetMaxChange(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x111016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _Application::GetFixedDecimal()
{
	BOOL result;
	InvokeHelper(0x111017, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetFixedDecimal(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111017, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Application::GetFixedDecimalPlaces()
{
	long result;
	InvokeHelper(0x111018, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetFixedDecimalPlaces(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x111018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Application::GetCopyObjectsWithCells()
{
	BOOL result;
	InvokeHelper(0x11101b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetCopyObjectsWithCells(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11101b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetMoveAfterReturn()
{
	BOOL result;
	InvokeHelper(0x111019, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetMoveAfterReturn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111019, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Application::GetMoveAfterReturnDirection()
{
	long result;
	InvokeHelper(0x11101a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetMoveAfterReturnDirection(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11101a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application::GetReferenceStyle()
{
	long result;
	InvokeHelper(0x11101c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetReferenceStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11101c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Application::GetUserName_()
{
	CString result;
	InvokeHelper(0x11101d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetUserName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11101d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Application::GetStandardFont()
{
	CString result;
	InvokeHelper(0x11101e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetStandardFont(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11101e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Application::GetStandardFontSize()
{
	long result;
	InvokeHelper(0x11101f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetStandardFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11101f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application::GetSheetsInNewWorkbook()
{
	long result;
	InvokeHelper(0x111020, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetSheetsInNewWorkbook(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x111020, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Application::GetShowChartTipNames()
{
	BOOL result;
	InvokeHelper(0x111021, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetShowChartTipNames(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111021, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetShowChartTipValues()
{
	BOOL result;
	InvokeHelper(0x111022, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetShowChartTipValues(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Application::GetCalculation()
{
	long result;
	InvokeHelper(0x111013, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetCalculation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x111013, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Application::GetDisplayRecentFiles()
{
	BOOL result;
	InvokeHelper(0x111024, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetDisplayRecentFiles(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111024, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Application::GetRecentFiles()
{
	LPDISPATCH result;
	InvokeHelper(0x111025, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetCOMAddIns()
{
	LPDISPATCH result;
	InvokeHelper(0x11102a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetDebugTools()
{
	LPDISPATCH result;
	InvokeHelper(0x11100e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetSheets()
{
	LPDISPATCH result;
	InvokeHelper(0x111032, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x111033, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetRows()
{
	LPDISPATCH result;
	InvokeHelper(0x111034, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0x111035, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Application::Quit()
{
	InvokeHelper(0x112000, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Application::Terminate(BOOL bForce)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x112001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bForce);
}

void _Application::Undo()
{
	InvokeHelper(0x112003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Application::Redo()
{
	InvokeHelper(0x112002, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Application::AddCustomList(const VARIANT& ListArray, const VARIANT& ByRow)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x112004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &ListArray, &ByRow);
}

void _Application::DeleteCustomList(long ListNum)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x112005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ListNum);
}

VARIANT _Application::GetCustomListContents(long ListNum)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x112006, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		ListNum);
	return result;
}

LPDISPATCH _Application::FindFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x112008, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::ReplaceFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x112009, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application::GetCustomListNum(const VARIANT& ListArray)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x112007, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&ListArray);
	return result;
}

double _Application::InchesToPoints(double inches)
{
	double result;
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x11200a, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		inches);
	return result;
}

long _Application::GetCutCopyMode()
{
	long result;
	InvokeHelper(0x111023, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetCutCopyMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x111023, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Application::SetDefaultChart(const VARIANT& FormatName)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x11200b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &FormatName);
}

void _Application::SetSaveAsFileType(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x111026, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT _Application::GetSaveAsFileType()
{
	VARIANT result;
	InvokeHelper(0x111026, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

BOOL _Application::GetEnableSpecialDiagonal()
{
	BOOL result;
	InvokeHelper(0x111027, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetEnableSpecialDiagonal(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111027, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetSaveBackupFileOnFirstSave()
{
	BOOL result;
	InvokeHelper(0x111030, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetSaveBackupFileOnFirstSave(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x111030, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _Application::GetDefaultWorkbookName()
{
	CString result;
	InvokeHelper(0x111028, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetDefaultWorkbookName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x111028, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Application::GetDefaultWorksheetName()
{
	CString result;
	InvokeHelper(0x111029, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetDefaultWorksheetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x111029, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Application::SetStatusBar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11102b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Application::GetStatusBar()
{
	CString result;
	InvokeHelper(0x11102b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetDisplayAlerts(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11102c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Application::GetDisplayAlerts()
{
	BOOL result;
	InvokeHelper(0x11102c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::OnKey(const VARIANT& Key1, const VARIANT& Key2, const VARIANT& Key3, const VARIANT& key4, const VARIANT& Procedure)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x11200c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key1, &Key2, &Key3, &key4, &Procedure);
}

VARIANT _Application::Run(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, 
		const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, 
		const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, 
		const VARIANT& Arg30)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT 
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x11200d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Macro, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
	return result;
}

CString _Application::GetVersion()
{
	CString result;
	InvokeHelper(0x11102d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetCaption()
{
	CString result;
	InvokeHelper(0x11102e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11102e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

VARIANT _Application::GetSaveAsFilename(const VARIANT& InitialFilename, const VARIANT& FileFilter, const VARIANT& FilterIndex, const VARIANT& Title, const VARIANT& ButtonText)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x11200e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&InitialFilename, &FileFilter, &FilterIndex, &Title, &ButtonText);
	return result;
}

long _Application::GetCursor()
{
	long result;
	InvokeHelper(0x11102f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetCursor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11102f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Application::GetName()
{
	CString result;
	InvokeHelper(0x111031, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetAutoRecover()
{
	LPDISPATCH result;
	InvokeHelper(0x111036, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetCells()
{
	LPDISPATCH result;
	InvokeHelper(0x111037, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetCustomDictionaries()
{
	LPDISPATCH result;
	InvokeHelper(0x111038, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetSpellingOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x111039, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application::GetWindowState()
{
	long result;
	InvokeHelper(0x11103a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetWindowState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11103a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Application::GetAddIns()
{
	LPDISPATCH result;
	InvokeHelper(0x11103b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetAdvApiRoot()
{
	LPDISPATCH result;
	InvokeHelper(0x11103c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Application::GetEnableAppWindow()
{
	BOOL result;
	InvokeHelper(0x11103d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetEnableAppWindow(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11103d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Application::GetTaskPanes()
{
	LPDISPATCH result;
	InvokeHelper(0x11103e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Application::Calculate()
{
	InvokeHelper(0x11200f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Application::GetScreenUpdating()
{
	BOOL result;
	InvokeHelper(0x11103f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Application::SetScreenUpdating(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11103f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Application::GetWorksheetFunction()
{
	LPDISPATCH result;
	InvokeHelper(0x111040, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetBuild()
{
	CString result;
	InvokeHelper(0x111041, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Workbooks properties

/////////////////////////////////////////////////////////////////////////////
// Workbooks operations

LPDISPATCH Workbooks::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Workbooks::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Workbooks::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Workbooks::GetDescription()
{
	CString result;
	InvokeHelper(0xfffff06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Workbooks::GetCount()
{
	long result;
	InvokeHelper(0x221000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Workbooks::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x221001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Workbooks::Get_Default(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Workbooks::Add(const VARIANT& Template)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x222000, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Template);
	return result;
}

void Workbooks::Close(const VARIANT& SaveChanges)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x222001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &SaveChanges);
}

LPDISPATCH Workbooks::Open(const VARIANT& Filename, const VARIANT& UpdateLinks, const VARIANT& ReadOnly, const VARIANT& Format, const VARIANT& password, const VARIANT& WriteResPassword, const VARIANT& IgnoreReadOnlyRecommended, const VARIANT& Origin, 
		const VARIANT& Delimiter, const VARIANT& Editable, const VARIANT& Notify, const VARIANT& Converter, const VARIANT& AddToMru)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x222002, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Filename, &UpdateLinks, &ReadOnly, &Format, &password, &WriteResPassword, &IgnoreReadOnlyRecommended, &Origin, &Delimiter, &Editable, &Notify, &Converter, &AddToMru);
	return result;
}

LPUNKNOWN Workbooks::Get_NewEnum()
{
	LPUNKNOWN result;
	InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPDISPATCH Workbooks::OpenText(LPCTSTR Filename, const VARIANT& Origin, const VARIANT& StartRow, const VARIANT& DataType, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, const VARIANT& Space, 
		const VARIANT& Other, const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& TextVisualLayout, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers, const VARIANT& Local)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x222003, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Filename, &Origin, &StartRow, &DataType, &ConsecutiveDelimiter, &Tab, &Semicolon, &Comma, &Space, &Other, &OtherChar, &FieldInfo, &TextVisualLayout, &DecimalSeparator, &ThousandsSeparator, &TrailingMinusNumbers, &Local);
	return result;
}

LPDISPATCH Workbooks::OpenDatabase(LPCTSTR Filename, const VARIANT& CommandText, const VARIANT& CommandType, const VARIANT& BackgroundQuery, const VARIANT& ImportDataAs)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x222004, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Filename, &CommandText, &CommandType, &BackgroundQuery, &ImportDataAs);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Workbook properties

/////////////////////////////////////////////////////////////////////////////
// _Workbook operations

LPDISPATCH _Workbook::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Workbook::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Workbook::GetDescription()
{
	CString result;
	InvokeHelper(0xfffff06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetContainer()
{
	LPDISPATCH result;
	InvokeHelper(0x311002, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x311003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x311004, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetActiveSheet()
{
	LPDISPATCH result;
	InvokeHelper(0x311006, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Workbook::GetFullName()
{
	CString result;
	InvokeHelper(0x311005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetStyles()
{
	LPDISPATCH result;
	InvokeHelper(0x311007, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetNames()
{
	LPDISPATCH result;
	InvokeHelper(0x311008, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Workbook::GetName()
{
	CString result;
	InvokeHelper(0x311009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

VARIANT _Workbook::GetColors(const VARIANT& Index)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x31100a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		&Index);
	return result;
}

void _Workbook::SetColors(const VARIANT& Index, const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x31100a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &Index, &newValue);
}

LPDISPATCH _Workbook::GetSheets()
{
	LPDISPATCH result;
	InvokeHelper(0x31100b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetExtraColors()
{
	LPDISPATCH result;
	InvokeHelper(0x31100c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Workbook::GetPath()
{
	CString result;
	InvokeHelper(0x31100d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Workbook::GetSaved()
{
	BOOL result;
	InvokeHelper(0x31100e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Workbook::SetSaved(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x31100e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void _Workbook::SetPassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x311010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Workbook::GetPasswordEncryptionProvider()
{
	CString result;
	InvokeHelper(0x311011, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Workbook::GetPasswordEncryptionAlgorithm()
{
	CString result;
	InvokeHelper(0x311012, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Workbook::GetPasswordEncryptionKeyLength()
{
	long result;
	InvokeHelper(0x311013, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _Workbook::GetPasswordEncryptionFileProperties()
{
	BOOL result;
	InvokeHelper(0x311014, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Workbook::GetHasPassword()
{
	BOOL result;
	InvokeHelper(0x31100f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Workbook::GetProtectStructure()
{
	BOOL result;
	InvokeHelper(0x311015, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Workbook::SetProtectStructure(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x311015, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Workbook::GetProtectWindows()
{
	BOOL result;
	InvokeHelper(0x311016, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Workbook::SetProtectWindows(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x311016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void _Workbook::NewWindow()
{
	InvokeHelper(0x312002, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Workbook::Close(const VARIANT& SaveChanges, const VARIANT& Filename, const VARIANT& RouteWorkbook)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x312000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &SaveChanges, &Filename, &RouteWorkbook);
}

void _Workbook::Activate()
{
	InvokeHelper(0x312003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Workbook::SaveAs(const VARIANT& Filename, const VARIANT& FileFormat, const VARIANT& password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, long AccessMode, const VARIANT& ConflictResolution, 
		const VARIANT& AddToMru, const VARIANT& TextCodepage, const VARIANT& TextVisualLayout)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x312001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Filename, &FileFormat, &password, &WriteResPassword, &ReadOnlyRecommended, &CreateBackup, AccessMode, &ConflictResolution, &AddToMru, &TextCodepage, &TextVisualLayout);
}

void _Workbook::PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x312004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName, ManualDuplexPrint, PrintZoomColumn, PrintZoomRow, PrintZoomPaperWidth, PrintZoomPaperHeight, FlipPrint, PaperTray, CutterLine, PaperOrder);
}

void _Workbook::DeleteNumberFormat(LPCTSTR NumberFormat)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x312005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NumberFormat);
}

void _Workbook::Protect(const VARIANT& password, const VARIANT& structure, const VARIANT& Window)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x312006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &password, &structure, &Window);
}

void _Workbook::Unprotect(const VARIANT& password)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x312007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &password);
}

void _Workbook::SetPasswordEncryptionOptions(LPCTSTR PasswordEncryptionProvider, LPCTSTR PasswordEncryptionAlgorithm, long PasswordEncryptionKeyLength, VARIANT* PasswordEncryptionFileProperties)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x312009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PasswordEncryptionProvider, PasswordEncryptionAlgorithm, PasswordEncryptionKeyLength, PasswordEncryptionFileProperties);
}

void _Workbook::SendMail(const VARIANT& Recipients, const VARIANT& Subject, const VARIANT& ReturnReceipt)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x31200a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Recipients, &Subject, &ReturnReceipt);
}

LPDISPATCH _Workbook::GetBuiltinDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x31200b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Workbook::GetCustomDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x31200c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Workbook::Save()
{
	InvokeHelper(0x312008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Workbook::SetWritePassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x311017, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _Workbook::GetSecurityPolicies()
{
	LPDISPATCH result;
	InvokeHelper(0x311018, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Workbook::GetUserMode()
{
	BOOL result;
	InvokeHelper(0x857, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Workbook::SetUserMode(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x857, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Workbook::GetTrackChanges()
{
	BOOL result;
	InvokeHelper(0x311019, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Workbook::SetTrackChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x311019, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Workbook::GetHighlightChangesOnScreen()
{
	BOOL result;
	InvokeHelper(0x31101a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Workbook::SetHighlightChangesOnScreen(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x31101a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Workbook::GetListChangesOnNewSheet()
{
	BOOL result;
	InvokeHelper(0x31101b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Workbook::SetListChangesOnNewSheet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x31101b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void _Workbook::HighlightChangesOptions(const VARIANT& When, const VARIANT& Who, const VARIANT& Where)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x31200d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &When, &Who, &Where);
}

LPDISPATCH _Workbook::GetChanges()
{
	LPDISPATCH result;
	InvokeHelper(0x31101c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Workbook::AcceptAllChanges()
{
	InvokeHelper(0x31200e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Workbook::RejectAllChanges()
{
	InvokeHelper(0x31200f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Worksheets properties

/////////////////////////////////////////////////////////////////////////////
// Worksheets operations

LPDISPATCH Worksheets::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Worksheets::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Worksheets::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Worksheets::GetDescription()
{
	CString result;
	InvokeHelper(0xfffff06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Worksheets::GetCount()
{
	long result;
	InvokeHelper(0x421001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Worksheets::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x421002, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Worksheets::Get_Default(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Worksheets::Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x422000, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Before, &After, &Count, &Type);
	return result;
}

LPUNKNOWN Worksheets::Get_NewEnum()
{
	LPUNKNOWN result;
	InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void Worksheets::PrintPreview(const VARIANT& EnableChanges)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51200c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &EnableChanges);
}


/////////////////////////////////////////////////////////////////////////////
// Range properties

/////////////////////////////////////////////////////////////////////////////
// Range operations

LPDISPATCH Range::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Range::GetDescription()
{
	CString result;
	InvokeHelper(0xfffff06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString Range::GetAddress(BOOL RowAbsolute, BOOL ColumnAbsolute, long ReferenceStyle, BOOL External, const VARIANT& RelativeTo)
{
	CString result;
	static BYTE parms[] =
		VTS_BOOL VTS_BOOL VTS_I4 VTS_BOOL VTS_VARIANT;
	InvokeHelper(0xc11001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		RowAbsolute, ColumnAbsolute, ReferenceStyle, External, &RelativeTo);
	return result;
}

VARIANT Range::Get_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		&RowIndex, &ColumnIndex);
	return result;
}

void Range::Set_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &RowIndex, &ColumnIndex, &newValue);
}

long Range::GetCount()
{
	long result;
	InvokeHelper(0xc11002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Range::GetFormula()
{
	CString result;
	InvokeHelper(0xc11008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Range::SetFormula(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc11008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString Range::GetFormulaR1C1()
{
	CString result;
	InvokeHelper(0xc1100a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Range::SetFormulaR1C1(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc1100a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

VARIANT Range::GetValue2()
{
	VARIANT result;
	InvokeHelper(0xc11007, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetValue2(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc11007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT Range::GetHasArray()
{
	VARIANT result;
	InvokeHelper(0xc11003, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

CString Range::GetFormulaArray()
{
	CString result;
	InvokeHelper(0xc1100b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Range::SetFormulaArray(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc1100b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString Range::GetText()
{
	CString result;
	InvokeHelper(0xc11005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

VARIANT Range::GetHidden()
{
	VARIANT result;
	InvokeHelper(0xc11004, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetHidden(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc11004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

double Range::GetRowHeight()
{
	double result;
	InvokeHelper(0xc1100c, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void Range::SetRowHeight(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xc1100c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double Range::GetColumnWidth()
{
	double result;
	InvokeHelper(0xc1100d, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void Range::SetColumnWidth(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xc1100d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Range::GetBorders()
{
	LPDISPATCH result;
	InvokeHelper(0xc1100f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(0xc11010, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetInterior()
{
	LPDISPATCH result;
	InvokeHelper(0xc11011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Range::GetNumberFormat()
{
	CString result;
	InvokeHelper(0xc11012, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Range::SetNumberFormat(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc11012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString Range::GetNumberFormatLocal()
{
	CString result;
	InvokeHelper(0xc11013, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Range::SetNumberFormatLocal(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc11013, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

VARIANT Range::GetAddIndent()
{
	VARIANT result;
	InvokeHelper(0xc11014, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetAddIndent(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc11014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long Range::GetIndentLevel()
{
	long result;
	InvokeHelper(0xc11015, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetIndentLevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc11015, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetHorizontalAlignment()
{
	long result;
	InvokeHelper(0xc11016, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetHorizontalAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc11016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetVerticalAlignment()
{
	long result;
	InvokeHelper(0xc11017, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetVerticalAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc11017, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT Range::GetOrientation()
{
	VARIANT result;
	InvokeHelper(0xc11018, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetOrientation(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc11018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long Range::GetReadingOrder()
{
	long result;
	InvokeHelper(0xc11019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetReadingOrder(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc11019, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT Range::GetShrinkToFit()
{
	VARIANT result;
	InvokeHelper(0xc1101a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetShrinkToFit(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1101a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT Range::GetWrapText()
{
	VARIANT result;
	InvokeHelper(0xc1101b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetWrapText(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1101b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT Range::GetLocked()
{
	VARIANT result;
	InvokeHelper(0xc1101c, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetLocked(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1101c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT Range::GetFormulaHidden()
{
	VARIANT result;
	InvokeHelper(0xc1101d, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetFormulaHidden(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1101d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT Range::GetMergeCells()
{
	VARIANT result;
	InvokeHelper(0xc1101e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetMergeCells(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1101e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT Range::GetStyle()
{
	VARIANT result;
	InvokeHelper(0xc1100e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetStyle(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1100e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH Range::GetEntireRow()
{
	LPDISPATCH result;
	InvokeHelper(0xc1101f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetEntireColumn()
{
	LPDISPATCH result;
	InvokeHelper(0xc11020, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetAreas()
{
	LPDISPATCH result;
	InvokeHelper(0xc11021, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0xc11022, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetRows()
{
	LPDISPATCH result;
	InvokeHelper(0xc11023, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT Range::GetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc11024, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		&RowIndex, &ColumnIndex);
	return result;
}

void Range::SetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc11024, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &RowIndex, &ColumnIndex, &newValue);
}

long Range::GetColumn()
{
	long result;
	InvokeHelper(0xc11025, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Range::GetRow()
{
	long result;
	InvokeHelper(0xc11026, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetFormatConditions()
{
	LPDISPATCH result;
	InvokeHelper(0xc11009, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::Insert(const VARIANT& Shift, const VARIANT& CopyOrigin)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Shift, &CopyOrigin);
}

void Range::Delete(const VARIANT& Shift)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc12002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Shift);
}

void Range::Copy(const VARIANT& Destination)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc12003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Destination);
}

void Range::Clear()
{
	InvokeHelper(0xc12005, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::ClearFormats()
{
	InvokeHelper(0xc12006, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::ClearContents()
{
	InvokeHelper(0xc12007, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::Select()
{
	InvokeHelper(0xc12008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

VARIANT Range::Sort(const VARIANT& Key1, long Order1, const VARIANT& Key2, const VARIANT& Type, long Order2, const VARIANT& Key3, long Order3, long Header, const VARIANT& OrderCustom, const VARIANT& MatchCase, long Orientation, long SortMethod, 
		long DataOption1, long DataOption2, long DataOption3)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0xc12009, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Key1, Order1, &Key2, &Type, Order2, &Key3, Order3, Header, &OrderCustom, &MatchCase, Orientation, SortMethod, DataOption1, DataOption2, DataOption3);
	return result;
}

VARIANT Range::AutoFill(LPDISPATCH Destination, long Type)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_I4;
	InvokeHelper(0xc1200a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Destination, Type);
	return result;
}

VARIANT Range::DataSeries(const VARIANT& Rowcol, long Type, long Date, const VARIANT& Step, const VARIANT& Stop, const VARIANT& Trend)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc1200b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Rowcol, Type, Date, &Step, &Stop, &Trend);
	return result;
}

VARIANT Range::FillDown()
{
	VARIANT result;
	InvokeHelper(0xc1200c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT Range::FillLeft()
{
	VARIANT result;
	InvokeHelper(0xc1200d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT Range::FillRight()
{
	VARIANT result;
	InvokeHelper(0xc1200e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT Range::FillUp()
{
	VARIANT result;
	InvokeHelper(0xc1200f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT Range::Consolidate(const VARIANT& Sources, long Function, const VARIANT& TopRow, const VARIANT& LeftColumn, const VARIANT& CreateLinks)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12010, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Sources, Function, &TopRow, &LeftColumn, &CreateLinks);
	return result;
}

VARIANT Range::AutoFilter(const VARIANT& Field, const VARIANT& Criteria1, long Operator, const VARIANT& Criteria2, const VARIANT& VisibleDropDown)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12011, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Field, &Criteria1, Operator, &Criteria2, &VisibleDropDown);
	return result;
}

VARIANT Range::Subtotal(long GroupBy, long Function, const VARIANT& TotalList, const VARIANT& Replace, const VARIANT& PageBreaks, long SummaryBelowData)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4;
	InvokeHelper(0xc12012, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		GroupBy, Function, &TotalList, &Replace, &PageBreaks, SummaryBelowData);
	return result;
}

VARIANT Range::RemoveSubtotal()
{
	VARIANT result;
	InvokeHelper(0xc12013, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::Activate()
{
	InvokeHelper(0xc12000, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0xc12014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName, ManualDuplexPrint, PrintZoomColumn, PrintZoomRow, PrintZoomPaperWidth, PrintZoomPaperHeight, FlipPrint, PaperTray, CutterLine, PaperOrder);
}

void Range::ApplyOutlineStyles()
{
	InvokeHelper(0xc12015, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Range::Find(const VARIANT& What, const VARIANT& After, const VARIANT& LookIn, const VARIANT& LookAt, const VARIANT& SearchOrder, long SearchDirection, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12016, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&What, &After, &LookIn, &LookAt, &SearchOrder, SearchDirection, &MatchCase, &MatchByte, &SearchFormat);
	return result;
}

VARIANT Range::Replace(const VARIANT& What, const VARIANT& Replacement, const VARIANT& LookAt, const VARIANT& SearchOrder, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat, const VARIANT& ReplaceFormat)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12017, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&What, &Replacement, &LookAt, &SearchOrder, &MatchCase, &MatchByte, &SearchFormat, &ReplaceFormat);
	return result;
}

void Range::Ungroup()
{
	InvokeHelper(0xc12018, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::Group(const VARIANT& Start, const VARIANT& End, const VARIANT& By, const VARIANT& Periods)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12019, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Start, &End, &By, &Periods);
}

void Range::Merge(BOOL Across)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc1201b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Across);
}

void Range::UnMerge()
{
	InvokeHelper(0xc1201c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Range::GetMergeArea()
{
	LPDISPATCH result;
	InvokeHelper(0xc11027, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::ClearOutline()
{
	InvokeHelper(0xc1201a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Range::FindNext(const VARIANT& After)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1201d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&After);
	return result;
}

LPDISPATCH Range::FindPrevious(const VARIANT& After)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1201e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&After);
	return result;
}

void Range::Cut(const VARIANT& Destination)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc1201f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Destination);
}

void Range::AutoFit()
{
	InvokeHelper(0xc12020, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::InsertIndent(long InsertAmount)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc12021, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 InsertAmount);
}

LPDISPATCH Range::AddComment(LPCTSTR Text)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc12022, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Text);
	return result;
}

LPDISPATCH Range::GetComment()
{
	LPDISPATCH result;
	InvokeHelper(0xc11028, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::ClearComments()
{
	InvokeHelper(0xc12023, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Range::GetHyperlinks()
{
	LPDISPATCH result;
	InvokeHelper(0xc11029, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT Range::PasteSpecial(long Paste, long Operation, const VARIANT& SkipBlanks, const VARIANT& Transpose)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12004, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Paste, Operation, &SkipBlanks, &Transpose);
	return result;
}

VARIANT Range::CreateNames(BOOL Top, BOOL Left, BOOL Bottom, BOOL Right)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL;
	InvokeHelper(0xc12024, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Top, Left, Bottom, Right);
	return result;
}

LPDISPATCH Range::GetWorksheet()
{
	LPDISPATCH result;
	InvokeHelper(0xc1102a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL Range::GetCommented()
{
	BOOL result;
	InvokeHelper(0xc1102b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetHyperlink()
{
	LPDISPATCH result;
	InvokeHelper(0xc1102c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::CorrectErrors(long opt)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc12025, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 opt);
}

VARIANT Range::GetValue(long Type)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc11006, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		Type);
	return result;
}

void Range::SetValue(long Type, const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0xc11006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Type, &newValue);
}

LPDISPATCH Range::Offset(const VARIANT& RowOffset, const VARIANT& ColumnOffset)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12026, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&RowOffset, &ColumnOffset);
	return result;
}

LPDISPATCH Range::GetCurrentRegion()
{
	LPDISPATCH result;
	InvokeHelper(0xc1102e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::SpecialCells(long Type, const VARIANT& Value)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0xc12027, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, &Value);
	return result;
}

LPDISPATCH Range::GetEnd(long Direction)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc1102f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Direction);
	return result;
}

LPDISPATCH Range::GetCharacters(const VARIANT& Start, const VARIANT& Length)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc11030, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Start, &Length);
	return result;
}

VARIANT Range::TextToColumns(const VARIANT& Destination, long DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, const VARIANT& Space, const VARIANT& Other, 
		const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc12028, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Destination, DataType, TextQualifier, &ConsecutiveDelimiter, &Tab, &Semicolon, &Comma, &Space, &Other, &OtherChar, &FieldInfo, &DecimalSeparator, &ThousandsSeparator, &TrailingMinusNumbers);
	return result;
}

LPDISPATCH Range::GetValidation()
{
	LPDISPATCH result;
	InvokeHelper(0xc1102d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetPageBreak()
{
	long result;
	InvokeHelper(0xc11031, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetPageBreak(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc11031, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void Range::Calculate()
{
	InvokeHelper(0xc1202a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Range::GetRange(const VARIANT& Cell1, const VARIANT& Cell2)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc11032, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Cell1, &Cell2);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Worksheet properties

/////////////////////////////////////////////////////////////////////////////
// _Worksheet operations

LPDISPATCH _Worksheet::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Worksheet::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Worksheet::GetDescription()
{
	CString result;
	InvokeHelper(0xfffff06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Worksheet::GetIndex()
{
	long result;
	InvokeHelper(0x511004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Worksheet::GetName()
{
	CString result;
	InvokeHelper(0x511002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Worksheet::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x511002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _Worksheet::GetUsedRange()
{
	LPDISPATCH result;
	InvokeHelper(0x511005, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetNames()
{
	LPDISPATCH result;
	InvokeHelper(0x511006, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetRange(const VARIANT& Cell1, const VARIANT& Cell2)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x511003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Cell1, &Cell2);
	return result;
}

LPDISPATCH _Worksheet::GetHPageBreaks()
{
	LPDISPATCH result;
	InvokeHelper(0x511007, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetVPageBreaks()
{
	LPDISPATCH result;
	InvokeHelper(0x511008, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetTab()
{
	LPDISPATCH result;
	InvokeHelper(0x511009, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetOutline()
{
	LPDISPATCH result;
	InvokeHelper(0x51100a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT _Worksheet::GetEnableSelection()
{
	VARIANT result;
	InvokeHelper(0x51100b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _Worksheet::SetEnableSelection(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51100b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

BOOL _Worksheet::GetProtectScenarios()
{
	BOOL result;
	InvokeHelper(0x51100c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Worksheet::GetProtectDrawingObjects()
{
	BOOL result;
	InvokeHelper(0x51100d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetProtection()
{
	LPDISPATCH result;
	InvokeHelper(0x51100e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Worksheet::GetVisible()
{
	BOOL result;
	InvokeHelper(0x51100f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Worksheet::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x51100f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

double _Worksheet::GetStandardWidth()
{
	double result;
	InvokeHelper(0x511010, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _Worksheet::SetStandardWidth(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x511010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Worksheet::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0x511012, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetRows()
{
	LPDISPATCH result;
	InvokeHelper(0x511013, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetPageSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x511014, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetCells()
{
	LPDISPATCH result;
	InvokeHelper(0x511015, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Worksheet::GetDisplayAutomaticPageBreaks()
{
	BOOL result;
	InvokeHelper(0x511019, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Worksheet::SetDisplayAutomaticPageBreaks(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x511019, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

double _Worksheet::GetStandardHeight()
{
	double result;
	InvokeHelper(0x511011, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _Worksheet::SetStandardHeight(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x511011, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Worksheet::GetShapes()
{
	LPDISPATCH result;
	InvokeHelper(0x511016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Worksheet::GetProtectContents()
{
	BOOL result;
	InvokeHelper(0x511017, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Worksheet::GetProtectMode()
{
	BOOL result;
	InvokeHelper(0x511018, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetHyperlinks()
{
	LPDISPATCH result;
	InvokeHelper(0x51101a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Worksheet::GetFilterMode()
{
	BOOL result;
	InvokeHelper(0x51101b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Worksheet::GetAutoFilterMode()
{
	BOOL result;
	InvokeHelper(0x51101c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Worksheet::SetAutoFilterMode(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x51101c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Worksheet::GetComments()
{
	LPDISPATCH result;
	InvokeHelper(0x51101f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Worksheet::Delete()
{
	InvokeHelper(0x512000, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Worksheet::Activate()
{
	InvokeHelper(0x512001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Worksheet::PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x512002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName, ManualDuplexPrint, PrintZoomColumn, PrintZoomRow, PrintZoomPaperWidth, PrintZoomPaperHeight, FlipPrint, PaperTray, CutterLine, PaperOrder);
}

void _Worksheet::Move(const VARIANT& Before, const VARIANT& After)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x512003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Before, &After);
}

void _Worksheet::Select(BOOL Replace)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x512004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Replace);
}

void _Worksheet::Protect(const VARIANT& password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly, const VARIANT& AllowFormattingCells, const VARIANT& AllowFormattingColumns, 
		const VARIANT& AllowFormattingRows, const VARIANT& AllowInsertingColumns, const VARIANT& AllowInsertingRows, const VARIANT& AllowInsertingHyperlinks, const VARIANT& AllowDeletingColumns, const VARIANT& AllowDeletingRows, 
		const VARIANT& AllowSorting, const VARIANT& AllowFiltering, const VARIANT& AllowUsingPivotTables)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x512005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &password, &DrawingObjects, &Contents, &Scenarios, &UserInterfaceOnly, &AllowFormattingCells, &AllowFormattingColumns, &AllowFormattingRows, &AllowInsertingColumns, &AllowInsertingRows, &AllowInsertingHyperlinks, &AllowDeletingColumns, 
		&AllowDeletingRows, &AllowSorting, &AllowFiltering, &AllowUsingPivotTables);
}

void _Worksheet::Unprotect(const VARIANT& password)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x512006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &password);
}

void _Worksheet::Copy(const VARIANT& Before, const VARIANT& After)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x512007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Before, &After);
}

void _Worksheet::Paste(const VARIANT& Destination, const VARIANT& Link)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x512008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Destination, &Link);
}

void _Worksheet::ShowAllData()
{
	InvokeHelper(0x512009, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Worksheet::PasteSpecial(const VARIANT& Format, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& NoHTMLFormatting)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x51200a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Format, &Link, &DisplayAsIcon, &IconFileName, &IconIndex, &IconLabel, &NoHTMLFormatting);
}

LPDISPATCH _Worksheet::GetAutoFilter()
{
	LPDISPATCH result;
	InvokeHelper(0x51101d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::GetQueryTables()
{
	LPDISPATCH result;
	InvokeHelper(0x51101e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Worksheet::ChartObjects(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51200b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

void _Worksheet::PrintPreview(const VARIANT& EnableChanges)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51200c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &EnableChanges);
}

void _Worksheet::Calculate()
{
	InvokeHelper(0x51200d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Sheets properties

/////////////////////////////////////////////////////////////////////////////
// Sheets operations

LPDISPATCH Sheets::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Sheets::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Sheets::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Sheets::GetDescription()
{
	CString result;
	InvokeHelper(0xfffff06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Sheets::GetCount()
{
	long result;
	InvokeHelper(0x1821000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Sheets::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1821001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

void Sheets::PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x1822000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName, ManualDuplexPrint, PrintZoomColumn, PrintZoomRow, PrintZoomPaperWidth, PrintZoomPaperHeight, FlipPrint, PaperTray, CutterLine, PaperOrder);
}

void Sheets::Copy(const VARIANT& Before, const VARIANT& After)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x1822001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Before, &After);
}

void Sheets::Move(const VARIANT& Before, const VARIANT& After)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x1822002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Before, &After);
}

void Sheets::Select(BOOL Replace)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1822004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Replace);
}

LPUNKNOWN Sheets::Get_NewEnum()
{
	LPUNKNOWN result;
	InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPDISPATCH Sheets::Get_Default(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

BOOL Sheets::GetVisible()
{
	BOOL result;
	InvokeHelper(0x1821002, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Sheets::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1821002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void Sheets::Delete()
{
	InvokeHelper(0x1822003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Sheets::FillAcrossSheets(LPDISPATCH Range, long Type)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_I4;
	InvokeHelper(0x1822005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Range, Type);
}

LPDISPATCH Sheets::Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x1822006, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Before, &After, &Count, &Type);
	return result;
}

void Sheets::PrintPreview(const VARIANT& EnableChanges)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1822007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &EnableChanges);
}
