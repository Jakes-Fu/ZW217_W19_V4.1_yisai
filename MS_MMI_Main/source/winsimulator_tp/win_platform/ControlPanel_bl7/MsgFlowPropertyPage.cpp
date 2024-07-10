// MsgFlowPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "MsgFlowPropertyPage.h"
#include "MsgFlowView.h"

#include "ConPanLayer.h"
#include "signals_header.h"

#include "share.h"
#include "io.h"

#define ID_TIMER_MSGFLOW    130
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgFlowPropertyPage property page

IMPLEMENT_DYNCREATE(CMsgFlowPropertyPage, CPropertyPage)

CMsgFlowPropertyPage::CMsgFlowPropertyPage() : CPropertyPage(CMsgFlowPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CMsgFlowPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pevMsgFlow    = NULL;
	m_csMsgFlowFile = "";
    m_nFileLastLength = -1;
    m_bScrollToEnd = FALSE;
    m_bRefreshNewMsg = FALSE;
}

CMsgFlowPropertyPage::~CMsgFlowPropertyPage()
{
    
    if (m_pevMsgFlow)
    {
        delete m_pevMsgFlow;
        m_pevMsgFlow = NULL;
    }
}

void CMsgFlowPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgFlowPropertyPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgFlowPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMsgFlowPropertyPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, OnButtonOpenFile)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SAVEFILE, OnButtonSavefile)
	ON_BN_CLICKED(IDC_CHECK_SCREND, OnCheckScrend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgFlowPropertyPage message handlers


void CMsgFlowPropertyPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

BOOL CMsgFlowPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	char szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);
    m_csMsgFlowFile = szCurPath;
	m_csMsgFlowFile += "\\trace.log";
	
	// TODO: Add extra initialization here
	CRect rcPage, rcView, rcEdit;
	CEdit *pedEdit = (CEdit *) GetDlgItem(IDC_EDIT_LOGFILE);
	ASSERT(pedEdit);
	GetClientRect(&rcPage);
	pedEdit->GetClientRect(&rcEdit);
	rcView.left     = rcPage.left + 5;
	rcView.right    = rcPage.right - 5;
	rcView.top      = rcPage.top + 5;
	rcView.bottom   = rcPage.bottom - rcEdit.Height() - 50 ;
	
	m_pevMsgFlow = new CMsgFlowView();
	ASSERT(m_pevMsgFlow);

	m_pevMsgFlow->Create(NULL,
						 NULL,
						 WS_CHILD | WS_VISIBLE | WS_BORDER,
						 rcView,
						 this,
						 0,
						 NULL);

    m_pevMsgFlow->SetScrollSizes(MM_TEXT, CSize(rcView.Width(), rcView.Height()));	

	CComboBox *pcbRecsFiles = (CComboBox *) GetDlgItem(IDC_COMBO_RECSFILES);
	ASSERT(pcbRecsFiles);
	pcbRecsFiles->InsertString(0, m_csMsgFlowFile);
	pcbRecsFiles->SetCurSel(0);
    
    SetDlgItemText(IDC_EDIT_LOGFILE, m_csMsgFlowFile);
    CButton *pbtScrollToEnd = (CButton *) GetDlgItem(IDC_CHECK_SCREND);
    pbtScrollToEnd->SetCheck(1);
    m_bScrollToEnd = TRUE;

	SetTimer(ID_TIMER_MSGFLOW, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMsgFlowPropertyPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if (nIDEvent == ID_TIMER_MSGFLOW)
    {
        
        BOOL bUpdateMsgFlowView = FALSE;
        CStdioFile sfMsgFlow;
        CFileException feMsgFile;
        if( !sfMsgFlow.Open((LPCTSTR) m_csMsgFlowFile, 
                            CFile::modeRead | CFile::shareDenyNone,
                            &feMsgFile))
        {
            bUpdateMsgFlowView = TRUE;
        }
        else
        {
            UINT nFileLength = -1;
            TRY
            {
                nFileLength = sfMsgFlow.GetLength();
            }
            CATCH(CException, ex)
            {
            }
            END_CATCH
            if (m_nFileLastLength != nFileLength)
            {
                if (m_nFileLastLength != -1)
                {
                    m_bRefreshNewMsg = TRUE;    //refresh since new message arrived
                }
                m_nFileLastLength = nFileLength;
                bUpdateMsgFlowView = TRUE;
            }
            sfMsgFlow.Close();
        }       
        
        if (bUpdateMsgFlowView)
        {
            m_pevMsgFlow->Invalidate(TRUE);
            m_pevMsgFlow->UpdateWindow();
        }   
        

#ifdef _DEBUG_MODIFY_TRACE_FILE        
        FILE *fp = NULL;
        if ((fp = _fsopen((LPCTSTR) m_csMsgFlowFile, "a+", _SH_DENYNO))!=NULL)
        {
            rewind(fp);
            char szBufLine[512];
            fgets(szBufLine, sizeof(szBufLine), fp);
            fseek(fp, SEEK_END, 0);
            fputs(szBufLine, fp);
	        fclose(fp);
        }
#endif

        return ;
    }
    
   	CPropertyPage::OnTimer(nIDEvent);
}

void CMsgFlowPropertyPage::OnButtonOpenFile() 
{
	// TODO: Add your control notification handler code here
	char BASED_CODE szFilter[] = "Config Files (*.log)|*.log||";
	CFileDialog fdDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilter ,NULL);
	if (fdDlg.DoModal() == IDCANCEL)
	{
		return ;
	}
	CString csFile;
	csFile = fdDlg.GetPathName();
	if (csFile.IsEmpty())
	{
		return ;
	}

	m_csMsgFlowFile = csFile;
    m_nFileLastLength = -1;

	CComboBox *pcbRecsFiles = (CComboBox *) GetDlgItem(IDC_COMBO_RECSFILES);
	ASSERT(pcbRecsFiles);
	int nIndex = pcbRecsFiles->FindString(-1, csFile);
	if (nIndex >= 0)
	{
		pcbRecsFiles->DeleteString(nIndex);			
	}
	pcbRecsFiles->InsertString(0, csFile);

	SetDlgItemText(IDC_EDIT_LOGFILE, csFile);	
	
	m_pevMsgFlow->Invalidate();
    m_pevMsgFlow->UpdateWindow();



}



void CMsgFlowPropertyPage::OnButtonSavefile() 
{
    // TODO: Add your control notification handler code here
    char BASED_CODE szFilter[] = "Config Files (*.log)|*.log||";
    CFileDialog fdDlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter ,NULL);
    if (fdDlg.DoModal() == IDCANCEL)
    {
        return ;
    }
    CString csDestFile;
    csDestFile = fdDlg.GetPathName();
    if (csDestFile.IsEmpty())
    {
        MessageBox("Error. No destination file name !", "Error", MB_ICONWARNING);
        return ;
    }
    if (csDestFile.CompareNoCase(m_csMsgFlowFile) == 0)
    {
        MessageBox("Error. Soure file and destination file is the same!", "Error", MB_ICONWARNING);
        return ;
    }  
    CFileException feMsgFile;

    //open destination file.
    CStdioFile sfDestFile;
    if( !sfDestFile.Open((LPCTSTR) csDestFile, 
                            CFile::modeWrite |
                            CFile::modeCreate| 
                            CFile::shareDenyWrite,
                        &feMsgFile))
    {
        char szMsgError[512];
        feMsgFile.GetErrorMessage(szMsgError, sizeof(szMsgError));
        CString csMsgError ;
        csMsgError = "Error. Can not open destination file: " + csDestFile + " to write.";
        csMsgError += "Error Message Information:";
        csMsgError += szMsgError;
        MessageBox(csMsgError, "Error", MB_ICONSTOP);
        
        return ;
    }
    
    //open source file
    CStdioFile sfSrcFile;
    if( !sfSrcFile.Open((LPCTSTR) m_csMsgFlowFile, 
                        CFile::modeRead | CFile::shareDenyNone,
                        &feMsgFile))
    {
        char szMsgError[512];
        feMsgFile.GetErrorMessage(szMsgError, sizeof(szMsgError));
        CString csMsgError = "";
        csMsgError += "Error. Can not open source file: " + m_csMsgFlowFile + " to read.";
        csMsgError += "Error Message Information:";
        csMsgError += szMsgError;
        MessageBox(csMsgError, "Error", MB_ICONSTOP);
        
        sfDestFile.Close();
        
        return ;
    }
    char szBuf[512];
    int nReadBytes = 0;
    while ((nReadBytes = sfSrcFile.Read(szBuf, sizeof(szBuf))) ==sizeof(szBuf))
    {
        sfDestFile.Write(szBuf, nReadBytes);
    }
    if (nReadBytes > 0)
    {
        sfDestFile.Write(szBuf, nReadBytes);
    }
    sfDestFile.Close();
    sfSrcFile.Close();
    CString csSuccessMsg = "";
    csSuccessMsg = "Succeed to copy from source file: " + 
        m_csMsgFlowFile + 
        " to destination file: " +
        csDestFile + 
        " !";
    MessageBox(csSuccessMsg, "Succeed", MB_OK);
}

void CMsgFlowPropertyPage::OnCheckScrend() 
{
	// TODO: Add your control notification handler code here
	m_bScrollToEnd = (BOOL) IsDlgButtonChecked(IDC_CHECK_SCREND);
}
