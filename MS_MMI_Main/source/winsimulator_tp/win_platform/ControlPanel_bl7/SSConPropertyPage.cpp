// SSConPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConPanFunc.h"
#include "conpan.h"
#include "SSConPropertyPage.h"

#include "ConPanLayer.h"
#include "signals_header.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef enum {
    NOT_PROVIDED = 0, 
    NOT_REGISTERED = 1,
    NOT_ACTIVE = 2,
    ACTIVE_OPERATIVE =3 ,
    ACITVE_QUIESCENT =4 
} SS_SERVICE_STATE_E;

char *SSServiceStatusNames[ACITVE_QUIESCENT + 1] = {
        "NOT_PROVIDED", 
        "NOT_REGISTERED",
        "NOT_ACTIVE",
        "ACTIVE_OPERATIVE",
        "ACITVE_QUIESCENT" 
        
};


/////////////////////////////////////////////////////////////////////////////
// CSSConPropertyPage property page

IMPLEMENT_DYNCREATE(CSSConPropertyPage, CPropertyPage)

CSSConPropertyPage::CSSConPropertyPage() : CPropertyPage(CSSConPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CSSConPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pSigKeptForAct = NULL;
    m_apActAfterGetPwd = SS_UNKNOWN;
}

CSSConPropertyPage::~CSSConPropertyPage()
{
    //set status to unknown ,signal if existed will be freeed.
    SetActAfterGetPwd(SS_UNKNOWN);
}

void CSSConPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSConPropertyPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSSConPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSSConPropertyPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSConPropertyPage message handlers


BOOL CSSConPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    // TODO: Add extra initialization here   

    char szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);
    m_csSSIniFile = szCurPath;
	m_csSSIniFile += "\\conpan_ss.ini";   
    
    InitializeSSIniFile(m_csSSIniFile);
    InitializeAllControls();
 
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSSConPropertyPage::InitializeSSIniFile(CString csFile)
{
    CFile fIniFile;
    CFileException feIniFile;
    if (fIniFile.Open((LPCTSTR) csFile, 
        CFile::modeRead | CFile::shareDenyNone,
        &feIniFile) )
    {
        fIniFile.Close();
    }
    else
    {
        //file not exist ,so create it.
        
        if (!fIniFile.Open((LPCTSTR) csFile, 
            CFile::modeRead | CFile::modeWrite | CFile::modeCreate,
            &feIniFile))
        {
            //failed to create.
            char szMsgError[512];
            CString csMsgError;
            feIniFile.GetErrorMessage(szMsgError, sizeof(szMsgError));
            csMsgError = szMsgError;
            MessageBox(csMsgError, "Error", MB_ICONSTOP);
            
            return FALSE;
        }
        fIniFile.Close();   //succeed to create file.
        
        //write prarams
        
        if (!m_ifSSIni.OpenIniFile((LPCTSTR) csFile, 
            &feIniFile))
        {
            char szMsgError[512];
            CString csMsgError;
            feIniFile.GetErrorMessage(szMsgError, sizeof(szMsgError));
            csMsgError = szMsgError;
            MessageBox(csMsgError, "Error", MB_ICONSTOP);
            
            return FALSE;
        }
        CString csSection;
        CString csVarName;
        
        csSection = "Call Forward";   
        m_ifSSIni.SetVarStr(csSection, "ACF_Status", "NOT_REGISTERED");
        m_ifSSIni.SetVarStr(csSection, "ACF_PhoneNumber", "88888888");        
        m_ifSSIni.SetVarStr(csSection, "CFU_Status", "NOT_REGISTERED");
        m_ifSSIni.SetVarStr(csSection, "CFU_PhoneNumber", "88888888");        
        m_ifSSIni.SetVarStr(csSection, "ACCF_Status", "NOT_REGISTERED");
        m_ifSSIni.SetVarStr(csSection, "ACCF_PhoneNumber", "88888888");        
        m_ifSSIni.SetVarStr(csSection, "CFB_Status", "NOT_REGISTERED");
        m_ifSSIni.SetVarStr(csSection, "CFB_PhoneNumber", "88888888");        
        m_ifSSIni.SetVarStr(csSection, "CFNRY_Status", "NOT_REGISTERED");        
        m_ifSSIni.SetVarStr(csSection, "CFNRY_PhoneNumber", "88888888");
        m_ifSSIni.SetVarStr(csSection, "CFNRY_NoReplyTime", "60");
        m_ifSSIni.SetVarStr(csSection, "CFNRC_Status", "NOT_REGISTERED");
        m_ifSSIni.SetVarStr(csSection, "CFNRC_PhoneNumber", "88888888");        
        
        csSection = "Call Barrier";
        m_ifSSIni.SetVarStr(csSection, "BOIC_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "BOIC_Password", "1111");
        m_ifSSIni.SetVarStr(csSection, "BAOC_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "BAOC_Password", "1111");
        m_ifSSIni.SetVarStr(csSection, "BOIC_ExHC_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "BOIC_ExHC_Password", "1111");
        m_ifSSIni.SetVarStr(csSection, "BAIC_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "BAIC_Password", "1111");
        m_ifSSIni.SetVarStr(csSection, "BICRoam_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "BICRoam_Password", "1111");
        m_ifSSIni.SetVarStr(csSection, "ABS_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "ABS_Password", "1111");
        m_ifSSIni.SetVarStr(csSection, "BarOutgoing_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "BarOutgoing_Password", "1111");
        m_ifSSIni.SetVarStr(csSection, "BarIncoming_Status", "NOT_ACTIVE");
        m_ifSSIni.SetVarStr(csSection, "BarIncoming_Password", "1111");
        
        
        csSection = "Line Identification Supplementary Service";
        m_ifSSIni.SetVarStr(csSection, "CLIP_Status", "ACTIVE_OPERATIVE");
        m_ifSSIni.SetVarStr(csSection, "CLIR_Status", "ACTIVE_OPERATIVE");
        m_ifSSIni.SetVarStr(csSection, "COLP_Status", "ACTIVE_OPERATIVE");
        m_ifSSIni.SetVarStr(csSection, "COLR_Status", "ACTIVE_OPERATIVE");
        
        csSection = "Call Wait";
        m_ifSSIni.SetVarStr(csSection, "CW_Status", "NOT_ACTIVE");
        
        m_ifSSIni.Close();
    }  
     
    return TRUE;
}

BOOL CSSConPropertyPage::FlushIniByIDC(UINT nIDC, CString &csText)
{
    ASSERT(nIDC != 0);
    BOOL bResult;
    bResult = TRUE;

    CString csSection;
    CString csVarName;

    switch (nIDC)
    {
    //call forward controls
    case IDC_COMBO_CF_1:
        csSection = "Call Forward";
        csVarName = "ACF_Status";
        break;
    case IDC_COMBO_CF_2:
        csSection = "Call Forward";
        csVarName = "CFU_Status";
        break;
    case IDC_COMBO_CF_3:
        csSection = "Call Forward";
        csVarName = "ACCF_Status";
        break;
    case IDC_COMBO_CF_4:
        csSection = "Call Forward";
        csVarName = "CFB_Status";
        break;
    case IDC_COMBO_CF_5:
        csSection = "Call Forward";
        csVarName = "CFNRY_Status";
        break;
    case IDC_COMBO_CF_6:
        csSection = "Call Forward";
        csVarName = "CFNRC_Status";
        break;
    case IDC_EDIT_CF_PHONUM_1:
        csSection = "Call Forward";
        csVarName = "ACF_PhoneNumber";
        break;
    case IDC_EDIT_CF_PHONUM_2:
        csSection = "Call Forward";
        csVarName = "CFU_PhoneNumber";
        break;
    case IDC_EDIT_CF_PHONUM_3:
        csSection = "Call Forward";
        csVarName = "ACCF_PhoneNumber";
        break;
    case IDC_EDIT_CF_PHONUM_4:
        csSection = "Call Forward";
        csVarName = "CFB_PhoneNumber";
        break;
    case IDC_EDIT_CF_PHONUM_5:
        csSection = "Call Forward";
        csVarName = "CFNRY_PhoneNumber";
        break;
    case IDC_EDIT_CF_PHONUM_6:
        csSection = "Call Forward";
        csVarName = "CFNRC_PhoneNumber";
        break;
   case IDC_EDIT_CF_NOREPTIM:
        csSection = "Call Forward";
        csVarName = "CFNRY_NoReplyTime";
        break;
    
    //call barrier
    case IDC_COMBO_CB_1:
        csSection = "Call Barrier";
        csVarName = "BOIC_Status";
        break;
    case IDC_COMBO_CB_2:
        csSection = "Call Barrier";
        csVarName = "BAOC_Status";
        break;
    case IDC_COMBO_CB_3:
        csSection = "Call Barrier";
        csVarName = "BOIC_ExHC_Status";
        break;
    case IDC_COMBO_CB_4:
        csSection = "Call Barrier";
        csVarName = "BAIC_Status";
        break;
    case IDC_COMBO_CB_5:
        csSection = "Call Barrier";
        csVarName = "BICRoam_Status";
        break;
    case IDC_COMBO_CB_6:
        csSection = "Call Barrier";
        csVarName = "ABS_Status";
        break;
    case IDC_COMBO_CB_7:
        csSection = "Call Barrier";
        csVarName = "BarOutgoing_Status";
        break;
    case IDC_COMBO_CB_8:
        csSection = "Call Barrier";
        csVarName = "BarIncoming_Status";
        break;
    case IDC_EDIT_CB_PWD_1:
        csSection = "Call Barrier";
        csVarName = "BOIC_Password";
        break;
    case IDC_EDIT_CB_PWD_2:
        csSection = "Call Barrier";
        csVarName = "BAOC_Password";
        break;
    case IDC_EDIT_CB_PWD_3:
        csSection = "Call Barrier";
        csVarName = "BOIC_ExHC_Password";
        break;
    case IDC_EDIT_CB_PWD_4:
        csSection = "Call Barrier";
        csVarName = "BAIC_Password";
        break;
    case IDC_EDIT_CB_PWD_5:
        csSection = "Call Barrier";
        csVarName = "BICRoam_Password";
        break;
   case IDC_EDIT_CB_PWD_6:
        csSection = "Call Barrier";
        csVarName = "ABS_Password";
        break;
   case IDC_EDIT_CB_PWD_7:
        csSection = "Call Barrier";
        csVarName = "BarOutgoing_Password";
        break;
   case IDC_EDIT_CB_PWD_8:
        csSection = "Call Barrier";
        csVarName = "BarIncoming_Password";
        break;
    //LISS
    case IDC_COMBO_LISS_1:
        csSection = "Line Identification Supplementary Service"; 
        csVarName = "CLIP_Status";
        break;
    case IDC_COMBO_LISS_2:
        csSection = "Line Identification Supplementary Service"; 
        csVarName = "CLIR_Status";
        break;
    case IDC_COMBO_LISS_3:
        csSection = "Line Identification Supplementary Service"; 
        csVarName = "COLP_Status";
        break;
    case IDC_COMBO_LISS_4:
        csSection = "Line Identification Supplementary Service"; 
        csVarName = "COLR_Status";
        break;
 case IDC_COMBO_CW_1:
        csSection = "Call Wait";   
        csVarName = "CW_Status";
        break;
        default:
        ASSERT(FALSE);
        bResult = FALSE;
        break;
    }

    if (bResult)
    {
        if (!(m_ifSSIni.SetVarStr(csSection, csVarName, csText) &&
             m_ifSSIni.Flush()))
        {
            bResult = FALSE;
        }
    }

    return bResult;

}

static BOOL MNStatusToMyStatus(int nMMSvrStatus, int &nMyStatus)
{
    nMyStatus = -1;
    BOOL bResult = TRUE;
    switch(nMMSvrStatus)
    {
    case MN_SS_NOT_PROVITION:
        nMyStatus = NOT_PROVIDED;
        break;
    case MN_SS_NOT_REGISTERED:
        nMyStatus = NOT_REGISTERED;
        break;
    case MN_SS_NOT_ACTIVE:
        nMyStatus = NOT_ACTIVE;
        break;
    case MN_SS_ACTIVE_OPERATIVE:
        nMyStatus = ACTIVE_OPERATIVE;
        break;
        
    case MN_SS_ACITVE_QUIESCENT:
        nMyStatus = ACITVE_QUIESCENT;
        break;
    default:
        bResult = FALSE;
        break;
    }
    
    return bResult;
    
}

static BOOL MyStatusToMNStatus(int nMyStatus,int &nMMSvrStatus)
{
    nMMSvrStatus = -1;
    BOOL bResult = TRUE;
    switch(nMyStatus)
    {
        
    case NOT_PROVIDED:
        nMMSvrStatus = MN_SS_NOT_PROVITION ;
        break;
    case NOT_REGISTERED:
        nMMSvrStatus = MN_SS_NOT_REGISTERED ;
        break;
    case NOT_ACTIVE:
        nMMSvrStatus = MN_SS_NOT_ACTIVE ;
        break;
    case ACTIVE_OPERATIVE:
        nMMSvrStatus = MN_SS_ACTIVE_OPERATIVE ;
        break;
    case ACITVE_QUIESCENT:
        nMMSvrStatus = MN_SS_ACITVE_QUIESCENT ;
        break;
    default:
        bResult = FALSE;
        break;
    }
    
    return bResult;
    
}

BOOL CSSConPropertyPage::SetIDCText(UINT nIDC, CString csText)
{
    BOOL bResult;
    bResult = FALSE;
    SetDlgItemText(nIDC, csText);
    bResult = FlushIniByIDC(nIDC, csText);
    
    return bResult;

}

BOOL CSSConPropertyPage::GetIDCText(UINT nIDC, CString &csText)
{
    BOOL bResult;
    bResult = FALSE;
    bResult = GetDlgItemText(nIDC, csText);
   
    return bResult;
}


BOOL CSSConPropertyPage::SetItemServiceStatus(int nMNSSType,
                                              int nMNSSSvrState)
{    
    BOOL bResult;
    CComboBox *pcbCombo;
    UINT nIDC;
    pcbCombo = NULL;
    ASSERT(GetIDCFromMNSSType(nMNSSType, nIDC));
    pcbCombo = (CComboBox *) GetDlgItem(nIDC);
    ASSERT(pcbCombo);   

    int nSvrState;

    MNStatusToMyStatus(nMNSSSvrState, nSvrState);
    ASSERT(nSvrState >= NOT_PROVIDED && nSvrState <= ACITVE_QUIESCENT);
    int nIndex;
    nIndex = pcbCombo->FindString(-1, SSServiceStatusNames[nSvrState]);
    ASSERT(nIndex >= 0);    
    
    bResult = TRUE;
    
    if (nIndex != pcbCombo->GetCurSel())
    {
        pcbCombo->SetCurSel(nIndex);
        CString csText;
        csText = SSServiceStatusNames[nSvrState];
        bResult = FlushIniByIDC(nIDC, csText);
    }
    return bResult;
}

BOOL CSSConPropertyPage::GetItemServiceStatus(int nMNSSType,
                                              int &nMNSSSvrStatus)
{

    CComboBox *pcbCombo;
    UINT nIDC;
    pcbCombo = NULL;
    ASSERT(GetIDCFromMNSSType(nMNSSType, nIDC));
    pcbCombo = (CComboBox *) GetDlgItem(nIDC);
    ASSERT(pcbCombo);   

    CString csCurString;
    pcbCombo->GetLBText(pcbCombo->GetCurSel(), csCurString);
    int nSvrState;
    nSvrState = -1;
    for (int i = NOT_PROVIDED; i < ACITVE_QUIESCENT + 1; i++)
    {
        if (stricmp(SSServiceStatusNames[i], csCurString) == 0)
        {
            nSvrState = i;
            break;
        }
    }  
    if (nSvrState == -1)
    {
        return FALSE;
    }
    else
    {
        MyStatusToMNStatus(nSvrState, nMNSSSvrStatus);
    }
    return TRUE;
}

BOOL CSSConPropertyPage::GetIDCFromMNSSType(int nMNSSType, UINT &nIDC)
{

    BOOL bResult;
    bResult = TRUE;
    nIDC = 0;
    switch (nMNSSType)
    {
        //Call Forward
    case MN_SS_ALL_FORWARD:
        nIDC = IDC_COMBO_CF_1;
        break;
    case MN_SS_CFU:
        nIDC = IDC_COMBO_CF_2;
        break;
    case MN_SS_ALL_COND_FORWARD:
        nIDC = IDC_COMBO_CF_3;
        break;
    case MN_SS_CFB:
        nIDC = IDC_COMBO_CF_4;
        break;
    case MN_SS_CFNRY:
        nIDC = IDC_COMBO_CF_5;
        break;
    case MN_SS_CFNRC:
        nIDC = IDC_COMBO_CF_6;
        break;
        
        
        //Call Barrier
    case MN_SS_BOIC:
        nIDC = IDC_COMBO_CB_1;
        break;  
    case MN_SS_BAOC:
        nIDC = IDC_COMBO_CB_2;
        break;
    case MN_SS_BOIC_EXHC:
        nIDC = IDC_COMBO_CB_3;
        break;
    case MN_SS_BAIC:
        nIDC = IDC_COMBO_CB_4;
        break;
    case MN_SS_BICROAM:
        nIDC = IDC_COMBO_CB_5;
        break;
    case MN_SS_ALL_BAR_SERVICE:
        nIDC = IDC_COMBO_CB_6;
        break;
    case MN_SS_BAR_OUTGOING:
        nIDC = IDC_COMBO_CB_7;
        break;
    case MN_SS_BAR_INCOMING:
        nIDC = IDC_COMBO_CB_8;
        break;

        //LISS
    case MN_SS_CLIP:
        nIDC = IDC_COMBO_LISS_1;
        break;                
    case MN_SS_CLIR:
        nIDC = IDC_COMBO_LISS_2;
        break;
    case MN_SS_COLP:
        nIDC = IDC_COMBO_LISS_3;
        break;
    case MN_SS_COLR:
        nIDC = IDC_COMBO_LISS_4;
        break;  
        
        //Call Wait
    case MN_SS_CW:
        nIDC = IDC_COMBO_CW_1;
        break;
    default:
        bResult = FALSE;
        break;        
    }

    return bResult;
}

void CSSConPropertyPage::SetActAfterGetPwd(int nActAfterGetPwd, void *pSig/* =NULL */)
{
    ACTION_AFTER_GET_PASSWORD_E apAction = (ACTION_AFTER_GET_PASSWORD_E) nActAfterGetPwd;
    ASSERT(apAction >= SS_UNKNOWN && 
            apAction <= SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY);    
    if (apAction == SS_UNKNOWN ||
        apAction == SS_MODIFYPWD_CHECK_NEWPWD_FIRSTLY || 
        apAction == SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY)
    {
        ASSERT(pSig == NULL);
    }    
    else
    {
        ASSERT(pSig != NULL);
    }

    if (apAction == SS_MODIFYPWD_CHECK_NEWPWD_FIRSTLY || 
        apAction == SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY)
    {
        m_apActAfterGetPwd = apAction;
        return ;
    }
    else
    {
        if (m_apActAfterGetPwd == SS_ACTIVATE)
        {
            delete ((MN_ACTIVATE_SS_REQ_T *) m_pSigKeptForAct);
        }
        else if (m_apActAfterGetPwd == SS_DEACTIVATE)
        {
                delete ((MN_DEACTIVATE_SS_REQ_T *) m_pSigKeptForAct);
        }
        else if (m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_OLDPWD ||
                m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_NEWPWD_FIRSTLY || 
                m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY)
        {
            delete ((MN_MODIFY_PASSWARD_REQ_T *) m_pSigKeptForAct);             
        }
        m_pSigKeptForAct = NULL;
    }
    m_apActAfterGetPwd = apAction;
    m_pSigKeptForAct = pSig;

}

BOOL CSSConPropertyPage::InitializeAllControls()
{

    BOOL bResult;
    bResult = TRUE;
    CComboBox *pcbCFs[6];
    CComboBox *pcbCBs[8];
    CComboBox *pcbLISSs[4];
    CComboBox *pcbCW;
    CEdit *pedCFPhoneNums[6];
    CEdit *pedCFNoRepTim;
    CEdit *pedCBPasswords[8];

    int i,j;


    pcbCFs[0] = (CComboBox *) GetDlgItem(IDC_COMBO_CF_1);
    pcbCFs[1] = (CComboBox *) GetDlgItem(IDC_COMBO_CF_2);
    pcbCFs[2] = (CComboBox *) GetDlgItem(IDC_COMBO_CF_3);
    pcbCFs[3] = (CComboBox *) GetDlgItem(IDC_COMBO_CF_4);
    pcbCFs[4] = (CComboBox *) GetDlgItem(IDC_COMBO_CF_5);
    pcbCFs[5] = (CComboBox *) GetDlgItem(IDC_COMBO_CF_6);

    pedCFPhoneNums[0] = (CEdit *) GetDlgItem(IDC_EDIT_CF_PHONUM_1);
    pedCFPhoneNums[1] = (CEdit *) GetDlgItem(IDC_EDIT_CF_PHONUM_2);
    pedCFPhoneNums[2] = (CEdit *) GetDlgItem(IDC_EDIT_CF_PHONUM_3);
    pedCFPhoneNums[3] = (CEdit *) GetDlgItem(IDC_EDIT_CF_PHONUM_4);
    pedCFPhoneNums[4] = (CEdit *) GetDlgItem(IDC_EDIT_CF_PHONUM_5);
    pedCFPhoneNums[5] = (CEdit *) GetDlgItem(IDC_EDIT_CF_PHONUM_6);

    pedCFNoRepTim = (CEdit *) GetDlgItem(IDC_EDIT_CF_NOREPTIM);

    pcbCBs[0] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_1);
    pcbCBs[1] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_2);
    pcbCBs[2] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_3);
    pcbCBs[3] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_4);
    pcbCBs[4] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_5);
    pcbCBs[5] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_6);
    pcbCBs[6] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_7);
    pcbCBs[7] = (CComboBox *) GetDlgItem(IDC_COMBO_CB_8);
    
    pedCBPasswords[0] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_1);
/*
    pedCBPasswords[1] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_2);
    pedCBPasswords[2] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_3);
    pedCBPasswords[3] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_4);
    pedCBPasswords[4] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_5);
    pedCBPasswords[5] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_6);
    pedCBPasswords[6] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_7);
    pedCBPasswords[7] = (CEdit *) GetDlgItem(IDC_EDIT_CB_PWD_8);
*/    
    pcbLISSs[0] = (CComboBox *) GetDlgItem(IDC_COMBO_LISS_1);
    pcbLISSs[1] = (CComboBox *) GetDlgItem(IDC_COMBO_LISS_2);
    pcbLISSs[2] = (CComboBox *) GetDlgItem(IDC_COMBO_LISS_3);
    pcbLISSs[3] = (CComboBox *) GetDlgItem(IDC_COMBO_LISS_4);

    pcbCW = (CComboBox *) GetDlgItem(IDC_COMBO_CW_1);

    ASSERT(pedCFNoRepTim);

    for (i = 0; i < 6; i++)
    {
        ASSERT(pcbCFs[i]);
        ASSERT(pedCFPhoneNums[i]);
        
    }
    for (i = 0; i < 8; i++)
    {
        ASSERT(pcbCBs[i]);
    }
    ASSERT(pedCBPasswords[0]);

    for (i = 0; i < 4; i++)
    {
        ASSERT(pcbLISSs[i]);
        
    }
    ASSERT(pcbCW);



    char *CF_ServiceStates[5] = { 
            "NOT_PROVIDED", 
            "NOT_REGISTERED",
            "NOT_ACTIVE",
            "ACTIVE_OPERATIVE",
            "ACITVE_QUIESCENT" };

    char *CB_ServiceStates[4] = { 
            "NOT_PROVIDED", 
            "NOT_ACTIVE",
            "ACTIVE_OPERATIVE",
            "ACITVE_QUIESCENT" };

     char *LISS_ServiceStates[3] = { 
            "NOT_PROVIDED", 
            "ACTIVE_OPERATIVE",
            "ACITVE_QUIESCENT" };

     char *CW_ServiceStates[4] = { 
            "NOT_PROVIDED", 
            "NOT_ACTIVE",
            "ACTIVE_OPERATIVE",
            "ACITVE_QUIESCENT" };

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 5; j++)
        {
            pcbCFs[i]->InsertString(-1, CF_ServiceStates[j]);
        }
    }

      for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            pcbCBs[i]->InsertString(-1, CB_ServiceStates[j]);
        }
    }

        for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            pcbLISSs[i]->InsertString(-1, LISS_ServiceStates[j]);
        }
    }

    for (j = 0; j < 4; j++)
        {
            pcbCW->InsertString(-1, CW_ServiceStates[j]);
        }
      
    CString csSection;
    CString csIniStr;
    int nIniInt;

    CString csCFStatus[6];
    CString csCBStatus[8];
    CString csLISSStatus[4];
    CString csCWStatus;

    CString csPhoneNumbers[6];    
    int nCFNRY_NoReplyTime;
    CString csPasswords[8];

    //open ini file to read params.
    CFileException feIniFile;
    if (!m_ifSSIni.OpenIniFile((LPCTSTR) m_csSSIniFile, &feIniFile))
    {
        bResult = FALSE;
    }
    csSection = "Call Forward";   

    csCFStatus[0] = m_ifSSIni.GetVarStr(csSection, "ACF_Status", csIniStr) ? csIniStr : "NOT_REGISTERED";
    csCFStatus[1] = m_ifSSIni.GetVarStr(csSection, "CFU_Status", csIniStr) ? csIniStr : "NOT_REGISTERED";
    csCFStatus[2] = m_ifSSIni.GetVarStr(csSection, "ACCF_Status", csIniStr) ? csIniStr : "NOT_REGISTERED";
    csCFStatus[3] = m_ifSSIni.GetVarStr(csSection, "CFB_Status", csIniStr) ? csIniStr : "NOT_REGISTERED";
    csCFStatus[4] = m_ifSSIni.GetVarStr(csSection, "CFNRY_Status", csIniStr) ? csIniStr : "NOT_REGISTERED";
    csCFStatus[5] = m_ifSSIni.GetVarStr(csSection, "CFNRC_Status", csIniStr) ? csIniStr : "NOT_REGISTERED";
    
    csPhoneNumbers[0] = m_ifSSIni.GetVarStr(csSection, "ACF_PhoneNumber", csIniStr) ? csIniStr : "88888888";
    csPhoneNumbers[1] = m_ifSSIni.GetVarStr(csSection, "CFU_PhoneNumber", csIniStr) ? csIniStr : "88888888";
    csPhoneNumbers[2] = m_ifSSIni.GetVarStr(csSection, "ACCF_PhoneNumber", csIniStr) ? csIniStr : "88888888";
    csPhoneNumbers[3] = m_ifSSIni.GetVarStr(csSection, "CFB_PhoneNumber", csIniStr) ? csIniStr : "88888888";
    csPhoneNumbers[4] = m_ifSSIni.GetVarStr(csSection, "CFNRY_PhoneNumber", csIniStr) ? csIniStr : "88888888";
    csPhoneNumbers[5] = m_ifSSIni.GetVarStr(csSection, "CFNRC_PhoneNumber", csIniStr) ? csIniStr : "88888888";

    nCFNRY_NoReplyTime = m_ifSSIni.GetVarInt(csSection, "CFNRY_NoReplyTime", nIniInt) ? nIniInt : 60;
   
    csSection = "Call Barrier";   

    csCBStatus[0] = m_ifSSIni.GetVarStr(csSection, "BOIC_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    csCBStatus[1] = m_ifSSIni.GetVarStr(csSection, "BAOC_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    csCBStatus[2] = m_ifSSIni.GetVarStr(csSection, "BOIC_ExHC_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    csCBStatus[3] = m_ifSSIni.GetVarStr(csSection, "BAIC_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    csCBStatus[4] = m_ifSSIni.GetVarStr(csSection, "BICRoam_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    csCBStatus[5] = m_ifSSIni.GetVarStr(csSection, "ABS_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    csCBStatus[6] = m_ifSSIni.GetVarStr(csSection, "BarOutgoing_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    csCBStatus[7] = m_ifSSIni.GetVarStr(csSection, "BarIncoming_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";
    
    csPasswords[0]    = m_ifSSIni.GetVarStr(csSection, "BOIC_Password", csIniStr) ? csIniStr : "1111";
    csPasswords[1]    = m_ifSSIni.GetVarStr(csSection, "BAOC_Password", csIniStr) ? csIniStr : "1111";
    csPasswords[2]    = m_ifSSIni.GetVarStr(csSection, "BOIC_Password", csIniStr) ? csIniStr : "1111";
    csPasswords[3]    = m_ifSSIni.GetVarStr(csSection, "BOIC_ExHC_Password", csIniStr) ? csIniStr : "1111";
    csPasswords[4]    = m_ifSSIni.GetVarStr(csSection, "BICRoam_Password", csIniStr) ? csIniStr : "1111";
    csPasswords[5]    = m_ifSSIni.GetVarStr(csSection, "ABS_Password", csIniStr) ? csIniStr : "1111";
    csPasswords[6]    = m_ifSSIni.GetVarStr(csSection, "BarOutgoing_Password", csIniStr) ? csIniStr : "1111";
    csPasswords[7]    = m_ifSSIni.GetVarStr(csSection, "BarIncoming_Password", csIniStr) ? csIniStr : "1111";

    csSection = "Line Identification Supplementary Service";   

    csLISSStatus[0] = m_ifSSIni.GetVarStr(csSection, "CLIP_Status", csIniStr) ? csIniStr : "ACTIVE_OPERATIVE";
    csLISSStatus[1] = m_ifSSIni.GetVarStr(csSection, "CLIR_Status", csIniStr) ? csIniStr : "ACTIVE_OPERATIVE";
    csLISSStatus[2] = m_ifSSIni.GetVarStr(csSection, "COLP_Status", csIniStr) ? csIniStr : "ACTIVE_OPERATIVE";
    csLISSStatus[3] = m_ifSSIni.GetVarStr(csSection, "COLR_Status", csIniStr) ? csIniStr : "ACTIVE_OPERATIVE";
    
     csSection = "Call Wait";   
     csCWStatus = m_ifSSIni.GetVarStr(csSection, "CW_Status", csIniStr) ? csIniStr : "NOT_ACTIVE";

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (stricmp(CF_ServiceStates[j], (LPCTSTR)csCFStatus[i]) == 0)
            {
                pcbCFs[i]->SetCurSel(j);
                break ;
            }
        }        
        pedCFPhoneNums[i]->SetWindowText(csPhoneNumbers[i]);
    } 
    
    CString csNoRepTim;
    csNoRepTim.Format("%d", nCFNRY_NoReplyTime);
    pedCFNoRepTim->SetWindowText(csNoRepTim);
    for (i = 0; i < 6; i++)
    {
        if (!(stricmp(CF_ServiceStates[ACTIVE_OPERATIVE], (LPCTSTR)csCFStatus[i]) == 0 ||
             stricmp(CF_ServiceStates[ACITVE_QUIESCENT], (LPCTSTR)csCFStatus[i]) == 0))
        {
            pedCFPhoneNums[i]->SetWindowText("");
            if ( i == 4 )   //need to empty no reply time
            {
                pedCFNoRepTim->SetWindowText("");
            }
        }
    }
  
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (stricmp(CB_ServiceStates[j], (LPCTSTR)csCBStatus[i]) == 0)
            {
                pcbCBs[i]->SetCurSel(j);
                break ;
            }
        }
    }    
    pedCBPasswords[0]->SetWindowText(csPasswords[0]);
    
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (stricmp(LISS_ServiceStates[j], (LPCTSTR)csLISSStatus[i]) == 0)
            {
                pcbLISSs[i]->SetCurSel(j);
                break ;
            }
        }
    }    

    for (j = 0; j < 4; j++)
        {
            if (stricmp(CW_ServiceStates[j], (LPCTSTR)csCWStatus) == 0)
            {
                pcbCW->SetCurSel(j);
                break ;
            }
        }

    
    return bResult;

}

void CSSConPropertyPage::DealCPMsg(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg; 
   
    switch(psig->SignalCode)
    {
    case MN_REGISTER_SS_REQ :   //only for call forward 
        DealMsgReg(pmsg);
        break;        
    case MN_ERASE_SS_REQ ://only for call forward 
        DealMsgUnReg(pmsg);
        break;
    case MN_ACTIVATE_SS_REQ :   //for call forward, call barrier, call wait
        DealMsgActivate(pmsg);
        break;
    case MN_DEACTIVATE_SS_REQ :  //for call forward, call barrier, call wait
        DealMsgDeactivate(pmsg);
        break;
    case MN_INTERROGATE_SS_REQ ://for Call Forward, Call Barrier, LISS, Call Wait
        DealMsgInterogate(pmsg);
        break;
    case MN_MODIFY_PASSWARD_REQ :  //for  call barrier
        DealMsgModifyPwd(pmsg);
        break;
    case MN_PASSWARD_RSP :
        DealMsgPwdRsp(pmsg);
        break;
    case MN_PROC_USSD_REQ :
		DealMsgProcUSSD(pmsg);
		break;
	case MN_USSD_RSP:
		DealMsgUssdRsp(pmsg);
		break;
    default:
        ASSERT(FALSE);
        break;	
    }
    
}

void CSSConPropertyPage::DealMsgReg(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;

    MN_REGISTER_SS_REQ_T *prcvsig = (MN_REGISTER_SS_REQ_T *) psig;
    MN_SS_STATUS_E ssSvrState = MN_SS_ACTIVE_OPERATIVE;
    char szPhoneNumber[128];
    GetStrFromCall(&prcvsig->forward_number, 
                   (unsigned char *) szPhoneNumber, 
                   sizeof(szPhoneNumber)); 

    switch (prcvsig->ss_code)
    {
        //Call Forward
    case MN_SS_ALL_FORWARD:
        SetItemServiceStatus(MN_SS_ALL_FORWARD, ssSvrState);                   
        SetIDCText(IDC_EDIT_CF_PHONUM_1, szPhoneNumber);
        break;
    case MN_SS_CFU:
        SetItemServiceStatus(MN_SS_CFU, ssSvrState);                   
        SetIDCText(IDC_EDIT_CF_PHONUM_2, szPhoneNumber);
        break;
    case MN_SS_ALL_COND_FORWARD:
        SetItemServiceStatus(MN_SS_ALL_COND_FORWARD, ssSvrState);                   
        SetIDCText(IDC_EDIT_CF_PHONUM_3, szPhoneNumber);
        break;
    case MN_SS_CFB:
        SetItemServiceStatus(MN_SS_CFB, ssSvrState);                   
        SetIDCText(IDC_EDIT_CF_PHONUM_4, szPhoneNumber);
        break;
    case MN_SS_CFNRY:
        {
            int nNoReplyTime = prcvsig->no_reply_time;
            CString csNoReplyTime;
            csNoReplyTime.Format("%d", nNoReplyTime);
            SetItemServiceStatus(MN_SS_CFNRY, ssSvrState);                   
            SetIDCText(IDC_EDIT_CF_PHONUM_5, szPhoneNumber);
            SetIDCText(IDC_EDIT_CF_NOREPTIM, csNoReplyTime);               
        }
        break;
    case MN_SS_CFNRC:
        SetItemServiceStatus(MN_SS_CFNRC, ssSvrState);                   
        SetIDCText(IDC_EDIT_CF_PHONUM_6, szPhoneNumber);
        break;
        
        //Call Barrier
    case MN_SS_BOIC:
        SetItemServiceStatus(MN_SS_BOIC, ssSvrState);                   
        break;  
    case MN_SS_BAOC:
        SetItemServiceStatus(MN_SS_BAOC, ssSvrState);                   
        break;
        
    case MN_SS_BOIC_EXHC:
        SetItemServiceStatus(MN_SS_BOIC_EXHC, ssSvrState);                   
        break;
    case MN_SS_BAIC:
        SetItemServiceStatus(MN_SS_BAIC, ssSvrState);                   
        break;
    case MN_SS_BICROAM:
        SetItemServiceStatus(MN_SS_BICROAM, ssSvrState);                   
        break;
  case MN_SS_ALL_BAR_SERVICE:
        SetItemServiceStatus(MN_SS_ALL_BAR_SERVICE, ssSvrState);                   
        break;        
        //Call Wait
    case MN_SS_CW:
        SetItemServiceStatus(MN_SS_CW, ssSvrState);                   
        break;
        
    }//switch (prcvsig->ss_code)

        xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
        InnerSCICreateSignal(&sig_ptr,
            MN_REGISTER_SS_CNF,
            sizeof(MN_REGISTER_SS_CNF_T),
            P_CPANEL);
        ((MN_REGISTER_SS_CNF_T *) sig_ptr)->is_success = TRUE;    
		((MN_REGISTER_SS_CNF_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
		((MN_REGISTER_SS_CNF_T *) sig_ptr)->ss_code = prcvsig->ss_code;
		((MN_REGISTER_SS_CNF_T *) sig_ptr)->ss_status =  ssSvrState;
		((MN_REGISTER_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
        SendCPMsgToPSDummy(sig_ptr);

}

void CSSConPropertyPage::DealMsgUnReg(void *pmsg)
{
   ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;

    MN_ERASE_SS_REQ_T *prcvsig = (MN_ERASE_SS_REQ_T *) psig;
    MN_SS_STATUS_E ssSvrState = MN_SS_NOT_REGISTERED;
    char szPhoneNumber[] = "";
    switch (prcvsig->ss_code)
    {
    case MN_SS_ALL_FORWARD:
        SetItemServiceStatus(MN_SS_ALL_FORWARD, ssSvrState);                   
        SetIDCText(IDC_EDIT_CF_PHONUM_1, szPhoneNumber);
        break;
    case MN_SS_CFU:
        SetItemServiceStatus(MN_SS_CFU, ssSvrState);                    
        SetIDCText(IDC_EDIT_CF_PHONUM_2, szPhoneNumber);
        break;
    case MN_SS_ALL_COND_FORWARD:
        SetItemServiceStatus(MN_SS_ALL_COND_FORWARD, ssSvrState);                    
        SetIDCText(IDC_EDIT_CF_PHONUM_3, szPhoneNumber);
        break;
    case MN_SS_CFB:
        SetItemServiceStatus(MN_SS_CFB, ssSvrState);                    
        SetIDCText(IDC_EDIT_CF_PHONUM_4, szPhoneNumber);
        break;
    case MN_SS_CFNRY:
        {
            SetItemServiceStatus(MN_SS_CFNRY, ssSvrState);                    
            SetIDCText(IDC_EDIT_CF_PHONUM_5, szPhoneNumber);
            SetIDCText(IDC_EDIT_CF_NOREPTIM, "");                           
        }
        break;
    case MN_SS_CFNRC:
        SetItemServiceStatus(MN_SS_CFNRC, ssSvrState);                    
        SetIDCText(IDC_EDIT_CF_PHONUM_6, szPhoneNumber);
        break;
    default:
        ASSERT(FALSE);
        break;
        
    }//switch (prcvsig->ss_code)

        xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
        InnerSCICreateSignal(&sig_ptr,
            MN_ERASE_SS_CNF,
            sizeof(MN_ERASE_SS_CNF_T),
            P_CPANEL);
        ((MN_ERASE_SS_CNF_T *) sig_ptr)->is_success = TRUE;    
		((MN_ERASE_SS_CNF_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
		((MN_ERASE_SS_CNF_T *) sig_ptr)->ss_code = prcvsig->ss_code;
		((MN_ERASE_SS_CNF_T *) sig_ptr)->ss_status =  ssSvrState;
		((MN_ERASE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
		
        SendCPMsgToPSDummy(sig_ptr);

}

void CSSConPropertyPage::DealMsgActivate(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;
    MN_ACTIVATE_SS_REQ_T *prcvsig = (MN_ACTIVATE_SS_REQ_T *) psig;
    MN_SS_STATUS_E ssSvrState = MN_SS_ACTIVE_OPERATIVE;
    if (prcvsig->ss_code == MN_SS_BOIC ||
        prcvsig->ss_code == MN_SS_BAOC ||
        prcvsig->ss_code == MN_SS_BOIC_EXHC ||
        prcvsig->ss_code == MN_SS_BAIC ||
        prcvsig->ss_code == MN_SS_BICROAM ||
        prcvsig->ss_code == MN_SS_ALL_BAR_SERVICE) //password confirm for active call barrier
    {   
        //dump the signal and keep the action.              
        MN_ACTIVATE_SS_REQ_T *pSigDump = new MN_ACTIVATE_SS_REQ_T;
        ASSERT(pSigDump);
        memcpy(pSigDump, prcvsig, sizeof(MN_ACTIVATE_SS_REQ_T));
        
        SetActAfterGetPwd(SS_ACTIVATE, pSigDump);
        
        xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
        InnerSCICreateSignal(&sig_ptr,
            MN_PASSWARD_IND,
            sizeof(MN_PASSWARD_IND_T),
            P_CPANEL);
        ((MN_PASSWARD_IND_T *) sig_ptr)->passward_type = MN_SS_ENTER_PW;  
		((MN_PASSWARD_IND_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
		((MN_PASSWARD_IND_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
        SendCPMsgToPSDummy(sig_ptr);
    }
    else    //activate service
    {
        BOOL bResult;
        bResult = FALSE;
        
        switch (prcvsig->ss_code)
        {
            //Call Forward
        case MN_SS_ALL_FORWARD:
            bResult = SetItemServiceStatus(MN_SS_ALL_FORWARD, ssSvrState);                   
            break;
        case MN_SS_CFU:
            bResult = SetItemServiceStatus(MN_SS_CFU, ssSvrState);                    
            break;
        case MN_SS_ALL_COND_FORWARD:
            bResult = SetItemServiceStatus(MN_SS_ALL_COND_FORWARD, ssSvrState);                    
            break;
        case MN_SS_CFB:
            bResult = SetItemServiceStatus(MN_SS_CFB, ssSvrState);                    
            break;
        case MN_SS_CFNRY:
            bResult = SetItemServiceStatus(MN_SS_CFNRY, ssSvrState);                    
            break;
        case MN_SS_CFNRC:
            bResult = SetItemServiceStatus(MN_SS_CFNRC, ssSvrState);                    
            break;
            
            //Call Barrier
        case MN_SS_BOIC:
            bResult = SetItemServiceStatus(MN_SS_BOIC, ssSvrState);                    
            break;  
        case MN_SS_BAOC:
            bResult = SetItemServiceStatus(MN_SS_BAOC, ssSvrState);                   
            break;
            
        case MN_SS_BOIC_EXHC:
            bResult = SetItemServiceStatus(MN_SS_BOIC_EXHC, ssSvrState);                    
            break;
        case MN_SS_BAIC:
            bResult = SetItemServiceStatus(MN_SS_BAIC, ssSvrState);                    
            break;
        case MN_SS_BICROAM:
            bResult = SetItemServiceStatus(MN_SS_BICROAM, ssSvrState);                    
            break;
        case MN_SS_ALL_BAR_SERVICE:
            bResult = SetItemServiceStatus(MN_SS_ALL_BAR_SERVICE, ssSvrState);                    
            break;            
            //Call Wait
        case MN_SS_CW:
            bResult = SetItemServiceStatus(MN_SS_CW, ssSvrState);                   
            break;
        default:
            ASSERT(FALSE);
            break;
            
        }
        xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
        InnerSCICreateSignal(&sig_ptr,
            MN_ACTIVATE_SS_CNF,
            sizeof(MN_ACTIVATE_SS_CNF_T),
            P_CPANEL);
        ((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->is_success = bResult ? 1 : 0;    
		((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
		((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
        SendCPMsgToPSDummy(sig_ptr);
        
    }//if
    
}

void CSSConPropertyPage::DealMsgDeactivate(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;
    MN_DEACTIVATE_SS_REQ_T *prcvsig = (MN_DEACTIVATE_SS_REQ_T *) psig;
    MN_SS_STATUS_E ssSvrState = MN_SS_NOT_ACTIVE;
    if (prcvsig->ss_code == MN_SS_BOIC ||
        prcvsig->ss_code == MN_SS_BAOC ||
        prcvsig->ss_code == MN_SS_BOIC_EXHC ||
        prcvsig->ss_code == MN_SS_BAIC ||
        prcvsig->ss_code == MN_SS_BICROAM ||
        prcvsig->ss_code == MN_SS_ALL_BAR_SERVICE) //password confirm for active call barrier
    {
        //dump the signal and keep the action
        m_pSigKeptForAct = prcvsig;
        MN_DEACTIVATE_SS_REQ_T *pSigDump = new MN_DEACTIVATE_SS_REQ_T;
        ASSERT(pSigDump);
        memcpy(pSigDump, prcvsig, sizeof(MN_DEACTIVATE_SS_REQ_T));
        
        SetActAfterGetPwd(SS_DEACTIVATE, pSigDump);
        
        xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
        InnerSCICreateSignal(&sig_ptr,
            MN_PASSWARD_IND,
            sizeof(MN_PASSWARD_IND_T),
            P_CPANEL);
        ((MN_PASSWARD_IND_T *) sig_ptr)->passward_type = MN_SS_ENTER_PW;  
		((MN_PASSWARD_IND_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
		((MN_PASSWARD_IND_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
        SendCPMsgToPSDummy(sig_ptr);
    }
    else    //activate service
    {
        BOOL bResult;
        bResult = FALSE;
        
        switch (prcvsig->ss_code)
        {
            //Call Forward
        case MN_SS_ALL_FORWARD:
            bResult = SetItemServiceStatus(MN_SS_ALL_FORWARD, ssSvrState);                   
            break;
        case MN_SS_CFU:
            bResult = SetItemServiceStatus(MN_SS_CFU, ssSvrState);                    
            break;
        case MN_SS_ALL_COND_FORWARD:
            bResult = SetItemServiceStatus(MN_SS_ALL_COND_FORWARD, ssSvrState);                    
            break;
        case MN_SS_CFB:
            bResult = SetItemServiceStatus(MN_SS_CFB, ssSvrState);                    
            break;
        case MN_SS_CFNRY:
            bResult = SetItemServiceStatus(MN_SS_CFNRY, ssSvrState);                    
            break;
        case MN_SS_CFNRC:
            bResult = SetItemServiceStatus(MN_SS_CFNRC, ssSvrState);                    
            break;
            
            //Call Barrier
        case MN_SS_BOIC:
            bResult = SetItemServiceStatus(MN_SS_BOIC, ssSvrState);                    
            break;  
        case MN_SS_BAOC:
            bResult = SetItemServiceStatus(MN_SS_BAOC, ssSvrState);                   
            break;
            
        case MN_SS_BOIC_EXHC:
            bResult = SetItemServiceStatus(MN_SS_BOIC_EXHC, ssSvrState);                    
            break;
        case MN_SS_BAIC:
            bResult = SetItemServiceStatus(MN_SS_BAIC, ssSvrState);                    
            break;
        case MN_SS_BICROAM:
            bResult = SetItemServiceStatus(MN_SS_BICROAM, ssSvrState);                    
            break;
        case MN_SS_ALL_BAR_SERVICE:
            bResult = SetItemServiceStatus(MN_SS_ALL_BAR_SERVICE, ssSvrState);                    
            break;            
            //Call Wait
        case MN_SS_CW:
            bResult = SetItemServiceStatus(MN_SS_CW, ssSvrState);                   
            break;
        default:
            ASSERT(FALSE);
            break;
            
        }
        xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
        InnerSCICreateSignal(&sig_ptr,
            MN_DEACTIVATE_SS_CNF,
            sizeof(MN_DEACTIVATE_SS_CNF_T),
            P_CPANEL);
        ((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->is_success = bResult ? 1 : 0;      
		((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
		((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
        SendCPMsgToPSDummy(sig_ptr);
        
    }//if
    
}

void CSSConPropertyPage::DealMsgInterogate(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;
    MN_INTERROGATE_SS_REQ_T *prcvsig = (MN_INTERROGATE_SS_REQ_T *) psig;
    
    xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
    InnerSCICreateSignal(&sig_ptr,
        MN_INTERROGATE_SS_CNF,
        sizeof(MN_INTERROGATE_SS_CNF_T),
        P_CPANEL);
    ((MN_INTERROGATE_SS_CNF_T *) sig_ptr)->is_success = 1;  
	((MN_INTERROGATE_SS_CNF_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
	((MN_INTERROGATE_SS_CNF_T *) sig_ptr)->ss_code = prcvsig->ss_code;
	
    MN_SS_INTERROGATE_INFO_T *pInfo = &((MN_INTERROGATE_SS_CNF_T *) sig_ptr)->info;
    
    pInfo->ss_status_present = 1;  
    pInfo->forward_num_present = 0;  
    pInfo->forward_subaddr_present = 0;  
    pInfo->no_reply_time_present = 0;  
    pInfo->clir_info_present = 0;  
    
    int nMNSSSvrState;
    CString csPhoneNumber;
    
    
    switch (prcvsig->ss_code)            
    {
        //Call Forward
    case MN_SS_ALL_FORWARD:
        GetItemServiceStatus(MN_SS_ALL_FORWARD, nMNSSSvrState);
        GetIDCText(IDC_EDIT_CF_PHONUM_1, csPhoneNumber);
        pInfo->forward_num_present = 1;                      
        SetStrToMNCall(&pInfo->forward_num, (unsigned char *) csPhoneNumber.GetBuffer(0));
        break;
    case MN_SS_CFU:
        GetItemServiceStatus(MN_SS_CFU, nMNSSSvrState);
        GetIDCText(IDC_EDIT_CF_PHONUM_2, csPhoneNumber);
        pInfo->forward_num_present = 1;                      
        SetStrToMNCall(&pInfo->forward_num, (unsigned char *) csPhoneNumber.GetBuffer(0));
        break;
    case MN_SS_ALL_COND_FORWARD:
        GetItemServiceStatus(MN_SS_ALL_COND_FORWARD, nMNSSSvrState);
        GetIDCText(IDC_EDIT_CF_PHONUM_3, csPhoneNumber);
        pInfo->forward_num_present = 1;                      
        SetStrToMNCall(&pInfo->forward_num, (unsigned char *) csPhoneNumber.GetBuffer(0));
        break;
    case MN_SS_CFB:
        GetItemServiceStatus(MN_SS_CFB, nMNSSSvrState);
        GetIDCText(IDC_EDIT_CF_PHONUM_4, csPhoneNumber);
        pInfo->forward_num_present = 1;                      
        SetStrToMNCall(&pInfo->forward_num, (unsigned char *) csPhoneNumber.GetBuffer(0));
        break;
    case MN_SS_CFNRY:
        {
            GetItemServiceStatus(MN_SS_CFNRY, nMNSSSvrState);
            GetIDCText(IDC_EDIT_CF_PHONUM_5, csPhoneNumber);
            if (nMNSSSvrState == MN_SS_PROVITION || nMNSSSvrState == MN_SS_ACTIVE_OPERATIVE
				|| nMNSSSvrState == MN_SS_ACITVE_QUIESCENT)
			{
				pInfo->forward_num_present = 1;                      
				SetStrToMNCall(&pInfo->forward_num, (unsigned char *) csPhoneNumber.GetBuffer(0));                                        
				pInfo->no_reply_time_present = 1;                      
				pInfo->no_reply_time = GetDlgItemInt(IDC_EDIT_CF_NOREPTIM); 
			}               
        }
        break;
    case MN_SS_CFNRC:
        GetItemServiceStatus(MN_SS_CFNRC, nMNSSSvrState);
        GetIDCText(IDC_EDIT_CF_PHONUM_6, csPhoneNumber);
        pInfo->forward_num_present = 1;                      
        SetStrToMNCall(&pInfo->forward_num, (unsigned char *) csPhoneNumber.GetBuffer(0));
        break;
        
        //Call Barrier
    case MN_SS_BOIC:
        GetItemServiceStatus(MN_SS_BOIC, nMNSSSvrState);
        break;  
    case MN_SS_BAOC:
        GetItemServiceStatus(MN_SS_BAOC, nMNSSSvrState);
        break;
        
    case MN_SS_BOIC_EXHC:
        GetItemServiceStatus(MN_SS_BOIC_EXHC, nMNSSSvrState);
        break;
    case MN_SS_BAIC:
        GetItemServiceStatus(MN_SS_BAIC, nMNSSSvrState);
        break;
    case MN_SS_BICROAM:
        GetItemServiceStatus(MN_SS_BICROAM, nMNSSSvrState);
        break;
    case MN_SS_ALL_BAR_SERVICE:
        GetItemServiceStatus(MN_SS_ALL_BAR_SERVICE, nMNSSSvrState);
        break;
        //LISS
    case MN_SS_CLIP:
        GetItemServiceStatus(MN_SS_CLIP, nMNSSSvrState);
        break;                
    case MN_SS_CLIR:
        GetItemServiceStatus(MN_SS_CLIR, nMNSSSvrState);
        break;
    case MN_SS_COLP:
        GetItemServiceStatus(MN_SS_COLP, nMNSSSvrState);
        break;
    case MN_SS_COLR:
        GetItemServiceStatus(MN_SS_COLR, nMNSSSvrState);
        break;
        
        //Call Wait
    case MN_SS_CW:
        GetItemServiceStatus(MN_SS_CW, nMNSSSvrState);
        break;
    default:
        {
            ASSERT(FALSE);
            xSignalHeaderRec *pSigToFree = (xSignalHeaderRec *) sig_ptr;
            InnerSCIFreeSignal(sig_ptr);
            sig_ptr = NULL;
        }
        break;
    }//switch
    if (sig_ptr)
    {
        pInfo->ss_status = (MN_SS_STATUS_E) nMNSSSvrState;
		((MN_INTERROGATE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
        SendCPMsgToPSDummy(sig_ptr);
    }                
    
}

void CSSConPropertyPage::DealMsgModifyPwd(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;

    MN_MODIFY_PASSWARD_REQ_T *pSigDump = new MN_MODIFY_PASSWARD_REQ_T;
    ASSERT(pSigDump);
    memcpy(pSigDump, psig, sizeof(MN_MODIFY_PASSWARD_REQ_T));
    m_csNewPwd = "";
    SetActAfterGetPwd(SS_MODIFYPWD_CHECK_OLDPWD, pSigDump);
    
    xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
    InnerSCICreateSignal(&sig_ptr,
        MN_PASSWARD_IND,
        sizeof(MN_PASSWARD_IND_T),
        P_CPANEL);
    ((MN_PASSWARD_IND_T *) sig_ptr)->passward_type = MN_SS_ENTER_PW; 
	((MN_PASSWARD_IND_T *) sig_ptr)->invoke_id = pSigDump->invoke_id;
	((MN_PASSWARD_IND_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
    SendCPMsgToPSDummy(sig_ptr);
    
}

void CSSConPropertyPage::DealMsgPwdRsp(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;
    CString csPwd;
    BOOL bResult;
    bResult = FALSE;
    
    char szPwd[MN_SS_NUM_OF_PASSWORD + 1];
    char szRcvedPwd[MN_SS_NUM_OF_PASSWORD + 1];
    UINT nIDCPassword = 0;
    
    strncpy(szRcvedPwd, 
        (const char *)((MN_PASSWARD_RSP_T *) psig)->password, 
        MN_SS_NUM_OF_PASSWORD);               
    szRcvedPwd[MN_SS_NUM_OF_PASSWORD] = 0;  //password may not has zero tail, so add it.    
    
    //get password of ss_code 
    if (m_apActAfterGetPwd == SS_ACTIVATE ||
        m_apActAfterGetPwd == SS_DEACTIVATE || 
        m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_OLDPWD || 
        m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY)    
    {
        ASSERT(m_pSigKeptForAct);
        MN_SS_CODE_E nSSCode;
        if (m_apActAfterGetPwd == SS_ACTIVATE)
        {
            nSSCode = ((MN_ACTIVATE_SS_REQ_T *) m_pSigKeptForAct)->ss_code;
        }
        else if (m_apActAfterGetPwd == SS_DEACTIVATE)
        {
            nSSCode = ((MN_DEACTIVATE_SS_REQ_T *) m_pSigKeptForAct)->ss_code;
        }
        else if(m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_OLDPWD || 
                m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY)
        {
            nSSCode = ((MN_MODIFY_PASSWARD_REQ_T *) m_pSigKeptForAct)->ss_code;
        }
             
        switch (nSSCode)
        {
        case MN_SS_BOIC:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;  
        case MN_SS_BAOC:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;        
        case MN_SS_BOIC_EXHC:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;
        case MN_SS_BAIC:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;
        case MN_SS_BICROAM:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;
        case MN_SS_ALL_BAR_SERVICE:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;
        case MN_SS_BAR_OUTGOING:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;
        case MN_SS_BAR_INCOMING:
            nIDCPassword = IDC_EDIT_CB_PWD_1;
            break;

        default:
            ASSERT(FALSE);
            break;            
        }
        szPwd[0]=0;
        GetDlgItemText(nIDCPassword, szPwd, sizeof(szPwd));
    }
    
    
    if (m_apActAfterGetPwd == SS_ACTIVATE ||
        m_apActAfterGetPwd == SS_DEACTIVATE)
    {       
        if (strcmp(szPwd, szRcvedPwd) == 0) //password is right.
        {
            if (m_apActAfterGetPwd == SS_ACTIVATE)
            {
                
                bResult = SetItemServiceStatus(((MN_ACTIVATE_SS_REQ_T *)m_pSigKeptForAct)->ss_code,
                    MN_SS_ACTIVE_OPERATIVE);

                xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
                InnerSCICreateSignal(&sig_ptr,
                    MN_ACTIVATE_SS_CNF,
                    sizeof(MN_ACTIVATE_SS_CNF_T),
                    P_CPANEL);
                ((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->is_success = bResult ? 1 : 0;  
				((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
                ((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
				SendCPMsgToPSDummy(sig_ptr);               
                SetActAfterGetPwd(SS_UNKNOWN);                
            }
            else
            {
                bResult = SetItemServiceStatus(((MN_DEACTIVATE_SS_REQ_T *)m_pSigKeptForAct)->ss_code,
                    MN_SS_NOT_ACTIVE);
                
                xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
                InnerSCICreateSignal(&sig_ptr,
                    MN_DEACTIVATE_SS_CNF,
                    sizeof(MN_DEACTIVATE_SS_CNF_T),
                    P_CPANEL);
                ((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->is_success = bResult ? 1 : 0;  
				((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
                ((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
				SendCPMsgToPSDummy(sig_ptr);
                SetActAfterGetPwd(SS_UNKNOWN);
                
            }
        }
        else
        {
            if (m_apActAfterGetPwd == SS_ACTIVATE)
            {             
                xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
                InnerSCICreateSignal(&sig_ptr,
                    MN_ACTIVATE_SS_CNF,
                    sizeof(MN_ACTIVATE_SS_CNF_T),
                    P_CPANEL);
                ((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->is_success = 0;   
				((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
                ((MN_ACTIVATE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
				SendCPMsgToPSDummy(sig_ptr);
                SetActAfterGetPwd(SS_UNKNOWN);
            }
            else
            {
                xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
                InnerSCICreateSignal(&sig_ptr,
                    MN_DEACTIVATE_SS_CNF,
                    sizeof(MN_DEACTIVATE_SS_CNF_T),
                    P_CPANEL);
                ((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->is_success = 0;  
				((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
                ((MN_DEACTIVATE_SS_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
				SendCPMsgToPSDummy(sig_ptr);
                SetActAfterGetPwd(SS_UNKNOWN);
            }
        }
    }
    else if(m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_OLDPWD)
    {
        if (strcmp(szRcvedPwd, szPwd) == 0) //password is right.
        {
            xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
            InnerSCICreateSignal(&sig_ptr,
                MN_PASSWARD_IND,
                sizeof(MN_PASSWARD_IND_T),
                P_CPANEL);
            ((MN_PASSWARD_IND_T *) sig_ptr)->passward_type = MN_SS_ENTER_NEW_PW;  
			((MN_PASSWARD_IND_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
            ((MN_PASSWARD_IND_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
			SendCPMsgToPSDummy(sig_ptr);
            SetActAfterGetPwd(SS_MODIFYPWD_CHECK_NEWPWD_FIRSTLY);
        }
        else
        {
            xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
            InnerSCICreateSignal(&sig_ptr,
                MN_MODIFY_PASSWARD_CNF,
                sizeof(MN_MODIFY_PASSWARD_CNF_T),
                P_CPANEL);
            ((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->is_success = 0;    	
			((MN_MODIFY_PASSWARD_CNF_T *)sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
            ((MN_MODIFY_PASSWARD_CNF_T *)sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
			SendCPMsgToPSDummy(sig_ptr);
            SetActAfterGetPwd(SS_UNKNOWN);
        }
    }
    else if(m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_NEWPWD_FIRSTLY)
    {
        m_csNewPwd = szRcvedPwd;
        if (strlen(szRcvedPwd) > 0 ) //good new password.
        {
            xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
            InnerSCICreateSignal(&sig_ptr,
                MN_PASSWARD_IND,
                sizeof(MN_PASSWARD_IND_T),
                P_CPANEL);
            ((MN_PASSWARD_IND_T *) sig_ptr)->passward_type = MN_SS_ENTER_NEW_PW_AGAIN;  
			((MN_PASSWARD_IND_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
            ((MN_PASSWARD_IND_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
			SendCPMsgToPSDummy(sig_ptr);
            
            SetActAfterGetPwd(SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY);
        }
        else
        {
            xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
            InnerSCICreateSignal(&sig_ptr,
                MN_MODIFY_PASSWARD_CNF,
                sizeof(MN_MODIFY_PASSWARD_CNF_T),
                P_CPANEL);
            ((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->is_success = 0;    
			((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
            ((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
			SendCPMsgToPSDummy(sig_ptr);
            SetActAfterGetPwd(SS_UNKNOWN);
        }
        
    }
    else if(m_apActAfterGetPwd == SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY)
    {    
        ASSERT(m_pSigKeptForAct);        
        if (strcmp((LPCTSTR) m_csNewPwd, szRcvedPwd) != 0) //two password not equal.
        {
            bResult = FALSE;
            
            xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
            InnerSCICreateSignal(&sig_ptr,
                MN_MODIFY_PASSWARD_CNF,
                sizeof(MN_MODIFY_PASSWARD_CNF_T),
                P_CPANEL);
            ((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->is_success = 0; 
			((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
            ((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
			SendCPMsgToPSDummy(sig_ptr);
            SetActAfterGetPwd(SS_UNKNOWN);
        }
        else
        {
            //set password
            ASSERT(nIDCPassword != 0);
            bResult = SetIDCText(nIDCPassword, szRcvedPwd);
            
            xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
            InnerSCICreateSignal(&sig_ptr,
                MN_MODIFY_PASSWARD_CNF,
                sizeof(MN_MODIFY_PASSWARD_CNF_T),
                P_CPANEL);
            ((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->is_success = bResult ? 1 : 0;   
			((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->invoke_id = ((MN_PASSWARD_RSP_T *)psig)->invoke_id;
            ((MN_MODIFY_PASSWARD_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
			SendCPMsgToPSDummy(sig_ptr);
            SetActAfterGetPwd(SS_UNKNOWN);
        }
    }
    else
    {
        ASSERT(FALSE);
    }
           
}

void CSSConPropertyPage::DealMsgProcUSSD(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;

    MN_PROC_USSD_REQ_T *prcvsig = (MN_PROC_USSD_REQ_T *) psig;

    xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;

	char ussd_data[] = {"1-sports\n2-life\n3-finance\n4-contact\n5-browser\n6-email"};

	InnerSCICreateSignal(&sig_ptr,
    MN_PROC_USSD_CNF,
    sizeof(MN_PROC_USSD_CNF_T),
    P_CPANEL);

	((MN_PROC_USSD_CNF_T *) sig_ptr)->is_success = TRUE;    
	((MN_PROC_USSD_CNF_T *) sig_ptr)->is_follow_facility = FALSE;    
	((MN_PROC_USSD_CNF_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
	((MN_PROC_USSD_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);

	if((prcvsig->ussd_data.ussd_str[1] == '1') &&
		(prcvsig->ussd_data.ussd_str[2] == '0') &&
		(prcvsig->ussd_data.ussd_str[3] == '1'))
	{
		//if sc = '101',news,cpanel support,echo 1-sports,2-life
		((MN_PROC_USSD_CNF_T *) sig_ptr)->is_follow_facility = TRUE;  
		((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_DEFAULT_DCS_VALUE;
		((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(ussd_data) - 1;
		memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,ussd_data,sizeof(ussd_data) - 1);
	}
	else if((prcvsig->ussd_data.ussd_str[1] == '1') &&
		(prcvsig->ussd_data.ussd_str[2] == '0') &&
		(prcvsig->ussd_data.ussd_str[3] == '2'))
	{
		//if sc = '102',weather forecast,cpanel not support
		((MN_PROC_USSD_CNF_T *) sig_ptr)->is_success = FALSE; 
	}
	else
	{	//default,application ok
		((MN_PROC_USSD_CNF_T *) sig_ptr)->is_success = TRUE; 
		((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data  = prcvsig->ussd_data;
	}
	
	
    SendCPMsgToPSDummy(sig_ptr);
}

void CSSConPropertyPage::DealMsgUssdRsp(void *pmsg)
{
    ASSERT(pmsg);
    
    xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;

    MN_PROC_USSD_REQ_T *prcvsig = (MN_PROC_USSD_REQ_T *) psig;

    xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;

	InnerSCICreateSignal(&sig_ptr,
    MN_PROC_USSD_CNF,
    sizeof(MN_PROC_USSD_CNF_T),
    P_CPANEL);

	((MN_PROC_USSD_CNF_T *) sig_ptr)->is_success = TRUE;    
	((MN_PROC_USSD_CNF_T *) sig_ptr)->is_follow_facility = FALSE;    
	((MN_PROC_USSD_CNF_T *) sig_ptr)->invoke_id = prcvsig->invoke_id;
	((MN_PROC_USSD_CNF_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);

	char sport_string[] ={"basket ball match will start at 9:00 am."};
	char life_string[] = {"bus 42 has changed route."};
	char finance_string[] = {"centry bank adjust rate to 1%."};
	char contact_string[] = {"02150802727"};
	char browser_string[] = {"http://www.spreadtrum.com/"};
	WORD email_string[] = {0x7300, 0x7000, 0x7200, 0x6500, 0x6100, 0x6400, 0x7400, 0x7200, 0x7500, 0x6D00, 0x4000, 0x7300, 0x7000, 0x7200, 0x6500, 0x6100, 0x6400, 0x7400, 0x7200, 0x7500, 0x6D00, 0x2E00, 0x6300, 0x6F00, 0x6D00};
	char unknown_string[] = {"unknown service code"};

	switch(prcvsig->ussd_data.ussd_str[0])
	{
		case '1':
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_DEFAULT_DCS_VALUE;
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(sport_string) - 1;
			memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,sport_string,sizeof(sport_string) -1);
			break;
		case '2':
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_DEFAULT_DCS_VALUE;
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(life_string) -1 ;
			memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,life_string,sizeof(life_string) -1);
			break;
		case '3':
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_DEFAULT_DCS_VALUE;
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(finance_string) - 1;
			memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,finance_string,sizeof(finance_string) - 1);
			break;
		case '4':
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_DEFAULT_DCS_VALUE;
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(contact_string) - 1;
			memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,contact_string,sizeof(contact_string) -1);
			break;
		case '5':
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_DEFAULT_DCS_VALUE;
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(browser_string) -1 ;
			memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,browser_string,sizeof(browser_string) -1);
			break;
		case '6':
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_USC2_NO_CLASS_DCS;
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(email_string);
			memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,email_string,sizeof(email_string));
			break;
		default:
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.data_code_scheme  = MN_SS_DEFAULT_DCS_VALUE;
			((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.str_len = sizeof(unknown_string) - 1;
			memcpy(((MN_PROC_USSD_CNF_T *) sig_ptr)->ussd_data.ussd_str,unknown_string,sizeof(unknown_string) - 1);
			break;

	}
	
    SendCPMsgToPSDummy(sig_ptr);
}
