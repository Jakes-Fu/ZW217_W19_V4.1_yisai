/***************************************************************************************
** FileName    :  ConPanCfgAccess.cpp                                                 **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  18/07/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  implementation of the CConPanCfgAccess class.                       **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  18/07/2010     Mingxuan.Zhu         Created                                       **
**  16/09/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "stdafx.h"
#include "ConPanCfgAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// the name of the ConPan config file
static const char gs_szIniFile[]         = "ConPanCfg.ini";

// the settings of the "ATEST MODULE" section
static const char gs_szSect_ATM[]         = "ATEST MODULE";
static const char gs_szEntry_AtmST[]      = "ATest Control Switch";
static const char gs_szEntry_AtmCAR[]     = "CMD Auto Run";
static const char gs_szEntry_AtmVS[]      = "VER Switch";
static const char gs_szEntry_AtmOAR[]     = "OTA Auto Run";
static const char gs_szEntry_AtmLAR[]     = "LOC Auto Run";
static const char gs_szEntry_AtmPAR[]     = "PB Auto Run";
static const char gs_szEntry_AtmRAR[]     = "R8 Auto Run";
static const char gs_szEntry_AtmIAR[]     = "Inputmethod Auto Run";
static const char gs_szEntry_AtmOTCP[]    = "OTA TestCase Path";
static const char gs_szEntry_AtmOXMLTCP[] = "OTA XML TestCase Path";
static const char gs_szEntry_AtmLTCP[]    = "Location TestCase Path";
static const char gs_szEntry_AtmPTCP[]    = "PB TestCase Path";
static const char gs_szEntry_AtmTRMP[]    = "TestResult MainPath";
static const char gs_szEntry_Atmpbnum[]   = "PB Case Number";
static const char gs_szEntry_AtmR8num[]   = "R8 Case Number";

// the settings of the "INPUT METHOD" section
static const char gs_szSect_IM[]        = "INPUT METHOD";

static const char gs_szSect_CALENDAR[]  = "CALENDAR";
static const char gs_szSect_CCTP[]  = "Comparition Table path";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConPanCfgAccess::CConPanCfgAccess()
{

}

CConPanCfgAccess::~CConPanCfgAccess()
{

}

/**************************************************************************************/
// Description      : get the absolute path of the current .exe file.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CConPanCfgAccess::GetCurrentDirectory()
{
    char    szDirectory[MAX_PATH];
    CString strDirectory;
    
    GetModuleFileName(NULL, szDirectory, MAX_PATH);
    strDirectory = szDirectory;
    strDirectory = strDirectory.Left(strDirectory.ReverseFind('\\') + 1);
    
    return strDirectory;
}

/**************************************************************************************/
// Description      : get the path of the file named simulator test case.xls from ConPanCfg.ini
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CConPanCfgAccess::GetOTATestCasePath()
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    char strPath[MAX_PATH]={0};
    CString strOTATestCasePath="";

    GetPrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmOTCP, NULL, strPath, sizeof(strPath), strIniPath);
    strOTATestCasePath.Format(_T("%s"),strPath);

    return strOTATestCasePath;
}

/**************************************************************************************/
// Description      : get the path of the folder which contains xml format file from ConPanCfg.ini
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CConPanCfgAccess::GetOTAXmlTestCasePath()
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    char strPath[MAX_PATH]={0};
    CString strOTAXmlTestCasePath="";

    GetPrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmOXMLTCP, NULL, strPath, sizeof(strPath), strIniPath);
    strOTAXmlTestCasePath.Format(_T("%s"),strPath);

    return strOTAXmlTestCasePath;
}

/**************************************************************************************/
// Description      : set the path of the .xls test case file which contains wbxml format data.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString &sTestCaseFilePath
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetOTAXmlTestCasePath(CString &sTestCaseFilePath)
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    CString sXmlTestCaseFilePath = sTestCaseFilePath;

    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmOTCP, sXmlTestCaseFilePath, strIniPath);
}

/**************************************************************************************/
// Description      : get the path of the location test case from ConPanCfg.ini.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : char *strPath
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::GetLocationTestCasePath(char *strPath)  //OUT Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    memset(strPath,0,sizeof(char)*MAX_PATH);
    
    GetPrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmLTCP, NULL, strPath, sizeof(char)*MAX_PATH, strIniPath);
}

/**************************************************************************************/
// Description      : get the path of the PB test case from ConPanCfg.ini.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : char *strPath
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::GetPBTestCasePath(char *strPath)      //OUT Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    memset(strPath,0,sizeof(char)*MAX_PATH);

    GetPrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmPTCP, NULL, strPath, sizeof(char)*MAX_PATH, strIniPath);
}
/**************************************************************************************/
// Description      : get the path of the inputmethod case from ConPanCfg.ini.
//--------------------------------------------------------------------------------------
// Input Parameter  : int imkeypad
// Output Parameter : char *strPath
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetImTestCasePath(int imkeypad,char *strPath)      //OUT Param
{
    char *gs_szSect_IMKeyPad;
	ATEST_IM_KEYPAD_TABLE_E Im_Keypad_Table;

	CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    memset(strPath,0,sizeof(char)*MAX_PATH);

	if(!(imkeypad >= 0 && imkeypad < MAX_TABLE_NUM))
	{
		return FALSE;
	}
	
	Im_Keypad_Table = ATEST_IM_KEYPAD_TABLE_E(imkeypad);
	switch(Im_Keypad_Table)
	{
	case S9_TYPICAL_TABLE:
		gs_szSect_IMKeyPad = "S9 Typicaltable path";
		break;
	case S9_QWERTY_TABLE:
		gs_szSect_IMKeyPad = "S9 Qwertytable path";
		break;
	case CSTAR_TYPICAL_TABLE:
		gs_szSect_IMKeyPad = "CStar Typicaltable path";
		break;
	case CSTAR_QWERTY_TABLE:
		gs_szSect_IMKeyPad = "CStar Qwertytable path";
		break;
	case T9_TYPICAL_TABLE:
		gs_szSect_IMKeyPad = "T9 Typicaltable path";
		break;
	case T9_QWERTY_TABLE:
		gs_szSect_IMKeyPad = "T9 Qwertytable path";
		break;
	case SOGOU_TYPICAL_TABLE:
		gs_szSect_IMKeyPad = "Sogou Typicaltable path";
		break;
	case SOGOU_QWERTY_TABLE:
		gs_szSect_IMKeyPad = "Sogou Qwertytable path";
		break;
	default:
		return FALSE;
	}

 	GetPrivateProfileString(gs_szSect_IM, gs_szSect_IMKeyPad, NULL, strPath, sizeof(char)*MAX_PATH, strIniPath);
	return TRUE;
}

/**************************************************************************************/
// Description      : get the path of the result file from ConPanCfg.ini.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : char *strPath
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::GetTestResultSavePath(char *strPath)  //OUT Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    memset(strPath,0,sizeof(strPath));
    
    GetPrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmTRMP, NULL, strPath, sizeof(char)*MAX_PATH, strIniPath);
}

/**************************************************************************************/
// Description      : get the flag of the command line auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetCMDAutoRunFlag()
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt(gs_szSect_ATM, gs_szEntry_AtmCAR, FALSE, strIniPath);
}

/**************************************************************************************/
// Description      : get the flag of version switch.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetVERSwitchFlag()
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt(gs_szSect_ATM, gs_szEntry_AtmVS, FALSE, strIniPath);
}

/**************************************************************************************/
// Description      : get the flag of the OTA auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetOTAAutoRunFlag() 
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt(gs_szSect_ATM, gs_szEntry_AtmOAR, FALSE, strIniPath);
}

/**************************************************************************************/
// Description      : get the flag of the Location auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetLOCAutoRunFlag() 
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt(gs_szSect_ATM, gs_szEntry_AtmLAR, FALSE, strIniPath);
}

/**************************************************************************************/
// Description      : get the flag of the PB auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetPBAutoRunFlag() 
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt(gs_szSect_ATM, gs_szEntry_AtmPAR, FALSE, strIniPath);
}

/**************************************************************************************/
// Description      : get the flag of the R8 auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetR8AutoRunFlag() 
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt(gs_szSect_ATM, gs_szEntry_AtmRAR, FALSE, strIniPath);
}

/**************************************************************************************/
// Description      : get the flag of the InpitMethod auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetInputMethodAutoRunFlag()
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt(gs_szSect_ATM, gs_szEntry_AtmIAR, FALSE, strIniPath);
}

/**************************************************************************************/
// Description      : set the flag of the command line auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : const BOOL bFlag
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetCMDAutoRunFlag(const BOOL bFlag)  //IN Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    
    CString strCMDAutoRunFlag;
    strCMDAutoRunFlag.Format("%d",bFlag);
    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmCAR, strCMDAutoRunFlag, strIniPath);
}

/**************************************************************************************/
// Description      : set the flag of version switch.
//--------------------------------------------------------------------------------------
// Input Parameter  : const BOOL bFlag
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetVERSwitchFlag(const BOOL bFlag)  //IN Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    
    CString strVERSwitchFlag;
    strVERSwitchFlag.Format("%d",bFlag);
    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmVS, strVERSwitchFlag, strIniPath);
}

/**************************************************************************************/
// Description      : set the flag of the OTA auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : const BOOL bFlag
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetOTAAutoRunFlag(const BOOL bFlag)  //IN Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    
    CString strOTAAutoRunFlag;
    strOTAAutoRunFlag.Format("%d",bFlag);
    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmOAR, strOTAAutoRunFlag, strIniPath);
}

/**************************************************************************************/
// Description      : set the flag of the LOC auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : const BOOL bFlag
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetLOCAutoRunFlag(const BOOL bFlag)  //IN Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    
    CString strLOCAutoRunFlag;
    strLOCAutoRunFlag.Format("%d",bFlag);
    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmLAR, strLOCAutoRunFlag, strIniPath);
}

/**************************************************************************************/
// Description      : set the flag of the PB auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : const BOOL bFlag
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetPBAutoRunFlag(const BOOL bFlag)  //IN Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    
    CString strPBAutoRunFlag;
    strPBAutoRunFlag.Format("%d",bFlag);
    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmPAR, strPBAutoRunFlag, strIniPath);
}

/**************************************************************************************/
// Description      : set the flag of the R8 auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : const BOOL bFlag
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetR8AutoRunFlag(const BOOL bFlag)  //IN Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    
    CString strR8AutoRunFlag;
    strR8AutoRunFlag.Format("%d",bFlag);
    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmRAR, strR8AutoRunFlag, strIniPath);
}

/**************************************************************************************/
// Description      : set the flag of the InputMethod auto run.
//--------------------------------------------------------------------------------------
// Input Parameter  : const BOOL bFlag
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CConPanCfgAccess::SetInputMethodAutoRunFlag(const BOOL bFlag)  //IN Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    
    CString strInputMethodAutoRunFlag;
    strInputMethodAutoRunFlag.Format("%d",bFlag);
    WritePrivateProfileString(gs_szSect_ATM, gs_szEntry_AtmIAR, strInputMethodAutoRunFlag, strIniPath);
}

/**************************************************************************************/
// Description      : get the PB case number from ConPanCfg.ini.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : char *strPath
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::GetPBCaseNum(char *strPath)  
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    memset(strPath,0,sizeof(strPath));
    
    GetPrivateProfileString( gs_szSect_ATM, gs_szEntry_Atmpbnum, NULL, strPath, sizeof(char)*MAX_PATH, strIniPath );
}

/**************************************************************************************/
// Description      : get the R8 case number from ConPanCfg.ini.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : char *strPath
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/
void CConPanCfgAccess::GetR8CaseNum(char *strPath)  
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    memset(strPath,0,sizeof(strPath));
    
    GetPrivateProfileString( gs_szSect_ATM, gs_szEntry_AtmR8num, NULL, strPath, sizeof(char)*MAX_PATH, strIniPath );
}

/**************************************************************************************/
// Description      : get the flag of ATest Control sheet.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CConPanCfgAccess::GetATestControlFlag() 
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    return GetPrivateProfileInt( gs_szSect_ATM, gs_szEntry_AtmST, FALSE, strIniPath );
}

/**************************************************************************************/
// Description      : get the path of the calendar comparition table path
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CConPanCfgAccess::GetCalendarComparitionTablePath(char *strPath)  //OUT Param
{
    CString strIniPath = GetCurrentDirectory() + gs_szIniFile;
    memset(strPath,0,sizeof(strPath));
    
    GetPrivateProfileString(gs_szSect_CALENDAR, gs_szSect_CCTP, NULL, strPath, sizeof(char)*MAX_PATH, strIniPath);
}
