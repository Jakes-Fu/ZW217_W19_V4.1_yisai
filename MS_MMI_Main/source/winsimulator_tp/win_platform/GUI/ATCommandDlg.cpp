// ATCommandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSDev.h"
#include "ATCommandDlg.h"
#include "MSDevDlg.h"

#include "sio.h"
#include "msk_api_gui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CATCommandDlg dialog


CATCommandDlg::CATCommandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CATCommandDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATCommandDlg)
	m_strResponse = _T("");
	//}}AFX_DATA_INIT
}


void CATCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATCommandDlg)
	DDX_Control(pDX, IDC_AT_RESPONSE, m_wndResponse);
	DDX_Control(pDX, IDC_AT_REQUEST, m_wndRequest);
	DDX_Text(pDX, IDC_AT_RESPONSE, m_strResponse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CATCommandDlg, CDialog)
	//{{AFX_MSG_MAP(CATCommandDlg)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
    ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATCommandDlg message handlers

BOOL CATCommandDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
        {
            if (::GetDlgCtrlID(pMsg->hwnd) == IDC_AT_REQUEST)
            {
                SendMessage(WM_KEYDOWN, pMsg->wParam);
                return TRUE;
            }
        }
        else if ( pMsg->wParam == 'Z')
        {
            int state = GetKeyState(VK_CONTROL);
            state = state & 0x80;
            if (state > 0)
            {
                SendMessage(WM_KEYDOWN, pMsg->wParam);
                return TRUE;
            }
        }
    }         
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CATCommandDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_strResponse = "";
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CATCommandDlg::OnClose()
{
    m_strResponse = "";
    SetDlgItemText(IDC_AT_REQUEST, "");

    DestroyWindow();
}

void CATCommandDlg::OnDestroy() 
{
	CDialog::OnDestroy();
    m_strResponse = "";
    SetDlgItemText(IDC_AT_REQUEST, "");
}

void CATCommandDlg::SendATCommand(CString strCommand) 
{
    // Construct the frame of AT command 
    unsigned int    nCommandLength  = strCommand.GetLength();
    BYTE            *request        = new BYTE[nCommandLength + PROTOCOL_HEADER_SIZE + 2];
    MSG_HEAD_T      MsgHeader       = {0};

    memset(request, FLAG_BYTE, nCommandLength + PROTOCOL_HEADER_SIZE + 2);

    MsgHeader.len       = nCommandLength + sizeof (MSG_HEAD_T);
    MsgHeader.type      = ATC_CMD_R;
    MsgHeader.subtype   = 0;

    memcpy (request+1, &MsgHeader, sizeof (MSG_HEAD_T));
    memcpy (request+1+sizeof (MSG_HEAD_T), strCommand, nCommandLength);

    MSK_SendMsgToRTOS(request, nCommandLength + sizeof (MSG_HEAD_T)+2);
    
    delete[] request;
}

void CATCommandDlg::SendATResponse(CString strATResponse) 
{
    //Set response
    UpdateData(TRUE);
    m_strResponse += strATResponse;
    SetDlgItemText(IDC_AT_RESPONSE, m_strResponse);
    int nLength = m_wndResponse.GetLineCount();
    m_wndResponse.LineScroll(nLength - 1);
}

void CATCommandDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (VK_RETURN == nChar)     // when press ENTER process command 
    {
        CString strCommand;
        GetDlgItemText(IDC_AT_REQUEST, strCommand);
  
        UpdateData(TRUE);
        m_strResponse += ">";
        m_strResponse += strCommand;
        m_strResponse += "\r\n";
        SetDlgItemText(IDC_AT_RESPONSE, m_strResponse);
            
        int nLength = m_wndResponse.GetLineCount();
        m_wndResponse.LineScroll(nLength - 1);
        
        // Append 0x13 at the end;
        strCommand += '\r';

        SendATCommand(strCommand);
        SetDlgItemText(IDC_AT_REQUEST, "");
        m_strResponse = "";
    }
    else if (VK_ESCAPE == nChar)
    {
        CString strCommand;
        GetDlgItemText(IDC_AT_REQUEST, strCommand);
  
        UpdateData(TRUE);
        m_strResponse += ">";
        m_strResponse += strCommand;
        m_strResponse += "\r\n";
        SetDlgItemText(IDC_AT_RESPONSE, m_strResponse);
            
        int nLength = m_wndResponse.GetLineCount();
        m_wndResponse.LineScroll(nLength - 1);
        
        // Append 0x1B   at the end;
        strCommand += '\x1B';

        SendATCommand(strCommand);
        SetDlgItemText(IDC_AT_REQUEST, "");
        m_strResponse = "";
    }
    else if ('Z' == nChar)
    {
        int state = GetKeyState(VK_CONTROL);
        state = state & 0x80;

        if (state > 0)
        {
            int i;
            i = 0;

            CString strCommand;
            GetDlgItemText(IDC_AT_REQUEST, strCommand);
  
            UpdateData(TRUE);
            m_strResponse += ">";
            m_strResponse += strCommand;
            m_strResponse += "\r\n";
            SetDlgItemText(IDC_AT_RESPONSE, m_strResponse);
            
            int nLength = m_wndResponse.GetLineCount();
            m_wndResponse.LineScroll(nLength - 1);

            // Append 0x1A at the end;
            strCommand += '\x1A';

            SendATCommand(strCommand);
            SetDlgItemText(IDC_AT_REQUEST, "");
            m_strResponse = "";
        }
    }

    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
