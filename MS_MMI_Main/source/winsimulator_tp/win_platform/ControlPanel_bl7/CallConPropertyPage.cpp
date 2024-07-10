// CallConPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "CallConPropertyPage.h"
#include "ConPanPropertySheet.h"
#include "SysInfoPropertyPage.h"/*@jassmine MS00004729 2003-9-12*/

#include "ConPanLayer.h"
#include "ConPanFunc.h"
#include "ConPan.h"
#include "signals_header.h"
#include "string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_CALL_NUM            7
#define ID_TIMER_CALL_MOVIE     129
/////////////////////////////////////////////////////////////////////////////
// CCallConPropertyPage property page

IMPLEMENT_DYNCREATE(CCallConPropertyPage, CPropertyPage)

CCallConPropertyPage::CCallConPropertyPage() : CPropertyPage(CCallConPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CCallConPropertyPage)
	//}}AFX_DATA_INIT
    m_pilPhoneImages = NULL;
    m_nCallTi = -1;

}

CCallConPropertyPage::~CCallConPropertyPage()
{
    if (m_pilPhoneImages)
    {
        for (int i=0;i < m_pilPhoneImages->GetImageCount();i++)
        {
            m_pilPhoneImages->Remove(i);
        }
        delete m_pilPhoneImages;
        m_pilPhoneImages = NULL;

    }
    m_aCalls.RemoveAll();

}

void CCallConPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallConPropertyPage)
	DDX_Control(pDX, IDC_CALL_IMAGE, m_wndCallImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallConPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCallConPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_MAKECALL, OnButtonMakecall)
	ON_BN_CLICKED(IDC_BUTTON_ANSWER, OnButtonAnswer)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_CALLNO, OnSelchangeComboCallno)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CALL, OnItemchangedListCall)
	ON_BN_CLICKED(IDC_CHECK_SILCALLED, OnCheckSilcalled)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallConPropertyPage message handlers

void CCallConPropertyPage::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::PostNcDestroy();
	//delete this;
}


BOOL CCallConPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here	    
    SetDlgItemText(IDC_EDIT_SELFNUM,"13012345678");	
    
    m_pilPhoneImages = new CImageList();
    m_pilPhoneImages->Create(36, 36, ILC_COLOR8 | ILC_MASK, 0, 4);
    ASSERT(m_pilPhoneImages);

    CBitmap bmLoad;
    bmLoad.LoadBitmap(IDB_BITMAP_CALLEDPHONE_1);
    m_pilPhoneImages->Add(&bmLoad, RGB(255, 255, 255));
    bmLoad.DeleteObject();
    bmLoad.LoadBitmap(IDB_BITMAP_CALLEDPHONE_2);
    m_pilPhoneImages->Add(&bmLoad, RGB(255, 255, 255));
    bmLoad.DeleteObject();

    InitializeCallList(MAX_CALL_NUM);
    CButton *pbtSlientWhenCalled = (CButton *) GetDlgItem(IDC_CHECK_SILCALLED);
    ASSERT(pbtSlientWhenCalled);
    pbtSlientWhenCalled->SetCheck(0);
    m_bSilentWhenCalled = FALSE;
        
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
void CCallConPropertyPage::DealCPMsg(void *pmsg)
{	
	ASSERT(pmsg);
	xSignalHeaderRec *psig = (xSignalHeaderRec *) pmsg;
	switch(psig->SignalCode)
	{
	case MN_SETUP_REQ:
		{		
            /*@jassmine MS0004729 2003-9-12*/
            uint16 numlen = ((MN_SETUP_REQ_T *) psig)->called_num.num_len;
		    unsigned char *pszCalledNum = new unsigned char[numlen*2+1];
		    int isCalledNum =GetStrFromCall(&((MN_SETUP_REQ_T *)psig)->called_num,
					                    pszCalledNum,
                                        numlen*2+1);

            if((!((CConPanPropertySheet *)GetParent())->IsServiceAvailable(sys_id)) &&
                (strcmp((char *)pszCalledNum,"112") != 0))
            {
                xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			    InnerSCICreateSignal(&sig_ptr,
				    MN_REJ_IND,
				    sizeof(MN_REJ_IND_T),
				    P_CPANEL);
               
                // set the parameters
            	((MN_REJ_IND_T *) sig_ptr)->ti	= ((MN_SETUP_REQ_T *) psig)->ti;
                ((MN_REJ_IND_T *) sig_ptr)->error_cause = MN_CAUSE_EMERGENCY_CALL_ONLY;
				((MN_REJ_IND_T *) sig_ptr)->dual_sys = ((MN_SETUP_REQ_T *)psig)->dual_sys;
	            SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);
            }
            /*@End MS0004729 2003-9-12*/
            else
            {
			    if (isCalledNum == -1)
			    {
				    MessageBox("can not get called num from MN_SETUP_REQ.");
				    *pszCalledNum = 0;
			    }
       
			    SetCallStatus(((MN_SETUP_REQ_T *) psig)->ti, CALL_BEING_CALLED, "", pszCalledNum);	
            }
            delete pszCalledNum;
		}
		break;
	case MN_SETUP_RSP:
		SetCallStatus(((MN_SETUP_RSP_T *) psig)->ti, CALL_COMMUNICATING);
		break;
	case MN_DISC_REQ:
		{
			SetCallStatus(((MN_DISC_REQ_T *) psig)->ti , CALL_IDLE);
			 xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			 InnerSCICreateSignal(&sig_ptr,
				    MN_REL_IND,
				    sizeof(MN_REL_IND_T),
				    P_CPANEL);
			 ((MN_REL_IND_T*)sig_ptr)->ti = ((MN_DISC_REQ_T*)psig)->ti;
			 ((MN_REL_IND_T*)sig_ptr)->dual_sys = ((MN_DISC_REQ_T *)psig)->dual_sys;
			 SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);
		}
		break;
	case MN_HOLD_REQ:
		{
			 xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			 InnerSCICreateSignal(&sig_ptr,
				    MN_HOLD_CNF,
				    sizeof(MN_HOLD_CNF_T),
				    P_CPANEL);
			 ((MN_HOLD_CNF_T*)sig_ptr)->ti = ((MN_HOLD_REQ_T*)psig)->ti;
			 ((MN_HOLD_CNF_T*)sig_ptr)->is_success = TRUE;  //add control mechanism later
			 ((MN_HOLD_CNF_T*)sig_ptr)->dual_sys = ((MN_HOLD_REQ_T*)psig)->dual_sys;
			 SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);
		}
		break;
	case MN_RETRIEVE_REQ:
		{
			 xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			 InnerSCICreateSignal(&sig_ptr,
				    MN_RETRIEVE_CNF,
				    sizeof(MN_RETRIEVE_CNF_T),
				    P_CPANEL);
			 ((MN_RETRIEVE_CNF_T*)sig_ptr)->ti = ((MN_RETRIEVE_REQ_T*)psig)->ti;
			 ((MN_RETRIEVE_CNF_T*)sig_ptr)->is_success = TRUE; //add control mechanism later
			 ((MN_RETRIEVE_CNF_T*)sig_ptr)->dual_sys = ((MN_RETRIEVE_REQ_T*)psig)->dual_sys;
			 SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);
		}
		break;
	case MN_CALL_SS_REQ:
		{
			 xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			 InnerSCICreateSignal(&sig_ptr,
				    MN_CALL_SS_CNF,
				    sizeof(MN_CALL_SS_CNF_T),
				    P_CPANEL);
			 ((MN_CALL_SS_CNF_T*)sig_ptr)->ti = ((MN_CALL_SS_REQ_T*)psig)->ti;
			 ((MN_CALL_SS_CNF_T*)sig_ptr)->is_success = TRUE; //add control mechanism later
			 ((MN_CALL_SS_CNF_T*)sig_ptr)->invoke_id = ((MN_CALL_SS_REQ_T*)psig)->invoke_id;
			 ((MN_CALL_SS_CNF_T*)sig_ptr)->operation_code = ((MN_CALL_SS_REQ_T*)psig)->operation_code;
			 ((MN_CALL_SS_CNF_T*)sig_ptr)->dual_sys = ((MN_CALL_SS_REQ_T*)psig)->dual_sys;
			 SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);
		}
		break;
#ifdef CP_DISP_MN_DTMF
    case MN_START_DTMF_REQ:
        {
			 xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			 InnerSCICreateSignal(&sig_ptr,
				    MN_START_DTMF_CNF,
				    sizeof(MN_START_DTMF_CNF_T),
				    P_CPANEL);
			 ((MN_START_DTMF_CNF_T*)sig_ptr)->ti = ((MN_START_DTMF_REQ_T*)psig)->ti;
			 ((MN_START_DTMF_CNF_T*)sig_ptr)->dual_sys = ((MN_START_DTMF_REQ_T*)psig)->dual_sys;
             ((MN_START_DTMF_CNF_T*)sig_ptr)->dtmf_char = ((MN_START_DTMF_REQ_T*)psig)->dtmf_char;
             ((MN_START_DTMF_CNF_T*)sig_ptr)->is_success = TRUE;

			 SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);
             SetDtmfString(((MN_START_DTMF_REQ_T*)psig)->ti,((MN_START_DTMF_REQ_T*)psig)->dtmf_char);
		}
		break;
#endif 
	default:
		break;
	}	
}


void CCallConPropertyPage::OnButtonMakecall() 
{
	// TODO: Add your control notification handler code here	
    if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(sys_id))
    {
        MessageBox("Sorry. Mobile is power off.",
            "Error",
            MB_ICONSTOP);
        return ;
    }
    
    if (!((CConPanPropertySheet *)GetParent())->IsServiceAvailable(sys_id))
    {
        
        MessageBox("Sorry. No service available.",
            "Error",
            MB_ICONSTOP);
        return ;
    }

	CEdit * pedPhoneNumber = (CEdit *) GetDlgItem(IDC_EDIT_DIALNUM);
	unsigned char strphnum[32];	
	pedPhoneNumber->GetWindowText((char *)strphnum,32);	
	unsigned char strselfno[32];
	GetDlgItemText(IDC_EDIT_SELFNUM,(char *)strselfno,32);
    
    if (strlen((const char *)strselfno) == 0)
    {
        MessageBox("Sorry, self number can not be empty.",
            "Error",
            MB_ICONSTOP);
        return ;
    }
    if (strlen((const char *)strphnum) == 0)
    {
        MessageBox("Sorry, dial number can not be empty.",
            "Error",
            MB_ICONSTOP);
        return ;
    }
    int nMinAvailableCallNo;
    nMinAvailableCallNo = GetMinAvailableCallNo();
    if (nMinAvailableCallNo == -1)
    {
        //actually it will not happen, since this button is disabled.
         MessageBox("Sorry, There is no available call No.",
            "Error",
            MB_ICONSTOP);
        return ;
    }
    else if (nMinAvailableCallNo != m_nIndexSelectedCall)
    {
        CString csMessage;
        csMessage.Format("Attention, The min available call No = %d,", 
                         nMinAvailableCallNo);
        csMessage += "you must use it to make call. would you like to make call ?";
        if (MessageBox(csMessage, 
                        "Attention", 
                        MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1) == IDNO)
        {
            return ;
        }

    }
    
	xSignalHeaderRec* sig_ptr;	
	InnerSCICreateSignal(
					  &sig_ptr,
					  MN_SETUP_IND,
					  sizeof(MN_SETUP_IND_T),
					  P_CPANEL);
	// set the parameters
	((MN_SETUP_IND_T *)sig_ptr)->ti	= nMinAvailableCallNo |0x08;
	SetStrToCalling(&((MN_SETUP_IND_T *)sig_ptr)->call_number,	strselfno);
	// send signal
	((MN_SETUP_IND_T *)sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
	SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);

    SetDlgItemText(IDC_EDIT_REMNUM,(const char *)strphnum);	
	SetCallStatus(nMinAvailableCallNo, CALL_CALLING, strselfno, strphnum);
}

void CCallConPropertyPage::OnButtonAnswer() 
{
	// TODO: Add your control notification handler code here
	unsigned char strselfno[32];
	GetDlgItemText(IDC_EDIT_SELFNUM,(char *)strselfno,32);

    xSignalHeaderRec *sig_ptr;
	InnerSCICreateSignal(
					  &sig_ptr,
					  MN_SETUP_CNF,
					  sizeof(MN_SETUP_CNF_T),
					  P_CPANEL);
	((MN_SETUP_CNF_T *) sig_ptr)->ti	= m_nIndexSelectedCall;
	SetStrToCalling(&((MN_SETUP_CNF_T *)sig_ptr)->conn_addr,
				strselfno);
	((MN_SETUP_CNF_T *)sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
	// send signal
	SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);

	SetCallStatus(m_nIndexSelectedCall, CALL_COMMUNICATING);
}

void CCallConPropertyPage::OnButtonDisconnect() 
{
	
	CALL_ELEMENT_T ceCall = m_aCalls.GetAt(m_nIndexSelectedCall); 
	// TODO: Add your control notification handler code here
	xSignalHeaderRec *sig_ptr;
	InnerSCICreateSignal(
					  &sig_ptr,
					  MN_DISC_IND,
					  sizeof(MN_DISC_IND_T),
					  P_CPANEL);

	// set the parameters
	if (ceCall.bStartFromMobile == TRUE)
	{
		((MN_DISC_IND_T *) sig_ptr)->ti	= m_nIndexSelectedCall;
	}else
	{
		((MN_DISC_IND_T *) sig_ptr)->ti	= m_nIndexSelectedCall | 0x08;
	}
	
	((MN_DISC_IND_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
	SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);

	InnerSCICreateSignal(
					  &sig_ptr,
					  MN_REL_IND,
					  sizeof(MN_REL_IND_T),
					  P_CPANEL);
	if (ceCall.bStartFromMobile == TRUE)
	{
		((MN_REL_IND_T *) sig_ptr)->ti	= m_nIndexSelectedCall;
	}else
	{
		((MN_REL_IND_T *) sig_ptr)->ti	= m_nIndexSelectedCall | 0x08;
	}
	
	((MN_REL_IND_T *) sig_ptr)->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
	SendCPMsgToPSDummy((xSignalHeaderRec *)sig_ptr);
	
    SetDlgItemText(IDC_EDIT_REMNUM, "");
	SetCallStatus(m_nIndexSelectedCall, CALL_IDLE);
}

static BOOL *bParams[2] ={NULL, NULL};

DWORD WINAPI CalledRingThreadFunc(LPVOID lpParameter)
{
    
    BOOL * pbStopCalledRing = bParams[0];
    BOOL * pbSilentWhenCalled = bParams[1];
    ASSERT(pbStopCalledRing);
    ASSERT(pbSilentWhenCalled);

    while (*pbStopCalledRing == FALSE)
    {
        if (*pbSilentWhenCalled == FALSE)
        {
            Beep(1000, 200);
            Beep(3000, 200);
            Beep(2000, 200);            
        }
        Sleep(300);
    }
    return 0;
}



BOOL CCallConPropertyPage::StartCalledRing()
{  
    m_bStopCalledRing = FALSE;
    bParams[0] = &m_bStopCalledRing;
    bParams[1] = &m_bSilentWhenCalled;

    return (BOOL) CreateThread(NULL, 
                               0,
                               CalledRingThreadFunc,
                               (LPVOID) 0,
                               0,
                               0);

}

void CCallConPropertyPage::StopCalledRing()
{
    m_bStopCalledRing = TRUE;
}

BOOL CCallConPropertyPage::StartMoive()
{
    m_bStopMovie = FALSE;
    m_nMoiveCount = 0;

    m_wndCallImage.ShowWindow(SW_SHOW);

    SetTimer(ID_TIMER_CALL_MOVIE, 300, NULL);
    return TRUE;
}

void CCallConPropertyPage::StopMovie()
{
    m_bStopMovie = TRUE;
    KillTimer(ID_TIMER_CALL_MOVIE);
    
    m_wndCallImage.ShowWindow(SW_HIDE);
    m_wndCallImage.Invalidate();        
    UpdateWindow();
    
}

void CCallConPropertyPage::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
    
    if(m_aCalls.GetAt(m_nIndexSelectedCall).csCallCurStatus == CALL_BEING_CALLED)
    {
        
        m_nMoiveCount++;
        m_nMoiveCount %= 2;
        HICON hIcon0, hIcon1;
        hIcon0 = m_pilPhoneImages->ExtractIcon(m_nMoiveCount);
        hIcon1 = m_wndCallImage.SetIcon(hIcon0);
        
        UpdateWindow();
    }
	CPropertyPage::OnTimer(nIDEvent);
}

int nIndex = 0;
CListCtrl *pCallList ;
void CCallConPropertyPage::OnRButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    pCallList = (CListCtrl *) GetDlgItem(IDC_LIST_CALL);
    ASSERT(pCallList);
    for (int i = 0; i < MAX_CALL_NUM; i++)
    {
        if (pCallList->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            //unselect it.
            pCallList->SetItemState(i, 0, LVIS_SELECTED);
        }
    }//for
    
    //select the call.
    pCallList->SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
    pCallList->SetFocus();
    pCallList->Invalidate();

    nIndex++;
    nIndex %= MAX_CALL_NUM;
#if 0
        m_nMoiveCount++;
        m_nMoiveCount %= 2;
      HICON hIcon0, hIcon1;
      hIcon0 = m_pilPhoneImages->ExtractIcon(m_nMoiveCount);
      hIcon1 = m_wndCallImage.SetIcon(hIcon0);

    CPropertyPage::OnRButtonDown(nFlags, point);
#endif
}

void CCallConPropertyPage::SetCallStatus(int nIndex, 
                                         int nCallStatus, 
                                         CString csFromPhoneNum, 
                                         CString csToPhoneNum)
{
    ASSERT(nIndex >= 0);
	if (nIndex > MAX_CALL_NUM)
	{
		nIndex &= 0x07;
	}
    CALL_STATUS_E csCallStatus = (CALL_STATUS_E) nCallStatus;
    ASSERT(csCallStatus >= CALL_IDLE && csCallStatus <= CALL_COMMUNICATING);
    CALL_ELEMENT_T ceCall = m_aCalls.GetAt(nIndex);    
    switch (csCallStatus)
    {
    case CALL_BEING_CALLED:
        ceCall.csRemotePhoneNum = csToPhoneNum;
        ceCall.bStartFromMobile = TRUE;
        break;
    case CALL_CALLING:
        ceCall.csRemotePhoneNum = csToPhoneNum;
        ceCall.bStartFromMobile = FALSE;
        break;
    case CALL_IDLE:
        ceCall.csDtmfString = "\0" ;
        break;
    case CALL_COMMUNICATING:
        break;
    default:
        break;
    }
    ceCall.csCallLastStatus = ceCall.csCallCurStatus;
    ceCall.csCallCurStatus = csCallStatus;
    m_aCalls.SetAt(nIndex, ceCall);

    DisplayCall(nIndex);

}


int CCallConPropertyPage::GetActiveCallNumStartFromConPan()
{
    int nActiveCallNumStartFromConPan = 0;
    for (int i = 0; i < m_aCalls.GetSize(); i++)
    {
        if (m_aCalls.GetAt(i).csCallCurStatus != CALL_IDLE
            && m_aCalls.GetAt(i).bStartFromMobile == FALSE)
        {
            nActiveCallNumStartFromConPan++;
        }
    }
    return nActiveCallNumStartFromConPan;
}

int CCallConPropertyPage::GetActiveCallNum()
{
    int nActiveCallNum = 0;
    for (int i = 0; i < m_aCalls.GetSize(); i++)
    {
        if (m_aCalls.GetAt(i).csCallCurStatus != CALL_IDLE)
        {
            nActiveCallNum++;
        }
    }
    return nActiveCallNum;
}

int CCallConPropertyPage::GetMinAvailableCallNo()
{
    int nMinIdleCallNo = -1;    //no call is idle.
    
    for (int i = 0; i < m_aCalls.GetSize(); i++)
    {
        if (m_aCalls.GetAt(i).csCallCurStatus == CALL_IDLE)
        {
            nMinIdleCallNo = i;
            break;
        }
    }

    return nMinIdleCallNo;  
}

BOOL CCallConPropertyPage::InitializeCallList(int nTotalCallNum)
{
    ASSERT(nTotalCallNum >= 0);
    int i;
    for (i = 0; i < MAX_CALL_NUM; i++)
    {
        CALL_ELEMENT_T ceOneCall;
        ceOneCall.csCallCurStatus = CALL_IDLE;
        ceOneCall.csRemotePhoneNum = "";
        ceOneCall.csDialNum.Format("1301111%04d", i);        
        m_aCalls.Add(ceOneCall);
    }
    
    SetDlgItemInt(IDC_STATIC_TOTCALLNUM, nTotalCallNum);
    CComboBox *pcbCombo = (CComboBox *) GetDlgItem(IDC_COMBO_CALLNO);
    ASSERT(pcbCombo);
    for (i = 0; i < nTotalCallNum; i++)
    {
        CString csText;
        csText.Format("%d", i);
        pcbCombo->InsertString(i, csText);
    }

     

      char *szColumnNames[5] =
      {
              "No.",
              "Status",
              "Start From",
              "Remote Phone Number",
              "Dtmf"
      };
      int nColumnWidths[5] =
      {
          60,
          100,
          100,
          100,
          100
      };
      CListCtrl *pCallList  = (CListCtrl *) GetDlgItem(IDC_LIST_CALL);
      CString csText;
      for (i = 0; i < 5; i++)
      {
          pCallList->InsertColumn(i, 
                        szColumnNames[i], 
                        LVCFMT_LEFT, 
                        nColumnWidths[i],
                        i);
      }
      for (i = 0; i < MAX_CALL_NUM; i++)
      {
          csText.Format(" %d ", i);
          pCallList->InsertItem(LVIF_TEXT | LVIF_STATE, csText); 
                
          pCallList->SetItemText(i, 0, csText);

      }
      pCallList->Invalidate();
      m_nIndexSelectedCall = 0;
      
      ListView_SetExtendedListViewStyle(pCallList->GetSafeHwnd(), LVS_EX_FULLROWSELECT);

      DisplayCall(0);
     
    return TRUE;
 
}

void CCallConPropertyPage::OnSelchangeComboCallno() 
{
	// TODO: Add your control notification handler code here
    CComboBox *pcbCombo = (CComboBox *) GetDlgItem(IDC_COMBO_CALLNO);
    ASSERT(pcbCombo);

    //ShowOneCall(pcbCombo->GetCurSel());
    DisplayCall(pcbCombo->GetCurSel());
}

void CCallConPropertyPage::OnPowerOff()
{
    for (int i = 0; i < m_aCalls.GetSize(); i++)
    {
        if (m_aCalls.GetAt(i).csCallCurStatus != CALL_IDLE)
        {
            SetCallStatus(i, CALL_IDLE);
        }
    }
    

}

void CCallConPropertyPage::DisplayCall(int nIndex)
{
    ASSERT(nIndex >= 0 && nIndex < MAX_CALL_NUM);

    //show all call info 
    
    SetDlgItemInt(IDC_STATIC_ACTCALLNUM, GetActiveCallNum());
    CListCtrl *pCallList = (CListCtrl *) GetDlgItem(IDC_LIST_CALL);
    ASSERT(pCallList);

    int i;
    for (i = 0; i < m_aCalls.GetSize(); i++)
    {
        CALL_ELEMENT_T ceElement = m_aCalls.GetAt(i);
        CString csTextCallStatus;
        CString csStartFrom ;
        CString csRemotePhoneNum;
        CString csDtmfString ;
        
        csTextCallStatus = "";
        csStartFrom ="";
        csRemotePhoneNum = "";
        csDtmfString = "\0";
        switch (ceElement.csCallCurStatus)
        {
        case CALL_BEING_CALLED:
            csTextCallStatus = "Being Called";
            csStartFrom = ceElement.bStartFromMobile ? "Mobile" : "Control Panel";
            csRemotePhoneNum = ceElement.csRemotePhoneNum;
            break;
        case CALL_CALLING:
            csTextCallStatus = "Calling";
            csStartFrom = ceElement.bStartFromMobile ? "Mobile" : "Control Panel";
            csRemotePhoneNum = ceElement.csRemotePhoneNum;
            break;
        case CALL_IDLE:
            csTextCallStatus = "Idle";
            break;
        case CALL_COMMUNICATING:
            csTextCallStatus = "Communicating";
            csStartFrom = ceElement.bStartFromMobile ? "Mobile" : "Control Panel";
            csRemotePhoneNum = ceElement.csRemotePhoneNum;
            csDtmfString = ceElement.csDtmfString;
            break;
            
        default:
            break;
        }
        
        pCallList->SetItemText(i, 1, csTextCallStatus);
        pCallList->SetItemText(i, 2, csStartFrom);
        pCallList->SetItemText(i, 3, csRemotePhoneNum);
        pCallList->SetItemText(i, 4, csDtmfString);
    }//for

    BOOL bItemSelected = FALSE;    
    for (i = 0; i < m_aCalls.GetSize(); i++)
    {
        if (pCallList->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            if (i == nIndex)
            {
                //already selected.
                bItemSelected = TRUE;
                break; 
            }
            else
            {
                //unselect it.
                pCallList->SetItemState(i, 0, LVIS_SELECTED);
            }
        }
        
    }
    if (!bItemSelected)
    {
        
        //select the call.
        pCallList->SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
        pCallList->SetItemState(nIndex, LVIS_FOCUSED, LVIS_FOCUSED);
    }
    
    pCallList->SetFocus();
    pCallList->Invalidate();
    

    //show one call info

    
    if (m_nIndexSelectedCall != nIndex)
    {
        if(m_aCalls.GetAt(m_nIndexSelectedCall).csCallCurStatus == CALL_BEING_CALLED)
        {
            StopMovie();
            StopCalledRing();
        }
    }
    

    m_nIndexSelectedCall = nIndex;
    CComboBox *pcbCombo = (CComboBox *) GetDlgItem(IDC_COMBO_CALLNO);
    ASSERT(pcbCombo);

    pcbCombo->SetCurSel(nIndex);

	CButton * pbtMakeCall   = (CButton *) GetDlgItem(IDC_BUTTON_MAKECALL);
	CButton * pbtAnswerCall = (CButton *) GetDlgItem(IDC_BUTTON_ANSWER);
	CButton * pbtDisCall    = (CButton *) GetDlgItem(IDC_BUTTON_DISCONNECT);
	CStatic * pstCallStatus = (CStatic *) GetDlgItem(IDC_STATIC_CALLSTATUS);
	CEdit * pedPhoneNumber  = (CEdit *) GetDlgItem(IDC_EDIT_DIALNUM);

 

    CALL_STATUS_E csCurStatus = m_aCalls.GetAt(nIndex).csCallCurStatus;
    CALL_STATUS_E csLastStatus = m_aCalls.GetAt(nIndex).csCallLastStatus;
    CString csRemotePhoneNum = m_aCalls.GetAt(nIndex).csRemotePhoneNum;
	CString csDialNum = m_aCalls.GetAt(nIndex).csDialNum;
    switch(csCurStatus)
	{
	case CALL_IDLE:
        if (csLastStatus == CALL_BEING_CALLED)
        {
            StopCalledRing();
            StopMovie();
        }
		pbtMakeCall->EnableWindow(TRUE);
		pbtAnswerCall->EnableWindow(FALSE);
		pbtDisCall->EnableWindow(FALSE);
		pstCallStatus->SetWindowText("Idle");
        SetDlgItemText(IDC_EDIT_DIALNUM, csDialNum);
        SetDlgItemText(IDC_EDIT_REMNUM, "");
		break;
	case CALL_CALLING:
		pbtMakeCall->EnableWindow(FALSE);
		pbtAnswerCall->EnableWindow(FALSE);
		pbtDisCall->EnableWindow(TRUE);
		pstCallStatus->SetWindowText("Calling...");
        SetDlgItemText(IDC_EDIT_REMNUM, csRemotePhoneNum);
		break;
	case CALL_BEING_CALLED:
		pbtMakeCall->EnableWindow(FALSE);
		pbtAnswerCall->EnableWindow(TRUE);
		pbtDisCall->EnableWindow(TRUE);
		pstCallStatus->SetWindowText("Being called...");        
        SetDlgItemText(IDC_EDIT_REMNUM, csRemotePhoneNum);
        StartCalledRing();
        StartMoive();
		break;
	case CALL_COMMUNICATING:
		pbtMakeCall->EnableWindow(FALSE);
		pbtAnswerCall->EnableWindow(FALSE);
		pbtDisCall->EnableWindow(TRUE);
		pstCallStatus->SetWindowText("Communicating...");
        SetDlgItemText(IDC_EDIT_REMNUM, csRemotePhoneNum);
        if (csLastStatus == CALL_BEING_CALLED)
        {
            StopCalledRing();
            StopMovie();
        }
		break;
	default:
		break;
	}

}



void CCallConPropertyPage::OnItemchangedListCall(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

    //only when user select another item, it will work.
    if (pNMListView->uChanged &&
        !(pNMListView->uOldState & LVIS_SELECTED) &&
        (pNMListView->uNewState & LVIS_SELECTED) &&
        (pNMListView->uNewState & LVIS_FOCUSED) && 
        pNMListView->iItem != m_nIndexSelectedCall)
    {
        DisplayCall(pNMListView->iItem);
    }

	*pResult = 0;
}



void CCallConPropertyPage::OnCheckSilcalled() 
{
	// TODO: Add your control notification handler code here
	m_bSilentWhenCalled = IsDlgButtonChecked(IDC_CHECK_SILCALLED);
   
}

void CCallConPropertyPage::SetDtmfString(int nIndex, 
                       char csDtmfString)
{
    char temp_string[255] = {0};
    ASSERT(nIndex >= 0);
	if (nIndex > MAX_CALL_NUM)
	{
		nIndex &= 0x07;
	}
    CALL_ELEMENT_T ceCall = m_aCalls.GetAt(nIndex);    
    
    int len1 = ceCall.csDtmfString.GetLength();
    

    memcpy(temp_string,ceCall.csDtmfString,len1);
    temp_string[len1] = csDtmfString;
    ceCall.csDtmfString = temp_string;
    m_aCalls.SetAt(nIndex, ceCall);

    DisplayCall(nIndex);

}