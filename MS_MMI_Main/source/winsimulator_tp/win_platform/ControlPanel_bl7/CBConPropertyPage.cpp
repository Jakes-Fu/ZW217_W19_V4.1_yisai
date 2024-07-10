// CBPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "ConPanFunc.h"
#include "conpan.h"
#include "CBConPropertyPage.h"
#include "ConPanPropertySheet.h"

#include "ConPanLayer.h"
#include "signals_header.h"
#include "UnicodeConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CB_MESSAGE_NUM      4
#define ID_TIMER_CBCON    131

/////////////////////////////////////////////////////////////////////////////
// CCBConPropertyPage property page

IMPLEMENT_DYNCREATE(CCBConPropertyPage, CPropertyPage)

CCBConPropertyPage::CCBConPropertyPage() : CPropertyPage(CCBConPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CCBConPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCBConPropertyPage::~CCBConPropertyPage()
{
}

void CCBConPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCBConPropertyPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCBConPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCBConPropertyPage)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_RADIO_MSGTYPE_1, OnRadioMsgtype1)
	ON_BN_CLICKED(IDC_RADIO_MSGTYPE_2, OnRadioMsgtype2)
	ON_BN_CLICKED(IDC_RADIO_MSGTYPE_3, OnRadioMsgtype3)
	ON_BN_CLICKED(IDC_RADIO_MSGTYPE_4, OnRadioMsgtype4)
	ON_BN_CLICKED(IDC_CHECK_DISALL, OnCheckDisall)
	ON_BN_CLICKED(IDC_CHECK_ENATHIS, OnCheckEnathis)
	ON_EN_CHANGE(IDC_EDIT_CONTENT, OnChangeEditContent)
	ON_EN_MAXTEXT(IDC_EDIT_CONTENT, OnMaxtextEditContent)
	ON_EN_CHANGE(IDC_EDIT_PERIOD, OnChangeEditPeriod)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBConPropertyPage message handlers

BOOL CCBConPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *pbtTraffic = (CButton *) GetDlgItem(IDC_RADIO_MSGTYPE_1);
    CEdit   *pedContent = (CEdit *) GetDlgItem(IDC_EDIT_CONTENT);

    ASSERT(pbtTraffic);
    ASSERT(pedContent);

    pbtTraffic->SetCheck(1);
    pedContent->SetLimitText(MN_CB_MSG_LEN - 3);
    
    for (int i = 0; i < CB_MESSAGE_NUM; i++)
    {
        CB_MESSAGE_ELEMENT_T cmeElement;
        cmeElement.bEnable = FALSE;
        cmeElement.nMessageID    = 0xffff;
        cmeElement.nMessageCode = 0;
        
        switch (i)
        {
        case 0:
            cmeElement.nMessageID    = 0x03ee;
            cmeElement.csMessageType = "Traffic";
            cmeElement.nPeriod       = 5;
            cmeElement.nMinuteLeft       = cmeElement.nPeriod;
            break;
        case 1:
            cmeElement.nMessageID = 0x1000;
            cmeElement.csMessageType = "Weather";
            cmeElement.nPeriod       = 30;
            cmeElement.nMinuteLeft       = cmeElement.nPeriod;
            break;            
        case 2:
            cmeElement.nMessageID = 0x1080;
            cmeElement.csMessageType = "Business";
            cmeElement.nPeriod       = 20;
            cmeElement.nMinuteLeft       = cmeElement.nPeriod;
            break;
        case 3:
            cmeElement.nMessageID = 0xa000;
            cmeElement.csMessageType = "Advertisement";
            cmeElement.nPeriod       = 10;
            cmeElement.nMinuteLeft       = cmeElement.nPeriod;
            break;        
        default:
            break;
        }
        m_aCBMsg.Add(cmeElement);
    }
    ShowMessageType(0);
    SetTimer(ID_TIMER_CBCON, 60*1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCBConPropertyPage::OnCancelMode() 
{
	CPropertyPage::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CCBConPropertyPage::ShowMessageType(int nIndex)
{
    ASSERT(nIndex >= 0 && nIndex < CB_MESSAGE_NUM);
    if (nIndex >= m_aCBMsg.GetSize())
    {
        return ;
    }

    CString csMessageID;
    csMessageID.Format("%04x", m_aCBMsg.GetAt(nIndex).nMessageID);
    csMessageID.MakeUpper();
    csMessageID = "0x" + csMessageID;
    SetDlgItemText(IDC_EDIT_MSGID, csMessageID);
    SetDlgItemInt(IDC_EDIT_MSGCODE, m_aCBMsg.GetAt(nIndex).nMessageCode);
    SetDlgItemInt(IDC_EDIT_PERIOD, m_aCBMsg.GetAt(nIndex).nPeriod);
    
    // @Liu Jun 2004-02-26 #CR7514
    // SetDlgItemText(IDC_EDIT_CONTENT, m_aCBMsg.GetAt(nIndex).csContent);

    CButton *pbtDisAll = (CButton *) GetDlgItem(IDC_CHECK_DISALL);
    CButton *pbtEnaThis = (CButton *) GetDlgItem(IDC_CHECK_ENATHIS);
    CButton *pbtApply  = (CButton *) GetDlgItem(IDC_BUTTON_APPLY);
    CButton *pbtCancel  = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL);
    
    ASSERT(pbtDisAll);
    ASSERT(pbtEnaThis);
    ASSERT(pbtApply);
    ASSERT(pbtCancel);
        
    if (m_aCBMsg.GetAt(nIndex).bEnable)
    {
        pbtEnaThis->SetCheck(1);
        if (IsDlgButtonChecked(IDC_CHECK_DISALL))
        {
            pbtEnaThis->EnableWindow(TRUE);
            pbtDisAll->SetCheck(0);
        }
    }
    else
    {
        pbtEnaThis->SetCheck(0);
    }
    
    pbtApply->EnableWindow(FALSE);
    pbtCancel->EnableWindow(FALSE);
    
    m_nCurMessageTypeIndex = nIndex;
    
}

void CCBConPropertyPage::OnRadioMsgtype1() 
{
	// TODO: Add your control notification handler code here
    int nMessageTypeIndex = 0;
	if (m_nCurMessageTypeIndex != nMessageTypeIndex)
    {
        ShowMessageType(nMessageTypeIndex);
    }

}

void CCBConPropertyPage::OnRadioMsgtype2() 
{
	// TODO: Add your control notification handler code here
    int nMessageTypeIndex = 1;
	if (m_nCurMessageTypeIndex != nMessageTypeIndex)
    {
        ShowMessageType(nMessageTypeIndex);
    }
	
}

void CCBConPropertyPage::OnRadioMsgtype3() 
{
	// TODO: Add your control notification handler code here
    int nMessageTypeIndex = 2;
	if (m_nCurMessageTypeIndex != nMessageTypeIndex)
    {
        ShowMessageType(nMessageTypeIndex);
    }
	
}

void CCBConPropertyPage::OnRadioMsgtype4() 
{
	// TODO: Add your control notification handler code here
    int nMessageTypeIndex = 3;
	if (m_nCurMessageTypeIndex != nMessageTypeIndex)
    {
        ShowMessageType(nMessageTypeIndex);
    }
	
}

void CCBConPropertyPage::OnCheckDisall() 
{
	// TODO: Add your control notification handler code here
    CButton *pbtEnaThis = (CButton *) GetDlgItem(IDC_CHECK_ENATHIS);
    ASSERT(pbtEnaThis);
    
	if (IsDlgButtonChecked(IDC_CHECK_DISALL))
    {
        pbtEnaThis->EnableWindow(FALSE);
        pbtEnaThis->SetCheck(0);
        CB_MESSAGE_ELEMENT_T cmeElement;
        for (int i = 0; i < m_aCBMsg.GetSize(); i++)
        {
            cmeElement = m_aCBMsg.GetAt(i);
            cmeElement.bEnable = FALSE;
            m_aCBMsg.SetAt(i, cmeElement);            
        }
    }
    else
    {
        pbtEnaThis->EnableWindow(TRUE);
    }
}

void CCBConPropertyPage::OnCheckEnathis() 
{
	// TODO: Add your control notification handler code here
    CB_MESSAGE_ELEMENT_T cmeElement;
    cmeElement = m_aCBMsg.GetAt(m_nCurMessageTypeIndex);
    cmeElement.bEnable = IsDlgButtonChecked(IDC_CHECK_ENATHIS);
    m_aCBMsg.SetAt(m_nCurMessageTypeIndex, cmeElement);
        
}

void CCBConPropertyPage::OnChangeEditContent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CButton *pbtApply = (CButton *) GetDlgItem(IDC_BUTTON_APPLY);
	CButton *pbtCancel = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL);   
 
    ASSERT(pbtApply);
    ASSERT(pbtCancel);

    pbtApply->EnableWindow(TRUE);
    pbtCancel->EnableWindow(TRUE);	

}

void CCBConPropertyPage::OnMaxtextEditContent() 
{
	// TODO: Add your control notification handler code here
    CString csMessage;
    csMessage.Format("Error, Message must be no more than %d characters.", MN_CB_MSG_LEN);
	MessageBox(csMessage, "Error", MB_ICONSTOP );
}

void CCBConPropertyPage::OnChangeEditPeriod() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CButton *pbtApply = (CButton *) GetDlgItem(IDC_BUTTON_APPLY);
	CButton *pbtCancel = (CButton *) GetDlgItem(IDC_BUTTON_CANCEL);   
 
    ASSERT(pbtApply);
    ASSERT(pbtCancel);

    pbtApply->EnableWindow(TRUE);
    pbtCancel->EnableWindow(TRUE);	

}

static BOOL IsChineseString(CString &csString)
{
    BOOL bResult;
    int nLen;
    nLen = csString.GetLength();
    bResult = FALSE;
    for (int i = 0; i < nLen; i++)
    {
        if(csString.GetAt(i) & 0x0080)
        {
            bResult = TRUE;
            break;
        }
    }
    return bResult;

}
void CCBConPropertyPage::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
    if (!IsDlgButtonChecked(IDC_CHECK_ENATHIS))
    {
        //enable this message type.
        CButton *pbtEnaThis = (CButton *) GetDlgItem(IDC_CHECK_ENATHIS);
        CButton *pbtDisAll = (CButton *) GetDlgItem(IDC_CHECK_DISALL);

        ASSERT(pbtEnaThis);
        ASSERT(pbtDisAll);

        pbtDisAll->SetCheck(0);
        pbtEnaThis->EnableWindow(TRUE);
        pbtEnaThis->SetCheck(1);
    }

    CString csContent;
    GetDlgItemText(IDC_EDIT_CONTENT, csContent);

    int nPeriod;
    nPeriod = GetDlgItemInt(IDC_EDIT_PERIOD);
    if (nPeriod <= 0)
    {
        MessageBox("Sorry, Period must no less than 1 minute !",
                    "Error",  
                    MB_ICONSTOP);
        return ;
    }
	
    CB_MESSAGE_ELEMENT_T cmeElement;
    cmeElement = m_aCBMsg.GetAt(m_nCurMessageTypeIndex);
    cmeElement.nMessageCode++;
    cmeElement.bEnable = TRUE;
    cmeElement.nPeriod = nPeriod;
    cmeElement.nMinuteLeft = nPeriod;

    // @Liu Jun 2004-02-26 #CR7514
    // [ 

    cmeElement.dcs.class_is_present    = FALSE;
    cmeElement.dcs.language_id_present = TRUE;

    CUnicodeConvert uniConv;
    int nChineseCharCount = uniConv.GetChineseCharCount( csContent );
    if ( nChineseCharCount == 0 ) // English message
    {
        cmeElement.dcs.language_id   = MN_SMSCB_ENGLISH_ID;
        cmeElement.dcs.alphabet_type = MN_SMS_DEFAULT_ALPHABET;

        if( csContent.GetLength() < MN_CB_MSG_LEN )
            cmeElement.nMessageLen = csContent.GetLength();
        else
            cmeElement.nMessageLen = MN_CB_MSG_LEN;
        
        memcpy( cmeElement.arrMessage, (LPSTR)(LPCTSTR)csContent, cmeElement.nMessageLen );
    }
    else // Chinese message
    {
        cmeElement.dcs.language_id   = MN_SMSCB_CHINESE_ID;
        cmeElement.dcs.alphabet_type = MN_SMS_UCS2_ALPHABET;

        int nUnicodeCount  = csContent.GetLength() - nChineseCharCount;
        int nUnicodeLen    = nUnicodeCount * sizeof( WCHAR );
        WCHAR * pwcUnicode = new WCHAR[ nUnicodeCount ];
        
        uniConv.ConvertToUnicode( pwcUnicode,
                                  nUnicodeLen,
                                  csContent,
                                  csContent.GetLength() );

        if ( nUnicodeLen < MN_CB_MSG_LEN )
            cmeElement.nMessageLen = nUnicodeLen;
        else
            cmeElement.nMessageLen = MN_CB_MSG_LEN;

        char * pcTemp = ( char * )pwcUnicode;
        for ( int i = 0; i < cmeElement.nMessageLen; i += 2 )
        {
            cmeElement.arrMessage[i]   = * ( pcTemp + i + 1 );
            cmeElement.arrMessage[i+1] = * ( pcTemp + i );
        }

        delete [] pwcUnicode;
    }

    // ]
    
    m_aCBMsg.SetAt(m_nCurMessageTypeIndex, cmeElement);
    ShowMessageType(m_nCurMessageTypeIndex);
    
    
    if (!((CConPanPropertySheet *) GetParent())->IsPowerOn(sys_id))
    {
       MessageBox("Message is set, but will not send since power is off.",
                    "Warning",  
                    MB_ICONWARNING);
        return ;
    }

    SendCBMsgToPS(cmeElement);

}

void CCBConPropertyPage::SendCBMsgToPS(CB_MESSAGE_ELEMENT_T &cmeElement)
{
    ASSERT(cmeElement.nMessageID != 0xffff);
    ASSERT(cmeElement.nMessageCode >= 0);
    
    xSignalHeaderRec* sig_ptr;	
    InnerSCICreateSignal(&sig_ptr,
        MN_CB_MSG_IND,
        sizeof(MN_CB_MSG_IND_T),
        P_CPANEL);
    
    // @Liu Jun 2004-02-26 #CR7514
    // [ 

    ((MN_CB_MSG_IND_T *) sig_ptr)->cb_msg_id   = cmeElement.nMessageID;
    ((MN_CB_MSG_IND_T *) sig_ptr)->cb_msg_code = cmeElement.nMessageCode;
    ((MN_CB_MSG_IND_T *) sig_ptr)->dcs         = cmeElement.dcs;
    ((MN_CB_MSG_IND_T *) sig_ptr)->msg_len     = cmeElement.nMessageLen;

    memcpy( ((MN_CB_MSG_IND_T *)sig_ptr)->cb_msg_arr, cmeElement.arrMessage, cmeElement.nMessageLen );

    // ]
    
	((MN_CB_MSG_IND_T *)sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
    SendCPMsgToPSDummy(sig_ptr);
    
    LogMessageSent(cmeElement);
}

void CCBConPropertyPage::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	ShowMessageType(m_nCurMessageTypeIndex);
}


void CCBConPropertyPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if (nIDEvent == ID_TIMER_CBCON)
    {
        CB_MESSAGE_ELEMENT_T cmeElement;
        for (int i = 0; i < m_aCBMsg.GetSize(); i++)
        {
            cmeElement = m_aCBMsg.GetAt(i);
            if (cmeElement.bEnable && cmeElement.nPeriod > 0)
            {
                cmeElement.nMinuteLeft--;
                if (cmeElement.nMinuteLeft <=0 )
                {
                    //send msg.
                    if (((CConPanPropertySheet *) GetParent())->IsPowerOn(sys_id))
                    {
                        SendCBMsgToPS(cmeElement);
                    }
                    cmeElement.nMinuteLeft = cmeElement.nPeriod;
                }

            }
        }
    }
	CPropertyPage::OnTimer(nIDEvent);
}

void CCBConPropertyPage::LogMessageSent(CB_MESSAGE_ELEMENT_T &cmeElement)
{
    CEdit *pedLog = (CEdit *) GetDlgItem(IDC_EDIT_LOG);
    ASSERT(pedLog);
    
    CString csLogRecord;
    CString csTemp;
    CTime tmNow;    
    tmNow = CTime::GetCurrentTime();
    csTemp = "*** ";
    csTemp += tmNow.Format("%Y-%m-%d %H:%M:%S");
    csTemp += " ***\r\n";

    csLogRecord = csTemp;
    csLogRecord += "  send a message\r\n";
    csLogRecord +="    [type=" + cmeElement.csMessageType + ",";
    csTemp.Format("%04x", cmeElement.nMessageID);
    csTemp.MakeUpper();
    csTemp = "MessageID=0x" + csTemp +",";
    csLogRecord += csTemp;
    
    csTemp.Format("MessageCode=%d]\r\n", cmeElement.nMessageCode);
    csLogRecord += csTemp;

    // @Liu Jun 2004-02-26 #CR7514
    // [

/*
    CString csForAlign = "    Content={";
    CString csContent;
    
    for (int i = 0; i < csForAlign.GetLength(); i++)
    {
        csForAlign.SetAt(i, ' ');
    }
    csForAlign.Insert(0, "\r\n");
    csContent = cmeElement.csContent;
    csContent.Replace("\r\n", csForAlign);
    
    csTemp.Format("    Content={%s}\r\n\r\n", csContent);
    csLogRecord += csTemp;
*/

    // ]
    
    int nLen;
    nLen = pedLog->GetWindowTextLength();
    pedLog->SetSel(nLen, nLen);
    pedLog->ReplaceSel((LPCTSTR) csLogRecord);
    
    
}
