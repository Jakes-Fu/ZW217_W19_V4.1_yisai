/***************************************************************************************
** FileName    :  ConPanCfgAccess.h                                                   **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  24/02/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  interface for the CConPanCfgAccess class.                           **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  26/02/2010     Mingxuan.Zhu         Created                                       **
**  16/09/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#if !defined(AFX_CONPANCFGACCESS_H__7094F4A8_6055_450D_BFEA_EC4FC4BA348D__INCLUDED_)
#define AFX_CONPANCFGACCESS_H__7094F4A8_6055_450D_BFEA_EC4FC4BA348D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum
{
	S9_TYPICAL_TABLE = 0,
	S9_QWERTY_TABLE,
	CSTAR_TYPICAL_TABLE,
	CSTAR_QWERTY_TABLE,
	T9_TYPICAL_TABLE,
	T9_QWERTY_TABLE,
	SOGOU_TYPICAL_TABLE,
	SOGOU_QWERTY_TABLE,
	MAX_TABLE_NUM
}ATEST_IM_KEYPAD_TABLE_E;
class CConPanCfgAccess  
{
public:
    CConPanCfgAccess();
    virtual ~CConPanCfgAccess();
    
public:
    static CString GetCurrentDirectory();                            // get the absolute path of the current .exe file.
    static CString GetOTATestCasePath();                             // get the path of OTA TestCase.
    static CString GetOTAXmlTestCasePath();                          // get the path of OTA XML TestCase.
    static void SetOTAXmlTestCasePath(CString &sTestCaseFilePath);   // set the path of OTA XML TestCase.
    static void GetLocationTestCasePath(char *strPath);              // get the path of Location TestCase.
    static void GetPBTestCasePath(char *strPath);                    // get the path of PB TestCase.
    static void GetTestResultSavePath(char *strPath);                // get the main path of the test result
    static BOOL GetCMDAutoRunFlag();                                 // get the flag of the command line auto run.
    static BOOL GetVERSwitchFlag();                                  // get the flag of version switch.
    static BOOL GetOTAAutoRunFlag();                                 // get the flag of the OTA auto run.
    static BOOL GetLOCAutoRunFlag();                                 // get the flag of the Location auto run.
    static BOOL GetPBAutoRunFlag();                                  // get the flag of the PB auto run.
    static BOOL GetR8AutoRunFlag();									// get the flag of the R8 auto run.
	static BOOL GetInputMethodAutoRunFlag();						//get the flag of the InputMethod auto run.

	static BOOL GetImTestCasePath(int imkeypad,char *strPath);      //set the path of IM TestCase.
    static void SetCMDAutoRunFlag(const BOOL bFlag);                 // set the flag of the command line auto run.
    static void SetVERSwitchFlag(const BOOL bFlag);                  // set the flag of version switch.
    static void SetOTAAutoRunFlag(const BOOL bFlag);                 // set the flag of the OTA auto run.
    static void SetLOCAutoRunFlag(const BOOL bFlag);                 // set the flag of the Location auto run.
    static void SetPBAutoRunFlag(const BOOL bFlag);                  // set the flag of the PB auto run.
    static void SetR8AutoRunFlag(const BOOL bFlag);					// set the flag of the R8 auto run.
	static void SetInputMethodAutoRunFlag(const BOOL bFlag);		// set the flag of the InputMethod auto run.
	static void GetPBCaseNum(char *strPath);                         //get the pb case number
	static void GetR8CaseNum(char *strPath);                         //get the r8 case number
    static BOOL GetATestControlFlag();                               // get the flag of ATest Control sheet.	
	static void GetCalendarComparitionTablePath(char *strPath);		//get the path of the calendar comparition table path
};

#endif // !defined(AFX_CONPANCFGACCESS_H__7094F4A8_6055_450D_BFEA_EC4FC4BA348D__INCLUDED_)
