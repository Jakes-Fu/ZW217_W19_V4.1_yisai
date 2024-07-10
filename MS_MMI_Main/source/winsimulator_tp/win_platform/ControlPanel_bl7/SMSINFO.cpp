// SMSINFO.cpp : implementation file
//

#include "stdafx.h"
#include "conpan.h"
#include "SMSINFO.h"
#include "SmsConPropertyPage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSINFO dialog


CSMSINFO::CSMSINFO(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSINFO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSINFO)
	m_winSourPort = 0;
	m_winDestPort = 0;
	//}}AFX_DATA_INIT
}


void CSMSINFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSINFO)
	DDX_Control(pDX, IDC_RADIO_CLASS, m_winClassPresent);
	DDX_Control(pDX, IDC_RADIO_MSGWAIT, m_winMsgWaitPresent);
	DDX_Control(pDX, IDC_RADIO_SAVE, m_winSaveMsg);
	DDX_Control(pDX, IDC_RADIO_DISCARD, m_winDiscardMsg);
	DDX_Control(pDX, IDC_RADIO_ACTIVE, m_winActiveInd);
	DDX_Control(pDX, IDC_RADIO_INACTIVE, m_winDactiveInd);
	DDX_Control(pDX, IDC_RADIO_CAT8, m_winCatRef8);
	DDX_Control(pDX, IDC_RADIO_CAT16, m_winCatRef16);
	DDX_Control(pDX, IDC_RADIO_PORT8, m_winPort8);
	DDX_Control(pDX, IDC_RADIO_PORT16, m_winPort16);
	DDX_Text(pDX, IDC_EDIT_SOURPORT, m_winSourPort);
	DDX_Text(pDX, IDC_EDIT_DESTPORT, m_winDestPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSINFO, CDialog)
	//{{AFX_MSG_MAP(CSMSINFO)
	ON_BN_CLICKED(IDC_CHECK_PID, OnCheckPid)
	ON_BN_CLICKED(IDC_CHECK_PORT, OnCheckPort)
	ON_BN_CLICKED(IDC_RADIO_MSGWAIT, OnRadioMsgwait)
	ON_BN_CLICKED(IDC_RADIO_CLASS, OnRadioClass)
	ON_BN_CLICKED(ID_SET, OnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSINFO message handlers


BOOL CSMSINFO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox *psmsClass = (CComboBox *) GetDlgItem(IDC_COMBO_CLASS);
	CComboBox *psmsPID = (CComboBox *) GetDlgItem(IDC_COMBO_PID);
	CComboBox *psmsMsgType = (CComboBox *) GetDlgItem(IDC_COMBO_MSGTYPE);

	m_winClassPresent.SetCheck(1);
	m_winCatRef8.SetCheck(1);
	
	psmsClass->InsertString(-1,"CLASS 0 ");
	psmsClass->InsertString(-1,"CLASS 1 ");
	psmsClass->InsertString(-1,"CLASS 2 ");
	psmsClass->InsertString(-1,"CLASS 3 ");
	psmsClass->InsertString(-1," NO CLASS");
	psmsClass->SetCurSel(4);
	
	psmsMsgType->InsertString(-1,"voice mail message");
	psmsMsgType->InsertString(-1,"fax message");
	psmsMsgType->InsertString(-1,"electronic mail message");
	psmsMsgType->InsertString(-1,"other message");
	psmsMsgType->SetCurSel(0);

	psmsPID->InsertString(-1,"Short Message Type 0");
	psmsPID->InsertString(-1,"Replace Short Message Type 1");
	psmsPID->InsertString(-1,"Replace Short Message Type 2");
	psmsPID->InsertString(-1,"Replace Short Message Type 3");
	psmsPID->InsertString(-1,"Replace Short Message Type 4");
	psmsPID->InsertString(-1,"Replace Short Message Type 5");
	psmsPID->InsertString(-1,"Replace Short Message Type 6");
	psmsPID->InsertString(-1,"Replace Short Message Type 7");
	psmsPID->InsertString(-1,"SIM Data download");
	psmsPID->InsertString(-1,"TELEMATIC_INTW");
	psmsPID->InsertString(-1,"EMAIL_TYPE");
	psmsPID->InsertString(-1,"MN_SMS_PID_VOICE_TYPE");
	
	psmsPID->SetCurSel(0);
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSMSINFO::OnCheckPid() 
{
	CComboBox *psmsPID = (CComboBox *) GetDlgItem(IDC_COMBO_PID);
	
	if(IsDlgButtonChecked(IDC_CHECK_PID))
	{	psmsPID->EnableWindow(TRUE);
		m_formatData.is_pid_exist = TRUE;
	}
	else
	{
		psmsPID->EnableWindow(FALSE);
		m_formatData.is_pid_exist = FALSE;
	}
	return;	
}

void CSMSINFO::OnCheckPort() 
{
	CEdit * pSourPort = (CEdit*) GetDlgItem(IDC_EDIT_SOURPORT);
	CEdit * pDestPort = (CEdit*) GetDlgItem(IDC_EDIT_DESTPORT);
	if (IsDlgButtonChecked(IDC_CHECK_PORT))
	{
		m_winPort8.SetCheck(1);
		m_winPort8.EnableWindow(TRUE);
		m_winPort16.EnableWindow(TRUE);

		pSourPort->EnableWindow(TRUE);
		pDestPort->EnableWindow(TRUE);
	}else
	{
		m_winPort8.EnableWindow(FALSE);
		m_winPort16.EnableWindow(FALSE);

		pSourPort->EnableWindow(FALSE);
		pDestPort->EnableWindow(FALSE);
	}
	
	return;
}

void CSMSINFO::OnRadioMsgwait() 
{
	CComboBox *psmsMsgType = (CComboBox *) GetDlgItem(IDC_COMBO_MSGTYPE);
	CComboBox *psmsClass = (CComboBox *) GetDlgItem(IDC_COMBO_CLASS);
	if (m_winMsgWaitPresent.GetCheck())
	{
		psmsClass->EnableWindow(FALSE);
		m_winSaveMsg.EnableWindow(TRUE);
		m_winDiscardMsg.EnableWindow(TRUE);
		m_winActiveInd.EnableWindow(TRUE);
		m_winDactiveInd.EnableWindow(TRUE);
		psmsMsgType->EnableWindow(TRUE);
	}
	return;
	
}

void CSMSINFO::OnRadioClass() 
{
	CComboBox *psmsMsgType = (CComboBox *) GetDlgItem(IDC_COMBO_MSGTYPE);
	CComboBox *psmsClass = (CComboBox *) GetDlgItem(IDC_COMBO_CLASS);
		
	if (m_winClassPresent.GetCheck())
	{
		psmsClass->EnableWindow(TRUE);
		m_winSaveMsg.EnableWindow(FALSE);
		m_winDiscardMsg.EnableWindow(FALSE);
		m_winActiveInd.EnableWindow(FALSE);
		m_winDactiveInd.EnableWindow(FALSE);
		psmsMsgType->EnableWindow(FALSE);
	}
	return;
}

void CSMSINFO::OnSet() 
{
	UpdateData(TRUE);

	if (IsDlgButtonChecked(IDC_CHECK_PORT))
	{
		if (m_winPort8.GetCheck())
		{
			if (m_winSourPort > 255 || m_winDestPort > 255)
			{
				MessageBox("Port Address must not larger than 255!", "Warning", MB_ICONWARNING);
					return ;
			}
		}else
		{
			if (m_winSourPort > 65535 || m_winDestPort > 65535)
			{
				MessageBox("Port Address must not larger than 65535!", "Warning", MB_ICONWARNING);
					return ;
			}
		}
	}
	

	memset((void*)&m_formatData,0,sizeof(SMS_FORMAT_T));

	CComboBox *psmsClass = (CComboBox *) GetDlgItem(IDC_COMBO_CLASS);
	CComboBox *psmsPID = (CComboBox *) GetDlgItem(IDC_COMBO_PID);
	CComboBox *psmsMsgType = (CComboBox *) GetDlgItem(IDC_COMBO_MSGTYPE);

	//dcs
	if (m_winClassPresent.GetCheck())
	{
		//the last is no class
		if ((psmsClass->GetCount() - 1) == psmsClass->GetCurSel())
		{
			m_formatData.dcs.class_is_present = FALSE;
		}else
		{
			m_formatData.dcs.class_is_present = TRUE;
			m_formatData.dcs.sms_class = (MN_SMS_CLASS_E)psmsClass->GetCurSel();
		}
		
	}else
	{
		m_formatData.dcs.msg_is_waiting = TRUE;
		m_formatData.dcs.is_save_wait_msg = m_winSaveMsg.GetCheck();
		m_formatData.dcs.is_active_wait_ind = m_winActiveInd.GetCheck();
		m_formatData.dcs.wait_type = (MN_SMS_MSG_WAIT_E)psmsMsgType->GetCurSel();
	}

	//user header
	if (m_winCatRef8.GetCheck())
	{
		m_formatData.user_header.ref_type = SMS_CAT_8BIT_REF;
	}else
	{
		m_formatData.user_header.ref_type = SMS_CAT_16BIT_REF;
	}
	if (IsDlgButtonChecked(IDC_CHECK_PORT))
	{
		m_formatData.user_header.is_port_exist = TRUE;
		if (m_winPort8.GetCheck())
		{
			m_formatData.user_header.port_type = SMS_PORT_8BIT;
		}else
		{
			m_formatData.user_header.port_type = SMS_PORT_16BIT;
		}
		
		m_formatData.user_header.sour_port = m_winSourPort;
		m_formatData.user_header.dest_port = m_winDestPort;
	}
	if (IsDlgButtonChecked(IDC_CHECK_PID))
	{
		m_formatData.is_pid_exist = TRUE;
		switch(psmsPID->GetCurSel()) {
		case 0:
			m_formatData.pid = MN_SMS_PID_TYPE_ZERO;
			break;
		case 1:
			m_formatData.pid = MN_SMS_PID_REPLACE_ONE;
			break;
		case 2:
			m_formatData.pid = MN_SMS_PID_REPLACE_TWO;
			break;
		case 3:
			m_formatData.pid = MN_SMS_PID_REPLACE_THR;
			break;
		case 4:
			m_formatData.pid = MN_SMS_PID_REPLACE_FOU;
			break;
		case 5:
			m_formatData.pid = MN_SMS_PID_REPLACE_FIV;
			break;
		case 6:
			m_formatData.pid = MN_SMS_PID_REPLACE_SIX;
			break;
		case 7:
			m_formatData.pid = MN_SMS_PID_REPLACE_SEV;
			break;
		case 8:
			m_formatData.pid = MN_SMS_PID_SIM_DOWNLOAD;
			break;
		case 9:
			m_formatData.pid = MN_SMS_PID_TELEMATIC_INTW;
			break;
		case 10:
			m_formatData.pid = MN_SMS_PID_EMAIL_TYPE;
			break;
		case 11:
			m_formatData.pid = MN_SMS_PID_VOICE_TYPE;
			break;
		default:
			break;
		}
	}else
	{
		m_formatData.is_pid_exist = FALSE;
	}
	
	this->ShowWindow(SW_HIDE);
}
