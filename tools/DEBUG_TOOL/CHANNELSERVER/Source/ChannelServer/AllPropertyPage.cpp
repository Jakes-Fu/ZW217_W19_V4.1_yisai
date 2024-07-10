// AllPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "AllPropertyPage.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { CDF_CENTER, CDF_TOPLEFT, CDF_NONE };

static void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag, BOOL bSub)
{
    CRect windowRect;

    // grab old and new text metrics
    TEXTMETRIC tmOld, tmNew;
    CDC * pDC = pWnd->GetDC();
    CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
    pDC->GetTextMetrics(&tmOld);
    pDC->SelectObject(pFont);
    pDC->GetTextMetrics(&tmNew);
    pDC->SelectObject(pSavedFont);
    pWnd->ReleaseDC(pDC);

	if(bSub)
	{	
		CWnd *pPWnd = pWnd->GetParent();
		CDC *pPDC = pPWnd->GetDC();
		pPDC->GetTextMetrics(&tmNew);		
		pPWnd->ReleaseDC(pPDC);		
		
		CRect rc;
		pPWnd->GetWindowRect(&rc);
		CRect newRC;
		newRC.left = newRC.top = 0;
		newRC.right = rc.Width();
		newRC.bottom = rc.Height();
		pWnd->MoveWindow(&newRC);
	}
	
   
    long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
    long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;
    
    if (nFlag != CDF_NONE)
    {
        // calculate new dialog window rectangle
        CRect clientRect, newClientRect, newWindowRect;
        
        pWnd->GetWindowRect(windowRect);
        pWnd->GetClientRect(clientRect);
        long xDiff = windowRect.Width() - clientRect.Width();
        long yDiff = windowRect.Height() - clientRect.Height();
        
        newClientRect.left = newClientRect.top = 0;
        newClientRect.right = XX_MulDiv(clientRect.right,tmNew.tmAveCharWidth,tmOld.tmAveCharWidth); //clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
        newClientRect.bottom = XX_MulDiv(clientRect.bottom,newHeight,oldHeight);//clientRect.bottom * newHeight / oldHeight;
        
        if (nFlag == CDF_TOPLEFT) // resize with origin at top/left of window
        {
            newWindowRect.left = windowRect.left;
            newWindowRect.top = windowRect.top;
            newWindowRect.right = windowRect.left + newClientRect.right + xDiff;
            newWindowRect.bottom = windowRect.top + newClientRect.bottom + yDiff;
        }
        else if (nFlag == CDF_CENTER) // resize with origin at center of window
        {
            newWindowRect.left = windowRect.left - 
                (newClientRect.right - clientRect.right)/2;
            newWindowRect.top = windowRect.top -
                (newClientRect.bottom - clientRect.bottom)/2;
            newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
            newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
        }
        pWnd->MoveWindow(newWindowRect);
    }

	CRect clientRect;
	
    pWnd->GetClientRect(clientRect);
    
    pWnd->SetFont(pFont);
    
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);
    
    while (pChildWnd)
    {
        pChildWnd->SetFont(pFont);
        pChildWnd->GetWindowRect(windowRect);      		
		
        CString strClass;
        ::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
        strClass.MakeUpper();
        if(strClass==_T("COMBOBOX"))
        {
            CRect rect;
            pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
            windowRect.right = rect.right;
            windowRect.bottom = rect.bottom;	
			
			pChildWnd->SetFocus();
        }		
        
        pWnd->ScreenToClient(windowRect);

    	if(bSub)
		{	
			if(windowRect.right > clientRect.right)
			{
				windowRect.right = clientRect.right;
			}
		}
		else
		{
			windowRect.left = XX_MulDiv(windowRect.left,tmNew.tmAveCharWidth,tmOld.tmAveCharWidth);//windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
			windowRect.right = XX_MulDiv(windowRect.right,tmNew.tmAveCharWidth,tmOld.tmAveCharWidth);//windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		}

		windowRect.top = XX_MulDiv(windowRect.top,newHeight,oldHeight);//windowRect.top * newHeight / oldHeight;
		windowRect.bottom = XX_MulDiv(windowRect.bottom,newHeight,oldHeight);//windowRect.bottom * newHeight / oldHeight;
		

        pChildWnd->MoveWindow(windowRect);
        
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CAllPropertyPage

IMPLEMENT_DYNAMIC(CAllPropertyPage, CPropertySheet)

CAllPropertyPage::CAllPropertyPage(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddAllPages();
}

CAllPropertyPage::CAllPropertyPage(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddAllPages();
}

CAllPropertyPage::CAllPropertyPage():CPropertySheet(_T("Channel Server Configure"),NULL,0)
{
	AddAllPages();
}

CAllPropertyPage::~CAllPropertyPage()
{
    if (m_fntPage.m_hObject)
        m_fntPage.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAllPropertyPage, CPropertySheet)
	//{{AFX_MSG_MAP(CAllPropertyPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllPropertyPage message handlers

void CAllPropertyPage::BuildPropPageArray()
{
    CPropertySheet::BuildPropPageArray();
  
    // get first page
    CPropertyPage* pPage = GetPage(0);
    ASSERT (pPage);
	if(pPage==NULL)
	{
		return;
	}    
    // dialog template class in afxpriv.h
    CDialogTemplate dlgtemp;
    // load the dialog template
    VERIFY (dlgtemp.Load(pPage->m_psp.pszTemplate));
    // get the font information
    CString strFace;
    WORD	wSize;
    VERIFY (dlgtemp.GetFont (strFace, wSize));
    if (m_fntPage.m_hObject)
       m_fntPage.DeleteObject();
    // create a font using the info from first page
    VERIFY (m_fntPage.CreatePointFont(wSize*12, strFace));
}

BOOL CAllPropertyPage::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	// get the font for the first active page
	CPropertyPage* pCurActPage = GetActivePage ();
    ASSERT (pCurActPage);

	// change the font for the sheet
    ChangeDialogFont (this, &m_fntPage, CDF_CENTER,FALSE);
	
    // change the font for each page
    for (int iCntr = 0; iCntr < GetPageCount (); iCntr++)
    {
        VERIFY (SetActivePage (iCntr));
        CPropertyPage *pPage = GetActivePage ();
        ASSERT (pPage);	
		ChangeDialogFont (pPage, &m_fntPage, CDF_CENTER,FALSE);
		if(iCntr == 0)
		{
			//ChangeDialogFont(&m_PageCfg.m_dlgUartConfig,&m_fntPage,CDF_NONE,TRUE);
			//ChangeDialogFont(&m_PageCfg.m_dlgFileConfig,&m_fntPage,CDF_NONE,TRUE);
			//ChangeDialogFont(&m_PageCfg.m_dlgSocketConfig,&m_fntPage,CDF_NONE,TRUE);
			m_PageCfg.m_dlgUartConfig.ChangeDialogFont(&m_fntPage);
			m_PageCfg.m_dlgFileConfig.ChangeDialogFont(&m_fntPage);
			m_PageCfg.m_dlgSocketConfig.ChangeDialogFont(&m_fntPage);			
		}
    }

	VERIFY (SetActivePage (pCurActPage));  

	// TODO: Add your specialized code here
	CenterWindow(CWnd::GetDesktopWindow ());

	return bResult;
}

void CAllPropertyPage::AddAllPages()
{
	AddPage(&m_PageCfg);
    AddPage(&m_PageInfo);	
}

int CAllPropertyPage::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// Remove application button
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;

	return CPropertySheet::DoModal();
}
