/***************************************************************************************
** FileName    :  ATestPropertyPage.h                                                 **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  24/02/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  interface for the CATestPropertyPage class.                         **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  08/02/2010     Mingxuan.Zhu         Created                                       **
**  27/09/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#if !defined(AFX_ATESTPROPERTYPAGE_H__BE46386C_213E_4325_9176_5D4AC2C31A4F__INCLUDED_)
#define AFX_ATESTPROPERTYPAGE_H__BE46386C_213E_4325_9176_5D4AC2C31A4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ATestPropertyPage.h : header file
//

#include "resource.h"
#include "ConPanLayer.h"
#include "ExcelApplication.h"
#include "atest_signal.h"

#define ATEST           9     // the start signal of ATEST module

// static constant definition.
static const unsigned int MAX_NUM_OF_PROJ = 4;
static const short RANGE_BG_COLOR = 3;
static const CString OTA_CONFIG_SHEET_NAME = "OTA";
static const unsigned int ROW_NO_OF_THE_TITLE = 1;
static const CString PDU_START_KEY_WORD = "Pdu1";
static const CString PDU_END_KEY_WORD = "Expected Results";
static const CString MS_OTA_INFO_COLUMN = "Actual Result";
static const CString NULL_END_KEY_WORD = "";
static const int EACH_OTA_ITEM_NUM = 9;
static const int OTA_ITEM_TOTAL_NUM = 90;     // 10*EACH_OTA_ITEM__NUM
static const int OTA_ITEM_MAX_LENGTH = 200;
static const int STRING_MAX_LEN =1000;
static const CString STATIC_TEXT_HEADER_OF_OTA_RESULT = " OTA Result Path : ";
static const CString STATIC_TEXT_HEADER_OF_LOC_RESULT = " Location Result Path : ";
static const CString STATIC_TEXT_END_OF_OTA_RESULT = "\\OTA";
static const CString STATIC_TEXT_END_OF_LOC_RESULT = "\\Location";
static const CString STATIC_TEXT_HEADER_OF_MULTILANG_RESULT = " Multi-Lang Coding Result Path : ";

//add by linchen for inputmethod testing
static const unsigned int UNICODEZERO = 0x0030;
static const unsigned int UNICODENINE = 0x0039;
static const unsigned int UNICODEAB = 0x0041;
static const unsigned int UNICODEFB = 0x0046;
static const unsigned int UNICODEAL = 0x0061;
static const unsigned int UNICODEFL = 0x0066;
static const unsigned int THREEGRADE = 4096;
static const unsigned int TWOGRADE = 256;
static const unsigned int ONEGRADE = 16;
static const unsigned int NUMGRADE = 48;
static const unsigned int BIGGRADE = 87;
static const unsigned int LITTERGRADE = 55;
static const unsigned int EXCEL_FIND_RANGE = 20;
static const unsigned int FIRST_COL = 1;
static const unsigned int THIRD_COL = 3;
static const unsigned int MAX_ABC_CHAR_NUM = 26;
static const unsigned int MAXFILEPATH = 100;
static const unsigned int MAXKEYCHAR = 10;
static const unsigned int MAXLANGMARK = 20;

//add by lin.chen for Calendar Testing
static const unsigned int ATEST_YEAR_START = 1900;
static const unsigned int ATEST_YEAR_END = 2100;
static const unsigned int ATEST_MONTH_START = 1;
static const unsigned int ATEST_MONTH_END = 12;
static const unsigned int ATEST_DAY_START = 1;
static const unsigned int ATEST_DAY_END = 31;

typedef unsigned short wchar;
typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E     dual_sys;
    uint16            signal_id;
    uint16            check_type;
    uint16            status_code;
    char              record_data[STRING_MAX_LEN];
	char              test_data[STRING_MAX_LEN];
}ATEST_CP_SIG_T;      // the signal between CP and APP   

typedef struct
{
    SIGNAL_VARS
    MN_DUAL_SYS_E     dual_sys;
    uint16            signal_id;
    uint16            check_type;
    uint16            status_code;
    char              source_file_path[MAX_PATH];
    char              result_file_path[MAX_PATH];
	char              config_file_path[MAX_PATH];
    char              record_data[STRING_MAX_LEN];
}ATEST_CP_DATA_T;     // the data of the testing between CP and APP
/*
typedef enum
{
    ATEST_CM_ENTER_IDLE_IND = ATEST + 1,
    ATEST_MC_SEND_FIRST_OTA_REQ,
    ATEST_CM_SEND_OTA_IND,
    ATEST_MC_SEND_NEXT_OTA_REQ,
    ATEST_CM_END_OTA_CNF,
    ATEST_CM_LOCATION_IND,
    ATEST_MC_END_LOCATION_REQ,
    ATEST_CM_PB_RUN_IND,
    ATEST_CM_PB_SELF_IND,
    ATEST_CM_R8_RUN_IND,
	ATEST_MC_PB_RUN_REQ,
	ATEST_MC_PB_SELF_REQ,
	ATEST_MC_R8_RUN_REQ,
}ATEST_CM_SIGNAL_E;
*/
typedef enum
{
    OTA,
    LOCATION,
    PB,
	PB_CONF,
	R8,
	INPUT_METHOD,
	CALENDAR_LUNAR,
	PLMN,
    OTHER_ITEM
}ATEST_ITEM_E;  // the name of the atest item.

typedef enum
{
    EMPTY_IN_INBOX = 0,
    NO_EMPTY_IN_INBOX
}ATEST_MSG_NUMBER_IN_INBOX_E;  // the message number of the Inbox.

typedef enum
{
    PROJ_OTA = 0,
    PROJ_LOC,
    PROJ_PB,
    PROJ_R8,
	PROJ_IM,
}ATEST_PROJ_ITEM_E;  // the message number of the Inbox.
typedef enum
{
	S9_TYPICAL = 0,
	S9_QWERTY,
	CSTAR_TYPICAL,
	CSTAR_QWERTY,
	T9_TYPICAL,
	T9_QWERTY,
	SOGOU_TYPICAL,
	SOGOU_QWERTY,
}ATEST_IM_KEYPAD_E;
typedef struct
{
	char* CStar_English_key;
	char*CStar_English_char;	
}CSTAR_ENGLISH_QWERTY_T;
typedef struct
{
	char* Qwerty_key;
	uint16 Qwerty_char;	
}QWERTY_KEY_MAP_T;

/////////////////////////////////////////////////////////////////////////////
// CATestPropertyPage dialog

class CATestPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CATestPropertyPage)

public:
    int sys_id;
    
protected:
    // static member

    // non-static member
    unsigned int m_iLineNoOfPDU;
    unsigned int m_iRowNumber;
    unsigned int m_iStartColNoOfSms;
    unsigned int m_iEndColNoOfSms;
    unsigned int m_iStartRowNoOfSms;
    unsigned int m_iEndRowNoOfSms;
    unsigned int m_iRowNo;
    unsigned int m_iColNo;
    unsigned int m_iPduNumber;
    unsigned int m_iPduNo;
    unsigned int m_iProjPos;
    BOOLEAN m_bSaveFile;
    BOOLEAN m_bMultiProjSwitch;
    CString m_sTestCaseFilePath;
    CString m_sResultFilePath;
    CString m_sResultMainPath;
    char    m_sSourceFilePath[MAX_PATH];
    char    m_sOutputFilePath[MAX_PATH];
    char    m_sFilePath[MAX_PATH];
	char    m_pbr8num[MAX_PATH];
    char    m_sPduData[STRING_MAX_LEN];
    CExcelApplication m_excelBook;
	CString m_pbsourcepath;
	CString m_pbrespath;
	CString m_r8respath;
	CString m_pbconfigpath;
    int     m_iProjSelectStatus[MAX_NUM_OF_PROJ];
	CString	m_inputmedhodpath;
	CString m_TestIMType;
	CString m_sLangSheetName;
	CString m_sLangUpName;
	CString m_KeyPadType;
	BOOLEAN b_KeyPadFlag;
	ATEST_IM_KEYPAD_E m_bIMKeypadSwitch;
	CString m_CStarExcelFilePath;
	long	m_bRow;
	long	m_bCol;

	CString	m_Calendarpath;
	CString	m_PLMNpath;
	
// Construction
public:
	CATestPropertyPage();
	~CATestPropertyPage();
    
    void DealCPMsg(void *pmsg);

// Dialog Data
    //{{AFX_DATA(CATestPropertyPage)
	enum { IDD = IDD_DIALOG_ATEST };
	CComboBox	m_sMultiLangCodingSwitch;
	int		m_iLocationSwitch;
	int		m_iFileFormatSwitch;
	CString	m_sOtaResultPath;
	CString	m_sLocResultPath;
	BOOL	m_bLocationSwitch;
	BOOL	m_bOTASwitch;
	BOOL	m_bPBSwitch;
	BOOL	m_bR8Switch;
	int		m_iVersionSwitch;
	CString	m_sMultiLangResultPath;
	int		m_MultiLangCurSel;
	BOOL	m_bIMTestSwitch;
	CString	m_MultiRresultPath;
	//}}AFX_DATA
    
// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CATestPropertyPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
// Implementation
public:
    // Generated message map functions
    //{{AFX_MSG(CATestPropertyPage)	
    afx_msg void OnButtonRunOtaTest();
	afx_msg void OnRadioLocationFl();
	afx_msg void OnRadioLocationMp();
	afx_msg void OnRadioLocationAll();
	afx_msg void OnButtonLocationRun();
	afx_msg void OnButtonPbRun();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioXlsFormat();
	afx_msg void OnRadioXmlFormat();
	afx_msg void OnButtonR8Create();
	afx_msg void OnButtonR8Run();
	afx_msg void OnButtonSelfTest();
	afx_msg void OnButtonComboRun();
	afx_msg void OnRadioChinese();
	afx_msg void OnRadioMultilingual();
	afx_msg void OnButtonInputMethodRun();
	afx_msg void OnButtonCalendarLunarRun();
	afx_msg void OnButtonPlmnRun();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        
protected:
    // business operation
    unsigned int GetTotalNumberOfTestCase();
    unsigned int GetTotalNumberOfPduInCurrentCase();
    void SendOTAMsgToMSSim();
    void PlaySendSMSSound();
    BOOL CheckMSSimStatus();
    BOOL CheckOTAConsistency( CString originalCfg,  /*IN Param*/
                              CString actualCfg     /*IN Param*/);
    unsigned int GetArrayFromCfgInfo( char* otaCfgStr,                             /*IN Param*/
                                      char  otaCfgItemArray[][OTA_ITEM_MAX_LENGTH] /*OUT Param*/);
    // excel operation
    void CleanActualResultInExcel();
    void SetActualResultInExcel(ATEST_CP_SIG_T *pOtaData);
    unsigned int GetEndRowNo( unsigned int rowNo,       /*IN Param*/
                              unsigned int colNo,       /*IN Param*/
                              CString keyWord   /*IN Param*/);
    unsigned int GetStartColNo( unsigned int rowNo,     /*IN Param*/
                                unsigned int colNo,     /*IN Param*/
                                CString keyWord /*IN Param*/);
    unsigned int GetEndColNo( unsigned int rowNo,     /*IN Param*/
                              unsigned int colNo,     /*IN Param*/
                              CString keyWord /*IN Param*/);
    // file path operation
    void GetTestCaseFilePath(ATEST_ITEM_E itemName);
    void GetTestResultFilePath(ATEST_ITEM_E itemName); 
    void SetButtonStatusOfComboRun();
    void ResetMultiProjSetting();

	//IM testing
	void SendIMMsgToMSSim(BOOL TestKeyPadType);
	void IMCodeTesting();
	void IMTestingResult(CString InputMethodpath);
	BOOL GetBeginPosition(char *StringFlag);
	BOOL GetCellValue(long cell_row,long cell_col, char *GetValue_ptr);
	void DealCstarExcel(CString InputMethodpath);
	uint16 ConvertCharToUint16(char *keychar);
	void DealSogouExcel(CString InputMethodpath);
	BOOL SearchFileInFolder(CString Filepath,CString& FileName);
	
	//Calendar Testing
	void SendCalendarMsgToMSSim();
	void Atest_CmpLundarData();

	void SendPLMNMsgToMSSim() ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATESTPROPERTYPAGE_H__BE46386C_213E_4325_9176_5D4AC2C31A4F__INCLUDED_)
