// MsgFlowView.cpp : implementation file
//

#include "stdafx.h"
#include "share.h"

#include "conpan.h"
#include "MsgFlowView.h"
#include "MsgFlowPropertyPage.h"
#include "sci_api.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMsgFlowView

IMPLEMENT_DYNCREATE(CMsgFlowView, CScrollView)

CMsgFlowView::CMsgFlowView()
{
}

CMsgFlowView::~CMsgFlowView()
{
}


BEGIN_MESSAGE_MAP(CMsgFlowView, CScrollView)
	//{{AFX_MSG_MAP(CMsgFlowView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static int ParseMsgLine(char *pszLine,
                        char *pszSender,
                        int nSenderLen,
                        char *pszReceiver,
                        int nReceiverLen,
                        char *pszMsgType,
                        int nMsgTypeLen)
{
    ASSERT(pszLine);
    ASSERT(pszSender);
    ASSERT(pszReceiver);
    ASSERT(pszMsgType);
    ASSERT(nSenderLen > 0);
    ASSERT(nReceiverLen > 0);
    ASSERT(nMsgTypeLen > 0);
    
    strcpy(pszSender, "");
    strcpy(pszReceiver, "");
    strcpy(pszMsgType, "");
    
    CString csLine = pszLine;

    if (csLine.Find("(") < 0)
    {
        return -1;
    }
    int nStartPos, nLeftPos, nRightPos;
    nStartPos = 0;
    nLeftPos =  csLine.Find("(", nStartPos);
    nRightPos =  csLine.Find(")", nStartPos);
    strncpy(pszSender, (LPCTSTR) csLine.Mid(nLeftPos + 1, nRightPos - nLeftPos - 1), nSenderLen - 1);
    
    nStartPos = nRightPos + 1 ;
    nLeftPos =  csLine.Find("(", nStartPos);
    nRightPos =  csLine.Find(")", nStartPos);
    strncpy(pszReceiver, (LPCTSTR) csLine.Mid(nLeftPos + 1, nRightPos - nLeftPos - 1), nReceiverLen - 1 );
    
    nStartPos = nRightPos + 1 ;
    nLeftPos =  csLine.Find("(", nStartPos);
    nRightPos =  csLine.Find(")", nStartPos);
    strncpy(pszMsgType, (LPCTSTR) csLine.Mid(nLeftPos + 1, nRightPos - nLeftPos - 1), nMsgTypeLen -1 );
    
    return 0;
   
}

//static    int nCurHeight = 0;

/////////////////////////////////////////////////////////////////////////////
// CMsgFlowView drawing

void CMsgFlowView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
	
	CString csLogFile;   
    csLogFile = ((CMsgFlowPropertyPage *)GetParent())->m_csMsgFlowFile;	
    
    CStdioFile sfMsgFlow;
    CFileException feMsgFile;
    if( !sfMsgFlow.Open((LPCTSTR) csLogFile, 
                        CFile::modeRead | CFile::shareDenyNone,
                        &feMsgFile))
    {
        char szMsgError[1024];
        CString csMessageError;
        feMsgFile.GetErrorMessage(szMsgError, sizeof(szMsgError));
        
        csMessageError = " Log file: "+csLogFile+" .";
        csMessageError += " Error. Failed to open file.";
        csMessageError += szMsgError;

        pDC->TextOut(10,0," Log file: "+csLogFile+" .");
        pDC->TextOut(10,20," Error. Failed to open file. ");
        pDC->TextOut(10,40,szMsgError);
        
        CRect rcSelf;
        GetClientRect(&rcSelf);

        //pDC->DrawText(csMessageError, &rcSelf, DT_LEFT);
        SetScrollSizes(MM_TEXT, CSize(rcSelf.Width() * 2 , rcSelf.Height()));
        return ;
        
    }
    
    char LineBuf[512];
    char szMsg[512];
    char szSenderName[256], szReceiverName[256], szMsgTypeName[256];
    int nCurHeight;
    int nArrowHeight;
    CPoint ptArray[2], ptArrow[3];
    int nStepHeight;
    nStepHeight = 30;
    nCurHeight = nStepHeight;
    
    COLORREF crOldColor;
    
    CRect rcView;
    GetClientRect(&rcView);
    int nxLeftLine  = 20 ;
    int nxRightLine = rcView.Width() - 20 ;  
    int nxLeftTip   = nxLeftLine -10 > 0 ?  nxLeftLine -10 : 0;
    int nxRightTip  = nxRightLine -10 > 0 ?  nxRightLine -10 : 0;
    
    pDC->MoveTo(0,0);    
    //crOldColor  = pDC->SetTextColor(RGB(255, 0, 0));
    COLORREF COLOR_BLACK = RGB(0, 0, 0);
    COLORREF COLOR_PINK = RGB(255, 0, 128);
    COLORREF COLOR_GREEN = RGB(0, 255, 0);
    COLORREF COLOR_RED = RGB(255, 0, 0);
    COLORREF COLOR_BLUE = RGB(0, 0, 255);

    crOldColor  = pDC->SetTextColor(COLOR_PINK);
    pDC->TextOut(nxLeftTip, 0, "PS");
    pDC->TextOut(nxRightTip, 0, "MMI");
    pDC->SetTextColor(crOldColor);
    
    int i = 0;
    int nCurLine = 0;
    
    while (sfMsgFlow.ReadString(LineBuf, sizeof(LineBuf)))
    {
        nCurLine++;
        if (strlen(LineBuf) > 0)
        {
            
            ParseMsgLine(LineBuf,
                        szSenderName,
                        sizeof(szSenderName),
                        szReceiverName,
                        sizeof(szReceiverName),
                        szMsgTypeName,
                        sizeof(szMsgTypeName));
            sprintf(szMsg, "%d.  %s", i+1, szMsgTypeName);
            
            if( stricmp(szSenderName, "PS") == 0 && 
                stricmp(szReceiverName, "MMI") == 0)
            {
                //draw arrow head
                nArrowHeight = nCurHeight + nStepHeight / 2 ;
                ptArrow[0].x = nxRightLine - 5 ;
                ptArrow[0].y = nArrowHeight - 5;
                ptArrow[1].x = nxRightLine;
                ptArrow[1].y = nArrowHeight;
                ptArrow[2].x = nxRightLine - 5 ;
                ptArrow[2].y = nArrowHeight + 5;
                pDC->Polyline(ptArrow, 3);
                crOldColor = pDC->SetTextColor(COLOR_BLUE);
                pDC->TextOut(nxLeftLine + 20 , nArrowHeight - nStepHeight / 2 -3, szMsg);
                pDC->SetTextColor(crOldColor);
            }
            else if( stricmp(szSenderName, "MMI") == 0 && 
                stricmp(szReceiverName, "PS") == 0)
            {
                //draw arrow head
                nArrowHeight = nCurHeight + nStepHeight / 2 ;
                ptArrow[0].x = nxLeftLine + 5 ;
                ptArrow[0].y = nArrowHeight - 5;
                ptArrow[1].x = nxLeftLine;
                ptArrow[1].y = nArrowHeight;
                ptArrow[2].x = nxLeftLine + 5 ;
                ptArrow[2].y = nArrowHeight + 5;
                pDC->Polyline(ptArrow, 3);
                crOldColor = pDC->SetTextColor(COLOR_RED);
                pDC->TextOut(nxLeftLine + 20, nArrowHeight - nStepHeight / 2 - 3, szMsg);
                pDC->SetTextColor(crOldColor);
            }
            else
            {
                continue ;
            }
            
            //draw vertical lines.
            ptArray[0].x = nxLeftLine;
            ptArray[0].y = nCurHeight;
            ptArray[1].x = nxLeftLine;
            ptArray[1].y = nCurHeight + nStepHeight;
            pDC->Polyline(ptArray, 2);
            
            ptArray[0].x = nxRightLine;
            ptArray[0].y = nCurHeight;
            ptArray[1].x = nxRightLine;
            ptArray[1].y = nCurHeight + nStepHeight;
            pDC->Polyline(ptArray, 2);
            
            //draw horizontal line.
            ptArray[0].x = nxLeftLine;
            ptArray[0].y = nArrowHeight;
            ptArray[1].x = nxRightLine;
            ptArray[1].y = nArrowHeight;
            pDC->Polyline(ptArray, 2);
            
            nCurHeight += nStepHeight;
            i++;
        }
    }
    sfMsgFlow.Close();    
    SetScrollSizes(MM_TEXT, CSize(rcView.Width(), nCurHeight + 100 ));
    if (((CMsgFlowPropertyPage *)GetParent())->m_bRefreshNewMsg &&
        ((CMsgFlowPropertyPage *)GetParent())->m_bScrollToEnd)
    {
        POINT ptEndPoint;
        ptEndPoint.x = 0;
        ptEndPoint.y = nCurHeight;
        ScrollToPosition(ptEndPoint);
        ((CMsgFlowPropertyPage *)GetParent())->m_bRefreshNewMsg = FALSE;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMsgFlowView diagnostics

#ifdef _DEBUG
void CMsgFlowView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMsgFlowView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgFlowView message handlers

void CMsgFlowView::OnInitialUpdate() 
{
    
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
    
}
