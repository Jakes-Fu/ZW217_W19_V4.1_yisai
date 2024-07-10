// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class
#ifndef _ETAPP_H__
#define _ETAPP_H__

class _Application : public COleDispatchDriver
{
public:
	_Application() {}		// Calls COleDispatchDriver default constructor
	_Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Application(const _Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetDescription();
	LPDISPATCH GetWorkbooks();
	LPDISPATCH GetWindows();
	LPDISPATCH GetActiveWindow();
	LPDISPATCH GetActiveWorkbook();
	LPDISPATCH GetActiveSheet();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	BOOL GetUserControl();
	void SetUserControl(BOOL bNewValue);
	LPDISPATCH GetNames();
	LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
	LPDISPATCH GetSelection();
	LPDISPATCH GetActiveCell();
	long GetCustomListCount();
	LPDISPATCH GetCommandBars();
	CString GetActivePrinter();
	void SetActivePrinter(LPCTSTR lpszNewValue);
	CString GetDefaultFilePath();
	void SetDefaultFilePath(LPCTSTR lpszNewValue);
	BOOL GetShowStartupDialog();
	void SetShowStartupDialog(BOOL bNewValue);
	BOOL GetDisplayFormulaBar();
	void SetDisplayFormulaBar(BOOL bNewValue);
	BOOL GetDisplayStatusBar();
	void SetDisplayStatusBar(BOOL bNewValue);
	BOOL GetIteration();
	void SetIteration(BOOL bNewValue);
	long GetMaxIterations();
	void SetMaxIterations(long nNewValue);
	double GetMaxChange();
	void SetMaxChange(double newValue);
	BOOL GetFixedDecimal();
	void SetFixedDecimal(BOOL bNewValue);
	long GetFixedDecimalPlaces();
	void SetFixedDecimalPlaces(long nNewValue);
	BOOL GetCopyObjectsWithCells();
	void SetCopyObjectsWithCells(BOOL bNewValue);
	BOOL GetMoveAfterReturn();
	void SetMoveAfterReturn(BOOL bNewValue);
	long GetMoveAfterReturnDirection();
	void SetMoveAfterReturnDirection(long nNewValue);
	long GetReferenceStyle();
	void SetReferenceStyle(long nNewValue);
	CString GetUserName_();
	void SetUserName(LPCTSTR lpszNewValue);
	CString GetStandardFont();
	void SetStandardFont(LPCTSTR lpszNewValue);
	long GetStandardFontSize();
	void SetStandardFontSize(long nNewValue);
	long GetSheetsInNewWorkbook();
	void SetSheetsInNewWorkbook(long nNewValue);
	BOOL GetShowChartTipNames();
	void SetShowChartTipNames(BOOL bNewValue);
	BOOL GetShowChartTipValues();
	void SetShowChartTipValues(BOOL bNewValue);
	long GetCalculation();
	void SetCalculation(long nNewValue);
	BOOL GetDisplayRecentFiles();
	void SetDisplayRecentFiles(BOOL bNewValue);
	LPDISPATCH GetRecentFiles();
	LPDISPATCH GetCOMAddIns();
	LPDISPATCH GetDebugTools();
	LPDISPATCH GetSheets();
	LPDISPATCH GetWorksheets();
	LPDISPATCH GetRows();
	LPDISPATCH GetColumns();
	void Quit();
	void Terminate(BOOL bForce);
	void Undo();
	void Redo();
	void AddCustomList(const VARIANT& ListArray, const VARIANT& ByRow);
	void DeleteCustomList(long ListNum);
	VARIANT GetCustomListContents(long ListNum);
	LPDISPATCH FindFormat();
	LPDISPATCH ReplaceFormat();
	long GetCustomListNum(const VARIANT& ListArray);
	double InchesToPoints(double inches);
	long GetCutCopyMode();
	void SetCutCopyMode(long nNewValue);
	void SetDefaultChart(const VARIANT& FormatName);
	void SetSaveAsFileType(const VARIANT& newValue);
	VARIANT GetSaveAsFileType();
	BOOL GetEnableSpecialDiagonal();
	void SetEnableSpecialDiagonal(BOOL bNewValue);
	BOOL GetSaveBackupFileOnFirstSave();
	void SetSaveBackupFileOnFirstSave(BOOL bNewValue);
	CString GetDefaultWorkbookName();
	void SetDefaultWorkbookName(LPCTSTR lpszNewValue);
	CString GetDefaultWorksheetName();
	void SetDefaultWorksheetName(LPCTSTR lpszNewValue);
	void SetStatusBar(LPCTSTR lpszNewValue);
	CString GetStatusBar();
	void SetDisplayAlerts(BOOL bNewValue);
	BOOL GetDisplayAlerts();
	void OnKey(const VARIANT& Key1, const VARIANT& Key2, const VARIANT& Key3, const VARIANT& key4, const VARIANT& Procedure);
	VARIANT Run(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, 
		const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, 
		const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, 
		const VARIANT& Arg30);
	CString GetVersion();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	VARIANT GetSaveAsFilename(const VARIANT& InitialFilename, const VARIANT& FileFilter, const VARIANT& FilterIndex, const VARIANT& Title, const VARIANT& ButtonText);
	long GetCursor();
	void SetCursor(long nNewValue);
	CString GetName();
	LPDISPATCH GetAutoRecover();
	LPDISPATCH GetCells();
	LPDISPATCH GetCustomDictionaries();
	LPDISPATCH GetSpellingOptions();
	long GetWindowState();
	void SetWindowState(long nNewValue);
	LPDISPATCH GetAddIns();
	LPDISPATCH GetAdvApiRoot();
	BOOL GetEnableAppWindow();
	void SetEnableAppWindow(BOOL bNewValue);
	LPDISPATCH GetTaskPanes();
	void Calculate();
	BOOL GetScreenUpdating();
	void SetScreenUpdating(BOOL bNewValue);
	LPDISPATCH GetWorksheetFunction();
	CString GetBuild();
};
/////////////////////////////////////////////////////////////////////////////
// Workbooks wrapper class

class Workbooks : public COleDispatchDriver
{
public:
	Workbooks() {}		// Calls COleDispatchDriver default constructor
	Workbooks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Workbooks(const Workbooks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetDescription();
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPDISPATCH Add(const VARIANT& Template);
	void Close(const VARIANT& SaveChanges);
	LPDISPATCH Open(const VARIANT& Filename, const VARIANT& UpdateLinks, const VARIANT& ReadOnly, const VARIANT& Format, const VARIANT& password, const VARIANT& WriteResPassword, const VARIANT& IgnoreReadOnlyRecommended, const VARIANT& Origin, 
		const VARIANT& Delimiter, const VARIANT& Editable, const VARIANT& Notify, const VARIANT& Converter, const VARIANT& AddToMru);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH OpenText(LPCTSTR Filename, const VARIANT& Origin, const VARIANT& StartRow, const VARIANT& DataType, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, const VARIANT& Space, 
		const VARIANT& Other, const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& TextVisualLayout, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers, const VARIANT& Local);
	LPDISPATCH OpenDatabase(LPCTSTR Filename, const VARIANT& CommandText, const VARIANT& CommandType, const VARIANT& BackgroundQuery, const VARIANT& ImportDataAs);
};
/////////////////////////////////////////////////////////////////////////////
// _Workbook wrapper class

class _Workbook : public COleDispatchDriver
{
public:
	_Workbook() {}		// Calls COleDispatchDriver default constructor
	_Workbook(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Workbook(const _Workbook& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetDescription();
	LPDISPATCH GetContainer();
	LPDISPATCH GetWorksheets();
	LPDISPATCH GetWindows();
	LPDISPATCH GetActiveSheet();
	CString GetFullName();
	LPDISPATCH GetStyles();
	LPDISPATCH GetNames();
	CString GetName();
	VARIANT GetColors(const VARIANT& Index);
	void SetColors(const VARIANT& Index, const VARIANT& newValue);
	LPDISPATCH GetSheets();
	LPDISPATCH GetExtraColors();
	CString GetPath();
	BOOL GetSaved();
	void SetSaved(BOOL bNewValue);
	void SetPassword(LPCTSTR lpszNewValue);
	CString GetPasswordEncryptionProvider();
	CString GetPasswordEncryptionAlgorithm();
	long GetPasswordEncryptionKeyLength();
	BOOL GetPasswordEncryptionFileProperties();
	BOOL GetHasPassword();
	BOOL GetProtectStructure();
	void SetProtectStructure(BOOL bNewValue);
	BOOL GetProtectWindows();
	void SetProtectWindows(BOOL bNewValue);
	void NewWindow();
	void Close(const VARIANT& SaveChanges, const VARIANT& Filename, const VARIANT& RouteWorkbook);
	void Activate();
	void SaveAs(const VARIANT& Filename, const VARIANT& FileFormat, const VARIANT& password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, long AccessMode, const VARIANT& ConflictResolution, 
		const VARIANT& AddToMru, const VARIANT& TextCodepage, const VARIANT& TextVisualLayout);
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder);
	void DeleteNumberFormat(LPCTSTR NumberFormat);
	void Protect(const VARIANT& password, const VARIANT& structure, const VARIANT& Window);
	void Unprotect(const VARIANT& password);
	void SetPasswordEncryptionOptions(LPCTSTR PasswordEncryptionProvider, LPCTSTR PasswordEncryptionAlgorithm, long PasswordEncryptionKeyLength, VARIANT* PasswordEncryptionFileProperties);
	void SendMail(const VARIANT& Recipients, const VARIANT& Subject, const VARIANT& ReturnReceipt);
	LPDISPATCH GetBuiltinDocumentProperties();
	LPDISPATCH GetCustomDocumentProperties();
	void Save();
	void SetWritePassword(LPCTSTR lpszNewValue);
	LPDISPATCH GetSecurityPolicies();
	BOOL GetUserMode();
	void SetUserMode(BOOL bNewValue);
	BOOL GetTrackChanges();
	void SetTrackChanges(BOOL bNewValue);
	BOOL GetHighlightChangesOnScreen();
	void SetHighlightChangesOnScreen(BOOL bNewValue);
	BOOL GetListChangesOnNewSheet();
	void SetListChangesOnNewSheet(BOOL bNewValue);
	void HighlightChangesOptions(const VARIANT& When, const VARIANT& Who, const VARIANT& Where);
	LPDISPATCH GetChanges();
	void AcceptAllChanges();
	void RejectAllChanges();
};
/////////////////////////////////////////////////////////////////////////////
// Worksheets wrapper class

class Worksheets : public COleDispatchDriver
{
public:
	Worksheets() {}		// Calls COleDispatchDriver default constructor
	Worksheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Worksheets(const Worksheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetDescription();
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type);
	LPUNKNOWN Get_NewEnum();
	void PrintPreview(const VARIANT& EnableChanges);
};
/////////////////////////////////////////////////////////////////////////////
// Range wrapper class

class Range : public COleDispatchDriver
{
public:
	Range() {}		// Calls COleDispatchDriver default constructor
	Range(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Range(const Range& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetDescription();
	CString GetAddress(BOOL RowAbsolute, BOOL ColumnAbsolute, long ReferenceStyle, BOOL External, const VARIANT& RelativeTo);
	VARIANT Get_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex);
	void Set_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue);
	long GetCount();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	CString GetFormulaR1C1();
	void SetFormulaR1C1(LPCTSTR lpszNewValue);
	VARIANT GetValue2();
	void SetValue2(const VARIANT& newValue);
	VARIANT GetHasArray();
	CString GetFormulaArray();
	void SetFormulaArray(LPCTSTR lpszNewValue);
	CString GetText();
	VARIANT GetHidden();
	void SetHidden(const VARIANT& newValue);
	double GetRowHeight();
	void SetRowHeight(double newValue);
	double GetColumnWidth();
	void SetColumnWidth(double newValue);
	LPDISPATCH GetBorders();
	LPDISPATCH GetFont();
	LPDISPATCH GetInterior();
	CString GetNumberFormat();
	void SetNumberFormat(LPCTSTR lpszNewValue);
	CString GetNumberFormatLocal();
	void SetNumberFormatLocal(LPCTSTR lpszNewValue);
	VARIANT GetAddIndent();
	void SetAddIndent(const VARIANT& newValue);
	long GetIndentLevel();
	void SetIndentLevel(long nNewValue);
	long GetHorizontalAlignment();
	void SetHorizontalAlignment(long nNewValue);
	long GetVerticalAlignment();
	void SetVerticalAlignment(long nNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetShrinkToFit();
	void SetShrinkToFit(const VARIANT& newValue);
	VARIANT GetWrapText();
	void SetWrapText(const VARIANT& newValue);
	VARIANT GetLocked();
	void SetLocked(const VARIANT& newValue);
	VARIANT GetFormulaHidden();
	void SetFormulaHidden(const VARIANT& newValue);
	VARIANT GetMergeCells();
	void SetMergeCells(const VARIANT& newValue);
	VARIANT GetStyle();
	void SetStyle(const VARIANT& newValue);
	LPDISPATCH GetEntireRow();
	LPDISPATCH GetEntireColumn();
	LPDISPATCH GetAreas();
	LPDISPATCH GetColumns();
	LPDISPATCH GetRows();
	VARIANT GetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex);
	void SetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue);
	long GetColumn();
	long GetRow();
	LPDISPATCH GetFormatConditions();
	void Insert(const VARIANT& Shift, const VARIANT& CopyOrigin);
	void Delete(const VARIANT& Shift);
	void Copy(const VARIANT& Destination);
	void Clear();
	void ClearFormats();
	void ClearContents();
	void Select();
	VARIANT Sort(const VARIANT& Key1, long Order1, const VARIANT& Key2, const VARIANT& Type, long Order2, const VARIANT& Key3, long Order3, long Header, const VARIANT& OrderCustom, const VARIANT& MatchCase, long Orientation, long SortMethod, 
		long DataOption1, long DataOption2, long DataOption3);
	VARIANT AutoFill(LPDISPATCH Destination, long Type);
	VARIANT DataSeries(const VARIANT& Rowcol, long Type, long Date, const VARIANT& Step, const VARIANT& Stop, const VARIANT& Trend);
	VARIANT FillDown();
	VARIANT FillLeft();
	VARIANT FillRight();
	VARIANT FillUp();
	VARIANT Consolidate(const VARIANT& Sources, long Function, const VARIANT& TopRow, const VARIANT& LeftColumn, const VARIANT& CreateLinks);
	VARIANT AutoFilter(const VARIANT& Field, const VARIANT& Criteria1, long Operator, const VARIANT& Criteria2, const VARIANT& VisibleDropDown);
	VARIANT Subtotal(long GroupBy, long Function, const VARIANT& TotalList, const VARIANT& Replace, const VARIANT& PageBreaks, long SummaryBelowData);
	VARIANT RemoveSubtotal();
	void Activate();
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder);
	void ApplyOutlineStyles();
	LPDISPATCH Find(const VARIANT& What, const VARIANT& After, const VARIANT& LookIn, const VARIANT& LookAt, const VARIANT& SearchOrder, long SearchDirection, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat);
	VARIANT Replace(const VARIANT& What, const VARIANT& Replacement, const VARIANT& LookAt, const VARIANT& SearchOrder, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat, const VARIANT& ReplaceFormat);
	void Ungroup();
	void Group(const VARIANT& Start, const VARIANT& End, const VARIANT& By, const VARIANT& Periods);
	void Merge(BOOL Across);
	void UnMerge();
	LPDISPATCH GetMergeArea();
	void ClearOutline();
	LPDISPATCH FindNext(const VARIANT& After);
	LPDISPATCH FindPrevious(const VARIANT& After);
	void Cut(const VARIANT& Destination);
	void AutoFit();
	void InsertIndent(long InsertAmount);
	LPDISPATCH AddComment(LPCTSTR Text);
	LPDISPATCH GetComment();
	void ClearComments();
	LPDISPATCH GetHyperlinks();
	VARIANT PasteSpecial(long Paste, long Operation, const VARIANT& SkipBlanks, const VARIANT& Transpose);
	VARIANT CreateNames(BOOL Top, BOOL Left, BOOL Bottom, BOOL Right);
	LPDISPATCH GetWorksheet();
	BOOL GetCommented();
	LPDISPATCH GetHyperlink();
	void CorrectErrors(long opt);
	VARIANT GetValue(long Type);
	void SetValue(long Type, const VARIANT& newValue);
	LPDISPATCH Offset(const VARIANT& RowOffset, const VARIANT& ColumnOffset);
	LPDISPATCH GetCurrentRegion();
	LPDISPATCH SpecialCells(long Type, const VARIANT& Value);
	LPDISPATCH GetEnd(long Direction);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT TextToColumns(const VARIANT& Destination, long DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, const VARIANT& Space, const VARIANT& Other, 
		const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers);
	LPDISPATCH GetValidation();
	long GetPageBreak();
	void SetPageBreak(long nNewValue);
	void Calculate();
	LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
};
/////////////////////////////////////////////////////////////////////////////
// _Worksheet wrapper class

class _Worksheet : public COleDispatchDriver
{
public:
	_Worksheet() {}		// Calls COleDispatchDriver default constructor
	_Worksheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Worksheet(const _Worksheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetDescription();
	long GetIndex();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetUsedRange();
	LPDISPATCH GetNames();
	LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	LPDISPATCH GetTab();
	LPDISPATCH GetOutline();
	VARIANT GetEnableSelection();
	void SetEnableSelection(const VARIANT& newValue);
	BOOL GetProtectScenarios();
	BOOL GetProtectDrawingObjects();
	LPDISPATCH GetProtection();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetStandardWidth();
	void SetStandardWidth(double newValue);
	LPDISPATCH GetColumns();
	LPDISPATCH GetRows();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetCells();
	BOOL GetDisplayAutomaticPageBreaks();
	void SetDisplayAutomaticPageBreaks(BOOL bNewValue);
	double GetStandardHeight();
	void SetStandardHeight(double newValue);
	LPDISPATCH GetShapes();
	BOOL GetProtectContents();
	BOOL GetProtectMode();
	LPDISPATCH GetHyperlinks();
	BOOL GetFilterMode();
	BOOL GetAutoFilterMode();
	void SetAutoFilterMode(BOOL bNewValue);
	LPDISPATCH GetComments();
	void Delete();
	void Activate();
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder);
	void Move(const VARIANT& Before, const VARIANT& After);
	void Select(BOOL Replace);
	void Protect(const VARIANT& password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly, const VARIANT& AllowFormattingCells, const VARIANT& AllowFormattingColumns, 
		const VARIANT& AllowFormattingRows, const VARIANT& AllowInsertingColumns, const VARIANT& AllowInsertingRows, const VARIANT& AllowInsertingHyperlinks, const VARIANT& AllowDeletingColumns, const VARIANT& AllowDeletingRows, 
		const VARIANT& AllowSorting, const VARIANT& AllowFiltering, const VARIANT& AllowUsingPivotTables);
	void Unprotect(const VARIANT& password);
	void Copy(const VARIANT& Before, const VARIANT& After);
	void Paste(const VARIANT& Destination, const VARIANT& Link);
	void ShowAllData();
	void PasteSpecial(const VARIANT& Format, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& NoHTMLFormatting);
	LPDISPATCH GetAutoFilter();
	LPDISPATCH GetQueryTables();
	LPDISPATCH ChartObjects(const VARIANT& Index);
	void PrintPreview(const VARIANT& EnableChanges);
	void Calculate();
};
/////////////////////////////////////////////////////////////////////////////
// Sheets wrapper class

class Sheets : public COleDispatchDriver
{
public:
	Sheets() {}		// Calls COleDispatchDriver default constructor
	Sheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Sheets(const Sheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetDescription();
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName, BOOL ManualDuplexPrint, 
		long PrintZoomColumn, long PrintZoomRow, long PrintZoomPaperWidth, long PrintZoomPaperHeight, BOOL FlipPrint, long PaperTray, BOOL CutterLine, long PaperOrder);
	void Copy(const VARIANT& Before, const VARIANT& After);
	void Move(const VARIANT& Before, const VARIANT& After);
	void Select(BOOL Replace);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Get_Default(const VARIANT& Index);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	void Delete();
	void FillAcrossSheets(LPDISPATCH Range, long Type);
	LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type);
	void PrintPreview(const VARIANT& EnableChanges);
};

#endif // _ETAPP_H__
