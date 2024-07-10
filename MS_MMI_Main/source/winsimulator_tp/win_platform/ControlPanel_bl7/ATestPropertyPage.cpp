/***************************************************************************************
** FileName    :  AtestPropertyPage.cpp                                               **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  02/06/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  implementation of the CAtestPropertyPage class.                     **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  02/06/2010     Mingxuan.Zhu         Created                                       **
**  27/09/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "stdafx.h"
#include "ConPanPropertySheet.h"
#include "ATestPropertyPage.h"
#include "SmsConPropertyPage.h"
#include "PduHexEncoder.h"
#include "ConPan.h"
#include "ConPanFunc.h"
#include "ConPanCfgAccess.h" 
#include "shlwapi.h"
#include "mmi_signal.h"
#include "pduHexEncoder.h"  


#pragma comment(lib,"shlwapi")  // PathFileExists    

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ATestPropertyPage property page

LOCAL char TableKey_data_arr[MAX_PATH] = {0};
LOCAL QWERTY_KEY_MAP_T QwertyKeyMap[MAX_ABC_CHAR_NUM]=
{
	{"A",0xb1},
	{"B",0xb2},
	{"C",0xb3},
	{"D",0xb4},
	{"E",0xb5},
	{"F",0xb6},
	{"G",0xb7},
	{"H",0xb8},
	{"I",0xb9},
	{"J",0xba},
	{"K",0xbb},
	{"L",0xbc},
	{"M",0xbd},
	{"N",0xbe},
	{"O",0xbf},
	{"P",0xc0},
	{"Q",0xc1},
	{"R",0xc2},
	{"S",0xc3},
	{"T",0xc4},
	{"U",0xc5},
	{"V",0xc6},
	{"W",0xc7},
	{"X",0xc8},
	{"Y",0xc9},
	{"Z",0xca},
};
LOCAL CSTAR_ENGLISH_QWERTY_T CStarEnglishQwertyKeyMap[MAX_ABC_CHAR_NUM]=
{
	{"A1","0x0061"},
	{"B1","0x0062"},
	{"C1","0x0063"},
	{"D1","0x0064"},
	{"E1","0x0065"},
	{"F1","0x0066"},
	{"G1","0x0067"},
	{"H1","0x0068"},
	{"I1","0x0069"},
	{"J1","0x006a"},
	{"K1","0x006b"},
	{"L1","0x006c"},
	{"M1","0x006d"},
	{"N1","0x006e"},
	{"O1","0x006f"},
	{"P1","0x0070"},
	{"Q1","0x0071"},
	{"R1","0x0072"},
	{"S1","0x0073"},
	{"T1","0x0074"},
	{"U1","0x0075"},
	{"V1","0x0076"},
	{"W1","0x0077"},
	{"X1","0x0078"},
	{"Y1","0x0079"},
	{"Z1","0x007a"},
};
IMPLEMENT_DYNCREATE(CATestPropertyPage, CPropertyPage)

CATestPropertyPage::CATestPropertyPage() : CPropertyPage(CATestPropertyPage::IDD)
{ 
    //{{AFX_DATA_INIT(CATestPropertyPage)
	m_iLocationSwitch = 0;
	m_iFileFormatSwitch = 0;
	m_sOtaResultPath = _T("");
	m_sLocResultPath = _T("");
	m_bLocationSwitch = FALSE;
	m_bOTASwitch = FALSE;
	m_bPBSwitch = FALSE;
	m_bR8Switch = FALSE;
	m_iVersionSwitch = 0;
	m_MultiLangCurSel = -1;
	m_bIMTestSwitch = FALSE;
	m_MultiRresultPath = _T("");
	//}}AFX_DATA_INIT
}

CATestPropertyPage::~CATestPropertyPage()
{
}

void CATestPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATestPropertyPage)
	DDX_Radio(pDX, IDC_RADIO_LOCATION_ALL, m_iLocationSwitch);
	DDX_Radio(pDX, IDC_RADIO_XLS_FORMAT, m_iFileFormatSwitch);
	DDX_Text(pDX, IDC_STATIC_OTA_RESULT_PATH, m_sOtaResultPath);
	DDX_Text(pDX, IDC_STATIC_LOCATION_RESULT_PATH, m_sLocResultPath);
	DDX_Check(pDX, IDC_CHECK_LOCATION, m_bLocationSwitch);
	DDX_Check(pDX, IDC_CHECK_OTA, m_bOTASwitch);
	DDX_Check(pDX, IDC_CHECK_PHONEBOOK, m_bPBSwitch);
	DDX_Check(pDX, IDC_CHECK_R8_CODE, m_bR8Switch);
	DDX_Radio(pDX, IDC_RADIO_CHINESE, m_iVersionSwitch);
	DDX_Check(pDX, IDC_CHECK_IM, m_bIMTestSwitch);
	DDX_Text(pDX, IDC_STATIC_MULTI_LAN_PATH, m_MultiRresultPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATestPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CATestPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_RUN_OTA_TEST, OnButtonRunOtaTest)
	ON_BN_CLICKED(IDC_RADIO_LOCATION_FL, OnRadioLocationFl)
	ON_BN_CLICKED(IDC_RADIO_LOCATION_MP, OnRadioLocationMp)
	ON_BN_CLICKED(IDC_RADIO_LOCATION_ALL, OnRadioLocationAll)
	ON_BN_CLICKED(IDC_BUTTON_LOCATION_RUN, OnButtonLocationRun)
	ON_BN_CLICKED(IDC_BUTTON_PB_RUN, OnButtonPbRun)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_XLS_FORMAT, OnRadioXlsFormat)
	ON_BN_CLICKED(IDC_RADIO_XML_FORMAT, OnRadioXmlFormat)
	ON_BN_CLICKED(IDC_BUTTON_R8_RUN, OnButtonR8Run)
	ON_BN_CLICKED(IDC_BUTTON_SELF_TEST, OnButtonSelfTest)
	ON_BN_CLICKED(IDC_BUTTON_COMBO_RUN, OnButtonComboRun)
	ON_BN_CLICKED(IDC_RADIO_CHINESE, OnRadioChinese)
	ON_BN_CLICKED(IDC_RADIO_MULTILINGUAL, OnRadioMultilingual)
	ON_BN_CLICKED(IDC_BUTTON_IM_RUN, OnButtonInputMethodRun)
	ON_BN_CLICKED(IDC_BUTTON_CALENDAR_RUN, OnButtonCalendarLunarRun)
	ON_BN_CLICKED(IDC_BUTTON_PLMN_RUN, OnButtonPlmnRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATestPropertyPage message handlers
/////////////////////////////////////////////////////////////////////////////
// CATestPropertyPage property page

/**************************************************************************************/
// Description      : send ota message to ms simulator by PDU sequence.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::SendOTAMsgToMSSim() 
{
    if(!CheckMSSimStatus())
    {
        return;
    }
    
    if(m_iPduNumber != m_iPduNo)
    {           
        CString pduData;
        pduData = m_excelBook.GetRangeValue(m_iStartRowNoOfSms+m_iLineNoOfPDU,m_iStartColNoOfSms+m_iPduNo);
        strcpy(m_sPduData,pduData.GetBuffer(pduData.GetLength()));
        pduData.ReleaseBuffer();

        xSignalHeaderRec *pSmsPageInd;
        
        InnerSCICreateSignal(
            &pSmsPageInd,
            ATEST_CP_SEND_PDU_IND,
            sizeof(ATEST_CP_DATA_T),
            P_CPANEL);
        
        // set the sig_ptr's param
        strcpy(((ATEST_CP_DATA_T*)pSmsPageInd)->record_data,m_sPduData); 
        
        SendCPMsgToConPan(pSmsPageInd);
        m_iPduNo++;

        SetTimer(1, 1*1000, NULL);
    }
}

/**************************************************************************************/
// Description      : get the row number of the last row from test case file.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. unsigned int rowNo
//                    2. unsigned int colNo
//                    3. CString keyWord      
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
unsigned int CATestPropertyPage::GetEndRowNo(unsigned int rowNo,unsigned int colNo,CString keyWord)
{   
    while(0 != m_excelBook.GetRangeValue(rowNo,colNo).CompareNoCase(keyWord))
    {
        rowNo++;
    }
    return rowNo;
}

/**************************************************************************************/
// Description      : get the column number of the start column from test case file.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. unsigned int rowNo
//                    2. unsigned int colNo
//                    3. CString keyWord 
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
unsigned int CATestPropertyPage::GetStartColNo(unsigned int rowNo,unsigned int colNo,CString keyWord)
{
    while(0 != m_excelBook.GetRangeValue(rowNo,colNo).CompareNoCase(keyWord))
    {
        colNo++;
    }
    return colNo;
}

/**************************************************************************************/
// Description      : get the column number of the last column from test case file.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. unsigned int rowNo
//                    2. unsigned int colNo
//                    3. CString keyWord 
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
unsigned int CATestPropertyPage::GetEndColNo(unsigned int rowNo,unsigned int colNo,CString keyWord)
{
    while(0 != m_excelBook.GetRangeValue(ROW_NO_OF_THE_TITLE,colNo).CompareNoCase(keyWord))
    {
        if(0 == m_excelBook.GetRangeValue(rowNo,colNo).CompareNoCase(""))
        {
            return colNo;
        }
        colNo++;
    }
    return colNo;
}

/**************************************************************************************/
// Description      : play the sound in the process of sending SMS.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::PlaySendSMSSound()
{
    Beep(2000,100);
    Sleep(500);
    Beep(2000,100);
}

/**************************************************************************************/
// Description      : handle the message from C_PANEL.
//--------------------------------------------------------------------------------------
// Input Parameter  : void *pmsg  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::DealCPMsg(void *pmsg)
{
    xSignalHeaderRec *psig = (xSignalHeaderRec*)pmsg;
    ATEST_CP_SIG_T *pData = (ATEST_CP_SIG_T*)pmsg;
    FILE *fpTestCaseFile = NULL;

    switch(pData->signal_id)
    {
    case ATEST_SEND_FIRST_OTA_REQ:  // a signal that requesting send OTA message.
        {
            m_bSaveFile = FALSE;
            GetTestCaseFilePath(OTA);
            GetTestResultFilePath(OTA);
            m_excelBook.CreateExcelObject();
            fpTestCaseFile = fopen(m_sTestCaseFilePath,"r"); 
            // The path of the OTA TestCase File is not correct.
            if(fpTestCaseFile == NULL)
            {
                AfxMessageBox("The Path of the OTA TestCase File is not correct!");
                m_excelBook.ReleaseExcelObjects();
				m_bMultiProjSwitch = FALSE;
                return;
            }
            else
            {
                fclose(fpTestCaseFile);
            }
            m_excelBook.GetSelectedWorkbook(m_sTestCaseFilePath);
            m_excelBook.GetWorksheets();
            // There is no sheet named OTA in current excel file.
            if(!m_excelBook.GetSelectedSheet(OTA_CONFIG_SHEET_NAME))
            {
                AfxMessageBox("There is no sheet named OTA in current excel file!!!");
                m_excelBook.ReleaseExcelObjects();
				m_bMultiProjSwitch = FALSE;
                return;
            }
            // There are no test case in the file of the OTA TestCase.
            if(!GetTotalNumberOfTestCase())
            {
                m_excelBook.ReleaseExcelObjects();
                MessageBox("There are no test case in the file of the OTA TestCase!", "Warning", MB_ICONWARNING);
                m_bMultiProjSwitch = FALSE;
				return ;
            }

            CleanActualResultInExcel();
            GetTotalNumberOfPduInCurrentCase();
            SendOTAMsgToMSSim();
        }
        break;
    case ATEST_SEND_NEXT_OTA_REQ:  // a signal that requesting send next OTA message.
        {   // check consistency code            
            if( m_iLineNoOfPDU >= m_iRowNumber )
            {   // the last OTA message        
                SetActualResultInExcel(pData);
                if( m_iRowNumber == 1 )
                {   // there is a OTA config information.
                    m_excelBook.SaveExcelFile(m_sResultFilePath);
                }
                else
                {   // there are much OTA config information.
                    m_excelBook.SaveExcelFile();
                }
                m_excelBook.ReleaseExcelObjects();
                m_iLineNoOfPDU = 0;
                
                xSignalHeaderRec* sig_ptr;

                // create a signal
                InnerSCICreateSignal(
                    &sig_ptr,
                    APP_ATEST_IND,
                    sizeof(ATEST_CP_DATA_T),
                    P_APP);

                // set the sig_ptr's param
                ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_END_OTA_CNF;
                
                // send a signal to MMI.
                SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);
                
                // the flow process of auto testing about the command line.
                if(CConPanCfgAccess::GetCMDAutoRunFlag() || m_bMultiProjSwitch)
                {
                    m_iProjPos++;
                    if(m_iProjPos<MAX_NUM_OF_PROJ)
                    {
                        OnButtonComboRun();
                        if(m_iProjPos==MAX_NUM_OF_PROJ)
                        {
                            ResetMultiProjSetting();
                        }
                    }
                    else
                    {
                        ResetMultiProjSetting();
                    }   
                }
                else
                {
                    AfxMessageBox( "OTA auto testing is over normally!", MB_ICONINFORMATION );
                }
            }
            else
            {   // the normal OTA message
                SetActualResultInExcel(pData);
                if(!m_bSaveFile)
                {
                    m_excelBook.SaveExcelFile(m_sResultFilePath);
                    m_bSaveFile = TRUE;
                }
                else
                {
                    m_excelBook.SaveExcelFile();
                }
                SetTimer(2, 2*1000, NULL);  // set the timer which sends the PDU data.       
            }        
        }
        break;
    case ATEST_END_LOCATION_REQ:  // a signal that informs the location testing is over.
        if (CConPanCfgAccess::GetCMDAutoRunFlag() || m_bMultiProjSwitch)
        {
            m_iProjPos++;
            if(m_iProjPos<MAX_NUM_OF_PROJ)
            {
                OnButtonComboRun();
                if(m_iProjPos==MAX_NUM_OF_PROJ)
                {
                    ResetMultiProjSetting();
                }
            }
            else
            {
                ResetMultiProjSetting();
            }
        } 
        else
        {
            AfxMessageBox( "Phone Location auto testing is over normally!", MB_ICONINFORMATION );
        }
        break;
	case ATEST_MC_PB_RUN_REQ:
		if (CConPanCfgAccess::GetCMDAutoRunFlag() || m_bMultiProjSwitch)
        {
            m_iProjPos++;
            if(m_iProjPos<MAX_NUM_OF_PROJ)
            {
                OnButtonComboRun();
                if(m_iProjPos==MAX_NUM_OF_PROJ)
                {
                    ResetMultiProjSetting();
                }
            }
            else
            {
                ResetMultiProjSetting();
            }
        } 
        else
        {
            AfxMessageBox( "Phone Book Coding auto testing is over normally!", MB_ICONINFORMATION );
        }        
		break;
	case ATEST_MC_PB_SELF_REQ:
         AfxMessageBox( "Phone Book Coding self cases testing is over normally!", MB_ICONINFORMATION );
		 break;
	case  ATEST_MC_R8_RUN_REQ:
		if (CConPanCfgAccess::GetCMDAutoRunFlag() || m_bMultiProjSwitch)
        {
            m_iProjPos++;
            if(m_iProjPos<MAX_NUM_OF_PROJ)
            {
                OnButtonComboRun();
                if(m_iProjPos==MAX_NUM_OF_PROJ)
                {
                    ResetMultiProjSetting();
                }
            }
            else
            {
                ResetMultiProjSetting();
            }
        } 
        else
        {
            AfxMessageBox( "Phone R8 Coding auto testing is over normally!", MB_ICONINFORMATION );
        }        
         break;
	case ATEST_MC_INPUTMETHOD_INFO:
		{
			//保存收到的信息，包括section名，语言名和keypad信息
			m_sLangSheetName = CString(pData->record_data);//excel的sheet名 or 文件名的关键字
			m_sLangUpName = CString(pData->test_data);//生中间ini文件的section名，各语言的大写名
			
			b_KeyPadFlag = pData->check_type;//键盘信息
			
			m_TestIMType = m_sLangUpName.Left(m_sLangUpName.Find("&")); //输入法
			m_sLangUpName.Delete(0,m_sLangUpName.Find("&")+1);

			IMCodeTesting();

			if(!b_KeyPadFlag)//universe:9键测完后，再测全键
			{
				SendIMMsgToMSSim(!b_KeyPadFlag);
			}
			else
			{
				CString MessageText(m_TestIMType);
				MessageText = MessageText+"\ auto testing is over normally!";
				AfxMessageBox( MessageText, MB_ICONINFORMATION );
			}
		}
		break;
	case ATEST_MC_CALENDAR_INFO:
		{
			if(pData->check_type)
			{
				Atest_CmpLundarData();
				AfxMessageBox( "Calendar Lunar Testing is over normally!", MB_ICONINFORMATION );
			}
			else
			{
				AfxMessageBox( "Calendar Lunar Testing is failed,Please restart the MSSim and ReTest!", MB_ICONINFORMATION );
			}	
		}
		break;

	case ATEST_MC_PLMN_INFO:
		{
			if(pData->check_type)
			{
				AfxMessageBox( "Getting PLMN Data  is over normally!", MB_ICONINFORMATION );
			}
			else
			{
				AfxMessageBox( "Getting PLMN Data failed,Please restart the MSSim and ReTest!", MB_ICONINFORMATION );
			}	
		}
		break;
    default:
        break;
    }
}

/**************************************************************************************/
// Description      : set the value of the range in the column named actual result.
//--------------------------------------------------------------------------------------
// Input Parameter  : MN_ATEST_DATA_T *pOtaData  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::SetActualResultInExcel(ATEST_CP_SIG_T *pOtaData)
{
    CString m_OTAExpectedCfgInfo ="";
    CString m_OTAActualCfgInfo ="";
    unsigned int resultRowNo = m_iStartRowNoOfSms;
    unsigned int resultColNo = m_iStartColNoOfSms;

    if(NO_EMPTY_IN_INBOX == pOtaData->status_code)
    {   // NO_EMPTY_IN_INBOX
        m_OTAActualCfgInfo.Format(_T("%s"),pOtaData->record_data);
        m_OTAActualCfgInfo.Replace("\r","\n");
        
        resultColNo = GetStartColNo(m_iRowNo,m_iColNo,MS_OTA_INFO_COLUMN);    
        m_excelBook.GetRange((resultRowNo+m_iLineNoOfPDU-1),resultColNo);
        m_excelBook.SetRangeValue(m_OTAActualCfgInfo);
        m_OTAExpectedCfgInfo = m_excelBook.GetRangeValue((resultRowNo+m_iLineNoOfPDU-1),(resultColNo-1));
        m_excelBook.GetRange((resultRowNo+m_iLineNoOfPDU-1),(resultColNo+1));
        if(0 == m_OTAExpectedCfgInfo.Compare(""))
        {
            if(0 == m_OTAActualCfgInfo.Compare(""))
            {
                m_excelBook.SetRangeValue("PASS");
            }
            else
            {
                m_excelBook.SetRangeBgColor(RANGE_BG_COLOR);
                m_excelBook.SetRangeValue("FAIL");
            }
        }
        else
        {   
            if(0 == m_OTAActualCfgInfo.Compare(""))
            {
                m_excelBook.SetRangeBgColor(RANGE_BG_COLOR);
                m_excelBook.SetRangeValue("FAIL");
            }
            else
            {
                if(CheckOTAConsistency(m_OTAExpectedCfgInfo,m_OTAActualCfgInfo))
                {
                    m_excelBook.SetRangeValue("PASS");
                }
                else
                {
                    m_excelBook.SetRangeBgColor(RANGE_BG_COLOR);
                    m_excelBook.SetRangeValue("FAIL");
                }
            }
        }
    }
    else
    {   // EMPTY_IN_INBOX
        resultColNo = GetStartColNo(m_iRowNo,m_iColNo,MS_OTA_INFO_COLUMN);
        m_OTAExpectedCfgInfo = m_excelBook.GetRangeValue((resultRowNo+m_iLineNoOfPDU-1),(resultColNo-1));
        m_excelBook.GetRange((resultRowNo+m_iLineNoOfPDU-1),(resultColNo+1));
        if(0 == m_OTAExpectedCfgInfo.Compare(""))
        {
            m_excelBook.SetRangeValue("PASS");
        }
        else
        {
            m_excelBook.SetRangeBgColor(RANGE_BG_COLOR);
            m_excelBook.SetRangeValue("FAIL");
        }
    }
}

/**************************************************************************************/
// Description      : clean the value of the range in the column named actual result. 
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::CleanActualResultInExcel()
{
    unsigned int theColNoOfactualResult = m_iStartColNoOfSms;
    // gain the start column number for pduMsg information.
    m_iStartColNoOfSms = GetStartColNo(m_iRowNo,m_iColNo,PDU_START_KEY_WORD);
    theColNoOfactualResult = GetStartColNo(m_iRowNo,m_iColNo,MS_OTA_INFO_COLUMN);
    m_iEndRowNoOfSms = GetEndRowNo(m_iStartRowNoOfSms,m_iStartColNoOfSms,NULL_END_KEY_WORD) - 1;  // 1: null line
    
    for(unsigned int iRow = 2; iRow <= m_iEndRowNoOfSms; iRow++)
    {
        m_excelBook.GetRange(iRow,theColNoOfactualResult);
        m_excelBook.SetRangeValue("");
    }
}

/**************************************************************************************/
// Description      : get the path of the test case file by the test project.
//--------------------------------------------------------------------------------------
// Input Parameter  : TEST_ITEM_E itemName  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::GetTestCaseFilePath(ATEST_ITEM_E itemName)
{
    switch (itemName)
    {
    case OTA:
        m_sTestCaseFilePath = CConPanCfgAccess::GetOTATestCasePath();
        break;
    case LOCATION:
        {
            CConPanCfgAccess::GetLocationTestCasePath(m_sFilePath);
            strcpy(m_sSourceFilePath,m_sFilePath);
            strcat(m_sSourceFilePath,"\\");    
        }
        break;
    case PB:
        {
            CConPanCfgAccess::GetPBTestCasePath(m_sFilePath);
            strcpy(m_sSourceFilePath,m_sFilePath);
            strcat(m_sSourceFilePath,"\\");    
        }
        break;
    default:
        break;
    }
}

/**************************************************************************************/
// Description      : get the path of the test result file by the test project.
//--------------------------------------------------------------------------------------
// Input Parameter  : TEST_ITEM_E itemName  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::GetTestResultFilePath(ATEST_ITEM_E itemName)
{
    // Initialize Time Object
    COleDateTime sTime;
    COleDateTime sDate;
    CString sTimeStr = "";
    CString sDateStr = "";
    CString sTimeStamp = "";
    // Initialize Path Variable
    CString sResultFilePath ="";
    
    sTime = COleDateTime::GetCurrentTime();
    sDate = COleDateTime::GetCurrentTime();
    sTimeStr = sTime.Format("%H%M%S");
    sDateStr = sDate.Format("%Y%m%d");
    
    // convert char* into CString
    CConPanCfgAccess::GetTestResultSavePath(m_sFilePath);
    sResultFilePath.Format(_T("%s"),m_sFilePath);

    sTimeStamp = sDateStr +"_" + sTimeStr;  // timeStamp

    if(!PathFileExists(sResultFilePath))  // create the main path of the result
    {
        CreateDirectory( sResultFilePath, NULL);
    }
    
    switch (itemName)
    {
    case OTA:
        {
            if(!PathFileExists(sResultFilePath+"\\OTA")) // create the sub path of the result
            {
                CreateDirectory((sResultFilePath+"\\OTA"),NULL);
            }
            m_sResultFilePath = sResultFilePath + "\\OTA\\checkedOTAResult" + sTimeStamp + ".xls";
        }
    	break;
    case LOCATION:
        {
            if(!PathFileExists(sResultFilePath+"\\Location")) // create the sub path of the result
            {
                CreateDirectory((sResultFilePath+"\\Location"),NULL);
            }
            CreateDirectory((sResultFilePath+"\\Location\\"+sTimeStamp),NULL);

            strcpy(m_sOutputFilePath,m_sFilePath);
            strcat(m_sOutputFilePath,"\\Location\\");
            strcat(m_sOutputFilePath,sTimeStamp);
            strcat(m_sOutputFilePath,"\\");
        }
    	break;
	//Add by shuyan.hu
    case PB:
        {
            //TODO: ADD CODE HERE
            if(!PathFileExists(sResultFilePath+"\\PB")) 
            {
                CreateDirectory((sResultFilePath+"\\PB"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\PB\\pb_f")) 
            {
                CreateDirectory((sResultFilePath+"\\PB\\pb_f"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\PB\\pb_con")) 
            {
                CreateDirectory((sResultFilePath+"\\PB\\pb_con"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\PB\\pb_res")) 
            {
                CreateDirectory((sResultFilePath+"\\PB\\pb_res"),NULL);   // create the sub path 
            }
			m_pbsourcepath = sResultFilePath+"\\PB\\pb_f\\" +sTimeStamp;
			m_pbrespath = sResultFilePath + "\\PB\\pb_res\\" +sTimeStamp;
			m_pbconfigpath = sResultFilePath+"\\PB\\pb_con\\";
			if(!PathFileExists(m_pbsourcepath)) // create the sub path of the result
            {
                CreateDirectory(m_pbsourcepath, NULL);      // create the sub path of the result
            }
			if(!PathFileExists(m_pbrespath))      
            {
                CreateDirectory(m_pbrespath, NULL);       // create the sub path of the result
            }
			
        }
    	break;
	case PB_CONF:
		{
            //TODO: ADD CODE HERE
			if(!PathFileExists(sResultFilePath+"\\PB")) 
            {
                CreateDirectory((sResultFilePath+"\\PB"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\PB\\pb_f")) 
            {
                CreateDirectory((sResultFilePath+"\\PB\\pb_f"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\PB\\pb_con")) 
            {
                CreateDirectory((sResultFilePath+"\\PB\\pb_con"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\PB\\pb_res")) 
            {
                CreateDirectory((sResultFilePath+"\\PB\\pb_res"),NULL);   // create the sub path 
            }
			m_pbconfigpath = sResultFilePath+"\\PB\\pb_con\\";
        }
		break;
	case R8:
		{
            //TODO: ADD CODE HERE
            if(!PathFileExists(sResultFilePath+"\\R8")) 
            {
                CreateDirectory((sResultFilePath+"\\R8"),NULL);       // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\R8\\R8_f")) 
            {
                CreateDirectory((sResultFilePath+"\\R8\\R8_f"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\R8\\R8_con")) 
            {
                CreateDirectory((sResultFilePath+"\\R8\\R8_con"),NULL);  // create the sub path 
            }
			if(!PathFileExists(sResultFilePath+"\\R8\\R8_res")) 
            {
                CreateDirectory((sResultFilePath+"\\R8\\R8_res"),NULL);  // create the sub path 
            }
            m_r8respath = sResultFilePath+"\\R8\\R8_res\\" +sTimeStamp;
			if(!PathFileExists(m_r8respath))             
            {
                CreateDirectory(m_r8respath, NULL);    // create the sub path of the result
            }  	
        }
    	break;
	case INPUT_METHOD:
		{
			//TODO: ADD CODE HERE
			if(!PathFileExists(sResultFilePath+"\\InputMethod")) 
            {
                CreateDirectory(sResultFilePath+"\\InputMethod",NULL); 
			}
			m_inputmedhodpath = sResultFilePath+"\\InputMethod";
		}
		break;
	case CALENDAR_LUNAR:
		{
			//TODO: ADD CODE HERE
			if(!PathFileExists(sResultFilePath+"\\Calendar")) 
            {
                CreateDirectory(sResultFilePath+"\\Calendar",NULL); 
			}

			if(!PathFileExists(sResultFilePath+"\\Calendar\\Lundar")) 
            {
                CreateDirectory(sResultFilePath+"\\Calendar\\Lundar",NULL); 
			}
			m_Calendarpath = sResultFilePath+"\\Calendar\\Lundar";
		}
		break;	
	case PLMN:
		{
			//TODO: ADD CODE HERE
			if(!PathFileExists(sResultFilePath+"\\PLMN")) 
            {
                CreateDirectory(sResultFilePath+"\\PLMN",NULL); 
			}
			m_PLMNpath = sResultFilePath+"\\PLMN";
		}
		break;
    default:
        break;
    }
}

/**************************************************************************************/
// Description      : check whether configured ota information is accord with the original config. 
//--------------------------------------------------------------------------------------
// Input Parameter  : 1.CString originalCfg
//                    2.CString actualCfg  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CATestPropertyPage::CheckOTAConsistency( CString originalCfg, /*IN Param*/
                                              CString actualCfg    /*IN Param*/)
{
    int iOriginalCfg = 0;
    int iActualCfg = 0;
    int iOTANumber = 1;
    int iOTAActNumber = 0;
    int iOriginal = 0;
    int iActual = 0;
    int iOriginalOTA = 0;
    int iActualOTA = 0;

    int matchResult = 0;
    char originalCfgArray[OTA_ITEM_TOTAL_NUM][OTA_ITEM_MAX_LENGTH]={0};
    char actualCfgArray[OTA_ITEM_TOTAL_NUM][OTA_ITEM_MAX_LENGTH]={0};
    char* originalCfgInfo = originalCfg.GetBuffer(originalCfg.GetLength());
    char* actualCfgInfo = actualCfg.GetBuffer(actualCfg.GetLength());

    originalCfg.ReleaseBuffer();
    actualCfg.ReleaseBuffer();

    iOriginalCfg = GetArrayFromCfgInfo(originalCfgInfo,originalCfgArray);
    iActualCfg = GetArrayFromCfgInfo(actualCfgInfo,actualCfgArray);
    iOTANumber = iOriginalCfg / EACH_OTA_ITEM_NUM;
    iOTAActNumber = iActualCfg / EACH_OTA_ITEM_NUM;

    if(iOTANumber != iOTAActNumber)
    {
        return FALSE;
    }
    
    for(iActualOTA=0;iActualOTA<iOTANumber;iActualOTA++)
    {       
        for(iOriginalOTA=0;iOriginalOTA<iOTANumber;iOriginalOTA++)
        {
            int matchNum = 0;
            for(iActual=0;iActual<EACH_OTA_ITEM_NUM;iActual++)
            {
                if(strcmp(actualCfgArray[iActual+iActualOTA*EACH_OTA_ITEM_NUM],originalCfgArray[iOriginal])==0)
                {
                    matchNum++;
                }
                iOriginal++;
            }
            if( matchNum == EACH_OTA_ITEM_NUM )
            {
                matchResult++;    // computing the number of matched ota config info 
                break;
            }
        }
        iOriginal = 0;
    }

    if(matchResult == iOTANumber && iOTANumber != 0)
    {
        return TRUE;
    }
	else
    {
        return FALSE;
    }
}

/**************************************************************************************/
// Description      : get the ota config array from the string named otaCfgStr.
//--------------------------------------------------------------------------------------
// Input Parameter  : char* otaCfgStr  
// Output Parameter : char otaCfgItemArray[][OTA_ITEM_MAX_LENGTH]
// Return Value     : int iItemOfOTA
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
unsigned int CATestPropertyPage::GetArrayFromCfgInfo( char* otaCfgStr,                            /*IN Param*/
                                                      char otaCfgItemArray[][OTA_ITEM_MAX_LENGTH] /*OUT Param*/ )
{
    unsigned int iItemOfOTA = 0;
    
    otaCfgStr = strtok(otaCfgStr,"\n");
    
    if(otaCfgStr!=NULL)
    {
        strcpy(otaCfgItemArray[iItemOfOTA],otaCfgStr);
    }
    
    while(otaCfgStr!=NULL)
    {
        iItemOfOTA++;
        otaCfgStr = strtok(NULL,"\n");
        if(otaCfgStr==NULL)
        {
            break;
        }
        strcpy(otaCfgItemArray[iItemOfOTA],otaCfgStr);
    }
    return iItemOfOTA;
}

/**************************************************************************************/
// Description      : run the OTA auto testing.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonRunOtaTest() 
{
    // check the ms simulator status.
    if(!CheckMSSimStatus())
    {
        return;
    }

    // reset the flag of the command line auto testing for single project testing.
    if(!m_bMultiProjSwitch)
    {
        CConPanCfgAccess::SetCMDAutoRunFlag(FALSE);
    }

    if(0 == m_iFileFormatSwitch)
    {   // process .xls file
        // send to simulator
        xSignalHeaderRec* sig_ptr;
        
        // create the signal
        InnerSCICreateSignal(
            &sig_ptr,
            APP_ATEST_IND,
            sizeof(ATEST_CP_DATA_T),
            P_APP);
        
        // set the sig_ptr's param
        ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_ENTER_IDLE_IND;
        
        // send the signal to MMI.
        SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);
    }
    else
    {   // process .xml file 
        CPduHexEncoder pPduHexEncoder;

        CString sXmlFolderPath = CConPanCfgAccess::GetOTAXmlTestCasePath();

        pPduHexEncoder.LoadAllXmlFile(sXmlFolderPath);
        if(pPduHexEncoder.GetErrorFormatSwitch())
        {
            AfxMessageBox("There are error format in the xml file.");
        }
        else
        {
            m_iFileFormatSwitch = 0;
            UpdateData(FALSE);
            AfxMessageBox("The test case file that format is XLS is generated.");
        }
    }
}

/**************************************************************************************/
// Description      : set the flag of the fixed-line mode.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnRadioLocationFl() 
{
    // set fixed-line mode
    m_iLocationSwitch = 2;
}

/**************************************************************************************/
// Description      : set the flag of the mobile phone mode.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnRadioLocationMp() 
{
    // set mobile phone mode
    m_iLocationSwitch = 1;
}

/**************************************************************************************/
// Description      : set the flag of all mode.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnRadioLocationAll() 
{
    // set all mode
    m_iLocationSwitch = 0;
}

/**************************************************************************************/
// Description      : run the location auto testing.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonLocationRun() 
{
    // check the ms simulator status.
    if(!CheckMSSimStatus())
    {
        return;
    }
    // reset the flag of the command line auto testing for single project testing.
    if(!m_bMultiProjSwitch)
    {
        CConPanCfgAccess::SetCMDAutoRunFlag(FALSE);
    }
    
    GetTestCaseFilePath(LOCATION);    // read the path of the test case file from ConPanCfg.ini.
    GetTestResultFilePath(LOCATION);  // read the path of the result file from ConPanCfg.ini.

    // send to simulator
    xSignalHeaderRec* sig_ptr;

    // create the signal
    InnerSCICreateSignal(
        &sig_ptr,
        APP_ATEST_IND,
        sizeof(ATEST_CP_DATA_T),
        P_APP);
    
    // set the sig_ptr's param
    ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_LOCATION_IND;
    ((ATEST_CP_DATA_T *)sig_ptr)->check_type = m_iLocationSwitch;      // location type
    memcpy(((ATEST_CP_DATA_T *)sig_ptr)->source_file_path,m_sSourceFilePath,MAX_PATH);// the path of the test case file
    memcpy(((ATEST_CP_DATA_T *)sig_ptr)->result_file_path,m_sOutputFilePath,MAX_PATH);  // the path of the result file    

    // send the signal to MMI.
    SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);   
}

/**************************************************************************************/
// Description      : run the PB code auto testing.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonPbRun() 
{
    // TODO: Add your control notification handler code here
    CString pbsourcefolder;
	CString pbresfolder;
    uint16  pbcasenum;
	
	GetTestResultFilePath(PB);
	//get pb case number
	memset(m_pbr8num, 0, MAX_PATH);
	CConPanCfgAccess::GetPBCaseNum(m_pbr8num);
	pbcasenum = atoi(m_pbr8num);
	
	
	// send to simulator
    xSignalHeaderRec* sig_ptr;
	
    // create the signal
    InnerSCICreateSignal(
        &sig_ptr,
        APP_ATEST_IND,
        sizeof(ATEST_CP_DATA_T),
        P_APP);
    
    // set the sig_ptr's param
    ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_PB_RUN_IND;
    ((ATEST_CP_DATA_T *)sig_ptr)->status_code = pbcasenum;      
    memcpy(((ATEST_CP_DATA_T *)sig_ptr)->source_file_path, m_pbsourcepath.GetBuffer(m_pbsourcepath.GetLength()),MAX_PATH);// the path of the test case file
    memcpy(((ATEST_CP_DATA_T *)sig_ptr)->result_file_path, m_pbrespath.GetBuffer(m_pbrespath.GetLength()),MAX_PATH);  // the path of the result file    
	memcpy(((ATEST_CP_DATA_T *)sig_ptr)->config_file_path, m_pbconfigpath.GetBuffer(m_pbconfigpath.GetLength()), MAX_PATH);  // the path of the config file  
 
    // send the signal to MMI.
    SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);  

}

/**************************************************************************************/
// Description      : check the status of the MS simulator.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CATestPropertyPage::CheckMSSimStatus()
{
    if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(sys_id))
    {
        MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
        return FALSE;
    }
    
    if (!((CConPanPropertySheet *)GetParent())->IsServiceAvailable(sys_id))
    {
        MessageBox("Sorry. No service available.", "Warning", MB_ICONWARNING);
        return FALSE;
	}

    return TRUE;
}

/**************************************************************************************/
// Description      : handle the process of the timer.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnTimer(UINT nIDEvent) 
{
    if( nIDEvent == 1 ) 
    {
        KillTimer(nIDEvent);
        
        if(m_iPduNumber == m_iPduNo)
        {
            m_iLineNoOfPDU++;
            m_iPduNo = 0;
            m_iPduNumber = 0;
            SetTimer(3, 4*1000, NULL);
        }
        else
        {
            SendOTAMsgToMSSim();
        }
    }
    else if( nIDEvent == 2 )
    {
        KillTimer(nIDEvent);
        GetTotalNumberOfPduInCurrentCase();
        SendOTAMsgToMSSim();
    }
    else if( nIDEvent == 3 ) // set the timer which configures the OTA information.
    {   
        KillTimer(nIDEvent);
        // send to simulator
        xSignalHeaderRec* sig_ptr;
        InnerSCICreateSignal(
            &sig_ptr,
            APP_ATEST_IND,
            sizeof(ATEST_CP_DATA_T),
            P_APP);

        // set the sig_ptr's param
        ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_SEND_OTA_IND;

        // send the signal to MMI.
        SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);
    }
	CPropertyPage::OnTimer(nIDEvent);
}

/**************************************************************************************/
// Description      : extra initialization for ATEST.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOL CATestPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    // Initialize COM Support Lib
    if(0 != CoInitialize(NULL))
    {
        AfxMessageBox("Fail in Initializing COM Support Lib!"); 
        exit(1); 
    }
    
    // Initialize variable
    m_iStartColNoOfSms = 1;
    m_iEndColNoOfSms = 1;
    m_iStartRowNoOfSms = 2;
    m_iEndRowNoOfSms = 2;
    m_iRowNo = 1;
    m_iColNo = 1;
    m_iPduNo = 0;
    m_iLineNoOfPDU = 0;
    m_bSaveFile = FALSE;
    m_iRowNumber = 0;
    m_iPduNumber = 0;
    m_sTestCaseFilePath = "";
    m_sResultFilePath = "";
	m_pbsourcepath = "";
	m_r8respath ="";
	m_pbrespath = "";
	m_pbconfigpath = "";
    m_iProjPos = 0;
    m_bMultiProjSwitch = FALSE;

    memset(m_iProjSelectStatus,0,sizeof(int)*MAX_NUM_OF_PROJ);
    memset(m_sPduData,0,sizeof(char)*STRING_MAX_LEN);
    memset(m_sSourceFilePath,0,sizeof(char)*MAX_PATH);
    memset(m_sOutputFilePath,0,sizeof(char)*MAX_PATH);
    memset(m_sFilePath,0,sizeof(char)*MAX_PATH);

    CConPanCfgAccess::GetTestResultSavePath(m_sFilePath);
    m_sResultMainPath.Format(_T("%s"),m_sFilePath);

    m_sOtaResultPath = STATIC_TEXT_HEADER_OF_OTA_RESULT + m_sResultMainPath + STATIC_TEXT_END_OF_OTA_RESULT;
	m_sLocResultPath = STATIC_TEXT_HEADER_OF_LOC_RESULT + m_sResultMainPath + STATIC_TEXT_END_OF_LOC_RESULT;
	m_MultiRresultPath = STATIC_TEXT_HEADER_OF_MULTILANG_RESULT + m_sResultMainPath;
    m_iVersionSwitch = CConPanCfgAccess::GetVERSwitchFlag();
    m_bOTASwitch = CConPanCfgAccess::GetOTAAutoRunFlag();
    m_bLocationSwitch = CConPanCfgAccess::GetLOCAutoRunFlag();
    m_bPBSwitch = CConPanCfgAccess::GetPBAutoRunFlag();
    m_bR8Switch = CConPanCfgAccess::GetR8AutoRunFlag();
	m_bIMTestSwitch = CConPanCfgAccess::GetInputMethodAutoRunFlag();
    
    if(m_iVersionSwitch==0)
    {
        // disable R8 mode in Chinese Version.
        GetDlgItem(IDC_CHECK_R8_CODE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_R8_RUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IM)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_IM_RUN)->EnableWindow(FALSE);
    }
    else
    {
        // disable location mode in Multilingual Version.
        GetDlgItem(IDC_BUTTON_LOCATION_RUN)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_LOCATION)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_LOCATION_FL)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_LOCATION_MP)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_LOCATION_ALL)->EnableWindow(FALSE); 
    }

	UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**************************************************************************************/
// Description      : get total number of the test case.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
unsigned int CATestPropertyPage::GetTotalNumberOfTestCase()
{
    // gain the start column number for pduMsg information.
    m_iStartColNoOfSms = GetStartColNo(m_iRowNo,m_iColNo,PDU_START_KEY_WORD);
    m_iEndRowNoOfSms = GetEndRowNo(m_iStartRowNoOfSms,m_iStartColNoOfSms,NULL_END_KEY_WORD) - 1;
    m_iRowNumber = m_iEndRowNoOfSms - ROW_NO_OF_THE_TITLE;

    return m_iRowNumber;
}

/**************************************************************************************/
// Description      : get total number of pdu in the current case.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
unsigned int CATestPropertyPage::GetTotalNumberOfPduInCurrentCase()
{
    m_iEndColNoOfSms = GetEndColNo(m_iStartRowNoOfSms+m_iLineNoOfPDU,m_iStartColNoOfSms,PDU_END_KEY_WORD) - 1;
    m_iPduNumber = m_iEndColNoOfSms - m_iStartColNoOfSms + 1;

    return m_iPduNumber;
}

/**************************************************************************************/
// Description      : set the flag of file format(XLS).
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnRadioXlsFormat() 
{
	// set file format to XLS
	m_iFileFormatSwitch = 0;
}

/**************************************************************************************/
// Description      : set the flag of file format(XML).
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnRadioXmlFormat() 
{
	// set file format to XML
	m_iFileFormatSwitch = 1;
}


/**************************************************************************************/
// Description      : auto test r8 coding test cases.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonR8Run() 
{
	// TODO: Add your control notification handler code here
	uint16  r8casenum;
	
	//get R8 case number
	memset(m_pbr8num, 0, MAX_PATH);
	CConPanCfgAccess::GetR8CaseNum(m_pbr8num);
	 r8casenum = atoi(m_pbr8num);

	GetTestResultFilePath(R8);
	// send to simulator
    xSignalHeaderRec* sig_ptr;
	
    // create the signal
    InnerSCICreateSignal(
        &sig_ptr,
        APP_ATEST_IND,
        sizeof(ATEST_CP_DATA_T),
        P_APP);
    
    // set the sig_ptr's param
    ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_R8_RUN_IND;
    ((ATEST_CP_DATA_T *)sig_ptr)->check_type = 1;  
    ((ATEST_CP_DATA_T *)sig_ptr)->status_code = r8casenum;
    memcpy(((ATEST_CP_DATA_T *)sig_ptr)->result_file_path,m_r8respath.GetBuffer(m_r8respath.GetLength()), MAX_PATH);  // the path of the result file    
	
    // send the signal to MMI.
    SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr); 
}


/**************************************************************************************/
// Description      : auto test pb coding test cases from config file.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonSelfTest() 
{
	// TODO: Add your control notification handler code here
    CFileFind confind;
	char m_file[MAX_PATH];
	FILE *fp_ptr;
	wchar fheader[2] ={0xfeff, 0x0000};

    memset(m_file, 0, MAX_PATH);    
	GetTestResultFilePath(PB_CONF);
	// send to simulator
    xSignalHeaderRec* sig_ptr;
	
    
	memcpy(m_file, m_pbconfigpath.GetBuffer(m_pbconfigpath.GetLength()),MAX_PATH);
	strcat(m_file, "pb_selftest.txt");
	//check the pb_selftest file is on
    if(!confind.FindFile(m_file))
	{
       fp_ptr = fopen(m_file, "wb+");
	   if(NULL == fp_ptr)
	   {
			return;
	   }
	   fputws(fheader, fp_ptr);
	   fclose(fp_ptr);
       AfxMessageBox("Pb test selftest case file does't exist!");
	}
	else
	{
		// create the signal
		InnerSCICreateSignal(
			&sig_ptr,
			APP_ATEST_IND,
			sizeof(ATEST_CP_DATA_T),
			P_APP);
		// set the sig_ptr's param
		((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_PB_SELF_IND;
		((ATEST_CP_DATA_T *)sig_ptr)->check_type = 1;          
		memcpy(((ATEST_CP_DATA_T *)sig_ptr)->config_file_path, m_pbconfigpath.GetBuffer(m_pbconfigpath.GetLength()),MAX_PATH);    // the path of the config file
		
		// send the signal to MMI.
        SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr); 
	}
	
}

/**************************************************************************************/
// Description      : run the combo auto testing.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : m_bMultiProjSwitch
// Author: Mingxuan.Zhu
// Note: MAX_NUM_OF_PROJ need to be modified, When the total number of the project is increased.       
/**************************************************************************************/
void CATestPropertyPage::OnButtonComboRun()
{
	// update the status of the auto testing project.
	UpdateData(TRUE);
    SetButtonStatusOfComboRun();
    m_bMultiProjSwitch = TRUE;

    // check the ms simulator status.
    if(!CheckMSSimStatus())
    {
        return;
    }
    
    // get the position of the selected auto testing project.
    while(m_iProjSelectStatus[m_iProjPos]==0 && m_iProjPos<MAX_NUM_OF_PROJ)
    {
        m_iProjPos++;
    }

    // Multi-Proj auto testing.
    if(m_iProjSelectStatus[m_iProjPos]==1)
    {
        switch (m_iProjPos)
        {
        case PROJ_OTA:
            {   // run ota auto testing.
                OnButtonRunOtaTest();                
            }
            break;
        case PROJ_LOC:
            {   // check whether version is Chinese or not.
                if(m_iVersionSwitch==0)
                {   // run location auto testing.
                    OnButtonLocationRun();
                }                
            }
            
            break;
        case PROJ_PB:
            {
                //TODO: ADD CODE HERE
				OnButtonPbRun();
            }
            break;
        case PROJ_R8:
            {   // check whether version is Multilingual or not.
                if(m_iVersionSwitch==1)
                {
                    //TODO: ADD CODE HERE
					OnButtonR8Run();
                }                
            }
            break;
		case PROJ_IM:
            {   // check whether version is Multilingual or not.
                if(m_iVersionSwitch==1)
                {
                    //TODO: ADD CODE HERE
					OnButtonInputMethodRun();
                }                
            }
            break;
        default:
            break;
        }
    }
}

/**************************************************************************************/
// Description      : set the status of the button in Chinese version.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : m_iVersionSwitch
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnRadioChinese() 
{
	//
    if(m_iVersionSwitch==1)
    {
        m_iVersionSwitch = 0;
        m_bOTASwitch = FALSE;
        m_bLocationSwitch = FALSE;
        m_bPBSwitch = FALSE;
	    m_bR8Switch = FALSE;
		m_bIMTestSwitch = FALSE;
        // disable R8 mode in Chinese Version.
        GetDlgItem(IDC_CHECK_R8_CODE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_R8_RUN)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_LOCATION_RUN)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_LOCATION)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_LOCATION_FL)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_LOCATION_MP)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_LOCATION_ALL)->EnableWindow(TRUE); 
		GetDlgItem(IDC_CHECK_IM)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_IM_RUN)->EnableWindow(FALSE);
        UpdateData(FALSE);
    }
}

/**************************************************************************************/
// Description      : set the status of the button in Multilingual version.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : m_iVersionSwitch
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnRadioMultilingual() 
{
    //
    if(m_iVersionSwitch==0)
    {   
        m_iVersionSwitch = 1;
        m_bOTASwitch = FALSE;
        m_bLocationSwitch = FALSE;
        m_bPBSwitch = FALSE;
	    m_bR8Switch = FALSE;
		m_bIMTestSwitch = FALSE;
        // disable Location mode.
        GetDlgItem(IDC_BUTTON_LOCATION_RUN)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_LOCATION)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_LOCATION_FL)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_LOCATION_MP)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_LOCATION_ALL)->EnableWindow(FALSE); 
        GetDlgItem(IDC_CHECK_R8_CODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_IM)->EnableWindow(TRUE);
		if(1 == m_MultiLangCurSel)
		{
       		GetDlgItem(IDC_BUTTON_R8_RUN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_IM_RUN)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_R8_RUN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_IM_RUN)->EnableWindow(TRUE);
		}
        UpdateData(FALSE); 
    } 	
}

/**************************************************************************************/
// Description      : set the status of the button in combo auto testing.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::SetButtonStatusOfComboRun()
{
    CConPanCfgAccess::SetVERSwitchFlag(m_iVersionSwitch);
    CConPanCfgAccess::SetOTAAutoRunFlag(m_bOTASwitch);
    CConPanCfgAccess::SetLOCAutoRunFlag(m_bLocationSwitch);
    CConPanCfgAccess::SetPBAutoRunFlag(m_bPBSwitch);
    CConPanCfgAccess::SetR8AutoRunFlag(m_bR8Switch);
	CConPanCfgAccess::SetInputMethodAutoRunFlag(m_bIMTestSwitch);
    m_iProjSelectStatus[PROJ_OTA] = m_bOTASwitch;
    m_iProjSelectStatus[PROJ_LOC] = m_bLocationSwitch;
    m_iProjSelectStatus[PROJ_PB] = m_bPBSwitch;
    m_iProjSelectStatus[PROJ_R8] = m_bR8Switch;
	m_iProjSelectStatus[PROJ_IM] = m_bIMTestSwitch;
}

/**************************************************************************************/
// Description      : reset the multi projects settings and send a message which informs 
//                    the auto testing of all projects is over.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CATestPropertyPage::ResetMultiProjSetting()
{
    // reset global variable.
    m_bMultiProjSwitch = FALSE;
    m_iProjPos = 0;
    AfxMessageBox( "Auto testing of all projects is over normally!", MB_ICONINFORMATION );
}

/**************************************************************************************/
// Description      : begin inputmethod auto testing.
//--------------------------------------------------------------------------------------
// Input Parameter  : BOOL TestKeyPadType:universe 键盘类型  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonInputMethodRun() 
{
	// TODO: Add your control notification handler code here
	BOOL TestKeyPadType = FALSE;
	GetTestResultFilePath(INPUT_METHOD);

	AfxMessageBox( "Inputmethod testing!");
	SendIMMsgToMSSim(TestKeyPadType);	//UNIVERSE 9键盘
}
/**************************************************************************************/
// Description      : send signal to atest to fetch the chars fron spim
//--------------------------------------------------------------------------------------
// Input Parameter  : BOOL TestKeyPadType:universe 键盘类型  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::SendIMMsgToMSSim(BOOL TestKeyPadType) 
{
	// TODO: Add your control notification handler code here
	// send to simulator
    xSignalHeaderRec* sig_ptr;
	
    // create the signal
    InnerSCICreateSignal(
        &sig_ptr,
        APP_ATEST_IND,
        sizeof(ATEST_CP_DATA_T),
        P_APP);

    // set the sig_ptr's param
    ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_INPUTMETHOD_RUN_IND;
	((ATEST_CP_DATA_T *)sig_ptr)->check_type = TestKeyPadType;
	
	 memcpy(((ATEST_CP_DATA_T *)sig_ptr)->result_file_path, m_inputmedhodpath.GetBuffer(m_inputmedhodpath.GetLength()),MAX_PATH);  // the path of the result file    
	
	// send the signal to MMI.
    SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);  
}


/**************************************************************************************/
// Description      : input method testing
//--------------------------------------------------------------------------------------
// Input Parameter  : char *inputmethodMark,BOOL KeyPadFlag
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : m_TestIMType，b_KeyPadFlag
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::IMCodeTesting()
{
	if(!b_KeyPadFlag)
	{
		m_KeyPadType = "Typical";
	}
	else
	{
		m_KeyPadType = "Qwerty";
	}
	if(0 == m_TestIMType.Compare("CStar")) //CStar
	{
		if(!b_KeyPadFlag)//Typical
		{
			m_bIMKeypadSwitch = CSTAR_TYPICAL;
		}
		else //Qwerty
		{
			m_bIMKeypadSwitch = CSTAR_QWERTY;
		}
		DealCstarExcel(m_inputmedhodpath);
		IMTestingResult(m_inputmedhodpath);
	}
	else if(0 == m_TestIMType.Compare("Sogou")) //Sogou
	{
		if(!b_KeyPadFlag)//Typical
		{
			m_bIMKeypadSwitch = SOGOU_TYPICAL;
		}
		else //Qwerty
		{
			m_bIMKeypadSwitch = SOGOU_QWERTY;
		}
		DealSogouExcel(m_inputmedhodpath);
		IMTestingResult(m_inputmedhodpath);
	}
	else //other is not support
	{
		AfxMessageBox( "The Input Method is not support!", MB_ICONINFORMATION );
		return;
	}
}

/**************************************************************************************/
// Description      : handle the excel data of CSTAR
//--------------------------------------------------------------------------------------
// Input Parameter  : CString InputMethodpath,BOOL keypadflag
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : char TableKey_data_arr[MAX_PATH]
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::DealSogouExcel(CString InputMethodpath)
{	
	FILE *SogouTableResult_ptr;
	char *TableKey_ptr = NULL;
	char SogouExcelCfgPath[MAX_PATH]={0};
	int	LangBit = 0;
	int SogouRow = 0;
	int keynum = 0;
	uint16 sogou_table_char;
	uint16 key;
	BOOL Testing_Flag = FALSE;
	CString SogouTableResPath;
	CString Sogousheet;
	CString SelectsheetName;
	CString SogouSection;
	CString LaguageSection(m_sLangUpName);
	CString LaguageSheet(m_sLangSheetName);
	SogouTableResPath = InputMethodpath+"\\"+m_TestIMType+"\_"+m_KeyPadType+"\_Table_Multitap.ini";
	SogouTableResult_ptr = fopen(SogouTableResPath,"w+");
	if(NULL == SogouTableResult_ptr)
	{
		return;
	}
	if(!CConPanCfgAccess::GetImTestCasePath(m_bIMKeypadSwitch,SogouExcelCfgPath))
	{
		return;
	}
	else
	{
		if(!strcmp(SogouExcelCfgPath,""))
		{
			if(!b_KeyPadFlag)
			{
				AfxMessageBox( "Please set the Sogou Typical table path in ConPanCfg.ini !", MB_ICONINFORMATION );
			}
			else
			{
				AfxMessageBox( "Please set the Sogou Qwerty table path in ConPanCfg.ini !", MB_ICONINFORMATION );
			}
			return;
		}
	}

	while(!Testing_Flag)
	{
		LangBit = LaguageSheet.Find("&");
		if(!((LangBit>0) && (LangBit<20)))
		{
			Testing_Flag = TRUE;
			continue;
		}
		else
		{
			//写入section
			LangBit = LaguageSection.Find("&");
			SogouSection = LaguageSection.Left(LangBit);
			LaguageSection.Delete(0,LangBit+1);
			memset(TableKey_data_arr, 0, MAX_PATH);
			TableKey_ptr = TableKey_data_arr;
	   		memcpy(TableKey_ptr, (char*)(SogouSection.GetBuffer(SogouSection.GetLength())),SogouSection.GetLength());
			fprintf(SogouTableResult_ptr,"[%s]\n",TableKey_ptr);
			
			LangBit = LaguageSheet.Find("&");
			Sogousheet = LaguageSheet.Left(LangBit);
			LaguageSheet.Delete(0,LangBit+1);
			CString SogouTablePath(Sogousheet);			
			if(!SearchFileInFolder(SogouExcelCfgPath,SogouTablePath))
			{
				fprintf(SogouTableResult_ptr,"无此语言的对照表!\n");
				continue;
			}					
			m_excelBook.CreateExcelObject();
			m_excelBook.GetSelectedWorkbook(SogouTablePath);
			m_excelBook.GetWorksheets();
			if(!b_KeyPadFlag)
			{
				SelectsheetName = "9键";
			}
			else
			{
				SelectsheetName = "qwerty键盘";
			}
			if(!m_excelBook.GetSelectedSheet(SelectsheetName))
			{
				return;			
			}

			// 找到读取的开始位置
			if(!b_KeyPadFlag)//9键盘
			{
				if(!GetBeginPosition("智能数字键键位"))
				{
					if(!GetBeginPosition("数字键键位"))
					{
						AfxMessageBox( "请检查对照表，是否有:智能数字键键位或数字键键位!", MB_ICONINFORMATION );
						return;
					}
				}

				for (SogouRow = 0;;SogouRow++)
				{
					// 判断是否有数据
					memset(TableKey_data_arr, 0, MAX_PATH);
					TableKey_ptr = TableKey_data_arr;
					GetCellValue(SogouRow+m_bRow,m_bCol-FIRST_COL,TableKey_ptr);
					if(0 == TableKey_ptr[0])
					{
						break;
					}
					else
					{
						// 读取字符unicode码，转换成16进制数，并过滤掉标点和数字
						memset(TableKey_data_arr, 0, MAX_PATH);
						TableKey_ptr = TableKey_data_arr;
						GetCellValue(SogouRow+m_bRow,m_bCol+FIRST_COL,TableKey_ptr);
						sogou_table_char = ConvertCharToUint16(TableKey_ptr);
						if(!((sogou_table_char <= 0x0040) 
							|| (sogou_table_char >= 0x005b && sogou_table_char <= 0x0060) 
							|| (sogou_table_char >= 0x007b && sogou_table_char <= 0x00bf)
							|| (sogou_table_char >= 0x2000 && sogou_table_char <= 0x20b9) 
							|| (sogou_table_char >= 0x2400 && sogou_table_char <= 0x2426)))
						{
							//将字符unicode码写入
							memset(TableKey_data_arr, 0, MAX_PATH);
							TableKey_ptr = TableKey_data_arr;
							GetCellValue(SogouRow+m_bRow,m_bCol,TableKey_ptr);

							// 有些字符的键位的个数大于16个后用g~z进行添加，转换成从16开始增加
							if( TableKey_ptr[1] >= 0x0067 && TableKey_ptr[1] <= 0x007a)
							{
								key = TableKey_ptr[0] - 0x30;
								keynum = TableKey_ptr[1] - 0x67 + 0x10;
								fprintf(SogouTableResult_ptr,"key%d%x=",key,keynum);
							}
							else
							{
		 						fprintf(SogouTableResult_ptr,"key%s=",TableKey_ptr);
							}
								
							//将字符写入
							memset(TableKey_data_arr, 0, MAX_PATH);
							TableKey_ptr = TableKey_data_arr;
							GetCellValue(SogouRow+m_bRow,m_bCol+FIRST_COL,TableKey_ptr);
							if(NULL == strpbrk(TableKey_ptr,"0x"))
							{
								fprintf(SogouTableResult_ptr,"0x%s\n",TableKey_ptr);
							}
							else
							{
								fprintf(SogouTableResult_ptr,"%s\n",TableKey_ptr);
							}
						}
					}
				}
			}
			else //全键盘
			{
				if(!GetBeginPosition("全字母键键值"))
				{
					AfxMessageBox( "请检查对照表，是否有关键字:全字母键键值!", MB_ICONINFORMATION );
					return;
				}

				for (SogouRow = 0;;SogouRow++)
				{
					memset(TableKey_data_arr, 0, MAX_PATH);
					TableKey_ptr = TableKey_data_arr;
					GetCellValue(SogouRow+m_bRow,m_bCol,TableKey_ptr);
					//若读取此cell的数据为空，就结束本sheet
					if(0 == TableKey_ptr[0]) 
					{
						break;
					}
					else
					{
						//读取按键字符
						memset(TableKey_data_arr, 0, MAX_PATH);
						TableKey_ptr = TableKey_data_arr;
						GetCellValue(SogouRow+m_bRow,m_bCol+FIRST_COL,TableKey_ptr);
						sogou_table_char = ConvertCharToUint16(TableKey_ptr);

						//过滤数字和标点符号
						if(!((sogou_table_char <= 0x0040) 
							|| (sogou_table_char >= 0x005b && sogou_table_char <= 0x0060) 
							|| (sogou_table_char >= 0x007b && sogou_table_char <= 0x00bf) 
							|| (sogou_table_char >= 0x2000 && sogou_table_char <= 0x20b9) 
							|| (sogou_table_char >= 0x2400 && sogou_table_char <= 0x2426))) 
						{
							//将按键信息和字符写入
							memset(TableKey_data_arr, 0, MAX_PATH);
							TableKey_ptr = TableKey_data_arr;
							GetCellValue(SogouRow+m_bRow,m_bCol+THIRD_COL,TableKey_ptr);
							fprintf(SogouTableResult_ptr,"key%s=",TableKey_ptr);
							memset(TableKey_data_arr, 0, MAX_PATH);
							TableKey_ptr = TableKey_data_arr;
							GetCellValue(SogouRow+m_bRow,m_bCol+FIRST_COL,TableKey_ptr);

							//若有的字符uicode码存放的没有0x开头的话，就加上
							if(strstr(TableKey_ptr,"0x") == NULL)
							{
								fprintf(SogouTableResult_ptr,"0x%s\n",TableKey_ptr);
							}
							else
							{
								fprintf(SogouTableResult_ptr,"%s\n",TableKey_ptr);
							}
						}
					}
				}
			}
			m_excelBook.ReleaseExcelObjects();
		}
	}
	fclose(SogouTableResult_ptr);	
}

/**************************************************************************************/
// Description      : search file in the folder according to some key words
//--------------------------------------------------------------------------------------
// Input Parameter  : CString Filepath,CString& FileName
// Output Parameter : CString& FileName
// Return Value     : BOOL
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
BOOL CATestPropertyPage::SearchFileInFolder(CString Filepath,CString& FileName)
{
	WIN32_FIND_DATA find_data;
	HANDLE          find_handle;
	char PathBuffer[MAX_PATH]={0};
	CString FindFileName;

	memcpy(PathBuffer, (char*)(Filepath.GetBuffer(Filepath.GetLength())),Filepath.GetLength());	
    strcat(PathBuffer,"\\*"); 

	find_handle = FindFirstFile(PathBuffer, &find_data );
	
	if ( find_handle == INVALID_HANDLE_VALUE )	
	{
	    return FALSE;
	}
	else
	{
	  while( 0 != FindNextFile( find_handle,&find_data))
      {
         FindFileName = find_data.cFileName;
		 if(FindFileName.Find(FileName) != -1)
		 {
			FileName = Filepath +"\\"+ FindFileName;
			return TRUE;
		 }
      }
	}
	return FALSE;
}

/**************************************************************************************/
// Description      : handle the excel data of CSTAR
//--------------------------------------------------------------------------------------
// Input Parameter  : CString InputMethodpath,BOOL keypadflag
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : char TableKey_data_arr[MAX_PATH]
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::DealCstarExcel(CString InputMethodpath)
{	
	char CStarExcelCfgPath[MAX_PATH]={0};
	char *TableKey_ptr = NULL;
	FILE *CStarTableResult_ptr;
	BOOL Find_Flag = FALSE;
	BOOL Testing_Flag = FALSE;	
	uint16	cstar_table_char = 0;
	uint16	keynum = 0;
	uint16	key = 0;
	int	i = 0;
	int	LangBit = 0;
	int	SectionBit = 0;
	int cstar_row = 0;
	CString cstarsheet;
	CString CstarSection;
	CString CStarTableResPath;
	CString LaguageSection(m_sLangUpName);
	CString LaguageSheet(m_sLangSheetName);

	// 创建结果存放文件
	CStarTableResPath = InputMethodpath+"\\"+m_TestIMType+"\_"+m_KeyPadType+"\_Table_Multitap.ini";
	CStarTableResult_ptr = fopen(CStarTableResPath,"w+");
	if(NULL == CStarTableResult_ptr)
	{
		return;
	}

	//获取对照表路径
	if(!CConPanCfgAccess::GetImTestCasePath(m_bIMKeypadSwitch,CStarExcelCfgPath))
	{
		return;
	}
	else
	{
		if(!strcmp(CStarExcelCfgPath,""))
		{
			if(!b_KeyPadFlag)
			{
				AfxMessageBox( "Please set the CStar Typical table path in ConPanCfg.ini !", MB_ICONINFORMATION );
			}
			else
			{
				AfxMessageBox( "Please set the CStar Qwerty table path in ConPanCfg.ini !", MB_ICONINFORMATION );
			}
			return;
		}
	}

	CString CStarPath(CStarExcelCfgPath);
	m_CStarExcelFilePath = CStarPath;
	m_excelBook.CreateExcelObject();
	m_excelBook.GetSelectedWorkbook(m_CStarExcelFilePath);
	m_excelBook.GetWorksheets();
	
	while(!Testing_Flag)
	{
		LangBit = LaguageSheet.Find("&");
		if(!((LangBit>0) && (LangBit<20)))
		{
			Testing_Flag = TRUE;
			continue;
		}
		else
		{
			//写入section
			SectionBit = LaguageSection.Find("&");
			CstarSection = LaguageSection.Left(SectionBit);
			LaguageSection.Delete(0,SectionBit+1);
			memset(TableKey_data_arr, 0, MAX_PATH);
			TableKey_ptr = TableKey_data_arr;
	   		memcpy(TableKey_ptr, (char*)(CstarSection.GetBuffer(CstarSection.GetLength())),CstarSection.GetLength());
			fprintf(CStarTableResult_ptr,"[%s]\n",TableKey_ptr);
		
			//获取sheet名
			cstarsheet = LaguageSheet.Left(LangBit);
			LaguageSheet.Delete(0,LangBit+1);
			memset(TableKey_data_arr, 0, MAX_PATH);
			TableKey_ptr = TableKey_data_arr;
   			memcpy(TableKey_ptr, (char*)(cstarsheet.GetBuffer(cstarsheet.GetLength())),cstarsheet.GetLength());

			// 若是English，Hinglish，Indonesian，Malay，Swahili，Tagalog当标准英语处理
			if(!(strcmpi(TableKey_ptr,"English") 
				&& strcmpi(TableKey_ptr,"Hinglish")
				&& strcmpi(TableKey_ptr,"Indonesian")
				&& strcmpi(TableKey_ptr,"Malay")
				&& strcmpi(TableKey_ptr,"Swahili")
				&& strcmpi(TableKey_ptr,"Tagalog")))
			{
				for(i = 0;i < MAX_ABC_CHAR_NUM;i++)
				{
					fprintf(CStarTableResult_ptr,"key%s=",CStarEnglishQwertyKeyMap[i].CStar_English_key);
					fprintf(CStarTableResult_ptr,"%s\n",CStarEnglishQwertyKeyMap[i].CStar_English_char);
				}
			}
			else
			{
 				if(!m_excelBook.GetSelectedSheet(cstarsheet))
				{
					fprintf(CStarTableResult_ptr,"对照表中无此语言对应的sheet!\n");
					continue;
				}
				else
				{
 					if(!GetBeginPosition("键位"))
					{	
						AfxMessageBox( "Please check the table data!", MB_ICONINFORMATION );
						return;
 					}
 					keynum = 1;
					
					for (cstar_row = 0;;cstar_row++)
					{
						// 判断是否有数据
						memset(TableKey_data_arr, 0, MAX_PATH);
						TableKey_ptr = TableKey_data_arr;
						GetCellValue(cstar_row+m_bRow,m_bCol,TableKey_ptr);
						if(0 == TableKey_ptr[0])
						{
							break;
						}
						else
						{
							// 读取字符unicode码，转换成16进制数，并过滤掉标点和数字
							memset(TableKey_data_arr, 0, MAX_PATH);
							TableKey_ptr = TableKey_data_arr;
							GetCellValue(cstar_row+m_bRow,m_bCol+FIRST_COL,TableKey_ptr);
							cstar_table_char = ConvertCharToUint16(TableKey_ptr);
							if(!((cstar_table_char <= 0x0040) 
								|| (cstar_table_char >= 0x005b && cstar_table_char <= 0x0060) 
								|| (cstar_table_char >= 0x007b && cstar_table_char <= 0x00bf)
								|| (cstar_table_char >= 0x2000 && cstar_table_char <= 0x20b9) 
								|| (cstar_table_char >= 0x2400 && cstar_table_char <= 0x2426))) 
							{
								//将按键信息写入
								memset(TableKey_data_arr, 0, MAX_PATH);
								TableKey_ptr = TableKey_data_arr;
								GetCellValue(cstar_row+m_bRow,m_bCol,TableKey_ptr);

								if(!b_KeyPadFlag)//9键盘
								{
									if(key == TableKey_ptr[0]-0x30)
									{
										keynum += 1;
									}
									else
									{
										keynum = 1;
									}
									key = TableKey_ptr[0]-0x30;
									fprintf(CStarTableResult_ptr,"key%d%x=",key,keynum);
								}
								else//全键盘
								{
									if(TableKey_ptr[0] >= 0x0041 && TableKey_ptr[0] <= 0x005a)
									{
										if(key == TableKey_ptr[0])
										{
											keynum += 1;
										}
										else
										{
											keynum = 1;
										}
										//写入按键信息
										CString CS_Key_Q(TableKey_ptr);
										CS_Key_Q.Delete(1,2);
										memset(TableKey_data_arr, 0, MAX_PATH);
										TableKey_ptr = TableKey_data_arr;
										strncpy(TableKey_ptr,(char *)(CS_Key_Q.GetBuffer(CS_Key_Q.GetLength())),CS_Key_Q.GetLength());
										fprintf(CStarTableResult_ptr,"key%s",TableKey_ptr);
										fprintf(CStarTableResult_ptr,"%x=",keynum);
										key = TableKey_ptr[0];
									}
									else
									{
										continue;
									}
								}
								//将字符unicode写入
								memset(TableKey_data_arr, 0, MAX_PATH);
								TableKey_ptr = TableKey_data_arr;
								GetCellValue(cstar_row+m_bRow,m_bCol+FIRST_COL,TableKey_ptr);
								fprintf(CStarTableResult_ptr,"%s\n",TableKey_ptr);
							}
						}
					}
				}
			}
		}
	}
	m_excelBook.ReleaseExcelObjects();
	fclose(CStarTableResult_ptr);	
}

/**************************************************************************************/
// Description      : fine the begin col and row
//--------------------------------------------------------------------------------------
// Input Parameter  : char *StringFlag
// Output Parameter : NONE
// Return Value     : BOOL
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : TableKey_data_arr
// Author: lin.chen
// Note:
/**************************************************************************************/
BOOL CATestPropertyPage::GetBeginPosition(char *StringFlag)
{
	BOOL Find_Flag;
	char *TableKey_ptr = NULL;
	for(m_bRow = 1;m_bRow < EXCEL_FIND_RANGE;m_bRow++)
	{
		Find_Flag = FALSE;
		for(m_bCol = 1;m_bCol < EXCEL_FIND_RANGE;m_bCol++)
		{
			memset(TableKey_data_arr, 0, MAX_PATH);
			TableKey_ptr = TableKey_data_arr;
			GetCellValue(m_bRow,m_bCol,TableKey_ptr);
			if(!(strcmp(TableKey_ptr,StringFlag)))
			{
				Find_Flag = TRUE;
				break;
			}
		}
		if(Find_Flag)
		{
			m_bRow++;
			break;
		}
	}			
	return Find_Flag;
}
/**************************************************************************************/
// Description      : run InputMethod Res compare auto testing.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString InputMethodpath
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : m_sLangUpName
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::IMTestingResult(CString InputMethodpath) 
{
	char LangMark[MAXLANGMARK] = {0};
	char KeyMark[MAXKEYCHAR] = {0};
	char MultitapKeychar[MAXKEYCHAR] = {0};
	char KeyNum[MAXKEYCHAR] = {0};
	char TableKeychar[MAXKEYCHAR] = {0};
	char KeyNumBig[MAXKEYCHAR] = {0};
	char KeyNumLit[MAXKEYCHAR] = {0};
	char *title = NULL;
	uint16 Multitapchar = 0;
	int num = 0;
	int LangBit = 0;
	int key = 0;
	FILE *InputMethod_ptr;
	CString lang_testing;
	CString Multitap_path;
	CString Table_path;
	CString KeyPadType;
	CString InputMethodResPath;
	CString LanguageFilePath;
	CString LaguageSection(m_sLangUpName);
	
	Multitap_path = InputMethodpath+"\\"+m_TestIMType+"\_"+m_KeyPadType+"\_Sp_Multitap.ini";
	Table_path = InputMethodpath+"\\"+m_TestIMType+"\_"+m_KeyPadType+"\_Table_Multitap.ini";		
	
	//生成最后结果文件夹
	InputMethodResPath = InputMethodpath+"\\"+m_TestIMType+"\ "+m_KeyPadType+"\ Result";
	if(!PathFileExists(InputMethodResPath)) 
 	{
     	CreateDirectory(InputMethodResPath,NULL); 
	}

	while(1)
	{
		//测试的语言名，逐个获取
		LangBit = LaguageSection.Find("&");
		if(0 >= LangBit)
		{
			break;
		}
		memset(LangMark,0,sizeof(LangMark));
		lang_testing = LaguageSection.Left(LangBit);
		strncat(LangMark,(char *)(lang_testing.GetBuffer(lang_testing.GetLength())),lang_testing.GetLength());

		//生成结果文件，最后的比较文件
		LanguageFilePath = InputMethodResPath+"\\"+lang_testing+"\.xls";
		InputMethod_ptr = fopen(LanguageFilePath, "w+");
		if(NULL == InputMethod_ptr)
		{
			return;
		}

		fprintf(InputMethod_ptr,"%s\t",LangMark);
		title = "Sp\tTable\tResult";
		fprintf(InputMethod_ptr,"%s\n",title);
		LaguageSection.Delete(0,LangBit+1);

		if(b_KeyPadFlag) //全键盘测试
		{
			for(key = 0;key < MAX_ABC_CHAR_NUM; key++)
			{
				for(num = 1;;num++)
				{
					//为获取ini文件的信息，构造相关信息
					memset(KeyMark,0,sizeof(KeyMark));
					strncpy(KeyMark, "key", strlen("key"));
					strncat(KeyMark, QwertyKeyMap[key].Qwerty_key,strlen(QwertyKeyMap[key].Qwerty_key));
					memset(KeyNum,0,sizeof(KeyNum));
					::itoa(num,KeyNum,16);
					strncat(KeyMark, KeyNum,strlen(KeyNum));
					
					//从ini获得数据
					memset(MultitapKeychar,0,sizeof(MultitapKeychar));
					GetPrivateProfileString(LangMark, KeyMark, "", MultitapKeychar, MAXFILEPATH, Multitap_path);
					Multitapchar = ConvertCharToUint16(MultitapKeychar);
					if((Multitapchar >= 0x2000 && Multitapchar <= 0x20b9) || (Multitapchar >= 0x2400 && Multitapchar <= 0x2426)) 
					{
						memset(MultitapKeychar,0,sizeof(MultitapKeychar));
					}
					
					memset(TableKeychar,0,sizeof(TableKeychar));
					GetPrivateProfileString(LangMark, KeyMark, "", TableKeychar, MAXFILEPATH, Table_path);
					
					if( (0 == MultitapKeychar[0]) && (0 == TableKeychar[0]))
					{
						break;
					}
					else
					{
 						fprintf(InputMethod_ptr,"%s",KeyMark);
						fprintf(InputMethod_ptr,"\t%s",MultitapKeychar);
						fprintf(InputMethod_ptr,"\t%s",TableKeychar);

						//对数据进行部分大小写的比较
						if(!(strcmpi(MultitapKeychar,TableKeychar)))
							title = "True";
						else
							title = "False";
						fprintf(InputMethod_ptr,"\t%s\n",title);
					}
				}
			}
		}
		else  //9键盘测试
		{
			for(key = 0; key <= 9;key++)
			{
				for(num = 1;;num++)
				{
					//为获取ini文件的信息，构造相关信息
					memset(KeyMark,0,sizeof(KeyMark));
					strncpy(KeyMark, "key", strlen("key"));
					memset(KeyNum,0,sizeof(KeyNum));
					::itoa(key,KeyNum,10);
					strncat(KeyMark, KeyNum, strlen(KeyNum));
					memset(KeyNum,0,sizeof(KeyNum));
					::itoa(num,KeyNum,16);

					//从ini获得数据
					memset(KeyNumBig,0,sizeof(KeyNumBig));
					strncpy(KeyNumBig, KeyMark,strlen(KeyMark));
					strncat(KeyNumBig, KeyNum,strlen(KeyNum));
					memset(MultitapKeychar,0,sizeof(MultitapKeychar));
					GetPrivateProfileString(LangMark, KeyNumBig, "", MultitapKeychar, MAXFILEPATH, Multitap_path);
					Multitapchar = ConvertCharToUint16(MultitapKeychar);
					if((Multitapchar >= 0x2000 && Multitapchar <= 0x20b9) || (Multitapchar >= 0x2400 && Multitapchar <= 0x2426)) 
					{
						memset(MultitapKeychar,0,sizeof(MultitapKeychar));
					}

					//9键盘的对照表中字符大于10个的时候使用大写A~Z来表示，转换成小写
					memset(KeyNumLit,0,sizeof(KeyNumLit));
					strncpy(KeyNumLit, KeyMark,strlen(KeyMark));
					strncat(KeyNumLit, (char*)strupr(KeyNum),strlen((char*)strupr(KeyNum)));
					memset(TableKeychar,0,sizeof(TableKeychar));
					GetPrivateProfileString(LangMark, KeyNumLit, "", TableKeychar, MAXFILEPATH, Table_path);
				
					if( (MultitapKeychar[0] == 0) && (TableKeychar[0] == 0 ))
					{
						break;
					}
					else
					{
						fprintf(InputMethod_ptr,"%s",KeyNumBig);
						fprintf(InputMethod_ptr,"\t%s",MultitapKeychar);
						fprintf(InputMethod_ptr,"\t%s",TableKeychar);

						//对数据进行部分大小写的比较
						if(!(strcmpi(MultitapKeychar,TableKeychar)))
							title = "True";
						else
							title = "False";
						fprintf(InputMethod_ptr,"\t%s\n",title);
					}
				}
			}
		}
		fclose(InputMethod_ptr);
	}
}

/**************************************************************************************/
// Description      : get the cell data of the excel 
//--------------------------------------------------------------------------------------
// Input Parameter  : long cell_row,long cell_col, char *GetValue_ptr
// Output Parameter : NONE
// Return Value     : BOOL
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
BOOL CATestPropertyPage::GetCellValue(long cell_row,long cell_col, char *GetValue_ptr)
{
	CString cell_data;

	if(NULL == GetValue_ptr)
	{
		return FALSE;
	}
	m_excelBook.GetRange(cell_row, cell_col);
	cell_data = m_excelBook.GetRangeValue(cell_row,cell_col);
	memset(GetValue_ptr, 0, MAX_PATH);
    memcpy(GetValue_ptr, (char*)(cell_data.GetBuffer(cell_data.GetLength())), cell_data.GetLength());

	return TRUE;
}

/**************************************************************************************/
// Description      : convert char to uint16
//--------------------------------------------------------------------------------------
// Input Parameter  : char *keychar 
// Output Parameter : NONE
// Return Value     : keychardata
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
uint16 CATestPropertyPage::ConvertCharToUint16(char *keychar)
{
	uint16 keychardata =0,bit = 0;
	uint16 Bitvalue[4] = {0};
	
	for(bit = 2;bit <= 5;bit++)
	{
		if( keychar[bit] >= UNICODEZERO && keychar[bit] <= UNICODENINE )  //1~9
		{
			Bitvalue[bit-2] = (uint16)(keychar[bit])-NUMGRADE;
		}
		else if( keychar[bit] >= UNICODEAL && keychar[bit] <= UNICODEFL ) //a~z
		{
			Bitvalue[bit-2] = (uint16)(keychar[bit])-BIGGRADE;
		}
		else if( keychar[bit] >= UNICODEAB && keychar[bit] <= UNICODEFB ) //A~Z
		{
			Bitvalue[bit-2] = (uint16)(keychar[bit])-LITTERGRADE;
		}
	}
	keychardata = THREEGRADE*Bitvalue[0] + TWOGRADE*Bitvalue[1] + ONEGRADE*Bitvalue[2] + Bitvalue[3];
	return keychardata;
}


/**************************************************************************************/
// Description      : Calendar Lunar Run
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonCalendarLunarRun() 
{
	// TODO: Add your control notification handler code here
	GetTestResultFilePath(CALENDAR_LUNAR);

	AfxMessageBox( "Calendar Lunar testing!");
	SendCalendarMsgToMSSim();	//UNIVERSE 9键盘	
}

/**************************************************************************************/
// Description      : send signal to atest to test Calendar lunar
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE 
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::SendCalendarMsgToMSSim() 
{
	// TODO: Add your control notification handler code here
	// send to simulator
    xSignalHeaderRec* sig_ptr;
	
    // create the signal
    InnerSCICreateSignal(
        &sig_ptr,
        APP_ATEST_IND,
        sizeof(ATEST_CP_DATA_T),
        P_APP);

    // set the sig_ptr's param
    ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_CALENDAR_RUN_IND;

	memcpy(((ATEST_CP_DATA_T *)sig_ptr)->result_file_path, m_Calendarpath.GetBuffer(m_Calendarpath.GetLength()),MAX_PATH);  // the path of the result file    
	
	// send the signal to MMI.
    SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);  
}

/**************************************************************************************/
// Description      : get the lundar data from the comparition table
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE 
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::Atest_CmpLundarData()
{
	FILE *Result_ptr = NULL;
	uint16	solar_year = 0;
    uint8	solar_month = 0;
    uint8	solar_day = 0;
    uint16	m_lunar_year_ptr = 0;
    uint8	m_lunar_month_ptr = 0;
    uint8	m_lunar_day_ptr = 0;
	uint16	a_lunar_year_ptr = 0;
    uint8	a_lunar_month_ptr = 0;
    uint8	a_lunar_day_ptr = 0;
	BOOL	CmpResFlag = FALSE;
	char	CmpTablePath[MAX_PATH] = {0};
	char	ResultPath[MAX_PATH] = {0};
	char	NewResultPath[MAX_PATH] = {0};
	char	YearInfo[20] = {0};
	char	solarDateInfo[20] = {0};
	char	m_lunarDateInfo[20] = {0};
	char	a_lunarDateInfo[20] = {0};

	if(!PathFileExists(m_Calendarpath+"\\Result")) 
    {
        CreateDirectory(m_Calendarpath+"\\Result",NULL); 
	}
	CString LundarResPath = m_Calendarpath+"\\Result";
	CString LundarPassPath = LundarResPath+"\\PASS";
	CString LundarFailPath = LundarResPath+"\\FAIL";
	
	CConPanCfgAccess::GetCalendarComparitionTablePath(CmpTablePath);	//对照表
	CString AtestLunarPath(CmpTablePath);
	CString MocorLunarPath = m_Calendarpath+"\\Lunar_Mocor.ini";		//从平台获得的农历表A	

	for(solar_year = ATEST_YEAR_START ;solar_year <= ATEST_YEAR_END;solar_year++)
	{
		CmpResFlag = TRUE;
		sprintf(ResultPath,"%s\\%d.xls",LundarResPath,solar_year);
		Result_ptr = fopen(ResultPath, "w+");
		if(NULL == Result_ptr)
		{
			return;
		}
		fprintf(Result_ptr,"Solar\tLunar(Mocor)\tLunar(Atest)\tResult\n");

		sprintf(YearInfo,"%d",solar_year);	
		for(solar_month = ATEST_MONTH_START;solar_month<=ATEST_MONTH_END;solar_month++)
		{
			for(solar_day = ATEST_DAY_START; solar_day <= ATEST_DAY_END; solar_day++)
			{
				sprintf(solarDateInfo,"%d-%d-%d",solar_year,solar_month,solar_day);	
				memset(m_lunarDateInfo,0,20);
				memset(a_lunarDateInfo,0,20);
				GetPrivateProfileString(YearInfo, solarDateInfo, "", m_lunarDateInfo, MAXFILEPATH, MocorLunarPath);	
				GetPrivateProfileString(YearInfo, solarDateInfo, "", a_lunarDateInfo, MAXFILEPATH, AtestLunarPath);

				if( (0 == m_lunarDateInfo[0]) && (0 == a_lunarDateInfo[0]))
				{
					break;
				}
				else
				{
					if( (0 != m_lunarDateInfo[0]) && (0 != a_lunarDateInfo[0]))
					{
						if(!(strcmpi(m_lunarDateInfo,a_lunarDateInfo)))
						{
							fprintf(Result_ptr,"%s\t%s\t%s\tTRUE\n",solarDateInfo,m_lunarDateInfo,a_lunarDateInfo);
						}
						else
						{
							CmpResFlag = FALSE;
							fprintf(Result_ptr,"%s\t%s\t%s\tFAIL\n",solarDateInfo,m_lunarDateInfo,a_lunarDateInfo);
						}
					}
					else if(0 == m_lunarDateInfo[0])
					{
						CmpResFlag = FALSE;
						fprintf(Result_ptr,"%s\t\t%s\tFAIL\n",solarDateInfo,a_lunarDateInfo);
					}
					else
					{
						CmpResFlag = FALSE;
						fprintf(Result_ptr,"%s\t%s\t\tFAIL\n",solarDateInfo,m_lunarDateInfo);
					}
				}	
			}	
		}
		fclose(Result_ptr);

		if(CmpResFlag)
		{
			if(!PathFileExists(LundarPassPath)) 
			{
				CreateDirectory(LundarPassPath,NULL); 
			}
			sprintf(NewResultPath,"%s\\%d.xls",LundarPassPath,solar_year);
			MoveFileA(ResultPath,NewResultPath);
		}
		else
		{
			if(!PathFileExists(LundarFailPath)) 
			{
				CreateDirectory(LundarFailPath,NULL); 
			}
			sprintf(NewResultPath,"%s\\%d.xls",LundarFailPath,solar_year);
			MoveFileA(ResultPath,NewResultPath);
		}
	}
}

/**************************************************************************************/
// Description      : get the plmn data From mmi
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE 
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::OnButtonPlmnRun() 
{
	// TODO: Add your control notification handler code here
	GetTestResultFilePath(PLMN);

	AfxMessageBox( "Getting PLMN data!");
	SendPLMNMsgToMSSim();
}

/**************************************************************************************/
// Description      : send signal to atest  to get the plmn data in mmi
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE 
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: lin.chen
// Note:
/**************************************************************************************/
void CATestPropertyPage::SendPLMNMsgToMSSim() 
{
	// TODO: Add your control notification handler code here
	// send to simulator
    xSignalHeaderRec* sig_ptr;
	
    // create the signal
    InnerSCICreateSignal(
        &sig_ptr,
        APP_ATEST_IND,
        sizeof(ATEST_CP_DATA_T),
        P_APP);

    // set the sig_ptr's param
    ((ATEST_CP_DATA_T *)sig_ptr)->signal_id = ATEST_PLMN_RUN_IND;

	memcpy(((ATEST_CP_DATA_T *)sig_ptr)->result_file_path, m_PLMNpath.GetBuffer(m_PLMNpath.GetLength()),MAX_PATH);  // the path of the result file    
	
	// send the signal to MMI.
    SendCPMsgToMMI((xSignalHeaderRec *)sig_ptr);  
}	
