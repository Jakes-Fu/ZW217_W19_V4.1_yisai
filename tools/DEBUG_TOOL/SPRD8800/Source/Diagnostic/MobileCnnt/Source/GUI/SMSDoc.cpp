// SMSDoc.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SMSDoc.h"
#include "Storage.h"
#include "msgiddef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const _TCHAR CSMSDoc::PRESETSMS_FILENAME[] = _T("PresetSMS.dat");
/////////////////////////////////////////////////////////////////////////////
// CSMSDoc

IMPLEMENT_DYNCREATE(CSMSDoc, CDocument)

CSMSDoc::CSMSDoc()
{

	m_bLoadAll       = FALSE;
	m_bLoadPB        = FALSE;
	m_bSync          = FALSE;
	m_bReadAccessory = TRUE;
	m_dwCateg        = 0x00;
	
	m_pProgThread = NULL;
	m_pCurLoadThread = NULL;
	m_bLoadFinish    = FALSE;

	CStorage stg;
	stg.ReadPresetSMSFromFile( GetPresetSMSFileName() );
}

BOOL CSMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CStorage stg;
	stg.ReadPresetSMSFromFile( GetPresetSMSFileName() );

	return TRUE;
}

CSMSDoc::~CSMSDoc()
{
	m_pProgThread = NULL;
	m_pCurLoadThread = NULL;
}


BEGIN_MESSAGE_MAP(CSMSDoc, CDocument)
	//{{AFX_MSG_MAP(CSMSDoc)
	ON_COMMAND(ID_EDIT_NEW, OnEditNew)
	ON_COMMAND(ID_OPTION_SMS_CENTER, OnOptionSmsCenter)
	ON_COMMAND(ID_OPTION_SMS_PRESET, OnOptionSmsPreset)
	ON_COMMAND(ID_EDIT_EDIT, OnEditEdit)
	ON_COMMAND(ID_OPTION_LOAD_MS_ALL, OnOptionLoadMSAll)
	ON_COMMAND(ID_OPTION_LOAD_SMS, OnOptionLoadSms)
	ON_COMMAND(ID_OPTION_LOAD_PB, OnOptionLoadPb)
	ON_COMMAND(ID_OPTION_LOAD_MEMO, OnOptionLoadMemo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDIT, OnUpdateEditEdit)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_OPTION_LOAD_MS_ALL, OnUpdateOptionLoadMSAll)
	ON_UPDATE_COMMAND_UI(ID_OPTION_LOAD_MEMO, OnUpdateOptionLoadMemo)
	ON_UPDATE_COMMAND_UI(ID_OPTION_LOAD_PB, OnUpdateOptionLoadPb)
	ON_UPDATE_COMMAND_UI(ID_OPTION_LOAD_SMS, OnUpdateOptionLoadSms)
	ON_UPDATE_COMMAND_UI(ID_OPTION_SMS_CENTER, OnUpdateOptionSmsCenter)
	ON_COMMAND(ID_OPTION_LOAD_PBG, OnOptionLoadPBG)
	ON_UPDATE_COMMAND_UI(ID_OPTION_LOAD_PBG, OnUpdateOptionLoadPBG)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NEW, OnUpdateEditNew)
	ON_COMMAND(ID_OPTION_SMS_SEND_MODE, OnOptionSmsSendMode)
	ON_UPDATE_COMMAND_UI(ID_OPTION_SMS_SEND_MODE, OnUpdateOptionSmsSendMode)
	ON_COMMAND(ID_OPTION_SMS_STORE, OnOptionSmsStore)
	ON_UPDATE_COMMAND_UI(ID_OPTION_SMS_STORE, OnUpdateOptionSmsStore)
	ON_COMMAND(ID_OPTION_PB_TYPE, OnOptionPBType)
	ON_UPDATE_COMMAND_UI(ID_OPTION_PB_TYPE, OnUpdateOptionPBType)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_EDIT_NEW_SMS, ID_EDIT_NEW_MEMO, OnEditNew)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSDoc diagnostics

#ifdef _DEBUG
void CSMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSMSDoc serialization

void CSMSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSMSDoc commands
void CSMSDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	CStorage stg;
	stg.SavePresetSMSToFile( GetPresetSMSFileName() ); 
	CDocument::OnCloseDocument();
}

BOOL CSMSDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	CStorage stg;
	return stg.ReadMSFromFile( lpszPathName );
}

BOOL CSMSDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CStorage stg;
//  CInfoListView * pView = (CInfoListView *)GetInfoListView();
//	pView->SetSaveToFileFlag();
//	pView->SendMessage(WM_COMMAND,ID_SYNC_PB,0);
//	pView->SendMessage(WM_COMMAND,ID_SYNC_MEMO,0);
// 	pView->ClearSaveToFileFlag();
	if(stg.SaveMSToFile( lpszPathName ))
	{
		SetModifiedFlag(FALSE);
		return TRUE;
	}
    
	return FALSE;
}

void CSMSDoc::UpdateFrameCounts() 
{
	// TODO: Add your specialized code here and/or call the base class
	//CDocument::UpdateFrameCounts();

	// 去掉显示多余的数字编号
	CFrameWnd * pFrame = (CFrameWnd *)AfxGetMainWnd();
	if( pFrame != NULL )
		pFrame->m_nWindow = 0;
}

CString CSMSDoc::GetPresetSMSFileName()
{
	CString strFileName;
	::GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer(_MAX_PATH);

	int nFind = strFileName.ReverseFind(_T('\\'));
	_ASSERTE( nFind > -1 );

	strFileName = strFileName.Left(nFind + 1);
	strFileName += PRESETSMS_FILENAME;

	return strFileName;
}

BOOL CSMSDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( IsModified() && theApp.m_pSMSFrame != NULL)
	{
		CString strTitle;
		strTitle.LoadString(IDS_SMS_TITLE);
		CString strResult;
		strResult.LoadString(IDS_SAVE_PROMPT);
		theApp.m_pSMSFrame->EnableWindow(FALSE);
		if( MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_YESNO|MB_ICONWARNING) == IDYES )
		{
// 			if(!DoFileSave())
// 			{
// 				theApp.m_pSMSFrame->EnableWindow(TRUE);
// 				return FALSE;
// 			}
			OnFileSave();
			theApp.m_pSMSFrame->EnableWindow(TRUE);
		    SetModifiedFlag(FALSE);
		}
		theApp.m_pSMSFrame->EnableWindow(TRUE);
	}
	return TRUE;//CDocument::SaveModified();
}

void CSMSDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,_T(".msf"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("back up file (*.msf)|*.msf||"),NULL);
	if(dlg.DoModal() == IDOK)
	{
		OnOpenDocument(dlg.GetPathName());
		UpdateAllViews(NULL, WM_CATEGORY_CHANGE, (CObject*)m_dwCateg);
	}
	
}

void CSMSDoc::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	UNREFERENCED_PARAMETER(pCmdUI);

	// TODO: Add your command update UI handler code here
	
}

void CSMSDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,_T(".msf"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		            _T("back up file (*.msf)|*.msf||"),NULL);
	if(dlg.DoModal() == IDOK)
	{
		OnSaveDocument(dlg.GetPathName());
	}
}

void CSMSDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	UNREFERENCED_PARAMETER(pCmdUI);
	// TODO: Add your command update UI handler code here
	
}
