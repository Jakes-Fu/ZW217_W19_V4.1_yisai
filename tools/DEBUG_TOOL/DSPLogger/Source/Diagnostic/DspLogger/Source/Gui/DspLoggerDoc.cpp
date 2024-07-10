// DspLoggerDoc.cpp : implementation of the CDspLoggerDoc class
//

#include "stdafx.h"
#include "DspLogger.h"
#include "DspLoggerDoc.h"
#include "MainFrm.h"
#include "FindMainFrame.h"
#include "FilterMainFrame.h"
#include "KeyInfoFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerDoc

IMPLEMENT_DYNCREATE(CDspLoggerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDspLoggerDoc, CDocument)
	//{{AFX_MSG_MAP(CDspLoggerDoc)
	ON_COMMAND(ID_FIND_TP, OnFindTp)
	ON_COMMAND(ID_FILTER_TP, OnFilterTp)
	ON_COMMAND(ID_KEYINFO, OnKeyinfo)
	ON_UPDATE_COMMAND_UI(ID_KEYINFO, OnUpdateKeyinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerDoc construction/destruction

CDspLoggerDoc::CDspLoggerDoc()
{
    m_nFindDataNum = 0;
    
}//lint !e1401

CDspLoggerDoc::~CDspLoggerDoc()
{
}

BOOL CDspLoggerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    return  TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerDoc serialization

void CDspLoggerDoc::Serialize(CArchive& ar)
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
// CDspLoggerDoc diagnostics

#ifdef _DEBUG
void CDspLoggerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDspLoggerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerDoc commands

BOOL CDspLoggerDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
    return CDocument::OnSaveDocument(lpszPathName);
}

void CDspLoggerDoc::OnCloseDocument() 
{
    
	CDocument::OnCloseDocument();
}

BOOL CDspLoggerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;	
    
	return TRUE;
}

void CDspLoggerDoc::OnFindTp() 
{		
	int pos=-1;
	//find a empty space for new frame
	for (int i=0; i<MAX_CHART_WND_NUM; i++)
	{
		if(NULL== g_theApp.m_pFindWnd[i])
		{	
			pos=i;
			break;
		}
	}
	if (!g_theApp.m_bSupportMultiChart && pos >0)
	{
		return;
	}
	//full
	if (-1== pos ) return;

	//create chart frame
	CMultiDocTemplate * pmtmpl = g_theApp.m_pFindTemplate;
	CDocument* pDocument = pmtmpl->CreateNewDocument();
	_ASSERTE( pmtmpl != NULL );
	CFindMainFrame* pFrame = (CFindMainFrame* )pmtmpl->CreateNewFrame(pDocument, NULL);
	_ASSERTE( pFrame != NULL );

	//save it to app obj
	g_theApp.m_pFindWnd[pos] = pFrame;

	//set frame index
	pFrame->m_nFrameIndex=pos;
	//init frame
	pmtmpl->InitialUpdateFrame(pFrame, pDocument);

	//set active input item
	pFrame->SetInputItem();

}


void CDspLoggerDoc::OnFilterTp() 
{
	//create chart frame
	CMultiDocTemplate * pmtmpl = g_theApp.m_pFilterTemplate;
	CDocument* pDocument = pmtmpl->CreateNewDocument();
	_ASSERTE( pmtmpl != NULL );
	CFilterMainFrame* pFrame = (CFilterMainFrame* )pmtmpl->CreateNewFrame(pDocument, NULL);
	_ASSERTE( pFrame != NULL );
	//init frame
	pmtmpl->InitialUpdateFrame(pFrame, pDocument);

}



void CDspLoggerDoc::OnKeyinfo() 
{
	// TODO: Add your command handler code here
	if(!g_theApp.m_bCreateKeyInfoFrame)
	{
		CMultiDocTemplate * pmtmpl = g_theApp.m_pKeyinfoTemplate;
		CDocument* pDocument = pmtmpl->CreateNewDocument();
		_ASSERTE( pmtmpl != NULL );
		CKeyInfoFrame* pFrame = (CKeyInfoFrame* )pmtmpl->CreateNewFrame(pDocument, NULL);
		_ASSERTE( pFrame != NULL );
		//init frame
		pmtmpl->InitialUpdateFrame(pFrame, pDocument);
		g_theApp.m_pKeyInfoDoc = (CKeyInfoDoc *)pDocument;
		g_theApp.m_bCreateKeyInfoFrame = TRUE;
	}
}

void CDspLoggerDoc::OnUpdateKeyinfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
