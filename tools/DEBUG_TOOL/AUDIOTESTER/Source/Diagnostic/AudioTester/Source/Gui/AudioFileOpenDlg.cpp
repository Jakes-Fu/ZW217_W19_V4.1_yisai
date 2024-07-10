// AudioFileOpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "AudioFileOpenDlg.h"

#include "Typedef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAudioFileOpenDlg dialog


CAudioFileOpenDlg::CAudioFileOpenDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
                                     DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
    
    
    m_ofn.Flags |= OFN_ENABLETEMPLATE | OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST;
    
    m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DIALOG_AUDIO_FILE_OPEN);
    
}


void CAudioFileOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAudioFileOpenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAudioFileOpenDlg, CFileDialog)
	//{{AFX_MSG_MAP(CAudioFileOpenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioFileOpenDlg message handlers
void CAudioFileOpenDlg::OnFileNameChange()
{
    CFileDialog::OnFileNameChange();
    CString strFileName = CFileDialog::GetFileName();        
    if( strFileName != "")
    {
        try
        {
            CFile fReadFile(strFileName,CFile::modeRead);
            CArchive ofFile( &fReadFile,CArchive::load );   
            CString strFileInfo;
            
            float fFileVersion = 0.0;
            ofFile >> fFileVersion;
            strFileInfo.Format(_T("File Version :%.1f\r\n"),fFileVersion );
            
            CTime creatTime;
            ofFile >> creatTime;
            CString strDate( creatTime.Format("%Y-%m-%d %H:%M:%S") );
            strFileInfo += "Created Time:" + strDate +"\r\n";                        
            CString strValue;
            ofFile >> strValue;
            strFileInfo += "Producer Name:" + strValue +"\r\n";
            
            // Read audio data size 
            int nAudioDataSize = EXTEND_VERSION_SIZE;
            if( fFileVersion > 2.0 )
            {
                // Only 3.0 and later version has this field
                ofFile >> nAudioDataSize;
            }
            strDate.Format( _T("%d"),nAudioDataSize / 2 );
            strFileInfo += "Audio data size:" + strDate + " words\r\n";
            
            GetDlgItem( IDC_STATIC_FILE_INFO )->SetWindowText( strFileInfo );
            
            ofFile.Close();
            fReadFile.Close();
        }
        catch(CFileException* pException )
        {
            ASSERT( pException );
            GetDlgItem( IDC_STATIC_FILE_INFO )->SetWindowText(_T(""));
            if( CFileException::fileNotFound != pException->m_cause )
            {
                throw pException;
            }                                    
        }        
    }
}

void CAudioFileOpenDlg::ChangeFont()
{
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = GetWindow(GW_CHILD);
    CFont* pFatherFont = GetParent()->GetFont();
    SetFont(pFatherFont);
    
    while (pChildWnd)
    {
        pChildWnd->SetFont(pFatherFont);
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }
}

BOOL CAudioFileOpenDlg::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
    ChangeFont();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
